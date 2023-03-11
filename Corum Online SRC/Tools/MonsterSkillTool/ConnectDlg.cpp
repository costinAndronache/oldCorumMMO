// ConnectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MonsterSkillTool.h"
#include "ConnectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectDlg dialog


CConnectDlg::CConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectDlg::IDD, pParent)
{
	TCHAR szDataSource[0xff]={0,};
	TCHAR szDatabase[0xff]={0,};
	TCHAR szAccount[0xff]={0,};
	TCHAR szPassword[0xff]={0,};
	TCHAR szTimeout[0xff]={0,};

	GetPrivateProfileString(_T("MonsterSkillTool"), _T("DataSource"), _T("222.106.120.45"), szDataSource, 0xff, _T("./MonsterSkillTool.Ini"));
	GetPrivateProfileString(_T("MonsterSkillTool"), _T("Database"),	_T(""), szDatabase, 0xff, _T("./MonsterSkillTool.Ini"));
	GetPrivateProfileString(_T("MonsterSkillTool"), _T("Account"), _T(""), szAccount, 0xff, _T("./MonsterSkillTool.Ini"));
	GetPrivateProfileString(_T("MonsterSkillTool"), _T("Password"), _T(""), szPassword, 0xff, _T("./MonsterSkillTool.Ini"));
	GetPrivateProfileString(_T("MonsterSkillTool"), _T("Timeout"), _T("20"), szTimeout, 0xff, _T("./MonsterSkillTool.Ini"));

	//{{AFX_DATA_INIT(CConnectDlg)
	m_strAccount = szAccount;
	m_strDatabase = szDatabase;
	m_strDataSource = szDataSource;
	m_strPassword = szPassword;
	m_nTimeout = atoi(szTimeout);
	//}}AFX_DATA_INIT
}


void CConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectDlg)
	DDX_Text(pDX, IDC_EDIT_ACCOUNT, m_strAccount);
	DDX_Text(pDX, IDC_EDIT_DATABASE, m_strDatabase);
	DDX_Text(pDX, IDC_EDIT_DATASOURCE, m_strDataSource);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT, m_nTimeout);
	DDV_MinMaxUInt(pDX, m_nTimeout, 20, 2000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectDlg, CDialog)
	//{{AFX_MSG_MAP(CConnectDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectDlg message handlers

void CConnectDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

BOOL CConnectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LPCSTR CConnectDlg::GetDataSourceName()
{
	return (LPCSTR)(LPCTSTR)m_strDataSource;
}

LPCSTR CConnectDlg::GetDatabaseName()
{
	return (LPCSTR)(LPCTSTR)m_strDatabase;
}

LPCSTR CConnectDlg::GetAccount()
{
	return (LPCSTR)(LPCTSTR)m_strAccount;
}

LPCSTR CConnectDlg::GetPassword()
{
	return (LPCSTR)(LPCTSTR)m_strPassword;
}

DWORD CConnectDlg::GetTimeout()
{
	return m_nTimeout;
}
