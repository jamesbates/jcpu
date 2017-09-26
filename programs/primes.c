#include <stdio.h>
#include <stdint.h>
#define MAX_VALUE 255

uint8_t primes[128];
uint8_t prime_count = 0;

uint8_t modulo(uint16_t val, uint8_t divider) {

    while (val >= divider) {

        val -= divider;
    }

    return val;
}

uint16_t find_next_prime() {

    for (uint16_t candidate = primes[prime_count-1]+1; ; candidate++) {

	uint8_t primeno;

        for (primeno=0; primeno < prime_count; primeno++) {

	    if (modulo(candidate, primes[primeno]) == 0) {

                break;
	    }
        }

	if (primeno == prime_count) {

	    return candidate;
	}
    }
}

int main(void) {

    uint16_t next_prime = 2;

    do {

	printf("Next prime is: %d\n", next_prime);
        primes[prime_count++] = next_prime;

	next_prime = find_next_prime();
    } while (next_prime <= MAX_VALUE);

    return 0;
}
