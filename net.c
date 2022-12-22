#include "net.h"

conn *conn_init(char *host, int port) {
	int sock = 0, conn_fd;
	struct sockaddr_in conn_addr;

	conn_addr.sin_family = AF_INET;
	conn_addr.sin_port = htons(port);

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return NULL;
	}

	if (inet_pton(conn_addr.sin_family, host, &conn_addr.sin_addr) <= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return NULL;
	}

	if ((conn_fd = connect(sock, (struct sockaddr *)&conn_addr,
			       sizeof(conn_addr))) < 0) {
		printf("\nConnection Failed \n");
		return NULL;
	}
	conn *c = malloc(sizeof(*c));
	char *tmp_host = malloc(strlen(host));
	strncpy(tmp_host, host, strlen(host));
	c->host = host;
	c->sock = sock;
	return c;
}