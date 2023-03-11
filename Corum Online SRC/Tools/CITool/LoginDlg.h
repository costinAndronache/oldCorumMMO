#if !defined(AFX_LOGINDLG_H__2D27A941_73E0_442C_A2B1_1B9C4F18AE1B__INCLUDED_)
#define AFX_LOGINDLG_H__2D27A941_73E0_442C_A2B1_1B9C4F18AE1B__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


class CLoginDlg : public CDialog
{

public:

	CLoginDlg(CWnd* pParent = NULL);  

	//{{AFX_DATA(CLoginDlg)
	enum { IDD = IDD_LOGIN };
	CString	m_szID;
	CString	m_szPassWord;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CLoginDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CLoginDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:

	void	EnterAtPassword		();
	int		CheckID				(CString id);
};

#endif 
