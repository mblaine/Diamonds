#include <wx/wxprec.h>

#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include "SDL_util.h"

#include "EditorFrame.h"
#include "Globals.h"

#include <fstream>
using namespace std;

BEGIN_EVENT_TABLE(EditorFrame, wxFrame)
   EVT_CLOSE(EditorFrame::OnClose)
   EVT_CHAR(EditorFrame::OnChar)
   EVT_MENU(FILE_NEW_LEVELSET, EditorFrame::OnNewLevelset)
   EVT_MENU(FILE_OPEN_LEVELSET, EditorFrame::OnOpenLevelset)
   EVT_MENU(FILE_CLOSE_LEVELSET, EditorFrame::OnCloseLevelset)
   EVT_MENU(FILE_SAVE_LEVELSET, EditorFrame::OnSaveLevelset)
   EVT_MENU(FILE_SAVE_LEVELSET_AS, EditorFrame::OnSaveLevelsetAs)
   EVT_MENU(FILE_NEW_LEVEL, EditorFrame::OnNewLevel)
   EVT_MENU(FILE_OPEN_LEVEL, EditorFrame::OnOpenLevel)
   EVT_MENU(FILE_SAVE_LEVEL, EditorFrame::OnSaveLevel)
   EVT_MENU(FILE_COPY_LEVEL, EditorFrame::OnCopyLevel)
   EVT_MENU(FILE_MOVE_LEVEL, EditorFrame::OnMoveLevel)
   EVT_MENU(FILE_IMPORT_LEVELSET, EditorFrame::OnImportLevelset)
   EVT_MENU(FILE_EXPORT_LEVELSET, EditorFrame::OnExportLevelset)
   EVT_MENU(FILE_EXIT, EditorFrame::OnExit)
   EVT_MENU(HELP_ABOUT, EditorFrame::OnAbout)
END_EVENT_TABLE()

EditorFrame::EditorFrame() : wxFrame()
{
   Create( NULL, wxID_ANY, wxT("Diamonds Level Editor"), wxDefaultPosition,
           wxSize(WINWIDTH, WINHEIGHT), wxCAPTION | wxSYSTEM_MENU | 
           wxMINIMIZE_BOX | wxCLOSE_BOX);
   
   IsOpen = true;
   settings = new Settings(settingsDir("editor.ini"));
              
   //menu structure                                                                               
   menuBar = new wxMenuBar();
   wxMenu* fileMenu = new wxMenu();
   wxMenu* helpMenu = new wxMenu();
   
   fileMenu->Append(FILE_NEW_LEVELSET, wxT("&New levelset"));
   fileMenu->Append(FILE_OPEN_LEVELSET, wxT("&Open levelset"));
   fileMenu->Append(FILE_CLOSE_LEVELSET, wxT("&Close levelset"));
   fileMenu->Append(FILE_SAVE_LEVELSET, wxT("&Save levelset"));
   fileMenu->Append(FILE_SAVE_LEVELSET_AS, wxT("Save levelset &as"));
   fileMenu->AppendSeparator();
   fileMenu->Append(FILE_NEW_LEVEL, wxT("Ne&w level"));
   fileMenu->Append(FILE_OPEN_LEVEL, wxT("O&pen level"));
   fileMenu->Append(FILE_SAVE_LEVEL, wxT("Sa&ve level"));
   fileMenu->Append(FILE_COPY_LEVEL, wxT("Cop&y level"));
   fileMenu->Append(FILE_MOVE_LEVEL, wxT("Move &level"));
   fileMenu->AppendSeparator();
   fileMenu->Append(FILE_IMPORT_LEVELSET, wxT("&Import levelset"));
   fileMenu->Append(FILE_EXPORT_LEVELSET, wxT("&Export levelset"));
   fileMenu->AppendSeparator();
   fileMenu->Append(FILE_EXIT, wxT("E&xit\tESC"));
   
   helpMenu->Append(HELP_ABOUT, wxT("&About Diamonds"));
   
   menuBar->Append(fileMenu, wxT("&File"));
   menuBar->Append(helpMenu, wxT("&Help"));
   SetMenuBar(menuBar);
   
   //program icons
   wxIconBundle icons;
   wxImage::AddHandler( new wxXPMHandler );
   icons.AddIcon(wxT("./data/editor.xpm"), wxBITMAP_TYPE_XPM);
   icons.AddIcon(wxT("./data/editor16.xpm"), wxBITMAP_TYPE_XPM);
   SetIcons(icons);
      
   //fire up SDL
   SetClientSize(WINWIDTH, WINHEIGHT);
   
   Centre();
   Show(true);
}

void EditorFrame::OnClose(wxCloseEvent&)
{
   //cleanup...
   IsOpen = false;
     
   delete settings;  

   Destroy();
}

//fire menu item events based on key strokes
void EditorFrame::OnChar(wxKeyEvent& event)
{
  wxCommandEvent dummy;
  switch(event.GetKeyCode())
  {
    case WXK_ESCAPE:
         OnExit(dummy);
         break;
  }
}

void EditorFrame::OnNewLevelset(wxCommandEvent&)
{
}

void EditorFrame::OnOpenLevelset(wxCommandEvent&)
{
}

void EditorFrame::OnCloseLevelset(wxCommandEvent&)
{
}

void EditorFrame::OnSaveLevelset(wxCommandEvent&)
{
}

void EditorFrame::OnSaveLevelsetAs(wxCommandEvent&)
{
}

void EditorFrame::OnNewLevel(wxCommandEvent&)
{
}

void EditorFrame::OnOpenLevel(wxCommandEvent&)
{
}

void EditorFrame::OnSaveLevel(wxCommandEvent&)
{
}

void EditorFrame::OnCopyLevel(wxCommandEvent&)
{
}

void EditorFrame::OnMoveLevel(wxCommandEvent&)
{
}

void EditorFrame::OnImportLevelset(wxCommandEvent&)
{
}

void EditorFrame::OnExportLevelset(wxCommandEvent&)
{
}

void EditorFrame::OnExit(wxCommandEvent&)
{
    Close(true);
}

void EditorFrame::OnAbout(wxCommandEvent&)
{
   wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

   wxDialog dialog(this, wxID_ANY, wxT("About Diamonds"), wxDefaultPosition, wxDefaultSize);
   wxPanel panel(&dialog, wxID_ANY);
   wxStaticBox box(&panel, wxID_ANY, wxT("Diamonds 0.5"), wxPoint(5, 5), wxSize(340, 175));
   wxStaticText text(&panel, wxID_ANY,
      wxT("Diamonds is a tribute to the original game, created by Oliver "
      "Dreer and released by Varcon Systems in 1992.\n\n"
      "This program is free software, released under the terms of the GNU "
      "General Public License v3 (http://www.gnu.org/licenses/).\n\n"
      "Source available at http://diamonds.sourceforge.net/\n\n"
      "© 2008 Matthew Blaine"
      ),
      wxPoint(15, 25), wxSize(325, 125));
   wxButton okButton(&panel, wxID_OK, wxT("&OK"), wxPoint(138, 150), wxDefaultSize);
   
   vbox->Add(&panel, 1);
   
   dialog.SetSizer(vbox);
   dialog.SetClientSize(350, 185);
   dialog.Centre();
   dialog.ShowModal();
   dialog.Destroy();
}
