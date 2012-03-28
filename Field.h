#ifndef _FIELD_H
#define _FIELD_H

#include <string>
#include "Ball.h"
#include "Constants.h"
#include "Levelset.h"

using namespace std;

//handles most of the intelligence for a game of Diamonds;
//see Field::moveBall
class Field
{
   private:
 
      static Levelset levels;
      Block blocks[ARRAYHEIGHT][ARRAYWIDTH];
      Ball* ball;

      int lives;
      int score;
      int levelNumber;

      //decreases as a level is played, amount remaining adds to score
      //when a level is completed
      int timeBonus;
      
      bool hasKey;
      bool horizReversed;
      
      bool practiceMode;
      
      //how many diamond blocks are left (0 == level over)
      int diamondBlocks;
      
      //how many color blocks are left (0 == diamond blocks destructable)
      int colorBlocks;

   public:      
      GameState gameState;
                
      //used elsewhere to determine if a sound effect should be played
      Block lastBlockHit;
      bool oneUpLastMove; 
                  
   public:
      Field();
      Field(int levelNumber); //starts practice mode
      ~Field();
      
      //static functions for handling a levelset loaded from a text file
      static void loadLevelset(string filename);
      static int levelCount();
      static string levelsetName();
      
      string currentLevelName();
      int currentLevelNumber();

      void nextLevel();
      void restartLevel();
      void resetBall();
      
      //for controlling the ball; most important part of the whole program
      void moveBall(MoveDirection direction = NONE);
     
      //for accessing the ball
      unsigned int getBallX();
      unsigned int getBallY();
      Color getBallColor();
      bool getBallHitWallLastMove();
      
      Block getBlock(unsigned int y, unsigned int x);

      int getLives();
      void incLives();
      void decLives();

      int getScore();
      void incScore(unsigned int amount);
      
      int getTimeBonus();
      void decTimeBonus();
            
      bool getHorizReversed();
      bool getHasKey();
      
      bool getPracticeMode();
 
   private:
      //sets up blocks for a level
      void setLevel();
};

#endif
