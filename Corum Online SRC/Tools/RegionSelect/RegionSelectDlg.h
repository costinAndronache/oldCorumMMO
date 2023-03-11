// RegionSelectDlg.h : header file
//

#if !defined(AFX_REGIONSELECTDLG_H__728F0CEF_175F_4ED5_BB05_7D917F8AF589__INCLUDED_)
#define AFX_REGIONSELECTDLG_H__728F0CEF_175F_4ED5_BB05_7D917F8AF589__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRegionSelectDlg dialog

#define MAX_SERVER_SET_NAME_LENGTH 32

class CRegionSelectDlg : public CDialog
{
// Construction
public:
	CRegionSelectDlg(CWnd* pParent = NULL);	// standard constructor
	void CRegionSelectDlg::LoadRegionInfos();
	int CRegionSelectDlg::GetSelectedRegionCode();
	void CRegionSelectDlg::ExecuteAutopatcher(int iRegionCode);
	void CRegionSelectDlg::SetCurrentDirectoryToFolderOfExecutable();
// Dialog Data
	//{{AFX_DATA(CRegionSelectDlg)
	enum { IDD = IDD_REGIONSELECT_DIALOG };
	CListBox	m_lstRegion;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegionSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRegionSelectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDblclkListRegion();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGIONSELECTDLG_H__728F0CEF_175F_4ED5_BB05_7D917F8AF589__INCLUDED_)
