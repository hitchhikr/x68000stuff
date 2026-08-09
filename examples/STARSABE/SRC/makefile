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

files = main.o dim.o matrix.o getkey.o calc.o star.o model.o timelist.o enemy.o motion.o move.o \
        setbox.o spdisp.o grdisp.o txdisp.o sound.o interupt.o disp.o intrupt.o

all: RUN

RUN: $(files)
	$(CC) $(LDFLAGS) -o ../$@ $(files) $(LIBS)
	$(ELF2X68K) ../$@ -s

clean:
	-$(RM) $(files) ../RUN
