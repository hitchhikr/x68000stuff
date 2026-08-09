ifeq ($(XSDK),)
$(error Environment variable "XSDK" is undefined)
endif
CC = $(XSDK)/bin/human68k-gcc
AS = $(XSDK)/bin/vasm
RM = $(XSDK)/bin/rm
OBJCOPY	= $(XSDK)/bin/human68k-objcopy
STRIP = $(XSDK)/bin/stripx
PACKER = $(XSDK)/bin/salvador
CFLAGS = -Ofast -Wno-multichar -fomit-frame-pointer -fstrength-reduce -fforce-addr -fcall-used-d2 -fcall-used-a2
LDFLAGS = -Wl,--no-warn-rwx-segments -nostartfiles
LIBS = 

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<
.s.o:
	$(AS) -I$(XSDK)/human68k/include -nomsg=2050 -nomsg=2054 -nomsg=2052 -devpac -quiet -Felf -o $@ $<

files = main.o
depacker = unzx0_68000.o

all: WOBBLE WOBBLE.R

WOBBLE: $(files)
	$(CC) $(LDFLAGS) -o $@ $(files) $(LIBS)
	$(OBJCOPY) -O binary $@ $@
	$(PACKER) -stats $@ PACKED

WOBBLE.R: $(depacker)
	$(CC) $(LDFLAGS) -o $@ $(depacker) $(LIBS)
	$(OBJCOPY) -O binary $@ $@
	-$(RM) WOBBLE PACKED

clean:
	-$(RM) $(files) $(depacker)
