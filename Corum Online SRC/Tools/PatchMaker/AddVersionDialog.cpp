// AddVersionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "patchmaker.h"
#include "AddVersionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AddVersionDialog dialog


AddVersionDialog::AddVersionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(AddVersionDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(AddVersionDialog)
	m_strDescription = _T("");
	m_strVersion = _T("");
	//}}AFX_DATA_INIT
}


void AddVersionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AddVersionDialog)
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_strDescription);
	DDV_MaxChars(pDX, m_strDescription, 256);
	DDX_Text(pDX, IDC_EDIT_VERSION_NAME, m_strVersion);
	DDV_MaxChars(pDX, m_strVersion, 8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AddVersionDialog, CDialog)
	//{{AFX_MSG_MAP(AddVersionDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AddVersionDialog message handlers

void AddVersionDialog::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	if(m_strVersion.GetLength() == 0)
	{
		CDialog::OnCancel();
	}
	else
	{
		CDialog::OnOK();
	}
}

void AddVersionDialog::GetVersionData(LPVERSION_DATA pData)
{
	lstrcpy(pData->szVersion, m_strVersion);
	GetLocalTime(&pData->timeBuilt);
	lstrcpy(pData->szDescription, m_strDescription);
}

BOOL AddVersionDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SYSTEMTIME time;
	GetLocalTime(&time);
	m_strVersion.Format(_T("%02d%02d%02d01"), time.wYear - 2000, time.wMonth, time.wDay);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
