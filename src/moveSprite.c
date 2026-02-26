//test version of moveSrite so I don't have to pass it a Game struct

#include "sub.h"

pathBool moveSpriteTest(Game *game, boxNode *firstBoxNode, Box *playerLocation, Box *clickLocation, Color backgroundColor, Node *firstnode)
{
	//for now, a primitive implementation of A* Algorithm that we will optimize
	//later with waypoints. Currently we will analyze the entire grid each time.
	//I am thinking we can add weights to distant boxes as well, since the likelihood of
	//traveling to a distant box being optimal is very little
	
	//we will symbolize unkown (infinite) distances with -1
	
	/*
	 * NEED TO FIX BUG
	 * PLAYER CAN CURRENTLY NOT WALK THROUGH WALLS
	 * BUT WILL PHASE INTO ONE IF IT IS THE FINAL DESTINATION
	 * THINKING: ADD CHECK TO ENSURE DESTINATION DESTINATION TILE IS NOT A WALL BEFORE 
	 * RUNNING ALGORITHM
	*/

	if (clickLocation->isWall) {
		pathBool output;
		output.pathIsFound=false;
		output.pathSize=0;
		return output;
	}
	
	Box *path[512]; //a list of UP/DOWN/LEFT/RIGHT commands to follow
	Box *outputPath[512];
	int pathSize=0;
		       
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

	//printf("Starting while loop\n\n");
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

		if (isBoxEqual(*currentBox, *clickLocation)) {
			pathIsFound=true;
			break;
		}

		if (currentBox->isWall) {
			addBox(firstNodeClosed, currentBox);
			deleteBox(firstNodeOpen, currentBox, false);
			continue;
		}

		addBox(firstNodeClosed, currentBox);
		deleteBox(firstNodeOpen, currentBox, false);

		for (int i=0;i<4;i++) {
			//printf("entering for loop!\n");

			if (i == UP) {
				if (currentBox->up == NULL) {
					continue;
				}

				if (isMemberOf(firstNodeClosed, *currentBox->up)) {
					//printf("Up is member of closedList!\n");
					continue;
				}

				currG=currentBox->g+1;

				if (!isMemberOf(firstNodeOpen, *currentBox->up)) {
					//printf("Up is not in openList!\n");
					addBox(firstNodeOpen, currentBox->up);
				}
	
				else if (currG >= currentBox->up->g) {
					//printf("CurrG is greater than Up->g!\n");
					continue;
				}

				//printf("Up->parent is now currentBox!\n");
				currentBox->up->parent=currentBox;
				currentBox->up->g=currG;
				currentBox->up->h=getDistanceTwoBoxes(*currentBox->up, *clickLocation);
				currentBox->up->f=currentBox->up->g+currentBox->up->h;
			}

			if (i == LEFT) {
				if (currentBox->left == NULL) {
					continue;
				}

				if (isMemberOf(firstNodeClosed, *currentBox->left)) {
					//printf("Left is member of closedList!\n");
					continue;
				}

				currG=currentBox->g+1;

				if (!isMemberOf(firstNodeOpen, *currentBox->left)) {
					//printf("Left is not in openList!\n");
					addBox(firstNodeOpen, currentBox->left);
				}
	
				else if (currG >= currentBox->left->g) {
					//printf("CurrG is greater than Left->g!\n");
					continue;
				}

				//printf("Left->parent is now currentBox!\n");
				currentBox->left->parent=currentBox;
				currentBox->left->g=currG;
				currentBox->left->h=getDistanceTwoBoxes(*currentBox->left, *clickLocation);
				currentBox->left->f=currentBox->left->g+currentBox->left->h;
			}

			if (i == RIGHT) {
				if (currentBox->right == NULL) {
					continue;
				}

				if (isMemberOf(firstNodeClosed, *currentBox->right)) {
					//printf("Right is member of closedList!\n");
					continue;
				}

				currG=currentBox->g+1;

				if (!isMemberOf(firstNodeOpen, *currentBox->right)) {
					//printf("Right is not in openList!\n");
					addBox(firstNodeOpen, currentBox->right);
				}
	
				else if (currG >= currentBox->right->g) {
					//printf("CurrG is greater than Right->g!\n");
					continue;
				}

				//printf("Right->parent is now currentBox!\n");
				currentBox->right->parent=currentBox;
				currentBox->right->g=currG;
				currentBox->right->h=getDistanceTwoBoxes(*currentBox->right, *clickLocation);
				currentBox->right->f=currentBox->right->g+currentBox->right->h;
			}

			if (i == DOWN) {
				if (currentBox->down == NULL) {
					continue;
				}

				if (isMemberOf(firstNodeClosed, *currentBox->down)) {
					//printf("Down is a member of closedList!\n");
					continue;
				}

				currG=currentBox->g+1;

				if (!isMemberOf(firstNodeOpen, *currentBox->down)) {
					//printf("Down is not in openList!\n");
					addBox(firstNodeOpen, currentBox->down);
				}
	
				else if (currG >= currentBox->down->g) {
					//printf("CurrG is greater than Down->g!\n");
					continue;
				}

				//printf("Down->parent is now currentBox!\n");
				currentBox->down->parent=currentBox;
				currentBox->down->g=currG;
				currentBox->down->h=getDistanceTwoBoxes(*currentBox->down, *clickLocation);
				currentBox->down->f=currentBox->down->g+currentBox->down->h;
			}
		}
		//printf("\n");
	}

	//printf("While loop finished\n");
	if (pathIsFound) {
		//printf("Found path!\n");

		//up, left, right, down
		//0, 1, 2, 3
			
		/*if (clickLocation->parent == clickLocation->up) {
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
		pathSize++;*/

		Box *temp=clickLocation;

		while (temp->parent != NULL) {
			if (temp->parent == temp->up) {
				path[pathSize]=temp->up;
			}

			if (temp->parent == temp->left) {
				path[pathSize]=temp->left;
			}
	
			if (temp->parent == temp->right) {
				path[pathSize]=temp->right;
			}
	
			if (temp->parent == temp->down) {
				path[pathSize]=temp->down;
			}

			pathSize++;
			temp=temp->parent;
		}


		//printf("Generated Path before Reversing:\n");
		//printPath(path, pathSize);
		//printf("\n");

		//reverse the path
		for (int i=0;i<pathSize;i++) {
			outputPath[i]=path[pathSize-(i+1)];
			//have to reverse the path since the algorithm provides it
			//target->source and we must move source->target
		}

		outputPath[pathSize]=clickLocation;
		pathSize++;

		//outputPath should now contain the optimal path in the form of ints
		//(which we have #define'd to reference directions)
		
		//printPath(outputPath, pathSize);
		for (int i=0;i<pathSize;i++) {
			spriteUpdateCoords(game, outputPath[i]->center);
			SDL_SetRenderDrawColor(game->renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
			SDL_RenderClear(game->renderer);
			//SDL_RenderCopy(game->renderer, game->background, NULL, NULL);
			
			Coord o2MachineLitPos=makeNewCoord(game->o2Machine.x, game->o2Machine.y);	
			Coord spriteRectPos=makeNewCoord(game->spriteRect.x, game->spriteRect.y);
			
			if (getDistanceTwoCoords(&o2MachineLitPos, &spriteRectPos) <= 50) {
				SDL_RenderCopy(game->renderer, game->o2MachineLitTexture, NULL, &game->o2Machine);
			}

			else {
				SDL_RenderCopy(game->renderer, game->o2MachineTexture, NULL, &game->o2Machine);
			}

			SDL_RenderCopy(game->renderer, game->spriteImage, NULL, &game->spriteRect);
			SDL_RenderCopy(game->renderer, game->o2Texture, NULL, &game->o2Level);
			SDL_RenderCopy(game->renderer, game->textImage, NULL, &game->textRect);

			SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
			drawGrid(game, firstnode);

			SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
			drawGridWalls(game, firstBoxNode);

			SDL_RenderPresent(game->renderer);
			SDL_PumpEvents();
			SDL_Delay(64);	
		}
	}

	/*else if (!pathIsFound) {
		printf("Could not find path!\n");
	}*/

	pathBool output;
	output.pathIsFound=pathIsFound;
	output.pathSize=pathSize;
	for (int i=0;i<pathSize;i++) {
		output.path[i]=outputPath[i];
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
	return output;
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

		grid[i].isWall=false;

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
