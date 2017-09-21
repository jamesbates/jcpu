; POWERS-OF-3

begin:	data Rc,	;  0 00010111
		#3	;  1 00000011
loop:	mov Ra, Rc	;  2 00000010
	mov Rb, Rc	;  3 00001010
	add Rc, Rb	;  4 11001110
	jc		;  5 00111000
		#begin	;  6 00000000
	add Rc, Rb	;  7 11001110
	jmp		;  8 00101111
		#loop	;  9 00000010
