/* $PSLibId: Runtime Library Release 3.6$ */
/*			tuto0
 *			
 :	   TMD viewer prototype (without shading, without texture)
 *	   TMD �r���[�A�v���g�^�C�v�i�����v�Z�Ȃ�  F3 �^�j
 *
 *		Copyright (C) 1994 by Sony Corporation
 *			All rights Reserved
 *
 *	 Version	Date		Design
 *	---------------------------------------------------------	
 *	1.00		Mar,15,1994	suzu
 *	1.10		Jan,22,1996	suzu	(English comment)
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <inline_c.h>
#include <gtemac.h>
#include "rtp.h"

#define SCR_Z		1024	/* projection: ���e�ʂ܂ł̋��� */
#define OTSIZE		4096	/* depth of OT: �n�s�̒i���i�Q�ׂ̂���j */
#define MAX_POLY	3000	/* max polygon: �\���|���S���̍ő�l */

#define MODELADDR	((u_long *)0x80100000)	/* TMD address */

/*
 * Vertex database of triangle primitive 
 : �R�p�`�|���S���̒��_���
 */
typedef struct {
	SVECTOR	n0;		/* normal: �@�� */
	SVECTOR	v0, v1, v2;	/* vertex: ���_ */
} VERT_F3;

/*
 * Triangle primitive buffer. This buffer should be allocaed dynamically
 * according to total primitive size by malloc()
 : �R�p�`�|���S���p�P�b�g
 * ���_�E�p�P�b�g�o�b�t�@�́Amalloc() �œ��I�Ɋ���t����ׂ��ł��B	
 */
typedef struct {
	int		n;		/* primitive number: �|���S���� */
	VERT_F3		vert[MAX_POLY];	/* vertex: ���_�o�b�t�@ �i�P�j*/
	POLY_F3		prim[2][MAX_POLY];/* primitive: �p�P�b�g�o�b�t�@ */
} OBJ_F3;

static int pad_read(int nprim);
int loadTMD_F3(u_long *tmd, OBJ_F3 *obj);

main()
{
	static OBJ_F3	obj;			/* object */
	static u_long	otbuf[2][OTSIZE];	/* OT */
	u_long		*ot;			/* current OT */
	int		id = 0;			/* primitive buffer ID */
	VERT_F3		*vp;			/* work */
	POLY_F3		*pp;			/* work */
	int		nprim;			/* work */
	int		i; 			/* work */

	/* initialize: �_�u���o�b�t�@�̏����� */
	db_init(640, 480, SCR_Z, 60, 120, 120);	
	
	/* read TMD: TMD ��ǂݍ��� */
	loadTMD_F3(MODELADDR, &obj);		
	
	/* start display: �\���J�n */
	SetDispMask(1);				

	/* main loop: ���C�����[�v */
	nprim = obj.n;
	while ((nprim = pad_read(nprim)) != -1) {
		
		/* clip max primitive in [0,max_nprim]
		 :  [0,max_nprim] �� nprim ���N���b�v
		 */
		limitRange(nprim, 0, obj.n);

		/* swap primitive buffer ID: �p�P�b�g�o�b�t�@ ID���X���b�v */
		id = id? 0: 1;
		ot = otbuf[id];
		
		/* clear OT: �n�s���N���A */
		ClearOTagR(ot, OTSIZE);			

		/* set primitive vertex: �v���~�e�B�u�̐ݒ� */
		vp = obj.vert;
		pp = obj.prim[id];
		
		/* 3D operation: �R�c�\���́A���̂S�s�ɏW�񂳂�� */
		for (i = 0; i < nprim; i++, vp++, pp++) {
			/* rotTransPers3 is macro. see rtp.h
			 : rotTransPers3 �̓}�N���Brtp.h ���Q�Ƃ̂���
			 */
			pp = &obj.prim[id][i];
			rotTransPers3(ot, OTSIZE, pp,
				      &vp->v0, &vp->v1, &vp->v2);

		}
		
		/* print debug information: �f�o�b�O�X�g�����O�̐ݒ� */
		FntPrint("total=%d\n", i);
		
		/* swap OT and primitive buffer
		 :�_�u���o�b�t�@�̃X���b�v�Ƃn�s�`��
		 */
		db_swap(ot+OTSIZE-1);
	}
	PadStop();
	StopCallback();
	return;
}

static int pad_read(int nprim)
{
	static SVECTOR	ang   = {512, 512, 512};	/* rotate angle */
	static VECTOR	vec   = {0,     0, SCR_Z};	
	static MATRIX	m;				/* matrix */
	static int	scale = ONE/4;
	
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

	ang.vz += 8;
	ang.vy += 8;
	
	setVector(&svec, scale, scale, scale);
	RotMatrix(&ang, &m);	
	TransMatrix(&m, &vec);	
	ScaleMatrix(&m, &svec);
	SetRotMatrix(&m);
	SetTransMatrix(&m);

	return(nprim);
}		

/*
 * load TMD: TMD �̉��
 */
int loadTMD_F3(u_long *tmd, OBJ_F3 *obj)
{
	VERT_F3		*vert;
	POLY_F3		*prim0, *prim1;
	TMD_PRIM	tmdprim;
	int		col, i, n_prim = 0;

	vert  = obj->vert;
	prim0 = obj->prim[0];
	prim1 = obj->prim[1];
	
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
		SetPolyF3(prim0);

		/* copy normal and vertex: �@���ƒ��_�x�N�g�����R�s�[���� */
		copyVector(&vert->n0, &tmdprim.n0);
		copyVector(&vert->v0, &tmdprim.x0);
		copyVector(&vert->v1, &tmdprim.x1);
		copyVector(&vert->v2, &tmdprim.x2);
		
		col = (tmdprim.n0.vx+tmdprim.n0.vy)*128/ONE/2+128;
		setRGB0(prim0, col, col, col);
		
		/* duplicate primitive for primitive double buffering
		 : �_�u���o�b�t�@���g�p����̂Ńv���~�e�B�u�̕���������
		 * ��g�����Ă���
		 */  
		memcpy(prim1, prim0, sizeof(POLY_F3));
		vert++, prim0++, prim1++;
	}
	return(obj->n = i);
}