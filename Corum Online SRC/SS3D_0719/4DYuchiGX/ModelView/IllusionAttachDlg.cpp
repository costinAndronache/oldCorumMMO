// IllusionAttachDlg.cpp : implementation file
//

#include "stdafx.h"
#include "modelview.h"
#include "IllusionAttachDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIllusionAttachDlg dialog


CIllusionAttachDlg::CIllusionAttachDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIllusionAttachDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIllusionAttachDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}


void CIllusionAttachDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIllusionAttachDlg)
	DDX_Control(pDX, IDC_ILLUSION_OBJECT_LIST, m_ObjectListBox);
	DDX_Control(pDX, IDC_ILLUSION_MOD_LIST, m_ModListBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIllusionAttachDlg, CDialog)
	//{{AFX_MSG_MAP(CIllusionAttachDlg)
	ON_LBN_SELCHANGE(IDC_ILLUSION_MOD_LIST, OnSelchangeIllusionModList)
	ON_LBN_SELCHANGE(IDC_ILLUSION_OBJECT_LIST, OnSelchangeIllusionObjectList)
	ON_BN_CLICKED(ID_ILLUMINATION_LOAD_OK, OnIlluminationLoadOk)
	ON_BN_CLICKED(ID_ILLUMINATION_DLG_CANCEL, OnIlluminationDlgCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIllusionAttachDlg message handlers

BOOL CIllusionAttachDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

//	int		iGXObjectCount;
	//	chx ����ΰ� mod ����ΰ�.?
//	if( g_ToolInfo.m_bIsChFile)
	{
//		iGXObjectCount	=	g_ToolInfo.m_iTotalGxObject;
	}


	// ���̾�αװ� �� �� g_ToolInfo���� mod ����Ʈ�� �޾ƿ´�.
	int		iModCount	=	g_ToolInfo.m_lstModFileName.GetCount();
	// chr�̳� chx�� ���� �� modlist�� �������̴�.

	if( iModCount <= 0)
	{
		_asm int 3
	}

	CString		strFileName;
	char		szFileName[MAX_PATH];
	BOOL		bResult;

	// mod ����Ʈ�� ���ͼ� MOD����Ʈ�ڽ��� �ø���.
	POSITION		pos	=	g_ToolInfo.m_lstModFileName.GetHeadPosition();

	while( pos)
	{
		strFileName = g_ToolInfo.m_lstModFileName.GetNext(pos);
		bResult	=	GetPureFileName( szFileName, (LPSTR)(LPCTSTR)strFileName, MAX_PATH);
		if( bResult == FALSE)
		{
			_asm int 3;
		}

		m_ModListBox.AddString( szFileName);

	}

	m_gxhAttachTarget	=	0;
	m_pModel			=	0;
	memset( m_szObjectName, 0, sizeof(char)*1024);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIllusionAttachDlg::OnSelchangeIllusionModList() 
{
	// TODO: Add your control notification handler code here
	int		iSelected	=	m_ModListBox.GetCurSel ();
	if( LB_ERR == iSelected)
	{
		// �̷� ��찡 ���� �� �ֳ�.?
		_asm int 3;
	}

	// ObjList ���� �ϰ�,
	m_ObjectListBox.ResetContent();

	// iSelected �� ���õ� model�� �޾ƿ´�.
	GXOBJECT_HANDLE	gxh;
	DWORD			dwModReffIndex;
	I3DModel*		pModel;
	DWORD			i;
	DWORD			dwObjCount;
	char*			szObjName;
	if( g_ToolInfo.m_bIsChFile)					// chr, chx������ ��� gxh�� �ϳ��ۿ� �����Ƿ�, ��带 �޴´�.
	{
		gxh = g_ToolInfo.m_lstGXObject.GetHead();
		// mod ���� ������Ʈ ����Ʈ�� ��´�.
		g_pExecutive->GXOGetModel( gxh, &pModel, &dwModReffIndex, iSelected);
	}
	else		// mod�� ���ŵ� ������ ���, ���� �ϳ��� �޴´�.
	{
		POSITION	pos =	g_ToolInfo.m_lstGXObject.FindIndex(iSelected);
		gxh				=	g_ToolInfo.m_lstGXObject.GetAt(pos);
		g_pExecutive->GXOGetModel( gxh, &pModel, &dwModReffIndex, 0);
	}

	// gxobject
	m_gxhAttachTarget	=	gxh;

	// ���� ���� ���鼭 �̸��� ����Ʈ�ڽ��� ����Ѵ�.
	dwObjCount	=	pModel->GetObjectNum();
	for( i = 0; i < dwObjCount; i++)
	{
		szObjName	=	pModel->GetObjectName( i);
		m_ObjectListBox.AddString( szObjName);
	}
	m_pModel	=	pModel;

	_asm nop;



/*	GXOBJECT_HANDLE		gxhSelected	=	g_ToolInfo.GetNThMod( iSelected);


		POSITION pos = m_lstGXObject.FindIndex(iCount);
		GXOBJECT_HANDLE gxh = m_lstGXObject.GetAt(pos);


	g_ToolInfo.m_lstGXObject.GetHead()

		GXOBJECT_HANDLE gxh = m_lstGXObject.GetHead();
*/

/*
	// ���õ� mod���� ������Ʈ�� �� object list box���� �ִ´�.
	GXOBJECT_HANDLE		hSelected	=	this->m_pModList[iSelected];

	g_ToolInfo.m_
	virtual ULONG				__stdcall	GXOGetModel(GXOBJECT_HANDLE gxh,I3DModel** ppModel,DWORD* pdwRefIndex,DWORD dwModelIndex) = 0;// Release()�ʿ�


	m_ObjectListBox.ResetContent();
*/


	_asm nop
	
}

void CIllusionAttachDlg::OnSelchangeIllusionObjectList() 
{
	// TODO: Add your control notification handler code here
	// ���⼱ ���� �ϴ°� ����.
}

void CIllusionAttachDlg::OnIlluminationLoadOk() 
{
	// TODO: Add your control notification handler code here
	int		sel	=	m_ObjectListBox.GetCurSel();
	if( sel == LB_ERR)
	{
		// ���� �ƹ��͵� �Ȱ� ���¿��� load�� ���� ���,
		MessageBox("�ܻ�� MOD�� ����ġ�� ������Ʈ�� ����ּ���.", "Object Select Error", MB_OK);
		return	;
	}

	CString		strObj;
	m_ObjectListBox.GetText( sel, strObj);


	// �̱��� ���� ������ִ� �����̹Ƿ� �� ������Ʈ�� �̸��� �����´�.
	char* szObjName	=	m_pModel->GetObjectName( sel);
	lstrcpy( m_szObjectName, szObjName);


	// ���� ���̾�α׸� ����.
	// ���� �ڵ忡�� �貸��.
	char szFilter[] = "Model Files(*.mod)|*.mod||";

	char* pFileName;
		
	CString strPathName;
	CFileDialog dlg(TRUE,
				    NULL,
					NULL,
					OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES,
					szFilter,
					NULL);
	
	int		iReturn	=	dlg.DoModal();

	if (iReturn == IDOK)
	{
		strPathName = (LPSTR)(LPCSTR)(dlg.GetPathName());
		if (strPathName == "")
		{
			_asm int 3;
//			return;
			OnIlluminationDlgCancel();
			return	;
		}
	}
	else if( iReturn == IDCANCEL)
	{
		OnIlluminationDlgCancel();
		return	;
	}

	if (!(pFileName = (LPSTR)(LPCSTR)strPathName))
	{
		_asm int 3;
		OnIlluminationDlgCancel();
	}

	// �� ������, ������Ʈ �̸�, ���� ������Ʈ �̸� �� ���Դ�.
	g_ToolInfo.OnLoadIllusionMesh( m_gxhAttachTarget, szObjName, pFileName);


	OnIlluminationDlgCancel();

	EndDialog(1);
}

void CIllusionAttachDlg::OnIlluminationDlgCancel() 
{
	// TODO: Add your control notification handler code here
	if( m_pModel)
	{
		m_pModel->Release();
	}
	this->EndDialog(0);
}
