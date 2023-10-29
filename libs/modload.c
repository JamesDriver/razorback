#include <string.h>
#include <dlfcn.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include "modload.h"

enum {
	F_START_SZ = 8,
};

typedef struct {
	size_t func_ct;
	size_t func_idx;
	int (**func)(int, char **);
	const char **name;
} funcs_t;

static funcs_t *funcs = NULL;
typedef int (*init_func_t)(void *);

static void build_path(char *out, const char *dir_path, const char *file_name);
static int funcs_init();
static int func_register(fnptr_t func, const char *name);
static int is_shared_obj(const char *filename);

fnptr_t get_func(const char *name)
{
	if (!funcs) {
		return NULL;
	}
	if (!name) {
		return NULL;
	}
	for (size_t i = 0; i < funcs->func_idx; i++) {
		if (strcmp(name, funcs->name[i]) == 0) {
			return funcs->func[i];
		}
	}
	return NULL;
}

int load_module(const char *path)
{
	if (!path) {
		return -1;
	}

	if (funcs_init() != 0) {
		return -1;
	}

	void *mod_handle = dlopen(path, RTLD_NOW);
	if (!mod_handle) {
		dlerror(); // DBG
		return -1;
	}

	init_func_t init_module_func = dlsym(mod_handle, "init_module");
	// can technically be NULL on success but only in weird cases. not gonna deal with those
	if (!init_module_func) {
		dlclose(mod_handle);
		dlerror(); // DBG
		return -1;
	}

	// call init module function with fp that adds funcs to global func list
	void *frptr = &func_register; // doing this for -werror. not gonna check if not null bc I said so
	if (init_module_func(frptr) != 0) {
		dlclose(mod_handle);
		dlerror(); // DBG
		return -1;
	}

	// TODO: do I need to dlclose here? idk should close when process exits
	// but also, func references should be counted so shouldn't be unloaded until
	//  they are removed from the global
	return EXIT_SUCCESS;
}

int load_modules(const char *dir_path)
{
	if (!dir_path)
		return ENOENT;

	if (funcs_init() != 0) {
		return -1;
	}

	DIR *d;
	const struct dirent *dir;
	int err = 0;

	d = opendir(dir_path);
	if (!d)
		return ENOENT;

	char *fpath = malloc(PATH_MAX);
	while ((dir = readdir(d)) != NULL) {
		memset(fpath, 0, PATH_MAX);
		if (is_shared_obj(dir->d_name) != 0) {
			continue;
		}
		build_path(fpath, dir_path, dir->d_name);
		err = load_module(fpath);
		if (err != 0) {
			fprintf(stderr, "Error for file %s: %s\n", fpath, dlerror());
		}
	}
	free(fpath);
	closedir(d);
	return EXIT_SUCCESS;
}

static int funcs_init()
{
	if (funcs) {
		return 0;
	}
	funcs = malloc(sizeof(funcs_t));
	if (!funcs)
		return -1;
	funcs->func = malloc(sizeof(funcs->func) * F_START_SZ);
	if (!funcs->func) {
		free(funcs);
		return -1;
	}
	funcs->name = malloc(sizeof(funcs->name) * F_START_SZ);
	if (!funcs->name) {
		free(funcs->func);
		free(funcs);
		return -1;
	}
	funcs->func_ct	= F_START_SZ;
	funcs->func_idx = 0;
	return 0;
}

static int func_register(fnptr_t func, const char *name)
{
	if (!funcs) {
		return -1;
	}
	if (!func || !name) {
		return -1;
	}
	if (get_func(name)) {
		return -2;
	}
	// TODO: realloc
	funcs->func[funcs->func_idx] = func;
	funcs->name[funcs->func_idx] = name;
	funcs->func_idx++;
	return 0;
}

static int is_shared_obj(const char *filename)
{
	if (!filename) {
		return -1;
	}
	const char *ext = strrchr(filename, '.');
	if (!ext) {
		return -1;
	}
	return strcmp(ext, ".so");
}

static void build_path(char *out, const char *dir_path, const char *file_name)
{
	strcat(out, dir_path);
	int len = strlen(out);
	if (out[len - 1] != '/')
		out[len] = '/';
	strcat(out, file_name);
}
