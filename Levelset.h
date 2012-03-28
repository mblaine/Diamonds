#ifndef _LEVELSET_H
#define _LEVELSET_H

#include <vector>
#include <string>
#include <fstream>

using namespace std;

extern const char* LEVELERROR;

//Reads in a Diamonds levelset from a text file
//and provides access to the name and layout of
//each individual level

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
      string filename
      
   public:
      Levelset();
      Levelset(const string& filename);
      void ParseFile(const string& filename);
      int GetLevelCount();
      Level GetLevel(unsigned int levelNumber);
      string GetSetName();
      
   //for use by the level editor
      void Save();
      void SaveAs(string filename);
      void MoveLevel(unsigned int currentLevelNumber, unsigned int newLevelNumber);      
      void SetLevel(unsigned int levelNumber, const Level& l);
      void InsertLevel(unsigned int levelNumber, const Level& l);
};

#endif
