#ifndef _BALL_H
#define _BALL_H

#include "Constants.h"

//class to keep track of where and what color the ball is;
//a ball object is mostly manipulated by Field::moveBall(),
//but this class does take charge of keeping the ball from
//going completely off the screen (see Ball::move())
class Ball
{
   public:
   
      //ball's top-left corner's coordinates
      unsigned int x, y;

      Color color;
      
      //number of pixels to add to the x and y
      //coordinates per frame
      int deltaX, deltaY;
      
      //should the ball move horizontally this frame?
      bool moveX; 
      
      //if x-position is moving due to a bounce and not a key-press
      //(will move 1.5x as far)
      bool xBounce;

      Ball();
      
      //change the ball's direction; left/right only affect next frame;
      //if bounce is true, the ball will move 1.5x as far
      void up();
      void down();
      void left(bool bounce = false);
      void right(bool bounce = false);
      
      //moves the ball, protecting against leaving the playing field;
      //see also: Field::moveBall()
      void move();
      
      //if the ball ran into a wall after the previous move;
      //used elsewhere to determine if a sound effect should be played
      bool hitWallLastMove;
};

#endif
