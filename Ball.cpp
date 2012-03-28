/*
 * BALL.CPP
 * to accompany Ball.h
 */
 
 #include "Ball.h"
 #include "Field.h"
 #include "D2Globals.h"

/*CONTRUCTOR****************************************************************/
 Ball::Ball( int x, int y, int delta, Field *ptr)
 {
       xpos = x; ypos = y;
       deltaX = delta;
       deltaY = delta;
       
       prevBlockX = xpos/BLOCKWIDTH;
       prevBlockY = ypos/BLOCKHEIGHT;
       prevBlockX2 = (xpos+BALLWIDTH)/BLOCKWIDTH;
       prevBlockY2 = (ypos+BALLHEIGHT)/BLOCKHEIGHT;
       
       ballColor = ltBlue;
       moveX = false;
       xBounce = false;
       if ( ptr == NULL )
       {}
       else
       { field = ptr; }
 }

/*DESTRUCTOR****************************************************************/
 Ball::~Ball()
 {;}

/*setDYup*******************************************************************/
 void Ball::setDYup()
 {
  if(deltaY > 0)
  {deltaY *= -1;}
 }
 
/*setDYdown*****************************************************************/
 void Ball::setDYdown()
 {
  if(deltaY < 0)
  {deltaY *= -1;}
 }

/*moveLeftONCE**************************************************************/
 void Ball::moveLeftOnce()
 { 
   if(deltaX > 0 && !moveX)
   { deltaX *= -1; }
     moveX = true;
 }

/*moveRightONCE*************************************************************/
 void Ball::moveRightOnce()
 { 
   if(deltaX < 0 && !moveX)
   { deltaX *= -1; }
     moveX = true;
 }

/*MOVE**********************************************************************/
 void Ball::move()
 {
      //move ball
      ypos += deltaY;
      if(moveX)
      {
       xpos += deltaX; moveX = false; 
       if(xBounce)      
       {xpos += deltaX; xBounce = false;}
      }

      //if now in different field grid rectangle than last time
      if( (prevBlockX != xpos/BLOCKWIDTH)
       || (prevBlockY != ypos/BLOCKHEIGHT)
       || (prevBlockX2 != (xpos+BALLWIDTH)/BLOCKWIDTH)
       || (prevBlockY2 != (ypos+BALLHEIGHT)/BLOCKHEIGHT) )
      //check for a collision
      {
         BallToField info = {xpos, ypos, ballColor};//info to send
         //send it and recieve 'returnInfo'
         FieldToBall returnInfo = field->checkCollision(info);
         //respond to returned info
         if(returnInfo.yMoveInfo == YGOUP)
         {setDYup();}
         else if(returnInfo.yMoveInfo == YGODOWN)
         {setDYdown();}
         
         if(returnInfo.xMoveInfo == XBACKLEFT)
         {moveLeftOnce(); xBounce = true;}
         else if(returnInfo.xMoveInfo == XBACKRIGHT)
         {moveRightOnce(); xBounce = true;}
         
         if(returnInfo.change != nullColor)
         {ballColor = returnInfo.change;}
         
         //set these for next time around
         prevBlockX = xpos/BLOCKWIDTH;
         prevBlockY = ypos/BLOCKHEIGHT;
         prevBlockX2 = (xpos+BALLWIDTH)/BLOCKWIDTH;
         prevBlockY2 = (ypos+BALLHEIGHT)/BLOCKHEIGHT;
      }//if check for collision

      //check for hitting edge of playing field
      if( ypos<=0 )
      {
         setDYdown();
         #ifdef WIN32
         if(sound)
         bounce_snd->Play();
         #endif
      }
      else if( (ypos+BALLHEIGHT)>=FIELDHEIGHT-VERTINDENT*2 )
      {
         setDYup();
         #ifdef WIN32
         if(sound)
         bounce_snd->Play();
         #endif
      }
      if( xpos<=0 )
      {
         moveRightOnce(); xBounce = true;
         #ifdef WIN32
         if(sound)
         bounce_snd->Play();
         #endif
      }
      else if( (xpos+BALLWIDTH) >= FIELDWIDTH -HORIZINDENT*2 )
      {
         moveLeftOnce(); xBounce = true;
         #ifdef WIN32
         if(sound)
         bounce_snd->Play();
         #endif
      }
      
 }//end move()
/***************************************************************************/
/***************************************************************************/
