	.equ prime_count, 0
	.equ primes, 1
	.org 0
;
;    void main(): halt
;
main:		data sp, #0
		sto [#prime_count], sp	; prime_count=0
		data ra, #2
		data rc, #find_primes
		call rc
		hlt

	.org 32
;
;   void find_primes()
;
;   clobbers: ra, rb, rc, rd
;
find_primes:	data rd, #3		; rd = candidate = 3;
		tst rd
_4:		jc #_ret
		push rd			; stack = [..,candidate]
		data rc, #is_prime
		call rc
		tst rd
		jz #_5
		lod ra, [sp]		; stack = [..,candidate].ra= candidate
		data rd, #primes
		lod rb, [#prime_count]
		add rd, rb
		sto [rd], ra		; rc =primes[prime_count]
		inc rb
		sto [#prime_count], rb	; prime_count++
_5:		pop rd
		data rb, #2
		add rd, rb		; candidate +=2;
		jmp #_4
_ret:		ret

	.org 64
;
;   bool is_prime(unsigned int candidate)
;
;   expects: rd = candidate
;   returns: rd = return value.
;   clobbers: rb, rc
;
is_prime:	data rb, #0		; unsigned int prime_index=0;
_2:		lod rc, [#prime_count]  ; rc = prime_count;
		cmp rb,rc
		jc #_return_true	; if (prime_index >= prime_count)
		push rd			; stack = [..,candidate]
		data rd, #primes
		add rd, rb
		lod rc, [rd]		; rc =primes[prime_index]
		pop rd			; stack = [..]. rd = candidate
		push rb			; stack = [..,prime_index]
		data rb, #15
		cmp rb, rc
		jc #_3
		pop rb
		jmp #_return_true
_3:		mov rb, rc
		push rd			; stack = [..,prime_index,candidate]
		data rc, #calc_remaind
		call rc
		tst rd
		jz #_return_false
		pop rd			; stack =[..,prime_index].rd=candidate
		pop rb			; stack = [..]. rb = prime_index
		inc rb			; prime_index++
		jmp #_2
_return_false:	pop rd			; stack = [..,prime_index]
		pop rb			; stack = [..]
		data rd,#0
		ret
_return_true:	data rd,#1
		ret

	.org 128
;
;   unsigned int calc_remaind(unsigned int dividend, unsigned int divisor)
;
;   expects: rd = dividend. rb = divisor
;   returns: rd = return value.
;
calc_remaind:	cmp rd, rb
		jc #_1
		ret
_1:		sub rd, rb
		jmp #calc_remaind

