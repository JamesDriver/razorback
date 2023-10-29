#ifndef FUNC_H
#define FUNC_H

#include "err.h"
#include "net.h"
#include <stdio.h>

// function pointer type
typedef int (*cmd_fp)(int argc, char **argv);

typedef struct {
	char *name;
	char *help;
	cmd_fp *cmd;
} shell_cmd;

// realloc for every command because there are probably not going to be that many commands
typedef struct {
	shell_cmd *cmds;
	size_t idx;
} shell_cmds;

int init_cmds(shell_cmds **cmds);

int load_shell_cmds(shell_cmds *cmds, void *mod_handle);

shell_cmds *init_shell_cmds();
shell_cmd *init_shell_cmd();
#endif