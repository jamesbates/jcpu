    	xor rb,rb
lp0:    mov ra,rb
        inc rb
lp1: 	inc ra
        mov rc,rb
    	mov rd,rb
    	mov sp,rb
    	nop
    	nop
    	nop
    	nop
    	nop
    	nop
    	nop
    	nop
    	nop
    	nop    	
    	add rb,rb
	jc #lp0
    	jmp #lp1
