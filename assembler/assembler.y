%{
#include <stdio.h>
#include <stdlib.h>
 
#include "parsenode.h"
#include "asmerror.h"
#include "instruction.h"
#include "directive.h"

#define YYSTYPE union parsenode *

int yylex();
int yyparse(void);

void yyerror(const char *str)
{
        fprintf(stderr,"Line %d: error: %s\n",lineno,str);
}
 
int yywrap()
{
        return 1;
} 
  
int main(void)
{
        yyparse();
	assemble_program();
	output_program();
} 

%}

%token PERIOD IDENTIFIER NUMBER NEWLINE COLON HASH COMMA LSQBRACKET RSQBRACKET


%%
program:
	/* empty */
	|
	program empty
	{
		lineno++;
	}
	|
	program line
	{
		handle_instruction(&$2->instruction);
		lineno++;
	}
	|
	program directive
	{
		lineno++;
	}
	;

empty:
	NEWLINE
	;

directive:
	PERIOD IDENTIFIER NUMBER NEWLINE
	{
		handle_directive($2->identifier, NULL, $3->value);
	}
	|
	PERIOD IDENTIFIER IDENTIFIER COMMA NUMBER NEWLINE
	{
		handle_directive($2->identifier, $3->identifier, $4->value);
	}
	;

line:
	label simpleline NEWLINE
	{
		$$ = $2;
		$2->instruction.label = $1->identifier;
	}
	|
	simpleline NEWLINE
	{
		$$ = $1;
	}
	;

simpleline:
	instruction
	{
		$$ = $1;
	}
	|
	literal_instruction
	{
		$$ = $1;
	}
	;

label:
	IDENTIFIER COLON
	{
		$$ = $1;
	}
	;

literal_instruction:
	num_literal
	{
		$$ = parsenode_instruction1(LITERAL, parsenode_immval_operand($1->value));
	}
	|
	symbol_literal
	{
		$$ = parsenode_instruction1(LITERAL, parsenode_immsymbol_operand($1->identifier));
	}
	;

symbol_literal:
	HASH IDENTIFIER
	{
		$$ = $2;
	}
	;

num_literal:
	HASH NUMBER
	{
		$$ = $2;
	}
	;

instruction:
	mnemonic
	{
		$$ = parsenode_instruction0(mnemonic($1->identifier));
	}
	|
	mnemonic operand
	{
		$$ = parsenode_instruction1(mnemonic($1->identifier), $2);
	}
	|
	mnemonic operand COMMA operand
	{
		$$ = parsenode_instruction2(mnemonic($1->identifier), $2, $4);
	}
	;

mnemonic:
	IDENTIFIER
	{
		$$ = $1;
	}
	;

operand:
	direct_operand
	{
		$$ = $1;
	}
	|
	indirect_operand
	{
		$$ = $1;
	}
	;

direct_operand:
	register
	{
		$$ = parsenode_reg_operand(reg($1->identifier));
	}
	|
	symbol_literal
	{
		$$ = parsenode_immsymbol_operand($1->identifier);
	}
	|
	num_literal
	{
		$$ = parsenode_immval_operand($1->value);
	}
	;

register:
	IDENTIFIER
	{
		$$ = $1;
	}
	;

indirect_operand:
	LSQBRACKET direct_operand RSQBRACKET
	{
		$$ = $2;
		$$->operand.is_indirect = true;
	}
	;

