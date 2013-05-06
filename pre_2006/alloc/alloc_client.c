/*
** A program that uses the less error-prone memory allocator.
*/
#include <stdlib.h>
#include <stdio.h>
#include "alloc.h"
#include <signal.h>

void interrupt_proc(int);

#define TRUE 1
int ctrl_c;
int allocs;
int alloc_size;

int
main(int argc, char **argv)
{	signal(SIGINT,interrupt_proc); //catch SIGINT
	if(argc != 2){
		puts("One argument: the number of integers to get.");
		exit(EXIT_FAILURE);
	}

	alloc_size=atoi(argv[1]);
	while( TRUE ){
		MALLOC(alloc_size,int);
		allocs++;
	}

	return 0; //will never come here
}

void
interrupt_proc(int sig)
{	
	if(ctrl_c==1){
		printf("\nNumber of allocations:%d"\
			"\nTotal memory allocated:%d\n"\
			, allocs, allocs*alloc_size*sizeof(int));
		exit(EXIT_SUCCESS);
	}
	printf(	"You pressed Ctrl-C!"\
		"Press again to quit.");
	fflush(stdout);
	ctrl_c++;
}
