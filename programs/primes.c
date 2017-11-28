#include <stdbool.h>
#include <stdio.h>

unsigned int prime_count = 0;
unsigned int primes[128];


unsigned int calc_remaind(unsigned int dividend, unsigned int divisor) {

    while (dividend >= divisor) {
   	dividend -= divisor;
    }
    return dividend;
}

bool is_prime(unsigned int candidate) {

    for (
		unsigned int prime_index=0;
		(prime_index < prime_count) && (primes[prime_index] < 16);
		prime_index++) {

        if (calc_remaind(candidate, primes[prime_index]) == 0) {

	     return false;
        }
    }

    return true;
}

void show_prime(unsigned int prime) {

    printf("Next prime number: %d\n", prime);
}

void find_primes() {

    for (unsigned int candidate = 3; candidate < 256; candidate+=2) {

	if (is_prime(candidate)) {

	    show_prime(candidate);
	    primes[prime_count++] = candidate;
        }
    }
}

int main(int argc, char **argv) {

    show_prime(2);
    find_primes();
}
