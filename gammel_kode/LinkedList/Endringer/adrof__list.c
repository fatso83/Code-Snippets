#include <assert.h>	// assert
#include <stdio.h>	// NULL
#include <stdlib.h>  	// malloc
#include <string.h> 	// strcat
#include "list.h"		// private
//#define NDEBUG 0		// True="Ikke debugg"=1 False="Debugg"=0

#define DEBUG
#define PRINTLINE printf("Fil:%s Linjenummer: %d\n", __FILE__, __LINE__);
//#define PRINTLINE

#ifdef DEBUG
	#include <unistd.h> //sleep()
#endif

Customer*
create_customer(string name, string address)
{
	Customer *customer;

	if( ! (customer =  (Customer*) malloc(sizeof(Customer)) ) )
		return NULL; 	//if allocation fails, return NULL pointer

        customer->name= malloc(strlen(name)+1);  //+NUL byte
        customer->adr =   malloc(strlen(address)+1); //+NUL byte

        if ( customer->name==NULL || customer->adr == NULL){
                free(customer->name);
                free(customer->adr);
                free(customer);
                return NULL;
        }

	strcpy(customer->name, name);
	strcpy(customer->adr, address);
	customer->merchandice = NULL;

	assert(customer != NULL);
	return customer;
}

ListItem*
add_to_list(ListItem** p_node,string id_str, int count)
{



PRINTLINE
        int first_node = 0;
        ListItem *head;

PRINTLINE

        if(*p_node ==NULL) first_node = 1;
        else head = (*p_node)->head;



PRINTLINE
#ifdef DEBUG
        //printf("\n%s,L%d\nAdressen til head=%X\nAdressen til next=%X\nAdressen til noden:%X\nfirst_node=%d\n",
          //      __FILE__, __LINE__, node->head, node->next, node, first_node);
if( *p_node == NULL){PRINTLINE printf("aNULL pointer! first_node=%d", first_node);}
else if( (*p_node)->next == NULL){PRINTLINE puts("bNULL pointer!");}
else if( (*p_node)->head == NULL){PRINTLINE puts("cNULL pointer!");}
else {PRINTLINE printf("\n%s,L%d\nAdressen til head=%X\nAdressen til next=%X\nAdressen til noden:%X\nfirst_node=\n",
                __FILE__, __LINE__, (*p_node)->head, (*p_node)->next, *p_node);}
#endif
PRINTLINE




	while (*p_node != NULL)
		p_node = (*p_node)->next;

PRINTLINE

	if( (*p_node = (ListItem*) malloc(sizeof(ListItem))) == NULL){
                printf("No memory allocated for listobject!\nTerminating...\n");
                exit(1);
        }
        assert( *p_node!=NULL);

PRINTLINE

        if( ( (*p_node)->identifier=malloc(strlen(id_str)+1)) == NULL )
                return NULL;
PRINTLINE
	strcpy( (*p_node)->identifier, id_str);
PRINTLINE
	(*p_node)->number_of_items = count;
PRINTLINE
        if(first_node)
                (*p_node)->head=p_node;
#ifdef DEBUG
        printf("\n%s,L%d\nAdressen til head=%X\nAdressen til next=%X\nAdressen til noden:%X\n",
                __FILE__, __LINE__, (*p_node)->head, (*p_node)->next, *p_node);
#endif


        assert(*p_node != NULL);*/
	return *p_node;
}


/* Bør view metoder legges i en egen fil?
   Den underliggende representasjonen bør jo
   være uavhengig av presentasjonen
   Essensielt burde kanskje en view_customer gi
   tilbake en streng?
*/

string
view_customer( Customer *customer )
{
	char details[256];
	sprintf( details, "\nKundenavn: %s\nAdresse:%s\n\n",
				customer->name, customer->adr);
	return details;
}

string
view_list( ListItem *element )
{
	char presentation[2048];
	char tmp[128];

	element  = element->head;
	while( element != NULL ) {
		sprintf(tmp, "Type: %s\tAntall%d\n",
			element->identifier, element->number_of_items);
		strcat( presentation, tmp);
		element = element->next;
	}
	return presentation;
}



