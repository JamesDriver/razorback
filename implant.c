#include "libs/net.h"

void error(char *msg) {
	perror(msg);
	exit(0);
}

static void print_hex(void *data, int num_bytes) {
	uint8_t *bits = data;
	for (int i = 0; i < num_bytes; i++) {
		printf("%x ", bits[i]);
	}
}

void print_msg(int_msg *msg) {
	printf("type: %d\n", msg->type);
	printf("modifier: %d\n", msg->modifier);
	printf("data_sz: %d\n", msg->data_sz);
	printf("data: %s\n", msg->data);
}

fwd_data *parse_send(char *data) {
	if (!data) {
		return NULL;
	}

	fwd_data *f = malloc(sizeof(*f));
	if (!f) {
		return NULL;
	}
	memcpy(f, data, FWD_D_STATIC_LEN);

	char *ip = malloc(f->ip_sz);
	if (!ip) {
		free(f);
		return NULL;
	}

	char *tmp_data = malloc(f->data_sz);
	if (!tmp_data) {
		free(f);
		free(ip);
		return NULL;
	}

	memcpy(ip, data + FWD_D_STATIC_LEN, f->ip_sz);
	memcpy(tmp_data, data + FWD_D_STATIC_LEN + f->ip_sz, f->data_sz);
	f->ip = ip;
	f->data = tmp_data;
	return f;
}

int forward_data(char *data) {
	fwd_data *fwd = parse_send(data);
	conn *c = conn_init(fwd->ip, fwd->port);
	send(c->sock, fwd->data, fwd->data_sz, 0);
	return 0;
}

void parse_msg(char *buff) {
	int msg_sz = sizeof(int_msg) - sizeof(void *);
	int_msg *msg = malloc(msg_sz);
	memcpy(msg, buff, msg_sz);
	char *data = malloc(msg->data_sz);
	memcpy(data, buff + msg_sz, msg->data_sz);
	msg->data = data;
	if (msg->type == SEND) {
		forward_data(msg->data);
	}
	print_msg(msg);
}

int main(int argc, char *argv[]) {
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	char *hello = "Hello from server";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
		       sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(8001);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
				 (socklen_t *)&addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	valread = read(new_socket, buffer, 1024);
	parse_msg(buffer);
	// closing the connected socket
	close(new_socket);
	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}