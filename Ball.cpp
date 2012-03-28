#include "Ball.h"
#include "Constants.h"

Ball::Ball()
{
   x = BALLSTARTX;
   y = BALLSTARTY;
   deltaX = BALLDELTA;
   deltaY = BALLDELTA;
   
   hitWallLastMove = false;
   
   color = ltBlue;
   moveX = false;
   xBounce = false;
}

void Ball::up()
{
   if(deltaY > 0)
     deltaY *= -1;
}
 
void Ball::down()
{
   if(deltaY < 0)
     deltaY *= -1;
}

void Ball::left(bool bounce)
{ 
   if(deltaX > 0 && !moveX)
     deltaX *= -1;
   moveX = true;
   xBounce = bounce;
}

void Ball::right(bool bounce)
{ 
   if(deltaX < 0 && !moveX)
     deltaX *= -1;
   moveX = true;
   xBounce = bounce;
}

void Ball::move()
{
   hitWallLastMove = false;

   signed int tempY = y;
   signed int tempX = x;
   
   //move ball
   tempY += deltaY;
   if(moveX)
   {
     tempX += deltaX;
     moveX = false; 
     if(xBounce)      
     {
       //move a little farther
       tempX += deltaX/2;
       xBounce = false;
     }
   }
   
   //check for hitting edge of playing field
   if( tempY<=0 )
   {
     y = 0;
     down();
     hitWallLastMove = true;
   }
   else if( (tempY+BALLHEIGHT)>=FIELDHEIGHT-BORDERWIDTH*2 )
   {
     y = FIELDHEIGHT-BORDERWIDTH*2-BALLHEIGHT;
     up();
     hitWallLastMove = true;
   }
   else
     y = tempY;

   if( tempX<=0 )
   {
     x = 0;
     right();
     xBounce = true;
     hitWallLastMove = true;
   }
   else if( (tempX+BALLWIDTH) >= FIELDWIDTH-BORDERWIDTH*2 )
   {
     x = FIELDWIDTH-BORDERWIDTH*2-BALLWIDTH;
     left();
     xBounce = true;
     hitWallLastMove = true;
   }
   else
     x = tempX;   
}

