#include <wx/wxprec.h>

#ifndef WX_PRECOMP
   #include <wx/wx.h>
   #include <wx/dir.h>
   #include <wx/numdlg.h>
#endif

#include "SDL_util.h"

#include "DiamondsFrame.h"
#include "Globals.h"

#include <fstream>
using namespace std;

BEGIN_EVENT_TABLE(DiamondsFrame, wxFrame)
   EVT_CLOSE(DiamondsFrame::OnClose)
   EVT_TIMER(TIMER_MAIN, DiamondsFrame::OnTimerMain)
   EVT_TIMER(TIMER_BONUS, DiamondsFrame::OnTimerBonus)   
   EVT_CHAR(DiamondsFrame::OnChar)
   EVT_MENU(GAME_NEW, DiamondsFrame::OnNew)
   EVT_MENU(GAME_PAUSE, DiamondsFrame::OnPause)
   EVT_MENU(GAME_RESTARTLEVEL, DiamondsFrame::OnRestartLevel)
   EVT_MENU(GAME_QUITGAME, DiamondsFrame::OnQuitGame)
   EVT_MENU(GAME_PRACTICE, DiamondsFrame::OnPractice)
   EVT_MENU(GAME_LAUNCHEDITOR, DiamondsFrame::OnLaunchEditor)
   EVT_MENU(GAME_SOUNDTOGGLE, DiamondsFrame::OnSoundToggle)
   EVT_MENU(GAME_CLEARSCORES, DiamondsFrame::OnClearScores)
   EVT_MENU(GAME_EXIT, DiamondsFrame::OnExit)
   EVT_MENU(HELP_HOWTOPLAY, DiamondsFrame::OnHowToPlay)
   EVT_MENU(HELP_ABOUT, DiamondsFrame::OnAbout)
END_EVENT_TABLE()

DiamondsFrame::DiamondsFrame() : wxFrame()
{
   Create( NULL, wxID_ANY, wxT("Diamonds"), wxDefaultPosition,
           wxSize(WINWIDTH, WINHEIGHT), wxCAPTION | wxSYSTEM_MENU | 
           wxMINIMIZE_BOX | wxCLOSE_BOX);
   
   IsOpen = true;
   settings = new Settings(settingsDir("diamonds.ini"));
              
   //menu structure                                                                               
   menuBar = new wxMenuBar();
   wxMenu* gameMenu = new wxMenu();
   wxMenu* levelsetMenu = new wxMenu();
   wxMenu* helpMenu = new wxMenu();
   
   gameMenu->Append(GAME_NEW, wxT("&New\tF2"));
   gameMenu->Append(GAME_PAUSE, wxT("&Pause\tP"));
   gameMenu->Append(GAME_RESTARTLEVEL, wxT("&Restart level\tR"));
   gameMenu->Append(GAME_QUITGAME, wxT("&Quit game\tQ"));
   gameMenu->AppendSeparator();
   gameMenu->Append(GAME_PRACTICE, wxT("Pr&actice"));
   gameMenu->Append(GAME_LAUNCHEDITOR, wxT("Launch &editor"));
   gameMenu->AppendSeparator();
   gameMenu->AppendCheckItem(GAME_SOUNDTOGGLE, wxT("&Sound"))->Check(settings->soundon);
   gameMenu->Append(GAME_CLEARSCORES, wxT("&Clear scores"));
   gameMenu->AppendSeparator();
   gameMenu->Append(GAME_EXIT, wxT("E&xit\tESC"));
   
   BuildLevelsetMenu(levelsetMenu);
   
   helpMenu->Append(HELP_HOWTOPLAY, wxT("&How to Play"));
   helpMenu->Append(HELP_ABOUT, wxT("&About Diamonds"));
   
   menuBar->Append(gameMenu, wxT("&Game"));
   menuBar->Append(levelsetMenu, wxT("&Levelset"));
   menuBar->Append(helpMenu, wxT("&Help"));
   SetMenuBar(menuBar);
   
   //program icons
   #ifdef __WXMSW__ 
   wxImage::AddHandler( new wxICOHandler );
   SetIcon(wxIcon(wxT("A"), wxBITMAP_TYPE_ICO_RESOURCE));
   #else
   wxIconBundle icons;
   wxImage::AddHandler( new wxXPMHandler );
   icons.AddIcon(wxT("data/diamonds16.xpm"), wxBITMAP_TYPE_XPM);
   icons.AddIcon(wxT("data/diamonds.xpm"), wxBITMAP_TYPE_XPM);
   SetIcons(icons);
   #endif

   //read in or create new high scores file
   scores = new HiScore();
   ifstream infile(settingsDir("scores.dat"), ios::in | ios::binary);
   if(infile.is_open())
     scores->deserialize(infile);
   infile.close();
   
   //fire up SDL
   SetClientSize(WINWIDTH, WINHEIGHT);
   panel = new SDLPanel(this);
   sound = new SDLSound(settings->soundon);
   
   //init timers
   mainTimer = new wxTimer(this, TIMER_MAIN);
   bonusTimer = new wxTimer(this, TIMER_BONUS);
   
   field = NULL;
   gameOn = false;
   paused = false;

   Field::loadLevelset(wxString::Format(settingsDir("%s"),settings->levelset.c_str()).c_str());

   Centre();
   Show(true);
}

void DiamondsFrame::OnClose(wxCloseEvent&)
{
   //cleanup...
   IsOpen = false;
     
   delete mainTimer;
   delete bonusTimer;
   
   if(field)
     delete field;

   //save high scores
   ofstream outfile(settingsDir("scores.dat"), ios::out|ios::binary);
   if(outfile.is_open())
     scores->serialize(outfile);
   outfile.close();
   delete scores;
   
   delete settings;  
   delete sound;
   Destroy();
}

//the main game loop
void DiamondsFrame::OnTimerMain(wxTimerEvent&)
{
   if(field->gameState == PLAYING)
   {
     if(wxGetKeyState(WXK_LEFT))
       field->moveBall(LEFT);
     else if(wxGetKeyState(WXK_RIGHT))
       field->moveBall(RIGHT);
     else
       field->moveBall();

     sound->DoSoundEffects(field);
   }
   else if(field->gameState == BALLDIED)
   {
     mainTimer->Stop();
     //game over
     if(field->getLives() <= 0)
     {
       bonusTimer->Stop();
       field->gameState = GAMEOVER;
       
       sound->PlayLauging();
       
       //if a high score
       if(field->getScore() >= scores->daScores[9].score)
         EnterHighScore();
       else
         wxMilliSleep(1000);
       
       delete field;
       field = NULL;
       gameOn = false;
       menuBar->EnableTop(1, true);
     }
     else //lives remain, game continues...
     {
       sound->PlayDie();
       
       wxMilliSleep(1000);
       field->decLives();                
       field->resetBall();
       field->gameState = PLAYING;
       mainTimer->Start(MAIN_INTERVAL);
     }
   }
   else if(field->gameState == LEVELWON)
   { 
     mainTimer->Stop();
     bonusTimer->Stop();
     
     sound->PlayLevelWon();

     wxMilliSleep(1200);
     panel->AnimateTimeBonus(sound);
     wxMilliSleep(1000);
           
     if(field->currentLevelNumber() < field->levelCount())
     {
       field->nextLevel();
       panel->Repaint();
       wxMilliSleep(1000);
       
       mainTimer->Start(MAIN_INTERVAL);
       bonusTimer->Start(BONUS_INTERVAL);
     }
     else//out of levels - game over
     { 
       field->gameState = GAMEOVER;

       //if a high score
       if(field->getScore() >= scores->daScores[9].score)
         EnterHighScore();
       else
         wxMilliSleep(1000);
       
       delete field;
       field = NULL;
       gameOn = false;
       menuBar->EnableTop(1, true);
     }
   }

   panel->Repaint(); 
}

//decrease the remaining time bonus
void DiamondsFrame::OnTimerBonus(wxTimerEvent&)
{
   field->decTimeBonus();
   if(field->getTimeBonus()<=0)
     bonusTimer->Stop();
}

//fire menu item events based on key strokes
void DiamondsFrame::OnChar(wxKeyEvent& event)
{
  wxCommandEvent dummy;
  switch(event.GetKeyCode())
  {
    case WXK_F2:
         OnNew(dummy);
         break;
    case 'P':
    case 'p':
         OnPause(dummy);
         break;
    case 'R':
    case 'r':
         OnRestartLevel(dummy);
         break;
    case 'Q':
    case 'q':
         OnQuitGame(dummy);
         break;
    case WXK_ESCAPE:
         OnExit(dummy);
         break;
  }
}

//menu item events
void DiamondsFrame::OnNew(wxCommandEvent&)
{
   mainTimer->Stop();
   bonusTimer->Stop();
   
   menuBar->EnableTop(1, false);
   
   if(field)
     delete field;
   field = new Field();
   
   gameOn = true;
   panel->Repaint();
   
   mainTimer->Start(MAIN_INTERVAL);
   bonusTimer->Start(BONUS_INTERVAL);

}

void DiamondsFrame::OnPause(wxCommandEvent&)
{
   if(!gameOn)
     return;

   if(!paused)
   {
     mainTimer->Stop();
     bonusTimer->Stop();
     paused = true;
   }
   else
   {
     mainTimer->Start(MAIN_INTERVAL);
     if(field->getTimeBonus()>0)
       bonusTimer->Start(BONUS_INTERVAL);
     paused = false;
     panel->Repaint();
   }
}

void DiamondsFrame::OnRestartLevel(wxCommandEvent&)
{
   if(!gameOn)
     return;

   mainTimer->Stop();
   field->gameState = BALLDIED;
   field->restartLevel();
   mainTimer->Start(MAIN_INTERVAL);
   
   if(paused)
   {
     paused = false;
     if(field->getTimeBonus()>0)
       bonusTimer->Start(BONUS_INTERVAL);
   }

}

void DiamondsFrame::OnQuitGame(wxCommandEvent&)
{
   if(!gameOn)
     return;
   
   mainTimer->Stop();
   bonusTimer->Stop();
   
   sound->PlayLauging();
      
   //if high score
   if(field->getScore() >= scores->daScores[9].score)
     EnterHighScore();
   else
     wxMilliSleep(1000);

   delete field;
   field = NULL;
   gameOn = false;
   panel->Repaint();
   menuBar->EnableTop(1, true);
}

void DiamondsFrame::OnPractice(wxCommandEvent&)
{
   int level;     
   level = wxGetNumberFromUser(wxT("Which level would you like to practice?"),
           wxString::Format(wxT("Level (1-%d):"),Field::levelCount()),
           "Diamonds Practice", 1, 1, Field::levelCount(), this);
   if(level == -1)
     return;
    
   mainTimer->Stop();
   bonusTimer->Stop();
   
   menuBar->EnableTop(1, false);
   
   if(field)
     delete field;
   field = new Field(level);
   
   gameOn = true;
   panel->Repaint();
   
   mainTimer->Start(MAIN_INTERVAL);
   bonusTimer->Start(BONUS_INTERVAL);
}

void DiamondsFrame::OnLaunchEditor(wxCommandEvent&)
{
  
}

void DiamondsFrame::OnSoundToggle(wxCommandEvent& event)
{
   sound->SoundOn = event.IsChecked();
   settings->soundon = sound->SoundOn;
}

void DiamondsFrame::OnClearScores(wxCommandEvent&)
{
  if(
    wxMessageBox(wxT("Are you sure you want to clear the high scores?"),
      wxT("Clear High Scores"), wxYES_NO | wxNO_DEFAULT | wxICON_EXCLAMATION)
     == wxYES)
  {
    delete scores;
    scores = new HiScore();
    panel->Repaint();
  }
}

void DiamondsFrame::OnExit(wxCommandEvent&)
{
    Close(true);
}

void DiamondsFrame::OnLevelset(wxCommandEvent& event)
{
   if(gameOn)
     return;

   settings->levelset = levelFiles[event.GetId()-LEVELSET];
   Field::loadLevelset(wxString::Format(settingsDir("%s"),settings->levelset.c_str()).c_str());
}

void DiamondsFrame::OnHowToPlay(wxCommandEvent&)
{
   wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

   wxDialog dialog(this, wxID_ANY, wxT("How to Play Diamonds"), wxDefaultPosition, wxDefaultSize);
   wxPanel panel(&dialog, wxID_ANY);
   wxStaticText text(&panel, wxID_ANY,
      wxT("Use the left and right arrow keys to guide the ball as it bounces "
    "up and down. First, clear away the color blocks by bouncing the ball "
    "against them while the ball matches whichever block's color. Then "
    "get rid of the diamond blocks to beat the level and move on to the "
    "next.\n\n"

    "To change the color of the ball, run it into a paintbrush block. It's "
    "important you clear all the light blue blocks first, the only time the "
    "ball is light blue is at the beginning of a level or after losing a "
    "ball to one of the skull blocks.\n\n"
    
    "See how many levels you can complete or how high a score you can achieve!"
      ),
      wxPoint(15, 15), wxSize(320, 170));
   wxButton okButton(&panel, wxID_OK, wxT("&OK"), wxPoint(136, 195), wxDefaultSize);
   
   vbox->Add(&panel, 1);
   
   dialog.SetSizer(vbox);
   dialog.SetClientSize(350, 225);
   dialog.Centre();
   dialog.ShowModal();
   dialog.Destroy();
}

void DiamondsFrame::OnAbout(wxCommandEvent&)
{
   wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

   wxDialog dialog(this, wxID_ANY, wxT("About Diamonds"), wxDefaultPosition, wxDefaultSize);
   wxPanel panel(&dialog, wxID_ANY);
   wxStaticBox box(&panel, wxID_ANY, wxT("Diamonds 0.5"), wxPoint(5, 5), wxSize(350, 175));
   wxStaticText text(&panel, wxID_ANY,
      wxT("Diamonds is a tribute to the original game, created by Oliver "
      "Dreer and released by Varcon Systems in 1992.\n\n"
      "This program is free software, released under the terms of the GNU "
      "General Public License v3 (http://www.gnu.org/licenses/).\n\n"
      "Source available at https://github.com/mblaine/Diamonds/\n\n"
      "© 2008 Matthew Blaine"
      ),
      wxPoint(15, 25), wxSize(335, 125));
   wxButton okButton(&panel, wxID_OK, wxT("&OK"), wxPoint(138, 150), wxDefaultSize);
   
   vbox->Add(&panel, 1);
   
   dialog.SetSizer(vbox);
   dialog.SetClientSize(360, 185);
   dialog.Centre();
   dialog.ShowModal();
   dialog.Destroy();
}

//get the player's name
void DiamondsFrame::EnterHighScore()
{
   if(field->getPracticeMode())
   {
     wxMilliSleep(1000);
     return;
   }
     
   wxString name = wxGetTextFromUser("You got a high score! "
                    "Enter your name below.", "Congratulations!", "", this,
                    wxDefaultCoord, wxDefaultCoord, true);
   
   if(name.Length() <= 0)
     name = "--------------------";
   //limit to 25 characters
   else if(name.Length() > 25)
     name = name.Left(25);
     
   score toEnter = { string(name.c_str()),
                     field->getScore(),
                     field->levelsetName(),
                     field->currentLevelNumber() };

   scores->enterScore(toEnter);
}

void DiamondsFrame::BuildLevelsetMenu(wxMenu* menu)
{
   wxDir dir(SETTINGSDIR);
    
   wxString filename;
   bool more = dir.GetFirst(&filename, wxT("*.txt"), wxDIR_FILES);

   int i = 0;
   while(more)
   {
     levelFiles.push_back(filename);
     wxString menuText = filename.Left(filename.Len()-4);
     menuText.Replace("_"," ");
     wxMenuItem* item = menu->AppendRadioItem(LEVELSET+i, menuText);
     if(filename==settings->levelset)
       item->Check(true);
     Connect(LEVELSET+i, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(DiamondsFrame::OnLevelset));
     i++;
     more = dir.GetNext(&filename);
   }
}
