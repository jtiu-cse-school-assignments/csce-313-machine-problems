#include<stdlib.h>
#include "free_list.h"

void FL_remove(FL_HEADER * free_list, FL_HEADER * block) {

	if(block->next != NULL || block->prev != NULL) {
		block->prev->next = block->next;
		block->next->prev = block->prev;
	}
	
}

void FL_add(FL_HEADER ** free_list, FL_HEADER * block) {

	block->next = (*free_list);
	(*free_list)->prev = block;
	(*free_list) = block;
	
}