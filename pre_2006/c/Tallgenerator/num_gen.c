/* num_gen version 0.2 
   Author: Carl-Erik Kopseng (lommetennis@hotmail.com)

   Ver0.1: 2004-06-15
   Ver0.2: 2006-09-07

Changelog:
	2006-09-07 - 	Removed a lot of redundant code, especially the printing			function needed a touch up. Previous version only
			printed up to 3 numbers per line. Now it can print
			from one to every number on a line.
*/


#include <stdlib.h> // rand and srand
#include <time.h>   // time_t time(time_t *tp)
#include <stdio.h>  // printf()/puts()
#include <math.h>   // ceil()

#define BUFFER 1000

int print( int [], int, int );
int * generate( int [], int, int );
void helpmsg( void );

int main(int argc, char *argv[])
{
	int mode, range, total;
	int ran_num[ BUFFER ];
	int num_buf = 0, remainder = 0;

	if( argc < 4 ){
		helpmsg();
		exit(EXIT_FAILURE); 
	}
   
	mode = atoi(argv[1]);
	if( RAND_MAX <= (range=atoi(argv[2])))
		printf(" RANGE must be less than %d!\n", RAND_MAX );
	total = atoi( argv[3] );
  
	/* Seed rand() */
	srand( time(NULL) );

	if( (remainder = total % BUFFER) != 0){
		generate( ran_num, remainder, range );
		print( ran_num, remainder, mode );
	}
	
	num_buf = (total - remainder)/BUFFER;
	while( num_buf-- > 0){
		generate( ran_num, BUFFER, range );
		print( ran_num, BUFFER, mode );    
	}
  
	return 0;
  
}

///////////////////////////////////////////////////////////////////////////////
//          End of main()
///////////////////////////////////////////////////////////////////////////////


int * generate( int *buffer, int amount, int roof )
{
	int i = 0;

	while( i < amount)
		buffer[i++] = ( ( (double) rand() * (double) roof ) 
				/ (double) RAND_MAX );
	return buffer;
}
///////////////////////////////////////////////////////////////////////////////

int print( int *buf, int total, int mode )
{
	int i = 0;
	while(i<total){
		printf("%d ", buf[i]);
		i++;
		if(i%mode==0) puts("");
	}
  
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

void helpmsg( void )
{
	/* "num_gen" can be replaced by some more sofisticated method 
	   where you put argv[0], the program name, in instead.
	   Ex. printf("Usage: %s MODE RANGE TOTAL  \n%s", argv[0], msg);*/

	char msg[] = "Program usage: num_gen MODE RANGE TOTAL\n"\
		"where MODE is how many numbers output per line,\n"\
		"RANGE is what the maximum positive range is,\n"\
		"and TOTAL is how many numbers you want output\n";
	printf("%s", msg);
}
//////////////////////////////////////////////////////////////////////////////
