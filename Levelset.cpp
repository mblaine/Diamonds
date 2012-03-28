#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "Levelset.h"

using namespace std;

//helper function used in ParseFile()
void Replace(string& original, string find, string replaceWith)
{
       unsigned int i = string::npos;
       while( (i = original.find(find)) != string::npos)
         original.replace(i, find.length(),replaceWith);
}

const char* LEVELERROR = "nnnnnnnnnnnn"
                         "SSSnSSnnSSnn"
                         "SnnnSnSnSnSn"
                         "SnnnSnSnSnSn"
                         "SSSnSSnnSSnn"
                         "SnnnSnSnSnSn"
                         "SnnnSnSnSnSn"
                         "SSSnSnSnSnSn"
                         "nnnnnnnnnnnn"
                         "nnnnnnnnnnnn"
                         "nnnnnnnnnnnn"
                         "nnnnnnnnnnnn";

Levelset::Levelset()
{
}

Levelset::Levelset(string filename)
{
   ParseFile(filename);
}

void Levelset::ParseFile(string filename)
{
   Level level;
   string line;
   ostringstream buffer;
   
   levels.clear();
   
   name = filename;
   Replace(name, "data/", ""); //remove path
   Replace(name, ".txt", "");  //remove extension
   Replace(name, "_", " ");    //change _ to spaces

   ifstream infile(filename.c_str());
   if(infile.good())
   {
     while(!infile.eof())
     {
       getline(infile, line);
       
       //ignore blank lines, let lines starting with
       // a '#' be comments
       if(line.length() == 0 || line[0] == '#')
         continue;
       
       //if beginning another level
       if(line.length()>=5 && line.substr(0, 5)=="name=")
       {
         //if level data collected previously
         if(buffer.str().length() > 0)
         {
           level.data = buffer.str();
           levels.push_back(level);
         }
         
         buffer.str("");
         level.name = line.substr(5, line.length()-1);
       }
       else
       {
         buffer << line;
       }    
     }  
     
     //if level data collected
     if(buffer.str().length() > 0)
     {
       level.data = buffer.str();
       levels.push_back(level);
     }
       
     infile.close();
   }
  
   //what, no levels?
   if(levels.size() <= 0)
   {
     level.name = "";
     level.data = LEVELERROR;

     levels.push_back(level);
   }
}

int Levelset::GetLevelCount()
{
   return levels.size();
}

Level Levelset::GetLevel(unsigned int levelNumber)
{
   if(levelNumber <= levels.size())
     return levels[levelNumber-1];
   else
     return Level();
}

string Levelset::GetSetName()
{
   return name;
}
