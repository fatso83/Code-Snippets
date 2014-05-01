//#include "../../Diverse/alloc/alloc.h"
#include <stdio.h>
#include <stdlib.h>

/* 	Ha opp en systemmonitor over minneforbruk.
	Vil da vise i sanntid hvordan minnet minsker for hvert sekund
	Bruker ca. 47sekunder på å tømme 640MB RAM + 1 GB swap.
*/

#define KB 0x400
#define MB 0x100000
#define SZ MB
int
main()
{
	char *buffer;

	//while( 1 ) 
	//	MALLOC(1, char);
	//kommer aldri hit
	//MALLOC gir exit(1) når det er tomt for minne
	printf("Henter ut %dB\n", SZ);
	fflush(stdout);
	while( 1 ){
		buffer = (char *) malloc(SZ);
		if ( buffer == NULL ){
			printf("Out of memory");
			return 0;
		}
		//free(buffer);
		//printf("Fikk minne\n");
	}
		


	printf("Burde aldri komme hit");
	return 0;
}
