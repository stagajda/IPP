#ifndef history_tree_h
#define history_tree_h
#include <stdint.h>

void add_new_history(my_string *);

void remove_history(my_string *);

int valid_history(my_string *);

uint64_t get_history_energy(my_string *);

int set_history_energy(my_string *, uint64_t);

int equalize_histories (my_string *, my_string *);

void init_structure();

void free_structure();
#endif