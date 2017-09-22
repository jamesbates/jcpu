#include "instruction.h"
#include <stdio.h>
#include "asmerror.h"
#include "symbols.h"

unsigned int out_address = 0;
unsigned int instruction_count = 0;

struct instruction instructions[256];

static void print_mnemonic(enum mnemonic mnemonic) {

    switch (mnemonic) {

        case NOP:
		printf("NOP");
		break;
	case HLT:
		printf("HLT");
		break;
	case MOV:
		printf("MOV ");
		break;
	case DATA:
		printf("DATA ");
		break;
	case JMP:
		printf("JMP ");
		break;
	case JC:
		printf("JC ");
		break;
	case JZ:
		printf("JZ ");
		break;
	case JN:
		printf("JN ");
		break;
	case JO:
		printf("JO ");
		break;
	case LOD:
		printf("LOD ");
		break;
	case POP:
		printf("POP ");
		break;
	case RET:
		printf("RET");
		break;
	case STO:
		printf("STO ");
		break;
	case PUSH:
		printf("PUSH ");
		break;
	case CALL:
		printf("CALL ");
		break;
	case ADD:
		printf("ADD ");
		break;
	case ADC:
		printf("ADC ");
		break;
	case SUB:
		printf("SUB ");
		break;
	case SBC:
		printf("SBC ");
		break;
	case INC:
		printf("INC ");
		break;
	case DEC:
		printf("DEC ");
		break;
	case XOR:
		printf("XOR ");
		break;
	case OR:
		printf("OR ");
		break;
	case AND:
		printf("AND ");
		break;
	case NOT:
		printf("NOT ");
		break;
	case LITERAL:
		printf("");
		break;
    }
}

static void print_register(enum reg reg) {

    switch(reg) {

        case RA:
		printf("Ra");
		break;
	case RB:
		printf("Rb");
		break;
	case RC:
		printf("Rc");
		break;
	case RD:
		printf("Rd");
		break;
	case SP:
		printf("SP");
		break;
	case PC:
		printf("PC");
		break;
    }
}

static void print_operand(struct operand *o) {

    if (o->is_indirect) {

        printf("[");
    }

    if (o->type == REGISTER) {

        print_register(o->value.reg);
    } else if (o->type == IMMEDIATE_VAL) {

        printf("#%d", o->value.immvalue);
    } else {

        printf("#%s", o->value.immsymbol);
    }

    if (o->is_indirect) {

        printf("]");
    }
}

void print_binary(int number)
{
    if (number) {
        print_binary(number >> 1);
        printf("%c", (number & 1) ? '1' : '0');
    }
}

void output_instruction(struct instruction *i) {

    printf("%03d [", i->address);
    print_binary(i->address);
    printf("] ");
    switch (i->mnemonic) {

        case NOP:
	case HLT:
	case MOV:
	case DATA:
	case JMP:
	case JZ:
	case JO:
	case JN:
	case JC:
	    printf("00");
	    break;
    }
}


void print_instruction(struct instruction *i) {
    if (i->label) {
    	printf("%s:\t", i->label);
    } else {
	printf("\t\t");
    }

    print_mnemonic(i->mnemonic);
    if (i->l_operand.type != NONE) {

        print_operand(&i->l_operand);
	if (i->r_operand.type != NONE) {

	    printf(", ");
	    print_operand(&i->r_operand);
	}
    }
    printf("\n");
}

void handle_instruction(struct instruction *i) {

    if ((i->l_operand.type == IMMEDIATE_VAL || i->l_operand.type == IMMEDIATE_SYMBOL) && (i->r_operand.type == IMMEDIATE_VAL || i->r_operand.type == IMMEDIATE_SYMBOL)) {

	asmerror("Instruction cannot have two immediate operands", NULL);
    }

    if (out_address >= 256) {
        char buf[5];
	sprintf(buf, "%d", out_address);

        asmerror("Memory overflow. Address %s exceeds memory limit(256 bytes).", buf);
    }
    i->address = out_address;
    if (i->label) {

        add_symbol(i->label, i->address);
    }

    if ((i->mnemonic != LITERAL) && (i->l_operand.type == IMMEDIATE_VAL || i->l_operand.type == IMMEDIATE_SYMBOL || i->r_operand.type == IMMEDIATE_VAL || i->r_operand.type == IMMEDIATE_SYMBOL)) {

        out_address += 2;
    } else {

        out_address += 1;
    }
    instructions[instruction_count] = (*i);
}
