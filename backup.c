#include <stdio.h>
#include "input_read.h"
#include "my_string.h"
#include "command_check"

#define TYP_ERROR 0
#define TYP_DECLARE 1
#define TYP_REMOVE 2
#define TYP_VALID 3
#define TYP_ENERGY1 4
#define TYP_ENERGY2 5
#define TYP_EQUAL 6
#define TYP_EOF 7
#define TYP_IGNORE 8 // ok


line_type * new_line_type(){
	line_type * nowy = malloc(sizeof(line_type));
	nowy->slowo2 = new_my_string();
	nowy->slowo3 = new_my_string();
}

command_line * new_command_line(){
	command_line * nowy = malloc(sizeof(command_line));
	nowy->command = new_my_string();
	nowy->arg1 = new_my_string();
	nowy->arg2 = new_my_string();
	nowy->is_correct = 1;
	return nowy;
}

my_string * wczytaj_linie_tekstu(){
	my_string * wynik = new_my_string();
	char znak;
	while(1){
		znak = getchar();
		if(znak == '\n')
			break;
		dodaj_litere(wynik, znak);
	}
	return wynik;
}
// sprawdza spracje w odpowiednich miejscach, sprawdza, czy sa conajwyzej 3 slowa w wierszu
command_line * rozbij_na_slowa(my_string * linia_tekstu){
	command_line * wynik = new_command_line();
	// jezeli jakies 2 spacje sasiaduja ze soba, lub spacja jest pierwszym znakiem w wierszu, to wyrzucamy blad
	if(linia_tekstu->string_array[0] == ' '){
		wynik->is_correct = 0;
		return wynik;
	}
	for(int i = 1;i < linia_tekstu->dlugosc_slowa;i++){
		if(linia_tekstu->string_array[i] == ' ' && linia_tekstu->string_array[i-1] == ' '){
			wynik->is_correct = 0;
			return wynik;
		}
	}
	while(my_string->string_array[i]!= ' ' && i<linia_tekstu->dlugosc_slowa){
		dodaj_litere(wynik->command,linia_tekstu->string_array[i]);
		i++;
	}
	i++;
	while(my_string->string_array[i]!= ' ' && i<linia_tekstu->dlugosc_slowa){
		dodaj_litere(wynik->arg1,linia_tekstu->string_array[i]);
		i++;
	}
	i++;
	while(my_string->string_array[i]!= ' ' && i<linia_tekstu->dlugosc_slowa){
		dodaj_litere(wynik->arg1,linia_tekstu->string_array[i]);
		i++;
	}
	if(i < linia_tekstu->dlugosc_slowa){
		wynik->is_correct = 0;
	}
	return wynik;
}

// zwraca przetworzona linie w formie typu line_type
line_type * przetworz_linie_tesktu(){
	line_type * wynik = new_line_type();
	my_string * linia = wczytaj_linie_tekstu();
	if(linia->dlugosc_slowa == 0){
		wynik->typ_polecenia = TYP_IGNORE;
		return wynik;
	}
	if(linia->string_array[0] == '#'){
		wynik->typ_polecenia = TYP_IGNORE;
		return wynik;
	}
	if(linia->string_array[0] == ' '){
		wynik->typ_polecenia = TYP_ERROR;
		return wynik;
	}
	command_line * command_line = rozbij_na_slowa(linia);
	if(command_line->is_correct == 0){
		wynik->typ_polecenia = TYP_ERROR;
		return wynik;
	}
	my_string * command = command_line->command;
	wynik->typ_polecenia = check_type_of_command(command_line->command);
	wynik->slowo2 =
}


// bedzie dzielila wejsciowa linie na 2 slowa lub 3, jezeli bedzie mniej lub wiecej, to wyrzucamy blad

// probuj wczytac slowo

// przetworz linie

// 








#ifndef input_read_h
#define input_read_h
#include "my_string.h"
// polecenie niezgodne z formatem

/// REZYGNUJE Z TYPE_OF_LINE
struct type_of_line{
	
	// drugie slowo zawarte w danej liscie
	my_string * slowo2;
	my_string * slowo3;
};
typedef struct type_of_line line_type;

struct command_line{
	my_string * command;
	my_string * arg1;
	my_string * arg2;
	// liczba calkowita odpowiadajaca typowi polecenia:
	// TYP_ERROR 0
	// TYP_DECLARE 1
	// TYP_REMOVE 2
	// TYP_VALID 3
	// TYP_ENERGY1 4
	// TYP_ENERGY2 5
	// TYP_EQUAL 6
	// TYP_EOF 7
	// TYP_IGNORE 8
	int typ_polecenia; 
	int is_correct;
};
typedef struct command_line command_line;

line_type * new_line_type();

line_type * przetworz_linie_tesktu();

trzy_slowa * rozbij_na_slowa();

#endif