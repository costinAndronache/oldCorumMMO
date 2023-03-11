// DialogTileSet.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "DialogTileSet.h"
#include "DialogTilePalette.h"
#include "DialogEditTileSet.h"
#include "TileSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogTileSet dialog


CDialogTileSet::CDialogTileSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTileSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogTileSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	
	Create(CDialogTileSet::IDD,pParent);
}


void CDialogTileSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogTileSet)
	DDX_Control(pDX, IDC_LIST1, m_TileSetListBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogTileSet, CDialog)
	//{{AFX_MSG_MAP(CDialogTileSet)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_BN_CLICKED(IDC_NEW, OnNew)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_LOAD, OnLoad)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_Modify, OnModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogTileSet message handlers

void CDialogTileSet::OnSelchangeList1() 
{
	// TODO: Add your control notification handler code here
	int n = m_TileSetListBox.GetCurSel();
	if(n == LB_ERR)
		return;
	
	m_TileSetListBox.GetText(n,m_strSelectedTileSet);
}

void CDialogTileSet::Clear()
{	
	m_strSelectedTileSet = "";

	/*
	CTileSet* pTileSet;
	POSITION_ pos = m_listTileSet.GetHeadPosition();
	while(pTileSet = (CTileSet*)m_listTileSet.GetNext(pos))
	{
		delete pTileSet;
	}
	*/
	m_listTileSet.RemoveAll();
	
	m_TileSetListBox.ResetContent();
}

void CDialogTileSet::OnNew() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("정말 초기화 하시겠습니까?",NULL,MB_YESNO) != IDYES)
		return;

	Clear();
}

void CDialogTileSet::OnSave() 
{
	char szFilters[] = "Tile Set Files (*.tst) | *.tst | All files (*.*) |*.*||";
	char* filename;
		
	CString pathname;
	CFileDialog dlg(FALSE,"tst","*.tst",NULL,szFilters,AfxGetMainWnd());

	if (dlg.DoModal() != IDOK)
		return;

	pathname = (LPSTR)(LPCSTR)(dlg.GetPathName());
	if (pathname == "")
		return;
		
	// TODO: Add your control notification handler code here
	FILE* fp = fopen(pathname,"w");

	fprintf(fp,"%d\n",m_listTileSet.GetCount());
		
	CTileSet* pTileSet;
	POSITION_ pos = m_listTileSet.GetHeadPosition();
	while(pTileSet = (CTileSet*)m_listTileSet.GetNext(pos))
	{
		pTileSet->Save(fp);
	}
	fclose(fp);
}

void CDialogTileSet::OnLoad() 
{
	// TODO: Add your control notification handler code here
	char szFilters[] = "Tile Set Files (*.tst) | *.tst | All files (*.*) |*.*||";
	char* filename;
		
	CString pathname;
	CFileDialog dlg(TRUE,"tst","*.tst",NULL,szFilters,AfxGetMainWnd());

	if (dlg.DoModal() != IDOK)
		return;

	pathname = (LPSTR)(LPCSTR)(dlg.GetPathName());
	if (pathname == "")
		return;

	FILE* fp = fopen(pathname,"r");
	if(fp == NULL)
		return;

	Clear();

	int TotalCount;
	fscanf(fp,"%d",&TotalCount);
	char temp[256];
	fgets(temp,256,fp);
	for(int n=0;n<TotalCount;++n)
	{
		CTileSet NewSet;
		NewSet.Load(fp);
		AddTileSet(&NewSet);
	}
	fclose(fp);
}

void CDialogTileSet::OnAdd() 
{
	// TODO: Add your control notification handler code here
	CTileSet temp;		// Cleared In Consructor
	CString NewName = "NewTileSet";
	temp.SetName(NewName);
	m_pEditTileSetDlg->InitTileSet(&temp);
	m_pEditTileSetDlg->EnableView();
}

void CDialogTileSet::OnDelete() 
{
	// TODO: Add your control notification handler code here
	CTileSet* pTileSet = GetSelectedTileSet();
	if(pTileSet == NULL)
		return;
	int pos = m_TileSetListBox.FindString(0,pTileSet->GetName());
	ASSERT(pos != LB_ERR);
	m_TileSetListBox.DeleteString(pos);

	POSITION_ ppos = m_listTileSet.Find(pTileSet);
	if(ppos)
		m_listTileSet.RemoveAt(ppos);
}

void CDialogTileSet::OnModify() 
{
	// TODO: Add your control notification handler code here
	CTileSet* pTileSet = GetTileSet(m_strSelectedTileSet);
	m_pEditTileSetDlg->InitTileSet(pTileSet);
	m_pEditTileSetDlg->EnableView();
}

void CDialogTileSet::Initialize(CDialogTilePalette* pPaletteDlg,CDialogEditTileSet* pEditTileSetDlg)
{
	m_pPaletteDlg = pPaletteDlg;
	m_pEditTileSetDlg = pEditTileSetDlg;	
}
void CDialogTileSet::EnableView()
{
	ShowWindow(SW_SHOWDEFAULT);	
}
void CDialogTileSet::DisableView()
{
	ShowWindow(SW_HIDE);	
}

CTileSet* CDialogTileSet::GetTileSet(CString& TileSetName)
{
	CTileSet* pTileSet;
	POSITION_ pos = m_listTileSet.GetHeadPosition();
	while(pTileSet = (CTileSet*)m_listTileSet.GetNext(pos))
	{
		if(pTileSet->GetName() == TileSetName)
			return pTileSet;
	}
	return NULL;
}
void CDialogTileSet::AddTileSet(CTileSet* pSet)
{
	CTileSet* pPrevSet = GetTileSet(pSet->GetName());
	if(pPrevSet == NULL)
	{
		CTileSet* pNewSet = new CTileSet;
		pNewSet->CopyFrom(pSet);
		m_listTileSet.AddTail(pNewSet);
		m_TileSetListBox.AddString(pNewSet->GetName());
	}
	else
	{
		//Confirm
		if(MessageBox("정말 이 TileSet을 수정하시겠습니까?",NULL,MB_YESNO) != IDYES)
			return;

		pPrevSet->CopyFrom(pSet);
	}
}
