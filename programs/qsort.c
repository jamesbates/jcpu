#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

uint8_t values[100];


void swap(uint8_t first, uint8_t second) {

    uint8_t t = values[first];
    values[first] = values[second];
    values[second] = t;
}


uint8_t partition(uint8_t low, uint8_t high) {

    uint8_t pivot = values[high];
    uint8_t i = low;
    for (uint8_t j=low; j < high; j++) {

        if (values[j] < pivot) {

	    swap(i,j);
	    i++;
	}
    }

    if (values[high] < values[i]) {

        swap(high, i);
    }

    return i;
}


void quicksort(uint8_t low, uint8_t high) {

    if (low < high) {

        uint8_t p = partition(low, high);

        if (p > 0) {
	    quicksort(low, p-1);
	}

        quicksort(p+1, high);
    }
}



void init_values() {
   
    srand((unsigned int) time(NULL));

    for (int c=0; c < 100; c++) {

	values[c] = (uint8_t) rand();
    }
}

void show_values() {

    for (int c=0; c < 100; c++) {
    
        printf("Random number %d is: %d\n", c, values[c]);
    }
}

int main(int argc, char **argv) {

    init_values();
    quicksort(0, 99);
    show_values();

    return 0;
}

