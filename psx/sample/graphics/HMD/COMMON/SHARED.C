/*
 * $PSLibId: Run-time Library Release 4.4$
 */
/*
	Scanning routine for all of category 1 shared primitive types.
	Copyright (C) 1997,1998  Sony Computer Entertainment Inc.
	All rights Reserved.
*/

#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libhmd.h>
#include <stdio.h>
#include <assert.h>
#include "scan.h"

u_long
scan_shared(u_long type)
{
	/* CTG 1: SHARED */

	GsUNIT_Funcp	func = NULL;

	assert((type >> 24) == 0x01);

	switch (type) {
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(0)|PRIM_TYPE(0); */
	case 0x01000000:	func = GsU_01000000;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(0)|PRIM_TYPE(TRI|IIP); */
	case 0x0100000c:	func = GsU_0100000c;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(0)|PRIM_TYPE(TRI|IIP|TME); */
	case 0x0100000d:	func = GsU_0100000d;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(0)|PRIM_TYPE(TRI|IIP|COL); */
	case 0x0100000e:	func = GsU_0100000e;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(0)|PRIM_TYPE(TRI|IIP|COL|TME); */
	case 0x0100000f:	func = GsU_0100000f;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(0)|PRIM_TYPE(QUAD|IIP); */
	case 0x01000014:	func = GsU_01000014;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(0)|PRIM_TYPE(QUAD|IIP|TME); */
	case 0x01000015:	func = GsU_01000015;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(0)|PRIM_TYPE(QUAD|IIP|COL); */
	case 0x01000016:	func = GsU_01000016;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(0)|PRIM_TYPE(QUAD|IIP|COL|TME); */
	case 0x01000017:	func = GsU_01000017;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(0)|PRIM_TYPE(TILE|TRI|IIP|TME); */
	case 0x0100020d:	func = GsU_0100020d;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(0)|PRIM_TYPE(TILE|TRI|IIP|COL|TME); */
	case 0x0100020f:	func = GsU_0100020f;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(0)|PRIM_TYPE(TILE|QUAD|IIP|TME); */
	case 0x01000215:	func = GsU_01000215;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(0)|PRIM_TYPE(TILE|QUAD|IIP|COL|TME); */
	case 0x01000217:	func = GsU_01000217;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(FOG)|PRIM_TYPE(TRI|IIP); */
	case 0x0102000c:	func = GsU_0102000c;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(FOG)|PRIM_TYPE(TRI|IIP|TME); */
	case 0x0102000d:	func = GsU_0102000d;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(FOG)|PRIM_TYPE(TRI|IIP|COL); */
	case 0x0102000e:	func = GsU_0102000e;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(FOG)|PRIM_TYPE(TRI|IIP|COL|TME); */
	case 0x0102000f:	func = GsU_0102000f;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(FOG)|PRIM_TYPE(QUAD|IIP); */
	case 0x01020014:	func = GsU_01020014;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(FOG)|PRIM_TYPE(QUAD|IIP|TME); */
	case 0x01020015:	func = GsU_01020015;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(FOG)|PRIM_TYPE(QUAD|IIP|COL); */
	case 0x01020016:	func = GsU_01020016;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(FOG)|PRIM_TYPE(QUAD|IIP|COL|TME); */
	case 0x01020017:	func = GsU_01020017;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(FOG)|PRIM_TYPE(TILE|TRI|IIP|TME); */
	case 0x0102020d:	func = GsU_0102020d;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(FOG)|PRIM_TYPE(TILE|TRI|IIP|COL|TME); */
	case 0x0102020f:	func = GsU_0102020f;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(FOG)|PRIM_TYPE(TILE|QUAD|IIP|TME); */
	case 0x01020215:	func = GsU_01020215;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(FOG)|PRIM_TYPE(TILE|QUAD|IIP|COL|TME); */
	case 0x01020217:	func = GsU_01020217;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(LGT)|PRIM_TYPE(LMD|TRI); */
	case 0x01040048:	func = GsU_01040048;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(LGT)|PRIM_TYPE(LMD|TRI|TME); */
	case 0x01040049:	func = GsU_01040049;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(LGT)|PRIM_TYPE(LMD|TRI|IIP); */
	case 0x0104004c:	func = GsU_0104004c;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(LGT)|PRIM_TYPE(LMD|TRI|IIP|TME); */
	case 0x0104004d:	func = GsU_0104004d;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(LGT)|PRIM_TYPE(LMD|QUAD); */
	case 0x01040050:	func = GsU_01040050;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(LGT)|PRIM_TYPE(LMD|QUAD|TME); */
	case 0x01040051:	func = GsU_01040051;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(LGT)|PRIM_TYPE(LMD|QUAD|IIP); */
	case 0x01040054:	func = GsU_01040054;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(LGT)|PRIM_TYPE(LMD|QUAD|IIP|TME); */
	case 0x01040055:	func = GsU_01040055;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(LGT)|PRIM_TYPE(TILE|LMD|TRI|TME); */
	case 0x01040249:	func = GsU_01040249;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(LGT)|PRIM_TYPE(TILE|LMD|TRI|IIP|TME); */
	case 0x0104024d:	func = GsU_0104024d;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(LGT)|PRIM_TYPE(TILE|LMD|QUAD|TME); */
	case 0x01040251:	func = GsU_01040251;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(LGT)|PRIM_TYPE(TILE|LMD|QUAD|IIP|TME); */
	case 0x01040255:	func = GsU_01040255;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(BOT|LGT)|PRIM_TYPE(LMD|TRI); */
	case 0x01140048:	func = GsU_01140048;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(BOT|LGT)|PRIM_TYPE(LMD|TRI|TME); */
	case 0x01140049:	func = GsU_01140049;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(BOT|LGT)|PRIM_TYPE(LMD|TRI|IIP); */
	case 0x0114004c:	func = GsU_0114004c;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(BOT|LGT)|PRIM_TYPE(LMD|TRI|IIP|TME); */
	case 0x0114004d:	func = GsU_0114004d;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(BOT|LGT)|PRIM_TYPE(LMD|QUAD); */
	case 0x01140050:	func = GsU_01140050;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(BOT|LGT)|PRIM_TYPE(LMD|QUAD|TME); */
	case 0x01140051:	func = GsU_01140051;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(BOT|LGT)|PRIM_TYPE(LMD|QUAD|IIP); */
	case 0x01140054:	func = GsU_01140054;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(BOT|LGT)|PRIM_TYPE(LMD|QUAD|IIP|TME); */
	case 0x01140055:	func = GsU_01140055;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(BOT|LGT)|PRIM_TYPE(TILE|LMD|TRI|TME); */
	case 0x01140249:	func = GsU_01140249;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(BOT|LGT)|PRIM_TYPE(TILE|LMD|TRI|IIP|TME); */
	case 0x0114024d:	func = GsU_0114024d;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(BOT|LGT)|PRIM_TYPE(TILE|LMD|QUAD|TME); */
	case 0x01140251:	func = GsU_01140251;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(BOT|LGT)|PRIM_TYPE(TILE|LMD|QUAD|IIP|TME); */
	case 0x01140255:	func = GsU_01140255;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP)|PRIM_TYPE(TRI|IIP); */
	case 0x0120000c:	func = GsU_0120000c;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP)|PRIM_TYPE(TRI|IIP|TME); */
	case 0x0120000d:	func = GsU_0120000d;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP)|PRIM_TYPE(TRI|IIP|COL); */
	case 0x0120000e:	func = GsU_0120000e;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP)|PRIM_TYPE(TRI|IIP|COL|TME); */
	case 0x0120000f:	func = GsU_0120000f;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP)|PRIM_TYPE(QUAD|IIP); */
	case 0x01200014:	func = GsU_01200014;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP)|PRIM_TYPE(QUAD|IIP|TME); */
	case 0x01200015:	func = GsU_01200015;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP)|PRIM_TYPE(QUAD|IIP|COL); */
	case 0x01200016:	func = GsU_01200016;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP)|PRIM_TYPE(QUAD|IIP|COL|TME); */
	case 0x01200017:	func = GsU_01200017;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP)|PRIM_TYPE(TILE|TRI|IIP|TME); */
	case 0x0120020d:	func = GsU_0120020d;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP)|PRIM_TYPE(TILE|TRI|IIP|COL|TME); */
	case 0x0120020f:	func = GsU_0120020f;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP)|PRIM_TYPE(TILE|QUAD|IIP|TME); */
	case 0x01200215:	func = GsU_01200215;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP)|PRIM_TYPE(TILE|QUAD|IIP|COL|TME); */
	case 0x01200217:	func = GsU_01200217;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|LGT)|PRIM_TYPE(LMD|TRI); */
	case 0x01240048:	func = GsU_01240048;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|LGT)|PRIM_TYPE(LMD|TRI|TME); */
	case 0x01240049:	func = GsU_01240049;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|LGT)|PRIM_TYPE(LMD|TRI|IIP); */
	case 0x0124004c:	func = GsU_0124004c;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|LGT)|PRIM_TYPE(LMD|TRI|IIP|TME); */
	case 0x0124004d:	func = GsU_0124004d;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|LGT)|PRIM_TYPE(LMD|QUAD); */
	case 0x01240050:	func = GsU_01240050;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|LGT)|PRIM_TYPE(LMD|QUAD|TME); */
	case 0x01240051:	func = GsU_01240051;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|LGT)|PRIM_TYPE(LMD|QUAD|IIP); */
	case 0x01240054:	func = GsU_01240054;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|LGT)|PRIM_TYPE(LMD|QUAD|IIP|TME); */
	case 0x01240055:	func = GsU_01240055;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|LGT)|PRIM_TYPE(TILE|LMD|TRI|TME); */
	case 0x01240249:	func = GsU_01240249;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|LGT)|PRIM_TYPE(TILE|LMD|TRI|IIP|TME); */
	case 0x0124024d:	func = GsU_0124024d;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|LGT)|PRIM_TYPE(TILE|LMD|QUAD|TME); */
	case 0x01240251:	func = GsU_01240251;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|LGT)|PRIM_TYPE(TILE|LMD|QUAD|IIP|TME); */
	case 0x01240255:	func = GsU_01240255;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|BOT|LGT)|PRIM_TYPE(LMD|TRI); */
	case 0x01340048:	func = GsU_01340048;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|BOT|LGT)|PRIM_TYPE(LMD|TRI|TME); */
	case 0x01340049:	func = GsU_01340049;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|BOT|LGT)|PRIM_TYPE(LMD|TRI|IIP); */
	case 0x0134004c:	func = GsU_0134004c;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|BOT|LGT)|PRIM_TYPE(LMD|TRI|IIP|TME); */
	case 0x0134004d:	func = GsU_0134004d;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|BOT|LGT)|PRIM_TYPE(LMD|QUAD); */
	case 0x01340050:	func = GsU_01340050;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|BOT|LGT)|PRIM_TYPE(LMD|QUAD|TME); */
	case 0x01340051:	func = GsU_01340051;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|BOT|LGT)|PRIM_TYPE(LMD|QUAD|IIP); */
	case 0x01340054:	func = GsU_01340054;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|BOT|LGT)|PRIM_TYPE(LMD|QUAD|IIP|TME); */
	case 0x01340055:	func = GsU_01340055;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|BOT|LGT)|PRIM_TYPE(TILE|LMD|TRI|TME); */
	case 0x01340249:	func = GsU_01340249;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|BOT|LGT)|PRIM_TYPE(TILE|LMD|TRI|IIP|TME); */
	case 0x0134024d:	func = GsU_0134024d;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|BOT|LGT)|PRIM_TYPE(TILE|LMD|QUAD|TME); */
	case 0x01340251:	func = GsU_01340251;	break;
	/* DEV_ID(SCE)|CTG(CTG_SHARED)|DRV(STP|BOT|LGT)|PRIM_TYPE(TILE|LMD|QUAD|IIP|TME); */
	case 0x01340255:	func = GsU_01340255;	break;
	default:
		printf("unsupported SHARED primitive 0x%08x.\n", type);
	}

	return (u_long)func;
}
