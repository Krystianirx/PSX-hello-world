/* $PSLibId: Runtime Library Release 3.6$ */
/* 			tuto8: 1D scroll
 *
 *         Copyright (C) 1993-1995 by Sony Computer Entertainment
 *			All rights Reserved
 *
 *		    Sample of 1-D BG scroll
 :		   1D �X�N���[�� BG �̎���
 */
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

/* depth of OT is 8: �I�[�_�����O�e�[�u���̐[���́A�W */
#define OTSIZE		8

/* 
 * Sprite primiteives have no texture page member. 
 * We define new primitive type which has a texture pages.
 : �ʏ�̃X�v���C�g�́A�e�N�X�`���y�[�W�������Ȃ��̂ŁA�����ŁA
 * �V�����e�N�X�`���y�[�W�t���X�v���C�g���`����B
 */
typedef struct {
	DR_MODE	mode;	/* mode primitive: ���[�h�ݒ�v���~�e�B�u */
	SPRT	sprt;	/* sprite primitive: �X�v���C�g �v���~�e�B�u */
} TSPRT;

/* Structures which contains a information about texture location.
   : �e�N�X�`���֌W�̏����W�߂��\���� */

typedef struct {
	/* texture address on main memory: �e�N�X�`���p�^�[��������A�h���X */
	u_long	*addr;	
	
	/* texture addres on frame buffer: �e�N�X�`���p�^�[�������[�h����ꏊ*/
	short	tx, ty;	
	
	/* texture address on frame buffer: �e�N�X�`�� CLUT �����[�h����ꏊ */
	short	cx, cy;	
	
	/* texture page ID: �e�N�X�`���y�[�W ID �i�ォ��v�Z����j*/
	u_short	tpage;	
	
	/* texture CLUT ID: �e�N�X�`�� CLUT ID �i�ォ��v�Z����j*/
	u_short	clut;	
} TEX;

/* primitive double buffer :�v���~�e�B�u�_�u���o�b�t�@ */
typedef struct {
	DRAWENV		draw;		/* drawing environment: �`��� */
	DISPENV		disp;		/* display environment: �\���� */
	u_long		ot[OTSIZE];	/* OT: �I�[�_�����O�e�[�u�� */
	TSPRT		far0, far1;	/* far mountains: �����̎R�X */
	TSPRT		near0, near1;	/* near mountains: �߂��̎R�X */
	TSPRT		window;		/* train window: �d�Ԃ̑� */
} DB;

static int update(DB *db,int padd);
static void add_sprite(u_long *ot,TSPRT *tsprt,
			int x0,int y0,int u0,int v0,int w,int h,TEX *tex);
static int SetTSprt(TSPRT *tsprt,int dfe,int dtd,int tpage,RECT *tw);

main()
{
	/* packet double buffer: �p�P�b�g�_�u���o�b�t�@ */
	DB		db[2];		
							  
	/* current burrer: ���݂̃o�b�t�@�A�h���X */
	DB		*cdb;		
	
	/* controller data: �R���g���[���p�b�h�̃f�[�^ */
	int	padd;		

	/* reset controller: �R���g���[���̃��Z�b�g */
	PadInit(0);             
	
	/* reset graphic system: �`��E�\�����̃��Z�b�g */
	ResetGraph(0);		
	
	/* set debug mode: �f�o�b�O���[�h�̐ݒ� */
	SetGraphDebug(0);	

	/* �e�N�X�`���f�[�^���t���[���o�b�t�@�ɓ]�� */
	load_tex();		

	/* set double buffer environment: �_�u���o�b�t�@�p�̊��ݒ� */
	/*	buffer #0:	(0,  0)-(255,239) (256x240)
	 *	buffer #1:	(0,240)-(255,479) (256x240)
	*/
	SetDefDrawEnv(&db[0].draw, 0,   0, 256, 240);
	SetDefDrawEnv(&db[1].draw, 0, 240, 256, 240);
	SetDefDispEnv(&db[0].disp, 0, 240, 256, 240);
	SetDefDispEnv(&db[1].disp, 0,   0, 256, 240);
	db[0].draw.isbg = db[1].draw.isbg = 1;

	SetDispMask(1);		/* �\���J�n */

	while (((padd = PadRead(1)) & PADselect) == 0) {

		/* swap double buffer ID: db[0], db[1] ������ */
		cdb = (cdb==db)? db+1: db;	

		/* update coordinates: ���W���A�b�v�f�[�g */
		update(cdb, padd);		

		DrawSync(0);	/* wait for end of drawing: �`��̏I����҂� */
		VSync(0);	/* wait for VBLNK: ����������҂� */

		/* swap double buffer: �_�u���o�b�t�@�̐ؑւ� */
		PutDrawEnv(&cdb->draw);	/* update DRAWENV: �`����̍X�V */
		PutDispEnv(&cdb->disp);	/* update DISPENV: �\�����̍X�V */
		DrawOTag(cdb->ot);	/* draw: �`�� */
	}

	/* quit: �I�� */
	PadStop();	
	ResetGraph(1);
	StopCallback();
	return;
}

/* 
 * 5 8bit texture pages are used here.
 * Both Polygons used for far-landscape and close-landscape use 256x256
 * These pages are used for far landscae, near landescape and train window.
 * 2 pages (512x256) are used for each landscape. 1 page is used for
 * train window.
 * Each landscape is rap-rounded by 512 pixel.
 * Details of the sprite is as follows:
 : �e�N�X�`���y�[�W�͂T���g�p�i8bit ���[�h)
 * ���i�A�ߌi�Ƃ��A256x256 ���Q�����ɂȂ�ׂĂ��̈ꕔ��\������B
 * �i�F�̓��b�v���E���h����B
 * �e�y�[�W�̓��e�͈ȉ��̒ʂ�
 */
extern u_long far0[];		/* far mountains(left):   �����̎R�X(����)*/
extern u_long far1[];		/* far mountains(right):  �����̎R�X(�E��)*/
extern u_long near0[];		/* near mountains(left):  �߂��̎R�X(����)*/
extern u_long near1[];		/* near mountains(right): �߂��̎R�X(�E��)*/
extern u_long window[];		/* train window: �D�Ԃ̑� */

/* define ID to every textures: �e�e�N�X�`���ɒʂ��ԍ����`����B*/
#define TEX_FAR0	0
#define TEX_FAR1	1
#define TEX_NEAR0	2
#define TEX_NEAR1	3
#define TEX_WINDOW	4

/* Initial value of the texture structure: �e�N�X�`���\���̂̏����l */
static TEX tex[] = {
	/*addr   tx   ty cx   cy tpage clut			*/
	/*--------------------------------------------------------------*/ 
	far0,   512,   0, 0, 481,    0,   0,	/* far0    */ 
	far1,   512, 256, 0, 482,    0,   0,	/* far1    */ 
	near0,  640,   0, 0, 483,    0,   0,	/* near0   */ 
	near1,  640, 256, 0, 484,    0,   0,	/* near1   */ 
	window, 768,   0, 0, 485,    0,   0,	/* window  */ 
	0,					/* terminator: �I�[ */
};

/* Load all texture data to V-RAM at once.
   : �e�N�X�`���y�[�W���܂Ƃ߂ăt���[���o�b�t�@�֓]������B*/
load_tex(void)
{
	int	i;

	/* loop while addr is not 0 : 'addr' �� 0 �łȂ����胋�[�v */
	for (i = 0; tex[i].addr; i++) {	

		/* Load texture pattern : �e�N�X�`���p�^�[�������[�h */
		tex[i].tpage = LoadTPage(tex[i].addr+0x80, 1, 0,
					 tex[i].tx, tex[i].ty,  256, 256);
		
		/* Load texture clut : �e�N�X�`�� CLUT �����[�h���� */
		tex[i].clut = LoadClut(tex[i].addr, tex[i].cx, tex[i].cy);
	}
}

/* Read controller and update the scroll parameters*
   : �R���g���[���p�b�h��ǂ�ŃX�N���[���̃p�����[�^�����肷��B*/

/* DB	*db,	primitive buffer: �v���~�e�B�u�o�b�t�@ */
/* int	padd	controller data:  �R���g���[���̃f�[�^ */
static int update(DB *db, int padd)
{
	
	static int 	ofs_x = 0;	/* position: ���i�̈ړ��� */
	static int	v     = 0;	/* verosicy: �ړ����x */
	
	/* texture: �e�N�X�`���f�[�^ */
	TEX	*tp[2];			
	
	/* direction 0:right to left, 1:reverse: �ړ����� 0:�E����, 1:�����E */
	int	side;			
	
	/* flag to disaplay near mountains: �ߌi�\�� 1:����, else:���Ȃ� */
	int	isnear   = 1;		
	
	/* flag to display window:  ���̕\�� 1:����, else:���Ȃ� */
	int	iswindow = 1;		
	
	int	d;

	/* update speed: �R���g���[���̉�� */
	if (padd & PADLright)	v++;	/* speed up: �X�s�[�h�A�b�v */
	if (padd & PADLleft)	v--;	/* speed down: �X�s�[�h�_�E�� */
	ofs_x += v;			/* total amount: �ړ��ʂ̗ݐς��Ƃ� */

	/* if needed, it is surpressed to display landscapes or a window.  
	   : �ꍇ�ɂ���ẮA�ߌi�A�D�Ԃ̑��̕\�����֎~����B*/
	if (padd & PADR1)	isnear   = 0;
	if (padd & PADL1)	iswindow = 0;

	/* clear OT: �n�s�̃N���A */
	ClearOTag(db->ot, OTSIZE);

	/* display far landscape. (The period of rap-round is 512 pixel) 
	   : �����̎R�X��\������B�i�F�͉� 512 �Ń��b�v���E���h */
    display_far:

	side = (ofs_x/4)&0x100;	/* decide L->R or R->L: �\������������ */
	d    = (ofs_x/4)&0x0ff;	/* limited between 0-256:
				   �ړ��ʂ� 256 �Ő��K�� */

	/* Put TSPRT primite into OT.
	 * The scroll is achived by changing (u0,v0) not (x0, y0).
	 * Left primitive draws the area of (0,0)-(256-d,240)
	 * Right primitive draws the area of (256-d,0)-(256,240)
	 * 'd' is valid according to the  scroll position.
	 *
	 : TSPRT �� �n�s�ɓo�^����֐����R�[������ 
	 * x0, y0 �ł͂Ȃ��Au0, v0 �𓮂����ăX�N���[�����s�Ȃ�
	 * �E�̈ꖇ�́A��� (256-d,0)-(256,240) ��S������
	 * ���̈ꖇ�́A��� (0, 0)-(256-d,240) ��S������B
	 */
	tp[0] = &tex[side==0? TEX_FAR0: TEX_FAR1];	/* right hand */
	tp[1] = &tex[side==0? TEX_FAR1: TEX_FAR0];	/* left hand */

	add_sprite(db->ot, &db->far0, 0,     16, d, 0, 256-d, 168, tp[0]);
	add_sprite(db->ot, &db->far1, 256-d, 16, 0, 0, d,     168, tp[1]);

	/* Display near landscape.( The cycle of lap-round is 512 pixel  
	   : �߂��̎R�X��\������B�i�F�͉� 512 �Ń��b�v���E���h */
    display_near:
	if (isnear == 0) goto display_window;

	side = (ofs_x/2)&0x100;	
	d    = (ofs_x/2)&0x0ff;

	tp[0] = &tex[side==0? TEX_NEAR0: TEX_NEAR1];
	tp[1] = &tex[side==0? TEX_NEAR1: TEX_NEAR0];

	add_sprite(db->ot+1, &db->near0, 0,     32, d, 0, 256-d, 168, tp[0]);
	add_sprite(db->ot+1, &db->near1, 256-d, 32, 0, 0, d,     168, tp[1]);

	/* draw window: ����`�� */
    display_window:
	if (iswindow == 0) return;

	add_sprite(db->ot+2,
		   &db->window, 0, 0, 0, 0, 256, 200, &tex[TEX_WINDOW]);
}

/* Put TSPR primitive into OT : TSPRT ���n�s�ɓo�^���� */
/* u_long *ot,	  OT: OT */
/* TSPRT  *tsprt, TSPRT primitive: �o�^����e�N�X�`���X�v���C�g�v���~�e�B�u */
/* int	  x0,y0	  left-upper corner of TSPRT: �X�v���C�g�̍�����W�l */
/* int	  u0,v0	  left-upper corner of TSPRT(U): �e�N�X�`���̍�����W�l�i���j*/
/* int	  w,h	  size of texture pattern: �e�N�X�`���̃T�C�Y*/
/* TEX	  *tex	  texture attributes: �e�N�X�`���f�[�^ */

static void add_sprite(u_long *ot,TSPRT *tsprt,
			int x0,int y0,int u0,int v0,int w,int h,TEX *tex)
{
	/* initialize TSPRT primitive: TSPRT �v���~�e�B�u�������� */
	SetTSprt(tsprt, 1, 1, tex->tpage, 0);

	/* shading off: �V�F�[�f�B���O�I�t */
	SetShadeTex(&tsprt->sprt, 1);	
	
	/* set left-upper corner point of (x,y): �X�v���C�g�̍�����W�l */
	setXY0(&tsprt->sprt, x0, y0);	
	
	/* set left-upper corner point of (u,v): �e�N�X�`���̍�����W�l */
	setUV0(&tsprt->sprt, u0, v0);	
	
	/* set size of sprite: �X�v���C�g�̃T�C�Y */
	setWH(&tsprt->sprt, w, h);	
	
	/* set CLUT ID: �e�N�X�`��CLUT ID */
	tsprt->sprt.clut = tex->clut;	

	/* add to OT: OT �ɓo�^ */
	AddPrim(ot, &tsprt->mode);	
}

/* TSPRT *tsprt, TSPRT pointer: �l���Z�b�g���� TSPRT �\���̂ւ̃|�C���^ */
/* int	 dfe,	 same as DRAWENV.dfe: �\���̈�ւ̕`��  0:�s�A1:�� */
/* int	 dtd,	 same as DRAWENV.dtd: �f�B�U  0:�Ȃ��A1:���� */
/* int	 tpage,	 texture page: �e�N�X�`���y�[�W */
/* RECT	 *tw	 texture window: �e�N�X�`���E�B���h�E */

static int SetTSprt(TSPRT *tsprt,int dfe,int dtd,int tpage,RECT *tw)
{
	/* initialize MODE primitive: MODE �v���~�e�B�u�������� */
 	SetDrawMode(&tsprt->mode, dfe, dtd, tpage, tw);

	/* initialize SPRT primitive: SPRT �v���~�e�B�u�������� */
	SetSprt(&tsprt->sprt);

	/* Merge 2 primitives to one.
	 * Merging primitives should be located on the continuous
	 * memory area.
	 * The total size of 2 primitives must be under 16 words.
	 : �Q�̃v���~�e�B�u���}�[�W����B
	 * �}�[�W����v���~�e�B�u�́A��������̘A�������̈�ɒu����Ȃ���
	 *  �͂����Ȃ��B�}�[�W����v���~�e�B�u�̃T�C�Y�� 16 ���[�h�ȉ�
	 */
	if (MargePrim(&tsprt->mode, &tsprt->sprt) != 0) {
		printf("Marge failed!\n");
		return(-1);
	}
	return(0);
}
