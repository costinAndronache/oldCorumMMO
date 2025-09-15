
// CorumPreLauncherDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "CorumPreLauncher.h"
#include "CorumPreLauncherDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCorumPreLauncherDlg dialog

using namespace CorumPreferences;

CCorumPreLauncherDlg::CCorumPreLauncherDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CORUMPRELAUNCHER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCorumPreLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_COMBO1, _resolutionsList);
  DDX_Control(pDX, IDC_CHECK1, _windowModeCheck);
}

BEGIN_MESSAGE_MAP(CCorumPreLauncherDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
        ON_BN_CLICKED(IDC_BUTTON1, &CCorumPreLauncherDlg::OnBnClickedButton1)
        ON_BN_CLICKED(IDC_EXITBUTTON,
                      &CCorumPreLauncherDlg::OnBnClickedExitbutton)
        END_MESSAGE_MAP()


// CCorumPreLauncherDlg message handlers

BOOL CCorumPreLauncherDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	_resolutionsListModels = resolutionsListModels();
	for(const auto& item: _resolutionsListModels) {
		_resolutionsList.AddString(item.text.c_str());
	}

	setupFromStoredPreferences();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCorumPreLauncherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCorumPreLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCorumPreLauncherDlg::setupFromStoredPreferences() {
	CorumPreferences::Preferences* prefs = Preferences::buildFromFile(Preferences::defaultFileName());
	if(!prefs) { return; }

	_windowModeCheck.SetCheck(prefs->windowMode());

	auto res = prefs->resolution();
	for(int i=0; i < _resolutionsListModels.size(); i++) {
		const auto& item = _resolutionsListModels[i];
		if (item.resolution.width == res.width && item.resolution.height == res.height) {
			_resolutionsList.SetCurSel(i);
			return;
		}
	}
	
}

void CCorumPreLauncherDlg::storeCurrentPreferences() {
	CorumPreferences::Preferences* prefs = Preferences::buildFromFile(Preferences::defaultFileName());
	prefs->storeWindowMode(_windowModeCheck.GetCheck());
	auto currentSelection = _resolutionsList.GetCurSel();
	prefs->storeResolution(
		(_resolutionsListModels.begin() + currentSelection)->resolution
	);

	prefs->serializeToFile(Preferences::defaultFileName());
}

std::vector<CCorumPreLauncherDlg::ResolutionsListItemModel> CCorumPreLauncherDlg::resolutionsListModels() {
	CorumPreferences::Resolution fullScreen = {
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN)
	};

	RECT workrect;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workrect, 0);
    int workwidth = workrect.right -  workrect.left;
    int workheight = workrect.bottom - workrect.top;

	CorumPreferences::Resolution withTaskBar{workwidth, workheight};

	std::vector<ResolutionsListItemModel> result{
		{ L"Full screen with task bar",  withTaskBar}, 
		{ L"Full screen", fullScreen},
        { L"1024 x 768", {1024, 768}},
		{ L"1280 x 800", {1280, 800}},
		{ L"1360 x 768", {1360, 768}},
		{ L"1440 x 900", {1440, 900}},
        { L"1600 x 900", {1600, 900}}
	};

	return result;
}

void CCorumPreLauncherDlg::OnBnClickedButton1() {
	storeCurrentPreferences();

	PROCESS_INFORMATION ProcessInfo; // This is what we get as an [out] parameter

    STARTUPINFO StartupInfo; // This is an [in] parameter

    ZeroMemory(&StartupInfo, sizeof(StartupInfo));
    StartupInfo.cb = sizeof StartupInfo; // Only compulsory field

    if (CreateProcess(L"CorumOnlineResult.exe", NULL, NULL, NULL, FALSE, 0,
                          NULL, NULL, &StartupInfo, &ProcessInfo)) {
          WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
          CloseHandle(ProcessInfo.hThread);
          CloseHandle(ProcessInfo.hProcess);
    } else {
          MessageBox(L"The game could not be started...");
    }
}

void CCorumPreLauncherDlg::OnBnClickedExitbutton() {
  PostQuitMessage(0);
}
