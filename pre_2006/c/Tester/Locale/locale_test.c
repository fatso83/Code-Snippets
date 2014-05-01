#include <locale.h>
#include <malloc.h>

int
main()
{
	int i;
	struct  lconv *local_variables;
	char * s="\3", * var;
	
	printf("Det kjørende \"locale\" er :\"%s\"\n", setlocale(LC_ALL,NULL));

	local_variables = localeconv();
	var= local_variables->decimal_point;

	printf("grouping=\"%s\"\n", local_variables->grouping);

	for( i=0;i<10;i++) printf("%s\n", var+i);


	printf("Setter grouping til 3");
	setlocale( LC_MONETARY, "no_NO" );

	localeconv()->grouping=s;



	printf("grouping=\"%s\"", local_variables->grouping);

	return 0;
}
