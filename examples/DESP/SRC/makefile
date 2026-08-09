ifeq ($(XSDK),)
$(error Environment variable "XSDK" is undefined)
endif
CC = $(XSDK)/bin/human68k-gcc
AS = $(XSDK)/bin/vasm
RM = $(XSDK)/bin/rm
ELF2X68K = $(XSDK)/bin/elf2x68k
STRIP = $(XSDK)/bin/stripx
CFLAGS = -Ofast -Wno-multichar -fomit-frame-pointer -fstrength-reduce -fforce-addr -fcall-used-d2 -fcall-used-a2
LDFLAGS = -Wl,--no-warn-rwx-segments
LIBS = -ldos -liocs -lbas -lc -lfloatfnc

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<
.s.o:
	$(AS) -I$(XSDK)/human68k/include -quiet -Felf -o $@ $<

files = atari.o bomb.o decode.o disp2.o display.o dsp.o dspmove.o ending.o eneborn.o enekill.o enemove.o \
        enemove2.o enemy.o enepattern.o graph.o gtrans.o item.o loadsp.o name.o sound.o sps.o title.o \
        txput.o vdispint.o version.o zlib.o

all: DSP

DSP: $(files)
	$(CC) $(LDFLAGS) -o ../$@ $(files) $(LIBS)
	$(ELF2X68K) ../$@ -s
 
clean:
	-$(RM) $(files) ../DSP
