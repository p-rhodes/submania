//Made by Parker Rhodes
//Functions associated with both my struct for colors, and the SDL_Color struct

#include "sub.h"

Color createNewColor(int r, int g, int b, int a)
{
	Color temp;
	temp.r=r;
	temp.g=g;
	temp.b=b;
	temp.a=a;
	return temp;
}

Color getRandomColor()
{
	Color temp;
	temp.r=rand()%256;
	temp.g=rand()%256;
	temp.b=rand()%256;
	temp.a=rand()%256;
	return temp;
}

void setColorSDL(SDL_Color *color, int r, int g, int b, int a)
{
	color->r=r;
	color->g=g;
	color->b=b;
	color->a=a;
	return;
}
