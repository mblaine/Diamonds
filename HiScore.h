/*
 * HISCORE.H
 * created by Matthew Blaine, November 2005
 *
 * This class serves to hold the high scores for a game of 'Diamonds'. It
 * also facilitates reading and writing its member variables to and from a
 * binary file.
 */

#ifndef _HISCORE_H
#define _HISCORE_H

#include <cstdlib>
#include <string>
#include <fstream>

using namespace::std;

struct score
{
 string name;
 int score;
 int level;
};

class HiScore
{
      private:
      //used when serializing the 10 name strings
      int namelength[10];
      public:
      //what do you think?
      score daScores[10];
      
      HiScore();
      ~HiScore();
      
      //enter a score into this object
      void enterScore(score toEnter);
      //called by enterScore()
      int indexToInsertAt(int score);
      
      //for writing object to binary file
      void serialize(fstream &out);
      //for reading object from binary file
      void deserialize(fstream &in);
      
      
};//end class HiScore
/***************************************************************************/

#endif

