#include <wx/wxprec.h>

#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include "EditorApp.h"
#include "EditorFrame.h"

IMPLEMENT_APP(EditorApp)

bool EditorApp::OnInit()
{
   frame = new EditorFrame();
   SetTopWindow(frame);

   return true;
}

int EditorApp::OnExit()
{
   //for some reason the frame's OnClose event
   //doesn't fire when the window is closed by
   //the "X" button in the corner; SDL may be
   //interfering???
   if(frame->IsOpen)
     frame->Close(true);

   return wxApp::OnExit();;
}
