; =======================================================
; Wobble
; A 506 bytes effect for the X68000.
; Written by Franck 'hitchhikr' Charlet.
; NOTE: NO EMULATOR CAN RENDER THIS PROPERLY
;       DUE TO THE SPRITES BEING RELOADED AT EACH LINE !
; =======================================================

; =======================================================
_CRTMOD                 equ     $10                             ; Set Screen Attributes
_VDISPST                equ     $6c                             ; De/Install a vblank interrupt handler
_CRTCRAS                equ     $6d                             ; De/Install a raster interrupt handler

_EXIT		            equ	    $ff00                           ; Terminate program
_SUPER		            equ	    $ff20                           ; Switch to supervisor/user mode

TVRAM_PLANE0            equ     $E00000                         ; TVRAM planes
CRT_RASTER              equ     $E80012                         ; Raster number
CRT_TEXT_X              equ     $E80014                         ; TVRAM planes X Position
GFX_EN                  equ     $E82600                         ; Enable/Disable various gfx stuff
MFP_GPIP                equ     $E88001                         ; General-purpose I/O register
MFP_IERA                equ     $E88007                         ; Interrupt enable register A
MFP_IERB                equ     $E88009                         ; Interrupt enable register B
MFP_UDR                 equ     $E8802F                         ; USART data register
SPR_DATA                equ     $EB0000                         ; Sprite Data  (128 * 8)
SPR_BG_CTRL             equ     $EB0808                         ; Enable/Disable Cynthia elements
SPR_PAL0_DTA            equ     $E82200                         ; Sprites/BGs/TEXT planes palette 1
PCG_DATA                equ     $EB8000                         ; PCG/Sprites data

                        RSSET   0
SPR_X                   RS.W    1
SPR_Y                   RS.W    1
SPR_NFO                 RS.W    1
SPR_PRI                 RS.W    1
SPR_SIZEOF              RS.W    0

DOS                     macro
                        dc.w	\1
                        endm

SINTBL_SIZE             equ     (512-4)
SINTBL_AMP              equ     255
PI                      equ     205887
PI2                     equ     646814

; =======================================================
; a6 = start from depacker
                        xdef    _start
_start:
                IFD EXIT
                        lea     _start(pc),a6
                ENDC

                        clr.l   -(a7)
                        DOS     _SUPER

                IFD EXIT
                        move.b  MFP_IERB,old_MFP_IERB-_start(a6)
                ENDC

                        moveq   #11,d1                          ; 256x256 256 colors 15 Khz
                        moveq   #_CRTMOD,d0
                        trap    #15

                        ; disable all interrupts
                        moveq   #0,d0
                        move.b  d0,MFP_IERA
                        move.b  d0,MFP_IERB
                        move.l  d0,CRT_TEXT_X                   ; CRT_TEXT_X/CRT_TEXT_Y

                        lea     SPR_DATA,a0
                        moveq   #128-1,d7
.clear_sprs:
                        move.l  d0,(a0)+
                        move.l  d0,(a0)+
                        dbf     d7,.clear_sprs

                        lea     $80000,a0
                        move.l  #(2*PI*SINTBL_AMP/SINTBL_SIZE),d1
                        moveq  	#(4*PI2/SINTBL_SIZE/SINTBL_SIZE),d2
                        move.w  #381-1,d7
make_sintbl:
                        swap    d0
                        move.w  d0,(a0)
                        add.w	#SINTBL_AMP,(a0)+
			            swap    d0
                        move.l  d0,d3
                        swap    d3
                        muls.w  d2,d3
                        sub.l   d3,d1
                        add.l   d1,d0
                        dbf     d7,make_sintbl
                        clr.l   (a0)+
                        clr.w   (a0)
                        lea	    2(a0),a1
			            moveq	#128-1,d7
make_sintbl_end:
                        move.w  -(a0),(a1)+
                        dbf     d7,make_sintbl_end

                        ; Wait for the screen to cope
                        ; moveq   #120-1,d7
; .wait_monitor:
                        ; bsr.w   wait_vblank
                        ; dbf     d7,.wait_monitor

                        ; set sprites fixed data
                        lea     SPR_DATA,a0
                        move.w  #180,d0
                        move.w  d0,(a0)+                ; x
                        move.w  d0,(a0)+                ; y
                        move.w  d0,(a0)+                ; palette + flip
                        move.w  #%1,(a0)+               ; priority + on
                        add.w   #16,d0
                        move.w  d0,(a0)+
                        move.w  d0,(a0)+
                        move.w  d0,(a0)+
                        move.w  #%1,(a0)+
                        add.w   #16,d0
                        move.w  d0,(a0)+
                        move.w  d0,(a0)+
                        move.w  d0,(a0)+
                        move.w  #%1,(a0)+
                        add.w   #16,d0
                        move.w  d0,(a0)+
                        move.w  d0,(a0)+
                        move.w  d0,(a0)+
                        move.w  #%1,(a0)+

                        lea     PCG_DATA,a0             ; left pixels
                        lea     $40(a0),a1              ; right pixels
                        moveq   #33-1,d6
                        moveq   #-1,d1
                        moveq   #-1,d2
                        moveq   #-1,d3
                        moveq   #-1,d4
.copy_spr_all:
                        ; sprite 1 (borders)
                        move.l  d1,(a0)+                ; 8 pixels
                        move.l  d2,(a1)+                ; 8 pixels
                        lea     128-4(a0),a0
                        lea     128-4(a1),a1
                        ; sprite 2 (middles)
                        move.l  d3,(a0)+                ; 8 pixels
                        move.l  d4,(a1)+                ; 8 pixels
                        lea     128-4(a0),a0
                        lea     128-4(a1),a1
                        lsr.l   #4,d1
                        bcs.b   .remove_dot
                        lsr.l   #4,d2
                        bcs.b   .remove_dot
                        lsr.l   #4,d3
                        bcs.b   .remove_dot
                        lsr.l   #4,d4
.remove_dot:
                        dbf     d6,.copy_spr_all

                        lea     logo(pc),a0
                        lea     TVRAM_PLANE0+(216*128)+3,a1
                        moveq   #6-1,d7
.copy_logo:
                        move.b  (a0)+,(a1)+
                        move.b  (a0)+,(a1)+
                        move.b  (a0)+,(a1)+
                        move.b  (a0)+,(a1)+
                        move.b  (a0)+,(a1)+
                        move.b  (a0)+,(a1)+
                        move.b  (a0)+,(a1)+
                        move.b  (a0)+,(a1)+
                        move.b  (a0)+,(a1)+
                        move.b  (a0)+,(a1)+
                        lea     128-10(a1),a1
                        dbf     d7,.copy_logo

                        ; RGB
                        move.l  #((($6<<5|$0<<10|$e)<<1)<<16)+(($1f<<5|$1f<<10|$1f)<<1),SPR_PAL0_DTA

                        ; permanent regs
                        lea     $80000,a2
                        lea     SPR_DATA+SPR_Y,a3
                        lea     pos_wobbler(pc),a4
                        lea     pos_raster(pc),a5
                        lea     SPR_PAL0_DTA+(15*2),a6

                        ; install vblank int
                        lea     v_sync(pc),a1
                        moveq   #1,d1
                        moveq   #_VDISPST,d0
                        trap    #15

                        ; install raster int
                        lea     rast_int(pc),a1
                        moveq   #1,d1
                        moveq   #_CRTCRAS,d0
                        trap    #15

                        ; enable the sprites and the TVRAM
                        move.w  #%1000000000,SPR_BG_CTRL
                        move.w  #%0000010001100000,GFX_EN

; =======================================================
loop:         
                IFD EXIT
                        ; wait for the escape key
                        cmp.b   #1,MFP_UDR
                        bne.b   loop

                        bsr.b   wait_vblank

                        sub.l   a1,a1
                        moveq   #0,d1
                        moveq   #_CRTCRAS,d0
                        trap    #15
                        
                        sub.l   a1,a1
                        moveq   #1,d1
                        moveq   #_VDISPST,d0
                        trap    #15

                        sf.b    MFP_UDR
                        move.b  #%00011110,MFP_IERA
                        move.b  old_MFP_IERB(pc),MFP_IERB

                        moveq   #16,d1
                        moveq   #_CRTMOD,d0
                        trap    #15

                        DOS     _EXIT
                ELSE
                        bra.b    loop
                ENDC

                IFD EXIT

wait_vblank:
                        lea     MFP_GPIP,a0
wait_vblank_1:
                        btst.b  #4,(a0)
                        beq.b   wait_vblank_1
wait_vblank_2:
                        btst.b  #4,(a0)
                        bne.b   wait_vblank_2
                        rts
                ENDC

; =======================================================
v_sync:
                        ; move it
                        add.l   #$60001,(a4)
                        and.l   #$1ff01ff,(a4)
                        rte

; =======================================================
rast_int:
; can help to stabilize the line
;.wait:
;                       btst.b  #7,MFP_GPIP
;                       bne.b   .wait
                        move.w  (a5),d5
                        move.w  d5,d1
                        move.w  d5,d2
                        ; minimum displayed line
                        add.w   #16,d5
                        move.w  d5,CRT_RASTER
                        add.w   d2,d2
                        add.w   d2,d2
                        add.w   2(a4),d2
                        and.w   #$1ff,d2
                        add.w   d2,d2
                        add.w   (a4),d1
                        and.w   #$1ff,d1
                        add.w   d1,d1
                        move.w  (a2,d1.w),d6
                        add.w   (a2,d2.w),d6
                        lsr.w   #5,d6
                        ; we use 4 sprites with 2 pictures
                        add.w   d6,d6
                        move.w  d6,d2
                        sub.w   #63,d2
                        neg.w   d2
                        move.w  d2,d3
                        lsr.w   #1,d3
                        lsl.w   #6,d3
                        add.w   d3,d2
                        ; set the wobbler colors
                        move.w  d2,(a6)
                        lsl.w   #5,d3
                        add.w   d3,d2
                        move.w  d2,(16*2)(a6)
                        ; y + 1
                        addq.w  #1,d5
                        movem.w d5/d6,(a3)
                        ; next sprite
                        addq.w  #1,d6
                        movem.w d5/d6,SPR_SIZEOF*1(a3)
                        ; x flip + second palette
                        or.w    #(1<<14)|(1<<8),d6
                        movem.w d5/d6,SPR_SIZEOF*2(a3)
                        ; previous sprite + x flip
                        subq.w  #1,d6
                        movem.w d5/d6,SPR_SIZEOF*3(a3)
                        addq.w  #1,(a5)
                        cmp.w   #256-14,(a5)
                        blt.b   .reset_rast
                        clr.w   (a5)
.reset_rast:
                        rte

; =======================================================
logo:
                        dc.w    %1111111111100111,%1111110001111111,%1111001111111110,%0011110000000011,%1111111000000000
                        dc.w    %1111000111100111,%1000000001111000,%1111001111000000,%0011110000000011,%1100000000000000
                        dc.w    %1111111111100111,%1111000001111111,%1111001111111000,%0011110000000011,%1111111111000000
                        dc.w    %1111100011110111,%1100000001111100,%0111101111100000,%0011111000000000,%0000011111000000
                        dc.w    %1111100011110111,%1100000001111100,%0111101111100000,%0011111000000000,%0000011111000000
                        dc.w    %1111100011110111,%1111111101111111,%1111101111111111,%1011111111111011,%1111111111000000

pos_raster:
                        dc.w    0
pos_wobbler:
                        dc.w    0,0

                IFD EXIT
old_MFP_IERB:           dc.b    0
                ENDC
                        even

; =======================================================
                        end
