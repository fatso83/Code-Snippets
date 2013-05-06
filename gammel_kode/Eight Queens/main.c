/* 	Eight Queens solution program
   	Start date: 2006-08-08 
	Functioning date: 2006-08-12
	Author: Carl-Erik Kopseng  <carlerik@gmail.com>
   	Purpose: Solve the eight queens problem by backtracking
*/

/*To print the chessboards, compile with gcc -D PRINT  */
/* Time usage on Pentium 4 1.8GHz,640MB (the program uses <300KB):
	Queens	Time		Solutions
	8	0,0163		92
	9	0,046
	10	0,28
	11	1,8
	12	11,8
	13	77.5		77312
*/ 

#include <stdio.h>            
#include <time.h>
#include "main.h"

#define NMBR 11


int main(int argc, char *argv[])
{
	extern int nmbr_solutions;
	int queens[NMBR], i;
	float seconds;
	clock_t start,running_time; //time_t is long
	
	//initialize to non-existant values
	for(i=0; i<NMBR ; i++) queens[i]=-1;

	start=clock();
	backtrack(queens, 0, NMBR);
	running_time = clock()-start;
	seconds=(double)running_time/CLOCKS_PER_SEC;

	printf(   "The number of solutions is %14d", nmbr_solutions);
	printf( "\nTime spent:             %16dl"
		"\nSeconds:                %3.15f\n", running_time, seconds);


	return 0;
}	
