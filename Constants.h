#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#define ARRAYWIDTH 12
#define ARRAYHEIGHT 12

extern const unsigned int BLOCKWIDTH;
extern const unsigned int BLOCKHEIGHT;

extern const unsigned int FIELDWIDTH;
extern const unsigned int FIELDHEIGHT;

extern const unsigned int BALLWIDTH;
extern const unsigned int BALLHEIGHT;
extern const unsigned int BALLDELTA;
extern const unsigned int BALLSTARTX;
extern const unsigned int BALLSTARTY;

//used w/ FieldtoBall
extern const unsigned int NOXMOVE;
extern const unsigned int XBACKLEFT;
extern const unsigned int XBACKRIGHT;
extern const unsigned int NOYCHANGE;
extern const unsigned int YGOUP;
extern const unsigned int YGODOWN;

//for window size
extern const unsigned int WINWIDTH;
extern const unsigned int WINHEIGHT;
extern const unsigned int BORDERWIDTH;

//for heads up display
extern const unsigned int HUDWIDTH;
extern const unsigned int HUDHEIGHT;

extern const unsigned int LIVESWIDTH;
extern const unsigned int LIVESHEIGHT;

extern const unsigned int HASKEYWIDTH;
extern const unsigned int HASKEYHEIGHT;

extern const unsigned int NUMWIDTH;
extern const unsigned int NUMHEIGHT;

extern const unsigned int BONUSLIGHTWIDTH;
extern const unsigned int BONUSLIGHTHEIGHT;


extern const unsigned int MAIN_INTERVAL;
extern const unsigned int BONUS_INTERVAL;

//for Field::moveBall()
enum MoveDirection {NONE, LEFT, RIGHT};

enum GameState{PLAYING, LEVELWON, BALLDIED, GAMEOVER};

//for the ball's current color
enum Color { nullColor, ltBlue, blue, red, green, brown, purple, orange};

enum Block{NULL_BLOCK, COLOR_LTBLUE,
           COLOR_BLUE, COLOR_RED, COLOR_GREEN, COLOR_BROWN, COLOR_PURPLE,
           BRUSH_BLUE, BRUSH_RED, BRUSH_GREEN, BRUSH_BROWN, BRUSH_PURPLE,
           BRUSH_ORANGE, DIAMOND,  KEY, LOCK, SOLID, SKULL, REVERSE};


#endif
