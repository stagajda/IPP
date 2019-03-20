#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "find_and_union.h"
#include <inttypes.h>
#include <stdbool.h>

element * new_relation_set(){
	element * node = (element *) malloc(sizeof(element));
	node->parent = node;
	node->in_number = 1;
	node->energy = 0;
	node->czy_usuwac = false;
	return node;
}

void consider_deleting(element * node){
	if(node->czy_usuwac){
		if(node->in_number == 0){
			node->parent->in_number--;
			consider_deleting(node->parent);
			free(node);
		}
		else if(node->in_number == 1 && node->parent == node){
			 	free(node);
		}
	}
}

void delete_node(element * node){
	node->czy_usuwac = true;
	consider_deleting(node);	
}

// znajduje reprezentanta klasy rownowaznosci do ktorej nalezy node
element * find(element * node){
	if(node->parent == node){
		return node;
	}
	else{
		element * prev_parent = node->parent;
		(node->parent)->in_number--;
		node->parent = find(node->parent);
		(node->parent)->in_number++;
		consider_deleting(prev_parent);
		return node->parent;
	}
}

void set_energy(element * node, uint64_t val){
	find(node)->energy = val;
}

uint64_t get_energy(element * node){
	return find(node)->energy;
}

int union_sets(element * node1, element * node2){
	// rep1 i rep2 zawsze maja strzalke do siebie
	element * rep1 = find(node1);
	element * rep2 = find(node2);
	if(rep1->energy == 0 && rep2->energy == 0){
		// oba nie maja energii
		return 0;
	}
	else if(rep1->energy == 0){
		element * prev_parent = rep1->parent;
		rep1->parent = rep2;
		rep2->in_number++;
		prev_parent->in_number--;
		consider_deleting(prev_parent);
	} 
	else if(rep2->energy == 0){
		element * prev_parent = rep2->parent;
		rep2->parent = rep1;
		rep1->in_number++;
		prev_parent->in_number--;
		consider_deleting(prev_parent);
	}
	else{
		element * prev_parent = rep1->parent;
		rep1->parent = rep2;
		rep2->in_number++;
		prev_parent->in_number--;
		consider_deleting(prev_parent);
		uint64_t mod = rep1->energy%2 + rep2->energy%2;
		rep2->energy = (rep1->energy)/2+(rep2->energy)/2 + mod/2;
	}
	return 1;
}
