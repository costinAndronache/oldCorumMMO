// RegionSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RegionSelect.h"
#include "RegionSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegionSelectDlg dialog

CRegionSelectDlg::CRegionSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegionSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegionSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRegionSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegionSelectDlg)
	DDX_Control(pDX, IDC_LIST_REGION, m_lstRegion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRegionSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CRegionSelectDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_LBN_DBLCLK(IDC_LIST_REGION, OnDblclkListRegion)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegionSelectDlg message handlers

BOOL CRegionSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SetCurrentDirectoryToFolderOfExecutable();
	LoadRegionInfos();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRegionSelectDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRegionSelectDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CRegionSelectDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CRegionSelectDlg::OnOK() 
{
	// TODO: Add extra validation here
	int iRegionCode = GetSelectedRegionCode();

	// save last used region info to ini file.
	char szLastRegion[0xff]={0,};
	sprintf(szLastRegion, "%d", iRegionCode);
	WritePrivateProfileString("REGION_INFO", "lastusedregion", szLastRegion, "./Region.ini");

	// execute autopatcher!
	ExecuteAutopatcher(iRegionCode);

	CDialog::OnOK();
}

void CRegionSelectDlg::OnDblclkListRegion() 
{
	// TODO: Add your control notification handler code here
	OnOK();	
}

int CRegionSelectDlg::GetSelectedRegionCode()
{
	int iSel = m_lstRegion.GetCurSel();
	if(LB_ERR == iSel) iSel = 0;
	return (iSel+1);
}

void CRegionSelectDlg::LoadRegionInfos()
{
	char szKey[0xff];
	char szKeyVal[0xff];

	int iRegionNum = GetPrivateProfileInt("REGION_INFO", "totalregionnum", 0, "./Region.ini");
	int iLastRegion = GetPrivateProfileInt("REGION_INFO", "lastusedregion", 1, "./Region.ini");

	// get region names!
	for(int i = 0; i < iRegionNum; i++)
	{
		ZeroMemory(szKeyVal, sizeof(szKeyVal));
		sprintf(szKey, "region_%02d_name", i+1);
		GetPrivateProfileString("REGION_INFO", szKey, "", szKeyVal, MAX_SERVER_SET_NAME_LENGTH, "./Region.ini");
		if(lstrlen(szKeyVal) == 0) break;

		m_lstRegion.AddString(szKeyVal);
	}

	// set last used region to default.
	m_lstRegion.SetCurSel(iLastRegion-1);
}

void CRegionSelectDlg::ExecuteAutopatcher(int iRegionCode)
{
	char szCmdLine[0xff]={0,};
	STARTUPINFO			suInfo;
	PROCESS_INFORMATION procInfo;
	BOOL				bRet = 0;

	ZeroMemory(&procInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&suInfo, sizeof(STARTUPINFO));
	suInfo.cb = sizeof(STARTUPINFO);	

	// execute!
	sprintf(szCmdLine, " /region:%d", iRegionCode);
	bRet = CreateProcess(".\\CorumAutopatch.exe", szCmdLine, NULL, NULL, TRUE, 
			CREATE_DEFAULT_ERROR_MODE | NORMAL_PRIORITY_CLASS, NULL, NULL, &suInfo, &procInfo);

	if(!bRet)
	{
		MessageBox("Execution of CorumAutopatch.exe Failed!", "Execution Error", MB_OK|MB_ICONERROR);
	}
}

void CRegionSelectDlg::SetCurrentDirectoryToFolderOfExecutable()
{
	char	szModulePath[_MAX_PATH] = {0,};
	char	szNewPath[_MAX_PATH] = {0,};
	char	c = '\\';
	LPSTR	pLastOccur = NULL;
	size_t	nLen = 0;

	GetModuleFileName(NULL, szModulePath, sizeof(szModulePath)/sizeof(szModulePath[0]));
	
	pLastOccur = strrchr(szModulePath, c); 
	nLen = size_t(pLastOccur - szModulePath + 1);
	
	strncpy(szNewPath, szModulePath, nLen);

	SetCurrentDirectory(szNewPath);
}

