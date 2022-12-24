#ifndef FUNC_H
#define FUNC_H

int list_implants(int argc, char **argv);

int use_implant(int argc, char **argv); //implant use <id/name>

int name_implant(int argc, char **argv); //implant name <id> <new_name> & rename

int server_listen(int argc, char **argv); //both listen and listen -d

int forward(int argc, char **argv); //forward and forward -r (remove)

int exec(int argc, char **argv); //exec cmd on implant

#endif