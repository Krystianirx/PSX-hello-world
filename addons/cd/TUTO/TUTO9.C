/* $PSLibId: Runtime Library Release 3.6$ */
/*                      tuto9: Load and Exec
 *                      
 *      Copyright (C) 1994 by Sony Computer Entertainment
 *                      All rights Reserved
 *
 *               Version        Date            Design
 *              -----------------------------------------       
 *              1.00            May.16,1995     suzu
 *      
 *                    Load and execute programs
 *
 *      This program reads the executable data from CD-ROM and execute
 *      it as a child process. The process returns to parent when child
 *      is over. Because child program usually starts from 0x80010000,
 *      parent should be loaded not to share the same memomry area.
 *
 :      CD-ROM ��̎��s�`���v���O������ǂ݂����A�q�v���Z�X�Ƃ��ċN��
 *      ����B�q�v���Z�X�I����͐e�i���̃v���O�����j�փ��^�[������
 *      �q�v���Z�X�͑�� 0x80010000 ���烍�[�h�����̂Őe�v���O�����́A
 *      ���̃A�h���X������ă��[�h�E���s����K�v������B
 *
 */
#include <sys/types.h>
#include <libcd.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <kernel.h>

/* menu title */
char *title = "    Exec test: Use DTL-S2002    ";

/* file name table to execute: ���s����t�@�C���̃��X�g */
static char *file[] = {
	"\\EXECMENU\\RCUBE.EXE;1", 
	"\\EXECMENU\\ANIM.EXE;1",
	"\\EXECMENU\\BALLS.EXE;1", 
	0,
};

/* callback parameters */
static int      is_exec = 0;                    /* indicate executing */
static int      menu_id = -1;                   /* menu ID */
static void cbvsync(void)
{
	/* change drawing contents according to cdExec executio�.
	 : cdExec() �����s�����ǂ����ŉ�ʂ�ؑւ���
	 */
	if (is_exec == 0) {
		bgUpdate(96);
		menu_id = menuUpdate(title, file, PadRead(1));
	}
	else {
		bgUpdate(48);
		FntPrint("loading...\n");
		FntFlush(-1);
	}
}

int cdExec(char *file, int mode);


/*
 * Read ���ɉ�ʂ���~����͍̂D�܂����Ȃ��̂ŁA�ǂݍ��ݒ��ɃR�[���o�b
 * �N���ŊȒP�ȕ`���T�E���h�̍Đ����s�Ȃ�
 */
main()
{
	int     id;
	DRAWENV draw;
	DISPENV disp;
	u_long  padd;
	
	PadInit(0);
	ResetGraph(0);
	CdInit();
	
	menuInit(0, 388, 512);      /* initialize menu */
	
	/* draw background and menu system in the VSync callback
	 : VSync �̃R�[���o�b�N���ŕ`����N������
	 */
	VSyncCallback(cbvsync); 

	/* play background music by SPU
	 : �o�b�N�O���E���h�� BGM ���Đ�����
	 */
	sndInit();
	SetDispMask(1);         /* start display */
	
	/* initialize graphics */
	while (((padd = PadRead(1))&PADselect) == 0) {

		VSync(0);
		if ((id = menu_id) != -1) {
			
			/* execute: �v���O���������s���� */
			is_exec = 1;    /* notify to callback */
			cdExec(file[id], 0);
			is_exec = 0;    /* notify to callback */
			
			/* clear PAD */
			do VSync(0); while (PadRead(1));
		}
	}
	printf("ending...\n");
	sndEnd();
	CdFlush();
	ResetGraph(1);
	PadStop();
	StopCallback();
	return;
}
	

/***************************************************************************
 * 
 *CdExec        Read an executable file form CD-ROM and execute it.
 *
 * SYNOPSIS
 *      EXEC *CdExec(char *file, int mode)
 *
 * ARGUMENT
 *      file    file name of the executable module
 *      mode    execution mode (reserved)
 *
 * DESCRIPTION
 *      CdExec() reads an executable file named 'file' and execute it.
 *
 * BUGS
 *      "mode" is invalid now
 *
 * RETURN
 *      none
 *
 : CdExec       ���s�\�f�[�^�����[�h���Ď��s
 *
 * �`�� EXEC *CdExec(char *file, int mode)
 *
 * ����         file    ���s�t�@�C����
 *              mode    ���s���[�h�i�\��j
 *
 * ���         �ʒu file �����s�\�f�[�^�����[�h���āA������q�v���Z�X
 *              �Ƃ��Ď��s����B
 *
 * ���l
 *              mode �͌��݂͌��Ă��Ȃ�         
 *
 * �Ԃ�l       �Ȃ�
 *
 ***************************************************************************/

static void cdRead(CdlLOC *pos, u_long *buf, int nbyte);

int cdExec(char *file, int mode)
{
	static u_long   headbuf[2048/4];
	static struct XF_HDR *head = (struct XF_HDR *)headbuf;

	DRAWENV draw;
	DISPENV disp;
	CdlFILE fp;
	CdlLOC  p0, p1;

	/* read header of EXE file */
	printf("loading %s...\n", file);
	if (CdReadFile(file, (u_long *)head, 2048) == 0) {
		printf("%s: cannot open\n", file);
		return(0);
	}
	CdReadSync(0, 0);
	
	head->exec.s_addr = 0;
	head->exec.s_size = 0;
	
	/* load contents of EXE file to t_addr */
	CdReadFile(0, (u_long *)head->exec.t_addr, 0);
	CdReadSync(0, 0);
	printf("executing %s at %08x...\n", file, head->exec.t_addr);
	
	/* execute */
	GetDispEnv(&disp);              /* push DISPENV */
	GetDrawEnv(&draw);              /* push DRAWENV */
	SetDispMask(0);                 /* black out */
	CdSync(0, 0);                   /* terminate background command */
	CdFlush();                      /* flush CD-ROM */
	PadStop();                      /* stop PAD */
	ResetGraph(1);                  /* flush GPU */
	StopCallback();                 /* stop callback */
	Exec(&head->exec,1,0);          /* execute */
	RestartCallback();              /* recover callback */
	PadInit(0);                     /* recover PAD */
	PutDispEnv(&disp);              /* pop DISPENV */
	PutDrawEnv(&draw);              /* pop DRAWENV */
	SetDispMask(1);                 /* recover dispmask */
	
	return(head->exec.t_addr);
}