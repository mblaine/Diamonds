#include "HiScore.h"
#include <cstdlib>
#include <string>
#include <fstream>

using namespace::std;

HiScore::HiScore()
{
   version = 1;
   int i = 0;
   int j = 10;
   for( ; i<10; i++, j--)
   {
     daScores[i].level = 1;
     daScores[i].levelset = "";
     daScores[i].name = "--------------------";
     daScores[i].score = j;
   }
}

void HiScore::enterScore(score toEnter)
{
   int index = indexToInsertAt(toEnter.score);
   
   //if not a high score
   if(index >=10)
     return;
   
   //shift other scores down
   for(int i = 9; i > index; i--)
   {
     daScores[i].level = daScores[i-1].level;
     daScores[i].name = daScores[i-1].name;
     daScores[i].levelset = daScores[i-1].levelset;
     daScores[i].score = daScores[i-1].score;
   }
   
   //insert new score
   daScores[index].level = toEnter.level;
   daScores[index].name = toEnter.name;
   daScores[index].levelset = toEnter.levelset;
   daScores[index].score = toEnter.score;
}

int HiScore::indexToInsertAt(int score)
{
   int index = 0;
   for(int i=0; i<10; i++)
   {
     //if my score is bigger or equal
     if(score >= daScores[i].score)
     {
       index = i; //put it at this index
       break;
     }
   }
   return index;
}

void HiScore::serialize(ofstream &out)
{
   out.write(reinterpret_cast<char*>(&version),sizeof(&version));
     
   //get length of each string
   int namelength[10];
   int levelsetlength[10];
   for(int i=0; i<10; i++)
     namelength[i] = daScores[i].name.length();
   for(int i=0; i<10; i++)
     levelsetlength[i] = daScores[i].levelset.length();
   
   out.write(reinterpret_cast<char*>(&namelength),sizeof(namelength));
   out.write(reinterpret_cast<char*>(&levelsetlength),sizeof(levelsetlength));
   
   //stream each one
   for(int j=0; j<10; j++)
   {
     out.write(daScores[j].name.c_str(), namelength[j] + 1);
     out.write(daScores[j].levelset.c_str(), levelsetlength[j] + 1);
     out.write(reinterpret_cast<char*>(&daScores[j].score),sizeof(&daScores[j].score));
     out.write(reinterpret_cast<char*>(&daScores[j].level),sizeof(&daScores[j].level));
   }
}

void HiScore::deserialize(ifstream &in)
{
   in.read(reinterpret_cast<char*>(&version),sizeof(&version));
     
   int namelength[10];
   int levelsetlength[10];
   in.read(reinterpret_cast<char*>(&namelength),sizeof(namelength));
   in.read(reinterpret_cast<char*>(&levelsetlength),sizeof(levelsetlength));
   
   char *temp;  //use char* to hold each string temporarily
   for(int k=0; k<10; k++)
   {
     temp = new char[namelength[k]+1];
     in.read(temp, namelength[k]+1);
     daScores[k].name = temp;
     delete [] temp;
     
     temp = new char[levelsetlength[k]+1];
     in.read(temp, levelsetlength[k]+1);
     daScores[k].levelset = temp;
     delete [] temp;
     
     in.read(reinterpret_cast<char*>(&daScores[k].score),sizeof(&daScores[k].score));
     in.read(reinterpret_cast<char*>(&daScores[k].level),sizeof(&daScores[k].level));
   }
}
