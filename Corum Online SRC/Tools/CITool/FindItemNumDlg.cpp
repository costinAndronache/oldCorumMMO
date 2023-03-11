#include "stdafx.h"
#include "NewSerial.h"
#include "FindItemNumDlg.h"
#include "MainFrame.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CFindItemNumDlg, CDialog)
	//{{AFX_MSG_MAP(CFindItemNumDlg)
	ON_BN_CLICKED(IDC_BUTTON, OnSelectItemSave)
	ON_BN_CLICKED(IDC_RIGHT, OnRightMove)
	ON_BN_CLICKED(IDC_LEFT, OnLeftMove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


#define WS_EX_LAYERED 0x00080000
#define LWA_ALPHA     0x00000002


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
CFindItemNumDlg::CFindItemNumDlg(CWnd* pParent /*=NULL*/) : CDialog(CFindItemNumDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindItemNumDlg)
	m_wID		= 0;
	//}}AFX_DATA_INIT
	m_bCreate	= FALSE;
	m_pUIThread	= NULL;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CFindItemNumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindItemNumDlg)
	DDX_Control(pDX, IDC_LIST_SELECT, m_List_SelectItem);
	DDX_Control(pDX, IDC_LIST, m_List_Item);
	DDX_Text(pDX, IDC_EDIT, m_wID);
	//}}AFX_DATA_MAP
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
BOOL CFindItemNumDlg::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CFindItemNumDlg::PostNcDestroy() 
{
	((CMainFrame*)AfxGetMainWnd())->m_pFindItemNumDlg = NULL;
	delete this;
	CDialog::PostNcDestroy();
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
BOOL CFindItemNumDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

//	typedef BOOL(WINAPI * SLWA)(HWND, COLORREF, BYTE, DWORD);
//	SLWA pSetLayeredWindowAttributes = NULL;
//  HINSTANCE hmodUSER32 = LoadLibrary("USER32.DLL");											// 인스턴스 얻음.
//  pSetLayeredWindowAttributes=(SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes");  // 함수포인터 얻음.
//  HWND hwnd = this->m_hWnd;                                                                   // 다이얼로그의 핸들 얻음.
//  SetWindowLong(hwnd, GWL_EXSTYLE,GetWindowLong(hwnd,GWL_EXSTYLE) | WS_EX_LAYERED);
//  pSetLayeredWindowAttributes(hwnd,0,(255 * 70) / 100,LWA_ALPHA);

	m_List_Item.SetHeadings(_T("Item Name,315"));
	m_List_Item.SetGridLines(TRUE);
	m_List_Item.SetCheckboxes(TRUE);

	m_List_SelectItem.SetHeadings(_T("Item Name,330"));
	m_List_SelectItem.SetGridLines(TRUE);
	m_List_SelectItem.SetCheckboxes(TRUE);

	CApp*					pApp				= (CApp*)AfxGetApp();
	POSITION				pos					= pApp->m_ItemList.GetHeadPosition();
	CBaseItem				BaseItem;
	char					szBuf[0xff]			= {0,};
	DWORD					dwCount				= 0;
	LVITEM					item;
	
	while(pos)
	{
		BaseItem = pApp->m_ItemList.GetNext(pos);
		
		sprintf(szBuf, "%d.  %s", BaseItem.wID,BaseItem.szItemName_Kor);
		
		// ItemName
		item.mask		= LVIF_TEXT | LVIF_PARAM;
		item.pszText	= szBuf;
		item.iItem		= dwCount;	
		item.iSubItem	= 0;		
		item.lParam		= dwCount;
		m_List_Item.InsertItem(&item);
		
		dwCount++;		
	}

	CenterWindow();
	m_bCreate = TRUE;
	
	return TRUE;  
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
BOOL CFindItemNumDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if(pMsg->hwnd == GetDlgItem(IDC_EDIT)->m_hWnd)
		{
			FindItem();
			return TRUE;
		}		
	}

	if( (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) || (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)		)
    	return TRUE;						
	
	return CDialog::PreTranslateMessage(pMsg);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CFindItemNumDlg::FindItem()
{
	UpdateData();
	CString szItem;
	WORD	wID;

	for(int i = 0; i < m_List_Item.GetItemCount(); i++)
	{
		szItem	= m_List_Item.GetItemText(i,0);
		wID		= atoi(szItem.Left(szItem.Find('.')));

		if(m_wID == wID)
		{
			m_List_Item.CheckItem(i);
			
			// 스크롤바 이동..
			POINT			point;
			RECT			rect;
			CHeaderCtrl*	pHeader = (CHeaderCtrl*)m_List_Item.GetDlgItem(0);			
			pHeader->GetClientRect( &rect );			
			int	header	= rect.bottom;			
			m_List_Item.GetItemPosition(i,&point);
			m_List_Item.Scroll(CSize(0,point.y - header));

			break;
		}
	}
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CFindItemNumDlg::OnSelectItemSave() 
{
	CApp*	pApp			= (CApp*)AfxGetApp();
	CString szSelectItem;
	WORD	SelectID		= 0;

	pApp->m_ItemNoList.RemoveAll();

	if( m_List_SelectItem.GetItemCount() > 0 )
	{
		for(int i = 0; i < m_List_SelectItem.GetItemCount(); i++)
		{
			szSelectItem	= m_List_SelectItem.GetItemText(i,0);
			SelectID		= atoi(szSelectItem.Left(szSelectItem.Find('.')));
			pApp->m_ItemNoList.AddTail(SelectID);
		}
		
		AfxMessageBox("Save Item Number Success!!!",MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		AfxMessageBox("Fail To Item Number Save...",MB_OK|MB_ICONSTOP);
	}
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CFindItemNumDlg::OnRightMove() 
{
	DWORD	dwCount		= m_List_SelectItem.GetItemCount();
	LVITEM	item;
	CString szCheckItem;
	CString szSelectItem;
	WORD	CheckID		= 0;
	WORD	SelectID	= 0;
	BOOL	bsame		= FALSE;
	
	for(int i = 0; i < m_List_Item.GetItemCount(); i++)
	{
		if(m_List_Item.IsItemChecked(i))
		{
			szCheckItem		= m_List_Item.GetItemText(i,0);
			CheckID			= atoi(szCheckItem.Left(szCheckItem.Find('.')));

			for(int j = 0; j < m_List_SelectItem.GetItemCount(); j++)
			{
				szSelectItem	= m_List_SelectItem.GetItemText(j,0);
				SelectID		= atoi(szSelectItem.Left(szSelectItem.Find('.')));
				if(CheckID == SelectID)
				{
					bsame = TRUE;
					m_List_Item.UnCheckItem(i);
					break;
				}
			}

			if(!bsame)
			{
				// ItemName
				item.mask		= LVIF_TEXT | LVIF_PARAM;
				item.pszText	= szCheckItem.GetBuffer(szCheckItem.GetLength());
				item.iItem		= dwCount;	
				item.iSubItem	= 0;		
				item.lParam		= dwCount;
				m_List_SelectItem.InsertItem(&item);
				m_List_Item.UnCheckItem(i);
				
				dwCount++;
			}
			
			bsame = FALSE;
		}
	}	
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CFindItemNumDlg::OnLeftMove() 
{
	m_List_SelectItem.DeleteAllCheckedItems();	
}



