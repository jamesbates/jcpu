; Show all power series (n^1, n^2, n^3, ...) up to base 15. Limited
; to results <= 255

	.org 0
begin:	data SP, #0
	data Rd, #1
	sto [#0], Rd
	data Rb, #1
	sto [#1], Rb 
next:	data Rc, #dopow
	call Rc
	lod Rb, [#1]
	tst Rb
	jz #begin
	jmp #next
dopow:	lod Rd, [#0]
	inc Rd
	data Rb, #15
	and Rd,Rb
	jz #restart
	sto [#0], Rd
	mov Ra, Rd
	mov Rb, Rd
loop0:	data Rc, #0
loop1: 	dec Rd
	jc #cont
	mov Ra, Rc
	mov Rb, Rc
	lod Rd, [#0]
	jmp #loop0
cont:	add Rc, Rb
	jc #ret
	jmp #loop1
restart:data Rb, #0
	sto [#1], Rb
ret:	ret
