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
	if (!c) {
		return NULL_CMD;
	}
	if (!c->exec) {
		return NULL_CMD_EXEC;
	}
	return (c->exec)(c->argc, c->argv);
}

void autocomplete(char *buff, int sz) {
	printf("h");
	if (!buff) {
		return;
	}
	if (sz < 1) {
		return;
	}
	char **words = malloc(sizeof(*words) * 8);
	if (!words) {
		return;
	}
	size_t idx = 0, cap = 8;
	for (int i = 0; i < func_ct; i++) {
		if (strlen(func_names[i]) < sz) {
			continue;
		}
		if (0 == strncmp(buff, func_names[i], sz)) {
			if (idx == cap) {
				cap *= 2;
				char **tmp =
					realloc(words, sizeof(*words) * cap);
				if (!tmp) {
					free(words);
					return;
				}
				words = tmp;
			}
			words[idx] = func_names[i];
			idx++;
		}
	}
	printf("h\n");
	if (idx = 1) {
		printf("%s", words[0]);
	}
	if (idx > 1) {
		return;
	}
	free(words);
}

char *get_line() {
	int buff_sz = CL_BUFF_SZ;
	int pos = 0;
	char *buff = malloc(sizeof(char) * buff_sz);
	int ch;

	if (!buff) {
		return NULL;
	}
	fflush(stdout);
	while (1) {
		ch = getchar();
		// printf("|%d|\n", ch);
		//if backspace
		//if tab --autocomplete
		if (ch == EOF) {
			free(buff);
			return NULL;
		} else if (ch == '\t') {
			autocomplete(buff, pos);
		} else if (ch == 127) {
			if (pos < 1) {
				continue;
			}
			printf("\b");
			printf(" ");
			printf("\b");
			pos--;
			buff[pos] = '\0';
			continue;
		}
		// if (input[i + 2] == 'A') {
		// 	upArrow = 1;
		// }
		else if (ch == '\n') {
			buff[pos] = '\0';
			printf("\n");
			break;
		} else {
			printf("%c", ch);
			buff[pos] = ch;
		}
		pos++;

		if (pos >= buff_sz) {
			buff_sz *= 2;
			char *tmp = realloc(buff, buff_sz);
			if (!tmp) {
				free(buff);
				return NULL;
			}
			buff = tmp;
		}
	}
	return buff;
}

command *parse_command(char *buff) {
	if (!buff) {
		return NULL;
	}
	int pos = strlen(buff);
	command *c = NULL;
	char *token;
	char **tokens = malloc(sizeof(char *) * TOKENS_CT);
	int t_cap = TOKENS_CT, t_idx = 0;
	for (int i = 0; i < func_ct; i++) {
		int func_len = strlen(func_names[0]);
		if (func_len > pos) {
			continue;
		}
		if (0 == strncmp(func_names[i], buff, func_len)) {
			pos = 0;
			c = malloc(sizeof(*c));
			c->exec = funcs[i];
			token = strtok(buff, DELIM);
			while (token != NULL) {
				tokens[t_idx] = token;
				t_idx++;

				if (t_idx >= t_cap) {
					char **t_tmp =
						realloc(tokens, t_cap * 2);
					if (!t_tmp) {
						free(tokens);
						free(buff);
						free(c);
						return NULL;
					}
					tokens = t_tmp;
				}
				token = strtok(NULL, DELIM);
			}
			c->argc = t_idx;
			c->argv = tokens;
			break;
		}
	}
	return c;
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