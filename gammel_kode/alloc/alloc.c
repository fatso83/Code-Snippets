/*
** Implementation for a less error-prone memory allocator
*/
#include <stdio.h>
#include "alloc.h"
#undef 	malloc

void  *
alloc( size_t size )
{
	void *new_mem;
	
	/*
	** Ask for the rquested memory, and check that we really 
	** got it
	*/
	new_mem = malloc( size);
	if( new_mem == NULL){
		printf( "Out of memory!\n" );
		exit( EXIT_FAILURE );
	}
	return new_mem;
}
