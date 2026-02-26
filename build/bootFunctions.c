//Made by Parker Rhodes
//Functions involving booting up Submania

#include "sub.h"

Game constructNewGame() 
{
	Game game;
	game.window=NULL;
	game.renderer=NULL;
	game.background=NULL;
	game.textFont=NULL;
	setColorSDL(&game.textColor, 0, 128, 0, 255);
	game.textRect.x=100;
	game.textRect.y=0;
	game.textRect.w=0;
	game.textRect.h=0;
	game.textImage=NULL;
	game.textVelX=3;
	game.textVelY=3;
	game.spriteImage=NULL;
	game.spriteRect.x=SCR_WIDTH/2;
	game.spriteRect.y=(SCR_HEIGHT/2)+10;
	game.spriteRect.w=0;
	game.spriteRect.h=0;
	game.spriteVel=3;
	game.keyState=SDL_GetKeyboardState(NULL);
	game.playerDepth=0;
	game.o2Level.x=580;
	game.o2Level.y=0;
	game.o2Level.w=40;
	game.o2Level.h=150;
	game.o2Texture=NULL;
	game.currentO2=100;

	game.o2Machine.x=325;
	game.o2Machine.y=275;
	game.o2Machine.w=50;
	game.o2Machine.h=50;
	game.o2MachineTexture=NULL;

	game.o2Machine.x=325;
	game.o2MachineLit.y=275;
	game.o2MachineLit.w=50;
	game.o2MachineLit.h=50;
	game.o2MachineLitTexture=NULL;

	return game;
}

void shutdown(Game *game, Node *firstnode, boxNode *firstBoxNode, int exitStatus)
{
	SDL_DestroyTexture(game->o2Texture);
	SDL_DestroyTexture(game->spriteImage);
	SDL_DestroyTexture(game->textImage);
	TTF_CloseFont(game->textFont);
	SDL_DestroyTexture(game->background);
	SDL_DestroyTexture(game->o2MachineTexture);
	SDL_DestroyTexture(game->o2MachineLitTexture);
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);
	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
	freeAll(firstnode);
	freeAllBoxes(firstBoxNode, true);
	exit(exitStatus);
	return;
}

bool initialize(Game *game)
{
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		fprintf(stderr, "Error initializing SDL!\nError: %s\n", SDL_GetError());
		return true;
	}

	int imgReturnValue=IMG_Init(IMG_FLAG);
	if ((imgReturnValue & IMG_FLAG) != IMG_FLAG) {
		fprintf(stderr, "Error initializing SDL_Image!\nError: %s\n", IMG_GetError());
		return true;
	}

	if (TTF_Init()) {
		fprintf(stderr, "Error initializing SDL_TTF!\nError: %s\n", TTF_GetError());	
		return true;
	}

	game->window=SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_FLAG);

	if (!game->window) {
		fprintf(stderr, "Error Creating Window!\nError: %s\n", SDL_GetError());

		return true;
	}

	game->renderer=SDL_CreateRenderer(game->window, -1, 0);
	if (!game->renderer) {
		fprintf(stderr, "Error Creating Renderer!\nError: %s\n", SDL_GetError());
		return true;
	}

	srand((unsigned)time(NULL)); //seeding rand() with current system time

	return false;
}

bool loadMedia(Game *game)
{
	game->o2MachineLitTexture=IMG_LoadTexture(game->renderer, "o2MachineLit.png");
	if (!game->o2MachineLitTexture) {
		fprintf(stderr, "Error creating o2Img texture!\nError: %s\n", IMG_GetError());
		return true;
	}

	game->o2MachineTexture=IMG_LoadTexture(game->renderer, "o2Machine.png");
	if (!game->o2MachineTexture) {
		fprintf(stderr, "Error creating o2Img texture!\nError: %s\n", IMG_GetError());
		return true;
	}

	game->background=IMG_LoadTexture(game->renderer, "SubMoveTest.png");
	if (!game->background) {
		fprintf(stderr, "Error creating IMG texture!\nError: %s\n", IMG_GetError());
		return true;
	}

	game->textFont=TTF_OpenFont("freesansbold.ttf", TEXT_SIZE);
	if (!game->textFont) {
		fprintf(stderr, "Error loading Font!\nError: %s\n", SDL_GetError());
		return true;
	}

	SDL_Surface *surface=TTF_RenderText_Blended(game->textFont, "E=DOWN, Q=UP", game->textColor); 
	if (!surface) {
		fprintf(stderr, "Error loading surface!\nError: %s\n", SDL_GetError());
		return true;
	}

	game->textRect.w=surface->w;
	game->textRect.h=surface->h;
	game->textImage=SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);
	if (!game->textImage) {
		fprintf(stderr, "Error loading Texture from Surface!\nError: %s\n", SDL_GetError());
		return true;
	}

	game->spriteImage=IMG_LoadTexture(game->renderer, "playerSprite.png");
	if (!game->spriteImage) {
		fprintf(stderr, "Error loading player sprite!\nError: %s\n", IMG_GetError());
		return true;
	}

	if (SDL_QueryTexture(game->spriteImage, NULL, NULL, &game->spriteRect.w, &game->spriteRect.h)) {
		fprintf(stderr, "Error querying Player texture!\nError: %s\n", SDL_GetError());
		return true;
	}

	return false;
}
