; FIBONACCI

begin:	xor Rb, Rb	;  0 11010001
	mov Ra, Rb	;  1 00000001
	mov Rc, Ra	;  2 00010000
	inc Rb		;  3 11000001
loop:	add Rc, Rb	;  4 11001110
	jc		;  5 00111000
		#begin	;  6 00000000
	mov Ra, Rc	;  7 00000010
	mov Rc, Rb	;  8 00010001
	mov Rb, Ra	;  9 00001000
	jmp		; 10 00101111
		#loop	; 11 00000100
