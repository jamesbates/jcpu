.org 0

	data sp, #0
	data ra, #1
	data rc, #func1
	call rc
	data ra, #2
	call #func2
	data ra, #3
	data rc, #val_4
	ldp ra, [rc]
	hlt

func1:	data ra, #10
	ret

func2:	data ra, #20
	ret

.org 255
val_4:	#4
