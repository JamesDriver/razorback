// #include "libs/shell.h"

#include <ncurses.h> /* ncurses.h includes stdio.h */
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <dirent.h>

typedef int (*ModuleFunc)(int argc, char **argv);

/*
// MAYBE?
struct Function {
	char *name;
	char *description;
	ModuleFunc func;
}

*/

char *getExt(const char *filename)
{
	char *e = strrchr(filename, '.');
	return e;
}

int is_so(const char *filename)
{
	if (!filename) {
		return -1;
	}
	const char *ext = getExt(filename);
	if (!ext) {
		return -1;
	}
	return strcmp(ext, ".so");
}

// int load_modules(void *handles, size_t *handle_ct, size_t *handle_cap)
// {
// 	// TODO: realloc
// 	if (handles) {
// 		return -1; // handles must be null (overwrites contents)
// 	}
// 	*handle_ct  = 0;
// 	*handle_cap = 8;
// 	handles	    = malloc(sizeof(handles) * *handle_cap);
// 	if (!handles) {
// 		return -1;
// 	}
// 	DIR *d;
// 	struct dirent *dir;
// 	const char path[] = "/home/james/dev/razorback/modules/";
// 	// TODO: move to relative path
// 	d = opendir(path);
// 	if (d) {
// 		while ((dir = readdir(d)) != NULL) {
// 			// load all .so files in the modules directory
// 			if (is_so(dir->d_name) != 0) {
// 				continue;
// 			}
// 			// TODO: check type of entry dir->d_type
// 			char *buff = calloc(1, 255);
// 			strcat(buff, path);
// 			strcat(buff, dir->d_name);
// 			handles[0]	    = dlopen(buff, RTLD_LAZY);
// 			handles[*handle_ct] = dlopen(buff, RTLD_LAZY);
// 			if (!handles[*handle_ct]) {
// 				/* fail to load the library */
// 				fprintf(stderr, "Error for %s: %s\n", buff,
// 					dlerror());
// 				return EXIT_FAILURE;
// 			}
// 			(*handle_ct)++;
// 			printf("%s successfully loaded\n", buff);
// 		}
// 		closedir(d);
// 	} else {
// 		printf("Unable to locate modules directory\n");
// 	}
// }

int main(void)
{
	void *handles = malloc(sizeof(void *) * 10);
	handles[0]    = dlopen("test", RTLD_LAZY);
	size_t handle_ct;
	// size_t handle_cap;

	// ModuleFunc *funcs[2];
	// size_t func_ct;
	// size_t func_cap;

	// int err = load_modules(&handles, &handle_ct, &handle_cap);
	// if (err != 0) {
	// 	fprintf(stderr, "err\n");
	// 	return -1;
	// }

	// for (int i = 0; i < 2; i++) {
	// 	*(void **)(&funcs[i]) = dlsym(handles[i], "test_func");
	// 	if (!funcs[i]) {
	// 		/* no such symbol */
	// 		fprintf(stderr, "Error: %s\n", dlerror());
	// 		dlclose(handles[i]);
	// 		return EXIT_FAILURE;
	// 	}
	// 	funcs[i]("tree");
	// 	dlclose(handles[i]);
	// }

	return 0;
}
// int main()
// {

// 	// startup();
// 	// shutdown();
// 	// return 0;
// }
// int main()
// {
// 	//eventually add some startup scripts for re-opening listeners or
// something
// 	//load_conf();
// 	shell_init();
// 	return 0;
// }

// typedef struct ll_node_ {
// 	char *cmd;
// 	struct ll_node_ *prev;
// } ll_node;

// void app_loop(void)
// {
// 	// position instantiations
// 	//  NOTE: not resize safe currently. to make resize safe, add logic in
// 	//  loop.
// 	int row, col, max_x, max_y;

// 	bool tab_complete_staging = false; // set to true if tab pressed once
// 					   // (twice results in autocompletion)
// 	char *curr_w_buf = calloc(sizeof(char), 16); // TODO: ABC // current
// 						     // word buffer
// 	ll_node *prev_cmd = NULL;

// 	// prompt setup
// 	getmaxyx(stdscr, max_x, max_y);
// 	mvprintw(max_x - 1, 1, ">");
// 	getyx(stdscr, row, col);

// 	// main cli loop
// 	while (1) {
// 		// TODO: arrow keys, history
// 		// line wrapping
// 		// command auto complete
// 		char inp = getch();
// 		if (inp == EOF || inp == 3 || inp == 4) {
// 			return;
// 		} else if (inp == '\t') {
// 			if (tab_complete_staging) {
// 				getyx(stdscr, row, col);
// 				mvprintw(row + 1, 0, "%s", curr_w_buf);
// 				move(row, col);
// 			} else {
// 				tab_complete_staging = true;
// 				continue;
// 			}
// 		} else if (inp == 7) {
// 			// mvprintw(0, 0, "%d", col);
// 			if (col <= 2) {
// 				continue;
// 			}
// 			col -= 1;
// 			mvprintw(row, col, " ");
// 			move(row, col);
// 			if (col > 0) {
// 				curr_w_buf[col - 1] = 0;
// 			}
// 		} else if (inp == 10) {
// 			// send here
// 		} else {
// 			mvprintw(row, col, "%c", inp);
// 			if (col > 0) {
// 				curr_w_buf[col - 1] = inp;
// 			}
// 			col += 1;
// 		}
// 		tab_complete_staging = false;
// 	}
// }

// void startup(void)
// {
// 	char mesg[] = "Welcome to Razorback"; /* message to be appeared on the
// 					       * screen
// 					       */
// 	int row, col; /* to store the number of rows and *
// 		       * the number of colums of the screen */
// 	initscr(); /* start the curses mode */
// 	raw();
// 	keypad(stdscr, TRUE); // We get special keys
// 	noecho();
// 	getmaxyx(stdscr, row, col); /* get the number of rows and columns */
// 	mvprintw(0, (col - strlen(mesg)) / 2, "%s", mesg); /* print the
// 								    message at
// 								    the center
// 								    of the
// 								    screen */
// 	app_loop();
// }

// void shutdown(void)
// {
// 	refresh();
// 	endwin();
// }