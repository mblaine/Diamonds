#include <windows.h>
#include <dsound.h>
#include "D2Globals.h"
#include "SoundBuffer.h"
#include "dsutil.h"

// makes use of code from :
//   http://www.sunlightd.com/Windows/DirectX/DirectX7/Sound.html

/*CONSTRUCTOR***************************************************************/
SoundBuffer::SoundBuffer(LPDIRECTSOUND hDS, LPCSTR name)
// DirectSound object, and then use MAKEINTRESOURCE( WAVE_RESOURCE_ID )
{
 wavName = name;
 
               //helper function
 soundBuffer = DSLoadSoundBuffer(hDS, wavName);
 
 if(soundBuffer)
 {creationSuccess = true;}
 else
 {creationSuccess = false;}
 
}//end constructor

/*DESTRUCTOR****************************************************************/
SoundBuffer::~SoundBuffer()
{
 if(soundBuffer)
 {soundBuffer->Release();}
}//end destructor


/*PLAY**********************************************************************/
void SoundBuffer::Play()
{
     HRESULT hRet;//to check for success
     hRet = soundBuffer->Play(0, 0, 0);//play once;
     
     while (FAILED(hRet))
      {
          if ((hRet == DSERR_BUFFERLOST) &&
              DSReloadSoundBuffer(soundBuffer, wavName))
          {
              hRet = soundBuffer->Play(0, 0, 0);// try again
          }

      }
      
      return;
     
}//end Play()
