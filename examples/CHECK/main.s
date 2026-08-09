; =======================================================
; Check
; A 512 bytes intro written by hitchhikr/Rebels
; =======================================================

                        opt     o+
                        opt     a+

; =======================================================
_CRTMOD                 equ     $10                             ; Set Screen Attributes
_VDISPST                equ     $6c                             ; De/Install a vblank interrupt handler
_CRTCRAS                equ     $6d                             ; De/Install a raster interrupt handler

_EXIT		            equ	    $ff00                           ; Terminate program
_SUPER		            equ	    $ff20                           ; Switch to supervisor/user mode

TVRAM_PLANE0            equ     $E00000                         ; TVRAM planes
TVRAM_PLANE2            equ     $E40000

CRT_RASTER              equ     $E80012                         ; Raster number
SPR_PAL0_DTA            equ     $E82200                         ; Sprites/BGs/TEXT planes palette 1
GFX_EN                  equ     $E82600                         ; Enable/Disable various gfx stuff

MFP_GPIP                equ     $E88001                         ; General-purpose I/O register
MFP_IERA                equ     $E88007                         ; Interrupt enable register A
MFP_IERB                equ     $E88009                         ; Interrupt enable register B
MFP_UDR                 equ     $E8802F                         ; USART data register

FM_CTRL                 equ     $E90001                         ; Register number setting port

DOS                     macro
                        dc.w	\1
                        endm

; =======================================================
CHESSBOARD_WIDTH        equ     256
CHESSBOARD_HEIGHT       equ     128
CHESSBOARD_BYTES        equ     (CHESSBOARD_WIDTH/8)
; must be odd
COPPERLINES             equ     51

; =======================================================
                        xdef    _start
_start:
                IFD EXIT
                        lea     start(pc),a6
                ENDC

                        clr.l   -(a7)
                        DOS     _SUPER

                IFD EXIT
                        move.b  MFP_IERB,old_MFP_IERB-start(a6)
                ENDC

                        moveq   #3,d1                          ; 256x240 16 colors 15 Khz ~60hz
                        moveq   #_CRTMOD,d0
                        trap    #15

                        lea     CRT_RASTER,a5
                        lea     (SPR_PAL0_DTA+2)-CRT_RASTER(a5),a4

                        ; disable all interrupts
                        moveq   #0,d0
                        move.b  d0,MFP_IERA-CRT_RASTER(a5)
                        move.b  d0,MFP_IERB-CRT_RASTER(a5)

                        ; paint it black
                        move.l  d0,(a4)
                        move.w  d0,GFX_EN-CRT_RASTER(a5)

                        ; construct the chessboards bitmap
                        lea     TVRAM_PLANE0,a0
                        lea     ((256-1)*128)(a0),a1
                        moveq   #CHESSBOARD_HEIGHT-1,d7
                        moveq   #69,d5                          ; deepness
                        swap    d5
.draw_line:
                        moveq   #0,d2
                        move.l  #(256/2)<<16,(a6)               ; x center<<16
.set_left_part:
                        bchg    #17,d2                          ; $00000/$20000
                        sub.l   d5,(a6)
                        bpl.b   .set_left_part
                        move.w  (a6),d4
                        add.l   d5,(a6)
.plot_line:
                        ; plot a pixel
                        move.w  d4,d2
                        move.w  d4,d3
                        lsr.w   #3,d2
                        not.b   d3
                        bset    d3,(a0,d2.l)
                        bset    d3,(a1,d2.l)
                        addq.w  #1,d4
                        cmp.w   #CHESSBOARD_WIDTH,d4            ; right border
                        bge.b   .next_line
                        cmp.w   (a6),d4
                        ble.b   .plot_line
                        bchg    #17,d2                          ; $00000/$20000
                        add.l   d5,(a6)
                        bra.b   .plot_line
.next_line:
                        lea     128(a0),a0
                        lea     -128(a1),a1
                        sub.l   #$7000,d5
                        dbf     d7,.draw_line
                        
                        ; construct the colors perspective ($180e bytes)
                        lea     chessboard_gradient(pc),a1
                        moveq   #COPPERLINES-1,d5
                        moveq   #1,d4
.set_blocks:
                        lea     (a1),a6
                        moveq   #COPPERLINES-1,d7
.set_block_y:
                        move.l  #40*4096,d1
                        divs.w  d4,d1
                        move.w  d1,(a6)
                        addq.w  #8,d4
                        lea     (COPPERLINES*2)(a6),a6
                        dbf     d7,.set_block_y
                        addq.l  #2,a1
                        dbf     d5,.set_blocks

                        ; a6 points to lines memory block
                        ; d5 = -1
                        lea     logo(pc),a0
                        lea     TVRAM_PLANE2+(118*128)+11,a1
                        lea     4(a4),a2
                        moveq   #6-1,d6
.copy_logo_y:
                        moveq   #10-1,d7
.copy_logo_x:
                        ; set the logo colors here
                        move.w  d5,(a2)+
                        move.b  (a0)+,(a1)+
                        dbf     d7,.copy_logo_x
                        lea     128-10(a1),a1
                        dbf     d6,.copy_logo_y

                        moveq   #0,d5
                        sub.l   a2,a2
                        
                        ; song frames delay
                        moveq   #0,d6
                        lea     song(pc),a3

                        ; install vblank int
                        lea     v_sync(pc),a1
                        moveq   #1,d1
                        moveq   #_VDISPST,d0
                        trap    #15

                        ; install raster int
                        lea     rast_int(pc),a1
                        moveq   #_CRTCRAS,d0
                        trap    #15

                        st      (a5)

                        move.w  #%0000010001100000,GFX_EN-CRT_RASTER(a5)

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

old_MFP_IERB:           dc.b    0
                        even

                ENDC

; =======================================================
instrument:
                        ; feedback/connection
                        dc.b    $20+0,%11000000
                        ; keyscale/attack
                        dc.b    $80+24+0,%00000001
                        ; volume
                        dc.b    $60+24+0,%00000000
                        ; decay/release
                        ;dc.b    $E0+24+0,%00000110
finstrument:

NOTE_CS                 equ     0
NOTE_D                  equ     1
NOTE_DS                 equ     2
NOTE_E                  equ     4
NOTE_F                  equ     5
NOTE_FS                 equ     6
NOTE_G                  equ     8
NOTE_GS                 equ     9
NOTE_A                  equ     10
NOTE_AS                 equ     12
NOTE_B                  equ     13
NOTE_C                  equ     14

song:
                        ; channel / note
                        dc.b    0,%00010000+NOTE_G,1,%01000000+NOTE_B
                        ; delay
                        dc.b    -10
                        dc.b    0,%00110000+NOTE_G
                        dc.b    -50
                        ; note
                        dc.b    0,%00010000+NOTE_C,1,%01000000+NOTE_E
                        ; delay
                        dc.b    -10
                        dc.b    1,%00110000+NOTE_C
                        dc.b    -10
                        ; note 
                        dc.b    0,%00010000+NOTE_A,1,%01000000+NOTE_CS
                        ; delay
                        dc.b    -10
                        dc.b    0,%00110000+NOTE_A
                        dc.b    -50
                        ; note 
                        dc.b    0,%00010000+NOTE_D,1,%01000000+NOTE_FS
                        ; delay
                        dc.b    -10
                        dc.b    1,%00110000+NOTE_D
                        dc.b    -25
                        dc.b    -1
                        even

key_on:
;                        beq.b   key_off
                        ; randomize it a bit
                        move.b  d5,d2
                        lsl.b   #4,d2
                        add.b   d2,d3
                        ; octave/note
                        moveq   #$28+0,d2
                        add.b   d0,d2
                        bsr.b   set_fm_reg
                        moveq   #((finstrument-instrument)/2)-1,d7
                        lea     instrument(pc),a1
.set_regs:
                        move.b  (a1)+,d2
                        add.b   d0,d2
                        move.b  (a1)+,d3
                        bsr.b   set_fm_reg
                        dbf     d7,.set_regs
                        ; set noise on channel 7
                        ;moveq   #$f,d2
                        ;moveq   #%10000000,d3
                        ;bsr.b   set_fm_reg
                        ; key on
                        moveq   #%01111000,d3
;key_off:
                        add.b   d0,d3
                        moveq   #8,d2
set_fm_reg:
                        lea     FM_CTRL,a0
;wait_fm_chip_reg:
 ;                       tst.b   (a0)
  ;                      bmi.b   wait_fm_chip_reg
                        move.b  d2,(a0)
;wait_fm_chip_data:
 ;                       tst.b   (a0)
  ;                      bmi.b   wait_fm_chip_data
                        move.b  d3,2(a0)
                        rts

; =======================================================
logo:
                        dc.w    %0000111111111110,%0111111111000111,%1111111100111111,%1110001111000000,%0011111111100000
                        dc.w    %0000111100011110,%0111100000000111,%1000111100111100,%0000001111000000,%0011110000000000
                        dc.w    %0000111111111110,%0111111100000111,%1111111100111111,%1000001111000000,%0011111111111100
                        dc.w    %0000111110001111,%0111110000000111,%1100011110111110,%0000001111100000,%0000000001111100
                        dc.w    %0000111110001111,%0111110000000111,%1100011110111110,%0000001111100000,%0000000001111100
                        dc.w    %0000111110001111,%0111111111110111,%1111111110111111,%1111101111111111,%1011111111111100

; =======================================================
rast_int:
                        move.w  (a6)+,(a5)
                        ; d3 = register
                        move.w  d5,d3
                        ; d2 = shade
                        move.w  (a6)+,d2
                        ror.w   #1,d2
                        bcc.b   .set_plane_2
                        eor.w   d4,d3
.set_plane_2:
                        rol.w   #2,d2
                        move.w  d2,(a4,d3.w)
                        eor.w   d4,d3
                        add.w   d2,d2
                        move.w  d2,(a4,d3.w)
                        rte

; =======================================================
v_sync:
                        lea     chessboard_gradient+$180e(pc),a6
                        clr.w   (a5)
                        moveq   #%10,d4
                        ; move the boards
                        sub.w   #COPPERLINES*6,a2
                        cmp.w   #COPPERLINES*4,a2
                        bgt.b   .move_chessboards
                        eor.w   d4,d5
                        move.w  #((COPPERLINES-1)*(COPPERLINES*2)),a2
.move_chessboards:
                        ; raster lines & colors buffer
                        lea     chessboard_gradient(pc,a2.w),a1
                        lea     (a6),a0
                        moveq   #$e,d1
                        moveq   #(COPPERLINES-37)-1,d7
.set_upper_chessboard:
                        move.w  #164,d0
                        sub.w   (a1)+,d0
                        ble.b   .min_gradient
                        move.w  d0,(a0)+
                        move.w  d1,(a0)+            ; set the color to blue shade
.min_gradient:
                        subq.w  #1,d1
                        dbf     d7,.set_upper_chessboard
                        moveq   #(COPPERLINES-37)-1,d7
.set_lower_chessboard:
                        move.w  -(a1),d0
                        add.w   #112,d0
                        move.w  d0,(a0)+
                        move.w  d1,(a0)+            ; set the color to blue shade
                        addq.w  #1,d1
                        dbf     d7,.set_lower_chessboard

                        ; replay routine
                        addq.b  #1,d6
                        ble.b   .next_data
                        move.b  (a3)+,d0
                        bmi.b   .negative
                        move.b  (a3)+,d3
                        bsr.w   key_on
                        rte
.negative:
                        addq.b  #1,d0
                        bne.b   .delay
                        ; reset song
                        lea     song(pc),a3
                        ; d0 = 0
.delay:
                        move.b  d0,d6
.next_data:
                        rte

; =======================================================
chessboard_gradient:
