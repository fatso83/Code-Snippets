#include <assert.h>	// assert
#include <stdio.h>	// NULL
#include <stdlib.h>  	// malloc
#include <string.h> 	// strcat
#include "list.h"		// private
//#define NDEBUG 
//#define DEBUG

#ifdef DEBUG
  #define PRINTLINE printf("Fil:%s Linjenummer: %d\n", __FILE__, __LINE__);
#else
  #define PRINTLINE
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
        int first_node = 0;
        ListItem *head, *new_node, *last_node;
        
	if(*p_node ==NULL) first_node = 1;
        else head = (*p_node)->head;


PRINTLINE
#ifdef DEBUG
if( *p_node == NULL){PRINTLINE printf("NULL pointer (*p_node)! first_node=%d\n", first_node);}
else if( (*p_node)->next == NULL){PRINTLINE puts("NULL pointer! (*p_node)->next");}
else if( (*p_node)->head == NULL){PRINTLINE puts("NULL pointer! (*p_node)->head");}
else {PRINTLINE printf("\n%s,L%d\nAdressen til head=%X\nAdressen til next=%X\nAdressen til noden:%X\nfirst_node=\n",
                __FILE__, __LINE__, (*p_node)->head, (*p_node)->next, *p_node);}
#endif

PRINTLINE

        if( ! first_node){
	        while ( (*p_node)->next != NULL)
		        *p_node = (*p_node)->next;
                last_node = *p_node;
        }


	if( (new_node= (ListItem*) malloc(sizeof(ListItem))) == NULL){
                printf("No memory allocated for listobject!\nTerminating...\n");
                exit(1);
        }
        assert( new_node!=NULL);
        PRINTLINE
        if( ! first_node )
                last_node->next=new_node;

PRINTLINE

/* Tildel verdier til objektet */
        if( ( new_node->identifier=malloc(strlen(id_str)+1)) == NULL )
                return NULL;
	strcpy( new_node->identifier, id_str);
	new_node->number_of_items = count;

/* Sett første objekt */
        if(first_node){
                new_node->head=new_node;
        }
        else{
                new_node->head=last_node->head;
#ifdef DEBUG		
                printf("new_node->head=%X", new_node->head);
#endif		
        }


#ifdef DEBUG
        printf("\n%s,L%d\nAdressen til head=%X\nAdressen til next=%X\nAdressen til noden:%X\n",
                __FILE__, __LINE__, new_node->head, new_node->next, new_node);
#endif


        assert(new_node != NULL);
        assert( new_node->head != NULL );
	return new_node;;
}


/* BÃ¸r view metoder legges i en egen fil?
   Den underliggende representasjonen bÃ¸r jo
   vÃ¦re uavhengig av presentasjonen
   Essensielt burde kanskje en view_customer gi
   tilbake en streng?
*/

string
view_customer( Customer *customer )
{
	static char details[256];
	sprintf( details, "\nKundenavn: %s\nAdresse:%s\n\n",
				customer->name, customer->adr);
	return details;
}

string
view_list( ListItem *element )
{
	static char presentation[2048]; //static == 0-init
	char tmp[100];

	element  = element->head;
	while( element != NULL ) {
		sprintf(tmp, "Type: %s\tAntall: %d\n",
			element->identifier, element->number_of_items);
		strcat( presentation, tmp);
#ifdef DEBUG
                if( (element->next) != NULL){
			printf("Presentation string:%d\n",strlen(presentation));
                        sprintf(tmp, "Peker til neste objekt:%X\n", element->next);
			printf("Lengde på streng:%d\n",strlen(tmp)+strlen(presentation));
                        strcat( presentation, tmp );
                }
#endif
		element = element->next;
	}
	return presentation;
}



