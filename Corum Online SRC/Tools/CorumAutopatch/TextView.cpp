#include "TextView.h"


int TextView::Init( HWND hwMain, HFONT hFtTv, DWORD dwiTextHeight, DWORD dwiViewLine_Max, long lLeft, long lTop, long lWidth, long lHeight)
{
	hwThis				= hwMain;
	hFt					= hFtTv;
	dwViewLine_Max		= dwiViewLine_Max;
	dwTextHeight		= dwiTextHeight;
	pszMemoryFile		= NULL;
	
	rc.left			= lLeft;
	rc.top			= lTop;
	
	//	Width, Height
	po.x			= lWidth;
	po.y			= lHeight;

	rc.right		= lLeft + lWidth;
	rc.bottom		= lTop + lHeight;

	return 0;
}

int TextView::Update( HDC hDC_BmSrc, HDC hDC_Mbm, int iLine)
{
	HFONT		hFtOld;
	int			iBkModeOld;
	COLORREF	clrOld;
	Line*		pLine;
	int			iCount;
	int			iCountMax;
	int			iy;


	iCountMax = ((int) dwLine_Max) - iLine;
	//	적어도 한줄은 표시가능하다.
	if( iCountMax < 1 )
		return 0;

	BitBlt( hDC_Mbm, rc.left, rc.top, po.x, po.y,
			hDC_BmSrc, rc.left, rc.top, SRCCOPY);

	hFtOld		= (HFONT) SelectObject( hDC_Mbm, hFt);
	iBkModeOld	= SetBkMode( hDC_Mbm, TRANSPARENT);
	clrOld		= SetTextColor( hDC_Mbm, RGB( 255, 255, 255));
	
	//	출력라인 한계.
	if( iCountMax > ((int) dwViewLine_Max) )
		iCountMax = (int) dwViewLine_Max;

	pLine = aLine + iLine;
	iCount	= 0;

	while( iCount < iCountMax )
	{
		iy = rc.top + iCount * dwTextHeight;
		TextOut( hDC_Mbm, rc.left, iy, pLine->pszStr, pLine->dwLength);
		pLine++;
		iCount++;
	}

	SelectObject( hDC_Mbm, hFtOld);
	SetBkMode( hDC_Mbm, iBkModeOld);
	SetTextColor( hDC_Mbm, clrOld);

	return 0;
}

int TextView::Clear( HDC hDC_BmSrc, HDC hDC_Mbm)
{
	BitBlt( hDC_Mbm, rc.left, rc.top, po.x, po.y,
			hDC_BmSrc, rc.left, rc.top, SRCCOPY);

	return 0;
}

int	TextView::Invalidate()
{
	InvalidateRect( hwThis, &rc, false);
	return 0;
}