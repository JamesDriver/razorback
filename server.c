#include "libs/shell.h"
#include "libs/modload.h"
#include <stdio.h>

int main()
{
	load_modules("/home/james/dev/razorback/moduleserver");
	shell_init();
	return 0;
}