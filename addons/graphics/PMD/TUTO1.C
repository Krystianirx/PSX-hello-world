/* $PSLibId: Runtime Library Release 3.6$ */
/*			tuto1
 *			
 :	display TMD as PMD (POLY_F3 surface, without lighting) 	
 *	   PMD-TMD �r���[�A�v���g�^�C�v�i�����v�Z�Ȃ�  F3 �^�j
 *
 *		Copyright (C) 1994 by Sony Corporation
 *			All rights Reserved
 *
 *	 Version	Date		Design
 *	-----------------------------------------	
 *	1.00		Mar,15,1994	suzu
 *	2.00		Jul,14,1994	suzu	(using PMD)
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

#define SCR_Z		1024			/* projection */
#define OTLENGTH	12			/* depth of OT */
#define OTSIZE		(1<<OTLENGTH)		/* depth of OT */
#define MAX_POLY	3000			/* max polygon */

#define MODELADDR	((u_long *)0x80100000)	/* TMD address */

/*
 : triangle primitive buffer
 * This area may be allocated dynamically using malloc() according to 'n'.
 * �R�p�`�|���S���p�P�b�g
 * ���_�E�p�P�b�g�o�b�t�@�|���S���̌��ɉ����āAmalloc() �œ��I�Ɋ���
 * �t����ׂ��ł��B	
 */
typedef struct {
	int	n;
	struct {			
		POLY_F3	prim[2];
		SVECTOR v0, v1, v2;
	} p[MAX_POLY];
} OBJ_F3;

static int pad_read(int nprim);
static int loadTMD_F3(u_long *tmd, OBJ_F3 *obj);
main()
{
	static OBJ_F3	obj;			/* object */
	static u_long	otbuf[2][OTSIZE];	/* OT buffer */
	int		id = 0;			/* double buffer ID */
	int		i, nprim;		/* work */
	
	db_init(640, 480, SCR_Z, 60, 120, 120);	/* initialize */
	nprim = loadTMD_F3(MODELADDR, &obj);	/* read TMD */
	SetDispMask(1);				/* start display */

	/* main loop: ���C�����[�v */
	while ((obj.n = pad_read(obj.n)) != -1) {
		
		/* limit primitive number: [0,max_nprim] �� nprim ���N���b�v */
		limitRange(obj.n, 0, nprim);

		/* swap packet ID: �p�P�b�g�o�b�t�@ ID���X���b�v */
		id = id? 0: 1;
		
		/* clear OT : �n�s���N���A */
		ClearOTagR(otbuf[id], OTSIZE);			

		/* rotation PMD object: �v���~�e�B�u�̐ݒ� */
		RotPMD_F3((long *)&obj, otbuf[id], OTLENGTH, id, 0);
		
		/* print: �f�o�b�O�X�g�����O�̐ݒ� */
		FntPrint("total=%d\n", obj.n);
		
		/* draw OT and swap frame double buffer
		 : �_�u���o�b�t�@�̃X���b�v�Ƃn�s�`��
		 */
		db_swap(otbuf[id]+OTSIZE-1);
	}
	PadStop();
	StopCallback();
	return;
}

static int pad_read(int nprim)
{
	static SVECTOR	ang   = {512, 512, 512};	/* rotate angle */
	static VECTOR	vec   = {0,     0, SCR_Z*8};	
	static MATRIX	m;				/* matrix */
	static int	scale = ONE;
	
	VECTOR	svec;
	int 	padd = PadRead(1);
	
	if (padd & PADselect)	return(-1);
	if (padd & PADLup) 	nprim += 4;
	if (padd & PADLdown)	nprim -= 4;
	if (padd & PADRup)	ang.vx += 32;
	if (padd & PADRdown)	ang.vx -= 32;
	if (padd & PADRleft) 	ang.vy += 32;
	if (padd & PADRright)	ang.vy -= 32;
	if (padd & PADL1)	vec.vz += 32;
	if (padd & PADL2) 	vec.vz -= 32;
	if (padd & PADR1)	scale  -= ONE/256;
	if (padd & PADR2)	scale  += ONE/256;
	if (padd & PADLup)	nprim  += 32;
	if (padd & PADLdown)	nprim  -= 32;

	ang.vz += 8;
	ang.vy += 8;
	
	setVector(&svec, scale, scale, scale);
	RotMatrix(&ang, &m);	
	TransMatrix(&m, &vec);	
	ScaleMatrix(&m, &svec);
	db_set_matrix(&m);

	return(nprim);
}		

/*
 * load TMD: TMD �̉��
 */
static int loadTMD_F3(u_long *tmd, OBJ_F3 *obj)
{
	TMD_PRIM	tmdprim;
	int		col, i, n_prim = 0;

	/* open TMD: TMD �̃I�[�v�� */
	if ((n_prim = OpenTMD(tmd, 0)) > MAX_POLY)
		n_prim = MAX_POLY;

	/*
	 * Set unchanged member of primitive here to deliminate main
	 * memory write access
	 : ���������C�g�A�N�Z�X�����炷���߂Ƀv���~�e�B�u�o�b�t�@�̂����A
	 * ���������Ȃ����̂����炩���ߐݒ肵�Ă���
	 */	 
	for (i = 0; i < n_prim && ReadTMD(&tmdprim) != 0; i++) {
		
		/* initialize primitive: �v���~�e�B�u�̏����� */
		SetPolyF3(&obj->p[i].prim[0]);
		
		/* copy normal and vertex: �@���ƒ��_�x�N�g�����R�s�[���� */
		copyVector(&obj->p[i].v0, &tmdprim.x0);
		copyVector(&obj->p[i].v1, &tmdprim.x1);
		copyVector(&obj->p[i].v2, &tmdprim.x2);

		col = (tmdprim.n0.vx+tmdprim.n0.vy)*128/ONE/2+128;
		setRGB0(&obj->p[i].prim[0], col, col, col);
		
		/* duplicate primitive for primitive double buffering
		 : �_�u���o�b�t�@���g�p����̂Ńv���~�e�B�u�̕���������
		 * ��g�����Ă���
		 */  
		memcpy(&obj->p[i].prim[1],
		       &obj->p[i].prim[0], sizeof(POLY_F3));
	}
	return(obj->n = i);
}