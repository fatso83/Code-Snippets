#ifndef _MAIN_H_
#define _MAIN_H_
#define COLLISION -1
#define OK		   0
#define TRUE 1
#define FALSE 0
#define NDEBUG


int diagonal_check( int [], int );
int vertical_check( int [], int ); 
void draw_chessboard( int [], int);
char * reverse_string( char * string );
void backtrack(int [], int, int );
#endif
/*end _MAIN_H_ */
