// PatchMakerDlg.h : header file
//

#if !defined(AFX_PATCHMAKERDLG_H__0D3336F9_795F_4782_843B_C05E89120925__INCLUDED_)
#define AFX_PATCHMAKERDLG_H__0D3336F9_795F_4782_843B_C05E89120925__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PatchEngine.h"
#include "ProgressCtrlX.h"

/////////////////////////////////////////////////////////////////////////////
// CPatchMakerDlg dialog

class CPatchMakerDlg : public CDialog
{
// Construction
public:
	CPatchMakerDlg(CWnd* pParent = NULL);	// standard constructor
	void	EnableButton(int nButtonID, BOOL bEnable);
	void	EnableAllButton(BOOL bEnable);
	void	InitProgressiveBar();
// Dialog Data
	//{{AFX_DATA(CPatchMakerDlg)
	enum { IDD = IDD_PATCHMAKER_DIALOG };
	CProgressCtrlX	m_ProgressCtrlSub;
	CProgressCtrlX	m_ProgressCtrl;
	CListCtrl		m_lstVersion;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatchMakerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPatchMakerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonAddVersion();
	afx_msg void OnButtonDeleteLastVersion();
	afx_msg void OnButtonLockLastVersion();
	afx_msg void OnButtonUnlockLastVersion();
	afx_msg void OnButtonBuildLastFull();
	afx_msg void OnButtonBuildLastPatch();
	afx_msg void OnButtonLoadAllVersion();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnButtonBuildPatchFilesWithBreaking();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	CPatchEngine	m_Engine;

private:

	CWinThread*		m_pWinThread;
	HANDLE			m_hThread;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATCHMAKERDLG_H__0D3336F9_795F_4782_843B_C05E89120925__INCLUDED_)
