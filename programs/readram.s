.org	0
	jmp #start
.org 12
start:	data SP,#0
loop:	pop Ra
	jc #hlt
	jmp #loop
hlt:	hlt

