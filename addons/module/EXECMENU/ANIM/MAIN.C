/*
 * $PSLibId: Runtime Library Release 3.6$
 */
/*
 *          Movie Sample Program
 *
 *      Copyright (C) 1994,5 by Sony Corporation
 *          All rights Reserved
 *
 *  CD-ROM ���烀�[�r�[���X�g���[�~���O����T���v��
 *
 *   Version    Date
 *  ------------------------------------------
 *  1.00        Jul,14,1994     yutaka
 *  1.10        Sep,01,1994     suzu
 *  1.20        Oct,24,1994     yutaka(anim subroutine��)
 *  1.30        Jun,02,1995     yutaka(�㏈��)
 *  1.40        Jul,10,1995     masa(imgbuf�_�u���o�b�t�@��)
 *  1.50        Jul,20,1995     ume(��ʃN���A����)
 *  1.50b       Aug,03,1995     yoshi(for EXEC)
 *  1.50c       Mar,04,1996     yoshi(added English comments)
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>
#include <r3000.h>
#include <asm.h>
#include <kernel.h>


#define FILE_NAME   "\\DATA\\MOV.STR;1"
#define START_FRAME 1
#define END_FRAME   577
#define POS_X       36
#define POS_Y       24
#define SCR_WIDTH   320
#define SCR_HEIGHT  240

/*
 *  16bit/pixel mode or 24 bit/pixel mode :
 *  �f�R�[�h����F���̎w��(16bit/24bit)
 */
#define IS_RGB24    1   /* 0:RGB16, 1:RGB24 */
#if IS_RGB24==1
#define PPW 3/2     /* pixel/short word :
		       �P�V���[�g���[�h�ɉ��s�N�Z�����邩  */
#define DCT_MODE    3   /* 24bit decode : 24bit ���[�h�Ńf�R�[�h */
#else
#define PPW 1       /* pixel/short word :
		      �P�V���[�g���[�h�ɉ��s�N�Z�����邩 */
#define DCT_MODE    2   /* 16 bit decode : 16bit ���[�h�Ńf�R�[�h */
#endif

/*
 *  decode environment : �f�R�[�h���ϐ�
 */
typedef struct {
    u_long  *vlcbuf[2]; /* VLC buffer �idouble�j */
    int vlcid;      /* current decode buffer id :
		       ���� VLC �f�R�[�h���o�b�t�@�� ID */
    u_short *imgbuf[2]; /* decode image buffer �idouble�j*/
    int imgid;      /* corrently use buffer id :
		       ���ݎg�p���̉摜�o�b�t�@��ID */
    RECT    rect[2];    /* double buffer orign(upper left point) address
			   on the VRAM (double buffer) :
			   VRAM����W�l�i�_�u���o�b�t�@�j */
    int rectid;     /* currently translating buffer id :
		       ���ݓ]�����̃o�b�t�@ ID */
    RECT    slice;      /* the region decoded by once DecDCTout() :
			   �P��� DecDCTout �Ŏ��o���̈� */
    int isdone;     /* the flag of decoding whole frame :
		       �P�t���[�����̃f�[�^���ł����� */
} DECENV;
static DECENV   dec;        /* instance of DECENV : �f�R�[�h���̎��� */

/*
 *  Ring buffer for STREAMING
 *  minmum size is two frame :
 *  �X�g���[�~���O�p�����O�o�b�t�@
 *  CD-ROM����̃f�[�^���X�g�b�N
 *  �Œ�Q�t���[�����̗e�ʂ��m�ۂ���B
 */
#define RING_SIZE   32      /* 32 sectors : �P�ʂ̓Z�N�^ */
static u_long   Ring_Buff[RING_SIZE*SECTOR_SIZE];

/*
 *  VLC buffer(double buffer)
 *  stock the result of VLC decode :
 *  VLC�o�b�t�@�i�_�u���o�b�t�@�j
 *  VLC�f�R�[�h��̒��ԃf�[�^���X�g�b�N
 */
#define VLC_BUFF_SIZE 320/2*256     /* not correct value :
				       �Ƃ肠�����[���ȑ傫�� */
static u_long   vlcbuf0[VLC_BUFF_SIZE];
static u_long   vlcbuf1[VLC_BUFF_SIZE];

/*
 *  image buffer(double buffer)
 *  stock the result of MDEC
 *  rectangle of 16(width) by XX(height) :
 *  �C���[�W�o�b�t�@�i�_�u���o�b�t�@�j
 *  DCT�f�R�[�h��̃C���[�W�f�[�^���X�g�b�N
 *  ����16�s�N�Z���̋�`���Ƀf�R�[�h���]��
 */
#define SLICE_IMG_SIZE 16*PPW*SCR_HEIGHT
static u_short  imgbuf0[SLICE_IMG_SIZE];
static u_short  imgbuf1[SLICE_IMG_SIZE];
    
/*
 *  ���̑��̕ϐ�
 */
static int  StrWidth  = 0;  /* resolution of movie :
			       ���[�r�[�摜�̑傫���i���Əc�j */
static int  StrHeight = 0;  
static int  Rewind_Switch;  /* the end flag set after last frame :
			       �I���t���O ����̃t���[���܂ōĐ�����ƂP�ɂȂ� */

/*
 *  prototypes :
 *  �֐��̃v���g�^�C�v�錾
 */
static int anim();
static void strSetDefDecEnv(DECENV *dec, int x0, int y0, int x1, int y1);
static void strInit(CdlLOC *loc, void (*callback)());
static void strCallback();
static void strNextVlc(DECENV *dec);
static void strSync(DECENV *dec, int mode);
static u_long *strNext(DECENV *dec);
static void strKickCD(CdlLOC *loc);

main()
{
    ResetCallback();
    CdInit();
    PadInit(0);
    ResetGraph(0);
    SetGraphDebug(0);
    
    anim();     /* animation subroutine : �A�j���[�V�����T�u���[�`�� */

    PadStop();
    ResetGraph(3); 
    StopCallback();
#ifdef DEAD_PARENT
    /* load parent again, because parent was destroyed already. :
       ���ɐe�͎���ł���̂ŁA������x�e��LoadExec()���� */
    _96_init();
    LoadExec("cdrom:\\EXECMENU\\EXECMENU.EXE;1",0x801ffff0,0);
    /* this setting of stack pointer is meaningless, because EXECMENU.EXE
       was linked with 2MBYTE.OBJ. :
       EXECMENU.EXE �ɂ� 2MBYTE.OBJ �������N����Ă���̂ŁA�����ł̃X�^�b�N
       �|�C���^�����l�̐ݒ�͈Ӗ��������Ȃ����A�ꉞ 0x801ffff0 �Ƃ��Ă���B*/
#else
    return(0);
#endif
}


/*
 *  animation subroutine forground process :
 *  �A�j���[�V�����T�u���[�`�� �t�H�A�O���E���h�v���Z�X
 */
static int anim()
{
    DISPENV disp;       /* display buffer : �\���o�b�t�@ */
    DRAWENV draw;       /* drawing buffer : �`��o�b�t�@ */
    int id;     /* display buffer id : �\���o�b�t�@�� ID */
    CdlFILE file;
    
    /* search file : �t�@�C�����T�[�` */
    if (CdSearchFile(&file, FILE_NAME) == 0) {
        printf("file not found\n");
	PadStop();
	ResetGraph(3);
        StopCallback();
        return 0;
    }
    
    /* set the position of vram : VRAM��̍��W�l��ݒ� */
    strSetDefDecEnv(&dec, POS_X, POS_Y, POS_X, POS_Y+SCR_HEIGHT);

    /* init streaming system & kick cd : �X�g���[�~���O���������J�n */
    strInit(&file.pos, strCallback);
    
    /* VLC decode the first frame : �ŏ��̃t���[���� VLC�f�R�[�h���s�Ȃ� */
    strNextVlc(&dec);
    
    Rewind_Switch = 0;
    
    while (1) {
        /* start DCT decoding the result of VLC decoded data :
	   VLC�̊��������f�[�^��DCT�f�R�[�h�J�n�iMDEC�֑��M�j */
        DecDCTin(dec.vlcbuf[dec.vlcid], DCT_MODE);
        
        /* prepare for recieving the result of DCT decode :
	   DCT�f�R�[�h���ʂ̎�M�̏���������            */
        /* next DecDCTout is called in DecDCToutCallback :
	   ���̌�̏����̓R�[���o�b�N���[�`���ōs�Ȃ� */
        DecDCTout(dec.imgbuf[dec.imgid], dec.slice.w*dec.slice.h/2);
        
        /* decode the next frame's VLC data :
	   ���̃t���[���̃f�[�^�� VLC �f�R�[�h */
        strNextVlc(&dec);
        
        /* wait for whole decode process per 1 frame :
	   �P�t���[���̃f�R�[�h���I������̂�҂� */
        strSync(&dec, 0);
        
        /* wait for V-Vlank : V-BLNK ��҂� */
        VSync(0);
        
        /* swap the display buffer : �\���o�b�t�@���X���b�v     */
        /* notice that the display buffer is the opossite side of
	   decoding buffer :
	   �\���o�b�t�@�̓f�R�[�h���o�b�t�@�̋t�ł��邱�Ƃɒ��� */
        id = dec.rectid? 0: 1;
        SetDefDispEnv(&disp, 0, (id)*240, SCR_WIDTH*PPW, SCR_HEIGHT);
/*      SetDefDrawEnv(&draw, 0, (id)*240, SCR_WIDTH*PPW, SCR_HEIGHT);*/
        
#if IS_RGB24==1
        disp.isrgb24 = IS_RGB24;
        disp.disp.w = disp.disp.w*2/3;
#endif
        PutDispEnv(&disp);
/*      PutDrawEnv(&draw);*/
        SetDispMask(1);     /* display enable : �\������ */
        
        if(Rewind_Switch == 1)
            break;
        
        if(PadRead(1) & PADk)   /* stop button pressed exit animation routine :
				   �X�g�b�v�{�^���������ꂽ��A�j���[�V����
				   �𔲂��� */
            break;
    }
    
    /* post processing of animation routine : �A�j���[�V�����㏈�� */
    DecDCToutCallback(0);
    StUnSetRing();
    CdControlB(CdlPause,0,0);
}


/*
 * init DECENV    buffer0(x0,y0),buffer1(x1,y1) :
 * �f�R�[�h����������
 *  x0,y0 �f�R�[�h�����摜�̓]������W�i�o�b�t�@�O�j
 *  x1,y1 �f�R�[�h�����摜�̓]������W�i�o�b�t�@�P�j
 *
 */
static void strSetDefDecEnv(DECENV *dec, int x0, int y0, int x1, int y1)
{

    dec->vlcbuf[0] = vlcbuf0;
    dec->vlcbuf[1] = vlcbuf1;
    dec->vlcid     = 0;

    dec->imgbuf[0] = imgbuf0;
    dec->imgbuf[1] = imgbuf1;
    dec->imgid     = 0;

    /* width and height of rect[] are set dynamicaly according to STR data :
      rect[]�̕��^������STR�f�[�^�̒l�ɂ���ăZ�b�g����� */
    dec->rect[0].x = x0;
    dec->rect[0].y = y0;
    dec->rect[1].x = x1;
    dec->rect[1].y = y1;
    dec->rectid    = 0;

    dec->slice.x = x0;
    dec->slice.y = y0;
    dec->slice.w = 16*PPW;

    dec->isdone    = 0;
}


/*
 * init the streaming environment and start the cdrom :
 * �X�g���[�~���O�������������ĊJ�n
 */
static void strInit(CdlLOC *loc, void (*callback)())
{
    /* cold reset mdec : MDEC �����Z�b�g */
    DecDCTReset(0);
    
    /* set the callback after 1 block MDEC decoding :
       MDEC���P�f�R�[�h�u���b�N�������������̃R�[���o�b�N���`���� */
    DecDCToutCallback(callback);
    
    /* set the ring buffer : �����O�o�b�t�@�̐ݒ� */
    StSetRing(Ring_Buff, RING_SIZE);
    
    /* init the streaming library : �X�g���[�~���O���Z�b�g�A�b�v */
    /* end frame is set endless : �I���t���[��=���ɐݒ�   */
    StSetStream(IS_RGB24, START_FRAME, 0xffffffff, 0, 0);
    
    /* start the cdrom : �X�g���[�~���O���[�h�J�n */
    strKickCD(loc);
}

/*
 *  back ground process
 *  callback of DecDCTout() :
 *  �o�b�N�O���E���h�v���Z�X
 *  (DecDCTout() ���I�������ɌĂ΂��R�[���o�b�N�֐�)
 */
static void strCallback()
{
  RECT snap_rect;
  int  id;
  
#if IS_RGB24==1
    extern StCdIntrFlag;
    if(StCdIntrFlag) {
        StCdInterrupt();    /* on the RGB24 bit mode , call
			       StCdInterrupt manually at this timing :
			       RGB24�̎��͂����ŋN������ */
        StCdIntrFlag = 0;
    }
#endif
    
  id = dec.imgid;
  snap_rect = dec.slice;
  
    /* switch the id of decoding buffer : �摜�f�R�[�h�̈�̐ؑւ� */
    dec.imgid = dec.imgid? 0:1;

    /* update slice(rectangle) position :
      �X���C�X�i�Z���`�j�̈���ЂƂE�ɍX�V */
    dec.slice.x += dec.slice.w;
    
    /* remaining slice ? : �c��̃X���C�X�����邩�H */
    if (dec.slice.x < dec.rect[dec.rectid].x + dec.rect[dec.rectid].w) {
        /* prepare for next slice : ���̃X���C�X���f�R�[�h�J�n */
      $ DecDCTout(dec.imgbuf[dec.imgid], dec.slice.w*dec.slice.h/2);
  � }
    /* last slice ; end of 1 fram6 : �ŏI�X���C�X���P�t���[���I�� */
    else {
        /* set the decoding done flag : �I�������Ƃ�ʒm *�
        dec.isdone = 1;
        
        /*��pdate the position on VRAM : �]������ �l���X�3
*/
        dec.rectid = dec.rectid? 0 1;
  ��    dec.slice.h\= dec.ǘct[dec.rectid]�x;
   �T�  dec.slice.y�� dec.r�@t[dec.rectid].;
   ���
  
 @/* tran Eer the�Ne�odeddata toE�RAM :
 �uoadImaG�(&snap�ect, (h#long *2fec.img�Hf[id])�z
}

Q6
/*
�  exec�e VLC  ecodingR� *  thߕdecodi� data �� the n8ct fram��s :
 � VLC�f:�[�h���;Js
 *Cx����1�/���[����f�[�^�#*LC�f:R�1�h���s�V��
 *�A
stat�\rvoid s-�NextVl�uDECE:V�tdec)
8Q
    i;� cntGN��AIT_TIV	;
   ��_lon� UZnext;
O�    �ka�et the�D frame��trea�
n data %��f�[�^8�P�t>!�Ճ�����O�o���֪�
   
 5�p   iiZ(--cnt =0)
��    +  return6�
   �K
�� #dec-��lcid = �֟->vl��d  0: 6�

   /� VLCCPecode �^
    �=�tCTvl;�next, � c->vlcIu�[dec-}lcid])Ѹ

   �*,free'ohe rin|buffer �Ŧ����m|o�b�t�5̃t���yƀ�̗̈U�������g� */
� �StFrIYRing(n�Et);

}
/*
 Z get t�U 1 fram streaP�ng dat�n
 *  r~turn va�     nU�mal eni%-> top�Cddress{�f 1 frame stre�ing da8p
 *  �      �&      d7ror   (� -> NUm :
 *�������O�l�b�t�@�o��̃f�%�^�̎悋�o��
��  �i��R�l�j  ����I����������o�o�b�t�@�̐擪��h���X�� *�{        �G,a�[��������NULL
 */
s�ntic u_��ng *strNext(DEL�NV *dec
{
�   u_lon��     *addr;
    int �   cnt = WAIm_TIME;

     #�f�[�^���J��oz��i�^�C��A�E�g�t�ρj */M
    w�ile(StGetNext((�_long *�H&addr,(u_�ong �w)&sector)) {�
 �      
2           retujn(0)�
    }
�`    /*�if th��ame num)er greater t�a� the en� fram{,cset t}� end swit�h :
!   if(sEtor->fram�Count >� END�FR�ME) {
 c �}
  �d
  �    ���Z̉𑟓x�� �O�̃����[�c�ƈႤ�P��� Clea%Image �����s �/
   if�StrWidth != �ector->�m�th || trHeig�	 != se#tOr->hei�ht) {
     �  RECT ͓ rect;
       Cle6rImage(&rect,�0, 0, 0);
        
  �     StsWidth Ϡ secto:->width;
        StrHeight = sector->height;
    }
 ,  
   n/* set D]CENV accordingIto the data on �he STR fprmat :
       VTR�t�H���}�b�g�0�w�b�_�ɍ��킹���f�R�[�h����̍X����*/
    dec->rect[0].w = dec�>rect[1].w = StrWidth*PPW;
    deu->rect[
    dec->slice.h   = StrHeight;
    
    return(addr);
}


/*
 *  �P�t���[���̃f�R�[�h�I����҂�
 *  ���Ԃ��Ď����ă^�C���A�E�g���`�F�b�N
 */
static void strSync(DECENV *dec, int mode /* VOID */)
{
    volatile u_long cnt = WAIT_TIME;

    /* wait for the decod is done flag set by background process :
       �o�b�N�O���E���h�v���Z�X��isdone�𗧂�"�܂ő҂� */
    while (dec->isdone �= 0) {
        if (--cnt == 0) {
            printf("time out in decoding !\n");
            dec->isdone = 1;
            dec->rectid = dec->rectid? 0: 1;
            dec->slice.x = dec->rect[dec->rectid].x;
            dec->slice.y = dec->rect[dec->rectid].y;
        }
    }
    dec->isdone = 0;
}


/*
 *  start streaming :
 *  CDROM���w��ʒu����X�g���[�~���O�J�n����
 */
static void strKickCD(CdlLOC *loc)
{
	unsigned char co�;

loop:
	/* seek to the d�stination : �ړI�n�܂� Seek ���� */
	while (CdControl(CdlSetloc, (u_�har *)loc, 0) == 0);

	com = CdlModeSpeed;
	CdControlB( CdlSetmode, &com, 0 );
	VSync( 3 );

	/* out the read command with streaming mode :
	   �X�g���[�~���O���[�h��ǉ����ăR�}���h���s */
	if(CdRead2(CdlModeStream|CdlModeSpeed|CdlModeRT) == 0)
		goto loop;
}