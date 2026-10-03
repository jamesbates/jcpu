	.equ	prime_count, 0
	.equ	primes, 1

.org 0
		data sp, #0
		sto [#prime_count], sp
		call #main
		hlt



.org 32
;
;	unsigned int calc_remaind(unsigned int dividend, unsigned int divisor)
;
;	expects: rd=dividend. rb=divisor
;	returns: rd
;
calc_remaind:	cmp rd, rb
		jc #_1
		ret
_1:		sub rd, rb
		jmp #calc_remaind

.org 64
;
;	bool is_prime(unsigned int candidate)
;
;	expects: rd=candidate
;	returns: rd
;	clobbers rb,rc
;
is_prime:	data rb, #0
_5:		lod rc,[#prime_count]
		cmp rb,rc
		jc #_2
		data rc,#primes
		add rc,rb
		lod rc,[rc]
		push rb
		data rb,#16
		cmp rc,rb
		jc #_3
		mov rb,rc
		mov rc,rd
		call #calc_remaind
		tst rd
		jz #_4
		mov rd,rc
		pop rb
		inc rb
		jmp #_5
_4:		pop rb
		data rd, #0
		ret
_3:		pop rb
_2:		data rd, #1
		ret


.org 128

;
;	void find_primes()
;
;	clobbers: ra,rb,rc,rd
;
find_primes:	data rd, #3
		tst rd
_6:		jc	#_7
		push rd
		call #is_prime
		tst rd
		jz #_8
		lod ra, [sp]
		data rc, #primes
		lod rb,[#prime_count]
		add rc, rb
		sto [rc], ra
		inc rb
		sto [#prime_count],rb
_8:		pop rd
		data rb,#2
		add rd,rb
		jmp #_6
_7:		ret

.org 160

;
;	void main(void)
;
;	clobbers: ra,rb,rc,rd
main:		data ra, #2
		call #find_primes
		ret



