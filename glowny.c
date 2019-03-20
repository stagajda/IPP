#include <stdio.h>
#include <stdlib.h>
#include "my_string.h"
#include "input_read.h"
#include "history_tree.h"
#include <inttypes.h>
#define TYP_ERROR 0
#define TYP_DECLARE 1
#define TYP_REMOVE 2
#define TYP_VALID 3
#define TYP_ENERGY 4
#define TYP_EQUAL 5
#define TYP_EOF 6
#define TYP_IGNORE 7

int main(){
	init_structure();
	while(1){
		command_line * new_line = przetworz_linie_tekstu();
		if(new_line->typ_polecenia == TYP_ERROR){
			fprintf( stderr, "ERROR\n");
		}

		if(new_line->typ_polecenia == TYP_DECLARE){
			add_new_history(new_line->arg1);
			printf("OK\n");
		}

		if(new_line->typ_polecenia == TYP_REMOVE){
			remove_history(new_line->arg1);
			printf("OK\n");
		}

		if(new_line->typ_polecenia == TYP_VALID){
			if(valid_history(new_line->arg1))
				printf("YES\n");
			else
				printf("NO\n");
		}

		if(new_line->typ_polecenia == TYP_ENERGY){
			if(new_line->arg2->length == 0){
				uint64_t energy = get_history_energy(new_line->arg1);
				if(energy == 0)
					fprintf(stderr, "ERROR\n");
				else
					printf("%" PRIu64 "\n", energy);
			}
			else{
				if(set_history_energy(new_line->arg1, new_line->energy_number))
					printf("OK\n");
				else
					fprintf(stderr, "ERROR\n");
			}
		}

		if(new_line->typ_polecenia == TYP_EQUAL){
			if(equalize_histories(new_line->arg1, new_line->arg2))
				printf("OK\n");
			else
				fprintf(stderr, "ERROR\n");
		}
		if(new_line->typ_polecenia == TYP_EOF){
			free_command_line(new_line);
			free_structure();
			break;
		}
		free_command_line(new_line);
	}
	
}

