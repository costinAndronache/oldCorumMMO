#include "stdafx.h"
#include "NewSerial.h"
#include "CopyItemFrame.h"
#include "CopyItemView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCopyItemFrame, CMDIChildWnd)


BEGIN_MESSAGE_MAP(CCopyItemFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CCopyItemFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
CCopyItemFrame::CCopyItemFrame()
{
	m_minWidth		= 0;
    m_minHeight		= 0; 
	m_nFrameID		= 0;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
CCopyItemFrame::~CCopyItemFrame()
{

}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
BOOL CCopyItemFrame::PreCreateWindow(CREATESTRUCT& cs) 
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
LRESULT CCopyItemFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_SYSCOMMAND)
	{
		if(wParam == SC_CLOSE)
		{
			CCopyItemView* pView = (CCopyItemView*)GetActiveView();
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
