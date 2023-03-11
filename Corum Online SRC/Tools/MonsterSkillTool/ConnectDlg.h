#if !defined(AFX_CONNECTDLG_H__B523F177_34B2_4B40_AB66_28DBF3693152__INCLUDED_)
#define AFX_CONNECTDLG_H__B523F177_34B2_4B40_AB66_28DBF3693152__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConnectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConnectDlg dialog

class CConnectDlg : public CDialog
{
// Construction
public:
	CConnectDlg(CWnd* pParent = NULL);   // standard constructor

	LPCSTR CConnectDlg::GetDataSourceName();
	LPCSTR CConnectDlg::GetDatabaseName();
	LPCSTR CConnectDlg::GetAccount();
	LPCSTR CConnectDlg::GetPassword();
	DWORD CConnectDlg::GetTimeout();

// Dialog Data
	//{{AFX_DATA(CConnectDlg)
	enum { IDD = IDD_DIALOG_CONNET };
	CString	m_strAccount;
	CString	m_strDatabase;
	CString	m_strDataSource;
	CString	m_strPassword;
	UINT	m_nTimeout;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConnectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConnectDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNECTDLG_H__B523F177_34B2_4B40_AB66_28DBF3693152__INCLUDED_)
