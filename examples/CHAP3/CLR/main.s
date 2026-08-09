;*********************************************
;*  Filename clr.r
;*  Time Stamp Date 94-06-25
;*             Time 17:16:16
;*
;*  Base address 000000
;*  Exec address 000000
;*  Text size    0000c8 bytes
;*  Data size    000000 byte(s)
;*  Bss  size    000000 byte(s)
;*  3 Labels
;*  Code Generate date Thu Feb 23 07:43:21 1995
;*  Command Line A:\usr\pds\dis.r clr.r clr.s 
;*          DIS.X version 2.01b
;*********************************************
    mc68000
    opt o+
    opt a+

	include	doscalls.inc
	include	iocscalls.inc
	
    section .text,code
    xdef    _start

_start:
	IOCS	_B_CUROFF
	move.w	#-1,-(a7)
	move.w	#14,-(a7)
	DOS	    _CONCTRL
	addq.l	#4,a7
	move.w	d0,-(a7)
	suba.l	a1,a1
	IOCS	_B_SUPER
	move.l	d0,-(a7)
	movea.l	#$E8002A,a6
	move.w	(a6),d7
	move.w	d7,-(a7)
	or.w	#%111110000,d7
	move.w	d7,(a6)
	movea.l	#$E04924,a0
	movea.l	#$E09248,a1
	movea.l	#$E0DB6C,a2
	movea.l	#$E12490,a3
	movea.l	#$E16DB4,a4
	movea.l	#$E1B6D8,a5
	movea.l	#$E20000,a6
	moveq	#0,d0
	move.l	d0,d1
	move.l	d0,d2
	move.l	d0,d3
	move.l	d0,d4
	move.l	d0,d5
	move.l	d0,d6
	movem.l	d0-d4,-(a0)
	movem.l	d0-d4,-(a1)
	movem.l	d0-d4,-(a2)
	movem.l	d0-d4,-(a3)
	movem.l	d0-d4,-(a4)
	movem.l	d0-d4,-(a5)
	movem.l	d0-d5,-(a6)
	move.l	#668-1,d7
.loop:
	movem.l	d0-d6,-(a0)
	movem.l	d0-d6,-(a1)
	movem.l	d0-d6,-(a2)
	movem.l	d0-d6,-(a3)
	movem.l	d0-d6,-(a4)
	movem.l	d0-d6,-(a5)
	movem.l	d0-d6,-(a6)
	dbra	d7,.loop
	movea.l	#$E8002A,a6
	move.w	(a7)+,d7
	move.w	d7,(a6)
	IOCS	_B_CURON
	IOCS	_B_LOCATE
	movea.l	(a7)+,a1
	IOCS	_B_SUPER
	move.w	#14,-(a7)
	DOS	    _CONCTRL
	addq.l	#4,a7
	DOS	    _EXIT
