// pc_statusDlg.h : header file
//

#if !defined(AFX_PC_STATUSDLG_H__42BD323F_5732_489C_855C_4F355D30757F__INCLUDED_)
#define AFX_PC_STATUSDLG_H__42BD323F_5732_489C_855C_4F355D30757F__INCLUDED_

#include "cpu_info.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPc_statusDlg dialog

#include "iconButton.h"
#include "newlabel.h"
#include "speclist.h"
#include "XListCtrl.h"

#define TRANSPARENTCOLOR RGB(255, 0, 255);
class CPc_statusDlg : public CDialog
{
// Construction
public:
	CString GetSomeSpecInfo(CString & rString);
	void SetSpecList();

	CPc_statusDlg(CWnd* pParent = NULL);	// standard constructor
	~CPc_statusDlg() {}
	
	BITMAP			m_Bitmap;		// Struct to hold info about the bitmap
	HBITMAP			m_hBitmap;		// Handle of the bitmap
	
	int SaveSystemSpec(FILE *fp);
	void SpecSystem();
	
// Dialog Data
	//{{AFX_DATA(CPc_statusDlg)
	enum { IDD = IDD_PC_STATUS_DIALOG };
	CSpecList	m_CtrlSpecList;
	CNewLabel	m_CtrlIsCompressTextureStatic;
	CNewLabel	m_CtrlOSStatic;
	CNewLabel	m_CtrlCPUStatic;
	CNewLabel	m_CtrlSoundStatic;
	CNewLabel	m_CtrlRemainedHardStatic;
	CNewLabel	m_CtrlRamStatic;
	CNewLabel	m_CtrlGraphicMemStatic;
	CNewLabel	m_CtrlGraphicDRVVerStatic;
	CNewLabel	m_CtrlGraphicChipStatic;
	CNewLabel	m_CtrlGraphicAGPMemStatic;
	CNewLabel	m_CtrlDXVerStatic;
	CNewLabel	m_CtrlDisplayStatic;
	CEdit		m_CtrlCpuEdit;
	CNewLabel	m_CtrlGoClientPageStatic;
	CNewLabel	m_CtrlLogoStatic;
	CNewLabel	m_CtrlClientSupportStatic;
	CNewLabel	m_CtrlNewLabelStatic;
	CString	m_StrInfoEdit;
	CString	m_StrCPU;
	CString	m_StrDisplay;
	CString	m_StrDXVer;
	CString	m_StrGraphicAGP;
	CString	m_StrGraphicChip;
	CString	m_StrGraphicDrv;
	CString	m_StrGraphicMem;
	CString	m_StrOS;
	CString	m_StrRAM;
	CString	m_StrRemainedHard;
	CString	m_StrSound;
	CString	m_StrIsCompressTexture;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPc_statusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPc_statusDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonPrintFile();
	afx_msg void OnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnStaticLogo();
	afx_msg void OnStaticClinetpageGo();
	afx_msg void OnButtonExit();
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PC_STATUSDLG_H__42BD323F_5732_489C_855C_4F355D30757F__INCLUDED_)
