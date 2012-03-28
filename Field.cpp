/*
 * FIELD.CPP
 * to accompany Field.h
 */
 
 #include "Field.h"
 #include "Ball.h"
 #include "D3Globals.h"


/*CONSTRUCTOR***************************************************************/
 Field::Field()//default constructor
 {
  currentLevel = 1;
  lives = 6;
  timeBonus = 46;
  ball = NULL;
  ball = new Ball(11*BLOCKWIDTH, 11*BLOCKHEIGHT, BALLDELTA,this);
  setLevel();
 
  gameState = PLAYING;
  hasKey = false;
  horizReversed = false;
  score = 0;
  setLevelName();
 
 }//end default constructor

/*DESTRUCTOR****************************************************************/
 Field::~Field()//destructor
 {
  if(ball != NULL)
  {
    delete ball;
    ball = NULL;
  }
 }//end destructor

/*nextLEVEL*****************************************************************/
void Field::nextLevel()
{
  currentLevel++;
  
  delete ball;
  ball = NULL;
  
  gameState = PLAYING;
  hasKey = false;
  horizReversed = false;

  ball = new Ball(11*BLOCKWIDTH, 11*BLOCKHEIGHT, (int)BALLDELTA, this);
  setLevel();
  timeBonus = 46;
  setLevelName();
     
}//end nextLevel()

/*restartLEVEL**************************************************************/
void Field::restartLevel()
{      
  hasKey = false;
  horizReversed = false;

  setLevel();
     
}//end restartLevel()

/*resetBALL*****************************************************************/
void Field::resetBall()
{
 delete ball; ball = NULL;
 ball = new Ball(11*BLOCKWIDTH, 11*BLOCKHEIGHT, BALLDELTA,this);
 horizReversed = false;
}//end resetBall()

/*fillInBLOCKS**************************************************************/
void Field::fillInBlocks(string input)
{
 /*
  * l(ight blue), b(lue), r(ed), g(reen), (bro)w(n), p(urple) = color blocks
  * B(lue), R(ed), G(reen), (bro)W(n), P(urple), O(range) = color brushes
  * d(iamond), s(kull), (k)ey, L(ock), S(olid), (re)v(erse), n(ull)
  */

 //we're going to switch on each character in the input string to determine
 //  what to feed into the constructor for each Block pointer in the array.
 char c;

 if(input.length() != ARRAYWIDTH*ARRAYHEIGHT)
 {return;}

 for( int y=0; y<ARRAYHEIGHT; y++)//for every row
  {
       for( int x=0; x<ARRAYWIDTH; x++)//for every block in that row
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


               }//end switch
           }//end for every x
      }//end for every y
}//end fillInBlocks()

/*checkCOLLISION************************************************************/
 //the primary purpose of this class, called by a 'ball' object
 FieldToBall Field::checkCollision(BallToField info)
 {
      //SET EVERYTHING UP////////////////////////////////////////////////////
      FieldToBall toReturn = {NOYCHANGE, NOXMOVE, nullColor};//to return

      bool collisionOccured = false;//if there's a collision
      int cellX = -1;//used in event of a collison
      int cellY = -1;

      int west = info.x/BLOCKWIDTH; // x-coord of left side of ball
      int east = (info.x+BALLWIDTH)/BLOCKWIDTH;//right side
      int north = info.y/BLOCKHEIGHT;//y-coord of top of ball
      int south = (info.y+BALLHEIGHT)/BLOCKHEIGHT;//bottom
      
    //DETERMINE IF A COLLISION///////////////////////////////////////////////     

           //collision w/ ball's top-left corner, but not if ball is wedged
           //  in a corner, touching a block it shouldn't
     if(north>=0 && west>=0)
     {
      if(blocks[north][west]!=NULL_BLOCK 
       &&!(blocks[south][west]!=NULL_BLOCK&&blocks[north][east]!=NULL_BLOCK))
       {cellY = north; cellX = west; collisionOccured = true;}
     }

          //collision w/ ball's top-right corner, but not if ball is wedged
          //  in a corner, touching a block it shouldn't
    if(north>=0 && east<ARRAYWIDTH && !collisionOccured)
    {
      if(blocks[north][east]!=NULL_BLOCK
       &&!(blocks[south][east]!=NULL_BLOCK&&blocks[north][west]!=NULL_BLOCK))
       {cellY = north; cellX = east; collisionOccured = true;}
    }

          //collision w/ ball's bottom-right corner, but not if ball is
          //  wedged in a corner, touching a block it shouldn't
    if(south<ARRAYHEIGHT && east<ARRAYWIDTH && !collisionOccured)
    {
      if(blocks[south][east]!=NULL_BLOCK 
       &&!(blocks[north][east]!=NULL_BLOCK&&blocks[south][west]!=NULL_BLOCK))
       {cellY = south; cellX = east; collisionOccured = true;}
    }

        //collision w/ ball's bottom-left corner, but not if ball is
        //  wedged in a corner, touching a block it shouldn't
    if(south<ARRAYHEIGHT && west>=0 && !collisionOccured)
    {
      if(blocks[south][west]!=NULL_BLOCK
       &&!(blocks[north][west]!=NULL_BLOCK&&blocks[south][east]!=NULL_BLOCK))
       {cellY = south; cellX = west; collisionOccured = true;}
    }

    //IF COLLISION, ALTER BALL'S DIRECTION BASED ON ADJACENT BLOCK POSITIONS
    if(!collisionOccured)
    {return toReturn;}


   if(north==cellY && west==cellX)//impact by top-left corner of ball
   {
    //checking for all possible situations
      
    if(north==south)//squarely hit side of block
    {toReturn.xMoveInfo = XBACKRIGHT;}
      
    //hit border between blocks on the side, not hitting blocks above and
    //  below
    else if(north!=south && blocks[south][west]!=NULL
          &&blocks[south][east]==NULL_BLOCK&&blocks[north][east]==NULL_BLOCK)
          {toReturn.xMoveInfo = XBACKRIGHT;}
      
    //hit corner of block, not touching any others blocks around
    else if(north!=south && blocks[south][west]==NULL_BLOCK
          &&blocks[south][east]==NULL_BLOCK&&blocks[north][east]==NULL_BLOCK)
    {      
             //if close enough to corner...
             if( info.y > cellY*BLOCKHEIGHT + 2*BLOCKHEIGHT/3 )
             {toReturn.yMoveInfo = YGODOWN;}
             if( info.x > cellX*BLOCKWIDTH + 2*BLOCKWIDTH/3 )
             {toReturn.xMoveInfo = XBACKRIGHT;}
    }
      
    else if(west==east)//squarely hit bottom of block
    {toReturn.yMoveInfo = YGODOWN;}

    //hit border between two blocks, not hitting blocks on either side
    else if(west!=east && blocks[north][east]!=NULL_BLOCK
          &&blocks[south][east]==NULL_BLOCK&&blocks[south][west]==NULL_BLOCK)
          {toReturn.yMoveInfo = YGODOWN;}
            
    //ball actually wedged in a corner, should move opposite direction
    //  than normally expected
    else if(west!=east && blocks[south][east]!=NULL_BLOCK)
    {
             if(info.y >= (cellY+1)*BLOCKHEIGHT - BALLDELTA )
             {toReturn.yMoveInfo = YGODOWN; toReturn.xMoveInfo = XBACKLEFT;}
             else
             {toReturn.yMoveInfo = YGOUP; toReturn.xMoveInfo = XBACKRIGHT;}
    }
      
   }//end if NW impact




   if(north==cellY && east==cellX)//impact by top-right corner of ball
   {
    //checking for all possible situations
      
    if(north==south)//squarely hit side of block
    {toReturn.xMoveInfo = XBACKLEFT;}
      
    //hit border between blocks on the side, not hitting blocks above and
    //  below
    else if(north!=south && blocks[south][east]!=NULL_BLOCK
          &&blocks[south][west]==NULL_BLOCK&&blocks[north][west]==NULL_BLOCK)
          {toReturn.xMoveInfo = XBACKLEFT;}
      
    //hit corner of block, not touching any others blocks around
    else if(north!=south && blocks[south][west]==NULL_BLOCK
          &&blocks[south][east]==NULL_BLOCK&&blocks[north][west]==NULL_BLOCK)
    {      
             //if close enough to corner...
             if( info.y > cellY*BLOCKHEIGHT + 2*BLOCKHEIGHT/3 )
             {toReturn.yMoveInfo = YGODOWN;}
             if( info.x < cellX*BLOCKWIDTH + 1*BLOCKWIDTH/3 )
             {toReturn.xMoveInfo = XBACKLEFT;}
    }
      
    else if(west==east)//squarely hit bottom of block
    {toReturn.yMoveInfo = YGODOWN;}

    //hit border between two blocks, not hitting blocks on either side
    else if(west!=east && blocks[north][west]!=NULL_BLOCK
          &&blocks[south][east]==NULL_BLOCK&&blocks[south][west]==NULL_BLOCK)
          {toReturn.yMoveInfo = YGODOWN;}
      
    //ball actually wedged in a corner, should move opposite direction
    //  than normally expected
    else if(west!=east && blocks[south][west]!=NULL_BLOCK)
    {
             if(info.y >= (cellY+1)*BLOCKHEIGHT - BALLDELTA )
             {toReturn.yMoveInfo = YGODOWN; toReturn.xMoveInfo = XBACKRIGHT;}
             else
             {toReturn.yMoveInfo = YGOUP; toReturn.xMoveInfo = XBACKLEFT;}
    }
      
   }//end if NE impact




   if(south==cellY && east==cellX)//impact by bottom-right corner of ball
   {
    //checking for all possible situations
      
    if(north==south)//squarely hit side of block
    {toReturn.xMoveInfo = XBACKLEFT;}
      
    //hit border between blocks on the side, not hitting blocks above and
    //  below
    else if(north!=south && blocks[north][east]!=NULL_BLOCK
          &&blocks[south][west]==NULL_BLOCK&&blocks[north][west]==NULL_BLOCK)
          {toReturn.xMoveInfo = XBACKLEFT;}
      
    //hit corner of block, not touching any others blocks around
    else if(north!=south && blocks[south][west]==NULL_BLOCK
          &&blocks[north][west]==NULL_BLOCK&&blocks[north][east]==NULL_BLOCK)
    {      
             //if close enough to corner...
             if( info.y < cellY*BLOCKHEIGHT + 1*BLOCKHEIGHT/3 )
             {toReturn.yMoveInfo = YGOUP;}
             if( info.x < cellX*BLOCKWIDTH + 1*BLOCKWIDTH/3 )
             {toReturn.xMoveInfo = XBACKLEFT;}
    }
      
    else if(west==east)//squarely hit top of block
    {toReturn.yMoveInfo = YGOUP;}

    //hit border between two blocks, not hitting blocks on either side
    else if(west!=east && blocks[south][west]!=NULL_BLOCK
          &&blocks[north][east]==NULL_BLOCK&&blocks[north][west]==NULL_BLOCK)
          {toReturn.yMoveInfo = YGOUP;}
      
    //ball actually wedged in a corner, should move opposite direction
    //  than normally expected
    else if(west!=east && blocks[north][west]!=NULL_BLOCK)
    {
             if(info.y >= (cellY+1)*BLOCKHEIGHT - BALLDELTA )
             {toReturn.yMoveInfo = YGODOWN; toReturn.xMoveInfo = XBACKLEFT;}
             else
             {toReturn.yMoveInfo = YGOUP; toReturn.xMoveInfo = XBACKRIGHT;}
    }
      
   }//end if SE impact




   if(south==cellY && west==cellX)//impact by bottom-left corner of ball
   {
    //checking for all possible situations
    
    if(north==south)//squarely hit side of block
    {toReturn.xMoveInfo = XBACKRIGHT;}
      
    //hit border between blocks on the side, not hitting blocks above and
    //  below
    else if(north!=south && blocks[north][west]!=NULL_BLOCK
          &&blocks[south][east]==NULL_BLOCK&&blocks[north][east]==NULL_BLOCK)
          {toReturn.xMoveInfo = XBACKRIGHT;}
      
    //hit corner of block, not touching any others blocks around
    else if(north!=south && blocks[north][east]==NULL_BLOCK
          &&blocks[south][east]==NULL_BLOCK&&blocks[north][west]==NULL_BLOCK)
    {      
             //if close enough to corner...
             if( info.y < cellY*BLOCKHEIGHT + 1*BLOCKHEIGHT/3 )
             {toReturn.yMoveInfo = YGOUP;}
             if( info.x > cellX*BLOCKWIDTH + 2*BLOCKWIDTH/3 )
             {toReturn.xMoveInfo = XBACKRIGHT;}
    }
      
    else if(west==east)//squarely hit top of block
    {toReturn.yMoveInfo = YGOUP;}

    //hit border between two blocks, not hitting blocks on either side
    else if(west!=east && blocks[south][east]!=NULL_BLOCK
          &&blocks[north][east]==NULL_BLOCK&&blocks[north][west]==NULL_BLOCK)
          {toReturn.yMoveInfo = YGOUP;}
      
    //ball actually wedged in a corner, should move opposite direction
    //  than normally expected
    else if(west!=east && blocks[north][east]!=NULL_BLOCK)
    {
             if(info.y <= cellY*BLOCKHEIGHT + BALLDELTA )
             {toReturn.yMoveInfo = YGOUP; toReturn.xMoveInfo = XBACKLEFT;}
             else
             {toReturn.yMoveInfo = YGODOWN; toReturn.xMoveInfo = XBACKRIGHT;}
    }
      
   }//end if SW impact




   //IF A COLLISION, REACT DEPENDING ON WHAT KIND OF BLOCK WAS HIT//////////    
   if(collisionOccured)
   {


        switch(blocks[cellY][cellX])//switch on type of Block hit
        {
          case DIAMOND:
               if(colorBlocks == 0)//if no color blocks left
               {
                blocks[cellY][cellX] = NULL_BLOCK;//destroy it
                diamondBlocks--;
                score+=100;
                #ifdef WIN32
                if(sound)
                diamond_snd->Play();
                #endif
               }
               else
               {
                #ifdef WIN32
                if(sound)
                bounce2_snd->Play();
                #endif
               }
               if(diamondBlocks == 0)//if no more diamonds now
                {gameState = LEVELWON;}
               break;
          
          case SKULL:
               gameState = BALLDIED;
               break;

          case BRUSH_BLUE://tell ball to change its color
          case BRUSH_RED:
          case BRUSH_GREEN:
          case BRUSH_BROWN:
          case BRUSH_PURPLE:
          case BRUSH_ORANGE:
                            //convert from Block to Color enum
               if(info.c != (Color)blocks[cellY][cellX]-BRUSH_BLUE+2)
               {
                toReturn.change = (Color)(blocks[cellY][cellX]-BRUSH_BLUE+2);
                #ifdef WIN32
                if(sound)
                colorbrush_snd->Play();
                #endif
               }
               else
               {
                #ifdef WIN32
                if(sound)
                bounce2_snd->Play();
                #endif
               }
               break;

          case COLOR_LTBLUE:
          case COLOR_BLUE:
          case COLOR_RED:
          case COLOR_GREEN:
          case COLOR_BROWN:
          case COLOR_PURPLE:
               //if colors match    (convert from BLock to Color enum)
               if(info.c == (Color)blocks[cellY][cellX]-COLOR_LTBLUE+1)
               {
                blocks[cellY][cellX] = NULL_BLOCK;//destroy it
                colorBlocks--;
                score+=3;
                #ifdef WIN32
                if(sound)
                colorblk_snd->Play();
                #endif
               }
               else
               {
                #ifdef WIN32
                if(sound)
                bounce2_snd->Play();
                #endif
               }
               break;

          case SOLID://hit immovable block
                #ifdef WIN32
                if(sound)
                bounce2_snd->Play();
                #endif
               break;

          case KEY://hit a key block
               if(!hasKey && info.c == orange)
               {
                blocks[cellY][cellX] = NULL_BLOCK;//destroy key block
                hasKey = true;
                score+=25;
                #ifdef WIN32
                if(sound)
                key_snd->Play();
                #endif
               }
               else
               {
                #ifdef WIN32
                if(sound)
                bounce2_snd->Play();
                #endif
               }
               break;

          case LOCK://hit a lock
               if(hasKey && info.c == orange)
               {
                blocks[cellY][cellX] = NULL_BLOCK;//destroy lock
                hasKey = false;
                score+=50;
                #ifdef WIN32
                if(sound)
                lock_snd->Play();
                #endif
               }
               else
               {
                #ifdef WIN32
                if(sound)
                bounce2_snd->Play();
                #endif
               }
               break;

          case REVERSE://hit a 'reverse' block
               if(!horizReversed)
               {horizReversed = true;}
               else
               {horizReversed = false;}
               blocks[cellY][cellX] = NULL_BLOCK;//destroy it
               #ifdef WIN32
               if(sound)
                reverse_snd->Play();
               #endif
               break;
                
          default: break;
        }//end switch
   }//end if collision happened
      
      return toReturn;//info back to ball object that called this function
 }//end checkCollision()

/*setLEVEL******************************************************************/
void Field::setLevel()
{ 
 /*
  * This method is to set up each level as it begins. Why represent each
  * level using char's in a string? Using a two-dimensional array of the
  * constants representing each type of block was tedious and difficult
  * to edit. I know string concatination is inefficient, however this
  * only takes place when each level begins, and under the WM_TIMER
  * message in WindowProc() I include an arbitrary delay between levels
  * so the player can tell what's going on anyway. Also, the function
  * fillInBlocks(), cycling through each char in the string to initalize
  * the blocks[] array, is able to count how many color and diamond blocks
  * there are.
  */

	 string setup = "";     

	 diamondBlocks = 0;
	 colorBlocks = 0;

/*
 * l(ight blue), b(lue), r(ed), g(reen), (bro)w(n), p(urple) =  color blocks
 * B(lue), R(ed), G(reen), (bro)W(n), P(urple), O(range) =  color brushes
 * d(iamond), s(kull), (k)ey, L(ock), S(olid), (re)v(erse), n(ull)
 */
  

	 //fill 'er up 
	switch(currentLevel)
	{
	case 1:
	{
		setup  = "snnnnwwnnnns";
		setup += "SnnnSggSnnnS";
		setup += "nnndggggdnnn";
		setup += "nWdbbbbbbdBn";
		setup += "nSllnnnnllSn";
		setup += "nnnnnddnnnnn";
		setup += "nnnnnddnnnnn";
		setup += "nSllnnnnllSn";
		setup += "nGdggggggdPn";
		setup += "nnndbbbbdnnn";
		setup += "nnnnSbbSnnnn";
		setup += "nnnnnppnnnnn";
	fillInBlocks(setup);
	break;
	}// end case 1

	case 2:
	{
		setup  = "nnnnnnnnnnnn";
		setup += "nnnnllllnnnn";
		setup += "nnnlrrrrlnnn";
		setup += "nnlRbbbbBlnn";
		setup += "nsrbddddbrsn";
		setup += "nsrbddddbrsn";
		setup += "nsrbddddbrsn";
		setup += "nsrbddddbrsn";
		setup += "nnlBbbbbRlnn";
		setup += "nnnlrrrrlnnn";
		setup += "nnnnllllnnnn";
		setup += "nnnnnnnnnnnn";
	fillInBlocks(setup);
	break;
	}// end case 2

	case 3:
	{
		setup  = "ngnnnnnnnGdn";
		setup += "gsgnnnnbnnSl";
		setup += "ngnnnnbsbnSn";
		setup += "nnnnlnnbnndl";
		setup += "nnnlBlnnnnSn";
		setup += "nnnnlnnnnnSl";
		setup += "nnnnnnnnnndn";
		setup += "dSdSSdSdSlSl";
		setup += "lnlnlnlnlnSn";
		setup += "nSSdSSdSSdSn";
		setup += "lnlnlnlnlnln";
		setup += "SdSSdSSdSSSn";
	fillInBlocks(setup);
	break;
	}// end case 3

	case 4:
	{
		setup  = "nnnnnnnnnnnn";
		setup += "nSSSSllSSSSn";
		setup += "nSBnnnnnnRSn";
		setup += "nSnnnnnnnnSn";
		setup += "nSnnsllsnnSn";
		setup += "nSnnnnnnnnSn";
		setup += "nSnnnnnnnnSn";
		setup += "nSnnSLLSnnSn";
		setup += "nSbbSddSrrSn";
		setup += "nSbbsddsrrSn";
		setup += "nSSSSSSSSSSn";
		setup += "nnnnvOkvnnnn";
	fillInBlocks(setup);
	break;
	}// end case 4

	case 5:
	{
		setup  = "dddnnnnnnddd";
		setup += "ddsnnnnnnsdd";
		setup += "dpnnnSSnnnld";
		setup += "ppnnSggSnnll";
		setup += "pnnnSggSnnnl";
		setup += "pnnPSggSknnl";
		setup += "pnnSSSLSSnnl";
		setup += "nnnSsnnOSGnn";
		setup += "nnSnnnnnnSnn";
		setup += "nSnnnnnnnsSn";
		setup += "nSnnnnnnnnSn";
		setup += "npnnnnnnnnSn";
	fillInBlocks(setup);
	break;
	}// end case 5

	case 6:
	{
		setup  = "SSSSSSnnlgwk";
		setup += "nnnnSsnGlgww";
		setup += "nnnnSsnnlOgg";
		setup += "ddddSSnnllll";
		setup += "nssnLnnnnWnn";
		setup += "nssnSnnnnnnn";
		setup += "nssnSnnnnnnn";
		setup += "nssnLnnnnnnn";
		setup += "ddddSSnnSnSn";
		setup += "nnnnSsnnnnnn";
		setup += "nnnnSsnnnnnn";
		setup += "SSSSSSnnnnnn";
	fillInBlocks(setup);
	break;
	}// end case 6

	case 7:
	{
		setup  = "slnnnnnnnnls";
		setup += "lnnnnllnnnnn";
		setup += "nnnngddbnnnn";
		setup += "nnngbnngbnnn";
		setup += "nnBnddddnGnn";
		setup += "nddndssdnddn";
		setup += "nddndssdnddn";
		setup += "nnGnddddnBnn";
		setup += "nnnbgnnbgnnn";
		setup += "nnnnbddgnnnn";
		setup += "lnnnnllnnnnl";
		setup += "slnnnnnnnnln";
	fillInBlocks(setup);
	break;
	}// end case 7

	case 8:
	{
		setup  = "llnnngnnnlln";
		setup += "lnnngggnnnln";
		setup += "nnnngvgnnnnn";
		setup += "nBbbgggbbBnn";
		setup += "nbddbgbddbnn";
		setup += "nsddbsbddsnn";
		setup += "nbddbwbddbnn";
		setup += "nGbbwwwbbWnn";
		setup += "nnnnwvwnnnnn";
		setup += "nnnnwwwnnnnn";
		setup += "lnnnnwnnnnln";
		setup += "llnnnnnnnlln";
	fillInBlocks(setup);
	break;
	}// end case 8

	case 9:
	{
		setup  = "lpnnnsnnnnnd";
		setup += "vnnnndnnnnnn";
		setup += "lnnnnnnnnSnn";
		setup += "Snnnndnnnlld";
		setup += "lnnnnnnnslll";
		setup += "vpnnnnnnnlll";
		setup += "lpnnndnnnSll";
		setup += "lnnPnnnnnnll";
		setup += "lnnnndnndnkl";
		setup += "SnnnnnnnLndn";
		setup += "lnnnndnnGOdn";
		setup += "dpRnnsnndddn";
	fillInBlocks(setup);
	break;
	}// end case 9
	
	case 10:
	{
		setup  = "dnnbnnvnsnvn";
		setup += "Sdnnlnnnsnnn";
		setup += "dnnbnnnnsnnn";
		setup += "Sdnnlnsnsnsn";
		setup += "dnnbnnsnsnsn";
		setup += "Sdnnlnsnsnsn";
		setup += "dnnbnnsnsnsn";
		setup += "Sdnnlnsnsnsn";
		setup += "dnnbnnsnsnsn";
		setup += "Sdnnlnsnnnsn";
		setup += "dnnbnnsnnnsn";
		setup += "SdnnlnBnvnsn";
	fillInBlocks(setup);
	break;
	}// end case 10

	case 11:
	{
		setup  = "dnnssnsssnLL";
		setup += "dnnnknkknnLL";
		setup += "dnnnknkknnLL";
		setup += "dnnnknkknnLL";
		setup += "dnnnknkknnLL";
		setup += "dnnnknkknnLL";
		setup += "dnnnknkknnLL";
		setup += "dnnnknkknnLL";
		setup += "dnnnknkknnLL";
		setup += "dnnnknkknnLL";
		setup += "dnnnknkknnnO";
		setup += "dnnssnsssnnn";
	fillInBlocks(setup);
	break;
	}// end case 11

	case 12:
	{
		setup  = "svBSdddSRvsO";
		setup += "nnnSdddSnnnn";
		setup += "nnnSSLSSnnnn";
		setup += "nnnsSLSsnnnn";
		setup += "nnnnSLSnnnnn";
		setup += "nSSnSLSnSSnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnllllllllln";
		setup += "nnbbbrrrbbbn";
		setup += "knbkbrkrbkbn";
	fillInBlocks(setup);
	break;
	}// end case 12

	case 13:
	{
		setup  = "lnnlSlnlSlnn";
		setup += "nSSnSnSnSnSn";
		setup += "nsSlnlSnSnSn";
		setup += "nGgSlSSnSnSn";
		setup += "npgsSSSnSnSl";
		setup += "npgWwSSnSnSn";
		setup += "npgbwsSlnlSn";
		setup += "npgbwOSSlSln";
		setup += "npgbwnSdSkln";
		setup += "npgbwnSddSln";
		setup += "npBbwnSdddSn";
		setup += "PpsbsnLdddSn";
	fillInBlocks(setup);
	break;
	}// end case 13

	case 14:
	{
		setup  = "ddOrbwvsvsvs";
		setup += "ddSrbwnsnsns";
		setup += "ddSrbwnsnsns";
		setup += "LSkrbwnSnSnS";
		setup += "rrrrbwnnnnnn";
		setup += "bbbbbwwwwwwW";
		setup += "wwwwwwwggggg";
		setup += "nnnnnnwgpppp";
		setup += "SnSnSnwgplll";
		setup += "snsnsnwgplnn";
		setup += "snsnsnwgplnn";
		setup += "sRsBsnwGpPnn";
	fillInBlocks(setup);
	break;
	}// end case 14

	case 15:
	{
		setup  = "Rnnnbnnldnnl";
		setup += "snrldnwndndn";
		setup += "Snrnbnsngnpn";
		setup += "Bnsnbnwngnpn";
		setup += "Snrnbnwngnpn";
		setup += "Wlrnbnwngnpl";
		setup += "Sndndnwngnsn";
		setup += "Gnrnbnwnsnpn";
		setup += "Snrlsndlgndl";
		setup += "sndnblwndnpn";
		setup += "Snrnbnwngnpn";
		setup += "Pnrnnnwnnnpn";
	fillInBlocks(setup);
	break;
	}// end case 15

	case 16:
	{
		setup  = "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnslllllsnnn";
		setup += "nnWwwwwwwnnn";
		setup += "nlgvdvdvglnn";
		setup += "nlgdsdsdglnn";
		setup += "nlgvdSdvglnn";
		setup += "nlgdvdvdglnn";
		setup += "nnwwwwwwGnnn";
		setup += "nnslllllsnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
	fillInBlocks(setup);
	break;
	}// end case 16

	case 17:
	{
		setup  = "dwwSppSbbSgg";
		setup += "dwwLppLbbLgg";
		setup += "dwdSpdSbdSgd";
		setup += "wwSppSbbSggS";
		setup += "wwSppSbbSggS";
		setup += "wdkpdkbdkgdS";
		setup += "wdppSbbSggSd";
		setup += "wSppSbbSggSd";
		setup += "dkWdSOdSLdPd";
		setup += "Gnnnnnnnnnnn";
		setup += "snnnnnnnnnnn";
		setup += "Bnnnnnnnnnnn";
	fillInBlocks(setup);
	break;
	}// end case 17

	case 18:
	{
		setup  = "nvnnnnnnnvBn";
		setup += "npnnnnnnnnln";
		setup += "npnvlllvnnln";
		setup += "npndSdSdnnln";
		setup += "npnSSOSSnnbn";
		setup += "npnSsdsSnsbn";
		setup += "npnSSLSSnsbn";
		setup += "npndSdSdnnbn";
		setup += "npnvlklvnnln";
		setup += "npnnnnnSnnln";
		setup += "nvnnnnnSnvln";
		setup += "PpnnnnnSnnln";
	fillInBlocks(setup);
	break;
	}// end case 18

	case 19:
	{
		setup  = "WsnnnlnnnnsB";
		setup += "nnnnldlnnnnn";
		setup += "nnnlrdglnnnn";
		setup += "nnlrrdgglnnn";
		setup += "nlrrrdggglnn";
		setup += "ldddddddddln";
		setup += "nlbbbdwwwlnn";
		setup += "nnlbbdwwlnnn";
		setup += "nnnlbdwlnnnn";
		setup += "nnnnldlnnnnn";
		setup += "nnnnnlnnnnnn";
		setup += "GsnnnnnnnsRn";
	fillInBlocks(setup);
	break;
	}// end case 19

	case 20:
	{
		setup  = "knnnnlnnnnnB";
		setup += "nnnnlblnnnnn";
		setup += "nnnlbsblnnnn";
		setup += "nnlbnbnblnnn";
		setup += "nlbndLdnblnn";
		setup += "lbsbsdsbsbln";
		setup += "nlbndLdnblnn";
		setup += "nnlbnbnblnnn";
		setup += "nnnlbsblnnnn";
		setup += "nnnnlblnnnnn";
		setup += "nnnnnlnnnnnn";
		setup += "Onnnnnnnnnnn";
	fillInBlocks(setup);
	break;
	}// end case 20
	
	case 21:
	{
		setup  = "Olnnnnnnnnlk";
		setup += "llnnnnnnnnll";
		setup += "nnrrrrrrrrnn";
		setup += "nnrsppppsrnn";
		setup += "nnrpPssPprnn";
		setup += "nnrpgddgprnn";
		setup += "nnrpgddgprnn";
		setup += "nnrpggggprnn";
		setup += "nnrspLppsrnn";
		setup += "nnrrGnnRrrnn";
		setup += "llnnnnnnnnnn";
		setup += "llnnnnnnnnnn";
	fillInBlocks(setup);
	break;
	}// end case 21

	case 22:
	{
		setup  = "RnsnGnnnllll";
		setup += "nnnnnnnSlddl";
		setup += "snknsnnSllll";
		setup += "nnnnnnnSSSSS";
		setup += "OLsLBnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnWlslGn";
		setup += "SSLSsnnlllnn";
		setup += "SdddSnslklsn";
		setup += "SdddSnnlllnn";
		setup += "sdddsnPlslRn";
	fillInBlocks(setup);
	break;
	}// end case 22
	
	case 23:
	{
		setup  = "nnnnnsnnnnnn";
		setup += "nnnnnsnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "sssnnnnnsssn";
		setup += "sssnnnnnsssn";
		setup += "ssnnnnnnnssn";
		setup += "snnnsnsnnnsn";
		setup += "nnnssnssnnnn";
		setup += "nnsssnrssnnn";
		setup += "nnnssnrsnnnn";
		setup += "nnnssndsnnnn";
		setup += "Rnnsslssnnnn";
	fillInBlocks(setup);
	break;
	}// end case 23

	case 24:
	{
		setup  = "snSddddddSvs";
		setup += "svSSSSSSvSns";
		setup += "dnSSSSSSvSvd";
		setup += "dvvddddddSnd";
		setup += "SnSvSvsvSSvS";
		setup += "svSddddddSns";
		setup += "snSddddddSvs";
		setup += "SvSSvsvSvSnS";
		setup += "dnSllllGlSvd";
		setup += "dvSllllllSnd";
		setup += "snvllllllvvv";
		setup += "svgnnnnnllvn";
	fillInBlocks(setup);
	break;
	}// end case 24

	case 25:
	{
		setup  = "sdgnnPnnnbds";
		setup += "ldgnnnpnnbdl";
		setup += "ldgnnnpnnbdl";
		setup += "ldgnnnpnnbdl";
		setup += "ldsBnssnGsdl";
		setup += "nndnnwnnndnn";
		setup += "nnlnnwnnnlnn";
		setup += "nndnnwnnndnn";
		setup += "nnlnnnWnnlnn";
		setup += "nndnnnnnndnn";
		setup += "nndddnndddnn";
		setup += "nnnnllllnnnn";
	fillInBlocks(setup);
	break;
	}// end case 25

	case 26:
	{
		setup  = "vsvsvsvsvsvs";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nlllllllllln";
		setup += "nddddddddddn";
		setup += "nppppPPppppn";
		setup += "nlllllllllln";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "svsvsvsvsvsn";
	fillInBlocks(setup);
	break;
	}// end case 26

	case 27:
	{
		setup  = "nnnnlllnnnnn";
		setup += "nnnnnrnnnnnn";
		setup += "nsnnnrnnsnnn";
		setup += "nggsnrnnddsn";
		setup += "nggnnBnnddnn";
		setup += "sggnnrnsddnn";
		setup += "nnRnnrnnnsnn";
		setup += "nnnnnsnnnnns";
		setup += "nnnnnbbsnnnG";
		setup += "snnnnbbnnnnn";
		setup += "llnnsbbnnnnn";
		setup += "llsnnnsnnnsn";
	fillInBlocks(setup);
	break;
	}// end case 27

	case 28:
	{
		setup  = "npnpnnsnlPSs";
		setup += "nnnnnnSnllSn";
		setup += "pnpnpnOnSSSn";
		setup += "LSSSSnSnnnnn";
		setup += "nrddSnSSSSnl";
		setup += "nrddSnSnnnSn";
		setup += "nSSSknSnvnsl";
		setup += "nsnnnnsnSnSn";
		setup += "nsnSSSSpSnSl";
		setup += "nsnGnnRpSnpn";
		setup += "gsnnnnnLSnpn";
		setup += "gsnnnnnnSkSn";
	fillInBlocks(setup);
	break;
	}// end case 28

	case 29:
	{
		setup  = "dddSSSnsnnnn";
		setup += "dddsggnSGSSs";
		setup += "SSLSggnLllnn";
		setup += "nnnSkgnSllnn";
		setup += "nnnSSSnSSSSn";
		setup += "nnnnnnndddsn";
		setup += "nnnnnnndddsn";
		setup += "SSSnnnnSWSSn";
		setup += "kwwnnnnSklln";
		setup += "wwgnnnnSllln";
		setup += "wwwnnSnSSSdn";
		setup += "SsSnnSnOnnnn";
	fillInBlocks(setup);
	break;
	}// end case 29

	case 30:
	{
		setup  = "lllllnSsOnln";
		setup += "nnnnSnSdSnSn";
		setup += "nnnnSnSdSnSn";
		setup += "nnnlSnSkSnSn";
		setup += "nnnlSnsLSnSn";
		setup += "nssnSnnnnnSn";
		setup += "nssnSnnnnnSn";
		setup += "nRnlBbSLsnSn";
		setup += "nnnlSbSdSnSn";
		setup += "nnnnSbSdSnSn";
		setup += "knnnSrSdSnSn";
		setup += "klllrrSsSnvn";
	fillInBlocks(setup);
	break;
	}// end case 30

	case 31:
	{
		setup  = "rnrnnslRlnnG";
		setup += "nrnnnnnlnnnn";
		setup += "nnnnnnnnnnnS";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnSnnnSnn";
		setup += "nnnnnSnnnSnn";
		setup += "nnnnnSnnnSnn";
		setup += "nnnnnSnnnSnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnndnnnnndn";
		setup += "ngnnndndndnn";
		setup += "gngnnndndnnn";
	fillInBlocks(setup);
	break;
	}// end case 31

	case 32:
	{
		setup  = "snnnnnlnnnRs";
		setup += "nnnnnlrlnnnn";
		setup += "nnnnlrlrlnnn";
		setup += "nnnlrlnlrlnn";
		setup += "nnlrlnnnlrln";
		setup += "nlrlnnnlrlnn";
		setup += "lrlnnnlrlnnn";
		setup += "nlrlnnnlrnnn";
		setup += "nnlrlnlrnnln";
		setup += "nlrlnlrlnlrl";
		setup += "lrlnnnlrlrln";
		setup += "dlnnnnnlrlnn";
	fillInBlocks(setup);
	break;
	}// end case 32

	case 33:
	{
		setup  = "snbnnsnnnnbn";
		setup += "nblnnnnnnbln";
		setup += "nnbnnnnbnnbn";
		setup += "nnnnnnblnnnn";
		setup += "nnnnbnnbnnnd";
		setup += "nnnblnnnnndn";
		setup += "nnnnbnnnbnnd";
		setup += "nnbnnnnblnnn";
		setup += "nblnnnnnbnnn";
		setup += "nnbnnnbnnnbn";
		setup += "nnnnnblnnbln";
		setup += "Bnnnnnbnnnbn";
	fillInBlocks(setup);
	break;
	}// end case 33

	case 34:
	{
		setup  = "nnnnnnnnnnnn";
		setup += "nbbbbbllnnnn";
		setup += "nnnnrbdrnnnn";
		setup += "nnnrbdrnnnnn";
		setup += "nnrbdrnnnnnn";
		setup += "nrbdsnnnnnnn";
		setup += "nbddddrnnnnn";
		setup += "nbbbbblrnnnn";
		setup += "nnnnrbllrnnn";
		setup += "Rnnnnrbbllln";
		setup += "Snnnnnrbbbbn";
		setup += "Bnnnnnnnnnnn";
	fillInBlocks(setup);
	break;
	}// end case 34

	case 35:
	{
		setup  = "nnnSsSnnnPnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nlllplllwnnn";
		setup += "nlllpplwwnnn";
		setup += "nllpdpwwlnnn";
		setup += "nlpppppllnnn";
		setup += "ndpppppllnnn";
		setup += "nppdpdpplnnn";
		setup += "Sppppppplnnn";
		setup += "Wdplpllllnnn";
		setup += "pplllllllnnn";
	fillInBlocks(setup);
	break;
	}// end case 35

	case 36:
	{
		setup  = "rrnsrrPpGgnn";
		setup += "rrrrrrnpngnn";
		setup += "nrrrrnnpngnn";
		setup += "nnrrnnnlngnn";
		setup += "nSrrSnnpngnn";
		setup += "RSddsnnpnlnn";
		setup += "nSddSnnpngnn";
		setup += "nnrrnnnpngnn";
		setup += "nnllnnnpngnn";
		setup += "nrrrrnnpngnn";
		setup += "rrrrrrnpngnn";
		setup += "rrsnrrnpngnn";
	fillInBlocks(setup);
	break;
	}// end case 36

	case 37:
	{
		setup  = "nlbbblbbbbvB";
		setup += "nbblnnnlbbnS";
		setup += "nbbnblbnblns";
		setup += "nlnnbnblbbnn";
		setup += "nbblbdbbblnn";
		setup += "nlbbblbnbbnn";
		setup += "nbblnnnlbbnn";
		setup += "nbbbblbnnlnn";
		setup += "nlbbbbblbbnn";
		setup += "nbblbbbbblnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
	fillInBlocks(setup);
	break;
	}// end case 37

	case 38:
	{
		setup  = "dndndndndndn";
		setup += "nnnnnnnnnnnn";
		setup += "nnRnnnrnnnbn";
		setup += "nlnlnrnrnbnb";
		setup += "nnnlnnnrnnnb";
		setup += "snlnsnrnsnbn";
		setup += "nnnlnnnrnnnb";
		setup += "nlnlnrnrnbnb";
		setup += "nnBnnnrnnnbn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "ndndndndndnn";
	fillInBlocks(setup);
	break;
	}// end case 38

	case 39:
	{
		setup  = "nnnnnnnnWnnB";
		setup += "nrnrnnnnnnnn";
		setup += "nnsnnwnwnnnn";
		setup += "nrdrnnsnnbnb";
		setup += "nndnnwdwnnsn";
		setup += "Rnnnnndnnbdb";
		setup += "nnnnnnnnnldn";
		setup += "nnnnnnpnplln";
		setup += "ngngnnnsnlnn";
		setup += "nnsnnnpdplnn";
		setup += "ngdgnnndllnn";
		setup += "GndnnPnnnnnn";
	fillInBlocks(setup);
	break;
	}// end case 39

	case 40:
	{
		setup  = "skkkkkkkkkkO";
		setup += "knbnnnnnnnnb";
		setup += "knbSSSSSSSvb";
		setup += "knbBddddddSb";
		setup += "snbSddddddSb";
		setup += "knnSddddddBl";
		setup += "knnSLSSSSSSl";
		setup += "knnSLLLLLSSl";
		setup += "snnSSSSSLSSl";
		setup += "knnSLLLLLSSl";
		setup += "knnSLSSSSSSv";
		setup += "kkkSLLLLLLnn";
	fillInBlocks(setup);
	break;
	}// end case 40

	case 41:
	{
		setup  = "nnnnnnnnnnnn";
		setup += "nnnnwwwwnnnn";
		setup += "nnwwllllwwnn";
		setup += "nwlllbblllwn";
		setup += "wlllbbbbllls";
		setup += "wlllbddbllls";
		setup += "Bwllbddbllnn";
		setup += "nnwlbbbbwwnn";
		setup += "nnnwwWbwnnnn";
		setup += "nnnnnwwnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnsnnnnnnnnn";
	fillInBlocks(setup);
	break;
	}// end case 41

	case 42:
	{
		setup  = "Pnnnnnnnnnnn";
		setup += "nnnnnrnnnlln";
		setup += "nnnndddnnlln";
		setup += "nnnndddnnlln";
		setup += "nnndddddnlln";
		setup += "nnppppppplln";
		setup += "lnnpppppnlln";
		setup += "lnnnpppnnlln";
		setup += "snnnpppnnlln";
		setup += "nnnnpppnnlln";
		setup += "nnnnpppnnlln";
		setup += "Rnnpppppnnnn";
	fillInBlocks(setup);
	break;
	}// end case 42

	case 43:
	{
		setup  = "WnnnnnnnnnnB";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnRnnnnnn";
		setup += "nnnnnrnnnnnn";
		setup += "nnnnwrwnnnnn";
		setup += "nnnnvrvnnnnn";
		setup += "nnnvlblvnnnn";
		setup += "nnnlbbblnnnn";
		setup += "nnnlSdSlnnnn";
		setup += "nnnlpsglnnnn";
		setup += "nnnlpdglnnnn";
		setup += "nnGlpsglPnnn";
	fillInBlocks(setup);
	break;
	}// end case 43

	case 44:
	{
		setup  = "SppSwwSdddsn";
		setup += "gSppSwwSdddn";
		setup += "ggSppSwwSddn";
		setup += "SggSppSwwSdn";
		setup += "bSggSppSwwSn";
		setup += "bbSggSppSwwn";
		setup += "SbbSggSppSWn";
		setup += "rSbbSggSppSn";
		setup += "rrSbbSggSppn";
		setup += "SrrSbbSggSPn";
		setup += "dSrRSbBSgGSn";
		setup += "nnnnnnnnnnnn";
	fillInBlocks(setup);
	break;
	}// end case 44

	case 45:
	{
		setup  = "snnsnnnnnvnn";
		setup += "nnnnnnnppSln";
		setup += "nnnnnnpnRSln";
		setup += "nnnSppnnnSln";
		setup += "nnnrBnnnnSln";
		setup += "nnnrnndnnSln";
		setup += "nnnrndddnSln";
		setup += "nnnrnndnnSln";
		setup += "nnnrnnnnPSln";
		setup += "GnnrnnnbbSnO";
		setup += "nnnLnnbnnnnn";
		setup += "kvnSbbnnnnnn";
	fillInBlocks(setup);
	break;
	}// end case 45

	case 46:
	{
		setup  = "OvvsvvrPklbd";
		setup += "vvvvvrplbllb";
		setup += "svvvrpbllbll";
		setup += "vvvrpllbllbl";
		setup += "vvrplbllblll";
		setup += "vrpbllbllbll";
		setup += "RpBlbllbllbl";
		setup += "plbllbllblll";
		setup += "bllbllbllbln";
		setup += "Lbllbllbllbn";
		setup += "dSbllbllbnnn";
		setup += "ddSblllnnnnn";
	fillInBlocks(setup);
	break;
	}// end case 46

	case 47:
	{
		setup  = "dddddddddSBk";
		setup += "dddddddddSnn";
		setup += "dddddddddSnn";
		setup += "dddddddddsnR";
		setup += "dddddddddSnn";
		setup += "dddddddddSnn";
		setup += "dddddddddSPn";
		setup += "dddddddddSnn";
		setup += "dddddddddsnn";
		setup += "dddddddddSnG";
		setup += "dddddddddLnn";
		setup += "SSSSSSSSSSOn";
	fillInBlocks(setup);
	break;
	}// end case 47

	case 48:
	{
		setup  = "PnRnWnBnGnOn";
		setup += "nnnnnnnnnnnn";
		setup += "nOnGnBnWnPnR";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnrnwnpnbngn";
		setup += "nsdgdgdgdrnn";
		setup += "nsdldldldrnn";
		setup += "nnrnwnpnbngs";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "lnnlnnlnnlnn";
	fillInBlocks(setup);
	break;
	}// end case 48

	case 49:
	{
		setup  = "lrrrrrrrrrln";
		setup += "lrgggggggrRn";
		setup += "lGgpppppgGln";
		setup += "lrPdpppdPrln";
		setup += "lrgpspspgrln";
		setup += "lrgppsppgrln";
		setup += "lrgpspspgrln";
		setup += "lrgdpppdgrln";
		setup += "lrgpppppgrln";
		setup += "lrgggggggrln";
		setup += "lrrrrrrrrrln";
		setup += "llllllllllln";
	fillInBlocks(setup);
	break;
	}// end case 49

	case 50:
	{
		setup  = "PnwSnGSdvdSk";
		setup += "SSwSnnSdSdSg";
		setup += "nnnsnsSdSdSg";
		setup += "wSSbnpSdSdSg";
		setup += "wnnbnpSdSdSg";
		setup += "SSnbSpSdSdSg";
		setup += "WSnbSpSLSSSg";
		setup += "bbbbSpnnSOsb";
		setup += "BSSnSpnnSbnb";
		setup += "nnnnnpsnSnnn";
		setup += "nSSSSSSSSnnn";
		setup += "nlnlnlnlnnnn";
	fillInBlocks(setup);
	break;
	}// end case 50

	case 51:
	{
		setup  = "llrrrllrllls";
		setup += "lllllllllllR";
		setup += "lllllllllllr";
		setup += "llllllllllrr";
		setup += "lllllllllsrr";
		setup += "slvlvlvlllrr";
		setup += "lllllllllllr";
		setup += "llllllllllln";
		setup += "llllllllllln";
		setup += "llrrrllrllln";
		setup += "lsdrllrrrlln";
		setup += "lnrrrllrllln";
	fillInBlocks(setup);
	break;
	}// end case 51

	case 52:
	{
		setup  = "ssssssssssss";
		setup += "nnnnnnnnnnnn";
		setup += "nlnlnlnlnlnn";
		setup += "SvSvSvSvSvSn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "LnOnWnGnRnBn";
		setup += "LLbbrrggwwkn";
		setup += "dLbbrrggwwkn";
	fillInBlocks(setup);
	break;
	}// end case 52

	case 53:
	{
		setup  = "OBGnnlnnnsnn";
		setup += "nnnnbbbnnnnn";
		setup += "nnnbbbbbnnns";
		setup += "nnsbbbbbsnnn";
		setup += "ngbbbvbbbgnn";
		setup += "nbggbvbggbnn";
		setup += "ndbggbggbdnn";
		setup += "nSdSSbSSdSnn";
		setup += "nnnddLddnnnn";
		setup += "nnnSdddSnnnn";
		setup += "nnnngdgnnnnn";
		setup += "snnnkgnnnnnn";
	fillInBlocks(setup);
	break;
	}// end case 53

	case 54:
	{
		setup  = "Ssnnnnngnnls";
		setup += "BrnnwPnnnnnl";
		setup += "nnvnSwnnpGnn";
		setup += "nnlnnnnnspnn";
		setup += "nnvlnnbnnnnn";
		setup += "gSnnnnSbnnrs";
		setup += "Wgnnrnnnnnvr";
		setup += "nnnnnrnngRnd";
		setup += "nnpSnnnnngnn";
		setup += "nnnpnnwsnnnn";
		setup += "bsnnnnnwnnnn";
		setup += "dbnnbsnnnnnn";
	fillInBlocks(setup);
	break;
	}// end case 54

	case 55:
	{
		setup  = "ngggnssssnnn";
		setup += "gdggsngdgsnn";
		setup += "gggsggggggss";
		setup += "dgsgggggggdn";
		setup += "ggggdgdgdggn";
		setup += "gdgggnggggnn";
		setup += "nGggnnnggnnn";
		setup += "nnWwnnnwwnnn";
		setup += "nnwwnnnwwnnn";
		setup += "nnwwnnnwwnnn";
		setup += "nnwwnnnwwnnn";
		setup += "lnllnlnllnln";
	fillInBlocks(setup);
	break;
	}// end case 55

	case 56:
	{
		setup  = "blRwpwpwBbns";
		setup += "nnnnnPnnnbns";
		setup += "ggnSSSSSnbns";
		setup += "gbnnnnnnnbLL";
		setup += "grnnnpwwnSgg";
		setup += "gpnSnwlllsgg";
		setup += "gwnSnwWnlsgg";
		setup += "gbnSnkSnSsSL";
		setup += "glnsnkSnnnSd";
		setup += "gnnnnsSSSnSd";
		setup += "gnSSSSSnSnSS";
		setup += "GnvnnnnOSlnn";
	fillInBlocks(setup);
	break;
	}// end case 56

	case 57:
	{
		setup  = "nnSnnsnnnnnn";
		setup += "nnsnnnlnnnnn";
		setup += "nnnnnnsnnnnn";
		setup += "nnnsnnnlnnnn";
		setup += "nnnnnnnsnnnn";
		setup += "dSnnsnnnlnnn";
		setup += "lSnnnnnnsnnn";
		setup += "lSSnnsnnnlnn";
		setup += "llnSSnnnnsnn";
		setup += "llnnnnsnnnln";
		setup += "llnnnnnnnnsn";
		setup += "llnnnnnsnnnn";
	fillInBlocks(setup);
	break;
	}// end case 57

	case 58:
	{
		setup  = "dsnnnnnnnnBn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnsnnnnnn";
		setup += "nnnnsbnnnnnn";
		setup += "nnnWbrnnnnnn";
		setup += "dnsbrrrsgggs";
		setup += "dnsbwwwsllls";
		setup += "nnnGbwnnnnnn";
		setup += "nnnnsbnnnnnn";
		setup += "nnnnnsnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "dsnnnnnnnnRn";
	fillInBlocks(setup);
	break;
	}// end case 58

	case 59:
	{
		setup  = "knnnnnnnnnnk";
		setup += "nnnnnLLnnnnn";
		setup += "nnnnSLvSnnnn";
		setup += "nnnLvssLLnnn";
		setup += "nnLLLLLsvLnn";
		setup += "nLvsLddvLLLn";
		setup += "nnLLsLLsvLnn";
		setup += "nnnLvsLLLnnn";
		setup += "nnnnSLvSnnnn";
		setup += "nnnnnLLnnnnn";
		setup += "nnnnnnnnnnnk";
		setup += "Onnnnnnnnnnn";
	fillInBlocks(setup);
	break;
	}// end case 59

	case 60:
	{
		setup  = "GvngsgnnnnnB";
		setup += "vnnngnnnnnnd";
		setup += "nnnnnngnnnnd";
		setup += "ngggggggnnsd";
		setup += "nggggggggnsd";
		setup += "slllllllnnsd";
		setup += "nlllllllnnsd";
		setup += "nrrrrrrrnnnv";
		setup += "nrrrrrrrrnnn";
		setup += "nbbbbbbbbbnn";
		setup += "nbbbsbbnnbnn";
		setup += "RsppnppPnnnn";
	fillInBlocks(setup);
	break;
	}// end case 60

	case 61:
	{
		setup  = "bbrrrdllbbrr";
		setup += "bblllnllbbrr";
		setup += "rrrrrnllbbrr";
		setup += "lllllnllbbrr";
		setup += "rrrrrnllbbrr";
		setup += "dnnnnvnnndBR";
		setup += "llrllnllllll";
		setup += "llrllnllrrll";
		setup += "rrrrrnllrrll";
		setup += "llrllnllrrll";
		setup += "llrllnllllll";
		setup += "dnnnnsnnnnnn";
	fillInBlocks(setup);
	break;
	}// end case 61

	case 62:
	{
		setup  = "kkSRSpvSWngS";
		setup += "kkSnbSnGSnSB";
		setup += "kkSnSnLSnnsr";
		setup += "kkSnsnSnLSsn";
		setup += "nSSnsnsnSPsn";
		setup += "nSnnsnsnswsn";
		setup += "nsLSSLSLSLSL";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nSSSSSSSSSSS";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnvdOn";
	fillInBlocks(setup);
	break;
	}// end case 62

	case 63:
	{
		setup  = "gpwObgpwrbnl";
		setup += "sgpwrbspwknd";
		setup += "rbSpwrbgpwnl";
		setup += "vrsgpwRbGpnd";
		setup += "pwrbgpwrbgnl";
		setup += "gpwSbgpwrbnd";
		setup += "bspWrbgswrnl";
		setup += "rbgpwrbgpwnd";
		setup += "wrbgpwrBgpnl";
		setup += "Lwkbgpwrbgnd";
		setup += "gLwrbgpwrbnl";
		setup += "Pnnnnnnnnnnn";
	fillInBlocks(setup);
	break;
	}// end case 63

	case 64:
	{
		setup  = "SPnnnGSRnnnB";
		setup += "SnnnnnSnnnnn";
		setup += "SnnnnnSnnnnn";
		setup += "SnPSGnSnRSBn";
		setup += "SnnSnnSnnSnn";
		setup += "SnnSnnSnnSnn";
		setup += "SPnSnGSRnSnB";
		setup += "SSnSnnSnnSnn";
		setup += "nnnSnnsnnSnn";
		setup += "nnnSGnSnRSBn";
		setup += "lnnSnnnnnnrn";
		setup += "dlnSnnnnnSnn";
	fillInBlocks(setup);
	break;
	}// end case 64

	case 65:
	{
		setup  = "dbbdlllvllll";
		setup += "bdbbllllllll";
		setup += "bbdbnrrsrrrR";
		setup += "dbbdnrrrrrrr";
		setup += "bdbbllllvlll";
		setup += "bbdbllllllll";
		setup += "rrrrrrrrsrrn";
		setup += "rrrrrrrrrrrn";
		setup += "lllvllllllll";
		setup += "llllllllllll";
		setup += "rrrsrrrrrnrn";
		setup += "rrrrrrrrrrBn";
	fillInBlocks(setup);
	break;
	}// end case 65

	case 66:
	{
		setup  = "pppprrrrbbbb";
		setup += "ppprrrrrrbbb";
		setup += "ppprrssrrbbb";
		setup += "ppsrrnnrrsbb";
		setup += "pprrvnnvrrbb";
		setup += "pprrnnnnrrbb";
		setup += "psrrrrrrrrsb";
		setup += "prrrrrrrrrrb";
		setup += "prrBnnnnPrrb";
		setup += "prrnnnnnnrrb";
		setup += "rrnnnnnnnnrR";
		setup += "rrnnnddnnnrn";
	fillInBlocks(setup);
	break;
	}// end case 66

	case 67:
	{
		setup  = "sssssssdssss";
		setup += "sSsbsssnssss";
		setup += "snnnsssnssss";
		setup += "snnnssSnsssS";
		setup += "snnBsnnnnsnn";
		setup += "snnssnnnnnnn";
		setup += "nnnSnnnnnnnn";
		setup += "nnnnnnsnnnnn";
		setup += "nnnnnssdsSnn";
		setup += "nnnnnSssssnn";
		setup += "bbbSnlsSSSnn";
		setup += "ssssblslnnnn";
	fillInBlocks(setup);
	break;
	}// end case 67

	case 68:
	{
		setup  = "ssssssssssss";
		setup += "dnSnnnngsnnn";
		setup += "nnGndnSnnnnn";
		setup += "nnnnnRnnnnSn";
		setup += "nsnsnnnSnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "snnnnsnnnnnn";
		setup += "nnnnnnnnsnnn";
		setup += "rnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "ndnnnnnsnnnn";
		setup += "snsnnlnnnsnn";
	fillInBlocks(setup);
	break;
	}// end case 68

	case 69:
	{
		setup  = "vpbbSddSrrgv";
		setup += "pvbbSddSrrvg";
		setup += "vpbbSLLSrrgv";
		setup += "pvbbnnnnrrvg";
		setup += "vpbnnnnnnrgv";
		setup += "bbbRSnnSBrrr";
		setup += "bbbSPnnGSrrr";
		setup += "bbSnnnnnnSrr";
		setup += "kSnnnslnnnSk";
		setup += "SnnnnlsnnnnS";
		setup += "nnnnnslnnnnn";
		setup += "Onnnnlsnnnnn";
	fillInBlocks(setup);
	break;
	}// end case 69

	case 70:
	{
		setup  = "kllnnnnnLnnd";
		setup += "nllSSSvSSdnn";
		setup += "nnsSnnnnsSdn";
		setup += "nnsSnnnnsSdn";
		setup += "nnnsnnsnnsSd";
		setup += "nnnslnsnnsSd";
		setup += "nsnnsnSsnnsS";
		setup += "nsnnslnsnnsS";
		setup += "nSsnnsnlsnns";
		setup += "nrsnnRnnsnns";
		setup += "nrSsnnnnnsnn";
		setup += "nrOsnnnnnsnn";
	fillInBlocks(setup);
	break;
	}// end case 70

	default:
	{
		setup  = "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
		setup += "nnnnnnnnnnnn";
	fillInBlocks(setup);
	break;
	}// end default



	}//end switch

}//end setLevel()

/*setLevelNAME**************************************************************/
void Field::setLevelName()
{
 switch (currentLevel)
 {
  case 1: levelName = "Simpleton"; break;
  case 2: levelName = "Old Favorite"; break;
  case 3: levelName = "Plus Signs"; break;
  case 4: levelName = "Ring"; break;
  case 5: levelName = "Bank Shot"; break;
  case 6: levelName = "Locked Vault"; break;
  case 7: levelName = "Quicksand"; break;
  case 8: levelName = "Brackets"; break;
  case 9: levelName = "Middle Wall"; break;
  case 10: levelName = "Zig Zag"; break;
  case 11: levelName = "Key Wall"; break;
  case 12: levelName = "Super Blocks"; break;
  case 13: levelName = "Locksmith"; break;
  case 14: levelName = "Jail"; break;
  case 15: levelName = "Switch Back"; break;
  case 16: levelName = "Blockhead"; break;
  case 17: levelName = "Full House"; break;
  case 18: levelName = "Flag"; break;
  case 19: levelName = "Diamonds R 4-Ever"; break;
  case 20: levelName = "Diamonds 3"; break;
  case 21: levelName = "Symmetry"; break;
  case 22: levelName = "4 Blocks"; break;
  case 23: levelName = "Nerves of Steel"; break;
  case 24: levelName = "Insanity"; break;
  case 25: levelName = "U-Shape"; break;
  case 26: levelName = "Flip Flop"; break;
  case 27: levelName = "Spinners"; break;
  case 28: levelName = "Maze"; break;
  case 29: levelName = "Nooks"; break;
  case 30: levelName = "Death Alley"; break;
  case 31: levelName = "More Diamonds Welcome"; break;
  case 32: levelName = "Easy Street"; break;
  case 33: levelName = "Blue Birds"; break;
  case 34: levelName = "Zoom!"; break;
  case 35: levelName = "Amazing Grapes"; break;
  case 36: levelName = "Bar None"; break;
  case 37: levelName = "Foam"; break;
  case 38: levelName = "Three of a Kind"; break;
  case 39: levelName = "Death Stars"; break;
  case 40: levelName = "Fetch the Key"; break;
  case 41: levelName = "Deyemonds"; break;
  case 42: levelName = "Diamond Delight"; break;
  case 43: levelName = "Spire"; break;
  case 44: levelName = "Staircase"; break;
  case 45: levelName = "Rhombus"; break;
  case 46: levelName = "Tidal Wave"; break;
  case 47: levelName = "Jump!"; break;
  case 48: levelName = "Splatter"; break;
  case 49: levelName = "X Marks the Spot"; break;
  case 50: levelName = "Sparkling Passages"; break;
  case 51: levelName = "Color Blue"; break;
  case 52: levelName = "Upsidedown"; break;
  case 53: levelName = "Big Vee"; break;
  case 54: levelName = "Watch Your Step"; break;
  case 55: levelName = "Eden"; break;
  case 56: levelName = "Trespass"; break;
  case 57: levelName = "Diamond in the Rough"; break;
  case 58: levelName = "This Way..."; break;
  case 59: levelName = "Diamaunde"; break;
  case 60: levelName = "Lean to the Right"; break;
  case 61: levelName = "Countries"; break;
  case 62: levelName = "Frenzy"; break;
  case 63: levelName = "Dime and Despair"; break;
  case 64: levelName = "Virtual Reality"; break;
  case 65: levelName = "America"; break;
  case 66: levelName = "Ace"; break;
  case 67: levelName = "Graveyard"; break;
  case 68: levelName = "Easy as Die"; break;
  case 69: levelName = "\"Die, Mon!\""; break;
  case 70: levelName = "Pure Faith"; break;
  default: levelName = ""; break;
 }//end switch
 
}// end setLevelName()

/*moveBALL******************************************************************/
void Field::moveBall()
{
     int previousScore = score;
     ball->move();
     if(previousScore / 5000 < score/5000 )
                        //5000pts and less
     {
      lives++;
      #ifdef WIN32
      if(sound)
      oneup_snd->Play();
      #endif
     }
}//end moveBall()

/*setToMoveLEFT*************************************************************/
void Field::setToMoveLeft()
{
 if(!horizReversed)
  ball->moveLeftOnce();
 else
  ball->moveRightOnce();
}

/*setToMoveRIGHT************************************************************/
void Field::setToMoveRight()
{
 if(!horizReversed)
  ball->moveRightOnce();
 else
  ball->moveLeftOnce();
}

/*getBLOCK******************************************************************/
Block Field::getBlock(int y, int x)
{
 if( y>=0 && y<ARRAYHEIGHT
  && x>=0 && x<ARRAYWIDTH)
 {
	 return blocks[y][x];
 }
 else
 {return NULL_BLOCK;}
}

/***************************************************************************/
/***************************************************************************/
