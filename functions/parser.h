#ifndef CLPARSE_H
#define CLPARSE_H

#include "list.h"

#include <stdlib.h>

//static int (*funcs[])(int argc, char **args) = { &list_implants };

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

// commands *init_commands();

command *parse_command();

int *exec_command(command *c);

#endif