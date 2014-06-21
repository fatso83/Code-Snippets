#include <string.h>	/* strstr() */
#include <assert.h>
#include <malloc.h>
#include <stdlib.h>
#include "battery.h"

#include <stdio.h> //debugging
#define NDEBUG

Node *root=NULL;

int
minutes_left( int mWh_left )
{
	int avg = average_power_usage();
	if( avg > 0 )
		return mWh_left*60/avg;
	else
		return 0;
}

int
average_power_usage(void)
{
	int total = 0, count = 0;
	Node *current = root;

	while( current != NULL ){

#ifndef NDEBUG
		printf("current->value=%d\n", current->value);
#endif
		total+=current->value;
		count++;
		current=current->next;
	}

	if(count==0)
		return 0;
	return total/count;
}

Node *
add_rate( int rate )
{
	Node *tmp,*current;
	int count = 1;

	if(root==NULL){
		#ifndef NDEBUG
		puts("root==NULL");
		#endif
		root=malloc(sizeof(Node));
	
		if(root==NULL){
			printf("Out of memory! Ending...");
			exit(EXIT_FAILURE);
		}
		root->value=rate;
		#ifndef NDEBUG
		printf("root->value=%d",root->value);
		#endif
		return root;
	}

	current=root;
	while( current->next != NULL ){
		current = current->next;
		count++;
	}	

	if( count == 10 ){
		tmp = root->next;
		free(root);
		root = tmp;
	}
	current->next = malloc(sizeof(Node));
	current = current->next;
	current->value=rate;
	current->next=NULL;

	return current;
}
		

int
get_value(const char *pre_pattern, FILE *fp)
{
	char buffer[BUFSIZE];
	char *substr = NULL;

	/* Finn stedet i fila der verdien finner seg */
	while(fgets( buffer,BUFSIZE, fp) !=NULL ){
		if( (substr = strstr(buffer, pre_pattern)) != NULL)
			break;
	}
	assert( substr != NULL );
	/* Verdien befinner seg etter pre_pattern*/
	return 	atoi(strtok(substr+strlen(pre_pattern), " "));	
}
