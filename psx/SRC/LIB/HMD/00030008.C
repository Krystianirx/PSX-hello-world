/* $PSLibId: Run-time Library Release 4.4$ */

/* 
 *	Copyright(C) 1998 Sony Computer Entertainment Inc.
 *  	All rights reserved. 
 */


#define NO_PROTOTYPE

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libhmd.h>

typedef struct {
	u_char  r0, g0, b0, code;
	u_short n0, v0;
	u_short v1, v2;
}       HMD_P_F3;

/*
 * Divide Fog Flat Triangle (GsUDFF3)
 */
u_long *GsU_00030008(GsARGUNIT *sp)
{
	GsARGUNIT_NORMAL	*ap = (GsARGUNIT_NORMAL *)sp;
	POLY_F3			*pp;
	HMD_P_F3		*tp;
	DIVPOLYGON3		*divp = (DIVPOLYGON3 *)(&(ap->nortop)+1);
  	RVECTOR			*r0, *r1, *r2;
	SVECTOR			*vp = ap->vertop;
	SVECTOR			*np = ap->nortop;
  	int			i;
  	u_short			sz;
  	long			p, flag;
  	CRVECTOR3		*crv;
  	long			lv;
	int			num;

	pp = (POLY_F3 *)ap->out_packetp;
	num = *(ap->primp)>>16;

	ap->primp++;
	tp = (HMD_P_F3 *)(ap->primtop+(*(ap->primp)&0x00ffffff));

	divp->ndiv = *(ap->primp)>>24;
	divp->pih = HWD0;
	divp->piv = VWD0;

  	r0= &divp->r0;
  	r1= &divp->r1;
  	r2= &divp->r2;
  	lv=0;
  	crv = divp->cr;
  	divp->cr[lv].r0 = r0;		
  	divp->cr[lv].r1 = r1;		
  	divp->cr[lv].r2 = r2;		
  
  	for (i = 0; i < num; i++, tp++) {
		r0->v = *(vp + tp->v0);
		r1->v = *(vp + tp->v1);
		r2->v = *(vp + tp->v2);
		if (RotAverageNclipColorDpq3(&r0->v, &r1->v, &r2->v,
	  	    np+tp->n0, np+tp->n0, np+tp->n0, &tp->r0, 
		    &r0->sxy, &r1->sxy, &r2->sxy, 
		    &divp->rgbc, &r1->c,  &r2->c, &sz, &flag) > 0) {
	  		ReadSZfifo3(&r0->sz, &r1->sz, &r2->sz);
	  		divp->ot = (u_long *)(ap->tagp->org + 
				((sz - ap->offset) >> ap->shift));
	  		divp->rgbc.cd = tp->code;
	  		pp =(POLY_F3 *)RCpolyF3A(pp, divp, lv, crv);		
		}
    	}
	GsOUT_PACKET_P = (PACKET *)pp;
  	return(ap->primp+1);
}
