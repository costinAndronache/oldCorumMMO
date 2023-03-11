#pragma once

#include "./ZipArchive/ZipArchive.h"


#define BUTTON_STATUS_NUM	4

enum BUTTON_STATUS
					{
						BS_DISABLE = 0,
						BS_NORMAL,
						BS_SELECTED,
						BS_OVERBUTTON,
						BS_NONE = 0xFFFFFFFF
					};


struct ButtonString
{
	char*			pszStr;
	int				iLength;
	int				iX;
	int				iY;
};

extern ButtonString BTNSTR[];

enum BUTTON_STRING_ID
					{
						BSI_CONNECT = 0,
						BSI_CANCEL,
						BSI_EXIT,
						BSI_START,
						BSI_NONE = 0xFFFFFFFF
					};


struct Button
{
	HWND			hWnd;
	HFONT			hFt;
	//	Compatible Memory Bitmap
	HBITMAP			hMbm;
	//	Source Bitmap(s)
	HBITMAP			ahBm[ BUTTON_STATUS_NUM];
	//	Button(Bitmap) Common Size
	DWORD			dwWidth;
	DWORD			dwHeight;
	//	Current Caption String
	ButtonString*	pBtnStr;
	//	Current Button Status
	BUTTON_STATUS	eBtnStatus;

	int		Init( HDC hDC, HWND hWndBtn, HFONT hFtBtn, HBITMAP hBm0, HBITMAP hBm1, HBITMAP hBm2, HBITMAP hBm3);
	int		Update( HDC hDC, BUTTON_STATUS bs, BUTTON_STRING_ID bsi);
};
