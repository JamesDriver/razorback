#ifndef SHELL_H
#define SHELL_H

#include "func.h"
#include "err.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "termios.h"
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

void shell_init(void);

#endif