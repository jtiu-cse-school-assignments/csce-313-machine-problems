/* 
    File: my_allocator.c

    Author: <Julian Tiu>
            Department of Computer Science
            Texas A&M University
    Date  : <date>

    Modified: 

    This file contains the implementation of the module "MY_ALLOCATOR".

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include<stdlib.h>
#include<stdio.h>

#include "my_allocator.h"
#include "free_list.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/

/* Don't forget to implement "init_allocator" and "release_allocator"! */

char *start;
FL_HEADER * startOfFL;

unsigned int init_allocator(unsigned int _basic_block_size, unsigned int _length) {
    
    start = (char *)malloc(_length);
    if(start == 0) {
        return 0;
    }
    
    startOfFL = (FL_HEADER *)start;
    startOfFL->length = _length;
    startOfFL->next = NULL;
    startOfFL->prev = NULL;
    
    return _length;
}

int release_allocator() {
    free(startOfFL);
    return 0;
}

Addr my_malloc(size_t _length) {
  /* This preliminary implementation simply hands the call over the 
     the C standard library! 
     Of course this needs to be replaced by your implementation.*/
    
    FL_HEADER *current;
    FL_HEADER *newHeader;
    current = startOfFL;
	
    if(current->next == NULL) {
		if((current->length)-sizeof(FL_HEADER) > _length) {
			FL_remove(startOfFL, current);
			newHeader = (FL_HEADER *)((char *)(current) + sizeof(FL_HEADER) + _length);
			newHeader->length = current->length - (sizeof(FL_HEADER) + _length);
			FL_add(&startOfFL, newHeader);
			return (char *)(current) + sizeof(FL_HEADER);
		}
		else if((current->length)-sizeof(FL_HEADER) == _length) {
			FL_remove(startOfFL, current);
			return (char *)(current) + sizeof(FL_HEADER);
		}
    }
    else {
		while(current->next != NULL) {
			if((current->length)-sizeof(FL_HEADER) > _length) {
				//FL_remove(startOfFL, current);
				newHeader = (FL_HEADER *)((char *)(current) + sizeof(FL_HEADER) + _length);
				newHeader->length = current->length - (sizeof(FL_HEADER) + _length);
				FL_add(&startOfFL, newHeader);
				return (char *)(current) + sizeof(FL_HEADER);
			}
			else if((current->length)-sizeof(FL_HEADER) == _length) {
				FL_remove(startOfFL, current);
				return (char *)(current) + sizeof(FL_HEADER);
			}
		
			current = current->next;
		}
    }
	
	return NULL;
}

int my_free(Addr _a) {
  /* Same here! */
  
  FL_HEADER *headerToAddBack;
  
  headerToAddBack = (FL_HEADER *)((char *)(_a) - sizeof(FL_HEADER));
  FL_add(&startOfFL, headerToAddBack);
  
  return 0;
}