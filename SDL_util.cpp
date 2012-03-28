#include "SDL_util.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

SDL_Color SDL_RGB(int r, int g, int b)
{
   SDL_Color color = {r,g,b,0};
   return color;
}

SDL_Surface* LoadSurface(const char* filename)
{
  SDL_Surface *temp, *surface;

  temp = IMG_Load(filename);
  if(temp == NULL)
    return NULL;

  surface = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  return surface;  
}

int Blit(SDL_Surface* src, int fromX, int fromY, int width, int height,
         SDL_Surface* dst, int   toX, int   toY)
{
  SDL_Rect srcRect = {fromX, fromY, width, height};
  SDL_Rect dstRect = {  toX,   toY, width, height};
  return SDL_BlitSurface(src, &srcRect, dst, &dstRect);
}

void DrawText(const char* text, TTF_Font* font, SDL_Surface* dest, int x, int y, SDL_Color color)
{
   if(strlen(text)<=0)
     return;

   SDL_Surface* output = TTF_RenderText_Solid(font, text, color);

   Blit(output, 0, 0, output->w, output->h, dest, x, y);
   
   SDL_FreeSurface(output);
}

void DrawText(const char* text, TTF_Font* font, SDL_Surface* dest, int x, int y, int r, int g, int b)
{
   SDL_Color color = {r,g,b,0};
   DrawText(text, font, dest, x, y, color);
}

int FillRectangle(SDL_Surface* dest, int x, int y, int w, int h, SDL_Color color)
{
   return FillRectangle(dest, x, y, w, h, color.r, color.g, color.b);
  
}

int FillRectangle(SDL_Surface* dest, int x, int y, int w, int h, int r, int g, int b)
{
   SDL_Rect rect = {x,y,w,h};
   return SDL_FillRect(dest, &rect, SDL_MapRGBA(dest->format,r,g,b,0));
  
}
