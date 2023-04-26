#include "func.h"

int disp_help(int argc, char **argv)
{
	printf("Available Commands: \n");
	for (int i = 0; i < func_ct; i++) {
		printf(" %s\n", func_names[i]);
	}
}

int list_implants(int argc, char **argv)
{
	printf("listing implants\n");
	return 0;
}

int use_implant(int argc, char **argv)
{
	printf("attempting to use implant");
	return 0;
}

int server_listen(int argc, char **argv)
{
	int opt;
	char options[] = "hp:i";

	while ((opt = getopt(argc, argv, options)) != -1) {
		switch (opt) {
		case 'h':
			printf("printing help\n");
			return 0;
		default:
			fprintf(stderr, "Unrecognized option: -%c\n", opt);
		}
	}

	if (argc - optind == 0) {
		if (!get_words(w, stdin)) {
			fprintf(stderr, "Error while parsing input from stdin");
		}
	}
	for (int i = 0; i < argc - optind; i++) {
		FILE *fp = fopen(argv[i + optind], "r");
		if (!fp) {
			fprintf(stderr, "Unable to open file: %s\n",
				argv[i + optind]);
			continue;
		}
		if (!get_words(w, fp)) {
			fprintf(stderr, "Error while parsing file: %s\n",
				argv[i + optind]);
		}
		fclose(fp);
	}

	while ((opt = getopt(argc, argv, options)) != -1) {
	}
	return 0;
}

int portfwd(int argc, char **argv)
{
	printf("attempting to forward on implant");
	return 0;
}
