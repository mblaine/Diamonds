#include <wx/wxprec.h>

#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include "Constants.h"
#include "Globals.h"
#include "SDLPanel.h"
#include "SDLSound.h"

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_util.h"

#include "HiScore.h"
#include "Field.h"
#include "Ball.h"

BEGIN_EVENT_TABLE(SDLPanel, wxPanel)
    EVT_PAINT(SDLPanel::OnPaint)
END_EVENT_TABLE()

SDLPanel::SDLPanel(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
   size = parent->GetClientSize();
                            
   SetMinSize(size);
   SetMaxSize(size);
   
   //init SDL video
   char SDL_windowidhack[64];
   sprintf(SDL_windowidhack, "SDL_WINDOWID=%ld", (long)parent->GetHandle());
   putenv(SDL_windowidhack);
   
   SDL_Init(SDL_INIT_VIDEO);
   screen_unused = SDL_SetVideoMode(WINWIDTH, WINHEIGHT, 24, SDL_SWSURFACE);
   //not sure why this is necessary
   screen_unused->format->Rmask = 0x000000FF;
   screen_unused->format->Bmask = 0x00FF0000;
   
   TTF_Init();
   
   //dimensions must be multiple of 4 to blit using a wxDC
   SDL_Surface* temp = SDL_CreateRGBSurface(SDL_SWSURFACE, size.x+4-size.x%4,
                              size.y+4-size.y%4, 24,  0x000000FF,
			                     0x0000FF00, 0x00FF0000, 0xFF000000);
   screen = SDL_DisplayFormat(temp);
   SDL_FreeSurface(temp);
			                                                   
   borderColors[0] = SDL_RGB(0,204,255);//ltBlue
   borderColors[1] = SDL_RGB(0,36,255); //blue
   borderColors[2] = SDL_RGB(255,39,0); //red
   borderColors[3] = SDL_RGB(0,255,0);  //green
   borderColors[4] = SDL_RGB(146,91,0); //brown
   borderColors[5] = SDL_RGB(204,0,204);//purple
   borderColors[6] = SDL_RGB(255,153,0);//orange
                              
   ballSurface = LoadSurface(wxT("data/ball.png"));
   blockSurface = LoadSurface(wxT("data/block.png"));
   hudSurface = LoadSurface(wxT("data/hud.png"));
   backgroundSurface = LoadSurface(wxT("data/background.png"));
   hudbackgroundSurface = LoadSurface(wxT("data/hudbackground.png"));
   logoSurface = LoadSurface(wxT("data/logo.png"));
   
   //make black pixels transparent
   SDL_SetColorKey(ballSurface, SDL_SRCCOLORKEY, SDL_MapRGB(ballSurface->format,0,0,0));
   SDL_SetColorKey(hudSurface, SDL_SRCCOLORKEY, SDL_MapRGB(ballSurface->format,0,0,0));
      
   font = TTF_OpenFont(wxT("data/diamonds.ttf"), 14);
}

SDLPanel::~SDLPanel()
{
   if(screen)
     SDL_FreeSurface(screen);
   if(ballSurface)
     SDL_FreeSurface(ballSurface);
   if(blockSurface)
     SDL_FreeSurface(blockSurface);
   if(hudSurface)
     SDL_FreeSurface(hudSurface);
   if(backgroundSurface)
     SDL_FreeSurface(backgroundSurface);
   if(hudbackgroundSurface)
     SDL_FreeSurface(hudbackgroundSurface);
   if(logoSurface)
     SDL_FreeSurface(logoSurface);
   if(font)
     TTF_CloseFont(font);

   //shutdown SDL video     
   SDL_FreeSurface(screen_unused);
   TTF_Quit();
   SDL_QuitSubSystem(SDL_INIT_VIDEO);
   if(SDL_WasInit(SDL_INIT_EVERYTHING)==0)
     SDL_Quit();
}

void SDLPanel::OnPaint(wxPaintEvent&)
{
   wxPaintDC dc(this);
   if(gameOn)
     DrawGameScreen(&dc, field);
   else
     DrawStartScreen(&dc, scores);
}

//force a screen repaint outside of an OnPaint event
void SDLPanel::Repaint()
{
   wxClientDC dc(this);
   if(gameOn)
     DrawGameScreen(&dc, field);
   else
     DrawStartScreen(&dc, scores);
}

void SDLPanel::DrawGameScreen(wxWindowDC* dc, Field* field)
{
   if(SDL_MUSTLOCK(screen))
     SDL_LockSurface(screen);

   //border color
   FillRectangle(screen, 0, 0, FIELDWIDTH, 10, borderColors[(field->getBallColor() - 1)]);
   FillRectangle(screen, FIELDWIDTH-10, 0, 10, FIELDHEIGHT, borderColors[(field->getBallColor() - 1)]);
   FillRectangle(screen, 0, FIELDHEIGHT-10, FIELDWIDTH, 10, borderColors[(field->getBallColor() - 1)]);
   FillRectangle(screen, 0, 10, 10, FIELDHEIGHT, borderColors[(field->getBallColor() - 1)]);

   //block field
   for(unsigned int y = 0; y < ARRAYHEIGHT; y++)
   {
     for(unsigned int x = 0; x < ARRAYWIDTH; x++)
     {
       Block block = field->getBlock(y,x);
       if(block != NULL_BLOCK)
         Blit(blockSurface, 0, BLOCKHEIGHT*(block-1), BLOCKWIDTH,
              BLOCKHEIGHT, screen, BLOCKWIDTH*x+10, BLOCKHEIGHT*y+10);
       else
         Blit(backgroundSurface, BLOCKWIDTH*x, BLOCKHEIGHT*y, BLOCKWIDTH,
              BLOCKHEIGHT, screen, BLOCKWIDTH*x+10, BLOCKHEIGHT*y+10);
     }
   }

   //the ball, if it exists
   if(field->gameState!=BALLDIED)
     Blit(ballSurface, 0, ((int)field->getBallColor()-1)*BALLHEIGHT,
          BALLWIDTH, BALLHEIGHT, screen, field->getBallX() + 10, field->getBallY() + 10);

   //hud background
   Blit(hudbackgroundSurface, 0, 0, HUDWIDTH, HUDHEIGHT, screen, 0, FIELDHEIGHT);

   //hud labels
   DrawText(wxT("Score"), font, screen, NUMWIDTH, FIELDHEIGHT+4, 255, 255, 255);
   DrawText(wxString::Format(wxT("Level: %d"), field->currentLevelNumber()), font, screen, NUMWIDTH*8, FIELDHEIGHT+4, 255, 255, 255);
   DrawText(field->currentLevelName().c_str(), font, screen, NUMWIDTH*8, FIELDHEIGHT + NUMHEIGHT/4 + 19, 255, 255, 255);
   DrawText(wxT("Key"), font, screen, (FIELDWIDTH-HASKEYWIDTH)/2-25, FIELDHEIGHT+4, 255, 255, 255);
   DrawText(wxT("Time Bonus"), font, screen, 428, FIELDHEIGHT+4, 255, 255, 255);
   DrawText(wxString::Format(wxT("Balls: %d"), field->getLives()), font, screen, FIELDWIDTH-LIVESWIDTH-20, FIELDHEIGHT+4, 255, 255, 255);

   //score
   int score = field->getScore();
   Blit(hudSurface, 0, 88+(score/100000)*NUMHEIGHT, NUMWIDTH, NUMHEIGHT, screen, NUMWIDTH, FIELDHEIGHT+20);
   Blit(hudSurface, 0, 88+(score%100000/10000)*NUMHEIGHT, NUMWIDTH, NUMHEIGHT, screen, NUMWIDTH*2, FIELDHEIGHT+20);
   Blit(hudSurface, 0, 88+(score%10000/1000)*NUMHEIGHT, NUMWIDTH, NUMHEIGHT, screen, NUMWIDTH*3, FIELDHEIGHT+20);
   Blit(hudSurface, 0, 88+(score%1000/100)*NUMHEIGHT, NUMWIDTH, NUMHEIGHT, screen, NUMWIDTH*4, FIELDHEIGHT+20);
   Blit(hudSurface, 0, 88+(score%100/10)*NUMHEIGHT, NUMWIDTH, NUMHEIGHT, screen, NUMWIDTH*5, FIELDHEIGHT+20);
   Blit(hudSurface, 0, 88+(score%10)*NUMHEIGHT, NUMWIDTH, NUMHEIGHT, screen, NUMWIDTH*6, FIELDHEIGHT+20);

   //has a key?
   if(field->getHasKey())
     Blit(hudSurface, 0, 29, HASKEYWIDTH, HASKEYHEIGHT, screen, (FIELDWIDTH-HASKEYWIDTH)/2-25, FIELDHEIGHT+20);
   else
     Blit(hudSurface, 0, 57, HASKEYWIDTH, HASKEYHEIGHT, screen, (FIELDWIDTH-HASKEYWIDTH)/2-25, FIELDHEIGHT+20);
   
   //time bonus
   //first row, lit-up
   for(int i = 0; i<23 && i<field->getTimeBonus(); i++)
     Blit(hudSurface, 0, 348, BONUSLIGHTWIDTH, BONUSLIGHTHEIGHT, screen, 428+BONUSLIGHTWIDTH*i, FIELDHEIGHT+23);
   //first row, turned-off
   for(int i = field->getTimeBonus(); i<23; i++)
     Blit(hudSurface, 0, 360, BONUSLIGHTWIDTH, BONUSLIGHTHEIGHT, screen, 428+BONUSLIGHTWIDTH*i, FIELDHEIGHT+23);
   //second row, lit-up
   for(int i = 0; i<23 && i<(field->getTimeBonus()-23); i++)
     Blit(hudSurface, 0, 348, BONUSLIGHTWIDTH, BONUSLIGHTHEIGHT, screen, 428+BONUSLIGHTWIDTH*i, FIELDHEIGHT+BONUSLIGHTHEIGHT+23);
   //second row, turned-off
   int j = field->getTimeBonus() - 23;
   if(j<0)
     j=0;
   for( ; j < 23; j++)
     Blit(hudSurface, 0, 360, BONUSLIGHTWIDTH, BONUSLIGHTHEIGHT, screen, 428+BONUSLIGHTWIDTH*j, FIELDHEIGHT+BONUSLIGHTHEIGHT+23);
     
   //lives
   for(int i = 0; i<8; i++)
   {
     if(i<field->getLives())//draw a ball
       Blit(hudSurface, 0,  0, 22, 29, screen, FIELDWIDTH-LIVESWIDTH-20+22*i, FIELDHEIGHT+20); 
   }

   //done
   wxMemoryDC mdc;
   wxBitmap bmp(wxImage(screen->w, screen->h, (unsigned char*)(screen->pixels), true));

   if(SDL_MUSTLOCK(screen))
     SDL_UnlockSurface(screen);

   mdc.SelectObject(bmp);
   dc->Blit(0, 0, size.x, size.y, &mdc, 0, 0);
}

void SDLPanel::DrawStartScreen(wxWindowDC* dc, HiScore* scores)
{
   if(SDL_MUSTLOCK(screen))
     SDL_LockSurface(screen);

   //black background
   SDL_FillRect(screen, NULL, 0);

   //red rectangle
   FillRectangle(screen, 10, 10, WINWIDTH-20, 1, 255, 0, 0);
   FillRectangle(screen, WINWIDTH-10, 10, 1, WINHEIGHT-20, 255, 0, 0);
   FillRectangle(screen, 10, WINHEIGHT-10, WINWIDTH-20+1, 1, 255, 0, 0);
   FillRectangle(screen, 10, 10, 1, WINHEIGHT-20, 255, 0, 0);
   
   //green rectangle
   FillRectangle(screen, 20, 20, WINWIDTH-40, 1, 0, 255, 0);
   FillRectangle(screen, WINWIDTH-20, 20, 1, WINHEIGHT-40, 0, 255, 0);
   FillRectangle(screen, 20, WINHEIGHT-20, WINWIDTH-40+1, 1, 0, 255, 0);
   FillRectangle(screen, 20, 20, 1, WINHEIGHT-40, 0, 255, 0);
   
   //blue rectangle
   FillRectangle(screen, 30, 30, WINWIDTH-60, 1, 0, 0, 255);
   FillRectangle(screen, WINWIDTH-30, 30, 1, WINHEIGHT-60, 0, 0, 255);
   FillRectangle(screen, 30, WINHEIGHT-30, WINWIDTH-60+1, 1, 0, 0, 255);
   FillRectangle(screen, 30, 30, 1, WINHEIGHT-60, 0, 0, 255);

   //logo
   Blit(logoSurface, 0, 0, logoSurface->w, logoSurface->h, 
        screen, (size.x-logoSurface->w)/2, 55);
  
   SDL_Color colors[10] = { {255,0,0,0},//red
                            {241,107,22,0},//dark-orange
                            {255,128,0,0},//orange
                            {255,255,0,0},//yellow
                            {0,183,0,0},//lime-green
                            {0,95,0,0},//green
                            {0,113,87,0},//blue-green
                            {0,156,255,0},//light-blue
                            {0,0,255,0},//blue
                            {128,0,150,0} };//purple
      
   DrawText("Name", font, screen, 160, 150, 255, 255, 255);
   DrawText("Score", font, screen, WINWIDTH/2 - 30, 150, 255, 255, 255);
   DrawText("Levelset", font, screen, WINWIDTH/2 + 80, 150, 255, 255, 255);
   DrawText("Level", font, screen, WINWIDTH/2 + 230, 150, 255, 255, 255);

   //high scores
   for(int i = 0; i < 10; i++)
   {
      DrawText(wxString::Format("%d", i+1).c_str(), font, screen, 110, 180+33*i, colors[i]);
      DrawText(scores->daScores[i].name.c_str(), font, screen, 160, 180+33*i, colors[i]);
      DrawText(wxString::Format("%d", scores->daScores[i].score).c_str(), font, screen, WINWIDTH/2 - 30, 180+33*i, colors[i]);
      DrawText(scores->daScores[i].levelset.c_str(), font, screen, WINWIDTH/2 + 80, 180+33*i, colors[i]);
      DrawText(wxString::Format("%d", scores->daScores[i].level).c_str(), font, screen, WINWIDTH/2 + 230, 180+33*i, colors[i]);
   }

   //done
   wxMemoryDC mdc;
   wxBitmap bmp(wxImage(screen->w, screen->h, (unsigned char*)(screen->pixels), true));

   if(SDL_MUSTLOCK(screen))
     SDL_UnlockSurface(screen);

   mdc.SelectObject(bmp);
   dc->Blit(0, 0, size.x, size.y, &mdc, 0, 0);
}

void SDLPanel::AnimateTimeBonus(SDLSound* sound)
{
   //gonna draw straight to the window
   wxClientDC dc(this);
   wxMemoryDC mdc;
   wxBitmap bmp(wxT("data/hud_timebonus.bmp"), wxBITMAP_TYPE_BMP);
   mdc.SelectObject(bmp);

   //for determining if extra life earned   
   int prevTotal;
   
   //count down remaining time bonus now that a level must've been won
   for(int count = field->getTimeBonus(); count>0; count--)
   {
     prevTotal = field->getScore();
     field->incScore(25);
     field->decTimeBonus();
   
     //score
     int score = field->getScore();
     dc.Blit(NUMWIDTH  , FIELDHEIGHT+20, NUMWIDTH, NUMHEIGHT, &mdc, 0, 30+(score/100000)*NUMHEIGHT);
     dc.Blit(NUMWIDTH*2, FIELDHEIGHT+20, NUMWIDTH, NUMHEIGHT, &mdc, 0, 30+(score%100000/10000)*NUMHEIGHT);
     dc.Blit(NUMWIDTH*3, FIELDHEIGHT+20, NUMWIDTH, NUMHEIGHT, &mdc, 0, 30+(score%10000/1000)*NUMHEIGHT);
     dc.Blit(NUMWIDTH*4, FIELDHEIGHT+20, NUMWIDTH, NUMHEIGHT, &mdc, 0, 30+(score%1000/100)*NUMHEIGHT);
     dc.Blit(NUMWIDTH*5, FIELDHEIGHT+20, NUMWIDTH, NUMHEIGHT, &mdc, 0, 30+(score%100/10)*NUMHEIGHT);
     dc.Blit(NUMWIDTH*6, FIELDHEIGHT+20, NUMWIDTH, NUMHEIGHT, &mdc, 0, 30+(score%10)*NUMHEIGHT);
   
     //time bonus
     //first row, lit-up
     for(int i = 0; i<23 && i<field->getTimeBonus(); i++)
       dc.Blit(428+BONUSLIGHTWIDTH*i+1, FIELDHEIGHT+23+1, BONUSLIGHTWIDTH-2, BONUSLIGHTHEIGHT-2, &mdc, 0, 290);
     //first row, turned-off
     for(int i = field->getTimeBonus(); i<23; i++)
       dc.Blit(428+BONUSLIGHTWIDTH*i+1, FIELDHEIGHT+23+1, BONUSLIGHTWIDTH-2, BONUSLIGHTHEIGHT-2, &mdc, 0, 300);
     //second row, lit-up
     for(int i = 0; i<23 && i<(field->getTimeBonus()-23); i++)
       dc.Blit(428+BONUSLIGHTWIDTH*i+1, FIELDHEIGHT+BONUSLIGHTHEIGHT+23+1, BONUSLIGHTWIDTH-2, BONUSLIGHTHEIGHT-2, &mdc, 0, 290);
     //second row, turned-off
     int j = field->getTimeBonus() - 23;
     if(j<0)
       j=0;
     for( ; j < 23; j++)
       dc.Blit(428+BONUSLIGHTWIDTH*j+1, FIELDHEIGHT+BONUSLIGHTHEIGHT+23+1, BONUSLIGHTWIDTH-2, BONUSLIGHTHEIGHT-2, &mdc, 0, 300);
     
   
     if(prevTotal / 5000 < field->getScore()/5000 )
     {
        field->incLives();
        
        //redraw "lives"
        #if 0
        //not working...
        //label will be updated at start of next level anyway
        SDL_Color color = {0,0,0,0};
        SDL_Surface* temp1 = TTF_RenderText_Solid(font, wxString::Format(wxT("Balls: %d   "), field->getLives()), color);
        SDL_Surface* temp = SDL_DisplayFormat(temp1);
        SDL_FreeSurface(temp1);
        wxBitmap bmpTemp(wxImage(temp->w-temp->w%4, temp->h-temp->h%4, (unsigned char*)(temp->pixels), true));
        mdc.SelectObject(bmpTemp);
        dc.Blit(FIELDWIDTH-LIVESWIDTH-20, FIELDHEIGHT+4, bmpTemp.GetWidth(), bmpTemp.GetHeight(), &mdc, 0, 0);
        mdc.SelectObject(bmp);
        SDL_FreeSurface(temp);
        #endif

        for(int i = 0; i<8; i++)
        {
          if(i<field->getLives())//draw a ball
            dc.Blit(FIELDWIDTH-LIVESWIDTH-20+22*i, FIELDHEIGHT+20, 22, 29, &mdc, 0, 0);
        }

        sound->PlayOneUp();
     }
     else
     {
       sound->PlayTimeBonus();
     }
      
      //wait another 50ms every other time
      wxMilliSleep(120 + (count%2)*50);
   }
}
