// DialogEditTileSet.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "DialogEditTileSet.h"
#include "DialogTileSet.h"
#include "Tool.h"
#include "DialogEditEntry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogEditTileSet dialog


CDialogEditTileSet::CDialogEditTileSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogEditTileSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogEditTileSet)
	m_TileSetName = _T("");
	//}}AFX_DATA_INIT

	Create(CDialogEditTileSet::IDD,pParent);
}


void CDialogEditTileSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEditTileSet)
	DDX_Text(pDX, IDC_TILESETNAME, m_TileSetName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogEditTileSet, CDialog)
	//{{AFX_MSG_MAP(CDialogEditTileSet)
	ON_BN_CLICKED(IDC_ENTRY1, OnEntry1)
	ON_BN_CLICKED(IDC_ENTRY2, OnEntry2)
	ON_BN_CLICKED(IDC_ENTRY3, OnEntry3)
	ON_BN_CLICKED(IDC_ENTRY4, OnEntry4)
	ON_BN_CLICKED(IDC_ENTRY5, OnEntry5)
	ON_BN_CLICKED(IDC_ENTRY6, OnEntry6)
	ON_BN_CLICKED(IDC_ENTRY7, OnEntry7)
	ON_BN_CLICKED(IDC_ENTRY8, OnEntry8)
	ON_BN_CLICKED(IDC_ENTRY9, OnEntry9)
	ON_BN_CLICKED(IDC_ENTRY10, OnEntry10)
	ON_BN_CLICKED(IDC_ENTRY11, OnEntry11)
	ON_BN_CLICKED(IDC_ENTRY12, OnEntry12)
	ON_BN_CLICKED(IDC_ENTRY13, OnEntry13)
	ON_EN_CHANGE(IDC_TILESETNAME, OnChangeTilesetname)
	ON_BN_CLICKED(IDC_ENTRY0, OnEntry0)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogEditTileSet message handlers


void CDialogEditTileSet::Initialize(CDialogTileSet* pTileSetDlg,CDialogEditEntry* pEditEntryDlg)
{
	m_pEditEntryDlg = pEditEntryDlg;
	m_pTileSetDlg= pTileSetDlg;	
	
	return;
}

void CDialogEditTileSet::EnableView()
{
	ShowWindow(SW_SHOWDEFAULT);
}
void CDialogEditTileSet::DisableView()
{
	ShowWindow(SW_HIDE);	
}

void CDialogEditTileSet::InitTileSet(CTileSet* pSet)
{
	m_TileSet.CopyFrom(pSet);
	m_TileSetName = m_TileSet.GetName();
	UpdateData(FALSE);
}

BOOL CDialogEditTileSet::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::OnCommand(wParam, lParam);
}

void CDialogEditTileSet::OnEditEntry(int n)
{
	DWORD EntryNum = n;
	m_pEditEntryDlg->InitEntry(EntryNum,m_TileSet.GetEntry(EntryNum));
	m_pEditEntryDlg->EnableView();
}

void CDialogEditTileSet::OnEntry0() {	OnEditEntry(TILEBIT_NONE	);	}
void CDialogEditTileSet::OnEntry1() {	OnEditEntry(TILEBIT_RD	);	}
void CDialogEditTileSet::OnEntry2() {	OnEditEntry(TILEBIT_LD|TILEBIT_RD	);	}
void CDialogEditTileSet::OnEntry3() {	OnEditEntry(TILEBIT_LD	);	}
void CDialogEditTileSet::OnEntry4() {	OnEditEntry(TILEBIT_RU|TILEBIT_RD	);	}
void CDialogEditTileSet::OnEntry5() {	OnEditEntry(TILEBIT_ALL	);	}
void CDialogEditTileSet::OnEntry6() {	OnEditEntry(TILEBIT_LU|TILEBIT_LD	);	}
void CDialogEditTileSet::OnEntry7() {	OnEditEntry(TILEBIT_RU	);	}
void CDialogEditTileSet::OnEntry8() {	OnEditEntry(TILEBIT_LU|TILEBIT_RU	);	}
void CDialogEditTileSet::OnEntry9() {	OnEditEntry(TILEBIT_LU	);	}
void CDialogEditTileSet::OnEntry10() {	OnEditEntry(TILEBIT_ALL^TILEBIT_LU	);	}
void CDialogEditTileSet::OnEntry11() {	OnEditEntry(TILEBIT_ALL^TILEBIT_RU	);	}
void CDialogEditTileSet::OnEntry12() {	OnEditEntry(TILEBIT_ALL^TILEBIT_LD	);	}
void CDialogEditTileSet::OnEntry13() {	OnEditEntry(TILEBIT_ALL^TILEBIT_RD	);	}


void CDialogEditTileSet::SetEntry(DWORD EntryNum,CTileSetEntry* pEntry)
{
	m_TileSet.SetEntry(EntryNum,pEntry);
}

void CDialogEditTileSet::OnChangeTilesetname() 
{
	UpdateData(TRUE);
	m_TileSet.SetName(m_TileSetName);
}

void CDialogEditTileSet::OnOK() 
{
	// TODO: Add extra validation here
	m_pTileSetDlg->AddTileSet(&m_TileSet);

//	CDialog::OnOK();
}

void CDialogEditTileSet::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
