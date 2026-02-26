//Made by Parker Rhodes
//Function related to drawing stuff to the screen for Submania
//Exlcluding preliminary stuff and setup that was already done in bootFunctions.c


#include "sub.h"

void makeListTLCoords(Node *firstnode)
{
	//Make a linked list of all 'top left' coordinates in the grid, which we will use to 
	//generate the grid of boxes
	for (int i=50;i<750;i+=50) {
		for (int j=50;j<550;j+=50) {
			Coord temp=makeNewCoord(i, j);
			//printf("Temp: (%d, %d)\n", temp.x, temp.y);
			add(firstnode, &temp);
		}
	}	

	return;
}

double getDistanceTwoCoords(Coord *a, Coord *b)
{
	//printf("A: (%i, %i)\nB: (%i, %i)\n", a->x, a->y, b->x, b->y);

	//(x2-x1)**2+(y2-y2)**2

	long double distance=0;
	double xVal=b->x-a->x;
	double yVal=b->y-a->y;
	
	distance=pow(xVal, 2)+pow(yVal, 2);

	distance=pow(distance, 0.5);

	//printf("DISTANCE: %Lf\n", distance);

	return distance;
}

int getDistanceTwoBoxes(Box a, Box b) //heuristic = estimate between current and end
				      //g = distance between current and start
{
	double distance=getDistanceTwoCoords(&a.center, &b.center);
	distance=distance/50;
	return (int)distance;
}

int getStepsTwoBoxes(Box a, Box b)
{
	int horzSteps=0;
	return horzSteps;
}	

Box *getBoxPtrCoords(boxNode *firstBoxNode, int x, int y)
{
	boxNode *temp=firstBoxNode->next;
	Coord lastClick=makeNewCoord(x, y);
	double distance=0;

	while (temp->next != NULL) {
		distance=getDistanceTwoCoords(&lastClick, &temp->data->center);
		if (distance <= 30) {
			Box *output=temp->data;
			return output;
		}
		temp=temp->next;
	}

	distance=getDistanceTwoCoords(&lastClick, &temp->data->center);

	//35.35
	if (distance <= 30) {
		Box *output=temp->data;
		return output;
	}

	printf("Could not find an associated Box Index\n");
	return NULL;
}

void makeBoxGrid(boxNode *firstBoxNode, Node *firstnode)
{
	Node *temp=firstnode->next;

	while (temp->next != NULL) {
		//do, in fact, need to allocate memory with mallocate
		//but how to do so properly???
		Box *tempBox=malloc(sizeof(struct Box));

		int tempX=temp->data.x;
		int tempY=temp->data.y;

		//printf("tempX: %d\ntempY: %d\n\n", tempX, tempY);

		tempBox->TL.x=tempX;
		tempBox->TL.y=tempY;

		tempBox->TR.x=tempX;
		tempBox->TR.y=tempY;
		tempBox->TR.x+=50;

		tempBox->BL.x=tempX;
		tempBox->BL.y=tempY;
		tempBox->BL.y+=50;

		tempBox->BR.x=tempX;
		tempBox->BR.y=tempY;
		tempBox->BR.x+=50;
		tempBox->BR.y+=50;

		tempBox->center.x=tempX;
		tempBox->center.y=tempY;
		tempBox->center.x+=25;
		tempBox->center.y+=25;
			
		//printf("tempBox->Center: (%d, %d)\n", tempBox->center.x, tempBox->center.y);

		tempBox->isWall=false;
		tempBox->parent=NULL;

		tempBox->up=NULL;
		tempBox->left=NULL;
		tempBox->right=NULL;
		tempBox->down=NULL;

		tempBox->g=0;
		tempBox->h=0;
		tempBox->f=0;

		addBox(firstBoxNode, tempBox);
		
		//free(tempBox);

		temp=temp->next;
	}	

	Box *tempBox=malloc(sizeof(Box));

	int tempX=temp->data.x;
	int tempY=temp->data.y;

	tempBox->TL.x=tempX;
	tempBox->TL.y=tempY;

	tempBox->TR.x=tempX;
	tempBox->TR.y=tempY;
	tempBox->TR.x+=50;

	tempBox->BL.x=tempX;
	tempBox->BL.y=tempY;
	tempBox->BL.y+=50;

	tempBox->BR.x=tempX;
	tempBox->BR.y=tempY;
	tempBox->BR.x+=50;
	tempBox->BR.y+=50;

	tempBox->center.x=tempX;
	tempBox->center.y=tempY;
	tempBox->center.x+=25;
	tempBox->center.y+=25;

	//printf("tempBox->Center: (%d, %d)\n", tempBox->center.x, tempBox->center.y);

	tempBox->isWall=false;

	tempBox->parent=NULL;

	tempBox->g=0;
	tempBox->h=0;
	tempBox->f=0;

	addBox(firstBoxNode, tempBox);

	setNeighborsAllBoxes(firstBoxNode);
	return;
}

void drawGridWalls(Game *game, boxNode *firstBoxNode)
{
	boxNode *temp=firstBoxNode->next;

	while (temp != NULL) {
		if (temp->data->isWall) {
			SDL_Rect wall;
			
			wall.x=temp->data->TL.x;
			wall.y=temp->data->TL.y;
			wall.w=50;
			wall.h=50;

			if (0 != SDL_RenderFillRect(game->renderer, &wall)) {
				fprintf(stderr, "Error drawing Walls!\nError: %s\n", SDL_GetError());
				continue;
			}
		}
		temp=temp->next;
	}

	return;
}

void drawGrid(Game *game, Node *firstnode)
{
	Node *temp=firstnode->next;
	Coord lastDot=makeNewCoord(0, 0);

	while (temp != NULL) {
		if (0 != SDL_RenderDrawPoint(game->renderer, temp->data.x, temp->data.y)) {
			fprintf(stderr, "Error rendering grid!\nError: %s\n", SDL_GetError());
			continue;
		}

		lastDot.x=temp->data.x+50;
		lastDot.y=temp->data.y+50;

		temp=temp->next;
	}	

	if (0 != SDL_RenderDrawPoint(game->renderer, lastDot.x, lastDot.y)) {
		fprintf(stderr, "Error rendering grid!\nError: %s\n", SDL_GetError());
	}

	return;
}

Coord *convertListToArray(Node *firstnode, int listSize)
{
	//DO NOT CALL UNLESS SHUTDOWN (bootFunctions.c) IS UPDATED TO FREE THE MEMORY
	Coord *arr=malloc(listSize*sizeof(Coord));
	Node *temp=firstnode;
	int index=0;

	if (arr == NULL) {
		fprintf(stderr, "Error allocating memory for list to array conversion!\n");
		return NULL;
	}

	while (temp->next != NULL) {
		arr[index]=temp->data;
		temp=temp->next;
		index++;
	}

	return arr;
}

bool updateO2Text(Game *game, SDL_Color o2Color)
{
	if (game->o2Texture != NULL) {
		SDL_DestroyTexture(game->o2Texture);
	}

	char o2[20];
	sprintf(o2, "%d", game->currentO2);
	//printf("o2: <%s>\n", o2);

	SDL_Surface *surface=TTF_RenderText_Blended(game->textFont, o2, o2Color);
	if (!surface) {
		fprintf(stderr, "Error loading Oxygen surface!\nError: %s\n", SDL_GetError());
		return true;
	}

	game->o2Level.w=surface->w;
	game->o2Level.h=surface->h;
	game->o2Texture=SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	if (!game->o2Texture) {
		fprintf(stderr, "Error loading O2 texture!\nError: %s\n", SDL_GetError());
		return true;
	}

	return false;
}

/*bool createO2Rect(Game *game)
{
	SDL_Surface *surface=SDL_CreateRGBSurface(0, game->o2Level.w, game->o2Level.h

	int o2Color=SDL_MapRGBA(surface->format, 180, 10, 40, 32);

	game->o2Texture=SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	if (!game->o2Texture) {
		fprintf(stderr, "Error loading o2 Level!\nError: %s\n", SDL_GetError());
		return true;
	}

	return false;
}*/ //UNDER CONSTRUCTION

void textUpdate(Game *game)
{
	int currScreenWidth;
	int currScreenHeight;

	SDL_GetWindowSize(game->window, &currScreenWidth, &currScreenHeight);
	game->textRect.x+=game->textVelX;
	game->textRect.y+=game->textVelY;

	if (game->textRect.x+game->textRect.w > currScreenWidth) {
		game->textVelX=-3;
	}
	if (game->textRect.x < 0) {
		game->textVelX=3;
	}

	if (game->textRect.y+game->textRect.h > currScreenHeight) {
		game->textVelY=-3;
	}
	if (game->textRect.y < 0) {
		game->textVelY=3;
	}

	return;
}

bool isEntireGridVisited(int *distances, int gridSize)
{
	for (int i=0;i<gridSize;i++) {
		if (distances[i] == -1) {
			return false;
		}
	}
	return true;
}

/*int getBoxIndex(int gridSize, Box *grid, Box target)
{
	for (int i=0;i<gridSize;i++) {
		if (isBoxEqual(grid[i], target)) {
			return i;
		}
	}
	return -1;
}*/

Box *getBoxNeighborPtr(int neighbor, boxNode *firstBoxNode, Box *currentBox)
{
	//returns a pointer to the neighbor (direction based on neighbor value)
	//or -1 if neighbor not found
	
	//currently causing an infinite loop?
	//currently being very undefined lol
	//because we are checking for NULL neighbors
	boxNode *temp=firstBoxNode->next;
	
	if (neighbor == UP) {
		while (temp->next != NULL) {
			if (isBoxEqual(*temp->data, *currentBox)) {
				temp=temp->next;
				continue;
			}
	
			if (isCoordEqual(temp->data->BL, currentBox->TL)) {
				if (isCoordEqual(temp->data->BR, currentBox->TR)) {
					return temp->data;
				}
			}
			temp=temp->next;
		}
		if (!isBoxEqual(*temp->data, *currentBox)) {
			if (isCoordEqual(temp->data->BL, currentBox->TL)) {
				if (isCoordEqual(temp->data->BR, currentBox->TR)) {
					return temp->data;
				}
			}
		}
	}
	if (neighbor == LEFT) {
		while (temp->next != NULL) {
			if (isBoxEqual(*temp->data, *currentBox)) {
				temp=temp->next;
				continue;
			}
	
			if (isCoordEqual(temp->data->TR, currentBox->TL)) {
				if (isCoordEqual(temp->data->BR, currentBox->BL)) {
					return temp->data;
				}
			}
			temp=temp->next;
		}
		if (!isBoxEqual(*temp->data, *currentBox)) {
			if (isCoordEqual(temp->data->TR, currentBox->TL)) {
				if (isCoordEqual(temp->data->BL, currentBox->BL)) {
					return temp->data;
				}
			}
		}
	}
	if (neighbor == RIGHT) {
		while (temp->next != NULL) {
			if (isBoxEqual(*temp->data, *currentBox)) {
				temp=temp->next;
				continue;
			}

			if (isCoordEqual(temp->data->TL, currentBox->TR)) {
				if (isCoordEqual(temp->data->BL, currentBox->BR)) {
					return temp->data;
				}
			}
			temp=temp->next;
		}
		if (!isBoxEqual(*temp->data, *currentBox)) {
			if (isCoordEqual(temp->data->TL, currentBox->TR)) {
				if (isCoordEqual(temp->data->BL, currentBox->BR)) {
					return temp->data;
				}
			}
		}
	}
	if (neighbor == DOWN) {
		while (temp->next != NULL) {
			if (isBoxEqual(*temp->data, *currentBox)) {
				temp=temp->next;
				continue;
			}

			if (isCoordEqual(temp->data->TL, currentBox->BL)) {
				if (isCoordEqual(temp->data->TR, currentBox->BR)) {
					return temp->data;
				}
			}
			temp=temp->next;
		}
		if (!isBoxEqual(*temp->data, *currentBox)) {
			if (isCoordEqual(temp->data->TL, currentBox->BL)) {
				if (isCoordEqual(temp->data->TR, currentBox->BR)) {
					return temp->data;
				}
			}
		}
	}
	return NULL;
}

void setNeighborsAllBoxes(boxNode *firstBoxNode)
{
	boxNode *temp=firstBoxNode->next;

	while (temp->next != NULL) {
		temp->data->up=getBoxNeighborPtr(UP, firstBoxNode, temp->data);
		temp->data->left=getBoxNeighborPtr(LEFT, firstBoxNode, temp->data);
		temp->data->right=getBoxNeighborPtr(RIGHT, firstBoxNode, temp->data);
		temp->data->down=getBoxNeighborPtr(DOWN, firstBoxNode, temp->data);
		
		temp=temp->next;
	}

	temp->data->up=getBoxNeighborPtr(UP, firstBoxNode, temp->data);
	temp->data->left=getBoxNeighborPtr(LEFT, firstBoxNode, temp->data);
	temp->data->right=getBoxNeighborPtr(RIGHT, firstBoxNode, temp->data);
	temp->data->down=getBoxNeighborPtr(DOWN, firstBoxNode, temp->data);

	return;
}

Box *getFMinFromList(boxNode *firstBoxNode)
{
	boxNode *temp=firstBoxNode->next;
	Box *output=temp->data;

	//setBoxToZero(output); //zeros everything out so there's not wacky values (seg faulted)

	while (temp->next != NULL) {
		if (temp->data->f <= output->f) {
			output=temp->data;
		}
		temp=temp->next;
	}

	if (temp->data->f < output->f) {
		output=temp->data;
	}

	return output;
}

/*bool moveSpriteLastIndex(Game *game, Box *grid, int lastIndexClick, int lastIndexPlayer, int gridSize)
{
	//for now, a primitive implementation of A* Algorithm that we will optimize
	//later with waypoints. Currently we will analyze the entire grid each time.
	//I am thinking we can add weights to distance boxes as well, since the likelihood of
	//traveling to a distant box being optimal is very little
	
	//we will symbolize unkown (infinite) distances with -1
	
	int path[512]; //a list of UP/DOWN/LEFT/RIGHT commands to follow
		       
	Box openList[10000];
	int openListSize=0;

	Box closedList[10000];
	int closedListSize=0;

	grid[lastIndexPlayer].g=0;
	grid[lastIndexPlayer].h=getDistanceTwoBoxes(grid[lastIndexPlayer], grid[lastIndexClick]);
	grid[lastIndexPlayer].f=grid[lastIndexPlayer].g+grid[lastIndexPlayer].h;
	grid[lastIndexPlayer].parent=NULL;

	openList[openListSize]=grid[lastIndexPlayer]; //add start to openList
	openListSize++;

	int currF=0;
	int currG=0;
	int currH=0;

	bool pathIsFound=false;

	printf("Starting while loop\n");
	while (openListSize > 0) {
		printf("openListSize: <%d>\nclosedListSize: <%d>\n", openListSize, closedListSize);

		if (openListSize >= 10000 || closedListSize >= 10000) {
			break;
		}

		Box *currentBox=getFMinFromArray(openList, openListSize);

		if (isBoxEqual(*currentBox, grid[lastIndexClick])) {
			pathIsFound=true;
			break;
		}

		closedList[closedListSize]=*currentBox;
		closedListSize++;
		openListSize--;

		for (int i=0;i<4;i++) {
			//printf("entering for loop!\n");

			if (isMemberOf(closedList, grid[currentBox->neighbors[i]], closedListSize)) {
				printf("aha 1\n");
				continue;
			}

			currG=currentBox->g+1;

			if (!isMemberOf(openList, grid[currentBox->neighbors[i]], openListSize)) {
				printf("aha 2\n");
				openList[openListSize]=grid[currentBox->neighbors[i]];
				openListSize++;
			}

			else if (currG >= grid[currentBox->neighbors[i]].g) {
				continue;
			}

			grid[currentBox->neighbors[i]].parent=currentBox;
			grid[currentBox->neighbors[i]].g=currG;
			grid[currentBox->neighbors[i]].h=getDistanceTwoBoxes(grid[currentBox->neighbors[i]], grid[lastIndexClick]);
			grid[currentBox->neighbors[i]].f=grid[currentBox->neighbors[i]].g+grid[currentBox->neighbors[i]].h;
		}
	}

	printf("While loop finished\n");
	if (pathIsFound) {
		printf("pathisfound\n");
		int pathSize=0;
		Box *temp=&grid[lastIndexClick];

		path[0]=getBoxIndex(gridSize, grid, *temp);
		pathSize++;

		while (temp->parent != NULL) {
			path[pathSize]=getBoxIndex(gridSize, grid, *temp->parent);
			pathSize++;
			temp=temp->parent;
		}

		path[pathSize]=getBoxIndex(gridSize, grid, *temp);
		pathSize++;

		int outputPath[pathSize+10];

		//reverse the path
		for (int i=0;i<pathSize;i++) {
			outputPath[i]=path[pathSize-i];
			//have to reverse the path since the algorithm provides it
			//target->source and we must move source->target
		}

		//outputPath should now contain the optimal path in the form of ints
		//(which we have #define'd to reference directions)
		for (int i=0;i<pathSize;i++) {
			if (outputPath[i] == 0) {
				printf("<%d> UP\n", i);		
			}

			if (outputPath[i] == 1) {
				printf("<%d> LEFT\n", i);		
			}

			if (outputPath[i] == 2) {
				printf("<%d> RIGHT\n", i);		
			}

			if (outputPath[i] == 3) {
				printf("<%d> DOWN\n", i);		
			}
		}	
	}	

	setBoxParentsNull(grid, gridSize);
	return pathIsFound;
}*/

void setBoxParentsNull(boxNode *firstBoxNode)
{
	boxNode *temp=firstBoxNode->next;

	while (temp->next != NULL) {
		temp->data->parent=NULL;
		temp=temp->next;
	}

	temp->data->parent=NULL;

	return;
}

bool isBoxEqual(Box a, Box b)
{
	//bool output=false;

	if (isCoordEqual(a.TL, b.TL) && isCoordEqual(a.TR, b.TR)) {
		if (isCoordEqual(a.BL, b.BL) && isCoordEqual(a.BR, b.BR)) {
			return true;
		}
	}
	//printf("Box yay\n");
	return false;
}

bool isMemberOf(boxNode *firstBoxNode, Box box)
{
	if (getBoxListSize(firstBoxNode) == 1) {
		return false;
	}
	
	boxNode *temp=firstBoxNode->next;

	while (temp != NULL) {
		if (isBoxEqual(*temp->data, box)) {
			return true;
		}
		temp=temp->next;
	}

	/*if (isBoxEqual(*temp->data, box)) {
		return true;
	}*/

	return false;
}	

void spriteUpdateCoords(Game *game, Coord targetPos)
{

	Coord relativeTargetPos=makeNewCoord(targetPos.x-16, targetPos.y-16);

	/*while (game->spriteRect.x != relativeTargetPos.x && game->spriteRect.y != relativeTargetPos.y) {
		if (abs(game->spriteRect.x-relativeTargetPos.x) <= 5 && abs(game->spriteRect.y-relativeTargetPos.y) <= 5) {
			game->spriteRect.x=relativeTargetPos.x;
			game->spriteRect.y=relativeTargetPos.y;
			continue;
		}

		if (relativeTargetPos.x > game->spriteRect.x) {
			game->spriteRect.x+=game->spriteVel;
		}
		
		if (relativeTargetPos.x < game->spriteRect.x) {
			game->spriteRect.x-=game->spriteVel;
		}

		if (relativeTargetPos.y > game->spriteRect.y) {
			game->spriteRect.y+=game->spriteVel;
		}

		if (relativeTargetPos.y < game->spriteRect.y) {
			game->spriteRect.y-=game->spriteVel;
		}

		//SDL_Delay(5);
	}*/

	game->spriteRect.x=relativeTargetPos.x;
	game->spriteRect.y=relativeTargetPos.y;

	return;
}
			
void spriteUpdate(Game *game)
{
	if (game->keyState[SDL_SCANCODE_A]) {
		game->spriteRect.x-=game->spriteVel;
	}
	if (game->keyState[SDL_SCANCODE_D]) {
		game->spriteRect.x+=game->spriteVel;
	}
	if (game->keyState[SDL_SCANCODE_W]) {
		game->spriteRect.y-=game->spriteVel;
	}
	if (game->keyState[SDL_SCANCODE_S]) {
		game->spriteRect.y+=game->spriteVel;
	}
	return;
}

void setCoord(Coord *coord, int x, int y)
{
	coord->x=x;
	coord->y=y;
	return;
}

Coord makeNewCoord(int x, int y)
{
	Coord temp;
	temp.x=x;
	temp.y=y;
	return temp;
}

bool isCoordEqual(Coord a, Coord b)
{
	//bool output=false;
	if (a.x == b.x && a.y == b.y) {
		return true;
	}
	//printf("Coord yay!\n");
	return false;
}

void setBox(Box *src, Box *dst)
{
	src->TL=dst->TL;
	src->TR=dst->TR;
	src->BL=dst->BL;
	src->BR=dst->BR;

	src->center=dst->center;

	src->isWall=dst->isWall;

	src->f=dst->f;
	src->g=dst->g;
	src->h=dst->h;

	src->up=dst->up;
	src->left=dst->left;
	src->right=dst->right;
	src->down=dst->down;

	src->parent=dst->parent;

	return;
}

void printBox(Box box)
{
	printf("TL: (%d, %d)\n", box.TL.x, box.TL.y);
	printf("TR: (%d, %d)\n", box.TR.x, box.TR.y);
	printf("BL: (%d, %d)\n", box.BL.x, box.BL.y);
	printf("BR: (%d, %d)\n", box.BR.x, box.BR.y);
	printf("Center: (%d, %d)\n", box.center.x, box.center.y);
	printf("isWall: %d\n", box.isWall);
	printf("Parent address: %p\n", box.parent);
	printf("UP address: %p\n", box.up);
	printf("LEFT address: %p\n", box.left);
	printf("RIGHT address: %p\n", box.right);
	printf("DOWN address: %p\n", box.down);
	printf("F->G->H: %d, %d, %d\n\n", box.f, box.g, box.h);
	return;
}

void setBoxToZero(Box *box)
{
	box->TL.x=0;
	box->TL.y=0;

	box->TR.x=0;
	box->TR.y=0;

	box->BL.x=0;
	box->BL.y=0;

	box->BR.x=0;
	box->BR.y=0;

	box->center.x=0;
	box->center.y=0;

	box->isWall=false;

	box->f=0;
	box->g=0;
	box->h=0;

	box->up=NULL;
	box->left=NULL;
	box->right=NULL;
	box->down=NULL;

	box->parent=NULL;

	return;
}

void printPath(Box **path, int pathSize)
{
	printf("Path From Player:\n");

	for (int i=0;i<pathSize;i++) {
		printf("%p\n", path[i]);
	}
	
	return;
}


