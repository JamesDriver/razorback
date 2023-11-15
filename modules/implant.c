#include <stdio.h>
#include <stdlib.h>
#include "../libs/modtypes.h"
#include "implant.h"

int init_module(int (*register_func)(fnptr_t, const char *))
{
	if (!register_func) {
		return -1;
	}
	int err = 0;
	err	= register_func(&implant, "implant");
	if (err != 0) {
		return err;
	}
	err = register_func(&implant_use, "implant use");
	if (err != 0) {
		return err;
	}
	return register_func(&implant_create, "implant create");
}

int implant(int argc, char *argv[])
{
	printf("create - creates an implant\nuse - use an existing implant\n");
}

int implant_create(int argc, char *argv[])
{
	printf("test2\n");
	printf("%d\n", argc);
	if (!argv) {
		printf("no argv");
		return 0;
	}
	for (int i = 0; i < argc; i++)
		printf("%s\n", argv[i]);
	return 0;
}

int implant_use(int argc, char *argv[])
{
	printf("test2\n");
	printf("%d\n", argc);
	if (!argv) {
		printf("no argv");
		return 0;
	}
	for (int i = 0; i < argc; i++)
		printf("%s\n", argv[i]);
	return 0;
}