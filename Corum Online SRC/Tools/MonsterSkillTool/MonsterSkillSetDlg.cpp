// MonsterSkillSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MonsterSkillTool.h"
#include "MonsterSkillSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonsterSkillSetDlg dialog


CMonsterSkillSetDlg::CMonsterSkillSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonsterSkillSetDlg::IDD, pParent)
{
	m_pMonsterSkillInfo = NULL;

	//{{AFX_DATA_INIT(CMonsterSkillSetDlg)
	m_strName = _T("");
	m_nNonSkillRate = 0;
	m_nUseSkillRate = 0;
	m_nID = 0;
	m_strSkill1 = _T("");
	m_nSkillLev1 = 0;
	m_strSkill2 = _T("");
	m_strSkill3 = _T("");
	m_strSkill4 = _T("");
	m_strSkill5 = _T("");
	m_strSkill6 = _T("");
	m_strSkill7 = _T("");
	m_strSkill8 = _T("");
	m_strSkill9 = _T("");
	m_strSkill10 = _T("");
	m_strSkill11 = _T("");
	m_strSkill12 = _T("");
	m_strSkill13 = _T("");
	m_strSkill14 = _T("");
	m_strSkill15 = _T("");
	m_nSkillLev3 = 0;
	m_nSkillLev2 = 0;
	m_nSkillLev4 = 0;
	m_nSkillLev5 = 0;
	m_nSkillLev6 = 0;
	m_nSkillLev7 = 0;
	m_nSkillLev8 = 0;
	m_nSkillLev9 = 0;
	m_nSkillLev10 = 0;
	m_nSkillLev11 = 0;
	m_nSkillLev12 = 0;
	m_nSkillLev13 = 0;
	m_nSkillLev14 = 0;
	m_nSkillLev15 = 0;
	//}}AFX_DATA_INIT
}


void CMonsterSkillSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonsterSkillSetDlg)
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 50);
	DDX_Text(pDX, IDC_EDIT_NONSKILLRATE, m_nNonSkillRate);
	DDV_MinMaxInt(pDX, m_nNonSkillRate, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_USESKILLRATE, m_nUseSkillRate);
	DDV_MinMaxInt(pDX, m_nUseSkillRate, 0, 32767);
	DDX_Text(pDX, IDC_EDIT_ID, m_nID);
	DDX_Text(pDX, IDC_EDIT_SKILL1, m_strSkill1);
	DDX_Text(pDX, IDC_EDIT_SKILL1_LEV, m_nSkillLev1);
	DDV_MinMaxInt(pDX, m_nSkillLev1, 0, 50);
	DDX_Text(pDX, IDC_EDIT_SKILL2, m_strSkill2);
	DDX_Text(pDX, IDC_EDIT_SKILL3, m_strSkill3);
	DDX_Text(pDX, IDC_EDIT_SKILL4, m_strSkill4);
	DDX_Text(pDX, IDC_EDIT_SKILL5, m_strSkill5);
	DDX_Text(pDX, IDC_EDIT_SKILL6, m_strSkill6);
	DDX_Text(pDX, IDC_EDIT_SKILL7, m_strSkill7);
	DDX_Text(pDX, IDC_EDIT_SKILL8, m_strSkill8);
	DDX_Text(pDX, IDC_EDIT_SKILL9, m_strSkill9);
	DDX_Text(pDX, IDC_EDIT_SKILL10, m_strSkill10);
	DDX_Text(pDX, IDC_EDIT_SKILL11, m_strSkill11);
	DDX_Text(pDX, IDC_EDIT_SKILL12, m_strSkill12);
	DDX_Text(pDX, IDC_EDIT_SKILL13, m_strSkill13);
	DDX_Text(pDX, IDC_EDIT_SKILL14, m_strSkill14);
	DDX_Text(pDX, IDC_EDIT_SKILL15, m_strSkill15);
	DDX_Text(pDX, IDC_EDIT_SKILL3_LEV, m_nSkillLev3);
	DDV_MinMaxInt(pDX, m_nSkillLev3, 0, 50);
	DDX_Text(pDX, IDC_EDIT_SKILL2_LEV, m_nSkillLev2);
	DDV_MinMaxInt(pDX, m_nSkillLev2, 0, 50);
	DDX_Text(pDX, IDC_EDIT_SKILL4_LEV, m_nSkillLev4);
	DDV_MinMaxInt(pDX, m_nSkillLev4, 0, 50);
	DDX_Text(pDX, IDC_EDIT_SKILL5_LEV, m_nSkillLev5);
	DDV_MinMaxInt(pDX, m_nSkillLev5, 0, 50);
	DDX_Text(pDX, IDC_EDIT_SKILL6_LEV, m_nSkillLev6);
	DDV_MinMaxInt(pDX, m_nSkillLev6, 0, 50);
	DDX_Text(pDX, IDC_EDIT_SKILL7_LEV, m_nSkillLev7);
	DDV_MinMaxInt(pDX, m_nSkillLev7, 0, 50);
	DDX_Text(pDX, IDC_EDIT_SKILL8_LEV, m_nSkillLev8);
	DDV_MinMaxInt(pDX, m_nSkillLev8, 0, 50);
	DDX_Text(pDX, IDC_EDIT_SKILL9_LEV, m_nSkillLev9);
	DDV_MinMaxInt(pDX, m_nSkillLev9, 0, 50);
	DDX_Text(pDX, IDC_EDIT_SKILL10_LEV, m_nSkillLev10);
	DDV_MinMaxInt(pDX, m_nSkillLev10, 0, 50);
	DDX_Text(pDX, IDC_EDIT_SKILL11_LEV, m_nSkillLev11);
	DDV_MinMaxInt(pDX, m_nSkillLev11, 0, 50);
	DDX_Text(pDX, IDC_EDIT_SKILL12_LEV, m_nSkillLev12);
	DDV_MinMaxInt(pDX, m_nSkillLev12, 0, 50);
	DDX_Text(pDX, IDC_EDIT_SKILL13_LEV, m_nSkillLev13);
	DDV_MinMaxInt(pDX, m_nSkillLev13, 0, 50);
	DDX_Text(pDX, IDC_EDIT_SKILL14_LEV, m_nSkillLev14);
	DDV_MinMaxInt(pDX, m_nSkillLev14, 0, 50);
	DDX_Text(pDX, IDC_EDIT_SKILL15_LEV, m_nSkillLev15);
	DDV_MinMaxInt(pDX, m_nSkillLev15, 0, 50);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMonsterSkillSetDlg, CDialog)
	//{{AFX_MSG_MAP(CMonsterSkillSetDlg)
	ON_BN_CLICKED(IDC_BUTTON_SKILL1, OnButtonSkill1)
	ON_BN_CLICKED(IDC_BUTTON_SKILL2, OnButtonSkill2)
	ON_BN_CLICKED(IDC_BUTTON_SKILL3, OnButtonSkill3)
	ON_BN_CLICKED(IDC_BUTTON_SKILL4, OnButtonSkill4)
	ON_BN_CLICKED(IDC_BUTTON_SKILL5, OnButtonSkill5)
	ON_BN_CLICKED(IDC_BUTTON_SKILL6, OnButtonSkill6)
	ON_BN_CLICKED(IDC_BUTTON_SKILL7, OnButtonSkill7)
	ON_BN_CLICKED(IDC_BUTTON_SKILL8, OnButtonSkill8)
	ON_BN_CLICKED(IDC_BUTTON_SKILL9, OnButtonSkill9)
	ON_BN_CLICKED(IDC_BUTTON_SKILL10, OnButtonSkill10)
	ON_BN_CLICKED(IDC_BUTTON_SKILL11, OnButtonSkill11)
	ON_BN_CLICKED(IDC_BUTTON_SKILL12, OnButtonSkill12)
	ON_BN_CLICKED(IDC_BUTTON_SKILL13, OnButtonSkill13)
	ON_BN_CLICKED(IDC_BUTTON_SKILL14, OnButtonSkill14)
	ON_BN_CLICKED(IDC_BUTTON_SKILL15, OnButtonSkill15)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonsterSkillSetDlg message handlers

void CMonsterSkillSetDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	m_pMonsterSkillInfo->SetNoneSkillRate(m_nNonSkillRate);
	m_pMonsterSkillInfo->SetUseSkillRate(m_nUseSkillRate);
	
	m_pMonsterSkillInfo->SetSkillInfo(0, m_nSkillID[0], m_nSkillLev1);
	m_pMonsterSkillInfo->SetSkillInfo(1, m_nSkillID[1], m_nSkillLev2);
	m_pMonsterSkillInfo->SetSkillInfo(2, m_nSkillID[2], m_nSkillLev3);
	m_pMonsterSkillInfo->SetSkillInfo(3, m_nSkillID[3], m_nSkillLev4);
	m_pMonsterSkillInfo->SetSkillInfo(4, m_nSkillID[4], m_nSkillLev5);
	m_pMonsterSkillInfo->SetSkillInfo(5, m_nSkillID[5], m_nSkillLev6);
	m_pMonsterSkillInfo->SetSkillInfo(6, m_nSkillID[6], m_nSkillLev7);
	m_pMonsterSkillInfo->SetSkillInfo(7, m_nSkillID[7], m_nSkillLev8);
	m_pMonsterSkillInfo->SetSkillInfo(8, m_nSkillID[8], m_nSkillLev9);
	m_pMonsterSkillInfo->SetSkillInfo(9, m_nSkillID[9], m_nSkillLev10);
	m_pMonsterSkillInfo->SetSkillInfo(10, m_nSkillID[10], m_nSkillLev11);
	m_pMonsterSkillInfo->SetSkillInfo(11, m_nSkillID[11], m_nSkillLev12);
	m_pMonsterSkillInfo->SetSkillInfo(12, m_nSkillID[12], m_nSkillLev13);
	m_pMonsterSkillInfo->SetSkillInfo(13, m_nSkillID[13], m_nSkillLev14);
	m_pMonsterSkillInfo->SetSkillInfo(14, m_nSkillID[14], m_nSkillLev15);

	for(int i = 0; i < 10; i++)
	{
		if(m_pMonsterSkillInfo->GetSkillInfo(i)->GetLevel() && m_pMonsterSkillInfo->GetSkillInfo(i)->GetID() == 0)
		{
			OutputDebugString("id가 0인 스킬은 무효!");
			m_pMonsterSkillInfo->SetSkillInfo(i, 0, 0);
		}

		if(m_pMonsterSkillInfo->GetSkillInfo(i)->GetLevel() == 0 && m_pMonsterSkillInfo->GetSkillInfo(i)->GetID())
		{
			OutputDebugString("lvl이 0인 스킬은 무효!");
			m_pMonsterSkillInfo->SetSkillInfo(i, 0, 0);
		}
	}

	
	CDialog::OnOK();
}

void CMonsterSkillSetDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CMonsterSkillSetDlg::OnButtonSkill1() 
{
	// TODO: Add your control notification handler code here
	CString* pstrSkillName = NewSkillSelect(0);
	if(pstrSkillName)
	{
		m_strSkill1 = *pstrSkillName;

		UpdateData(FALSE);
	}	
}

void CMonsterSkillSetDlg::OnButtonSkill2() 
{
	// TODO: Add your control notification handler code here
	CString* pstrSkillName = NewSkillSelect(1);
	if(pstrSkillName)
	{
		m_strSkill2 = *pstrSkillName;

		UpdateData(FALSE);
	}	
}

void CMonsterSkillSetDlg::OnButtonSkill3() 
{
	// TODO: Add your control notification handler code here
	CString* pstrSkillName = NewSkillSelect(2);
	if(pstrSkillName)
	{
		m_strSkill3 = *pstrSkillName;

		UpdateData(FALSE);
	}	
}

void CMonsterSkillSetDlg::OnButtonSkill4() 
{
	// TODO: Add your control notification handler code here
	CString* pstrSkillName = NewSkillSelect(3);
	if(pstrSkillName)
	{
		m_strSkill4 = *pstrSkillName;

		UpdateData(FALSE);
	}	
}

void CMonsterSkillSetDlg::OnButtonSkill5() 
{
	// TODO: Add your control notification handler code here
	CString* pstrSkillName = NewSkillSelect(4);
	if(pstrSkillName)
	{
		m_strSkill5 = *pstrSkillName;

		UpdateData(FALSE);
	}	
}

void CMonsterSkillSetDlg::OnButtonSkill6() 
{
	// TODO: Add your control notification handler code here
	CString* pstrSkillName = NewSkillSelect(5);
	if(pstrSkillName)
	{
		m_strSkill6 = *pstrSkillName;

		UpdateData(FALSE);
	}	
}

void CMonsterSkillSetDlg::OnButtonSkill7() 
{
	// TODO: Add your control notification handler code here
	CString* pstrSkillName = NewSkillSelect(6);
	if(pstrSkillName)
	{
		m_strSkill7 = *pstrSkillName;

		UpdateData(FALSE);
	}	
}

void CMonsterSkillSetDlg::OnButtonSkill8() 
{
	// TODO: Add your control notification handler code here
	CString* pstrSkillName = NewSkillSelect(7);
	if(pstrSkillName)
	{
		m_strSkill8 = *pstrSkillName;

		UpdateData(FALSE);
	}	
}

void CMonsterSkillSetDlg::OnButtonSkill9() 
{
	// TODO: Add your control notification handler code here
	CString* pstrSkillName = NewSkillSelect(8);
	if(pstrSkillName)
	{
		m_strSkill9 = *pstrSkillName;

		UpdateData(FALSE);
	}	
}

void CMonsterSkillSetDlg::OnButtonSkill10() 
{
	// TODO: Add your control notification handler code here
	CString* pstrSkillName = NewSkillSelect(9);
	if(pstrSkillName)
	{
		m_strSkill10 = *pstrSkillName;

		UpdateData(FALSE);
	}	
}

void CMonsterSkillSetDlg::OnButtonSkill11() 
{
	// TODO: Add your control notification handler code here
	CString* pstrSkillName = NewSkillSelect(10);
	if(pstrSkillName)
	{
		m_strSkill11 = *pstrSkillName;

		UpdateData(FALSE);
	}	
}

void CMonsterSkillSetDlg::OnButtonSkill12() 
{
	// TODO: Add your control notification handler code here
	CString* pstrSkillName = NewSkillSelect(11);
	if(pstrSkillName)
	{
		m_strSkill12 = *pstrSkillName;

		UpdateData(FALSE);
	}	
}

void CMonsterSkillSetDlg::OnButtonSkill13() 
{
	// TODO: Add your control notification handler code here
	CString* pstrSkillName = NewSkillSelect(12);
	if(pstrSkillName)
	{
		m_strSkill13 = *pstrSkillName;

		UpdateData(FALSE);
	}
}

void CMonsterSkillSetDlg::OnButtonSkill14() 
{
	// TODO: Add your control notification handler code here
	CString* pstrSkillName = NewSkillSelect(13);
	if(pstrSkillName)
	{
		m_strSkill14 = *pstrSkillName;

		UpdateData(FALSE);
	}	
}

void CMonsterSkillSetDlg::OnButtonSkill15() 
{
	// TODO: Add your control notification handler code here
	CString* pstrSkillName = NewSkillSelect(14);
	if(pstrSkillName)
	{
		m_strSkill15 = *pstrSkillName;

		UpdateData(FALSE);
	}	
}

CString* CMonsterSkillSetDlg::NewSkillSelect(int nIndex)
{
	int iID = 0;
	static CString strName;

	UpdateData();

	int nRet = m_SkillDescDlg.DoModal();
	if(nRet == IDOK)
	{
		iID = m_SkillDescDlg.GetSelectedSkillID();
		strName = m_SkillDescDlg.GetSelectedSkillName();

		m_nSkillID[nIndex] = iID;
		return &strName;
	}

	return NULL;
}

void CMonsterSkillSetDlg::InitMonsterSkillInfo(MONSTER_SKILL_INFO* pInfo, SKILL_DESC_STRUCT* pSkillDescStruct)
{
	m_pMonsterSkillInfo = pInfo;
	m_pSkillDescStruct = pSkillDescStruct;

	m_nID			= pInfo->GetMonsterID();
	m_strName		= pInfo->GetMonsterName();
	m_nNonSkillRate = pInfo->GetNoneSkillRate();
	m_nUseSkillRate = pInfo->GetUseSkillRate();		

	m_strSkill1		= pSkillDescStruct->GetSkillName(pInfo->GetSkillInfo(0)->GetID());
	m_strSkill2		= pSkillDescStruct->GetSkillName(pInfo->GetSkillInfo(1)->GetID());
	m_strSkill3		= pSkillDescStruct->GetSkillName(pInfo->GetSkillInfo(2)->GetID());
	m_strSkill4		= pSkillDescStruct->GetSkillName(pInfo->GetSkillInfo(3)->GetID());
	m_strSkill5		= pSkillDescStruct->GetSkillName(pInfo->GetSkillInfo(4)->GetID());
	m_strSkill6		= pSkillDescStruct->GetSkillName(pInfo->GetSkillInfo(5)->GetID());
	m_strSkill7		= pSkillDescStruct->GetSkillName(pInfo->GetSkillInfo(6)->GetID());
	m_strSkill8		= pSkillDescStruct->GetSkillName(pInfo->GetSkillInfo(7)->GetID());
	m_strSkill9		= pSkillDescStruct->GetSkillName(pInfo->GetSkillInfo(8)->GetID());
	m_strSkill10	= pSkillDescStruct->GetSkillName(pInfo->GetSkillInfo(9)->GetID());
//	m_strSkill11	= pSkillDescStruct->GetSkillName(pInfo->GetSkillInfo(10)->GetID());
//	m_strSkill12	= pSkillDescStruct->GetSkillName(pInfo->GetSkillInfo(11)->GetID());
//	m_strSkill13	= pSkillDescStruct->GetSkillName(pInfo->GetSkillInfo(12)->GetID());
//	m_strSkill14	= pSkillDescStruct->GetSkillName(pInfo->GetSkillInfo(13)->GetID());
//	m_strSkill15	= pSkillDescStruct->GetSkillName(pInfo->GetSkillInfo(14)->GetID());

	for(int i = 0; i < 10; i++)
	{
		m_nSkillID[i] = pInfo->GetSkillInfo(0)->GetID();
	}

	m_nSkillLev1	= pInfo->GetSkillInfo(0)->GetLevel();
	m_nSkillLev2	= pInfo->GetSkillInfo(1)->GetLevel();
	m_nSkillLev3	= pInfo->GetSkillInfo(2)->GetLevel();
	m_nSkillLev4	= pInfo->GetSkillInfo(3)->GetLevel();
	m_nSkillLev5	= pInfo->GetSkillInfo(4)->GetLevel();
	m_nSkillLev6	= pInfo->GetSkillInfo(5)->GetLevel();
	m_nSkillLev7	= pInfo->GetSkillInfo(6)->GetLevel();
	m_nSkillLev8	= pInfo->GetSkillInfo(7)->GetLevel();
	m_nSkillLev9	= pInfo->GetSkillInfo(8)->GetLevel();
	m_nSkillLev10	= pInfo->GetSkillInfo(9)->GetLevel();
	m_nSkillLev11	= pInfo->GetSkillInfo(10)->GetLevel();
	m_nSkillLev12	= pInfo->GetSkillInfo(11)->GetLevel();
	m_nSkillLev13	= pInfo->GetSkillInfo(12)->GetLevel();
	m_nSkillLev14	= pInfo->GetSkillInfo(13)->GetLevel();
	m_nSkillLev15	= pInfo->GetSkillInfo(14)->GetLevel();

	m_SkillDescDlg.InitSkillDesc(pSkillDescStruct);
}

BOOL CMonsterSkillSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ZeroMemory(m_nSkillID, sizeof(m_nSkillID));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
