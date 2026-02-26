//Made by Parker Rhodes
//Code relating to a LinkedList which I am building to store boxes for my SDL renderer

#include "sub.h"

boxNode *makeNewBoxList()
{
	boxNode *temp;
	temp=malloc(sizeof(boxNode));

	Box *data=malloc(sizeof(Box));
	setBoxToZero(data);

	temp->next=NULL;
	temp->data=data;
	return temp;
}

boxNode *newBoxNode(Box *box)
{
	boxNode *temp;
	temp=malloc(sizeof(boxNode));

	temp->data=box;

	temp->next=NULL;
	return temp;
}

void addBox(boxNode *firstBoxNode, Box *box)
{
	//printf("Inside addBox() -> *box: %p\n", box);
	boxNode *newnode=newBoxNode(box);

	newnode->next=firstBoxNode->next;
	firstBoxNode->next=newnode;

	return;
}

void printBoxList(boxNode *firstBoxNode)
{
	boxNode *temp=firstBoxNode->next;
	
	while (temp != NULL) {
		printBox(*temp->data);
		temp=temp->next;
	}
	return;
}

void deleteBox(boxNode *firstBoxNode, Box *box, bool doFree)
{
	boxNode *temp=firstBoxNode;
	boxNode *toDel=temp;
	bool success=false;

	while (temp->next != NULL) {
		if (isBoxEqual(*temp->next->data, *box)) {
			toDel=temp->next;
			temp->next=temp->next->next;
			success=true;
			if (doFree) {
				free(toDel->data);
			}
			free(toDel);
			return;
		}
		temp=temp->next;
	}

	if (!success) {
		fprintf(stderr, "Unable to delete Box from List!\n");
	}
	return;
}

int getBoxListSize(boxNode *firstBoxNode)
{
	int output=0;
	boxNode *temp=firstBoxNode;

	while (temp->next != NULL) {
		output++;
		temp=temp->next;
	}
	return output;
}

void printAllWalls(boxNode *firstBoxNode)
{
	Box *walls[512];
	int wallCount=0;
	boxNode *temp=firstBoxNode->next;

	while (temp != NULL) {
		if (temp->data->isWall) {
			walls[wallCount]=temp->data;
			wallCount++;
		}
		temp=temp->next;
	}

	printf("Total Walls:\n");

	for (int i=0;i<wallCount;i++) {
		printf("%p\n", walls[i]);
	}

	return;
}

void freeAllBoxes(boxNode *firstBoxNode, bool freeData)
{
	boxNode *temp=firstBoxNode->next;
	boxNode *prev=firstBoxNode;
	free(prev->data);

	while (temp != NULL) {
		if (freeData && prev != firstBoxNode) {
			//printf("Inside Free! -> Prev->data: %p\n", prev->data);
			free(prev->data);
		}
		free(prev);

		prev=temp;
		temp=temp->next;
	}
	if (freeData) {
		free(prev->data);
	}
	free(prev);
	return;
}
