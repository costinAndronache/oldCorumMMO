#if !defined(AFX_SKILLDESCDLG_H__DB8B2B81_624F_4486_9215_02A9FA812ABB__INCLUDED_)
#define AFX_SKILLDESCDLG_H__DB8B2B81_624F_4486_9215_02A9FA812ABB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkillDescDlg.h : header file
//

#include "IOleDBThread.h"
#include "structure.h"

/////////////////////////////////////////////////////////////////////////////
// CSkillDescDlg dialog




class CSkillDescDlg : public CDialog
{
// Construction
public:
	CSkillDescDlg(CWnd* pParent = NULL);   // standard constructor
	~CSkillDescDlg();

	void CSkillDescDlg::InitSkillDesc(SKILL_DESC_STRUCT* pStruct);
	int	CSkillDescDlg::GetSelectedSkillID();
	LPCSTR CSkillDescDlg::GetSelectedSkillName();


private:
	int					m_iSelectedSkill;
	CString				m_strSelectedSkill;
	SKILL_DESC_STRUCT*	m_pSkillDescStruct;

// Dialog Data
	//{{AFX_DATA(CSkillDescDlg)
	enum { IDD = IDD_DIALOG_SKILL_DESC };
	CListCtrl	m_lstSkillDesc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkillDescDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSkillDescDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnItemActivateListSkillDesc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKILLDESCDLG_H__DB8B2B81_624F_4486_9215_02A9FA812ABB__INCLUDED_)
