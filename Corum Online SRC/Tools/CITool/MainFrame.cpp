#include "stdafx.h"
#include "NewSerial.h"
#include "MainFrame.h"
#include "FindItemNumDlg.h"
#include "CopyItemFrame.h"
#include "CopyItemView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)


BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_FIND_ITEM, OnFindItem)
	ON_COMMAND(ID_NEW_SERIAL, OnNewSerial)
	ON_COMMAND(ID_GUARDIAN, OnGuardian)
	ON_COMMAND(ID_GUILD_INIT, OnGuildInit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


//------------------------------------------------------------------------------
//	DESCRIPTION	:	
//------------------------------------------------------------------------------
CMainFrame::CMainFrame()
{
	m_pFindItemNumDlg = NULL;	
}


CMainFrame::~CMainFrame()
{

}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{


	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP	| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      
	}

//	CImageList imageList;
//	CBitmap    bitmap;
	
//  bitmap.			LoadBitmap(IDB_TOOLBAR);
//	imageList.		Create(36,36,ILC_COLORDDB|ILC_MASK,3,1);
//  imageList.		Add(&bitmap, RGB(255,0,255));									// 바탕의 분홍색이 마스크
//  m_wndToolBar.	SendMessage(TB_SETIMAGELIST,0,(LPARAM)imageList.m_hImageList);
//  imageList.		Detach();
//	bitmap.			Detach();

	m_wndToolBar.EnableDocking(CBRS_ALIGN_LEFT);
	EnableDocking(CBRS_ALIGN_LEFT);
	DockControlBar(&m_wndToolBar,AFX_IDW_DOCKBAR_LEFT);
	CenterWindow();
//	ShowWindow(SW_SHOWMAXIMIZED);

	return 0;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{

	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style	&= ~FWS_ADDTOTITLE;	// 제목없음 안나오게
	cs.lpszName = "::: Corum Online Management Tool :::";	
	
	return TRUE;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	- 아이템 선택 다이얼로그 
//------------------------------------------------------------------------------
void CMainFrame::OnFindItem() 
{
	if( m_pFindItemNumDlg != NULL )
	{
		// 다이얼로그박스가 이미 출력되어 있는 경우
		if( m_pFindItemNumDlg->m_bCreate )
		{
			m_pFindItemNumDlg->SetFocus();				
			m_pFindItemNumDlg->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		// 다이얼로그박스가 출력되어 있지 않은 경우
		m_pFindItemNumDlg = new CFindItemNumDlg;
		m_pFindItemNumDlg->Create(this);
		m_pFindItemNumDlg->ShowWindow(SW_SHOW);
	}
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	- 복사아이템 딜리트  
//------------------------------------------------------------------------------
void CMainFrame::OnNewSerial() 
{
#ifdef JAPANESS_MODE
	CMultiDocTemplate *pDocTemplate;
	CApp *pApp = (CApp*)AfxGetApp();
	
	POSITION pos = pApp->GetFirstDocTemplatePosition();
	pDocTemplate = (CMultiDocTemplate*)pApp->GetNextDocTemplate(pos);	
	pDocTemplate = (CMultiDocTemplate*)pApp->GetNextDocTemplate(pos);
	pDocTemplate = (CMultiDocTemplate*)pApp->GetNextDocTemplate(pos);
	pDocTemplate = (CMultiDocTemplate*)pApp->GetNextDocTemplate(pos);
	pDocTemplate->m_hMenuShared = NULL;
	pDocTemplate->OpenDocumentFile(NULL);
	SendMessage(WM_COMMAND,ID_WINDOW_TILE_HORZ,0);
#else
#endif	
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	- 아이템 통계
//------------------------------------------------------------------------------
void CMainFrame::OnGuardian() 
{
	CMultiDocTemplate *pDocTemplate;
	CApp *pApp = (CApp*)AfxGetApp();
	
	POSITION pos = pApp->GetFirstDocTemplatePosition();
	pDocTemplate = (CMultiDocTemplate*)pApp->GetNextDocTemplate(pos);	
	pDocTemplate = (CMultiDocTemplate*)pApp->GetNextDocTemplate(pos);
	pDocTemplate = (CMultiDocTemplate*)pApp->GetNextDocTemplate(pos);
	pDocTemplate->m_hMenuShared = NULL;
	pDocTemplate->OpenDocumentFile(NULL);
	SendMessage(WM_COMMAND,ID_WINDOW_TILE_HORZ,0);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	- 길드 
//------------------------------------------------------------------------------
void CMainFrame::OnGuildInit() 
{
/*	CMultiDocTemplate *pDocTemplate;
	CApp *pApp = (CApp*)AfxGetApp();
	
	POSITION pos = pApp->GetFirstDocTemplatePosition();
	pDocTemplate = (CMultiDocTemplate*)pApp->GetNextDocTemplate(pos);	
	pDocTemplate = (CMultiDocTemplate*)pApp->GetNextDocTemplate(pos);
	pDocTemplate = (CMultiDocTemplate*)pApp->GetNextDocTemplate(pos);
	pDocTemplate = (CMultiDocTemplate*)pApp->GetNextDocTemplate(pos);
	pDocTemplate = (CMultiDocTemplate*)pApp->GetNextDocTemplate(pos);
	pDocTemplate->m_hMenuShared = NULL;
	pDocTemplate->OpenDocumentFile(NULL);
	SendMessage(WM_COMMAND,ID_WINDOW_TILE_HORZ,0);	*/
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	- 
//------------------------------------------------------------------------------
LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_SYSCOMMAND)
	{
		if(wParam == SC_CLOSE)
		{			
		}
	}

	return CMDIFrameWnd::WindowProc(message, wParam, lParam);
}



#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif 





























