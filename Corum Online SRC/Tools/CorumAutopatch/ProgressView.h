#pragma once

#include "./ZipArchive/ZipArchive.h"
//	#define MAX_STRING_FORMAT_LENGTH 20


struct ProgressView
{
	HWND		hwnd;
	HBITMAP		hBm;
	//	ProgressView Rectengle
	RECT		rc;
	//	Width, Height
	POINT		po;

	long		lBar;
	long		lBar_Max;
	long		lBarOld;

	long		lMax;


	int			Init( HWND hwnd, HBITMAP hBmPgBar, long lLeft, long lTop, long lWidth, long lHeight, long lPvBar_Max);
	int			Init_Range( long lPv_Max);
	int			Update( int iProgress);
	int			UpdateBar( HDC hDC, HDC hDC_Mbm);
	int			Clear( HDC hDC_BmSrc, HDC hDC_Mbm);
};