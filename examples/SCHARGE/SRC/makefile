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

files = battle.o man.o stick.o atari.o bg.o display.o decode.o \
        item.o textput.o sound.o zlib.o stage.o title.o etc.o cpu.o

all: BATTLE

BATTLE: $(files)
	$(CC) $(LDFLAGS) -o ../$@ $(files) $(LIBS)
	$(ELF2X68K) ../$@ -s

clean:
	-$(RM) $(files) ../BATTLE
