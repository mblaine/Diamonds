#include "Constants.h"

const unsigned int BLOCKWIDTH = 66;//in pixels
const unsigned int BLOCKHEIGHT = 40;

const unsigned int FIELDWIDTH = ARRAYWIDTH * BLOCKWIDTH + 2 * BORDERWIDTH; //in pixels
const unsigned int FIELDHEIGHT = ARRAYHEIGHT * BLOCKHEIGHT + 2 * BORDERWIDTH;

const unsigned int BALLWIDTH = 20;//in pixels
const unsigned int BALLHEIGHT = 20;
const unsigned int BALLDELTA = 9;
const unsigned int BALLSTARTX = 11*BLOCKWIDTH;
const unsigned int BALLSTARTY = 11*BLOCKHEIGHT;

//used w/ FieldtoBall
const unsigned int NOXMOVE = 0;
const unsigned int XBACKLEFT = 1;
const unsigned int XBACKRIGHT = 2;
const unsigned int NOYCHANGE = 3;
const unsigned int YGOUP = 4;
const unsigned int YGODOWN = 5;

//for window size
const unsigned int WINWIDTH = FIELDWIDTH; //in pixels
const unsigned int WINHEIGHT = FIELDHEIGHT + HUDHEIGHT;
const unsigned int BORDERWIDTH = 10;

//for heads up display
const unsigned int HUDWIDTH = FIELDWIDTH;
const unsigned int HUDHEIGHT = 61;

const unsigned int LIVESWIDTH = 171;
const unsigned int LIVESHEIGHT = 34;

const unsigned int HASKEYWIDTH = 45;
const unsigned int HASKEYHEIGHT = 29;

const unsigned int NUMWIDTH = 22;
const unsigned int NUMHEIGHT = 26;

const unsigned int BONUSLIGHTWIDTH = 7;
const unsigned int BONUSLIGHTHEIGHT = 12;


const unsigned int MAIN_INTERVAL = 26;
const unsigned int BONUS_INTERVAL = 1250;
