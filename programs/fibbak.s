begin:	data Rb,#144
	data Ra, #233
loop:	mov Rc,Ra
	sub Rc,Rb
	mov Ra,Rb
	mov Rb,Rc
	jz #end
	jmp #loop
end:	mov Ra,Rb
	jmp #begin

