; MULTIPLY

	data Rb,	;  0 00001111
		#5	;  1 00000101
	data Rc,	;  2 00010111
		#3	;  3 00000011
	xor Rd, Rd	;  4 11010011
loop:	dec Rc		;  5 11100010
	jc		;  6 00111000 
		#cont	;  7 00001010
	mov Ra, Rd	;  8 00000011
	hlt		;  9 00101101
cont:	add Rd, Rb	; 10 11001111
	jmp		; 11 00101111
		#loop	; 12 00001100

