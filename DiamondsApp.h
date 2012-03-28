#ifndef _DIAMONDSAPP_H
#define _DIAMONDSAPP_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include "DiamondsFrame.h"

class DiamondsApp : public wxApp
{
   private:
     DiamondsFrame* frame;

   public:
     virtual bool OnInit();
     int OnExit();
};

#endif
