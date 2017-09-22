#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H

#include "parsenode.h"

extern unsigned int out_address;

void handle_instruction(struct instruction *i);
void assemble_program(void);
void output_program(void);

#endif

