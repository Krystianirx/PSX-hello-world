/*
 * $PSLibId: Runtime Library Release 3.6$
 */
/*
 * tmdview4: GsDOBJ2 object viewing rotine(multi OT) 
 * 
 * "tuto0.c" ******** simple GsDOBJ5 Viewing routine 
 * 
 * Version 1.00	Jul,  14, 1994 
 * 
 * Copyright (C) 1993 by Sony Computer Entertainment All rights Reserved 
 */

#include <sys/types.h>

#include <libetc.h>		/* for Control Pad :
				   PAD���g�����߂ɃC���N���[�h����K�v���� */
#include <libgte.h>		/* LIBGS uses libgte :
				   LIGS���g�����߂ɃC���N���[�h����K�v���� */
#include <libgpu.h>		/* LIBGS uses libgpu :
				   LIGS���g�����߂ɃC���N���[�h����K�v���� */
#include <libgs.h>		/* for LIBGS : �O���t�B�b�N���C�u���� ���g�����߂�
				   �\���̂Ȃǂ���`����Ă��� */

#define OBJECTMAX 100		/* Max Objects :
				   �RD�̃��f���͘_���I�ȃI�u�W�F�N�g�� �������邱�̍ő吔
				   ���`���� */

#define TEX_ADDR   0x80010000	/* Top Address of texture data1 (TIM FORMAT)
				   : �e�N�X�`���f�[�^�iTIM�t�H�[�}�b�g�j���������A�h���X */

#define TEX_ADDR1   0x80020000	/* Top Address of texture data1 (TIM FORMAT) */
#define TEX_ADDR2   0x80030000	/* Top Address of texture data2 (TIM FORMAT) */


#define MODEL_ADDR 0x80040000	/* Top Address of modeling data (TMD FORMAT)
				   : ���f�����O�f�[�^�iTMD�t�H�[�}�b�g�j���������A�h���X */

#define OT_LENGTH  4		/* bit length of OT :
				   �I�[�_�����O�e�[�u���̉𑜓x */


GsOT    Wot1[2];		/* OT handler for double buffers :
				   �I�[�_�����O�e�[�u���n���h���_�u���o�b�t�@�̂��߂Q�K�v */

GsOT    Wot2[2];		/* OT handler for double buffers :
				   �I�[�_�����O�e�[�u���n���h���_�u���o�b�t�@�̂��߂Q�K�v */

GsOT_TAG zsorttable1[2][1 << OT_LENGTH];	/* substance of OT :
						   �I�[�_�����O�e�[�u������ */
GsOT_TAG zsorttable2[2][1 << OT_LENGTH];	/* substance of OT :
						   �I�[�_�����O�e�[�u������ */

GsDOBJ2 object[OBJECTMAX];	/* Array of Object Handler :
				   �I�u�W�F�N�g�n���h���I�u�W�F�N�g�̐������K�v */

u_long  Objnum;			/* valibable of number of Objects :
				   ���f�����O�f�[�^�̃I�u�W�F�N�g�̐��� �ێ����� */


GsCOORDINATE2 DWorld, DLocal1, DLocal2;	/* coordinates for objects :
					   �I�u�W�F�N�g���Ƃ̍��W�n */

/* work short vector for making Coordinate parameter :
   ���W�n����邽�߂̃��[�e�[�V�����x�N�^�[ */
SVECTOR PWorld, PLocal1, PLocal2;


GsRVIEW2 view;			/* View Point Handler : ���_��ݒ肷�邽�߂̍\���� */
GsF_LIGHT pslt[3];		/* Flat Lighting Handler : ���s������ݒ肷�邽�߂̍\���� */
u_long  padd;			/* Controler data : �R���g���[���̃f�[�^��ێ����� */

u_long  out_packet[2][0x14000]; /* GPU PACKETS AREA */

/************* MAIN START ******************************************/
main()
{
	int     i;
	GsDOBJ2 *op;		/* the pointer of Object handler :
				   �I�u�W�F�N�g�n���h���ւ̃|�C���^ */
	GsOT_TAG *p;
	int     outbuf_idx;
	MATRIX  tmpls;

	ResetCallback();
	init_all();

	while (1) {
		if (obj_interactive() == 0)
			return 0;	/* interactive parameter get :
					   �p�b�h�f�[�^���瓮���̃p�����[�^������ */
		GsSetRefView2(&view);	/* caliculate World/Screen matrix :
					   ���[���h�X�N���[���}�g���b�N�X�v�Z */
		outbuf_idx = GsGetActiveBuff();	/* get the double buffer
						   index : �_�u���o�b�t�@�̂ǂ��炩�𓾂� */

		/* auto divide packet work setting : ���������p�p�P�b�g���[�N�ݒ� */
		GsSetWorkBase((PACKET *) out_packet[outbuf_idx]);
		GsClearOt(0, 0, &Wot1[outbuf_idx]);	/* Clear OT1 : OT1���N���A���� */
		/* Clear OT2, object2 always exists in front of object1
		   because OT2's point value is 0 : OT2���N���A���� point��0
		   ����ăI�u�W�F�N�g�Q�́A�K���I�u�W�F�N�g�P �̎�O�ɂȂ� */
		GsClearOt(0, 12, &Wot2[outbuf_idx]);

		/* Clear OT2, object2 always lays behaind object1 because
		   OT2's point value is set to 1<<OT_LENGTH-1 : OT2���N���A����
		   point��1<<OT_LENGTH-1 ����ăI�u�W�F�N�g�Q��
		   �K���I�u�W�F�N�g�P�̌��ɂȂ�
		   GsClearOt(0,1<<OT_LENGTH-1,&Wot2[outbuf_idx]); */

		for (i = 0, op = object; i < Objnum; i++) {	/* object1 caliculation
								   : �I�u�W�F�N�g�P�v�Z */
			/* Calculate Local-World Matrix :
			   ���[���h�^���[�J���}�g���b�N�X���v�Z���� */
			GsGetLw(op->coord2, &tmpls);
			/* Set LWMATRIX to GTE Lighting Registers :
			   ���C�g�}�g���b�N�X��GTE�ɃZ�b�g���� */
			GsSetLightMatrix(&tmpls);
			/* Calculate Local-Screen Matrix :
			   �X�N���[���^���[�J���}�g���b�N�X���v�Z���� */
			GsGetLs(op->coord2, &tmpls);
			/* Set LSAMTRIX to GTE Registers :
			   �X�N���[���^���[�J���}�g���b�N�X��GTE�ɃZ�b�g���� */
			GsSetLsMatrix(&tmpls);
			/* �I�u�W�F�N�g�𓧎��ϊ����I�[�_�����O�e�[�u���ɓo�^���� */
			GsSortObject4(op, &Wot1[outbuf_idx], 14 - OT_LENGTH, getScratchAddr(0));
			op++;
		}
		for (i = 0, op = &object[Objnum]; i < Objnum; i++) {	/* object2 caliculation
									   : �I�u�W�F�N�g�Q�v�Z */
			/* Calculate Local-World Matrix :
			   ���[���h�^���[�J���}�g���b�N�X���v�Z���� */
			GsGetLw(op->coord2, &tmpls);
			/* Set LWMATRIX to GTE Lighting Registers :
			   ���C�g�}�g���b�N�X��GTE�ɃZ�b�g���� */
			GsSetLightMatrix(&tmpls);
			/* Calculate Local-Screen Matrix :
			   �X�N���[���^���[�J���}�g���b�N�X���v�Z���� */
			GsGetLs(op->coord2, &tmpls);
			/* Set LSAMTRIX to GTE Registers :
			   �X�N���[���^���[�J���}�g���b�N�X��GTE�ɃZ�b�g���� */
			GsSetLsMatrix(&tmpls);
			/* Perspective Translate Object and Set OT :
			   �I�u�W�F�N�g�𓧎��ϊ����I�[�_�����O�e�[�u���ɓo�^���� */
			GsSortObject4(op, &Wot2[outbuf_idx], 14 - OT_LENGTH, getScratchAddr(0));
			op++;
		}
		padd = PadRead(1);	/* Readint Control Pad data :
					   �p�b�h�̃f�[�^��ǂݍ��� */
		VSync(0);	/* Wait VSYNC : V�u�����N��҂� */

		ResetGraph(1);	/* Reset GPU : GPU�����Z�b�g���� */
		GsSwapDispBuff();	/* Swap double buffer : �_�u���o�b�t�@��ؑւ��� */
		SetDispMask(1);
		/* Set SCREEN CLESR PACKET to top of OT :
		   ��ʂ̃N���A���I�[�_�����O�e�[�u���̍ŏ��ɓo�^���� */
		GsSortClear(0x0, 0x0, 0x0, &Wot1[outbuf_idx]);
		/* Sort OT each other , Wot1 is represent sorted OTS :
		   �I�[�_�����O�e�[�u�����m���\�[�g���� �\�[�g���ꂽOT�� Wot1�ɓ�������� */
		GsSortOt(&Wot2[outbuf_idx], &Wot1[outbuf_idx]);
		/* Start Drawing :
		   �I�[�_�����O�e�[�u���ɓo�^����Ă���p�P�b�g�̕`����J�n���� */
		GsDrawOt(&Wot1[outbuf_idx]);
	}
}


obj_interactive()
{
	SVECTOR v1;
	MATRIX  tmp1;

	/* Rotate object1 around Y : �I�u�W�F�N�g1��Y����]������ */
	if ((padd & PADRleft) > 0)
		PLocal1.vy -= 5 * ONE / 360;
	/* Rotate object1 around Y : �I�u�W�F�N�g1��Y����]������ */
	if ((padd & PADRright) > 0)
		PLocal1.vy += 5 * ONE / 360;
	/* Rotate object1 around X : �I�u�W�F�N�g1��X����]������ */
	if ((padd & PADRup) > 0)
		PLocal1.vx += 5 * ONE / 360;
	/* Rotate object1 around X : �I�u�W�F�N�g1��X����]������ */
	if ((padd & PADRdown) > 0)
		PLocal1.vx -= 5 * ONE / 360;
	/* Transfer object1 along Z : �I�u�W�F�N�g1��Z���ɂ����ē����� */
	if ((padd & PADl) > 0)
		DLocal1.coord.t[2] -= 100;
	/* Transfer object1 along Z : �I�u�W�F�N�g1��Z���ɂ����ē����� */
	if ((padd & PADm) > 0)
		DLocal1.coord.t[2] += 100;
	/* Calculate Matrix from Object Parameter and Set Coordinate :
	   �I�u�W�F�N�g�̃p�����[�^����}�g���b�N�X���v�Z�����W�n�ɃZ�b�g */
	set_coordinate(&PLocal1, &DLocal1);

	/* Rotate object2 around Y : �I�u�W�F�N�g2��Y����]������ */
	if ((padd & PADLleft) > 0)
		PLocal2.vy -= 5 * ONE / 360;
	/* Rotate object2 around Y : �I�u�W�F�N�g2��Y����]������ */
	if ((padd & PADLright) > 0)
		PLocal2.vy += 5 * ONE / 360;
	/* Rotate object2 around X : �I�u�W�F�N�g2��X����]������ */
	if ((padd & PADLup) > 0)
		PLocal2.vx += 5 * ONE / 360;
	/* Rotate object2 around X : �I�u�W�F�N�g2��X����]������ */
	if ((padd & PADLdown) > 0)
		PLocal2.vx -= 5 * ONE / 360;
	/* Transfer object2 along Z : �I�u�W�F�N�g2��Z���ɂ����ē����� */
	if ((padd & PADn) > 0)
		DLocal2.coord.t[2] -= 100;
	/* Transfer object2 along Z : �I�u�W�F�N�g2��Z���ɂ����ē����� */
	if ((padd & PADo) > 0)
		DLocal2.coord.t[2] += 100;
	/* Calculate Matrix from Object Parameter and Set Coordinate :
	   �I�u�W�F�N�g�̃p�����[�^����}�g���b�N�X���v�Z�����W�n�ɃZ�b�g */

	/* exit program : �v���O�������I�����ă��j�^�ɖ߂� */
	if ((padd & PADk) > 0) {
		PadStop();
		ResetGraph(3);
		StopCallback();
		return 0;
	}
	set_coordinate(&PLocal2, &DLocal2);
	return 1;
}


init_all()
{				/* initialize rotines : ���������[�`���Q */
	ResetGraph(0);		/* reset GPU : GPU���Z�b�g */
	PadInit(0);		/* init controler : �R���g���[�������� */
	padd = 0;		/* init contorler value : �R���g���[���l������ */

#if 0
	GsInitGraph(640, 480, GsINTER | GsOFSGPU, 1, 0);
	/* set the resolution of screen (interrace mode) :
	   �𑜓x�ݒ�i�C���^�[���[�X���[�h�j */
	GsDefDispBuff(0, 0, 0, 0);	/* set the double buffers :
					   �_�u���o�b�t�@�w�� */
#endif

	GsInitGraph(640, 240, GsNONINTER | GsOFSGPU, 1, 0);
	/* set the resolution of screen (on interrace mode) :
	   �𑜓x�ݒ�i�C���^�[���[�X���[�h�j�f�B�U�I�� GPU�I�t�Z�b�g */
	GsDefDispBuff(0, 0, 0, 240);	/* set the double buffers :
					   �_�u���o�b�t�@�w�� */
	GsInit3D();		/* init 3d part of libgs : �RD�V�X�e�������� */

	Wot1[0].length = OT_LENGTH;	/* set the length of OT :
					   �I�[�_�����O�e�[�u���n���h���ɉ𑜓x�ݒ� */
	Wot1[0].org = zsorttable1[0];	/* set the top address of OT1 tags :
					   �I�[�_�����O�e�[�u���n���h����
					   �I�[�_�����O�e�[�u���̎��̐ݒ� */
	/* set anoter OT for double buffer : �_�u���o�b�t�@�̂��߂�������ɂ������ݒ� */
	Wot1[1].length = OT_LENGTH;
	Wot1[1].org = zsorttable1[1];

	Wot2[0].length = OT_LENGTH;
	Wot2[0].org = zsorttable2[0];

	Wot2[1].length = OT_LENGTH;
	Wot2[1].org = zsorttable2[1];

	coord_init();		/* initialize the coordinate system : ���W��` */
	model_init();		/* set up the modeling data :
				   ���f�����O�f�[�^�ǂݍ��� */
	view_init();		/* set the viewpoint : ���_�ݒ� */
	light_init();		/* set the flat light : ���s�����ݒ� */

	texture_init(TEX_ADDR);	/* 16bit texture load */
	texture_init(TEX_ADDR1);/* 8bit  texture load */
	texture_init(TEX_ADDR2);/* 4bit  texture load */
}


view_init()
{				/* set the viewpoint : ���_�ݒ� */
	/*---- Set projection,view ----*/
	GsSetProjection(1000);	/* set the projection : �v���W�F�N�V�����ݒ� */

	/* set the viewpoint parameter : ���_�p�����[�^�ݒ� */
	view.vpx = 0;
	view.vpy = 0;
	view.vpz = 2000;
	/* set the refarence point parameter : �����_�p�����[�^�ݒ� */
	view.vrx = 0;
	view.vry = 0;
	view.vrz = -4000;
	/* set the roll pameter of viewpoint : ���_�̔P��p�����[�^�ݒ� */
	view.rz = 0;
	view.super = WORLD;	/* set the view coordinate : ���_���W�p�����[�^�ݒ� */

	/* set the view point from parameters (libgs caliculate World-Screen
	   Matrix) : ���_�p�����[�^���Q���王�_��ݒ肷��
	   ���[���h�X�N���[���}�g���b�N�X���v�Z���� */
	GsSetRefView2(&view);
}


light_init()
{				/* init Flat light : ���s�����ݒ� */
	/* Setting Light ID 0 : ���C�gID�O �ݒ� */
	/* Setting direction vector of Light0 : ���s���������p�����[�^�ݒ� */
	pslt[0].vx = 20;
	pslt[0].vy = -100;
	pslt[0].vz = -100;

	/* Setting color of Light0 : ���s�����F�p�����[�^�ݒ� */
	pslt[0].r = 0xd0;
	pslt[0].g = 0xd0;
	pslt[0].b = 0xd0;

	/* Set Light0 from parameters : �����p�����[�^��������ݒ� */
	GsSetFlatLight(0, &pslt[0]);

	/* Setting Light ID 1 : ���C�gID�P �ݒ� */
	pslt[1].vx = 20;
	pslt[1].vy = -50;
	pslt[1].vz = 100;
	pslt[1].r = 0x80;
	pslt[1].g = 0x80;
	pslt[1].b = 0x80;
	GsSetFlatLight(1, &pslt[1]);

	/* Setting Light ID 2 : ���C�gID�Q �ݒ� */
	pslt[2].vx = -20;
	pslt[2].vy = 20;
	pslt[2].vz = -100;
	pslt[2].r = 0x60;
	pslt[2].g = 0x60;
	pslt[2].b = 0x60;
	GsSetFlatLight(2, &pslt[2]);

	/* Setting Ambient : �A���r�G���g�ݒ� */
	GsSetAmbient(0, 0, 0);

	/* Setting default light mode : �����v�Z�̃f�t�H���g�̕����ݒ� */
	GsSetLightMode(0);
}

coord_init()
{				/* Setting coordinate : ���W�n�ݒ� */
	/* init coordinates : ���W�̒�` */
	GsInitCoordinate2(WORLD, &DWorld);
	GsInitCoordinate2(&DWorld, &DLocal1);
	GsInitCoordinate2(&DWorld, &DLocal2);
	/* Init work vector : �}�g���b�N�X�v�Z���[�N�̃��[�e�[�V�����x�N�^�[������ */
	PWorld.vx = PWorld.vy = PWorld.vz = 0;
	PLocal1 = PLocal2 = PWorld;
	/* the org point of DWold is set to Z = -40000 :
	   �I�u�W�F�N�g�̌��_�����[���h��Z = -4000�ɐݒ� */
	DWorld.coord.t[2] = -4000;
}


/* Set coordinte parameter from work vector :
   ���[�e�V�����x�N�^����}�g���b�N�X���쐬�����W�n�ɃZ�b�g���� */
set_coordinate(pos, coor)
	SVECTOR *pos;		/* work vector : ���[�e�V�����x�N�^ */
	GsCOORDINATE2 *coor;	/* Coordinate : ���W�n */
{
	MATRIX  tmp1;

	/* Set translation : ���s�ړ����Z�b�g���� */
	tmp1.t[0] = coor->coord.t[0];
	tmp1.t[1] = coor->coord.t[1];
	tmp1.t[2] = coor->coord.t[2];

	/* Rotate Matrix : ���[�e�[�V�����x�N�^�����]�}�g���b�N�X���쐬���� */
	RotMatrix(pos, &tmp1);

	/* Set Matrix to Coordinate : ���߂��}�g���b�N�X�����W�n�ɃZ�b�g���� */
	coor->coord = tmp1;

	/* Clear flag becase of changing parameter :
	   �}�g���b�N�X�L���b�V�����t���b�V������ */
	coor->flg = 0;
}

/* Load texture to VRAM : �e�N�X�`���f�[�^��VRAM�Ƀ��[�h���� */
texture_init(addr)
	u_long  addr;
{
	RECT    rect1;
	GsIMAGE tim1;

	/* Get texture information of TIM FORMAT :
	   TIM�f�[�^�̃w�b�_����e�N�X�`���̃f�[�^�^�C�v�̏��𓾂� */
	GsGetTimInfo((u_long *) (addr + 4), &tim1);

	rect1.x = tim1.px;	/* X point of image data on VRAM :
				   �e�N�X�`�������VRAM�ł�X���W */
	rect1.y = tim1.py;	/* Y point of image data on VRAM :
				   �e�N�X�`�������VRAM�ł�Y���W */
	rect1.w = tim1.pw;	/* Width of image : �e�N�X�`���� */
	rect1.h = tim1.ph;	/* Height of image : �e�N�X�`������ */

	/* Load texture to VRAM : VRAM�Ƀe�N�X�`�������[�h���� */
	LoadImage(&rect1, tim1.pixel);

	/* Exist Color Lookup Table : �J���[���b�N�A�b�v�e�[�u�������݂��� */
	if ((tim1.pmode >> 3) & 0x01) {
		rect1.x = tim1.cx;	/* X point of CLUT data on VRAM :
					   �N���b�g�����VRAM�ł�X���W */
		rect1.y = tim1.cy;	/* Y point of CLUT data on VRAM :
					   �N���b�g�����VRAM�ł�Y���W */
		rect1.w = tim1.cw;	/* Width of CLUT : �N���b�g�̕� */
		rect1.h = tim1.ch;	/* Height of CLUT : �N���b�g�̍��� */

		/* Load CULT data to VRAM : VRAM�ɃN���b�g�����[�h���� */
		LoadImage(&rect1, tim1.clut);
	}
}

model_init()
{				/* set up the modeling data :
				   ���f�����O�f�[�^�̓ǂݍ��� */
	u_long *dop;
	GsDOBJ2 *objp;		/* the handler or modeling data :
				   ���f�����O�f�[�^�n���h�� */
	int     i;

	dop = (u_long *) MODEL_ADDR;	/* the top address of modeling data :
					   ���f�����O�f�[�^���i�[����Ă���A�h
					   ���X */
	dop++;			/* hedder skip */

	GsMapModelingData(dop);	/* map the modeling data to real address :
				   ���f�����O�f�[�^�iTMD�t�H�[�}�b�g�j��
				   ���A�h���X�Ƀ}�b�v���� */
	dop++;
	Objnum = *dop;		/* get the number of objects :
				   �I�u�W�F�N�g����TMD�̃w�b�_���瓾�� */
	dop++;			/* inc the address to link to the handler :
				   GsLinkObject5�Ń����N���邽�߂�TMD��
				   �I�u�W�F�N�g�̐擪�ɂ����Ă��� */

	for (i = 0; i < Objnum; i++)	/* Link TMD data and Object Handler :
					   TMD�f�[�^�ƃI�u�W�F�N�g�n���h����ڑ����� */
		GsLinkObject4((u_long) dop, &object[i], i);

	/* Link TMD data and Object Handler (reuse same modeling with
	   diffrent object handler) :
	   TMD�f�[�^�ƃI�u�W�F�N�g�n���h����ڑ�����iobject�g���񂵁j */
	for (i = 0; i < Objnum; i++)
		GsLinkObject4((u_long) dop, &object[i + Objnum], i);

	for (i = 0, objp = object; i < Objnum; i++) {	
		/* default object coordinate : �f�t�H���g�̃I�u�W�F�N�g�̍��W�n�̐ݒ� */
		objp->coord2 = &DLocal1;
		objp->attribute = 0;	/* init attributes */
		objp++;
	}

	for (i = 0, objp = &object[Objnum]; i < Objnum; i++) {
		/* default object coordinate : �f�t�H���g�̃I�u�W�F�N�g�̍��W�n�̐ݒ� */
		objp->coord2 = &DLocal2;
		objp->attribute = 0;	/* init attributes */
		objp++;
	}
}