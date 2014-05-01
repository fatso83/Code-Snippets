#include <stdio.h>

struct NODE {
	const char *streng;
	struct NODE *next;
};
struct NODE node_a, node_b, *pNode;

const char tmp[] = "en liten test";

int main()
{
	node_a.streng=tmp;
	node_a.next= &node_b;
	pNode = &node_a;

	puts((*pNode).streng);
	puts(node_a.streng);
	puts(pNode->streng);

	return 0;

}
