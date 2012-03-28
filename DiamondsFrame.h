#ifndef _DIAMONDSFRAME_H
#define _DIAMONDSFRAME_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include "SDLPanel.h"
#include "SDLSound.h"

#include "Settings.h"

#include <vector>
using namespace std;

class DiamondsFrame : public wxFrame
{
   private:
      //objects managing animation and sound effects
      SDLPanel* panel;
      SDLSound* sound;
      //timers for driving the game
      wxTimer* mainTimer;
      wxTimer* bonusTimer;
      
      Settings* settings;
      
      wxMenuBar* menuBar;
      vector<wxString> levelFiles;
      bool paused;
      
      //asks the player their name
      void EnterHighScore();
      void BuildLevelsetMenu(wxMenu* menu);
      
   public:
      DiamondsFrame();
      
      void OnClose(wxCloseEvent&);
      void OnTimerMain(wxTimerEvent&);
      void OnTimerBonus(wxTimerEvent&);
      void OnChar(wxKeyEvent&);
      
      //menuitem events
      void OnNew(wxCommandEvent&);
      void OnPause(wxCommandEvent&);
      void OnRestartLevel(wxCommandEvent&);
      void OnQuitGame(wxCommandEvent&);
      void OnPractice(wxCommandEvent&);
      void OnLaunchEditor(wxCommandEvent&);
      void OnSoundToggle(wxCommandEvent&);
      void OnClearScores(wxCommandEvent&);
      void OnExit(wxCommandEvent&);
      void OnLevelset(wxCommandEvent&);
      void OnHowToPlay(wxCommandEvent&);
      void OnAbout(wxCommandEvent&);
      
      //see: DiamondsApp::OnExit
      bool IsOpen;
      
      DECLARE_EVENT_TABLE()
};

//menu and timer id's
enum
{
   GAME_NEW = 1,
   GAME_PAUSE,
   GAME_RESTARTLEVEL,
   GAME_QUITGAME,
   GAME_PRACTICE,
   GAME_LAUNCHEDITOR,
   GAME_SOUNDTOGGLE,
   GAME_CLEARSCORES,
   GAME_EXIT,
   HELP_HOWTOPLAY,
   HELP_ABOUT,
   TIMER_MAIN,
   TIMER_BONUS,
   LEVELSET //has to be last
};

#endif
