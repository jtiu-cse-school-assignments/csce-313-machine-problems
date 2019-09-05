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
#include<math.h>

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

FL_HEADER ** FL_Array;
FL_HEADER * nextInLine;
const int SIZE_OF_HEADER = sizeof(FL_HEADER);
int sizeOfHeaderArray;
char * start;

FL_HEADER * orignalHeaderFinder(FL_HEADER * a, FL_HEADER * b) {
	
	/* Finding the memory address of the original FL_HEADER
	*
	*	In this case "< 0" because if a appears first, then it must have a less value
	*	for its memory address
	*/
	if((char *)a - (char *)b < 0) return a;
	else return b;
	return a;
}

unsigned int init_allocator(unsigned int _basic_block_size, unsigned int _length) {
    
    FL_HEADER *longestFLH;
        
    sizeOfHeaderArray = log2(_length/_basic_block_size) + 1;
    FL_Array = (FL_HEADER **)malloc(SIZE_OF_HEADER * sizeOfHeaderArray);
    
    start = (char *)malloc(_length);
    longestFLH = (FL_HEADER *)start;
    longestFLH->status = 1;
    longestFLH->length = _length;
    longestFLH->next = NULL;
    longestFLH->prev = NULL; 
	
	for(int i = 0; i < sizeOfHeaderArray; i++) {
		
		FL_Array[i] = (FL_HEADER *)malloc(SIZE_OF_HEADER);
		FL_Array[i]->status = 0;
		FL_Array[i]->length = _length;
		FL_Array[i]->next = NULL;
		FL_Array[i]->prev = NULL;
		
		nextInLine = (FL_HEADER *)malloc(SIZE_OF_HEADER);
		nextInLine->status = 0;
		nextInLine->length = 0;
		nextInLine->next = NULL;
		nextInLine->prev = NULL;
		
		FL_add(&FL_Array[i], nextInLine);
		
		_length /= 2;
	}
	
	FL_add(&FL_Array[0], longestFLH);
    
    return 0;
}

int release_allocator() {
    free(FL_Array);
    free(start);
    return 0;
}

Addr my_malloc(size_t _length) {

	FL_HEADER * toAdd;
	FL_HEADER * dividedHeader;
	FL_HEADER * matchedHeader;
		
	for(int i = sizeOfHeaderArray-1; i >=0; i--) {
		if(FL_Array[i]->next->length >= (_length + SIZE_OF_HEADER) && (FL_Array[i]->next->length < (_length + SIZE_OF_HEADER) * 2)  && (FL_Array[i]->next->status != 0)) {
			matchedHeader = FL_remove(FL_Array[i], FL_Array[i]->next);
			return (char *)matchedHeader + SIZE_OF_HEADER;
		}
		else if((FL_Array[i]->next->length > (_length + SIZE_OF_HEADER) * 2) && (FL_Array[i]->next->status != 0)) {
			toAdd = (FL_HEADER *)((char *)(FL_Array[i]->next) + (FL_Array[i]->length)/2);
			toAdd->length = FL_Array[i]->length/2;
			toAdd->status = 1;
			FL_Array[i]->next->length = (FL_Array[i]->next->length)/2;
			FL_add(&FL_Array[i+1], toAdd);
			dividedHeader = FL_remove(FL_Array[i], FL_Array[i]->next);
			FL_add(&FL_Array[i+1], dividedHeader);
			
			return my_malloc(_length);
		}
	}
	
	return NULL;
}

int my_free(Addr _a) {
	
	FL_HEADER * headerOfA;
	FL_HEADER * headerOfB;
	FL_HEADER * current;
	FL_HEADER * buddy;
	FL_HEADER * addToUpTier;
	int lengthOfA;
	char * _aStart;
	
	/* Acquiring the Buddy's Address
	*
	*	start is subtracted, so when we XOR, the addressed being XORed is referenced as if
	*	it was in the beginning. If we do not do this when we XOR, we might get an
	*	unexpected result. WE HAVE TO SHIFT THE ADDRESS OF headerOfA
	*
	*	After we get the result of XOR, we must add start back to acquire the buddy's
	*	actual address
	*
	*	_aStart and start are both converted to long, because we can't XOR memory
	*	addresses. That way, we need to set them as numbers instead so we can operate
	*	on them. Then, we set them to bAddress back as an address as (char *)
	*/
	headerOfA = (FL_HEADER*)((char *)(_a) - (SIZE_OF_HEADER));
 	_aStart = (char*)((char *)headerOfA - start);
 	lengthOfA = headerOfA->length;
	headerOfB = (FL_HEADER *)((char *)(((long)_aStart ^ (long)lengthOfA) + (long)start));
	
	///////////////////////////////////////////////////////////////
	/* Looping Through List of FL_HEADERs to find headerOfA's Buddy
	*
	*	To avoid complications, when we find the appropriate tier, we add headerOfA there
	*	immediately, and if a buddy is found, then remove headerOfA and buddy from that
	*	tier.
	*	
	*	To decide which FL_HEADER (headerOfA or headerOfB) is the original, we need to
	*	subtract their addresses first (originalHeaderFinder() function).
	*	Once the original address is found, add it to the level above it (level with *2
	*	length of buddy.
	*/
	for(int i = sizeOfHeaderArray-1; i >=0; i--) {
		if(FL_Array[i]->length == lengthOfA) {
			FL_add(&FL_Array[i], headerOfA);
			
			current = FL_Array[i];
			
			while(current != NULL) {
				if(headerOfB == current) {
					buddy = FL_remove(FL_Array[i], current);
					headerOfA = FL_remove(FL_Array[i], headerOfA);
					addToUpTier = orignalHeaderFinder(headerOfA, buddy);
					addToUpTier->length = lengthOfA * 2;
					FL_add(&FL_Array[i-1], addToUpTier);
					break;
					
				}
				
				current = current->next;
			}
		}
	}
  
	//////////////////////////////////////////////////////////////

  return 0;
}





/************************************************************************* Working set */

// 10:33 pm 9/27/17
// int my_free(Addr _a) {
// 	
//   	//go through FL_Array[] and find the corresponding length of _a then add _a in that list
//   	//do that math magic to find its buddy on the current FL_A[i]. it should spit out an address
//   	//	go through FL_A[i]'s list until matching address is found
//   	//		if found, join that FL_HEADER* with _a
//   	//		if not found, do nothing
// 	
// 	FL_HEADER * headerOfA;
// 	FL_HEADER * headerOfB;
// 	FL_HEADER * current;
// 	FL_HEADER * buddy;
// 	int lengthOfA;
// 	char * _aStart;
// 	
// 	/* Acquiring the Buddy's Address
// 	*
// 	*	start is subtracted, so when we XOR, the addressed being XORed is referenced as if
// 	*	it was in the beginning. If we do not do this when we XOR, we might get an
// 	*	unexpected result. WE HAVE TO SHIFT THE ADDRESS OF headerOfA
// 	*
// 	*	After we get the result of XOR, we must add start back to acquire the buddy's
// 	*	actual address
// 	*
// 	*	_aStart and start are both converted to long, because we can't XOR memory
// 	*	addresses. That way, we need to set them as numbers instead so we can operate
// 	*	on them. Then, we set them to bAddress back as an address as (char *)
// 	*/
// 	headerOfA = (FL_HEADER*)((char *)(_a) - (SIZE_OF_HEADER));
//  	_aStart = (char*)((char *)headerOfA - start);
//  	lengthOfA = headerOfA->length;
// 	headerOfB = (FL_HEADER *)((char *)(((long)_aStart ^ (long)lengthOfA) + (long)start));
// 	
// 	///////////////////////////////////////////////////////////////
// 	/* Looping Through List of FL_HEADERs to find headerOfA's Buddy
// 	*
// 	*	To decide which FL_HEADER (headerOfA or headerOfB) is the original, we need to
// 	*	subtract their addresses first.
// 	*/
// 	for(int i = sizeOfHeaderArray-1; i >=0; i--) {
// 		if(FL_Array[i]->length == headerOfA->length) {
// 			current = FL_Array[i];
// 			while(current != NULL) {
// 				if(headerOfB == current) {
// 					printf("\nfound buddy\n");
// 					//if buddy is found, join together and add to tier above
// 					buddy = FL_remove(FL_Array[i], current);
// 					//FL_add(&FL_Array[i-1], either headerOfA or headerOfB)
// 					break;
// 				}
// 				else {
// 					printf("\nsorry, buddy not found\n");
// 					//if buddy not found, then add to the list
// 					FL_add(&FL_Array[i], headerOfA);
// 				}
// 				current = current->next;
// 			}
// 		}
// 	}
//   
// 	//////////////////////////////////////////////////////////////
// 
//   return 0;
// }

/************************************************************************** Testing set */