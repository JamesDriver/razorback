#ifndef FUNC_H
#define FUNC_H

#include "err.h"
#include "net.h"
#include <stdio.h>
#include <unistd.h>

int disp_help(int argc, char **argv);

int list_implants(int argc, char **argv);

int use_implant(int argc, char **argv); //implant use <id/name>

int name_implant(int argc, char **argv); //implant name <id> <new_name> & rename

int server_listen(int argc, char **argv); //both listen and listen -d

int portfwd(int argc, char **argv); //forward and forward -r (remove)

int exec(int argc, char **argv); //exec cmd on implant

static int (*funcs[])(int argc, char **argv) = { &disp_help, &list_implants,
						 &use_implant, &portfwd,
						 &server_listen };
static char *func_names[] = { "help", "list", "use", "portfwd", "listen" };
static int func_ct = sizeof(func_names) / sizeof(func_names[0]);

#endif