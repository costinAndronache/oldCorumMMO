#pragma once

////////////////////////////////////////////////////////////////////////////
//							H I S T O R Y
//
//	Date:					Action:
//	.......................................................................
//  jun. 02					underline for the hotkey
//							icon left/right align
//	jul. 02					implement tip function for the button
//
////////////////////////////////////////////////////////////////////////////

const int TIPBGRCOLOR = RGB ( 255, 255, 0 );

class CIconButton : public CButton
{
public:
	CIconButton();
	virtual ~CIconButton();

	void SetItalic ( bool bVal = true )				{ m_TextMetrics.tmItalic = bVal; NewFont (); };
	void SetUnderline ( bool bVal = true )			{ m_TextMetrics.tmUnderlined = bVal; NewFont (); };
	void SetStrikeOut ( bool bVal = true )			{ m_TextMetrics.tmStruckOut = bVal; NewFont (); };
	void SetFaceName ( const CString &sVal )		{ m_sFaceName = sVal; NewFont (); };
	void SetWeight ( const int nVal )				{ m_TextMetrics.tmWeight = nVal; NewFont (); };
	void SetHeight ( const int nVal)				{ m_TextMetrics.tmHeight = nVal; NewFont (); };
	void SetWidth ( const int nVal)					{ m_TextMetrics.tmMaxCharWidth = nVal; NewFont (); };
	
	void SetIconID ( const UINT nID )				{ m_nIconID = nID; };
	void SetTextColor ( const COLORREF color )		{ m_nColor = color; };
	void SetIconSize ( const int x, const int y )	{ m_nIcon.cx = x; m_nIcon.cy = y; };
	void SetIconRight ( bool bVal = true )			{ m_bIconRight = bVal; };
	void Disable ( void );
	void Enable ( void );
	void SetTipText ( const CString &sTxt )			{ m_sTip = sTxt; m_bWithTip = true; };

	//{{AFX_VIRTUAL(CIconButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL


protected:
	//{{AFX_MSG(CIconButton)
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

	void	NewFont ( void );
	int		DelAmpersand ( CString &sTxt );
	void	HideTip ( void );
	void	ShowTip ( void );
	void	GetCurrentTextMetric ( void );
	void	DrawTipBoxAndText ( void );
	bool	RestoreBitmap ( void );
	bool	SaveBitmap ( void );
	//
	bool			m_bIconRight;
	TEXTMETRIC		m_TextMetrics;
	CSize			m_nIcon;
	UINT			m_nIconID;
	COLORREF		m_nColor;
	CFont			m_fnt;
	CString			m_sFaceName;
	//
	bool			m_bWithTip;
	bool			m_bTipIsOpen;
	CString			m_sTip;
	CSize			m_TipDim;
	CPoint			m_TipPos;
	CRect			m_TipRect;
	HBITMAP			m_hMemBmp;
	HDC				m_hMemDC;
	HDC				m_hOwnerDC;
};
