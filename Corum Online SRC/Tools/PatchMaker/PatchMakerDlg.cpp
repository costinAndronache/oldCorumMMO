// PatchMakerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PatchMaker.h"
#include "PatchMakerDlg.h"
#include "LoginDialog.h"
#include "AddVersionDialog.h"
#include "DFXForm.h"
#include "VDFFile.h"

#include "ExceptionFilter.h"
#include "PatchEngine.h"
#include "FolderOp.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT ThreadFunc(LPVOID pParam);

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatchMakerDlg dialog

CPatchMakerDlg::CPatchMakerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPatchMakerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPatchMakerDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon			= AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pWinThread	= NULL;
	m_hThread		= NULL;
}

void CPatchMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPatchMakerDlg)
	DDX_Control(pDX, IDC_PROGRESS2, m_ProgressCtrlSub);
	DDX_Control(pDX, IDC_PROGRESS, m_ProgressCtrl);
	DDX_Control(pDX, IDC_LIST_VERSION, m_lstVersion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPatchMakerDlg, CDialog)
	//{{AFX_MSG_MAP(CPatchMakerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ADD_VERSION, OnButtonAddVersion)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_LAST_VERSION, OnButtonDeleteLastVersion)
	ON_BN_CLICKED(IDC_BUTTON_LOCK_LAST_VERSION, OnButtonLockLastVersion)
	ON_BN_CLICKED(IDC_BUTTON_UNLOCK_LAST_VERSION, OnButtonUnlockLastVersion)
	ON_BN_CLICKED(IDC_BUTTON_BUILD_LAST_FULL, OnButtonBuildLastFull)
	ON_BN_CLICKED(IDC_BUTTON_BUILD_LAST_PATCH, OnButtonBuildLastPatch)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_ALL_VERSION, OnButtonLoadAllVersion)
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_BUTTON_BUILD_PATCH_FILES_WITH_BREAKING, OnButtonBuildPatchFilesWithBreaking)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatchMakerDlg message handlers

BOOL CPatchMakerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SET_GLOBAL_EXCEPTION_FILTER();

	// administrator 패스워드 확인!
#ifndef _DEBUG
	CLoginDialog dlg;
	dlg.DoModal();

	CString strPwd = dlg.GetPassword();
	char szPwd[_MAX_PATH]={0,};
#ifdef _UNICODE
	strcpy(szPwd, Convert2Multi((LPCTSTR)strPwd));
#else
	strcpy(szPwd, (LPSTR)(LPCTSTR)strPwd);
#endif

	if(!m_Engine.CheckPassword((LPCSTR)szPwd))
	{
		MessageBox(_T("Wrong password!"), _T("Administrator"), MB_ICONERROR|MB_OK);
		OnCancel();
		return TRUE;
	}
#endif // _DEBUG

	if(!m_Engine.Initialize(m_hWnd,&m_ProgressCtrl,&m_ProgressCtrlSub))
	{
		OnCancel();
		return TRUE;
	}

	CRect rect(1,1,1,1);
	m_ProgressCtrl.SetBkColor(RGB(228,228,228));
	m_ProgressCtrl.SetGradientColors(RGB(0,109,255),RGB(0,0,62));
	m_ProgressCtrl.SetBorders(&rect);
	m_ProgressCtrl.SetRange(0,100);

	m_ProgressCtrlSub.SetBkColor(RGB(228,228,228));
	m_ProgressCtrlSub.SetGradientColors(RGB(0,109,255),RGB(0,0,62));
	m_ProgressCtrlSub.SetBorders(&rect);
	m_ProgressCtrlSub.SetRange(0,100);

	m_lstVersion.InsertColumn(1, _T("Version"), LVCFMT_LEFT, 70);
	m_lstVersion.InsertColumn(2, _T("Built Time"), LVCFMT_LEFT, 130);
	m_lstVersion.InsertColumn(3, _T("Lock"), LVCFMT_LEFT, 60);
	m_lstVersion.InsertColumn(4, _T("Description"), LVCFMT_LEFT, 300);

	m_lstVersion.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_TWOCLICKACTIVATE);

	// Set Dialog Caption.
	CString strText;
	GetWindowText(strText);

	strText += _T(" - ");
	strText += CPatchMakerConfig::GetInstance()->GetGameName();
	strText += _T(" [ ");
	strText += CPatchMakerConfig::GetInstance()->GetLocale();
	strText += _T(" @ ");
	strText += CPatchMakerConfig::GetInstance()->GetGameStatus();
	strText += _T(" ]");

	SetWindowText(strText);

	if(!CPatchMakerConfig::GetInstance()->IsMigrated())
	{
		MessageBox(_T("VDF Miration Needed! Now Migrate."), _T("VDF Migration"), MB_OK|MB_ICONWARNING);

		m_Engine.MigrateVDF();

		MessageBox(_T("Migration Finished!"), _T("VDF Migration"), MB_OK|MB_ICONINFORMATION);
	}
	
	// 모든 버전을 미리 로드.
	OnButtonLoadAllVersion();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPatchMakerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPatchMakerDlg::OnPaint() 
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
HCURSOR CPatchMakerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
void CPatchMakerDlg::OnButtonLoadAllVersion() 
{
	// TODO: Add your control notification handler code here
	EnableAllButton(FALSE);

	m_lstVersion.DeleteAllItems();

	BOOL bSilent = m_Engine.IsThereNoPatch();

	m_Engine.UnloadVDFs();

	if(!m_Engine.LoadExistingVDF())
	{
		MessageBox(_T("There's no VDF"), _T("Browsing Existing VDF"), MB_OK|MB_ICONINFORMATION);
	}

	if(!bSilent) 
		MessageBox(_T("All VDF are loaded"), _T("VDF Loading"), MB_OK|MB_ICONINFORMATION);

	EnableAllButton(TRUE);
}

void CPatchMakerDlg::OnButtonAddVersion() 
{
	// TODO: Add your control notification handler code here
	AddVersionDialog dlg;

	if(IDCANCEL == dlg.DoModal()) return;

	EnableAllButton(FALSE);

	VERSION_DATA data;
	dlg.GetVersionData(&data);

	if(!m_Engine.AddNewVersion(&data))
	{
		MessageBox(_T("Failed to add new version"), 
			_T("Adding New Version"), MB_OK|MB_ICONERROR);
	}

	EnableAllButton(TRUE);
}

void CPatchMakerDlg::OnButtonDeleteLastVersion() 
{
	// TODO: Add your control notification handler code here
	if(IDYES == MessageBox(_T("Are You Sure to DELETE LATEST Version?"),
		_T("Confirm Deletion"), MB_YESNO|MB_ICONWARNING))
	{
		EnableAllButton(FALSE);

		if(m_Engine.RemoveLastVersion())
		{
			// ui 에서 제거한다.
			int iCount = m_lstVersion.GetItemCount();
			m_lstVersion.DeleteItem(iCount-1);

			MessageBox(_T("Last version deleted"), _T("Delete Last Version"), MB_OK|MB_ICONINFORMATION);
		}
		else
		{
			MessageBox(_T("Failed To delete last version"), _T("Delete Last Version"), MB_OK|MB_ICONERROR);
		}

		EnableAllButton(TRUE);
	}	
}

void CPatchMakerDlg::OnButtonLockLastVersion() 
{
	// TODO: Add your control notification handler code here
	if(!m_Engine.LockLastVersion(TRUE))
	{
		MessageBox(_T("Failed To lock last version"), _T("Lock Last Version"), MB_OK|MB_ICONERROR);
	}
	else
	{
		int iCount = m_lstVersion.GetItemCount();
		m_lstVersion.SetItemText(iCount-1, 2, _T("Locked"));

	//	MessageBox(_T("Last Version Locked"), _T("Lock Last Version"), MB_OK|MB_ICONINFORMATION);
	}
}

void CPatchMakerDlg::OnButtonUnlockLastVersion() 
{
	// TODO: Add your control notification handler code here
	if(!m_Engine.LockLastVersion(FALSE))
	{
		MessageBox(_T("Failed To unlock last version"), _T("Unlock Last Version"), MB_OK|MB_ICONERROR);
	}
	else
	{
		int iCount = m_lstVersion.GetItemCount();
		m_lstVersion.SetItemText(iCount-1, 2, _T("Unlocked"));

	//	MessageBox(_T("Last Version Unlocked"), _T("Unlock Last Version"), MB_OK|MB_ICONINFORMATION);
	}
}

void CPatchMakerDlg::OnButtonBuildLastFull() 
{
	// TODO: Add your control notification handler code here
	EnableAllButton(FALSE);

	if(!m_Engine.BuildLastFullVersion())							// 최신의 full버전을 제작
	{
		MessageBox(_T("Failed To Export Full version"), _T("Export Full Version"), MB_OK|MB_ICONERROR);
	}
	else
	{
		MessageBox(_T("Full version Exported"), _T("Export Full Version"), MB_OK|MB_ICONINFORMATION);
	}

	EnableAllButton(TRUE);
}

void CPatchMakerDlg::OnButtonBuildLastPatch() 
{
	//--
	m_pWinThread	= ::AfxBeginThread(ThreadFunc,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	::DuplicateHandle(GetCurrentProcess(),m_pWinThread->m_hThread,GetCurrentProcess(),&m_hThread,0,FALSE,DUPLICATE_SAME_ACCESS);
	m_pWinThread->ResumeThread();
	//--
/*
	// TODO: Add your control notification handler code here
	EnableAllButton(FALSE);

#ifdef __SLICED_ZIP_FORMAT
	if(!m_Engine.BuildLastPatchFilesWithSlicing())	// 최신의 패치 파일들을 제작(분할압축형태) 
#else
	if(!m_Engine.BuildLastPatchFiles())				// 최신의 패치 파일들을 제작
#endif
	{
		MessageBox(_T("Failed to export patch files"), _T("Build Patch"), MB_OK|MB_ICONERROR);
	}
	else
	{
		MessageBox(_T("Patch Files Exported"), _T("Build Patch"), MB_OK|MB_ICONINFORMATION);
	}

	EnableAllButton(TRUE);
*/
}

BOOL CPatchMakerDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	// TODO: Add your message handler code here and/or call default

	LPVERSION_DATA pVersionData = (LPVERSION_DATA)pCopyDataStruct->lpData;

	// UI에 LOAD한 VERSION 정보를 뿌려준다.
	int nItem = m_lstVersion.InsertItem(m_lstVersion.GetItemCount(), pVersionData->szVersion);

	TCHAR szBuiltTime[0xff]={0,};
	wsprintf(szBuiltTime, _T("%4u-%02d-%02d %02d:%02d:%02d"), 
		pVersionData->timeBuilt.wYear,
		pVersionData->timeBuilt.wMonth,
		pVersionData->timeBuilt.wDay,
		pVersionData->timeBuilt.wHour,
		pVersionData->timeBuilt.wMinute,
		pVersionData->timeBuilt.wSecond);

	TCHAR szLock[0xff]={0,};
	if(pVersionData->bLocked)
	{
		lstrcpy(szLock, _T("Locked"));
	}
	else
	{
		lstrcpy(szLock, _T("Unlocked"));
	}
	m_lstVersion.SetItemText(nItem, 1, szBuiltTime);
	m_lstVersion.SetItemText(nItem, 2, szLock);
	m_lstVersion.SetItemText(nItem, 3, pVersionData->szDescription);

	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}

void CPatchMakerDlg::EnableButton(int nButtonID, BOOL bEnable)
{
	GetDlgItem(nButtonID)->EnableWindow(bEnable);
}

void CPatchMakerDlg::EnableAllButton(BOOL bEnable)
{
	EnableButton(IDC_BUTTON_ADD_VERSION, bEnable);
	EnableButton(IDC_BUTTON_DELETE_LAST_VERSION, bEnable);
	EnableButton(IDC_BUTTON_BUILD_LAST_PATCH, bEnable);
	EnableButton(IDC_BUTTON_BUILD_LAST_FULL, bEnable);
	EnableButton(IDC_BUTTON_LOCK_LAST_VERSION, bEnable);
	EnableButton(IDC_BUTTON_UNLOCK_LAST_VERSION, bEnable);
	EnableButton(IDC_BUTTON_LOAD_ALL_VERSION, bEnable);
	EnableButton(IDC_BUTTON_BUILD_PATCH_FILES_WITH_BREAKING, bEnable);
}

void CPatchMakerDlg::InitProgressiveBar()
{
	// Progressive Bar 다시 초기화.
	m_ProgressCtrlSub.SetPos(0);
	m_ProgressCtrl.SetPos(0);	

	m_ProgressCtrl.SetTextFormat(_T(""), 0);
	m_ProgressCtrlSub.SetTextFormat(_T(""), 0);
}

void CPatchMakerDlg::OnButtonBuildPatchFilesWithBreaking() 
{
	// TODO: Add your control notification handler code here
	EnableAllButton(FALSE);

#ifdef __SLICED_ZIP_FORMAT
	if(!m_Engine.BuildLastPatchFilesWithSlicing(TRUE))	// 최신의 패치 파일들을 제작(분할압축형태) 
#else
	if(!m_Engine.BuildLastPatchFiles(TRUE))				// 최신의 패치 파일들을 제작
#endif
	{
		MessageBox(_T("Failed to export patch files"), _T("Build Patch"), MB_OK|MB_ICONERROR);
	}
	else
	{		
		if(!m_Engine.ResetPatchAfterBreaking())
		{
			MessageBox(_T("Failed to reset version information"), _T("Reset Version Info"), MB_OK|MB_ICONERROR);
		}
		else
		{
			MessageBox(_T("Patch Files Exported and Version Information Reset!"), _T("Breaking"), MB_OK|MB_ICONINFORMATION);
			OnButtonLoadAllVersion();
		}
	}

	EnableAllButton(TRUE);
}


//=====================================================
// 
//=====================================================
UINT ThreadFunc(LPVOID pParam)
{
	CPatchMakerDlg* pPatchMakerDlg = (CPatchMakerDlg*)pParam;

	pPatchMakerDlg->EnableAllButton(FALSE);

#ifdef __SLICED_ZIP_FORMAT
	if(!pPatchMakerDlg->m_Engine.BuildLastPatchFilesWithSlicing())	// 최신의 패치 파일들을 제작(분할압축형태) 
#else
	if(!pPatchMakerDlg->m_Engine.BuildLastPatchFiles())				// 최신의 패치 파일들을 제작
#endif
	{
		MessageBox(pPatchMakerDlg->m_hWnd,_T("Failed to export patch files"), _T("Build Patch"), MB_OK|MB_ICONERROR);
	}
	else
	{
		MessageBox(pPatchMakerDlg->m_hWnd,_T("Patch Files Exported"), _T("Build Patch"), MB_OK|MB_ICONINFORMATION);		
	}

	pPatchMakerDlg->InitProgressiveBar();
	pPatchMakerDlg->EnableAllButton(TRUE);

	return 0;
}
