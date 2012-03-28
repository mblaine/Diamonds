#ifndef _SDLSOUND_H
#define _SDLSOUND_H

#include "SDL.h"
#include "SDL_mixer.h"

#include "Field.h"

class SDLSound
{
   private:          
      Mix_Chunk* bounceSound;
      Mix_Chunk* bounce2Sound;
      Mix_Chunk* colorBlockSound;
      Mix_Chunk* colorChangeSound;
      Mix_Chunk* diamondsSound;
      Mix_Chunk* dieSound;
      Mix_Chunk* keySound;
      Mix_Chunk* laughingSound;
      Mix_Chunk* levelWonSound;
      Mix_Chunk* lockSound;
      Mix_Chunk* oneUpSound;
      Mix_Chunk* reverseSound;
      Mix_Chunk* timeBonusSound;
   
   public:
      bool SoundOn;
          
      SDLSound(bool soundOn = true);
      ~SDLSound();
      
      //play any necessary sounds after a frame of animation
      void DoSoundEffects(Field* field);
      
      void PlayLauging();
      void PlayDie();
      void PlayLevelWon();
      void PlayTimeBonus();
      void PlayOneUp();
};

#endif
