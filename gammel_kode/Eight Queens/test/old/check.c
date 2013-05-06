/* 	Functions to check whether there exists any collisions.
	If so return COLLISION, else OK.
	The arguments are the coordinate map of the queens and the size of the board
	Coordinates of the queen7 in row 7 is ( 7, queen[7] )
	(7,-1) if not placed yet.
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
int diagonal_check( int queen[], int size )
{
	int i,j;
	for( i=0; i<size; i++){
		if(queen[i]==-1) //not set yet - early retreat
			return OK;
		
		// upleft //
		//start at row above i
		for( j=i+1; j<size; j++){
			TWOVAR(i,j);
			if((j-i)==(queen[i]-queen[j]))
				return COLLISION;
			if(queen[j]==-1) // no position to compare
				break;
		}

		// upright //
		for( j=i+1; j<size; j++){
			TWOVAR(i,j);
			if((j-i) == (queen[j]-queen[i]))
				return COLLISION;
			if(queen[j]==-1) // no position to compare
				break;
		}

		// downleft //
		//start at row below i
		for( j=i-1; j>=0; j--){
			TWOVAR(i,j);
			if((i-j)== (queen[i]-queen[j]))
				return COLLISION;
			if(queen[j]==-1) // no position to compare
				break;
		}

		// downright //
		for( j=i-1; j>=0; j--){
			TWOVAR(i,j);
			if((i-j) == (queen[j]-queen[i]))
				return COLLISION;
			if(queen[j]==-1) // no position to compare
				break;
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
			TWOVAR(i,j);
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
