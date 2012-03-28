/*
 * BALL.H
 * created by Matthew Blaine, October 2005
 *
 * This class represents a ball in a game of 'Diamonds'. The ball monitors
 * it's own position on the screen, reverses direction at the screen's edges,
 * and calls a 'Field' object every time it moves enough to potentially be
 * hitting a 'block' in the 'field'. The 'field' will return information on
 * if the ball should reverse direction or change its color.
 */
 
#ifndef _BALL_H
#define _BALL_H

#include <cstdlib>
#include <string>
#include "Constants.h"
#include "Resource_Diamonds.h"
using namespace std;
class Field;

class Ball
{
 private:
 //coord's of ball's top, left-hand corner
 int xpos, ypos;
 
 //current color
 color ballColor;
 
 //pixel's to move per frame
 int deltaX, deltaY;
 
 //whether ball should move horizontally this frame
 bool moveX; 
 
 /* coordinates that will correspond to a field object's 2D
  *  block array index, used to store location at last frame
  *  X & Y are for top left corner of ball, X2 & Y2 are for
  *  bottom right corner of ball
  */
 int prevBlockX;
 int prevBlockY;
 int prevBlockX2;
 int prevBlockY2;
 
 //pointer to play field object
 Field *field;

 //if x-position is moving due to a bounce and not a key-press
 //  (will move twice as far)
 bool xBounce;

 public:
 //constructor
 Ball( int x, int y, int delta, Field *ptr);
 //destructor
 ~Ball();
 
 //get ball's x-coordinate
 inline int getXpos(){return xpos;}
 //get ball's y-coordinate
 inline int getYpos(){return ypos;}
 //get ball's color
 inline color getColor(){return ballColor;}
 
 //set ball's vertical motion to up
 void setDYup();
 //set ball's vertical motion to down
 void setDYdown();
 //move left next time 'move()' is called
 void moveLeftOnce();
 //move right next time 'move()' is called
 void moveRightOnce();

 //alters the x and y coord's, ball's direction, and color
 //   based on a call to a 'Field' object's 'checkCollison()' function
 void move();

};//end class
/***************************************************************************/

#endif
