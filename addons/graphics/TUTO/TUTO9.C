/* $PSLibId: Runtime Library Release 3.6$ */
/*			tuto9: cell type BG
 *
 *		 Version	Date		Design
 *		-----------------------------------------
 *		1.00		Jul,29,1994	suzu 
 *		2.00		May,22,1995	sachiko
 *
 *         Copyright (C) 1993-1995 by Sony Computer Entertainment
 * 			All rights Reserved
 *
 * Here is a sample program for coventional cell type BG estimation using
 * 8bit/4bit textures.
 * 8bit textured primitive is slower than 4bit textured primite.
 * this sample can draw 4-full-BGs in 1/60 sec using 8bit POLY_FT4 
 *
 * When you want to use special effect like magnify, rotate, and so 
 * on at the BG, you have to use POLY_FT4 instead of SPRT even though
 * POLY_FT4  is 2 times slower than SPRT.
 *
 * POLY_FT4 primitives are neccesary for magnified or rotated BG cells, 
 * but it is slower than SPRT. therefore if you do not need these functions,
 * you should use SPRT instead of POLY_FT4. Please define BG_SPRT when you
 * want to use SPRT primitevs as each BG cells.
 *
 :			�Z���^�C�v�� BG �̎��� 
 *
 *  	���̃v���O�����́A4bit/8bit �e�N�X�`�����g�p���� BG �̑��x�]��
 *	�v���O�����ł��B
 *	8bit �e�N�X�`���̏ꍇ�� 4bit �e�N�X�`���ɔ�ׂđ��x���ቺ���܂��B
 *	���̗�ł� POLY_FT4 �̃A���C���g�p���āA4 ���̃t���T�C�Y BG ��`
 *	�悵�Ă��܂��B
 *
 *	BG_SPRT �� define ����ƃX�v���C�g���g�p���� BG ��`�悵�܂��B
 *
 *	BG ����]�E�g�債�����ꍇ�� SPRT ���g�p�������� POLY_FT4
 *	���g�p����K�v������܂��BPOLY_FT4 �� SPRT �����`�摬�x���ቺ��
 *	�܂��B�]���Ă��̗�̗l�ɓ��{�̃}�b�s���O�݂̂��g�p����ꍇ�� SPRT
 *	�̕��������ł��B
 *
 *
 */
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

/* BG �Z���̒P�ʂ� 32x32 */
#define BG_CELLX	32
#define BG_CELLY	32

#define BG_WIDTH	288	/* max BG width: BG �̍ő啝 */
#define BG_HEIGHT	192	/* max BG height: BG �̍ő卂�� */

/* number of BG cells: �Z���̌��i�������j*/
#define BG_NX		(BG_WIDTH/BG_CELLX+2)
#define BG_NY		(BG_HEIGHT/BG_CELLY+2)

/* depth of OT: OT �̕���\�� 4 */
#define OTSIZE		4	

/* Define BG structure: BG �\���̂�V������`���� */
typedef struct {
	/* local DRAWENV for each BG: �e BG ���Ƃ̕`��� */
	DRAWENV		env;	
	
	/* local OT for each BG: �e BG ���Ƃ� OT */
	u_long		ot[OTSIZE];	
	
	SPRT		cell[BG_NY*BG_NX];	/* BG cells (SPRT) */
	POLY_FT4	cell2[BG_NY*BG_NX];	/* BG cells (POLY_FT4) */
} BG;

/*
 * primitive double buffer: �v���~�e�B�u�_�u���o�b�t�@
 */
#define MAXBG	4			/* max 4 BGs: �ő� BG �� */
typedef struct {
	DISPENV	disp;			/* display environment: �\���� */
	BG	bg[MAXBG];		/* BG */
} DB;

/* define this when you use SPRT BG
   : �X�v���C�g BG���g�p����ꍇ�i�|���S���̂Ƃ��́A�R�����g�A�E�g�j */
#define BG_SPRT

/* define this when you use 4bit texture
   : 4bit �e�N�X�`�����g�p����ꍇ�i8bit �̂Ƃ��̓R�����g�A�E�g�j */
#define BG_4bit

static void bg_init(BG *bg, int x, int y);
static void bg_update(BG *bg, SVECTOR *mapxy);
static void bg_draw(BG *bg);
static int pad_read(SVECTOR *mapxy);

main()
{
	/* primitive double buffer: �p�P�b�g�_�u���o�b�t�@�̎��� */
	DB	db[2];		
	
	/* current double buffer: �ݒ�p�P�b�g�o�b�t�@�փ|�C���^ */
	DB	*cdb;		
	
	/* current OT: �ݒ�p�P�b�g�n�s */
	u_long	*ot;		
	
	/* current location of BG: bg�̌��݂̃}�b�v�ʒu�i���͎g�p���Ȃ��j */
	SVECTOR	mapxy[4];	
	
	int	i;
	PadInit(0);		/* reset PAD: �R���g���[�������Z�b�g */
	ResetGraph(0);		/* reset GPU: �O���t�B�b�N�V�X�e�������Z�b�g */

	/* clear frame buffer: �t���[���_�u���o�b�t�@���N���A */
	{
		RECT rect;
		setRECT(&rect, 0, 0, 320, 480);
		ClearImage(&rect, 0, 0, 0);
	}

	/* define frame double buffer: �_�u���o�b�t�@�̒�` */
	/*	Buffer0   #0:	(  0   0) -(320,240)
	 *	Buffer1   #1:	(  0,240) -(320,480)
	 */
	SetDefDispEnv(&db[0].disp,  0, 240, 320, 240);
	SetDefDispEnv(&db[1].disp,  0,   0, 320, 240);

	/* Set Initial value of primitive buffer
	 * link primitive list for BG here.
	 : �v���~�e�B�u�o�b�t�@�̓��e�̏����l��ݒ�
	 * �����ŁABG �p�̃v���~�e�B�u���X�g�̃����N�܂Œ����Ă��܂�
	 */

	bg_init(&db[0].bg[0], 0, 4);		/* BG #0 */
	bg_init(&db[1].bg[0], 0, 4+240);

	bg_init(&db[0].bg[1], 20, 12);		/* BG #1 */
	bg_init(&db[1].bg[1], 20, 12+240);

	bg_init(&db[0].bg[2], 40, 20);		/* BG #2 */
	bg_init(&db[1].bg[2], 40, 20+240);

	bg_init(&db[0].bg[3], 60, 28);		/* BG #3 */
	bg_init(&db[1].bg[3], 60, 28+240);

	/* �}�b�v�̏����l��ݒ� */
	setVector(&mapxy[0],  0,  0,0);	/* default position (  0,  0) */
	setVector(&mapxy[1],256,256,0);	/* default position (256,256) */
	setVector(&mapxy[2],  0,256,0);	/* default position (  0,256) */
	setVector(&mapxy[3],256,  0,0);	/* default position (256,  0) */

	/* display enable: �\���J�n */
	SetDispMask(1);

	/* main loop: ���C�����[�v */
	while (pad_read(mapxy) == 0) {		/* mapxy ���l�� */

		/* swapt primitive buffer ID
		   : �ݒ�v���~�e�B�u�o�b�t�@�|�C���^���l������B*/
		cdb = (cdb==db)? db+1: db;

		/* update BG: BG ���X�V */
		for (i = 0; i < 4; i++)
			bg_update(&cdb->bg[i], &mapxy[i]);

		/* swap double buffer: �_�u���o�b�t�@���X���b�v */
		DrawSync(0);
		VSync(0);
		PutDispEnv(&cdb->disp); 

		/* draw 4 plane of BG: BG ���S�ʕ`�悷�� */
		for (i = 0; i < 4; i++)
			bg_draw(&cdb->bg[i]);
	}

	/* quit: �I�� */
        PadStop();		
	ResetGraph(1);
	StopCallback();
	return;
}

/***************************************************************************
 *
 *	BG management
 :	BG �n���h�����O
 *
 ***************************************************************************/
/* BG cell structure
 * This sample does not use clut member because it uses only 1 clut.
 * This sample does not use attr member because no attributes are defined here.
 * See bgmap.h for detail.
 : BG �Z���\����
 * ���̗�ł� CLUT �͂P�{�Ȃ̂� clut �����o�͎g�p���Ȃ��B
 * ���̗�ł� ������������Ȃ��̂� attr �����o�͎g�p���Ȃ��B
 * bgmap.h ���Q�� 
 */
typedef struct {
	u_char	u, v;	/* cell texture UV: �Z���e�N�X�`���p�^�[�����W */
	u_short	clut;	/* cell texture CLUT: �Z���e�N�X�`���p�^�[�� CLUT */
	u_long	attr;	/* attribute: �A�g���r���[�g */
} CTYPE;

/* 2D array for cell type: �Z���^�C�v���L�q�����Q�����z�� */
#include "bgmap.h"		

#ifdef BG_4bit
extern	u_long	bgtex[];	/* BG texture�ECLUT (4bit) */
#else
extern	u_long	bgtex8[];	/* BG texture�ECLUT (8bit) */
#endif

/*
 * initialize BG.
 * All parameters which would not be changed is set here.
 : BG �̏�����:���炩���߂ł��邱�Ƃ݂͂�Ȃ����Őݒ肵�Ă��܂��B
 */
/* BG	*bg,	BG data: BG �f�[�^ */
/* int	x,y	location on screen: �X�N���[����̕\���ʒu�i�w�j */
static void bg_init(BG *bg, int x, int y)
{
	SPRT		*cell;
	POLY_FT4	*cell2;
	u_short		clut, tpage;
	int		ix, iy;

	/* set drawing environment: �`�����ݒ� */
	SetDefDrawEnv(&bg->env, x, y, BG_WIDTH, BG_HEIGHT);

	/* load texture data and CLUT: �e�N�X�`���E�e�N�X�`�� CLUT �����[�h */

#ifdef BG_4bit	/* 4bit mode */
	tpage = LoadTPage(bgtex+0x80, 0, 0, 640, 0, 256, 256);
	clut  = LoadClut(bgtex, 0, 480);
#else		/* 8bit mode */
	tpage = LoadTPage(bgtex8+0x80, 1, 0, 640, 0, 256, 256);
	clut  = LoadClut(bgtex8, 0, 480);
#endif
	/* set default texture page: �f�t�H���g�e�N�X�`���y�[�W��ݒ� */
	bg->env.tpage = tpage;

	/* clear local OT: ���[�J���n�s���N���A */
	ClearOTag(bg->ot, OTSIZE);

	/* make primitive list for BG
	   : BG ���\������v���~�e�B�u���X�g���쐬����B*/

	for (cell = bg->cell, cell2 = bg->cell2, iy = 0; iy < BG_NY; iy++)
	    for (ix = 0; ix < BG_NX; ix++, cell++, cell2++) {

#ifdef BG_SPRT
		SetSprt(cell);			/* SPRT Primitive */
		SetShadeTex(cell, 1);		/* ShadeTex forbidden */
		setWH(cell, BG_CELLX, BG_CELLY);/* Set the sizes of the cell */
		cell->clut = clut;		/* Set texture CLUT  */
		AddPrim(&bg->ot[0], cell);	/* Put SPRT primitive into OT */
#else
		SetPolyFT4(cell2);		/* POLY_FT4 Primitive */
		SetShadeTex(cell2, 1);		/* ShadeTex forbidden */
		cell2->tpage = tpage;		/* Set texture pages*/
		cell2->clut  = clut;		/* Set texture CLUT*/
		AddPrim(&bg->ot[0], cell2);	/* Put POLY_FT4 primitives 
						   into OT  */
#endif
	    }
}

/* updatea BG members: BG �̃����o���X�V���� */
/* BG		*bg,	BG data: BG �f�[�^ */
/* SVECTOR	*mapxy	map location: �e�N�X�`���� BG �ւ̃}�b�v�ʒu */

static void bg_update(BG *bg, SVECTOR *mapxy)
{
	/* cell data for sprites: �X�v���C�g�p�Z���f�[�^ */
	SPRT		*cell;		
	
	/* cell data for polygon: �|���S���p�Z���f�[�^ */
	POLY_FT4	*cell2;		
	
	/* cell type: �Z���^�C�v */
	CTYPE		*ctype;		
	
	/* absolute postion on map: �}�b�v�̃I�t�Z�b�g�l */
	int		mx, my;		
	
	/* relative position on map: �}�b�v�̒��ł̃I�t�Z�b�g�l */
	int		dx, dy;		
	
	int		tx, ty;		/* work */
	int		x, y, ix, iy;

	/* (tx, ty) is rap-rounded at BG_CELLX*BG_MAPX 
	   : (tx, ty) ���݈ʒu�iBG_CELLX*BG_MAPX �Ń��b�v���E���h�j*/
	tx = (mapxy->vx)&(BG_CELLX*BG_MAPX-1);
	ty = (mapxy->vy)&(BG_CELLY*BG_MAPY-1);

	/*: tx �� BG_CELLX �Ŋ������l���}�b�v�̈ʒu (mx)
	 *  tx �� BG_CELLX �Ŋ������]�肪�\���ړ��� (dx)
	 */
	mx =  tx/BG_CELLX;	my =  ty/BG_CELLY;
	dx = -(tx%BG_CELLX);	dy = -(ty%BG_CELLY);

	/* update (u0,v0), (x0,y0) in the primitive list.
	   : �v���~�e�B�u���X�g�� (u0,v0), (x0,y0) �����o��ύX */

	cell  = bg->cell;
	cell2 = bg->cell2;
	for (iy = y = 0; iy < BG_NY; iy++, y += BG_CELLY) {
		for (ix = x = 0; ix < BG_NX; ix++, x += BG_CELLX) {

			/* rap-rounded at BG_MAPX, BG_MAPY 
			   :(BG_MAPX, BG_MAPY) �Ń��b�v���E���h */
			tx = (mx+ix)&(BG_MAPX-1);
			ty = (my+iy)&(BG_MAPY-1);

			/* get cell-type from the map data.
			 * Id code is stored  as a ASCII code in Map[][] 
			 : �}�b�v����Z���^�C�v���l�� 
			 * Map[][] �̓L�����N�^�R�[�h�� ID �������Ă��� 
			 */ 
			ctype = &CType[(Map[ty])[tx]-'0'];
#ifdef BG_SPRT
			/* to reduce the memory access, update only
			 * (u0,v0), (x0, y0) menbers
			 : (u0,v0), (x0, y0) �݂̂��X�V
			 */
			setUV0(cell, ctype->u, ctype->v);
			setXY0(cell, x+dx, y+dy);
#else
			/* to reduce the memory access, update only
			 *  (u0,v0), (x0, y0) menbers (POLY_FT4)
			 : (u,v), (x, y) ���X�V (POLY_FT4)
			 */
			setUVWH(cell2, ctype->u, ctype->v,
				BG_CELLX-1, BG_CELLY-1);
			setXYWH(cell2, x+dx, y+dy, BG_CELLX, BG_CELLY);
#endif
			cell++;
			cell2++;
		}
	}
}

/*
 * draw BG: BG ��`��
 */
static void bg_draw(BG *bg)
{
	/* Update Drawing environment for each BG
	   : BG ���Ƃɐݒ肵���`����ɍX�V */
	PutDrawEnv(&bg->env);	
	
	DrawOTag(bg->ot);	/* draw: �`�� */
}

/*
 * read controller: �R���g���[���̃f�[�^��ǂ�ŁA BG �̈ړ����������߂�B
 */
/* SVECTOR *mapxy	map location: �e�N�X�`���� BG �ւ̃}�b�v�ʒu */
static int pad_read(SVECTOR	*mapxy)
{
	static SVECTOR	v[4] = {
		 2, 0, 0, 0,	0,  2, 0, 0,
		-2, 0, 0, 0,	0, -2, 0, 0,
	};

	int	i,id;
	u_long	padd = PadRead(1);

	/* quit: �I�� */
	if(padd & PADselect) 	return(-1);

	id = 2;
	if (padd & (PADR1|PADL1|PADR2|PADL2)) id = 0;

	/* BG #0 �i#2 if PADR1 or PADL1 is pushed)
	   : BG #0 �i PADR1 �܂��� PADL1 ��������Ă����ꍇ�� BG #2 �j*/
	if(padd & PADLup)	setVector(&v[id],  0, -2, 0);
	if(padd & PADLdown)	setVector(&v[id],  0,  2, 0);
	if(padd & PADLleft)	setVector(&v[id], -2,  0, 0);
	if(padd & PADLright)	setVector(&v[id],  2,  0, 0);

	/* BG #1 �i#3 if PADR1 or PADL1 is pushed)
	   : BG #1 �i PADR1 �܂��� PADL1 ��������Ă����ꍇ�� BG #3 �j*/
	if(padd & PADRup)	setVector(&v[id+1],  0, -2, 0);
	if(padd & PADRdown)	setVector(&v[id+1],  0,  2, 0);
	if(padd & PADRleft)	setVector(&v[id+1], -2,  0, 0);
	if(padd & PADRright)	setVector(&v[id+1],  2,  0, 0);

	/* Add the motion range 
	   : ����̈ړ��ʂ����Z���� */
	for (i = 0; i < 4; i++)
		addVector(&mapxy[i], &v[i]);

	return(0);
}

