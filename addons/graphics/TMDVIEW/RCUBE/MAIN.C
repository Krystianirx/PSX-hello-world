/*
 * $PSLibId: Runtime Library Release 3.6$
 */
/*
 *	rcube: PS-X Demonstration program
 *
 *	"main.c" Main routine
 *
 *		Version 3.02	Jan, 9, 1995
 *
 *		Copyright (C) 1993,1994,1995 by Sony Computer Entertainment
 *			All rights Reserved
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include "table.h"
#include "pos.h"

/* Texture information 
 * テクスチャ情報
 */
#define TIM_ADDR 0x80020000		/* Address where target TIM file is stored 
/* 使用するTIMファイルの格納アドレス */

#define TIM_HEADER 0x00000010

/* Modeling data information 
 * モデリングデータ情報
 */
#define TMD_ADDR 0x80010000		/* Address where target TMD file is stored 
/* 使用するTMDファイルの格納アドレス */

u_long *TmdBase;			/* Address of object in TMD
/* TMDのうち、オブジェクト部のアドレス */

int CurrentTmd; 			/* TMD no. in use /* 使用中のTMD番号 */

/* Ordering table (OT)
 * オーダリングテーブル (OT)
 */
#define OT_LENGTH  7			/* OT resolution (size) /* OT解像度（大きさ） */
GsOT WorldOT[2];			/* OT info (double buffer) /* OT情報（ダブルバッファ） */
GsOT_TAG OTTags[2][1<<OT_LENGTH];	/* OT's tag area (double buffer) /* OTのタグ領域（ダブルバッファ） */

/* GPU packet generation area 
 * GPUパケット生成領域
 */
#define PACKETMAX 1000			/* Maximum number of packets per frame
/* 1フレームの最大パケット数 */

PACKET GpuPacketArea[2][PACKETMAX*64];	/* Packet area (double buffer) 
/* パケット領域（ダブルバッファ） */

/*  Object (cube) variable
 *  オブジェクト（キューブ）変数
 */
#define NCUBE 44
#define OBJMAX NCUBE
int nobj;				/* Number of cubes /* キューブ個数 */
GsDOBJ2 object[OBJMAX];			/* 3D object variable /* 3Dオブジェクト変数 */
GsCOORDINATE2 objcoord[OBJMAX];		/* Local coordinate variable 
/* ローカル座標変数 */

SVECTOR Rot[OBJMAX];			/* Rotation angle /* 回転角 */
SVECTOR RotV[OBJMAX];			/* Rotation speed (angular velocity)
/* 回転スピード（角速度） */

VECTOR Trns[OBJMAX];			/* Cube position (translatio distance)
/* キューブ位置（平行移動量） */

VECTOR TrnsV[OBJMAX];			/* Translation speed /* 移動スピード */

/*  Viewpoint (VIEW)
 *  視点（VIEW）
 */
GsRVIEW2  View;			/* Viewpoint variable /* 視点変数 */
int ViewAngleXZ;		/* Viewpoint height /* 視点の高さ */
int ViewRadial;			/* Distance from viewpoint /* 視点からの距離 */
#define DISTANCE 600		/* Initial value of radial /* Radialの初期値 */

/*  Light source
 *  光源
 */
GsF_LIGHT pslt[3];			/* Light source information variable x3
/* 光源情報変数×3 */

/*  Other
 *  その他...
 */
int Bakuhatu;				/* Explosion processing flag /* 爆発処理フラグ */
u_long PadData;				/* Control pad information /* コントロールパッドの情報 */
u_long oldpad;				/* Pad information from previous frame /* １フレーム前のパッド情報 */
GsFOGPARAM dq;				/* Parameter for depth cueing (FOG) /* デプスキュー(フォグ)用パラメータ */
int dqf;				/* FOG ON/OFF /* フォグがONかどうか */
int back_r, back_g, back_b;		/* Background color /* バックグラウンド色 */

#define FOG_R 160
#define FOG_G 170
#define FOG_B 180

/*  Function prototype declaration 
 *  関数のプロトタイプ宣言
 */
void drawCubes();
int moveCubes();
void initModelingData();
void allocCube();
void initSystem();
void initAll();
void initTexture();
void initView();
void initLight();
void changeFog();
void changeTmd();

/*  Main routine
 *  メインルーチン
 */
main()
{
	/* System initialization /* システムの初期化 */
	ResetCallback();
	initSystem();

	/* Other initialization /* その他の初期化 */
	Bakuhatu = 0;
	PadData = 0;
	CurrentTmd = 0;
	dqf = 0;
	back_r = back_g = back_b = 0;
	initView();
	initLight(0, 0xc0);
	initModelingData(TMD_ADDR);
	initTexture(TIM_ADDR);
	allocCube(NCUBE);
	
	/* Main loop /* メインループ */
	while(1) {
		if(moveCubes()==0)
		  return 0;
		GsSetRefView2(&View);
		drawCubes();
	}
}


/*  Draw 3D object (cube)
 *  3Dオブジェクト（キューブ）の描画
 */
void drawCubes()
{
	int i;
	GsDOBJ2 *objp;
	int activeBuff;
	MATRIX LsMtx;

	/* Which double buffer is active /* ダブルバッファのうちどちらがアクティブか？ */
	activeBuff = GsGetActiveBuff();

	/* Set GPU packet generation address to start of area
	/* GPUパケット生成アドレスをエリアの先頭に設定 */
	GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);

	/* Clear contents of OT /* OTの内容をクリア */
	GsClearOt(0, 0, &WorldOT[activeBuff]);

	/* Register 3D object (cube to OT)
	/* 3Dオブジェクト（キューブ）のOTへの登録 */
	objp = object;
	for(i = 0; i < nobj; i++) {

		/* Set Rotation angle to matrix /* 回転角->マトリクスにセット */
		RotMatrix(Rot+i, &(objp->coord2->coord));
		
		/* Reset flag after matrix has been updated
		/* マトリクスを更新したのでフラグをリセット */
		objp->coord2->flg = 0;

		/* Set Translation distance to matrix
		/* 平行移動量->マトリクスにセット */
		TransMatrix(&(objp->coord2->coord), &Trns[i]);
		
		/* Calculate matrix for perspective transformation and set to GTE
		/* 透視変換のためのマトリクスを計算してＧＴＥにセット */
		GsGetLs(objp->coord2, &LsMtx);
		GsSetLsMatrix(&LsMtx);
		GsSetLightMatrix(&LsMtx);

		/* Perform perspective transformation and register to OT
		/* 透視変換してOTに登録 */
		GsSortObject4(objp, &WorldOT[activeBuff], 14-OT_LENGTH,getScratchAddr(0));
		objp++;
	}

	/* Fetch pad contents for buffer /* パッドの内容をバッファに取り込む */
	oldpad = PadData;
	PadData = PadRead(1);
	
	/* Wait for V-BLANK /* V-BLANKを待つ */
	VSync(0);
	
	/* Force termination of previous frame's drawing operation
	/* 前のフレームの描画作業を強制終了 */
	ResetGraph(1);

	/* Replace double buffer /* ダブルバッファを入れ換える */
	GsSwapDispBuff();

	/* Insert screen clear command at start of OT
	/* OTの先頭に画面クリア命令を挿入 */
	GsSortClear(back_r, back_g, back_b, &WorldOT[activeBuff]);

	/* Start drawing contents of OT as background
	/* OTの内容をバックグラウンドで描画開始 */
	GsDrawOt(&WorldOT[activeBuff]);
}

/* Cube translation
 *  キューブの移動
 */
int moveCubes()
{
	int i;
	GsDOBJ2   *objp;
	
	/* exit program
	/* プログラムを終了してモニタに戻る */
	if((PadData & PADk)>0) {PadStop();ResetGraph(3);StopCallback();return 0;}
	
	/* Process according to pad value /* パッドの値によって処理 */
	if((PadData & PADLleft)>0) {
		ViewAngleXZ++;
		if(ViewAngleXZ >= 72) {
			ViewAngleXZ = 0;
		}
	}
	if((PadData & PADLright)>0) {
		ViewAngleXZ--;
		if(ViewAngleXZ < 0) {
		  ViewAngleXZ = 71;
		}
	}
	if((PadData & PADLup)>0) View.vpy += 100;
	if((PadData & PADLdown)>0) View.vpy -= 100;
	if((PadData & PADRdown)>0) {
		ViewRadial-=3;
		if(ViewRadial < 8) {
			ViewRadial = 8;
		}
	}
	if((PadData & PADRright)>0) {
		ViewRadial+=3;
		if(ViewRadial > 450) {
			ViewRadial = 450;
		}
	}
	if((PadData & PADk)>0) return(-1);
	if(((PadData & PADRleft)>0)&&((oldpad&PADRleft) == 0)) changeFog();
	if(((PadData & PADRup)>0)&&((oldpad&PADRup) == 0)) changeTmd();
	if(((PadData & PADn)>0)&&((oldpad&PADn) == 0)) Bakuhatu = 1;
	if(((PadData & PADl)>0)&&((oldpad&PADl) == 0)) allocCube(NCUBE);

	View.vpx = rotx[ViewAngleXZ]*ViewRadial;
	View.vpz = roty[ViewAngleXZ]*ViewRadial;

	/* Update cube's position information /* キューブの位置情報更新 */
	objp = object;
	for(i = 0; i < nobj; i++) {

		/* Start explosion /* 爆発の開始 */
		if(Bakuhatu == 1) {

			/* Auto-rotation speed /* 自転速度 up */
			RotV[i].vx *= 3;
			RotV[i].vy *= 3;
			RotV[i].vz *= 3;

			/* Set translation direction and speed /* 移動方向&速度設定 */
			TrnsV[i].vx = objp->coord2->coord.t[0]/4+
						(rand()-16384)/200;	
			TrnsV[i].vy = objp->coord2->coord.t[1]/6+
						(rand()-16384)/200-200;
			TrnsV[i].vz = objp->coord2->coord.t[2]/4+
						(rand()-16384)/200;
		}
		/* Processing during explosion /* 爆発中の処理 */
		else if(Bakuhatu > 1) {
			if(Trns[i].vy > 3000) {
				Trns[i].vy = 3000-(Trns[i].vy-3000)/2;
				TrnsV[i].vy = -TrnsV[i].vy*6/10;
			}
			else {
				TrnsV[i].vy += 20*2;	/* Free fall /* 自由落下 */
			}

			if((TrnsV[i].vy < 70)&&(TrnsV[i].vy > -70)&&
			   (Trns[i].vy > 2800)) {
				Trns[i].vy = 3000;
				TrnsV[i].vy = 0;

				RotV[i].vx *= 95/100;
				RotV[i].vy *= 95/100;
				RotV[i].vz *= 95/100;
			}


			TrnsV[i].vx = TrnsV[i].vx*97/100;
			TrnsV[i].vz = TrnsV[i].vz*97/100;
		}

		/* Update rotation angle /* 回転角(Rotation)の更新 */
 		Rot[i].vx += RotV[i].vx;
		Rot[i].vy += RotV[i].vy;
		Rot[i].vz += RotV[i].vz;

		/* Upadte translation distance /* 平行移動量(Transfer)の更新 */
 		Trns[i].vx += TrnsV[i].vx;
		Trns[i].vy += TrnsV[i].vy;
		Trns[i].vz += TrnsV[i].vz;

		objp++;
	}

	if(Bakuhatu == 1)
		Bakuhatu++;

	return(1);
}

/*	Map cube to initial position
 *  キューブを初期位置に配置
 */
void allocCube(n)
int n;
{	
	int x, y, z;
	int i;
	int *posp;
	GsDOBJ2 *objp;
	GsCOORDINATE2 *coordp;

	posp = cube_def_pos;
	objp = object;
	coordp = objcoord;
	nobj = 0;
	for(i = 0; i < NCUBE; i++) {

		/* Initialize object structure /* オブジェクト構造体の初期化 */
		GsLinkObject4((u_long)TmdBase, objp, CurrentTmd);
		GsInitCoordinate2(WORLD, coordp);
		objp->coord2 = coordp;
		objp->attribute = 0;
		coordp++;
		objp++;

		/* Set initial position (read from pos.h)
		/* 初期位置の設定(pos.hから読む) */
		Trns[i].vx = *posp++;
		Trns[i].vy = *posp++;
		Trns[i].vz = *posp++;
		Rot[i].vx = 0;
		Rot[i].vy = 0;
		Rot[i].vz = 0;

		/* Initialize speed /* 速度の初期化 */
		TrnsV[i].vx = 0;
		TrnsV[i].vy = 0;
		TrnsV[i].vz = 0;
		RotV[i].vx = rand()/300;
		RotV[i].vy = rand()/300;
		RotV[i].vz = rand()/300;

		nobj++;
	}
	Bakuhatu = 0;
}

/*	Initialization functions
 *  イニシャライズ関数群
 */
void initSystem()
{
	int i;

	/* Initialize pad /* パッドの初期化 */
	PadInit(0);

	/* Initialize graphic /* グラフィックの初期化 */
	GsInitGraph(640, 480, 2, 0, 0);
	GsDefDispBuff(0, 0, 0, 0);

	/* Initialize OT /* OTの初期化 */
	for(i = 0; i < 2; i++) {	
		WorldOT[i].length = OT_LENGTH;
		WorldOT[i].org = OTTags[i];
	}	

	/* Initialize 3D system /* 3Dシステムの初期化 */
	GsInit3D();
}

void initModelingData(tmdp)
u_long *tmdp;
{
	u_long size;
	int i;
	int tmdobjnum;
	
	/* Skip header /* ヘッダをスキップ */
	tmdp++;

	/* Mapping to real address /* 実アドレスへマッピング */
	GsMapModelingData(tmdp);

	tmdp++;
	tmdobjnum = *tmdp;
	tmdp++; 		/* Point to top object /* 先頭のオブジェクトをポイント */
	TmdBase = tmdp;
}

/*	Read texture (transfer to VRAM)
 *  テクスチャの読み込み（VRAMへの転送）
 */
void initTexture(tex_addr)
u_long *tex_addr;
{
	RECT rect1;
	GsIMAGE tim1;
	int i;
	
	while(1) {
		if(*tex_addr != TIM_HEADER) {
			break;
		}
		tex_addr++;	/* Skip header (1word) /* ヘッダのスキップ(1word) */
		GsGetTimInfo(tex_addr, &tim1);
		tex_addr += tim1.pw*tim1.ph/2+3+1;	/* Go to next block 
			/* 次のブロックまで進める */

		rect1.x=tim1.px;
		rect1.y=tim1.py;
		rect1.w=tim1.pw;
		rect1.h=tim1.ph;
		LoadImage(&rect1,tim1.pixel);
		if((tim1.pmode>>3)&0x01) {	/* Transfer CLUT (if there are any) 
		/* CLUTがあれば転送 */

			rect1.x=tim1.cx;
			rect1.y=tim1.cy;
			rect1.w=tim1.cw;
			rect1.h=tim1.ch;
			LoadImage(&rect1,tim1.clut);
			tex_addr += tim1.cw*tim1.ch/2+3;
		}
	}
}

/*	Initialize viewpoint
 *  視点の初期化
 */
void initView()
{
	/* Set initial position as viewpoint variable
	/* 初期位置を視点変数にセット */
	ViewAngleXZ = 54;
	ViewRadial = 75;
	View.vpx = rotx[ViewAngleXZ]*ViewRadial;
	View.vpy = -100;
	View.vpz = roty[ViewAngleXZ]*ViewRadial;
	View.vrx = 0; View.vry = 0; View.vrz = 0;
	View.rz=0;

	/* Super coordinates of viewpoint /* 視点の親座標 */
	View.super = WORLD;

	/* Setting /* 設定 */
	GsSetRefView2(&View);

	/* Projection */
	GsSetProjection(1000);

	/* Mode = 'normal lighting' */
	GsSetLightMode(0);
}

/*	Initialize light source 
 *  光源の初期化
 */
void initLight(c_mode, factor)
int c_mode;	/* Light color = wight when 0, cocktail light when 1
/* ０のとき白色光、１のときカクテルライト */
int factor;	/* Intensity factor (0 to 255) /* 明るさのファクター(0〜255) */
{
	if(c_mode == 0) {
		/* Set White light color /* 白色光のセット */
		pslt[0].vx = 200; pslt[0].vy= 200; pslt[0].vz= 300;
		pslt[0].r = factor; pslt[0].g = factor; pslt[0].b = factor;
		GsSetFlatLight(0,&pslt[0]);
		
		pslt[1].vx = -50; pslt[1].vy= -1000; pslt[1].vz= 0;
		pslt[1].r=0x20; pslt[1].g=0x20; pslt[1].b=0x20;
		GsSetFlatLight(1,&pslt[1]);
		
		pslt[2].vx = -20; pslt[2].vy= 20; pslt[2].vz= 100;
		pslt[2].r=0x0; pslt[2].g=0x0; pslt[2].b=0x0;
		GsSetFlatLight(2,&pslt[2]);
	}
	else {
		/* Cocktail light (used for Gouraud) 
		/* カクテルライト（Gouraudで使用） */
		pslt[0].vx = 200; pslt[0].vy= 100; pslt[0].vz= 0;
		pslt[0].r = factor; pslt[0].g = 0; pslt[0].b = 0;
		GsSetFlatLight(0,&pslt[0]);
		
		pslt[1].vx = -200; pslt[1].vy= 100; pslt[1].vz= 0;
		pslt[1].r=0; pslt[1].g=0; pslt[1].b=factor;
		GsSetFlatLight(1,&pslt[1]);
		
		pslt[2].vx = 0; pslt[2].vy= -200; pslt[2].vz= 0;
		pslt[2].r=0; pslt[2].g=factor; pslt[2].b=0;
		GsSetFlatLight(2,&pslt[2]);
	}	

	/* Ambient (peripheral) light /* アンビエント（周辺光） */
	GsSetAmbient(ONE/2,ONE/2,ONE/2);
}

/* FOG on/off
 * フォグのON/OFF
 */
void changeFog()
{
	if(dqf) {
		/* Reset FOG /* フォグのリセット */
		GsSetLightMode(0);
		dqf = 0;
		back_r = 0;
		back_g = 0;
		back_b = 0;
	}
	else {
		/* Set Fog /* フォグの設定 */
		dq.dqa = -600;
		dq.dqb = 5120*4096;
		dq.rfc = FOG_R;
		dq.gfc = FOG_G;
		dq.bfc = FOG_B;
		GsSetFogParam(&dq);
		GsSetLightMode(1);
		dqf = 1;
		back_r = FOG_R;
		back_g = FOG_G;
		back_b = FOG_B;
	}
}

/*	Switch TMD data
 *  TMDデータの切り替え
 */
void changeTmd()
{
	u_long *tmdp;
	GsDOBJ2 *objp;
	int i;

	/* Switch TMD /* TMDを切り替え */
	CurrentTmd++;
	if(CurrentTmd == 4) {
		CurrentTmd = 0;
	}
	objp = object;
	for(i = 0; i < nobj; i++) {
		GsLinkObject4((u_long)TmdBase, objp, CurrentTmd);
		objp++;
	}

	/* Switch light source color/intensity according to type of TMD
	/* TMDの種類にあわせて光源の色/明るさを切り替え */
	switch(CurrentTmd) {
	    case 0:
                /* Normal (flat) /* ノーマル (flat) */
		initLight(0, 0xc0);
		break;
	    case 1:
	        /* Opaque (flat) /* 半透明 (flat) */
		initLight(0, 0xc0);
		break;
	    case 2:
	        /* Gouraud */
		initLight(1, 0xff);
		break;
	    case 3:
	        /* Textured /* テクスチャ付き */
		initLight(0, 0xff);
		break;
	}
}

