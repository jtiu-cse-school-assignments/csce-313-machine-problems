#include<stdlib.h>
#include<stdio.h>
#include "free_list.h"

FL_HEADER * FL_remove(FL_HEADER * free_list, FL_HEADER * block) {

	block->next->prev = block->prev;
	block->prev->next = block->next;
	
	return block;
	
}

void FL_add(FL_HEADER ** free_list, FL_HEADER * block) {
	
	if((*free_list)->next == NULL) {
		//printf("\nFL_add if triggered\n\n");
		(*free_list)->next = block;
		block->prev = (*free_list);
	}
	else if((*free_list)->next->next == NULL) {
		//printf("\nFL_add else if triggered\n\n");
		block->next = (*free_list)->next;
		(*free_list)->next->prev = block;
		block->prev = (*free_list);
		(*free_list)->next = block;
	}
	else {
		block->next = (*free_list)->next;
		(*free_list)->next->prev = block;
		block->prev = (*free_list);
		(*free_list)->next = block;
	}
	
}