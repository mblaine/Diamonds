/*
 * CONSTANTS.H
 * created by Matthew Blaine, October 2005
 *
 * These are constants associated with a game of 'Diamonds'
 */

#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include "Resource_Diamonds.h"


//handle to the instance of the main program, necessary for making sounds
//defined in 'Diamonds.cpp'
#ifdef WIN32
#include <windows.h>
#include <mmsystem.h>
extern HINSTANCE hinst1;
extern bool sound;//if sound is on/off
#endif


extern const int ARRAYWIDTH;
extern const int ARRAYHEIGHT;

extern const int BLOCKWIDTH;
extern const int BLOCKHEIGHT;

extern const int FIELDWIDTH;
extern const int FIELDHEIGHT;

extern const int BALLWIDTH;
extern const int BALLHEIGHT;
extern const int BALLDELTA;

//used w/ FieldtoBall
extern const int NOXMOVE;
extern const int XBACKLEFT;
extern const int XBACKRIGHT;
extern const int NOYCHANGE;
extern const int YGOUP;
extern const int YGODOWN;

//messages sent inf BlockToField structure
#define MSG_DIAMOND 10
#define MSG_COLORCHANGEKEY 11
#define MSG_SMALLKEY 12
#define MSG_COLORBLOCK 13
#define MSG_LOCK 14
#define MSG_SOLID 15
#define MSG_SKULL 16
#define MSG_REVERSE 17

//for game state
extern const int PLAYING;
extern const int LEVELWON;
extern const int BALLDIED;
extern const int GAMEOVER;

extern const int LASTLEVEL;

//for window size
extern const int WINWIDTH;
extern const int WINHEIGHT;
extern const int HORIZINDENT;
extern const int VERTINDENT;

//for readouts
extern const int READOUTSWIDTH;
extern const int READOUTSHEIGHT;

extern const int LIVESWIDTH;
extern const int LIVESHEIGHT;

extern const int HASKEYWIDTH;
extern const int HASKEYHEIGHT;

extern const int NUMWIDTH;
extern const int NUMHEIGHT;

extern const int BONUSLIGHTWIDTH;
extern const int BONUSLIGHTHEIGHT;


enum color { nullColor, ltBlue, blue, red, green, brown, purple, orange};

//sent from ball to field object
struct BallToField
{
 int x;//ball's x position
 int y;//y coord
 color c;//current color
};

//sent from field to ball object
struct FieldToBall
{
 int yMoveInfo;//if ball should reverse it's vertical motion
 int xMoveInfo;//tells whether should move horizontally or not
 color change;//color to change to, zero if no change
};

//sent from block to field object
struct BlockToField
{
 int msg;//message, depends on block subclass sending this struct
 int info;//added info, if needed
};

#endif
