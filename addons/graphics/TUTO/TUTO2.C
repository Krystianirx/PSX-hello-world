/* $PSLibId: Runtime Library Release 3.6$ */
/*			tuto2: RotTransPers
 *
 *         Copyright (C) 1993-1995 by Sony Computer Entertainment
 *			All rights Reserved
 *
 *		  Draw rotating plate using GTE functions
 :		  ��]����P���̔� GTE ���g���ĕ`�悷��
 */
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

#define SCR_Z	512		/* distant to screen: �X�N���[���̐[�� */

/*#define DEBUG*/

/* primitive buffer: �v���~�e�B�u�o�b�t�@ */
typedef struct {
	DRAWENV		draw;		/* drawing environment: �`��� */
	DISPENV		disp;		/* display environment: �\���� */
	u_long		ot;		/* ordering table: �n�s */
	POLY_G4		wall;		/* primitive: �|���S���v���~�e�B�u */
} DB;

static int pad_read(void);
static void init_prim(DB *db);

main()
{
	SVECTOR	x[4];		/* walls position: 'wall'�̍��W�l */
	DB	db[2];		/* primitive double buffer
				   : �v���~�e�B�u�_�u���o�b�t�@ */
	DB	*cdb;		/* current DB
				   : �_�u���o�b�t�@�̂������݂̃o�b�t�@ */
	long	dmy, flg;	/* work */
	int	i;

	/* initialize wall position: 'wall'�̃X�^�[�g���W�l�̐ݒ� */
	setVector(&x[0], -128, -128, 0); setVector(&x[1],  128, -128, 0);
	setVector(&x[2], -128,  128, 0); setVector(&x[3],  128,  128, 0);

	/* initialize environment for double buffer 
	   : �`��E�\�������_�u���o�b�t�@�p�ɐݒ肷��*/
	/*	buffer #0:	(0,  0)-(320,240) (320x240)
	 *	buffer #1:	(0,240)-(320,480) (320x240)
	 */
	init_system(160, 120, SCR_Z, 0);
	SetDefDrawEnv(&db[0].draw, 0,   0, 320, 240);
	SetDefDrawEnv(&db[1].draw, 0, 240, 320, 240);
	SetDefDispEnv(&db[0].disp, 0, 240, 320, 240);
	SetDefDispEnv(&db[1].disp, 0,   0, 320, 240);

	/* init primitives: �v���~�e�B�u�̏����ݒ� */
	init_prim(&db[0]);
	init_prim(&db[1]);

	/* display: �\���J�n */
	SetDispMask(1);

	/* main loop: ���C�����[�v */
	while (pad_read() == 0) {

		FntPrint("tuto2: use GTE\n");

		/* swap double buffer ID: �_�u���o�b�t�@�̌��� */
		cdb = (cdb==db)? db+1: db;	

		/* clea ordering table: �n�s�̃N���A */
		ClearOTag(&cdb->ot, 1);		

		/* rotate and shift each vertex of the wall
		 : ���[�J�����W����X�N���[�����W�ցA���W�ϊ��Ɠ����ϊ����s���B
		 * �ϊ��}�g���b�N�X�́Apad_read()���Őݒ�B
		 */
                RotTransPers(&x[0], (long *)&cdb->wall.x0, &dmy, &flg);
                RotTransPers(&x[1], (long *)&cdb->wall.x1, &dmy, &flg);
                RotTransPers(&x[2], (long *)&cdb->wall.x2, &dmy, &flg);
                RotTransPers(&x[3], (long *)&cdb->wall.x3, &dmy, &flg);

#ifdef DEBUG
		dumpXY4(&cdb->wall);
#endif
		/*: �n�s�ɓo�^ */
		AddPrim(&cdb->ot, &cdb->wall);		

		DrawSync(0);	/* wait for end of drawing: �`��̏I����҂� */
		VSync(0);	/* wait for the next VBLNK: ����������҂� */

		/* swap double buffer
		  ; �_�u���o�b�t�@�̎��ۂ̐؂�ւ��͂����ōs���� */
		
		PutDrawEnv(&cdb->draw); /* update DRAWENV: �`����̍X�V */
		PutDispEnv(&cdb->disp); /* update DISPENV: �\�����̍X�V */
#ifdef DEBUG
		DumpOTag(&cdb->ot);
#endif
		DrawOTag(&cdb->ot);	/* draw: �`�悷�� */
		FntFlush(-1);		/* flush print buffer */
	}
        PadStop();	
	ResetGraph(1);
	StopCallback();
	return;
}

/* analyzie controller and set matrix for rotation and translation.
 : �R���g���[���̉�͂����āA��]�E���s�ړ��̃}�g���b�N�X��ݒ肷��B
 */
static int pad_read(void)
{
	/* Because PlayStation treats angles like that :360��= 4096,
	 * you need to set angles as follows:
	 : Play Station �ł́A�p�x�� 360��= 4096 �ň����܂��B
	 * ���������āA�p�x�̐ݒ������ꍇ�́A�ȉ��̂悤�ɂȂ�B
	 */
	/*	 45��=  512
	 *	 90��= 1024
	 *	180��= 2048
	 */
	/* rotation angle: ��]�p�x */
	static SVECTOR	ang   = {512, 512, 512};	
	
	/* translate vector: ���s�ړ��x�N�g�� */
	static VECTOR	vec   = {0,     0, SCR_Z};	
	
	/* work matrix */
	static MATRIX	m;				

	int	ret = 0;

	/* get controller status: �R���g���[������f�[�^��ǂݍ��� */
	u_long	padd = PadRead(1);	

	/* quit from program: �v���O�����̏I�� */
	if (padd & PADselect) 	ret = -1;

	/* change value of rotation angle: ��]�p�x�̕ύX */
	if (padd & PADRup)	ang.vx += 32;
	if (padd & PADRdown)	ang.vx -= 32;
	if (padd & PADRleft) 	ang.vy += 32;
	if (padd & PADRright)	ang.vy -= 32;

	/* change scale: �g��E�k�� */
	if (padd & PADLup)	vec.vz += 8;
	if (padd & PADLdown) 	vec.vz -= 8;

	ang.vx += 32;

	/* rotate: ��]�p�x�iz, y, x �̏��ŉ�]�j */
	RotMatrix(&ang, &m);		
	
	/* translate:  ���s�ړ��x�N�g�� */
	TransMatrix(&m, &vec);		

	/* set rotation: ��]�p�x */
	SetRotMatrix(&m);		
	
	/* set translation: ���s�ړ��x�N�g�� */
	SetTransMatrix(&m);		

	return(ret);
}

/* Initialization of Primitive buffer
 : �v���~�e�B�u�o�b�t�@�̏�����
 */
/* DB *db;	�v���~�e�B�u�o�b�t�@ */
static void init_prim(DB *db)
{
	/* auto background clear mode: �w�i�����N���A���[�h */
	db->draw.isbg = 1;			
	
	/* set background color: �w�i�F�̐ݒ� */
	setRGB0(&db->draw, 60, 120, 120);	

	/* initialize 'wall' primitive: 'wall'�̏����ݒ� */
	SetPolyG4(&db->wall);
	
	/* set colors for each vertex: �S���_�̐F��ݒ� */
	setRGB0(&db->wall, 0xff, 0xff, 0x00);
	setRGB1(&db->wall, 0xff, 0x00, 0xff);
	setRGB2(&db->wall, 0x00, 0xff, 0xff);
	setRGB3(&db->wall, 0xff, 0x00, 0x00);
}
