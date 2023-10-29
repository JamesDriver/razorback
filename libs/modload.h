#ifndef MODLOAD_STD
#define MODLOAD_STD

#include <stdlib.h>
#include "modtypes.h" //types

fnptr_t get_func(const char *name);

int load_modules(const char *dir_path);

int load_module(const char *path);

#endif