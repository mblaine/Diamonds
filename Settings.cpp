#include "Settings.h"

#include <wx/fileconf.h>
#include <wx/filefn.h>
#include <wx/wfstream.h>

Settings::Settings(const char* filename)
{
   this->filename = filename;

   //defaults
   soundon = true;
   levelset = wxT("Diamonds.txt");
                          
   if(wxFileExists(this->filename))
   {
     wxFileInputStream infile(this->filename);
     settings = new wxFileConfig(infile);
     settings->Read(wxT("soundon"), &soundon);
     settings->Read(wxT("levelset"), &levelset);
   }
   else
   {
     settings = new wxFileConfig();
   }
}

Settings::~Settings()
{
   settings->Write(wxT("soundon"), soundon);
   settings->Write(wxT("levelset"), levelset);
   
   wxFileOutputStream outfile(filename);

   if(outfile.IsOk())
     settings->Save(outfile);

   delete settings;
}
