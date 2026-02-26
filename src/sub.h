//Made by Parker Rhodes
//Primary header file for Submania development. Mainly just #includes, #defines, typedefs, 
//and function prototypes

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define WINDOW_TITLE "Submania DEMO"
#define SCR_WIDTH 800
#define SCR_HEIGHT 600
#define IMG_FLAG IMG_INIT_PNG	//what image format to initialize processing for
#define SDL_WINDOW_FLAG 0 	//0 for auto, chooses which backend. (metal, vulkan, opengl)
#define RESIZE_FLAG SDL_TRUE
#define TEXT_SIZE 36 //point (not pixel, size)
#define CONSOLE_OUTPUT_ENABLED true 

#define UP 0
#define LEFT 1
#define RIGHT 2
#define DOWN 3

struct Coord {
	int x;
	int y;
};
typedef struct Coord Coord;

struct Node {
	Coord data;
	struct Node *next;
};
typedef struct Node Node;

struct Color {
	int r;
	int g;
	int b;
	int a;
};
typedef struct Color Color;

struct Box {
	//here we go again
	Coord TL;
	Coord TR;
	Coord BL;
	Coord BR;

	Coord center;

	bool isWall;

	//A* variables
	int f; //total cost of the node (box)
	int g; //distance between this box and starting box
	int h; //estimated distance between this box to end box (heuristic)

	struct Box *up;
	struct Box *left;
	struct Box *right;
	struct Box *down;

	//0=up
	//1=left
	//2=right
	//3=down

	struct Box *parent;
};
typedef struct Box Box;

struct boxNode {
	struct Box *data;
	struct boxNode *next;
};
typedef struct boxNode boxNode;

struct pathBool {
	bool pathIsFound;
	int pathSize;
	Box *path[512];
};
typedef struct pathBool pathBool;

struct Game {
	SDL_Window *window;
	SDL_Renderer *renderer;

	SDL_Texture *background;

	TTF_Font *textFont;
	SDL_Color textColor;
	SDL_Rect textRect;
	SDL_Texture *textImage;
	int textVelX;
	int textVelY;

	SDL_Texture *spriteImage;
	SDL_Rect spriteRect;
	int spriteVel;

	const Uint8 *keyState;
	//GameState *currentGameState;
	//
	int playerDepth;

	SDL_Rect o2Level;
	SDL_Texture *o2Texture;

	SDL_Rect o2Machine;
	SDL_Texture *o2MachineTexture;

	SDL_Rect o2MachineLit;
	SDL_Texture *o2MachineLitTexture;
	int currentO2;
};
typedef struct Game Game;

double getDistanceTwoCoords(Coord *a, Coord *b);
Box *getBoxPtrCoords(boxNode *firstBoxNode, int x, int y);
Coord makeNewCoord(int x, int y);
void makeListTLCoords(Node *firstnode);
void drawGrid(Game *game, Node *firstnode);
bool updateO2Text(Game *game, SDL_Color o2Color);
bool createO2Rect(Game *game);
void setColorSDL(SDL_Color *color, int r, int g, int b, int a);
void textUpdate(Game *game);
void spriteUpdate(Game *game);
void spriteUpdateCoords(Game *game, Coord targetPos);
Color createNewColor(int r, int g, int b, int a);
Color getRandomColor();
Game constructNewGame();
bool initialize(Game *game);
bool loadMedia(Game *game);
void shutdown(Game *game, Node *firstnode, boxNode *firstBoxNode, int exitStatus);
Node *makeNewList();
Node *newNode(int x, int y);
void add(Node *firstnode, Coord *coord);
void printList(Node *firstnode);
void delete(Node *firstnode, Coord *coord);
void freeAll(Node *firstnode);
int getSize(Node *firstnode);
Coord *convertListToArray(Node *firstnode, int listSize);
void makeBoxGrid(boxNode *firstBoxNode, Node *firstnode);
void setNeighborsAllBoxes(boxNode *firstBoxNode);
bool moveSpriteLastIndex(Game *game, boxNode *firstBoxNode, int lastIndexClick, int lastIndexPlayer, int gridSize);
Box *getBoxNeighborPtr(int neighbor, boxNode *firstBoxNode, Box *currentBox);
bool isEntireGridVisited(int *distances, int gridSize);
bool isCoordEqual(Coord a, Coord b);
bool isBoxEqual(Box a, Box b);
bool isMemberOf(boxNode *firstBoxNode, Box box); 
int getStepsTwoBoxes(Box a, Box b);
int getDistanceTwoBoxes(Box a, Box b);
void printBox(Box box);
void setBoxParentsNull(boxNode *firstBoxNode);
pathBool moveSpriteTest(Game *game, boxNode *firstBoxNode, Box *playerLocation, Box *clickLocation, Color backgroundColor, Node *firstnode);
//bool moveSpriteTest(boxNode *firstBoxNode, Box *playerLocation, Box *clickLocation);
Box *getFMinFromList(boxNode *firstBoxNode);
void setBox(Box *src, Box *dst);
void setBoxArrayToZero(Box *grid, int gridSize);
void setBoxToZero(Box *box);
boxNode *makeNewBoxList();
boxNode *newBoxNode(Box *box);
void addBox(boxNode *firstBoxNode, Box *box);
void printBoxList(boxNode *firstBoxNode);
void deleteBox(boxNode *firstBoxNode, Box *box, bool doFree);
int getBoxListSize(boxNode *firstBoxNode);
void freeAllBoxes(boxNode *firstBoxNode, bool freeData);
void setAllParentsNull(boxNode *firstBoxNode);
void printPath(Box **path, int pathSize);
void printAllWalls(boxNode *firstBoxNode);
void drawGridWalls(Game *game, boxNode *firstBoxNode);
void setCoord(Coord *coord, int x, int y);
