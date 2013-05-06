/* display functions */
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stddef.h> //NULL
#include <assert.h>

#define NUL '\0' //nullbyte
#define HIGHBEEP 149
#define LOWBEEP    7
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
	for(i=0; i<size ;i++){
		if(queen[i]!=-1)
			printbuf[ (size+1)*i + queen[i] ] = 'Q';
	}

	printf("\n\n%s\n", printbuf);

}

//prints a strings in reverse order
char * reverse_string( char *string )
{
	int i;
	size_t len;
	
	char  revString[ (len=strlen(string))+1 ]; //+ NUL byte
	
	for( i=0; i< (int) len; i++)
		revString[len-1-i] = *(string+i);
	revString[len]=NUL;

	strcpy(string,revString);
	return string;
}
		
void beep()
{
	printf("%c", HIGHBEEP );
}
