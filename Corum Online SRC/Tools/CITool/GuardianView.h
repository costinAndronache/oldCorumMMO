#if !defined(AFX_GUARDIANVIEW_H__DB22FDAD_82F9_4847_B1B6_4D01DEE7B23C__INCLUDED_)
#define AFX_GUARDIANVIEW_H__DB22FDAD_82F9_4847_B1B6_4D01DEE7B23C__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

/*
#pragma pack(push,1)
typedef struct GUARDIAN_INFO
{
	DWORD	dwID;
	char	szName[50];
	DWORD	dwKind;
	DWORD	dwLevel;
	DWORD	dwExp;
	DWORD	dwHP;
	DWORD	dwMP;
} *LPGUARDIAN_INFO,GUARDIAN_INFO;
#pragma pack(pop)
*/

#include "ProgressCtrlX.h"


class CGuardianView : public CFormView
{

protected:
	CGuardianView(); 
	DECLARE_DYNCREATE(CGuardianView)

public:

	//{{AFX_DATA(CGuardianView)
	enum { IDD = IDD_GUARDIAN };
	CComboBox	m_ComBoCtrl;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CGuardianView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	virtual ~CGuardianView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//{{AFX_MSG(CGuardianView)
	afx_msg void OnGo();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	CApp*						m_pApp;
	int							m_nViewID;
	LOGFONT						m_logFont;
	CFont						m_Font;
	CEdit*						m_pEdit;
	CProgressCtrlX*				m_pProgress;
	DWORD						m_dwGuardian;

public:
	
	CWinThread*					m_pWinThread;
	BOOL						m_bRun;

public:

	HANDLE						m_hThread;

public:
	
	IOleDBThread*				m_pDb;
	
public:
	
	char						m_CORUM_DB_NAME		[CURRENT_DB_NUM][32];		
	char						m_CORUM_DB_ACCOUNT	[CURRENT_DB_NUM][32];		
	char						m_CORUM_DB_PASSWORD	[CURRENT_DB_NUM][32];	
	char						m_CORUM_DB_ADDRESS	[CURRENT_DB_NUM][32];
	char						m_CORUM_TDB_NAME	[32];		
	char						m_CORUM_TDB_ACCOUNT	[32];		
	char						m_CORUM_TDB_PASSWORD[32];	
	char						m_CORUM_TDB_ADDRESS	[32];
	BYTE						m_CORUM_GAME_DB		[CURRENT_DB_NUM];
	BYTE						m_CORUM_TOTAL_DB;

public:

	void						NewGuardian				();
	void						StepProgress			();
	void						InsertGuardianInfo		(BYTE bFindServerSet,CString szName,BYTE bWhere,BYTE bIndex,CItem GuardianItem);

	BOOL						InitDatabase			();
	BOOL						InitCorumDB				();
	void						InitDBAccount			();
};

#endif 
