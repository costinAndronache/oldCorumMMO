#if !defined(AFX_GUILDINITVIEW_H__7E168AF7_9E76_4C45_8903_419A61E0080E__INCLUDED_)
#define AFX_GUILDINITVIEW_H__7E168AF7_9E76_4C45_8903_419A61E0080E__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


#include "CopyItemView.h"

class CGuildOfflineHash;
class CGuildTableHash;
class CGuildWarHash;

#define MAX_GUILD_NAME_LENGTH			20
#define MAX_CHARACTER_NAME_LENGTH		20
#define	MAX_GUILD_WAR					8

#pragma pack(push, 1)

struct GUILD
{
	DWORD	dwGuildId;
	
	char	szGuildName[MAX_CHARACTER_NAME_LENGTH];
	
	char	szRankName1[MAX_CHARACTER_NAME_LENGTH];
	char	szRankName2[MAX_CHARACTER_NAME_LENGTH];
	char	szRankName3[MAX_CHARACTER_NAME_LENGTH];
	char	szRankName4[MAX_CHARACTER_NAME_LENGTH];
	
	BYTE	byType;
	BYTE	byCount;
	
	WORD	wLevel;
	DWORD	dwExp;
	
	BYTE	byMember[4];
	
	DWORD	dwPoint;
	
};

struct GUILD_NODE
{
	DWORD		dwGuildId;	
	
	time_t		sTime;
	
	WORD		wPoint1;
	WORD		wPoint2;
};

struct GUILD_LIST
{	
	DWORD		dwGuildId;	
	GUILD_NODE	sGuildNode[MAX_GUILD_WAR];	
};

struct GUILD_OFFLINE_USER
{
	DWORD		dwUserIndex;
	
	BYTE		byRank;
	
	DWORD		dwLevel;
	
	WORD		wClass;
	
	char		szName[MAX_CHARACTER_NAME_LENGTH];
	char		szNickName[MAX_CHARACTER_NAME_LENGTH];
	
	DBTIMESTAMP sDate;
	
};

#pragma pack(pop)

struct GUILD_TABLE
{	
	GUILD					sGuild;	
	CGuildOfflineHash*		pGuildOfflineHash;			
};

struct GUILD_INFO_LIST
{
	DWORD		dwGuildId;
	char		szGuildName[MAX_GUILD_NAME_LENGTH];
};


struct CHARACTER_TABLE_GUILD
{
	DWORD		m_dwCharacterIndex;
	char		m_szName[MAX_CHARACTER_NAME_LENGTH];
	DWORD		m_dwLevel;
	char		m_szNickName[MAX_GUILD_NAME_LENGTH];
	BYTE		m_byType;
	BYTE		m_byRank;
	DWORD		m_dwGuildId;
	char		m_szGuildName[MAX_GUILD_NAME_LENGTH];
};

struct GROUP_INFO
{
	BYTE	byType;
	BYTE	byLevel;
	WORD	wMemeber[5];
	DWORD	dwExp;
	BYTE	byAura;
	BYTE	byMake_Size;	
};

struct CHARACTER_TABLE_GUILDEX
{
	DWORD		m_dwCharacterIndex;
	DWORD		m_dwGuildId;
};

class CGuildInitView : public CFormView
{

protected:
	CGuildInitView();          
	DECLARE_DYNCREATE(CGuildInitView)

public:

	//{{AFX_DATA(CGuildInitView)
	enum { IDD = IDD_GUILD };
	CComboBox	m_ComBoCtrl_Backup;
	CComboBox	m_ComBoCtrl;
	//}}AFX_DATA

public:

	//{{AFX_VIRTUAL(CGuildInitView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	virtual ~CGuildInitView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//{{AFX_MSG(CGuildInitView)
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeCombo();
	afx_msg void OnSelchangeComboBackup();
	afx_msg void OnGo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	
	CApp*						m_pApp;
	int							m_nViewID;
	LOGFONT						m_logFont;
	CFont						m_Font;
	CEdit*						m_pEdit;
	CProgressCtrlX*				m_pProgress;
	
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
	
	char						m_CORUM_BDB_NAME	[32];		
	char						m_CORUM_BDB_ACCOUNT	[32];		
	char						m_CORUM_BDB_PASSWORD[32];	
	char						m_CORUM_BDB_ADDRESS	[32];
	
	BYTE						m_CORUM_GAME_DB		[CURRENT_DB_NUM];
	
	DBSCHEMA					m_DbSchema			[MAX_DB_COUNT];
	int							m_nDbCount;
	
	CGuildTableHash*			m_pGuildTableHash;
	CGuildWarHash*				m_pGuildWarHash;
	
public:

	BOOL						InitDatabase		();
	BOOL						InitCorumDB			();
	void						InitDBAccount		();
	int							RemoveNotNumber		(CString &strNumber);
	void						GuildInitialize		();
	void						StepProgress		();	
	int							GuildInfoLoad		();
	void						GuildLuckyBag		();		

};

#endif 
