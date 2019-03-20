#include <stdio.h>
#include "input_read.h"
#include "my_string.h"
#include "command_recognition.h"


command_line * new_command_line(){
	command_line * nowy = malloc(sizeof(command_line));
	nowy->command = new_my_string();
	nowy->arg1 = new_my_string();
	nowy->arg2 = new_my_string();
	nowy->is_correct = 1;
	return nowy;
}

void free_command_line(command_line * command_line){
	free_my_string(command_line->command);
	free_my_string(command_line->arg1);
	free_my_string(command_line->arg2);
	free(command_line);
}


my_string * wczytaj_linie_tekstu(){
	my_string * wynik = new_my_string();
	char znak = getchar();
	if(znak == '\n')
		return wynik;
	push_back(wynik, znak);
	if(znak == EOF)
		return wynik;
	while(1){
		znak = getchar();
		if(znak == '\n'|| znak == EOF)
			break;
		push_back(wynik, znak);
	}
	return wynik;
}
// sprawdza spracje w odpowiednich miejscach, sprawdza, czy sa conajwyzej 3 slowa w wierszu
// returns command_line with set command and args
command_line * rozbij_na_slowa(my_string * linia_tekstu){
	command_line * wynik = new_command_line();
	// jezeli jakies 2 spacje sasiaduja ze soba, lub spacja jest pierwszym znakiem w wierszu, to wyrzucamy blad
	if(linia_tekstu->characters[0] == ' '){
		wynik->is_correct = 0;
		return wynik;
	}
	for(int i = 1;i < linia_tekstu->length;i++){
		if(linia_tekstu->characters[i] == ' ' && linia_tekstu->characters[i-1] == ' '){
			wynik->is_correct = 0;
			return wynik;
		}
	}
	
	// there are 2 or 1 space in correct input line
	int space1 = -1;
	int space2 = -1;
	for(int i = 0;i < linia_tekstu->length;i++){
		if(linia_tekstu->characters[i] == ' '){
			space1 = i;
			break;
		}
	}
	if(space1 == -1){
		wynik->is_correct = 0;
		return wynik;
	}

	for(int i = space1+1;i < linia_tekstu->length;i++){
		if(linia_tekstu->characters[i] == ' '){
			space2 = i;
			break;
		}
	}
	if(space2 == -1){
		for(int i = 0;i < space1;i++){
			push_back(wynik->command, linia_tekstu->characters[i]);
		}
		for(int i = space1 + 1;i < linia_tekstu->length;i++){
			push_back(wynik->arg1, linia_tekstu->characters[i]);
		}
		return wynik;
	}
	if(space2 == linia_tekstu->length-1){
		wynik->is_correct = 0;
		return wynik;
	}
	for(int i = 0;i < space1;i++){
		push_back(wynik->command, linia_tekstu->characters[i]);
	}
	for(int i = space1 + 1;i < space2;i++){
		push_back(wynik->arg1, linia_tekstu->characters[i]);
	}
	for(int i = space2 + 1;i < linia_tekstu->length; i++){
		push_back(wynik->arg2, linia_tekstu->characters[i]);
	}
	return wynik;
}

// changes command_line is_correct status, when args don't have correct format
void do_args_match_command(command_line * command_line){
	int command_type = command_line->typ_polecenia;
	if(command_type == TYP_DECLARE || command_type == TYP_REMOVE || command_type == TYP_VALID){
		if(is_legal_quant_history(command_line->arg1) == 0 || !is_empty(command_line->arg2)){
			command_line->is_correct = 0;
		}
	}

	if(command_type == TYP_ENERGY){
		if(is_legal_quant_history(command_line->arg1)){
			uint64_t energy_number = is_legal_energy_number(command_line->arg2);
			if(is_empty(command_line->arg2) || energy_number){
				command_line->energy_number = energy_number;
				return;
			}
		}
		command_line->is_correct = 0;
	}

	if(command_type == TYP_EQUAL){
		if(is_legal_quant_history(command_line->arg1) && is_legal_quant_history(command_line->arg2)){
			return;
		}
		else{
			command_line->is_correct = 0;
		}
	}
}


// zwraca przetworzona linie w formie typu line_type
command_line * przetworz_linie_tekstu(){
	my_string * linia = wczytaj_linie_tekstu();
	if(linia->length == 0){
		command_line * wynik = new_command_line();
		wynik->typ_polecenia = TYP_IGNORE;
		free_my_string(linia);
		return wynik;
	}
	if(linia->characters[0] == EOF){
		command_line * wynik = new_command_line();
		wynik->typ_polecenia = TYP_EOF;
		free_my_string(linia);
		return wynik;
	}
	
	if(linia->characters[0] == '#'){
		command_line * wynik = new_command_line();
		wynik->typ_polecenia = TYP_IGNORE;
		free_my_string(linia);
		return wynik;
	}
	
	if(linia->characters[0] == ' '){
		command_line * wynik = new_command_line();
		wynik->typ_polecenia = TYP_ERROR;
		free_my_string(linia);
		return wynik;
	}
	command_line * wynik = rozbij_na_slowa(linia);
	//printf("%d\n",wynik->is_correct);
	wynik->typ_polecenia = recognize_command(wynik->command);
	do_args_match_command(wynik);
	if(wynik->is_correct == 0){
		wynik->typ_polecenia = TYP_ERROR;
	}
	free_my_string(linia);
	return wynik;
}

