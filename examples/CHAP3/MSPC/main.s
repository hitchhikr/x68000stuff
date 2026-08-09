; Mouse Pointer Counter ver 0.99 by hal@kmc.kyoto-u.ac.jp

    mc68000
    opt o+
    opt a+

	include	iocscalls.inc
	include	doscalls.inc

    xdef _start

	section .text,code

prog_start:
	dc.b	'mspc'
old_vector:
	dc.l	0
stack:
	dc.l	0
	dc.l	0
Initialization:
	movem.l	d0-d7/a0-a6,-(sp)
	lea.l	stack,a6
	move.l	(a6)+,d7				;d7: Previous cell $0000-Normal, $ffff-Counter
	move.l	(a6)+,d2				;d2: The place you were at the time before
	IOCS	_MS_STAT
	tst.w	d0						;d0: Is the mouse pointer visible?
	beq		theend					;If the mouse pointer is not there, you're done.
	IOCS	_B_SFTSNS
	andi.w	#3,d0
	bne		change_cell			    ;If the shift or control key is pressed, turn the cell into a counter.
	tst.w	d7
	beq		theend					;If the previous cell was normal, then it's over.
	clr.w	d1						;d1: Cell number
	clr.w	d7
	IOCS	_MS_SEL					;Reset the cell to its default setting.
theend:
	move.l	d2,-(a6)
	move.l	d7,-(a6)
	movem.l	(sp)+,d0-d7/a0-a6
	move.l	old_vector,-(sp)
	rts

change_cell:
	move.w	d0,d4					;d4: Key Sense
	IOCS	_MS_CURGT				;d0: Current location
;(
	tst.w	d7						;d7: Previous cell $0000-Normal, $ffff-Counter
	beq		LA						;The previous cell is a counter, and
;)
	cmp.l	d2,d0					;If the location has not moved, you can finish without rewriting the cell.
	beq		theend
LA:
	lea.l	cell_pattern,a1		    ;a1: Address where the cell pattern is located
	lea.l	numbers_font,a3		    ;a3: Address with number font - 10
;*************************************Dataset
	move.l	d0,d2					;d2: The place you were at the time before
	move.w	d0,d3					;d3: y coordinate
	swap	d0						;d0: x coordinate
	cmpi.w	#1,d4
	beq		LB
	lsr.w	#3,d0					;If the control key is pressed, it will be corrected to a character unit.
	lsr.w	#4,d3
LB:
	lea.l	x_coord_placeholder,a0
	bsr		transfer			    ;inp(d3)/ret(a0,a3)/brk(d3,d4,d5,d6,a2,a4)
	move.w	d0,d3
	lea.l	y_coord_placeholder,a0
	bsr		transfer
	move.w	#1,d1					;d1: Cell number
	IOCS	_MS_PATST
	IOCS	_MS_SEL
	move.w	#$ffff,d7
	bra		theend

transfer:
*************************************Convert numeric value (d3) to number
	ext.l	d3
	divu.w	#10,d3
	move.l	d3,d5
	swap	d5						;d5.w = ones digit
	ext.l	d3
	divu.w	#10,d3
	move.l	d3,d4					;d4.w = tens place
	swap	d4						;d3.w = hundreds digit
	ext.l	d3
;	divu.w	#10,d3					*Uncomment this to properly handle the thousands digit.
;	swap	d3
*************************************Dataset
*************************************(100)
	movea.l	a0,a2
	movea.l	a3,a4
hundreds:
	lea.l	10(a4),a4
	dbra	d3,hundreds
 	move.w	(a4)+,(a2)+				;1
	move.w	(a4)+,(a2)+				;2
	move.w	(a4)+,(a2)+				;3
	move.w	(a4)+,(a2)+				;4
	move.w	(a4),(a2)				;5
*************************************(10)
	movea.l	a0,a2
	movea.l	a3,a4
tens:
	lea.l	10(a4),a4
	dbra	d4,tens
	move.w	(a4)+,d6				;1
	asr.w	#5,d6
	and.w	d6,(a2)+
	move.w	(a4)+,d6				;2
	asr.w	#5,d6
	and.w	d6,(a2)+
	move.w	(a4)+,d6				;3
	asr.w	#5,d6
	and.w	d6,(a2)+
	move.w	(a4)+,d6				;4
	asr.w	#5,d6
	and.w	d6,(a2)+
	move.w	(a4),d6					;5
	asr.w	#5,d6
	and.w	d6,(a2)
*************************************(1)
	movea.l	a0,a2
	movea.l	a3,a4
ones:
	lea.l	10(a4),a4
	dbra	d5,ones
	move.w	(a4)+,d6				;1
	asr.w	#8,d6
	asr.w	#2,d6
	and.w	d6,(a2)+
	move.w	(a4)+,d6				;2
	asr.w	#8,d6
	asr.w	#2,d6
	and.w	d6,(a2)+
	move.w	(a4)+,d6				;3
	asr.w	#8,d6
	asr.w	#2,d6
	and.w	d6,(a2)+
	move.w	(a4)+,d6				;4
	asr.w	#8,d6
	asr.w	#2,d6
	and.w	d6,(a2)+
	move.w	(a4),d6					;5
	asr.w	#8,d6
	asr.w	#2,d6
	and.w	d6,(a2)
	rts

cell_pattern:
	dc.w	0,0						;	2word = 4byte
	dc.w	%0111111111111111		;4	1word = 2byte
	dc.w	%0011111111111111		;6
	dc.w	%0001111111111111		;8
	dc.w	%0000000000000000		;10
	dc.w	%1111111111111111		;12
x_coord_placeholder:
	dc.w	%1111111111111111		;14**
	dc.w	%1111111111111111		;16
	dc.w	%1111111111111111		;18
	dc.w	%1111111111111111		;20
	dc.w	%1111111111111111		;22
	dc.w	%1111111111111111		;24
y_coord_placeholder:
	dc.w	%1111111111111111		;26**
	dc.w	%1111111111111111		;
	dc.w	%1111111111111111		;
	dc.w	%1111111111111111		;
	dc.w	%1111111111111111		;
	dc.w	%0000000000000000		;
	dc.w	%0000000000000000
	dc.w	%0000000000000000
	dc.w	%0000000000000000
	dc.w	%0000000000000000
	dc.w	%0000000000000000
	dc.w	%0000000000000000
	dc.w	%0000000000000000
	dc.w	%0000000000000000
	dc.w	%0000000000000000
	dc.w	%0000000000000000
numbers_font:
	dc.w	%0000000000000000
	dc.w	%0000000000000000
	dc.w	%0000000000000000
	dc.w	%0000000000000000
	dc.w	%0000000000000000		;End of cell pattern
;Number font data start
	dc.w	%1000111111111111		;0
	dc.w	%1010111111111111
	dc.w	%1010111111111111
	dc.w	%1010111111111111
	dc.w	%1000111111111111
	dc.w	%1101111111111111		;1
	dc.w	%1101111111111111
	dc.w	%1101111111111111
	dc.w	%1101111111111111
	dc.w	%1101111111111111
	dc.w	%1000111111111111		;2
	dc.w	%1110111111111111
	dc.w	%1000111111111111
	dc.w	%1011111111111111
	dc.w	%1000111111111111
	dc.w	%1000111111111111		;3
	dc.w	%1110111111111111
	dc.w	%1000111111111111
	dc.w	%1110111111111111
	dc.w	%1000111111111111
	dc.w	%1010111111111111		;4
	dc.w	%1010111111111111
	dc.w	%1000111111111111
	dc.w	%1110111111111111
	dc.w	%1110111111111111
	dc.w	%1000111111111111		;5
	dc.w	%1011111111111111
	dc.w	%1000111111111111
	dc.w	%1110111111111111
	dc.w	%1000111111111111
	dc.w	%1000111111111111		;6
	dc.w	%1011111111111111
	dc.w	%1000111111111111
	dc.w	%1010111111111111
	dc.w	%1000111111111111
	dc.w	%1000111111111111		;7
	dc.w	%1010111111111111
	dc.w	%1110111111111111
	dc.w	%1110111111111111
	dc.w	%1110111111111111
	dc.w	%1000111111111111		;8
	dc.w	%1010111111111111
	dc.w	%1000111111111111
	dc.w	%1010111111111111
	dc.w	%1000111111111111
	dc.w	%1000111111111111		;9
	dc.w	%1010111111111111
	dc.w	%1000111111111111
	dc.w	%1110111111111111
	dc.w	%1000111111111111
prog_end:

_start:
	bsr		are_we_resident		    ;ret(d7,a5)/brk(d0,d7,a0,a1,a5,a6)
check_options:
	move.b	1(a2),d0
	cmp.b	#'-',d0
	beq		there_was_an_option
	cmp.b	#'/',d0
	bne		no_options
there_was_an_option:
	move.b	2(a2),d0
	cmp.b	#'s',d0
	beq		install_resident_option
	cmp.b	#'S',d0
	beq		install_resident_option
	cmp.b	#'r',d0
	beq		remove_resident_option
	cmp.b	#'R',d0
	beq		remove_resident_option
no_options:
	lea.l	usage_message,a1
	IOCS	_B_PRINT
	tst.w	d7
	bne		L1
	lea.l	currently_resident_message,a1
	bra		L2
L1:
	lea.l	currently_not_resident_message,a1
L2:
	IOCS	_B_PRINT
	DOS		_EXIT

install_resident_option:
	tst.w	d7
	bne		.already_resident
	lea.l	already_resident_message,a1
	IOCS	_B_PRINT
	DOS		_EXIT
.already_resident:
	lea.l	installing_message,a1
	IOCS	_B_PRINT
	lea.l	Initialization(pc),a1
	move.w	#$0045,d1
	IOCS	_B_INTVCS
	move.l	d0,old_vector
	clr.w	-(sp)
	move.l	#prog_end-prog_start,-(sp)
	DOS		_KEEPPR

remove_resident_option:
	tst.w	d7
	beq		.unresident
	lea.l	cannot_be_removed_message,a1
	IOCS	_B_PRINT
	DOS		_EXIT
.unresident:
	lea.l	disabled_message,a1
	IOCS	_B_PRINT
	movea.l	(a5),a1
	move.w	#$0045,d1
	IOCS	_B_INTVCS
	pea.l	$0010(a0)
	DOS		_MFREE
	addq.l	#4,sp
	DOS		_EXIT

are_we_resident:
	move.w	#$ffff,d7			    ;If it is not resident, d7.w = $ffff
	suba.l	a1,a1
	IOCS	_B_SUPER
	move.l	d0,a1
.investigate:
	move.l	(a0),d0
	beq		.return
	movea.l	d0,a0
	lea.l	$0100(a0),a5
	lea.l	prog_start(pc),a6
	moveq.l	#$03,d0
.keyword_check:
	cmpm.b	(a5)+,(a6)+
	bne		.investigate
	dbra	d0,.keyword_check
	clr.w	d7				        ;Since it was resident, d7.w = 0
.return:
	IOCS	_B_SUPER
	rts

usage_message:
	dc.b	13,10,'MSPC.X --- MouSe Pointer Counter --- ver 0.99 by hal',13,10
	dc.b	'usage: MSPC (options)',13,10
	dc.b	'options: -s    I will be there.',13,10
	dc.b	'         -r    Cancels residency.',13,10,10
	dc.b	'When the mouse pointer is displayed, the coordinates of the mouse pointer are displayed.',13,10
	dc.b	'Shift key: Dot units',13,10
	dc.b	'Control key: Character unit',13,10,0
currently_resident_message:
	dc.b	'Currently, MSPC.X is resident.',13,10,0
currently_not_resident_message:
	dc.b	'Currently, MSPC.X is not resident.',13,10,0
installing_message:
	dc.b	'Made MSPC.X resident.',13,10,0
disabled_message:
	dc.b	'MSPC.X has been disabled as a resident program.',13,10,0
already_resident_message:
	dc.b	'MSPC.X is already resident. It cannot remain resident any longer.',13,10,0
cannot_be_removed_message:
	dc.b	'MSPC.X is not resident, so it cannot be made removed.',13,10,0
	
    end
