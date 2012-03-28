#include "SDLSound.h"
#include "SDL.h"
#include "SDL_mixer.h"

SDLSound::SDLSound(bool SoundOn)
{
   this->SoundOn = SoundOn;
   
   //init SDL audio
   SDL_Init(SDL_INIT_AUDIO);
   Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 512);
                        
   bounceSound = Mix_LoadWAV("data/bounce.ogg");
   bounce2Sound = Mix_LoadWAV("data/bounce2.ogg");
   colorBlockSound = Mix_LoadWAV("data/colorblock.ogg");
   colorChangeSound = Mix_LoadWAV("data/colorchange.ogg");
   diamondsSound = Mix_LoadWAV("data/diamond.ogg");
   dieSound = Mix_LoadWAV("data/die.ogg");
   keySound = Mix_LoadWAV("data/key.ogg");
   laughingSound = Mix_LoadWAV("data/laughing.ogg");
   levelWonSound = Mix_LoadWAV("data/levelwon.ogg");
   lockSound = Mix_LoadWAV("data/lock.ogg");
   oneUpSound = Mix_LoadWAV("data/oneup.ogg");
   reverseSound = Mix_LoadWAV("data/reverse.ogg");
   timeBonusSound = Mix_LoadWAV("data/timebonus.ogg");
}

SDLSound::~SDLSound()
{
   if(bounceSound)
     Mix_FreeChunk(bounceSound);
   if(bounce2Sound)
     Mix_FreeChunk(bounce2Sound);
   if(colorBlockSound)
     Mix_FreeChunk(colorBlockSound);
   if(colorChangeSound)
     Mix_FreeChunk(colorChangeSound);
   if(diamondsSound)
     Mix_FreeChunk(diamondsSound);
   if(dieSound)
     Mix_FreeChunk(dieSound);
   if(keySound)
     Mix_FreeChunk(keySound);
   if(laughingSound)
     Mix_FreeChunk(laughingSound);
   if(levelWonSound)
     Mix_FreeChunk(levelWonSound);
   if(lockSound)
     Mix_FreeChunk(lockSound);
   if(oneUpSound)
     Mix_FreeChunk(oneUpSound);
   if(reverseSound)
     Mix_FreeChunk(reverseSound);
   if(timeBonusSound)
     Mix_FreeChunk(timeBonusSound);
   
   //shutdown SDL audio
   Mix_CloseAudio();
   SDL_QuitSubSystem(SDL_INIT_AUDIO);
   if(SDL_WasInit(SDL_INIT_EVERYTHING)==0)
     SDL_Quit();
}

void SDLSound::DoSoundEffects(Field* field)
{
   if(!SoundOn)
     return;
   
   if(field->oneUpLastMove)
     Mix_PlayChannel(-1, oneUpSound, 0);
   
   if(field->getBallHitWallLastMove())
     Mix_PlayChannel(-1, bounceSound, 0);
   
   switch(field->lastBlockHit)
   {
     case NULL_BLOCK:
     case SKULL:
          break;
     case DIAMOND:
          Mix_PlayChannel(-1, diamondsSound, 0);
          break;
     case SOLID:
          Mix_PlayChannel(-1, bounce2Sound, 0);
          break;
     case BRUSH_BLUE:
     case BRUSH_RED:
     case BRUSH_GREEN:
     case BRUSH_BROWN:
     case BRUSH_PURPLE:
     case BRUSH_ORANGE:
     case BRUSH_YELLOW:
     case BRUSH_BLACK:
     case BRUSH_WHITE:
          Mix_PlayChannel(-1, colorChangeSound, 0);
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
          Mix_PlayChannel(-1, colorBlockSound, 0);
          break;
     case KEY:
          Mix_PlayChannel(-1, keySound, 0);
          break;
     case LOCK:
          Mix_PlayChannel(-1, lockSound, 0);
          break;
     case REVERSE:
          Mix_PlayChannel(-1, reverseSound, 0);
          break;
   }
}

void SDLSound::PlayLauging()
{
   if(SoundOn)
     Mix_PlayChannel(-1, laughingSound, 0);
}

void SDLSound::PlayDie()
{
   if(SoundOn)
     Mix_PlayChannel(-1, dieSound, 0);
}

void SDLSound::PlayLevelWon()
{
   if(SoundOn)
     Mix_PlayChannel(-1, levelWonSound, 0);
}

void SDLSound::PlayTimeBonus()
{
   if(SoundOn)
     Mix_PlayChannel(-1, timeBonusSound, 0);
}

void SDLSound::PlayOneUp()
{
   if(SoundOn)
     Mix_PlayChannel(-1, oneUpSound, 0);
}
