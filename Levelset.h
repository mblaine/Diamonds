#ifndef _LEVELSET_H
#define _LEVELSET_H

#include <vector>
#include <string>
#include <fstream>

using namespace std;

extern const char* LEVELERROR;

struct Level
{
   string name;
   string data;
};

class Levelset
{
   private:
      vector<Level> levels;
      string name;
      
   public:
      Levelset();
      Levelset(string filename);
      void ParseFile(string filename);
      int GetLevelCount();
      Level GetLevel(unsigned int levelNumber);
      string GetSetName();
};

#endif
