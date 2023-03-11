// MonsterSkillToolDlg.h : header file
//

#if !defined(AFX_MONSTERSKILLTOOLDLG_H__B558D706_39A9_4190_81BE_50A80FBA5023__INCLUDED_)
#define AFX_MONSTERSKILLTOOLDLG_H__B558D706_39A9_4190_81BE_50A80FBA5023__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOleDBThread.h"
#include "DBStruct.h"
#include "SkillDescDlg.h"
#include "structure.h"
/////////////////////////////////////////////////////////////////////////////
// CMonsterSkillToolDlg dialog

// db 관련 메시지!!
#define WM_DB_RESULT	WM_USER+22




class CMonsterSkillToolDlg : public CDialog
{
// Construction
public:
	CMonsterSkillToolDlg(CWnd* pParent = NULL);	// standard constructor

	// OleDBThread 관련 멤버.
	void CMonsterSkillToolDlg::GetDBResult(DWORD dwVal);
	void CMonsterSkillToolDlg::InitOleDBThreadDll();
	void CMonsterSkillToolDlg::CreateConnection();
	void CMonsterSkillToolDlg::ConnectDatabase(char* szDataSource, char* szDefaultDb, char* szUserId, char* szPassword, int nwConnectTimeout = 20);
	void CMonsterSkillToolDlg::ChangeDB(char* szDb);

	void CMonsterSkillToolDlg::InitSkillDesc();
	void CMonsterSkillToolDlg::InitBaseMonsterData();
	void CMonsterSkillToolDlg::UpdateBaseMonsterInfo();

// Dialog Data
	//{{AFX_DATA(CMonsterSkillToolDlg)
	enum { IDD = IDD_MONSTERSKILLTOOL_DIALOG };
	CComboBox	m_cmbCurrentDB;
	CListCtrl	m_lstMonster;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonsterSkillToolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

private:
	IOleDBThread*	m_pDb;
	int				m_iConnection;

	SKILL_DESC_STRUCT	m_nSkillDescStruct;
	MONSTER_SKILL_INFO* m_pMonsterSkillInfo;
	int					m_nMonsterCount;

	// Generated message map functions
	//{{AFX_MSG(CMonsterSkillToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnItemActivateListMonster(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonChangeDb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONSTERSKILLTOOLDLG_H__B558D706_39A9_4190_81BE_50A80FBA5023__INCLUDED_)
