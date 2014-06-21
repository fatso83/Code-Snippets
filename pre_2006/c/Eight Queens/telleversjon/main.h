#ifndef _MAIN_H_
#define _MAIN_H_
#define COLLISION -1
#define OK		   0
#define TRUE 1
#define FALSE 0
#define PRINT FALSE
#define NDEBUG

/*debug helpers
//#define DEBUG 
#define BUGLINE() printf("Fil:%s\tLinje:%d", __FILE__, __LINE__)
#define TWOVAR(x,y) printf("\nFil:%s\tLinje:%d\t"	\
			   "VAR1=%d\tVAR2=%d", __FILE__, __LINE__,x,y)
*/

int diagonal_check( int [], int );
int vertical_check( int [], int ); //no horizontal check is needed
void draw_chessboard( int [], int);
char * reverse_string( char * string );
void backtrack(int [], int, int );
#endif
