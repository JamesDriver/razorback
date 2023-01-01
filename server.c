#include "net.h"
#include "functions/parser.h"
#include "termios.h"

int exec_line(char *line) {
	fgetc(stdin);
	return 0;
}

char *read_line() {
	return "";
}

void shell_init(void) {
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
}

static void print_hex(void *data, int num_bytes) {
	uint8_t *bits = data;
	for (int i = 0; i < num_bytes; i++) {
		printf("%x ", bits[i]);
	}
}

int send_msg(int_msg *msg, conn *c) {
	if (!msg) {
		return NULL_MSG;
	}
	if (!c) {
		return NULL_CONN;
	}
	int msg_size = sizeof(*msg) - sizeof(msg->data) + msg->data_sz;
	int offset = sizeof(*msg) - sizeof(msg->data);
	char *buff = malloc(msg_size);
	memcpy(buff, msg, offset);
	if (msg->type == SEND) {
		fwd_data *f = msg->data;
		memcpy(buff + offset, f, FWD_D_STATIC_LEN);
		offset += FWD_D_STATIC_LEN;
		memcpy(buff + offset, f->ip, f->ip_sz);
		offset += f->ip_sz;
		memcpy(buff + offset, f->data, f->data_sz);
	} else {
		memcpy(buff + offset, msg->data, msg->data_sz);
	}
	send(c->sock, buff, msg_size, 0);
	return 0;
}

int main() {
	// set stdin to stop buffering
	static struct termios oldtio, newtio;
	tcgetattr(0, &oldtio);
	newtio = oldtio;
	newtio.c_lflag &= ~ICANON;
	newtio.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &newtio);

	shell_init();
	return 0;
}