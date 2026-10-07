.org	0
	jmp #load

.org	247
load:	in rc		; length
_l1:	in rb	
	inp [rb]
	dec rc
	jz #end
	jmp #_l1
end:	hlt

