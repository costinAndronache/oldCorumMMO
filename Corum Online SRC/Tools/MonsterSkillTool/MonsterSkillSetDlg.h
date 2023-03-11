#if !defined(AFX_MONSTERSKILLSETDLG_H__2CEC9F93_D9ED_4172_8FB9_9F5F7F48C583__INCLUDED_)
#define AFX_MONSTERSKILLSETDLG_H__2CEC9F93_D9ED_4172_8FB9_9F5F7F48C583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MonsterSkillSetDlg.h : header file
//

#include "SkillDescDlg.h"
#include "structure.h"


/////////////////////////////////////////////////////////////////////////////
// CMonsterSkillSetDlg dialog

class CMonsterSkillSetDlg : public CDialog
{
// Construction
public:
	CMonsterSkillSetDlg(CWnd* pParent = NULL);   // standard constructor

	void InitMonsterSkillInfo(MONSTER_SKILL_INFO* pInfo, SKILL_DESC_STRUCT* pSkillDescStruct);
	CString* CMonsterSkillSetDlg::NewSkillSelect(int nIndex);

// Dialog Data
	//{{AFX_DATA(CMonsterSkillSetDlg)
	enum { IDD = IDD_DIALOG_MONSTER_SKILL_SET };
	CString	m_strName;
	int		m_nNonSkillRate;
	int		m_nUseSkillRate;
	int		m_nID;
	CString	m_strSkill1;
	int		m_nSkillLev1;
	CString	m_strSkill2;
	CString	m_strSkill3;
	CString	m_strSkill4;
	CString	m_strSkill5;
	CString	m_strSkill6;
	CString	m_strSkill7;
	CString	m_strSkill8;
	CString	m_strSkill9;
	CString	m_strSkill10;
	CString	m_strSkill11;
	CString	m_strSkill12;
	CString	m_strSkill13;
	CString	m_strSkill14;
	CString	m_strSkill15;
	int		m_nSkillLev3;
	int		m_nSkillLev2;
	int		m_nSkillLev4;
	int		m_nSkillLev5;
	int		m_nSkillLev6;
	int		m_nSkillLev7;
	int		m_nSkillLev8;
	int		m_nSkillLev9;
	int		m_nSkillLev10;
	int		m_nSkillLev11;
	int		m_nSkillLev12;
	int		m_nSkillLev13;
	int		m_nSkillLev14;
	int		m_nSkillLev15;
	//}}AFX_DATA

private:

	MONSTER_SKILL_INFO* m_pMonsterSkillInfo;
	SKILL_DESC_STRUCT*	m_pSkillDescStruct;
	CSkillDescDlg		m_SkillDescDlg;
	int					m_nSkillID[15];


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonsterSkillSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMonsterSkillSetDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonSkill1();
	afx_msg void OnButtonSkill2();
	afx_msg void OnButtonSkill3();
	afx_msg void OnButtonSkill4();
	afx_msg void OnButtonSkill5();
	afx_msg void OnButtonSkill6();
	afx_msg void OnButtonSkill7();
	afx_msg void OnButtonSkill8();
	afx_msg void OnButtonSkill9();
	afx_msg void OnButtonSkill10();
	afx_msg void OnButtonSkill11();
	afx_msg void OnButtonSkill12();
	afx_msg void OnButtonSkill13();
	afx_msg void OnButtonSkill14();
	afx_msg void OnButtonSkill15();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONSTERSKILLSETDLG_H__2CEC9F93_D9ED_4172_8FB9_9F5F7F48C583__INCLUDED_)
