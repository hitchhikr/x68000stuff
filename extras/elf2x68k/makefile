CC	= g++
LD	= ld
CFLAGS	= -Os
LDFLAGS	= -s

.cpp.o:
	$(CC) $(CFLAGS) -o $@ -c $<

files = elf2x68k.o

elf2x68k: $(files)
	$(CC) -o elf2x68k $(files) $(LDFLAGS)

clean:
	-rm $(files)
    
