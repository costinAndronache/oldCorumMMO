#include "stdafx.h"
#include "NewSerial.h"
#include "LoginDlg.h"
#include <Winsock2.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/) : CDialog(CLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDlg)
	m_szID = _T("");
	m_szPassWord = _T("");
	//}}AFX_DATA_INIT
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
	DDX_Text(pDX, IDC_EDIT_ID, m_szID);
	DDX_Text(pDX, IDC_EDIT_PW, m_szPassWord);
	//}}AFX_DATA_MAP
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
BOOL CLoginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE; 
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(	pMsg->hwnd		== GetDlgItem(IDC_EDIT_PW)->m_hWnd	&&
		pMsg->message	== WM_KEYDOWN						&&
		pMsg->wParam	== VK_RETURN							)
	{
		EnterAtPassword();
		return TRUE;
	}

	if(	pMsg->hwnd		== GetDlgItem(IDC_EDIT_ID)->m_hWnd	&&
		pMsg->message	== WM_KEYDOWN						&&
		pMsg->wParam	== VK_RETURN							)
	{
		(GetDlgItem(IDC_EDIT_PW))->SetFocus();
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


//-----------------------------------------------------------------------------
// : Password Edit 박스에서 Enter 쳤을때..
//-----------------------------------------------------------------------------
void CLoginDlg::EnterAtPassword()
{
	UpdateData(TRUE);
	
	switch(CheckID(m_szID))
	{
		case 1:		EndDialog(IDOK);									break;
		case -1:	MessageBox("Unregistered ID!");						return;
		case -2:	MessageBox("Invalid Password!");					return;
		case -3:	MessageBox("Unregistered IP Address!");				return;
		case -4:	MessageBox("Please, Check Account and try Again!");	return;
	}
}


//-----------------------------------------------------------------------------
// : 
//-----------------------------------------------------------------------------
int CLoginDlg::CheckID(CString id)
{
	CApp *pApp = (CApp*)AfxGetApp();
	
	//IP 얻어오기
	char szHostName[40];	
	::gethostname(szHostName, sizeof(szHostName));	
	HOSTENT FAR* lphostent = ::gethostbyname(szHostName);
	sprintf(pApp->m_szLocalAddress,"%s",inet_ntoa(*((struct in_addr*)lphostent->h_addr)));
	
	char	str[ 0xff ]		= {0,};
	WCHAR	query[ 0xff ]	= {0,};	
	
	sprintf(str,"UP_CM_LOGIN '%s'", id);
	
	int ret = g_pDb->OpenRecord(str, &pApp->m_LogInfo, 1, g_CORUM_TOTAL_DB); 
	
	if(ret == 0)
	{
		return -1;	//없는 아이디 
	}
	else 
	{	
		int temp = 0;
		
		if(!strcmp(pApp->m_LogInfo.Password, m_szPassWord))
		{
			temp++;
		}
		
		if(!strcmp(pApp->m_LogInfo.IP, pApp->m_szLocalAddress))
		{
			temp += 2;
		}
		
		switch(temp)
		{
			case 1:		return -3;	break;		//ip 틀림
			case 2:		return -2;	break;		//비번 틀림 
			case 3:		return  1;	break;
			case 0:		return -4;	break;		//ip,비번 둘다 틀림 
		}
	}
	
	return -5;	
}


