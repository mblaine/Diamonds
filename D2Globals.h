#ifndef _D2GLOBALS_H
#define _D2GLOBALS_H

#include <windows.h>
#include <ddraw.h>
#include <dsound.h>

#include "Constants.h"
#include "HiScore.h"
#include "Field.h"
#include "Ball.h"
#include "Block.h"
#include "Resource_Diamonds.h"
#include "Surface.h"
#include "SoundBuffer.h"



//global handles
extern HWND myHandle;
extern HINSTANCE hinst1;
extern HACCEL hmyaccel;
extern HMENU hmenu;

//handles to bitmaps
extern HBITMAP readout_bmp;
extern HBITMAP logo_bmp;

//handles to bitmap surfaces
extern Surface* ball_surf;
extern Surface* blocks_surf;
extern Surface* readout_surf;
extern Surface* bkgrnd_surf;

//handles to wave buffers
extern SoundBuffer* bounce_snd;
extern SoundBuffer* colorblk_snd;
extern SoundBuffer* colorkey_snd;
extern SoundBuffer* diamond_snd;
extern SoundBuffer* die_snd;
extern SoundBuffer* laughing_snd;
extern SoundBuffer* levelwon_snd;
extern SoundBuffer* lock_snd;
extern SoundBuffer* smallkey_snd;
extern SoundBuffer* reverse_snd;
extern SoundBuffer* timebonus_snd;
extern SoundBuffer* oneup_snd;
extern SoundBuffer* bounce2_snd;

//handle to brushes for drawing screen
extern HBRUSH ltGray;
extern HBRUSH gray;
extern HBRUSH borderBrushes[7];


//if a game is running
extern bool gameOn;

//if sound is on or off
extern bool sound;


//high scores
extern HiScore *scores;

//for actual game
extern Field *field;

extern bool isPaused;

//for DirectDraw
extern LPDIRECTDRAW        DirDrawObj;    // DirectDraw object
extern LPDIRECTDRAWSURFACE SurfaceFront;  // DirectDraw fronbuffer surface
extern LPDIRECTDRAWSURFACE SurfaceBack;   // DirectDraw backbuffer surface
extern LPDIRECTDRAWCLIPPER clipper;

//for DirectSound
extern LPDIRECTSOUND DirSoundObj;//DirectSound object


#endif
