	.org 0
; Initialize stack
		data SP,#0	;  0 [00000000]: 00100111
				;  1 [00000001]: 00000000

; Programme selector. Desired program is set in #progno
p_select:	data Rb,#3	;  2 [00000010]: 00001111
				;  3 [00000011]: 00000011
		data Rc,#progno	;  4 [00000100]: 00010111
				;  5 [00000101]: 11111111
		lod Rc,[Rc]	;  6 [00000110]: 01010010
		and Rc,Rb	;  7 [00000111]: 11011010
		data Rb,#progtab;  8 [00001000]: 00001111
				;  9 [00001001]: 00001111
		add Rc,Rb	; 10 [00001010]: 11001110
		lod Rc,[Rc]	; 11 [00001011]: 01010010
		call Rc		; 12 [00001100]: 10110101
		jmp #p_select	; 13 [00001101]: 00101111
				; 14 [00001110]: 00000010

; Programme entry points table (4 entries)
progtab:	#p_aternate	; 15 [00001111]: 00010011
		#p_fibonacci	; 16 [00010000]: 00100011
		#p_multtables	; 17 [00010001]: 00101111
		#p_powerseries	; 18 [00010010]: 00111011

; Alternate among 3 other programmes (Fibonacci, multiplication tables,
; power series)
p_alternate:	push Rb		; 19 [00010011]: 10110001
		data Rb, #3	; 20 [00010100]: 00001111
				; 21 [00010101]: 00000011
		lod Rc, [#0]	; 22 [00010110]: 01010111
				; 23 [00010111]: 00000000
alt_inc:	inc Rc		; 24 [00011000]: 11000010
		and Rc,Rb	; 25 [00011001]: 11011010
		jz #alt_inc	; 26 [00011010]: 00111001
				; 27 [00011011]: 00011000
		sto [#0],Rc	; 28 [00011100]: 10111010
				; 29 [00011101]: 00000000
		pop Rb		; 30 [00011110]: 01001110
		add Rc,Rb	; 31 [00011111]: 11001110
		lod Rc,[Rc]	; 32 [00100000]: 01010010
		call Rc		; 33 [00100001]: 10110101
ret:		ret		; 34 [00100010]: 01101110

; Calculate Fibonacci sequence up to 255
p_fibonacci:	xor Rb,Rb	; 35 [00100011]: 11010001
		mov Ra,Rb	; 36 [00100100]: 00000001
		mov Rc,Ra	; 37 [00100101]: 00010000
		inc Rb		; 38 [00100110]: 11000001
fib_loop:	add Rc,Rb	; 39 [00100111]: 11001110
		jc #ret		; 40 [00101000]: 00111000
				; 41 [00101001]: 00100010
		mov Ra,Rc	; 42 [00101010]: 00000010
		mov Rc,Rb	; 43 [00101011]: 00010001
		mov Rb,Ra	; 44 [00101100]: 00001000
		jmp #fib_loop	; 45 [00101101]: 00101111
				; 46 [00101110]: 00100111

; Show all multiplication tables up to 127. Limited to products <= 255
p_multtables:	xor Rb,Rb	; 47 [00101111]: 11010001
mult_newtab:	inc Rb		; 48 [00110000]: 11000001
		jn #ret		; 49 [00110001]: 00111010
				; 50 [00110010]: 00100010
		data Rc,#0	; 51 [00110011]: 00010111
				; 52 [00110100]: 00000000
mult_loop:	mov Ra,Rc	; 53 [00110101]: 00000010
		add Rc,Rb	; 54 [00110110]: 11001110
		jc #mult_newtab	; 55 [00110111]: 00111000
				; 56 [00111000]: 00110000
		jmp #mult_loop	; 57 [00111001]: 00101111
				; 58 [00111010]: 00110101

; Show all power series (n^1, n^2, n^3, ...) up to base 15. Limited
; to results <= 255
p_powerseries:	data Rd,#1	; 59 [00111011]: 00011111
				; 60 [00111100]: 00000001
		push Rd		; 61 [00111101]: 10110011
pow_next:	pop Rd		; 62 [00111110]: 01011110
		inc Rd		; 63 [00111111]: 11000011
		data Rb, #15	; 64 [01000000]: 00001111
				; 65 [01000001]: 00001111
		and Rd,Rb	; 66 [01000010]: 11011011
		jz #ret		; 67 [01000011]: 00111001
				; 68 [01000100]: 00100010
		push Rd		; 69 [01000101]: 10110011
		mov Ra,Rd	; 70 [01000110]: 00000011
		mov Rb,Rd	; 71 [01000111]: 00001011
pow_l_power:	data Rc,#0	; 72 [01001000]: 00010111
				; 73 [01001001]: 00000000
pow_l_mult:	dec Rd		; 74 [01001010]: 11100011
		jc #pow_cont	; 75 [01001011]: 00111000
				; 76 [01001100]: 01010010
		mov Ra,Rc	; 77 [01001101]: 00000010
		mov Rb,Rc	; 78 [01001110]: 00001010
		lod Rd,[SP]	; 79 [01001111]: 01011100
		jmp #pow_l_power; 80 [01010000]: 00101111
				; 81 [01010001]: 01001000
pow_cont:	add Rc,Rb	; 82 [01010010]: 11001110
		jc #pow_next	; 83 [01010011]: 00111000
				; 84 [01010100]: 00111110
		jmp #pow_l_mult	; 85 [01010101]: 00101111
				; 86 [01010110]: 01001010

	.org 255
; Change this to select a different programme to run in a continuous loop
progno:		#1		;255 [11111111]: 00000001
