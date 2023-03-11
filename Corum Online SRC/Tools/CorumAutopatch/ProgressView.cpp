#include "ProgressView.h"


int ProgressView::Init( HWND hwnd_in, HBITMAP hBmPgBar, long lLeft, long lTop, long lWidth, long lHeight, long lPvBar_Max)
{
	hwnd			= hwnd_in;
	hBm				= hBmPgBar;
	lBar			= 0;
	lBar_Max		= lPvBar_Max;
	lBarOld			= 0;

	po.x			= lWidth;
	po.y			= lHeight;

	rc.left			= lLeft;
	rc.top			= lTop;
	rc.right		= lLeft + lWidth * lPvBar_Max;
	rc.bottom		= lTop + lHeight;

	lMax			= 0;


	return 0;
}

int ProgressView::Init_Range( long lPv_Max)
{
	lBar		= 0;
	lBarOld		= 0;
	lMax		= lPv_Max;

	return 0;
}

int ProgressView::Update(int iProgress)
{
//	iCur = iProgress;
	lBar = (int) ((float) lBar_Max * ((float) iProgress / (float) lMax));

	if( lBar > lBarOld )
		return 1;

	return 0;
}

int ProgressView::UpdateBar( HDC hDC, HDC hDC_Mbm)
{
	HDC			hMDC_Src;
	HBITMAP		hBmOld_Src;
	
//	int			iCount;
//	int			iCountMax;

//	iCount		= lBarOld;
//	iCountMax	= lBar - lBarOld;

	hMDC_Src	= CreateCompatibleDC( hDC);
	hBmOld_Src	= (HBITMAP) SelectObject( hMDC_Src, hBm);

	while( lBarOld < lBar )
	{
		BitBlt( hDC_Mbm, rc.left + po.x * lBarOld, rc.top, po.x, po.y,
				hMDC_Src, 0, 0, SRCCOPY);
		lBarOld++;
	}
//	lBarOld = iCount;

	SelectObject( hMDC_Src, hBmOld_Src);
	DeleteDC( hMDC_Src);

	InvalidateRect( hwnd, &rc, false);
	
	return 0;
}

int ProgressView::Clear( HDC hDC_BmSrc, HDC hDC_Mbm)
{
	BitBlt( hDC_Mbm, rc.left, rc.top, po.x * lBarOld, po.y,
			hDC_BmSrc, rc.left, rc.top, SRCCOPY);

	return 0;
}