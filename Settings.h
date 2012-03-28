#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <wx/fileconf.h>

class Settings
{
   private:
      wxFileConfig* settings;
      wxString filename;  

   public:
      bool soundon;
      wxString levelset;
      
      Settings(const char* filename);
      ~Settings();
};
#endif
