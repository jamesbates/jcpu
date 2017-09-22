; MULTIPLY

	data Rb, #5	;  0 00001111
			;  1 00000101
	data Rc, #3	;  2 00010111
			;  3 00000011
	xor Rd, Rd	;  4 11010011
loop:	dec Rc		;  5 11100010
	jc #cont	;  6 00111000 
			;  7 00001010
	mov Ra, Rd	;  8 00000011
	hlt		;  9 00101101
cont:	add Rd, Rb	; 10 11001111
	jmp #loop	; 11 00101111
			; 12 00000101

