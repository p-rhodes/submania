#include "sub.h"

//made by Parker Rhodes
//test code to see if my Boxes are correctly working

void main()
{
	printf("%d\n", sizeof(struct Box));
	srand(time(NULL));

	Node *firstnode=makeNewList();
	makeListTLCoords(firstnode);
	int listSize=getSize(firstnode);

	boxNode *firstBoxNode=makeNewBoxList();
	makeBoxGrid(firstBoxNode, firstnode);
	setNeighborsAllBoxes(firstBoxNode);
	int boxListSize=getBoxListSize(firstBoxNode);

	//Coord randomClick=makeNewCoord(rand()%165, rand()%165);
	//Coord randomPlayer=makeNewCoord((rand() / 3)%165, (rand() / 3)%165);
	
	Coord randomClick=makeNewCoord(55, 62);
	Coord randomPlayer=makeNewCoord(250, 275);

	printf("randomclick: (%d, %d)\nrandomplayer: (%d, %d)\n", randomClick.x, randomClick.y, randomPlayer.x, randomPlayer.y);

	Box *playerLocation=getBoxPtrCoords(firstBoxNode, randomPlayer.x, randomPlayer.y);
	Box *clickLocation=getBoxPtrCoords(firstBoxNode, randomClick.x, randomClick.y);

	boxNode *temp=firstBoxNode->next;

	/*while (temp != NULL) {
		printf("%p\n", temp->data);
		printBox(*temp->data);
		temp=temp->next;
	}*/

	printf("listSize: %d\nboxListSize: %d\n", listSize, boxListSize);
	printf("pathIsFound: %d\n", moveSpriteTest(firstBoxNode, playerLocation, clickLocation));
	//setBoxParentsNull(firstBoxNode);
	//printf("RandomClick: %d\nRandomPlayer: %d\n", randomClick, randomPlayer);

	freeAllBoxes(firstBoxNode, true);
	freeAll(firstnode);
	return;
}
