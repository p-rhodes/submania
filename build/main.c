//Made by Parker Rhodes

#include "sub.h"

int main()
{
	Game game=constructNewGame();
	Color backgroundColor;
	Node *firstnode=makeNewList();
	makeListTLCoords(firstnode);
	SDL_Color o2Color;
	boxNode *firstBoxNode=makeNewBoxList();
	makeBoxGrid(firstBoxNode, firstnode);
	pathBool moveSpriteOutput;
	moveSpriteOutput.pathIsFound=false;
	Coord o2MachineLitPos;
	Coord spriteRectPos;
	Coord mousePos;
	bool pathError=false;
	int mouseX;
	int mouseY;
	int lastClickX=0;
	int lastClickY=0;
	int rightClickX=0;
	int rightClickY=0;
	int distancePlayerO2=0;
	int listSize=getSize(firstnode);
	Box *lastPlayerLocation=NULL;
	Box *lastClickLocation=NULL;
	Box *lastRightClickLocation=NULL;
	int lastTimeCheck=time(NULL);  //boot time, then the last referenced time
	int timeCheckFrame;		//reference time within each frame
	int currentTime=0; //hours since start. Reduce Oxygen every hour by referencing
			   //current time every frame and every minute since start reduce
			   //o2 (update: every 15 seconds instead of every minute

	if (initialize(&game)) {
		shutdown(&game, firstnode, firstBoxNode, EXIT_FAILURE);
	}
	 
	if (loadMedia(&game)) {
		shutdown(&game, firstnode, firstBoxNode, EXIT_FAILURE);
	}

	/*if (createO2Rect(&game)) {
		shutdown(&game, firstnode, firstBoxNode, EXIT_FAILURE);
	}*/

	SDL_SetWindowResizable(game.window, RESIZE_FLAG);
	backgroundColor=createNewColor(0, 100, 220, 0);
	SDL_SetRenderDrawColor(game.renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	SDL_SetRenderDrawBlendMode(game.renderer, SDL_BLENDMODE_BLEND);
	setColorSDL(&o2Color, 0, 200, 100, 255);

	while (true) {
		timeCheckFrame=time(NULL);
		if (timeCheckFrame-lastTimeCheck >= 3) {
			currentTime++;
			lastTimeCheck=timeCheckFrame;
			game.currentO2-=1;
		}

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					shutdown(&game, firstnode, firstBoxNode, EXIT_SUCCESS);
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.scancode) { 
						case SDL_SCANCODE_ESCAPE:
							shutdown(&game, firstnode, firstBoxNode, EXIT_SUCCESS);
							break;
						case SDL_SCANCODE_E:
							if (game.playerDepth < 2000) {
								game.playerDepth+=10;
							}
							if (backgroundColor.a > 0) {
								backgroundColor.a-=1;
							}
							if (backgroundColor.b > 0) {
								backgroundColor.b-=1;
							}
							if (backgroundColor.g > 0) {
								backgroundColor.g-=1;
							}
							SDL_SetRenderDrawColor(game.renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
							break;
						case SDL_SCANCODE_Q:
							if (game.playerDepth > 0) {
								game.playerDepth-=10;
							}	
							if (backgroundColor.a < 64 && backgroundColor.b >= 120) {
								backgroundColor.a+=1;
							}
							if (backgroundColor.b < 220) {
								backgroundColor.b+=1;
							}
							if (backgroundColor.g < 100 && backgroundColor.b >= 120) {
								backgroundColor.g+=1;
							}
							SDL_SetRenderDrawColor(game.renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
							break;
						default:
							break;
					}
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT) {
						lastClickX=event.button.x;
						lastClickY=event.button.y;
						pathError=false;
						
						lastClickLocation=getBoxPtrCoords(firstBoxNode, lastClickX, lastClickY);
						lastPlayerLocation=getBoxPtrCoords(firstBoxNode, game.spriteRect.x+16, game.spriteRect.y+16);

						if (lastClickLocation != NULL && lastPlayerLocation != NULL) {
							moveSpriteOutput=moveSpriteTest(&game, firstBoxNode, lastPlayerLocation, lastClickLocation, backgroundColor, firstnode);
							//spriteUpdateCoords(&game, lastClickLocation->center);
						}

						else if (lastClickLocation == NULL || lastPlayerLocation == NULL) {
							pathError=true;
						}
					}

					if (event.button.button == SDL_BUTTON_RIGHT) {
						rightClickX=event.button.x;
						rightClickY=event.button.y;

						lastRightClickLocation=getBoxPtrCoords(firstBoxNode, rightClickX, rightClickY);

						if (lastRightClickLocation != NULL) {
							if (lastRightClickLocation->isWall == false) {
								lastRightClickLocation->isWall=true;
							}
	
							else if (lastRightClickLocation->isWall == true) {
								lastRightClickLocation->isWall=false;
							}
						}
					}
					break;
				default:
					break;
			}
		}
		updateO2Text(&game, o2Color);
		//spriteUpdate(&game);
		SDL_GetMouseState(&mouseX, &mouseY);

		setCoord(&o2MachineLitPos, game.o2Machine.x, game.o2Machine.y);
		setCoord(&spriteRectPos, game.spriteRect.x, game.spriteRect.y);
		setCoord(&mousePos, mouseX, mouseY);

		if (CONSOLE_OUTPUT_ENABLED) {
			system("clear");

			printf("Mouse: (%i, %i)\n", mouseX, mouseY);
			printf("Player: (%i, %i)\n", game.spriteRect.x, game.spriteRect.y);
			printf("o2Machine: (%i, %i)\n", game.o2Machine.x, game.o2Machine.y);
			printf("distancePlayerO2: %d\n", distancePlayerO2);

			if (lastPlayerLocation != NULL) {
				printf("lastPlayerLocation:\n");
				printBox(*lastPlayerLocation);
			}

			if (lastClickLocation != NULL) {
				printf("lastClickLocation:\n");
				printBox(*lastClickLocation);
			}

			/*if (lastRightClickLocation != NULL) {
				printBox(*lastRightClickLocation);
			}*/

			printf("Last Click: (%d, %d)\n", lastClickX, lastClickY);
			printf("Last R.Click: (%d, %d)\n", rightClickX, rightClickY);
			printf("Last Path found: %d\n", moveSpriteOutput.pathIsFound);

			if (pathError) {
				printf("Error grabbing player or click box ptr's!\n");
			}

			if (moveSpriteOutput.pathIsFound) {
				printPath(moveSpriteOutput.path, moveSpriteOutput.pathSize);
				//return 0;
			}

			//printAllWalls(firstBoxNode);

			printf("Current Depth: %i\n\n", game.playerDepth);
		}

		SDL_SetRenderDrawColor(game.renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		SDL_RenderClear(game.renderer);

		//SDL_RenderCopy(game.renderer, game.background, NULL, NULL); //draw the submarine
		
		distancePlayerO2=getDistanceTwoCoords(&o2MachineLitPos, &spriteRectPos);
		
		if (distancePlayerO2 <= 50) {
			SDL_RenderCopy(game.renderer, game.o2MachineLitTexture, NULL, &game.o2Machine);
		}

		if (distancePlayerO2 > 50) {
			SDL_RenderCopy(game.renderer, game.o2MachineTexture, NULL, &game.o2Machine);
		}

		SDL_RenderCopy(game.renderer, game.textImage, NULL, &game.textRect);
		SDL_RenderCopy(game.renderer, game.o2Texture, NULL, &game.o2Level);
		SDL_RenderCopy(game.renderer, game.spriteImage, NULL, &game.spriteRect);

		SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
		drawGrid(&game, firstnode); 

		SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
		drawGridWalls(&game, firstBoxNode);

		SDL_RenderPresent(game.renderer);
		SDL_PumpEvents();
		SDL_Delay(16);
	}

	shutdown(&game, firstnode, firstBoxNode, EXIT_SUCCESS);
	return 0;
}
