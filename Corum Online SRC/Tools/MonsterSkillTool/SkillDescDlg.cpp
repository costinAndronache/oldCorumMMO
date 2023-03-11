// SkillDescDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MonsterSkillTool.h"
#include "SkillDescDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkillDescDlg dialog


CSkillDescDlg::CSkillDescDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkillDescDlg::IDD, pParent)
{
	m_pSkillDescStruct = NULL;
	m_iSelectedSkill = 0;
	m_strSelectedSkill = "";
	//{{AFX_DATA_INIT(CSkillDescDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSkillDescDlg::~CSkillDescDlg()
{
}

void CSkillDescDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkillDescDlg)
	DDX_Control(pDX, IDC_LIST_SKILL_DESC, m_lstSkillDesc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSkillDescDlg, CDialog)
	//{{AFX_MSG_MAP(CSkillDescDlg)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_SKILL_DESC, OnItemActivateListSkillDesc)
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkillDescDlg message handlers
BOOL CSkillDescDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_lstSkillDesc.InsertColumn(1, _T("ID"), LVCFMT_LEFT, 70);
	m_lstSkillDesc.InsertColumn(2, _T("Name"), LVCFMT_LEFT, 165);

	m_lstSkillDesc.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_TWOCLICKACTIVATE);

	CString strSkillID;
		
	for(int i = 0; i < m_pSkillDescStruct->nCount; i++)
	{
		strSkillID.Format("%u", m_pSkillDescStruct->pSkillDesc[i].GetSkillID());

		int nItem = m_lstSkillDesc.InsertItem(m_lstSkillDesc.GetItemCount(), strSkillID);
		m_lstSkillDesc.SetItemText(nItem, 1, m_pSkillDescStruct->pSkillDesc[i].GetSkillName());
		m_lstSkillDesc.SetItemData(nItem, (DWORD) (m_pSkillDescStruct->pSkillDesc + i));
	}

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSkillDescDlg::InitSkillDesc(SKILL_DESC_STRUCT* pStruct)
{
	m_pSkillDescStruct = pStruct;
}

void CSkillDescDlg::OnOK() 
{
	// TODO: Add extra validation here
	// 선택된 SKILL의 ID를 저장한다.
	int nItem = 0; 
	POSITION pos = m_lstSkillDesc.GetFirstSelectedItemPosition();
	if(pos)
	{
		nItem = m_lstSkillDesc.GetNextSelectedItem(pos);

		SKILL_DESC* pDesc = (SKILL_DESC*)m_lstSkillDesc.GetItemData(nItem);

		m_iSelectedSkill = pDesc->GetSkillID();
		m_strSelectedSkill = pDesc->GetSkillName();
	}
	else
	{
		m_iSelectedSkill = 0;
		m_strSelectedSkill = "";
	}
	
	CDialog::OnOK();
}

void CSkillDescDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_iSelectedSkill = 0;
	
	CDialog::OnCancel();
}

void CSkillDescDlg::OnItemActivateListSkillDesc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	OnOK();
	
	*pResult = 0;
}

BOOL CSkillDescDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	CSize size;
	size.cx = 0;
	size.cy = (0-zDelta);
	
	m_lstSkillDesc.Scroll(size);
	
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

int	CSkillDescDlg::GetSelectedSkillID()
{
	return m_iSelectedSkill;
}

LPCSTR CSkillDescDlg::GetSelectedSkillName()
{
	return m_strSelectedSkill;
}
