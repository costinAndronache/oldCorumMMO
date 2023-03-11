// SpecList.cpp : implementation file
//

#include "stdafx.h"
#include "pc_status.h"
#include "SpecList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpecList

CSpecList::CSpecList()
{
	m_iRecords = 0;
}

CSpecList::~CSpecList()
{
}


BEGIN_MESSAGE_MAP(CSpecList, CListCtrl)
	//{{AFX_MSG_MAP(CSpecList)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginlabeledit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpecList message handlers


//
//
//
void CSpecList::InitTitleString(char *pszTitles)
{
	CString		strBuf;

	LV_COLUMN COL;		// ��� ��Ʈ��

	COL.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT |LVCF_SUBITEM;
	COL.fmt=LVCFMT_LEFT;

	COL.cx=90;
	strBuf.LoadString(IDS_HEADER1);
	COL.pszText = (LPSTR)(LPCTSTR)strBuf;			// ù��° ���
	COL.iSubItem = 0;
	InsertColumn(0,&COL);


	COL.cx = 240;
	strBuf.LoadString(IDS_HEADER2);
	COL.pszText = (LPSTR)(LPCTSTR)strBuf;			// �ι�° ���
	COL.iSubItem = 1;
	InsertColumn(1,&COL);

	COL.cx=240;
	strBuf.LoadString(IDS_HEADER3);
	COL.pszText=(LPSTR)(LPCTSTR)strBuf;			// ����° ���
	COL.iSubItem = 2;
	InsertColumn(2,&COL);

	strBuf.LoadString(IDS_HEADER4);
	COL.cx=68;
	COL.pszText=(LPSTR)(LPCTSTR)strBuf;			// �׹�° ���
	COL.iSubItem=3;
	InsertColumn(3,&COL);
}


//
// ���ڵ带 �߰��ϴ� �Լ�
// �ε����� 1���� �����Ѵ�. 
// ���� ������ 0���� ����.
bool CSpecList::InsertRecordString(const char * pszContents)
{
	char	szText[512] = {0, };	// �ʵ� �ؽ�Ʈ  
	int		iContentPos = 0;		// ��ü ���� ��ġ 
	int		iPos = 0;				// �ʵ� �ؽ�Ʈ ��ġ 
	int		iFieldOrder = 0;		// �ʵ� ���� 
	char	szToken		= '|';		// ������

	LV_ITEM LI;
	// �ؽ�Ʈ�� �̹����� ���� �����۵��� ����Ѵ�.
	LI.mask=LVIF_TEXT|LVIF_IMAGE;
	LI.state=0;
	LI.stateMask=0;

	while (pszContents[iContentPos])
	{
		if (pszContents[iContentPos] == szToken)
		{
			szText[iPos] = '\0';

			if (iFieldOrder == 0)
			{ // ù �ʵ� �̴�.
				LI.iItem		= m_iRecords;
				LI.iSubItem		= iFieldOrder;
				LI.pszText		= szText;
				LI.cchTextMax	= 255;
				InsertItem(&LI);		

				// ������ �Ǵ°��� �����ش�.
				if (strstr(szText, "Warning!") || strstr(szText, "Stop!"))
					SetHotItem(m_iRecords);
				
				memset(szText, 0, 512);

				iPos = 0;
			}
			else
			{
				LI.iSubItem		= iFieldOrder;
				LI.pszText		= szText;		
				SetItem(&LI);			

				if (strstr(szText, "Warning!") || strstr(szText, "Stop!"))
					SetHotItem(m_iRecords);

				memset(szText, 0, 512);
				
				iPos = 0;
			}

			++iFieldOrder;
		}
		else
		{
			szText[iPos] = pszContents[iContentPos];
			++iPos;
		}

		++iContentPos;
	}

	m_iRecords++;

	if ( !iFieldOrder ) 
		return FALSE;		// �ʵ尡 �ϳ��� �߰����� �ʾҴ�.
	else
		return TRUE;
}

BOOL CSpecList::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CSpecList::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	
	
	return CListCtrl::OnCommand(wParam, lParam);
}

LRESULT CSpecList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	unsigned int wNotifyCode = HIWORD(wParam); 
	unsigned int wID = LOWORD(wParam); 

	switch(wID) 
	{
	case WM_COMMAND:
		if (wID == IDC_SPEC_LIST)
		{
			__asm nop 
		}
		break;
	default:
		break;
	}
	return CListCtrl::WindowProc(message, wParam, lParam);
}

void CSpecList::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CSpecList::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
	return;
}

int CSpecList::AddIconToList(int iIconID)
{
	CPc_statusApp * pApp = (CPc_statusApp *)AfxGetApp();
	return m_SmallImageList.Add(pApp->LoadIcon(iIconID));
}
