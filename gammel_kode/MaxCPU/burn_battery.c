#define BUFSIZE 0x10000 //65536
#define TRUE 1
int
main()
{
	int buffer[BUFSIZE];
	
	while(TRUE)
		for(int i=0;i<BUFSIZE;i++)
			buffer[i]=i;
	return 0; //will never get here
}
