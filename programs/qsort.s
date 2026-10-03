	.equ	values, 1
	.equ	pivot, 0

.org 0
;
; void main()
;
; halts
; clobbers: ra,rb,rc,rd

main:	data sp, #0
	data rc, #SIZE
	ldp rd,[rc]
	dec rd
	data rb, #0
	call #quicksort
	call, #show_values
	hlt

.org 16
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

.org 32
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
	call #swap
	pop rc			; stack = [high, j]. rc=i
	inc rc			; i++
_3:	pop rb			; stack = [high]. rb=j
	inc rb			; j++
	pop rd			; stack = []
	jmp #_1
_2:	push rd			; stack = [high]
	mov rb, rc		; rb = i
	data rd, #values
	add rd, rb		; rd = &values[i]
	lod rd,[rd]		; rd = values[i]
	lod rb,[#pivot]
	cmp rb, rd
	jc #_4
	lod rb,[sp]		; rb = high; stack = [high]	
	mov rd, rc		; rd = i
	push rc			; stack = [high, i]
	call #swap
	pop rc			; rc = i, stack=[high]
_4:	pop rd			; stack = []
	mov rd, rc
	ret
	
.org 96
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
	call #partition		; rd = p
	pop rb			; stack = [high]. rb=low
	push rd			; stack = [high, p]
	tst rd
	jz #_6
	dec rd
	call #quicksort
_6:	pop rb
	inc rb
	pop rd
	call #quicksort
_5:	ret

.org 128
;
; void show_values()
; clobbers: ra, rb, rc, rd
;
show_values:	data rb, #0
	data rc, #SIZE	
	ldp rc, [rc]
_8:	cmp rb, rc
	jc #_7
	data rd, #values
	add rd, rb
	lod ra, [rd]
	inc rb
	jmp #_8
_7:	ret
	
	
.org 255
SIZE:	#10
