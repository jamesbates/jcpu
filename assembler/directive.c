#include "directive.h"
#include "instruction.h"

#include "asmerror.h"
#include <string.h>
#include <stdio.h>

void handle_directive(char *directive, uint8_t value) {

    if (strcmp(directive, "org")) {

        asmerror("Unknown directive .%s", directive);
    }

    if (value < out_address) {

        char buf[5];
	sprintf(buf, "%d", value);

        asmerror("Cannot decrease output address to %s", buf);
    }

    out_address = value;
}
