#include "shell.h"

void shell_init(void);

static char *get_line(void);

static int exec_command(command *c);

static void autocomplete(char *buff, int *sz, int *pos);

static command *parse_command(char *buff);

void shell_init(void) {
	// set stdin to stop buffering
	static struct termios oldtio, newtio;
	tcgetattr(0, &oldtio);
	newtio = oldtio;
	newtio.c_lflag &= ~ICANON;
	newtio.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &newtio);

	printf("Welcome to Razorback, a tunneling framework written in C.\n");
	int status;
	while (1) {
		printf("> ");
		char *line = get_line();
		if (!line) {
			printf("Invalid Input\n");
			continue;
		}
		if (strlen(line) == 0) {
			continue;
		}
		command *c = parse_command(line);
		if (c) {
			exec_command(c);
			continue;
		} else {
			printf("command not found: %s\n", line);
			continue;
		}
	}
	tcsetattr(0, TCSANOW, &oldtio);
}

static int exec_command(command *c) {
	if (!c) {
		return NULL_CMD;
	}
	if (!c->exec) {
		return NULL_CMD_EXEC;
	}
	return (c->exec)(c->argc, c->argv);
}

static void autocomplete(char *buff, int *sz, int *pos) {
	if (!buff || !sz || !pos) {
		return;
	}
	if (*pos < 1) {
		return;
	}
	char **words = malloc(sizeof(*words) * 8);
	if (!words) {
		return;
	}
	size_t idx = 0, cap = 8;
	for (int i = 0; i < func_ct; i++) {
		if (strlen(func_names[i]) < *pos) {
			continue;
		}
		if (0 == strncmp(buff, func_names[i], *pos)) {
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
	if (idx == 1) {
		printf("%s", words[0] + *pos);
		size_t w_sz = strlen(words[0] + *pos);
		if (*sz < *pos + w_sz + 1) {
			char *tmp = realloc(buff, *pos + w_sz + 1);
			if (!tmp) {
				free(words);
				return;
			}
			buff = tmp;
			*sz = *pos + w_sz + 1;
		}
		strncat(buff, words[0] + *pos, w_sz);
	}
	if (idx > 1) {
	}
	free(words);
}

static char *get_line(void) {
	int buff_sz = CL_BUFF_SZ;
	int pos = 0;
	char *buff = malloc(sizeof(char) * buff_sz);
	int ch;

	if (!buff) {
		return NULL;
	}
	fflush(stdout);
	while (1) {
		fflush(stdout);
		ch = getchar();
		if (ch == EOF) {
			free(buff);
			return NULL;
		} else if (ch == '\t') {
			autocomplete(buff, &buff_sz, &pos);
			continue;
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
		} else if (ch == '\n') {
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

static command *parse_command(char *buff) {
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