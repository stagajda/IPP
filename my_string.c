#include <stdlib.h>
#include "my_string.h"
#include <stdio.h>

my_string * new_my_string(){
	my_string * new_string = malloc(sizeof(my_string));
	new_string->characters = malloc(sizeof(char)*10);
	new_string->length = 0;
	new_string->array_size = 10;
	return new_string;
}


void push_back(my_string * string, char litera){
	string->length++;
	if((string->length)>(string->array_size)){
		string->characters = realloc(string->characters, 2*(string->array_size));
		string->array_size *= 2;
	}
	string->characters[string->length-1] = litera;
}


int are_equal(my_string * string1, my_string * string2){
	if(string1->length != string2->length){
		return 0;
	}
	for(int i = 0; i < string1->length;i++){
		if(string1->characters[i] != string2->characters[i])
			return 0;
	}
	return 1;
}

int is_empty(my_string * string){
	if (string->length == 0)
		return 1;
	else
		return 0;
}

void free_my_string(my_string * string){
	free(string->characters);
	free(string);
}
