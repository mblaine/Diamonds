#include "Field.h"
#include "Ball.h"
#include "Constants.h"
#include "Levelset.h"

Levelset Field::levels;

Field::Field()
{
   levelNumber = 1;
   practiceMode = false;
   lives = 6;
   score = 0;
   lastBlockHit = NULL_BLOCK;
   oneUpLastMove = false;

   timeBonus = 46;
   setLevel();

   ball = new Ball();   

   gameState = PLAYING;
   hasKey = false;
   horizReversed = false;
}

Field::Field(int levelNumber)
{
   this->levelNumber = levelNumber;
   practiceMode = true;
   
   lives = 6;
   score = 0;
   lastBlockHit = NULL_BLOCK;
   oneUpLastMove = false;

   timeBonus = 46;
   setLevel();

   ball = new Ball();   

   gameState = PLAYING;
   hasKey = false;
   horizReversed = false;
}

Field::~Field()
{
   if(ball)
     delete ball;
}

void Field::loadLevelset(string filename)
{
   Field::levels.ParseFile(filename);
}

int Field::levelCount()
{
   return Field::levels.GetLevelCount();
}

string Field::levelsetName()
{
   return Field::levels.GetSetName();
}

string Field::currentLevelName()
{
  return levels.GetLevel(levelNumber).name;
}

int Field::currentLevelNumber()
{
   return levelNumber;
}

void Field::nextLevel()
{
  if(!practiceMode)
    levelNumber++;
  else
    score = 0;

  lastBlockHit = NULL_BLOCK;

  timeBonus = 46;
  setLevel();
  
  delete ball;
  ball = new Ball();
  
  gameState = PLAYING;
  hasKey = false;
  horizReversed = false;     
}

void Field::restartLevel()
{      
   lastBlockHit = NULL_BLOCK;
   oneUpLastMove = false;

   setLevel();  

   hasKey = false;
   horizReversed = false;
}

void Field::resetBall()
{
   delete ball;
   ball = new Ball();

   horizReversed = false;
}

void Field::moveBall(MoveDirection direction)
{
   int previousScore = score;
   lastBlockHit = NULL_BLOCK;
   oneUpLastMove = false;
   
   if(direction == LEFT)
     (horizReversed)? ball->right() : ball->left();
   else if(direction == RIGHT)
     (horizReversed)? ball->left() : ball->right();
   
   //the ball class protects against going completely off the screen
   ball->move();
   
   bool collisionOccured = false;

   //coordinates of block the ball hit, if
   //the ball is hitting any block at all
   unsigned int cellX = 0;
   unsigned int cellY = 0;
   
   //if the ball is hitting a block, coordinates
   //of cell (empty or not) diagonal to the one
   //the ball hit
   unsigned int cellNotX = 0;
   unsigned int cellNotY = 0;
   
   //convert the coordiantes of the ball's left, right, top, and
   //bottom sides to indexes int the block[][] array
   unsigned int west = ball->x/BLOCKWIDTH;
   unsigned int east = (ball->x+BALLWIDTH)/BLOCKWIDTH;
   unsigned int north = ball->y/BLOCKHEIGHT;
   unsigned int south = (ball->y+BALLHEIGHT)/BLOCKHEIGHT;
      

   //determine if a corner of the ball is making contact with a
   //block; don't count it if the ball is wedged in a corner
   //formed by three blocks; consider it to be hitting either
   //the block immediately above/below it, or the one next to
   //it, not the one diagonally   

   if(blocks[north][west]!=NULL_BLOCK 
      && !(blocks[south][west]!=NULL_BLOCK&&blocks[north][east]!=NULL_BLOCK))
   {
     cellY = north;
     cellX = west;
     cellNotY = south;
     cellNotX = east;
     collisionOccured = true;
   }

   if(east<ARRAYWIDTH && !collisionOccured)
   {
     if(blocks[north][east]!=NULL_BLOCK
        &&!(blocks[south][east]!=NULL_BLOCK&&blocks[north][west]!=NULL_BLOCK))
     {
       cellY = north;
       cellX = east;
       cellNotY = south;
       cellNotX = west;
       collisionOccured = true;
     }
   }

   if(south<ARRAYHEIGHT && east<ARRAYWIDTH && !collisionOccured)
   {
     if(blocks[south][east]!=NULL_BLOCK 
        &&!(blocks[north][east]!=NULL_BLOCK&&blocks[south][west]!=NULL_BLOCK))
     {
       cellY = south;
       cellX = east;
       cellNotY = north;
       cellNotX = west;
       collisionOccured = true;
     }
   }
   
   if(south<ARRAYHEIGHT && !collisionOccured)
   {
     if(blocks[south][west]!=NULL_BLOCK
        &&!(blocks[north][west]!=NULL_BLOCK&&blocks[south][east]!=NULL_BLOCK))
     {
       cellY = south;
       cellX = west;
       cellNotY = north;
       cellNotX = east;
       collisionOccured = true;
     }
   }

   if(!collisionOccured)
     return;

     
   //if a collision, determine which way the ball should go now;
   //also, adjust ball's position so it's not overlapping the block

   //squarely hit side of block
   if((north==south)
   ||
   //or hit border between two blocks on the side of the ball,
   //not hitting blocks above or below
   (north!=south && blocks[cellNotY][cellX] != NULL_BLOCK
                 && blocks[cellNotY][cellNotX] == NULL_BLOCK
                 && blocks[cellY][cellNotX] == NULL_BLOCK))
   {
     //coming in from the left? go right ...
     if(west==cellX)
     {
       ball->right(true);
       ball->x += BLOCKWIDTH - ball->x%BLOCKWIDTH;
     }
     else
     {
       ball->left(true);
       ball->x -= (ball->x+BALLWIDTH)%BLOCKWIDTH;
     }
   }
   
   //squarely hit top or bottom of block
   else if((west==east)
   ||
   //or hit border between two blocks either above or below the ball,
   //not hitting blocks on either side
   (west!=east && blocks[cellY][cellNotX] != NULL_BLOCK
               && blocks[cellNotY][cellX] == NULL_BLOCK
               && blocks[cellNotY][cellNotX] == NULL_BLOCK))
   {
     //coming in from below? go down ...
     if(north==cellY)
     {
       ball->down();
       ball->y += BLOCKHEIGHT - ball->y%BLOCKHEIGHT;
     }
     else
     {
       ball->up();
       ball->y -= (ball->y+BALLHEIGHT)%BLOCKHEIGHT;
     }
   }
   
   //hit corner of block, not touching any others blocks around
   else if(north!=south && blocks[cellNotY][cellX] == NULL_BLOCK
                        && blocks[cellNotY][cellNotX] == NULL_BLOCK
                        && blocks[cellY][cellNotX] == NULL_BLOCK)
   {
     //top of ball hitting block, less than half way into the block
     if(north==cellY && ball->y > (cellY+1)*BLOCKHEIGHT - BALLHEIGHT/2)
     {
       ball->down();
       ball->y += BLOCKHEIGHT - ball->y%BLOCKHEIGHT;
     }
     //bottom of ball hitting block, less than half way into the block
     else if(ball->y + BALLHEIGHT < cellY*BLOCKHEIGHT + BALLHEIGHT/2)
     {
       ball->up();
       ball->y -= (ball->y+BALLHEIGHT)%BLOCKHEIGHT;
     }
     //left side of ball hitting block, less than half way into block
     if(west==cellX && ball->x > (cellX+1)*BLOCKWIDTH - BALLWIDTH/2)
     {
       ball->right(true);
       ball->x += BLOCKWIDTH - ball->x%BLOCKWIDTH;
     }
     //right side of ball hitting block, less than half way into block 
     else if(ball->x + BALLWIDTH < cellX*BLOCKWIDTH + BALLWIDTH/2)
     {
       ball->left(true);
       ball->x -= (ball->x+BALLWIDTH)%BLOCKWIDTH;
     }
   }
   
   //ball actually wedged in a corner
   else if(west!=east && blocks[cellNotY][cellNotX]!=NULL_BLOCK)
   {
     //if ball less than half its height into the block above it,
     //it was coming from below: go back down
     if(ball->y >= south*BLOCKHEIGHT - BALLHEIGHT/2)
     {
       ball->down();
       ball->y += BLOCKHEIGHT - ball->y%BLOCKHEIGHT;
     }
     else
     {
       ball->up();
       ball->y -= (ball->y+BALLHEIGHT)%BLOCKHEIGHT;
     }
   
     //if ball less than half its width into block to its left,
     //it was coming from the right, go back that way
     if(ball->x >= east*BLOCKWIDTH - BALLWIDTH/2)
     {
       ball->right(true);
       ball->x += BLOCKWIDTH - ball->x%BLOCKWIDTH;
     }
     else
     {
       ball->left(true);
       ball->x -= (ball->x+BALLWIDTH)%BLOCKWIDTH;
     }
   }


   //handle just what kind of block was hit

   lastBlockHit = blocks[cellY][cellX];                   
   switch(lastBlockHit)
   {
     case DIAMOND:
          if(colorBlocks == 0)//if no color blocks left
          {
            blocks[cellY][cellX] = NULL_BLOCK;//destroy it
            diamondBlocks--;
            score+=100;
          }
          else
            lastBlockHit = SOLID;
 
          if(diamondBlocks == 0)//if no more diamonds now
            gameState = LEVELWON;
          break;
    
     case SKULL:
          gameState = BALLDIED;
          break;
   
     case BRUSH_BLUE://change ball's color
     case BRUSH_RED:
     case BRUSH_GREEN:
     case BRUSH_BROWN:
     case BRUSH_PURPLE:
     case BRUSH_ORANGE:
     case BRUSH_YELLOW:
     case BRUSH_BLACK:
     case BRUSH_WHITE:
          //convert from Block to Color enum
          if(ball->color != (Color)blocks[cellY][cellX]-BRUSH_BLUE+2)
            ball->color = (Color)(blocks[cellY][cellX]-BRUSH_BLUE+2);
          else
            lastBlockHit = SOLID;
          break;
   
     case COLOR_LTBLUE:
     case COLOR_BLUE:
     case COLOR_RED:
     case COLOR_GREEN:
     case COLOR_BROWN:
     case COLOR_PURPLE:
     case COLOR_ORANGE:
     case COLOR_YELLOW:
     case COLOR_BLACK:
     case COLOR_WHITE:
          //if colors match    (convert from BLock to Color enum)
          if(ball->color == (Color)blocks[cellY][cellX]-COLOR_LTBLUE+1)
          {
            blocks[cellY][cellX] = NULL_BLOCK;//destroy it
            colorBlocks--;
            score+=3;
          }
          else
            lastBlockHit = SOLID;
          break;
      
     case KEY:
          if(!hasKey && ball->color == orange)
          {
            blocks[cellY][cellX] = NULL_BLOCK;
            hasKey = true;
            score+=25;
          }
          else
            lastBlockHit = SOLID;         
          break;
   
     case LOCK:
          if(hasKey && ball->color == orange)
          {
            blocks[cellY][cellX] = NULL_BLOCK;
            hasKey = false;
            score+=50;
          }
          else
            lastBlockHit = SOLID;
         break;
   
     case REVERSE:
          horizReversed = !horizReversed;
          blocks[cellY][cellX] = NULL_BLOCK;
          break;
          
     default:
          break;
   }
   
   //extra life earned?
   if(previousScore/5000 < score/5000 )
   {
     lives++;
     oneUpLastMove = true;
   }    
}

unsigned int Field::getBallX()
{
   return ball->x;
}

unsigned int Field::getBallY()
{
   return ball->y;
}

Color Field::getBallColor()
{
   return ball->color;
}

bool Field::getBallHitWallLastMove()
{
   return ball->hitWallLastMove;
}

Block Field::getBlock(unsigned int y, unsigned int x)
{
   if( y<ARRAYHEIGHT && x<ARRAYWIDTH)
     return blocks[y][x];
   else
     return NULL_BLOCK;
}

int Field::getLives()
{
   return lives;
}

void Field::incLives()
{
   if(!practiceMode)
     lives++;
}

void Field::decLives()
{
   if(!practiceMode)
     lives--;
}

int Field::getScore()
{
   return score;
}

void Field::incScore(unsigned int amount)
{
   score+=amount;
}

int Field::getTimeBonus()
{
   return timeBonus;
}

void Field::decTimeBonus()
{
   timeBonus--;
}
      
bool Field::getHorizReversed()
{
   return horizReversed;
}

bool Field::getHasKey()
{
   return hasKey;
}

bool Field::getPracticeMode()
{
   return practiceMode;
}

void Field::setLevel()
{ 
	diamondBlocks = 0;
   colorBlocks = 0;
	
   Level current = levels.GetLevel(levelNumber);
   string input = current.data;
	
   /*
    * l(ight blue), b(lue), r(ed), g(reen), (bro)w(n), p(urple),
    *   o(range), y(ellow), (blac)k, (w)h(ite)      =  color blocks
    * B(lue), R(ed), G(reen), (bro)W(n), P(urple), O(range), Y(ellow),
    *      (blac)K, (w)H(ite)                         =  color brushes
    * d(iamond), s(kull), k(ey), L(ock), S(olid), (re)v(erse), n(ull)
    */

   char c;
   if(input.length() != ARRAYWIDTH*ARRAYHEIGHT)
     input = LEVELERROR;

   for(unsigned int y=0; y<ARRAYHEIGHT; y++)
   {
     for(unsigned int x=0; x<ARRAYWIDTH; x++)
     {
       c = input[ y*ARRAYWIDTH  + x];
       switch(c)
       {
         //color blocks
         case 'l':
         blocks[y][x] = COLOR_LTBLUE;
         colorBlocks++;
         break;
         
         case 'b':
         blocks[y][x] = COLOR_BLUE;
         colorBlocks++;
         break;
         
         case 'r':
         blocks[y][x] = COLOR_RED;
         colorBlocks++;
         break;
         
         case 'g':
         blocks[y][x] = COLOR_GREEN;
         colorBlocks++;
         break;
         
         case 'w':
         blocks[y][x] = COLOR_BROWN;
         colorBlocks++;
         break;
         
         case 'p':
         blocks[y][x] = COLOR_PURPLE;
         colorBlocks++;
         break;
         
         case 'o':
         blocks[y][x] = COLOR_ORANGE;
         colorBlocks++;
         break;
         
         case 'y':
         blocks[y][x] = COLOR_YELLOW;
         colorBlocks++;
         break;
         
         case 'c':
         blocks[y][x] = COLOR_BLACK;
         colorBlocks++;
         break;
         
         case 'h':
         blocks[y][x] = COLOR_WHITE;
         colorBlocks++;
         break;
               
         //brush blocks
         case 'B':
         blocks[y][x] = BRUSH_BLUE;
         break;
         
         case 'R':
         blocks[y][x] = BRUSH_RED;
         break;
         
         case 'G':
         blocks[y][x] = BRUSH_GREEN;
         break;
         
         case 'W':
         blocks[y][x] = BRUSH_BROWN;
         break;
         
         case 'P':
         blocks[y][x] = BRUSH_PURPLE;
         break;
         
         case 'O':
         blocks[y][x] = BRUSH_ORANGE;
         break;
         
         case 'Y':
         blocks[y][x] = BRUSH_YELLOW;
         break;
         
         case 'C':
         blocks[y][x] = BRUSH_BLACK;
         break;
         
         case 'H':
         blocks[y][x] = BRUSH_WHITE;
         break;
                  
         //the rest
         case 'd':
         blocks[y][x] = DIAMOND;
         diamondBlocks++;
         break;
         
         case 's':
         blocks[y][x] = SKULL;
         break;
         
         case 'k':
         blocks[y][x] = KEY;
         break;
         
         case 'L':
         blocks[y][x] = LOCK;
         break;
         
         case 'S':
         blocks[y][x] = SOLID;
         break;
         
         case 'v':
         blocks[y][x] = REVERSE;
         break;
         
         default:
         blocks[y][x] = NULL_BLOCK;
         break;
       }
     }
   }
}
