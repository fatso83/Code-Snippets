/* BatteryMon
 * Uses the /proc interface to the 36Wh battery supplied with the Armada M300
 * to get info on remaining power based on the 20 last seconds.
 * Author: Carl-Erik Kopseng <carlerik@gmail.com>
 * Date:   Jan 5 2007
 */  

#include <stdio.h>
#include <unistd.h>	/* sleep() */
#include <stdlib.h>	/* exit() */
#include "battery.h"	/* INTERVAL and other #defines */

int
main(int argc, char **argv)
{
	char *filename="/proc/acpi/battery/C0FF/state";
	FILE *fp;
	int rate,remaining,minutes,interval,usual_rate;
	char *pattern[2] = {"present rate:",
			   "remaining capacity:"};
	usual_rate=11000;
	if(argc==2)
		interval=atoi(argv[1]);
	else
		interval=INTERVAL;

	while( TRUE ){
		fp = fopen( filename, "r" );
		if( fp == NULL ){
			perror(filename);
			puts("Are ACPI modules loaded?");
		 	exit(EXIT_FAILURE);
		}

		rate=get_value(pattern[0],fp);
		remaining=get_value(pattern[1],fp);
		if(rate>0)
			add_rate(rate);
		fclose(fp);
		minutes=minutes_left(remaining) ;
		
		if(rate!=0){
			printf("\nUsing %d mW\nRemaining %d mWh", 
				rate, remaining);
		}else{
			printf("Using battery power. %d%% charged\n", 
				remaining*100/MAX_CHARGE);
		}

		if(minutes!=0){
			printf("\nApproximately %d minutes left of power\n",
				 minutes);
		}else{
			printf("\nNot enough measurements."\
				"\nBy %d mWh power usage there is approx. %d"\
				" minutes left (%d).\n", 
				usual_rate, 60*remaining/usual_rate, remaining);
		}

		fflush(stdout);
		sleep(interval);
	}
	/* Will never come this far. Will be stuck in loop */
	fclose(fp);	
	exit(EXIT_SUCCESS);
}

// use sighandler sighandler_t to catch SIGQUIT/CONTROL-C
