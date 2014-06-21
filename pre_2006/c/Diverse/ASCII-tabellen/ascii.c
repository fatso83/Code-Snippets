#include <stdio.h>
/*/#include <stddef.h>*/

#define NUL 0

int
main()
{
  int i;
  char s[] = {0,NUL};
  char  * p_s;
  p_s = &s[0];

  for( i = 0, s[0] = i ; i < 256 ; i++, s[0] = i)
    printf("Decimal :%d\t ASCII :%s\n", i, p_s);
  return 0;
}
