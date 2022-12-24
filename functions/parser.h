#ifndef CLPARSE_H
#define CLPARSE_H

#include "func.h"

#include <stdlib.h>

static int (*funcs[])(int argc, char **args) = { &list_implants };
//&use_implant
//&name_implant
static char *func_names[] = { "implants" };

enum {
	FUNCS_CAP = 4,
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
	int (*exec)(int argc, char **args); //function to execute with arguments
	int argc; //arg count
	char **args; //args
} command;

command *parse_command();

int exec_command(command *c);

#endif