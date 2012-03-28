#include "HiScore.h"
#include <cstdlib>
#include <string>
#include <fstream>

using namespace::std;

/*CONSTRUCTOR***************************************************************/
HiScore::HiScore()
{
    int i = 0; int j = 10;
    for(i; i<10; i++, j--)
    {
     daScores[i].level = 1;
     daScores[i].name = "----------";
     daScores[i].score = j;
    }
}

/*DESTRUCTOR****************************************************************/
HiScore::~HiScore(){;}

/*enterSCORE****************************************************************/
void HiScore::enterScore(score toEnter)
{

     int index = indexToInsertAt(toEnter.score);

     if(index >=10)//if not a high score
     {return;}
     
     for(int i = 9; i > index; i--)//shift other scores down
     {
      daScores[i].level = daScores[i-1].level;
      daScores[i].name = daScores[i-1].name;
      daScores[i].score = daScores[i-1].score;
     }
     
     //insert new score
     daScores[index].level = toEnter.level;
     daScores[index].name = toEnter.name;
     daScores[index].score = toEnter.score;
     return;
}

/*indexToInsertAT***********************************************************/
int HiScore::indexToInsertAt(int score)
{
    int index = 0;//to return
    for(int i=0; i<10; i++)
    {
     if(score >= daScores[i].score)//if my score is bigger or equal
     {index = i; break;}//put it at this index
    }
    return index;
}

/*SERIALIZE*****************************************************************/
void HiScore::serialize(fstream &out)
{
      for(int i=0; i<10; i++)//get length of each name string
      {namelength[i] = daScores[i].name.length();}
      
      out.write( reinterpret_cast<char*>(&namelength),sizeof(namelength) );
      
      for(int j=0; j<10; j++)//stream each one at a time
      {
       out.write( daScores[j].name.c_str(), namelength[j] + 1 );
       out.write(reinterpret_cast<char*>(&daScores[j].score), 
                                               sizeof(&daScores[j].score) );
       out.write(reinterpret_cast<char*>(&daScores[j].level), 
                                               sizeof(&daScores[j].level) );
      }
      return;
}

/*DESERIALIZE***************************************************************/
void HiScore::deserialize(fstream &in)
{
      in.read( reinterpret_cast<char*>(&namelength),sizeof(namelength) );
      
      char *temp;  //use char* to hold each name string temporarily
      for(int k=0; k<10; k++)
      {
      temp = new char[namelength[k]+1];
      in.read( temp, namelength[k]+1 );
      daScores[k].name = temp;
      delete [] temp;
      in.read(reinterpret_cast<char*>(&daScores[k].score), 
                                               sizeof(&daScores[k].score) );
      in.read(reinterpret_cast<char*>(&daScores[k].level), 
                                               sizeof(&daScores[k].level) );
      }
      return;
}
/***************************************************************************/
/***************************************************************************/
