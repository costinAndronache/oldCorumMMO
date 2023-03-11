// pc_statusDlg.cpp : implementation file
//

#define _WIN32_WINNT 0x0500


#include "stdafx.h"
#include "pc_status.h"
#include "pc_statusDlg.h"
#include "shlwapi.h"
#include "spec_info.h"
#include "graphic_card_info.h"
#include "getdxver.h"


void SpecCPU(char * pszCpu);
void GetDllVersion(char * dd);


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------------
// External function-prototypes
//-----------------------------------------------------------------------------


// functions prototype
//void	SpecCPU( char * StrCpu );




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
// CPc_statusDlg dialog

CPc_statusDlg::CPc_statusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPc_statusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPc_statusDlg)
	m_StrInfoEdit = _T("");
	m_StrCPU = _T("");
	m_StrDisplay = _T("");
	m_StrDXVer = _T("");
	m_StrGraphicAGP = _T("");
	m_StrGraphicChip = _T("");
	m_StrGraphicDrv = _T("");
	m_StrGraphicMem = _T("");
	m_StrOS = _T("");
	m_StrRAM = _T("");
	m_StrRemainedHard = _T("");
	m_StrSound = _T("");
	m_StrIsCompressTexture = _T("");
	m_hBitmap = NULL;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CPc_statusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPc_statusDlg)
	DDX_Control(pDX, IDC_SPEC_LIST, m_CtrlSpecList);
	DDX_Control(pDX, IDC_STATIC_CLINETPAGE_GO, m_CtrlGoClientPageStatic);
	DDX_Control(pDX, IDC_STATIC_LOGO, m_CtrlLogoStatic);
	DDX_Text(pDX, IDC_EDIT_INFO, m_StrInfoEdit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPc_statusDlg, CDialog)
	//{{AFX_MSG_MAP(CPc_statusDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PRINT_FILE, OnButtonPrintFile)
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_STATIC_LOGO, OnStaticLogo)
	ON_BN_CLICKED(IDC_STATIC_CLINETPAGE_GO, OnStaticClinetpageGo)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_WM_CREATE()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPc_statusDlg message handlers

BOOL CPc_statusDlg::OnInitDialog()
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
    CoInitialize( NULL );

//
	// 이미지 리스트 생성
	m_CtrlSpecList.m_SmallImageList.Create(16, 16, FALSE, 1,1);
	m_CtrlSpecList.AddIconToList(IDI_ICON_FIT);
	m_CtrlSpecList.AddIconToList(IDI_ICON_DANGEROUS);
	m_CtrlSpecList.AddIconToList(IDI_ICON_HARDCORE);

	m_CtrlSpecList.SetImageList(&m_CtrlSpecList.m_SmallImageList, LVSIL_NORMAL);

	
	CString	strBuf;
	// 기본 사양
	strBuf.LoadString(IDS_MSG3);
	m_StrCPU = (LPCTSTR)strBuf;					// CPU|Pentium® 3 600 Mhz 이상|
	strBuf.LoadString(IDS_MSG4);
	m_StrOS = (LPCTSTR)strBuf;					// OS|Windows® 98/ME/2000/XP|
	strBuf.LoadString(IDS_MSG5);
	m_StrRAM = (LPCTSTR)strBuf;					// RAM|128 MB|
	strBuf.LoadString(IDS_MSG6);
	m_StrRemainedHard	= (LPCTSTR)strBuf;		// _T("설치대상 남은 하드용량|1 GB 이상|");
	strBuf.LoadString(IDS_MSG7);
	m_StrGraphicChip	= (LPCTSTR)strBuf;		//_T("그래픽카드|NVIDIA®,ATI®|");
	strBuf.LoadString(IDS_MSG8);
	m_StrGraphicDrv		= (LPCTSTR)strBuf;		// _T("드라이버(버전)|Geforce®계열 또는 Radeon® 7500 이상|");
	strBuf.LoadString(IDS_MSG9);
	m_StrGraphicMem		= (LPCTSTR)strBuf;		// _T("비디오 메모리|32 MB 이상|");
	strBuf.LoadString(IDS_MSG10);
	m_StrGraphicAGP		= (LPCTSTR)strBuf;		//_T("AGP 메모리|128 MB 이상|");
	strBuf.LoadString(IDS_MSG11);
	m_StrIsCompressTexture = (LPCTSTR)strBuf;	//_T("압축텍스쳐|지원(옵션)|");
	strBuf.LoadString(IDS_MSG12);
	m_StrDXVer			=	(LPCTSTR)strBuf;	// _T("DirectX® 버전|DirectX® 8.1 이상|");
	strBuf.LoadString(IDS_MSG13);
	m_StrSound			= (LPCTSTR)strBuf;		//_T("사운드카드|Sound Blaster® 호환 기종|");

	m_CtrlSpecList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 헤더정의
	m_CtrlSpecList.InitTitleString(NULL);
//	m_CtrlSpecList.SetBkColor(GetSysColor(COLOR_GRAYTEXT));


	// 사양 체크 
	SpecSystem();
	// 리스트 컨트롤에 모두 넣음
	SetSpecList();

	m_CtrlLogoStatic.SetBackImage(IDB_BITMAP_LOGO);
	m_CtrlLogoStatic.StretchBitmap(TRUE);
	m_CtrlLogoStatic.SetLinkCursor(IDC_HAND_POINT);

	// 로고에 링크 만들기 
	m_CtrlGoClientPageStatic.UseHighLighting(TRUE);
	m_CtrlGoClientPageStatic.SetHighLightColor(RGB(255, 128, 0));
	m_CtrlGoClientPageStatic.SetTextColor(RGB(128,0,0));
	m_CtrlGoClientPageStatic.SetLinkCursor(IDC_HAND_POINT);

	UpdateData(FALSE);

	CoUninitialize();	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPc_statusDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPc_statusDlg::OnPaint() 
{
	CPaintDC dc(this); 
	//Create a memory DC
//	HDC hMemDC = ::CreateCompatibleDC(NULL);
	//Select the bitmap in the memory dc.
//	SelectObject(hMemDC, m_hBitmap);
	//Copy the memory dc into the screen dc
//	::BitBlt(dc.m_hDC, 0, 0, m_Bitmap.bmWidth, m_Bitmap.bmHeight, hMemDC, 0, 0, SRCCOPY);
//	CRect	rcWindow;
//	GetWindowRect( (LPRECT)rcWindow );
//	::StretchBlt(dc.m_hDC, 0, 0, rcWindow.Width(), rcWindow.Height(), hMemDC, 0, 0, m_Bitmap.bmWidth, m_Bitmap.bmHeight, SRCCOPY);
	//Delete the memory DC and the bitmap
//	::DeleteDC(hMemDC);
	
	m_CtrlGoClientPageStatic.SetTextColor(RGB(128,0,0));
	CDialog::OnPaint();
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPc_statusDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPc_statusDlg::SpecSystem()
{
	int			iAlertNum = 0;
	CString		StrBuf;
	CString		StrEtcAlert;
	CString		StrEtcMsg;
	char		szInfoBuf[1024];
	char		szInfoBuf1[1024];
	char		szInfoBuf2[1024];
	

	UpdateData();

	// cpu check
	SpecCPU(szInfoBuf);
	StrBuf.Format("%s|", szInfoBuf);
	m_StrCPU += StrBuf;

	if (m_StrCPU.Find("Hr", 0) == 0)
	{ // cpu 속도 체크를 하지못하였을때
		char str [MAX_PATH];
		LONG result;
		HKEY hKey;
		DWORD data;
		DWORD dataSize;

		// Get the processor speed info.

		result = ::RegOpenKeyEx (HKEY_LOCAL_MACHINE,
			"Hardware\\Description\\System\\CentralProcessor\\0", 0, KEY_QUERY_VALUE, &hKey);

		// Check if the function has succeeded.

		if (result == ERROR_SUCCESS) 
		{
			result = ::RegQueryValueEx (hKey, _T("~MHz"), NULL, NULL,
				(LPBYTE)&data, &dataSize);
			
			CString strCpuSpeed;

			strCpuSpeed.Format ("%d", data);
			strCpuSpeed += _T (" MHz");

			StrBuf += strCpuSpeed;
		}
	}
	else
	{
		DWORD dwSpeedMhz = g_CPUInfo.GetCPUSpeedMhz();
		if ( dwSpeedMhz < 600 )
		{
			StrEtcAlert.LoadString(IDS_CPU_ALERT);	
			StrBuf.Format("%d. %s", ++iAlertNum, (LPCTSTR) StrEtcAlert);
			m_StrInfoEdit += StrBuf;
			StrBuf = "Warning!|";
			m_StrCPU += StrBuf;
		}
		else
		{
			StrBuf = "Pass!|";
			m_StrCPU += StrBuf;
		}
	}

	// OS check
	SpecOS(szInfoBuf);
	StrBuf.Format("%s|", szInfoBuf);
	m_StrOS += StrBuf;
	if ( strstr((LPCTSTR) szInfoBuf, "Windows 95") )
	{
		StrEtcAlert.LoadString(IDS_OS_ALERT);
		StrBuf.Format("%d. %s", ++iAlertNum, (LPCTSTR)StrEtcAlert);
		m_StrInfoEdit += StrBuf;

		StrBuf = "Stop!|";
		m_StrOS += StrBuf;
	}
	else
	{
		StrBuf = "Pass!|";
		m_StrOS += StrBuf;
	}

	// System ram check
	DWORD	dwMemory;
	dwMemory = SpecRAM(szInfoBuf);
	StrBuf.Format("%s MB|", szInfoBuf);
	m_StrRAM += StrBuf;
	if ( dwMemory < 128 )
	{
		StrEtcAlert.LoadString(IDS_RAM_ALERT);
		StrBuf.Format("%d. %s", ++iAlertNum, (LPCTSTR)StrEtcAlert);
		m_StrInfoEdit += StrBuf;

		StrBuf = "Warning!|";
		m_StrRAM += StrBuf;
	}
	else
	{
		StrBuf = "Pass!|";
		m_StrRAM += StrBuf;
	}

	// HARD Disk check
	ULONG	ulFreeDiskToMB;
	ulFreeDiskToMB = 
	CurSpecHardDisk((char *)szInfoBuf);
	StrBuf.Format("%s MB|", (LPCTSTR)szInfoBuf);
	m_StrRemainedHard += StrBuf;
	if (ulFreeDiskToMB < 1024)
	{
		StrEtcAlert.LoadString(IDS_HARDDISK_ALERT);
		StrBuf.Format("%d. %s", ++iAlertNum, (LPCTSTR)StrEtcAlert);
		m_StrInfoEdit += StrBuf;

		StrBuf = "Warning!|";
		m_StrRemainedHard += StrBuf;
	}
	else
	{
		StrBuf = "Pass!|";
		m_StrRemainedHard += StrBuf;
	}

	// DX check
	DWORD	dwDXVer;
	dwDXVer = SpecDxVer(szInfoBuf);
	StrBuf.Format("%s|", szInfoBuf);
	m_StrDXVer += StrBuf;
	if (dwDXVer < DX81)
	{
		StrEtcAlert.LoadString( IDS_DIRECTX_VER_ALERT );
		StrBuf.Format("%d. %s", ++iAlertNum, (LPCTSTR)StrEtcAlert);
		m_StrInfoEdit += StrBuf;

		StrBuf = "Warning!|";
		m_StrDXVer += StrBuf;

		if (dwDXVer == 0)
		{
			MessageBox("Please, Setup DirectX!", "Stop");
			return;
		}
	}
	else
	{
		StrBuf = "Pass!|";
		m_StrDXVer += StrBuf;
	}

	// Graphic Card
	// memory check
	DWORD	dwGraphicVidMemoryToKilo;
	dwGraphicVidMemoryToKilo = SpecVidMem(szInfoBuf, szInfoBuf1);
	StrBuf.Format("%s KB|", szInfoBuf);
	m_StrGraphicMem += StrBuf;
	StrBuf.Format("%s KB|", szInfoBuf1);
	m_StrGraphicAGP += StrBuf;
	if (dwGraphicVidMemoryToKilo < 30000)
	{ // 32 미만 이다.
		StrEtcAlert.LoadString( IDS_GRAPHIC_CARD_MEM_ALERT );
		StrBuf.Format("%d. %s", ++iAlertNum, (LPCTSTR)StrEtcAlert);
		m_StrInfoEdit += StrBuf;
		
		StrBuf = "Warning!|";
		m_StrGraphicMem += StrBuf;
	}
	else
	{
		StrBuf = "Pass!|";
		m_StrGraphicMem += StrBuf;
	}

	DWORD	dwGraphicAGPMemory = atol(szInfoBuf1);
	if ( !dwGraphicAGPMemory )
	{ // AGP 메모리가 0 이다 
		StrEtcAlert.LoadString( IDS_GRAPHIC_CARD_AGP_ALERT );
		StrBuf.Format("%d. %s", ++iAlertNum, (LPCTSTR)StrEtcAlert);
		m_StrInfoEdit += StrBuf;

		StrBuf = "Warning!|";
		m_StrGraphicAGP += StrBuf;
	}
	else
	{
		StrBuf = "Pass!|";
		m_StrGraphicAGP += StrBuf;
	}

	// 그래픽카드 정보 

	int	nRetVer;
	nRetVer = SpecGrapicCard(szInfoBuf, szInfoBuf1, szInfoBuf2);
	// chip name
	StrBuf.Format("%s|", szInfoBuf);
	m_StrGraphicChip	+= StrBuf;
	// version 
	StrBuf.Format("%s|", szInfoBuf1);
	m_StrGraphicDrv	+= StrBuf;	
	// display mode
	StrBuf.Format("%s|", szInfoBuf2);
	m_StrDisplay	 = StrBuf;	
	// IdentiFy GraphicCard
	enum eGRAPHIC_CARD_OPTION eGraphicCard;
	eGraphicCard = IdentityGraphicCard();
	switch (eGraphicCard)
	{
	case POOR_CARD:	// 그래픽 카드 메모리가 32MB 이하이다. 
		{
			StrEtcMsg.LoadString(IDS_MSG14);
			StrBuf.Format("%d. %s", ++iAlertNum, (LPCTSTR)StrEtcMsg);
					// "nVidia Geforce 이상계열 그래픽카드 권장\r\n게임 진행이 느려질 수 있습니다.\r\n\r\n");
			m_StrInfoEdit += StrBuf;

			StrBuf = "Stop!|";
			m_StrGraphicChip += StrBuf;
		}
		break;
	case NORMAL: // nVidia 비디오 카드
		if ( nRetVer < NVIDIA_DRIVER_ENOUGH_VER )
		{
			StrEtcMsg.LoadString(IDS_MSG15);
			StrBuf.Format("%d. %s", ++iAlertNum,  (LPCTSTR)StrEtcMsg);
				// "그래픽 카드 드라이버 버전이 \r\n구버전 입니다. 신버전으로 업데이트 해주세요.\r\n게임성능에 영향을 줄수있습니다. \r\n\r\n"
			m_StrInfoEdit += StrBuf;
			StrBuf = "Warning!|";
			m_StrGraphicDrv += StrBuf;

			StrBuf = "Pass!|";
			m_StrGraphicChip += StrBuf;
		}
		else
		{
			StrBuf = "Pass!|";
			m_StrGraphicDrv += StrBuf;
		}
		break;
	case RADEON_7500_UNDER:
		if ( nRetVer < ATI_DRIVER_ENOUGH_VER )
		{
			StrEtcMsg.LoadString(IDS_MSG15); // "그래픽 카드 드라이버 버전이 \r\n구버전 입니다. 신버전으로 업데이트 해주세요.\r\n게임성능에 영향을 줄수있습니다. \r\n\r\n");
			StrBuf.Format("%d. %s", ++iAlertNum,  (LPCTSTR)StrEtcMsg);
			m_StrInfoEdit += StrBuf;

			StrBuf = "Warning!|";
			m_StrGraphicDrv = StrBuf;
		}

		StrEtcMsg.LoadString(IDS_MSG16); // 그래픽 카드가 라데온 7500 이하 계열에서는 라이팅과 \r\n그림자 옵션을 꺼야 한다.  그밖에 월드맵 배경이 \r\n잘 안나오니 양해바랍니다.  게임 플레이하는데에는 \r\n큰 무리는 없습니다.\r\n\r\n
		StrBuf.Format("%d. %s", ++iAlertNum,  (LPCTSTR)StrEtcMsg );
		m_StrInfoEdit += StrBuf;

		StrBuf = "Warning!|";
		m_StrGraphicChip += StrBuf;
		
		break;
	case RADEON_7500_ABOVE:
		if ( nRetVer < ATI_DRIVER_ENOUGH_VER )
		{
			StrEtcMsg.LoadString(IDS_MSG15); // "그래픽 카드 드라이버 버전이 \r\n구버전 입니다. 신버전으로 업데이트 해주세요.\r\n게임성능에 영향을 줄수있습니다. \r\n\r\n");
			StrBuf.Format("%d. %s", ++iAlertNum,  (LPCTSTR)StrEtcMsg);
			m_StrInfoEdit += StrBuf;

			
			StrBuf = "Warning!|";
			m_StrGraphicDrv = StrBuf;
			
		}
		else
		{			
			StrBuf = "Pass!|";
			m_StrGraphicDrv += StrBuf;
		}
			
		StrBuf = "Pass!|";
		m_StrGraphicChip += StrBuf;


		break;
	case MATROX:
		StrEtcMsg.LoadString(IDS_MSG17); // "월드맵 화면이 깨져 보일수도 있습니다.\r\n 게임진행엔 큰문제가 되지는 않습니다.\r\n\r\n");
		StrBuf.Format("%d. %s", ++iAlertNum, (LPCTSTR)StrEtcMsg); 
		m_StrInfoEdit += StrBuf;
		break;
	case VOODOO:
		StrEtcMsg.LoadString(IDS_MSG18); // "부두칩셋에서는 코룸온라인을 실행할수 없습니다!\r\n\r\n");
		StrBuf.Format("%d. %s", ++iAlertNum, (LPCTSTR)StrEtcMsg); 
		m_StrInfoEdit += StrBuf;

		StrBuf = "Stop!|";
		m_StrGraphicChip += StrBuf;
//		m_CtrlGraphicChipStatic.SetTextColor(RGB(255,0,0));
		break;

	case OPTION_NUM:
		StrEtcMsg.LoadString(IDS_MSG19);
		StrBuf.Format("%d. %s", ++iAlertNum, StrEtcMsg);		// 그래픽 카드를 식별하지 못하였습니다.!\r\n\r\n
		m_StrInfoEdit += StrBuf;

		StrBuf = "Stop!|";
		m_StrGraphicChip += StrBuf;
//		m_CtrlGraphicChipStatic.SetTextColor(RGB(255,0,0));
		break;

	default:
		break;
	}

	// 압축 텍스쳐 체크
	if (IsCompressedTextureFormatOk())
	{
		StrEtcMsg.LoadString(IDS_MSG20);
		StrBuf.Format("%s|Pass!|", (LPCTSTR)StrEtcMsg); // "지원|Pass!|";
		m_StrIsCompressTexture += StrBuf;
	}
	else
	{
		StrEtcMsg.LoadString(IDS_MSG21);
		StrBuf.Format("%s|Pass!|", (LPCTSTR)StrEtcMsg); // "지원하지 않음|Pass!|";
		m_StrIsCompressTexture = StrBuf;
											 
		StrEtcMsg.LoadString(IDS_MSG22);
		StrBuf.Format("%d. %s", ++iAlertNum, (LPCTSTR)StrEtcMsg); // "압축텍스쳐를 지원하지 않으면 게임속도가 저하됩니다.!\r\n\r\n");
		m_StrInfoEdit += StrBuf;
	}


	SpecSoundCard(szInfoBuf);
	StrBuf.Format("%s|", szInfoBuf);
	m_StrSound += StrBuf;
	if ( !strcmp("No Driver", (LPCTSTR)m_StrSound) )
	{
		StrEtcAlert.LoadString( IDS_SOUND_ALERT );
		StrEtcMsg.LoadString(IDS_MSG23);
		StrBuf.Format("%d. %s", ++iAlertNum, (LPCTSTR)StrEtcMsg );// "사운드 카드가 없거나 드라이버를 인식하지 못하였습니다!\r\n\r\n");
		m_StrInfoEdit += StrBuf;
		
		StrBuf = "None|";
		m_StrSound += StrBuf;
	}
	else
	{
		StrBuf = "Pass!|";
		m_StrSound += StrBuf;
	}

	// last message
	if (m_StrInfoEdit.IsEmpty())
	{
		StrEtcMsg.LoadString(IDS_MSG24);
		m_StrInfoEdit = (LPCTSTR)StrEtcMsg; // "당신은 코룸온라인을 100% 즐기실 준비가 되어있군요~~ ^^";
	}
	else
	{
		StrEtcMsg.LoadString(IDS_MSG25);
		m_StrInfoEdit += (LPCTSTR)StrEtcMsg; // "\r\n\r\n위의 항목을 확인하고 필요하다면\r\n고객센터 바로가기를 클릭 하시기 바랍니다.";
	}

	UpdateData(FALSE);
}



void CPc_statusDlg::OnButtonPrintFile() 
{
	// TODO: Add your control notification handler code here
	/*
		CFileDialog( BOOL bOpenFileDialog, LPCTSTR lpszDefExt = NULL, 
			LPCTSTR lpszFileName = NULL, 
			DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
			LPCTSTR lpszFilter = NULL, 
			CWnd* pParentWnd = NULL );
	*/

	CString StrEtcMsg;
	char szFullPath[1024];
	LPITEMIDLIST	pidl;
	LPMALLOC		pMalloc;

	::SHGetSpecialFolderLocation((HWND)GetSafeHwnd(), CSIDL_DESKTOP, &pidl);
	SHGetPathFromIDList(pidl, szFullPath);
	StrEtcMsg.LoadString(IDS_MSG26);
	strcat(szFullPath, (LPCTSTR)StrEtcMsg); // "\\코룸온라인 시스템 진단 리포트.txt");

	SHGetMalloc(&pMalloc);
	pMalloc->Free(pidl);
	pMalloc->Release();


	FILE * fp;
	if ( !(fp = fopen(szFullPath, "wt")) )
	{
		MessageBox("Error : File Not Open", "Error");
		return;
	}	

	if ( !SaveSystemSpec(fp) )
	{ // FAIL
		MessageBox("Fail Print to File..");
		return;
	}

	fclose(fp);

	STARTUPINFO suInfo;
	PROCESS_INFORMATION procInfo;
	CString		StrCvtFullPath;		

	memset (&suInfo, 0, sizeof(suInfo));
	suInfo.cb = sizeof(suInfo);

	StrCvtFullPath = "Notepad.exe ";
	StrCvtFullPath += szFullPath;
	
	CreateProcess(NULL, (LPSTR)(LPCTSTR)StrCvtFullPath, NULL,NULL, TRUE, 
		CREATE_NEW_CONSOLE, NULL, NULL, &suInfo, &procInfo);
}

int CPc_statusDlg::SaveSystemSpec(FILE *fp)
{
	fprintf(fp, "=====================================================\n");
	fprintf(fp, "\t\tMY PC STATUS\n");
	fprintf(fp, "=====================================================\n");
// SYSTEM INFO
	fprintf(fp, "=================\n");
	fprintf(fp, " SYSTEM INFO\n");
	fprintf(fp, "=================\n");
	SYSTEMTIME	st;
	GetLocalTime(&st);
	fprintf(fp, "Time of this report: %2d/%2d/%2d, %2d:%2d:%2d\n\n", st.wMonth, st.wDay, st.wYear, st.wHour, st.wMinute, st.wSecond);

	
//	fprintf(fp, "Machine Name : %s\n\n", GetMachineName() )

	fprintf(fp, "Process : %s\n\n", (LPCTSTR)GetSomeSpecInfo(m_StrCPU));
	fprintf(fp, "Opersating System : %s\n\n", (LPCTSTR)GetSomeSpecInfo(m_StrOS));
	fprintf(fp, "Memory : %s\n\n", (LPCTSTR)GetSomeSpecInfo(m_StrRAM));
 	fprintf(fp, "Installed HardDisk Drive Remained Spaces : %s\n", (LPCTSTR)GetSomeSpecInfo(m_StrRemainedHard));
	for (char ch = 'C'; ch < 'Z'; ch++)
	{
		char szDrive[5] = {0,};
		ULONG	uli_space = 0;
		
		sprintf(szDrive, "%c:\\", ch);
		uli_space = (ULONG)SpecHardDisk(szDrive);
		if (0 != uli_space)
			fprintf(fp, "   - %c:\\ : %ld MB Remained Spaces\n", ch, (ULONG)SpecHardDisk(szDrive));
	}
	fprintf(fp, "\n");
	fprintf(fp, "DirectX Version : %s\n\n", (LPCTSTR)GetSomeSpecInfo(m_StrDXVer));
	fprintf(fp, "Sound : %s\n\n", (LPCTSTR)GetSomeSpecInfo(m_StrSound));
// -- SYSTEM INFO

// DISPLAY INFO
	fprintf(fp, "=================\n");
	fprintf(fp, " DISPLAY INFO\n");
	fprintf(fp, "=================\n");
	
	fprintf(fp, "Graphic Card Name : %s\n\n", (LPCTSTR)GetSomeSpecInfo(m_StrGraphicChip));
	fprintf(fp, "Graphic Drive Version : %s\n\n", (LPCTSTR)GetSomeSpecInfo(m_StrGraphicDrv));
	fprintf(fp, "Grapihc Memory : %s\n\n", (LPCTSTR)GetSomeSpecInfo(m_StrGraphicMem));
	fprintf(fp, "AGP Memory : %s\n\n", (LPCTSTR)GetSomeSpecInfo(m_StrGraphicAGP));
	fprintf(fp, "Display Mode : %s\n\n", (LPCTSTR)GetSomeSpecInfo(m_StrDisplay));
	fprintf(fp, "Compressed Texture Supported : %s\n\n", (LPCTSTR)GetSomeSpecInfo(m_StrIsCompressTexture));
	fprintf(fp, "=====================================================");
// -- DISPLAY INFO
	return TRUE;
}




void CPc_statusDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (m_hBitmap)
		DeleteObject(m_hBitmap);	//not really need but what the heck.
	CDialog::OnClose();
}

void CPc_statusDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));	
	CDialog::OnLButtonDown(nFlags, point);
}

void CPc_statusDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	// Load the image
/*	m_hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), "pc_status.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (m_hBitmap == NULL)
	{
		MessageBox("Error loading bitmap");
		return;
	}
	//Get information about the bitmap..
	GetObject(m_hBitmap, sizeof(m_Bitmap), &m_Bitmap);	// Get info about the bitmap 
	// Put the bitmap into a memory device context
	CPaintDC dc(this);
	//get a memory dc object
	CDC dcMem;
	//create a compatible dc
	dcMem.CreateCompatibleDC(&dc);	// Select the bitmap into the in-memory DC
	//Select the bitmap into the dc
	CBitmap* pOldBitmap = dcMem.SelectObject(CBitmap::FromHandle(m_hBitmap)); 
	//Create a couple of region objects.
	CRgn crRgn, crRgnTmp;
	//create an empty region
	crRgn.CreateRectRgn(0, 0, 0, 0);
	//Create a region from a bitmap with transparency colour of Purple
	COLORREF crTransparent = TRANSPARENTCOLOR;	
	int iX = 0;
	for (int iY = 0; iY < m_Bitmap.bmHeight; iY++)
	{
		do
		{
			//skip over transparent pixels at start of lines.
			while (iX <= m_Bitmap.bmWidth && dcMem.GetPixel(iX, iY) == crTransparent)
				iX++;
			//remember this pixel
			int iLeftX = iX;
			//now find first non transparent pixel
			while (iX <= m_Bitmap.bmWidth && dcMem.GetPixel(iX, iY) != crTransparent)
				++iX;
			//create a temp region on this info
			crRgnTmp.CreateRectRgn(iLeftX, iY, iX, iY+1);
			//combine into main region.
			crRgn.CombineRgn(&crRgn, &crRgnTmp, RGN_OR);
			//delete the temp region for next pass (otherwise you'll get an ASSERT)
			crRgnTmp.DeleteObject();
		} while(iX < m_Bitmap.bmWidth);
		iX = 0;
	}
	//Centre it on current desktop
	SetWindowRgn(crRgn, TRUE);
	iX = (GetSystemMetrics(SM_CXSCREEN)) / 2 - (m_Bitmap.bmWidth / 2);
	iY = (GetSystemMetrics(SM_CYSCREEN)) / 2 - (m_Bitmap.bmHeight / 2);
	SetWindowPos(&wndTopMost, iX, iY, m_Bitmap.bmWidth, m_Bitmap.bmHeight, NULL); 

	// Free resources.
	dcMem.SelectObject(pOldBitmap);	// Put the original bitmap back (prevents memory leaks)
	dcMem.DeleteDC();
	crRgn.DeleteObject();

  */
}

void CPc_statusDlg::OnStaticLogo() 
{
	// TODO: Add your control notification handler code here
	//ShellExecute(NULL, "OPEN", "WWW.CORUMONLINE.CO.KR", NULL, NULL, SW_SHOWNORMAL);
	STARTUPINFO suInfo;
	PROCESS_INFORMATION procInfo;
	CString		StrCvtFullPath;		// \ -> \\ 형식으로 바꿔준다. 
	CString		StrEtcMsg;
	CString		StrSite;

	memset (&suInfo, 0, sizeof(suInfo));
	suInfo.cb = sizeof(suInfo);
	
	StrEtcMsg.LoadString(IDS_MSG27);
	StrSite.Format("explorer.exe %s", (LPCTSTR)StrEtcMsg);
	// "explorer.exe http://www.corumonline.co.kr";
	CreateProcess(NULL, (LPSTR)(LPCTSTR)StrSite, NULL,NULL, TRUE, 
		CREATE_NEW_CONSOLE, NULL, NULL, &suInfo, &procInfo);
}

void CPc_statusDlg::OnStaticClinetpageGo() 
{
	// TODO: Add your control notification handler code here
	STARTUPINFO suInfo;
	PROCESS_INFORMATION procInfo;
	CString		StrCvtFullPath;		// \ -> \\ 형식으로 바꿔준다. 
	CString		StrEtcMsg;
	CString		StrSite;

	memset (&suInfo, 0, sizeof(suInfo));
	suInfo.cb = sizeof(suInfo);
	

	StrEtcMsg.LoadString(IDS_MSG28);
	StrSite.Format("explorer.exe %s", (LPCTSTR)StrEtcMsg);
	// "explorer.exe http://www.corumonline.co.kr/Support_Zone"
	CreateProcess(NULL, (LPSTR)(LPCTSTR)StrSite, NULL,NULL, TRUE, 
		CREATE_NEW_CONSOLE, NULL, NULL, &suInfo, &procInfo);	
}




void CPc_statusDlg::OnButtonExit() 
{
	// TODO: Add your control notification handler code here
	SendMessage(WM_CLOSE);
}

void CPc_statusDlg::SetSpecList()
{
	m_CtrlSpecList.InsertRecordString((LPCTSTR)m_StrCPU);
	m_CtrlSpecList.InsertRecordString((LPCTSTR)m_StrOS );
	m_CtrlSpecList.InsertRecordString((LPCTSTR)m_StrRAM);
	m_CtrlSpecList.InsertRecordString((LPCTSTR)m_StrRemainedHard);
	m_CtrlSpecList.InsertRecordString((LPCTSTR)m_StrGraphicChip);
	m_CtrlSpecList.InsertRecordString((LPCTSTR)m_StrGraphicDrv);
	m_CtrlSpecList.InsertRecordString((LPCTSTR)m_StrGraphicMem);
	m_CtrlSpecList.InsertRecordString((LPCTSTR)m_StrGraphicAGP);
	m_CtrlSpecList.InsertRecordString((LPCTSTR)m_StrIsCompressTexture);
	m_CtrlSpecList.InsertRecordString((LPCTSTR)m_StrDXVer);
	m_CtrlSpecList.InsertRecordString((LPCTSTR)m_StrSound);
}

BOOL CPc_statusDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

		
	return CDialog::OnCommand(wParam, lParam);
}

UINT CPc_statusDlg::OnGetDlgCode() 
{
	// TODO: Add your message handler code here and/or call default
	
	return DLGC_WANTMESSAGE;
}

LRESULT CPc_statusDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	unsigned int wNotifyCode = HIWORD(wParam); 
	unsigned int wID = LOWORD(wParam); 

	switch(wID) 
	{
	case WM_COMMAND:
		if (wID == IDC_SPEC_LIST)
		{
			__asm nop 
		}
		break;
	default:
		break;
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}

//
// 각 스펙 정보의 필요한 내용만 가져온다. 
// 
CString CPc_statusDlg::GetSomeSpecInfo(CString & rString)
{
	CString StrBuf;
	int i;
	int pos = 0, pos_To = 0, retPos = 0;
	for (i = 0; i < 2; i++)
	{
		if (i == 0)
			pos = rString.Find("|", pos);
		else
			pos = rString.Find("|", pos+1);

	}


	pos_To = rString.Find("|", pos+1);

	retPos = pos_To - pos;
	if (retPos > 0)
		StrBuf = rString.Mid(pos+1, retPos-1);
	
	return StrBuf;
}
