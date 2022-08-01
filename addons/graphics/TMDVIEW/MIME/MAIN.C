/*
 * $PSLibId: Runtime Library Release 3.6$
 */
/*
 *	mime (from tmdview5: GsDOBJ5 object viewing rotine)
 *
 *	"tuto0.c" ******** simple GsDOBJ5 Viewing routine
 *
 *		Version 1.00	Jul,  14, 1994
 *
 *		Copyright (C) 1993,1994,1995 by Sony Computer Entertainment
 *		All rights Reserved
 */

#include <sys/types.h>

#include <libetc.h>		/* Must be included when using control pad :
				   PAD���g�����߂ɃC���N���[�h����K�v���� */
#include <libgte.h>		/* LIBGS uses libgte (Must be included when using LIBGS
				/* Must be included when using libgs :
				   LIGS���g�����߂ɃC���N���[�h����K�v����*/
#include <libgpu.h>		/* LIBGS uses libgpu (Must be included when using LIBGS
				/*L Must be included when using libgs :
				  IGS���g�����߂ɃC���N���[�h����K�v���� */
#include <libgs.h>		/* Structures, etc. are defined for using
				   graphics library (LIBGS) :
				   �O���t�B�b�N���C�u���� ���g�����߂�
				   �\���̂Ȃǂ���`����Ă��� */

#include "addr.h"
#include "nmime.h"  
#include "control.h"  

int obj_interactive();

#define OBJECTMAX 100		/* Define the maximum number of logical objects
				   into which a 3D model can be devided :
				   �RD�̃��f���͘_���I�ȃI�u�W�F�N�g��
                                   �������邱�̍ő吔 ���`���� */
   
#define OT_LENGTH  12		/* Resolution of OT :
				   �I�[�_�����O�e�[�u���̉𑜓x */


GsOT            Wot[2];		/* Two handlers are required for OT double
				   buffer :
				   �I�[�_�����O�e�[�u���n���h��
				   �_�u���o�b�t�@�̂��߂Q�K�v */

GsOT_TAG	zsorttable[2][1<<OT_LENGTH]; /* Ordering table (OT) entity :
						�I�[�_�����O�e�[�u������ */

GsDOBJ5		object[OBJECTMAX]; /* Requres as many object handlers as
				      there are objects :
				      �I�u�W�F�N�g�n���h��
				      �I�u�W�F�N�g�̐������K�v */

u_long          Objnum;		/* Retain number of modeling data objects :
				   ���f�����O�f�[�^�̃I�u�W�F�N�g�̐���
				   �ێ����� */

GsCOORDINATE2   DWorld;  /* Coordinate system for each object :
			    �I�u�W�F�N�g���Ƃ̍��W�n */

SVECTOR         PWorld; /* work short vector for making Coordinate parmeter :
			   ���W�n����邽�߂̃��[�e�[�V�����x�N�^�[ */

extern MATRIX GsIDMATRIX;	/* Unit Matrix : �P�ʍs�� */

GsRVIEW2  view;			/* View Point Handler :
				   ���_��ݒ肷�邽�߂̍\���� */
GsF_LIGHT pslt[3];		/* Flat Lighting Handler :
				   ���s������ݒ肷�邽�߂̍\���� */
u_long padd;			/* Controler data :
				   �R���g���[���̃f�[�^��ێ����� */

/* Work area for making packet data double size is required for double buffer:
   �p�P�b�g�f�[�^���쐬���邽�߂̃��[�N�_�u���o�b�t�@�̂���2�{�K�v */
/* u_long PacketArea[0x10000]; */


u_long *PacketArea;

/* Transfer Function for smooth control */

extern int cntrlarry[CTLMAX][CTLTIME]; 
extern CTLFUNC ctlfc[CTLMAX];

/* Data table for Transfer Function */

static int cnv0[16] = {
	  39,  79, 118, 197, 315, 394, 433, 473,
	 473, 433, 394, 315, 197, 118,  79,  39
};

static int cnv1[24] = {
	 200, 325, 450, 475, 500, 475, 450, 375,
	 300, 250, 200, 150, 100,  80,  55,   0,
	 -50, -75,-100, -70, -40, -25, -10,  -5
};

static int cnv2[32] = {
	  20,  30,  40,  50,  59,  79,  98, 128,
	 157, 177, 197, 206, 216, 226, 236, 238,
	 236, 226, 216, 206, 197, 177, 157, 128,
	  98,  79,  59,  50,  40,  30,  20,  10
};

u_long *SCRATCH  = (u_long *)0x1f800000;

/************* MAIN START ******************************************/
main()
{
  u_long  fn;			/* frame (field) No. */
  int     i;
  GsDOBJ5 *op;			/* pointer of Object Handler :
				   �I�u�W�F�N�g�n���h���ւ̃|�C���^ */
  int     outbuf_idx;		/* double buffer index */
  MATRIX  tmpls;
  
  ResetCallback();
  PadInit(0);
  PacketArea = (u_long *)PACKETBUF;
  init_all();
  /*== initialize Transfer Function  ====*/

  init_cntrlarry(cnv1,24);
  /* initialize MIMe function */
  
  init_mime_data(0, MODEL_ADDR, MDFDATAVTX, MDFDATANRM, ORGVTXBUF, ORGNRMBUF);

  /* Initialize object position */
  DWorld.coord.t[2] = -30000; 
  PWorld.vy = ONE/2;
  PWorld.vx = -ONE/12;

  for(fn=0;;fn++)
    {
      	if (obj_interactive()<0) return;/* set motion data by pad data */

		set_cntrl(fn);	/* generate control wave by transfer function */

		PWorld.vy += ctlfc[4].out;  /* Rotate around the Y axis */
		PWorld.vx += ctlfc[5].out;  /* Rotate around the X axis */
		DWorld.coord.t[2] += ctlfc[8].out; /* Move along the Z axis */
		DWorld.coord.t[0] += ctlfc[6].out; /* Move along the X axis */
		DWorld.coord.t[1] += ctlfc[7].out; /* Move along the Y axis */

		/*==MIME Function=======*/
		mimepr[0][0] = ctlfc[0].out; /* MIMe Weight (Control) value 0 */
		mimepr[0][1] = ctlfc[1].out; /* MIMe Weight (Control) value 1 */
		mimepr[0][2] = ctlfc[2].out; /* MIMe Weight (Control) value 2 */
		mimepr[0][3] = ctlfc[3].out; /* MIMe Weight (Control) value 3 */
		vertex_mime(0);			/* MIMe operation (Vertex) */
		normal_mime(0);			/* MIMe operation (Normal) */

      GsSetRefView2(&view);	/* Calculate World-Screen Matrix :
				   ���[���h�X�N���[���}�g���b�N�X�v�Z */
      outbuf_idx=GsGetActiveBuff();/* Get double buffer index :
				      �_�u���o�b�t�@�̂ǂ��炩�𓾂� */

      GsClearOt(0,0,&Wot[outbuf_idx]); /* Clear OT for using buffer :
					  �I�[�_�����O�e�[�u�����N���A���� */

      for(i=0,op=object;i<Objnum;i++)
	{
	  /* Calculate Local-World Matrix :
	     ���[���h�^���[�J���}�g���b�N�X���v�Z���� */
	  GsGetLw(op->coord2,&tmpls);
	  
	  /* Set LWMATRIX to GTE Lighting Registers :
	     ���C�g�}�g���b�N�X��GTE�ɃZ�b�g���� */
	  GsSetLightMatrix(&tmpls);
	  
	  /* Calculate Local-Screen Matrix :
	     �X�N���[���^���[�J���}�g���b�N�X���v�Z���� */
	  GsGetLs(op->coord2,&tmpls);

	  /* Set LSAMTRIX to GTE Registers :
	     �X�N���[���^���[�J���}�g���b�N�X��GTE�ɃZ�b�g���� */
	  GsSetLsMatrix(&tmpls);
	  
	  /* Perspective Translate Object and Set OT :
	     �I�u�W�F�N�g�𓧎��ϊ����I�[�_�����O�e�[�u���ɓo�^���� */
	  GsSortObject5(op,&Wot[outbuf_idx],14-OT_LENGTH,SCRATCH);
	  op++;
	}
	
      VSync(0);			/* Wait VSYNC : V�u�����N��҂� */
      padd=PadRead(1);		/* Readint Control Pad data :
				   �p�b�h�̃f�[�^��ǂݍ��� */
      GsSwapDispBuff();		/* Swap double buffer :
				   �_�u���o�b�t�@��ؑւ��� */
      
      /* Set SCREEN CLESR PACKET to top of OT :
	 ��ʂ̃N���A���I�[�_�����O�e�[�u���̍ŏ��ɓo�^���� */
      GsSortClear(0x0,0x0,0x0,&Wot[outbuf_idx]);
      
      /* Drawing OT :
	 �I�[�_�����O�e�[�u���ɓo�^����Ă���p�P�b�g�̕`����J�n���� */
      GsDrawOt(&Wot[outbuf_idx]);
    }
}


int obj_interactive()
{
  SVECTOR v1;
  MATRIX  tmp1;

/*===========================================================================*/ 
		/* set control data0 for MIMe value0 */
		if((padd & PADl)>0) ctlfc[0].in = ONE; else ctlfc[0].in = 0;
		/* set control data1 for MIMe value1 */
		if((padd & PADm)>0) ctlfc[1].in = ONE; else ctlfc[1].in = 0;
		/* set control data2 for MIMe value2 */
		if((padd & PADn)>0) ctlfc[2].in = ONE; else ctlfc[2].in = 0;
		/* set control data3 for MIMe value3 */
		if((padd & PADo)>0) ctlfc[3].in = ONE; else ctlfc[3].in = 0;
  		/* set control data4 for rotation of Y axis */
		if((padd & PADRleft)>0) 	 ctlfc[4].in =  ONE/64;
		else {  if((padd & PADRright)>0) ctlfc[4].in = -ONE/64; 
			else 			 ctlfc[4].in =      0; 
		}
  		/* set control data5 for rotation of X axis */
		if((padd & PADRup)>0) 		 ctlfc[5].in =  ONE/64;
		else { 	if((padd & PADRdown)>0)  ctlfc[5].in = -ONE/64;
			else			 ctlfc[5].in =      0; 
		}
  		/* set control data8 for motion along Z axis */
		if((padd & PADh)>0)		 ctlfc[8].in = -240;
		else {  if((padd & PADk)>0)	 ctlfc[8].in =  240; 
			else			 ctlfc[8].in =   0; 
		}
  		/* set control data6 for motion along X axis */
		if((padd & PADLleft)>0) 	 ctlfc[6].in =  180;
		else {  if((padd & PADLright)>0) ctlfc[6].in = -180; 
			else			 ctlfc[6].in =   0; 
		}
  		/* set control data7 for motion along Y axis */
		if((padd & PADLup)>0)		 ctlfc[7].in = -180;
		else {  if((padd & PADLdown)>0)	 ctlfc[7].in =  180;
			else			 ctlfc[7].in =   0; 
		}
/*===========================================================================*/ 
  if (((padd & PADk)>0)&&((padd & PADo)>0)&&((padd & PADm)>0)){
      PadStop();
      ResetGraph(3);
      StopCallback();
      return -1;
  }
 
  /* set matrix in the coordinate system  */
  set_coordinate(&PWorld,&DWorld);
  
  /* clear flag for recalcuration */
  DWorld.flg = 0;

  return 0;
}



/* Initialize routine : ���������[�`���Q */
init_all()
{
  ResetGraph(0);		/* Reset GPU : GPU���Z�b�g */
  padd=0;			/* Initialize controller value :
				   �R���g���[���l������ */

  GsInitGraph(640,480,GsOFSGPU|GsINTER,1,0);	/* rezolution set ,
						   interrace mode :
						   �𑜓x�ݒ�
						   �i�C���^�[���[�X���[�h�j */
    

  GsDefDispBuff(0,0,0,0);	/* Double buffer setting :
				   �_�u���o�b�t�@�w�� */
#if 0    
  GsInitGraph(320,240,1,0,0);	/* rezolution set , non interrace mode :
				   �𑜓x�ݒ�i�C���^�[���[�X���[�h�j */
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
}


view_init()			/* Setting view point : ���_�ݒ� */
{
  /*---- Set projection,view ----*/
  GsSetProjection(1000);	/* Set projection : �v���W�F�N�V�����ݒ� */
  
  /* Setting view point location : ���_�p�����[�^�ݒ� */
  view.vpx = 0; view.vpy = 0; view.vpz = 2000;
  
  /* Setting focus point location : �����_�p�����[�^�ݒ� */
  view.vrx = 0; view.vry = 0; view.vrz = 0;
  
  /* Setting bank of SCREEN : ���_�̔P��p�����[�^�ݒ� */
  view.rz=0;

  /* Setting parent of viewing coordinate : ���_���W�p�����[�^�ݒ� */
  view.super = WORLD;
  
  /* Calculate World-Screen Matrix from viewing paramter :
     ���_�p�����[�^���Q���王�_��ݒ肷��
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

texture_init(addr)
u_long addr;
{
  RECT rect1;
  GsIMAGE tim1;

  /* Get texture data type information from header of TIM data :
     TIM�f�[�^�̃w�b�_����e�N�X�`���̃f�[�^�^�C�v�̏��𓾂� */  
  GsGetTimInfo((u_long *)(addr+4),&tim1);
  
  rect1.x=tim1.px;		/* X coordinate (in VRAM) of upper-left of
				   texture :
				   �e�N�X�`�������VRAM�ł�X���W */
  rect1.y=tim1.py;		/* Y coordinate (in VRAM) of upper-left of
				   texture :
				   �e�N�X�`�������VRAM�ł�Y���W */
  rect1.w=tim1.pw;		/* Width of texture : �e�N�X�`���� */
  rect1.h=tim1.ph;		/* Height of texture : �e�N�X�`������ */
  
  /* Load texture to VRAM : VRAM�Ƀe�N�X�`�������[�h���� */
  LoadImage(&rect1,tim1.pixel);
  
  /* Exist Color Lookup Table : �J���[���b�N�A�b�v�e�[�u�������݂��� */  
  if((tim1.pmode>>3)&0x01)
    {
      rect1.x=tim1.cx;		/* X coordinate (in VRAM) of upper-left
				   of CLUT :
				   �N���b�g�����VRAM�ł�X���W */
      rect1.y=tim1.cy;		/* Y coordinate (in VRAM) of upper-left
				   of CLUT :
				   �N���b�g�����VRAM�ł�Y���W */
      rect1.w=tim1.cw;		/* Width of CLUT : �N���b�g�̕� */
      rect1.h=tim1.ch;		/* Height of CLUT : �N���b�g�̍��� */

      /* Load CULT data to VRAM : VRAM�ɃN���b�g�����[�h���� */
      LoadImage(&rect1,tim1.clut);
    }
}


/* Read modeling data (TMD FORMAT) : ���f�����O�f�[�^�̓ǂݍ��� */
model_init()
{
  u_long *dop;
  GsDOBJ5 *objp;		/* handler of object :
				   ���f�����O�f�[�^�n���h�� */
  int i;
  u_long *oppp;			/* packet area pointer */
  
  dop=(u_long *)MODEL_ADDR;	/* Top Address of MODELING DATA(TMD FORMAT) :
				   ���f�����O�f�[�^���i�[����Ă���A�h���X */
  dop++;			/* hedder skip */
  
  GsMapModelingData(dop);	/* Map modeling data (TMD format) to real
				   address :
				   ���f�����O�f�[�^�iTMD�t�H�[�}�b�g�j��
				   ���A�h���X�Ƀ}�b�v����*/

  dop++;
  Objnum = *dop;		/* Get number of Objects :
				   �I�u�W�F�N�g����TMD�̃w�b�_���瓾�� */

  dop++;			/* Bring to TMD object header in order to
				   link with GsLink Ojbect2 :
				   GsLinkObject2�Ń����N���邽�߂�TMD��
				   �I�u�W�F�N�g�̐擪�ɂ����Ă��� */

  /* Link ObjectHandler and TMD FORMAT MODELING DATA :
     TMD�f�[�^�ƃI�u�W�F�N�g�n���h����ڑ����� */
  for(i=0;i<Objnum;i++)
    GsLinkObject5((u_long)dop,&object[i],i);
  /* Top address of packet area :
     packet�̘g�g�����A�h���X */
  oppp = PacketArea;
  
  for(i=0,objp=object;i<Objnum;i++)
    {	
      /* Set Coordinate of Object Handler :
	 �t�H���g�̃I�u�W�F�N�g�̍��W�n�̐ݒ� */
      objp->coord2 =  &DWorld;
      
      objp->attribute = 0;	/* init attribute */
      
      /* The packet framework must be build beforhand for GsOBJ5-type objects :
	 GsDOBJ5�^�̃I�u�W�F�N�g�͗\�߃p�P�b�g�̘g�g�������
	 �����Ȃ���Ύg���Ȃ� */      
      oppp = GsPresetObject(objp,oppp);	
      
      objp++;
    }
}