#ifndef _PARSENODE_H
#define _PARSENODE_H

#include <stdint.h>
#include <stdbool.h>

enum reg {RA,RB,RC,RD,PC,SP};
enum reg reg(char *regname);

enum operand_type {NONE, REGISTER, IMMEDIATE_VAL, IMMEDIATE_SYMBOL}; 

struct operand {
    enum operand_type type;
    bool is_indirect;
    union operand_value {
        enum reg reg;
	uint8_t immvalue;
	char *immsymbol;
    } value;
};

enum mnemonic {
    DATA,MOV,JMP,JC,JN,JZ,JO,NOP,HLT,
    LOD,POP,RET,
    STO,PUSH,CALL,
    INC,DEC,ADD,ADC,SUB,SBC,XOR,OR,AND,NOT,
    LITERAL
};
enum mnemonic mnemonic(char *name);

struct instruction {
    enum mnemonic mnemonic;
    struct operand l_operand;
    struct operand r_operand;
    char *label;
    unsigned int address;
    uint8_t length;
    uint8_t byte0;
    uint8_t byte1;
};
    
union parsenode {
    char *identifier;
    uint8_t value;
    struct operand operand;
    struct instruction instruction;
};

union parsenode *parsenode_identifier(char *s);
union parsenode *parsenode_value(uint8_t value);
union parsenode *parsenode_reg_operand(enum reg reg);
union parsenode *parsenode_immval_operand(uint8_t value);
union parsenode *parsenode_immsymbol_operand(char *symbol);
union parsenode *parsenode_instruction0(enum mnemonic mnemonic);
union parsenode *parsenode_instruction1(enum mnemonic mnemonic, union parsenode *operand);
union parsenode *parsenode_instruction2(enum mnemonic mnemonic, union parsenode *l_operand, union parsenode *r_operand);

#endif

