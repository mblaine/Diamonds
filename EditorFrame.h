#ifndef _EDITORFRAME_H
#define _EDITORFRAME_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include "Settings.h"

class EditorFrame : public wxFrame
{
   private:
      Settings* settings;
      
      wxMenuBar* menuBar;
      
   public:
      EditorFrame();
      
      void OnClose(wxCloseEvent&);
      void OnChar(wxKeyEvent&);
      
      //menuitem events
      void OnNewLevelset(wxCommandEvent&);
      void OnOpenLevelset(wxCommandEvent&);
      void OnCloseLevelset(wxCommandEvent&);
      void OnSaveLevelset(wxCommandEvent&);
      void OnSaveLevelsetAs(wxCommandEvent&);
      void OnNewLevel(wxCommandEvent&);
      void OnOpenLevel(wxCommandEvent&);
      void OnSaveLevel(wxCommandEvent&);
      void OnCopyLevel(wxCommandEvent&);
      void OnMoveLevel(wxCommandEvent&);
      void OnImportLevelset(wxCommandEvent&);
      void OnExportLevelset(wxCommandEvent&);
      void OnExit(wxCommandEvent&);
      void OnAbout(wxCommandEvent&);
      
      //see: EditorApp::OnExit
      bool IsOpen;
      
      DECLARE_EVENT_TABLE()
};

//menu and timer id's
enum
{
   FILE_NEW_LEVELSET = 1,
   FILE_OPEN_LEVELSET,
   FILE_CLOSE_LEVELSET,
   FILE_SAVE_LEVELSET,
   FILE_SAVE_LEVELSET_AS,
   FILE_NEW_LEVEL,
   FILE_OPEN_LEVEL,
   FILE_SAVE_LEVEL,
   FILE_COPY_LEVEL,
   FILE_MOVE_LEVEL,
   FILE_IMPORT_LEVELSET,
   FILE_EXPORT_LEVELSET,
   FILE_EXIT,
   HELP_ABOUT
};

#endif
