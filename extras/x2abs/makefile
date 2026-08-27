CC	= gcc
LD	= ld
CFLAGS	= -Os
LDFLAGS	= -s

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

files = x2abs.o

x2abs: $(files)
	$(CC) -o x2abs $(files) $(LDFLAGS)

clean:
	-rm $(files)
    
