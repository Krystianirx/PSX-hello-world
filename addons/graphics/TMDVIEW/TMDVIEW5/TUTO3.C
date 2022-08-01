/* $PSLibId: Runtime Library Release 3.6$ */
/*
 *	tmdview5: GsDOBJ5 object viewing rotine (subdivide polygons)
 *
 *	"tuto3.c" ******** divide GsDOBJ5 Viewing routine
 *
 *		Version 1.00	Dec,  8, 1994
 *
 *		Copyright (C) 1993 by Sony Computer Entertainment
 *		All rights Reserved
 */

#include <sys/types.h>

#include <libetc.h>		/* for Control Pad :
				   PAD���g�����߂ɃC���N���[�h����K�v���� */
#include <libgte.h>		/* LIBGS uses libgte :
				   LIGS���g�����߂ɃC���N���[�h����K�v����*/
#include <libgpu.h>		/* LIBGS uses libgpu :
				   LIGS���g�����߂ɃC���N���[�h����K�v���� */
#include <libgs.h>		/* for LIBGS :
				   �O���t�B�b�N���C�u���� ���g�����߂�
				   �\���̂Ȃǂ���`����Ă��� */

#define OBJECTMAX 100		/* Max Objects :
				   �RD�̃��f���͘_���I�ȃI�u�W�F�N�g��
                                   �������邱�̍ő吔 ���`���� */

#define TEX_ADDR   0x80010000	/* Top Address of texture data1 (TIM FORMAT) :
				   �e�N�X�`���f�[�^�iTIM�t�H�[�}�b�g�j
				   ���������A�h���X */

#define TEX_ADDR1   0x80020000  /* Top Address of texture data1 (TIM FORMAT) */
#define TEX_ADDR2   0x80030000  /* Top Address of texture data2 (TIM FORMAT) */


#define MODEL_ADDR 0x80040000	/* Top Address of modeling data (TMD FORMAT) :
				   ���f�����O�f�[�^�iTMD�t�H�[�}�b�g�j
				   ���������A�h���X */

#define OT_LENGTH  12		/* bit length of OT :
				   �I�[�_�����O�e�[�u���̉𑜓x */


GsOT            Wot[2];		/* OT handler for double buffers :
				   �I�[�_�����O�e�[�u���n���h��
				   �_�u���o�b�t�@�̂��߂Q�K�v */

GsOT_TAG	zsorttable[2][1<<OT_LENGTH]; /* substance of OT :
						�I�[�_�����O�e�[�u������ */

GsDOBJ5		object[OBJECTMAX]; /* Array of Object Handler :
				      �I�u�W�F�N�g�n���h��
				      �I�u�W�F�N�g�̐������K�v */

u_long          Objnum;		/* valibable of number of Objects :
				   ���f�����O�f�[�^�̃I�u�W�F�N�g�̐���
				   �ێ����� */


GsCOORDINATE2   DWorld;  /* Coordinate for GsDOBJ5 :
			    �I�u�W�F�N�g���Ƃ̍��W�n */

SVECTOR         PWorld; /* work short vector for making Coordinate parameter :
			   ���W�n����邽�߂̃��[�e�[�V�����x�N�^�[ */

GsRVIEW2  view;			/* View Point Handler :
				  ���_��ݒ肷�邽�߂̍\���� */
GsF_LIGHT pslt[3];		/* Flat Lighting Handler :
				   ���s������ݒ肷�邽�߂̍\���� */
u_long padd;			/* Controler data :
				   �R���g���[���̃f�[�^��ێ����� */

/* work area of PACKET DATA used double size for packet double buffer :
   �p�P�b�g�f�[�^���쐬���邽�߂̃��[�N �_�u���o�b�t�@�̂���2�{�K�v */
u_long PacketArea[0x10000];

/* work area of PACKET DATA of sub divide for double buffer :
   ���������p�p�P�b�g�f�[�^���쐬���邽�߂̃��[�N
   �_�u���o�b�t�@�̂���2�{�K�v */
u_long PacketAreaDiv[2][0x8000];


/************* MAIN START ******************************************/
main()
{
  int     i;
  GsDOBJ5 *op;			/* the pointer of Object handler :
				   �I�u�W�F�N�g�n���h���ւ̃|�C���^ */
  int     outbuf_idx;
  MATRIX  tmpls,tmplw;

  ResetCallback();
  init_all();
    
  while(1)
    {
      if(obj_interactive()==0)
         return 0;	/* interactive parameter get :
				   �p�b�h�f�[�^���瓮���̃p�����[�^������ */
      GsSetRefView2(&view);	/* caliculate World/Screen matrix :
				   ���[���h�X�N���[���}�g���b�N�X�v�Z */
      
      outbuf_idx=GsGetActiveBuff();/* get the double buffer index :
				      �_�u���o�b�t�@�̂ǂ��炩�𓾂� */
      
      GsClearOt(0,0,&Wot[outbuf_idx]); /* clear ordering table :
					  �I�[�_�����O�e�[�u�����N���A���� */
      GsSetWorkBase((PACKET *)PacketAreaDiv[outbuf_idx]);
      for(i=0,op=object;i<Objnum;i++)
	{
	  /* Calculate Local-World Matrix & Local-Screen Matrix :
	     LW�^LS�}�g���b�N�X���v�Z���� */
	  GsGetLws(op->coord2,&tmplw,&tmpls);
	  /* Set LWMATRIX to GTE Lighting Registers :
	     ���C�g�}�g���b�N�X��GTE�ɃZ�b�g���� */
	  GsSetLightMatrix(&tmplw);
	  /* Set LSAMTRIX to GTE Registers :
	     �X�N���[���^���[�J���}�g���b�N�X��GTE�ɃZ�b�g���� */
	  GsSetLsMatrix(&tmpls);
	  /* Perspective Translate Object and Set OT :
	     �I�u�W�F�N�g�𓧎��ϊ����I�[�_�����O�e�[�u���ɓo�^���� */
	  GsSortObject5(op,&Wot[outbuf_idx],14-OT_LENGTH,getScratchAddr(0));
	  op++;
	}
      padd=PadRead(1);		/* Readint Control Pad data :
				   �p�b�h�̃f�[�^��ǂݍ��� */
      VSync(0);			/* Wait VSYNC : V�u�����N��҂� */
      ResetGraph(1);		/* Reset GPU : GPU�����Z�b�g���� */
      GsSwapDispBuff();		/* Swap double buffer :
				   �_�u���o�b�t�@��ؑւ��� */
      SetDispMask(1);
      
      /* Set SCREEN CLESR PACKET to top of OT :
	 ��ʂ̃N���A���I�[�_�����O�e�[�u���̍ŏ��ɓo�^���� */
      GsSortClear(0x0,0x0,0x0,&Wot[outbuf_idx]);
      
      /* Start Drawing :
	 �I�[�_�����O�e�[�u���ɓo�^����Ă���p�P�b�g�̕`����J�n���� */
      GsDrawOt(&Wot[outbuf_idx]);
    }
}

obj_interactive()
{
  SVECTOR v1;
  MATRIX  tmp1;

  /* Rotate Y : �I�u�W�F�N�g��Y����]������ */
  if((padd & PADRleft)>0) PWorld.vy -=5*ONE/360;
  /* Rotate Y : �I�u�W�F�N�g��Y����]������ */
  if((padd & PADRright)>0) PWorld.vy +=5*ONE/360;
  /* Rotate X : �I�u�W�F�N�g��X����]������ */
  if((padd & PADRup)>0) PWorld.vx+=5*ONE/360;
  /* Rotate X : �I�u�W�F�N�g��X����]������ */
  if((padd & PADRdown)>0) PWorld.vx-=5*ONE/360;

  /* Transfer Z : �I�u�W�F�N�g��Z���ɂ����ē����� */
  if((padd & PADm)>0) DWorld.coord.t[2]-=100;
  /* Transfer Z : �I�u�W�F�N�g��Z���ɂ����ē����� */
  if((padd & PADl)>0) DWorld.coord.t[2]+=100;
  /* Transfer X : �I�u�W�F�N�g��X���ɂ����ē����� */
  if((padd & PADLleft)>0) DWorld.coord.t[0] +=10;
  /* Transfer X : �I�u�W�F�N�g��X���ɂ����ē����� */
  if((padd & PADLright)>0) DWorld.coord.t[0] -=10;
  /* Transfer Y : �I�u�W�F�N�g��Y���ɂ����ē����� */
  if((padd & PADLdown)>0) DWorld.coord.t[1]+=10;
  /* Transfer Y : �I�u�W�F�N�g��Y���ɂ����ē����� */
  if((padd & PADLup)>0) DWorld.coord.t[1]-=10;
  /* Exit Program : �v���O�������I�����ă��j�^�ɖ߂� */
  if((padd & PADk)>0) {PadStop();ResetGraph(3);StopCallback();return 0;}
  
  /* Calculate Matrix from Object Parameter and Set Coordinate :
     �I�u�W�F�N�g�̃p�����[�^����}�g���b�N�X���v�Z�����W�n�ɃZ�b�g */
  set_coordinate(&PWorld,&DWorld);
  return 1;
}

init_all()			/* initialize rotines : ���������[�`���Q */
{
  ResetGraph(0);		/* reset GPU : GPU���Z�b�g */
  PadInit(0);			/* init controler : �R���g���[�������� */
  padd=0;			/* init contorler value :
				   �R���g���[���l������ */

#if 0
  GsInitGraph(640,480,GsINTER|GsOFSGPU,1,0);
  /* set the resolution of screen (interrace mode) :
     �𑜓x�ݒ�i�C���^�[���[�X���[�h�j */
  
  GsDefDispBuff(0,0,0,0);	/* set the double buffers :
				   �_�u���o�b�t�@�w�� */
#endif
  GsInitGraph(640,240,GsNONINTER|GsOFSGPU,1,0);
  /* set the resolution of screen (on interrace mode) :
     �𑜓x�ݒ�i�m���C���^�[���[�X���[�h�j */
  GsDefDispBuff(0,0,0,240);   /* set the double buffers :
				 �_�u���o�b�t�@�w�� */
  GsInit3D();                 /* init 3d part of libgs :
				 �RD�V�X�e�������� */
  
  Wot[0].length=OT_LENGTH;    /* set the length of OT1 : OT�P�ɉ𑜓x�ݒ� */
  Wot[0].org=zsorttable[0];   /* set the top address of OT1 tags :
				 OT�P�ɃI�[�_�����O�e�[�u���̎��̐ݒ� */
  /* set anoter OT for double buffer :
     �_�u���o�b�t�@�̂��߂�������ɂ������ݒ� */
  Wot[1].length=OT_LENGTH;
  Wot[1].org=zsorttable[1];
  
  coord_init();			/* initialize the coordinate system :
				   ���W��` */
  model_init();			/* set up the modeling data :
				   ���f�����O�f�[�^�ǂݍ��� */
  view_init();			/* set the viewpoint : ���_�ݒ� */
  light_init();			/* set the flat light : ���s�����ݒ� */
  
  texture_init(TEX_ADDR);	/* 16bit texture load */
  texture_init(TEX_ADDR1);	/* 8bit  texture load */
  texture_init(TEX_ADDR2);	/* 4bit  texture load */
}

view_init()			/* set the viewpoint : ���_�ݒ� */
{
  /*---- Set projection,view ----*/
  GsSetProjection(1000);	/* set the projection : �v���W�F�N�V�����ݒ� */
  
  /* set the viewpoint parameter : ���_�p�����[�^�ݒ� */
  view.vpx = 0; view.vpy = 0; view.vpz = 2000;
  /* set the refarence point parameter : �����_�p�����[�^�ݒ� */  
  view.vrx = 0; view.vry = 0; view.vrz = -4000;
  /* set the roll pameter of viewpoint : ���_�̔P��p�����[�^�ݒ� */
  view.rz=0;
  view.super = WORLD;		/* set the view coordinate :
				   ���_���W�p�����[�^�ݒ� */
  
  /* set the view point from parameters (libgs caliculate World-Screen Matrix)
   : ���_�p�����[�^���Q���王�_��ݒ肷��
     ���[���h�X�N���[���}�g���b�N�X���v�Z���� */
  GsSetRefView2(&view);
}

light_init()			/* init Flat light : ���s�����ݒ� */
{
  /* Setting Light ID 0 : ���C�gID�O �ݒ� */
  /* Setting direction vector of Light0 :
     ���s���������p�����[�^�ݒ� */
  pslt[0].vx = 20; pslt[0].vy= -100; pslt[0].vz= -100;
  
  /* Setting color of Light0 :
     ���s�����F�p�����[�^�ݒ� */
  pslt[0].r=0xd0; pslt[0].g=0xd0; pslt[0].b=0xd0;
  
  /* Set Light0 from parameters :
     �����p�����[�^��������ݒ� */
  GsSetFlatLight(0,&pslt[0]);
  
  /* Setting Light ID 1 : ���C�gID�P �ݒ� */
  pslt[1].vx = 20; pslt[1].vy= -50; pslt[1].vz= 100;
  pslt[1].r=0x80; pslt[1].g=0x80; pslt[1].b=0x80;
  GsSetFlatLight(1,&pslt[1]);
  
  /* Setting Light ID 2 : ���C�gID�Q �ݒ� */
  pslt[2].vx = -20; pslt[2].vy= 20; pslt[2].vz= -100;
  pslt[2].r=0x60; pslt[2].g=0x60; pslt[2].b=0x60;
  GsSetFlatLight(2,&pslt[2]);
  
  /* Setting Ambient : �A���r�G���g�ݒ� */
  GsSetAmbient(0,0,0);

  /* Setting default light mode : �����v�Z�̃f�t�H���g�̕����ݒ� */
  GsSetLightMode(0);
}

coord_init()			/* Setting coordinate : ���W�n�ݒ� */
{
  /* Setting hierarchy of Coordinate : ���W�̒�` */
  GsInitCoordinate2(WORLD,&DWorld);
  
  /* Init work vector :
     �}�g���b�N�X�v�Z���[�N�̃��[�e�[�V�����x�N�^�[������ */
  PWorld.vx=PWorld.vy=PWorld.vz=0;
  
  /* the org point of DWold is set to Z = -40000 :
     �I�u�W�F�N�g�̌��_�����[���h��Z = -4000�ɐݒ� */
  DWorld.coord.t[2] = -4000;
}


/* Set coordinte parameter from work vector :
   ���[�e�V�����x�N�^����}�g���b�N�X���쐬�����W�n�ɃZ�b�g���� */
set_coordinate(pos,coor)
SVECTOR *pos;			/* work vector : ���[�e�V�����x�N�^ */
GsCOORDINATE2 *coor;		/* Coordinate : ���W�n */
{
  MATRIX tmp1;
  
  /* Set translation : ���s�ړ����Z�b�g���� */
  tmp1.t[0] = coor->coord.t[0];
  tmp1.t[1] = coor->coord.t[1];
  tmp1.t[2] = coor->coord.t[2];
  
  /* Rotate Matrix :
     ���[�e�[�V�����x�N�^�����]�}�g���b�N�X���쐬���� */
  RotMatrix(pos,&tmp1);
  
  /* Set Matrix to Coordinate :
     ���߂��}�g���b�N�X�����W�n�ɃZ�b�g���� */
  coor->coord = tmp1;

  /* Clear flag becase of changing parameter :
     �}�g���b�N�X�L���b�V�����t���b�V������ */
  coor->flg = 0;
}

/* Load texture to VRAM :
   �e�N�X�`���f�[�^��VRAM�Ƀ��[�h���� */
texture_init(addr)
u_long addr;
{
  RECT rect1;
  GsIMAGE tim1;

  /* Get texture information of TIM FORMAT :
     TIM�f�[�^�̃w�b�_����e�N�X�`���̃f�[�^�^�C�v�̏��𓾂� */
  GsGetTimInfo((u_long *)(addr+4),&tim1);
  
  rect1.x=tim1.px;		/* X point of image data on VRAM :
				   �e�N�X�`�������VRAM�ł�X���W */
  rect1.y=tim1.py;		/* Y point of image data on VRAM :
				   �e�N�X�`�������VRAM�ł�Y���W */
  rect1.w=tim1.pw;		/* Width of image : �e�N�X�`���� */
  rect1.h=tim1.ph;		/* Height of image : �e�N�X�`������ */
  
  /* Load texture to VRAM : VRAM�Ƀe�N�X�`�������[�h���� */
  LoadImage(&rect1,tim1.pixel);
  
  /* Exist Color Lookup Table : �J���[���b�N�A�b�v�e�[�u�������݂��� */  
  if((tim1.pmode>>3)&0x01)
    {
      rect1.x=tim1.cx;		/* X point of CLUT data on VRAM :
				   �N���b�g�����VRAM�ł�X���W */
      rect1.y=tim1.cy;		/* Y point of CLUT data on VRAM :
				   �N���b�g�����VRAM�ł�Y���W */
      rect1.w=tim1.cw;		/* Width of CLUT : �N���b�g�̕� */
      rect1.h=tim1.ch;		/* Height of CLUT : �N���b�g�̍��� */

      /* Load CULT data to VRAM : VRAM�ɃN���b�g�����[�h���� */
      LoadImage(&rect1,tim1.clut);
    }
}

model_init()
{				/* set up the modeling data :
				   ���f�����O�f�[�^�̓ǂݍ��� */
  u_long *dop;
  GsDOBJ5 *objp;		/* the handler or modeling data :
				   ���f�����O�f�[�^�n���h�� */
  int i;
  u_long *oppp;
  
  dop=(u_long *)MODEL_ADDR;	/* the top address of modeling data :
				   ���f�����O�f�[�^���i�[����Ă���A�h���X */
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

  for(i=0;i<Objnum;i++)		/* Link TMD data and Object Handler :
				   TMD�f�[�^�ƃI�u�W�F�N�g�n���h����ڑ����� */
    GsLinkObject5((u_long)dop,&object[i],i);
  
  oppp = PacketArea;		/* preset packet area :
				   packet�̘g�g�����A�h���X */
  for(i=0,objp=object;i<Objnum;i++)
    { /* default object coordinate : �f�t�H���g�̃I�u�W�F�N�g�̍��W�n�̐ݒ� */
      objp->coord2 =  &DWorld;
      
      objp->attribute = 0;	/* attribute init */
      
      /* subdivide attribute 2x2 divide :
	 attribute�̕����|���S������i���̑���GsDIV2,3,4,5������j */
      objp->attribute |= GsDIV1;

      oppp = GsPresetObject(objp,oppp);
      objp++;
    }
}