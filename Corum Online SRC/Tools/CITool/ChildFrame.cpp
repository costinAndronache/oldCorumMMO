#include "stdafx.h"
#include "NewSerial.h"
#include "ChildFrame.h"
#include "MyView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)


BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
CChildFrame::CChildFrame()
{
	m_minWidth		= 0;
    m_minHeight		= 0; 
	m_nFrameID		= 0;	
}


CChildFrame::~CChildFrame()
{

}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style &= ~FWS_ADDTOTITLE;
	cs.style &= ~WS_THICKFRAME;		//Resize ¾ÈµÇ°Ô..
	cs.style &= ~WS_MAXIMIZEBOX;

	return TRUE;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
LRESULT CChildFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_SYSCOMMAND)
	{
		if(wParam == SC_CLOSE)
		{
			CMyView* pView = (CMyView*)GetActiveView();
			if(pView->m_bRun)
			{
				if(AfxMessageBox("Thread Running.... Is Quit???",MB_YESNO|MB_ICONINFORMATION) == IDYES)
				{
					pView->m_bRun = FALSE;

					while(1)
					{	
						DWORD dwWait = WaitForSingleObject(pView->m_hThread,100);

						if(dwWait == WAIT_OBJECT_0)
						{
							::CloseHandle(pView->m_hThread);
							break;
						}
						else if(dwWait == WAIT_FAILED)
						{
							int aaa = 0;
						}
						else if(dwWait == WAIT_TIMEOUT)
						{
							MSG msg;
							if( PeekMessage(&msg,NULL,0,0,PM_REMOVE) )
							{
								GetMessage(&msg,NULL,0,0);
								TranslateMessage(&msg);
								DispatchMessage(&msg);
							}

//							DWORD dwExit = 0;
//							::GetExitCodeThread(pView->m_hThread,&dwWait);
//
//							if(dwExit == STILL_ACTIVE)
//								int aaa = 0;
//
//							::TerminateThread(pView->m_pWinThread->m_hThread,dwExit);
//							AfxMessageBox("WAIT_TIMEOUT",MB_OK|MB_ICONSTOP);
						}
					}					
				}
				else
				{
					return TRUE;
				}
			}
		}
	}

	return CMDIChildWnd::WindowProc(message, wParam, lParam);
}


#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}


void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}
#endif 




