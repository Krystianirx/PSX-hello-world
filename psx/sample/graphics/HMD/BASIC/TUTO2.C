/* $PSLibId: Run-time Library Release 4.4$ */
/*
 *	"tuto2.c" main routine (shuttle)
 *
 *		Copyright (C) 1997  Sony Computer Entertainment
 *		All rights Reserved
 *
 *	This program works with data/hmd/scei/basic/shuttle.hmd
 *
 */

/*#define DEBUG		/**/

#include <sys/types.h>

#include <libetc.h>		/* for Control Pad */
#include <libgte.h>		/* LIBGS uses libgte */
#include <libgpu.h>		/* LIBGS uses libgpu */
#include <libgs.h>		/* for LIBGS */
#include <libhmd.h>             /* for LIBHMD */
#include <stdio.h>

#define OBJECTMAX 100		/* Max Objects */

#define MODEL_ADDR 0x80010000	/* Top Address of modeling data (HMD FORMAT) */

#define OT_LENGTH 12		/* Area of OT */

#define PACKETMAX (10000*24)

#define S_MAXID		9		/* shuttle max number of ID */
#define S_BODY		0		/* shuttle body ID */
#define S_HATCHR	1		/* shuttle right hatch ID */
#define S_HATCHL	2		/* shuttle left hatch ID */
#define S_SAT		3		/* shuttle satellite ID */
#define S_WING0		4		/* shuttle satellite wing 0 ID */
#define S_WING1		5		/* shuttle satellite wing 1 ID */
#define S_WING2		6		/* shuttle satellite wing 2 ID */
#define S_WING3		7		/* shuttle satellite wing 3 ID */
#define S_WING4		8		/* shuttle satellite wing 4 ID */
#define S_WING5		9		/* shuttle satellite wing 5 ID */

GsOT            Wot[2];		/* Handler of OT
                                   Uses 2 Hander for Dowble buffer */

GsOT_TAG	zsorttable[2][1<<OT_LENGTH];
				/* Area of OT */

GsUNIT		object[OBJECTMAX];
				/* Array of Object Handler */

u_long		bnum;		/* valibable of number of Objects */

GsCOORDUNIT	*DModel = NULL;	/* Coordinate for GsUNIT */

/* work short vector for making Coordinate parmeter */

GsRVIEWUNIT	view;		/* View Point Handler */
GsF_LIGHT	pslt[3];	/* Flat Lighting Handler */
u_long		padd;		/* Controler data */

PACKET          out_packet[2][PACKETMAX];
				/* GPU PACKETS AREA */

/* 
 * prototype
 */
void init_all(void);
int  obj_interactive(void);
void set_coordinate(GsCOORDUNIT *coor);
void set_coordinate2(GsCOORDUNIT *coor);
void model_init(void);
void view_init(void);
void light_init(void);

/************* MAIN START ******************************************/
main()
{
	GsUNIT	*op;		/* pointer of Object Handler */
	int	outbuf_idx;
	MATRIX	tmpls;
	int	i;
	
	ResetCallback();
	init_all();

	FntLoad(960, 256);
	FntOpen(-300, -200, 256, 200, 0, 512);

	GsInitVcount();
	while(1) {
		/* interactive parameter get */
		if(obj_interactive() == 0) {
			common_PadStop();
			ResetGraph(3);
			StopCallback();
			return 0;
		}
		GsSetRefViewUnit(&view);/* Calculate World-Screen Matrix */
		outbuf_idx=GsGetActiveBuff();
					/* Get double buffer index */
		GsSetWorkBase((PACKET*)out_packet[outbuf_idx]);
			
		GsClearOt(0, 0, &Wot[outbuf_idx]);
					/* Clear OT for using buffer */
			
		for(i=0,op=object; i<bnum; i++,op++) {
			if (op->primtop == NULL)
				continue;
			if (op->coord) {
				/* Calculate Local-World Matrix */
				GsGetLwUnit(op->coord, &tmpls);
	
				/* Set LWMATRIX to GTE Lighting Registers */
				GsSetLightMatrix(&tmpls);
	
				/* Set LSAMTRIX to GTE Registers */
				GsGetLsUnit(op->coord, &tmpls);
	
				/* Set LSMATRIX to GTE Lighting Registers */
				GsSetLsMatrix(&tmpls);
			}

			/* Perspective Translate Object and Set OT */
			GsSortUnit(op, &Wot[outbuf_idx], getScratchAddr(0));
		}
		padd = common_PadRead();/* Readint Control Pad data */
		FntPrint("Hcount = %d\n", GsGetVcount()); /**/
		VSync(0);		/* Wait for VSYNC */
		GsClearVcount();
		ResetGraph(1);		/* Reset GPU */
		GsSwapDispBuff();	/* Swap double buffer */
		/* Set SCREEN CLESR PACKET to top of OT */
		GsSortClear(0x0, 0x0, 0x0, &Wot[outbuf_idx]);

		/* Drawing OT */
		GsDrawOt(&Wot[outbuf_idx]);

		FntFlush(-1);
	}

	return 0;
}

int obj_interactive()
{
	int i;

	if (padd & PADRleft)	DModel[S_BODY].rot.vy += 5*ONE/360;
	if (padd & PADRright)	DModel[S_BODY].rot.vy -= 5*ONE/360;
	if (padd & PADRup)	DModel[S_BODY].rot.vx += 5*ONE/360;
	if (padd & PADRdown)	DModel[S_BODY].rot.vx -= 5*ONE/360;
	
	if (padd & PADLleft)	DModel[S_BODY].matrix.t[0] -= 10;
	if (padd & PADLright)	DModel[S_BODY].matrix.t[0] += 10;
	if (padd & PADLup)	DModel[S_BODY].matrix.t[1] -= 10;
	if (padd & PADLdown)	DModel[S_BODY].matrix.t[1] += 10;

	if (padd & PADstart)	DModel[S_BODY].matrix.t[2] -= 50;
	if (padd & PADselect)	DModel[S_BODY].matrix.t[2] += 50;

	if (padd & PADL1) {
		DModel[S_HATCHR].rot.vz += 2*ONE/360;
		DModel[S_HATCHL].rot.vz += 2*ONE/360;
		set_coordinate(&DModel[S_HATCHR]);
		set_coordinate(&DModel[S_HATCHL]);
	}
	if (padd & PADR1) {
		DModel[S_HATCHR].rot.vz -= 2*ONE/360;
		DModel[S_HATCHL].rot.vz -= 2*ONE/360;
		set_coordinate(&DModel[S_HATCHR]);
		set_coordinate(&DModel[S_HATCHL]);
	}

	if (padd & PADL2) {
		DModel[S_SAT].matrix.t[1] -= 10;
		DModel[S_SAT].rot.vy += 2*ONE/360;
		set_coordinate(&DModel[S_SAT]);
		for (i = 0; i < 6; i++) {
			DModel[S_WING0+i].rot.vx -= 1*ONE/360;
			set_coordinate2(&DModel[S_WING0+i]);
		}
	}
	if (padd & PADR2) {
		DModel[S_SAT].matrix.t[1] += 10;
		DModel[S_SAT].rot.vy -= 2*ONE/360;
		set_coordinate(&DModel[S_SAT]);
		for (i = 0; i < 6; i++) {
			DModel[S_WING0+i].rot.vx += 1*ONE/360;
			set_coordinate2(&DModel[S_WING0+i]);
		}
	}

	if ((padd & PADselect) && (padd & PADstart)) {
		return(0);
	}
	
	set_coordinate(&DModel[S_BODY]);

	return(1);
}


void init_all(void)		/* Initialize routine */
{
	ResetGraph(0);		/* reset GPU */
	common_PadInit();	/* Reset Controller */
	padd = 0;		/* controller value initialize */
	GsInitGraph(640, 480, 5, 1, 0);
				/* rezolution set , non interrace mode */
	GsDefDispBuff(0, 0, 0, 0);
				/* Double buffer setting */

	GsInit3D();		/* Init 3D system */
	
	Wot[0].length = OT_LENGTH;/* Set bit length of OT handler */
	Wot[0].org = zsorttable[0];
				/* Set Top address of OT Area to OT handler */
	/* same setting for anoter OT handler */
	Wot[1].length = OT_LENGTH;
	Wot[1].org = zsorttable[1];

	model_init();		/* Reading modeling data */
	view_init();		/* Setting view point */
	light_init();		/* Setting Flat Light */
}

void view_init(void)		/* Setting view point */
{
	/*---- Set projection,view ----*/
	GsSetProjection(1000);	/* Set projection */
	
	/* Setting view point location */
	view.vpx = 0; view.vpy = 0; view.vpz = -2000;

	/* Setting focus point location */
	view.vrx = 0; view.vry = 0; view.vrz = 0;

	/* Setting bank of SCREEN */
	view.rz=0;

	/* Setting parent of viewing coordinate */	
	view.super = WORLD;

	/* Calculate World-Screen Matrix from viewing paramter */
	GsSetRefViewUnit(&view);
}


void light_init(void)		/* init Flat light */
{
	/* Setting Light ID 0 */	
	/* Setting direction vector of Light0 */
	pslt[0].vx = 100; pslt[0].vy = 100; pslt[0].vz = 100;

	/* Setting color of Light0 */
	pslt[0].r = 0xd0; pslt[0].g = 0xd0; pslt[0].b = 0xd0;

	/* Set Light0 from parameters */
	GsSetFlatLight(0, &pslt[0]);
	
	/* Setting Light ID 1 */
	pslt[1].vx = 20; pslt[1].vy = -50; pslt[1].vz = -100;
	pslt[1].r = 0x80; pslt[1].g = 0x80; pslt[1].b = 0x80;
	GsSetFlatLight(1, &pslt[1]);
	
	/* Setting Light ID 2 */
	pslt[2].vx = -20; pslt[2].vy = 20; pslt[2].vz = 100;
	pslt[2].r = 0x60; pslt[2].g = 0x60; pslt[2].b = 0x60;
	GsSetFlatLight(2, &pslt[2]);
	
	/* Setting Ambient */
	GsSetAmbient(0, 0, 0);
	
	/* Setting default light mode */	
	GsSetLightMode(0);
}

/* Set coordinte parameter from work vector */
void set_coordinate(GsCOORDUNIT *coor)
{
	/* Rotate Matrix */
	RotMatrix(&coor->rot, &coor->matrix);

	/* Clear flag becase of changing parameter */
	coor->flg = 0;
}

/* Set coordinte parameter from work vector */
void set_coordinate2(GsCOORDUNIT *coor)
{
	/* Rotate Matrix */
	RotMatrixYXZ(&coor->rot, &coor->matrix);

	/* Clear flag becase of changing parameter */
	coor->flg = 0;
}

/* Read modeling data (HMD FORMAT) */
void model_init(void)
{
	u_long	*dop;
	int	i;

	dop = (u_long *)MODEL_ADDR;
				/* Top Address of MODELING DATA(HMD FORMAT) */
	GsMapUnit(dop);		/* Mappipng real address */
	dop++;			/* ID skip */
	dop++;			/* flag skip */
	dop++;			/* headder top skip */
	bnum = *dop;		/* Get number of Blocks */
	dop++;			/* skip block number */

	for (i = 0; i < bnum; i++) {
		GsUNIT	*objp = &object[i];
		GsTYPEUNIT	ut;

		objp->primtop = (u_long *)dop[i];
		if (objp->primtop == NULL)
			continue;

		GsScanUnit(objp->primtop, 0, 0, 0);
		while(GsScanUnit(0, &ut, &Wot[0], getScratchAddr(0))) {
			if (((ut.type>>24 == 0x00) || (ut.type>>24 == 0x01)) &&
			    (ut.type & 0x00800000)) {
				DModel = GsMapCoordUnit((u_long *)MODEL_ADDR, 
						ut.ptr);
				ut.type &= (~0x00800000);
			}
			switch(ut.type) {
			case 0:
#ifdef DEBUG
				printf("DEBUG:pointer is already set\n");
#endif /* DEBUG */
				break;
			case GsUF3:
				*(ut.ptr) = (u_long)GsU_00000008;
				break;
			case GsUG3:
				*(ut.ptr) = (u_long)GsU_0000000c;
				break;
			default:
				*(ut.ptr) = (u_long)GsU_00000000;
				printf("Unknown type:0x%08x\n",
						ut.type);
				break;
			}
#ifdef DEBUG
			printf("DEBUG:block:%d, Code:0x%08x\n", i, ut.type);
#endif /* DEBUG */
		}
		objp->coord = NULL;
	}

	if (DModel != NULL) {
		for (i = 1; i < bnum - 1; i++) {
			object[i].coord = &DModel[i - 1];
		}
	}
}
