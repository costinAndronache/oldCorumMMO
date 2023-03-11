#include "Corumpatch.h"
#include "CorumAutopatch.h"

CorumPatch*	g_pcp = NULL;


CorumPatch::CorumPatch()
{ 
	ZeroMemory( this, sizeof(CorumPatch));
}

CorumPatch::~CorumPatch()
{
	DestroyCursor( cr.hCurMain);
	DestroyCursor( cr.hCurOverButton);
	DestroyCursor( cr.hCurScrollBar);

//	DeleteObject( btn_3.hMbm);

	DeleteObject( btn_2.hMbm);
	DeleteObject( btn_1.hMbm);
	DeleteObject( mw.hMbm);

//	DeleteObject( cr.hFtBtn);
	DeleteObject( cr.hFtMain);

	DeleteObject( cr.hBmBtnPgbar);
	DeleteObject( cr.hBmBtnOverButton);
	DeleteObject( cr.hBmBtnSelected);
	DeleteObject( cr.hBmBtnNormal);
	DeleteObject( cr.hBmBtnDisable);
	DeleteObject( cr.hBmBackGround);

	if( tv.pszMemoryFile )
	{
		delete [] tv.pszMemoryFile;
		tv.pszMemoryFile = NULL;
	}
}

int	CorumPatch::OpenDC_SET( DC_SET* pDCset)
{
	HDC		hDC;
	HDC		hMDC_Src;
	HDC		hMDC_Des;

	hDC			= GetDC( mw.hWnd);
	hMDC_Src	= CreateCompatibleDC( hDC);
	hMDC_Des	= CreateCompatibleDC( hDC);

	m_Bitmap_Old.hBm_Src = (HBITMAP) SelectObject( hMDC_Src, cr.hBmBackGround);
	m_Bitmap_Old.hBm_Des = (HBITMAP) SelectObject( hMDC_Des, mw.hMbm);

	pDCset->hDC			= hDC;
	pDCset->hMDC_Src	= hMDC_Src;
	pDCset->hMDC_Des	= hMDC_Des;

	return 0;
}

int	CorumPatch::CloseDC_SET( DC_SET* pDCset)
{
	SelectObject( pDCset->hMDC_Src, m_Bitmap_Old.hBm_Src);
	SelectObject( pDCset->hMDC_Des, m_Bitmap_Old.hBm_Des);
	DeleteDC( pDCset->hMDC_Src);
	DeleteDC( pDCset->hMDC_Des);
	ReleaseDC( mw.hWnd, pDCset->hDC);

	return 0;
}

int CorumPatch::OnInitialize( HWND hWnd)
{
	HINSTANCE	hInstance;
	DC_SET		DCset;
	HDC			hDC;
	HWND		hWndBtn;
	BITMAP		bitmap;
	LOGFONT		logfont;
	int			iReturn;


	if( IsWindow( hWnd) == NULL )
		return -1;

	hInstance = GetModuleHandle( NULL);
	if( hInstance == NULL )
		return -2;

	//	Set Window Text
	SetWindowText( hWnd, "Corum");

	/////////////////////////////////////
	//		Launch Common Resource
	/////////////////////////////////////

	//	Load Cursor Resource
	cr.hCurMain			= LoadCursor( hInstance, MAKEINTRESOURCE(IDC_CURSOR_NORMAL));
	cr.hCurOverButton	= LoadCursor( hInstance, MAKEINTRESOURCE(IDC_CURSOR_BUTTON));
	cr.hCurScrollBar	= LoadCursor( hInstance, MAKEINTRESOURCE(IDC_CURSOR_NORMAL));

	//	Load Brush
	cr.hBrBlack	= (HBRUSH) GetStockObject( BLACK_BRUSH);

	//	Create MainFont
	ZeroMemory( &logfont, sizeof(LOGFONT));
	logfont.lfHeight				= -12;
	logfont.lfWidth					= 0;
//	logfont.lfEscapement			= 0;
//	logfont.lfOrientation			= 0; 
	logfont.lfWeight				= 400;
//	logfont.lfOutPrecision			= OUT_DEVICE_PRECIS;
//	logfont.lfClipPrecision			= CLIP_DEFAULT_PRECIS;
	logfont.lfQuality				= PROOF_QUALITY;
//	logfont.lfPitchAndFamily		= FF_MODERN ;

#ifdef CHINA_LOCALIZING
	logfont.lfCharSet				= GB2312_CHARSET;
	lstrcpy( logfont.lfFaceName,	"MingLiU");
#elif defined JAPAN_LOCALIZING
	logfont.lfCharSet				= SHIFTJIS_CHARSET;
	lstrcpy( logfont.lfFaceName,	"MS PGothic");	
#elif defined TAIWAN_LOCALIZING
	logfont.lfCharSet				= CHINESEBIG5_CHARSET;
	lstrcpy( logfont.lfFaceName,	"MingLiU");
#else
	logfont.lfCharSet				= HANGUL_CHARSET;
	lstrcpy( logfont.lfFaceName,	"굴림");
#endif	

	cr.hFtMain = CreateFontIndirect( &logfont);
	if( cr.hFtMain == NULL )
		return -4;

/*
	lstrcpy( logfont.lfFaceName,	"굴림"); //"Arial Black" );
	logfont.lfWeight				= 400;
	logfont.lfHeight				= -14;
	logfont.lfWidth					= 7;
	cr.hFtBtn = CreateFontIndirect( &logfont);
*/

	//	Load Bitmap Resource
	cr.hBmBackGround	= LoadBitmap( hInstance, MAKEINTRESOURCE( IDB_BackGround));
	cr.hBmBtnDisable	= LoadBitmap( hInstance, MAKEINTRESOURCE( IDB_Disable));
	cr.hBmBtnNormal		= LoadBitmap( hInstance, MAKEINTRESOURCE( IDB_Default));
	cr.hBmBtnSelected	= LoadBitmap( hInstance, MAKEINTRESOURCE( IDB_Click));
	cr.hBmBtnOverButton	= LoadBitmap( hInstance, MAKEINTRESOURCE( IDB_Light));
	cr.hBmBtnPgbar		= LoadBitmap( hInstance, MAKEINTRESOURCE( IDB_Pgbar));

//	cr.hBmTab_on		= LoadBitmap( hInstance, MAKEINTRESOURCE( IDB_Tab_on));
//	cr.hBmTab_off		= LoadBitmap( hInstance, MAKEINTRESOURCE( IDB_Tab_off));



	/////////////////////////////////////
	//	Init MainWindow

	//	Window Handle
	mw.hWnd			= hWnd;
	//	Bitmap Resource
	mw.hBm			= cr.hBmBackGround;
	//	Create Memory Bitmap
	ZeroMemory( &bitmap, sizeof( BITMAP)); 
	iReturn = GetObject( cr.hBmBackGround, sizeof( BITMAP), &bitmap);
	if( iReturn != sizeof( BITMAP) )
		return -5;

	hDC = GetDC( mw.hWnd);
	mw.hMbm = CreateCompatibleBitmap( hDC, bitmap.bmWidth, bitmap.bmHeight);
	if( mw.hMbm == NULL )
		return -6;
	ReleaseDC( mw.hWnd, hDC);

	//	Create MainWindow MemoryBitmap
	OpenDC_SET( &DCset);
	BitBlt( DCset.hMDC_Des, 0, 0, bitmap.bmWidth, bitmap.bmHeight,
			DCset.hMDC_Src, 0, 0, SRCCOPY );
	CloseDC_SET( &DCset);
	//	Window Movement
	//	mw.poMouse			= auto later;
	//	mw.poWindow			= auto later;
	//	mw.fWindowMoving	= FALSE;



	/////////////////////////////////////
	//	Init Button
	hDC = GetDC( mw.hWnd);

	//	Init Button 1
	hWndBtn = GetDlgItem( hWnd, IDC_BTN_1);
	btn_1.Init( hDC, hWndBtn, cr.hFtMain, cr.hBmBtnDisable, cr.hBmBtnNormal, cr.hBmBtnSelected, cr.hBmBtnOverButton);
	btn_1.Update( hDC, BS_NONE, BSI_CONNECT);

	//	Init Button 2
	hWndBtn = GetDlgItem( hWnd, IDC_BTN_2);
	btn_2.Init( hDC, hWndBtn, cr.hFtMain, cr.hBmBtnDisable, cr.hBmBtnNormal, cr.hBmBtnSelected, cr.hBmBtnOverButton);
	btn_2.Update( hDC, BS_NORMAL, BSI_CANCEL);

		//	Init Button 3
//		hWndBtn = GetDlgItem( hWnd, IDC_BTN_3);
//		btn_3.Init( hDC, hWndBtn, cr.hFtMain, cr.hBmTab_on, cr.hBmTab_on, cr.hBmTab_on, cr.hBmTab_off);
//		btn_3.Update( hDC, BS_NORMAL, BSI_TAB_ON);


	/////////////////////////////////////
	//	Init TextView
	tv.Init( mw.hWnd, cr.hFtMain, TEXT_HEIGHT, MAX_VIEW_LINE, 240, 68, MAX_WIDTH_STRING, 288);
				//	Init_TextView( hDC, "Notice.txt");		<-	

	ReleaseDC( mw.hWnd, hDC);


	/////////////////////////////////////
	//	Init TextScroll
	ts.hWnd		= GetDlgItem( hWnd, IDC_SCROLLBAR);
	ts.iLine	= 0;


	/////////////////////////////////////
	//	Init ProgressBar
	pv.Init( mw.hWnd, cr.hBmBtnPgbar, 9, 412, 10, 16, 50); 


	/////////////////////////////////////
	//	Init StaticView 

	//	공지 페이지 정보.
	sv_page.Init( mw.hWnd, cr.hFtMain, 555, 39, 66, 16);

	//	클라이언트				- Version Info.
	sv_ClientVer.Init( mw.hWnd, cr.hFtMain, 324, 385, 186,16);

	//	프로그램 스테이터스		- [ AP ]
	sv_Status.Init( mw.hWnd, cr.hFtMain, 20, 385, 200, 16);
	
	//	다운로드 파일사이즈.	- FileSize
	sv_FileC.Init( mw.hWnd, cr.hFtMain, 7, 432, 45, 16);
	sv_FileC2.Init( mw.hWnd, cr.hFtMain, 100, 432, 250, 16);

	//	다운로드 전송속도.		- Speed
//	sv_SpeedC.Init( mw.hWnd, cr.hFtMain, 6, 448, 73, 16);
//	sv_SpeedC2.Init( mw.hWnd, cr.hFtMain, 100, 448, 150, 16);

	//	다운로드 남은시간.		- Time
//	sv_TimeC.Init( mw.hWnd, cr.hFtMain, 6, 464, 73, 16);
//	sv_TimeC2.Init( mw.hWnd, cr.hFtMain, 100, 464, 250, 16);

	//	에러 메세지.
	sv_ErrInfo.Init( mw.hWnd, cr.hFtMain, 7, 464, 300, 16);

	/////////////////////////////////////

	//	압축파일 갯수			- Extract File
	sv_ExFileC.Init( mw.hWnd, cr.hFtMain, 20, 435, 73, 16);
	sv_ExFileC2.Init( mw.hWnd, cr.hFtMain, 100, 435, 200, 16);

	/////////////////////////////////////
	//	Init Cursor
	cr.hCurMainOld			= (HCURSOR) SetClassLong( mw.hWnd, GCL_HCURSOR,	(LONG) cr.hCurMain);
	cr.hCurOverButtonOld	= (HCURSOR) SetClassLong( btn_1.hWnd, GCL_HCURSOR,	(LONG) cr.hCurOverButton);
	cr.hCurScrollBarOld		= (HCURSOR) SetClassLong( ts.hWnd, GCL_HCURSOR,	(LONG) cr.hCurScrollBar);


	/////////////////////////////////////
	//	Output
	/////////////////////////////////////
	char Buf_sv[MAX_PATH];

	OpenDC_SET( &DCset);

	//	Status		프로그램 스테이터스
	UpdateAP( &DCset, AP);

	//	클라이언트 Version Info.
	wsprintf( Buf_sv,"Client Version [ %s ]", szVersion);
	sv_ClientVer.Update( DCset.hMDC_Src, DCset.hMDC_Des, Buf_sv);

	CloseDC_SET( &DCset);
	return 0;
}

BOOL CorumPatch::Init_TextScroll()
{
	SCROLLINFO si;

	if( tv.dwLine_Max <= tv.dwViewLine_Max )
		return FALSE;

	ts.iLineMax = (tv.dwLine_Max - tv.dwViewLine_Max);

	//	Init TextScroll
	ZeroMemory( &si, sizeof(SCROLLINFO));

	si.cbSize		= sizeof(SCROLLINFO);
	si.fMask		= SIF_ALL;
	si.nMin			= 0; 
	si.nMax			= tv.dwLine_Max - 1;
	si.nPage		= tv.dwViewLine_Max;
	si.nPos			= 0; 
	si.nTrackPos	= 0; 

	SetScrollInfo( ts.hWnd, SB_CTL, &si, false);
	ShowWindow( ts.hWnd, SW_SHOW);

	return TRUE;
}

int CorumPatch::OnPaint()
{
	HDC				hDC;
	HDC				hMDC;
	PAINTSTRUCT		ps;
	HBITMAP			hBmOld;

	hDC		= BeginPaint( mw.hWnd, &ps);
	hMDC	= CreateCompatibleDC( hDC);
	hBmOld	= (HBITMAP) SelectObject( hMDC, mw.hMbm);
	
	BitBlt( hDC, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top,
			hMDC, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);
	
	SelectObject( hMDC, hBmOld);
	DeleteDC( hMDC);
	
	EndPaint( mw.hWnd, &ps);

	return 0;
}

BOOL CorumPatch::OnCtlColorScrollBar( WPARAM wParam, LPARAM lParam)
{
	return ((BOOL) cr.hBrBlack);
}

int CorumPatch::OnDrawItem( WPARAM wParam, LPARAM lParam)
{
	DRAWITEMSTRUCT* pDIS;
	HDC				hDC;
	HDC				hMDC;
	Button*			pBtn;
	HBITMAP			hBmOld;

	hDC		= GetDC( mw.hWnd);
	pDIS	= (DRAWITEMSTRUCT*) lParam;


	//	Button Ctrl_ID 판별.
	switch( wParam )
	{
	case IDC_BTN_1:
			pBtn = &btn_1;
			break;
	case IDC_BTN_2:
			pBtn = &btn_2;
			break;

//		case IDC_BTN_3:
//				pBtn = &btn_3;
//				break;

	default:
		_ASSERTE( !"No Matching Ctrl_ID in DrawItem.");
		_asm int 3
		break;
	}

	//	버튼 상태 결정하고 버튼 메모리 비트맵을 다시 그린다.
	//	Is Disabled
	if( pDIS->itemState & ODS_DISABLED )
	{
		pBtn->Update( hDC, BS_DISABLE, BSI_NONE);
	} 
	else if( pDIS->itemState & ODS_SELECTED )
	{
		pBtn->Update( hDC, BS_SELECTED, BSI_NONE);
	}
	else if( pBtn->eBtnStatus == BS_DISABLE )
	{
		pBtn->Update( hDC, BS_NORMAL, BSI_NONE);
	}

	//	비트맵 이미지 출력.
	hMDC	= CreateCompatibleDC( pDIS->hDC);
	hBmOld	= (HBITMAP) SelectObject( hMDC, pBtn->hMbm);

	BitBlt( pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.right - pDIS->rcItem.left, pDIS->rcItem.bottom - pDIS->rcItem.top,
			hMDC, pDIS->rcItem.left, pDIS->rcItem.top, SRCCOPY);
	

	SelectObject( hMDC, hBmOld);
	DeleteDC( hMDC);
	ReleaseDC( mw.hWnd, hDC);

	return 0;
}

int CorumPatch::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	RECT	rcWindow;

	mw.poMouse.x = LOWORD(lParam);
	mw.poMouse.y = HIWORD(lParam);

	GetWindowRect( mw.hWnd, &rcWindow);
	mw.poWindow.x = rcWindow.left;
	mw.poWindow.y = rcWindow.top;


	SetCapture( mw.hWnd);
	mw.fWindowMoving = TRUE;

	return 0;
}

int	CorumPatch::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	ReleaseCapture();
	mw.fWindowMoving = FALSE;

	return 0;
}

int	CorumPatch::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	long lx;
	long ly;

	if( mw.fWindowMoving == FALSE )
		return 0;
	
	lx = (short) LOWORD(lParam);
	ly = (short) HIWORD(lParam);

	mw.poWindow.x += (lx - mw.poMouse.x);
	mw.poWindow.y += (ly - mw.poMouse.y);

	SetWindowPos( mw.hWnd, HWND_TOP, mw.poWindow.x, mw.poWindow.y, NULL, NULL, SWP_NOSIZE);
	return 0;
}

int CorumPatch::On_Btn_MouseMove( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	HDC			hDC;
	HWND		hWndCaptureOld;
	Button*		pBtn;
	int			iCtrl_ID;
	long		lx;
	long		ly;
	RECT		rcButton;
	BOOL		fOverButton;

	BOOL		bR;

	
	hDC = GetDC( mw.hWnd);

	iCtrl_ID = GetDlgCtrlID( hWnd);

	switch( iCtrl_ID )
	{
	case IDC_BTN_1:
			pBtn = &btn_1;
			break;

	case IDC_BTN_2:
			pBtn = &btn_2;
			break;

//		case IDC_BTN_3:
//				pBtn = &btn_3;
//				break;

	default:
		_ASSERTE( !"No Matching Ctrl_ID in On_Btn_MouseMove.");
		_asm int 3
	}

	lx = (short) LOWORD(lParam);
	ly = (short) HIWORD(lParam);

	GetClientRect( hWnd, &rcButton);
	fOverButton = !(lx < 0  ||  lx > rcButton.right  ||  ly < 0  ||  ly > rcButton.bottom);

	if( fOverButton )
	{
		if( !(wParam & MK_LBUTTON) )
			pBtn->Update( hDC, BS_OVERBUTTON, BSI_NONE);

		if( GetCapture() == NULL )
			hWndCaptureOld = SetCapture( hWnd);
	}
	else if( !(wParam & MK_LBUTTON) )
	{
		pBtn->Update( hDC, BS_NORMAL, BSI_NONE);

		if( GetCapture() != NULL )
			bR = ReleaseCapture();
	} 
	else
		pBtn->Update( hDC, BS_OVERBUTTON, BSI_NONE);

	InvalidateRect( hWnd, NULL, false);
	ReleaseDC( mw.hWnd, hDC);

	return 0;
}

int	CorumPatch::Init_TextView( HDC hDC, char* pszFileName)
{
	HANDLE		hFile;
	DWORD		dwBytesRead;

	HFONT		hFtOld;

	char*		pBuffer;
	Line*		pLine;
	int			iLine;

	BOOL		bWordBreak;
	BOOL		bEOF;

	DWORD		dwOffset_Read;
	DWORD		dwOffset_End;

	BOOL		bR;
	int			iReturn;


	//	Open File
	hFile = CreateFile(	pszFileName,
						GENERIC_READ,
						0,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL );

	if( hFile == INVALID_HANDLE_VALUE )
	{
		iReturn = -1;
		goto lb_Fail;
	}

	tv.dwMemoryFileSize = GetFileSize( hFile, NULL);
	if( tv.dwMemoryFileSize == INVALID_FILE_SIZE )
	{
		iReturn = -2;
		goto lb_Fail;
	}

	if( tv.pszMemoryFile )
		delete [] tv.pszMemoryFile;
	//	Alloc MemoryFile
	tv.pszMemoryFile = new char[ tv.dwMemoryFileSize + 2];		//	+1 Extra EOF Marking
	
	bR = ReadFile( hFile, tv.pszMemoryFile, tv.dwMemoryFileSize, &dwBytesRead, NULL);
	if( bR == 0 )
	{
		iReturn = -3;
		goto lb_Fail;
	}

	bR = CloseHandle( hFile);
	if( bR == 0 )
	{
		iReturn = -4;
		goto lb_Fail;
	}

	tv.pszMemoryFile[ tv.dwMemoryFileSize]		= 13;
	tv.pszMemoryFile[ tv.dwMemoryFileSize + 1]	= NULL;

	//	Set Font
	hFtOld = (HFONT) SelectObject( hDC, tv.hFt);

	///////////////////////////////////
	//	parsing - Document
	///////////////////////////////////

	dwOffset_Read	= 0;
	iLine			= 0;
	bWordBreak		= FALSE;
	bEOF			= FALSE;

	while( TRUE)
	{
		pLine	= tv.aLine + iLine;
		pBuffer	= tv.pszMemoryFile + dwOffset_Read;

		if( bWordBreak )
		{	
			bWordBreak = FALSE;
		}
		else
		{
			if( bEOF == TRUE )
			{
				iLine--;
				break;
			}

			dwOffset_End = 0;

			while( pBuffer[ dwOffset_End] != 13 )
			{
				dwOffset_End++;
			}

			if( pBuffer[ dwOffset_End + 1] == NULL)
			{
				if( dwOffset_End == 0 )
					break;
				else
					bEOF = TRUE;
			}
		}

		pLine->pszStr = pBuffer;

		if( dwOffset_End == 0 )
		{
			pLine->pszStr	= NULL;
			pLine->dwLength = 0;
			iLine++;
			dwOffset_Read += 2;
			continue;
		}

		//	이젠 정말 길이를 알아야 할때가 왓다.
		int			iFit;
		SIZE		Size;
		
		bR = GetTextExtentExPoint( 
					hDC,				// handle to DC
					pBuffer,			// character string
					dwOffset_End,		// dwTotalReadLength - dwOffset_Read,		// number of characters
					MAX_WIDTH_STRING,	// maximum width of formatted string
					&iFit,				// maximum number of characters
					NULL,				// array of partial string widths
					&Size				// string dimensions
					);

		if( iFit >= (int) dwOffset_End )		//	10이 들어가는지 확인. : 들어가지않으면 -> [ iFit > dwOffset ]
		{
			//	다행히 한줄에 들어간다.
			pLine->dwLength = dwOffset_End;
			iLine++;
			dwOffset_Read += (dwOffset_End + 2);

			continue;
		}

		//	WORD BREAK 상황
		int iCount;

		bWordBreak	= TRUE;
		iCount		= iFit - 1;		//	옵셋값으로 바꾼다.

		while( iCount >= 0 )
		{
			if( pBuffer[ iCount] == 32 )
				break;
		
			iCount--;
		}

		if( iCount < 0 )
		{
			//	공백없는 무지긴 한 단어.
			pLine->dwLength = (DWORD) iFit;
			iLine++;
			dwOffset_Read += ((DWORD) iFit);
			dwOffset_End -= ((DWORD) iFit);
		}
		else
		{
			//	공백이 잇는것 확인.
			dwOffset_Read	+= (iCount + 1);
			dwOffset_End	-= (iCount + 1);

			//	불필요한 공백을 제거하여 라인길이를 줄인다.
			while( (iCount > 0)  &&  (pBuffer[ iCount - 1] == 32) )
			{
				iCount--;
			}

			if( iCount > 0 )
			{
				//	길이를 줄여줌.
				pLine->dwLength = (DWORD) iCount;
				iLine++;
			}
//			else
//			{
				//	공백말고는 아무것도 없는 라인이다.
				//	라인 추가 무시하고 ReadOffset만 증가.
//			}
		}
		continue;
	}

	//	정리 
	tv.dwLine_Max = iLine + 1;

	SelectObject( hDC, hFtOld);


	return 0;

lb_Fail:

	if( tv.pszMemoryFile )
	{
		delete [] tv.pszMemoryFile;
		tv.pszMemoryFile = NULL;
	}

	return iReturn;
}


int	CorumPatch::OnVscroll( WPARAM wParam, LPARAM lParam)
{
	int		iLine;
	int		iIncrement;
	
	iLine		= ts.iLine;
	iIncrement	= 0;

	switch( LOWORD( wParam) )
	{
	case SB_BOTTOM:
	case SB_ENDSCROLL:
		break;

	case SB_LINEDOWN:
			iIncrement++;
		break;

	case SB_LINEUP:
			iIncrement--;
		break;

	case SB_PAGEDOWN:
			iIncrement += tv.dwViewLine_Max;
		break;

	case SB_PAGEUP:
			iIncrement -= tv.dwViewLine_Max;;
		break;

//	case SB_THUMBPOSITION:
//		break;

	case SB_THUMBTRACK:
			iIncrement = HIWORD( wParam);
			iIncrement -= iLine;
		break;
	default:
		break;
	}

	iLine += iIncrement;

	if( iLine < 0 )
		iLine = 0;
	if( iLine > ((int)tv.dwLine_Max - (int)tv.dwViewLine_Max) )
		iLine = ((int)tv.dwLine_Max - (int)tv.dwViewLine_Max);

	if( iLine == ts.iLine )
		return 0;

	ts.iLine = iLine;

	//	Output TextView
	DC_SET		DCset;
	SCROLLINFO	si;

	OpenDC_SET( &DCset);

	ZeroMemory( &si, sizeof(SCROLLINFO));
	si.cbSize		= sizeof(SCROLLINFO);
	si.fMask		= SIF_POS;
	si.nPos			= ts.iLine;
	SetScrollInfo( ts.hWnd, SB_CTL, &si, true);

	tv.Update( DCset.hMDC_Src, DCset.hMDC_Des, ts.iLine);
	g_pcp->tv.Invalidate();

	//	Output page Info.
	char Buf_sv[MAX_PATH];
	wsprintf( Buf_sv,"%d  / %d", ts.iLine + 1, tv.dwLine_Max - tv.dwViewLine_Max + 1);
	sv_page.Update_r( DCset.hMDC_Src, DCset.hMDC_Des, Buf_sv);

	//

	CloseDC_SET( &DCset);

	return 0;
}

int CorumPatch::OnBtn_1()
{
	DC_SET		DCset;
	BOOL		bR;
	
	if( AP == AP_INITIALIZED )
	{
		OpenDC_SET( &DCset);

		AP = AP_CONNECT;

		EnableWindow( btn_1.hWnd, false);

		sv_Status.Update( DCset.hMDC_Src, DCset.hMDC_Des, APS[ AP] );
		sv_Status.Invalidate();

		//	btn_ ( 2 )		EXIT - > CANCEL
		btn_2.Update( DCset.hDC, BS_NONE, BSI_CANCEL);

		SetFocus( ts.hWnd);

		CloseDC_SET( &DCset);
	}

	if( AP == AP_COMPLETED )
	{
		//	최신 버전이다.
		//	코룸 구동 파일을 실행한다.

 		bR = RunCorum();
		if( bR == FALSE )
		{
			//	코룸 구동 실패.
			MessageBox_Info("Fail to Run - ExecuteFile.");
		}

		return -3;
	}

	return 0;
}

int CorumPatch::OnBtn_2()
{
	int	iR;

	if( AP == AP_INITIALIZED )
		return -1;

	if( AP == AP_COMPLETED )
		return -2;

	SetFocus( ts.hWnd);

	AP = AP_INITIALIZED;
	iR = g_pFtp->Shutdown( g_hSession);
	_ASSERTE( iR == 0 );

	return 0;
}

