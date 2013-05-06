// Test av listeimplementasjon

/*
	CustomerListTest
	Programmet ber brukeren opprette et kundeobjekt.
	Brukeren kan deretter velge å opprette en ny kunde, skrive ut detaljer om kunden, eller
	fortsette å opprette varer på den siste kunden.
	Ctrl-D (EOF) for å avslutte
*/
#include <errno.h>
#include "helpers.h"
#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAMELEN 40
#define ADRLEN  100

//#define DEBUG
#ifdef DEBUG
  #define PRINTLINE printf("\nLinjenummer: %d\n", __LINE__);
#else
  #define PRINTLINE
#endif

int
main(char** argv, int argc)
{
	//argv er en peker til et array med kommandolinjeparametre
	//argc er antall argumenter

	char name [NAMELEN+1];
	char  address [ADRLEN+1];
        char id[20], tmpstring[20];
        int nmbr_itms = 0;
        ListItem *p_tmpList;
	Customer *pKunde;

#ifdef DEBUG
	//nullify
	setzero(name, NAMELEN+1);
	setzero(address, ADRLEN+1);
#endif

	printf("\n *** Customer List Test ***\n");
	printf("Oppgi ditt navn:\n\t\t");
	readln(name, NAMELEN+1);
	printf("og din adresse:\n\t\t");
	readln(address, ADRLEN+1);

	pKunde = create_customer(name, address);
	if(pKunde != NULL)
		printf("\n%s\n", view_customer(pKunde));
	else{
		puts("\nDet har skjedd en feil! Kunden er ikke opprettet.");
		exit(1);
	}

        //add item to the customers list
        p_tmpList= pKunde->merchandice;
        while(nmbr_itms!=-1){
                nmbr_itms=0;
                //id=NULL;
                printf("Legg til en ting i handlevognen:");
PRINTLINE
                readln(id, 20);
PRINTLINE
                puts("Spesifiser antall");
PRINTLINE
                readln(tmpstring, 20);
PRINTLINE
                sscanf(tmpstring, "%d", &nmbr_itms);
PRINTLINE
                if (nmbr_itms==-1) continue;
PRINTLINE


                p_tmpList = add_to_list( &p_tmpList, id, nmbr_itms);
PRINTLINE
                printf("Du har lagt til %d\t \"%s\"\n", p_tmpList->number_of_items, p_tmpList->identifier);
#ifdef DEBUG
printf("\nAdressen til head=%X\nAdressen til next=%X\nAdressen til noden:%X\n", p_tmpList->head, p_tmpList->next, p_tmpList);
#endif
        }

        printf("\nHer følger hva du har bestilt:\n%s", view_list(p_tmpList));



        puts("\n");
        return 0;
}



