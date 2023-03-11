#if !defined(AFX_ADDVERSIONDIALOG_H__E2910B31_5E2C_4486_B060_7B9D822A2DC3__INCLUDED_)
#define AFX_ADDVERSIONDIALOG_H__E2910B31_5E2C_4486_B060_7B9D822A2DC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddVersionDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// AddVersionDialog dialog
#include "VDFFile.h"

class AddVersionDialog : public CDialog
{
// Construction
public:
	AddVersionDialog(CWnd* pParent = NULL);   // standard constructor
	void GetVersionData(LPVERSION_DATA pData);
// Dialog Data
	//{{AFX_DATA(AddVersionDialog)
	enum { IDD = IDD_DIALOG_ADD_VERSION };
	CString	m_strDescription;
	CString	m_strVersion;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AddVersionDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(AddVersionDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDVERSIONDIALOG_H__E2910B31_5E2C_4486_B060_7B9D822A2DC3__INCLUDED_)
