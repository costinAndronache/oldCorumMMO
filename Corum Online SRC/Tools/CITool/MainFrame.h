#if !defined(AFX_MAINFRAME_H__B7CCA507_D1AD_40F3_BBFD_F2FCD20AFD8A__INCLUDED_)
#define AFX_MAINFRAME_H__B7CCA507_D1AD_40F3_BBFD_F2FCD20AFD8A__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif


class CFindItemNumDlg;


class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)

public:

	CMainFrame();
	virtual ~CMainFrame();

	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  

	CStatusBar			m_wndStatusBar;
	CToolBar			m_wndToolBar;

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFindItem();
	afx_msg void OnNewSerial();
	afx_msg void OnGuardian();
	afx_msg void OnGuildInit();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:

	CFindItemNumDlg*	m_pFindItemNumDlg;
};

#endif 
