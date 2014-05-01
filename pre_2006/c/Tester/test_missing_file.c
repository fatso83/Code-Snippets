#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int
main(int argc, char **argv)
{
	if(argc<2){
		puts("Trenger et filnavn som argument");
		exit(1);
	}

	for(int i=1 ; i<argc ; i++){
		FILE *filehandle;
		printf("\"%s\"\n", argv[i]);
		filehandle=fopen(argv[i],"r");
		if(filehandle==NULL)
			puts("filehandle==NULL");
		/*if(ferror(filehandle)!=0)
			puts("ferror()!=0");
		*/
		printf("errno=%d\n",errno);
		perror("fopen");
		//clearerr(filehandle);
		errno=0;
		puts("\n");
	}
	return 0;
}
