/*
 * FIELD.CPP
 * to accompany Field.h
 */
 
 #include "Field.h"
 #include "Ball.h"


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
  hasSmallKey = false;
  horizReversed = false;
  score = 0;
  setLevelName();
 
 }//end default constructor

/*DESTRUCTOR****************************************************************/
 Field::~Field()//destructor
 {
  for( int y=0; y<ARRAYHEIGHT; y++)
  {
       for( int x=0; x<ARRAYWIDTH; x++)
       { delete blocks[y][x]; blocks[y][x]=NULL; }
  }
 }//end destructor

/*nextLEVEL*****************************************************************/
void Field::nextLevel()
{
  currentLevel++;
  
  delete ball;
  ball = NULL;
     
  for( int y=0; y<ARRAYHEIGHT; y++)
  {
       for( int x=0; x<ARRAYWIDTH; x++)
       { delete blocks[y][x]; blocks[y][x]=NULL; }
  }
  
  gameState = PLAYING;
  hasSmallKey = false;
  horizReversed = false;

  ball = new Ball(11*BLOCKWIDTH, 11*BLOCKHEIGHT, (int)BALLDELTA, this);
  setLevel();
  timeBonus = 46;
  setLevelName();
     
}//end nextLevel()

/*restartLEVEL**************************************************************/
void Field::restartLevel()
{      
  for( int y=0; y<ARRAYHEIGHT; y++)
  {
       for( int x=0; x<ARRAYWIDTH; x++)
       { delete blocks[y][x]; blocks[y][x]=NULL; }
  }

  hasSmallKey = false;
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
  * B(lue), R(ed), G(reen), (bro)W(n), P(urple), O(range)  =color change keys
  * d(iamond), s(kull), small (k)ey, L(ock), S(olid), (re)v(erse), n(ull)
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
                blocks[y][x] = new ColorBlock(ltBlue);
                colorBlocks++;
                break;

                case 'b':
                blocks[y][x] = new ColorBlock(blue);
                colorBlocks++;
                break;
        
                case 'r':
                blocks[y][x] = new ColorBlock(red);
                colorBlocks++;
                break;
        
                case 'g':
                blocks[y][x] = new ColorBlock(green);
                colorBlocks++;
                break;
        
                case 'w':
                blocks[y][x] = new ColorBlock(brown);
                colorBlocks++;
                break;
        
                case 'p':
                blocks[y][x] = new ColorBlock(purple);
                colorBlocks++;
                break;


                //color change keys
                case 'B':
                blocks[y][x] = new ColorChangeKey(blue);
                break;
        
                case 'R':
                blocks[y][x] = new ColorChangeKey(red);
                break;
        
                case 'G':
                blocks[y][x] = new ColorChangeKey(green);
                break;
        
                case 'W':
                blocks[y][x] = new ColorChangeKey(brown);
                break;
        
                case 'P':
                blocks[y][x] = new ColorChangeKey(purple);
                break;
        
                case 'O':
                blocks[y][x] = new ColorChangeKey(orange);
                break;
        
        
                //the rest
                case 'd':
                blocks[y][x] = new Diamond();
                diamondBlocks++;
                break;
        
        	   	case 's':
                blocks[y][x] = new Skull();
                break;
        
                case 'k':
                blocks[y][x] = new SmallKey();
                break;
        
                case 'L':
                blocks[y][x] = new Lock();
                break;
        
                case 'S':
                blocks[y][x] = new Solid();
                break;
        
                case 'v':
                blocks[y][x] = new Reverse;
                break;
        
                default:
                blocks[y][x] = NULL;
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
      BlockToField returnedInfo;//info to be ret'd from a block object

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
       if(blocks[north][west]!=NULL 
          && !(blocks[south][west]!=NULL && blocks[north][east]!=NULL)  )
          {cellY = north; cellX = west; collisionOccured = true;}
      }

            //collision w/ ball's top-right corner, but not if ball is wedged
            //  in a corner, touching a block it shouldn't
      if(north>=0 && east<ARRAYWIDTH && !collisionOccured)
      {
       if(blocks[north][east]!=NULL 
          && !(blocks[south][east]!=NULL && blocks[north][west]!=NULL)  )
          {cellY = north; cellX = east; collisionOccured = true;}
      }

            //collision w/ ball's bottom-right corner, but not if ball is
            //  wedged in a corner, touching a block it shouldn't
      if(south<ARRAYHEIGHT && east<ARRAYWIDTH && !collisionOccured)
      {
       if(blocks[south][east]!=NULL 
          && !(blocks[north][east]!=NULL && blocks[south][west]!=NULL)  )
          {cellY = south; cellX = east; collisionOccured = true;}
      }

            //collision w/ ball's bottom-left corner, but not if ball is
            //  wedged in a corner, touching a block it shouldn't
      if(south<ARRAYHEIGHT && west>=0 && !collisionOccured)
      {
       if(blocks[south][west]!=NULL 
          && !(blocks[north][west]!=NULL && blocks[south][east]!=NULL)  )
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
             && blocks[south][east]==NULL && blocks[north][east]==NULL )
      {toReturn.xMoveInfo = XBACKRIGHT;}
      
      //hit corner of block, not touching any others blocks around
      else if(north!=south && blocks[south][west]==NULL
             && blocks[south][east]==NULL && blocks[north][east]==NULL )
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
      else if(west!=east && blocks[north][east]!=NULL
             && blocks[south][east]==NULL && blocks[south][west]==NULL )
      {toReturn.yMoveInfo = YGODOWN;}
            
      //ball actually wedged in a corner, should move opposite direction
      //  than normally expected
      else if(west!=east && blocks[south][east]!=NULL)
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
      else if(north!=south && blocks[south][east]!=NULL
             && blocks[south][west]==NULL && blocks[north][west]==NULL )
      {toReturn.xMoveInfo = XBACKLEFT;}
      
      //hit corner of block, not touching any others blocks around
      else if(north!=south && blocks[south][west]==NULL
             && blocks[south][east]==NULL && blocks[north][west]==NULL )
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
      else if(west!=east && blocks[north][west]!=NULL
             && blocks[south][east]==NULL && blocks[south][west]==NULL )
      {toReturn.yMoveInfo = YGODOWN;}
      
      //ball actually wedged in a corner, should move opposite direction
      //  than normally expected
      else if(west!=east && blocks[south][west]!=NULL)
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
      else if(north!=south && blocks[north][east]!=NULL
             && blocks[south][west]==NULL && blocks[north][west]==NULL )
      {toReturn.xMoveInfo = XBACKLEFT;}
      
      //hit corner of block, not touching any others blocks around
      else if(north!=south && blocks[south][west]==NULL
             && blocks[north][west]==NULL && blocks[north][east]==NULL )
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
      else if(west!=east && blocks[south][west]!=NULL
             && blocks[north][east]==NULL && blocks[north][west]==NULL )
      {toReturn.yMoveInfo = YGOUP;}
      
      //ball actually wedged in a corner, should move opposite direction
      //  than normally expected
      else if(west!=east && blocks[north][west]!=NULL)
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
      else if(north!=south && blocks[north][west]!=NULL
             && blocks[south][east]==NULL && blocks[north][east]==NULL )
      {toReturn.xMoveInfo = XBACKRIGHT;}
      
      //hit corner of block, not touching any others blocks around
      else if(north!=south && blocks[north][east]==NULL
             && blocks[south][east]==NULL && blocks[north][west]==NULL )
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
      else if(west!=east && blocks[south][east]!=NULL
             && blocks[north][east]==NULL && blocks[north][west]==NULL )
      {toReturn.yMoveInfo = YGOUP;}
      
      //ball actually wedged in a corner, should move opposite direction
      //  than normally expected
      else if(west!=east && blocks[north][east]!=NULL)
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
        returnedInfo = blocks[cellY][cellX]->collision();//get info

        switch(returnedInfo.msg)//switch on the message sent from the block
        {
          case MSG_DIAMOND:
               if(colorBlocks == 0)//if no color blocks left
               {
                delete blocks[cellY][cellX];//destroy it
                blocks[cellY][cellX] = NULL;
                diamondBlocks--;
                score+=100;
                #ifdef WIN32
                if(sound)
                PlaySound(MAKEINTRESOURCE(DIAMONDSND),
                   hinst1,SND_RESOURCE|SND_ASYNC);
                #endif
               }
               else
               {
                #ifdef WIN32
                if(sound)
                PlaySound(MAKEINTRESOURCE(BOUNCE2SND),
                   hinst1,SND_RESOURCE|SND_ASYNC);
                #endif
               }
               if(diamondBlocks == 0)//if no more diamonds now
                {gameState = LEVELWON;}
               break;
          
          case MSG_SKULL:
               gameState = BALLDIED;
               break;

          case MSG_COLORCHANGEKEY://tell ball to change its color
               if(info.c != (color)returnedInfo.info)
               {
                toReturn.change = (color)returnedInfo.info;
                #ifdef WIN32
                if(sound)
                PlaySound(MAKEINTRESOURCE(COLORKEYSND),
                   hinst1,SND_RESOURCE|SND_ASYNC);
                #endif
               }
               else
               {
                #ifdef WIN32
                if(sound)
                PlaySound(MAKEINTRESOURCE(BOUNCE2SND),
                   hinst1,SND_RESOURCE|SND_ASYNC);
                #endif
               }
               break;

          case MSG_COLORBLOCK:
               if(info.c == (color)returnedInfo.info)//if colors match
               {
                delete blocks[cellY][cellX];//destroy it
                blocks[cellY][cellX] = NULL;
                colorBlocks--;
                score+=3;
                #ifdef WIN32
                if(sound)
                PlaySound(MAKEINTRESOURCE(COLORBLKSND),
                   hinst1,SND_RESOURCE|SND_ASYNC);
                #endif
               }
               else
               {
                #ifdef WIN32
                if(sound)
                PlaySound(MAKEINTRESOURCE(BOUNCE2SND),
                   hinst1,SND_RESOURCE|SND_ASYNC);
                #endif
               }
               break;

          case MSG_SOLID://hit immovable block
                #ifdef WIN32
                if(sound)
                PlaySound(MAKEINTRESOURCE(BOUNCE2SND),
                   hinst1,SND_RESOURCE|SND_ASYNC);
                #endif
               break;

          case MSG_SMALLKEY://hit a small key block
               if(!hasSmallKey && info.c == orange)
               {
                delete blocks[cellY][cellX];//destroy key block
                blocks[cellY][cellX] = NULL;
                hasSmallKey = true;
                score+=25;
                #ifdef WIN32
                if(sound)
                PlaySound(MAKEINTRESOURCE(SMALLKEYSND),
                   hinst1,SND_RESOURCE|SND_ASYNC);
                #endif
               }
               else
               {
                #ifdef WIN32
                if(sound)
                PlaySound(MAKEINTRESOURCE(BOUNCE2SND),
                   hinst1,SND_RESOURCE|SND_ASYNC);
                #endif
               }
               break;

          case MSG_LOCK://hit a lock
               if(hasSmallKey && info.c == orange)
               {
                delete blocks[cellY][cellX];//destroy lock
                blocks[cellY][cellX] = NULL;
                hasSmallKey = false;
                score+=50;
                #ifdef WIN32
                if(sound)
                PlaySound(MAKEINTRESOURCE(LOCKSND),
                   hinst1,SND_RESOURCE|SND_ASYNC);
                #endif
               }
               else
               {
                #ifdef WIN32
                if(sound)
                PlaySound(MAKEINTRESOURCE(BOUNCE2SND),
                   hinst1,SND_RESOURCE|SND_ASYNC);
                #endif
               }
               break;

          case MSG_REVERSE://hit a 'reverse' block
               if(!horizReversed)
               {horizReversed = true;}
               else
               {horizReversed = false;}
               delete blocks[cellY][cellX];//destroy it
               blocks[cellY][cellX] = NULL;
                #ifdef WIN32
                if(sound)
                PlaySound(MAKEINTRESOURCE(REVERSESND),
                   hinst1,SND_RESOURCE|SND_ASYNC);
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

	 string setup = "";     

	 diamondBlocks = 0;
	 colorBlocks = 0;

/*
 * l(ight blue), b(lue), r(ed), g(reen), (bro)w(n), p(urple) =  color blocks
 * B(lue), R(ed), G(reen), (bro)W(n), P(urple), O(range) = color change keys
 * d(iamond), s(kull), small (k)ey, L(ock), S(olid), (re)v(erse), n(ull)
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

	 return;
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
  default: levelName = ""; break;
 }//end switch
 
}// end setLevelName()

/*moveBALL******************************************************************/
void Field::moveBall()
{
     int previousScore = score;
     ball->move();
     if(previousScore / 5000 < score/5000 && lives < 8 )
                        //5000pts and less than max. lives
     {
        lives++;
        #ifdef WIN32
        if(sound)
        PlaySound(MAKEINTRESOURCE(ONEUPSND),
           hinst1,SND_RESOURCE|SND_ASYNC);
        #endif
     }
}//end moveBall()

/*setToMoveLEFT*************************************************************/
void Field::setToMoveLeft()
{ball->moveLeftOnce();}

/*setToMoveRIGHT************************************************************/
void Field::setToMoveRight()
{ball->moveRightOnce();}

/*getBLOCK******************************************************************/
Block* Field::getBlock(int y, int x)
{
 if( y>=0 && y<ARRAYHEIGHT
  && x>=0 && x<ARRAYWIDTH)
 {
	 return blocks[y][x];
 }
 else
 {return NULL;}
}

/***************************************************************************/
/***************************************************************************/
