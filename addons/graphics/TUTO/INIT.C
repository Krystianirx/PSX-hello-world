/* $PSLibId: Runtime Library Release 3.6$ */
/*				init
 *
 *         Copyright (C) 1993-1995 by Sony Computer Entertainment
 *			All rights Reserved
 *
 *		      Initialize graphic environment
 :		 �O���t�B�b�N��������������
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

/* int x, y;	GTE offset		: GTE �I�t�Z�b�g	*/
/* int z;	distance to the screen	: �X�N���[���܂ł̋���	*/
/* int level;	debug level		: �f�o�b�O���x��	*/

void init_system(int x, int y, int z, int level)
{
	/* initialize controller: �R���g���[���̃��Z�b�g */
	PadInit(0);             
	
	/* reset graphic subsystem: �`��E�\�����̃��Z�b�g */
	ResetGraph(0);		
	FntLoad(960, 256);	
	SetDumpFnt(FntOpen(32, 32, 320, 64, 0, 512));
	
	/* set debug mode (0:off, 1:monitor, 2:dump) 
	 : �O���t�B�b�N�V�X�e���̃f�o�b�O (0:�Ȃ�, 1:�`�F�b�N, 2:�v�����g) */
	SetGraphDebug(level);	

	/* initialize geometry subsystem: �f�s�d�̏����� */
	InitGeom();			
	
	/* set geometry origin as (160, 120): �I�t�Z�b�g�̐ݒ� */
	SetGeomOffset(x, y);	
	
	/* distance to veiwing-screen: ���_����X�N���[���܂ł̋����̐ݒ� */
	SetGeomScreen(z);		
}