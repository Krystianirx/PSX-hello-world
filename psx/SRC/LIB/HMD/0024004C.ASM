/* $PSLibId: Run-time Library Release 4.4$ */

/* 
 *	Copyright(C) 1998 Sony Computer Entertainment Inc.
 *  	All rights reserved. 
 */


#include <r3000.h>
#include <asm.h>
#include <inline_s.h>
#include <gtereg_s.h>

#ifdef BACK_FACE
	.globl	GsU_0034004c		/* */
#else /* BACK_FACE */
	.globl	GsU_0024004c		/* */
#endif /* BACK_FACE */

	

	.text


/************************************************************************
* (Back-Facing) Semi-Trans Gouraud Triangle with no Lighting
*
*	a0: (GsARGUNIT *)sp
*
*	typedef struct {
*		u_long	*primp;		// 0(a0)
*		GsOT 	*tagp;		// 4(a0)
*		int	shift;		// 8(a0)
*		int	offset;		// 12(a0)
*		PACKET	*out_packetp;	// 16(a0)
*		u_long	*primtop;	// 20(a0)
*		SVECTOR	*vertop;	// 24(a0)
*		SVECTOR	*nortop;	// 28(a0)
*	}	GsSortUnitArg;
*	num:				// *(sp->primtop)>>16
*************************************************************************/

#ifdef BACK_FACE
GsU_0034004c:
#else /* BACK_FACE */
GsU_0024004c:
#endif /* BACK_FACE */

	.set    noreorder

	lw	a3,0(a0)		/* a3: primp */
	lw	t9,4(a0)		/* t9: otp; */
	lw	t8,0(a3)		/* *(sp->primp) */
	addu	a3,a3,4			/* skip size,num */
	lw	a2,0(a3)		/* prim idx:*(sp->primp) */
	lw	a1,20(a0)		/* sp->primtop */
	sll	a2,a2,2			/* idx*4 */
	srl	t8,t8,16		/* t8: n */
	lw	t7,8(a0)		/* shift */
	beq	t8,0,NG3_3		/* if(n==0) goto NG3_3: */
	addu	a3,a1,a2		/* primtop: sp->primtop+idx */

	lw	t0,4(t9)		/* t0: table */
	lw	t1,12(a0)		/* t1: otp->offset */

	addu	v0,a0,16
	lw	t9,0(v0)		/* t9: pkt_base */
	lw	a1,24(a0)		/* a1: vertop */

	lw	t2,12(a3)		/* t2: primtop->v0,v1 */
	lhu	t4,16(a3)		/* t4: primtop->v2 */
	srl	t3,t2,16		/* t3: primtop->v1 */
	sll	t2,t2,16		/* t2: primtop->v0 */
	srl	t2,t2,13		/* t2: v0*8 */
	sll	t3,t3,3			/* t3: v1*8 */
	sll	t4,t4,3			/* t4: v2*8 */

	addu	t2,t2,a1		/* t2: (vertop+primtop->v0) */
	addu	t3,t3,a1		/* t3: (vertop+primtop->v1) */
	addu	t4,t4,a1		/* t4: (vertop+primtop->v2) */

NG3_1:
	lwc2	C2_VXY0,(t2)		/* Vector 0 <--(vertop+primtop->v0) */
	lwc2	C2_VZ0,4(t2)

	lwc2	C2_VXY1,(t3)		/* Vector 1 <--(vertop+primtop->v1) */
	lwc2	C2_VZ1,4(t3)

	lwc2	C2_VXY2,(t4)		/* Vector 2 <--(vertop+primtop->v2) */
	lwc2	C2_VZ2,4(t4)
	nop
	nop

	/****/
        RTPT				
	/****/

	lw	t2,0(a3)		/* t2: primtop->rgb0 */
	lw	t3,4(a3)		/* t3: primtop->rgb1 */
	lw	t4,8(a3)		/* t4: primtop->rgb2 */
	or	t2,t2,0x02000000	/* add ABE bit */
	sw	t2,4(t9)		/* R0,G0,B0,cd -->4(pkt_base) */
	sw	t3,12(t9)		/* R1,G1,B1,cd -->12(pkt_base) */
	sw	t4,20(t9)		/* R2,G2,B2,cd -->20(pkt_base) */

	addu	a3,a3,20		/* primtop++ */

	lw	t2,12(a3)		/* t2: primtop->v0,v1 */
	lhu	t4,16(a3)		/* t4: primtop->v2 */
	srl	t3,t2,16		/* t3: primtop->v1 */
	sll	t2,t2,16		/* t2: primtop->v0 */
	srl	t2,t2,13		/* t2: v0*8 */
	sll	t3,t3,3			/* t3: v1*8 */
	sll	t4,t4,3			/* t4: v2*8 */

	addu	t2,t2,a1		/* t2: (vertop+primtop->v0) */
	addu	t3,t3,a1		/* t3: (vertop+primtop->v1) */
	addu	t4,t4,a1		/* t4: (vertop+primtop->v2) */

	cfc2	v0,C2_FLAG		/* v0: flag */
	nop
	blt	v0,0,NG3_2		/* clip if flag <0 */
	nop

#ifndef BACK_FACE
	/****/
        NCLIP				
	/****/

	mfc2	v0,C2_MAC0		/* opz read */
	nop
	ble	v0,0,NG3_2		/* Normal clip if MAC0(OPZ)<=0 */
	nop
#endif /* !BACK_FACE */

	/****/
	AVSZ3				
	/****/

	mfc2	t6,C2_OTZ		/* t6: otz */
	nop

	subu 	t6,t6,t1		/* sz = sz(otz) - otp->offset */
	srl 	t6,t6,t7		/* sz = (sz(otz) - otp->offset)>>shift; */
	and	t6,t6,0x0ffff		/* mask ? */
	sll	t6,t6,2			/* sz*4 */
	addu	t6,t6,t0		/* t6: &(table[sz]) */
	lw	t5,0(t6)		/* t5: table[sz].p */

	li	v0,0x6000000		/* (sizeof(POLY_G3)/4-1)<<24) */
	sll	v1,t5,8			
	srl	v1,v1,8			/* v1: clear (31..24) of table[sz].p */
	or	v0,v0,v1		/* v0: (table[sz].p)| */
					/* (sizeof(POLY_G3)/4-1)<<24) */
	sw	v0,0(t9) 		/* *(long *)pkt_base = v0 */
	
	xor	v0,t9,t5		/* v0 : pkt_base xor table[sz].p */
	sll	v1,v0,8			
	srl	v0,v1,8			/* v0: clear (31..24) of v0 */
	xor	v1,v0,t5		/* v1: v0 xor table[sz].p */
	sw	v1,0(t6)		/* table[sz].p = pkt_base; */

        swc2    C2_SXY0,8(t9)		/* sxy0 -->8(pkt_base) */
        swc2    C2_SXY1,16(t9)		/* sxy1 -->16(pkt_base) */
	swc2    C2_SXY2,24(t9) 		/* sxy2 -->24(pkt_base) */

	addu	t9,t9,28		/* pkt_base++; */

NG3_2:
	subu	t8,t8,1
	bne	t8,0,NG3_1		/* if(primtop != primtop+n) goto NG3_1 */
	nop

NG3_3:
	lw	v0,0(a0)		/* sp->primp */
	sw	t9,GsOUT_PACKET_P
	addu	v0,v0,8			/* sp->primp+2 */

        .set    reorder
        j       ra			/* return(primtop) */
