CC	= ccpsx
DMPSX	= dmpsx
CFLAGS	= -O3

OBJS = \
	00000008.o2	\
	00000009.o2	\
	0000000a.o2	\
	0000000b.o2	\
	0000000c.o2	\
	0000000d.o2	\
	0000000e.o2	\
	0000000f.o2	\
	00000010.o2	\
	00000011.o2	\
	00000012.o2	\
	00000013.o2	\
	00000014.o2	\
	00000015.o2	\
	00000016.o2	\
	00000017.o2	\
	00000018.o2	\
	00000019.o2	\
	0000001c.o2	\
	0000001d.o2	\
	00000209.o2	\
	0000020b.o2	\
	0000020d.o2	\
	0000020f.o2	\
	00000211.o2	\
	00000213.o2	\
	00000215.o2	\
	00000217.o2	\
	00020008.o2	\
	00020009.o2	\
	0002000a.o2	\
	0002000b.o2	\
	0002000c.o2	\
	0002000d.o2	\
	0002000e.o2	\
	0002000f.o2	\
	00020010.o2	\
	00020011.o2	\
	00020012.o2	\
	00020013.o2	\
	00020014.o2	\
	00020015.o2	\
	00020016.o2	\
	00020017.o2	\
	00020209.o2	\
	0002020b.o2	\
	0002020d.o2	\
	0002020f.o2	\
	00020211.o2	\
	00020213.o2	\
	00020215.o2	\
	00020217.o2	\
	00040048.o2	\
	00040049.o2	\
	0004004c.o2	\
	0004004d.o2	\
	00040050.o2	\
	00040051.o2	\
	00040054.o2	\
	00040055.o2	\
	00040058.o2	\
	00040059.o2	\
	0004005c.o2	\
	0004005d.o2	\
	00040148.o2	\
	00040149.o2	\
	0004014c.o2	\
	0004014d.o2	\
	00040150.o2	\
	00040151.o2	\
	00040154.o2	\
	00040155.o2	\
	00040249.o2	\
	0004024d.o2	\
	00040251.o2	\
	00040255.o2	\
	00100008.o2	\
	00100009.o2	\
	0010000a.o2	\
	0010000b.o2	\
	0010000c.o2	\
	0010000d.o2	\
	0010000e.o2	\
	0010000f.o2	\
	00100010.o2	\
	00100011.o2	\
	00100012.o2	\
	00100013.o2	\
	00100014.o2	\
	00100015.o2	\
	00100016.o2	\
	00100017.o2	\
	00100209.o2	\
	0010020b.o2	\
	0010020d.o2	\
	0010020f.o2	\
	00100211.o2	\
	00100213.o2	\
	00100215.o2	\
	00100217.o2	\
	00200008.o2	\
	00200009.o2	\
	0020000c.o2	\
	0020000d.o2	\
	00200010.o2	\
	00200011.o2	\
	00200014.o2	\
	00200015.o2	\
	00200209.o2	\
	0020020d.o2	\
	00200211.o2	\
	00200215.o2	\
	00240048.o2	\
	00240049.o2	\
	0024004c.o2	\
	0024004d.o2	\
	00240050.o2	\
	00240051.o2	\
	00240054.o2	\
	00240055.o2	\
	00240249.o2	\
	0024024d.o2	\
	00240251.o2	\
	00240255.o2	\
	00300008.o2	\
	00300009.o2	\
	0030000c.o2	\
	0030000d.o2	\
	00300010.o2	\
	00300011.o2	\
	00300014.o2	\
	00300015.o2	\
	00300209.o2	\
	0030020d.o2	\
	00300211.o2	\
	00300215.o2	\
	01000000.o2	\
	0100000c.o2	\
	0100000d.o2	\
	0100000e.o2	\
	0100000f.o2	\
	01000014.o2	\
	01000015.o2	\
	01000016.o2	\
	01000017.o2	\
	0100020d.o2	\
	0100020f.o2	\
	01000215.o2	\
	01000217.o2	\
	0102000c.o2	\
	0102000d.o2	\
	0102000e.o2	\
	0102000f.o2	\
	01020014.o2	\
	01020015.o2	\
	01020016.o2	\
	01020017.o2	\
	0102020d.o2	\
	0102020f.o2	\
	01020215.o2	\
	01020217.o2	\
	01040048.o2	\
	01040049.o2	\
	0104004c.o2	\
	0104004d.o2	\
	01040050.o2	\
	01040051.o2	\
	01040054.o2	\
	01040055.o2	\
	01040249.o2	\
	0104024d.o2	\
	01040251.o2	\
	01040255.o2	\
	0120000c.o2	\
	0120000d.o2	\
	01200014.o2	\
	01200015.o2	\
	0120020d.o2	\
	01200215.o2	\
	01240048.o2	\
	01240049.o2	\
	0124004c.o2	\
	0124004d.o2	\
	01240050.o2	\
	01240051.o2	\
	01240054.o2	\
	01240055.o2	\
	01240249.o2	\
	0124024d.o2	\
	01240251.o2	\
	01240255.o2	\
	anim.obj	\
	anim2.obj	\
	bezier.obj	\
	beta.obj	\
	mime.obj	\
	00000000.obj	\
	00010008.obj	\
	00010009.obj	\
	0001000c.obj	\
	0001000d.obj	\
	00010010.obj	\
	00010011.obj	\
	00010014.obj	\
	00010015.obj	\
	00030008.obj	\
	00030009.obj	\
	0003000c.obj	\
	0003000d.obj	\
	00030010.obj	\
	00030011.obj	\
	00030014.obj	\
	00030015.obj	\
	00050048.obj	\
	00050049.obj	\
	0005004c.obj	\
	0005004d.obj	\
	00050050.obj	\
	00050051.obj	\
	00050054.obj	\
	00050055.obj	\
	02000000.obj	\
	02000001.obj	\
	03000000.obj	\
	03000001.obj	\
	03000002.obj	\
	03000003.obj	\
	03000009.obj	\
	0300000a.obj	\
	0300000b.obj	\
	03000100.obj	\
	03000901.obj	\
	03000902.obj	\
	03000909.obj	\
	0300090a.obj	\
	03010110.obj	\
	03010111.obj	\
	03010112.obj	\
	03010121.obj	\
	03010122.obj	\
	03010141.obj	\
	03010142.obj	\
	03010171.obj	\
	03010172.obj	\
	03010182.obj	\
	03010210.obj	\
	03010211.obj	\
	03010212.obj	\
	03010221.obj	\
	03010222.obj	\
	03010241.obj	\
	03010242.obj	\
	03010271.obj	\
	03010272.obj	\
	03010310.obj	\
	03010311.obj

all: $(OBJS)
	@echo done

00000008.obj: 00000008.asm
	$(CC) $(CFLAGS) -c  00000008.asm -o 00000008.obj
00000008.o2: 00000008.obj
	$(DMPSX) -o 00000008.o2 00000008.obj
00000009.obj: 00000009.asm
	$(CC) $(CFLAGS) -c  00000009.asm -o 00000009.obj
00000009.o2: 00000009.obj
	$(DMPSX) -o 00000009.o2 00000009.obj
0000000a.obj: 0000000a.asm
	$(CC) $(CFLAGS) -c  0000000a.asm -o 0000000a.obj
0000000a.o2: 0000000a.obj
	$(DMPSX) -o 0000000a.o2 0000000a.obj
0000000b.obj: 0000000b.asm
	$(CC) $(CFLAGS) -c  0000000b.asm -o 0000000b.obj
0000000b.o2: 0000000b.obj
	$(DMPSX) -o 0000000b.o2 0000000b.obj
0000000c.obj: 0000000c.asm
	$(CC) $(CFLAGS) -c  0000000c.asm -o 0000000c.obj
0000000c.o2: 0000000c.obj
	$(DMPSX) -o 0000000c.o2 0000000c.obj
0000000d.obj: 0000000d.asm
	$(CC) $(CFLAGS) -c  0000000d.asm -o 0000000d.obj
0000000d.o2: 0000000d.obj
	$(DMPSX) -o 0000000d.o2 0000000d.obj
0000000e.obj: 0000000e.asm
	$(CC) $(CFLAGS) -c  0000000e.asm -o 0000000e.obj
0000000e.o2: 0000000e.obj
	$(DMPSX) -o 0000000e.o2 0000000e.obj
0000000f.obj: 0000000f.asm
	$(CC) $(CFLAGS) -c  0000000f.asm -o 0000000f.obj
0000000f.o2: 0000000f.obj
	$(DMPSX) -o 0000000f.o2 0000000f.obj
00000010.obj: 00000010.asm
	$(CC) $(CFLAGS) -c  00000010.asm -o 00000010.obj
00000010.o2: 00000010.obj
	$(DMPSX) -o 00000010.o2 00000010.obj
00000011.obj: 00000011.asm
	$(CC) $(CFLAGS) -c  00000011.asm -o 00000011.obj
00000011.o2: 00000011.obj
	$(DMPSX) -o 00000011.o2 00000011.obj
00000012.obj: 00000012.asm
	$(CC) $(CFLAGS) -c  00000012.asm -o 00000012.obj
00000012.o2: 00000012.obj
	$(DMPSX) -o 00000012.o2 00000012.obj
00000013.obj: 00000013.asm
	$(CC) $(CFLAGS) -c  00000013.asm -o 00000013.obj
00000013.o2: 00000013.obj
	$(DMPSX) -o 00000013.o2 00000013.obj
00000014.obj: 00000014.asm
	$(CC) $(CFLAGS) -c  00000014.asm -o 00000014.obj
00000014.o2: 00000014.obj
	$(DMPSX) -o 00000014.o2 00000014.obj
00000015.obj: 00000015.asm
	$(CC) $(CFLAGS) -c  00000015.asm -o 00000015.obj
00000015.o2: 00000015.obj
	$(DMPSX) -o 00000015.o2 00000015.obj
00000016.obj: 00000016.asm
	$(CC) $(CFLAGS) -c  00000016.asm -o 00000016.obj
00000016.o2: 00000016.obj
	$(DMPSX) -o 00000016.o2 00000016.obj
00000017.obj: 00000017.asm
	$(CC) $(CFLAGS) -c  00000017.asm -o 00000017.obj
00000017.o2: 00000017.obj
	$(DMPSX) -o 00000017.o2 00000017.obj
00000018.obj: 00000018.asm
	$(CC) $(CFLAGS) -c  00000018.asm -o 00000018.obj
00000018.o2: 00000018.obj
	$(DMPSX) -o 00000018.o2 00000018.obj
00000019.obj: 00000019.asm
	$(CC) $(CFLAGS) -c  00000019.asm -o 00000019.obj
00000019.o2: 00000019.obj
	$(DMPSX) -o 00000019.o2 00000019.obj
0000001c.obj: 0000001c.asm
	$(CC) $(CFLAGS) -c  0000001c.asm -o 0000001c.obj
0000001c.o2: 0000001c.obj
	$(DMPSX) -o 0000001c.o2 0000001c.obj
0000001d.obj: 0000001d.asm
	$(CC) $(CFLAGS) -c  0000001d.asm -o 0000001d.obj
0000001d.o2: 0000001d.obj
	$(DMPSX) -o 0000001d.o2 0000001d.obj
00000209.obj: 00000209.asm
	$(CC) $(CFLAGS) -c  00000209.asm -o 00000209.obj
00000209.o2: 00000209.obj
	$(DMPSX) -o 00000209.o2 00000209.obj
0000020b.obj: 0000020b.asm
	$(CC) $(CFLAGS) -c  0000020b.asm -o 0000020b.obj
0000020b.o2: 0000020b.obj
	$(DMPSX) -o 0000020b.o2 0000020b.obj
0000020d.obj: 0000020d.asm
	$(CC) $(CFLAGS) -c  0000020d.asm -o 0000020d.obj
0000020d.o2: 0000020d.obj
	$(DMPSX) -o 0000020d.o2 0000020d.obj
0000020f.obj: 0000020f.asm
	$(CC) $(CFLAGS) -c  0000020f.asm -o 0000020f.obj
0000020f.o2: 0000020f.obj
	$(DMPSX) -o 0000020f.o2 0000020f.obj
00000211.obj: 00000211.asm
	$(CC) $(CFLAGS) -c  00000211.asm -o 00000211.obj
00000211.o2: 00000211.obj
	$(DMPSX) -o 00000211.o2 00000211.obj
00000213.obj: 00000213.asm
	$(CC) $(CFLAGS) -c  00000213.asm -o 00000213.obj
00000213.o2: 00000213.obj
	$(DMPSX) -o 00000213.o2 00000213.obj
00000215.obj: 00000215.asm
	$(CC) $(CFLAGS) -c  00000215.asm -o 00000215.obj
00000215.o2: 00000215.obj
	$(DMPSX) -o 00000215.o2 00000215.obj
00000217.obj: 00000217.asm
	$(CC) $(CFLAGS) -c  00000217.asm -o 00000217.obj
00000217.o2: 00000217.obj
	$(DMPSX) -o 00000217.o2 00000217.obj
00020008.obj: 00020008.asm
	$(CC) $(CFLAGS) -c  00020008.asm -o 00020008.obj
00020008.o2: 00020008.obj
	$(DMPSX) -o 00020008.o2 00020008.obj
00020009.obj: 00020009.asm
	$(CC) $(CFLAGS) -c  00020009.asm -o 00020009.obj
00020009.o2: 00020009.obj
	$(DMPSX) -o 00020009.o2 00020009.obj
0002000a.obj: 0002000a.asm
	$(CC) $(CFLAGS) -c  0002000a.asm -o 0002000a.obj
0002000a.o2: 0002000a.obj
	$(DMPSX) -o 0002000a.o2 0002000a.obj
0002000b.obj: 0002000b.asm
	$(CC) $(CFLAGS) -c  0002000b.asm -o 0002000b.obj
0002000b.o2: 0002000b.obj
	$(DMPSX) -o 0002000b.o2 0002000b.obj
0002000c.obj: 0002000c.asm
	$(CC) $(CFLAGS) -c  0002000c.asm -o 0002000c.obj
0002000c.o2: 0002000c.obj
	$(DMPSX) -o 0002000c.o2 0002000c.obj
0002000d.obj: 0002000d.asm
	$(CC) $(CFLAGS) -c  0002000d.asm -o 0002000d.obj
0002000d.o2: 0002000d.obj
	$(DMPSX) -o 0002000d.o2 0002000d.obj
0002000e.obj: 0002000e.asm
	$(CC) $(CFLAGS) -c  0002000e.asm -o 0002000e.obj
0002000e.o2: 0002000e.obj
	$(DMPSX) -o 0002000e.o2 0002000e.obj
0002000f.obj: 0002000f.asm
	$(CC) $(CFLAGS) -c  0002000f.asm -o 0002000f.obj
0002000f.o2: 0002000f.obj
	$(DMPSX) -o 0002000f.o2 0002000f.obj
00020010.obj: 00020010.asm
	$(CC) $(CFLAGS) -c  00020010.asm -o 00020010.obj
00020010.o2: 00020010.obj
	$(DMPSX) -o 00020010.o2 00020010.obj
00020011.obj: 00020011.asm
	$(CC) $(CFLAGS) -c  00020011.asm -o 00020011.obj
00020011.o2: 00020011.obj
	$(DMPSX) -o 00020011.o2 00020011.obj
00020012.obj: 00020012.asm
	$(CC) $(CFLAGS) -c  00020012.asm -o 00020012.obj
00020012.o2: 00020012.obj
	$(DMPSX) -o 00020012.o2 00020012.obj
00020013.obj: 00020013.asm
	$(CC) $(CFLAGS) -c  00020013.asm -o 00020013.obj
00020013.o2: 00020013.obj
	$(DMPSX) -o 00020013.o2 00020013.obj
00020014.obj: 00020014.asm
	$(CC) $(CFLAGS) -c  00020014.asm -o 00020014.obj
00020014.o2: 00020014.obj
	$(DMPSX) -o 00020014.o2 00020014.obj
00020015.obj: 00020015.asm
	$(CC) $(CFLAGS) -c  00020015.asm -o 00020015.obj
00020015.o2: 00020015.obj
	$(DMPSX) -o 00020015.o2 00020015.obj
00020016.obj: 00020016.asm
	$(CC) $(CFLAGS) -c  00020016.asm -o 00020016.obj
00020016.o2: 00020016.obj
	$(DMPSX) -o 00020016.o2 00020016.obj
00020017.obj: 00020017.asm
	$(CC) $(CFLAGS) -c  00020017.asm -o 00020017.obj
00020017.o2: 00020017.obj
	$(DMPSX) -o 00020017.o2 00020017.obj
00020209.obj: 00020209.asm
	$(CC) $(CFLAGS) -c  00020209.asm -o 00020209.obj
00020209.o2: 00020209.obj
	$(DMPSX) -o 00020209.o2 00020209.obj
0002020b.obj: 0002020b.asm
	$(CC) $(CFLAGS) -c  0002020b.asm -o 0002020b.obj
0002020b.o2: 0002020b.obj
	$(DMPSX) -o 0002020b.o2 0002020b.obj
0002020d.obj: 0002020d.asm
	$(CC) $(CFLAGS) -c  0002020d.asm -o 0002020d.obj
0002020d.o2: 0002020d.obj
	$(DMPSX) -o 0002020d.o2 0002020d.obj
0002020f.obj: 0002020f.asm
	$(CC) $(CFLAGS) -c  0002020f.asm -o 0002020f.obj
0002020f.o2: 0002020f.obj
	$(DMPSX) -o 0002020f.o2 0002020f.obj
00020211.obj: 00020211.asm
	$(CC) $(CFLAGS) -c  00020211.asm -o 00020211.obj
00020211.o2: 00020211.obj
	$(DMPSX) -o 00020211.o2 00020211.obj
00020213.obj: 00020213.asm
	$(CC) $(CFLAGS) -c  00020213.asm -o 00020213.obj
00020213.o2: 00020213.obj
	$(DMPSX) -o 00020213.o2 00020213.obj
00020215.obj: 00020215.asm
	$(CC) $(CFLAGS) -c  00020215.asm -o 00020215.obj
00020215.o2: 00020215.obj
	$(DMPSX) -o 00020215.o2 00020215.obj
00020217.obj: 00020217.asm
	$(CC) $(CFLAGS) -c  00020217.asm -o 00020217.obj
00020217.o2: 00020217.obj
	$(DMPSX) -o 00020217.o2 00020217.obj
00040048.obj: 00040048.asm
	$(CC) $(CFLAGS) -c  00040048.asm -o 00040048.obj
00040048.o2: 00040048.obj
	$(DMPSX) -o 00040048.o2 00040048.obj
00040049.obj: 00040049.asm
	$(CC) $(CFLAGS) -c  00040049.asm -o 00040049.obj
00040049.o2: 00040049.obj
	$(DMPSX) -o 00040049.o2 00040049.obj
0004004c.obj: 0004004c.asm
	$(CC) $(CFLAGS) -c  0004004c.asm -o 0004004c.obj
0004004c.o2: 0004004c.obj
	$(DMPSX) -o 0004004c.o2 0004004c.obj
0004004d.obj: 0004004d.asm
	$(CC) $(CFLAGS) -c  0004004d.asm -o 0004004d.obj
0004004d.o2: 0004004d.obj
	$(DMPSX) -o 0004004d.o2 0004004d.obj
00040050.obj: 00040050.asm
	$(CC) $(CFLAGS) -c  00040050.asm -o 00040050.obj
00040050.o2: 00040050.obj
	$(DMPSX) -o 00040050.o2 00040050.obj
00040051.obj: 00040051.asm
	$(CC) $(CFLAGS) -c  00040051.asm -o 00040051.obj
00040051.o2: 00040051.obj
	$(DMPSX) -o 00040051.o2 00040051.obj
00040054.obj: 00040054.asm
	$(CC) $(CFLAGS) -c  00040054.asm -o 00040054.obj
00040054.o2: 00040054.obj
	$(DMPSX) -o 00040054.o2 00040054.obj
00040055.obj: 00040055.asm
	$(CC) $(CFLAGS) -c  00040055.asm -o 00040055.obj
00040055.o2: 00040055.obj
	$(DMPSX) -o 00040055.o2 00040055.obj
00040058.obj: 00040058.asm
	$(CC) $(CFLAGS) -c  00040058.asm -o 00040058.obj
00040058.o2: 00040058.obj
	$(DMPSX) -o 00040058.o2 00040058.obj
00040059.obj: 00040059.asm
	$(CC) $(CFLAGS) -c  00040059.asm -o 00040059.obj
00040059.o2: 00040059.obj
	$(DMPSX) -o 00040059.o2 00040059.obj
0004005c.obj: 0004005c.asm
	$(CC) $(CFLAGS) -c  0004005c.asm -o 0004005c.obj
0004005c.o2: 0004005c.obj
	$(DMPSX) -o 0004005c.o2 0004005c.obj
0004005d.obj: 0004005d.asm
	$(CC) $(CFLAGS) -c  0004005d.asm -o 0004005d.obj
0004005d.o2: 0004005d.obj
	$(DMPSX) -o 0004005d.o2 0004005d.obj
00040148.obj: 00040148.asm
	$(CC) $(CFLAGS) -c  00040148.asm -o 00040148.obj
00040148.o2: 00040148.obj
	$(DMPSX) -o 00040148.o2 00040148.obj
00040149.obj: 00040149.asm
	$(CC) $(CFLAGS) -c  00040149.asm -o 00040149.obj
00040149.o2: 00040149.obj
	$(DMPSX) -o 00040149.o2 00040149.obj
0004014c.obj: 0004014c.asm
	$(CC) $(CFLAGS) -c  0004014c.asm -o 0004014c.obj
0004014c.o2: 0004014c.obj
	$(DMPSX) -o 0004014c.o2 0004014c.obj
0004014d.obj: 0004014d.asm
	$(CC) $(CFLAGS) -c  0004014d.asm -o 0004014d.obj
0004014d.o2: 0004014d.obj
	$(DMPSX) -o 0004014d.o2 0004014d.obj
00040150.obj: 00040150.asm
	$(CC) $(CFLAGS) -c  00040150.asm -o 00040150.obj
00040150.o2: 00040150.obj
	$(DMPSX) -o 00040150.o2 00040150.obj
00040151.obj: 00040151.asm
	$(CC) $(CFLAGS) -c  00040151.asm -o 00040151.obj
00040151.o2: 00040151.obj
	$(DMPSX) -o 00040151.o2 00040151.obj
00040154.obj: 00040154.asm
	$(CC) $(CFLAGS) -c  00040154.asm -o 00040154.obj
00040154.o2: 00040154.obj
	$(DMPSX) -o 00040154.o2 00040154.obj
00040155.obj: 00040155.asm
	$(CC) $(CFLAGS) -c  00040155.asm -o 00040155.obj
00040155.o2: 00040155.obj
	$(DMPSX) -o 00040155.o2 00040155.obj
00040249.obj: 00040249.asm
	$(CC) $(CFLAGS) -c  00040249.asm -o 00040249.obj
00040249.o2: 00040249.obj
	$(DMPSX) -o 00040249.o2 00040249.obj
0004024d.obj: 0004024d.asm
	$(CC) $(CFLAGS) -c  0004024d.asm -o 0004024d.obj
0004024d.o2: 0004024d.obj
	$(DMPSX) -o 0004024d.o2 0004024d.obj
00040251.obj: 00040251.asm
	$(CC) $(CFLAGS) -c  00040251.asm -o 00040251.obj
00040251.o2: 00040251.obj
	$(DMPSX) -o 00040251.o2 00040251.obj
00040255.obj: 00040255.asm
	$(CC) $(CFLAGS) -c  00040255.asm -o 00040255.obj
00040255.o2: 00040255.obj
	$(DMPSX) -o 00040255.o2 00040255.obj
00100008.obj: 00100008.asm
	$(CC) $(CFLAGS) -c  00100008.asm -o 00100008.obj
00100008.o2: 00100008.obj
	$(DMPSX) -o 00100008.o2 00100008.obj
00100009.obj: 00100009.asm
	$(CC) $(CFLAGS) -c  00100009.asm -o 00100009.obj
00100009.o2: 00100009.obj
	$(DMPSX) -o 00100009.o2 00100009.obj
0010000a.obj: 0010000a.asm
	$(CC) $(CFLAGS) -c  0010000a.asm -o 0010000a.obj
0010000a.o2: 0010000a.obj
	$(DMPSX) -o 0010000a.o2 0010000a.obj
0010000b.obj: 0010000b.asm
	$(CC) $(CFLAGS) -c  0010000b.asm -o 0010000b.obj
0010000b.o2: 0010000b.obj
	$(DMPSX) -o 0010000b.o2 0010000b.obj
0010000c.obj: 0010000c.asm
	$(CC) $(CFLAGS) -c  0010000c.asm -o 0010000c.obj
0010000c.o2: 0010000c.obj
	$(DMPSX) -o 0010000c.o2 0010000c.obj
0010000d.obj: 0010000d.asm
	$(CC) $(CFLAGS) -c  0010000d.asm -o 0010000d.obj
0010000d.o2: 0010000d.obj
	$(DMPSX) -o 0010000d.o2 0010000d.obj
0010000e.obj: 0010000e.asm
	$(CC) $(CFLAGS) -c  0010000e.asm -o 0010000e.obj
0010000e.o2: 0010000e.obj
	$(DMPSX) -o 0010000e.o2 0010000e.obj
0010000f.obj: 0010000f.asm
	$(CC) $(CFLAGS) -c  0010000f.asm -o 0010000f.obj
0010000f.o2: 0010000f.obj
	$(DMPSX) -o 0010000f.o2 0010000f.obj
00100010.obj: 00100010.asm
	$(CC) $(CFLAGS) -c  00100010.asm -o 00100010.obj
00100010.o2: 00100010.obj
	$(DMPSX) -o 00100010.o2 00100010.obj
00100011.obj: 00100011.asm
	$(CC) $(CFLAGS) -c  00100011.asm -o 00100011.obj
00100011.o2: 00100011.obj
	$(DMPSX) -o 00100011.o2 00100011.obj
00100012.obj: 00100012.asm
	$(CC) $(CFLAGS) -c  00100012.asm -o 00100012.obj
00100012.o2: 00100012.obj
	$(DMPSX) -o 00100012.o2 00100012.obj
00100013.obj: 00100013.asm
	$(CC) $(CFLAGS) -c  00100013.asm -o 00100013.obj
00100013.o2: 00100013.obj
	$(DMPSX) -o 00100013.o2 00100013.obj
00100014.obj: 00100014.asm
	$(CC) $(CFLAGS) -c  00100014.asm -o 00100014.obj
00100014.o2: 00100014.obj
	$(DMPSX) -o 00100014.o2 00100014.obj
00100015.obj: 00100015.asm
	$(CC) $(CFLAGS) -c  00100015.asm -o 00100015.obj
00100015.o2: 00100015.obj
	$(DMPSX) -o 00100015.o2 00100015.obj
00100016.obj: 00100016.asm
	$(CC) $(CFLAGS) -c  00100016.asm -o 00100016.obj
00100016.o2: 00100016.obj
	$(DMPSX) -o 00100016.o2 00100016.obj
00100017.obj: 00100017.asm
	$(CC) $(CFLAGS) -c  00100017.asm -o 00100017.obj
00100017.o2: 00100017.obj
	$(DMPSX) -o 00100017.o2 00100017.obj
00100209.obj: 00100209.asm
	$(CC) $(CFLAGS) -c  00100209.asm -o 00100209.obj
00100209.o2: 00100209.obj
	$(DMPSX) -o 00100209.o2 00100209.obj
0010020b.obj: 0010020b.asm
	$(CC) $(CFLAGS) -c  0010020b.asm -o 0010020b.obj
0010020b.o2: 0010020b.obj
	$(DMPSX) -o 0010020b.o2 0010020b.obj
0010020d.obj: 0010020d.asm
	$(CC) $(CFLAGS) -c  0010020d.asm -o 0010020d.obj
0010020d.o2: 0010020d.obj
	$(DMPSX) -o 0010020d.o2 0010020d.obj
0010020f.obj: 0010020f.asm
	$(CC) $(CFLAGS) -c  0010020f.asm -o 0010020f.obj
0010020f.o2: 0010020f.obj
	$(DMPSX) -o 0010020f.o2 0010020f.obj
00100211.obj: 00100211.asm
	$(CC) $(CFLAGS) -c  00100211.asm -o 00100211.obj
00100211.o2: 00100211.obj
	$(DMPSX) -o 00100211.o2 00100211.obj
00100213.obj: 00100213.asm
	$(CC) $(CFLAGS) -c  00100213.asm -o 00100213.obj
00100213.o2: 00100213.obj
	$(DMPSX) -o 00100213.o2 00100213.obj
00100215.obj: 00100215.asm
	$(CC) $(CFLAGS) -c  00100215.asm -o 00100215.obj
00100215.o2: 00100215.obj
	$(DMPSX) -o 00100215.o2 00100215.obj
00100217.obj: 00100217.asm
	$(CC) $(CFLAGS) -c  00100217.asm -o 00100217.obj
00100217.o2: 00100217.obj
	$(DMPSX) -o 00100217.o2 00100217.obj
00200008.obj: 00200008.asm
	$(CC) $(CFLAGS) -c  00200008.asm -o 00200008.obj
00200008.o2: 00200008.obj
	$(DMPSX) -o 00200008.o2 00200008.obj
00200009.obj: 00200009.asm
	$(CC) $(CFLAGS) -c  00200009.asm -o 00200009.obj
00200009.o2: 00200009.obj
	$(DMPSX) -o 00200009.o2 00200009.obj
0020000c.obj: 0020000c.asm
	$(CC) $(CFLAGS) -c  0020000c.asm -o 0020000c.obj
0020000c.o2: 0020000c.obj
	$(DMPSX) -o 0020000c.o2 0020000c.obj
0020000d.obj: 0020000d.asm
	$(CC) $(CFLAGS) -c  0020000d.asm -o 0020000d.obj
0020000d.o2: 0020000d.obj
	$(DMPSX) -o 0020000d.o2 0020000d.obj
00200010.obj: 00200010.asm
	$(CC) $(CFLAGS) -c  00200010.asm -o 00200010.obj
00200010.o2: 00200010.obj
	$(DMPSX) -o 00200010.o2 00200010.obj
00200011.obj: 00200011.asm
	$(CC) $(CFLAGS) -c  00200011.asm -o 00200011.obj
00200011.o2: 00200011.obj
	$(DMPSX) -o 00200011.o2 00200011.obj
00200014.obj: 00200014.asm
	$(CC) $(CFLAGS) -c  00200014.asm -o 00200014.obj
00200014.o2: 00200014.obj
	$(DMPSX) -o 00200014.o2 00200014.obj
00200015.obj: 00200015.asm
	$(CC) $(CFLAGS) -c  00200015.asm -o 00200015.obj
00200015.o2: 00200015.obj
	$(DMPSX) -o 00200015.o2 00200015.obj
00200209.obj: 00200209.asm
	$(CC) $(CFLAGS) -c  00200209.asm -o 00200209.obj
00200209.o2: 00200209.obj
	$(DMPSX) -o 00200209.o2 00200209.obj
0020020d.obj: 0020020d.asm
	$(CC) $(CFLAGS) -c  0020020d.asm -o 0020020d.obj
0020020d.o2: 0020020d.obj
	$(DMPSX) -o 0020020d.o2 0020020d.obj
00200211.obj: 00200211.asm
	$(CC) $(CFLAGS) -c  00200211.asm -o 00200211.obj
00200211.o2: 00200211.obj
	$(DMPSX) -o 00200211.o2 00200211.obj
00200215.obj: 00200215.asm
	$(CC) $(CFLAGS) -c  00200215.asm -o 00200215.obj
00200215.o2: 00200215.obj
	$(DMPSX) -o 00200215.o2 00200215.obj
00240048.obj: 00240048.asm
	$(CC) $(CFLAGS) -c  00240048.asm -o 00240048.obj
00240048.o2: 00240048.obj
	$(DMPSX) -o 00240048.o2 00240048.obj
00240049.obj: 00240049.asm
	$(CC) $(CFLAGS) -c  00240049.asm -o 00240049.obj
00240049.o2: 00240049.obj
	$(DMPSX) -o 00240049.o2 00240049.obj
0024004c.obj: 0024004c.asm
	$(CC) $(CFLAGS) -c  0024004c.asm -o 0024004c.obj
0024004c.o2: 0024004c.obj
	$(DMPSX) -o 0024004c.o2 0024004c.obj
0024004d.obj: 0024004d.asm
	$(CC) $(CFLAGS) -c  0024004d.asm -o 0024004d.obj
0024004d.o2: 0024004d.obj
	$(DMPSX) -o 0024004d.o2 0024004d.obj
00240050.obj: 00240050.asm
	$(CC) $(CFLAGS) -c  00240050.asm -o 00240050.obj
00240050.o2: 00240050.obj
	$(DMPSX) -o 00240050.o2 00240050.obj
00240051.obj: 00240051.asm
	$(CC) $(CFLAGS) -c  00240051.asm -o 00240051.obj
00240051.o2: 00240051.obj
	$(DMPSX) -o 00240051.o2 00240051.obj
00240054.obj: 00240054.asm
	$(CC) $(CFLAGS) -c  00240054.asm -o 00240054.obj
00240054.o2: 00240054.obj
	$(DMPSX) -o 00240054.o2 00240054.obj
00240055.obj: 00240055.asm
	$(CC) $(CFLAGS) -c  00240055.asm -o 00240055.obj
00240055.o2: 00240055.obj
	$(DMPSX) -o 00240055.o2 00240055.obj
00240249.obj: 00240249.asm
	$(CC) $(CFLAGS) -c  00240249.asm -o 00240249.obj
00240249.o2: 00240249.obj
	$(DMPSX) -o 00240249.o2 00240249.obj
0024024d.obj: 0024024d.asm
	$(CC) $(CFLAGS) -c  0024024d.asm -o 0024024d.obj
0024024d.o2: 0024024d.obj
	$(DMPSX) -o 0024024d.o2 0024024d.obj
00240251.obj: 00240251.asm
	$(CC) $(CFLAGS) -c  00240251.asm -o 00240251.obj
00240251.o2: 00240251.obj
	$(DMPSX) -o 00240251.o2 00240251.obj
00240255.obj: 00240255.asm
	$(CC) $(CFLAGS) -c  00240255.asm -o 00240255.obj
00240255.o2: 00240255.obj
	$(DMPSX) -o 00240255.o2 00240255.obj
00300008.obj: 00300008.asm
	$(CC) $(CFLAGS) -c  00300008.asm -o 00300008.obj
00300008.o2: 00300008.obj
	$(DMPSX) -o 00300008.o2 00300008.obj
00300009.obj: 00300009.asm
	$(CC) $(CFLAGS) -c  00300009.asm -o 00300009.obj
00300009.o2: 00300009.obj
	$(DMPSX) -o 00300009.o2 00300009.obj
0030000c.obj: 0030000c.asm
	$(CC) $(CFLAGS) -c  0030000c.asm -o 0030000c.obj
0030000c.o2: 0030000c.obj
	$(DMPSX) -o 0030000c.o2 0030000c.obj
0030000d.obj: 0030000d.asm
	$(CC) $(CFLAGS) -c  0030000d.asm -o 0030000d.obj
0030000d.o2: 0030000d.obj
	$(DMPSX) -o 0030000d.o2 0030000d.obj
00300010.obj: 00300010.asm
	$(CC) $(CFLAGS) -c  00300010.asm -o 00300010.obj
00300010.o2: 00300010.obj
	$(DMPSX) -o 00300010.o2 00300010.obj
00300011.obj: 00300011.asm
	$(CC) $(CFLAGS) -c  00300011.asm -o 00300011.obj
00300011.o2: 00300011.obj
	$(DMPSX) -o 00300011.o2 00300011.obj
00300014.obj: 00300014.asm
	$(CC) $(CFLAGS) -c  00300014.asm -o 00300014.obj
00300014.o2: 00300014.obj
	$(DMPSX) -o 00300014.o2 00300014.obj
00300015.obj: 00300015.asm
	$(CC) $(CFLAGS) -c  00300015.asm -o 00300015.obj
00300015.o2: 00300015.obj
	$(DMPSX) -o 00300015.o2 00300015.obj
00300209.obj: 00300209.asm
	$(CC) $(CFLAGS) -c  00300209.asm -o 00300209.obj
00300209.o2: 00300209.obj
	$(DMPSX) -o 00300209.o2 00300209.obj
0030020d.obj: 0030020d.asm
	$(CC) $(CFLAGS) -c  0030020d.asm -o 0030020d.obj
0030020d.o2: 0030020d.obj
	$(DMPSX) -o 0030020d.o2 0030020d.obj
00300211.obj: 00300211.asm
	$(CC) $(CFLAGS) -c  00300211.asm -o 00300211.obj
00300211.o2: 00300211.obj
	$(DMPSX) -o 00300211.o2 00300211.obj
00300215.obj: 00300215.asm
	$(CC) $(CFLAGS) -c  00300215.asm -o 00300215.obj
00300215.o2: 00300215.obj
	$(DMPSX) -o 00300215.o2 00300215.obj
01000000.obj: 01000000.asm
	$(CC) $(CFLAGS) -c  01000000.asm -o 01000000.obj
01000000.o2: 01000000.obj
	$(DMPSX) -o 01000000.o2 01000000.obj
0100000c.obj: 0100000c.asm
	$(CC) $(CFLAGS) -c  0100000c.asm -o 0100000c.obj
0100000c.o2: 0100000c.obj
	$(DMPSX) -o 0100000c.o2 0100000c.obj
0100000d.obj: 0100000d.asm
	$(CC) $(CFLAGS) -c  0100000d.asm -o 0100000d.obj
0100000d.o2: 0100000d.obj
	$(DMPSX) -o 0100000d.o2 0100000d.obj
0100000e.obj: 0100000e.asm
	$(CC) $(CFLAGS) -c  0100000e.asm -o 0100000e.obj
0100000e.o2: 0100000e.obj
	$(DMPSX) -o 0100000e.o2 0100000e.obj
0100000f.obj: 0100000f.asm
	$(CC) $(CFLAGS) -c  0100000f.asm -o 0100000f.obj
0100000f.o2: 0100000f.obj
	$(DMPSX) -o 0100000f.o2 0100000f.obj
01000014.obj: 01000014.asm
	$(CC) $(CFLAGS) -c  01000014.asm -o 01000014.obj
01000014.o2: 01000014.obj
	$(DMPSX) -o 01000014.o2 01000014.obj
01000015.obj: 01000015.asm
	$(CC) $(CFLAGS) -c  01000015.asm -o 01000015.obj
01000015.o2: 01000015.obj
	$(DMPSX) -o 01000015.o2 01000015.obj
01000016.obj: 01000016.asm
	$(CC) $(CFLAGS) -c  01000016.asm -o 01000016.obj
01000016.o2: 01000016.obj
	$(DMPSX) -o 01000016.o2 01000016.obj
01000017.obj: 01000017.asm
	$(CC) $(CFLAGS) -c  01000017.asm -o 01000017.obj
01000017.o2: 01000017.obj
	$(DMPSX) -o 01000017.o2 01000017.obj
0100020d.obj: 0100020d.asm
	$(CC) $(CFLAGS) -c  0100020d.asm -o 0100020d.obj
0100020d.o2: 0100020d.obj
	$(DMPSX) -o 0100020d.o2 0100020d.obj
0100020f.obj: 0100020f.asm
	$(CC) $(CFLAGS) -c  0100020f.asm -o 0100020f.obj
0100020f.o2: 0100020f.obj
	$(DMPSX) -o 0100020f.o2 0100020f.obj
01000215.obj: 01000215.asm
	$(CC) $(CFLAGS) -c  01000215.asm -o 01000215.obj
01000215.o2: 01000215.obj
	$(DMPSX) -o 01000215.o2 01000215.obj
01000217.obj: 01000217.asm
	$(CC) $(CFLAGS) -c  01000217.asm -o 01000217.obj
01000217.o2: 01000217.obj
	$(DMPSX) -o 01000217.o2 01000217.obj
0102000c.obj: 0102000c.asm
	$(CC) $(CFLAGS) -c  0102000c.asm -o 0102000c.obj
0102000c.o2: 0102000c.obj
	$(DMPSX) -o 0102000c.o2 0102000c.obj
0102000d.obj: 0102000d.asm
	$(CC) $(CFLAGS) -c  0102000d.asm -o 0102000d.obj
0102000d.o2: 0102000d.obj
	$(DMPSX) -o 0102000d.o2 0102000d.obj
0102000e.obj: 0102000e.asm
	$(CC) $(CFLAGS) -c  0102000e.asm -o 0102000e.obj
0102000e.o2: 0102000e.obj
	$(DMPSX) -o 0102000e.o2 0102000e.obj
0102000f.obj: 0102000f.asm
	$(CC) $(CFLAGS) -c  0102000f.asm -o 0102000f.obj
0102000f.o2: 0102000f.obj
	$(DMPSX) -o 0102000f.o2 0102000f.obj
01020014.obj: 01020014.asm
	$(CC) $(CFLAGS) -c  01020014.asm -o 01020014.obj
01020014.o2: 01020014.obj
	$(DMPSX) -o 01020014.o2 01020014.obj
01020015.obj: 01020015.asm
	$(CC) $(CFLAGS) -c  01020015.asm -o 01020015.obj
01020015.o2: 01020015.obj
	$(DMPSX) -o 01020015.o2 01020015.obj
01020016.obj: 01020016.asm
	$(CC) $(CFLAGS) -c  01020016.asm -o 01020016.obj
01020016.o2: 01020016.obj
	$(DMPSX) -o 01020016.o2 01020016.obj
01020017.obj: 01020017.asm
	$(CC) $(CFLAGS) -c  01020017.asm -o 01020017.obj
01020017.o2: 01020017.obj
	$(DMPSX) -o 01020017.o2 01020017.obj
0102020d.obj: 0102020d.asm
	$(CC) $(CFLAGS) -c  0102020d.asm -o 0102020d.obj
0102020d.o2: 0102020d.obj
	$(DMPSX) -o 0102020d.o2 0102020d.obj
0102020f.obj: 0102020f.asm
	$(CC) $(CFLAGS) -c  0102020f.asm -o 0102020f.obj
0102020f.o2: 0102020f.obj
	$(DMPSX) -o 0102020f.o2 0102020f.obj
01020215.obj: 01020215.asm
	$(CC) $(CFLAGS) -c  01020215.asm -o 01020215.obj
01020215.o2: 01020215.obj
	$(DMPSX) -o 01020215.o2 01020215.obj
01020217.obj: 01020217.asm
	$(CC) $(CFLAGS) -c  01020217.asm -o 01020217.obj
01020217.o2: 01020217.obj
	$(DMPSX) -o 01020217.o2 01020217.obj
01040048.obj: 01040048.asm
	$(CC) $(CFLAGS) -c  01040048.asm -o 01040048.obj
01040048.o2: 01040048.obj
	$(DMPSX) -o 01040048.o2 01040048.obj
01040049.obj: 01040049.asm
	$(CC) $(CFLAGS) -c  01040049.asm -o 01040049.obj
01040049.o2: 01040049.obj
	$(DMPSX) -o 01040049.o2 01040049.obj
0104004c.obj: 0104004c.asm
	$(CC) $(CFLAGS) -c  0104004c.asm -o 0104004c.obj
0104004c.o2: 0104004c.obj
	$(DMPSX) -o 0104004c.o2 0104004c.obj
0104004d.obj: 0104004d.asm
	$(CC) $(CFLAGS) -c  0104004d.asm -o 0104004d.obj
0104004d.o2: 0104004d.obj
	$(DMPSX) -o 0104004d.o2 0104004d.obj
01040050.obj: 01040050.asm
	$(CC) $(CFLAGS) -c  01040050.asm -o 01040050.obj
01040050.o2: 01040050.obj
	$(DMPSX) -o 01040050.o2 01040050.obj
01040051.obj: 01040051.asm
	$(CC) $(CFLAGS) -c  01040051.asm -o 01040051.obj
01040051.o2: 01040051.obj
	$(DMPSX) -o 01040051.o2 01040051.obj
01040054.obj: 01040054.asm
	$(CC) $(CFLAGS) -c  01040054.asm -o 01040054.obj
01040054.o2: 01040054.obj
	$(DMPSX) -o 01040054.o2 01040054.obj
01040055.obj: 01040055.asm
	$(CC) $(CFLAGS) -c  01040055.asm -o 01040055.obj
01040055.o2: 01040055.obj
	$(DMPSX) -o 01040055.o2 01040055.obj
01040249.obj: 01040249.asm
	$(CC) $(CFLAGS) -c  01040249.asm -o 01040249.obj
01040249.o2: 01040249.obj
	$(DMPSX) -o 01040249.o2 01040249.obj
0104024d.obj: 0104024d.asm
	$(CC) $(CFLAGS) -c  0104024d.asm -o 0104024d.obj
0104024d.o2: 0104024d.obj
	$(DMPSX) -o 0104024d.o2 0104024d.obj
01040251.obj: 01040251.asm
	$(CC) $(CFLAGS) -c  01040251.asm -o 01040251.obj
01040251.o2: 01040251.obj
	$(DMPSX) -o 01040251.o2 01040251.obj
01040255.obj: 01040255.asm
	$(CC) $(CFLAGS) -c  01040255.asm -o 01040255.obj
01040255.o2: 01040255.obj
	$(DMPSX) -o 01040255.o2 01040255.obj
0120000c.obj: 0120000c.asm
	$(CC) $(CFLAGS) -c  0120000c.asm -o 0120000c.obj
0120000c.o2: 0120000c.obj
	$(DMPSX) -o 0120000c.o2 0120000c.obj
0120000d.obj: 0120000d.asm
	$(CC) $(CFLAGS) -c  0120000d.asm -o 0120000d.obj
0120000d.o2: 0120000d.obj
	$(DMPSX) -o 0120000d.o2 0120000d.obj
01200014.obj: 01200014.asm
	$(CC) $(CFLAGS) -c  01200014.asm -o 01200014.obj
01200014.o2: 01200014.obj
	$(DMPSX) -o 01200014.o2 01200014.obj
01200015.obj: 01200015.asm
	$(CC) $(CFLAGS) -c  01200015.asm -o 01200015.obj
01200015.o2: 01200015.obj
	$(DMPSX) -o 01200015.o2 01200015.obj
0120020d.obj: 0120020d.asm
	$(CC) $(CFLAGS) -c  0120020d.asm -o 0120020d.obj
0120020d.o2: 0120020d.obj
	$(DMPSX) -o 0120020d.o2 0120020d.obj
01200215.obj: 01200215.asm
	$(CC) $(CFLAGS) -c  01200215.asm -o 01200215.obj
01200215.o2: 01200215.obj
	$(DMPSX) -o 01200215.o2 01200215.obj
01240048.obj: 01240048.asm
	$(CC) $(CFLAGS) -c  01240048.asm -o 01240048.obj
01240048.o2: 01240048.obj
	$(DMPSX) -o 01240048.o2 01240048.obj
01240049.obj: 01240049.asm
	$(CC) $(CFLAGS) -c  01240049.asm -o 01240049.obj
01240049.o2: 01240049.obj
	$(DMPSX) -o 01240049.o2 01240049.obj
0124004c.obj: 0124004c.asm
	$(CC) $(CFLAGS) -c  0124004c.asm -o 0124004c.obj
0124004c.o2: 0124004c.obj
	$(DMPSX) -o 0124004c.o2 0124004c.obj
0124004d.obj: 0124004d.asm
	$(CC) $(CFLAGS) -c  0124004d.asm -o 0124004d.obj
0124004d.o2: 0124004d.obj
	$(DMPSX) -o 0124004d.o2 0124004d.obj
01240050.obj: 01240050.asm
	$(CC) $(CFLAGS) -c  01240050.asm -o 01240050.obj
01240050.o2: 01240050.obj
	$(DMPSX) -o 01240050.o2 01240050.obj
01240051.obj: 01240051.asm
	$(CC) $(CFLAGS) -c  01240051.asm -o 01240051.obj
01240051.o2: 01240051.obj
	$(DMPSX) -o 01240051.o2 01240051.obj
01240054.obj: 01240054.asm
	$(CC) $(CFLAGS) -c  01240054.asm -o 01240054.obj
01240054.o2: 01240054.obj
	$(DMPSX) -o 01240054.o2 01240054.obj
01240055.obj: 01240055.asm
	$(CC) $(CFLAGS) -c  01240055.asm -o 01240055.obj
01240055.o2: 01240055.obj
	$(DMPSX) -o 01240055.o2 01240055.obj
01240249.obj: 01240249.asm
	$(CC) $(CFLAGS) -c  01240249.asm -o 01240249.obj
01240249.o2: 01240249.obj
	$(DMPSX) -o 01240249.o2 01240249.obj
0124024d.obj: 0124024d.asm
	$(CC) $(CFLAGS) -c  0124024d.asm -o 0124024d.obj
0124024d.o2: 0124024d.obj
	$(DMPSX) -o 0124024d.o2 0124024d.obj
01240251.obj: 01240251.asm
	$(CC) $(CFLAGS) -c  01240251.asm -o 01240251.obj
01240251.o2: 01240251.obj
	$(DMPSX) -o 01240251.o2 01240251.obj
01240255.obj: 01240255.asm
	$(CC) $(CFLAGS) -c  01240255.asm -o 01240255.obj
01240255.o2: 01240255.obj
	$(DMPSX) -o 01240255.o2 01240255.obj
anim.obj: anim.c
	$(CC) $(CFLAGS) -c  anim.c -o anim.obj
anim2.obj: anim2.c
	$(CC) $(CFLAGS) -c  anim2.c -o anim2.obj
bezier.obj: bezier.c
	$(CC) $(CFLAGS) -c  bezier.c -o bezier.obj
beta.obj: beta.c
	$(CC) $(CFLAGS) -c  beta.c -o beta.obj
mime.obj: mime.c
	$(CC) $(CFLAGS) -c  mime.c -o mime.obj
00000000.obj: 00000000.c
	$(CC) $(CFLAGS) -c  00000000.c -o 00000000.obj
00010008.obj: 00010008.c
	$(CC) $(CFLAGS) -c  00010008.c -o 00010008.obj
00010009.obj: 00010009.c
	$(CC) $(CFLAGS) -c  00010009.c -o 00010009.obj
0001000c.obj: 0001000c.c
	$(CC) $(CFLAGS) -c  0001000c.c -o 0001000c.obj
0001000d.obj: 0001000d.c
	$(CC) $(CFLAGS) -c  0001000d.c -o 0001000d.obj
00010010.obj: 00010010.c
	$(CC) $(CFLAGS) -c  00010010.c -o 00010010.obj
00010011.obj: 00010011.c
	$(CC) $(CFLAGS) -c  00010011.c -o 00010011.obj
00010014.obj: 00010014.c
	$(CC) $(CFLAGS) -c  00010014.c -o 00010014.obj
00010015.obj: 00010015.c
	$(CC) $(CFLAGS) -c  00010015.c -o 00010015.obj
00030008.obj: 00030008.c
	$(CC) $(CFLAGS) -c  00030008.c -o 00030008.obj
00030009.obj: 00030009.c
	$(CC) $(CFLAGS) -c  00030009.c -o 00030009.obj
0003000c.obj: 0003000c.c
	$(CC) $(CFLAGS) -c  0003000c.c -o 0003000c.obj
0003000d.obj: 0003000d.c
	$(CC) $(CFLAGS) -c  0003000d.c -o 0003000d.obj
00030010.obj: 00030010.c
	$(CC) $(CFLAGS) -c  00030010.c -o 00030010.obj
00030011.obj: 00030011.c
	$(CC) $(CFLAGS) -c  00030011.c -o 00030011.obj
00030014.obj: 00030014.c
	$(CC) $(CFLAGS) -c  00030014.c -o 00030014.obj
00030015.obj: 00030015.c
	$(CC) $(CFLAGS) -c  00030015.c -o 00030015.obj
00050048.obj: 00050048.c
	$(CC) $(CFLAGS) -c  00050048.c -o 00050048.obj
00050049.obj: 00050049.c
	$(CC) $(CFLAGS) -c  00050049.c -o 00050049.obj
0005004c.obj: 0005004c.c
	$(CC) $(CFLAGS) -c  0005004c.c -o 0005004c.obj
0005004d.obj: 0005004d.c
	$(CC) $(CFLAGS) -c  0005004d.c -o 0005004d.obj
00050050.obj: 00050050.c
	$(CC) $(CFLAGS) -c  00050050.c -o 00050050.obj
00050051.obj: 00050051.c
	$(CC) $(CFLAGS) -c  00050051.c -o 00050051.obj
00050054.obj: 00050054.c
	$(CC) $(CFLAGS) -c  00050054.c -o 00050054.obj
00050055.obj: 00050055.c
	$(CC) $(CFLAGS) -c  00050055.c -o 00050055.obj
02000000.obj: 02000000.c
	$(CC) $(CFLAGS) -c  02000000.c -o 02000000.obj
02000001.obj: 02000001.c
	$(CC) $(CFLAGS) -c  02000001.c -o 02000001.obj
03000000.obj: 03000000.c
	$(CC) $(CFLAGS) -c  03000000.c -o 03000000.obj
03000001.obj: 03000001.c
	$(CC) $(CFLAGS) -c  03000001.c -o 03000001.obj
03000002.obj: 03000002.c
	$(CC) $(CFLAGS) -c  03000002.c -o 03000002.obj
03000003.obj: 03000003.c
	$(CC) $(CFLAGS) -c  03000003.c -o 03000003.obj
03000009.obj: 03000009.c
	$(CC) $(CFLAGS) -c  03000009.c -o 03000009.obj
0300000a.obj: 0300000a.c
	$(CC) $(CFLAGS) -c  0300000a.c -o 0300000a.obj
0300000b.obj: 0300000b.c
	$(CC) $(CFLAGS) -c  0300000b.c -o 0300000b.obj
03000100.obj: 03000100.c
	$(CC) $(CFLAGS) -c  03000100.c -o 03000100.obj
03000901.obj: 03000901.c
	$(CC) $(CFLAGS) -c  03000901.c -o 03000901.obj
03000902.obj: 03000902.c
	$(CC) $(CFLAGS) -c  03000902.c -o 03000902.obj
03000909.obj: 03000909.c
	$(CC) $(CFLAGS) -c  03000909.c -o 03000909.obj
0300090a.obj: 0300090a.c
	$(CC) $(CFLAGS) -c  0300090a.c -o 0300090a.obj
03010110.obj: 03010110.c
	$(CC) $(CFLAGS) -c  03010110.c -o 03010110.obj
03010111.obj: 03010111.c
	$(CC) $(CFLAGS) -c  03010111.c -o 03010111.obj
03010112.obj: 03010112.c
	$(CC) $(CFLAGS) -c  03010112.c -o 03010112.obj
03010121.obj: 03010121.c
	$(CC) $(CFLAGS) -c  03010121.c -o 03010121.obj
03010122.obj: 03010122.c
	$(CC) $(CFLAGS) -c  03010122.c -o 03010122.obj
03010141.obj: 03010141.c
	$(CC) $(CFLAGS) -c  03010141.c -o 03010141.obj
03010142.obj: 03010142.c
	$(CC) $(CFLAGS) -c  03010142.c -o 03010142.obj
03010171.obj: 03010171.c
	$(CC) $(CFLAGS) -c  03010171.c -o 03010171.obj
03010172.obj: 03010172.c
	$(CC) $(CFLAGS) -c  03010172.c -o 03010172.obj
03010182.obj: 03010182.c
	$(CC) $(CFLAGS) -c  03010182.c -o 03010182.obj
03010210.obj: 03010210.c
	$(CC) $(CFLAGS) -c  03010210.c -o 03010210.obj
03010211.obj: 03010211.c
	$(CC) $(CFLAGS) -c  03010211.c -o 03010211.obj
03010212.obj: 03010212.c
	$(CC) $(CFLAGS) -c  03010212.c -o 03010212.obj
03010221.obj: 03010221.c
	$(CC) $(CFLAGS) -c  03010221.c -o 03010221.obj
03010222.obj: 03010222.c
	$(CC) $(CFLAGS) -c  03010222.c -o 03010222.obj
03010241.obj: 03010241.c
	$(CC) $(CFLAGS) -c  03010241.c -o 03010241.obj
03010242.obj: 03010242.c
	$(CC) $(CFLAGS) -c  03010242.c -o 03010242.obj
03010271.obj: 03010271.c
	$(CC) $(CFLAGS) -c  03010271.c -o 03010271.obj
03010272.obj: 03010272.c
	$(CC) $(CFLAGS) -c  03010272.c -o 03010272.obj
03010310.obj: 03010310.c
	$(CC) $(CFLAGS) -c  03010310.c -o 03010310.obj
03010311.obj: 03010311.c
	$(CC) $(CFLAGS) -c  03010311.c -o 03010311.obj
