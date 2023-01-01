//autocomplete tree struct

#ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

#include <stdlib.h>
#include "word_arr.h"

typedef struct {
} w_node;

typedef struct {
	size_t sz;
	w_node *root;
} a_comp_tr;

a_comp_tr *autocomplete_init(); //initialize

word_arr *autocomplete(char *buff);

#endif