#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "temperature.h"
#include "../alloc/alloc.h"

#define BUFSIZE 100
char *acpi_temp_filename;
char *pre_pattern;

void
set_pre_pattern(char *pattern)
{	int bufsz = 100;
	pre_pattern=MALLOC(bufsz,char);
	strncpy(pre_pattern, pattern,bufsz);
}

void
set_file(char *filename) 
{ 
	acpi_temp_filename = filename;

	/* Test if the file exists */
	FILE *fp = fopen(acpi_temp_filename,"r");
	if(fp == NULL){
		perror(acpi_temp_filename);
		puts("Are ACPI modules loaded?");
	 	exit(EXIT_FAILURE);
	}
}	

int
get_temp()
{
	char buffer[BUFSIZE];
	char *substr = NULL;
	FILE *fp = fopen(acpi_temp_filename,"r");

	/* Finn stedet i fila der verdien finner seg */
	while(fgets( buffer,BUFSIZE, fp) !=NULL ){
	/* strstr() returns a substring of "buffer" */
		if( (substr = strstr(buffer, pre_pattern)) != NULL)
			break;
	}

	assert( substr != NULL );
	/* Verdien befinner seg etter pre_pattern*/
	return 	atoi(strtok(substr+strlen(pre_pattern), " \t"));	
}
