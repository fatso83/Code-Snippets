#include <stdio.h>
#include "battery.h"
#define SUCCESS  0
#define FAILURE -1

int values[]={1000,9000,20000,-1}; // -1 marks end of array

int
add_rate_test()
{
	for(int i=0; values[i]!=-1;i++){
		if( add_rate(values[i])->value!=values[i])
			return FAILURE;
	}
	return SUCCESS;
}

int
minutes_left_test(){return SUCCESS; }

int
average_power_usage_test()
{ 
	int total = 0,nmbr_vals=0;
	for(int i=0; values[i]!=-1;i++,nmbr_vals++){ total+=values[i]; }
#ifndef NDEBUG
	printf("total=%d nmbr_vals=%d\n", total, nmbr_vals);
	fflush(stdout);
#endif
	if(total/nmbr_vals==average_power_usage())
		return SUCCESS; 
	else	return FAILURE;
}

int
get_value_test(){ return SUCCESS; }

int
main()
{
	//lag en matrise med funksjonspekere man kan loope
	//(*) int() test;
	printf("add_rate:%s\n", add_rate_test()==SUCCESS?"OK":"FAILED");
	printf("average_power_usage:%s\n",
		average_power_usage_test()==SUCCESS?"OK":"FAILED");
	//printf("minutes_left:%s\n", minutes_left_test()==SUCCESS?"OK":"FAILED");
	//printf("get_value:%s\n", get_value_test()==SUCCESS?"OK":"FAILED");
}
