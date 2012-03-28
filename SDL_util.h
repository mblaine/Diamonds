#ifndef _SDL_UTIL_H
#define _SDL_UTIL_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

//just a few stand-alone functions for making life a little easier

SDL_Color SDL_RGB(int r, int g, int b);

SDL_Surface* LoadSurface(const char* filename);

int Blit(SDL_Surface* src, int fromX, int fromY, int width, int height,
         SDL_Surface* dst, int   toX, int   toY);

void DrawText(const char* text, TTF_Font* font, SDL_Surface* dest, int x, int y, SDL_Color color);         
void DrawText(const char* text, TTF_Font* font, SDL_Surface* dest, int x, int y, int r, int g, int b);

int FillRectangle(SDL_Surface* dest, int x, int y, int w, int h, SDL_Color color);
int FillRectangle(SDL_Surface* dest, int x, int y, int w, int h, int r, int g, int b);

#endif
