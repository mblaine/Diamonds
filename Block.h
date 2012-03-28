/*
 * BLOCK.H
 * created by Matthew Blaine, October 2005
 *
 * The base class 'Block' is a generic block in a game of 'Diamonds'. Each
 * sub-class represents each different variety of block. A 'Field' object 
 * will call a 'block' through a base class pointer in the event of a 'ball'
 * object making contact with a given block. The block returns information
 * about itself allowing the 'field' to have the 'ball' respond accordingly.
 */

#ifndef _BLOCK_H
#define _BLOCK_H

#include <cstdlib>
#include <string>
#include "Constants.h"
using namespace std;

/*BASE CLASS****************************************************************/
class Block
{
 public:
 Block(){;}
 virtual ~Block(){;}
 
 //called by field object and function that prints field to screen
 virtual BlockToField collision() = 0;

};//end class Block
/***************************************************************************/
/***************************************************************************/


/*DIAMOND*******************************************************************/
//a diamond block
class Diamond : public Block
{
 public:
 Diamond(){;}
 virtual ~Diamond(){;}
 
 virtual BlockToField collision()
 {
  BlockToField info = {MSG_DIAMOND, 0};
  return info;
 }
 
};//end class Diamond
/***************************************************************************/
/***************************************************************************/


/*SKULL*********************************************************************/
//a skull block
class Skull : public Block
{
 public:
 Skull(){;}
 virtual ~Skull(){;}
 
 virtual BlockToField collision()
 {
  BlockToField info = {MSG_SKULL, 0};
  return info;
 }
 
};//end class Skull
/***************************************************************************/
/***************************************************************************/


/*SMALL KEY*****************************************************************/
//a small key block, for removing a lock block
class SmallKey : public Block
{
 public:
 SmallKey(){;}
 virtual ~SmallKey(){;}
 
 virtual BlockToField collision()
 {
  BlockToField info = {MSG_SMALLKEY, 0};
  return info;
 }

};//end class SmallKey
/***************************************************************************/
/***************************************************************************/


/*LOCK**********************************************************************/
//a lock block
class Lock : public Block
{
 public:
 Lock(){;}
 virtual ~Lock(){;}
 
 virtual BlockToField collision()
 {
  BlockToField info = {MSG_LOCK, 0};
  return info;
 }
};//end class Lock
/***************************************************************************/
/***************************************************************************/


/*SOLID*********************************************************************/
//a solid, permanent block
class Solid : public Block
{
 public:
 Solid(){;}
 virtual ~Solid(){;}
 
 virtual BlockToField collision()
 {
  BlockToField info = {MSG_SOLID, 0};
  return info;
 }
};//end class Solid
/***************************************************************************/
/***************************************************************************/


/*REVERSE*******************************************************************/
//a block w/ two arrows, to reverse a ball's left and right motion
class Reverse : public Block
{
 public:
 Reverse(){;}
 virtual ~Reverse(){;}
 
 virtual BlockToField collision()
 {
  BlockToField info = {MSG_REVERSE, 0};
  return info;
 }
};//end class Reverse
/***************************************************************************/
/***************************************************************************/


/*COLOR CHANGE KEY**********************************************************/
//a key block that changes a ball's color
class ColorChangeKey : public Block
{
 private:
 color keyColor;
 
 public:
 ColorChangeKey(){;}
 ColorChangeKey(color c) {keyColor = c;}
 virtual ~ColorChangeKey(){;}
 
 virtual BlockToField collision()
 {
  BlockToField info = {MSG_COLORCHANGEKEY, keyColor};
  return info;
 }
};//end class ColorChangeKey
/***************************************************************************/
/***************************************************************************/


/*COLOR BLOCK***************************************************************/
//a block that can be destroyed when a ball w/ the same color makes contact
class ColorBlock : public Block
{
 private:
 color blockColor;

 public:
 ColorBlock(){;}
 ColorBlock(color c) {blockColor = c;}
 virtual ~ColorBlock(){;}
 
 virtual BlockToField collision()
 {
  BlockToField info = {MSG_COLORBLOCK, blockColor};
  return info;
 }
};//end class ColorBlock
/***************************************************************************/
/***************************************************************************/
#endif
