#include "my_string.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>
#define TYP_ERROR 0
#define TYP_DECLARE 1
#define TYP_REMOVE 2
#define TYP_VALID 3
#define TYP_ENERGY 4
#define TYP_EQUAL 5
#define TYP_EOF 6
#define TYP_IGNORE 7


bool is_declare(my_string * command){
	if(command->length != 7)
		return false;

	char arr_declare[7] = {'D', 'E', 'C', 'L', 'A', 'R', 'E'};
	for(int i = 0;i < 7;i++){
		if(command->characters[i] != arr_declare[i]){
			return false;
		}
	}
	return true;
}

bool is_remove(my_string * command){
	if(command->length != 6)
		return false;

	char arr_remove[6] = {'R', 'E', 'M', 'O', 'V', 'E'};
	for(int i = 0;i < 6;i++){
		if(command->characters[i] != arr_remove[i]){
			return false;
		}
	}
	return true;
}

bool is_valid(my_string * command){
	if(command->length != 5)
		return false;

	char arr_valid[5] = {'V', 'A', 'L', 'I', 'D'};
	for(int i = 0;i < 5;i++){
		if(command->characters[i] != arr_valid[i]){
			return false;
		}
	}
	return true;
}

bool is_energy(my_string * command){
	if(command->length != 6)
		return false;

	char arr_energy[6] = {'E', 'N', 'E', 'R', 'G', 'Y'};
	for(int i = 0;i < 6;i++){
		if(command->characters[i] != arr_energy[i]){
			return false;
		}
	}
	return true;
}

bool is_equal(my_string * command){
	if(command->length != 5)
		return false;

	char arr_equal[5] = {'E', 'Q', 'U', 'A', 'L'};
	for(int i = 0;i < 5;i++){
		if(command->characters[i] != arr_equal[i]){
			return false;
		}
	}
	return true;
}

int recognize_command(my_string * command){
	if(is_remove(command))
		return TYP_REMOVE;
	if(is_declare(command))
		return TYP_DECLARE;
	if(is_valid(command))
		return TYP_VALID;
	if(is_energy(command))
		return TYP_ENERGY;
	if(is_equal(command))
		return TYP_EQUAL;
	return TYP_ERROR;
}
// returns -1 if c is not a digit, otherwise returns digit in int format
int char_to_digit(char c){
	return c - '0';
}

bool is_legal_quant_history(my_string * history){
	if(history->length == 0)
		return false;

	for(int i = 0;i < history->length;i++){
		if(char_to_digit(history->characters[i]) < 0 || char_to_digit(history->characters[i]) > 3){
			return false;
		}
	}
	return true;
}

// converts my_string to uint64_t format
uint64_t my_string_to_uint64(my_string * num){
	uint64_t wynik = 0;
	for(int i = 0;i < num->length;i++){
		wynik *= 10;
		wynik += char_to_digit(num->characters[i]);
	}
	return wynik;
}

// function comparing 20 decimal digits numbers with 2^64-1
// returns 1 if num is higher, 0 otherwise
bool is_higher_than_max_int(my_string * num){
	// winner is equal to 1, when considering only last i digits num is > max_int
	// 0 otherwise (<=)
	int winner = 0;
	uint64_t max_int = UINTMAX_MAX;
	for(int i = 19; i >= 0; i--){
		if((uint64_t) char_to_digit(num->characters[i]) > max_int % 10){
			winner = 1;
		}
		else{
			if((uint64_t) char_to_digit(num->characters[i]) < max_int % 10){
				winner = 0;
			}
		}
		max_int = max_int/10;
	}
	return winner;
}

// returns 0 if energy number is too high or contains characters other than number,
// otherwise returns int_64 representing number of energy 
uint64_t is_legal_energy_number(my_string * energy){
	if(energy->length > 20)
		return 0;
	for(int i = 0;i < energy->length;i++){
		int digit = char_to_digit(energy->characters[i]);
		if(digit < 0 || digit > 9)
			return 0;
	}
	// check if it's higher than 2^64-1
	if(energy->length == 20){
		if(is_higher_than_max_int(energy)){
			return 0;
		}
	}
	return my_string_to_uint64(energy);
}

