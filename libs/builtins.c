#include <stdio.h>
#include <string.h>
#include "builtins.h"
#include "modload.h"

static int help(int argc, char *argv[]);

int load_builtins(int (*register_func)(fnptr_t, const char *))
{
	if (!register_func) {
		return -1;
	}
	int err = 0;
	err	= register_func(&help, "help");
	if (err != 0) {
		return err;
	}
	return err;
}

static int help(int argc, char *argv[])
{
	char **flist;
	size_t length;
	if (0 == list_funcs(&flist, &length)) {
		for (size_t i = 0; i < length; i++) {
			if (strcmp(flist[i], "help") == 0)
				continue;
			printf("%s\n", flist[i]);
		}
	}
	return 0;
}