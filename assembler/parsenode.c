#include "parsenode.h"

#include <malloc.h>
#include <string.h>

#include "asmerror.h"

enum reg reg(char *regname) {

    if (strcmp("ra",regname)==0) {

        return RA;
    }
    if (strcmp("rb",regname)==0) {

        return RB;
    }
    if (strcmp("rc",regname)==0) {

        return RC;
    }
    if (strcmp("rd",regname)==0) {

        return RD;
    }
    if (strcmp("sp",regname)==0) {

        return SP;
    }
    if (strcmp("pc",regname)==0) {

        return PC;
    }
    asmerror("Unknown register %s", regname);
    return RA;
}

enum mnemonic mnemonic(char *name) {

    if (strcmp("data",name)==0) {

        return DATA;
    }
    if (strcmp("mov",name)==0) {

        return MOV;
    }
    if (strcmp("nop",name)==0) {

        return NOP;
    }
    if (strcmp("hlt",name)==0) {

        return HLT;
    }
    if (strcmp("jmp",name)==0) {

        return JMP;
    }
    if (strcmp("jc",name)==0) {

        return JC;
    }
    if (strcmp("jz",name)==0) {

        return JZ;
    }
    if (strcmp("jn",name)==0) {

        return JN;
    }
    if (strcmp("jo",name)==0) {

        return JO;
    }
    if (strcmp("lod",name)==0) {

        return LOD;
    }
    if (strcmp("pop",name)==0) {

        return POP;
    }
    if (strcmp("ret",name)==0) {

        return RET;
    }
    if (strcmp("sto",name)==0) {

        return STO;
    }
    if (strcmp("push",name)==0) {

        return PUSH;
    }
    if (strcmp("call",name)==0) {

        return CALL;
    }
    if (strcmp("add",name)==0) {

        return ADD;
    }
    if (strcmp("adc",name)==0) {

        return ADC;
    }
    if (strcmp("sub",name)==0) {

        return SUB;
    }
    if (strcmp("sbc",name)==0) {

        return SBC;
    }
    if (strcmp("inc",name)==0) {

        return INC;
    }
    if (strcmp("dec",name)==0) {

        return DEC;
    }
    if (strcmp("xor",name)==0) {

        return XOR;
    }
    if (strcmp("or",name)==0) {

        return OR;
    }
    if (strcmp("and",name)==0) {

        return AND;
    }
    if (strcmp("not",name)==0) {

        return NOT;
    }
    if (strcmp("cmp",name)==0) {

        return CMP;
    }
    if (strcmp("tst",name)==0) {
        
        return TST;
    }
    asmerror("Unknown instruction %s", name);
    return NOP;
}

union parsenode *parsenode_identifier(char *s) {

    union parsenode *r = (union parsenode *) malloc(sizeof(union parsenode));
    r->identifier = s;
    return r;
}

union parsenode *parsenode_value(uint8_t value) {

    union parsenode *r = (union parsenode *) malloc(sizeof(union parsenode));
    r->value = value;
    return r;
}

union parsenode *parsenode_reg_operand(enum reg reg) {

    union parsenode *r = (union parsenode *) malloc(sizeof(union parsenode));
    r->operand.type = REGISTER;
    r->operand.is_indirect = false;
    r->operand.value.reg = reg;
    return r;
}

union parsenode *parsenode_immval_operand(uint8_t value) {

    union parsenode *r = (union parsenode *) malloc(sizeof(union parsenode));
    r->operand.type = IMMEDIATE_VAL;
    r->operand.is_indirect = false;
    r->operand.value.immvalue = value;
    return r;
}

union parsenode *parsenode_immsymbol_operand(char *symbol) {

    union parsenode *r = (union parsenode *) malloc(sizeof(union parsenode));
    r->operand.type = IMMEDIATE_SYMBOL;
    r->operand.is_indirect = false;
    r->operand.value.immsymbol = symbol;
    return r;
}

union parsenode *parsenode_instruction0(enum mnemonic mnemonic) {

    union parsenode *r = (union parsenode *) malloc(sizeof(union parsenode));
    r->instruction.mnemonic = mnemonic;
    r->instruction.l_operand.type = NONE;
    r->instruction.r_operand.type = NONE;
    r->instruction.label = NULL;
    return r;
}

union parsenode *parsenode_instruction1(enum mnemonic mnemonic, union parsenode *operand) {

    union parsenode *r = (union parsenode *) malloc(sizeof(union parsenode));
    r->instruction.mnemonic = mnemonic;
    r->instruction.l_operand = operand->operand;
    free(operand);
    r->instruction.r_operand.type = NONE;
    r->instruction.label = NULL;
    return r;
}

union parsenode *parsenode_instruction2(enum mnemonic mnemonic, union parsenode *l_operand, union parsenode *r_operand) {

    union parsenode *r = (union parsenode *) malloc(sizeof(union parsenode));
    r->instruction.mnemonic = mnemonic;
    r->instruction.l_operand = l_operand->operand;
    free(l_operand);
    r->instruction.r_operand = r_operand->operand;
    free(r_operand);
    r->instruction.label = NULL;
    return r;
}
