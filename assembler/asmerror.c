#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "asmerror.h"

unsigned int lineno = 1;

void asmerror(char *reason, char *param) {

    if (param) {

        char *fullreason = (char *) malloc(strlen(reason) + strlen(param));
	sprintf(fullreason, reason, param);
	fprintf(stderr, "Line %d: error: %s\n", lineno, fullreason);
	free(fullreason);
    } else {

        fprintf(stderr, "Line %d: error: %s\n", lineno, reason);
    }
    exit(1);
}
