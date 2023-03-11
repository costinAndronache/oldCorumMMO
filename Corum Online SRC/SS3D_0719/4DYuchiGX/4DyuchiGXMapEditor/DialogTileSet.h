#if !defined(AFX_DIALOGTILESET_H__681E2A47_0042_4742_94A9_D0CDEBA532D2__INCLUDED_)
#define AFX_DIALOGTILESET_H__681E2A47_0042_4742_94A9_D0CDEBA532D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogTileSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogTileSet dialog

#include "voidList.h"

class CTileSet;
class CDialogTilePalette;
class CDialogEditTileSet;

class CDialogTileSet : public CDialog
{
	CDialogTilePalette* m_pPaletteDlg;
	CDialogEditTileSet* m_pEditTileSetDlg;
	CString m_strSelectedTileSet;
	
	CVoidList m_listTileSet;

	void Clear();

	CTileSet* GetTileSet(CString& TileSetName);
public:
	CDialogTileSet(CWnd* pParent = NULL);   // standard constructor

	void Initialize(CDialogTilePalette* pPaletteDlg,CDialogEditTileSet* pEditTileSetDlg);
	void EnableView();
	void DisableView();

	inline CTileSet* GetSelectedTileSet()	{	return GetTileSet(m_strSelectedTileSet);	}

	void AddTileSet(CTileSet* pSet);
	
// Dialog Data
	//{{AFX_DATA(CDialogTileSet)
	enum { IDD = IDD_TILESET };
	CListBox	m_TileSetListBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogTileSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogTileSet)
	afx_msg void OnSelchangeList1();
	afx_msg void OnNew();
	afx_msg void OnSave();
	afx_msg void OnLoad();
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	afx_msg void OnModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTILESET_H__681E2A47_0042_4742_94A9_D0CDEBA532D2__INCLUDED_)
