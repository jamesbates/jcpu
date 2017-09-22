#ifndef _SYMBOLS_H
#define _SYMBOLS_H

struct symbol_t_entry {

    char *name;
    unsigned int address;
};

extern unsigned int symbol_t_size;
extern struct symbol_t_entry symbol_t[512];

void add_symbol(char *symbol, unsigned int address);
unsigned int find_symbol(char *symbol);

#endif

