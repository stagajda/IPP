#ifndef input_read_h
#define input_read_h
#include "my_string.h"
#include <stdio.h>
#include <inttypes.h>
#define TYP_ERROR 0
#define TYP_DECLARE 1
#define TYP_REMOVE 2
#define TYP_VALID 3
#define TYP_ENERGY 4
#define TYP_EQUAL 5
#define TYP_EOF 6
#define TYP_IGNORE 7

struct command_line{
	my_string * command;
	my_string * arg1;
	my_string * arg2;
	int typ_polecenia;
	int is_correct;
	uint64_t energy_number;
};

typedef struct command_line command_line;

command_line * przetworz_linie_tekstu();

void free_command_line(command_line *);

#endif