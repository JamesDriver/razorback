#include "parser.h"

// commands *init_commands() {
// 	commands *c = malloc(sizeof(*c));
// 	if (!c) {
// 		return NULL;
// 	}
// 	c->funcs = malloc(sizeof(c->funcs) * FUNCS_CAP);
// 	if (!c->funcs) {
// 		free(c);
// 		return NULL;
// 	}
// 	c->idx = 0;
// 	c->cap = FUNCS_CAP;
// 	size_t func_ct = sizeof(funcs) / sizeof(funcs[0]);
// 	for (int i = 0; i < func_ct; i++) {
// 		append_commands(c, funcs[i]);
// 	}
// }

int exec_command(command *c) {
	return (c->exec)(c->argc, c->args);
}

command *parse_command() {
}

// char *lsh_read_line(void) {
// #ifdef LSH_USE_STD_GETLINE
// 	char *line = NULL;
// 	ssize_t bufsize = 0; // have getline allocate a buffer for us
// 	if (getline(&line, &bufsize, stdin) == -1) {
// 		if (feof(stdin)) {
// 			exit(EXIT_SUCCESS); // We received an EOF
// 		} else {
// 			perror("lsh: getline\n");
// 			exit(EXIT_FAILURE);
// 		}
// 	}
// 	return line;
// #else
// #define LSH_RL_BUFSIZE 1024
// 	int bufsize = LSH_RL_BUFSIZE;
// 	int position = 0;
// 	char *buffer = malloc(sizeof(char) * bufsize);
// 	int c;

// 	if (!buffer) {
// 		fprintf(stderr, "lsh: allocation error\n");
// 		exit(EXIT_FAILURE);
// 	}

// 	while (1) {
// 		// Read a character
// 		c = getchar();

// 		if (c == EOF) {
// 			exit(EXIT_SUCCESS);
// 		} else if (c == '\n') {
// 			buffer[position] = '\0';
// 			return buffer;
// 		} else {
// 			buffer[position] = c;
// 		}
// 		position++;

// 		// If we have exceeded the buffer, reallocate.
// 		if (position >= bufsize) {
// 			bufsize += LSH_RL_BUFSIZE;
// 			buffer = realloc(buffer, bufsize);
// 			if (!buffer) {
// 				fprintf(stderr, "lsh: allocation error\n");
// 				exit(EXIT_FAILURE);
// 			}
// 		}
// 	}
// #endif
// }