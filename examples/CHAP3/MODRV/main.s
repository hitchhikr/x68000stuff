;
; 3.5inch 128Mbytes MO driver for X680x0/Human68k
;
    mc68000
    opt o+
    opt a+

	include	doscalls.inc
	
    section .text,code

    xdef _start

; Device Header
_start:
Deviceheader:
	dc.l	-1		        ; -1 since only one pointer to the next device driver is supported
	dc.w	$0000		    ; Device attributes Block device ioctl not allowed
	dc.l	Strategy	    ; Strategy Routine Entry Point
	dc.l	Interrupt	    ; Interrupt routine entry point
	dc.b	$01,'MOdrv  '	; Device name: Since it is a block device, the first byte must be a code below $20.

; Request Buffer

Request:
	dc.l	0		        ; Stores a pointer to the request header

; Strategy Routines
; input
; a5 : A pointer to the request header
; return
; None

Strategy:
	move.l	a5,Request
	rts

* SCSI ID storage

targetID:
	dc.l	6		        ; Stores the SCSI ID of the MO drive

; Interrupt Routine
; input
; None
; return
; d0 : (Device driver) error code

Interrupt:
	movem.l	d1/a4-a5,-(sp)
	move.l	Request,a5
	lea	JmpTbl,a4
	clr.l	d0
	move.b	2(a5),d0	    ; Enter the command code in d0
	lsl.w	#2,d0		    ; Table Jump
	adda.l	d0,a4
	move.l	(a4),a4
	jsr	(a4)
	move.w	d0,d1
	move.b	d1,3(a5)	    ; Error code (lo 8 bits) setting
	lsr.w	#8,d1
	move.b	d1,4(a5)	    ; Error code (hi 8 bits) setting
	movem.l	(sp)+,d1/a4-a5
	rts

JmpTbl:
	dc.l	Init_all	    ;  0 Initialization
	dc.l	Chk_exchange	;  1 Disk replacement check
	dc.l	ErrorReturn	    ;  2 Unused
	dc.l	ErrorReturn	    ;  3 Input via IOCTRL
	dc.l	MO_Read		    ;  4 Input
	dc.l	Drive_ctrl	    ;  5 Drive Control & Sense
	dc.l	ErrorReturn	    ;  6 Error
	dc.l	ErrorReturn	    ;  7 Errors
	dc.l	MO_Write	    ;  8 Output (verify off)
	dc.l	MO_Verify	    ;  9 Output (verify on)
	dc.l	ErrorReturn	    ; 10 Error
	dc.l	ErrorReturn	    ; 11 Unused
	dc.l	ErrorReturn	    ; 12 Output by IOCTRL

ErrorReturn:
	move.w	#$1003,d0	    ; Abort/Invalid command code
	rts

ReturnTRUE:
	clr.w	d0
	rts

; Command code 1 Disc change check
; input
; a5 : A pointer to the request header
; return
; d0 : (Device driver) error code

Chk_exchange:
	bsr.w	MO_testunit
	tst.w	d0
	beq.b	Chk_ex_not
	move.b	#-1,14(a5)	    ; Because it was replaced
	bra.b	Chk_ex_end
Chk_ex_not:
	move.b	#1,14(a5)	    ; Set 1 as it has not been replaced
Chk_ex_end:
	clr.w	d0
	rts

; Command Code 5 Drive Control & Sense
; input
; a5 : A pointer to the request header
; return
; d0 : (Device driver) error code

Drive_ctrl:
	clr.w	d0
	movem.l	a3,-(sp)
	lea	Drive_jump,a3
	clr.l	d0
	move.b	13(a5),d0	    ; Put internal command in d0
	cmpi.w	#7,d0		    ; d0 contains the MD of the DOS call drvctrl
	bgt.b	Drive_ctrl_nor	    ; So, there are cases where it is 8 or more, but we ignore it.
	lsl.w	#2,d0		    ; Table jump
	adda.l	d0,a3
	move.l	(a3),a3
	jsr	(a3)
Drive_ctrl_nor:
	jsr	Drive_sense
Drive_ctrl_end:
	movem.l	(sp)+,a3
	rts

Drive_jump:
	dc.l	ReturnTRUE	    ; 0 Check the drive status
	dc.l	Drive_eject	    ; 1 Eject
	dc.l	Drive_prevent1	; 2 Eject prohibition 1 (user)
	dc.l	Drive_allow1	; 3 Eject permission 1 (user)
	dc.l	ReturnTRUE	    ; 4 LED lights up when no disc is inserted
	dc.l	ReturnTRUE	    ; 5 LED turns off when no disc is inserted
	dc.l	Drive_prevent2	; 6 Eject Prohibited 2 (OS)
	dc.l	Drive_allow2	; 7 Eject Permission 2 (OS)

Drive_pamedium:			    ; Stores the eject permission/prohibition status
Drive_pamedium1:		    ; User-initiated ejection
	dc.w	0		        ; Allowed: 0 Prohibited: Other than 0
Drive_pamedium2:		    ; Eject by OS
	dc.w	0		        ; Allowed: 0 Prohibited: Other than 0

; Command code 5 Internal code 7 Eject permission 2 (OS)
; input
; a5 : A pointer to the request header
; return
; d0 : (Device driver) error code

Drive_allow2:
	clr.w	Drive_pamedium2
	bra.b	Drive_allow_GO

; Command code 5 Internal code 3 Eject permission 1 (user)
; input
; a5 : A pointer to the request header
; return
; d0 : (Device driver) error code

Drive_allow1:
	clr.w	Drive_pamedium1
Drive_allow_GO:
	tst.l	Drive_pamedium
	bne.b	Drive_allow1_end
	movem.l	d1/d3-d4,-(sp)
	move.l	targetID,d4	    ; MO SCSI ID
	moveq	#0,d3		    ; Eject permission instruction
	moveq	#$32,d1		    ; SCSI call number
	move.l	#$f5,d0		    ; IOCS Call Number
	trap	#$0f		    ; S_PAMEDIUM
	movem.l	(sp)+,d1/d3-d4
Drive_allow1_end:
	clr.l	d0
	rts

; Command code 5 Internal code 6 Eject prohibition 2 (OS)
; input
; a5 : A pointer to the request header
; return
; d0 : (Device driver) error code

Drive_prevent2:
	move.w	#$ff,Drive_pamedium2
	bra.b	Drive_prevent_GO

; Command code 5 Internal code 2 Eject prohibition 1 (user)
; input
; a5 : A pointer to the request header
; return
; d0 : (Device driver) error code

Drive_prevent1:
	move.w	#$ff,Drive_pamedium1
Drive_prevent_GO:
	movem.l	d1/d3-d4,-(sp)
	move.l	targetID,d4	    ; MO SCSI ID
	moveq	#1,d3		    ; Eject prohibition instruction
	moveq	#$32,d1		    ; SCSI call number
	move.l	#$f5,d0		    ; IOCS Call Number
	trap	#$0f		    ; S_PAMEDIUM
	movem.l	(sp)+,d1/d3-d4
	clr.l	d0
	rts

; Command Code 5 Internal Code 1 Eject
; input
; a5 : A pointer to the request header
; return
; d0 : (Device driver) error code

Drive_eject:
	tst.l	Drive_pamedium
	bne.b	Drive_eject_end
Drive_eject_GO:
	movem.l	d1/d3-d4,-(sp)
	move.l	targetID,d4	    ; MO SCSI ID
	moveq	#2,d3		    ; Eject instruction
	moveq	#$2f,d1		    ; SCSI call number
	move.l	#$f5,d0		    ; IOCS Call Number
	trap	#$0f		    ; S_STARTSTOP
	movem.l	(sp)+,d1/d3-d4
Drive_eject_end:
	clr.l	d0
	rts

; Command Code 5 Internal Code 0 Check the drive status
; input
; a5 : A pointer to the request header
; return
; d0 : (Device driver) error code

Drive_sense:
	movem.l	d1,-(sp)
	clr.w	d1
	tst.l	Drive_pamedium	; Check whether to allow or prohibit ejection
	beq.b	Drive_sense_pro
	bset.l	#6,d1
Drive_sense_pro:		    ; Write protection check
	bsr.w	MO_Protect
	tst.l	d0
	beq.b	Drive_sense_ready
	bset.l	#3,d1
Drive_sense_ready:		    ; Check drive availability
	bsr.w	MO_testunit
	bsr.w	MO_testunit
	tst.l	d0
	beq.b	Drive_sense_ok
	bset.l	#2,d1
	bra.b	Drive_sense_end
Drive_sense_ok:
	bset.l	#1,d1
Drive_sense_end:
	move.b	d1,13(a5)	    ; Set the result in the request header
	clr.l	d0
	movem.l	(sp)+,d1
	rts

; Enter command code 4
; input
; a5 : A pointer to the request header
; return
; d0 : (Device driver) error code

MO_Read:
	movem.l	d1-d5/a1,-(sp)	; Check drive availability
	bsr.w	MO_testunit	        ; Regardless of whether the media has been replaced
	bsr.w	MO_testunit	        ; If an error occurs in the second S_TESTUNIT, it cannot be used.
	tst.w	d0
	beq.b	MO_Read_GO
	move.w	#$3002,d0	    ; Abort, Retry/Drive not ready
	bra.b	MO_Read_end
MO_Read_GO:
	move.l	14(a5),a1	    ; A pointer to the transfer buffer
	moveq	#1,d5		    ; Block size: 512 bytes
	move.l	targetID,d4	    ; MO SCSI ID
	move.l	18(a5),d3	    ; Number of sectors
	move.l	22(a5),d2	    ; Starting sector number
	moveq	#$26,d1		    ; SCSI call number
	move.l	#$f5,d0		    ; IOCS Call Number
	trap	#$0f		    ; S_READEXT
	tst.l	d0
	beq.b	MO_Read_end
MO_Read_err:
	move.w	#$700b,d0	    ; Abort, Retry, Ignore/Read Error
MO_Read_end:
	movem.l	(sp)+,d1-d5/a1
	rts

; Command code 9 output (verify on)
; input
; a5 : A pointer to the request header
; return
; d0 : (Device driver) error code

MO_Verify:
	bsr.b	MO_Write	        ; First, write
	tst.l	d0		        ; If there is an error
	bne.b	MO_Verify_ret	    ; Return with an error

	link	a4,#-512
	movem.l	d1-d6/a1-a2,-(sp)
	move.l	14(a5),a2	    ; A pointer to the transfer buffer
	move.l	18(a5),d3	    ; Number of sectors
	move.l	22(a5),d2	    ; Starting sector number
	clr.l	d1		        ; Sector counter
	lea	-512(a4),a1	        ; A pointer to a temporary buffer
	moveq	#1,d5		    ; Sector size 512 bytes
	move.l	targetID,d4	    ; MO SCSI ID
MO_Verify_NextSec:
	movem.l	d1-d5/a1,-(sp)
	moveq	#1,d3		    ; Number of sectors read: 1
	add.l	d1,d2		    ; ì«Ç›çûÇ›à íu
	moveq	#$26,d1		    ; SCSI call number
	move.l	#$f5,d0		    ; IOCS Call Number
	trap	#$0f		    ; S_READEXT
MO_Verify_CHECK:		    ; Compare the read data with the data in the transfer buffer
	move.l	#512-1,d6
MO_Verify_CheckLoop:
	cmpm.b	(a1)+,(a2)+
	dbne	d6,MO_Verify_CheckLoop
MO_Verify_wrong:
	movem.l	(sp)+,d1-d5/a1
	tst.l	d0		        ; If an error occurs in S_READEXT
	bne.b	MO_Verify_err       ; Error
	cmpi.w	#-1,d6		    ; If the collated data is different
	bne.b	MO_Verify_err       ; Error
	addq.l	#1,d1		    ; Increment the counter and check if the number of sectors written is sufficient.
	cmp.l	d1,d3		    ; If not, go to the next sector
	bgt.b	MO_Verify_NextSec
	clr.l	d0
	bra.b	MO_Verify_end
MO_Verify_err:
	move.w	#$700a,d0	    ; Abort, Retry, Ignore/Write Error
MO_Verify_end:
	movem.l	(sp)+,d1-d6/a1-a2
	unlk	a4
MO_Verify_ret:
	rts

; Command code 8 output (verify off)
; input
; a5 : A pointer to the request header
; return
; d0 : (Device driver) error code

MO_Write:
	movem.l	d1-d5/a1,-(sp)	; Check drive availability
	bsr.b	MO_testunit	        ; Regardless of whether the media has been replaced
	bsr.b	MO_testunit	        ; If an error occurs in the second S_TESTUNIT, it cannot be used.
	tst.w	d0
	beq.b	MO_Write_GO
	move.w	#$3002,d0	    ; Abort, Retry/Drive not ready
	bra.b	MO_Write_end
MO_Write_GO:
	bsr.b	MO_Protect	        ; Check write protection
	tst.w	d0
	beq.b	MO_Write_GOGO
	move.w	#$300d,d0	    ; Abort, retry/write protect (cannot be replaced)
	bra.b	MO_Write_end
MO_Write_GOGO:
	move.l	14(a5),a1	    ; A pointer to the transfer buffer
	moveq	#1,d5		    ; Sector size 512 bytes
	move.l	targetID,d4	    ; MO SCSI ID
	move.l	18(a5),d3	    ; Number of sectors
	move.l	22(a5),d2	    ; Start sector position
	moveq	#$27,d1		    ; SCSI call number
	move.l	#$f5,d0		    ; IOCS Call Number
	trap	#$0f		    ; S_WRITEEXT
	tst.l	d0
	beq.b	MO_Write_end
MO_Write_err:
	move.w	#$700a,d0	    ; Abort, Retry, Ignore/Write Error
                            ; If the user selects "Ignore" here, the FAT may become inconsistent,
                            ; so one option is to remove the "Ignore" option.
MO_Write_end:
	movem.l	(sp)+,d1-d5/a1
	rts

;***

; Write protection check
; input
; None
; return
; d0 : 0        Not protected
;      Non-zero Protected or Error

MO_Protect:
	link	a5,#-255
	movem.l	d1-d4/a1,-(sp)
	lea	-255(a5),a1	        ; A pointer to the receive buffer
	move.l	targetID,d4	    ; MO SCSI ID
	move.l	#255,d3		    ; The length of the data to be received
	clr.l	d2		        ; Page code (currently 0)
	moveq	#$29,d1		    ; SCSI call number
	move.l	#$f5,d0		    ; IOCS Call Number
	trap	#$0f		    ; modesense
	tst.l	d0
	bne.b	MO_Pro_end
	btst.b	#$0007,2(a1)	; Check the bit that indicates write protection
	sne	d0
MO_Pro_end:
	movem.l	(sp)+,d1-d4/a1
	unlk	a5
	rts

; Checking drive availability
; input
; None
; return
; d0 : 0        Available
;      Non-zero Not available

MO_testunit:
	movem.l	d1/d4,-(sp)
	move.l	targetID,d4	    ; MO SCSI ID
	moveq	#$24,d1		    ; SCSI call number
	move.l	#$f5,d0		    ; IOCS Call Number
	trap	#$0f		    ; S_TESTUNIT
	movem.l	(sp)+,d1/d4
	rts

; BIOS Parameter Block

BPBdata:
	dc.w	512		        ; Bytes per sector
	dc.b	4		        ; Number of sectors per cluster
	dc.b	$82		        ; Number of FAT areas The 7th bit specifies the endian conversion of FAT
	dc.w	1		        ; Number of sectors in the reserved area:
                            ; 1 sector is reserved as the boot sector
	dc.w	512		        ; Number of entries in the root directory
	dc.w	0		        ; If the total number of sectors cannot be counted in words,
                            ; enter 0 as a long word at the end.
	dc.b	$f0		        ; Media Byte
	dc.b	243		        ; Number of sectors per FAT area
	dc.l	$3cbe0		    ; Total number of sectors

BPBtable:
	dc.l	BPBdata

;***

; Command code 0 Initialization
; input
; a5 : A pointer to the request header
; return
; d0 : (Device driver) error code

Init_all:
	pea	TitleMes	        ; Title display
	DOS	_PRINT
	addq.l	#4,sp
	movem.l	a1,-(sp)
	clr.l	d0
	move.b	#1,13(a5)	    ; Set the number of units
	move.l	#Init_all,14(a5) ; Set the end address of the device driver
	move.l	18(a5),a1	    ; Set the parameter pointer to a1
	move.l	#BPBtable,18(a5) ; Set the BPB table pointer
Init_Search0:
	move.b	(a1)+,d0	    ; Skip the first parameter
	bne.b	Init_Search0
	tst.b	(a1)		    ; If there is no second parameter
	beq.b	Init_Search_MO	    ; Find an MO drive
	move.l	(a1)+,d0
	cmpi.l	#'/id=',d0	    ; Parameter description check
	bne.b	Init_Error
	move.b	(a1)+,d0
	andi.l	#7,d0		    ; d0 contains the specified SCSI ID (a shortcut)
	bra.b	Init_OK
Init_Search_MO:
	bsr.w	Search_MO_id
	tst.l	d0
	bmi.b	Init_MO_notfound
	bra.b	Init_OK
Init_MO_notfound:
	pea	NotfoundMes
	bra.b	Init_err_prnt
Init_Error:
	pea	IllegalMes
Init_err_prnt:
	DOS	_PRINT
	addq.l	#4,sp
	pea	CRLF
	DOS	_PRINT
	addq.l	#4,sp
	move.l	#$700d,d0
	bra.b	Init_END
Init_OK:
	move.l	d0,targetID
	addi.w	#'0',d0		    ; Display the SCSI ID of the MO
	move.w	d0,-(sp)
	DOS	_PUTCHAR
	addq.l	#2,sp
	move.w	#' ',-(sp)
	DOS	_PUTCHAR
	addq.l	#2,sp
	bsr.b	Pr_inquiry	        ; View Inquiry Data
	pea	DriveMes	        ; Display the drive number
	DOS	_PRINT
	addq.l	#4,sp
	clr.w	d0
	move.b	22(a5),d0
	addi.w	#'A',d0
	move.w	d0,-(sp)
	DOS.b	_PUTCHAR
	addq.l	#2,sp
	move.w	#':',-(sp)
	DOS	_PUTCHAR
	addq.l	#2,sp
	pea	CRLF
	DOS	_PRINT
	addq.l	#4,sp
	clr.l	d0
Init_END:
	movem.l	(sp)+,a1
	rts

; View Inquiry Data
; input
; None
; return
; None

Pr_inquiry:
	link	a5,#-80
	movem.l	d0-d1/d3-d4/a1-a2,-(sp)
	lea.l	-80(a5),a1
	movea.l	a1,a2
	moveq	#79,d0
Pr_inq_0:
	clr.b	(a1)+
	dbf	d0,Pr_inq_0
	movea.l	a2,a1		    ; A pointer to the receive buffer
	move.l	targetID,d4	    ; MO SCSI ID
	moveq	#80,d3		    ; The length of the data to be received
	moveq	#$20,d1		    ; SCSI call number
	move.l	#$f5,d0		    ; IOCS Call Number
	trap	#$0f		    ; S_INQUIRY
	tst.l	d0
	bne.b	Pr_inq_err
	pea	Inq_Head
	DOS	_PRINT
	addq.l	#4,sp
	adda.l	#8,a1		    ; The first 8 bytes of the INQUIRY data are ignored
	pea	(a1)
	DOS	_PRINT
	addq.l	#4,sp
Pr_inq_err:
	pea	CRLF
	DOS	_PRINT
	addq.l	#4,sp
	movem.l	(sp)+,d0-d1/d3-d4/a1-a2
	unlk	a5
	rts

; If the first byte of the inquiry data is 7, it is an MO-DRIVE.
; input
; None
; return
; d0 : SCSI ID of MO-DRIVE
;      -1 if you can't find MO-DRIVE

Search_MO_id:
	link	a5,#-48
	movem.l	d1/d3-d4/a1,-(sp)
	lea.l	-$30(a5),a1	    ; A pointer to the receive buffer
	moveq	#7,d4		    ; The SCSI ID of the target
	moveq	#$30,d3		    ; The length of the data to be received
	moveq	#$20,d1		    ; SCSI call number
Search_loop:
	move.l	#$f5,d0		    ; IOCS Call Number
	trap	#$0f		    ; S_INQUIRY
	tst.w	d0		        ; If there is an error
	bne.b	Search_next	        ; Check next SCSI ID
	cmpi.b	#7,0(a1)	    ; Check the first byte of the INQUIRY data
	beq.b	Search_found
Search_next:
	dbf	d4,Search_loop
	moveq	#-1,d0		    ; Not found on the SCSI bus
	bra.b	Search_end
Search_found:
	move.l	d4,targetID
	move.l	d4,d0
Search_end:
	movem.l	(sp)+,d1/d3-d4/a1
	unlk	a5
	rts

	section .data,data

TitleMes:
	dc.b	$0d,$0a,'MO Driver ver.1.01N by J.Mitsugi(KMC)',$0d,$0a
	dc.b	'SCSI ID : ',$00
	even
Inq_Head:
	dc.b	$0d,$0a,'Identifier : ',$00
	even
IllegalMes:
	dc.b	'illegal',$00
	even
NotfoundMes:
	dc.b	'MO DRIVE not found',$00
	even
DriveMes:
	dc.b	'MO Drive is ',$00
	even
CRLF:
	dc.b	$0d,$0a,$00

	end
