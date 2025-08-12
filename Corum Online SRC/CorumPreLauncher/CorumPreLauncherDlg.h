
// CorumPreLauncherDlg.h : header file
//

#pragma once

#include "../CorumPreferences/CorumPreferences.h"
#include <string>

// CCorumPreLauncherDlg dialog
class CCorumPreLauncherDlg : public CDialogEx
{
// Construction
public:
	CCorumPreLauncherDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CORUMPRELAUNCHER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
      public:
        CComboBox _resolutionsList;
        CButton _windowModeCheck;
        afx_msg void OnBnClickedButton1();

	private:
		struct ResolutionsListItemModel {
			std::wstring text;
			CorumPreferences::Resolution resolution;
		};
		std::vector<ResolutionsListItemModel> _resolutionsListModels;

		void setupFromStoredPreferences();
		void storeCurrentPreferences();
		static std::vector<ResolutionsListItemModel> resolutionsListModels();

              public:
                afx_msg void OnBnClickedExitbutton();
};
