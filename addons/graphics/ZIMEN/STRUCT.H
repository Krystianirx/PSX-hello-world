/* $PSLibId: Runtime Library Release 3.6$ */
/*
 *	�n�ʂŎg�p����\����
 */
typedef struct {
	u_long nearz;	/* divide�𕪊����J�n����Z�l */
	u_long shift;	/* shift�l */
	u_long nz;	/* divide�i���̍ő�l */
	u_long *cond;	/* �������e�[�u���ւ̃|�C���^ */
} GsDIVCOND0;


typedef struct {
	u_char	u, v;		/* cell texture uv */
	u_char	du,dv;		/* texture width, height */
	u_short	tpage;		/* texture page */
	u_short	nclut;		/* number of CLUTs (for fog)  */
	u_short	*clut;		/* cell clut */
	u_long	attr;		/* attribute (reserved) */
} CTYPE;

typedef struct {
	/* �}�b�v���W�n��� */
	int	mx, my;		/* �}�b�v�T�C�Y */
	int	nctype;		/* �Z���̎�� */
	char	**map;		/* �}�b�v */
	CTYPE	*ctype;		/* �Z���^�C�v */
	
	/* 3D ���W�n��� */
	GsCOORDINATE2 *coord2;	/* ���[�J�����W�n */
	u_long cw,ch;		/* �Z���̕� */
	u_long nw,nh;		/* �Z���̏c���i�\���j�̐� */
	u_long iw,ih;		/* �Z���̏c���i�P�p�^�[���j�̐��i2^n�j */
	GsDIVCOND0 *cond;	/* �������������\���̂ւ̃|�C���^ */
} Gs3DBG0;
	
typedef struct
{
  u_short *clut;
  u_short cbit;
  RECT    rectc;
  CVECTOR bgc;
} GsDPCLUT0;