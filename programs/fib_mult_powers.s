; Global variables (in data memory)
		.equ	cur_prog,0
		
; program text (in program memory)
		.org 0
; Initialize stack
		data SP,#0

; Programme selector. Desired program is set in #progno
p_select:	data Rb,#3
		data Rc,#progno
		lod Rc,[Rc]
		and Rc,Rb
		data Rb,#progtab
		add Rc,Rb
		lod Rc,[Rc]
		call Rc
		jmp #p_select

; Programme entry points table (4 entries)
progtab:	#p_alternate
		#p_fibonacci
		#p_multtables
		#p_powerseries

; Alternate among 3 other programmes (Fibonacci, multiplication tables,
; power series)
p_alternate:	push Rb
		data Rb,#3
		lod Rc,[#cur_prog]
alt_inc:	inc Rc
		and Rc,Rb
		jz #alt_inc
		sto [#cur_prog],Rc
		pop Rb
		add Rc,Rb
		lod Rc,[Rc]
		call Rc
ret:		ret

; Calculate Fibonacci sequence up to 255
p_fibonacci:	xor Rb,Rb
		mov Ra,Rb
		mov Rc,Ra
		inc Rb
fib_loop:	add Rc,Rb
		jc #ret
		mov Ra,Rc
		mov Rc,Rb
		mov Rb,Ra
		jmp #fib_loop

; Show all multiplication tables up to 127. Limited to products <= 255
p_multtables:	xor Rb,Rb
mult_newtab:	inc Rb
		jn #ret
		data Rc,#0
mult_loop:	mov Ra,Rc
		add Rc,Rb
		jc #mult_newtab
		jmp #mult_loop

; Show all power series (n^1, n^2, n^3, ...) up to base 15. Limited
; to results <= 255
p_powerseries:	data Rd,#1
		push Rd
pow_next:	pop Rd
		inc Rd
		data Rb,#15
		and Rd,Rb
		jz #ret
		push Rd
		mov Ra,Rd
		mov Rb,Rd
pow_l_power:	data Rc,#0
pow_l_mult:	dec Rd
		jc #pow_cont
		mov Ra,Rc
		mov Rb,Rc
		lod Rd,[SP]
		jmp #pow_l_power
pow_cont:	add Rc,Rb
		jc #pow_next
		jmp #pow_l_mult

	.org 255
; Change this to select a different programme to run in a continuous loop
progno:		#0		;255 [11111111]: 00000000
