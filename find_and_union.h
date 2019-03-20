#ifndef find_and_union_h
#define find_and_union_h
#include <stdbool.h>
#include <stdint.h>

struct elem{
	// wskaznik na jego reprezentanta
	struct elem * parent;
	// liczba i takich, ze ten element jest jego rodzicem, na poczatku on sam jest swoim rodzicem, wiec jeden
	int in_number;
	// jezeli ten element jest jej reprezentantem swojej klasy rownowaznosci,
	// to wartosc energii w tej klasie wynosi energy
	uint64_t energy;
	// zmienia sie na true, gdy zostanie usunieta historia, ktora on reprezentuje, wtedy w momencie, kiedy in_number=0,
	// lub jedyna wchodzaca to on sam zwalniamy przewidziana dla niego pamiec
	bool czy_usuwac;
};

typedef struct elem element;

// dodaje nowy zbior jednoelementowy, zwraca wskaznik na ten element
element * new_set();

// zmienia status danego wezla na czekajacy na usuniecie
void delete_node(element *);

// zmienia energie klasy rownowaznosci zawierajacej dany wezel na dana
void set_energy(element *, uint64_t);

// zwraca energie ktora ma klasa rownowaznosci, zawierajaca dany wezel, jezeli nie przypisano zadnej to 0
uint64_t get_energy(element *);

// laczy zbiory zawierajace elementy na ktore wskazuja el1 i el2
// if both have not energy returns 0
// otherwise it equals energies in relation set and returns 1
int union_sets(element *,element *);

element * new_relation_set();

#endif