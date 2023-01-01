#ifndef CLPARSE_H
#define CLPARSE_H

#include "func.h"
#include "../helpers/err.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DELIM " \t\r\n\a"

enum {
	FUNCS_CAP = 4,
	CL_BUFF_SZ = 64,
	TOKENS_CT = 8,
};

typedef struct {
	int (*exec)(int argc, char **argv); //function to execute with arguments
	int argc; //arg count
	char **argv; //args
} command;

char *get_line(); //read line from stdin

command *parse_command();

int exec_command(command *c);

#endif