#if !defined(AFX_COPYITEMVIEW_H__E701D1AF_8DA7_4E20_8F51_4BEFA3457E52__INCLUDED_)
#define AFX_COPYITEMVIEW_H__E701D1AF_8DA7_4E20_8F51_4BEFA3457E52__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


struct HAVEITEMEX
{
	__int64 i64Serial;
	char	szCharName[20];
	WORD	wNo;
	BYTE	bwhere;
	BYTE	bindex;
};


#include "ProgressCtrlX.h"


class CCopyItemView : public CFormView
{

protected:

	CCopyItemView();           
	DECLARE_DYNCREATE(CCopyItemView)

public:

	//{{AFX_DATA(CCopyItemView)
	enum { IDD = IDD_COPYITEM };
	CComboBox	m_ComBoCtrl;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCopyItemView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	virtual ~CCopyItemView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//{{AFX_MSG(CCopyItemView)
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeCombo();
	afx_msg void OnGo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	
	CApp*						m_pApp;
	int							m_nViewID;
	LOGFONT						m_logFont;
	CFont						m_Font;
	CEdit*						m_pEdit;
	CProgressCtrlX*				m_pProgress;

public:
	
	CWinThread*					m_pWinThread;
	BOOL						m_bRun;
	
public:
	
	HANDLE						m_hThread;
	
public:
	
	IOleDBThread*				m_pDb;

public:
	
	char						m_CORUM_DB_NAME		[32];		
	char						m_CORUM_DB_ACCOUNT	[32];		
	char						m_CORUM_DB_PASSWORD	[32];	
	char						m_CORUM_DB_ADDRESS	[32];

	BYTE						m_CORUM_BACKUP_DB;
	DBSCHEMA					m_DbSchema			[MAX_DB_COUNT];
	int							m_nDbCount;

public:

	vector<HAVEITEMEX>			m_haveList;	

public:

	void						StepProgress			();
	BOOL						InitDatabase			();
	BOOL						InitCorumDB				();
	void						InitDBAccount			();
	int							RemoveNotNumber			(CString &strNumber);
	void						CopyItemSearch			();	
	void						ItemSearch				();
};


#endif 





















