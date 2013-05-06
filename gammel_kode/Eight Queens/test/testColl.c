#include "../main.h"
#include <stdio.h>

#define VERTI 1
#define DIAG 2
#define UNFIN 3
#ifndef STATUS
	#define STATUS 0
#endif

int main()
{
	int const size = 4;

	#if STATUS==DIAG
		int queens[] = { 0,2,0,3 }; 
	#elif STATUS==VERTI
		int queens[] = { 0,1,2,1 };
	#elif STATUS==UNFIN
		int queens[] = { 0,3,1,-1 };
	#else 
		int queens[] = { 1,3,0,2 }; //ingen kræsj
	#endif
	printf("status=%d", STATUS);
	draw_chessboard(queens, size);
	
	printf("Vertikale kollisjoner:");
	if( vertical_check(queens, size) == OK ) 
		puts(" Ingen");
	else 
		puts(" KOLLISJON");
	
	printf("Diagonale kollisjoner:");
	if( diagonal_check(queens, size) == OK ) 
		puts(" Ingen");
	else 
		puts(" KOLLISJON");


	return 0;

}
