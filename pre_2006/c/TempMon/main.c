/* TempMon
 * Uses the /proc/acpi/thermal_zone/C15C/temperature
 * Based on BatteryMon code
 * Author: Carl-Erik Kopseng <carlerik@gmail.com>
 * Date:   Jan 5 2007
 */  

#include <string.h>
#include <stdio.h>
#include <unistd.h>	/* sleep() */
#include <stdlib.h>	/* exit() */
#include "temperature.h"/* INTERVAL and other #defines */

int
main(int argc, char **argv)
{
	char *filename="/proc/acpi/thermal_zone/C15C/temperature";
	char *pre_pattern="temperature:";
	int temp,quiet=0;

	if((argc == 1) || strcmp(*(argv+1),"quiet") != 0){
		puts("Add \"quiet\" as parameter to only output numbers");
	}else{ quiet=1; }

	//Setup
	set_file(filename);
	set_pre_pattern(pre_pattern);

	while( TRUE ){
		temp=get_temp();
		printf("\n%s%d",
			 (quiet==0? "Temperature:" : ""), temp);
		fflush(stdout);
		sleep(INTERVAL);
	}
	/* Will never come this far. Will be stuck in loop */
	exit(EXIT_SUCCESS);
}
// use sighandler sighandler_t to catch SIGQUIT/CONTROL-C
