#ifndef my_string_h
#define my_string_h
#include <stdlib.h>

struct my_str{
	char * characters;
	int length;
	int array_size; 
};

typedef struct my_str my_string;

my_string * new_my_string();

// dodaj litere na koniec
void push_back(my_string *, char);

// zwraca 0 gdy slowa sa rozne, >0 gdy sa rowne
int are_equal(my_string *, my_string *);

// zwraca 0 gdy niepusty, 1 gdy pusty
int is_empty(my_string *);

void wypisz(my_string *);

void free_my_string(my_string *);
#endif