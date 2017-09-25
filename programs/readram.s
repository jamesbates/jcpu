	data SP,#0
loop:	pop Ra
	jc #hlt
	jmp #loop
hlt:	hlt

