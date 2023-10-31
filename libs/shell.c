#include "shell.h"
#include "modload.h"
#include "func.h"

// filled with argc, argv for easy execution
typedef struct {
	fnptr_t fptr; // function to execute with arguments
	int argc; // arg count
	char **argv; // args
} command_t;

void shell_init(void);

static char *get_line(void);

static char *autocomplete(char *buff);

static command_t *parse_command(char *buff);

static void cmd_destroy(command_t *c);

static command_t *cmd_init();
/*
TODO:
( ) - autocomplete
( ) - handling for return code (ret_code) so user can query it
( ) - capture ctrl + c (free mem before exit)
( ) - check for memory leaks
*/

void shell_init(void)
{
	command_t *cmd = NULL;
	char *line     = NULL;
	int ret_code   = 0;
	// set stdin to stop buffering
	static struct termios oldtio, newtio;
	tcgetattr(0, &oldtio);
	newtio = oldtio;
	newtio.c_lflag &= ~ICANON;
	newtio.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &newtio);

	printf("Welcome to Razorback, a tunneling framework written in C.\n");

	while (1) {
		printf("> ");
		line = get_line();
		if (!line) {
			printf("Invalid Input\n");
			continue;
		}
		if (strlen(line) == 0) {
			continue;
		}
		cmd = parse_command(line);
		if (!cmd) {
			printf("error occurred. maybe add error handling you stupid idiot\n");
			continue;
		} else if (!cmd->fptr) {
			printf("command not found: %s\n", line);
			continue;
		} else {
			ret_code = (cmd->fptr)(cmd->argc, cmd->argv);
		}
		cmd_destroy(cmd);
		free(line);
	}
	tcsetattr(0, TCSANOW, &oldtio);
}

static command_t *cmd_init()
{
	command_t *c = NULL;

	c = malloc(sizeof(*c));
	if (!c)
		return NULL;
	c->argv = malloc(sizeof(char *) * TOKENS_CT);
	if (!c->argv) {
		cmd_destroy(c);
		return NULL;
	}
	c->fptr = NULL;
	c->argc = 0;
	return c;
}

static void cmd_destroy(command_t *c)
{
	if (!c) {
		return;
	}
	if (c->argv) {
		for (int i = 0; i < c->argc; i++) {
			free(c->argv[i]);
		}
	}
	free(c->argv);
	free(c);
}

static char *get_line(void)
{
	int pos		  = 0;
	size_t buff_sz	  = CL_BUFF_SZ;
	char *completions = NULL;
	char *buff	  = malloc(sizeof(char) * buff_sz);
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
			buff[pos]	 = '\0';
			char *completion = autocomplete(buff);
			if (!completion)
				continue;
			size_t c_len = strlen(completion);
			for (; pos < c_len; pos++) {
				buff[pos] = completion[pos];
				printf("%c", completion[pos]);
			}
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

static command_t *parse_command(char *buff)
{
	if (!buff) {
		return NULL;
	}
	int t_cap    = TOKENS_CT;
	command_t *c = cmd_init();
	if (!c) {
		return NULL;
	}

	char *curr_tok = strtok(buff, DELIM);
	while (curr_tok != NULL) {
		c->argv[c->argc] = strdup(curr_tok);
		c->argc++;

		if (c->argc >= t_cap) {
			char **tmp = realloc(c->argv, t_cap * 2);
			if (!tmp) {
				cmd_destroy(c);
				return NULL;
			}
			c->argv = tmp;
		}
		curr_tok = strtok(NULL, DELIM);
	}
	if (c->argc > 0)
		c->fptr = get_func(c->argv[0]);
	return c;
}

static char *autocomplete(char *buff)
{
	if (!buff) {
		return NULL;
	}

	char *completion   = NULL;
	char **completions = NULL;
	size_t cl_sz	   = 0;
	size_t buff_sz	   = strlen(buff);

	if (0 != list_funcs(&completions, &cl_sz))
		goto cleanup;

	for (int i = 0; i < cl_sz; i++) {
		if (0 == strncmp(buff, completions[i], buff_sz)) {
			if (completion == NULL) {
				completion = strdup(completions[i]);
			} else {
				completion = NULL;
				break;
			}
		}
	}
cleanup:
	for (int i = 0; i < cl_sz; i++) {
		free(completions[i]);
	}
	free(completions);
	return completion;
}
