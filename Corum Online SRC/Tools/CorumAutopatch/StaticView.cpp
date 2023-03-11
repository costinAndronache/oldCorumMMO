#include "StaticView.h"


#define NO_CHECK


int StaticView::Init( HWND hwMain, HFONT hFtSv, long lLeft, long lTop, long lWidth, long lHeight)
{
	hwThis		= hwMain;
	//	Text Font
	hFt			= hFtSv;
	
	rc.left		= lLeft;
	rc.top		= lTop;

	po.x		= lWidth;
	po.y		= lHeight;

	rc.right	= lLeft + lWidth;	//
	rc.bottom	= lTop + lHeight;	//

	return 0;
}

int StaticView::Update( HDC hDC_BmSrc, HDC hDC_Mbm, char* pszStr)
{
	HFONT		hFtOld;
	int			iBkModeOld;
	COLORREF	clrOld;

	BitBlt( hDC_Mbm, rc.left, rc.top, po.x, po.y,
			hDC_BmSrc, rc.left, rc.top, SRCCOPY);
	
	hFtOld		= (HFONT) SelectObject( hDC_Mbm, hFt);
#ifdef NO_CHECK
	iBkModeOld	= SetBkMode( hDC_Mbm, TRANSPARENT);
	clrOld		= SetTextColor( hDC_Mbm, RGB( 255, 255, 255));
#endif
	TextOut( hDC_Mbm, rc.left, rc.top, pszStr, lstrlen(pszStr));

	SelectObject( hDC_Mbm, hFtOld);
#ifdef NO_CHECK
	SetTextColor( hDC_Mbm, clrOld);
	SetBkMode( hDC_Mbm, iBkModeOld);
#endif

	InvalidateRect( hwThis, &rc, FALSE);

	return 0;
}

int StaticView::Update_r( HDC hDC_BmSrc, HDC hDC_Mbm, char* pszStr)
{
	HFONT		hFtOld;
	int			iBkModeOld;
	COLORREF	clrOld;
	int			iLength;
	SIZE		sz;

	BitBlt( hDC_Mbm, rc.left, rc.top, po.x, po.y,
			hDC_BmSrc, rc.left, rc.top, SRCCOPY);
	
	hFtOld		= (HFONT) SelectObject( hDC_Mbm, hFt);
#ifdef NO_CHECK
	iBkModeOld	= SetBkMode( hDC_Mbm, TRANSPARENT);
	clrOld		= SetTextColor( hDC_Mbm, RGB( 255, 255, 255));
#endif
	iLength = lstrlen( pszStr);
	GetTextExtentPoint32(	hDC_Mbm,
							pszStr,
							iLength,
							&sz );

	TextOut( hDC_Mbm, rc.right - sz.cx, rc.top, pszStr, iLength);

	SelectObject( hDC_Mbm, hFtOld);
#ifdef NO_CHECK
	SetBkMode( hDC_Mbm, iBkModeOld);
	SetTextColor( hDC_Mbm, clrOld);
#endif

	InvalidateRect( hwThis, &rc, FALSE);

	return 0;
}

int StaticView::Clear( HDC hDC_BmSrc, HDC hDC_Mbm)
{
	BitBlt( hDC_Mbm, rc.left, rc.top, po.x, po.y,
			hDC_BmSrc, rc.left, rc.top, SRCCOPY);

	return 0;
}

void StaticView::Invalidate()
{
	InvalidateRect( hwThis, &rc, FALSE);
}

