#include <wx/wxprec.h>

#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include "DiamondsApp.h"
#include "DiamondsFrame.h"

IMPLEMENT_APP(DiamondsApp)

bool DiamondsApp::OnInit()
{
   frame = new DiamondsFrame();
   SetTopWindow(frame);

   return true;
}

int DiamondsApp::OnExit()
{
   //for some reason the frame's OnClose event
   //doesn't fire when the window is closed by
   //the "X" button in the corner; SDL may be
   //interfering???
   if(frame->IsOpen)
     frame->Close(true);

   return wxApp::OnExit();;
}
