/* display functions */
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stddef.h> //NULL
#include <assert.h>

#define NUL '\0' //nullbyte
#define DEBUG 

void draw_chessboard( int queen[], int size)
{
	int mapsize=(size*size)+size+1;	//(size^2)chars+(size)newlines+NULL
	char printbuf[mapsize];	
	int i, idx;
	
	//"clear" the map with x's
	idx=0;
	while(idx<mapsize-1)
		printbuf[idx++]='x';
	//make the newlines
	idx=0;
	while( (idx+=(size+1)) < mapsize )
		printbuf[idx-1]='\n';	//idx is off by one
	//insert NUL
	printbuf[mapsize-1] = NUL;
		
	/*fill in with the positions of the queens*/
	for(i=0; i<size ;i++)
		printbuf[ (size+1)*i + queen[i] ] = 'Q';
	
	printf("\n\n%s\n", printbuf);

}
