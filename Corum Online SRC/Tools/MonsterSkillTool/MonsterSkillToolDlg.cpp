// MonsterSkillToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MonsterSkillTool.h"
#include "MonsterSkillToolDlg.h"
#include "ConnectDlg.h"
#include "MonsterSkillSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// �޽��� �ڽ� �Լ�.....for DB
void DisplayMessage(char* szMsg)
{
	MessageBox(NULL, szMsg, "DB Error", MB_OK|MB_ICONERROR);
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonsterSkillToolDlg dialog

CMonsterSkillToolDlg::CMonsterSkillToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonsterSkillToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMonsterSkillToolDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pDb = NULL;
	m_nMonsterCount = 0;
	m_pMonsterSkillInfo = NULL; 
}

void CMonsterSkillToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonsterSkillToolDlg)
	DDX_Control(pDX, IDC_COMBO_CURRENT_DB, m_cmbCurrentDB);
	DDX_Control(pDX, IDC_LIST_MONSTER, m_lstMonster);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMonsterSkillToolDlg, CDialog)
	//{{AFX_MSG_MAP(CMonsterSkillToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_MONSTER, OnItemActivateListMonster)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_DB, OnButtonChangeDb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonsterSkillToolDlg message handlers

BOOL CMonsterSkillToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_lstMonster.InsertColumn(1, _T("ID"), LVCFMT_LEFT, 40);
	m_lstMonster.InsertColumn(2, _T("Name"), LVCFMT_LEFT, 130);
	
	m_lstMonster.InsertColumn(3, _T("NonSkillRate"), LVCFMT_LEFT, 60);
	m_lstMonster.InsertColumn(4, _T("UseSkillRate"), LVCFMT_LEFT, 60);

	CString strColumn;
	for(int i = 0; i < 10; i++)
	{
		strColumn.Format("Skill%u", i+1);
		m_lstMonster.InsertColumn(5+i*2, strColumn, LVCFMT_LEFT, 50);
		strColumn.Format("Skill%u_lev", i+1);
		m_lstMonster.InsertColumn(5+i*2, strColumn, LVCFMT_LEFT, 70);
	}

	m_lstMonster.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_TWOCLICKACTIVATE);

	CConnectDlg dlg;
	if(IDOK == dlg.DoModal())
	{
		CoInitialize(NULL);		//COM ���̺귯���� �ʱ�ȭ 

		InitOleDBThreadDll();

		CreateConnection();

		ConnectDatabase((LPSTR)dlg.GetDataSourceName(),
						(LPSTR)dlg.GetDatabaseName(),
						(LPSTR)dlg.GetAccount(),
						(LPSTR)dlg.GetPassword(),
						dlg.GetTimeout());

		DBSCHEMA schema[ 100 ];

		int nReturned = m_pDb->QueryDBCatalog(schema, 100 ,m_iConnection);
		if(nReturned < 0)
		{
			MessageBox("Fail to QueryDBCatalog!", NULL, MB_OK|MB_ICONERROR);
			PostQuitMessage(0);
		}
		else
		{
			for(int i = 0; i < nReturned; i++)
			{
				m_cmbCurrentDB.AddString(schema[i].szSchemaBuffer);

				if(lstrcmpi(dlg.GetDatabaseName(), schema[i].szSchemaBuffer) == 0)
				{
					m_cmbCurrentDB.SetCurSel(i);
				}
			}
		}

		InitSkillDesc();

		InitBaseMonsterData();
	}
	else
	{
		PostQuitMessage(0);
	}
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMonsterSkillToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMonsterSkillToolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMonsterSkillToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// Method: GetDBResult 
// Desc: Event Object ����ϰ�� DLL ���ο��� Event�� ���õǸ� �� �Լ��� ȣ������ν� 
//		 ������� �޾ƿ´�.
//-----------------------------------------------------------------------------
void CMonsterSkillToolDlg::GetDBResult(DWORD dwVal)
{
	m_pDb->GetDBResult();
}

//-----------------------------------------------------------------------------
// Method: InitDBModule 
// Desc: OleDBThread ������Ʈ�� �ʱ�ȭ
//-----------------------------------------------------------------------------
void CMonsterSkillToolDlg::InitOleDBThreadDll()
{

	HRESULT hr = ::CoCreateInstance(
			CLSID_OLEDBTHREAD,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_OLEDBTHREAD,
			(void**)&m_pDb);

	if (FAILED(hr))
	{
		MessageBox("Cannot create IID_OLEDBTHREAD","Error");
		return;
	}

	DB_INITIALIZE_DESC desc;

	desc.bUsingThread = FALSE;					//Thread ���� ��� ����, FALSE�� ��� (���ξ� TH~ �迭�� �޼ҵ�� ���Ұ�!)
	desc.bUsingEventObject = 0;					//TRUE  : Event Object ������� ��������� ����
												//FALSE : Message ������� ���� ����� Callback �Լ�  
	
	desc.ReceiveFunc = NULL;	//Event Object ����ϰ�� ������� ���� Callback �Լ� ������
	
	desc.hWndToPostMessage = m_hWnd;			//Message����� ��� Message�� ���� �������� �ڵ鰪 
	desc.uMessage = WM_DB_RESULT;				//Message����� ����� Message
	
	
	desc.wMaxNumOfProcessMessage_Input = 100;	//DLL���� ���ÿ� �����Ҽ� �ִ� �ִ� Input �޼��� ����..  (Input �޼���ť �޸�Ǯ�� �ִ�뷮)
	desc.wMaxNumOfProcessMessage_Output = 100;	//DLL���� ���ÿ� �����Ҽ� �ִ� �ִ� Output �޼��� ����..  (Output �޼���ť �޸�Ǯ�� �ִ�뷮)
	
	desc.wMaxRowPerRead = 1024;					//DB���� Data�� �о�帱�� �ѹ��� �о�帱�� �ִ� �ִ� ���ǰ��� 
												//��) ���� ���������� Return�Ǵ� �������� 200���̰� 
												//	  OpenRecordEx�� dwRowPerRead ���ڰ��� 20���� �־������
												//    �ѹ��� 20�྿ 10���� �о 200���� �޾ƿ´�.
												//	  �̶� dwRowPerRead���ڰ��� �������ִ� �ִ������ 50�̶�� �ǹ�!!
	
	desc.wMaxReturnedRowNum = 1024;				//���������� ��ȯ�� �� �ִ� ���� �ִ� ����
	desc.wMaxReturnedColNum = 128;				//���������� ��ȯ�� �� �ִ� Į���� �ִ� ����
	desc.dwMaxRowSize = 1024;					//���������� ��ȯ�� �� �ִ� �� ���� �ִ� ������	
	
	desc.OutputMessageFunc = DisplayMessage;	//DLL������ �޼����� �����޼����� ��¹��� �ܺ� �Լ� ������
	
	//DB��� �ʱ�ȭ 
	m_pDb->InitDBModule(&desc);				

}


//-----------------------------------------------------------------------------
// Method: CreateDBConnection, SetDefaultDBIndex
// Desc: ���ο� DB Connection�� �����ϰ�, �⺻ DB ConnectionIndex�� 
//		 �����Ѵ�.
//-----------------------------------------------------------------------------
void CMonsterSkillToolDlg::CreateConnection()
{
	//���ο� DB Connection�� �����Ѵ�.
	//�����Ҷ��� �ݵ�� ReleaseDBConnection()���� ������ ���������־�� �Ѵ�!
	int nRet = m_pDb->CreateDBConnection();

	if(nRet < 0)
	{
		MessageBox("Fail to create connection!");
		return;
	}

	m_iConnection = nRet;		//ConnectionIndex�� ����, �������� ������ �����ϱ�����...

	//����Ʈ DB Connection�� �����Ѵ�.
	//DefaultDBIndex�� �����ϸ� Connection�� �������϶� �����Լ����� ȣ���ϸ� 
	//��� �����Լ��� �� ������ ������ DB ConnectionIndex ��ȣ�� 
	//�Ƚ��־ �������� �⺻ DB Connection���� ������ ������.
	m_pDb->SetDefaultDBIndex( m_iConnection );
}

//-----------------------------------------------------------------------------
// Method: Connect
// Desc: �������� SQL �������� ������ �õ��Ѵ�.
//-----------------------------------------------------------------------------
void CMonsterSkillToolDlg::ConnectDatabase(char* szDataSource, char* szDefaultDb, char* szUserId, char* szPassword, int nwConnectTimeout)
{
	int nRet = m_pDb->Connect(
		szDataSource, 
		szDefaultDb, 
		szUserId, 
		szPassword, 
		nwConnectTimeout, 
		m_iConnection
		);	
																					//DefaultIndex�� �����س��� g_nCon[0]������ ������ �õ��Ѵ�.
	if(nRet < 0)
	{
		CString str;
		str.Format("SQL Server ���ӿ� �����߽��ϴ�! errcode: %d", nRet);
		MessageBox(str);
	//	PostQuitMessage(0);

		OnCancel();

		return;
	}
}


//-----------------------------------------------------------------------------
// Method: ChangeDB
// Desc: ���� Connection�� ����� DB�� �����Ѵ�.
//-----------------------------------------------------------------------------
void CMonsterSkillToolDlg::ChangeDB(char* szDb)
{
	if(!m_pDb->ChangeDB(szDb, m_iConnection))
	{
		MessageBox("Fail to change database!");
		return;
	}
}

void CMonsterSkillToolDlg::InitSkillDesc()
{
	DBRECEIVEDATA* pReceive = NULL;

	// Skill id, name�� ����!
	pReceive = 
		m_pDb->OpenRecordEx(
		"select [id], [name] from skill_description_kor order by [id]", 
		2048, 
		2048, 
		m_iConnection
		);

	if(!pReceive)
	{
		MessageBox("Query Failed!");
		return;
	}

	m_nSkillDescStruct.nCount = pReceive->Query.select.dwRowCount;

	m_nSkillDescStruct.pSkillDesc = new SKILL_DESC [m_nSkillDescStruct.nCount];
	memcpy(m_nSkillDescStruct.pSkillDesc, pReceive->Query.select.pResult, pReceive->Query.select.dwRowSize * m_nSkillDescStruct.nCount);
}

void CMonsterSkillToolDlg::InitBaseMonsterData()
{
	DBRECEIVEDATA* pReceive = NULL;

	// Skill id, name�� ����!
	char szQuery[10240]={0,};
	lstrcpy(szQuery, "select serial_no,[name],non_skill_rate,skillrate,"
		"skill1,skill1_lev,skill2,skill2_lev,skill3,skill3_lev,skill4,skill4_lev,skill5,skill5_lev,"
		"skill6,skill6_lev,skill7,skill7_lev,skill8,skill8_lev,skill9,skill9_lev,skill10,skill10_lev "
		"from basemonsterinfo order by serial_no");
		
	int len = lstrlen(szQuery);

	pReceive = m_pDb->OpenRecordEx(szQuery, 1024,1024, m_iConnection );
	if(!pReceive)
	{
		MessageBox("Query Failed!");
		return;
	}

	m_nMonsterCount = pReceive->Query.select.dwRowCount;
	MONSTER_SKILL_INFO* pMonsterSkillInfo = (MONSTER_SKILL_INFO*)pReceive->Query.select.pResult;

	m_pMonsterSkillInfo = new MONSTER_SKILL_INFO [pReceive->Query.select.dwRowCount];
	memcpy(m_pMonsterSkillInfo, pReceive->Query.select.pResult, pReceive->Query.select.dwRowCount* pReceive->Query.select.dwRowSize);


	CString strTemp;
	
	for(DWORD i = 0; i < pReceive->Query.select.dwRowCount; i++)
	{
		strTemp.Format("%u", m_pMonsterSkillInfo[i].GetMonsterID());
		int nItem = m_lstMonster.InsertItem(m_lstMonster.GetItemCount(), strTemp);

		m_lstMonster.SetItemText(nItem, 1, m_pMonsterSkillInfo[i].GetMonsterName());

		strTemp.Format("%u", m_pMonsterSkillInfo[i].GetNoneSkillRate());
		m_lstMonster.SetItemText(nItem, 2, strTemp);

		strTemp.Format("%u", m_pMonsterSkillInfo[i].GetUseSkillRate());
		m_lstMonster.SetItemText(nItem, 3, strTemp);
		
		for(int j = 0; j < 10; j++)
		{
			strTemp.Format("%u", m_pMonsterSkillInfo[i].GetSkillInfo(j)->GetID());
			m_lstMonster.SetItemText(nItem, 4 + j*2, strTemp);

			strTemp.Format("%u", m_pMonsterSkillInfo[i].GetSkillInfo(j)->GetLevel());
			m_lstMonster.SetItemText(nItem, 5 + j*2, strTemp);
		}

		m_lstMonster.SetItemData(nItem, (DWORD)(m_pMonsterSkillInfo + i));
	}
}

void CMonsterSkillToolDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(m_pDb)
	{	
		m_pDb->ReleaseDBConnectionForSingleThread(m_iConnection) ;	
		m_pDb->Release();
		m_pDb = NULL;
	}	

	if(m_pMonsterSkillInfo)
	{
		delete [] m_pMonsterSkillInfo;
		m_pMonsterSkillInfo = NULL;
	}

	if(m_nSkillDescStruct.pSkillDesc)
	{
		delete [] m_nSkillDescStruct.pSkillDesc;
		m_nSkillDescStruct.pSkillDesc = NULL;
		m_nSkillDescStruct.nCount = 0;
	}

	CDialog::OnCancel();
}

void CMonsterSkillToolDlg::UpdateBaseMonsterInfo()
{
	// update db!!!
	char szQuery[10240]={0,};

	for(int i = 0; i < m_nMonsterCount; i++)
	{
		wsprintf(szQuery, "update basemonsterinfo set non_skill_rate=%d,skillrate=%d,"
			"skill1=%d,skill2=%d,skill3=%d,skill4=%d,skill5=%d,skill6=%d,skill7=%d,skill8=%d,"
			"skill9=%d,skill10=%d"
			"skill1_lev=%d,skill2_lev=%d,skill3_lev=%d,skill4_lev=%d,skill5_lev=%d,skill6_lev=%d,"
			"skill7_lev=%d,skill8_lev=%d,skill9_lev=%d,skill10_lev=%d"
			" where serial_no=%d",
			m_pMonsterSkillInfo[i].GetNoneSkillRate(), 
			m_pMonsterSkillInfo[i].GetUseSkillRate(),
			m_pMonsterSkillInfo[i].GetSkillInfo(0)->GetID(),
			m_pMonsterSkillInfo[i].GetSkillInfo(1)->GetID(),
			m_pMonsterSkillInfo[i].GetSkillInfo(2)->GetID(),
			m_pMonsterSkillInfo[i].GetSkillInfo(3)->GetID(),
			m_pMonsterSkillInfo[i].GetSkillInfo(4)->GetID(),
			m_pMonsterSkillInfo[i].GetSkillInfo(5)->GetID(),
			m_pMonsterSkillInfo[i].GetSkillInfo(6)->GetID(),
			m_pMonsterSkillInfo[i].GetSkillInfo(7)->GetID(),
			m_pMonsterSkillInfo[i].GetSkillInfo(8)->GetID(),
			m_pMonsterSkillInfo[i].GetSkillInfo(9)->GetID(),
			m_pMonsterSkillInfo[i].GetSkillInfo(0)->GetLevel(),
			m_pMonsterSkillInfo[i].GetSkillInfo(1)->GetLevel(),
			m_pMonsterSkillInfo[i].GetSkillInfo(2)->GetLevel(),
			m_pMonsterSkillInfo[i].GetSkillInfo(3)->GetLevel(),
			m_pMonsterSkillInfo[i].GetSkillInfo(4)->GetLevel(),
			m_pMonsterSkillInfo[i].GetSkillInfo(5)->GetLevel(),
			m_pMonsterSkillInfo[i].GetSkillInfo(6)->GetLevel(),
			m_pMonsterSkillInfo[i].GetSkillInfo(7)->GetLevel(),
			m_pMonsterSkillInfo[i].GetSkillInfo(8)->GetLevel(),
			m_pMonsterSkillInfo[i].GetSkillInfo(9)->GetLevel(),
			m_pMonsterSkillInfo[i].GetMonsterID()	);

		if(0 > m_pDb->ExecuteSQL(szQuery))
		{
			MessageBox("Failed to update BaseMonsterInfo!!");
		}
	}

	MessageBox("Successfully BaseMonsterInfo table Updated!!");
}

void CMonsterSkillToolDlg::OnOK() 
{
	// TODO: Add extra validation here
	int nRet = MessageBox(	"Did you backup BaseMonsterInfo Table In GameDB?\r\n"
							"If not, backup First!!!\r\n\r\n"
							"Do you want to proceed to update BaseMonsterInfo Table?\r\n", 
							"DB Update Confirm", MB_YESNO|MB_ICONWARNING|MB_DEFBUTTON2);

	if(nRet == IDNO) return;

	UpdateBaseMonsterInfo();
}

void CMonsterSkillToolDlg::OnItemActivateListMonster(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	int nItem = 0; 
	POSITION pos = m_lstMonster.GetFirstSelectedItemPosition();
	if(pos)
	{
		nItem = m_lstMonster.GetNextSelectedItem(pos);		

		MONSTER_SKILL_INFO* pInfo = (MONSTER_SKILL_INFO*)m_lstMonster.GetItemData(nItem);

		if(!pInfo)
		{
			MessageBox("Monster Skill Info Not exist!");
			return;
		}

		CMonsterSkillSetDlg dlg;
		dlg.InitMonsterSkillInfo(pInfo, &m_nSkillDescStruct);
		int nRet = dlg.DoModal();

		if(IDOK == nRet)
		{
			CString strTemp;

			strTemp.Format("%u", pInfo->GetNoneSkillRate());
			m_lstMonster.SetItemText(nItem, 2, strTemp);

			strTemp.Format("%u", pInfo->GetUseSkillRate());
			m_lstMonster.SetItemText(nItem, 3, strTemp);
			
			for(int j = 0; j < 10; j++)
			{
				strTemp.Format("%u", pInfo->GetSkillInfo(j)->GetID());
				m_lstMonster.SetItemText(nItem, 4 + j*2, strTemp);

				strTemp.Format("%u", pInfo->GetSkillInfo(j)->GetLevel());
				m_lstMonster.SetItemText(nItem, 5 + j*2, strTemp);
			}
		}
	}
	
	*pResult = 0;
}

void CMonsterSkillToolDlg::OnButtonChangeDb() 
{
	// TODO: Add your control notification handler code here
	int nRet = MessageBox(	"Do you want to save this BaseMonsterTable\r\n"
							"before changing to Target database?\r\n\r\n"
							"Do you want to proceed to update BaseMonsterInfo Table?\r\n", 
							"DB Update Confirm", MB_YESNOCANCEL|MB_ICONWARNING|MB_DEFBUTTON2);

	if(nRet == IDYES) 
	{
		UpdateBaseMonsterInfo();
	}
	else if(nRet == IDCANCEL)
	{
		return;
	}


	// clear list!
	m_lstMonster.DeleteAllItems();	

	CString strDatabase;
	int iSel = m_cmbCurrentDB.GetCurSel();
	m_cmbCurrentDB.GetLBText(iSel, strDatabase);

	ChangeDB((LPSTR)(LPCTSTR)strDatabase);

	InitBaseMonsterData();
}
