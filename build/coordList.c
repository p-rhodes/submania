//Made by Parker Rhodes
//Code relating to a LinkedList which I am building to store coordinates for my SDL renderer

#include "sub.h"

Node *makeNewList()
{

	Node *temp;
	temp=malloc(sizeof(Node));

	temp->next=NULL;
	return temp;
}

Node *newNode(int x, int y)
{
	Node *temp;
	temp=malloc(sizeof(Node));
	temp->next=NULL;
	temp->data.x=x;
	temp->data.y=y;
	return temp;
}

int getSize(Node *firstnode)
{
	//return the size of the coordinate list
	int output=0;
	Node *temp=firstnode;

	while (temp->next != NULL) {
		output++;
		temp=temp->next;
	}
	return output;
}

void add(Node *firstnode, Coord *coord)
{
	Node *temp=firstnode;

	while (temp->next != NULL) {
		if (temp->next == NULL) {
			Node *newnode=newNode(coord->x, coord->y);
			temp->next=newnode;
			return;
		}
		temp=temp->next;
	}

	Node *newnode=newNode(coord->x, coord->y);
	newnode->next=temp->next;
	temp->next=newnode;

	return;
}

void printList(Node *firstnode)
{
	Node *temp=firstnode->next;
	
	while (temp != NULL) {
		printf("(%d, %d)\n", temp->data.x, temp->data.y);
		temp=temp->next;
	}
	return;
}

void delete(Node *firstnode, Coord *coord)
{
	Node *temp=firstnode;
	Node *toDel=firstnode->next;
	int success=0;

	while (temp->next != NULL) {
		if (temp->next->data.x == coord->x && temp->next->data.y == coord->y) {
			toDel=temp->next;
			temp->next=temp->next->next;
			success=1;
			//free(toDel->data);
			free(toDel);
			return;
		}
		temp=temp->next;
	}

	if (success == 0) {
		fprintf(stderr, "Unable to delete Coord from List!\n");
	}
	return;
}

void freeAll(Node *firstnode)
{
	Node *temp=firstnode->next;
	Node *prev=firstnode;

	while (temp != NULL) {
		free(prev);
		prev=temp;
		temp=temp->next;
	}
	free(prev);
	return;
}

/*Coord makeNewCoord(int x, int y)
{
	Coord temp;
	temp.x=x;
	temp.y=y;
	return temp;
}*/ //duplicate for compiling this by itself alongside some test code
