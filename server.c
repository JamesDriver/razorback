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
	char *lp_ip = "127.0.0.1";
	int lp_port = 8001;

	conn *lp_conn = conn_init(lp_ip, lp_port);
	int_msg *m = malloc(sizeof(*m));
	fwd_data *f = malloc(sizeof(f));
	char *fwd_ip = "127.0.0.1";
	char *words = "hey there stranger";
	f->port = 8002;
	f->ip_sz = strlen(fwd_ip);
	f->data_sz = strlen(words);
	f->ip = fwd_ip;
	f->data = words;

	m->data = f;
	m->type = SEND;
	m->modifier = 0;
	m->data_sz = FWD_D_STATIC_LEN + f->ip_sz + f->data_sz;
	send_msg(m, lp_conn);
	close(lp_conn->fd);
	//shell_init();
	return 0;
}