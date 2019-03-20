#include "my_string.h"
#include "find_and_union.h"

// gets digit 0...9 in char format, returns the same digit in int format
int digit_to_int(char c){
	return c-'0';
}

typedef struct tree tree;

struct tree{
	// odpowiadajacy temu wierzcholkowi drzewa wezel w strukturze F&U
	element * relation_set;
	// wskazniki na poddrzewa 
	tree * subtree[4];
};

tree * new_tree(){
	tree * new_tree = (tree *) malloc(sizeof(tree));
	for(int i = 0;i < 4;i++){
		new_tree->subtree[i] = NULL;
	}
	new_tree->relation_set = new_relation_set();
	return new_tree;
}

tree * main_tree;
void init_structure(){
	main_tree = new_tree();
}

tree * history_to_tree(my_string * history){
	tree * actual_tree = main_tree;
	for(int i = 0;i<history->length;i++){
		int num = digit_to_int(history->characters[i]);
		if(actual_tree->subtree[num] == NULL){
			return NULL;
		}
		actual_tree = actual_tree->subtree[num];
	}
	return actual_tree;
}

void add_new_history(my_string * history){
	tree * actual_tree = main_tree;
	for(int i = 0;i < history->length;i++){
		int num = digit_to_int(history->characters[i]);
		if(actual_tree->subtree[num] == NULL){
			actual_tree->subtree[num] = new_tree();
		}
		actual_tree = actual_tree->subtree[num];
	}
}

// frees memory
void delete_subtree(tree * del_tree){
	// TU TRZEBA DODAC PROBE ZWOLNIENIA PAMIECI NA RELATION SET
	for(int i = 0;i < 4;i++){
		if(del_tree->subtree[i] != NULL)
			delete_subtree(del_tree->subtree[i]);
	}
	delete_node(del_tree->relation_set);
	free(del_tree);
}

// idziemy do przedostatniego znaku historii, a potem wszystko usuwamy
void remove_history(my_string * history){
	tree * actual_tree = main_tree;
	for(int i = 0; i < history->length-1;i++){
		int num = digit_to_int(history->characters[i]);
		if(actual_tree->subtree[num] == NULL)
			return;
		else
			actual_tree = actual_tree->subtree[num];
	}
	int num = digit_to_int(history->characters[history->length-1]);
	if(actual_tree->subtree[num] == NULL)
		return;
	delete_subtree(actual_tree->subtree[num]);
	actual_tree->subtree[num] = NULL;
}

// returns 1 if history is valid, 0 otherwise
int valid_history(my_string * history){
	tree * actual_tree = main_tree;
	for(int i = 0;i < history->length;i++){
		int num = digit_to_int(history->characters[i]);
		if(actual_tree->subtree[num] != NULL){
			actual_tree = actual_tree->subtree[num];
		}
		else{
			return 0;
		}
	}
	return 1;
}

uint64_t get_history_energy(my_string * history){
	tree * history_tree = history_to_tree(history);
	if(history_tree == NULL)
		return 0;
	return get_energy(history_tree->relation_set);
}

// returns 1 if attribiuton was succed
// returns 0 otherwise
int set_history_energy(my_string * history, uint64_t energy_number){
	tree * history_tree = history_to_tree(history);
	if(history_tree == NULL)
		return 0;
	set_energy(history_tree->relation_set, energy_number);
	return 1;
}

// union relation sets of two histories
// returns 1 if everything is fine
// 0 in case of ERROR
int equalize_histories (my_string * history1, my_string * history2){
	if(are_equal(history1, history2))
		return 1;
	tree * history_tree1 = history_to_tree(history1);
	tree * history_tree2 = history_to_tree(history2);
	if(history_tree1 == NULL || history_tree2 == NULL)
		return 0;
	return union_sets(history_tree1->relation_set, history_tree2->relation_set);
}

void free_structure(){
	delete_subtree(main_tree);	
}