#if !defined(AFX_LOGINDIALOG_H__72A7FA6D_684E_48C1_873C_7C4B3860BE3E__INCLUDED_)
#define AFX_LOGINDIALOG_H__72A7FA6D_684E_48C1_873C_7C4B3860BE3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoginDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLoginDialog dialog

class CLoginDialog : public CDialog
{
// Construction
public:
	CLoginDialog(CWnd* pParent = NULL);   // standard constructor
	CString& CLoginDialog::GetPassword();

// Dialog Data
	//{{AFX_DATA(CLoginDialog)
	enum { IDD = IDD_DIALOG_LOGIN };
	CString	m_strPwd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoginDialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINDIALOG_H__72A7FA6D_684E_48C1_873C_7C4B3860BE3E__INCLUDED_)
