#include "func.h"
#include <stdio.h>

int disp_help(int argc, char **argv) {
	printf("Available Commands: \n");
	for (int i = 0; i < func_ct; i++) {
		printf(" %s\n", func_names[i]);
	}
}

int list_implants(int argc, char **argv) {
	printf("listing implants\n");
	return 0;
}

int use_implant(int argc, char **argv) {
	printf("attempting to use implant");
	return 0;
}

int server_listen(int argc, char **argv) {
	printf("listening");
	return 0;
}

int portfwd(int argc, char **argv) {
	printf("attempting to forward on implant");
	return 0;
}
