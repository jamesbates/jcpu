	.equ	values, 0
	.equ	pivot, 100

jmp #main


;
; void swap(uint8_t first, uint8_t second)
; 
; expects: rb=first. rd=second
; clobbers: rb, rc, rd
;
swap:	push rd
	data rd, #values
	add rd, rb
	lod rc, [rd]
	pop rb
	push rd
	data rd, #values
	add rd, rb
	lod rb, [rd]
	sto [rd], rc
	pop rd
	sto [rd], rb
	ret

;
; uint8_t partition(uint8_t low, uint8_t high)
;
; expects: rb=low. rd=high
; returns: rd= return value
; clobbers: rb, rc
partition: mov rc, rb		; rc=i=low
	push rd			; stack = [high]
	data rd, #values	; rd = &values
	pop rb			; rb = high. stack = []
	add rd, rb		; rd=&values[high]
	lod rd, [rd]		; rd=values[high]
	sto [#pivot],rd		; pivot=values[high]
	mov rd, rb		; rd=high
	mov rb, rc		; rb=j=low
_1:	cmp rb, rd
	jc #_2
	push rd			; stack = [high]
	data rd, #values
	add rd, rb		; rd= &values[j]
	lod rd, [rd]		; rd = vaues[j]
	push rb			; stack = [high, j]
	lod rb, [#pivot]
	cmp rd, rb
	jc #_3
	lod rd, [sp]		; rd = j
	mov rb, rc		; rb = i
	push rc			; stack = [high, j, i]
	data rc, #swap
	call rc
	pop rc			; stack = [high, j]. rc=i
	inc rc			; i++
_3:	pop rb			; stack = [high]. rb=j
	inc rb			; j++
	pop rd			; stack = []
	jmp #_1
_2:	push rd			; stack = [high]
	mov rb, rc		; rb = i
	add rd, rb		; rd = &values[i]
	lod rd,[rd]		; rd = values[i]
	lod rb,[#pivot]
	cmp rb, rd
	jc #_4
	lod rb,[sp]		; rb = high; stack = [high]	
	mov rd, rc		; rd = i
	push rc			; stack = [high, i]
	data rc, #swap
	call rc
	pop rc			; rc = i, stack=[high]
_4:	pop rd			; stack = []
	mov rd, rc
	ret
	

;
; void quicksort(uint8_t low, uint8_t high)
;
; expects: rb=low. rd=high
; clobbers: rb, rc, rd
;
quicksort: cmp rb, rd
	jc #_5
	push rd			; stack =  [high]
	push rb			; stack = [high, low]
	data rc, #partition
	call rc			; rd = p
	pop rb			; stack = [high]. rb=low
	push rd			; stack = [high, p]
	tst rd
	jz #_6
	dec rd
	data rc, #quicksort
	call rc
_6:	pop rb
	inc rb
	pop rd
	data rc, #quicksort
	call rc
_5:	ret

;
; void show_values()
; clobbers: ra, rb, rc, rd
;
show_values:	data rb, #0
	data rc, #100
_6:	cmp rb, rc
	jc #_7
	data rd, #values
	add rd, rb
	lod ra, [rd]
	inc rb
	jmp #_6
_7:	ret
	
;
; void main()
;
; halts
; clobbers: ra,rb,rc,rd

main:	data sp, #0
	data rc, #quicksort
	data rb, #0,
	data rd, #99
	call rc
	data rc, #show_values
	call rc
	hlt
	

