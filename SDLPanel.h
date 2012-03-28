#ifndef _SDLPANEL_H
#define _SDLPANEL_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include "Globals.h"
#include "SDLSound.h"

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_util.h"

class SDLPanel : public wxPanel
{
   private:
      
      SDL_Surface* screen_unused;
      SDL_Surface* screen;
      wxSize size;
      
      SDL_Color borderColors[7];

      SDL_Surface* ballSurface;
      SDL_Surface* blockSurface;
      SDL_Surface* hudSurface;
      SDL_Surface* backgroundSurface;
      SDL_Surface* hudbackgroundSurface;
      SDL_Surface* logoSurface;
      
      TTF_Font* font;
      
      void DrawGameScreen(wxWindowDC* dc, Field* field);
      void DrawStartScreen(wxWindowDC* dc, HiScore* scores);
           
   public:

      SDLPanel(wxWindow* parent);
      ~SDLPanel();
      
      void OnPaint(wxPaintEvent&);
      
      //force a repaint
      void Repaint();
            
      void AnimateTimeBonus(SDLSound* sound);

      DECLARE_EVENT_TABLE()
};

#endif
