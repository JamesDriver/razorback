#include <stdio.h>
#include <stdlib.h>
#include "../libs/modtypes.h"

static int test1(int argc, char *argv[]);
static int test2(int argc, char *argv[]);

int init_module(int (*register_func)(fnptr_t, const char *))
{
	if (!register_func) {
		return -1;
	}
	int err = 0;
	err	= register_func(&test1, "test");
	if (err != 0) {
		return err;
	}
	return register_func(&test2, "notest");
}

static int test1(int argc, char *argv[])
{
	if (!argv)
		printf("no argv");
	printf("test1\n");
	printf("%d\n", argc);
	return 0;
}

static int test2(int argc, char *argv[])
{
	if (!argv)
		printf("no argv");
	printf("test2\n");
	printf("%d\n", argc);
	for (int i = 0; i < argc; i++)
		printf("%s\n", argv[i]);
	return 0;
}