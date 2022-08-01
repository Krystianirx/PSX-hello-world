/* $PSLibId: Runtime Library Release 3.6$ */
/***********************************************
 *	
 *	"tuto1.c" main routine
 *
 *		Version 1.00	May. 29, 1995
 *
 *	Copyright (C) 1995 Sony Computer Entertainment Inc.
 *		All Rights Reserved.
 ***********************************************/
/*
 *	fog texture changing CLUT
 :	clut �ɂ��e�N�X�`���ւ̃t�H�O����
 */
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>


#define OT_LENGTH  10	/* resolution of OT: �I�[�_�����O�e�[�u���̉𑜓x */
#define PROJECTION 200

#define BGR 100					/* BG color Red */
#define BGG 100					/* BG color Green */
#define BGB 100					/* BG color Blue */

void draw_poly(GsCOORDINATE2 *co, int idx, GsOT_TAG *org, long *p);
int obj_interactive(u_long fn, u_long padd);
void main();

GsOT            Wot[2];	/* OT handler:  �I�[�_�����O�e�[�u���n���h�� */
GsOT_TAG	zsorttable[2][1<<OT_LENGTH]; /* OT: �I�[�_�����O�e�[�u������ */

POLY_FT4 ft4[2];				/* polygon */
SVECTOR fpos[4];				/* polygon vertex positions */
CVECTOR fcol;					/* polygon color */

GsCOORDINATE2   DWorld;		/* object coordinate: �I�u�W�F�N�g�̍��W�n */
SVECTOR PWorld;			/* rotation vector
				   : ���W�n����邽�߂̃��[�e�[�V�����x�N�^�[ */
extern MATRIX GsIDMATRIX;	/* Unit Matrix : �P�ʍs�� */

RECT rect1;
CVECTOR cv1[256];				/* original clut */
CVECTOR bgc;					/* background color */
short clutw;

int  outbuf_idx=0;

static unsigned short clut1[256];		/* clut with depth queued */
u_long *orgclut;
u_long fogclut[256];

static RECT rect2[2]={{100,225,256,1},{100,225+240,256,1}};
static RECT rect3[2]={{100,220,256,1},{100,220+240,256,1}};


/************* MAIN START ******************************************/
void main()
{
    u_long fn;
    u_long padd=0;		
    int i,j;
    long pvalue;

    ResetCallback();
    PadInit(0);			/* initialize controller: �R���g���[�������� */
    init_all();
/*    SetFogNear(8000, 5000);	/* set fog parameter */
    SetFogNearFar(1000, 20000, PROJECTION);	/* set fog parameter */
    bgc.r=BGR; bgc.g=BGG; bgc.b=BGB;

    /* main loop */
    for (fn=0;;fn++){
	outbuf_idx=GsGetActiveBuff();		/* get one of buffers */

	if (obj_interactive(fn, padd)<0) return; /* pad data handling */

	FntPrint("move along Z : L1, L2\n");

	GsClearOt(0,0,&Wot[outbuf_idx]);	/* clear ordering table */

	/* draw polygon(s) */
	draw_poly(&DWorld, outbuf_idx, Wot[outbuf_idx].org, &pvalue);

	padd=PadRead(1);			/* read pad */

	DrawSync(0);				/* wait drawing done */

	make_clut(pvalue);			/* make depth queued clut */

	VSync(0);				/* wait Vertical Sync */

	GsSwapDispBuff();			/* switch double buffers */
	
	/* BG auto clear enable
	   : ��ʂ̃N���A���I�[�_�����O�e�[�u���̍ŏ��ɓo�^���� */
	GsSortClear(BGR,BGG,BGB,&Wot[outbuf_idx]); 

	/* draw primitives on OT
	   : �I�[�_�����O�e�[�u���ɓo�^����Ă���p�P�b�g�̕`����J�n���� */
	GsDrawOt(&Wot[outbuf_idx]);
	FntFlush(-1);				/* draw print-stream */
	
	/* display depth queued CLUT: CLUTFOG �̂������� clut ����ʂɕ\�� */
	LoadImage(&rect2[outbuf_idx],(u_long *)fogclut);
	
	/* display orginal CLUT: ���� clut ����ʂɕ\�� */
	LoadImage(&rect3[outbuf_idx],(u_long *)orgclut);
    }
    return;
}

/* draw polygon(s) */
void draw_poly(GsCOORDINATE2 *co, int idx, GsOT_TAG *org, long *p)
{
    int i;
    long otz, flag;
    MATRIX tmpls;
    POLY_FT4 *f;
    long nclip;

    /* update coordinate and set to GTE: ���W���v�Z���� GTE �ɃZ�b�g */
    GsGetLs(co, &tmpls);
    GsSetLsMatrix(&tmpls);

    f= &ft4[idx];
    otz=RotAverage4(&fpos[0], &fpos[1], &fpos[2], &fpos[3],
			   (long *)(&f->x0),
			   (long *)(&f->x1),
			   (long *)(&f->x2),
			   (long *)(&f->x3),
			   p, &flag
			   );
    FntPrint("p=%d\n",*p);
    if (flag >=0				/* no error */
	&& otz>0)				/* otz OK */
	AddPrim(org+(otz>>(14-OT_LENGTH)), f); /* set f to OT */
}

/* make depth queued clut */
make_clut(long p)
{
    int i;
    CVECTOR newc;

    for (i=0; i<clutw; i++){	/*  8bit CLUT = 256 colors*/
	if (cv1[i].cd&0x80){	/* skip if transparent: �����Ȃ���}���Ȃ� */
	    clut1[i]=0;
	} else{
	    /* interpolate between orginal color and background color:
	       ���F�Ɣw�i�F�̓��}�v�Z (GTE �̊֐����p) */
	    LoadAverageCol((u_char *)(&cv1[i]),
			   (u_char *)(&bgc), (4096-p), p, (u_char *)(&newc));
	    
/*	    FntPrint("LAC:(%d,%d)->%d\n",cv1[i].b, bgc.b, newc.b);*/

	    /* make depth queued CLUT: FOG �̂������� CLUT ���쐬 */
	    if (cv1[i].cd&0x80){	/* skip if transparent: �����̂Ƃ� */
		cv1[i].cd=0;
	    } else{
		clut1[i]= (cv1[i].cd<<15)
		    | (((u_long)newc.r&0xf8)>>3)
		    | (((u_long)newc.g&0xf8)<<2)
		    | (((u_long)newc.b&0xf8)<<7);
	    }
	}
    }
    /* load depth queued CLUT: FOG �̂������� clut �� VRAM �ɓ]�� */
    LoadImage(&rect1,(u_long *)clut1);
    
    /* copy to drawing area: �`��p�o�b�t�@�ɃR�s�[ */
    bcopy(clut1, fogclut, clutw*sizeof(u_short));

}

/* pad data handling */
int obj_interactive(u_long fn, u_long padd)
{
#define DT 30
#define VT 30
#define ZT 30
    static u_long oldpadd=0;

    if (padd & PADk){			/* change PICTURE AREA with PADk */
    } else if (padd & PADh){
    } else{				/* translate and rotate */
	if (padd & PADLleft) DWorld.coord.t[0]-=DT;
	else if (padd & PADLright) DWorld.coord.t[0]+=DT;
	
	if (padd & PADLup) DWorld.coord.t[1]-=DT;
	else if (padd & PADLdown) DWorld.coord.t[1]+=DT;
	
	if (padd & PADo) DWorld.coord.t[2]+=ZT;
	else if (padd & PADn) DWorld.coord.t[2]-=ZT;
	
#if 0
	if (padd & PADRleft) PWorld.vy-=VT;
	else if (padd & PADRright) PWorld.vy+=VT;
	
	if (padd & PADRup) PWorld.vx+=VT;
	else if (padd & PADRdown) PWorld.vx-=VT;
	
	if (padd & PADm) PWorld.vz-=VT;
	else if (padd & PADl) PWorld.vz+=VT;
#endif /* 0 */
    }

    if (padd & PADselect){
	/* �v���O�����I�� */
	PadStop();
	ResetGraph(3);
	StopCallback();
	return -1;
    }

    set_coordinate(&PWorld,&DWorld);
    oldpadd=padd;
    return 0;
}

init_all()			/* initialize: ���������[�`���Q */
{
    ResetGraph(0);		/* initialize GPU: GPU���Z�b�g */

    draw_init();
    coord_init();
    view_init();
    fnt_init();
    poly_init();
    texture_init((u_long *)0x80100000);
}

poly_init()
{
    setPolyFT4(&ft4[0]);			/* initialize primitive */
    SetShadeTex(&ft4[0],1);			/* shading off */
    fcol.r=128;					/* set color */
    fcol.g=128;
    fcol.b=128;
    fcol.cd=ft4[0].code;
    setRGB0(&ft4[0], fcol.r, fcol.g, fcol.b);	/* set polygon color */
    setXY4(&ft4[0], -100, -100, 100, -100, -100, 100, 100, 100); /* set position */
    setUV4(&ft4[0], 0,0,255,0, 0, 255, 255, 255); /* set texture position */
    ft4[0].clut=GetClut(0, 480);		/* clut */
    ft4[0].tpage=GetTPage(1,0,640,0);		/* texture page */
    ft4[1]=ft4[0];				/* copy data to the other ft4*/

#define SIDELONG 500
    /* set vertex positions */
    fpos[0].vx= -SIDELONG;
    fpos[0].vy= -SIDELONG;
    fpos[0].vz= 0;
    fpos[1].vx= SIDELONG;
    fpos[1].vy= -SIDELONG;
    fpos[1].vz= 0;
    fpos[2].vx= -SIDELONG;
    fpos[2].vy= SIDELONG;
    fpos[2].vz= 0;
    fpos[3].vx= SIDELONG;
    fpos[3].vy= SIDELONG;
    fpos[3].vz= 0;

}

draw_init()
{
    /* set resolution (interlace mode): �𑜓x�ݒ�i�C���^�[���[�X���[�h�j */
    GsInitGraph(640,240,GsOFSGPU|GsINTER,1,0);	
    
    /* initialize double buffer: �_�u���o�b�t�@�w�� */
    GsDefDispBuff(0,0,0,240);	
    
    /* reset 3D system: �RD�V�X�e�������� */
    GsInit3D();			

    /* set OT resolution: �I�[�_�����O�e�[�u���n���h���ɉ𑜓x�ݒ� */
    Wot[0].length=OT_LENGTH;	
    Wot[1].length=OT_LENGTH;
    
    /* set OT on OT handler(0): �n�s�n���h���ɂn�s�̎��̐ݒ� */
    Wot[0].org=zsorttable[0];	
    Wot[1].org=zsorttable[1];
}

fnt_init()
{
    FntLoad(960, 256);				/* font load */
    SetDumpFnt(FntOpen(-290,-100,400,100,0,200)); /* stream open & define */
}

int view_init()
{
    GsRVIEW2  view;		/* View Point Handler*/

    /* Set projection : �v���W�F�N�V�����ݒ� */
    GsSetProjection(PROJECTION);
  
    /* Setting view point location : ���_�p�����[�^�ݒ� */
    view.vpx = 0; view.vpy = 0; view.vpz = -2000;
  
    /* Setting focus point location : �����_�p�����[�^�ݒ� */
    view.vrx = 0; view.vry = 0; view.vrz = 0;
  
    /* Setting bank of SCREEN : ���_�̔P��p�����[�^�ݒ� */
    view.rz=0;

    /* Setting parent of viewing coordinate : ���_���W�p�����[�^�ݒ� */
    view.super = WORLD;
  
    /* Calculate World-Screen Matrix from viewing paramter
       : ���_�p�����[�^���Q���王�_��ݒ肷��*/
    GsSetRefView2(&view);
  
    /* Set Near Clip: �j�A�N���b�v�ݒ� */
    GsSetNearClip(100);           
}


int coord_init()
{
    /* initialize coordinates: ���W�̒�` */
    GsInitCoordinate2(WORLD,&DWorld);
    DWorld.coord.t[2]= 0;

    /* set the rotation vector for rot-trans matrix:
       �}�g���b�N�X�v�Z���[�N�̃��[�e�[�V�����x�N�^�[������ */
    PWorld.vx=PWorld.vy=PWorld.vz=0;
    set_coordinate(&PWorld,&DWorld);
}

/* make rot-trans matrix from the rotation vector:
   ���[�e�V�����x�N�^����}�g���b�N�X���쐬�����W�n�ɃZ�b�g���� */
int set_coordinate(SVECTOR *pos, GsCOORDINATE2 *coor)
{
    MATRIX tmp1;
    SVECTOR v1;
    int i,j;

    /* start from th unit matrix: �P�ʍs�񂩂�o������ */
    tmp1   = GsIDMATRIX;	

    /* set translation vectro: ���s�ړ����Z�b�g���� */
    tmp1.t[0] = coor->coord.t[0];
    tmp1.t[1] = coor->coord.t[1];
    tmp1.t[2] = coor->coord.t[2];

    /* make rotaion matrix and hook to object handler
     : �}�g���b�N�X�Ƀ��[�e�[�V�����x�N�^����p������ 
     * ���߂��}�g���b�N�X�����W�n�ɃZ�b�g����
     */
    v1 = *pos;
    RotMatrix(&v1,&tmp1);
    coor->coord = tmp1;

    /* flush matrix cache
     : �}�g���b�N�X�L���b�V�����t���b�V������
     */
    coor->flg = 0;
}


texture_init(u_long *addr)
{
    int i;
    GsIMAGE tim1;
    unsigned short *clut;

    /* Get texture information of TIM FORMAT/
       : TIM�f�[�^�̃w�b�_����e�N�X�`���̃f�[�^�^�C�v�̏��𓾂� */  
    GsGetTimInfo(addr+1,&tim1);
  
    rect1.x=tim1.px;		/* X point of image data on VRAM
				   : �e�N�X�`�������VRAM�ł�X���W */
    rect1.y=tim1.py;		/* Y point of image data on VRAM
				   : �e�N�X�`�������VRAM�ł�Y���W */
    rect1.w=tim1.pw;		/* Width of image : �e�N�X�`���� */
    rect1.h=tim1.ph;		/* Height of image : �e�N�X�`������ */
  
    /* Load texture to VRAM : VRAM�Ƀe�N�X�`�������[�h���� */
    LoadImage(&rect1,tim1.pixel);
  
    /* Exist Color Lookup Table : �J���[���b�N�A�b�v�e�[�u�������݂��� */  
    if ((tim1.pmode>>3)&0x01){
	rect1.x=tim1.cx;		/* X point of CLUT data on VRAM
					   : �N���b�g�����VRAM�ł�X���W */
	rect1.y=tim1.cy;		/* Y point of CLUT data on VRAM
					   : �N���b�g�����VRAM�ł�Y���W */
	rect1.w=tim1.cw;		/* Width of CLUT : �N���b�g�̕� */
	rect1.h=tim1.ch;		/* Height of CLUT : �N���b�g�̍��� */

        /* Load CULT data to VRAM : VRAM�ɃN���b�g�����[�h���� */
	LoadImage(&rect1,tim1.clut);

	clut=(unsigned short *)tim1.clut;
	clutw=tim1.cw;
	orgclut=tim1.clut;
	for (i=0; i<tim1.cw; i++){
	    cv1[i].r=(((u_long)clut[i])<<3)&0xf8;
	    cv1[i].g=(((u_long)clut[i])>>2)&0xf8;
	    cv1[i].b=(((u_long)clut[i])>>7)&0xf8;
	    /* if not transparent: �����ȊO�̂Ƃ� */
	    if (cv1[i].r|cv1[i].g|cv1[i].b){	
		cv1[i].cd= (clut[i] & 0x8000)>>15;
		/* if semi-transparent,  cd=1: �������̂Ƃ� cd=1 */
		/* if non-transparent,   cd=0: �������̂Ƃ� cd=1 */
	    } else{
		/* if transparent: �����̂Ƃ� cd=0x80 */
		cv1[i].cd=0x80;			
	    }
	}
	    
    }
}