//Made by Parker Rhodes

#include "sub.h"

int main()
{
	Node *firstnode=makeNewList();

	Coord testA=makeNewCoord(12, 23);
	Coord testB=makeNewCoord(10, 55);

	add(firstnode, &testA);
	add(firstnode, &testB);

	printList(firstnode);

	freeAll(firstnode);
	return 0;
}
