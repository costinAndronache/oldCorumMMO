#if !defined(AFX_FINDITEMNUMDLG_H__5F74297F_A897_454C_AAC9_4EDEE0D9D87C__INCLUDED_)
#define AFX_FINDITEMNUMDLG_H__5F74297F_A897_454C_AAC9_4EDEE0D9D87C__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#include "UIThread.h"
#include "ReportCtrl.h"


class CFindItemNumDlg : public CDialog
{
public:

	CFindItemNumDlg(CWnd* pParent = NULL);  

	//{{AFX_DATA(CFindItemNumDlg)
	enum { IDD = IDD_ITEM };
	CReportCtrl		m_List_SelectItem;
	CReportCtrl		m_List_Item;
	UINT			m_wID;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CFindItemNumDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL Create(CWnd* pParentWnd);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CFindItemNumDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectItemSave();
	afx_msg void OnRightMove();
	afx_msg void OnLeftMove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	void		FindItem	();

public:

	BOOL		m_bCreate;

public:
	
	CUIThread*	m_pUIThread;
};

#endif 
