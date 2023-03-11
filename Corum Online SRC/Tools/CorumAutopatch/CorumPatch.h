#pragma once


#include "./ZipArchive/ZipArchive.h"
#include "resource.h"


#include "ConstantDefine.h"

#include "Button.h"
#include "TextView.h"
#include "TextScroll.h"
#include "StaticView.h"
#include "ProgressView.h"


struct DC_SET
{
	HDC		hDC;
	HDC		hMDC_Src;
	HDC		hMDC_Des;
};

struct BITMAP_OLD
{
	HBITMAP	hBm_Src;
	HBITMAP	hBm_Des;
};

struct CommonResource
{
	//	Cursor Resource Handle
	HCURSOR			hCurMain;
	HCURSOR			hCurOverButton;
	HCURSOR			hCurScrollBar;

	//	Cursor OLD
	HCURSOR			hCurMainOld;
	HCURSOR			hCurOverButtonOld;
	HCURSOR			hCurScrollBarOld;

	//	Brush
	HBRUSH			hBrBlack;

	//	Font
	HFONT			hFtMain;
	HFONT			hFtBtn;
	//	BitMap Resource Handle
	HBITMAP			hBmBackGround;

	HBITMAP			hBmBtnDisable;
	HBITMAP			hBmBtnNormal;
	HBITMAP			hBmBtnSelected;
	HBITMAP			hBmBtnOverButton;
	HBITMAP			hBmBtnPgbar;		//	Progress - bar

//	HBITMAP			hBmTab_on;
//	HBITMAP			hBmTab_off;


};

struct MainWindow
{
	//	Window Handle
	HWND			hWnd;

	//	Bitmap Resource
	HBITMAP			hBm;
	HBITMAP			hMbm;
 
	//	Window Movement
	POINT			poMouse;
	POINT			poWindow;
	BOOL			fWindowMoving;
};


class CorumPatch
{

public:
	/////////////////////////////////////
	//				GDI
	/////////////////////////////////////
	
	BITMAP_OLD		m_Bitmap_Old;

	CommonResource	cr;
	MainWindow		mw;
	Button			btn_1;
	Button			btn_2;


	TextView		tv;
	TextScroll		ts;

	StaticView		sv_page;
	StaticView		sv_Status;
	StaticView		sv_ClientVer;

	StaticView		sv_FileC;
	StaticView		sv_FileC2;

//	StaticView		sv_SpeedC;
//	StaticView		sv_SpeedC2;

//	StaticView		sv_TimeC;
//	StaticView		sv_TimeC2;

	StaticView		sv_ErrInfo;		

	StaticView		sv_ExFileC;
	StaticView		sv_ExFileC2;

	ProgressView	pv;

	/////////////////////////////////////
	
	int			OpenDC_SET( DC_SET* pDCset);
	int			CloseDC_SET( DC_SET* pDCset);

public:
	CorumPatch();
	~CorumPatch();


	//
	int			Init_TextView( HDC hDC, char* pszFileName);
	BOOL		Init_TextScroll();

	//	Main Dialog Window Procedure Call
	int			OnInitialize( HWND hWnd);

	//	
	int			OnBtn_1();
	int			OnBtn_2();


	///////////////////////////////////////////////////////////////////////
	//	Mouse move
	int			OnMouseMove(WPARAM wParam, LPARAM lParam);
	//	Button
	int			OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int			OnLButtonUp(WPARAM wParam, LPARAM lParam);
	//	Scroll
	BOOL		OnCtlColorScrollBar( WPARAM wParam, LPARAM lParam);
	int			OnVscroll( WPARAM wParam, LPARAM lParam);
	//	Paint	
	int			OnPaint();
	///////////////////////////////////////////////////////////////////////
	//	subclassing
	int			OnDrawItem( WPARAM wParam, LPARAM lParam);
	int			On_Btn_MouseMove( HWND hWnd, WPARAM wParam, LPARAM lParam);
	///////////////////////////////////////////////////////////////////////
};

extern CorumPatch* g_pcp;

