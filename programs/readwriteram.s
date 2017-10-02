begin:	data rd, #0
loop:	lod ra, [rd]
	sto [rd], rd
	inc rd
	jc #end
	jmp #loop
end:	hlt
