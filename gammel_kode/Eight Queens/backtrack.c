/* bactrack
 *
 * Is the main workhorse/algorithm of the program.
 * 
 * 
 * q[] = array containing info on the queens
 * idx = which queen/row
 * sz  = the number of queens
*/
#include "main.h"

/*Hvorfor ikke bare returnere denne til main?*/
int nmbr_solutions=0;

void
backtrack( int q[], int row_idx, int sz )
{ 
	int column=0, i;
	if( row_idx == sz ){ //last row was ok
		#ifdef PRINT
		  draw_chessboard(q,sz);
		#endif
		nmbr_solutions++;
		return;
	}

	while(column<sz){
		q[row_idx]=column;
		if( vertical_check(q,sz) == COLLISION )
			column++;		
		else if(diagonal_check(q,sz) == COLLISION)
			column++;		
		else{ //no collisions, go further up
			backtrack(q,row_idx+1, sz);			

			//reset rows above this one
			for(i=row_idx+1; i<sz ; i++) q[i]=-1; 
			column++;
		}
	}//end while
	/*The function returns to main*/
}
