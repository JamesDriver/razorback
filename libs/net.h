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
#include "err.h"

enum { //message types
	OPEN,
	CLOSE,
	SEND,
	RESPONSE,
	ERR,
};

enum { //sizes
	INT_M_STATIC_LEN = 8, //static size of int_msg
	FWD_D_STATIC_LEN = 8, //static size of forward_data
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
	void *data; //data
} int_msg; //internal message between lp and middleman

typedef struct {
	uint16_t port;
	uint16_t ip_sz;
	uint16_t data_sz;
	uint16_t reserved; //reserved for future use
	char *ip;
	char *data;
} fwd_data;

conn *conn_init(char *host, int port);

#endif