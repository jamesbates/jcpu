begin:		xor rb,rb			; rb = 0
                mov sp,rb                       ; sp = 0
		inc rb				; rb = 1
		sto [#0], rb			; prime_count = 1
		inc rb				; rb = 2
		sto [#1], rb                    ; primes[0] = 2
		mov ra, rb                      ; printf("%d",primes[0])
for_next_prime:	data rc, #find_next_prime
		call rc				; ra = find_next_prime()
		data rc, #1			; rc=primes
		lod rb, [#0]			; rb = prime_count
		add rc,rb			; rc=&primes[prime_count]
		sto [rc],ra			; primes[prime_count] = ra
		inc rb				; rb++
		sto [#0], rb			; prime_count++
		jmp #for_next_prime
find_next_prime:lod rd, [#0]			; rd=prime_count
		dec rd				; rd=prime_count-1
		data rb, #1			; rb=&primes
		add rd,rb			; rd=&primes[prime_count-1]
		lod rd, [rd]			; rd=primes[prime_count-1]
		inc rd				; rd=primes[prime_count-1]+1
		push rd				; candidate=primes[prime_count-1]+1; push candidate
for_candidate:	data rc, #0			; rc=0
for_primeno:	lod rd, [sp]			; rd=candidate
		push rc				; primeno=rc; push primeno
		data rb, #1 			; rb = &primes
		add rc,rb			; rc = &primes[primeno]
		mov rb,rc			; rb = &primes[primeno]
		lod rb, [rb]			; rb = primes[primeno]
		data rc, #modulo		; rc = &modulo
		call rc				; rd = modulo(candidate,primes[primeno])
		pop rc				; rc=primeno
		xor rb,rb			; rb=0
		cmp rd,rb			; if (modulo(candidate,primes[primeno]) == 0)
		jz #break			; 	goto break;
		inc rc				; primeno++
		lod rb, [#0]			; rb=prime_count
		cmp rc,rb			; if (primeno==prime_count)
		jz #found_prime			;     goto found_prime
		jmp #for_primeno			;
break:		pop rd				; rd = candidate
		inc rd				; candidate++
		jc #end				; if (candidate >= 256) goto end
		push rd				; push candidate
		jmp #for_candidate
found_prime:	pop rd				; rd=candidate
		mov ra,rd			; ra=candidate; printf("%d", candidate)
		ret				; return candidate
modulo:		cmp rd,rb			; if (rd >= rb)
		jc #mod_cont			;	goto mod_cont
		ret				; return rd;
mod_cont:	sub rd,rb			; rd -= rb;
		jmp #modulo			;    goto modulo
end:		hlt

