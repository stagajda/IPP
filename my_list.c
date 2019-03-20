#include "my_list.h"

history_list * new_list(int val){
	history_list * list = (history_list *) malloc(sizeof(history_list));
	list->next = (history_list *) NULL;
	list->last = list;
	list->value = val;
	//free(val);
	return list;
}

history_list * add_on_end(history_list * list, int val){
	history_list * new_last = new_list(val);
	(list->last)->next = new_last;
	list->last = new_last;
	return list;
}

history_list * pop_first(history_list * list){
	if(list->next == NULL){
		free(list);
		return NULL;
	}
	(list->next)->last = list->last;
	history_list * result_list = list->next;
	free(list);
	return result_list; 
}

