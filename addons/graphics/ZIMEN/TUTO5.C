/* $PSLibId: Runtime Library Release 3.6$ */
/*
 *
 *
 *	"tuto0.c" loop 3DBG (FOG MAP DATA)
 *
 *		Version 1.00	Jul,  14, 1994
 *
 *		Copyright (C) 1995 by Sony Computer Entertainment
 *		All rights Reserved
 */
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>	

#include "struct.h"

/***************************************************************************
 * 
 *	BG Handler:BG �n���h�����O
 *
 ***************************************************************************/
/* BG-CELL structure
 * This example uses only one clut. Therefore "clut" member does not used here.
 * This example does not use "attr" member. See bgmap.h
 : BG �Z���\���� 
 * ���̗�ł� CLUT �͂P�{�Ȃ̂� clut �����o�͎g�p���Ȃ��B
 * ���̗�ł� ������������Ȃ��̂� attr �����o�͎g�p���Ȃ��B
 * bgmap.h ���Q�� 
 */
extern	u_long	bgtex[];	/* BG texture�ECLUT (4bit) */
extern	u_long	bgtex8[];	/* BG texture�ECLUT (8bit) */

#define PACKETMAX 4000		/* Max GPU packets */

#define OBJECTMAX 100		/* Max Objects :
				   �RD�̃��f���͘_���I�ȃI�u�W�F�N�g��
                                   �������邱�̍ő吔 ���`���� */

#define PACKETMAX2 (PACKETMAX*24) /* size of PACKETMAX (byte)
                                     paket size may be 24 byte(6 word) */

#define OT_LENGTH  4		/* bit length of OT :
				   �I�[�_�����O�e�[�u���̉𑜓x */


GsOT            Wot[2];		/* Handler of OT
                                   Uses 2 Hander for Dowble buffer :
				   �I�[�_�����O�e�[�u���n���h��
				   �_�u���o�b�t�@�̂��߂Q�K�v */

GsOT_TAG	zsorttable[2][1<<OT_LENGTH]; /* Area of OT :
						�I�[�_�����O�e�[�u������ */

#include "bgmap.h"		/* 2D array about the cell pattern :
				   �Z���^�C�v���L�q�����Q�����z�� */
Gs3DBG0		gobj = {32, 32, 10, _map, _ctype};
GsDIVCOND0      cond;
GsDPCLUT0       ClutHandle;

u_long divz[6] = {3,2,2,2,1,1}; /* shift length */

#define CBIT 5
#define CNUM (1<<CBIT)
#define BGR 200
#define BGG 200
#define BGB 200

GsCOORDINATE2   DJimen;  /* Coordinate for Gs3DBG :
			    �I�u�W�F�N�g���Ƃ̍��W�n */

SVECTOR         PJimen; /* work short vector for making Coordinate parmeter :
			   ���W�n����邽�߂̃��[�e�[�V�����x�N�^�[ */
SVECTOR         PView; /* work short vector for making Coordinate parmeter :
			  ���W�n����邽�߂̃��[�e�[�V�����x�N�^�[ */

GsCOORDINATE2   DLocal;		/* local coordinate for setting viewpoint :
				   ���_���Ԃ炳���郍�[�J�����W */
SVECTOR         PLocal;		/* rotation work vector : ��]�x�N�g��work */


extern MATRIX GsIDMATRIX,GsIDMATRIX2;	/* Unit Matrix : �P�ʍs�� */
extern PACKET *GsOUT_PACKET_P;

GsVIEW2  view;			/* View Point Handler :
				   ���_��ݒ肷�邽�߂̍\���� */
GsF_LIGHT pslt[3];		/* Flat Lighting Handler :
				   ���s������ݒ肷�邽�߂̍\���� */
u_long padd;			/* Controler data :
				   �R���g���[���̃f�[�^��ێ����� */

PACKET		out_packet[2][PACKETMAX2];  /* GPU PACKETS AREA */

u_long Projection = 800;


#define DEBUG

/************* MAIN START ******************************************/
main()
{
  int     i;

  int     outbuf_idx;		/* double buffer index */
  MATRIX  tmpls,tmplw;
#ifdef DEBUG
  int ratch_vcount;
#endif
  ResetCallback();
  init_all();
  SetGraphDebug(0);
  GsInitVcount();
  
/*  KanjiFntOpen(-320+32, -240+32, 640-64, 200, 704, 0, 768, 256, 0, 512); */
  FntLoad(960, 256);
/*  SetDumpFnt(FntOpen(-320+32, -240+32, 640-64, 200, 0, 512)); */
  FntOpen(-320+32, -240+32, 640-64, 200, 0, 512);
  
  while((PadRead(1)&PADselect) == 0) 
    {
      obj_interactive2();	/* interactive parameter get :
				   �p�b�h�f�[�^���瓮���̃p�����[�^������ */

      GsSetView2(&view);	/* Calculate World-Screen Matrix :
				   ���[���h�X�N���[���}�g���b�N�X�v�Z */

      outbuf_idx=GsGetActiveBuff();/* Get double buffer index :
				      �_�u���o�b�t�@�̂ǂ��炩�𓾂� */

      /* Set top address of Packet Area for output of GPU PACKETS */
      GsSetWorkBase((PACKET*)out_packet[outbuf_idx]);

      GsClearOt(0,0,&Wot[outbuf_idx]); /* Clear OT for using buffer :
					  �I�[�_�����O�e�[�u�����N���A���� */

      /* Calculate Local-World Matrix :
	 ���[���h�^���[�J���}�g���b�N�X���v�Z���� */
	  GsGetLw(gobj.coord2,&tmplw);
      
      /* Set LWMATRIX to GTE Lighting Registers :
	 ���C�g�}�g���b�N�X��GTE�ɃZ�b�g���� */
	GsSetLightMatrix(&tmplw);
      
      /* Calculate Local-Screen Matrix :
	 �X�N���[���^���[�J���}�g���b�N�X���v�Z���� */
	GsGetLs(gobj.coord2,&tmpls);
      
#ifdef DEBUG
      ratch_vcount = VSync(1);
#endif
      GsSort3DBG0_DPQ(&gobj,&Wot[outbuf_idx],14-OT_LENGTH,&tmpls,&ClutHandle);
#ifdef DEBUG
      FntPrint("load = %d\n%d (%d %d %d)\n",VSync(1)-ratch_vcount,
		    PLocal.vy,
		    DLocal.coord.t[0],DLocal.coord.t[1],DLocal.coord.t[2]);
#endif
      VSync(0);			/* Wait VSYNC : V�u�����N��҂� */
/*      DrawSync(0);*/
      ResetGraph(1);
      padd=PadRead(1);		/* Readint Control Pad data :
				   �p�b�h�̃f�[�^��ǂݍ��� */
      
      GsSwapDispBuff();		/* Swap double buffer :
				   �_�u���o�b�t�@��ؑւ��� */

      /* Set SCREEN CLESR PACKET to top of OT :
	 ��ʂ̃N���A���I�[�_�����O�e�[�u���̍ŏ��ɓo�^���� */
      GsSortClear(BGR, BGG, BGB, &Wot[outbuf_idx]);

      
      /* Drawing OT :
	 �I�[�_�����O�e�[�u���ɓo�^����Ă���p�P�b�g�̕`����J�n���� */
      GsDrawOt(&Wot[outbuf_idx]);
      FntFlush(-1);
    }
    PadStop();
    StopCallback();
    return;
}


obj_interactive2()
{
  MATRIX  tmpm;
  SVECTOR dd;
  VECTOR  tmpv;

  dd.vx = dd.vy = dd.vz = 0;
  
  if((padd & PADRright)>0)
    PLocal.vy+=ONE/360/2;
  
  if((padd & PADRleft)>0)
    PLocal.vy-=ONE/360/2;

  set_coordinate(&PLocal,&DLocal);
  
  /* Translate Z :
     ���_��Z���ɂ����ē����� */
  if((padd & PADR1)>0)
    dd.vz = -30;
  
  /* Translate Z :
     ���_��Z���ɂ����ē����� */
  if((padd & PADR2)>0)
    dd.vz = 30;
  
  /* Translate X :
     ���_��X���ɂ����ē����� */
  if((padd & PADLleft)>0)
    dd.vx = -30;
  
  /* Translate X :
     ���_��X���ɂ����ē����� */
  if((padd & PADLright)>0)
    dd.vx = 30;

  /* Translate Y :
     ���_��Y���ɂ����ē����� */
  if((padd & PADLdown)>0)
    dd.vy = 10;
  
  /* Translate Y :
     ���_��Y���ɂ����ē����� */
  if((padd & PADLup)>0)
    dd.vy = -10;

  /* translate the translation in local to world :
     ��ϓI�ړ��̂��߁i���[�J�����W�ł̈ړ��j���[���h�̈ړ��ɕϊ����� */
  ApplyMatrix(&DLocal.coord,&dd,&tmpv);
  DLocal.coord.t[0]+=tmpv.vx;
  DLocal.coord.t[1]+=tmpv.vy;
  DLocal.coord.t[2]+=tmpv.vz;

  /* change effective area :
     ������͈͂�ύX���� */ 
  if((padd & PADo)>0)
    {
      gobj.nw +=1;
      gobj.nh +=1;
    }
  if((padd & PADn)>0 && gobj.nw>1)
    {
      gobj.nw -=1;
      gobj.nh -=1;
    }
  
  /* activate FOG around the edge of the area :
     �n�ʂ̏o������͈͂�FOG�Ŗڗ����Ȃ����� */
  SetFogFar(gobj.ch*gobj.nw/2,Projection);
}


/* Initialize routine :
   ���������[�`���Q */
init_all()
{
  ResetGraph(0);		/* Reset GPU : GPU���Z�b�g */
  PadInit(0);			/* Reset Controller : �R���g���[�������� */
  padd=0;			/* controler value initialize :
				   �R���g���[���l������ */


  GsInitGraph(640,480,GsINTER|GsOFSGPU,1,0);
  /* rezolution set , interrace mode :
     �𑜓x�ݒ�i�C���^�[���[�X���[�h�j */

  GsDefDispBuff(0,0,0,0);	/* Double buffer setting :
				   �_�u���o�b�t�@�w�� */

#if 0
  GsInitGraph(320,240,GsINTER|GsOFSGPU,1,0);
  /* rezolution set , non interrace mode :
     �𑜓x�ݒ�i�m���C���^�[���[�X���[�h�j */
  GsDefDispBuff(0,0,0,240);	/* Double buffer setting :
				   �_�u���o�b�t�@�w�� */
#endif

  GsInit3D();			/* Init 3D system : �RD�V�X�e�������� */
  
  Wot[0].length=OT_LENGTH;	/* Set bit length of OT handler :
				   �I�[�_�����O�e�[�u���n���h���ɉ𑜓x�ݒ� */

  Wot[0].org=zsorttable[0];	/* Set Top address of OT Area to OT handler :
				   �I�[�_�����O�e�[�u���n���h����
				   �I�[�_�����O�e�[�u���̎��̐ݒ� */
  
  /* same setting for anoter OT handler :
     �u���o�b�t�@�̂��߂�������ɂ������ݒ� */
  Wot[1].length=OT_LENGTH;
  Wot[1].org=zsorttable[1];
  
  coord_init();			/* Init coordinate : ���W��` */
  model_init();			/* Reading modeling data :
				   ���f�����O�f�[�^�ǂݍ��� */  
  view_init();			/* Setting view point : ���_�ݒ� */
  light_init();			/* Setting Flat Light : ���s�����ݒ� */
  texture_init();		/* load texture */
}


view_init()			/* Setting view point : ���_�ݒ� */
{
  /*---- Set projection,view ----*/
  GsSetProjection(Projection);	/* Set projection : �v���W�F�N�V�����ݒ� */
  
  view.view  = GsIDMATRIX2;
  view.super = &DLocal;
  view.view.t[2] = -100;
  view.view.t[1] = 0;
  view.view.t[0] = 0;
  PView.vx=PView.vy=PView.vz=0;
  /* Calculate World-Screen Matrix from viewing paramter :
     ���_�p�����[�^���Q���王�_��ݒ肷��
     ���[���h�X�N���[���}�g���b�N�X���v�Z���� */
  GsSetView2(&view);
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

  /* set fog parmaeter : �f�v�X�L���[�C���O�̐ݒ� */
  SetFogNear(8000,Projection);	/* default */
}

coord_init()			/* Setting coordinate : ���W�n�ݒ� */
{
  /* Setting hierarchy of Coordinate : ���W�̒�` */
  GsInitCoordinate2(WORLD,&DJimen);
  /* Init work vector :
     �}�g���b�N�X�v�Z���[�N�̃��[�e�[�V�����x�N�^�[������ */
  PJimen.vx= -90*ONE/360;	/* rotate the gournd 90 degree around x :
				   x���Œn�ʂ�|�� */
  PJimen.vy= PJimen.vz=0;
  set_coordinate(&PJimen,&DJimen);

  GsInitCoordinate2(WORLD,&DLocal);
  DLocal.coord.t[1] = -200;
  PLocal.vx=PLocal.vy=PLocal.vz=0;
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
     �}�g���b�N�X�Ƀ��[�e�[�V�����x�N�^����p������ */
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
texture_init()
{
	u_short		clut, tpage;
	int i;
  /* load texture data and texture clut :
     �e�N�X�`���E�e�N�X�`�� CLUT �����[�h */

#ifdef BG_4bit	/* 4bit mode */
	tpage = LoadTPage(bgtex+0x80, 0, 0, 640, 0, 256, 256);
	clut  = LoadClut(bgtex, 0, 480);
#else		/* 8bit mode */
	tpage = LoadTPage(bgtex8+0x80, 1, 0, 640, 0, 256, 256);
	clut  = LoadClut(bgtex8, 0, 480);
#endif

	ClutHandle.cbit   = CBIT;
	ClutHandle.clut   = (u_short *)bgtex8;
	ClutHandle.rectc.x = 640;
	ClutHandle.rectc.y = 480;
	ClutHandle.rectc.w = 256;
	ClutHandle.rectc.h = 1<<CBIT;
	ClutHandle.bgc.r=BGR;
	ClutHandle.bgc.g=BGG;
	ClutHandle.bgc.b=BGB;
	GsMakeDPClut0(&ClutHandle);
	
	for (i = 0; i < gobj.nctype; i++) {
		gobj.ctype[i].clut = &clut;
		gobj.ctype[i].tpage = tpage;
	}
}


/* Read modeling data (TMD FORMAT) : ���f�����O�f�[�^�̓ǂݍ��� */
model_init()
{
  /* Set Coordinate of Object Handler :
     �t�H���g�̃I�u�W�F�N�g�̍��W�n�̐ݒ� */
    gobj.coord2 =  &DJimen;
    gobj.cw = 1024;		/* widthe of the cell : �Z���̉��� */
    gobj.ch = 1024;		/* height of the cell : �Z���̏c�� */
    gobj.nw = 16;		/* the number of the cell of the width of
				   the map :
				   �Z���̉��̐� */
    gobj.nh = 16;		/* the number of the cell of the height of
				   the map :
				   �Z���̏c�̐� */
    gobj.iw = 0x1f;		/* repeat unit of the map (x) :
				   �Z���̌J��Ԃ��̒P��(x) */
    gobj.ih = 0x1f;		/* repeat unit of the map (y)
				   :�Z���̌J��Ԃ��̒P��(y) */
    gobj.cond = &cond;
    
				/* the condition of active sub divide :
				   �A�N�e�B�u�T�u�f�B�o�C�h���� */
    cond.nearz = Projection/4;	/* z point of satart dividing :
				   divide�𕪊����J�n����Z�l */
    cond.shift = 7;		/* shift : shift�l */
    cond.nz    = 6;		/* maximum divide number :
				   divide�i���̍ő�l */
    cond.cond  = divz;		/* pointer of the table of divide number :
				   divide���e�[�u���ւ̃|�C���^ */
}