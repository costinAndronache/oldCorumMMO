#include "Button.h"

ButtonString BTNSTR[] =	{
#ifdef CHINA_LOCALIZING
							{ _T("连接"), 4, 48, 18 },
							{ _T("取消"), 4, 48, 18 },
							{ _T("退出"), 4, 48, 18 },
							{ _T("开始"), 4, 48, 18 },
#else
							{ "Connect"	, 7, 36, 20 },
							{ "Cancel"	, 6, 40, 20 },
							{ "Exit"	, 4, 48, 20 },
							{ "Start"	, 5, 48, 20 },
#endif
							{ "Tab_On"	, 6,  5,  5 }
						};

int Button::Init( HDC hDC, HWND hWndBtn, HFONT hFtBtn, HBITMAP hBm0, HBITMAP hBm1, HBITMAP hBm2, HBITMAP hBm3)
{
	BITMAP		bitmap;
	int			iReturn;

	hWnd		= hWndBtn;
	hFt			= hFtBtn;

	ahBm[ BS_DISABLE]		= hBm0;
	ahBm[ BS_NORMAL]		= hBm1;
	ahBm[ BS_SELECTED]		= hBm2;
	ahBm[ BS_OVERBUTTON]	= hBm3;

	ZeroMemory( &bitmap, sizeof( BITMAP));
	iReturn = GetObject( hBm0, sizeof( BITMAP), &bitmap);
	if( iReturn != sizeof( BITMAP) )
		return -1;

	dwWidth		= bitmap.bmWidth;
	dwHeight	= bitmap.bmHeight;

	//	Compatible Memory Bitmap
	hMbm = CreateCompatibleBitmap( hDC, bitmap.bmWidth, bitmap.bmHeight);

	return 0;
}

int Button::Update( HDC hDC, BUTTON_STATUS bs, BUTTON_STRING_ID bsi)
{
	HDC				hMDC_Src;
	HDC				hMDC_Des;

	HBITMAP			hBmSrc_Old;
	HBITMAP			hBmDes_Old;

	HFONT			hFtOld;
	int				iBkOld;
	COLORREF		clrOld;

	BOOL			bChange;


	bChange = NULL;

	if( bs != BS_NONE  &&  eBtnStatus != bs )
	{
		eBtnStatus = bs;
		bChange++;
	}

	if( bsi != BSI_NONE  &&  pBtnStr != (BTNSTR + bsi) )
	{
		pBtnStr = BTNSTR + bsi;
		bChange++;
	}

	if( bChange == FALSE )
		return -1;

	hMDC_Src = CreateCompatibleDC( hDC);
	hMDC_Des = CreateCompatibleDC( hDC);

	hBmSrc_Old = (HBITMAP) SelectObject( hMDC_Src, ahBm[ eBtnStatus]);
	hBmDes_Old = (HBITMAP) SelectObject( hMDC_Des, hMbm);
	
	BitBlt( hMDC_Des, 0, 0, dwWidth, dwHeight, hMDC_Src, 0, 0, SRCCOPY );

	hFtOld = (HFONT) SelectObject( hMDC_Des, hFt);
	iBkOld = SetBkMode( hMDC_Des, TRANSPARENT);

	if( bs == BS_SELECTED )
		clrOld = SetTextColor( hMDC_Des, RGB( 10,10,255));
	else
		clrOld = SetTextColor( hMDC_Des, RGB( 255,255,255));

	TextOut( hMDC_Des, pBtnStr->iX, pBtnStr->iY, pBtnStr->pszStr, pBtnStr->iLength );
	
	SetBkMode( hMDC_Des, iBkOld);
	SetTextColor( hMDC_Des, clrOld);
	SelectObject( hMDC_Des, hFtOld);
	SelectObject( hMDC_Des, hBmDes_Old);
	SelectObject( hMDC_Src, hBmSrc_Old);
	DeleteDC( hMDC_Des);
	DeleteDC( hMDC_Src);

	InvalidateRect( hWnd, NULL, false);

	return 0;
}