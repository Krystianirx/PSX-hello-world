/*****************************************************************
 * -*- c -*-
 * $RCSfile: tuto2.c,v $
 *
 * Copyright (C) 1994 by Sony Computer Entertainment Inc.
 * All Rights Reserved.
 *
 * Sony Computer Entertainment Inc. R & D Division
 *
 *****************************************************************/
/*
 * $PSLibId: Runtime Library Release 3.6$
 */

#ifndef lint
static char rcsid [] = "$Id: tuto2.c,v 1.7 1996/05/21 07:59:57 hatto Exp $ : \
	Copyright (C) by 1994 Sony Computer Entertainment Inc.";
#endif

#include <r3000.h>
#include <asm.h>
#include <kernel.h>

#include <sys/types.h>
#include <libetc.h>
#include <libspu.h>
#ifdef __psx__
#include <libsn.h>
#else
#define pollhost()
#endif

/* #define DEBUG /**/

#ifdef DEBUG
#define PRINTF(x) printf x
#else
#define PRINTF(x)
#endif

int quitF;

#ifndef True
#define True 1
#endif
#ifndef False
#define False 0
#endif

#define SIN_DATA_SIZE (0x10 * 10)
unsigned char sin_wave [] = {
#include "sin.h"
};

void padHandle (unsigned long);

#define MALLOC_MAX 1
char spu_malloc_rec [SPU_MALLOC_RECSIZ * (MALLOC_MAX + 1)];

main (void)
{
    unsigned long addr;
    unsigned long size;
    SpuVoiceAttr s_attr;
    SpuCommonAttr c_attr;

    long top;

    /* ----------------------------------------------------------------
     *		���荞�݊��̏����� /
     *		Initialize interrupt environment.
     * ---------------------------------------------------------------- */

    ResetCallback();

    /* ----------------------------------------------------------------
     *		�O���t�B�b�N�X�̏����� /
     *		Initialize Graphics system
     * ---------------------------------------------------------------- */

    ResetGraph (0);

    /* ----------------------------------------------------------------
     *		SPU �̏����� /
     *		Initialize SPU
     * ---------------------------------------------------------------- */

    SpuInit ();
    SpuInitMalloc (MALLOC_MAX, spu_malloc_rec);

    /* ----------------------------------------------------------------
     *		�R���g���[���p�b�h �̏����� /
     *		Initialize Control pad 
     * ---------------------------------------------------------------- */

    PadInit (0);

    /* ----------------------------------------------------------------
     *		���ʑ����ݒ�
     *		:Set common attributes
     * ---------------------------------------------------------------- */

    c_attr.mask = (SPU_COMMON_MVOLL |
		   SPU_COMMON_MVOLR);

    c_attr.mvol.left  = 0x3fff;
    c_attr.mvol.right = 0x3fff;

    SpuSetCommonAttr (&c_attr);

    /* ----------------------------------------------------------------
     *		�g�`�]��
     *		:Transfer waveforms
     * ---------------------------------------------------------------- */

    /* �]�����[�h */
    /* :Transfer mode */
    SpuSetTransMode (SpuTransferByDMA);

    top = SpuMalloc (SIN_DATA_SIZE);

    /* �]���擪�A�h���X */
    /* :Transfer starting address */
    addr = SpuSetTransferStartAddr (top);
    PRINTF (("Set start addr    : %08x\n", top));
    PRINTF (("Return start addr : %08x\n", addr));
    PRINTF (("Get start addr    : %08x\n", SpuGetTransferStartAddr ()));
    
    /* �]�� */
    /* :Transfer */
    size = SpuWrite (sin_wave, SIN_DATA_SIZE);
    SpuIsTransferCompleted (SPU_TRANSFER_WAIT);

    PRINTF (("Send size   : %08x\n", SIN_DATA_SIZE));
    PRINTF (("Return size : %08x\n", size));

    /* ----------------------------------------------------------------
     *		�{�C�X�����ݒ�
     *		:Set voice attributes
     * ---------------------------------------------------------------- */

    /* �{�C�X���� */
    /* :Voice attributes */
    s_attr.mask = (SPU_VOICE_VOLL |
		   SPU_VOICE_VOLR |
		   SPU_VOICE_PITCH |
		   SPU_VOICE_WDSA |
		   SPU_VOICE_ADSR_AMODE |
		   SPU_VOICE_ADSR_SMODE |
		   SPU_VOICE_ADSR_RMODE |
		   SPU_VOICE_ADSR_AR |
		   SPU_VOICE_ADSR_DR |
		   SPU_VOICE_ADSR_SR |
		   SPU_VOICE_ADSR_RR |
		   SPU_VOICE_ADSR_SL
		   );

    s_attr.voice = (SPU_0CH);

    s_attr.volume.left  = 0x3fff;
    s_attr.volume.right = 0x3fff;
    s_attr.pitch        = 0x1000;
    s_attr.addr         = top;
    s_attr.a_mode       = SPU_VOICE_LINEARIncN;
    s_attr.s_mode       = SPU_VOICE_LINEARIncN;
    s_attr.r_mode       = SPU_VOICE_LINEARDecN;
    s_attr.ar           = 0x0;
    s_attr.dr           = 0x0;
    s_attr.sr           = 0x0;
    s_attr.rr           = 0x0;
    s_attr.sl           = 0xf;

    PRINTF (("set voice attr:\n"));
    SpuSetVoiceAttr (&s_attr);

    /* ----------------------------------------------------------------
     *		����
     *		:Key On
     * ---------------------------------------------------------------- */

    SpuSetKey (SPU_ON, (SPU_0CH));

    quitF = False;
    while (! quitF) {
	VSync (0);
#if 0
	pollhost ();
#endif
	padHandle (PadRead (0));
    }

    /* ----------------------------------------------------------------
     *		TEST: �~���[�g
     *		:TEST: Mute On/Off
     * ---------------------------------------------------------------- */

    SpuSetMute (SPU_ON);
#ifdef DEBUG
    switch (SpuGetMute ()) {
    case SPU_ON:
	PRINTF (("Mute : On\n"));
	break;
    case SPU_OFF:
	PRINTF (("Mute : Off\n"));
	break;
    default:
	PRINTF (("???\n"));
	break;
    }
#endif /* DEBUG */

    quitF = False;
    while (! quitF) {
	VSync (0);
#if 0
	pollhost ();
#endif
	padHandle (PadRead (0));
    }

    SpuSetMute (SPU_OFF);

#ifdef DEBUG
    switch (SpuGetMute ()) {
    case SPU_ON:
	PRINTF (("Mute : On\n"));
	break;
    case SPU_OFF:
	PRINTF (("Mute : Off\n"));
	break;
    default:
	PRINTF (("???\n"));
	break;
    }
#endif /* DEBUG */

    quitF = False;
    while (! quitF) {
	VSync (0);
#if 0
	pollhost ();
#endif
	padHandle (PadRead (0));
    }

    /* ----------------------------------------------------------------
     *		�I������ /
     *		Finalize this sample
     * ---------------------------------------------------------------- */

    SpuSetKey (SPU_OFF, SPU_ALLCH);
    SpuQuit ();

    PadStop ();		/* pad stop */

    ResetGraph (3);
    StopCallback ();

    return;
}

void
padHandle (unsigned long padd)
{
    static int key_h = False;

    if (padd & PADh) {
	if ((key_h == False)) {
	    key_h = True;
	}
    } else {
	if (key_h == True) {
	    key_h = False;
	    quitF = True;
	}
    }
}

/* ----------------------------------------------------------------
 *	End on File
 * ---------------------------------------------------------------- */
/* DON'T ADD STUFF AFTER THIS */