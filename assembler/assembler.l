%{
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "parsenode.h"

#define YYSTYPE union parsenode *

#include "assembler.tab.h"

char *stolower(char *in) {

    for (char *c = in; (*c) != '\0'; c++) {

        (*c) = tolower(*c);
    }

    return in;
}



%}

%%
\n				return NEWLINE;
[A-Za-z\_][A-Za-z0-9\_]*	yylval=parsenode_identifier(stolower(strdup(yytext))); return IDENTIFIER;
[0-9]+				yylval=parsenode_value(atoi(yytext)); return NUMBER;
\#				return HASH;
\:				return COLON;
\,				return COMMA;
\.				return PERIOD;
\[				return LSQBRACKET;
\]				return RSQBRACKET;
\;[^\n]*			/* comment: ignore */
[ \r\t]+			/* whitespace: ignore */;
%%

