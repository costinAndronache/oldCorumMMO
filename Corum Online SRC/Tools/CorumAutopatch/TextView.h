#pragma once

#include "./ZipArchive/ZipArchive.h"

#define MAX_WIDTH_STRING	370
#define	MAX_LINE			10000
#define MAX_VIEW_LINE		16
#define TEXT_HEIGHT			18


struct Line
{
	char*		pszStr;
	DWORD		dwLength;
};

struct TextView
{
	HWND		hwThis;
	char*		pszMemoryFile;
	DWORD		dwMemoryFileSize;
	
	Line		aLine[ MAX_LINE];
	DWORD		dwLine_Max;
	DWORD		dwViewLine_Max;

	//	Text Height
	DWORD		dwTextHeight;

	//	Text Font
	HFONT		hFt;

	//	TextView Rectengle
	RECT		rc;

	//	Width, Hieght
	POINT		po;


	int			Init( HWND hwMain, HFONT hFtTv, DWORD dwiTextHeight, DWORD dwiViewLine_Max, long lLeft, long lTop, long lWidth, long lHeight);
	int			Update( HDC hDC_BmSrc, HDC hDC_Mbm, int iScroll);
	int			Clear( HDC hDC_BmSrc, HDC hDC_Mbm);
	int			Invalidate();
};

