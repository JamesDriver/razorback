#ifndef ERR_H
#define ERR_H

enum { //status codes
	SUCCESS,
	NULL_CONN,

	NULL_MSG, //msg errors
	INVALID_MSG_TYPE,
	INVALID_MSG_MODIFIER,
	INVALID_MSG_SZ,
	INVALID_MSG_DATA,

	NULL_CMD, //cmd errs
	NULL_CMD_EXEC,
};

#endif