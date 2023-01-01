#ifndef WORD_ARR
#define WORD_ARR

#include <stdio.h>
#include <stdbool.h>
enum {
	WORDS_CAP = 8,
};

typedef struct word_arr_ {
	char **cmd_names;
	size_t idx;
	size_t cap;
} word_arr;

word_arr *words_init();

bool words_append(word_arr *w, char *word);

#endif