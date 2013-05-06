/* 	Functions to check whether there exists any collisions.
	If so return COLLISION, else OK.
	The arguments are the coordinate map of the queens and the size of the board
	Coordinates of the queen7 in row 7 is ( 7, queen[7] )
	(7,-1) if not placed yet.
*/

/* CHANGES
	v04: August 12: left and right versions of diagonal searching
			was almost identical. moved into same loopbody.
	     		13,3% faster (65 sec comp. to 75 sec, 13 queens) 
		
			Compilinng with optimisations can give huge boosts.
			gcc -O10 is more than twice as fast as no optimisations.
*/

/* Possible optimisations
	On the diagonal search left/right one can quit earlier.
	No need to check the numbers above the diagonal!
	If one, say a 8x8 board, is on the bottom row, and second column
	from the left, there is no need	to check for collisions above
	the second row.
*/

#include "main.h"
#include <assert.h>

/*	Example of 5x5 chess board (size=5). 
		4
		3
		2
		1
		0
		  0 1 2 3 4 
*/


/*	starts on the first row.
 *	first to the left, then to the right.
 *	first up, then down.
 * 	All relative to the current queen!
 */
long instruksjoner=0;

int diagonal_check( int queen[], int size )
{
	int i,j;
	for( i=0; i<size; i++){
		if(queen[i]==-1) //not set yet - early retreat
			return OK;
		
		//UP
		//start at row above i
		for( j=i+1; j<size; j++){
			instruksjoner++;
			if(queen[j]==-1) // no position to compare
				break;
			if((j-i) == (queen[j]-queen[i]))// upright //
				return COLLISION;
			if((j-i) == (queen[i]-queen[j]))// upleft //
				return COLLISION;
		}
		
		//DOWN
		//start at row below i
		for( j=i-1; j>=0; j--){
			instruksjoner++;
			if(queen[j]==-1) // no position to compare
				break;
			if((i-j)== (queen[i]-queen[j]))// downleft //
				return COLLISION;
			if((i-j) == (queen[j]-queen[i]))// downright //
				return COLLISION;
		}
	}//end for
	return OK; //no diogonal match
}

int vertical_check(int queen[], int size)
{
	int i,j;
	for(i=0; i<size; i++){
		if(queen[i]==-1) return OK;
		for( j=0; j<size; j++){
			instruksjoner++;
			
			if( (queen[i]==queen[j]) && i!=j)
				return COLLISION;
		}
	}

	/* should only come here
	 * if all queens are placed
	 */
	assert( i==size );
	return OK;
}
