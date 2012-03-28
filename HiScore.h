#ifndef _HISCORE_H
#define _HISCORE_H

#include <cstdlib>
#include <string>
#include <fstream>

using namespace::std;

//For persisting high scores

struct score
{
   string name;
   int score;
   string levelset;
   int level;
};

class HiScore
{
   private:
      int version;
   
      //called by enterScore()
      int indexToInsertAt(int score);

   public:
      score daScores[10];
      
      HiScore();
      
      //enter a score into this object
      void enterScore(score toEnter);
     
      void serialize(ofstream &out);
      void deserialize(ifstream &in);
};

#endif

