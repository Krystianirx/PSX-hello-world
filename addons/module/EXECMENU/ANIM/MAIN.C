/*
 * $PSLibId: Runtime Library Release 3.6$
 */
/*
 *          Movie Sample Program
 *
 *      Copyright (C) 1994,5 by Sony Corporation
 *          All rights Reserved
 *
 *  CD-ROM からムービーをストリーミングするサンプル
 *
 *   Version    Date
 *  ------------------------------------------
 *  1.00        Jul,14,1994     yutaka
 *  1.10        Sep,01,1994     suzu
 *  1.20        Oct,24,1994     yutaka(anim subroutine化)
 *  1.30        Jun,02,1995     yutaka(後処理)
 *  1.40        Jul,10,1995     masa(imgbufダブルバッファ化)
 *  1.50        Jul,20,1995     ume(画面クリア改良)
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
 *  デコードする色数の指定(16bit/24bit)
 */
#define IS_RGB24    1   /* 0:RGB16, 1:RGB24 */
#if IS_RGB24==1
#define PPW 3/2     /* pixel/short word :
		       １ショートワードに何ピクセルあるか  */
#define DCT_MODE    3   /* 24bit decode : 24bit モードでデコード */
#else
#define PPW 1       /* pixel/short word :
		      １ショートワードに何ピクセルあるか */
#define DCT_MODE    2   /* 16 bit decode : 16bit モードでデコード */
#endif

/*
 *  decode environment : デコード環境変数
 */
typedef struct {
    u_long  *vlcbuf[2]; /* VLC buffer （double） */
    int vlcid;      /* current decode buffer id :
		       現在 VLC デコード中バッファの ID */
    u_short *imgbuf[2]; /* decode image buffer （double）*/
    int imgid;      /* corrently use buffer id :
		       現在使用中の画像バッファのID */
    RECT    rect[2];    /* double buffer orign(upper left point) address
			   on the VRAM (double buffer) :
			   VRAM上座標値（ダブルバッファ） */
    int rectid;     /* currently translating buffer id :
		       現在転送中のバッファ ID */
    RECT    slice;      /* the region decoded by once DecDCTout() :
			   １回の DecDCTout で取り出す領域 */
    int isdone;     /* the flag of decoding whole frame :
		       １フレーム分のデータができたか */
} DECENV;
static DECENV   dec;        /* instance of DECENV : デコード環境の実体 */

/*
 *  Ring buffer for STREAMING
 *  minmum size is two frame :
 *  ストリーミング用リングバッファ
 *  CD-ROMからのデータをストック
 *  最低２フレーム分の容量を確保する。
 */
#define RING_SIZE   32      /* 32 sectors : 単位はセクタ */
static u_long   Ring_Buff[RING_SIZE*SECTOR_SIZE];

/*
 *  VLC buffer(double buffer)
 *  stock the result of VLC decode :
 *  VLCバッファ（ダブルバッファ）
 *  VLCデコード後の中間データをストック
 */
#define VLC_BUFF_SIZE 320/2*256     /* not correct value :
				       とりあえず充分な大きさ */
static u_long   vlcbuf0[VLC_BUFF_SIZE];
static u_long   vlcbuf1[VLC_BUFF_SIZE];

/*
 *  image buffer(double buffer)
 *  stock the result of MDEC
 *  rectangle of 16(width) by XX(height) :
 *  イメージバッファ（ダブルバッファ）
 *  DCTデコード後のイメージデータをストック
 *  横幅16ピクセルの矩形毎にデコード＆転送
 */
#define SLICE_IMG_SIZE 16*PPW*SCR_HEIGHT
static u_short  imgbuf0[SLICE_IMG_SIZE];
static u_short  imgbuf1[SLICE_IMG_SIZE];
    
/*
 *  その他の変数
 */
static int  StrWidth  = 0;  /* resolution of movie :
			       ムービー画像の大きさ（横と縦） */
static int  StrHeight = 0;  
static int  Rewind_Switch;  /* the end flag set after last frame :
			       終了フラグ 所定のフレームまで再生すると１になる */

/*
 *  prototypes :
 *  関数のプロトタイプ宣言
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
    
    anim();     /* animation subroutine : アニメーションサブルーチン */

    PadStop();
    ResetGraph(3); 
    StopCallback();
#ifdef DEAD_PARENT
    /* load parent again, because parent was destroyed already. :
       既に親は死んでいるので、もう一度親をLoadExec()する */
    _96_init();
    LoadExec("cdrom:\\EXECMENU\\EXECMENU.EXE;1",0x801ffff0,0);
    /* this setting of stack pointer is meaningless, because EXECMENU.EXE
       was linked with 2MBYTE.OBJ. :
       EXECMENU.EXE には 2MBYTE.OBJ がリンクされているので、ここでのスタック
       ポインタ初期値の設定は意味を持たないが、一応 0x801ffff0 としている。*/
#else
    return(0);
#endif
}


/*
 *  animation subroutine forground process :
 *  アニメーションサブルーチン フォアグラウンドプロセス
 */
static int anim()
{
    DISPENV disp;       /* display buffer : 表示バッファ */
    DRAWENV draw;       /* drawing buffer : 描画バッファ */
    int id;     /* display buffer id : 表示バッファの ID */
    CdlFILE file;
    
    /* search file : ファイルをサーチ */
    if (CdSearchFile(&file, FILE_NAME) == 0) {
        printf("file not found\n");
	PadStop();
	ResetGraph(3);
        StopCallback();
        return 0;
    }
    
    /* set the position of vram : VRAM上の座標値を設定 */
    strSetDefDecEnv(&dec, POS_X, POS_Y, POS_X, POS_Y+SCR_HEIGHT);

    /* init streaming system & kick cd : ストリーミング初期化＆開始 */
    strInit(&file.pos, strCallback);
    
    /* VLC decode the first frame : 最初のフレームの VLCデコードを行なう */
    strNextVlc(&dec);
    
    Rewind_Switch = 0;
    
    while (1) {
        /* start DCT decoding the result of VLC decoded data :
	   VLCの完了したデータをDCTデコード開始（MDECへ送信） */
        DecDCTin(dec.vlcbuf[dec.vlcid], DCT_MODE);
        
        /* prepare for recieving the result of DCT decode :
	   DCTデコード結果の受信の準備をする            */
        /* next DecDCTout is called in DecDCToutCallback :
	   この後の処理はコールバックルーチンで行なう */
        DecDCTout(dec.imgbuf[dec.imgid], dec.slice.w*dec.slice.h/2);
        
        /* decode the next frame's VLC data :
	   次のフレームのデータの VLC デコード */
        strNextVlc(&dec);
        
        /* wait for whole decode process per 1 frame :
	   １フレームのデコードが終了するのを待つ */
        strSync(&dec, 0);
        
        /* wait for V-Vlank : V-BLNK を待つ */
        VSync(0);
        
        /* swap the display buffer : 表示バッファをスワップ     */
        /* notice that the display buffer is the opossite side of
	   decoding buffer :
	   表示バッファはデコード中バッファの逆であることに注意 */
        id = dec.rectid? 0: 1;
        SetDefDispEnv(&disp, 0, (id)*240, SCR_WIDTH*PPW, SCR_HEIGHT);
/*      SetDefDrawEnv(&draw, 0, (id)*240, SCR_WIDTH*PPW, SCR_HEIGHT);*/
        
#if IS_RGB24==1
        disp.isrgb24 = IS_RGB24;
        disp.disp.w = disp.disp.w*2/3;
#endif
        PutDispEnv(&disp);
/*      PutDrawEnv(&draw);*/
        SetDispMask(1);     /* display enable : 表示許可 */
        
        if(Rewind_Switch == 1)
            break;
        
        if(PadRead(1) & PADk)   /* stop button pressed exit animation routine :
				   ストップボタンが押されたらアニメーション
				   を抜ける */
            break;
    }
    
    /* post processing of animation routine : アニメーション後処理 */
    DecDCToutCallback(0);
    StUnSetRing();
    CdControlB(CdlPause,0,0);
}


/*
 * init DECENV    buffer0(x0,y0),buffer1(x1,y1) :
 * デコード環境を初期化
 *  x0,y0 デコードした画像の転送先座標（バッファ０）
 *  x1,y1 デコードした画像の転送先座標（バッファ１）
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
      rect[]の幅／高さはSTRデータの値によってセットされる */
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
 * ストリーミング環境を初期化して開始
 */
static void strInit(CdlLOC *loc, void (*callback)())
{
    /* cold reset mdec : MDEC をリセット */
    DecDCTReset(0);
    
    /* set the callback after 1 block MDEC decoding :
       MDECが１デコードブロックを処理した時のコールバックを定義する */
    DecDCToutCallback(callback);
    
    /* set the ring buffer : リングバッファの設定 */
    StSetRing(Ring_Buff, RING_SIZE);
    
    /* init the streaming library : ストリーミングをセットアップ */
    /* end frame is set endless : 終了フレーム=∞に設定   */
    StSetStream(IS_RGB24, START_FRAME, 0xffffffff, 0, 0);
    
    /* start the cdrom : ストリーミングリード開始 */
    strKickCD(loc);
}

/*
 *  back ground process
 *  callback of DecDCTout() :
 *  バックグラウンドプロセス
 *  (DecDCTout() が終った時に呼ばれるコールバック関数)
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
			       RGB24の時はここで起動する */
        StCdIntrFlag = 0;
    }
#endif
    
  id = dec.imgid;
  snap_rect = dec.slice;
  
    /* switch the id of decoding buffer : 画像デコード領域の切替え */
    dec.imgid = dec.imgid? 0:1;

    /* update slice(rectangle) position :
      スライス（短柵矩形）領域をひとつ右に更新 */
    dec.slice.x += dec.slice.w;
    
    /* remaining slice ? : 残りのスライスがあるか？ */
    if (dec.slice.x < dec.rect[dec.rectid].x + dec.rect[dec.rectid].w) {
        /* prepare for next slice : 次のスライスをデコード開始 */
      $ DecDCTout(dec.imgbuf[dec.imgid], dec.slice.w*dec.slice.h/2);
  ? }
    /* last slice ; end of 1 fram6 : 最終スライス＝１フレーム終了 */
    else {
        /* set the decoding done flag : 終ったことを通知 *ｦ
        dec.isdone = 1;
        
        /*ｿﾚpdate the position on VRAM : 転送先座? 値を更?3
*/
        dec.rectid = dec.rectid? 0 1;
  ｲｺ    dec.slice.h\= dec.ﾇ歪t[dec.rectid]』;
   ﾍT?  dec.slice.y?ﾜ dec.rｷ@t[dec.rectid].;
   ｦ楹
  
 @/* tran Eer the?Neｼodeddata toEｺRAM :?    デコード結?}をフレ<ｾムバッ?焜@に転rｬ */
 ?uoadImaGﾚ(&snap?貌ct, (h#long *2fec.imgﾞHf[id])?z
}

Q6
/*
ｷ  exec彳 VLC  ecodingR? *  thﾟ錨ecodi? data ?ﾊ the n8ct fram?Ｔ :
 ･ VLCﾇf:ードのﾆ;Js
 *Cx次の1?/ｦ戟[ム?’ータﾖ#*LCデ:Rｻ1ドを行ｶVう
 *ﾊA
stat?\rvoid s-ﾇNextVlﾁuDECE:V冲dec)
8Q
    i;? cntGN削AIT_TIV	;
   ﾛ饑lonﾔ UZnext;   staﾈｵc u_/?ng *strNt();!
O､    ｰkaﾛet theｻD frameﾅﾎtreaﾆ
n data %父fーﾞ^8１フ>!?ﾕム分取｡O出す?ﾖｪ?
   ﾚh値e ((nﾐﾃt = 師BOext(?Xc)) == 0e{
 5｝   iiZ(--cnt =0)
??    +  return6?
   規*    ??    /*zsｻ?tch uﾅ匸decoK?ng area JﾏVLCボfR[ド廉ﾛ謔ﾌ切濁てz*/
陛 #dec-入lcid = 茄?->vl綜d  0: 6ﾉ

   /? VLCCPecode ﾋ^
    ｷ=?tCTvl;渡ext, ? c->vlcIuｿ[dec-}lcid])ﾑｸ

   ?*,free'ohe rin|buffer ?ﾅｦ潟塔m|oッフ?5ﾌフレ【ﾆ??ﾌ領?UÅ解放ｉg? */
ｽ 鏨tFrIYRing(nｽEt);
8  Q ret!創;
}ﾂ
/*
 Z get t?U 1 fram streaPﾓng dat熟
 *  r~turn va?     nU?mal eni%-> topﾝCddress{?f 1 frame streﾆing da8p
 *  ﾛ      ﾏ&      d7ror   (ｼ -> NUm :
 *?ｻリングネッファﾑoらのデ･%タの取ｋ出し
?ﾒ  （返R値）  盛常終了?＝リン椀バッファの先頭?ドレス｡ｯ *ﾔ{        エ,aー発生萱＝NULL
 */
s墨tic u_次ng *strNext(DEL?NV *dec
{
?   u_lonﾎﾍ     *addr;ｰ    StHEAR    *sctorIｨ
    int ?   cnt = WAIm_TIME;
ｻ    /2 get thBｵ1 frame 頻reamxng data lithe TIME-OUT :
     #データをﾁJり出zｷ（タイ?アウト付?ﾏ） */M
    w｢ile(StGetNext((ｬ_long *唏&addr,(u_?ong 毆)&sector)) {ｬ
 ?      ` (--cnto== 0)
2           retujn(0)ﾄ
    }
ｬ`    /*吠f thﾀﾏame num)er greater t｣a? the enｬ fram{,cset t}? end swit嬋 :? ｲ     景?謔ﾌフレーム番号が指定段Ｍら終了!*/
!   if(sEtor->fram?Count >? END擢R?ME) {/     ﾚ  Rewind_Switch =ｲ?;
 c ?}
  硬ｸ    /* if the esolut%on is ifﾁer t previo不 frame, ?ear f?ﾍ?e bup~er :
  ｮ    画般Zﾌ解臓度が 前ﾁﾌ?档戟[ツと違うｧPらば Clea%Image を実行 ?/
   ifｻStrWidth != 册ctor->?m?th || trHeigｾ	 != se#tOr->heiｫht) {?       ﾚﾕ
     ･  RECT ﾍ? rect;ｳ縊      蘰tREC?(&rjct, ﾉ,%0, ScR_?IDTHｬ* PPW, SWR_HEIGgT*2);
       Cle6rImage(&rect,?0, 0, 0);
        
  ?     StsWidth ﾏ? secto:->width;
        StrHeight = sector->height;
    }
 ,  
   n/* set D]CENV accordingIto the data on ﾏhe STR fprmat :
       VTRフォ＆マット?0ヘッダ篷合わせｃデコード環境をｲﾌ更する*/
    dec->rect[0].w = dec?>rect[1].w = StrWidth*PPW;
    deu->rect[~.h = dec->recﾔ[1].h = StrHeight;
    dec->slice.h   = StrHeight;
    
    return(addr);
}


/*& *  wait for finish decodeing 1 frame with TIME-OUT :
 *  １フレームのデコード終了を待つ
 *  時間を監視してタイムアウトをチェック
 */
static void strSync(DECENV *dec, int mode /* VOID */)
{
    volatile u_long cnt = WAIT_TIME;

    /* wait for the decod is done flag set by background process :
       バックグラウンドプロセスがisdoneを立て"驍ﾜで待つ */
    while (dec->isdone ﾕ= 0) {
        if (--cnt == 0) {u            /* if timeout force to switch buffer : 強制的に切替える */
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
 *  CDROMを指?位置からストリーミング開始する
 */
static void strKickCD(CdlLOC *loc)
{
	unsigned char co?;

loop:
	/* seek to the dピtination : 目的地まで Seek する */
	while (CdControl(CdlSetloc, (u_敬ar *)loc, 0) == 0);

	com = CdlModeSpeed;
	CdControlB( CdlSetmode, &com, 0 );
	VSync( 3 );

	/* out the read command with streaming mode :
	   ストリーミングモードを追加してコマンド発行 */
	if(CdRead2(CdlModeStream|CdlModeSpeed|CdlModeRT) == 0)
		goto loop;
}
