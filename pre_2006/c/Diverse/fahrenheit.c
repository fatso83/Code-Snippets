#include <stdio.h>

int
main(int argc, char **argv)
{
	if( argc == 2 )
	{
		int degrees = atoi(argv[1]);
		printf(	"Fahrenheit: %s \n"
			"Celsius:    %.1f\n", 
			argv[1], (double) (degrees-32)*5/9);
	}
	
	return 0;
}		
