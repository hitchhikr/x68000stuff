                    xdef    _start
_start:          
                    lea     .compdata(pc),a0
                    lea     .decompdata(pc),a6
                    pea     (a6)
                    moveq   #-128,d1
                    moveq   #-1,d2
.literals:      
                    bsr.b   .get_elias
.copy_lits:     
                    move.b  (a0)+,(a6)+
                    subq.l  #1,d0
                    bgt.b   .copy_lits
                    add.b   d1,d1
                    bcs.b   .get_offset
.rep_match:     
                    bsr.b   .get_elias
                    subq.l  #1,d0
.do_copy_offs:  
                    lea.l   (a6),a2
                    add.l   d2,a2               
.copy_match:    
                    move.b  (a2)+,(a6)+
                    dbf     d0,.copy_match
                    add.b   d1,d1
                    bcc.b   .literals
.get_offset:    
                    moveq   #-2,d0
                    bsr.b   .elias_loop
                    addq.b  #1,d0
                    beq.b   .got_elias
                    move.w  d0,d2
                    lsl.w   #8,d2
                    moveq   #1,d0
                    move.b  (a0)+,d2
                    asr.l   #1,d2
                    bcs.b   .do_copy_offs
                    bsr.b   .elias_bt
                    bra.b   .do_copy_offs
.get_elias:     
                    moveq   #1,d0
.elias_loop:    
                    add.b   d1,d1
                    bne.s   .got_bit
                    move.b  (a0)+,d1
                    addx.b  d1,d1
.got_bit:       
                    bcs.b   .got_elias
.elias_bt:      
                    add.b   d1,d1
                    addx.l  d0,d0
                    bra.b   .elias_loop
.got_elias:     
                    rts
.compdata:          
                    incbin  "PACKED"
                    even
.decompdata:
