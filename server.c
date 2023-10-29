#include "libs/shell.h"
#include "libs/modload.h"
#include <stdio.h>

int main()
{
	load_modules("/home/james/dev/razorback/modules");
	// fnptr_t test1 = get_func("test1");
	// if (!test1) {
	// 	printf("no test function :(\n");
	// 	return 0;
	// } else {
	// 	test1(1000, NULL);
	// }
	shell_init();
	return 0;
}