; =================================================================================
; X68000 Hardware registers constants and memory locations v1.3.
; Painstakingly translated & typed by Franck 'hitchhikr' Charlet.
; =================================================================================

;                           Address       Vector
INIT_SSP            equ     $0          ; $00 Initial SSP
INIT_PC             equ     $4          ; $01 Initial PC
BUS_ERROR           equ     $8          ; $02 Bus error
ADDR_ERROR          equ     $C          ; $03 Address error
ILLEGAL_INSTR       equ     $10         ; $04 Illegal instruction
DIV_ZERO            equ     $14         ; $05 Division by 0
CHK_INSTR           equ     $18         ; $06 CHK instruction
TRAPV_INSTR         equ     $1C         ; $07 TRAPV instruction, FTRAPcc instruction
PRIV_VIOL           equ     $20         ; $08 Privilege violation
TRACE_EXCEPT        equ     $24         ; $09 Trace exception handling
LINEA_INSTR         equ     $28         ; $0a Unimplemented instruction line 1010 emulator (SX call)
LINEF_INSTR         equ     $2C         ; $0b Unimplemented instruction line 1111 emulator (DOS calls, floating point arithmetic)
UNUSED_EXCEPT_1     equ     $30         ; $0c Unused
FPU_VIOL            equ     $34         ; $0d FPU protocol violation exception handling
FPU_FORMAT_ERR      equ     $38         ; $0e FPU format error exception handling
UNINIT_INT          equ     $3C         ; $0f Uninitialized Interrupt
UNUSED_EXCEPT_2     equ     $40         ; $10 Unused
UNUSED_EXCEPT_3     equ     $44         ; $11 Unused
UNUSED_EXCEPT_4     equ     $48         ; $12 Unused
UNUSED_EXCEPT_5     equ     $4C         ; $13 Unused
UNUSED_EXCEPT_6     equ     $50         ; $14 Unused
UNUSED_EXCEPT_7     equ     $54         ; $15 Unused
UNUSED_EXCEPT_8     equ     $58         ; $16 Unused
UNUSED_EXCEPT_9     equ     $5C         ; $17 Unused
SPUR_INT            equ     $60         ; $18 Spurious interrupts
LEV_1_INT           equ     $64         ; $19 Level 1 interrupt (auto vector)
NMI_INT             equ     $7C         ; $1f Non-maskable interrupt
SYS_RESERVED        equ     $A0         ; $28 Trap #8
OS_DEBUGGER         equ     $A4         ; $29 Trap #9
RESET_POWER_OFF     equ     $A8         ; $2a Trap #10
BREAK_KEY           equ     $AC         ; $2b Trap #11
COPY_KEY            equ     $B0         ; $2c Trap #12
CTRL_C_KEY          equ     $B4         ; $2d Trap #13
ERR_PROC            equ     $B8         ; $2e Trap #14
IOCS                equ     $BC         ; $2f Trap #15 (IOCS calls)
RTC_ALARM_IRQ       equ     $100        ; $40 MFP RTC alarm/1hz
EXT_POWER_IRQ       equ     $104        ; $41 MFP External power OFF
FRONT_POWER_IRQ     equ     $108        ; $42 MFP Front switch OFF
AUDIO_IRQ           equ     $10C        ; $43 MFP FM audio source
TIMER_D_IRQ         equ     $110        ; $44 MFP Timer-D (Used with BG processing)
TIMER_C_IRQ         equ     $114        ; $45 MFP Timer-C (Mouse/cursor/FDD control, etc.)
VBL_IRQ             equ     $118        ; $46 MFP V-DISP
RTC_CLOCK_IRQ       equ     $11C        ; $47 MFP RTC Clock
TIMER_B_IRQ         equ     $120        ; $48 MFP Timer-B
KEY_OUT_ERR_IRQ     equ     $124        ; $49 MFP Key serial output error
KEY_OUT_EMPTY_IRQ   equ     $128        ; $4a MFP Key serial output empty
KEY_IN_ERR_IRQ      equ     $12C        ; $4b MFP Key serial input error
KEY_IN_IRQ          equ     $130        ; $4c MFP Key serial input
TIMER_A_IRQ         equ     $134        ; $4d MFP Timer-A
CRTC_IRQ            equ     $138        ; $4e MFP CRTC*IRQ
HBL_IRQ             equ     $13C        ; $4f MFP H-SYNC
FDC_STATUS_IRQ      equ     $180        ; $60 I/O FDC status interruption
FDC_INS_REM_IRQ     equ     $184        ; $61 I/O FDC insertion/discharge interruption
HDC_STATUS_IRQ      equ     $188        ; $62 I/O HDC status interruption
PRINTER_RDY_IRQ     equ     $18C        ; $63 I/O Printer ready interruption
DMAC_0_END_IRQ      equ     $190        ; $64 DMAC #0 End (FDD)
DMAC_0_ERR_IRQ      equ     $194        ; $65 DMAC #0 Error ("")
DMAC_1_END_IRQ      equ     $198        ; $66 DMAC #1 End (SASI)
DMAC_1_ERR_IRQ      equ     $19C        ; $67 DMAC #1 Error ("")
DMAC_2_END_IRQ      equ     $1A0        ; $68 DMAC #2 End (IOCS _DMAMOVE,_DMAMOV_A,_DMAMOV_L)
DMAC_2_ERR_IRQ      equ     $1A4        ; $69 DMAC #2 Error ("")
DMAC_3_END_IRQ      equ     $1A8        ; $6a DMAC #3 End (ADPCM)
DMAC_3_ERR_IRQ      equ     $1AC        ; $6b DMAC #3 Error ("")
SCSI_INT_IRQ        equ     $200        ; $6c SPC SCSI interruption (Internal SCSI)
SCSI_EXT_IRQ        equ     $3d8        ; $f6 SPC SCSI interruption (SCSI board)
;
; =================================================================================
; Graphic VRAM
; =================================================================================
;
; The structure of G-VRAM differs depending on the actual screen size and number of colors, and a total of 512KB of VRAM is divided.
; The dot representation is vertical, and one dot is required in all modes.
; It is represented by one word, of which only 4 / 8 bits are valid in 16 / 256 color display mode.
;
; X = X coordinate in normal configuration mode
; Y = Y coordinate in normal configuration mode
; P = plane number
; ADR = Address when accessing (X, Y) point in 65536 color display mode
; BIT = Address when accessing (X, Y) bit position in 65536 color display mode
;
; Actual screen 512 * 512 dots, 65536 colors:
;
; Valid bits: Lower 16 bits
; 1 line 1024 bytes (512 words)
;
; Page 0 $c00000 - $c7ffff
;
; Actual screen 512 * 512 dots, 256 colors display:
;
; Valid bits: Lower 8 bits
; 1 line 1024 bytes (512 words)
;
; Page 0 $c00000 - $c7ffff
; Page 1 $c80000 - $cfffff
;
; ADR = $c00000 + Y * 1024 + X * 2
; BIT = 8 bits from P * 8 to P * 8 + 7
;
; Actual screen 512 * 512 dots, 16 colors display:
;
; Valid bits: Lower 4 bits
; 1 line 1024 bytes (512 words)
;
; Page 0 $c00000 - $c7ffff
; Page 1 $c80000 - $cfffff
; Page 2 $d00000 - $d7ffff
; Page 3 $d80000 - $dfffff
;
; ADR = $c00000 + Y * 1024 + X * 2
; BIT = 4 bits from P * 4 to P * 4 + 3
;
; Actual screen 1024 * 1024 dots, 16 colors display:
;
; Valid bits Lower 4 bits
; 1 line 2048 bytes (1024 words)
;
; Page 0 $c00000 - $dfffff
;
; ADR = $c00000 + (Y & 511) x 1024 + (X & 511) * 2
; BIT = (Y >= 512) * 8 + (X >= 512) * 4
;       (Y >= 512) * 8 + (X >= 512) * 4 + 3 (4 bits)
;
; Real screen 512 * 512 pixels 65536 color display (1 page)
; Real screen 512 * 512 pixels 256 color display (2 pages)
; Real screen 512 * 512 pixels 16 colors (4 pages)
; Real screen 1024 * 1024 pixels 16 colors (1 page)
;
GVRAM_PAGE0         equ     $C00000
GVRAM_PAGE1         equ     $C80000
GVRAM_PAGE2         equ     $D00000
GVRAM_PAGE3         equ     $D80000
;
; =================================================================================
; Text VRAM Planes
; =================================================================================
;
; The T-VRAM has a fixed actual screen size of 1024 * 1024 and 16-color display,
; and a total of 512KB of VRAM is allocated for 4 planes.
;
; The dot representation is horizontal,
; and 1 word is the palette code for each plane of 16 horizontal dots.
;
TVRAM_PLANE0        equ     $E00000
TVRAM_PLANE1        equ     $E20000
TVRAM_PLANE2        equ     $E40000
TVRAM_PLANE3        equ     $E60000
;
; =================================================================================
; CRTC (VICON)
; =================================================================================
;
;       H-Sync period * number of horizontal display dots
; R00 = ------------------------------------------------- -1
;                 Data display period * 8
;
; * The least significant bit must be set to 1.
;
CRT_HORZ_TOT        equ     $E80000         ; Total Horizontal
;
;       H-Sync pulse width x number of horizontal display dots
; R01 = ------------------------------------------------------ -1
;                    Data display period * 8
;
CRT_HSYNC_END       equ     $E80002         ; Horizontal Sync End Position
;
;       (H-Sync pulse width + horizontal back porch) x number of horizontal display dots
; R02 = -------------------------------------------------------------------------------- -5
;                               Data display period * 8
;
CRT_HDISP_STRT      equ     $E80004         ; Horizontal Display Start Position
;
;       (H-Sync pulse width - horizontal front porch) * number of horizontal display dots
; R03 = --------------------------------------------------------------------------------- -5
;                               Data display period * 8
;
CRT_HDISP_END       equ     $E80006         ; Horizontal Display End Position
;
;       V-Sync period
; R04 = ------------- -1
;       H-Sync period
;
CRT_VERT_TOT        equ     $E80008         ; Total Vertical
;
;       V-Sync pulse width
; R05 = ------------------ -1
;         H-Sync period
;
CRT_VSYNC_END       equ     $E8000A         ; Vertical Sync End Position
;
;       V-Sync pulse width + vertical back porch
; R06 = ---------------------------------------- -1
;                   H-Sync period
;
CRT_VDISP_STRT      equ     $E8000C         ; Vertical Display Start Position
;
;       V-Sync synchronization period + vertical front porch    
; R07 = ---------------------------------------------------- -1
;                      H-Sync period
;
CRT_VDISP_END       equ     $E8000E         ; Vertical Display End Position
CRT_EXT_HSYNC       equ     $E80010         ; External H-Sync adjustment
;
;       V-Sync pulse width + vertical back porch
; R09 = ---------------------------------------- + raster number
;                   H-Sync period
;     = R06 + 1 + raster number
;
CRT_RASTER          equ     $E80012         ; Raster number
;
; Text Screen Positions
;
CRT_TEXT_X          equ     $E80014         ; X Position
CRT_TEXT_Y          equ     $E80016         ; Y Position
;
; Graphic Screen Positions
;
; * Bits 8 to 0 are valid in 512 * 512 dot real screen mode.
;
CRT_GFX_X0          equ     $E80018         ; X0
;
; * Bits 8 to 0 are valid in 512 * 512 dot real screen mode.
;
CRT_GFX_Y0          equ     $E8001A         ; Y0
;
; * Not used in actual screen 1024 * 1024 dot mode.
;
CRT_GFX_X1          equ     $E8001C         ; X1
;
; * Not used in actual screen 1024 * 1024 dot mode.
;
CRT_GFX_Y1          equ     $E8001E         ; Y1
;
; * Not used in actual screen 1024 * 1024 dot mode.
;
CRT_GFX_X2          equ     $E80020         ; X2
;
; * Not used in actual screen 1024 * 1024 dot mode.
;
CRT_GFX_Y2          equ     $E80022         ; Y2
;
; * Not used in actual screen 1024 * 1024 dot mode.
;
CRT_GFX_X3          equ     $E80024         ; X3
;
; * Not used in actual screen 1024 * 1024 dot mode.
;
CRT_GFX_Y3          equ     $E80026         ; Y3
;
; bit 12: T-MEM T-VRAM usage mode
;
;   0 = For display
;   1 = For buffer
;
; bit 11: G-MEM G-VRAM usage mode
;
;   0 = For display
;   1 = For buffer (bits 10 to 8 are invalid)
;   (G-VRAM has the same structure as when displaying 65536 colors)
;
; bit 10: SIZE Actual screen size
;
;   0 = 512 * 512 dots
;   1 = 1024 * 1024 dots
;
; bit 9-8: COL Color number mode
;
;   00 = 16 colors
;   01 = 256 colors
;   10 = undefined
;   11 = 65536 colors
;
; bit 4: HF horizontal frequency
;
;   0 = 15.98 kHz
;   1 = 31.50 kHz
;
; bit 3-2: VD Number of vertical dots
;
;   00 = 256 dots
;   01 = 512 dots
;   10 = 1024 dot interlace (only when HF = 1)
;   11 = 1024 dot interlace (only when HF = 1)
;
; bit 1-0: HD horizontal dot count
;
;   00 = 256 dots
;   01 = 512 dots
;   10 = 768 dots
;   11 = Clock 50 MHz mode (Compact XVI or later only)
;
CRT_CTRL1           equ     $E80028         ; Memory Mode/View Mode Control (R/W)
CRT_R20             equ     CRT_CTRL1
;
; Simultaneous access/raster copy/Fast clear plane
;
; bit 9: MEN Text screen access mask
;
;   0 = invalid
;   1 = valid
;
; bit 8: SA Text screen simultaneous access
;
;   0 = invalid
;   1 = valid
;
; bit 7-4: AP3-AP0 Text screen simultaneous access target plane selection
;
;   0 = unselected
;   1 = selected
;
; bit 3-0: CP3-CP0 - Text screen raster copy target plane selection
;                  - Graphic screen high-speed clear target page
;
;   0 = unselected
;   1 = selected
;
CRT_CTRL2           equ     $E8002A         ; (R/W)
CRT_R21             equ     CRT_CTRL2
;
; bit 15-8: Source raster
; bit 7-0: Destination raster
;
CRT_R22             equ     $E8002C         ; Raster number for Raster Copy operation
;
; mask pattern:
;
;   0 = Data will be changed
;   1 = Data will not be changed
;
CRT_R23             equ     $E8002E         ; Text Screen Access mask pattern
;
; bit 3: RC Text screen raster copy start
;
;   0 = Stop
;   1 = Start
;
; bit 1: FC Graphic screen high-speed clear start
;
;   0 = Stop
;   1 = Start
;
;   * It automatically becomes 0 when the high-speed clear operation ends.
;
; bit 0: VI Image capture start
;
;   0 = Stop
;   1 = Start
;
CRT_R24             equ     $E80480         ; Raster Copy/Fast Clear/Image Capture control 
;
; =================================================================================
; Video Controller (CATHY / VIPS)
; =================================================================================
;
; Backgrounds palettes data (256 words)
; The graphic palette can be configured in 16, 256 or 65536 color display mode.
;
; 16 color display:
;
; The first 16 words are used.
; The value read from the palette is output as a color code.
;
; 256 color display:
;
; The first 256 words are used.
; The value read from the palette is output as is as a color code.
;
; 65536 color display:
;
; Text palettes and graphic palettes in 16 or 256 color display mode have one-word color codes lined up for the number of colors,
; but graphic palettes in 65536 color display mode divide the color code into upper/lower bytes.
; There are two sets (upper/lower) in which 256 of them are arranged in a row.
; When outputting a color code, the data in G-VRAM is divided into upper byte and lower byte,
; which are determined independently and then combined.
;
BG_PAL0_DTA         equ     $E82000
BG_PAL1_DTA         equ     $E82020
BG_PAL2_DTA         equ     $E82040
BG_PAL3_DTA         equ     $E82060
BG_PAL4_DTA         equ     $E82080
BG_PAL5_DTA         equ     $E820A0
BG_PAL6_DTA         equ     $E820C0
BG_PAL7_DTA         equ     $E820E0
BG_PAL8_DTA         equ     $E82100
BG_PAL9_DTA         equ     $E82120
BG_PAL10_DTA        equ     $E82140
BG_PAL11_DTA        equ     $E82160
BG_PAL12_DTA        equ     $E82180
BG_PAL13_DTA        equ     $E821A0
BG_PAL14_DTA        equ     $E821C0
BG_PAL15_DTA        equ     $E821E0
;
; Text palette, sprites + BG palette (256 words)
;
; Text palette ($E82200):
;
; The text palette and sprite + BG palette consist of 16 palette blocks of 16 words, and some of them are shared.
; For the text palette, only the first palette block 0 is used,
; and the value read from the palette is output as is as a color code.
;
; Sprite + BG palette ($E82200):
;
; It is similar to the text palette,
; but all palette blocks 0 to 15 can be used.
; The palette number used is that the PCG data is the lower 4 bits,
; and the sprite scroll register and his BG data area are the upper 4 bits.
;
SPR_PAL0_DTA        equ     $E82200
SPR_PAL1_DTA        equ     $E82220
SPR_PAL2_DTA        equ     $E82240
SPR_PAL3_DTA        equ     $E82260
SPR_PAL4_DTA        equ     $E82280
SPR_PAL5_DTA        equ     $E822A0
SPR_PAL6_DTA        equ     $E822C0
SPR_PAL7_DTA        equ     $E822E0
SPR_PAL8_DTA        equ     $E82300
SPR_PAL9_DTA        equ     $E82320
SPR_PAL10_DTA       equ     $E82340
SPR_PAL11_DTA       equ     $E82360
SPR_PAL12_DTA       equ     $E82380
SPR_PAL13_DTA       equ     $E823A0
SPR_PAL14_DTA       equ     $E823C0
SPR_PAL15_DTA       equ     $E823E0
;
; Set the same value bits as bits 10-8 of CRT_CTRL1
;
; bit 2: GVRAM size
;
;   0 = 512 * 512
;   1 = 1024 * 1024
;
; bit 0-1: Color mode
;
;   00 = 16 colors
;   01 = 256 colors
;   10 = undefined
;   11 = 65536 colors
;
GFX_COLMODE         equ     $E82400
;
; bit 13-12: sprites screen priority
; bit 11-10: text screen priority
; bit 9-8: graphic screen priority
; bit 7-6: GP3 fourth priority graphics screen page number
; bit 5-4: GP2 third priority graphics screen page number
; bit 3-2: GP1 second priority graphics screen page number
; bit 1-0: GP0 highest priority graphics screen page number
;
; * For SP/TX/GR, set 00/01/10 from the priority screen.
; * If the graphic screen is one page, GP3 to GP0 are treated as one set.
;
; Also, in the case of 2 pages, GP3 to GP2/GP1 to GP0 are treated as one set.
; * GP3-GP0 setting value
;
; 65536 color display: 11100100
;   256-color display: 11100100 (Page 1 < Page 0)
;                      01001110 (Page 0 < Page 1)
;   16-color display:  11100100 (Page 3 < Page 2 < Page 1 < Page 0)
;   ...
;                      00011011 (Page 0 < Page 1 < Page 2 < Page 3)
;
GFX_PRI             equ     $E82500
;
; bit 15: YS video cut (video image not displayed even when superimposed)
; bit 14: AH Transparent: Text palette 0
;
;   * Forcibly enters translucent mode regardless of EXON, H/P, etc.
;
; bit 13: VHT translucent: video image (used by color image unit)
; bit 12: EXON Special priority/translucent mode enabled
; bit 11: H/P
;
;   0 = Special priority selection
;   1 = Translucent mode selection
;
; bit 10: B/P
;
;   0 = Sharp reserved
;   1 = Specify area using the least significant bit of the graphic screen
;
; bit 9: G/G Translucent: Graphic screen
; bit 8: G/T translucent: text/sprite screen
;
;   * Valid only when the priority of the graphic screen is high)
;
; bit 7: BCON Border color display control
;
;   1 = on
;   0 = off
;
; bit 6: SON sprite screen control
;
;   1 = on
;   0 = off
;
; bit 5: TON Text screen control
;
;   1 = on
;   0 = off
;
; bit 4: GS4 graphic screen control
;
;   1 = on
;   0 = off
;   (When the graphic size is 1024 * 1024 dots)
;
; bit 3-0: GS3-GS0 Graphic screen control
;          (select the pages prorities to display set in GFX_PRI)
;
;   1 = on
;   0 = off
;   (When the graphic size is 512 * 512 dots)
;
; * If the graphic screen is one page, set the same value to each bit of GS3 to GS0.
;   Also, if the graphic screen is two pages, treat GS3 to GS2/GS1 to GS0 as one set,
;   and set each bit of the same set to the same value.
;
GFX_EN              equ     $E82600
;
; =================================================================================
; DMAC (HD63450)
; =================================================================================
;
; The HD63450 has four channels, and in the X680x0 channels #0, #1, and #3 are assigned to FD, HD, and ADPCM, respectively.
; Channel #2 is open to users and is used for REQ (DMA transfer request signal).
; ACK (response signal),
; PCL (general purpose input signal), etc. are wired to the expansion slot,
; so they can be used for memory-to-memory transfers and expansion boards.
;
; There are DMAC registers of $40 bytes starting from $e84000 for each channel.
; However, the GCR (general control register) is only on channel #3.
;
DMA_CHAN0           equ     $E84000
DMA_CHAN1           equ     $E84040
DMA_CHAN2           equ     $E84080
DMA_CHAN3           equ     $E840C0
                    RSSET   0
;
; bit 7: COC Channel Operation Complete
;
;   0 = channel operation not completed
;   1 = channel operation Completed
;
; bit 6: BTC block transfer complete
;
;   0 = block transfer operation not completed
;   1 = block transfer operation completed
;
; bit 5: NDT Normal device termination
;
;   0 = device is not stopped by DONE signal
;   1 = normal device stop due to DONE signal
;
; bit 4: ERR Error bit
;
;   0 = no error
;   1 = error occurred (error content is entered in the ERROR CODE bit)
;
; bit 3: ACT Channel active
;
;   0 = channel inactive
;   1 = channel active (in operation)
;
; bit 2: DIT DONE Input transition
;
;   0 = no DONE input
;   1 = DONE input was received when OCR BTD bit was 1
;
; bit 1: PCT PCL transition
;
;   0 = there is no falling of PCL (change from High to Low)
;   1 = a falling of PCL (change from High to Low) occurred
;
; bit 0: PCS PCL line status
;
;   0 = PCL = Low
;   1 = PCL = High
;
; * Once bits other than ACT and PCS become 1, they remain at 1 until you write 1 to that bit or reset it.
;   Especially when the COC, BTC, NDT, ERR, and ACT bits become 1.
;   When this is the case, the next transfer operation cannot be performed, so it must be cleared before use.
;
DMA_CSR             RS.B    1               ; .b (r/w) channel status ($00)
;
; bit 4-0: ERROR CODE
;
;   00000 = No error
;   00001 = Configuration error
;   00010 = Operation timing error
;   00011 = (unused)
;   001rr = Address error
;   010rr = Bus error
;   011rr = Count error
;   10000 = External forced stop
;   10001 = Software forced stop
;
DMA_CER             RS.B    1               ; .b (r) channel error register ($01)
                    RS.W    1
;
; bit 7-6: XRM external request mode
;
;   00 = Burst transfer mode
;   01 = (undefined)
;   10 = No hold cycle steal mode
;   11 = With hold cycle steal mode
;
; bit 5-4: DTYP Device type
;
;   00 = 68000 Bus type  Dual Address mode
;   01 = 6800  Bus type   
;   10 = Device with ACK Single Address mode
;   11 = Device with ACK and READY
;
;   * Use in dual address mode (usually 00).
;
; bit 3: DPS device port size
;
;   0 = 8 bit port
;   1 = 16 bit port
;
; bit 1-0: PCL Peripheral control line
;
;   00 = Status input
;   01 = Status input with interrupt
;   10 = 1/8 start pulse
;   11 = ABORT (forced termination) input
;
DMA_DCR             RS.B    1               ; .b (r/w) device control register ($04)
;
; bit 7: DIR Direction
;
;   0 = Memory to Device (MAR to DAR)
;   1 = Device to Memory (DAR to MAR)
;
; bit 6: BTD Multiple block transfer with DONE
;
;   0 = Normal operation
;   1 = Forces transfer of next block when DONE is input
;
; bit 5-4: SIZE Operand size
;
;   00 = Byte (8bit)
;   01 = Word (16bit)
;   10 = Long word (32bit)
;   11 = No pack Port size 8bit Byte (8bit) transfer
;
; bit 3-2: CHAIN Chaining operation
;
;   00 = No chain operation
;   01 = (unused)
;   10 = Array chaining
;   11 = Link array chaining
;
; bit 1-0: REQG Request generation method
;
;   00 = Auto request limited speed
;   01 = Auto request maximum speed
;   10 = External request transfer (via REQ line)
;   11 = The first transfer is an auto request.
;        The second and subsequent transfers are external request transfers.
;
;   * Channels #0, #1, and #3 must be used for external request transfers.
;
DMA_OCR             RS.B    1               ; .b (r/w) operation control register ($05)
;
; bit 3-2: MAC memory address count
;
;   00 = Memory address register is not changed
;   01 = Increment memory address register every time a transfer is performed
;   10 = Decrement memory address register every time a transfer is performed
;   11 = (unused)
;
; bit 1-0: DAC device address count
;
;   00 = Device address register is not changed
;   01 = Increment device address register after each transfer
;   10 = Decrement device address register after each transfer
;   11 = (unused)
;
DMA_SCR             RS.B    1               ; .b (r/w) sequence control register ($06)
;
; bit 7: STR start operation
;
;   1 = Operation start
;
; bit 6: CNT continue operation
;
;   0 = do not continue operation
;   1 = continue operation
;
; bit 5: HLT Halt operation
;
;   1 = Channel operation paused
;
; bit 4: SAB software abort
;
;   1 = Stop channel operation
;
; bit 3: INT Interrupt enable
;
;   0 = enable interrupt generation
;   1 = interrupt generation prohibited
;
DMA_CCR             RS.B    1               ; .b (r/w) channel control register ($07)
                    RS.W    1
;
; Specify the number of transfer operands.
;
DMA_MTC             RS.W    1               ; .w (r/w) memory transfer counter (10/$0A)
;
; Specify the memory address to transfer.
;
DMA_MAR             RS.L    1               ; .l (r/w) memory address register (12/$0C)
                    RS.L    1
;
; Specify the device address to transfer.
;
DMA_DAR             RS.L    1               ; .l (r/w) device address register (20/$14)
                    RS.W    1
;
; Specifies the number of transfer blocks
; (number of transfer information tables) in array chain mode.
;
DMA_BTC             RS.W    1               ; .w (r/w) base transfer counter (26/$1a)
;
; Specifies the start address of the transfer information table in array chain/link array chain mode.
;
DMA_BAR             RS.L    1               ; .l (r/w) base address register (28/$1c)
                    RS.L    1
                    RS.B    1
;
; Specifies the vector number to be used for interrupts that occur when CSR ERR is 0.
;
DMA_NIV             RS.B    1               ; .b (r/w) normal interrupt vector (37/$25)
                    RS.B    1
;
; Specifies the vector number to be used for interrupts that occur when CSR ERR is 1.
;
DMA_EIV             RS.B    1               ; .b (r/w) error interrupt vector (39/$27)
                    RS.B    1
;
; bit 2-0: FC2-FC0 memory function code
;
;   000 = (unused)
;   001 = User data
;   010 = User program
;   011 = (unused)
;   100 = (unused)
;   101 = Supervisor data
;   110 = Supervisor program
;   111 = Interrupt acknowledge
;
DMA_MFC             RS.B    1               ; .b (r/w) memory function code (41/$29)
                    RS.W    1
                    RS.B    1
;
; bit 1-0: CP channel priority
;
;   00 = highest priority
;   01 = Second highest priority
;   10 = Second highest priority
;   11 = lowest priority
;
DMA_CPR             RS.B    1               ; .b (r/w) channel priority register (45/$2D)
                    RS.W    1
                    RS.B    1
;
; (Same as MFC)
;
DMA_DFC             RS.B    1               ; .b (r/w) device function code (49/$31)
                    RS.W    3
                    RS.B    1
;
; (Same as MFC)
;
DMA_BFC             RS.B    1               ; .b (r/w) base function code (53/$35)
                    RS.L    1
                    RS.B    1
;
; bit 3-2: BT burst time  (number of DMA clock cycles per burst)
;
;   00 = 16 clocks
;   01 = 32 clocks
;   10 = 64 clocks
;   11 = 128 clocks
;
; bit 1-0: BR Band width ratio (band occupancy rate)
;
;   00 = 50.00%
;   01 = 25.00%
;   10 = 12.50%
;   11 = 6.25%
;
; * Affects only channels in limited speed auto request mode.
;
DMA_GCR             RS.B    1               ; .b (r/w) general control register (59/$3B)
DMA_SIZEOF          RS.W    0               ; (60/$3C)
;
; =================================================================================
; Memory controller privileged access settings (OHM/ASA)
; =================================================================================
;
; Registers for selecting user area/supervisor area of main memory.
; Can only be written and cannot be read.
;
; Area set register:
; How much memory area will be used as supervisor area starting from address 0 in 8KB units
; The addresses from address 0 to ((setting value + 1) * 8192 - 1) are super
; A maximum range of 2MB (($ff + 1) * 8192 - 1) can be specified.
; When the main unit is reset, this register is set to 0.
;
AREA_SET_REG        equ     $E86001         ; Area set register
;
; Extended area set register:
; Specify whether to use the supervisor area in 256KB units starting from address $200000.
; Register #0 bit 0 to bit 7, ..., register #4 bit 0 to bit 7
; You can specify units from $200000 to $bfffff in this order.
; When the main unit is reset, all 0s are set in this register.
;
; * Registers added in X68030 (can also be used in X68000, but when the main unit is reset they're not initialized (?)).
;
EXT_AREA_SET_REG_0  equ     $EAFF81         ; 1.b Extended area set register 0($200000-$3fffff)
EXT_AREA_SET_REG_1  equ     $EAFF83         ; 1.b                            1($400000-$5fffff)
EXT_AREA_SET_REG_2  equ     $EAFF85         ; 1.b                            2($600000-$7fffff)
EXT_AREA_SET_REG_3  equ     $EAFF87         ; 1.b                            3($800000-$9fffff)
EXT_AREA_SET_REG_4  equ     $EAFF89         ; 1.b                            4($a00000-$bfffff)
;
; =================================================================================
; MFP (MC68901)
; =================================================================================
;
; The multi-function peripheral MC68901 is a 4 timer, 1 channel serial
; Alport (USART) has an 8-bit general-purpose I/O port (GPIP).
; The allocation for X680x0/Human68k is as follows:
; - Timer A Interrupt by V-DISP (vertical display period) signal
; - Timer B Serial port clock generation
; - Timer C Cursor blinking/FDD control, etc. interrupt
; - Timer D BG Interrupt for process switching
; - Serial port keyboard input/output
; - General-purpose I/O port Interrupts by various signals
;
; bit 7: H-SYNC CRTC H-SYNC signal
;
;   0 = horizontal retrace period
;   1 = horizontal synchronization period
;
; bit 6: CIRQ CRTC interrupt request signal (raster interrupt)
;
;   0 = requesting interrupt
;   1 = none
;
; bit 5: (unused) always 1
; bit 4: V-DISP CRTC V-DISP signal
;
;   0 = vertical retrace period
;   1 = vertical display period
;
; bit 3: FMIRQ FM sound source IC interrupt request signal
;
;   0 = requesting interrupt
;   1 = none
;
; bit 2: POW SW Status of the power switch on the front of the main unit
;
;   0 = power switch ON (normal)
;   1 = power switch OFF
;
; bit 1: EXPON signal
;
;   1 = normal
;
; bit 0: ALARM signal of RTC
;
;   1 = normal
;
MFP_GPIP            equ     $E88001         ; General-purpose I/O register
;
; For each signal, set the direction of change that will cause an interrupt to occur.
;
;   0 = Interrupt occurs on change from 0 to 1
;   1 = Interrupt occurs on change from 1 to 0
;
MFP_AER             equ     $E88003         ; Active edge register
;
; For each signal, set whether to use it as input or output.
;
;   0 = input
;   1 = Output
;
; On X680x0, all are used as inputs.
;
MFP_DRR             equ     $E88005         ; Data direction register
;
; bit 7: CRTC H-SYNC (horizontal synchronization signal)
; bit 6: CRTC raster interrupt
; bit 5: Timer A
; bit 4: MPSC receive buffer full (receive data read request)
; bit 3: MPSC receive reception error
; bit 2: MPSC transmission buffer empty (transmission data write request)
; bit 1: MPSC transmission error
; bit 0: Timer B
;
; Set whether to enable or disable the occurrence of each interrupt.
;
;   0 = Disable interrupt generation
;   1 = Enable interrupt generation
;
MFP_IERA            equ     $E88007         ; Interrupt enable register A
;
; (Same as IERA)
;
; bit 7: (GPIP 5)
; bit 6: V-DISP(GPIP 4)
; bit 5: Timer C
; bit 4: Timer D
; bit 3: FMIRQ(GPIP 3)
; bit 2: POW SW(GPIP 2)
; bit 1: EXPON(GPIP 1)
; bit 0: ALARM(GPIP 0)
;
MFP_IERB            equ     $E88009         ; Interrupt enable register B
;
; Indicates that an interrupt request is pending.
; It will be 1 if it occurs, and will return to 0 if the interrupt request is received.
;
;   0 = no interrupt request is pending
;   1 = no interrupt request has been done
;
; * Interrupts are disabled by IERA/IERB or 0 is written to IPRA/IPRB.
;   Depending on the situation, it will return to 0.
;
MFP_IPRA            equ     $E8800B         ; Interrupt pending register A
;
; (Same as IERB)
;
MFP_IPRB            equ     $E8800D         ; Interrupt pending register B
;
; Indicates that the interrupt is in service (processing).
; In software EOI (End Of interrupt = service end notification) mode, the MFP
; When an interrupt is transmitted to the MPU, it becomes 1 and writes 0 to ISRA/ISRB.
; Returns to 0.
;
; In automatic EOI mode, it automatically returns to 0 when an interrupt is reported.
;
;   0 = Interrupt request is not being processed
;   1 = Interrupt request is being processed
;
MFP_ISRA            equ     $E8800F         ; Interrupt in service register A
;
; (Same as ISRA)
;
MFP_ISRB            equ     $E88011         ; Interrupt in service register B
;
; Set interrupt mask control.
;
;   0 = Mask interrupt requests (interrupts cannot be generated)
;   1 = Don't mask interrupt requests (interrupts possible)
;
; * IERA/IERB suppresses the occurrence of interrupts,
;   but IMRA/IMRB only does not output interrupt requests even if an interrupt factor occurs.
;
MFP_IMRA            equ     $E88013         ; Interrupt mask register A
;
; (Same as IMRA)
;
MFP_IMRB            equ     $E88015         ; Interrupt mask register B
;
; bit 7-4 Upper 4 bits of interrupt vector
;
;   Upper 4 bits of vector number output at interrupt by MFP
;   The lower 4 bits are set depending on the interrupt source.
;   Determined (1111 to 0000 in descending order of priority).
;
; bit 3 In-service register enable
;
;   0 = Auto EOI mode (in-service register disabled)
;   1 = Software EOI mode (in-service register Enabled)
;
MFP_VR              equ     $E88017         ; Vector register
;
; bit 4 Reset TAO
;
;   Timer output (TAO) reset
;   0 = Normal operation
;   1 = Set the state of timer output pin to low
;
; bit 3-0: AC3-AC0 Timer operating mode
;
;   0000 = Timer stop
;   0001 = Delay mode (/  4 prescaler)
;   0010 = Delay mode (/  10 prescaler)
;   0011 = Delay mode (/  16 prescaler)
;   0100 = Delay mode (/  50 prescaler)
;   0101 = Delay mode (/  64 prescaler)
;   0110 = Delay mode (/ 100 prescaler)
;   0111 = Delay mode (/ 200 prescaler)
;   1000 = Event count mode
;   1001 = Pulse width measurement mode (/   4 prescaler)
;   1010 = Pulse width measurement mode (/  10 prescaler)
;   1011 = Pulse width measurement mode (/  16 prescaler)
;   1100 = Pulse width measurement mode (/  50 prescaler)
;   1101 = Pulse width measurement mode (/  64 prescaler)
;   1110 = Pulse width measurement mode (/ 100 prescaler)
;   1111 = Pulse width measurement mode (/ 200 prescaler)
;
MFP_TACR            equ     $E88019         ; Timer A control register
;
; (Same as TACR)
;
MFP_TBCR            equ     $E8801B         ; Timer B control register
;
; bit 6-4: CC2-CC0 Timer C operating mode
;
;   000 = Timer stop
;   001 = Delay mode (/   4 prescaler)
;   010 = Delay mode (/  10 prescaler)
;   011 = Delay mode (/  16 prescaler)
;   100 = Delay mode (/  50 prescaler)
;   101 = Delay mode (/  64 prescaler)
;   110 = Delay mode (/ 100 prescaler)
;   111 = Delay mode (/ 200 prescaler)
;
; bit 2-0: DC2-DC0 Timer D operating mode
;          (Same as CC2-CC0)
;
MFP_RCDCR           equ     $E8801D         ; Timer C&D control register
;
; Timer A value.
;
MFP_TADR            equ     $E8801F         ; Timer A data register
;
; Timer B value.
;
MFP_TBDR            equ     $E88021         ; Timer B data register
;
; Timer C value.
;
MFP_TCDR            equ     $E88023         ; Timer C data register
;
; Timer D value.
;
MFP_TDDR            equ     $E88025         ; Timer D data register
;
; Use in synchronous transfer mode.
;
MFP_SCR             equ     $E88027         ; SYNC character register
;
; bit 7: CLK transmission/reception speed
;
;   0 = Same as input clock frequency
;   1 = 1/16 of input clock frequency
;
; bit 6-5: WL1-WL0 Data length
;
;   00 = 8bit
;   01 = 7bit
;   10 = 6bit
;   11 = 5bit
;
; bit 4-3: ST1-ST0 Synchronous mode/start bit length/stop bit length
;
;   00 = Synchronization 0 0
;   01 = Synchronization 1 1
;   10 = Synchronization 1 1.5
;   11 = Asynchronous    1 2
;
; bit 2: PE parity enable/disable
;
;   0 = Parity disabled
;   1 = Parity valid
;
; bit 1: E/O Parity even/odd
;
;   0 = Odd parity
;   1 = Even number parity
;
; * X680x0 uses asynchronous 1/16 clock because it is used to communicate with the keyboard.
;   clock mode, data length 8 bits, stop bit 1, no parity must be set to 2400bps.
;
MFP_UCR             equ     $E88029         ; USART control register
;
; bit 7: BF buffer full
;
;   0 = Receive buffer is empty
;   1 = Receive buffer contains data
;
; bit 6: OE overrun error
;
;   0 = Normal operation
;   1 = Overrun error occurred
;
; bit 5: PE parity error
;
;   0 = Normal operation
;   1 = Parity error occurred
;
; bit 4: FE framing error
;
;   0 = Normal operation
;   1 = A framing error occurred (stop bit not found)
;
; bit 3: F/S or B (Found/Search or Break)
;
;   In synchronous mode:
;
;       Writing 0 enters word search mode and SYNC
;       If the same data as the character is received, it becomes 1.
;
;   In asynchronous mode:
;
;       0 = Break detected
;       1 = Not in break state
;
; bit 2: M/CIP match/character processing in progress
;
;   In synchronous mode:
;
;       0 = Received word does not match SCR
;       1 = Received word does match SCR
;
; bit 1: SS synchronous stop
;
;   0 = Do not import characters matching SCR
;   1 = Also import characters matching SCR
;
; bit 0: RE receiver enable
;
;   0 = Receiver disabled
;   1 = Receiver enabled
;
MFP_RSR             equ     $E8802B         ; Receiver status register
;
; bit 7: BE Buffer empty
;
;   0 = Data is in the send buffer
;   1 = send buffer is empty
;
; bit 6: UE underrun error
;
;   0 = Normal operation
;   1 = Underrun occurred
;
; bit 5: AT auto turnaround
;
;   0 = Normal operation
;   1 = Automatically enable receiver when transmitter is disabled
;       (automatically cleared after receiver is enabled)
;
; bit 4: END
;
;   0 = Transmitter is enabled
;   1 = Transmitter is disabled
;
; bit 3: B break
;
;   0 = Normal operation
;   1 = Put SO terminal (transmission data terminal) in break state
;
;   * Valid only in asynchronous mode.
;
; bit 2-1: H-L High/Low
;
;   00 = When TE = 0, SO terminal = high impedance
;   01 = When TE = 0, SO terminal = low impedance
;   10 = When TE = 0, SO terminal = high impedance
;   11 = Loopback mode
;
; bit 0: TE transmitter enable
;
;   0 = Transmitter disabled
;   1 = Transmitter enabled
;
MFP_TSR             equ     $E8802D         ; Transmitter status register
;
; The key code of the last key pressed or released is read.
; Even after reading once, the last input key code is retained until the next input occurs.
;
; Reading:
;
; bit 7: Make/Break
;
;   0 = key pressed
;   1 = key released
;
; Writing:
;
; Write control commands to the keyboard.
;
; * When disabling an interrupt by manipulating the spurious interrupt MFP register,
; if the MPU's interrupt mask level is set to accept the interrupt you are trying to disable,
; if an interrupt occurs the moment you manipulate the register,
; the interrupt will not be processed correctly.
; To prevent this, you can set the MPU's interrupt mask to not accept interrupts,
; or replace it with a routine that returns without doing anything even if a spurious interrupt is called.
;
; Keyboard control commands:
;
; Generates display control signals.
;
; --- bit 7-6: 00 ---
;
; bit 4-0: TV CTRL Code
;
;   Code    Name         Function                                    Key
;
;   00000 = ---          (invalid)
;   00001 = Vol.up       Volume up                                   [/\]
;   00010 = Vol.down     Volume down                                 [\/]
;   00011 = Vol.normal   Volume Standard                             [,]
;   00100 = Call         channel call                                [CLR]
;   00101 = CS down      TV screen (initialization)
;   00110 = Mute                                                     [0]
;   00111 = CH 16        (disabled)
;   01000 = BR up        TV/computer screen switching (toggle)       [.]
;   01001 = BR down      TV/external input switching (toggle)        [=]
;   01010 = BR 1/2       Contrast normal
;   01011 = CH up        Channel up                                  [<-]
;   01100 = CH down      Channel down                                [->ê]
;   01101 = ---          (invalid)
;   01110 = Power ON/OFF Power ON/OFF (toggle)
;   01111 = CS 1/2       Superimpose ON/OFF (Toggle) Contrast Down   [+]
;   10000 = CH 1         Channel 1                                   [1]
;   10001 = CH 2         Channel 2                                   [2]
;   10010 = CH 3         Channel 3                                   [3]
;   10011 = CH 4         Channel 4                                   [4]
;   10100 = CH 5         Channel 5                                   [5]
;   10101 = CH 6         Channel 6                                   [6]
;   10110 = CH 7         Channel 7                                   [7]
;   10111 = CH 8         Channel 8                                   [8]
;   11000 = CH 9         Channel 9                                   [9]
;   11001 = CH 10        Channel 10                                  [/]
;   11010 = CH 11        Channel 11                                  [*]
;   11011 = CH 12        Channel 12                                  [-]
;   11100 = CH 13        TV screen                                   [=]
;   11101 = CH 14        Computer screen                             [.]
;   11110 = CH 15        Superimpose ON/OFF (Toggle) Contrast Down   [+]
;   11111 = ---          Superimpose ON/OFF (Toggle) Contrast Normal
;
; * In X1 compatible mode =. + works for 11100 to 11110.
;
; Control mouse control signals.
;
; --- bit 7-3: 01000 ---
;
; bit 0: MSCTRL
;
;   0 = Set MSCTRL to 'Low'
;   1 = Set MSCTRL to 'High'
;
; --- bit 7-3: 01001 ---
;
; Allow/prohibit sending of key data.
; Even when disabled, key scanning continues, so display control is possible.
;
; bit 0: KEY EN
;
;   0 = Prohibit sending key data
;   1 = Permit sending key data
;
; --- bit 7-2: 010100 ---
;
; Select display control key mode.
;
; bit 0: X68K/X1
;
;   0 = X1 compatible mode
;   1 = X68000 mode
;
; Key X68000 Mode X1 Compatible Mode:
;
; + Superimpose ON/OFF (Toggle) Superimpose
; = TV/external input switching (toggle) TV
; . TV/Computer Switching (Toggle) Computer
;
; --- bit 7-2: 010101 ---
;
; Select the brightness of the LEDs on the keyboard.
;
; bit 1-0: BRIGHT
;
;   00 = Bright
;   01 = Slightly bright
;   10 = Slightly dark
;   11 = dark
;
; --- bit 7-2: 010110 ---
;
; Set whether to accept display control commands requested from the main unit to the keyboard.
;
; bit 0: CTRL EN
;
;   0 = Disable display control from main unit
;   1 = Enable display control from main unit
;
; --- bit 7-2: 010111 ---
;
; OPT.2 Allow/disallow display control using keys.
;
; bit 0: OPT.2 EN
;
;   0 = Disable display control using OPT.2 key
;   1 = Enable display control using OPT.2 key
;
; --- bit 7-4: 0110 ---
;
; bit 3-0: REP. DELAY
;
; Set the time until key repeat starts.
; The time until key repeat starts is (REP. DELAY) x 100 + 200ms, and is initialized to 500ms at reset.
;
; --- bit 7-4: 0111 ---
;
; bit 3-0: REP. TIME
;
; Set the key repeat interval.
; The key repeat interval is (REP. TIME) ^ 2 x 5 + 30ms, and is initialized to 110ms at reset.
;
; --- bit 7: 1 ---
;
; Controls whether the LED on the keyboard turns on or off.
; Each bit turns on when set to %0 and turns off when set to %1.
;
; bit 6: full-width
; bit 5: hiragana
; bit 4: INS
; bit 3: CAPS
; bit 2: code input
; bit 1: Roman alphabet
; bit 0: kana
;
; * The following commands are for Compact keyboards (unconfirmed).
;
; --- bit 7-0: 01000111 ---
;
; Determine the type of keyboard.
; If $fe is input after outputting this command, it is a Compact keyboard.
;
; --- bit 7-1: 0100010 ---
;
; Controls whether the NUM key LED on the Compact keyboard turns on or off.
;
; bit 0: NUM LED
;
;   0 = Off
;   1 = Lit
;
MFP_UDR             equ     $E8802F         ; USART data register
;
; =================================================================================
; RTC (RP5C15)
; =================================================================================
;
; Two banks of RTC (real-time clock RP5C15) registers are assigned to the same address,
; and which bank to access is set by bit 0 of the MODE register.
; All registers for setting time are in BCD format.
;
;                                             Bank 0                  Bank 1
;
; bit 2-0: CLKOUT pin output waveform selection (bank 1)
;
;   000 = High impedance ('H') (lit)
;   001 = 16384 Hz (blinking)
;   010 = 1024 Hz (blinking)
;   011 = 128 Hz (blinking)
;   100 = 16 Hz (blinking)
;   101 = 1 Hz (blinking)
;   110 = 1/60 Hz (blinking)
;   111 = 'L' level fixed (off)
;
; * 101 = seconds counter advances at startup
;   110 = minutes counter advances at startup
;
RTC_CLKOUT          equ     $E8A001         ; 1 second counter        CLKOUT select register
;
; bit 0: Adjust Seconds counter adjustment (bank 1)
;
;   0 = Adjustment OFF
;   1 = Adjustment ON
;
; * If you make an adjustment, the counter will return to 0,
;   but if the seconds are 30 or more, it will advance to the next minute.
;
RTC_COUNT2          equ     $E8A003         ; 10 seconds counter      adjust
RTC_1MIN            equ     $E8A005         ; 1 minute counter        alarm 1 minute register
RTC_10MINS          equ     $E8A007         ; 1 minutes counter       alarm 10 minutes register
RTC_1HOUR           equ     $E8A009         ; 1 hour counter          alarm 1 hour register
RTC_10HOURS         equ     $E8A00B         ; 10 hours counter        alarm 10 hours register
RTC_1WEEKDAY        equ     $E8A00D         ; day of the week counter alarm day of the week register
RTC_1DAY            equ     $E8A00F         ; 1 day counter           alarm 1 day register
RTC_10DAYS          equ     $E8A011         ; 10 days counter         alarm 10 days register
RTC_1MONTH          equ     $E8A013         ; 1 month counter         (unused)
;
; bit 0: 12/24 12 hour meter/24 hour meter selection (bank 1)
;
;   0 = 12 hour meter
;   1 = 24 hour meter
;
; * For a 12-hour meter, bit 1 of the 10-hours ($E8A00B) register indicates AM/PM.
;
;   0 = Morning
;   1 = Afternoon
;
RTC_10MONTHS        equ     $E8A015         ; 10 months counter       12/24 hour selector
;
; bit 1-0: Number of years that have passed since the leap year
;
;   00 = This year is a leap year
;   01 = Three years later is a leap year
;   10 = Two years later is a leap year
;   11 = Next year is a leap year
;
; * Set the remainder when dividing the Western calendar by 4.
;
RTC_1YEAR           equ     $E8A017         ; 1 year counter          leap year counter
RTC_10YEARS         equ     $E8A019         ; 10 years counter        (unused)
;
; bit 3: timer EN Enable/disable timer operation
;
;   0 = Disable timer operation (counter stops after second)
;   1 = Enable timer operation (normal operation)
;
; bit 2: alarm EN Alarm operation permission/prohibition
;
;   0 = Alarm operation prohibited
;   1 = Alarm operation permitted
;
; bit 0: BANK 1/0 Register bank selection
;
;   0 = BANK 0
;   1 = BANK 1
;
RTC_MODE            equ     $E8A01B         ; MODE register           MODE register
;
; bit 3-0: TEST3-TEST0
;
; * Normally, set all to 0.
;
RTC_TEST            equ     $E8A01D         ; TEST register           TEST register
;
; bit 3: 1 Hz ON 1 Hz pulse output control from Alarm terminal
;
;   0 = Output ON
;   1 = Output OFF
;
; bit 2: 16 Hz ON 16 Hz pulse output control from Alarm terminal
;
;   0 = Output ON
;   1 = Output OFF
;
; bit 1: timer RESET Reset/cancel counter of sub-second digits
;
;   0 = Normal operation
;   1 = Counter reset
;
; bit 0: Alarm RESET
;
;   0 = Cancel alarm reset
;   1 = Reset alarm
;
RTC_RESET           equ     $E8A01F         ; RESET controller        RESET controller
;
; =================================================================================
; Printer port
; =================================================================================
;
; Write the data to be output to the printer.
;
PRT_DATA            equ     $E8C001         ; Printer data (w) 
;
; bit 0: STR0 STROBE signal
;
;   0 = Set STROBE signal to low level
;   1 = Set STROBE signal to high level
;
PRT_STROBE          equ     $E8C003         ; Printer strobe (w)
;
; =================================================================================
; System controller (MESSIAH/DOSA)
; =================================================================================
;
; bit 3-0: CONTRAST computer screen contrast
;
;   0000 = Darkest
;   ...
;   1111 = Brightest
;
SYS_PORT1           equ     $E8E001         ; Computer screen contrast (r/w)
;
; bit 3: TV CTRL
;
;   When writing:
;
;       0 = Set display control signal to 0
;       1 = Set display control signal to 1
;
;   When reading:
;
;       0 = Display power is ON
;       1 = Display power is OFF
;
; bit 1: 3D-L
;
;   0 = 3D scope left shutter CLOSE
;   1 = 3D scope left shutter OPEN
;
; bit 0: 3D-R
;
;   0 = 3D scope right shutter CLOSE
;   1 = 3D scope right shutter OPEN
;
SYS_PORT2           equ     $E8E003         ; Display/3D scope control (r/w)
;
; bits 4-0: Color image unit control
;
;   The written value is output as is from pins 17 to 21 of the IMAGE IN terminal.
;
SYS_PORT3           equ     $E8E005         ; Color image unit control (w)
;
; bit 3: KEY CTRL
;
;   When writing:
;
;       0 = Key data cannot be sent
;       1 = Key data can be sent
;
;   When reading:
;
;       0 = Keyboard connector is disconnected
;       1 = Keyboard connector is inserted
;
; bit 2: NMI RESET
;
;   0 = Do not reset NMI
;   1 = Reset NMI
;
;   * Write 1 when NMI processing is complete
;
; bit 1: HRL Dot clock switching in high resolution mode
;
;   0 = Set the division ratio to 1/2, 1/3, 1/6 (normal)
;   1 = Set the division ratio to 1/2, 1/4, 1/8
;
SYS_PORT4           equ     $E8E007         ; Keyboard/NMI/dot clock control (r/w)
;
; bit 7-4: ROM Wait
;
;   Number of waits when accessing ROM
;   A wait of set value +2 is applied.
;
; bit 3-0: DRAM Wait
;
;   When accessing DRAM, the wait number setting value + 2 is applied.
;   However, in the case of 0, 0 wait is applied during static column operation,
;   and 4 wait is applied when spanning between pages.
;
; * Port added in X68030.
; * $4 (6 waits) is equivalent to 16 MHz mode,
;   $a (12 waits) is equivalent to 10 MHz mode.
;
SYS_PORT5           equ     $E8E009         ; ROM/DRAM wait number control (w)
;
; bit 7-4: CPU Type
;
;   1100 = 68040
;   1101 = 68030
;   1110 = 68020
;   1111 = 68000
;
; bit 3-0: CPU Clock
;
;   1001 = 50 MHz
;   1010 = 40 MHz
;   1011 = 33 MHz
;   1100 = 25 MHz
;   1101 = 20 MHz
;   1110 = 16 MHz
;   1111 = 10 MHz
;
; * Port added in X68000 XVI. Earlier models read $ff.
;   Also, on the X68030, it is $dc even in 16/10 MHz equivalent mode.
;
SYS_PORT6           equ     $E8E00B         ; Read MPU type/operation clock (r)
;
; bit 7-0: SRAM Write Enable Control
;
;   Writing $31 will enable writing to SRAM, and writing any other value will disable it.
;
SYS_PORT7           equ     $E8E00D         ; SRAM write control (w)
;
; bit 3-0: Power Off Control
;
;   Writing $00, $0f, and $0f in sequence turns off the power of the main unit.
;
SYS_PORT8           equ     $E8E00F         ; Main unit power OFF control (w)
;
; =================================================================================
; OPM (Fm Operator Type-m YM2151)
; =================================================================================
;
; Writing to an OPM (Fm Operator Type-m YM2151) register is done by setting the register number 
; to be accessed through the register number setting port, and then writing data to the data port.
; Reading from the register is not possible, and the status register is always read.
;
; OPM Register List
;
; $00           ---
; $01           Test register
; $02 to $07    ---
; $08           KON Register
; $09 to $0e    ---
; $0f           Noise Generator Control Register
; $10           Timer A setting register (upper)
; $11           Timer A setting register (lower)
; $12           Timer B setting register
; $13		    ---
; $14           Timer Control Register
; $15 to $17    ---
; $18           LFO frequency setting register
; $19           PMD/AMD configuration register
; $1a		    ---
; $1b           General-purpose output/LFO output waveform control register
; $1c to $1f    ---
; $20-$27       Channel Configuration/Output Control Registers -+
; $28 to $2f    KC (Key Code) registers                         | Set for each channel
; $30 to $37    KF (Key Fraction) Register                      |
; $38 to $3f    PMS/AMS setting registers                      -+
; $40 to $5f    DT1/MUL setting register                       -+
; $60 to $7f    TL (total level) setting registers              |
; $80 to $7f    KS/AR setting registers                         | Set for each slot
; $a0 to $bf    AMS-EN/D1R setting register                     |
; $c0 to $df    DT2/D2R setting registers                       |
; $e0 to $ff    D1L/RR setting registers                       -+
;
; ---------------------------------------------------------
; $01 (Test register):
;
;   bit 7-2,0: TEST
;
;       Set all to 0 for testing purposes.
;
;   bit 1: LFO RESET
;
;       0 = Starts the LFO output
;       1 = Reset the LFO
;
; $08 (KON register):
;
;   bit 6: C2
;
;       0 = C2 output OFF
;       1 = C2 output ON
;
;   bit 5: C1
;
;       0 = C1 output OFF
;       1 = C1 output ON
;
;   bit 4: M2
;
;       0 = M2 output OFF
;       1 = M2 output ON
;
;   bit 3: M1
;
;       0 = M1 output OFF
;       1 = M1 output ON
;
;   bit 2-0: CH No. Channel number selection
;
;       000 = Channel 1
;       ...
;       111 = Channel 8
;
; $0f (Noise Generator Control Register):
;
;   bit 7: NE Noise Enable
;
;       0 = Noise output OFF
;       1 = Noise output ON
;
;   bit 4-0: NFRQ Noise Frequency
;
;                                  4000
;       Noise frequency Fnoise = --------- (kHz)
;                                32 ◊ NFRQ
;
;                                 2^17-1
;       Noise period Tnoise = --------------- (Hz)
;                             Fnoise x (10^3)
;
; $10 (Timer A Configuration Register):
;
;   bit 7-0: CLKA(upper)
;
; $11 (Timer A Configuration Register):
;
;   bit 2-0: CLKA(lower)
;
;       CLKA = CLKA(upper) << 2 + CLKA(lower)
;
;                       64 x (1024 - CLKA)
;       Timer time Ta = ------------------ (ms)
;                             4000
;
; $12 (Timer B setting register):
;
;   bit 7-0: CLKB
; 
;                       1024 x (256 - CLKB)
;       Timer time Tb = ------------------- (ms)
;                              4000
;
; $14 (Timer Control Register):
;
;   bit 7: CSM
;
;       0 = Normal operation
;       1 = All slots ON when timer A overflows
;
;   bit 5: F-RESET(B)
;
;       0 = Normal operation
;       1 = Timer B overflow flag reset
;
;   bit 4: F-RESET(A)
;
;       0 = Normal operation
;       1 = Timer A overflow flag reset
;
;   bit 3: IRQEN(B)
;
;       0 = Timer B interrupt disabled
;       1 = Timer B interrupt enabled
;
;   bit 2: IRQEN(A)
;
;       0 = Timer A interrupt disabled
;       1 = Timer A interrupt enabled
;
;   bit 1: LOAD(B)
;
;       0 = Timer B is stopped
;       1 = Timer B is started
;
;   bit 0: LOAD(A)
;
;       0 = Timer A is stopped
;       1 = Timer A is started
;
; $18 (LFO frequency setting register):
;
;   bit 7-0: Sets the LFO oscillation frequency.
;
; $19 (PMD/AMD configuration register):
;
;   bit 7: F
;          Select whether the lower 7 bits are set to PMD or AMD
;
;   bit 6-0: PMD/AMD
;            LFO output level setting
;
; $1b (General-purpose output/LFO output waveform control register):
;
;   bit 7: CT1 ADPCM clock switching
;
;       0 = 8MHz
;       1 = 4Mhz
;
;   bit 6: CT2 FDC READY terminal control
;
;       0 = Normal operation
;       1 = Force into READY state
;
;   bit 1-0: W LFO output waveform selection
;
;       00 = Sawtooth wave
;       01 = Square wave
;       10 = Triangle wave
;       11 = Noise
;
; * The OPM register cannot be read, so the setting values are stored in the IOCS work $9da.b 
;   in case you want to change only some of the ADPCM/FDC/LFO. 
;   Use the work values for settings that will not be changed,
;   and if you change the values, update the work as well.
;
; $20-$27 (Channel Configuration/Output Control Registers):
;
;   bit 7: R-CH EN
;
;       0 = Right channel output OFF
;       1 = Right channel output ON
;
;   bit 6: L-CH EN
;
;       0 = Left channel output OFF
;       1 = Left channel output ON
;
;   bit 5-3: FL M1 Slot Feedback Ratio
;
;       000 = 0 (Feedback OFF)
;       001 = PI/16
;       010 = PI/8
;       011 = PI/4
;       100 = PI/2
;       101 = PI
;       110 = 2PI
;       111 = 4PI
;
;   bit 2-0: CON Connection (Slot connection method)
;
;       000 = M1 -> C1 -> M2 -> C2 -> OUT  
;       001 = C1 +
;             M1 +-> M2 -> C2 -> OUT 
;       010 =       M1 +
;             C1 -> M2 +-> C2 -> OUT 
;       011 =       M2 +
;             M1 -> C1 +-> C2 -> OUT 
;       100 = M1 -> C1 +
;             M2 -> C2 +-> OUT 
;       101 =    +-> C1 +
;             M1 +-> M2 +-> OUT 
;                +-> C2 + 
;       110 = M1 -> C1 +
;                   M2 +-> OUT
;                   C2 + 
;       111 = M1 + 
;             C1 +-> OUT 
;             M2 +
;             C2 +
;
; $28 to $2f (KC registers):
;
;   bit 6-4: OCT Octave designation
;   bit 3-0: NOTE Note specification
;
;       0000 = D#
;       0001 = E (Mi)
;       0010 = F (Fa)
;       0100 = F#
;       0101 = G (Sol)
;       0110 = G#
;       1000 = A (La)
;       1001 = A#
;       1010 = B (Si)
;       1100 = C (Do)
;       1101 = C#
;       1110 = D (Re)
;
; * In the X680x0, the OPM is clocked at 4MHz instead of the normal 3.579545MHz,
;   so the sound is about one note (192.27 cents) higher.
;
; $30 to $37 (KF Register):
;
;   bit 7-2: KF Key Fraction
;            Adjusts the pitch in 1.6 cent (=100/64) increments between 1 note (100 cents).
;
; $38 to $3f (PMS/AMS setting registers):
;
;   bit 6-4: PMS LFO frequency modulation depth setting
;
;       000 = 0 cents
;       001 = +-5 cents
;       010 = +-10 cents
;       011 = +-20 cents
;       100 = +-50 cents
;       101 = +-100 cents
;       110 = +-400 cents
;       111 = +-700 cents
;
;   bit 1-0: AMS LFO output level modulation depth setting
;
;       00 = 0 dB
;       01 = 23.90625 dB
;       10 = 47.8125 dB
;       11 = 95.625 dB
;
; * All values are when the LFO output level is at maximum.
;
; $40 to $5f (DT1/MUL setting registers):
;
;   bit 6-4: DT1
;            Sets the amount of frequency shift given by KC and KF (fine adjustment)
;
;   bit 3-0: MUL
;            Sets the multiplication factor of the frequency given by KC and KF (0x is 0.5)
;
; $60 to $7f (TL configuration registers):
;
;   bit 6-0: TL Total Level
;            Sets the output level of each slot in 0.75dB units. 
;            TL = 0 is the highest output signal level, 
;            and TL = $7f is the lowest.
;
;            Lmax = Maximum output level
;            Output level L = Lmax x 10 ^ (-1.75 / 20 ◊ TL)
;
; $80 to $7f (KS/AR setting registers):
;
;   bit 7-6: KS
;            Sets the amount of change in attack, first decay, second decay, and release time by KC.
;            Add int(int(KC / 4) / (2 ^ (3 - KS))) to the rate of each setting value.
;
;   bit 4-0: AR Attack Rate
;            Sets the time it takes for the attenuation to reach 0 dB after the key is turned on.
;
; $a0 to $bf (AMS-EN/D1R setting register):
;
;   bit 7: AMS-EN
;          Output level modulation by LFO
;
;       0 = No modulation
;       1 = Modulation
;
;   bits 4-0: D1R First decay time
;             Time from first decay to second decay.
;
; $c0 to $df (DT2/D2R setting registers):
;
;   bit 7-6: DT2 Amount to further shift the frequency given by KC and KF
;
;       00 = +0 cents (x1.00 times)
;       01 = +600 cents (x1.41 times)
;       10 = +781 cents (x1.57 times)
;       11 = +950 cents (x1.73 times)
;
;   bit 4-9: D2R Second Decay Time
;            Time from second decay to complete silence.
;
; $e0 to $ff (D1L/RR setting registers):
;
;   bit 7-4: D1L
;            Determines the level Ld1 when moving from the first decay to the second decay.
;
;            When 0 <= D1L <= $e, Ld1 = -3◊D1L (dB)
;            When D1L = $f, Ld1 = -3◊D1L -48 (dB)
;
;   bits 3-0: RR Release Time
;             The time it takes for the sound to die away after the key is released.
;
; ---------------------------------------------------------
;
; bit 7-0: Register number
;
FM_CTRL             equ     $E90001         ; Register number setting port (w)
;
; bit 7-0: Data
;
FM_DATA             equ     $E90003         ; Data port (w)
;
; bit 7: BUSY
;
;   0 = Normal operation (next data can be written)
;   1 = Data writing (not possible)
;
; bit 1: 1ST(A)
;
;   0 = Timer A has not overflowed
;   1 = Timer A overflow occurred
;
; bit 0: 1ST(B)
;
;   0 = Timer B has not overflowed
;   1 = Timer B overflow occurred
;
FM_STATUS           equ     $E90003         ; Status register (r)
;
; =================================================================================
; ADPCM (MSM6258V)
; =================================================================================
;
; bit 7: PLAY/REC
;
;   0 = Playing
;   1 = Recording/Standby
;
; bit 2: REC ST
;
;   0 = Normal operation
;   1 = ADPCM recording started
;
; bit 1: PLAY ST 
;
;   0 = Normal operation 
;   1 = ADPCM playback start
;
; bit 0: SP
;
;   0 = Normal operation
;   1 = Recording/playback operation stopped
;
ADPCM_STATUS        equ     $E92001         ; ADPCM status
; A register for inputting and outputting ADPCM data. 
; It inputs and outputs 4-bit sampling data in pairs.
; The most significant bit of the sampling data is the sign, 
; and the remainder is the absolute value.
ADPCM_INOUT         equ     $E92003         ; Data input/output
; bit 7: IOA6 Joystick #1 Trigger B
;
;   0 = Normal operation
;   1 = Press the B button
;
; bit 6: IOA5 Joystick #1 Trigger A
;
;   0 = Normal operation
;   1 = Press the A button
;
; bit 5: PC5 Joystick #2 control
;
;   0 = Normal operation
;   1 = Operation prohibited
;
; bit 4: PC4 Joystick #1 control
;
;   0 = Normal operation
;   1 = Operation prohibited
;
; bit 3-2: Sampling Rate
;
;   ADPCM sampling rate switching:
;
;                                      4MHz  8MHz
;       00 : 1/1024 of the base clock (3.9k  7.8k)
;       01 : 1/768 of the base clock  (5.2k 10.4k)
;       10 : 1/512 of the base clock  (7.8k 15.6k)
;       11 : (Unused)
;
; bit 1-0: PCM PAN
;
;   ADPCM output control:
;
;       00: Both left and right ON
;       01: Only right ON
;       10: Only left ON
;       11: Both left and right OFF
;
; * The base clock is selected by bit 7 of OPM register $1b.
ADPCM_RATE          equ     $E92005         ; Sample rate/output control
;
; =================================================================================
; Floppy disk controller (FDC) (uPD72065)
; =================================================================================
;
; bit 7: RQM Request for Master
;
;   0 = FDC data transfer is not ready
;   1 = FDC data transfer is ready
;
; bit 6: DIO Data Input/Output
;
;   0 = Transfer from host to FDC
;   1 = Transfer from FDC to host
;
; bit 5: NDM Non-DMA Mode
;
;   0 = Not currently transferring data in Non-DMA mode.
;   1 = Currently transferring data in Non-DMA mode.
;
;   * Always 0 during C-PHASE and R-PHASE.
;
; bit 4: CB FDC Busy
;
;   0 = FDC not in operation
;   1 = FDC in operation
;
;   * Not set during E-PHASE of seek/calibrate.
;
; bits 3-0: D3B-D0B
;
;   FDn Busy
;   0 = The drive is seeking or a seek complete interrupt is not pending.
;   1 = The drive is seeking and a seek complete interrupt is pending.
;
FDC_STATUS          equ     $E94001         ; status register (r)
;
; This register is used to set initialization commands,
; including SET STANDBY, RESET STANDBY, and SOFTWARE RESET.
;
FDC_CTRL            equ     $E94001         ; command register (w)
;
; Register for inputting and outputting data.
;
FDC_DATA            equ     $E94003         ; data register (rw)
;
; Register that specifies normal commands.
;
FDC_CMD             equ     $E94003         ; data register (w)
;
; =================================================================================
; Floppy drive monitor (IOSC)
; =================================================================================
;
; bit 7: DISK IN
;
;   0 = No disk inserted
;   1 = Disk inserted
;
; bit 6: ERROR DISK
;
;   0 = Not a disk misinserted
;   1 = A disk misinserted (bit 7 is also 1)
;
IOC_STATUS          equ     $E94005         ; drive status (r)
;
; bit 7: LED CTRL
;
;   0 = Turn off the FD access lamp
;   1 = Blink the same
;
;   * Only effective when no media is inserted.
;
; bit 6: EJECT MASK
;
;   0 = Eject button enabled (LED above eject button is on)
;   1 = Eject button disabled (LED is off)
;
; bit 5: EJECT ON/OFF
;
;   0 = Media will not be ejected
;   1 = Media will be ejected
;
; bit 3-0: DRIVE #3-#0
;
;   When 1 changes to 0, the option function selected in bits 7-5 is activated.
;
IOC_CTRL            equ     $E94005         ; drive option signal control (w)
;
; bit 7: MOTOR ON
;
;   0 = Drive select is disabled, motor OFF (access lamp is green).
;   1 = Drive select is enabled, motor ON (access lamp is red).
;
; bit 4: 2HD/2DD
;
;   0 = 2HD disk
;   1 = 2DD/2D disk
;
;   * Only 2HD can be used with the X680x0 internal drive.
;
; bit 1-0: ACCESS DRIVE
;
;   Sets the drive number to access.
;
IOC_ACCESS          equ     $E94007         ; Access drive select, etc.
;
; =================================================================================
; SASI
; =================================================================================
;
; Data input/output port. When this port is accessed, a REQ-ACK handshake is automatically performed.
;
SASI_INOUT          equ     $E96001         ; Data input/output
;
; bit 4: MSG (Message)
;
;   0 = The DATA line does not contain message bytes.
;   1 = It does contain message bytes.
;
; bit 3: C/D Command/Data
;
;   0 = The contents of the DATA line are data
;   1 = Same as above Command/Status
;
; bit 2: I/O Input/Output
;
;   0 = Host to Controller (Output)
;   1 = Controller to Host (Input)
;
; bit 1: BSY BUSY
;
;   0 = Not in BUSY state
;   1 = In BUSY state
;
; bit 0: REQ Request
;
;   0 = Normal operation
;   1 = The controller is requesting data transfer from the host
;
SASI_INSTAT         equ     $E96003         ; Status input (r)
;
; The written data is output to the SASI bus and the SEL signal is set to '0' (H level).
;
SASI_SIGNH          equ     $E96003         ; Signal H (w)
;
; The RESET signal is set to '1' (L level) for approximately 300µs.
;
SASI_DATA           equ     $E96005         ; Data arbitrary (w)
;
; The written data is output to the SASI bus and the SEL signal is set to '1' (L level).
;
SASI_SIGNL          equ     $E96007         ; SEL signal L (w)
;
; =================================================================================
; SCC (Z8530)
; =================================================================================
;
; To access a register on the SCC (Serial Communications Controller Z8530), 
; write the register number you want to access to the command port, 
; then write or read a value to the command port.
; If the register number is not set, WR0/RR0 will be accessed. 
; After setting the register number, once the register number is accessed, 
; the register designation is released, and if it is not specified the next time, access will revert to WR0/RR0.
;
SERIAL              equ     $E98000
;
; =================================================================================
; PPI (82C55)
; =================================================================================
;
; joystick interface
; (0 = pressed / 1 = not pressed)
;
; When $E9A005 = %0000xxxx
;
; bit 0: up
; bit 1: down
; bit 2: left
; bit 3: right
; bit 5: fire 1 (A)
; bit 6: fire 2 (B)
;
; When $E9A005 = %0100xxxx
;
; bit 0: fire 4
; bit 1: fire 5
; bit 2: fire 6
; bit 3: mode
; bit 5: fire 3
; bit 6: start
;
JOY_1               equ     $E9A001         ; Port A (Joystick #1) (r)
;
; joystick interface
; (0 = pressed / 1 = not pressed)
;
; When $E9A005 = %0000xxxx
;
; bit 0: up
; bit 1: down
; bit 2: left
; bit 3: right
; bit 5: fire 1 (A)
; bit 6: fire 2 (B)
;
; When $E9A005 = %1000xxxx
;
; bit 0: fire 4
; bit 1: fire 5
; bit 2: fire 6
; bit 3: mode
; bit 5: fire 3
; bit 6: start
;
JOY_2               equ     $E9A003         ; Port B (Joystick #2) (r)
;
; bit 7: IOC7
;
;   0 = Normal operation
;   1 = Joystick #2 Option function B operation
;
; bit 6: IOC6
;
;   0 = Normal operation
;   1 = Joystick #1 Option function A operation
;
; bit 5: IOC5
;
;   0 = Normal operation
;   1 = Joystick #2 operation disabled
;
; bit 4: IOC4
;
;   0 = Normal operation
;   1 = Joystick #1 operation disabled
;
; bit 3-2: Sampling Rate
;
;   ADPCM sampling frequency setting
;
; bit 1-0: PCM PAN
;
;   ADPCM output setting
;
JOY_STATUS          equ     $E9A005         ; Port C (joystick control) (r/w)
;
; bit 3-1: BITSEL
;
;   Specifies the bit position (7-0) to be operated on.
;
; bit 0: DATA
;
;   Specifies the data to be set.
;
; * If the most significant bit is written with a value of %0,
;   any bit in port C will be manipulated (not the bit in the control word).
;
JOY_CTRL            equ     $E9A007         ; Control word (operation mode/bit manipulation) (w)
;
; =================================================================================
; I/O selector (IOSC)
; =================================================================================
;
; Reading:
;
;   bit 7: FDC INT
;
;       0 = FDC interrupt not occurring
;       1 = FDC interrupt occurring
;
;   bit 6: FDD INT
;
;       0 = FDD interrupt not occurring
;       1 = FDD interrupt occurring
;
;   bit 5: PRT INT
;
;       0 = Printer BUSY signal = High (BUSY state)
;       1 = Printer BUSY signal = Low (READY state)
;
;   bit 4: HDD INT
;
;       0 = HDD interrupt not occurring
;       1 = HDD interrupt occurring
;
;   bit 3: HDDI EN
;
;       0 = HDD interrupt disabled
;       1 = HDD interrupt enabled
;
;   bit 2: FDCI EN
;
;       0 = FDC interrupt disabled
;       1 = FDC interrupt enabled
;
;   bit 1: FDDI EN
;
;       0 = FDD interrupt disabled
;       1 = FDD interrupt enabled
;
; Writing:
;
;   bit 3: HDDI EN
;
;       0 = Disable HDD interrupt
;       1 = Enable HDD interrupt
;
;   bit 2: FDCI EN
;
;       0 = Disable FDC interrupt
;       1 = Enable FDC interrupt
;
;   bit 1: FDDI EN
;
;       0 = Disable FDD interrupt
;       1 = Enable FDD interrupt
;
; bit 0: PRTI EN
;
;       0 = Disable printer interrupt
;       1 = Enable printer interrupt
;
IO_CTRL             equ     $E9C001         ; IOC Interrupt Status (r/w)
;
; bits 7-2: Vect Vector number
;
;   Sets the vector number to be used when an interrupt occurs.
;   The lower 2 bits are automatically determined by the interrupt cause.
;
; bits 1-0: DEVICE causing interrupt 
;
;   00 = FDC interrupt
;   01 = FDD interrupt
;   10 = HDD interrupt
;   11 = Printer interrupt
;
IO_INT              equ     $E9C003         ; IOC Interrupt Vector (w)
;
; =================================================================================
; I/O expansion area (Sharp reserved)
; =================================================================================
;
; bit 15: CA Come again
;
;   1 = CPU has requested some kind of service
;
; bit 14: PC Program counter receive request
;
;   1 = Requested
;
; bit 13: DR Data (operand) transfer direction
;
;   0 = Host CPU to 68881
;   1 = 68881 to Host CPU
;
; * If the function bit is %00100 (null primitive),
;                          %11100 (pre-instruction exception capture primitive)
;                          %11101 (instruction exception capture primitive), then %0.
;
; bits 12-8: Function
;
;   00100 = Null primitive
;   10xxx = Execution address evaluation/data transfer primitive
;   01100 = Single main processor register transfer primitive
;   00001 = Multiple coprocessor register transfer primitive
;   11100 = Pre-instruction exception take primitive
;   11101 = In-instruction exception take primitive
;
; bits 7-0: Parameter
;
COP_1_RESP          equ     $E9E000         ; Response CIR (r) .w
;
; When this register is written, the 68881 immediately stops what it was doing,
; clears any pending exceptions, and returns to an idle state.
;
COP_1_CTRL          equ     $E9E002         ; Control CIR (w) .w
;
; Reading this register causes the 68881 to halt its current operation
; and return its operating status.
;
COP_1_SAVE          equ     $E9E004         ; Save CIR (r) .w
;
; When this register is written to, 
; the 68881 suspends operation and initiates a restore operation.
;
COP_1_RESTOR        equ     $E9E006         ; Restore CIR (r/w) .w
;
; Not used in 68881.
;
COP_1_OP_WORD       equ     $E9E008         ; Operation Word CIR (---) .w
;
; Register for writing instructions to the 68881.
;
COP_1_CMD           equ     $E9E00A         ; Command CIR (w) .w
;
; Used to check conditions.
;
COP_1_COND          equ     $E9E00E         ; Condition CIR (w) .w
;
; Used for data transfer.
;
COP_1_OPER          equ     $E9E010         ; Operand CIR (w) .l
;
; It is used to pass the register mask from the 68881 to
; the CPU when a register transfer instruction is executed.
;
COP_1_REG_SEL       equ     $E9E014         ; Register selection CIR (r) .w
;
; Used to pass on a PC.
;
COP_1_ADDR_SEL      equ     $E9E018         ; Instruction Address CIR (w) .l
;
; Operand address CIR
;
COP_1_OP_ADDR       equ     $E9E01C         ; Instruction Address CIR (---) .l
;
; For the second address.
;
COP_2_RESP          equ     $E9E080
COP_2_CTRL          equ     $E9E082
COP_2_SAVE          equ     $E9E084
COP_2_RESTOR        equ     $E9E086
COP_2_OP_WORD       equ     $E9E088
COP_2_CMD           equ     $E9E08A
COP_2_COND          equ     $E9E08E
COP_2_OPER          equ     $E9E090
COP_2_REG_SEL       equ     $E9E094
COP_2_ADDR_SEL      equ     $E9E098
COP_2_OP_ADDR       equ     $E9E09C
;
SCSI                equ     $EA0000
;
; =================================================================================
; Sprite register (CYNTHIA)
; =================================================================================
;
SPR_DATA            equ     $EB0000         ; Sprite Data  (128 * 8) (r/w)
;
SPR_VFLIP_BIT       equ     15
SPR_HFLIP_BIT       equ     14
;
; Sprite 0 have priority over sprite 1
; which have priority over sprite 2 etc.
SPR_PRI_OFF         equ     %00
SPR_PRI_BACK        equ     %01             ; BG0 > BG1 > Sprite
SPR_PRI_MIDDLE      equ     %10             ; BG0 > Sprite > BG1
SPR_PRI_FRONT       equ     %11             ; Sprite > BG0 > BG1
;
                    RSSET   0
SPR_X               rs.w    1               ; 0 (0 to 1023) (visible area: 16 to 271 or 16 to 527)
SPR_Y               rs.w    1               ; 2 (0 to 1023) (visible area: 16 to 271 or 16 to 527)
;
; bit 15-14 = flip bits
; bit 11-8 = palette from $0 to $f
; bit 7-0 = picture from $0 to $ff
;
SPR_INF             rs.w    1               ; 4
SPR_PRI             rs.w    1               ; 6
SPR_SIZEOF          rs.w    0               ; 8
;
BG0_X               equ     $EB0800
BG0_Y               equ     $EB0802
BG1_X               equ     $EB0804
BG1_Y               equ     $EB0806
;
; bit 9:
;
;   1 = Display sprites and BGx
;   0 = Allow CPU to fully access the VRAM
;       (mainly to be used during VBLANK)
;
; bit 4-5:
;
;   00 = Assign BG0_DATA to BG1
;   01 = Assign BG1_DATA to BG1
;
; bit 3: display BG1 (1 = ON)
;
; bit 1-2:
;
;   00 = Assign BG0_DATA to BG0
;   01 = Assign BG1_DATA to BG0
;
; bit 0: display BG0 (1 = ON)
;
SPR_BG_CTRL         equ     $EB0808
;
; Sprite controller CRTC registers
; H-TOTAL = Horizontal total (but only if 15 KHz and 256 dots or less)
;
; * The least significant bit must be set to 1.
;
; H-DISP = Horizontal display start position + 4
; V-DISP = Vertical display start position
;
SCREEN_TOT_HORIZ    equ     $EB080A         ; Total horizontal
SCREEN_HORZ_POS     equ     $EB080C         ; Horizontal position
SCREEN_VERT_POS     equ     $EB080E         ; Vertical position
;
; bit 4: FREQ
;
;   0 = 15.98 kHz
;   1 = 31.50 kHz
;
; bit 2-3: VRES
;
;   00 = 256
;   01 = 512
;
; bit 0-1: HRES
;
;   00 = 256
;   01 = 512
;
SCREEN_SET_RES      equ     $EB0810         ; Resolution settings
;
; =================================================================================
; Sprite/BG0/BG1 VRAM
; =================================================================================
;
; Tiles are 8x8 if resolution is 256x256
; and 16x16 if resolution is 512*512
;
; Format of PCG is 4 bit chunky
;
; Format of BGx is the same as SPR_INF (See above)
;
PCG_DATA            equ     $EB8000         ; PCG data (16x16 / 8x8) (16k) (Shared PCG area) (r/w)
BG0_DATA            equ     $EBC000         ; BG0 tiles offsets (8k) (r/w)
BG1_DATA            equ     $EBE000         ; BG1 tiles offsets (8k) (r/w)
;
; =================================================================================
; Battery backed-up SRAM
; =================================================================================
;
KEY_LED             equ     $ED001C         ; State of key LED at startup (state at power off is defined)
;
