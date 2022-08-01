/* $PSLibId: Runtime Library Release 3.6$ */
/*****************************************************************
 *
 * file: emouse.c
 *
 * 	Copyright (C) 1994,1995 by Sony Computer Entertainment Inc.
 *				          All Rights Reserved.
 *
 *	Sony Computer Entertainment Inc. Development Department
 *
 *****************************************************************/
/*
 *EMOUSE.C�͊g���@�\�̊񂹏W�߂̂��߁A�e�֐��͋@�\�w�b�_�Ŏ�ʂ킯���܂�
 * MAIN:	MAX.C�̃��C�����[�`���֌W�̊֐�
 * CDP:		CDLIB.C�֌W
 * CARD:	CARD.C�֌W
 * MOUSE:	�}�E�X���͎����̂��߂̊֐�
 */

#include "libps.h"

#include "maxtypes.h"
#include "psxload.h"
#include "spritex.h"
#include "menux.h"
#include "const.h"
#include "maxstat.h"

#include "menuset2.h"

#ifdef EMOUSE

#include "emouse.h"

/*#define DEBUG_EMOUSE       /* debug print ON */

/*����𐶂����ƂP���������Ă��鎞�ł��Q���p�b�h�̓��͂������� */
/*#define ENABLE_2P /* */

/*-----------------------��`�ƃO���[�o���ϐ�---------------------------------*/
extern void ReverseSprite( SPRTX* sprt );

extern void EMouseVclear(void) ;
static void EMouseAnimeON(void) ;

/*���j���[�A�C�e���̍ő吔�icard\menux.c���R�s�[�j */
#define ITEMMAX		96

/*EXIT�̃T�C�Y */
#define EXIT (DELTA-LINEW*2)

/*�t���X�N���[���ړ��͈� */
#define FULL_X		0
#define FULL_Y		8
#define FULL_DX		(640-DELTA/2-8)
#define FULL_DY		(480-48)

/*CARD:�J�[�h�A�C�R���̈� */
#define	CARDA_X		(XC1-12)
#define	CARDB_X		(XC2-36)
#define	CARD_DX		(DC*3+24)
#define	CARD_Y		(YC-12)
#define	CARD_DY		(DC*5)

/*CARD:�J�[�h�R�}���h�̈� */
#define	CARDCOM_X		(320-86-12)
#define	CARDCOM_DX		172
#define	CARDCOM_Y		(138-25-12)
#define	CARDCOM_DY		263

/*CARD:�J�[�hconfirm�̈� */
#define	CARDCF_X		(320-130-12)
#define	CARDCF_DX		260
#define	CARDCF_Y		(240-60-12)
#define	CARDCF_DY		120

/*���j���[�X�e�[�^�X pad.c����R�s�[ */
#define MAIN_MENU    0
#define MEMORY_CARD  1
#define CD_PLAYER    2
#define CD_PLAY_MODE 3
#define MESSAGE_VIEW 4

/*�O���Q�� */
extern int  ScreenMode;
extern u_long	padd, opadd;
extern MAXSTAT	maxstat;

/*�}�E�X�J�[�\�� */
#include "mouse.h"

#define	MOUSETYPEMAX	1

#define	MAXPAD		2
#define	RECVLEN		32

/*���j���[���ڎ��s�Ȃ�΁A�{�^����񂪃[���ɂȂ�܂Ń{�^�����͂�}������ */
/*�܂��AON/OFF�X�e�[�^�X�Ƃ��Ă� BT_ON/BT_OFF���g�p */
#define	BT_ON	1
#define	BT_OFF	0
static int BTcontrol = BT_OFF ;

/*�J�[�h�̃��j���[ ON�ŃJ�[�h���j���[�� */
static int cardmenu = BT_ON ;

/*���j���[�f�B���B */
static	int	menudelay = 0 ;

/*CARD:
 *�J�[�h���j���[�ɂăR�}���h�̖��Ȃǂ�K�؂ȕ����ɕ\������
 *���̂��߂ɁA���݂̕�����ۑ�����B
 * �J�[�h�R�}���h���o�[�X�t���O
 *	���o�[�X����ƂP
 *	[0]:COPY
 *	[1]:ALLCOPY
 *	[2]:DELETE
 */
static int	revflag[] = {0,0,0} ;
static int	revcard = 0 ;		/*���o�[�X����R�}���h�ԍ� 0..2 */
static int	revexecflag = 0 ;

/*
 * EXIT�̔w�i
 * ���̃X�v���C�g��ω������鎖�ɂ���Ă��������A�j�����s��
 */
static SPRTX	sprt_exit ;

/*MOUSE:
 *�s�h�l�f�[�^�G���g��
 *�@�s�h�l�^�̃f�[�^�|�C���^���}�E�X�J�[�\���̃^�C�v�Ƃ��ēo�^����
 *�@�}�E�X�J�[�\���̎�ނ𑝂₷�Ȃ�΁AMOUSETYPEMAX�𑝂₵�āA
 *�@�����ɂs�h�l�f�[�^�ւ̃|�C���^��ǉ�����Ηǂ��B
 */
static unsigned long *mousecursor[MOUSETYPEMAX] = {
	mouse_cursor
} ;

/*MOUSE:
 * �}�E�X�J�[�\����\���^�ɓo�^���Ȃ���
 */
static SPRTX	CursorSprites[MOUSETYPEMAX];

static int	display = 1 ;		/*�}�E�X�J�[�\���\�������^������ */
static int	cursorN = 0 ;		/*�}�E�X�J�[�\�����Ԃ�\�����邩 */
static int	mouseRSX = 0 ;		/*�}�E�X�̈ړ��̈� */
static int	mouseRSY = 0 ;
static int	mouseREX = 640 ;
static int	mouseREY = 480 ;
static int	mouseX = 10 ;		/*�}�E�X�̌��݂̈ʒu */
static int	mouseY = 10 ;
static int	mouseBT = 0 ;		/*�}�E�X�{�^���̏� */

static int      sencible=1;

/*���荞�݃v���O�����ɂ���āA���̃o�b�t�@�Ƀf�[�^������ */
/*EMouse�̉��ʃ��[�`���Ńf�R�[�h���s���̂Ŏg�����ł͋C�ɂ���K�v�͂Ȃ� */
static unsigned char	PADbuf[MAXPAD][RECVLEN] ;	/*��M�o�b�t�@�@*/

/*-----------------------�჌�x�����[�`��------------------------------------*/
/*///////////////////////////
// FT4�^�|���S���𔽓]������B���������͕ϐ���SPRTX�^
///////////////////////////*/
void	ReverseSprite( SPRTX* sprt )
{
	POLY_FT4	*poly ;
	int	x0, y0 ;
	int	x1, y1 ;
	int	x2, y2 ;
	int	x3, y3 ;

	poly = &sprt->poly;
	x0 = poly->u0 ;
	x1 = poly->u1 ;
	x2 = poly->u2 ;
	x3 = poly->u3 ;
	y0 = poly->v0 ;
	y1 = poly->v1 ;
	y2 = poly->v2 ;
	y3 = poly->v3 ;
	setUV4(poly,
	 x1, y1,
	 x0, y0,
	 x3, y3,
	 x2, y2
	) ;
}

/*MOUSE:
 * �}�E�X�J�[�\���ύX
 *	type:	�}�E�X�J�[�\���^�C�v
 */
void EMouseType(int type)
{
	cursorN = type ;
}

/*MOUSE:
 * �}�E�X�J�[�\���\��
 */
void EMouseON(void)
{
	display = 1 ;
}

/*MOUSE:
 * �}�E�X�J�[�\����\��
 */
void EMouseOff(void)
{
	display = 0 ;
}


/*MOUSE:
 * �}�E�X�J�[�\���ړ�
 *	x,y:	�ړ���
 */
void EMouseMove( int x, int y)
{
	if( x < mouseRSX) x = mouseRSX ;
		else if( x > mouseREX) x = mouseREX ;
	if( y < mouseRSY) y = mouseRSY ;
		else if( y > mouseREY) y = mouseREY ;
	mouseX = x ;
	mouseY = y ;
}

/*MOUSE:
 * �}�E�X�J�[�\���̂w�ʒu
 */
int EMouseX( void)
{
	return mouseX ;
}

/*MOUSE:
 * �}�E�X�J�[�\���̂x�ʒu
 */
int EMouseY( void)
{
	return mouseY ;
}

/*MOUSE:
 * �p�b�h���̃|�[�����O
 *	ch:	���肷��@PADbuf�̃`���l���ԍ�
 *	dx,dy:	�}�E�X�ړ���
 *	bt:	�{�^�����
 *
 *	return:		-1	�f�[�^�擾���s
 *			���4�r�b�g�F�[�����
 *                                   0x10:�}�E�X
 *		                     0x20:16�{�^���A�i���O
 *			             0x40:16�{�^��
 *			             0x80:�}���`�^�b�v
 */
int EMousePadStat( int ch, int *dx, int *dy, u_long *bt)
{
        static u_long bt_latest=0;
	int	err;
	u_long  tbt ;

	*dx = 0 ;
	*dy = 0 ;
	*bt = 0 ;

#ifdef DEBUG_EMOUSE
	printf("ch=%d:%02x, %02x, %02x, %02x, %02x, %02x ",
	       ch, PADbuf[ch][0], PADbuf[ch][1], PADbuf[ch][2],
	       PADbuf[ch][3], PADbuf[ch][4], PADbuf[ch][5]);
	printf("%02x, %02x, %02x ",
	       PADbuf[ch][6], PADbuf[ch][7], PADbuf[ch][8]) ;
	printf("\r");
#endif
	if( PADbuf[ch][0] != 0x00) {
		return -1 ;	/*��M���s */
	}

	switch( PADbuf[ch][1] & 0xf0) {
#ifdef NOMOUSE
	case 0x10:	/*XY MOUSE */
	        err = -1;
		break;
#else
	case 0x10:	/*XY MOUSE */
		if( PADbuf[ch][1] != 0x12) {
			err = -1 ;
			break ;
		}
		*dx = (signed char)PADbuf[ch][4] ;
		*dy = (signed char)PADbuf[ch][5] ;
		*bt = 0 ;
		tbt = 0;

		/*�P�U�{�^���������Ƃ̑Ή��B���{�^�����ہA�E�{�^�����X�^�[�g�ɂ���B */
		if( (~PADbuf[ch][3]) & 8) *bt |= _PADRright ;	/*�� */
		if( ScreenMode == MEMORY_CARD ) {
		    if( (~PADbuf[ch][3]) & 4) *bt |= _PADh ;	/*start */
		    if( (*bt&0xffff) == 0x0820) {
			*bt = _PADl | _PADm | _PADn | _PADo ;
		    }
		}
		err = PADbuf[ch][1] & 0xf0 ;
		/*
		 *�}�E�X���͂��}�X�N���邵����
		 *�J�[�h���j���[�ł�����ԂɂȂ���ςȂ��ɂȂ�̂�}������
		 *�i�������̊֐���ėp�Ŏg�p����̂Ȃ�΁A���̎d�|���͕K�v�Ȃ��j
		 */
		if( BTcontrol == BT_ON) {
			if( *bt == 0) BTcontrol = BT_OFF ;
			else {
				*bt = 0 ;
				*dx = 0 ;
				*dy = 0 ;
			}
		}
		break ;
#endif /* NOMOUSE */
	case 0x20:	/*16BT ANALOG */
	case 0x40:	/*16BT */
		*bt = (PADbuf[ch][2] << 8) | PADbuf[ch][3] ;
		*bt = ~*bt & 0xffff ;
		err = PADbuf[ch][1] & 0xf0 ;
		break ;
	default:
		err = -1 ;
		break ;
	}
	return err ;
}

extern int funcokflag;
/*MOUSE:
 * �}�E�X�J�[�\���̏󋵂��擾����
 *	x,y:	�}�E�X���W
 *	bt:	�{�^�����
 *
 *	return:		-1	�f�[�^�擾���s
 *			���4�r�b�g�F�[�����
 *                                   0x10:�}�E�X
 *		                     0x20:16�{�^���A�i���O
 *			             0x40:16�{�^��
 *			             0x80:�}���`�^�b�v
 */
int EMouseStat( int *x, int *y, u_long *bt)
{
	int	dx, dy ;
	int	_dx, _dy ;
	u_long  rbt, _rbt ;
	int	err, _err ;

	err = EMousePadStat( 0, &dx, &dy, &rbt) ;
	if( err == -1) {
		/*�t�@�[�X�g�����������ڑ��Ȃ�Z�J���h�������̃|�[�����O */
		err = EMousePadStat( 1, &dx, &dy, &rbt) ;
	}
#ifdef ENABLE_2P
	else if( err != 0x10 && rbt == 0) {
		/*�t�@�[�X�g�����������͖����Ȃ�Z�J���h�������̃|�[�����O */
		/*�������A�t�@�[�X�g���������}�E�X�ȊO�̎��ɓ��� */
		_dx = dx ;
		_dy = dy ;
		_rbt = rbt ;
		_err = err ;
		err = EMousePadStat( 1, &dx, &dy, &rbt) ;
		if( (err == -1 || err == 0x10) && _err != -1) {
			/*�����R���g���[���Q�ُ�Ȃ�R���g���[���P��� */
			/*�������R���g���[���P�ُ�Ȃ��
			�R���g���[���Q�ُ̈�������̂܂ܕԂ� */
			/*�܂��R���g���[���Q���}�E�X����
			�R���g���[���P�������Ă���΃R���g���[���P��� */
			dx = _dx ;
			dy = _dy ;
			rbt = _rbt ;
			err = _err ;
		}
	}
#endif
	if( !funcokflag ) {
	    EMouseMove( mouseX + dx, mouseY + dy) ;
	    *x = mouseX ;
	    *y = mouseY ;
	    *bt = rbt ;
	}

#ifdef DEBUG_EMOUSE
	printf("err=%02x:%d, %d, %04x\n", err, *x, *y, *bt) ;
#endif
	return err ;
}


/*-----------------------�A�j���[�V�������R���g���[��-------------------------*/
/*CDP:�ւ��܂��{�^���̈ʒu */
static int OFFbt_x = 0 ;
static int OFFbt_y = 0 ;
static int OFFbt2_x = 0 ;
static int OFFbt2_y = 0 ;

/*CARD:�J�[�h�A�C�R���̕\���f�[�^ .. EMouseCardConfirm�Ŏ����Ă��� */
extern	int	Target ;	/*�^�[�Q�b�g�A�C�R���̂��� */
extern	int	Event ;		/*confirm���̃C�x���g���� */
static	int CHGiconflag = 0;	/*�A�C�R���I���������Hyes=BT_ON */
static	SPRTX	ICONsprt ;	/*�I�������A�C�R���X�v���C�g��ێ����� */
static	SPRTX	BLUEsprt ;	/*�A�C�R�����I�����̃X�v���C�g */

static void EMouseTARGETdisp( DB *cdb) {}
static void EMouseOFFbt( DB *cdb) {}
void EMouseCDbt( int id) {}
void EMouseCDbt2( int id) {}
static void chgclut( int id, int flag) {}
void EMouse_dispreverb( int id) {}
static void EMouseCLUT() {}
static int EMouseInitOFFbt( void) {}
static int EMouseInitEXITbt( void) {}
static void EMouseAnimeDisp( DB *cdb) {}
static void EMouseInitCard() {}
void EMouseRevCard(int com, int direc) {}
void EMouseRevCardEXECON( int count) {}
void EMouseRevCardEXEC(int direc) {}
void EMouseRevCardEXECall() {}

/*MAIN:
 * �}�E�X�J�[�\���\���t�b�N
 *	flag:	0	�}�E�X�J�[�\���̕\���^��\���ɏ]��
 *		1	�}�E�X�J�[�\���̋����\��
 */
void EMouseDisp( int flag, DB *cdb)
{
	int	x, y ;

	EMouseMove( mouseX, mouseY) ;	/*���~�b�g�������� */
	x = mouseX ;
	y = mouseY ;
	if( (flag == 0 && display == 1) || flag == 1) {
	    SetSpritePosition( &CursorSprites[cursorN],
		 x+CursorSprites[cursorN].mx, y+CursorSprites[cursorN].my );
	    AddSprite2( cdb, &CursorSprites[cursorN]) ;
	}
}

/*MAIN:
 * �}�E�X�̕\������
 */
int EMouseInitDisp( void)
{
	int	i ;
	SPRTX *sprt ;

	EMouseInitEXITbt() ;
	EMouseInitOFFbt() ;
	EMouseInitCard() ;
	EMouseCLUT() ;

	sprt = CursorSprites ;
	for( i = 0; i < MOUSETYPEMAX; i++) {
              /* 95/03/24 sprt -> CursorSprites */
              MakeSpriteData( &CursorSprites[i], mousecursor[i], 0 );
              SetSpritePosition( &CursorSprites[i], 200, 100 );
              SetSpriteTrans( &CursorSprites[i], 0 );
              SetSpriteShadeTex( &CursorSprites[i], 1 );
              CursorSprites[i].priority = 1 ;         /*�v���C�I���e�B�ő� */
	}
	if( ScreenMode != MEMORY_CARD )
	  funcokflag = 0;

	return 0;
}

/*MOUSE:
 * �}�E�X�ړ��̈�ݒ�
 *	x,y:	��`�̈�̍���ʒu
 *	dx,dy:	��`�̈�̃T�C�Y
 */
void EMouseRange( int x, int y, int dx, int dy)
{
	mouseRSX = x ;
	mouseRSY = y ;
	mouseREX = mouseRSX + dx ;
	mouseREY = mouseRSY + dy ;
}

/*MAIN:
 * �}�E�X���W���[��������
 */
int EMouseInit(void)
{
	EMouseVclear() ;
        InitPAD(PADbuf[0], MAXPAD, PADbuf[1], MAXPAD);
       	EMouseMove( 80, 100);
        StartPAD();
	ChangeClearPAD(0);

	return 0;
}

/*MAIN:
 * �}�E�X�ړ��ɂƂ��Ȃ��A�J�����g�������ړ�����
 * return: 0	�}�E�X�̓��j���[���ڂ̏�ɂ��Ȃ�
 *	   -1	�}�E�X�̓��j���[���ڂ̏�ɂ���
 *	   -2	���������A�j����������
 *	   1	���������ړ�����
 *			padret�ɂ������l���i�[
 */
int EMouseMoveMenu( int mx, int my, SPRTX **sprt, u_long padd, u_long *padret)
{
    int	x, y, dx, dy, i, current_id, group  ;
    SPRTX	*sprite ;
    extern void	funcIcon( long pad );

    if( revexecflag) {
	revexecflag = 0 ;
	EMouseRevCardEXEC( 0) ;
    }

    group = 0 ;
    current_id = MENUgetCurrentItem();
    /*�J�����g�̃O���[�v���擾���� */
    for( i = 0;; i++) {
	if( MENU[i].RM.id == 0) continue ;  /* break -> continue 95/04/05 */
	if( current_id == MENU[i].RM.id) {
	    group = MENU[i].RM.group ;
	    break ;
	}
    }

    /*�������[�J�[�h�A�C�R���̂��߂̓��ꏈ���B
      ���E�̃}�E�X�ʒu�ɂ���Ă������ړ��𔭍s���� */
    /*�O���[�v�ԍ��𒼐ڂ݂Ă���̂ŁA
      �����J�[�h�̃��j���[���ύX���ꂽ�ꍇ�ɂ͒��ӁI */
    if ( ScreenMode == MEMORY_CARD && cardmenu == BT_OFF) {
	if( mx > CARDA_X+CARD_DX-15 && group == 3) {
	    *padret = _PADLright ;
	    return 1 ;
	}
	if( mx < CARDB_X+15 && group == 4) {
	    *padret = _PADLleft ;
	    return 1 ;
	}
    }

    /*�}�E�X�w���ʒu�̏C�� */
    mx = mx + CursorSprites[cursorN].h / 2 ;
    my = my + CursorSprites[cursorN].w / 2 ;

    /*���j���[�ړ����� */
    for( i = 0; i < ITEMMAX; i++) {
	if( MENU[i].RM.id == 0) continue ;
	if( MENU[i].RM.execFunc == NULL) continue ;
	if( MENU[i].Sprites[fot].priority == HIDE ) continue;

	sprite = &(MENU[i].Sprites[fot]) ;

	dx = MENU[i].RM.mw ;
	dy = MENU[i].RM.mh ;
	x = sprite->x - dx / 2 ;
	y = sprite->y - dy / 2 ;

	/*�}�E�X���X�v���C�g�̏�ɂ����烁�j���[�ړ� */
	if( (x < mx) && ((x+dx) > mx) && (y < my) && ((y+dy) > my) ) {
	    if( MENU[i].RM.id != current_id ) {
		/*effect��0x20�̂��́i�ړ����j�̓��j���[�Z�b�g���܂��� */
		if( MENU[i].RM.effect&0x20 ) ;
		else {
		    if( ScreenMode == CD_PLAY_MODE && MENU[i].RM.id == 22 ) {
			current_id = 10;
			MENUsetCurrentItem( current_id );	/*�ړ� */
		    }
		    else {
			MENUsetCurrentItem( MENU[i].RM.id );	/*�ړ� */
		    }
		}
	    }

	    *sprt = sprite ;
	    /*
	      * �J�[�h�̃A�C�R���̖��̂�\������
	      *	�A�C�R���G���A���ǂ����� ���j���[��
	      *      funcIcon���ݒ肳��Ă���
	      *	���ǂ����ɂ���Ĕ��肷��
	     */
	    if( MENU[i].RM.execFunc == funcIcon) {
		/*effect��0x20�̂��́i�ړ����j�̓K�C�h�\�������܂��� */
		if( MENU[i].RM.effect&0x20 ) ;
		else {
		    SetStringUVclr(1);
		    SetStringUV( MENU[i].RM.id) ;
		}
	    } else if( sencible ) {
		if( MENU[i].RM.execFunc == funcExit && (padd & OKKey)) {
		    /*�h�A�J���A�j�����I�� */
		    EMouseAnimeON() ;
		    return -2 ;
		}
	    }
	    return -1 ;
	}
    }

    if ( ScreenMode == MEMORY_CARD && cardmenu == BT_OFF) {
	if ( my > CARD_Y && my < CARD_Y+CARD_DY ) {
	    if ( mx > CARDA_X && mx < CARDA_X+CARD_DX-15 ) {
		SetStringUV(0);
	    } else if ( mx > CARDB_X && mx < CARDB_X+CARD_DX-15 ) {
		SetStringUV(1);
	    }
	}
    }

    return 0 ;
}

/*MAIN:
 * �o�`�c�ړ����̃��j���[���ߏ���
 * return: 0	���ꏈ���Ȃ�
 *	   -2	���������A�j������
 */
static int EMouseCheckMenu( u_long padd)
{
	int	i, id, group  ;

	if( revexecflag) {
		revexecflag = 0 ;
		EMouseRevCardEXEC( 0) ;
	}

	group = 0 ;
	id = MENUgetCurrentItem();
	/*�J�����g�̃O���[�v���擾���� */
	for( i = 0;; i++) {
		if( MENU[i].RM.id == 0) break ;
		if( id == MENU[i].RM.id) {
			group = MENU[i].RM.group ;
			break ;
		}
	}
	for( i = 0;; i++) {
		if( MENU[i].RM.id == 0) break ;
		if( MENU[i].RM.id != id) continue ;
		if( MENU[i].RM.execFunc == funcExit && (padd & OKKey)) {
			/*�h�A�J���A�j�����I�� */
			EMouseAnimeON() ;
			return -2 ;
		}
	}
	return 0 ;
}

/*CARD:
 *�J�[�h�̈ړ��͈͂�S��ʂ�
 */
void EMouseSetFree()
{
	EMouseRange( FULL_X, FULL_Y, FULL_DX, FULL_DY);
	EMouseMove( 100, 100);
	menudelay = 2 ;
	BTcontrol = BT_ON ;
	cardmenu = BT_OFF ;
}

/*CARD:
 *�J�[�h�̈ړ��͈͂��R�}���h�͈͂�
 */
void EMouseSetCardHome()
{
	SPRTX	*sprite ;

	if( CHGiconflag == BT_ON) {
		CHGiconflag = BT_OFF ;
		sprite = &ICONsprt ;
		SetSpritePriority( sprite, (u_long)HIDE );
		SetSpritePosition( sprite, BLUEsprt.x, BLUEsprt.y) ;
	}

	EMouseRevCardEXECall() ;

	EMouseRange( CARDCOM_X, CARDCOM_Y, CARDCOM_DX, CARDCOM_DY);

        sprite = MENUgetSprtxPtr(MENUgetCurrentItem());
        EMouseMove( sprite->x-DELTA/4, sprite->y-DELTA/4);
	BTcontrol = BT_ON ;
	cardmenu = BT_ON ;
}

/*CARD:
 *�J�[�h�̈ړ��͈͂��m�F�͈͂�
 */
void EMouseSetCardConfirm()
{
	EMouseRange( CARDCF_X, CARDCF_Y, CARDCF_DX, CARDCF_DY);

        if( Event == _ID_DELETE )
          EMouseMove( CARDCF_X+CARDCF_DX/2+DELTA, CARDCF_Y+CARDCF_DY/2);
        else
          EMouseMove( CARDCF_X+CARDCF_DX/2-DELTA, CARDCF_Y+CARDCF_DY/2);

	BTcontrol = BT_ON ;
	cardmenu = BT_OFF ;
}


/*CARD:
 *�J�[�h�̈ړ��͈͂��`���A�C�R���͈͂�
 */
void EMouseSetCardIconA()
{
	EMouseRevCardEXEC( 0) ;

	EMouseRange( CARDA_X, CARD_Y, CARD_DX, CARD_DY);
       	EMouseMove( CARDA_X+CARD_DX/2, CARD_Y+CARD_DY/2);
	BTcontrol = BT_ON ;
	cardmenu = BT_OFF ;
}

/*CARD:
 *�J�[�h�̈ړ��͈͂��a���͈͂�
 */
void EMouseSetCardIconB()
{
	EMouseRevCardEXEC( 1) ;

	EMouseRange( CARDB_X, CARD_Y, CARD_DX, CARD_DY);
       	EMouseMove( CARDB_X+CARD_DX/2, CARD_Y+CARD_DY/2);
	BTcontrol = BT_ON ;
	cardmenu = BT_OFF ;
}

/*�I���{�^���̃A�j������ */
int exitanime = 0 ;
static SPRTX	*animesprt ;

/*MAIN:
 *�A�j�����������ǂ����̃t���O�Amax.c�ɂĎQ�Ƃ��܂�
 */
int EMouse_get_exitanime()
{
	return exitanime ;
}

/*MAIN:
 * �I���A�j�������i�J�[�h���b�c�o���p)
 * �I���֐����O�ŏ�����܂��B�A�j����ĂԂ悤�ɂ��܂�
 */
static void EMouseAnimeON(void)
{
	int	item ;
	POLY_FT4	*poly ;

	item = MENUgetCurrentItem();
	animesprt = &sprt_exit ;
	exitanime = 8*10 ;
	poly = &(animesprt->poly) ;
}

/*MAIN:
 * ���C�����j���[����p�̓���
 *	�p�b�h�J�[�\�������炷�̂̓��C�����j���[�̂Q���ڂ݂̂Ȃ̂ŁA
 *	�Ԃɍ��킹�̊֐��ɂđΉ�
 */
static void MoveCursorMain( DB *cdb, SPRTX *cursor )
{
    SPRTX *current;

    if( ScreenMode != MESSAGE_VIEW ) { /* Warning ��ʂɂ̓J�[�\���s�v */
	current = MENUgetSprtxPtr( MENUgetCurrentItem() );

	SetSpritePosition( cursor,
			  current->x+cursor->w/2, current->y+cursor->h*5/2 );
	AddSprite2( cdb, cursor );
    }
}

/**********************************
MAIN:
  Mouse / PAD ���������@���͕�

�ȉ���extern���g�p���Ă��܂�
	extern u_long	padd;
	extern MAXSTAT	maxstat;
	extern int fot ;
**********************************/

static	int	delay = 0 ;		/*���C�����j���[�u�����N�p�̃J�E���^ */
static	int	delaya = 64 ;		/*���C�����j���[�u�����N�p�̃J�E���^ */
static	int	delayb = 64 ;		/*���C�����j���[�u�����N�p�̃J�E���^ */
static	SPRTX	*sprite ;		/*�}�E�X���q�b�g�����X�v���C�gX */
static	int	ctype ;			/*�f�o�C�X�^�C�v */

void EMouseIN( DB *cdb)
{
	int		mx, my, i ;
	u_long          bt, cardpad ;
	int		col_b, col_g, col_r ;	/*�u�����N�p�̃��[�N�ϐ� */
	POLY_FT4	*poly ;

	if( exitanime > 0 ) {
	    /*�I���{�^���̃A�j������ */
	    return ;
	} else if ( delay ) {
	    /*���C����ʂ̃{�^���u�����N���� */
	    return ;
	}
	/*�ʏ펞�̃}�E�X�^���������� */
        ctype = EMouseStat( &mx, &my, &bt) ;
        padd = bt ;
        if( ctype == 0x10) {
	    /* �}�E�X���� */
	    i = EMouseMoveMenu( mx, my, &sprite, padd, &cardpad) ;

	    if( padd == 0 ) {
		sencible = 1;
	    }

	    if( i == 0) {
		if( !(padd & _PADh) ) {   /*�E�{�^���͗̈�O�ł��L�� */
		    if( padd != 0 ) {
			sencible = 0;
		    }
		    padd = 0 ;	/* ���j���[���ڏ�łȂ��ꍇ�ɂ͋����I�ɖ��s�� */
		}
	    }
	    else if( i == -2) {
		exitanime = 0;
		delay = 64;
		padd = 0;
	    }
	    else {
		if( i == 1 && padd == 0) {
		    padd = cardpad ;	/*�J�[�h�A�C�R���̈ړ����� */
		}
	    }

	    if( !sencible ) padd = 0;
	
	    if ( ScreenMode == MAIN_MENU && (padd & OKKey) ) {
		delay = 64 ;
		padd = 0 ;
	    }
        } else if( ctype != -1) {
	    /*�}�E�X�ȊO�Ȃ�΁A�P�U�{�^���������Ƃ��Ĉ����Ă��܂� */
	    i = EMouseCheckMenu( padd) ;
	    if( i == -2) {
		padd = 0 ;	/* �A�j������ */
		exitanime = 0;
		delay = 64;
		sprite = MENUgetSprtxPtr( MENUgetCurrentItem() );
	    }
	    if ( ScreenMode == MAIN_MENU && (padd & OKKey) ) {
		/*���C����ʂł̃u�����N���� */
		delay = 64 ;
		padd = 0 ;
		sprite = MENUgetSprtxPtr( MENUgetCurrentItem() );
	    }
        }
}

/**********************************
MAIN:
  Mouse / PAD ���������@�\��

�ȉ���extern���g�p���Ă��܂�
	extern u_long	padd;
	extern MAXSTAT	maxstat;
	extern int fot ;
**********************************/
void EMouseDISP( DB *cdb)
{
	int		mx, my, i ;
	int		col ;	/*�u�����N�p�̃��[�N�ϐ� */
	POLY_FT4	*poly ;

	if ( ScreenMode == MESSAGE_VIEW ) {
		return ;
	}

	if( exitanime > 0) {
		/* �I���{�^���̃A�j������ */
		poly = (POLY_FT4 *)&(animesprt->poly) ;
		exitanime-- ;
		if( exitanime == 0) {
			padd = OKKey ;		/*EXIT ���s */
		} else if( (exitanime % 10) == 0) {
			setXY4(poly, poly->x0, poly->y0, poly->x1 - 1, poly->y1 + 1,
			       poly->x2, poly->y2, poly->x3 - 1, poly->y3 - 1 );
			EMouseAnimeDisp( cdb) ;
		} else {
			EMouseAnimeDisp( cdb) ;
		}
	} else if ( delay) {
		/*���C����ʂ̃{�^���u�����N���� */
		delay -= 2 ;
		col = ((delay-4)*2) & 0x7f ;
		SetSpriteRGB( sprite, col, col, col );
		if( delay <= 0) {
			padd = OKKey ;
			SetSpriteRGB( sprite, 128, 128, 128 );
		}
	} else {
		EMouseAnimeDisp( cdb) ;
	        if( ctype == 0x10) {
			/* �}�E�X���� */
			EMouseDisp( 0, cdb) ;
	        } else if( ctype != -1) {
		    MoveCursor( cdb, &maxstat.cursor[fot] );
	        }
	}
}

/*MAIN:
 * �������������ϐ��̏�����
 */
void EMouseVclear(void)
{
	BTcontrol = BT_OFF ;
	cardmenu = BT_ON ;
	menudelay = 0 ;
	revflag[0] = 0 ;
	revflag[1] = 0 ;
	revflag[2] = 0 ;
	revcard = 0 ;
	revexecflag = 0 ;
	mousecursor[0] = mouse_cursor ;
	display = 1 ;		/*�}�E�X�J�[�\���\�������^������ */
	cursorN = 0 ;		/*�}�E�X�J�[�\�����Ԃ�\�����邩 */
	mouseRSX = 0 ;		/*�}�E�X�̈ړ��̈� */
	mouseRSY = 0 ;
	mouseREX = 640 ;
	mouseREY = 480 ;
	mouseX = 10 ;		/*�}�E�X�̌��݂̈ʒu */
	mouseY = 10 ;
	mouseBT = 0 ;		/*�}�E�X�{�^���̏� */
	/*�ւ��܂��{�^���̈ʒu */
	OFFbt_x = 0 ;
	OFFbt_y = 0 ;
	OFFbt2_x = 0 ;
	OFFbt2_y = 0 ;
	/*�J�[�h�A�C�R���̕\���f�[�^ .. EMouseCardConfirm�Ŏ����Ă��� */
	CHGiconflag = 0;
	/*�I���{�^���̃A�j������ */
	exitanime = 0 ;
	delay = 0 ;			/*���C�����j���[�u�����N�p�̃J�E���^ */
	delaya = 64 ;			/*���C�����j���[�u�����N�p�̃J�E���^ */
	delayb = 64 ;			/*���C�����j���[�u�����N�p�̃J�E���^ */

	ICONsprt.priority = HIDE;

	sencible = 1;
}

#endif /* EMOUSE */