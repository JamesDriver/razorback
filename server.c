#include "libs/shell.h"

int main() {
	shell_init();
	return 0;
}

// static void print_hex(void *data, int num_bytes) {
// 	uint8_t *bits = data;
// 	for (int i = 0; i < num_bytes; i++) {
// 		printf("%x ", bits[i]);
// 	}
// }

// int send_msg(int_msg *msg, conn *c) {
// 	if (!msg) {
// 		return NULL_MSG;
// 	}
// 	if (!c) {
// 		return NULL_CONN;
// 	}
// 	int msg_size = sizeof(*msg) - sizeof(msg->data) + msg->data_sz;
// 	int offset = sizeof(*msg) - sizeof(msg->data);
// 	char *buff = malloc(msg_size);
// 	memcpy(buff, msg, offset);
// 	if (msg->type == SEND) {
// 		fwd_data *f = msg->data;
// 		memcpy(buff + offset, f, FWD_D_STATIC_LEN);
// 		offset += FWD_D_STATIC_LEN;
// 		memcpy(buff + offset, f->ip, f->ip_sz);
// 		offset += f->ip_sz;
// 		memcpy(buff + offset, f->data, f->data_sz);
// 	} else {
// 		memcpy(buff + offset, msg->data, msg->data_sz);
// 	}
// 	send(c->sock, buff, msg_size, 0);
// 	return 0;
// }