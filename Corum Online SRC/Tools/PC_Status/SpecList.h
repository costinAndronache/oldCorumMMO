#if !defined(AFX_SPECLIST_H__1556C463_492D_41EF_A7F0_95A498557566__INCLUDED_)
#define AFX_SPECLIST_H__1556C463_492D_41EF_A7F0_95A498557566__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpecList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpecList window

class CSpecList : public CListCtrl
{
// Construction
public:
	CSpecList();

// Attributes
public:
	int			m_iRecords;			// 레코드 수 
	CImageList	m_SmallImageList;	// 작은 아이콘 이미지 리스트


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpecList)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	int AddIconToList(int iIconID);
	bool InsertRecordString(const char * pszContents);
	void InitTitleString(char * pszTitles);
	virtual ~CSpecList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSpecList)
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPECLIST_H__1556C463_492D_41EF_A7F0_95A498557566__INCLUDED_)
