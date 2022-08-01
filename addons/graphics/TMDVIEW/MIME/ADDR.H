/* $PSLibId: Runtime Library Release 3.6$ */
/*
 *	mime
 *
 *	"addr.h" ���������蓖��
 *
 *		Copyright (C) 1995 by Sony Computer Entertainment
 *		All rights Reserved
 */

/* Top Address of modeling data (TMD format) :
   ���f�����O�f�[�^�iTMD�t�H�[�}�b�g�j ���������A�h���X */
#define MODEL_ADDR (u_long *)0x800d0000

/* Top Address of reserved vertex data  :
   �I���W�i���̒��_�f�[�^���ޔ������A�h���X */
#define ORGVTXBUF (u_long *)0x80050000	

/* Top Address of reserved normal data :
   �I���W�i���̖@���f�[�^���ޔ������A�h���X */	
#define ORGNRMBUF (u_long *)0x80060000	

/* Top Address of vertex differencial data (VDF format) :
   ���_�����f�[�^�iVDF�t�H�[�}�b�g�j ���������A�h���X */
#define MDFDATAVTX (u_long *)0x80090000	

/* Top Address of normal differencial data (NDF format) :
   �@�������f�[�^�iNDF�t�H�[�}�b�g�j ���������A�h���X */
#define MDFDATANRM (u_long *)0x800b0000	

/* Top Address of packet data :
   �p�P�b�g�f�[�^��u���A�h���X */
#define PACKETBUF  (u_long *)0x80100000