/*
*  Type definitions
*/
typedef char* string;

typedef struct LIST
{
	struct LIST *next;
	struct LIST *head;
	string identifier;
	int number_of_items;
} ListItem;

typedef struct CUSTOMER
{
	ListItem* merchandice;
	string name;
	string adr;
} Customer;



/*
*  Function definitions
*/
Customer* create_customer(string, string);
ListItem* add_to_list(ListItem**, string, int);
string view_customer( Customer *);
string view_list( ListItem * );
void delete_item(ListItem*);
void delete_customer(Customer *);

