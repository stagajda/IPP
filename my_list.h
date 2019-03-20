#ifndef my_list_h
#define my_list_h
#include <stdlib.h>

struct my_list{
	struct my_list * next;
	struct my_list * last;
	int value;
};

// typ list jednokierunkowych reprezentujacych historie kwantowe
typedef struct my_list history_list;

// zwraca wskaznik na jednoelementowa liste
history_list * new_list(int);

// zwraca wskaznik na liste przedluzona o jeden element
history_list * add_on_end(history_list *, int);

// zwraca wskaznik na liste bez pierwszego elementu
history_list * pop_first(history_list *);

#endif