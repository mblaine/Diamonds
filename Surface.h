#ifndef _SURFACE_H
#define _SURFACE_H
#include <ddraw.h>
// Surface.h used to contain and manipulate a bitmap to be used with Direct
// Draw.
// credit for this class goes to Mauricio Ritter
// http://www.codeproject.com/directx/BasicDD/
class Surface
{
    private:
    COLORREF ColorKey;
    UINT Height;
    UINT Width;
    LPDIRECTDRAWSURFACE SurfacePtr;
    public:
    BOOL creationSuccessful;
    
    public:
    Surface();
    Surface(LPDIRECTDRAW hDD,int nWidth,int nHeight,COLORREF dwColorKey=-1);
    ~Surface();

    BOOL LoadBitmap(HINSTANCE hInst,UINT resourceID,int fromX,int fromY,
                          int nWidth=0, int nHeight=0);
    BOOL Draw(LPDIRECTDRAWSURFACE lpDest, int toX, int toY, 
                    int fromX, int fromY, int nWidth=0, int nHeight=0);



      
};//end class Surface
/***************************************************************************/
/***************************************************************************/


#endif
