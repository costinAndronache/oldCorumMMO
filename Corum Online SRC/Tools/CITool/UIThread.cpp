#include "stdafx.h"
#include "NewSerial.h"
#include "UIThread.h"
#include "MyView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CUIThread, CWinThread)


BEGIN_MESSAGE_MAP(CUIThread, CWinThread)
	//{{AFX_MSG_MAP(CUIThread)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
CUIThread::CUIThread()
{
	
}


CUIThread::~CUIThread()
{

}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
BOOL CUIThread::InitInstance() 
{
	// TODO:
	return CWinThread::InitInstance();
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
int CUIThread::ExitInstance()
{
	// TODO:  
	return CWinThread::ExitInstance();
}



