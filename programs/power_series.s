		data SP,#0
		data Rd,#1
		push Rd
pow_next:	pop Rd
		inc Rd
		data Rb,#15
		and Rd,Rb
		jz #end
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
end:		hlt

