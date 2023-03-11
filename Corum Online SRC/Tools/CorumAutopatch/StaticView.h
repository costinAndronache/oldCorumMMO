#pragma once

#include "./ZipArchive/ZipArchive.h"
//#include "ConstantDefine.h"


//	#define MAX_STRING_FORMAT_LENGTH 20


struct StaticView
{
	HWND		hwThis;
	//	Text Font
	HFONT		hFt;
	//	StaticView Rectengle
	RECT		rc;

	//	Width, Hieght
	POINT		po;

	int			Init( HWND hwMain, HFONT hFtSv, long lLeft, long lTop, long lWidth, long lHeight);
	int			Update( HDC hDC_BmSrc, HDC hDC_Mbm, char* pszStr);
	int			Update_r( HDC hDC_BmSrc, HDC hDC_Mbm, char* pszStr);
	int			Clear( HDC hDC_BmSrc, HDC hDC_Mbm);
	void		Invalidate();
};