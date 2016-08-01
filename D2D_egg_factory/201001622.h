#pragma once

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#define _WIN32_DCOM
#define _CRT_SECURE_NO_DEPRECATE

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <dwmapi.h>
#include <stdio.h>
#include <shellapi.h>
#include <mmsystem.h>
#include <conio.h>
#include <strsafe.h>
#include "Animation.h"
//#include "SDKwavefile.h"
#include <dsound.h>
#include "SoundManager.h"


template<class Interface>
inline void
SafeRelease(
    Interface **ppInterfaceToRelease
    )
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();

        (*ppInterfaceToRelease) = NULL;
    }
}

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


class DemoApp
{
public:
    DemoApp();
    ~DemoApp();

    HRESULT Initialize();

    void RunMessageLoop();

private:
    HRESULT CreateDeviceIndependentResources();
    HRESULT CreateDeviceResources();
    void DiscardDeviceResources();

	HRESULT CreateGridPatternBrush(ID2D1RenderTarget *pRenderTarget, ID2D1BitmapBrush **ppBitmapBrush);
	HRESULT LoadResourceBitmap(ID2D1RenderTarget *pRenderTarget, IWICImagingFactory *pIWICFactory, PCWSTR resourceName, PCWSTR resourceType, ID2D1Bitmap **ppBitmap);
    HRESULT LoadBitmapFromFile(ID2D1RenderTarget *pRenderTarget, IWICImagingFactory *pIWICFactory, PCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap **ppBitmap);

    HRESULT OnRender();

	void OnKeyDown(
        SHORT vkey
        );

    void OnResize(
        UINT width,
        UINT height
        );

    static LRESULT CALLBACK WndProc(
        HWND hWnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
        );

private:
    HWND m_hwnd;
    ID2D1Factory *m_pD2DFactory;


    ID2D1HwndRenderTarget *m_pRT;
	IWICImagingFactory* m_pWICFactory;

  
	ID2D1PathGeometry* m_pPathGeometry;
	ID2D1PathGeometry* m_wheelPath;
	ID2D1PathGeometry* m_flyPath;
	

	ID2D1EllipseGeometry* m_pEgg1;
	ID2D1EllipseGeometry* m_pEgg2;
	ID2D1EllipseGeometry* m_pEgg3;
	ID2D1EllipseGeometry* m_pEgg4;
	ID2D1EllipseGeometry* m_pEgg5;
	ID2D1EllipseGeometry* m_pMovingEgg;
	ID2D1EllipseGeometry* m_blackHole;
	

	ID2D1EllipseGeometry* m_pChain;
	ID2D1EllipseGeometry* m_pChain2;	
	ID2D1EllipseGeometry* m_pWheel;

    ID2D1SolidColorBrush *m_pBrush;
    ID2D1SolidColorBrush *m_pBlackBrush;
	ID2D1SolidColorBrush *m_pRedBrush;

	ID2D1BitmapBrush *m_pMinionBrush;
	ID2D1BitmapBrush *m_pJoodyBrush;
	ID2D1BitmapBrush *m_pElsaBrush;
	ID2D1BitmapBrush *m_pMaxBrush;
	ID2D1BitmapBrush *m_pBlackHoleBrush;

	ID2D1RadialGradientBrush *m_pRadialGradientBrush;
	ID2D1LinearGradientBrush *m_pLinearGradientBrush;
	///////////////////////////////////
	ID2D1PathGeometry *m_pWingGeometry[4];
	ID2D1PathGeometry *m_pMiddleWingGeometry;
	ID2D1PathGeometry *m_pUpWingGeometry;
	ID2D1PathGeometry *m_pDownWingGeometry;

	ID2D1EllipseGeometry *m_pRightEyeGeometry;
	ID2D1EllipseGeometry *m_pLeftEyeGeometry;
	ID2D1EllipseGeometry *m_pFlyGeometry;
	///////////////////////////////////////
	ID2D1Bitmap* m_pBitmap;
	ID2D1Bitmap *m_pMinionBitmap;
	ID2D1Bitmap *m_pJoodyBitmap;
	ID2D1Bitmap *m_pElsaBitmap;
	ID2D1Bitmap *m_pMaxBitmap;
	ID2D1Bitmap *m_pBlackHoleBitmap;


	ID2D1RectangleGeometry *m_pRectGeo;
	

	IDWriteFactory *m_pDWriteFactory;
	IDWriteTextFormat *m_pTextFormat;

    EaseOutExponentialAnimation<float> m_Animation;
	LinearAnimation<float> m_linearAnimation;

    DWM_TIMING_INFO m_DwmTimingInfo;



};


