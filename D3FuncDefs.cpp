#include "D3Globals.h"
#include "Resource_Diamonds.h"
#include "Surface.h"
#include <ddraw.h>
#include <dsound.h>



//prototypes
LRESULT  CALLBACK WindowProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam);
BOOL    CALLBACK NameDlgProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam);
BOOL   CALLBACK AboutDlgProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
void DrawScreen(Field *field, Ball *ball);
void DrawMainMenu(HiScore *score);
void RackUpTimeBonus();

int InitDirectDraw();
void CleanUpDirDraw();
bool Flip();

int InitDirectSound();
void CleanUpDirSound();

/*WindowPROC****************************************************************/
LRESULT CALLBACK WindowProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
 switch(msg)
 {
  case WM_CREATE:
          {

           //load images
           readout_bmp = LoadBitmap(hinst1,MAKEINTRESOURCE(READOUT_BMP));
           logo_bmp = LoadBitmap(hinst1,MAKEINTRESOURCE(LOGO_BMP));

           
           //create brushes
           gray = (HBRUSH)GetStockObject(GRAY_BRUSH);
           
           borderBrushes[0] = CreateSolidBrush(RGB(0,204,255));//ltBlue
           borderBrushes[1] = CreateSolidBrush(RGB(0,36,255)); //blue
           borderBrushes[2] = CreateSolidBrush(RGB(255,39,0)); //red
           borderBrushes[3] = CreateSolidBrush(RGB(0,255,0));  //green
           borderBrushes[4] = CreateSolidBrush(RGB(146,91,0)); //brown
           borderBrushes[5] = CreateSolidBrush(RGB(204,0,204));//purple
           borderBrushes[6] = CreateSolidBrush(RGB(255,153,0));//orange

           //create font
           HDC     hdc          = GetDC(hwnd);
           HRSRC   hResource    = FindResource( NULL, 
                                   MAKEINTRESOURCE( FONT_DIAMONDS ),RT_FONT);
           HGLOBAL hGlobal      = LoadResource( NULL, hResource );
           PVOID   temp         = (PVOID)LockResource( hGlobal );
           DWORD   size         = (DWORD)SizeofResource( NULL, hResource );
           DWORD   howMany;     //how many fonts were just created ?????
           HANDLE  fontResource = AddFontMemResourceEx(temp,size,0,&howMany);
           DeleteObject( (HGDIOBJ)hGlobal );
           
           dfont = CreateFont(
                           (int)(-10.5*GetDeviceCaps(hdc, LOGPIXELSY)/72),
                           0,0,0,FW_NORMAL,0,0,0,0,0,0,
                           ANTIALIASED_QUALITY,0,"Diamonds"); 
           ReleaseDC(hwnd,hdc);
           
           //get menu handle
           hmenu = GetMenu(hwnd);
           
           //read in or create new hi scores file
           scores = new HiScore();
           fstream infile("hiscores.dat", ios::in | ios::binary);
           if(infile.is_open())// if opened sucssfully, read in values
           {scores->deserialize(infile);}
           infile.close();//else it'll just have the default values

           PostMessage(hwnd,WM_COMMAND,GAME_SOUNDONOFF,0);

          }
          return 0;
          break;

  case WM_COMMAND:
          switch(LOWORD(wparam))
          {
             case GAME_NEW:
             case KEY_NEW:
                  KillTimer(hwnd,MAINTIMER);
                  KillTimer(hwnd,BONUSTIMER);

                  delete field; field = NULL;
                  field = new Field();
                  
                  gameOn = true;
                  PostMessage(hwnd,WM_PAINT,0,0);
                  
                  SetTimer(hwnd,MAINTIMER,INTERVAL,NULL);
                  SetTimer(hwnd,BONUSTIMER,BONUSINTERVAL,NULL);
                  break;
             
             case GAME_PAUSE:
             case KEY_PAUSE:
                  if(!gameOn){break;}
                  
                  if(!isPaused)
                  {
                   KillTimer(hwnd,MAINTIMER);
                   KillTimer(hwnd,BONUSTIMER);
                   isPaused = true;
                  }
                  else
                  {
                   SetTimer(hwnd,MAINTIMER,INTERVAL,NULL);
                   if(field->getTimeBonus()>0)
                   {SetTimer(hwnd,BONUSTIMER,BONUSINTERVAL,NULL);}
                   isPaused = false;
                   PostMessage(hwnd,WM_PAINT,0,0);
                  }
                  break;
             
             case GAME_RESTARTLEVEL:
             case KEY_RESTART:
                  if(!gameOn){break;}
                  
                  KillTimer(hwnd,MAINTIMER);
                  field->setGameStateBallDied();
                  field->restartLevel();
                  SetTimer(hwnd,MAINTIMER,INTERVAL,NULL);
                  
                  if(isPaused)
                  {
                   isPaused = false;
                   if(field->getTimeBonus()>0)
                   {SetTimer(hwnd,BONUSTIMER,BONUSINTERVAL,NULL);}
                  }
                  PostMessage(hwnd,WM_PAINT,0,0);
                  
                  break;
             
             case GAME_SOUNDONOFF:
                  if(sound || dirSndFailed)
                  {
                   sound = false;
                   CheckMenuItem(hmenu,GAME_SOUNDONOFF,
                                              MF_BYCOMMAND|MF_UNCHECKED);
                  }
                  else
                  {
                   sound = true;
                   CheckMenuItem(hmenu,GAME_SOUNDONOFF,
                                              MF_BYCOMMAND|MF_CHECKED);
                  }
                  break;
             
             case GAME_CLEARSCORES:
                  if(MessageBox(hwnd,
                           "Are you sure you want to clear the high scores?",
                           "Clear High Scores",
                           MB_YESNO | MB_DEFBUTTON2 | MB_ICONWARNING )
                      == IDYES)
                  {
                   delete scores;
                   scores = new HiScore();
                   PostMessage(hwnd,WM_PAINT,0,0);
                  }
                  break;
             
             case GAME_QUIT:
             case KEY_QUIT:
                  if(!gameOn){break;}
                  
                  KillTimer(hwnd,MAINTIMER);
                  KillTimer(hwnd,BONUSTIMER);
                  
                  if(sound)
                  {laughing_snd->Play();}

                  //if high score
                  if(field->getScore() >= scores->daScores[9].score)
                  {DialogBox(hinst1, "EnterName", hwnd, NameDlgProc);}
                  else{Sleep(1000);}
                  delete field; field = NULL;
                  gameOn = false;
                  PostMessage(hwnd, WM_PAINT,0,0);
                  break;

             case GAME_EXIT:
             case KEY_EXIT:
                  PostMessage(hwnd, WM_DESTROY, 0, 0);
                  break;
                  
             case HELP_ABOUT:
                  DialogBox(hinst1, "About", hwnd, AboutDlgProc);
                  break;
             
             default:
                     break;
          }
          return 0;
          break;
  case WM_PAINT:
          {
            InvalidateRect(hwnd,NULL,TRUE);//invalidate entire rectangle
            if(gameOn)
            {DrawScreen(field,field->getBallPtr());}
            else
            {DrawMainMenu(scores);}
          }
          return 0;
          break;

  case WM_TIMER: 
          if(wparam==MAINTIMER)//if it's the main game timer
          {
          //if playing
          if(field->getGameState() == PLAYING)
          {
           //left key down
           if(GetAsyncKeyState(VK_LEFT) & 0x8000)
           { field->setToMoveLeft(); }
           //right key down
           else if(GetAsyncKeyState(VK_RIGHT) & 0x8000)
           { field->setToMoveRight(); }

           field->moveBall();

          }//end if playing

          //if ball died
          else if(field->getGameState() == BALLDIED)
          {
           KillTimer(hwnd,MAINTIMER);

           if(field->getLives() <= 0)
           {
                field->setGameStateGameOver();
                KillTimer(hwnd,BONUSTIMER);
                KillTimer(hwnd,MAINTIMER);
                
                if(sound)
                {laughing_snd->Play();}
                
                //if a high score
                if(field->getScore() >= scores->daScores[9].score)
                {DialogBox(hinst1, "EnterName", hwnd, NameDlgProc);}
                else
                {Sleep(1000);}
                
                gameOn = false;
                PostMessage(hwnd,WM_PAINT,0,0);
           }
           
           else//has lives left, game continues...
              {                
                if(sound)
                {die_snd->Play();}
                
                Sleep(1000);
                field->decLives();
                
                field->resetBall();
                field->setGameStatePlaying();
                SetTimer(hwnd,MAINTIMER,INTERVAL,NULL);
              }

          }// end if ball died
          
          //if level won
          else if(field->getGameState() == LEVELWON)
          {    
               if(field->getCurrentLevel() < LASTLEVEL)
               {
                KillTimer(hwnd,MAINTIMER);
                KillTimer(hwnd,BONUSTIMER);
                
                if(sound)
                {levelwon_snd->Play();}
                Sleep(1200);
                
                RECT readouts;//area of readouts at bottom of screen
                readouts.top = VERTINDENT + FIELDHEIGHT;
                readouts.bottom = VERTINDENT + FIELDHEIGHT + READOUTSHEIGHT;
                readouts.left = HORIZINDENT;
                readouts.right = HORIZINDENT + READOUTSWIDTH;
                InvalidateRect(hwnd,&readouts,FALSE);//invlidate area
                RackUpTimeBonus();
                
                Sleep(1000);
                field->nextLevel();
                SendMessage(hwnd,WM_PAINT,0,0);
                Sleep(1000);
                
                SetTimer(hwnd,MAINTIMER,INTERVAL,NULL);                
                SetTimer(hwnd,BONUSTIMER,BONUSINTERVAL,NULL);
               }
               else//no more levels
               {
                KillTimer(hwnd,MAINTIMER);
                KillTimer(hwnd,BONUSTIMER);
                
                if(sound)
                {levelwon_snd->Play();}
                Sleep(1200);
                
                RECT readouts;//area of readouts at bottom of screen
                readouts.top = VERTINDENT + FIELDHEIGHT;
                readouts.bottom = VERTINDENT + FIELDHEIGHT + READOUTSHEIGHT;
                readouts.left = HORIZINDENT;
                readouts.right = HORIZINDENT + READOUTSWIDTH;
                InvalidateRect(hwnd,&readouts,FALSE);//invlidate area
                RackUpTimeBonus();
                
                Sleep(1000);
                field->setGameStateGameOver();
                
                //if a high score
                if(field->getScore() >= scores->daScores[9].score)
                {DialogBox(hinst1, "EnterName", hwnd, NameDlgProc);}
                else
                {Sleep(1000);}
                
                gameOn = false;
                PostMessage(hwnd,WM_PAINT,0,0);
               }
           }//end if level won

           DrawScreen(field,field->getBallPtr());//draw screen
          }//end if MAINTIMER

          //if it's the bonus timer
          if(wparam==BONUSTIMER)
          {
            field->decTimeBonus();
            if(field->getTimeBonus()<=0)
            {KillTimer(hwnd,BONUSTIMER);}
          }

          return 0;
          break;//end case WM_TIMER
          
  case WM_DESTROY:
          {    
           KillTimer(hwnd,MAINTIMER);
           KillTimer(hwnd,BONUSTIMER);
           
           CleanUpDirDraw();
           
           //if it failed to begin with, no need to clean up
           if(!dirSndFailed)
            CleanUpDirSound();

           //clean up font
           DeleteObject(dfont);
           RemoveFontMemResourceEx(fontResource);

           //saving 'scores' to file
           fstream outfile("hiscores.dat",ios::out|ios::binary);
           if(outfile.is_open())// if opened/created sucssfully, save values
           {scores->serialize(outfile);}
           outfile.close();//on error, just do nothing

           delete field; field = NULL;
           delete scores; scores = NULL;
           DeleteObject(readout_bmp);
           DeleteObject(logo_bmp);
           DeleteObject(gray);

           for(int i =0; i<7; i++)
           {DeleteObject(borderBrushes[i]);}

           PostQuitMessage(0);
          }
          return 0;
          break;
             
  default: break;
 
 }//end primary switch
        
  return (DefWindowProc(hwnd,msg,wparam,lparam));
}//end WindowProc////////////////////////////////////////////////////////////




/*Enter Name Dialog Procedure***********************************************/
BOOL CALLBACK NameDlgProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
switch(msg)
    {
        case WM_INITDIALOG:
            SetDlgItemText(hwnd, NAME_INPUT, "");
            return TRUE;
            break;
        
        case WM_COMMAND:
            switch(LOWORD(wparam))
            {
              case IDOK:
                     
              //enter name into 'scores'
              
              //string to feed to set method
              string temp;

              //get length of screen in text box
              int len=GetWindowTextLength(GetDlgItem(hwnd,NAME_INPUT));
              
              if(len > 0)//if user input something
              {
                char* inData;//to hold input
                inData=(char*)GlobalAlloc(GPTR,len+1);//alocate char*
                GetDlgItemText(hwnd,NAME_INPUT,inData,len+1);//fill it
                temp=inData;//feed to c++ string
                GlobalFree((HANDLE)inData);//free memory for input
              }
              else{temp="----------";}
              
              temp = temp.substr(0,20);//limit name to 20 characters
              
              //enter the high score as a 'score' struct
              score toEnter = { temp,
                                field->getScore(),
                                field->getCurrentLevel() };
              scores->enterScore(toEnter);

              EndDialog(hwnd, IDOK);
              break;
            }
            return TRUE;
            break;
            
        case WM_CLOSE:
            EndDialog(hwnd, 0);
            return TRUE;
            break;
        
        default:
            return FALSE;
            break;
    }
}//END ENTER NAME DIALOG PROCEDURE///////////////////////////////////////////


/*About Dialog Procedure****************************************************/
BOOL CALLBACK AboutDlgProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK:
					EndDialog(hwnd, IDOK);
				break;
				
				case IDCANCEL:
					EndDialog(hwnd, IDCANCEL);
				break;
			}
		break;
		default:
			return FALSE;
	}
	return TRUE;
}//END ABOUT DIALOG PROCEDURE////////////////////////////////////////////////


/*DrawSCREEN****************************************************************/
void DrawScreen(Field *field, Ball *ball)//to draw the screen while a game
//              is in play
{
 //Draw(toSurface,toX,toY,fromX,fromY,width=0,height=0)
 //BitBlt(to DC,to X,to Y,image width,height,from DC,from X,from Y,flags);

 if(SurfaceFront->IsLost() || SurfaceBack->IsLost())
 {
 CleanUpDirDraw();
 InitDirectDraw();
 }

 HDC dXdc;//device context from DirectX
 SurfaceBack->GetDC(&dXdc);
 
 //select font into DC
 HFONT hfOld = (HFONT)SelectObject(dXdc, dfont);

 //drawing color for screen border
 int index;
 if(ball!=NULL)
 {index = (int)ball->getColor() - 1;}
 else
 {index = 0;}
 //border color; gotta catch old brush
 HBRUSH hbrushOld =(HBRUSH)SelectObject(dXdc,borderBrushes[index]);
 Rectangle(dXdc, 0, 0, FIELDWIDTH, FIELDHEIGHT+1);
 
 //color 'readout' brush gray
 SelectObject(dXdc, gray);//bkgrd color
 Rectangle(dXdc, 0, FIELDHEIGHT, READOUTSWIDTH,
                       FIELDHEIGHT+READOUTSHEIGHT);

 //write labels for displays
 char temp[20];
 SetBkMode(dXdc,TRANSPARENT);//let the background be transparent

 TextOut(dXdc,NUMWIDTH,FIELDHEIGHT,"Score",5);

 sprintf(temp,"Level: %d",field->getCurrentLevel());
 TextOut(dXdc,NUMWIDTH*8,FIELDHEIGHT,temp,strlen(temp));
 
 TextOut(dXdc,NUMWIDTH*8,FIELDHEIGHT+VERTINDENT*3.0/2.0 + NUMHEIGHT/4,
              field->getLevelName().c_str(),field->getLevelName().size() );


 TextOut(dXdc, (FIELDWIDTH-HASKEYWIDTH)/2,FIELDHEIGHT,
        "Key", 3);

 TextOut(dXdc,READOUTSWIDTH*.546,FIELDHEIGHT,"Time Bonus", 10);
 sprintf(temp,"Balls: %d",field->getLives());
 TextOut(dXdc,FIELDWIDTH-HORIZINDENT*2-LIVESWIDTH,
        FIELDHEIGHT, temp, strlen(temp));

 //cleaning up
 SelectObject(dXdc,hbrushOld);
 SelectObject(dXdc,hfOld);
 SurfaceBack->ReleaseDC(dXdc);
 




 //drawing background image

 bkgrnd_surf->Draw(SurfaceBack,HORIZINDENT,VERTINDENT,0,0,792,480);
 
 //drawing field of blocks
 if(field != NULL)//if field exists
 {
 //draw all blocks to temporary bitmap in secondary buffer
 for(int y = 0; y < ARRAYHEIGHT; y++)//for all rows
 {
  for(int x = 0; x < ARRAYWIDTH; x++)//for all cells in current row
  {
   if(field->getBlock(y,x) != NULL_BLOCK)//if block exists;
   {
    Block info  = field->getBlock(y,x);//need da info
    
    int drawToX, drawToY;//coordinates to draw to
    drawToX = BLOCKWIDTH * x + HORIZINDENT;
    drawToY = BLOCKHEIGHT * y + VERTINDENT;
    
    if(info == DIAMOND)
    { blocks_surf->Draw(SurfaceBack, drawToX, drawToY, 0, 560,
                         BLOCKWIDTH, BLOCKHEIGHT);}
    else if(info == SKULL)
    { blocks_surf->Draw(SurfaceBack, drawToX, drawToY, 0, 0,
                         BLOCKWIDTH, BLOCKHEIGHT);}
    else if(info == KEY)
    { blocks_surf->Draw(SurfaceBack, drawToX, drawToY, 0, 600,
                         BLOCKWIDTH, BLOCKHEIGHT);}
    else if(info == LOCK)
    { blocks_surf->Draw(SurfaceBack, drawToX, drawToY, 0, 640, 
                         BLOCKWIDTH, BLOCKHEIGHT);}
    else if(info == SOLID)
    { blocks_surf->Draw(SurfaceBack, drawToX, drawToY, 0, 520,
                         BLOCKWIDTH, BLOCKHEIGHT);}
    else if(info == REVERSE)
    { blocks_surf->Draw(SurfaceBack, drawToX, drawToY, 0, 680,
                         BLOCKWIDTH, BLOCKHEIGHT);}
    //the math below is to align w/ image in bitmap
    else if(info >= BRUSH_BLUE && info <= BRUSH_ORANGE)
    { 
      Color c = (Color)(info-BRUSH_BLUE + 2); //convert between enumerations
      blocks_surf->Draw(SurfaceBack, drawToX, drawToY,
                        0, 280 + BLOCKHEIGHT*(c-2),
                        BLOCKWIDTH, BLOCKHEIGHT );
    }
    //the math below is to align w/ image in bitmap
    else if(info >= COLOR_LTBLUE && info <= COLOR_PURPLE)
    { Color c = (Color)(info-COLOR_LTBLUE + 1); //convert between enum's
      blocks_surf->Draw(SurfaceBack, drawToX, drawToY,
                         0, 0 + BLOCKHEIGHT*(c),
                         BLOCKWIDTH, BLOCKHEIGHT);
    }
     //end cycling through possible types
   }//end if ptr not null
  }//end for every x 
 }//end for every y
}//end if field exists
 //done drawing blocks
 
  //NOW.. the ball, only if it exists
 if(ball != NULL && field->getGameState()!=BALLDIED)
 {
 //copy ball from memory DC to correct place in temporary bitmap
 ball_surf->Draw(SurfaceBack, ball->getXpos() + HORIZINDENT, 
                    ball->getYpos() + VERTINDENT,
                    0, ((int)ball->getColor()-1)*BALLHEIGHT,
                    BALLWIDTH, BALLHEIGHT);
 }
 
  //NOW... the readouts at the bottom of the screen

 //draw # of lives readout
 for(int i = 0; i<8; i++)
 {
 if(i<field->getLives())//draw a ball
 {readout_surf->Draw(SurfaceBack,
    FIELDWIDTH-HORIZINDENT*2-LIVESWIDTH + (BALLWIDTH+4)*i,
    FIELDHEIGHT+VERTINDENT*2,
    0,0,
    BALLWIDTH+4,BALLHEIGHT+11);}
 else//draw a blank
 {readout_surf->Draw(SurfaceBack,
    FIELDWIDTH-HORIZINDENT*2-LIVESWIDTH + (BALLWIDTH+4)*i,
    FIELDHEIGHT+VERTINDENT*2,
    0,60,
    BALLWIDTH+4,BALLHEIGHT+11);}
 }
 
  //the has a key? readout
 if(field!=NULL)
 {
    if(field->getHasKey())
    {readout_surf->Draw(SurfaceBack,(FIELDWIDTH-HASKEYWIDTH)/2,
                                     FIELDHEIGHT+VERTINDENT*2,
                                     0,29,HASKEYWIDTH,HASKEYHEIGHT);}
    else
    {readout_surf->Draw(SurfaceBack,(FIELDWIDTH-HASKEYWIDTH)/2,
                                     FIELDHEIGHT+VERTINDENT*2,
                                     0,57,HASKEYWIDTH,HASKEYHEIGHT);}
 }
 else
 {readout_surf->Draw(SurfaceBack,(FIELDWIDTH-HASKEYWIDTH)/2,
                                  FIELDHEIGHT+VERTINDENT*2,
                                  0,57,HASKEYWIDTH,HASKEYHEIGHT);}

 //the score readout
 int number;//the score to print
 if(field!=NULL)
 {number = field->getScore();}
 else
 {number = 0;}
 int hundThou = number / 100000;
 int tenThou = (number % 100000) / 10000;
 int thou = (number % 10000) / 1000;
 int hund = (number % 1000) / 100;
 int ten = (number % 100) / 10;
 int one = number % 10;
 readout_surf->Draw(SurfaceBack,NUMWIDTH,FIELDHEIGHT+VERTINDENT*2,
                 0,88+hundThou*NUMHEIGHT,NUMWIDTH,NUMHEIGHT);
 readout_surf->Draw(SurfaceBack,NUMWIDTH*2,FIELDHEIGHT+VERTINDENT*2,
                 0,88+tenThou*NUMHEIGHT,NUMWIDTH,NUMHEIGHT);
 readout_surf->Draw(SurfaceBack,NUMWIDTH*3,FIELDHEIGHT+VERTINDENT*2,
                 0,88+thou*NUMHEIGHT,NUMWIDTH,NUMHEIGHT);
 readout_surf->Draw(SurfaceBack,NUMWIDTH*4,FIELDHEIGHT+VERTINDENT*2,
                 0,88+hund*NUMHEIGHT,NUMWIDTH,NUMHEIGHT);
 readout_surf->Draw(SurfaceBack,NUMWIDTH*5,FIELDHEIGHT+VERTINDENT*2,
                 0,88+ten*NUMHEIGHT,NUMWIDTH,NUMHEIGHT);
 readout_surf->Draw(SurfaceBack,NUMWIDTH*6,FIELDHEIGHT+VERTINDENT*2,
                 0,88+one*NUMHEIGHT,NUMWIDTH+1,NUMHEIGHT);
                                                   //+1 to get the border
 
 //time bonus
 //first row, lit-up
 for(int i = 0; i<23 && i<field->getTimeBonus(); i++)
 {readout_surf->Draw(SurfaceBack,READOUTSWIDTH*.546+BONUSLIGHTWIDTH*i,
                    FIELDHEIGHT+VERTINDENT*2 + 3,
                    0,348,
                    BONUSLIGHTWIDTH,BONUSLIGHTHEIGHT);}
 //first row, turned-off
 for(int i = field->getTimeBonus(); i<23; i++)
 {readout_surf->Draw(SurfaceBack,READOUTSWIDTH*.546+BONUSLIGHTWIDTH*i,
                    FIELDHEIGHT+VERTINDENT*2 + 3,
                    0,360,
                    BONUSLIGHTWIDTH,BONUSLIGHTHEIGHT);}
 //second row, lit-up
 for(int i = 0; i<23 && i<(field->getTimeBonus()-23); i++)
 {readout_surf->Draw(SurfaceBack,READOUTSWIDTH*.546+BONUSLIGHTWIDTH*i,
                    FIELDHEIGHT+VERTINDENT*2+BONUSLIGHTHEIGHT + 3,
                    0,348,
                    BONUSLIGHTWIDTH,BONUSLIGHTHEIGHT);}
 //second row, turned-off
 int j = field->getTimeBonus() - 23;
 if(j<0){j=0;}
 for( j; j < 23; j++)
 {readout_surf->Draw(SurfaceBack,READOUTSWIDTH*.546+BONUSLIGHTWIDTH*j,
                    FIELDHEIGHT+VERTINDENT*2+BONUSLIGHTHEIGHT + 3,
                    0,360,
                    BONUSLIGHTWIDTH,BONUSLIGHTHEIGHT);}


 Flip();
 ValidateRect(myHandle,NULL);
}//end DrawScreen()/////////////////////////////////////////////////////////




/*DrawMainMENU()************************************************************/
void DrawMainMenu(HiScore *score)//draws main game menu when no game in play
{
//BitBlt(to DC,to X,to Y,image width,height,from DC,from X,from Y,flags);
 
 PAINTSTRUCT ps;
 HDC hdc = BeginPaint(myHandle, &ps);//screen(visible) HDC
 HDC hdcMem = CreateCompatibleDC(hdc);//couterpart HDC in memory
 HDC hdcBuffer2 = CreateCompatibleDC(hdc);//secondary buffer HDC
 
 
 //screen sized bitmap for 2nd buffer
 HBITMAP tempBitmap = CreateCompatibleBitmap(hdc, FIELDWIDTH,
                                         FIELDHEIGHT + READOUTSHEIGHT);
 //catch old bitmap from secondary buffer as our screen sized one goes in
 HBITMAP hbmOld2 =(HBITMAP) SelectObject(hdcBuffer2, tempBitmap);
 
 //drawing screen border
 HPEN redPen = CreatePen(PS_SOLID,0,RGB(255,0,0));
 HPEN greenPen = CreatePen(PS_SOLID,0,RGB(0,255,0));
 HPEN bluePen = CreatePen(PS_SOLID,0,RGB(0,0,255));

      //red rectangele
 HPEN oldPen = (HPEN)SelectObject(hdcBuffer2, redPen);
 MoveToEx(hdcBuffer2,10,10,NULL);
 LineTo(hdcBuffer2,FIELDWIDTH-10,10);
 LineTo(hdcBuffer2,FIELDWIDTH-10,FIELDHEIGHT+READOUTSHEIGHT-10);
 LineTo(hdcBuffer2,10,FIELDHEIGHT+READOUTSHEIGHT-10);
 LineTo(hdcBuffer2,10,10);
 
       //green rectangele
 SelectObject(hdcBuffer2, greenPen);
 MoveToEx(hdcBuffer2,20,20,NULL);
 LineTo(hdcBuffer2,FIELDWIDTH-20,20);
 LineTo(hdcBuffer2,FIELDWIDTH-20,FIELDHEIGHT+READOUTSHEIGHT-20);
 LineTo(hdcBuffer2,20,FIELDHEIGHT+READOUTSHEIGHT-20);
 LineTo(hdcBuffer2,20,20);
 
        //blue rectangele
 SelectObject(hdcBuffer2, bluePen);
 MoveToEx(hdcBuffer2,30,30,NULL);
 LineTo(hdcBuffer2,FIELDWIDTH-30,30);
 LineTo(hdcBuffer2,FIELDWIDTH-30,FIELDHEIGHT+READOUTSHEIGHT-30);
 LineTo(hdcBuffer2,30,FIELDHEIGHT+READOUTSHEIGHT-30);
 LineTo(hdcBuffer2,30,30);

 //catch old bitmap as our logo bitmap goes into memory DC
 HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, logo_bmp);
 int toDrawAtX;
 toDrawAtX = 175+HORIZINDENT;
 
 BitBlt(hdcBuffer2,toDrawAtX,43,414,89,hdcMem,0,0,SRCCOPY);
 
 HFONT hfOld = (HFONT)SelectObject(hdcBuffer2,dfont);//select Diamonds font
 SetBkMode(hdcBuffer2, TRANSPARENT);//make text transparent
 
 //array of COLORREF's to draw high scores
 COLORREF colors[10] = { RGB(216,47,0),//dark red
                         RGB(255,0,0),//red
                         RGB(255,128,0),//orange
                         RGB(255,192,0),//yellow-orange
                         RGB(255,255,0),//yellow
                         RGB(0,183,0),//lime-green
                         RGB(0,128,0),//green
                         RGB(0,113,87),//blue-green
                         RGB(0,0,255),//blue
                         RGB(128,0,150) };//purple
 
 //set my color, catch old one
 COLORREF oldTextColor = SetTextColor(hdcBuffer2,RGB(128,128,128));//gray
 
 //draw byline
 string by ="Created by Matt Blaine, Nov. 2005.     Based on ";
 by+="\"Diamonds\" ©1992 Varcon Systems, Inc.";
 TextOut(hdcBuffer2,HORIZINDENT*5,
          FIELDHEIGHT+READOUTSHEIGHT-6*VERTINDENT,by.c_str(),by.size());
 //draw column labels
 SetTextColor(hdcBuffer2,RGB(255,255,255));//white
 TextOut(hdcBuffer2,HORIZINDENT*21,
                               VERTINDENT*15,"Name",4);
 TextOut(hdcBuffer2,FIELDWIDTH/2,VERTINDENT*15,"Score",5);
 TextOut(hdcBuffer2,FIELDWIDTH/2+HORIZINDENT*16,VERTINDENT*15,"Level",5);
 
 //draw high scores
 for(int i = 0; i < 10; i++)
 {
  SetTextColor(hdcBuffer2,colors[i]);//set text color
  char temp[10];
  string tempstr;
  itoa(i+1,temp,10);
  tempstr = temp;
  //# 1 - 10
  TextOut(hdcBuffer2,HORIZINDENT*16,
                                VERTINDENT*(18+3*i),
           tempstr.c_str(),tempstr.size());
  //name
  TextOut(hdcBuffer2,HORIZINDENT*21,
                                VERTINDENT*(18+3*i),
           score->daScores[i].name.c_str(), score->daScores[i].name.size() );
  //the score
  itoa(score->daScores[i].score,temp,10);
  tempstr = temp;
  TextOut(hdcBuffer2,FIELDWIDTH/2,VERTINDENT*(18+3*i),
            tempstr.c_str(),tempstr.size());
  //the level #
  itoa(score->daScores[i].level,temp,10);
  tempstr = temp;
  TextOut(hdcBuffer2,FIELDWIDTH/2+HORIZINDENT*16,VERTINDENT*(18+3*i),
            tempstr.c_str(),tempstr.size());
 }
 
 //copying final product to primary screen buffer from secondary
 BitBlt(hdc, HORIZINDENT, VERTINDENT,
         FIELDWIDTH, FIELDHEIGHT+READOUTSHEIGHT, hdcBuffer2, 0, 0, SRCCOPY);

 //restoring original bitmaps, cleaning up
 SetTextColor(hdcBuffer2,oldTextColor);
 SelectObject(hdcBuffer2,hfOld);
 SelectObject(hdcMem, hbmOld);
 SelectObject(hdcBuffer2,hbmOld2);
 SelectObject(hdcBuffer2, oldPen);
 DeleteObject(tempBitmap);
 DeleteObject(greenPen);
 DeleteObject(bluePen);
 DeleteObject(redPen);
 DeleteDC(hdcMem);//give back memory DC
 DeleteDC(hdcBuffer2);//give back 2nd buffer DC
 EndPaint(myHandle, &ps);//give back screen DC

}//end DrawMainMenu()////////////////////////////////////////////////////////





/*RackUpTimeBONUS***********************************************************/
//used to draw 'timeBonus' as it cycles down at the end of a level
void RackUpTimeBonus()
{
//BitBlt(to DC,to X,to Y,image width,height,from DC,from X,from Y,flags);

 PAINTSTRUCT ps;
 HDC hdc = BeginPaint(myHandle, &ps);//screen(visible) HDC
 HDC hdcMem = CreateCompatibleDC(hdc);//couterpart HDC in memory
 //catch old bitmap as the one for the readouts goes into memory
 HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, readout_bmp);
 //for redrawing "Balls: #" label, if necessary...
 //select font into DC
 HFONT hfOld = (HFONT)SelectObject(hdc, dfont);
 //select gray background brush
 HBRUSH hbrushOld =(HBRUSH)SelectObject(hdc,gray);
 //select gray pen so rectangle outline not shown
 HPEN grayPen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
 HPEN hpenOld = (HPEN)SelectObject(hdc,grayPen);
 //buffer for building label string
 char temp[20];
 //let the background be transparent
 SetBkMode(hdc,TRANSPARENT);



 int prevTotal;
 for(int count = field->getTimeBonus(); count>0; count--)
 {
     prevTotal = field->getScore();
     field->incScore(25);
     field->decTimeBonus();

    /*redraw readouts*/
    //draw # of lives readout
    for(int i = 0; i<8; i++)
    {
    if(i<field->getLives())//draw a ball
    {BitBlt(hdc,FIELDWIDTH-HORIZINDENT-LIVESWIDTH + (BALLWIDTH+4)*i,
    FIELDHEIGHT+VERTINDENT*3,BALLWIDTH+4,BALLHEIGHT+11,hdcMem,0,0,SRCCOPY);}
    else//draw a blank
    {BitBlt(hdc,FIELDWIDTH-HORIZINDENT-LIVESWIDTH + (BALLWIDTH+4)*i,
    FIELDHEIGHT+VERTINDENT*3,BALLWIDTH+4,BALLHEIGHT+11,hdcMem,0,60,SRCCOPY);}
    }
 
      //the score readout
      int number;//the score to print
      number = field->getScore();
      int hundThou = number / 100000;
      int tenThou = (number % 100000) / 10000;
      int thou = (number % 10000) / 1000;
      int hund = (number % 1000) / 100;
      int ten = (number % 100) / 10;
      int one = number % 10;
      BitBlt(hdc,NUMWIDTH+HORIZINDENT,FIELDHEIGHT+VERTINDENT*3,
                 NUMWIDTH,NUMHEIGHT,hdcMem,0,88+hundThou*NUMHEIGHT,SRCCOPY);
      BitBlt(hdc,NUMWIDTH*2+HORIZINDENT,FIELDHEIGHT+VERTINDENT*3,
                  NUMWIDTH,NUMHEIGHT,hdcMem,0,88+tenThou*NUMHEIGHT,SRCCOPY);
      BitBlt(hdc,NUMWIDTH*3+HORIZINDENT,FIELDHEIGHT+VERTINDENT*3,
                     NUMWIDTH,NUMHEIGHT,hdcMem,0,88+thou*NUMHEIGHT,SRCCOPY);
      BitBlt(hdc,NUMWIDTH*4+HORIZINDENT,FIELDHEIGHT+VERTINDENT*3,
                     NUMWIDTH,NUMHEIGHT,hdcMem,0,88+hund*NUMHEIGHT,SRCCOPY);
      BitBlt(hdc,NUMWIDTH*5+HORIZINDENT,FIELDHEIGHT+VERTINDENT*3,
                      NUMWIDTH,NUMHEIGHT,hdcMem,0,88+ten*NUMHEIGHT,SRCCOPY);
      BitBlt(hdc,NUMWIDTH*6+HORIZINDENT,FIELDHEIGHT+VERTINDENT*3,
                    NUMWIDTH+1,NUMHEIGHT,hdcMem,0,88+one*NUMHEIGHT,SRCCOPY);
                    //+1 to get the border
 
      //time bonus
      //first row, lit-up
      for(int i = 0; i<23 && i<field->getTimeBonus(); i++)
      {BitBlt(hdc,READOUTSWIDTH*.546+HORIZINDENT+BONUSLIGHTWIDTH*i,
                    FIELDHEIGHT+VERTINDENT*3 + 3,
                    BONUSLIGHTWIDTH,BONUSLIGHTHEIGHT,hdcMem,0,348,SRCCOPY);}
      //first row, turned-off
      for(int i = field->getTimeBonus(); i<23; i++)
      {BitBlt(hdc,READOUTSWIDTH*.546+HORIZINDENT+BONUSLIGHTWIDTH*i,
                    FIELDHEIGHT+VERTINDENT*3 + 3,
                    BONUSLIGHTWIDTH,BONUSLIGHTHEIGHT,hdcMem,0,360,SRCCOPY);}
      //second row, lit-up
      for(int i =0; i<23 && i<(field->getTimeBonus()-23); i++)
      {BitBlt(hdc,READOUTSWIDTH*.546+HORIZINDENT+BONUSLIGHTWIDTH*i,
                    FIELDHEIGHT+VERTINDENT*3+BONUSLIGHTHEIGHT + 3,
                    BONUSLIGHTWIDTH,BONUSLIGHTHEIGHT,hdcMem,0,348,SRCCOPY);}
      //second row, turned-off
      int j = field->getTimeBonus() - 23;
      if(j<0){j=0;}
      for( j; j < 23; j++)
      {BitBlt(hdc,READOUTSWIDTH*.546+HORIZINDENT+BONUSLIGHTWIDTH*j,
                    FIELDHEIGHT+VERTINDENT*3+BONUSLIGHTHEIGHT + 3,
                    BONUSLIGHTWIDTH,BONUSLIGHTHEIGHT,hdcMem,0,360,SRCCOPY);}

     if(prevTotal / 5000 < field->getScore()/5000 )
                        //5000pts 
     {
        field->incLives();
        
        //redraw lives label
        Rectangle(hdc, FIELDWIDTH-HORIZINDENT*2-LIVESWIDTH+HORIZINDENT+45,
                       FIELDHEIGHT+VERTINDENT+1, 
                       FIELDWIDTH-HORIZINDENT*2-LIVESWIDTH+HORIZINDENT+150,
                       FIELDHEIGHT+VERTINDENT+18);
        sprintf(temp,"Balls: %d",field->getLives());
        TextOut(hdc,FIELDWIDTH-HORIZINDENT*2-LIVESWIDTH+HORIZINDENT,
                FIELDHEIGHT+VERTINDENT, temp, strlen(temp));

        
        if(sound)
        {oneup_snd->Play();}
     }
     else
     {
      if(sound)
      {timebonus_snd->Play();}
     }
      
      Sleep(120 + (count%2)*50);//wait another 50ms every other time
 }

//restoring original bitmaps, cleaning up
 SelectObject(hdcMem, hbmOld);
 DeleteDC(hdcMem);//give back memory DC
 SelectObject(hdc,hpenOld);
 SelectObject(hdc,hbrushOld);
 SelectObject(hdc,hfOld);
 EndPaint(myHandle, &ps);//give back screen DC

}//end RackUpTimeBonus()/////////////////////////////////////////////////////


/*InitDirectDRAW************************************************************/
// credit for this function goes to Mauricio Ritter
// http://www.codeproject.com/directx/BasicDD/
int InitDirectDraw()
{
   DDSURFACEDESC  ddsd;
   DDSCAPS        ddscaps;
   HRESULT        hRet;

   // Create the main DirectDraw object.
   hRet = DirectDrawCreate(NULL,&DirDrawObj,NULL);
   if( hRet != DD_OK )
       return -1;

   // Get exclusive mode.
   hRet = DirDrawObj->SetCooperativeLevel(myHandle, 
        DDSCL_NORMAL);
   //DDSCL_ALLOWMODEX|DDSCL_FULLSCREEN|DDSCL_EXCLUSIVE|DDSCL_ALLOWREBOOT);
   if( hRet != DD_OK )
       return -2;

   /*// Set the video mode
   hRet = DirDrawObj->SetDisplayMode(1024, 768, 16);
   if( hRet != DD_OK )
       return -3;*/

   // Prepare to create the primary surface by initializing
   // the fields of a DDSURFACEDESC structure.
   memset(&ddsd,0,sizeof(ddsd));
   ddsd.dwSize = sizeof(ddsd);
   ddsd.dwFlags = DDSD_CAPS;// | DDSD_BACKBUFFERCOUNT;
   ddsd.ddsCaps.dwCaps = 
        DDSCAPS_PRIMARYSURFACE;// | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
   // ddsd.dwBackBufferCount = 1;

   // Create the primary surface.
   hRet = DirDrawObj->CreateSurface(&ddsd, &SurfaceFront, NULL);
   if( hRet != DD_OK )
       return -4;

   /*// Get a pointer to the back buffer.
   ZeroMemory(&ddscaps, sizeof(ddscaps));
   ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
   hRet = SurfaceFront->GetAttachedSurface(&ddscaps, &SurfaceBack);
   if( hRet != DD_OK )
       return -5;*/
       
   // Create a clipper to ensure that our drawing stays inside our window
    hRet = DirDrawObj->CreateClipper( 0, &clipper, NULL );
    if( hRet != DD_OK )
    return - 6;

    // setting it to our hwnd gives the clipper the coords. from our window
    hRet = clipper->SetHWnd( 0, myHandle );
    if( hRet != DD_OK )
    return -7;



    // attach the clipper to the primary surface
    hRet = SurfaceFront->SetClipper( clipper );
    if( hRet != DD_OK )


    // create the backbuffer separately
    memset( &ddsd, 0, sizeof(ddsd) );
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth = FIELDWIDTH;
    ddsd.dwHeight = FIELDHEIGHT + READOUTSHEIGHT;

    hRet = DirDrawObj->CreateSurface( &ddsd, &SurfaceBack, NULL );
    if( hRet != DD_OK )
    return -7;
   
   //create the bitmaps       
   //                                      width, height, transparent color)
   ball_surf    = new Surface( DirDrawObj,    18,    145,     RGB(0,0,0) );
   blocks_surf  = new Surface( DirDrawObj,    66,    720);
   readout_surf = new Surface( DirDrawObj,    45,    372);
   bkgrnd_surf  = new Surface( DirDrawObj,   792,    480);
   
   ball_surf->LoadBitmap   (hinst1, BALL_BMP, 0, 0);
   blocks_surf->LoadBitmap (hinst1, BLOCKS_BMP, 0, 0);
   readout_surf->LoadBitmap(hinst1, READOUT_BMP, 0, 0);
   bkgrnd_surf->LoadBitmap( hinst1, BACKGROUND_BMP, 0, 0);

   return 0;

}//end InitDirectDraw()//////////////////////////////////////////////////////




/*CleanUpDirDRAW************************************************************/
// credit for this function goes to Mauricio Ritter
// http://www.codeproject.com/directx/BasicDD/
void CleanUpDirDraw()
{
    delete ball_surf;
    delete blocks_surf;
    delete readout_surf;
    delete bkgrnd_surf;
    
    if(clipper)
        clipper-> Release();
    
    if(SurfaceBack)
        SurfaceBack->Release();

    if(SurfaceFront)
        SurfaceFront->Release();

    if(DirDrawObj)
        DirDrawObj->Release();
     
}//end CleanUpDirDraw()//////////////////////////////////////////////////////

/*FLIP**********************************************************************/
bool Flip()
{
//credit for this function goes to Lan Mader
//http://www.gamedev.net/reference/articles/article608.asp
    HRESULT hRet;
    RECT rcRectSrc;
    RECT rcRectDest;
    POINT p;

    //first we need to figure out where on the primary surface our window is
    p.x = 0; p.y = 0;
    ClientToScreen(myHandle, &p);
    GetClientRect(myHandle, &rcRectDest);
    OffsetRect(&rcRectDest, p.x + HORIZINDENT, p.y + VERTINDENT);
    rcRectDest.right = rcRectDest.left + FIELDWIDTH;
    rcRectDest.bottom = rcRectDest.top + FIELDHEIGHT + READOUTSHEIGHT;
    
    SetRect(&rcRectSrc, 0, 0, FIELDWIDTH, FIELDHEIGHT + READOUTSHEIGHT);

    while(1)
    {

        hRet = SurfaceFront->Blt(&rcRectDest, SurfaceBack, &rcRectSrc, 
                                   DDBLT_WAIT,  NULL);

        if(hRet == DD_OK)
            break;

        if(hRet == DDERR_SURFACELOST)
        {
            SurfaceFront->Restore();
        }
        else
        {
            if(hRet != DDERR_WASSTILLDRAWING)
                return FALSE;
        }
    }

}//end Flip()////////////////////////////////////////////////////////////////


/*InitDirectSOUND***********************************************************/
int InitDirectSound()
{
    HRESULT hRet;//to check for errors
    
    //create object
    hRet = DirectSoundCreate(NULL, &DirSoundObj, NULL);
    if(hRet != DS_OK)
    {return -1;}
    
    //set coop level
    hRet = DirSoundObj->SetCooperativeLevel(myHandle, DSSCL_NORMAL);
    if(hRet != DS_OK)
    {return -2;}
    
  // load all sounds
  bounce_snd =   new SoundBuffer(DirSoundObj,MAKEINTRESOURCE(    BOUNCESND));
  colorblk_snd = new SoundBuffer(DirSoundObj,MAKEINTRESOURCE(  COLORBLKSND));
  colorbrush_snd=new SoundBuffer(DirSoundObj,MAKEINTRESOURCE(COLORBRUSHSND));
  diamond_snd =  new SoundBuffer(DirSoundObj,MAKEINTRESOURCE(   DIAMONDSND));
  die_snd =      new SoundBuffer(DirSoundObj,MAKEINTRESOURCE(       DIESND));
  laughing_snd = new SoundBuffer(DirSoundObj,MAKEINTRESOURCE(  LAUGHINGSND));
  levelwon_snd = new SoundBuffer(DirSoundObj,MAKEINTRESOURCE(  LEVELWONSND));
  lock_snd =     new SoundBuffer(DirSoundObj,MAKEINTRESOURCE(      LOCKSND));
  key_snd =      new SoundBuffer(DirSoundObj,MAKEINTRESOURCE(       KEYSND));
  reverse_snd =  new SoundBuffer(DirSoundObj,MAKEINTRESOURCE(   REVERSESND));
  timebonus_snd= new SoundBuffer(DirSoundObj,MAKEINTRESOURCE( TIMEBONUSSND));
  oneup_snd =    new SoundBuffer(DirSoundObj,MAKEINTRESOURCE(     ONEUPSND));
  bounce2_snd =  new SoundBuffer(DirSoundObj,MAKEINTRESOURCE(   BOUNCE2SND));
    
    
  return 0;

}//end InitDirectSound()/////////////////////////////////////////////////////

/*CleanUpDirSOUND***********************************************************/
void CleanUpDirSound()
{
     delete bounce_snd;
     delete colorblk_snd;
     delete colorbrush_snd;
     delete diamond_snd;
     delete die_snd;
     delete laughing_snd;
     delete levelwon_snd;
     delete lock_snd;
     delete key_snd;
     delete reverse_snd;
     delete timebonus_snd;
     delete oneup_snd;
     delete bounce2_snd;
     
     if(DirSoundObj)
     {DirSoundObj->Release();}
     
}//end CleanUpDirSound()/////////////////////////////////////////////////////
