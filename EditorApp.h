#ifndef _EDITORAPP_H
#define _EDITORAPP_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include "EditorFrame.h"

class EditorApp : public wxApp
{
   private:
     EditorFrame* frame;

   public:
     virtual bool OnInit();
     int OnExit();
};

#endif
