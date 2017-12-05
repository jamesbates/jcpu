#include "symbols.h"
#include "asmerror.h"
#include <string.h>

unsigned int symbol_t_size = 0;
struct symbol_t_entry symbol_t[512];

void add_symbol(char *symbol, unsigned int address) {

    if (find_symbol(symbol) != -1) {
    
    	asmerror("Duplicate symbol definition: %s", symbol);
    }
    
    symbol_t[symbol_t_size].name = symbol;
    symbol_t[symbol_t_size].address = address;
    symbol_t_size++;
}

unsigned int find_symbol(char *symbol) {

    for (int c=0; c < symbol_t_size; c++) {

        if (!strcmp(symbol, symbol_t[c].name)) {

	    return symbol_t[c].address;
	}
    }

    return -1;
}


