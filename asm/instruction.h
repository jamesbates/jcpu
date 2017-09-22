#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H

#include "parsenode.h"

extern unsigned int out_address;

void handle_instruction(struct instruction *i);
void print_instruction(struct instruction *i);

#endif

