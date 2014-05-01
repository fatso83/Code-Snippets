/* num_gen version 0.1 alpha
   Date: 2004-06-15
   Author: Carl-Erik Kopseng (lommetennis@hotmail.com)
   Purpose: Generate X random numbers in range 0-X when used in --single mode,
            or 2*X random numbers when used in --double mode.
	    Specifically written for use with the algorithms shown in [1].

   [1] Sedgewick, Algorithms in Java (2003)
*/


/*_______HELPER DEFINES_______________*/
#define FALSE 1
#define TRUE  0
#define REMOVE TRUE
#define DEBUG FALSE
/*------------------------------------*/

            
#include <stdlib.h> // rand and srand
#include <time.h>   // time_t time(time_t *tp)
#include <stdio.h>  // printf()/puts()
#include <math.h>   // ceil()

#define BUFFER 1000

int print( int [], int, int );
int * generate( int [], int, int );
int input( char [] );
void helpmsg( void );

int main(int argc, char *argv[])
{
  int mode, range, total;
  int ran_num[ BUFFER ];
  int num_buf, remainder = 0;

  /*Process input from user:
    Should be in the form:
    argv[1]="--mode=XX"
    argv[2]="--range=XX"
    argv[3]="--total=XX"
    
    Note: atoi (stdlib) converts string to integer
    PS DRAFT VERSION WILL ONLY USE NUMBERS!
  */

  if( argc < 4 ){
    helpmsg();
    return 1; /* Change to some ERR return code in final version */
  }
   
  mode = input( argv[1] );
  range = input( argv[2] );
  total = input( argv[3] );
  

#if (DEBUG == TRUE)
  printf("\n\nDEBUGMESSGE:\nmode=%d range=%d total=%d\n\n", mode,range,total);
#endif
 
  /* Initialize variables */
  num_buf = 0;  
  remainder = total % BUFFER;
  

  /* Seed rand() */
  srand( time(NULL) );

  if( remainder != 0){
    generate( ran_num, remainder, range );
    print( ran_num, remainder, mode );
    puts("\n\n");
  }
  
  if( total >= BUFFER)
    num_buf = (total - remainder)/BUFFER;
  else
    /* total < BUFFER means total equals remainder.
       The remainder has already been printed!
       Setting num_buf to null will thus exit the program.*/
    num_buf = 0;
  
  while( num_buf-- > 0){
    
#if DEBUG == TRUE
    printf("\n\n\nNUM_BUF=%d\n", num_buf);
#endif

    generate( ran_num, BUFFER, range );
    print( ran_num, BUFFER, mode );    
  }
  
  return 0;
  
}

///////////////////////////////////////////////////////////////////////////////
//          End of main()
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//          Helper functions
///////////////////////////////////////////////////////////////////////////////

int * generate( int *buffer, int amount, int roof )
{
  int i = 0;

  while( i < amount)
/*      Change, 9th of July 2004: changed ceil to floor().
	Then changed floor to (int)..Then deemed it unnessecary :) */
    buffer[i++] = ( ( (double) rand() * (double) roof ) 
			/ (double) RAND_MAX );

#if DEBUG == TRUE
  printf("\nbuffer[%d]=%d buffer[%d]=%d\n", 
	 0, buffer[0], amount-1, buffer[amount-1]);
#endif
  
  
  return buffer;
}
///////////////////////////////////////////////////////////////////////////////

int print( int *buf, int amount, int mode )
{

  int i = 0;
  
#if DEBUG == TRUE
  printf("Dette er print funksjonen\n");
#endif

  
  switch( mode ){
  case 0:
    printf("\n0 numbers per line?? Try again...\n");
    exit;
  case 1:
    while( i < amount){
      printf("%d\n", buf[i]);
      i=i+mode;
    }
  case 2:
    while( i < amount){
      printf("%d\t %d\n", buf[i], buf[i+1]);
      i=i+mode;
    }
  case 3:
    while( i < amount){
      printf("%d\t %d\t %d\n", buf[i], buf[i+1], buf[i+2]);
      i=i+mode;
    }
  }
  
  return 0;
}
///////////////////////////////////////////////////////////////////////////////

 int input( char * s )
{
#if DEBUG == TRUE
  puts("Dette er input-funksjonen");
#endif
  /* atoi() returns at most 2^31-1! in x86, which equals RAND_MAX */
  if( RAND_MAX <= atoi(s))
    printf(" RANGE must be less than %d!\n", RAND_MAX );

  return atoi(s);
}
///////////////////////////////////////////////////////////////////////////////

void helpmsg( void )
{
  /* "num_gen" can be replaced by some more sofisticated method 
     where you put argv[0], the program name, in instead.
Ex. printf("Usage: %s MODE RANGE TOTAL  \n%s", argv[0], msg);*/

  char msg[] = "Program usage: num_gen MODE RANGE TOTAL
                where MODE is how many numbers output per line,
                RANGE is what the maximum positive range is,
                and TOTAL is how many numbers you want output\n";
  printf("%s", msg);
}
//////////////////////////////////////////////////////////////////////////////
