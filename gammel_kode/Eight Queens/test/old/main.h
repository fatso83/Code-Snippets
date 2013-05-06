#define COLLISION -1
#define OK		   0
#define DEBUG
#define SZ 3

#define BUGLINE() printf("Fil:%s\tLinje:%d", __FILE__, __LINE__)
#define TWOVAR(x,y) printf("\nFil:%s\tLinje:%d\t"	\
			   "VAR1=%d\tVAR2=%d", __FILE__, __LINE__,x,y)
int diagonal_check( int [], int );
int vertical_check( int [], int ); //no horizontal check is needed
void draw_chessboard( int [], int);
