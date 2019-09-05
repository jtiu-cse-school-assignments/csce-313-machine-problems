#ifndef _free_list_h_  
#define _free_list_h_

	typedef struct fl_header { /* header for block in free list */
		
		unsigned int status;
		unsigned int length;
		struct fl_header *next;
		struct fl_header *prev; 
		
	 } FL_HEADER; 
	 
	 FL_HEADER * FL_remove(FL_HEADER * free_list, FL_HEADER * block);
	 /* Remove the given block from given free list. The free-list
		pointer points to the first block in the free list. Depending
		on your implementation you may not need the free-list pointer.*/
	 
	 void FL_add(FL_HEADER ** free_list, FL_HEADER * block);
	 /* Add a block to the free list. */

#endif