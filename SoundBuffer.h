#ifndef _SOUNDBUFFER_H
#define _SOUNDBUFFER_H

#include <windows.h>
#include <dsound.h>
#include "D3Globals.h"
#include "dsutil.h"

// makes use of code from David McCabe :
//   http://www.sunlightd.com/Windows/DirectX/DirectX7/Sound.html

class SoundBuffer
{
      private:
      LPDIRECTSOUNDBUFFER soundBuffer; // the buffer for holding the sound
      LPCSTR wavName; // the name of the .wav clip in the buffer
      
      public:
      bool creationSuccess;
      
      public:
      // DirectSound object, and then use MAKEINTRESOURCE( WAVE_RESOURCE_ID )
      SoundBuffer(LPDIRECTSOUND hDS, LPCSTR name);
      ~SoundBuffer();
      void Play();
};//end class SoundBuffer

#endif
