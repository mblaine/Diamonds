/*
 * DIAMONDS.CPP
 * created by Matthew Blaine, November 2005
 *
 * This WIN32 application uses classes 'Ball', 'Field', and 'Block' and its
 * subclasses to orchestrate a full-blown, 30-level game of 'Diamonds'. To
 * play Diamonds, the player uses the left and right arrow keys to direct a
 * constantly bouncing up-and-down ball through the playing field. To beat a
 * a level, destroy all the Diamond blocks, derived from the base class
 * 'Block'. To desroy a bock, simply cause the ball to make contact with it.
 * Diamond blocks cannot be destroyed until all color-blocks are destroyed.
 * These can only be destroyed if the ball matches the given block's color.
 * To change the ball's color, run into a 'key' block. There is no key block
 * to make the ball light blue, so remove those blocks first. If you hit a
 * key block, changing the ball's color, before all light blue blocks are
 * gone, you must run the ball into a 'skull' block, losing a life and
 * reseting the ball.
 *
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>

#include "Constants.h"
#include "Field.h"
#include "Ball.h"
#include "Block.h"
#include "Resource_Diamonds.h"
#include "HiScore.h"

#define CLASSNAME "MJB_Diamonds"

//global handles
HWND myHandle = NULL;
HINSTANCE hinst1 = NULL;
HACCEL hmyaccel = NULL;
HMENU hmenu = NULL;

//handles to bitmaps
HBITMAP ball_bmp = NULL;
HBITMAP blocks_bmp = NULL;
HBITMAP readout_bmp = NULL;
HBITMAP logo_bmp = NULL;
HBITMAP bkgrnd_bmp = NULL;


//handle to brushes for drawing screen
HBRUSH ltGray = NULL;
HBRUSH gray = NULL;
HBRUSH borderBrushes[7];

//if a game is running
bool gameOn = false;

//if sound is on or off
// - turned on in WM_CREATE
bool sound = false;

//high scores
HiScore *scores = NULL;

//for actual game
Field *field = NULL;

bool isPaused = false;

//Device contexts
 HDC hdc;//screen(visible) HDC
 HDC hdcMem;//couterpart HDC in memory
 HDC hdcBuffer2;//secondary buffer HDC

//prototypes
LRESULT  CALLBACK WindowProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam);
BOOL    CALLBACK NameDlgProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam);
BOOL   CALLBACK AboutDlgProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
void DrawScreen(Field *field, Ball *ball);
void DrawMainMenu(HiScore *score);
void RackUpTimeBonus();
//begin WinMain//////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance,
                                        LPSTR lpcmdline, int ncmdshow)
{
  WNDCLASSEX winclass;
  MSG msg;
  ATOM atom;
  hinst1 = hinstance;
  
  winclass.cbSize = sizeof(WNDCLASSEX);
  winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  winclass.lpfnWndProc = WindowProc;
  winclass.cbClsExtra = 0;
  winclass.cbWndExtra = 0;
  winclass.hInstance = hinstance;
  winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
  winclass.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
  winclass.lpszMenuName = "MyMenu";
  winclass.lpszClassName = CLASSNAME;
  winclass.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL),
                         MAKEINTRESOURCE(PROG_ICON), IMAGE_ICON, 16, 16, 0);

  atom=
  
   RegisterClassEx(&winclass);

  if(!atom)
  {return 0;}
  
  myHandle=
  
   CreateWindow(CLASSNAME,"Diamonds",WS_OVERLAPPED|WS_MINIMIZEBOX|
      WS_CAPTION|WS_SYSMENU,CW_USEDEFAULT,CW_USEDEFAULT,WINWIDTH,
      WINHEIGHT,NULL,NULL,hinstance,NULL);
             
  if(!myHandle)
  {return 0;}

  ShowWindow( myHandle, ncmdshow);

  //load keyboard accelerator table
  hmyaccel = LoadAccelerators(hinstance,"Accel");
   
  while( GetMessage(&msg,NULL,0,0) > 0 )
  {
  if (!TranslateAccelerator( myHandle, hmyaccel, &msg))//catch accel. keys
     {
     TranslateMessage(&msg);
     DispatchMessage(&msg);
     }
  }
  
  return msg.wParam;
  
}//end WinMain///////////////////////////////////////////////////////////////

LRESULT CALLBACK WindowProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
 switch(msg)
 {
  case WM_CREATE:
          {
           //set big program icon
           PostMessage(hwnd, WM_SETICON, ICON_BIG,
                (LPARAM)LoadIcon(hinst1,MAKEINTRESOURCE(PROG_ICON)));

           ball_bmp = LoadBitmap(hinst1,MAKEINTRESOURCE(BALL_BMP));
           blocks_bmp = LoadBitmap(hinst1,MAKEINTRESOURCE(BLOCKS_BMP));
           readout_bmp = LoadBitmap(hinst1,MAKEINTRESOURCE(READOUT_BMP));
           logo_bmp = LoadBitmap(hinst1,MAKEINTRESOURCE(LOGO_BMP));
           bkgrnd_bmp = LoadBitmap(hinst1,MAKEINTRESOURCE(BACKGROUND_BMP));

           ltGray = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
           gray = (HBRUSH)GetStockObject(GRAY_BRUSH);

           borderBrushes[0] = CreateSolidBrush(RGB(0,204,255));//ltBlue
           borderBrushes[1] = CreateSolidBrush(RGB(0,36,255));//blue
           borderBrushes[2] = CreateSolidBrush(RGB(255,39,0));//red
           borderBrushes[3] = CreateSolidBrush(RGB(0,255,0));//green
           borderBrushes[4] = CreateSolidBrush(RGB(146,91,0));//brown
           borderBrushes[5] = CreateSolidBrush(RGB(204,0,204));//purple
           borderBrushes[6] = CreateSolidBrush(RGB(255,153,0));//orange
           
           hmenu = GetMenu(hwnd);//get menu handle
           
           //set up device contexts
           hdc = GetDC(hwnd);//screen(visible) HDC
           hdcMem = CreateCompatibleDC(hdc);//couterpart HDC in memory
           hdcBuffer2 = CreateCompatibleDC(hdc);//secondary buffer HDC
           
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
                  
                  //reload all sounds
                  MAKEINTRESOURCE(BOUNCESND);
                  MAKEINTRESOURCE(COLORBLKSND);
                  MAKEINTRESOURCE(COLORKEYSND);
                  MAKEINTRESOURCE(DIAMONDSND);
                  MAKEINTRESOURCE(DIESND);
                  MAKEINTRESOURCE(LAUGHINGSND);
                  MAKEINTRESOURCE(LEVELWONSND);
                  MAKEINTRESOURCE(LOCKSND);
                  MAKEINTRESOURCE(SMALLKEYSND);
                  MAKEINTRESOURCE(REVERSESND);
                  MAKEINTRESOURCE(TIMEBONUSSND);
                  MAKEINTRESOURCE(ONEUPSND);

                  delete field; field = NULL;
                  field = new Field;
                  
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
                  if(sound)
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
                  delete scores;
                  scores = new HiScore();
                  PostMessage(hwnd,WM_PAINT,0,0);
                  break;
             
             case GAME_QUIT:
             case KEY_QUIT:
                  if(!gameOn){break;}
                  
                  KillTimer(hwnd,MAINTIMER);
                  KillTimer(hwnd,BONUSTIMER);
                  
                  if(sound)
                  {PlaySound(MAKEINTRESOURCE(LAUGHINGSND),
                                 hinst1,SND_RESOURCE|SND_ASYNC);}

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
            InvalidateRect(hwnd,NULL,FALSE);//invalidate entire rectangle
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
           {
            if(!field->getHorizReversed())
            {field->setToMoveLeft();}
            else
            {field->setToMoveRight();}
           }
           //right key down
           else if(GetAsyncKeyState(VK_RIGHT) & 0x8000)
           {
            if(!field->getHorizReversed())
            {field->setToMoveRight();}
            else
            {field->setToMoveLeft();}
           }

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
                PlaySound(MAKEINTRESOURCE(LAUGHINGSND),
                                 hinst1,SND_RESOURCE|SND_ASYNC);
                
                //if a high score
                if(field->getScore() >= scores->daScores[9].score)
                {DialogBox(hinst1, "EnterName", hwnd, NameDlgProc);}
                else{Sleep(1000);}
                gameOn = false;
                PostMessage(hwnd,WM_PAINT,0,0);
           }
           
           else
              {                
                if(sound)
                PlaySound(MAKEINTRESOURCE(DIESND),
                                 hinst1,SND_RESOURCE|SND_ASYNC);
                
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
                {PlaySound(MAKEINTRESOURCE(LEVELWONSND),
                                 hinst1,SND_RESOURCE|SND_ASYNC);}
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
                InvalidateRect(hwnd,NULL,FALSE);//invlidate entire screen
               }
               else // if no more levels left
               {
                KillTimer(hwnd,MAINTIMER);
                KillTimer(hwnd,BONUSTIMER);        

                if(sound)
                {PlaySound(MAKEINTRESOURCE(LEVELWONSND),
                                 hinst1,SND_RESOURCE|SND_ASYNC);}
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

          if(wparam==BONUSTIMER)//if it's the bonus timer
          {
            field->decTimeBonus();
            if(field->getTimeBonus()<=0)
            {KillTimer(hwnd,BONUSTIMER);}
          }

          return 0;
          break;// end case WM_TIMER
          
  case WM_DESTROY:
          {

           KillTimer(hwnd,MAINTIMER);       
           KillTimer(hwnd,BONUSTIMER);

           //saving 'scores' to file
           fstream outfile("hiscores.dat",ios::out|ios::binary);
           if(outfile.is_open())// if opened sucssfully, save values
           {scores->serialize(outfile);}
           outfile.close();//on error, just do nothing

           delete field; field = NULL;
           delete scores; scores = NULL;
           DeleteObject(ball_bmp);
           DeleteObject(blocks_bmp);
           DeleteObject(readout_bmp);
           DeleteObject(logo_bmp);
           DeleteObject(bkgrnd_bmp);
           DeleteObject(ltGray);
           DeleteObject(gray);
           for(int i =0; i<7; i++)
           {DeleteObject(borderBrushes[i]);}
           
           DeleteDC(hdcMem);//give back memory DC
           DeleteDC(hdcBuffer2);//give back 2nd buffer DC
           ReleaseDC(hwnd,hdc);//give back screen DC
       
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

              //get the length of the string to input
              int len=GetWindowTextLength(GetDlgItem(hwnd,NAME_INPUT));

              if(len > 0)//if user input something
              {
                char* inData;//to hold input
                inData=(char*)GlobalAlloc(GPTR,len+1);//alocate char*
                GetDlgItemText(hwnd,NAME_INPUT,inData,len+1);//fillit
                temp=inData;//feed to c++ string
                GlobalFree((HANDLE)inData);//free memory for input
              }
              else{temp="----------";}
              
              temp = temp.substr(0,25);//limit name to 25 characters
              
              //enter the high score as a 'score' struct
              score toEnter = {temp,
                               field->getScore(),
                               field->getCurrentLevel()};
                               
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
//BitBlt(to DC,to X,to Y,image width,height,from DC,from X,from Y,flags);
 
 //playing field sized bitmap for 2nd buffer
 HBITMAP tempBitmap = CreateCompatibleBitmap(hdc, FIELDWIDTH,
                                         FIELDHEIGHT + READOUTSHEIGHT);
 //catch old bitmap from secondary buffer as our field sized one goes in
 HBITMAP hbmOld2 =(HBITMAP) SelectObject(hdcBuffer2, tempBitmap);
 
 //drawing color for screen border
 int index;
 if(ball!=NULL)
 {index = (int)ball->getColor() - 1;}
 else
 {index = 0;}
 //border color; gotta catch old brush
 HBRUSH hbrushOld =(HBRUSH)SelectObject(hdcBuffer2,borderBrushes[index]);
 Rectangle(hdcBuffer2, 0, 0, FIELDWIDTH, FIELDHEIGHT);
 
 //drawing background image
 //catch old bitmap as our background bitmap goes into memory DC
 HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, bkgrnd_bmp);
 //the background image
 BitBlt(hdcBuffer2,HORIZINDENT,VERTINDENT,
                 FIELDWIDTH,FIELDHEIGHT,hdcMem,0,0,SRCCOPY);
 
 //get 'blocks' bitmap
 SelectObject(hdcMem, blocks_bmp);
 
 if(field != NULL)//if field exists
 {
 //draw all blocks to temporary bitmap in secondary buffer
 for(int y = 0; y < ARRAYHEIGHT; y++)//for all rows
 {
  for(int x = 0; x < ARRAYWIDTH; x++)//for all cells in current row
  {
   if(field->getBlock(y,x) != NULL)//if block exists;
   {
    BlockToField info  = field->getBlock(y,x)->collision();//need da info
    
    int drawToX, drawToY;//coordinates to draw to
    drawToX = BLOCKWIDTH * x + HORIZINDENT;
    drawToY = BLOCKHEIGHT * y + VERTINDENT;

    if(info.msg == MSG_DIAMOND)
    { BitBlt(hdcBuffer2, drawToX, drawToY,
                         BLOCKWIDTH, BLOCKHEIGHT, hdcMem, 0, 560, SRCCOPY);}
    else if(info.msg == MSG_SKULL)
    { BitBlt(hdcBuffer2, drawToX, drawToY,
                         BLOCKWIDTH, BLOCKHEIGHT, hdcMem, 0, 0, SRCCOPY);}
    else if(info.msg == MSG_SMALLKEY)
    { BitBlt(hdcBuffer2, drawToX, drawToY,
                         BLOCKWIDTH, BLOCKHEIGHT, hdcMem, 0, 600, SRCCOPY);}
    else if(info.msg == MSG_LOCK)
    { BitBlt(hdcBuffer2, drawToX, drawToY,
                         BLOCKWIDTH, BLOCKHEIGHT, hdcMem, 0, 640, SRCCOPY);}
    else if(info.msg == MSG_SOLID)
    { BitBlt(hdcBuffer2, drawToX, drawToY,
                         BLOCKWIDTH, BLOCKHEIGHT, hdcMem, 0, 520, SRCCOPY);}
    else if(info.msg == MSG_REVERSE)
    { BitBlt(hdcBuffer2, drawToX, drawToY,
                         BLOCKWIDTH, BLOCKHEIGHT, hdcMem, 0, 680, SRCCOPY);}
    else if(info.msg == MSG_COLORCHANGEKEY)
    { BitBlt(hdcBuffer2, drawToX, drawToY,//the math below is to
                                                //allign w/ image in
                         BLOCKWIDTH, BLOCKHEIGHT, hdcMem, 0,//bitmap
                         280 + BLOCKHEIGHT*(info.info-2),//info.info = color
                         SRCCOPY);}
    else if(info.msg == MSG_COLORBLOCK)
    { BitBlt(hdcBuffer2, drawToX, drawToY,//the math below is to
                                                //allign w/ image in
                         BLOCKWIDTH, BLOCKHEIGHT, hdcMem, 0,//bitmap
                         0 + BLOCKHEIGHT*(info.info),//info.info = color
                         SRCCOPY);}
                         //end cycling through possible types
   }//end if ptr not null
  }//end for every x 
 }//end for every y
}//end if field exists
 //done drawing blocks


 //move ball bitmap into memory DC
 SelectObject(hdcMem, ball_bmp);
 
  //NOW.. the ball, only if it exists
 if(ball != NULL && field->getGameState()!=BALLDIED)
 {
 //copy negative image so edges of ball image are transparent
 BitBlt(hdcBuffer2, ball->getXpos() + HORIZINDENT, 
                    ball->getYpos() + VERTINDENT,
                    BALLWIDTH, BALLHEIGHT, hdcMem, 0, 126, SRCAND);
 //copy ball from memory DC to correct place in temporary bitmap
 BitBlt(hdcBuffer2, ball->getXpos() + HORIZINDENT, 
                    ball->getYpos() + VERTINDENT,
                    BALLWIDTH, BALLHEIGHT, hdcMem, 0,
                    ((int)ball->getColor()-1)*BALLHEIGHT, SRCPAINT);
 }
 
  //NOW... the readouts at the bottom of the screen


 //color 'readout' brush gray

 SelectObject(hdcBuffer2, gray);//bkgrd color
 Rectangle(hdcBuffer2, 0, FIELDHEIGHT, READOUTSWIDTH,
                       FIELDHEIGHT+READOUTSHEIGHT);

 //write labels for displays
 SetBkMode(hdcBuffer2,TRANSPARENT);//let the background be transparent
 TextOut(hdcBuffer2,NUMWIDTH,FIELDHEIGHT+VERTINDENT/2,"Score",5);
 TextOut(hdcBuffer2,NUMWIDTH*9,FIELDHEIGHT+VERTINDENT/2,"Level",5);
 TextOut(hdcBuffer2, (FIELDWIDTH-HASKEYWIDTH)/2,FIELDHEIGHT+VERTINDENT/2,
        "Key", 3);
 TextOut(hdcBuffer2,READOUTSWIDTH*.546,FIELDHEIGHT+VERTINDENT/2,
        "Time Bonus", 10);
 TextOut(hdcBuffer2,FIELDWIDTH-HORIZINDENT*2-LIVESWIDTH,
        FIELDHEIGHT+VERTINDENT/2, "Balls", 5);

  //images for readouts
 SelectObject(hdcMem, readout_bmp);

 //draw # of lives readout
 for(int i = 0; i<8; i++)
 {
 if(i<field->getLives())//draw a ball
 {BitBlt(hdcBuffer2,FIELDWIDTH-HORIZINDENT*2-LIVESWIDTH + (BALLWIDTH+4)*i,
    FIELDHEIGHT+VERTINDENT*2,BALLWIDTH+4,BALLHEIGHT+11,hdcMem,0,0,SRCCOPY);}
 else//draw a blank
 {BitBlt(hdcBuffer2,FIELDWIDTH-HORIZINDENT*2-LIVESWIDTH + (BALLWIDTH+4)*i,
    FIELDHEIGHT+VERTINDENT*2,BALLWIDTH+4,BALLHEIGHT+11,hdcMem,0,60,SRCCOPY);}
 }
 
  //the has a key? readout
 if(field!=NULL)
 {if(field->getHasSmallKey())
  {BitBlt(hdcBuffer2,(FIELDWIDTH-HASKEYWIDTH)/2,FIELDHEIGHT+VERTINDENT*2,
                          HASKEYWIDTH,HASKEYHEIGHT,hdcMem,0,29,SRCCOPY);}
  else
  {BitBlt(hdcBuffer2,(FIELDWIDTH-HASKEYWIDTH)/2,FIELDHEIGHT+VERTINDENT*2,
                             HASKEYWIDTH,HASKEYHEIGHT,hdcMem,0,57,SRCCOPY);}
 }
 else
 {BitBlt(hdcBuffer2,(FIELDWIDTH-HASKEYWIDTH)/2,FIELDHEIGHT+VERTINDENT*2,
                             HASKEYWIDTH,HASKEYHEIGHT,hdcMem,0,57,SRCCOPY);}
 
 //the level # readout
 int levelTens = field->getCurrentLevel()/10;
 int levelOnes = field->getCurrentLevel()%10;
 BitBlt(hdcBuffer2,NUMWIDTH*9,FIELDHEIGHT+VERTINDENT*2,
                NUMWIDTH,NUMHEIGHT, hdcMem,0,88+levelTens*NUMHEIGHT,SRCCOPY);
 BitBlt(hdcBuffer2, NUMWIDTH*10,FIELDHEIGHT+VERTINDENT*2,
              NUMWIDTH+1,NUMHEIGHT,hdcMem,0,88+levelOnes*NUMHEIGHT,SRCCOPY);
              //+1 to get the border
 TextOut(hdcBuffer2,NUMWIDTH*11 + 5,FIELDHEIGHT+VERTINDENT*2 + NUMHEIGHT/4,
              field->getLevelName().c_str(),field->getLevelName().size() );
 
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
 BitBlt(hdcBuffer2,NUMWIDTH,FIELDHEIGHT+VERTINDENT*2,
                 NUMWIDTH,NUMHEIGHT,hdcMem,0,88+hundThou*NUMHEIGHT,SRCCOPY);
 BitBlt(hdcBuffer2,NUMWIDTH*2,FIELDHEIGHT+VERTINDENT*2,
                  NUMWIDTH,NUMHEIGHT,hdcMem,0,88+tenThou*NUMHEIGHT,SRCCOPY);
 BitBlt(hdcBuffer2,NUMWIDTH*3,FIELDHEIGHT+VERTINDENT*2,
                     NUMWIDTH,NUMHEIGHT,hdcMem,0,88+thou*NUMHEIGHT,SRCCOPY);
 BitBlt(hdcBuffer2,NUMWIDTH*4,FIELDHEIGHT+VERTINDENT*2,
                     NUMWIDTH,NUMHEIGHT,hdcMem,0,88+hund*NUMHEIGHT,SRCCOPY);
 BitBlt(hdcBuffer2,NUMWIDTH*5,FIELDHEIGHT+VERTINDENT*2,
                      NUMWIDTH,NUMHEIGHT,hdcMem,0,88+ten*NUMHEIGHT,SRCCOPY);
 BitBlt(hdcBuffer2,NUMWIDTH*6,FIELDHEIGHT+VERTINDENT*2,
                    NUMWIDTH+1,NUMHEIGHT,hdcMem,0,88+one*NUMHEIGHT,SRCCOPY);
                    //+1 to get the border
 
 //time bonus
 //first row, lit-up
 for(int i = 0; i<23 && i<field->getTimeBonus(); i++)
 {BitBlt(hdcBuffer2,READOUTSWIDTH*.546+BONUSLIGHTWIDTH*i,
                    FIELDHEIGHT+VERTINDENT*2 + 3,
                    BONUSLIGHTWIDTH,BONUSLIGHTHEIGHT,hdcMem,0,348,SRCCOPY);}
 //first row, turned-off
 for(int i = field->getTimeBonus(); i<23; i++)
 {BitBlt(hdcBuffer2,READOUTSWIDTH*.546+BONUSLIGHTWIDTH*i,
                    FIELDHEIGHT+VERTINDENT*2 + 3,
                    BONUSLIGHTWIDTH,BONUSLIGHTHEIGHT,hdcMem,0,360,SRCCOPY);}
 //second row, lit-up
 for(int i = 0; i<23 && i<(field->getTimeBonus()-23); i++)
 {BitBlt(hdcBuffer2,READOUTSWIDTH*.546+BONUSLIGHTWIDTH*i,
                    FIELDHEIGHT+VERTINDENT*2+BONUSLIGHTHEIGHT + 3,
                    BONUSLIGHTWIDTH,BONUSLIGHTHEIGHT,hdcMem,0,348,SRCCOPY);}
 //second row, turned-off
 int j = field->getTimeBonus() - 23;
 if(j<0){j=0;}
 for( j; j < 23; j++)
 {BitBlt(hdcBuffer2,READOUTSWIDTH*.546+BONUSLIGHTWIDTH*j,
                    FIELDHEIGHT+VERTINDENT*2+BONUSLIGHTHEIGHT + 3,
                    BONUSLIGHTWIDTH,BONUSLIGHTHEIGHT,hdcMem,0,360,SRCCOPY);}
 

 //copying final product to primary screen buffer from secondary
 if(field!=NULL || ball!=NULL)
 {
 BitBlt(hdc, HORIZINDENT, VERTINDENT,
         FIELDWIDTH, FIELDHEIGHT+READOUTSHEIGHT, hdcBuffer2, 0, 0, SRCCOPY);
 }


 //restoring original bitmaps, cleaning up
 SelectObject(hdcMem, hbmOld);
 SelectObject(hdcBuffer2,hbmOld2);
 SelectObject(hdcBuffer2, hbrushOld);
 DeleteObject(tempBitmap);
 
 ValidateRect(myHandle,NULL);

}//end DrawScreen()/////////////////////////////////////////////////////////




/*DrawMainMENU()************************************************************/
void DrawMainMenu(HiScore *score)//draws main game menu when no game in play
{
//BitBlt(to DC,to X,to Y,image width,height,from DC,from X,from Y,flags);

 
 //screen sized bitmap for 2nd buffer
 HBITMAP tempBitmap = CreateCompatibleBitmap(hdc, FIELDWIDTH,
                                         FIELDHEIGHT + READOUTSHEIGHT);
 //catch old bitmap from secondary buffer as our screen sized one goes in
 HBITMAP hbmOld2 =(HBITMAP) SelectObject(hdcBuffer2, tempBitmap);

 //catch old bitmap as our logo bitmap goes into memory DC
 HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, logo_bmp);
 int toDrawAtX;
 toDrawAtX = 175+HORIZINDENT;

 BitBlt(hdcBuffer2,toDrawAtX,43,414,89,hdcMem,0,0,SRCCOPY);
 
 SetBkMode(hdcBuffer2, TRANSPARENT);//make text transparent
 
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
 string by = "created by Matt Blaine, November 2005          based on ";
 by+="\"Diamonds\" created by Varcon Systems, Inc.";
 TextOut(hdcBuffer2,HORIZINDENT*8,
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
 

 
 //catch old bitmap as our 'blocks bitmap goes into memory DC
 //HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, blocks_bmp);

 //copying final product to primary screen buffer from secondary
 BitBlt(hdc, HORIZINDENT, VERTINDENT,
         FIELDWIDTH, FIELDHEIGHT+READOUTSHEIGHT, hdcBuffer2, 0, 0, SRCCOPY);

 //restoring original bitmaps, cleaning up
 SetTextColor(hdcBuffer2,oldTextColor);
 SelectObject(hdcMem, hbmOld);
 SelectObject(hdcBuffer2,hbmOld2);
 SelectObject(hdcBuffer2, oldPen);
 DeleteObject(tempBitmap);
 DeleteObject(greenPen);
 DeleteObject(bluePen);
 DeleteObject(redPen);
 DeleteObject(tempBitmap);

 ValidateRect(myHandle,NULL);
}//end DrawMainMenu()////////////////////////////////////////////////////////





/*RackUpTimeBONUS***********************************************************/
//used to draw 'timeBonus' as it cycles down at the end of a level
void RackUpTimeBonus()
{
//BitBlt(to DC,to X,to Y,image width,height,from DC,from X,from Y,flags);

 //catch old bitmap as the one for the readouts goes into memory
 HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, readout_bmp);

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

     if(prevTotal / 5000 < field->getScore()/5000 && field->getLives() < 8 )
                        //5000pts and less than max. lives
       {
        field->incLives();

                if(sound)
                {PlaySound(MAKEINTRESOURCE(ONEUPSND),
                                 hinst1,SND_RESOURCE|SND_ASYNC);}

       }
     else
     {

      if(sound)
      {PlaySound(MAKEINTRESOURCE(TIMEBONUSSND),
                          hinst1,SND_RESOURCE|SND_ASYNC);}

     }
      
      Sleep(120 + (count%2)*50);//wait another 50ms every other time
 }

//restoring original bitmaps, cleaning up
 SelectObject(hdcMem, hbmOld);

}//end RackUpTimeBonus()/////////////////////////////////////////////////////
