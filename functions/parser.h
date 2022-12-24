#ifndef CLPARSE_H
#define CLPARSE_H

#include "func.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DELIM " \t\r\n\a"

static int (*funcs[])(int argc, char **argv) = { &list_implants };
//&use_implant
//&name_implant
static char *func_names[] = { "implant list", "implant use", "implant name" };

enum {
	FUNCS_CAP = 4,
	CL_BUFF_SZ = 64,
	TOKENS_CT = 8,
};

// typedef struct {
// 	int (**funcs)(int argc, char **args);
// 	size_t idx;
// 	size_t cap;
// } commands;
/*
Parse and execute command line input
*/
typedef struct {
	int (*exec)(int argc, char **argv); //function to execute with arguments
	int argc; //arg count
	char **argv; //args
} command;

command *parse_command();

int exec_command(command *c);

#endif