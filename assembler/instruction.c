#include "instruction.h"
#include <stdio.h>
#include "asmerror.h"
#include "symbols.h"

unsigned int out_address = 0;
unsigned int instruction_count = 0;

struct instruction instructions[256];


#define is_imm(o) (o.type == IMMEDIATE_VAL || o.type == IMMEDIATE_SYMBOL)
#define is_alu(o) ((!o.is_indirect) && o.type == REGISTER && (o.value.reg == RA || o.value.reg == RB || o.value.reg == RC || o.value.reg == RD))

void check_instruction_operands(struct instruction *i) {

    if (is_imm(i->l_operand) && is_imm(i->r_operand)) {

	asmerror("Instruction cannot have two immediate operands", NULL);
    }

    switch(i->mnemonic) {

	case HLT:
	case NOP:
        case RET:
	    if ((i->l_operand.type != NONE) || (i->r_operand.type != NONE)) {

	        asmerror("Instruction does not support operands", NULL);
	    }
	    break;
	case JC:
	case JN:
	case JZ:
	case JO:
	case LITERAL:
	    if ((i->r_operand.type != NONE) || (!is_imm(i->l_operand)) || (i->l_operand.is_indirect)) {

	        asmerror("Instruction requires one direct immediate operand", NULL);
	    }
	    break;
	case JMP:
	case PUSH:
	    if ((i->l_operand.type == NONE) || (i->r_operand.type != NONE) || (i->l_operand.is_indirect)) {

	        asmerror("Instruction requires one direct immediate or register operand", NULL);
	    }
	    break;
	case POP:
	    if ((i->l_operand.type != REGISTER) || (i->r_operand.type != NONE) || (i->l_operand.is_indirect)) {

		asmerror("Instruction requires one direct register operand", NULL);
	    }
	    break;
	case CALL:
	    if ((i->l_operand.type != REGISTER) || (i->r_operand.type != NONE) || (i->l_operand.is_indirect) || (i->l_operand.value.reg != RC)) {

	        asmerror("Instruction requires single Rc operand", NULL);
	    }
	    break;
	case NOT:
	case INC:
	case DEC:
        case TST:
	    if ((!is_alu(i->l_operand)) || (i->r_operand.type != NONE)) {

	        asmerror("Instruction requires single direct ALU register operand", NULL);
	   }
	   break;
	case MOV:
	    if ((i->l_operand.type != REGISTER) || (i->l_operand.is_indirect) || (i->r_operand.type != REGISTER) || (i->r_operand.is_indirect)) {

	        asmerror("Instruction requires two direct register operands", NULL);
	    }
	    break;
	case DATA:
	    if ((i->l_operand.type != REGISTER) || (i->l_operand.is_indirect) || (!is_imm(i->r_operand)) || (i->r_operand.is_indirect)) {

		asmerror("Instruction requires one direct register and one direct immediate operand", NULL);
	    }
	    break;
	case LOD:
	    if ((i->l_operand.type != REGISTER) || (i->l_operand.is_indirect) || (i->r_operand.type == NONE) || (!i->r_operand.is_indirect)) {

	        asmerror("Instruction requires one direct register and one indirect operand", NULL);
	    }
	    break;
	case STO:
	    if ((i->l_operand.type == NONE) || (!i->l_operand.is_indirect) || (i->r_operand.type == NONE) || (i->r_operand.is_indirect)) {

	        asmerror("Instruction requires one indirect and one direct operand", NULL);
	    }
	    break;
	case ADD:
	case ADC:
	case SUB:
	case SBC:
	case AND:
	case OR:
	case XOR:
	case CMP:
	    if ((!is_alu(i->l_operand)) || (!is_alu(i->r_operand)) || (i->r_operand.value.reg != RB)) {

	        asmerror("Instruction requires direct ALU register and RB operands", NULL);
	    }
	    break;
	default:
	    asmerror("Unrecognized instruction.", NULL);
    }
}




void handle_instruction(struct instruction *i) {

    check_instruction_operands(i);

    if (out_address >= 256) {
        char buf[5];
	sprintf(buf, "%d", out_address);

        asmerror("Memory overflow. Address %s exceeds memory limit(256 bytes).", buf);
    }
    i->address = out_address;
    if (i->label) {

        add_symbol(i->label, i->address);
    }

    if ((i->mnemonic != LITERAL) && (is_imm(i->l_operand) || is_imm(i->r_operand))) {

        i->length = 2;
    } else {

	i->length = 1;
    }

    out_address += i->length;

    instructions[instruction_count++] = (*i);
}

#define CLASS_MOV 0b00000000;
#define CLASS_LOD 0b01000000;
#define CLASS_STO 0b10000000;
#define CLASS_ALU 0b11000000;

static inline uint8_t reg_code(enum reg reg) {

    switch (reg) {

        case RA: return 0b000;
	case RB: return 0b001;
        case RC: return 0b010;
	case RD: return 0b011;
	case SP: return 0b100;
	case PC: return 0b101;
    }
}

#define SREG(reg) reg_code(reg)
#define DREG(reg) (reg_code(reg) << 3)
#define SIMM 0b111
#define DIMM 0b111000
#define SSPi 0b110
#define DSPi 0b110000
#define ALU(carry, S) ((carry ? 0b00100000 : 0) | (S << 2))

static uint8_t instruction_class(enum mnemonic mnemonic) {

    switch (mnemonic) {

        case NOP:
	case HLT:
	case MOV:
	case DATA:
	case JMP:
	case JC:
	case JZ:
	case JN:
	case JO:
		return CLASS_MOV;
	case LOD:
	case POP:
	case RET:
		return CLASS_LOD;
	case STO:
	case PUSH:
	case CALL:
		return CLASS_STO;
	case ADD:
	case ADC:
	case SUB:
	case SBC:
	case INC:
	case DEC:
	case XOR:
	case OR:
	case AND:
	case NOT:
	case CMP:
        case TST:
		return CLASS_ALU;
    }
}

static void assemble_imm(uint8_t *dest, struct operand *o) {

    if (o->type == IMMEDIATE_SYMBOL) {

        unsigned int v = find_symbol(o->value.immsymbol);
	if (v == -1) {

	    asmerror("Undefined symbol %s", o->value.immsymbol);
	}
	*dest = v;
    } else {

        *dest = o->value.immvalue;
    }
}



static void assemble_dest_operand(struct instruction *i) {

    switch (i->mnemonic) {
        case MOV:
        case LOD:
        case STO:
        case POP:
        case DATA:
            if (is_imm(i->l_operand)) {
                i->byte0 |= DIMM;
            } else {
                i->byte0 |= DREG(i->l_operand.value.reg);
            }
            return;
         case PUSH:
         case CALL:
            i->byte0 |= DSPi;
            return;
        case JMP:
        case RET:
        case HLT:
            i->byte0 |= DREG(PC);
            return;
        case NOP:
            i->byte0 |= DREG(RA);
            return;
        case JZ:
        case JO:
        case JN:
        case JC:
            i->byte0 |= DIMM;
            return;
        /* ALU operations*/
        default:
            i->byte0 |= SREG(i->l_operand.value.reg);
    }
}


static void assemble_src_operand(struct instruction *i) {
    switch (i->mnemonic) {
        case MOV:
        case LOD:
        case STO:
        case DATA:
            if (is_imm(i->r_operand)) {
                i->byte0 |= SIMM;
            } else {
                i->byte0 |= SREG(i->r_operand.value.reg);
            }
            return;
         case PUSH:
         case JMP:
             if (is_imm(i->l_operand)) {
                i->byte0 |= SIMM;
            } else {
                i->byte0 |= SREG(i->l_operand.value.reg);
            }
            return;
         case CALL:
         case HLT:
            i->byte0 |= SREG(PC);
            return;
        case POP:
        case RET:
            i->byte0 |= SSPi;
            return;
        case NOP:
            i->byte0 |= SREG(RA);
            return;
        case JZ:
            i->byte0 |= 0b001;
            return;
        case JO:
            i->byte0 |= 0b100;
            return;
        case JN:
            i->byte0 |= 0b010;
            return;
        case JC:
            i->byte0 |= 0b000;
            return;
    }
}

static void assemble_alu_instruction(struct instruction *i) {
    switch (i->mnemonic) {

        case ADD:
            i->byte0 |= ALU(false,0b011);
            return;
        case ADC:
            i->byte0 |= ALU(true,0b011);
            return;
        case TST:
	    i->byte0 |= ALU(true,0b111);
	    return;
        case SUB:
            i->byte0 |= ALU(false, 0b010);
            return;
	case SBC:
	    i->byte0 |= ALU(true, 0b010);
	    return;
	case CMP:
	    i->byte0 |= ALU(true, 0b110);
	    return;
        case INC:
            i->byte0 |= ALU(false, 0b000);
            return;
	case DEC:
	    i->byte0 |= ALU(true, 0b000);
	    return;
	case XOR:
	    i->byte0 |= ALU(false, 0b100);
	    return;
	case OR:
	    i->byte0 |= ALU(false, 0b101);
	    return;
	case AND:
	    i->byte0 |= ALU(false, 0b110);
	    return;
	case NOT:
	    i->byte0 |= ALU(false, 0b111);
	    return;
    }
}


static void assemble_instruction(struct instruction *i) {

    if (i->mnemonic == LITERAL) {

        assemble_imm(&i->byte0, &i->l_operand);
        return;
    }

    i->byte0 = instruction_class(i->mnemonic);

    assemble_dest_operand(i);
    assemble_src_operand(i);
    assemble_alu_instruction(i);

    if (is_imm(i->l_operand)) {

        assemble_imm(&i->byte1, &i->l_operand);
    }

    if (is_imm(i->r_operand)) {

        assemble_imm(&i->byte1, &i->r_operand);
    }
}

void assemble_program() {

    for (int c=0; c < instruction_count; c++) {

        assemble_instruction(&instructions[c]);
    }
}




static void print_binary(uint8_t number)
{
    for (int8_t bit = 7; bit >=0; bit--) {
        printf("%c", (number & (1 << bit)) ? '1' : '0');
    }
}

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
	case CMP:
		printf("CMP ");
		break;
        case TST:
		printf("TST ");
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

static void print_instruction(struct instruction *i) {
    if (i->label) {
    	printf("%s:\t", i->label);
    } else {
	printf("\t");
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


static void output_instruction(struct instruction *i) {

    printf("%03d [", i->address);
    print_binary(i->address);
    printf("] ");
    print_binary(i->byte0);
    printf(" //\t");
    print_instruction(i);
    if (i->length == 2) {

        printf("%03d [", i->address+1);
    	print_binary(i->address+1);
    	printf("] ");
    	print_binary(i->byte1);
    	printf("\n");
    }
}

void output_program() {

    for (int c=0; c < instruction_count; c++) {

        output_instruction(&instructions[c]);
    }
}





