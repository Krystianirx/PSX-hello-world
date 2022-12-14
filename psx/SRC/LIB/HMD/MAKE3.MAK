CC	= ccpsx
DMPSX	= dmpsx
CFLAGS	= -O3

OBJS = \
	0300403a.o2	\
	0300503a.o2	\
	0300003b.o2	\
	0300103b.o2	\
	0300203b.o2	\
	0300303b.o2	\
	0300403b.o2	\
	0300503b.o2	\
	03000111.o2	\
	03001111.o2	\
	03002111.o2	\
	03003111.o2	\
	03004111.o2	\
	03005111.o2	\
	03000112.o2	\
	03001112.o2	\
	03002112.o2	\
	03003112.o2	\
	03004112.o2	\
	03005112.o2	\
	03000119.o2	\
	03001119.o2	\
	03002119.o2	\
	03003119.o2	\
	03004119.o2	\
	03005119.o2	\
	0300011a.o2	\
	0300111a.o2	\
	0300211a.o2	\
	0300311a.o2	\
	0300411a.o2	\
	0300511a.o2	\
	03000910.o2	\
	03001910.o2	\
	03002910.o2	\
	03003910.o2	\
	03004910.o2	\
	03005910.o2	\
	03000911.o2	\
	03001911.o2	\
	03002911.o2	\
	03003911.o2	\
	03004911.o2	\
	03005911.o2	\
	03000912.o2	\
	03001912.o2	\
	03002912.o2	\
	03003912.o2	\
	03004912.o2	\
	03005912.o2	\
	03000919.o2	\
	03001919.o2	\
	03002919.o2	\
	03003919.o2	\
	03004919.o2	\
	03005919.o2	\
	0300091a.o2	\
	0300191a.o2	\
	0300291a.o2	\
	0300391a.o2	\
	0300491a.o2	\
	0300591a.o2	\
	mime_s.o2

all: $(OBJS)
	@echo done

0300403a.obj: 0300003a.c
	$(CC) $(CFLAGS) -c -DZXY 0300003a.c -o 0300403a.obj
0300403a.o2: 0300403a.obj
	$(DMPSX) -o 0300403a.o2 0300403a.obj
0300503a.obj: 0300003a.c
	$(CC) $(CFLAGS) -c -DZYX 0300003a.c -o 0300503a.obj
0300503a.o2: 0300503a.obj
	$(DMPSX) -o 0300503a.o2 0300503a.obj
0300003b.obj: 0300003b.c
	$(CC) $(CFLAGS) -c -DXYZ 0300003b.c -o 0300003b.obj
0300003b.o2: 0300003b.obj
	$(DMPSX) -o 0300003b.o2 0300003b.obj
0300103b.obj: 0300003b.c
	$(CC) $(CFLAGS) -c -DXZY 0300003b.c -o 0300103b.obj
0300103b.o2: 0300103b.obj
	$(DMPSX) -o 0300103b.o2 0300103b.obj
0300203b.obj: 0300003b.c
	$(CC) $(CFLAGS) -c -DYXZ 0300003b.c -o 0300203b.obj
0300203b.o2: 0300203b.obj
	$(DMPSX) -o 0300203b.o2 0300203b.obj
0300303b.obj: 0300003b.c
	$(CC) $(CFLAGS) -c -DYZX 0300003b.c -o 0300303b.obj
0300303b.o2: 0300303b.obj
	$(DMPSX) -o 0300303b.o2 0300303b.obj
0300403b.obj: 0300003b.c
	$(CC) $(CFLAGS) -c -DZXY 0300003b.c -o 0300403b.obj
0300403b.o2: 0300403b.obj
	$(DMPSX) -o 0300403b.o2 0300403b.obj
0300503b.obj: 0300003b.c
	$(CC) $(CFLAGS) -c -DZYX 0300003b.c -o 0300503b.obj
0300503b.o2: 0300503b.obj
	$(DMPSX) -o 0300503b.o2 0300503b.obj
03000111.obj: 03000111.c
	$(CC) $(CFLAGS) -c -DXYZ 03000111.c -o 03000111.obj
03000111.o2: 03000111.obj
	$(DMPSX) -o 03000111.o2 03000111.obj
03001111.obj: 03000111.c
	$(CC) $(CFLAGS) -c -DXZY 03000111.c -o 03001111.obj
03001111.o2: 03001111.obj
	$(DMPSX) -o 03001111.o2 03001111.obj
03002111.obj: 03000111.c
	$(CC) $(CFLAGS) -c -DYXZ 03000111.c -o 03002111.obj
03002111.o2: 03002111.obj
	$(DMPSX) -o 03002111.o2 03002111.obj
03003111.obj: 03000111.c
	$(CC) $(CFLAGS) -c -DYZX 03000111.c -o 03003111.obj
03003111.o2: 03003111.obj
	$(DMPSX) -o 03003111.o2 03003111.obj
03004111.obj: 03000111.c
	$(CC) $(CFLAGS) -c -DZXY 03000111.c -o 03004111.obj
03004111.o2: 03004111.obj
	$(DMPSX) -o 03004111.o2 03004111.obj
03005111.obj: 03000111.c
	$(CC) $(CFLAGS) -c -DZYX 03000111.c -o 03005111.obj
03005111.o2: 03005111.obj
	$(DMPSX) -o 03005111.o2 03005111.obj
03000112.obj: 03000112.c
	$(CC) $(CFLAGS) -c -DXYZ 03000112.c -o 03000112.obj
03000112.o2: 03000112.obj
	$(DMPSX) -o 03000112.o2 03000112.obj
03001112.obj: 03000112.c
	$(CC) $(CFLAGS) -c -DXZY 03000112.c -o 03001112.obj
03001112.o2: 03001112.obj
	$(DMPSX) -o 03001112.o2 03001112.obj
03002112.obj: 03000112.c
	$(CC) $(CFLAGS) -c -DYXZ 03000112.c -o 03002112.obj
03002112.o2: 03002112.obj
	$(DMPSX) -o 03002112.o2 03002112.obj
03003112.obj: 03000112.c
	$(CC) $(CFLAGS) -c -DYZX 03000112.c -o 03003112.obj
03003112.o2: 03003112.obj
	$(DMPSX) -o 03003112.o2 03003112.obj
03004112.obj: 03000112.c
	$(CC) $(CFLAGS) -c -DZXY 03000112.c -o 03004112.obj
03004112.o2: 03004112.obj
	$(DMPSX) -o 03004112.o2 03004112.obj
03005112.obj: 03000112.c
	$(CC) $(CFLAGS) -c -DZYX 03000112.c -o 03005112.obj
03005112.o2: 03005112.obj
	$(DMPSX) -o 03005112.o2 03005112.obj
03000119.obj: 03000119.c
	$(CC) $(CFLAGS) -c -DXYZ 03000119.c -o 03000119.obj
03000119.o2: 03000119.obj
	$(DMPSX) -o 03000119.o2 03000119.obj
03001119.obj: 03000119.c
	$(CC) $(CFLAGS) -c -DXZY 03000119.c -o 03001119.obj
03001119.o2: 03001119.obj
	$(DMPSX) -o 03001119.o2 03001119.obj
03002119.obj: 03000119.c
	$(CC) $(CFLAGS) -c -DYXZ 03000119.c -o 03002119.obj
03002119.o2: 03002119.obj
	$(DMPSX) -o 03002119.o2 03002119.obj
03003119.obj: 03000119.c
	$(CC) $(CFLAGS) -c -DYZX 03000119.c -o 03003119.obj
03003119.o2: 03003119.obj
	$(DMPSX) -o 03003119.o2 03003119.obj
03004119.obj: 03000119.c
	$(CC) $(CFLAGS) -c -DZXY 03000119.c -o 03004119.obj
03004119.o2: 03004119.obj
	$(DMPSX) -o 03004119.o2 03004119.obj
03005119.obj: 03000119.c
	$(CC) $(CFLAGS) -c -DZYX 03000119.c -o 03005119.obj
03005119.o2: 03005119.obj
	$(DMPSX) -o 03005119.o2 03005119.obj
0300011a.obj: 0300011a.c
	$(CC) $(CFLAGS) -c -DXYZ 0300011a.c -o 0300011a.obj
0300011a.o2: 0300011a.obj
	$(DMPSX) -o 0300011a.o2 0300011a.obj
0300111a.obj: 0300011a.c
	$(CC) $(CFLAGS) -c -DXZY 0300011a.c -o 0300111a.obj
0300111a.o2: 0300111a.obj
	$(DMPSX) -o 0300111a.o2 0300111a.obj
0300211a.obj: 0300011a.c
	$(CC) $(CFLAGS) -c -DYXZ 0300011a.c -o 0300211a.obj
0300211a.o2: 0300211a.obj
	$(DMPSX) -o 0300211a.o2 0300211a.obj
0300311a.obj: 0300011a.c
	$(CC) $(CFLAGS) -c -DYZX 0300011a.c -o 0300311a.obj
0300311a.o2: 0300311a.obj
	$(DMPSX) -o 0300311a.o2 0300311a.obj
0300411a.obj: 0300011a.c
	$(CC) $(CFLAGS) -c -DZXY 0300011a.c -o 0300411a.obj
0300411a.o2: 0300411a.obj
	$(DMPSX) -o 0300411a.o2 0300411a.obj
0300511a.obj: 0300011a.c
	$(CC) $(CFLAGS) -c -DZYX 0300011a.c -o 0300511a.obj
0300511a.o2: 0300511a.obj
	$(DMPSX) -o 0300511a.o2 0300511a.obj
03000910.obj: 03000910.c
	$(CC) $(CFLAGS) -c -DXYZ 03000910.c -o 03000910.obj
03000910.o2: 03000910.obj
	$(DMPSX) -o 03000910.o2 03000910.obj
03001910.obj: 03000910.c
	$(CC) $(CFLAGS) -c -DXZY 03000910.c -o 03001910.obj
03001910.o2: 03001910.obj
	$(DMPSX) -o 03001910.o2 03001910.obj
03002910.obj: 03000910.c
	$(CC) $(CFLAGS) -c -DYXZ 03000910.c -o 03002910.obj
03002910.o2: 03002910.obj
	$(DMPSX) -o 03002910.o2 03002910.obj
03003910.obj: 03000910.c
	$(CC) $(CFLAGS) -c -DYZX 03000910.c -o 03003910.obj
03003910.o2: 03003910.obj
	$(DMPSX) -o 03003910.o2 03003910.obj
03004910.obj: 03000910.c
	$(CC) $(CFLAGS) -c -DZXY 03000910.c -o 03004910.obj
03004910.o2: 03004910.obj
	$(DMPSX) -o 03004910.o2 03004910.obj
03005910.obj: 03000910.c
	$(CC) $(CFLAGS) -c -DZYX 03000910.c -o 03005910.obj
03005910.o2: 03005910.obj
	$(DMPSX) -o 03005910.o2 03005910.obj
03000911.obj: 03000911.c
	$(CC) $(CFLAGS) -c -DXYZ 03000911.c -o 03000911.obj
03000911.o2: 03000911.obj
	$(DMPSX) -o 03000911.o2 03000911.obj
03001911.obj: 03000911.c
	$(CC) $(CFLAGS) -c -DXZY 03000911.c -o 03001911.obj
03001911.o2: 03001911.obj
	$(DMPSX) -o 03001911.o2 03001911.obj
03002911.obj: 03000911.c
	$(CC) $(CFLAGS) -c -DYXZ 03000911.c -o 03002911.obj
03002911.o2: 03002911.obj
	$(DMPSX) -o 03002911.o2 03002911.obj
03003911.obj: 03000911.c
	$(CC) $(CFLAGS) -c -DYZX 03000911.c -o 03003911.obj
03003911.o2: 03003911.obj
	$(DMPSX) -o 03003911.o2 03003911.obj
03004911.obj: 03000911.c
	$(CC) $(CFLAGS) -c -DZXY 03000911.c -o 03004911.obj
03004911.o2: 03004911.obj
	$(DMPSX) -o 03004911.o2 03004911.obj
03005911.obj: 03000911.c
	$(CC) $(CFLAGS) -c -DZYX 03000911.c -o 03005911.obj
03005911.o2: 03005911.obj
	$(DMPSX) -o 03005911.o2 03005911.obj
03000912.obj: 03000912.c
	$(CC) $(CFLAGS) -c -DXYZ 03000912.c -o 03000912.obj
03000912.o2: 03000912.obj
	$(DMPSX) -o 03000912.o2 03000912.obj
03001912.obj: 03000912.c
	$(CC) $(CFLAGS) -c -DXZY 03000912.c -o 03001912.obj
03001912.o2: 03001912.obj
	$(DMPSX) -o 03001912.o2 03001912.obj
03002912.obj: 03000912.c
	$(CC) $(CFLAGS) -c -DYXZ 03000912.c -o 03002912.obj
03002912.o2: 03002912.obj
	$(DMPSX) -o 03002912.o2 03002912.obj
03003912.obj: 03000912.c
	$(CC) $(CFLAGS) -c -DYZX 03000912.c -o 03003912.obj
03003912.o2: 03003912.obj
	$(DMPSX) -o 03003912.o2 03003912.obj
03004912.obj: 03000912.c
	$(CC) $(CFLAGS) -c -DZXY 03000912.c -o 03004912.obj
03004912.o2: 03004912.obj
	$(DMPSX) -o 03004912.o2 03004912.obj
03005912.obj: 03000912.c
	$(CC) $(CFLAGS) -c -DZYX 03000912.c -o 03005912.obj
03005912.o2: 03005912.obj
	$(DMPSX) -o 03005912.o2 03005912.obj
03000919.obj: 03000919.c
	$(CC) $(CFLAGS) -c -DXYZ 03000919.c -o 03000919.obj
03000919.o2: 03000919.obj
	$(DMPSX) -o 03000919.o2 03000919.obj
03001919.obj: 03000919.c
	$(CC) $(CFLAGS) -c -DXZY 03000919.c -o 03001919.obj
03001919.o2: 03001919.obj
	$(DMPSX) -o 03001919.o2 03001919.obj
03002919.obj: 03000919.c
	$(CC) $(CFLAGS) -c -DYXZ 03000919.c -o 03002919.obj
03002919.o2: 03002919.obj
	$(DMPSX) -o 03002919.o2 03002919.obj
03003919.obj: 03000919.c
	$(CC) $(CFLAGS) -c -DYZX 03000919.c -o 03003919.obj
03003919.o2: 03003919.obj
	$(DMPSX) -o 03003919.o2 03003919.obj
03004919.obj: 03000919.c
	$(CC) $(CFLAGS) -c -DZXY 03000919.c -o 03004919.obj
03004919.o2: 03004919.obj
	$(DMPSX) -o 03004919.o2 03004919.obj
03005919.obj: 03000919.c
	$(CC) $(CFLAGS) -c -DZYX 03000919.c -o 03005919.obj
03005919.o2: 03005919.obj
	$(DMPSX) -o 03005919.o2 03005919.obj
0300091a.obj: 0300091a.c
	$(CC) $(CFLAGS) -c -DXYZ 0300091a.c -o 0300091a.obj
0300091a.o2: 0300091a.obj
	$(DMPSX) -o 0300091a.o2 0300091a.obj
0300191a.obj: 0300091a.c
	$(CC) $(CFLAGS) -c -DXZY 0300091a.c -o 0300191a.obj
0300191a.o2: 0300191a.obj
	$(DMPSX) -o 0300191a.o2 0300191a.obj
0300291a.obj: 0300091a.c
	$(CC) $(CFLAGS) -c -DYXZ 0300091a.c -o 0300291a.obj
0300291a.o2: 0300291a.obj
	$(DMPSX) -o 0300291a.o2 0300291a.obj
0300391a.obj: 0300091a.c
	$(CC) $(CFLAGS) -c -DYZX 0300091a.c -o 0300391a.obj
0300391a.o2: 0300391a.obj
	$(DMPSX) -o 0300391a.o2 0300391a.obj
0300491a.obj: 0300091a.c
	$(CC) $(CFLAGS) -c -DZXY 0300091a.c -o 0300491a.obj
0300491a.o2: 0300491a.obj
	$(DMPSX) -o 0300491a.o2 0300491a.obj
0300591a.obj: 0300091a.c
	$(CC) $(CFLAGS) -c -DZYX 0300091a.c -o 0300591a.obj
0300591a.o2: 0300591a.obj
	$(DMPSX) -o 0300591a.o2 0300591a.obj
mime_s.obj: mime_s.asm
	$(CC) $(CFLAGS) -c  mime_s.asm -o mime_s.obj
mime_s.o2: mime_s.obj
	$(DMPSX) -o mime_s.o2 mime_s.obj
