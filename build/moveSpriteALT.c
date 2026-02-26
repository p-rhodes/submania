//test version of moveSrite so I don't have to pass it a Game struct

#include "sub.h"

bool moveSpriteTest(Game *game, boxNode *firstBoxNode, Box *playerLocation, Box *clickLocation)
{
	//for now, a primitive implementation of A* Algorithm that we will optimize
	//later with waypoints. Currently we will analyze the entire grid each time.
	//I am thinking we can add weights to distant boxes as well, since the likelihood of
	//traveling to a distant box being optimal is very little
	
	//we will symbolize unkown (infinite) distances with -1
	
	int path[512]; //a list of UP/DOWN/LEFT/RIGHT commands to follow
		       
	boxNode *firstNodeOpen=makeNewBoxList();
	boxNode *firstNodeClosed=makeNewBoxList();

	//printBox(*playerLocation); //for some reason these are
				   //not getting passed properly???
	//printBox(*clickLocation);

	addBox(firstNodeOpen, playerLocation);

	//set data for the start node (player location)
	firstNodeOpen->next->data->g=0;
	firstNodeOpen->next->data->h=getDistanceTwoBoxes(*playerLocation, *clickLocation);
	firstNodeOpen->next->data->f=firstNodeOpen->next->data->g+firstNodeOpen->next->data->h;
	firstNodeOpen->next->data->parent=NULL;

	int currF=0;
	int currG=0;
	int currH=0;

	bool pathIsFound=false;

	printf("Starting while loop\n\n");
	while (getBoxListSize(firstNodeOpen) > 0) {
		//int openListSize=getBoxListSize(firstNodeOpen);
		//int closedListSize=getBoxListSize(firstNodeClosed);

		//printf("openListSize: <%d>\nclosedListSize: <%d>\n", openListSize, closedListSize);

		/*if (openListSize >= 500 || closedListSize >= 500) {
			//safety button
			break;
		}*/

		Box *currentBox=getFMinFromList(firstNodeOpen);
		//printf("Current Box: %p\n", currentBox);
		//printBox(*currentBox);

		addBox(firstNodeClosed, currentBox);
		deleteBox(firstNodeOpen, currentBox, false);

		if (isBoxEqual(*currentBox, *clickLocation)) {
			pathIsFound=true;
			break;
		}

		for (int i=0;i<4;i++) {
			//printf("entering for loop!\n");

			if (i == UP) {
				if (currentBox->up == NULL) {
					continue;
				}

				if (isMemberOf(firstNodeClosed, *currentBox->up)) {
					printf("Up is member of closedList!\n");
					continue;
				}

				currentBox->up->g=currentBox->g+1;
				currentBox->up->h=getDistanceTwoBoxes(*currentBox->up, *clickLocation);
				currentBox->up->f=currentBox->up->g+currentBox->up->h;
				currG=currentBox->up->g;

				if (isMemberOf(firstNodeOpen, *currentBox->up)) {
					printf("Up is in openList!\n");
					if (currentBox->up->g > currentBox->g) {
						continue;
					}
				}
	
				printf("Up->parent is now currentBox!\n");
				currentBox->up->parent=currentBox;

				addBox(firstNodeOpen, currentBox->down);
			}

			if (i == LEFT) {
				if (currentBox->left == NULL) {
					continue;
				}

				if (isMemberOf(firstNodeClosed, *currentBox->left)) {
					printf("Left is member of closedList!\n");
					continue;
				}

				currentBox->left->g=currentBox->g+1;
				currentBox->left->h=getDistanceTwoBoxes(*currentBox->left, *clickLocation);
				currentBox->left->f=currentBox->left->g+currentBox->left->h;
				currG=currentBox->left->g;

				if (isMemberOf(firstNodeOpen, *currentBox->left)) {
					printf("Left is not in openList!\n");
					if (currentBox->left->g > currentBox->g) {
						continue;
					}
				}
	
				printf("Left->parent is now currentBox!\n");
				currentBox->left->parent=currentBox;

				addBox(firstNodeOpen, currentBox->left);
			}

			if (i == RIGHT) {
				if (currentBox->right == NULL) {
					continue;
				}

				if (isMemberOf(firstNodeClosed, *currentBox->right)) {
					printf("Right is member of closedList!\n");
					continue;
				}

				currentBox->right->g=currentBox->g+1;
				currentBox->right->h=getDistanceTwoBoxes(*currentBox->right, *clickLocation);
				currentBox->right->f=currentBox->right->g+currentBox->right->h;
				currG=currentBox->right->g;

				if (isMemberOf(firstNodeOpen, *currentBox->right)) {
					printf("Right is not in openList!\n");
					if (currentBox->right->g > currentBox->g) {
						continue;
					}
				}
	
				printf("Right->parent is now currentBox!\n");
				currentBox->right->parent=currentBox;

				addBox(firstNodeOpen, currentBox->right);
			}

			if (i == DOWN) {
				if (currentBox->down == NULL) {
					continue;
				}

				if (isMemberOf(firstNodeClosed, *currentBox->down)) {
					printf("Down is a member of closedList!\n");
					continue;
				}

				currentBox->down->g=currentBox->g+1;
				currentBox->down->h=getDistanceTwoBoxes(*currentBox->down, *clickLocation);
				currentBox->down->f=currentBox->down->g+currentBox->down->h;
				currG=currentBox->down->g;

				if (isMemberOf(firstNodeOpen, *currentBox->down)) {
					printf("Down is in openList!\n");
					if (currentBox->down->g > currentBox->g) {
						continue;
					}
				}

				printf("Down->parent is now currentBox!\n");
				currentBox->down->parent=currentBox;

				addBox(firstNodeOpen, currentBox->down);
			}
		}
		printf("\n");
	}

	printf("While loop finished\n");
	if (pathIsFound) {
		printf("Found path!\n");
		int pathSize=0;

		//up, left, right, down
		//0, 1, 2, 3
			
		if (clickLocation->parent == clickLocation->up) {
			path[0]=0;
		}

		if (clickLocation->parent == clickLocation->left) {
			path[0]=1;
		}

		if (clickLocation->parent == clickLocation->right) {
			path[0]=2;
		}

		if (clickLocation->parent == clickLocation->down) {
			path[0]=3;
		}
		pathSize++;

		Box *temp=clickLocation->parent;

		while (temp->parent != NULL) {
			if (clickLocation->parent == clickLocation->up) {
				path[pathSize]=0;
			}

			if (clickLocation->parent == clickLocation->left) {
				path[pathSize]=1;
			}
	
			if (clickLocation->parent == clickLocation->right) {
				path[pathSize]=2;
			}
	
			if (clickLocation->parent == clickLocation->down) {
				path[pathSize]=3;
			}

			pathSize++;
			temp=temp->parent;
		}

		//do we need to run the path code for the last parent, after exiting the loop?

		int outputPath[pathSize+10]; //when he's paranoid about buffer smashing

		//reverse the path
		for (int i=0;i<pathSize;i++) {
			outputPath[i]=path[pathSize-i];
			//have to reverse the path since the algorithm provides it
			//target->source and we must move source->target
		}

		//outputPath should now contain the optimal path in the form of ints
		//(which we have #define'd to reference directions)
		
		temp=clickLocation;

		while (temp->parent != NULL) {
			spriteUpdateCoords(game, temp->parent->center);
			temp=temp->parent;
		}

		printPath(outputPath, pathSize);
	}

	else if (!pathIsFound) {
		printf("Could not find path!\n");
	}

	boxNode *tempOpen=firstNodeOpen->next;
	while (tempOpen != NULL) {
		tempOpen->data=NULL;
		tempOpen=tempOpen->next;
	}

	boxNode *tempClosed=firstNodeClosed->next;
	while (tempClosed != NULL) {
		tempClosed->data=NULL;
		tempClosed=tempClosed->next;
	}

	setAllParentsNull(firstBoxNode);

	freeAllBoxes(firstNodeOpen, false);
	freeAllBoxes(firstNodeClosed, false);
	return pathIsFound;
}

void setAllParentsNull(boxNode *firstBoxNode)
{
	boxNode *temp=firstBoxNode->next;

	while (temp != NULL) {
		temp->data->parent=NULL;
		temp=temp->next;
	}

	return;
}

void setBoxArrayToZero(Box *grid, int gridSize)
{
	for (int i=0;i<gridSize;i++) {
		grid[i].TL.x=0;
		grid[i].TL.y=0;
		grid[i].TR.x=0;
		grid[i].TR.y=0;
		grid[i].BL.x=0;
		grid[i].BL.y=0;
		grid[i].BR.x=0;
		grid[i].BR.y=0;

		grid[i].center.x=0;
		grid[i].center.y=0;

		grid[i].isVisited=false;

		for (int j=0;j<4;j++) {
			if (j == UP) {
				grid[i].up=NULL;
			}
			if (j == LEFT) {
				grid[i].left=NULL;
			}
			if (j == RIGHT) {
				grid[i].right=NULL;
			}
			if (j == DOWN) {
				grid[i].down=NULL;
			}
		}

		grid[i].f=0;
		grid[i].g=0;
		grid[i].h=0;

		grid[i].parent=NULL;
	}
	return;
}
