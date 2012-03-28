/*
 * FIELD.H
 * created by Matthew Blaine, October 2005
 *
 * This class represents an entire playing field in a game of 'Diamonds'. It
 * contains a 2D array of 'Block' base class pointers. The 'ball' object
 * call the field's checkCollision() function, causing the field to call a
 * block's collision() function through the base class pointer, if the
 * pointer does not equal NULL. The field then causes the blocks and ball to
 * react accordingly.
 */
 
#ifndef _FIELD_H
#define _FIELD_H

#include <cstdlib>
#include <string>
#include "Constants.h"
#include "Block.h"
#include "Resource_Diamonds.h"
using namespace std;
class Ball;

class Field
{
 private:
         
 //pointers to block objects
 //           [y] [x]
 Block* blocks[12][12];

 //keeps track of whether level won or lost
 int gameState;
 
 //whether player has picked up a small key block
 bool hasSmallKey;
 
 //whether horizontal motion has been reversed
 bool horizReversed;
 
 //the point score for the level this field represents
 int score;
 
 //level this playing field is on
 int currentLevel;
 
 //current number of lives remaining
 int lives;
 
 //time bonus counter
 int timeBonus;
 
 //name of current level
 string levelName;
 
 //how many blocks that are diamonds remain
 int diamondBlocks;
 
 //how many normal color blocks remain
 int colorBlocks;
 
 //pointer to a 'ball' object
 Ball* ball;

 public:
 //default constructor
 Field();
 //destructor
 ~Field();
 
 //the primary purpose of this class, called by a 'ball' object
 FieldToBall checkCollision(BallToField info);
 //begins next level
 void nextLevel();
 //restarts current level
 void restartLevel();
 //reset the ball
 void resetBall();
 
 //functions for controlling the ball
 void moveBall();
 void setToMoveLeft();
 void setToMoveRight();
 
 inline Ball* getBallPtr() {return ball;}
 
 //get method for the Block pointer array
 Block* getBlock(int y, int x);
 
 //get 'lives'
 inline int getLives(){return lives;}
 //increments 'lives'
 inline void incLives(){lives++;}
 //decrements 'lives'
 inline void decLives() {lives--;}
 
 //get 'timeBonus'
 inline int getTimeBonus(){return timeBonus;}
 //decrements 'timeBonus'
 inline void decTimeBonus() {timeBonus--;}
 
 //get current level number
 inline int getCurrentLevel(){return currentLevel;}
 
 //get current game state
 inline int getGameState(){return gameState;}
 //set current gamestate
 inline void setGameStatePlaying(){gameState = PLAYING;}
 inline void setGameStateLevelWon(){gameState = LEVELWON;}
 inline void setGameStateBallDied(){gameState = BALLDIED;}
 inline void setGameStateGameOver(){gameState = GAMEOVER;}
 
 //get horizReversed
 inline bool getHorizReversed(){return horizReversed;}
 //get hasSmallKey
 inline bool getHasSmallKey(){return hasSmallKey;}
 
 //get name of current level (public get method for member variable)
 inline string getLevelName(){return levelName;}
 
 //get score
 inline int getScore(){return score;}
 //increase score
 inline void incScore(unsigned int amount){score+=amount;}
 
 private:
 //sets up blocks for a level
 void setLevel();
 //called by setLevel()
 void fillInBlocks(string input);
 //sets 'levelName' based on 'currentLevel'
 void setLevelName();
 
 
};//end class
/***************************************************************************/

#endif
