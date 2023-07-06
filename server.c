#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <dirent.h>
#include <errno.h>

typedef int (*ModuleFunc)(int argc, char **argv);
typedef struct {
	char *name;
	char *description;
	ModuleFunc func;
} Function;

typedef struct {
	Function **f;
	size_t idx;
	size_t cap;
} Functions;

typedef struct {
	void **data;
	size_t idx;
	size_t cap;
} obj_handles;

// Check if ext is correct for module
// TODO: check magic number instead (in addition to?) ext
int is_shared_obj(const char *filename)
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

int load_module(obj_handles *handles, const char *path)
{
	if (!handles || !path) {
		return -1;
	}
	if (!handles->data) {
		// handle should be initialized prior to loading
		return -1;
	}
	handles->data[handles->idx] = dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
	if (!handles->data[handles->idx]) {
		/* fail to load the library */
		fprintf(stderr, "Error for file %s: %s\n", path, dlerror());
		return -1;
	}
	handles->idx++;
	if (handles->idx >= handles->cap) {
		void **tmp =
			realloc(handles->data, sizeof(*tmp) * handles->cap * 2);
		if (!tmp) {
			return ENOMEM;
		}
	}
	return EXIT_SUCCESS;
}

int load_modules(obj_handles *handles, const char *mod_dir)
{
	if (!handles || !mod_dir) {
		return -1;
	}

	DIR *d;
	const struct dirent *dir;

	d = opendir(mod_dir);
	if (!d) {
		return ENOENT;
	}

	while ((dir = readdir(d)) != NULL) {
		if (is_shared_obj(dir->d_name) != 0) {
			continue;
		}
		char *fpath = calloc(1, 255);
		strcat(fpath, mod_dir);
		strcat(fpath, dir->d_name);
		load_module(handles, fpath); // TODO: error handling
		free(fpath);
	}
	closedir(d);
	return EXIT_SUCCESS;
}

obj_handles *init_handles()
{
	obj_handles *handles = malloc(sizeof(*handles));
	if (!handles) {
		return NULL;
	}
	handles->idx  = 0;
	handles->cap  = 8;
	handles->data = malloc(sizeof(*handles->data) * handles->cap);
	if (!handles->data) {
		free(handles);
		return NULL;
	}
	return handles;
}

void destroy_handles(obj_handles *handles)
{
	if (!handles) {
		return;
	}
	if (handles->data) {
		for (size_t i = 0; i < handles->idx; i++) {
			dlclose(handles->data[i]);
		}
	}
	free(handles->data);
	free(handles);
	return;
}

int main(void)
{
	obj_handles *handles = init_handles();
	const char *mod_dir  = "/home/james/dev/razorback/modules/";
	int err		     = load_modules(handles, mod_dir);
	if (err != 0) {
		goto cleanup;
	}

	int (*funcs[2])(const char *);

	for (size_t i = 0; i < handles->idx; i++) {
		*(void **)(&funcs[i]) = dlsym(handles->data[i], "test_func");
		if (!funcs[i]) {
			/* no such symbol */
			fprintf(stderr, "Error: %s\n", dlerror());
			goto cleanup;
		}
	}
cleanup:
	destroy_handles(handles);
	return 0;
}
