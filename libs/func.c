#include "func.h"

int init_cmds(shell_cmds **cmds)
{
	if (!cmds) {
		return -1; // todo: err code
	}
}

int load_shell_cmds(shell_cmds *cmds, void *mod_handle)
{
	if (!cmds || !mod_handle) {
		return -1; // todo: err code
	}
	int (*init_func)(shell_cmds *, size_t *) = dlsym(mod_handle, "module_init");
	if (!init_func) {
		return -1; // todo: err code
	}
	shell_cmds *module_cmds;
	size_t module_cmd_ct;
	init_func(&module_cmds, &module_cmd_ct);
	for (size_t i = 0; i < module_cmd_ct; i++) {
	}
}

shell_cmds *init_shell_cmds()
{
	shell_cmds *scmds = malloc(sizeof(*scmds));
	if (!scmds) {
		return NULL;
	}
	scmds->cmds = NULL;
	scmds->idx  = 0;
	return scmds;
}

shell_cmd *init_shell_cmd(char *name, char *help, cmd_fp *cmd)
{
	if (!name || !help || !cmd) {
		return NULL;
	}
	shell_cmd *scmd = malloc(sizeof(*scmd));
	if (!scmd) {
		return NULL;
	}
	scmd->name = name;
	scmd->help = help;
	scmd->cmd  = cmd;
	return scmd;
}