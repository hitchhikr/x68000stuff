ifeq ($(XSDK),)
$(error Environment variable "XSDK" is undefined)
endif
CC = $(XSDK)/bin/human68k-gcc
AS = $(XSDK)/bin/vasm
RM = $(XSDK)/bin/rm
ELF2X68K = $(XSDK)/bin/elf2x68k
STRIP = $(XSDK)/bin/stripx
CFLAGS = -w -Ofast -Wno-multichar -fomit-frame-pointer -fstrength-reduce -fforce-addr -fcall-used-d2 -fcall-used-a2
LDFLAGS = -Wl,--no-warn-rwx-segments
LIBS = -ldos -liocs -lbas -lc -lfloatfnc

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<
.s.o:
	$(AS) -I$(XSDK)/human68k/include -quiet -Felf -o $@ $<

files = main.o basic.o bind.o buffer.o display.o file.o fileio.o line.o random.o region.o search.o \
        spawn.o termio.o window.o word.o exec.o input.o kanji.o wildcard.o system.o hprint.o

all: ME

ME: $(files)
	$(CC) $(LDFLAGS) -O -o ../$@ $(files) $(LIBS)
	$(ELF2X68K) ../$@ -s

clean:
	-$(RM) $(files) ../ME
