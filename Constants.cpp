#include "Constants.h"
#include "Resource_Diamonds.h"

const int ARRAYWIDTH = 12;//in # of blocks
const int ARRAYHEIGHT = 12;

const int BLOCKWIDTH = 66;//in pixels
const int BLOCKHEIGHT = 40;

const int FIELDWIDTH = ARRAYWIDTH * BLOCKWIDTH + 2 * HORIZINDENT; //in pixels
const int FIELDHEIGHT = ARRAYHEIGHT * BLOCKHEIGHT + 2 * VERTINDENT;

const int BALLWIDTH = 18;//in pixels
const int BALLHEIGHT = 18;
const int BALLDELTA = 7;

//used w/ FieldtoBall
const int NOXMOVE = 0;
const int XBACKLEFT = 1;
const int XBACKRIGHT = 2;
const int NOYCHANGE = 3;
const int YGOUP = 4;
const int YGODOWN = 5;

//for game state
const int PLAYING = 1;
const int LEVELWON = 2;
const int BALLDIED = 3;
const int GAMEOVER = 4;

const int LASTLEVEL = 30;

//for window size
const int WINWIDTH = FIELDWIDTH + 3 * HORIZINDENT ;//all in pixels
const int WINHEIGHT = FIELDHEIGHT + 14 * VERTINDENT;
const int HORIZINDENT = 10;
const int VERTINDENT = 10;

//for readouts
const int READOUTSWIDTH = FIELDWIDTH;
const int READOUTSHEIGHT = 70;

const int LIVESWIDTH = 171;
const int LIVESHEIGHT = 34;

const int HASKEYWIDTH = 45;
const int HASKEYHEIGHT = 29;

const int NUMWIDTH = 22;
const int NUMHEIGHT = 26;

const int BONUSLIGHTWIDTH = 7;
const int BONUSLIGHTHEIGHT = 12;
