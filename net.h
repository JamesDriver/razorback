#ifndef NET_H
#define NET_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

enum { //status codes
	SUCCESS,
	NULL_CONN,
	NULL_MSG,
	INVALID_MSG_TYPE,
	INVALID_MSG_MODIFIER,
	INVALID_MSG_SZ,
	INVALID_MSG_DATA
};

enum { //message types
	OPEN,
	CLOSE,
	SEND,
	RESPONSE,
	ERR,
};

typedef struct {
	char *host;
	int port;
	int sock;
	int fd;
	int status;
	int id;
} conn;

typedef struct {
	uint16_t type; //type of internal message
	uint16_t modifier; //modifier for the type (conn id for instance)
	uint32_t data_sz; //size of data
	char *data; //data
} int_msg; //internal message between lp and middleman

conn *conn_init(char *host, int port);

#endif