#include "net.h"

int exec_line(char *line) {
	fgetc(stdin);
	return 0;
}

char *read_line() {
	return "";
}

void shell_init(void) {
	printf("Welcome to Razorback, a tunneling framework written in C.\n");
	char *line;
	int status;
	while (1) {
		printf(">");
		line = read_line();
		int status = exec_line(line);
		//free(line);
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
	memcpy(buff + offset, msg->data, msg->data_sz);
	send(c->sock, buff, msg_size, 0);
	return 0;
}

int main() {
	char *lp_ip = "127.0.0.1";
	int lp_port = 8001;

	conn *lp_conn = conn_init(lp_ip, lp_port);
	int_msg *m = malloc(sizeof(*m));
	char *words = "hey there stranger";
	m->data = words;
	m->type = SEND;
	m->modifier = 87;
	m->data_sz = strlen(words);
	send_msg(m, lp_conn);
	close(lp_conn->fd);
	//shell_init();
	return 0;
}