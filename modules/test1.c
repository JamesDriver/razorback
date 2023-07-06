#include <stdio.h>
#include "../test.h"

int test_func(const char *c)
{
	printf("hey! %s %d\n", c, get10());
	return 0;
}