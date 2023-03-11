#if !defined(AFX_COPYITEMDELETE_H__85AECF58_5A39_4033_8118_6FC2FDC4DD28__INCLUDED_)
#define AFX_COPYITEMDELETE_H__85AECF58_5A39_4033_8118_6FC2FDC4DD28__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


#include "CopyItemView.h"


#define	MAX_LEVEL	255
#define CORUM		10
#define NETMARBLE	11
#define NABU		7


class CCopyItemDelete : public CFormView
{

protected:

	CCopyItemDelete();
	DECLARE_DYNCREATE(CCopyItemDelete)

public:

	//{{AFX_DATA(CCopyItemDelete)
	enum { IDD = IDD_DELETECOPY };
	CButton		m_Check;
	CComboBox	m_ComBoCtrl_Backup;
	CComboBox	m_ComBoCtrl;
	CString		m_szCharName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCopyItemDelete)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	virtual ~CCopyItemDelete();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//{{AFX_MSG(CCopyItemDelete)
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeCombo();
	afx_msg void OnSelchangeComboBackup();
	afx_msg void OnGo();
	afx_msg void OnSkillinit();
	afx_msg void OnStatinit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	
	CApp*						m_pApp;
	int							m_nViewID;
	LOGFONT						m_logFont;
	CFont						m_Font;
	CEdit*						m_pEdit;
	CProgressCtrlX*				m_pProgress;
	ITEM_SERIAL					m_Serial;

public:
	
	CWinThread*					m_pWinThread;
	BOOL						m_bRun;
	
public:
	
	HANDLE						m_hThread;
	
public:
	
	IOleDBThread*				m_pDb;

public:

	char						m_CORUM_DB_NAME		[CURRENT_DB_NUM][32];		
	char						m_CORUM_DB_ACCOUNT	[CURRENT_DB_NUM][32];		
	char						m_CORUM_DB_PASSWORD	[CURRENT_DB_NUM][32];	
	char						m_CORUM_DB_ADDRESS	[CURRENT_DB_NUM][32];

	char						m_MEMBER_DB_NAME		[32];//kjk		
	char						m_MEMBER_DB_ACCOUNT		[32];		
	char						m_MEMBER_DB_PASSWORD	[32];	
	char						m_MEMBER_DB_ADDRESS		[32];

	char						m_MEMBER_NET_DB_NAME	[32];		
	char						m_MEMBER_NET_DB_ACCOUNT	[32];		
	char						m_MEMBER_NET_DB_PASSWORD[32];	
	char						m_MEMBER_NET_DB_ADDRESS	[32];

	char						m_CORUM_BDB_NAME	[32];		
	char						m_CORUM_BDB_ACCOUNT	[32];		
	char						m_CORUM_BDB_PASSWORD[32];	
	char						m_CORUM_BDB_ADDRESS	[32];

	char						m_CORUM_BDBB_NAME		[3][32];
	char						m_CORUM_BDBB_ACCOUNT	[3][32];		
	char						m_CORUM_BDBB_PASSWORD	[3][32];	
	char						m_CORUM_BDBB_ADDRESS	[3][32];


	BYTE						m_CORUM_GAME_DB		[CURRENT_DB_NUM];
	BYTE						m_CORUM_OLD2_DB;
	BYTE						m_CORUM_OLD3_DB;
	BYTE						m_CORUM_MEMBER_DB;
	BYTE						m_CORUM_MEMBER_NETMARBLE_DB;
	BYTE						m_CORUM_BACKUP_DB	[3];

	DBSCHEMA					m_DbSchema			[MAX_DB_COUNT];
	int							m_nDbCount;

public:
	
	vector<HAVEITEMEX>			m_haveList;

public:

	void						StepProgress			();
	BOOL						InitDatabase			();
	BOOL						InitCorumDB				();
	void						InitDBAccount			();
	int							RemoveNotNumber			(CString &strNumber);
	void						DeleteItemSearch		();
	void						SetAttrArmor			();
	void						SetAttrWeaponArmor		();
	void						UpdateCharacterInfo		(HAVEITEMEX h,int sel);
	void						SetLevelExp				();
	void						ConvertCItem			();
	void						SkillInitialize			();
	void						StatInitialize			();
	void						MapInfoInitialize		();
	void						DeleteItemmall			();
	void						ConvertGuardianInfo		();
	void						FindItem				();
	void						CharacterMove			();
	BOOL						ResetItemSerialNumber	();
	ITEM_SERIAL					GetNewSerial			();
	void						CharacterMoveSin		();
	void						CharacterMoveSin2		();
	void						TestSeverInitialize     ();
	void						GuardianInitialize		();
	void						GuardianLevelInitialize	();
	void						GuardianSkillInitialize	();
//	void						ItemConvert				( CItem2* pItem, CItem_Ex* pNewItem );
	void						InsertGuardianSkill		( GUARDIAN_SKILL_INFO* pGSkillInfo, GUARDIANSKILL* pBaseSkill );

	void ItemLog();		// 새로운 아이템 구조로...
	void ConvertNewItem(const char* szBackupAddName);		// 새로운 아이템 구조로...
};

const size_t SIZE_OF_EQUIP = sizeof(CItem2) * MAX_EQUIP;
const size_t SIZE_OF_LARGE = sizeof(CItem2) * MAX_INV_LARGE;
const size_t SIZE_OF_SMALL = sizeof(CItem2) * MAX_INV_SMALL;
const size_t SIZE_OF_BELT = sizeof(CItem2) * MAX_BELT;
const size_t SIZE_OF_GUARDIAN = sizeof(CItem2) * MAX_INV_GUARDIAN;

const size_t SIZE_OF_BANK_LARGE = sizeof(CItem2)*MAX_BANK_LARGE;
const size_t SIZE_OF_BANK_SMALL = sizeof(CItem2)*MAX_BANK_SMALL;

#endif 
