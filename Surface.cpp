
#include "Surface.h"
#include <ddraw.h>

// credit for this class goes to Mauricio Ritter
// http://www.codeproject.com/directx/BasicDD/

/*CONSTRUCTOR 1*************************************************************/
Surface::Surface()
{
    ColorKey = -1;
    Height = 0;
    Width = 0;
    SurfacePtr = NULL;
    creationSuccessful = false;
}//end constructor 1

/*CONSTRUCTOR 2*************************************************************/
Surface::Surface(LPDIRECTDRAW hDD,int nWidth,int nHeight,COLORREF dwColorKey)
{
    DDSURFACEDESC     ddsd;
    HRESULT           hRet;
    DDCOLORKEY        ddck;


    ZeroMemory( &ddsd, sizeof( ddsd ) );

    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
    
    ddsd.dwWidth  = nWidth;
    ddsd.dwHeight = nHeight;

    hRet = hDD->CreateSurface(&ddsd, &SurfacePtr, NULL );
    if( hRet != DD_OK )
    {
        
         if(hRet == DDERR_OUTOFVIDEOMEMORY)
         {
         ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN |
                       DDSCAPS_SYSTEMMEMORY;
    
         hRet = hDD->CreateSurface(&ddsd, &SurfacePtr, NULL );
         }

         if( hRet != DD_OK )
         {
             creationSuccessful = false;
         }
    }


    if((int)dwColorKey != -1)
    {
    ddck.dwColorSpaceLowValue = dwColorKey;
    ddck.dwColorSpaceHighValue = 0;
    SurfacePtr->SetColorKey(DDCKEY_SRCBLT, &ddck);
    }

    ColorKey = dwColorKey;
    Width  = nWidth;
    Height = nHeight;

    creationSuccessful = true;
}//end constructor 2


/*DESTRUCTOR****************************************************************/
Surface::~Surface()
{

    if(SurfacePtr != NULL)
    {
        SurfacePtr->Release();
        SurfacePtr = NULL;
    }
}//end DESTRUCTOR


/*LoadBitmap****************************************************************/
BOOL Surface::LoadBitmap(HINSTANCE hInst, UINT resourceID,
                          int fromX, int fromY, int nWidth, int nHeight)
{
    if(!creationSuccessful)
    {return FALSE;}
    
    HDC                     hdcImage;
    HDC                     hdc;
    BITMAP                  bm;
    DDSURFACEDESC           ddsd;
    HRESULT                 hr;

    HBITMAP    hbm;

    hbm = (HBITMAP) LoadImage(hInst, MAKEINTRESOURCE(resourceID), 
                              IMAGE_BITMAP, nWidth, nHeight, 0L);

    if (hbm == NULL || SurfacePtr == NULL)
        return FALSE;

    // Make sure this surface is restored.
    SurfacePtr->Restore();

    // Select bitmap into a memoryDC so we can use it.
    hdcImage = CreateCompatibleDC(NULL);
    if (!hdcImage)
        return FALSE;

    SelectObject(hdcImage, hbm);

    // Get size of the bitmap
    GetObject(hbm, sizeof(bm), &bm);

    if(nWidth == 0)
    nWidth = bm.bmWidth;
    
    if(nHeight == 0)
    nHeight = bm.bmHeight;
    
    // Get size of surface.
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    SurfacePtr->GetSurfaceDesc(&ddsd);

    if ((hr = SurfacePtr->GetDC(&hdc)) == DD_OK)
    {
        StretchBlt(hdc, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, 
                   fromX, fromY, nWidth, nHeight, SRCCOPY);
        SurfacePtr->ReleaseDC(hdc);
    }
    DeleteDC(hdcImage);

    /*m_srcInfo.m_hInstance   = hInst;
    m_srcInfo.m_nResource   = resourceID;
    m_srcInfo.m_iX          = nX;
    m_srcInfo.m_iY          = nY;
    m_srcInfo.m_iWidth      = nWidth;
    m_srcInfo.m_iHeight     = nHeight;*/
    
    return TRUE;
}//end LoadBitmap()

/*DRAW**********************************************************************/
BOOL Surface::Draw(LPDIRECTDRAWSURFACE lpDest, int toX, int toY, 
                    int fromX, int fromY, int nWidth, int nHeight)
{
    if(!creationSuccessful)
    {return FALSE;}

    RECT      rcRect;
    HRESULT    hRet;

    if(nWidth == 0)
        nWidth = Width;

    if(nHeight == 0)
        nHeight = Height;

    rcRect.left   = fromX;
    rcRect.top    = fromY;
    rcRect.right  = nWidth  + fromX;
    rcRect.bottom = nHeight + fromY;

    while(1)
    {
        if((int)ColorKey < 0)
        {
            hRet = lpDest->BltFast(toX, toY, SurfacePtr, 
                                   &rcRect,  DDBLTFAST_NOCOLORKEY);
        }
        else
        {
            hRet = lpDest->BltFast(toX, toY, SurfacePtr, 
                                   &rcRect,  DDBLTFAST_SRCCOLORKEY);
        }

        if(hRet == DD_OK)
            break;
            
        if(hRet == DDERR_SURFACELOST)
        {
            SurfacePtr->Restore();
        }
        else
        {
            if(hRet != DDERR_WASSTILLDRAWING)
                return FALSE;
        }
    }

    return TRUE;
}//end Draw()
