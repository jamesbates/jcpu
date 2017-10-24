#include "directive.h"
#include "instruction.h"
#include "symbols.h"

#include "asmerror.h"
#include <string.h>
#include <stdio.h>

void handle_directive(char *directive, char *symbol, uint8_t value) {

    if (!strcmp(directive, "org")) {

        if (symbol != NULL) {

	    asmerror(".org directive accepts only one operand", NULL);
	}

	if (value < out_address) {

	    char buf[5];
	    sprintf(buf, "%d", value);

            asmerror("Cannot decrease output address to %s", buf);
        }

	out_address = value;
    } else if (!strcmp(directive, "equ")) {

        if (symbol == NULL) {

	    asmerror(".equ directive requires two operands", NULL);
	}

	add_symbol(symbol, value);
    } else {

        asmerror("Unknown directive .%s", directive);
    }
}
