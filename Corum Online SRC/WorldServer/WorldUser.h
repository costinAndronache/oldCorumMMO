#ifndef __WORLD_USER_H__
#define __WORLD_USER_H__


#pragma once


#include "LinkedListEx.h"
#include "OnlyList.h"
#include "ChinaBillingProc.h"
#include "../CommonServer/Item.h"
#include "../CommonServer/CommonPacket.h"


#define MAX_BELT					8
#define MAX_EQUIP					21
#define MAX_INV_LARGE				21
#define MAX_INV_SMALL				42				


#define EQUIP_TYPE_RHAND1			0		//
#define EQUIP_TYPE_RHAND2			1		
#define EQUIP_TYPE_RIDE				2		//Ż��.. ������ڸ� 
#define EQUIP_TYPE_HELMET			3		//
#define EQUIP_TYPE_MANTLE			4		//����
#define EQUIP_TYPE_LHAND1			5		//
#define EQUIP_TYPE_LHAND2			6		
#define EQUIP_TYPE_RGLOVE			7			
#define EQUIP_TYPE_ARMOR			8		//����
#define EQUIP_TYPE_AMULE1			9
#define EQUIP_TYPE_AMULE2			10
#define EQUIP_TYPE_AMULE3			11
#define EQUIP_TYPE_BELT				12
#define EQUIP_TYPE_LGLOVE			13
#define EQUIP_TYPE_RRING1			14
#define EQUIP_TYPE_RRING2			15
#define EQUIP_TYPE_RRING3			16
#define EQUIP_TYPE_BOOTS			17
#define EQUIP_TYPE_LRING1			18
#define EQUIP_TYPE_LRING2			19
#define EQUIP_TYPE_LRING3			20


#define USER_TYPE_GM				1
#define USER_TYPE_NORMAL			0
#define USER_TYPE_PC_ROOM			2


#define MAX_BILLING_ORDER_COUNT		8


#define NTOS_INIT		0x1
#define NTOS_RUN		0x2


#pragma pack(push,1)
struct WORLD_USER_CHK
{
	DWORD dwUserIndex[4];
};

struct GUILD_CHK
{	
	BYTE	bType;	
	char	szName[MAX_CHARACTER_NAME_LENGTH];
};
#pragma pack(pop)


struct NTOS_INFO
{
	BOOL		bStatus;
	DWORD		dwSync;
	DWORD		dwSyncPrev;
	DWORD		dwTime;
};


class CWorldUser
{
public:
	
	DWORD					m_dwPropID;				
	DWORD					m_dwConnectionIndex;							// Server side connection index
	DWORD					m_dwStatus;										// ���� ���� 
	DWORD					m_dwCurServerPos;								// (���弭�� �����ϰ�)���� �����ϰ� �ִ� ���� ID	
	BYTE					m_bUserType;									// USER_TYPE_GM, USER_TYPE_NORMAL
//	CLASS_TYPE				m_eClass;//Ŭ���� Ÿ�� : hwoarang 050205
	char					m_szID[ MAX_ID_LENGTH ];						// ID	
	DWORD					m_dwUserIndex;
	char					m_szCharacterName[ MAX_CHARACTER_NAME_LENGTH  ];// ĳ���� Name	
	char					m_szGuildName[MAX_GUILD_NAME_LENGTH];
	char					m_szNickName[MAX_GUILD_NAME_LENGTH];
	BYTE					m_byType;
	BYTE					m_byRank;
	DWORD					m_dwGuildId;
	DWORD					m_dwPartyId;
	DWORD					m_dwDungeonID;									// �ֱ� �α׾ƿ��� ���� ��ȣ�� ���� 
	BYTE					m_bWorldId;										// �ֱ� ���� ����� ��ȣ(ID)	1������ 	
	BYTE					m_bySkillKey[16];
	BYTE					m_byPosKey[16];
	BYTE					m_byHelp[3];	
	BYTE					m_byCurrentHand;	
	BYTE					m_byMessenger;
	DBTIMESTAMP				m_sGuildJoinTime;
	DWORD					m_dwPrevGuildId;
	BYTE					m_byMessengerStatus;
	GUILD_CHK				m_sGuildChk[4];	
	DWORD					m_dwPrevDungeonId;
	BOOL					m_bCommand;
	BOOL					m_bParty;
	BOOL					m_bConnectGameRoom;								// ���ӹ濡�� �����ߴ�.
	short					m_wCriminalTime;								// ���� Ÿ��	
	BYTE					m_bDestWorldSpot;								// ��δ����϶� ���Դ����� �������� ����Ѵ�. ��) 0 �϶��� 2001, 1�϶��� 2002�� ������ �ϹǷ�..	
	DWORD					m_dwStartTick;									// �����ؼ� ���� ������ �ð�. 
	time_t					m_sTime;
	BYTE					m_byLeader;	
	WORD					m_wHp;
	WORD					m_wMaxHp;
	DWORD					m_dwLevel;
	WORD					m_wHead;										// �Ӹ� ��ȣ.
	WORD					m_wClass;				
	DWORD					m_dwMoney;
	CItem					m_pEquip[ MAX_EQUIP ];					
	VECTOR3					m_v3CurWorldPos;								// ����ʿ��� ���� ������ �ִ� ��ġ 
	POSITION_				m_ListPosInWorld;								// ���忡 �ִ� ���� ����Ʈ Position
	POSITION_				m_ListTotalUser;								// ��ü ���� ����Ʈ�� Position	
	BYTE					m_bIsMapLoading;								// �� �ε����� Flag
	BYTE					m_bCheckSended;									// Check Packet�� ���´� 
	BYTE					m_bKeepAliveCheckFlag;			
	DWORD					m_dwCDTableBucketID;							// ConnectionDistributeTable�� BucketID
	POSITION_				m_ListPosCDTable;								// ConnectionDistributeTable�� Node Position
	DWORD					m_dwCheckSendedTick;							// For Test
	BYTE					m_bKicked;										// �ߺ��α������� Kick���ϴ� �Ÿ� 
	BYTE					m_bBillingType;									// ���� Ÿ�� 
	DWORD					m_dwRemainTime;									// ������ �����ð� 
	DBTIMESTAMP				m_BillingEday;									// ���� �����½ð� 
	BYTE					m_byPortal;	
	CWorldUser*				m_pNextData;
	CWorldUser*				m_pPrevData;	
	CDugneonRefreshList*	m_pListRefreshFromDungeon;						// ���Ű��� �޾ƾ� �� ��������Ʈ��.
	COwnDugneonList*		m_pListOwnDungeon;								// �ڱ� ��������Ʈ
	char					m_szSearchName[MAX_CHARACTER_NAME_LENGTH];		// ĳ���� Name
	BOOL					m_bIsMonitor;									// ����͸� ���ΰ�?
	NTOS_INFO				m_NTOS;

	DWORD					m_dwBillingStartTick;							// ������ ������ �ð��� Tick
	BILLING_INFO			m_BillingInfo;									// ���� ����
	SCHINA_BILLING_INFO		m_sChinaBillingInfo;

	// ��Ƽ ��Ī : �ִ��� 2005.1.21
	PMATCH_CONDITION		m_PMatchCondition;
	DWORD					m_dwPMatchIndex;
	PARTY_CONFIG			m_PartyConfig;
	BYTE					m_bLevelDist;
	int						m_iSign;

	BOOL					m_bGuildWarRequestApply;						// ����� ���� ����
	GUILD_WAR_REQUEST		m_RequestGuildWarRequest;						// ����� ��û ����Ÿ

	// Guild Macth Condition Default; //TEAM_MATCH : hwoarang 050129
	G_MC_DEFAULT			m_GMC;
	BYTE					m_byLayerIndex;//������ÿ��� ���� �˴ϴ�. ���ÿ��� ��� ���� ������
	DWORD					m_dwGuildWarFlag;//G_W_F_xxxx
	size_t					m_team_index;//

	bool IsHaveGMC();
	void ClearGuildWarData();

public:

	void					SetCurDungeon(WORD wDungeonID);
	void					SetExchangePKCount(int nCount, DWORD dwPKEndTime);
	void					SetExchangePKRepeatCount(int nCount, DWORD dwPKRepeatEndTime);
	void					GuildIdChange();
	void					GuildIdPoint();	
	BOOL					IsGMUser() const	{	return BOOL(USER_TYPE_GM == m_bUserType);	}

	CWorldUser()
	{
		m_byLayerIndex = 0;
		m_dwGuildWarFlag = 0;
		m_team_index = 0;
	};

	~CWorldUser(){};

};

#endif
