#include <stdio.h>

void DBG_arguments(void){
	printf("argc=%d\n",argc);
	for(int i=0;i < argc;i++) printf("\nargv[%d]=%s",i,*(argv+i));
}

