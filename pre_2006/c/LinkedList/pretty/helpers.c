#include <stdio.h>
#include <string.h>
#include <assert.h>

//#define NDEBUG
//#define DEBUG

void hexdump( char *string, int len)
{
	int i=0, j=0, k=0;

start:
	puts("\n");
	while( i<len){
		printf("%5d", i++);
		if( 0 == i%16) break;
	}
	puts("");
	while( j<len){
		printf("%5d", (int) string[j++]);
		if( 0 == j%16) break;
	}
	puts("");
	while( k<len){
		printf("%5c", string[k++]);
		if( 0 == k%16) break;
	}

	//printf(" i=%d j=%d k=%d", i,j,k);
	//printf("i%16=%d  j%16=%d   k%16=%d", i%16, j%16, k%16);

	if ( i%16 == 0 && i!=len) goto start;
}

size_t readln( char *s, size_t bufferlen )
{

	//bufferlen -1 == \0
	//bufferlen -2 == last letter
	char c = 0;
	char *lastchar = s+bufferlen-2;
	size_t size = 0;
	fgets(s, bufferlen, stdin);

#ifdef DEBUG
	printf("Lengden på bufferet er %d\nLengden på strengen er %d\n", bufferlen, strlen(s));
	printf("Siste bokstav i bufferet er \"%c\" %d\n\n", *lastchar, (unsigned int) *lastchar);
#endif

	/*Check if there is any remaining characters on the line*/
	if( (size =strlen(s)) == bufferlen-1 ){
		if( *lastchar != '\n' ){
			//eat the rest
			while( (c = (unsigned char) getc(stdin)) !=  '\n' ){
#ifdef DEBUG
				printf("\nLeste bokstav:  \"%c\" (nummer %u)", c, (unsigned int) c);
				printf("\\n (NewLine) har kode %X (%u decimal)", '\n', '\n');
#endif
			}
		}
#ifdef DEBUG
		printf("\nsize=%d   (bufferlen-1)=%d", size, bufferlen);
#endif
		assert( size==bufferlen-1 );
		assert( *(s+bufferlen-1) == '\0');

                *lastchar = '\0'; //last char is newline. changes to NUL
		return size;
	}
	else{
		assert ( size < bufferlen -1 );
                *(s+size-1) = '\0';
		return size;
	}
}


void setzero(char *buf, int size)
{
	int i=0;
	do{
		*(buf+i) = 0;
	}while ( ++i < size);
}
