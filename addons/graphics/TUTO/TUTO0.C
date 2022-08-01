/* $PSLibId: Runtime Library Release 3.6$ */
/*			  tuto0: draw sprites
 *
 *	   Copyright (C) 1993-1995 by Sony Computer Entertainment
 *			All rights Reserved
 *	
 *		Display 2 moving sprites with sprite animation.
 :		�Ƃɂ�����ʏ�ɃX�v���C�g��\�����Ă݂�B
 *		��������������X�v���C�g���A�j���[�V��������B
 *
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

#define DT	4	/* sprite moving unit: �X�v���C�g�̈ړ��P�� */

static void load_texture4(u_short tpage[4], u_short clut[4]);

main()
{
	/* drawing environment and display environment.
	 * Two pair of environments are needed for double buffer
	 : �_�u���o�b�t�@�g�p�̂��߁A���ꂼ��Q�g���p��
	 */
	DRAWENV		draw[2];	/* drawing environment: �`��� */
	DISPENV		disp[2];	/* display environment: �\���� */

	POLY_FT4	mat0, mat1;	/* texture mapped polygon:
					   : �e�N�X�`���}�b�v�|���S�� */
	SVECTOR		x0, x1;		/* diplay position (vs vy is used)
					   : �\���ʒu�ivx,vy �����o�̂ݎg�p�j*/

	u_short		tpage[4];	/* texture page ID (4pages)
					   : �e�N�X�`���y�[�W ID �i�S���j*/
	u_short		clut[4];	/* texture CLUT ID
					   : �e�N�X�`�� CLUT ID �S�G���g���j*/
	int		frame = 0;	/* counter: �J�E���^ */
	int		w = 64, h = 64;	/* scale: �g��E�k���� */
	int		u, v;		/* work */
	u_long		padd;		/* work */

	/* reset controller: �R���g���[���̃��Z�b�g */
	PadInit(0);		
	
	/* reset graphics subsystem : �`��E�\�����̃��Z�b�g */
	ResetGraph(0);		
	
	/* initialize debug print message function.
	 * FntLoad(x,y) loads font pattern to (x,y) on the frame buffer.
	 * SetDumpFnt() defines standard on-screen print stream.
	 : �f�o�b�O�v�����g�֐��̏�����
	 * FntLoad(x,y) �̓t���[���o�b�t�@�� (x,y) �Ƀt�H���g�����[�h����
	 * SetDumpFnt() �͕W���X�g���[�����`����
	 */
	FntLoad(960, 256);	
	SetDumpFnt(FntOpen(16, 16, 256, 64, 0, 512));
	   
	/* set 2 drawing and display environments to construct frame
	 * double buffer.
	 * When buffer #1 is drawing, buffer #2 is displayed.
	 * When buffer #2 is drawing, buffer #1 is displayed.
	 : �t���[���o�b�t�@��Ń_�u���o�b�t�@���\�����邽�߂̕`����E
	 * �\�����\���̂̃����o��ݒ肷��B
	 * �o�b�t�@ #1 �ɕ`�悵�Ă���Ԃ� �o�b�t�@ #2 ��\��
	 * �o�b�t�@ #2 �ɕ`�悵�Ă���Ԃ� �o�b�t�@ #1 ��\������B
	 */
	/*	buffer #1	(0,  0)-(320,240)
	 *	buffer #2	(0,240)-(320,480)
	 */
	SetDefDrawEnv(&draw[0], 0,   0, 320, 240);
	SetDefDispEnv(&disp[0], 0, 240, 320, 240);
	SetDefDrawEnv(&draw[1], 0, 240, 320, 240);
	SetDefDispEnv(&disp[1], 0,   0, 320, 240);

	/* indicate to clear BG: �w�i����((R,G,B)=(0,0,0))�ŃN���A���� */
	draw[0].isbg = draw[1].isbg = 1;
	setRGB0(&draw[0], 0, 0, 0);	/* (R,G,B)=(0,0,0) */
	setRGB0(&draw[1], 0, 0, 0);	/* (R,G,B)=(0,0,0) */

	/* initialize polygon primiteivs. 
	 * SetPolyFT4() initialize POLY_FT4 primitive.
	 * SetShadeTex() indicates to inhibit ShadeTex option.
	 : �|���S���v���~�e�B�u�i�g��E�k���X�v���C�g�j��������
	 * SetPolyFT4() �� POLY_FT4 �v���~�e�B�u������������
	 * SetShadeTex() �̓v���~�e�B�u�� ShadeTex �@�\�i�e�N�X�`���ɃV�F
	 * �[�f�B���O��������@�\�j���֎~����
	 */
	SetPolyFT4(&mat0);		
	SetShadeTex(&mat0, 1);		/* ShadeTex disable */
	SetPolyFT4(&mat1);
	SetShadeTex(&mat1, 1);		/* ShadeTex disable */

	/* set the initial position of each sprite
	   : �X�v���C�g�̕\���ʒu�̏����l������ */
	x0.vx = 80-w/2;  x1.vx = 240-w/2;
	x0.vy = 120-h/2; x1.vy = 120-h/2;

	/* load the texture pattern to frame buffer, and set default tpage
	 * and CLUT  
	 : �e�N�X�`���p�^�[�����t���[���o�b�t�@�ɓ]��
	 * ���ʂ̃e�N�X�`���y�[�W�E�e�N�X�`�� CLUT �� tpage, clut �ɕԂ��B
	 */
	load_texture4(tpage, clut);

	/* display enable: �\���J�n */
	SetDispMask(1);

	/* main loop: ���C�����[�v */
	while (1) {
		/* swap frame double buffer : �_�u���o�b�t�@���X���b�v */
		PutDrawEnv(frame%2? &draw[0]:&draw[1]);
		PutDispEnv(frame%2? &disp[0]:&disp[1]);

		/* get current controller status: �R���g���[���p�b�h����� */
		padd = PadRead(1);

		/* calculate sprite position: �X�v���C�g�̈ړ��ʂ����� */
		if (padd & PADLup)	x0.vy -= DT;
		if (padd & PADLdown)	x0.vy += DT;
		if (padd & PADLleft) 	x0.vx -= DT;
		if (padd & PADLright)	x0.vx += DT;

		if (padd & PADRup)	x1.vy -= DT;
		if (padd & PADRdown)	x1.vy += DT;
		if (padd & PADRleft) 	x1.vx -= DT;
		if (padd & PADRright)	x1.vx += DT;

		/* scale: �g��E�k���������� */
		if (padd & PADL1)	w--, h--;
		if (padd & PADR1) 	w++, h++;

		/* : �A�j���[�V�����̏I�� */
		if (padd & PADselect) 	break;

		/* draw the sprite (Matrchang).
		 * Texture patterns are changed by each frame to do the  
		 * sprite animations
		 : �X�v���C�g (Matchang) ��`�悷��B
		 * �X�v���C�g (Matchang) ��`�悷��B
		 * �X�v���C�g�A�j���[�V���������邽�߂ɁA�S���̃e�N�X�`��
		 * �y�[�W�ɂ����ꂽ�A�j���[�V�����p�^�[�������Ԃɐؑւ��Ă����B
		 */
 
		/* set texture pages: �e�N�X�`���y�[�W��ݒ肷��B*/
		mat0.tpage = mat1.tpage = tpage[(frame/16)%4];
		mat0.clut  = mat1.clut  = clut[(frame/16)%4];

		/* set texture point (u,v): �e�N�X�`�����W (u,v) ��ݒ� */
		u = (frame%4)*64;
		v = ((frame/4)%4)*64;
		setUVWH(&mat0, u, v, 63, 63);
		setUVWH(&mat1, u, v, 63, 63);

		/* calculate sprite position: �X�v���C�g�̍��W��ݒ� */
		setXYWH(&mat0, x0.vx, x0.vy, w, h);
		setXYWH(&mat1, x1.vx, x1.vy, w, h);

		/* draw primitives: �X�v���C�g�v���~�e�B�u�̕`�� */
		DrawPrim(&mat0);
		DrawPrim(&mat1);

		/* print debug message: �f�o�b�O���b�Z�[�W���o�� */
		FntPrint("tuto0: simplest sample\n");
		FntPrint("mat0=");dumpXY0(&mat0);
		FntPrint("mat1=");dumpXY0(&mat1);

		/* FntFlush(-1) indicates flush the standard stream
		   : FntFlush(-1) �ŕW���X�g���[�����t���b�V������ */
		FntFlush(-1);
		
		/* wait for V-BLNK: V-BLNK ������̂�҂� */
		VSync(0);

		/* update frame counter: �t���[���ԍ����A�b�v�f�[�g */
		frame++;
	}

	PadStop();
	ResetGraph(1);
	StopCallback();
	return;
}

/*
 * load texture pattern to frame buffer
 : �e�N�X�`�����[�h�֐�
 */
/* u_short tpage[4];	�e�N�X�`���y�[�W�h�c */
/* u_short clut[4];	�e�N�X�`�� CLUT �h�c */
static void load_texture4(u_short tpage[4], u_short clut[4])
{
	/* mat0, mat1, mat2, mat3 are Sprite animation image patterns.
	 * Animation patterns use 4 texture pages, and each page contains
	 * 64 patterns (64x64 size)
	 * Image format here is;
	 *	0x000-0x07f	CLUT  (256x2byte entry)
	 *	0x200-		INDEX (4bit mode, size=256x256)
	 :
	 * �X�v���C�g�A�j���[�V�����f�[�^
	 * 64x64 �̃X�v���C�g�A�j���[�V������ 64 �p�^�[���A
	 * �S���̃e�N�X�`���y�[�W�ɓn���Ċi�[����Ă���B
	 * �e�y�[�W�̐擪�A�h���X�� (mat0, mat1, mat2, mat3)�B
	 * ���e�́A
	 *	0x000-0x07f	CLUT  (256x2byte entry)
	 *	0x200-		INDEX (4bit mode, size=256x256)
	 */
	extern	u_long	mat0[];		/* Matchang of texture page #0 */
	extern	u_long	mat1[];		/* Matchang of texture page #1 */
	extern	u_long	mat2[];		/* Matchang of texture page #2 */
	extern	u_long	mat3[];		/* Matchang of texture page #3 */

	/* load 4 texture pages.
	 * Since each texture pattern is 4bit mode, it uses 64x256 area
	 * (not 256x256) of the frame buffer. 
	 :
	 * �e�N�X�`���y�[�W�����[�h����B�i�S�����j
	 * �S�r�b�g���[�h�̂��߁A���ۂ̃t���[���o�b�t�@���̗̈�́A
	 * 64x256 �����g�p���Ȃ����Ƃɒ��ӁB
	 */
	tpage[0] = LoadTPage(mat0+0x80, 0, 0, 640,  0, 256,256);
	tpage[1] = LoadTPage(mat1+0x80, 0, 0, 640,256, 256,256);
	tpage[2] = LoadTPage(mat2+0x80, 0, 0, 704,  0, 256,256);
	tpage[3] = LoadTPage(mat3+0x80, 0, 0, 704,256, 256,256);

	/* load 4 texture CLUTS
	   : �e�N�X�`�� CLUT �����[�h����B�i�S�{�j */
	clut[0] = LoadClut(mat0, 0,500);
	clut[1] = LoadClut(mat1, 0,501);
	clut[2] = LoadClut(mat2, 0,502);
	clut[3] = LoadClut(mat3, 0,503);
}
