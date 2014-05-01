#include <stdio.h>
int main()
{
	int lengde;
	const char *streng;
	char tmp[] = "Dette er en liten test";
	streng = tmp;
	const char **p_streng = &streng;

	printf("\nstreng[]:\nStrengens adresse:%X\nStrengpekerens adresse:%X\n", streng, &streng);
	printf("&streng:\t%s\n", &streng);
	printf("streng:\t%s\n", streng );

	printf("\n**p_streng:\nAdressen til strengpekeren (streng[]):%X\nStrengpeker-pekerens adresse:%X\n", p_streng, &p_streng);
	printf("&p_streng:\t%s\n", &p_streng);
	printf("p_streng:\t%s\n", p_streng);
	printf("*p_streng:\t%s\n", *p_streng);


	return 0;

}
