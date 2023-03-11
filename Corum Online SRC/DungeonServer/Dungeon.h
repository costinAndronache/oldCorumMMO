#pragma once
#include "Linkedlist.h"
#include "User.h"


#define TILE_SIZE				(float)125.0f
#define MAX_EVENT_DUNGEON_KIND	16

struct DB_CHANGE_DUNGEON;
class CMonster;
class DUNGEON_DATA_EX;
class CDungeonLayer;


#pragma pack(push, 1)
struct EVENT_DUNGEON_PROPERTY
{
	BYTE		byDungeonType;					// �̺�Ʈ ������ ����.
	BYTE		byMaxEnterNo;					// �ִ� ���尴 ��.
	BYTE		byStartKillMonsterCount;		// ���ô������� ó�� ���������� �������̾�� ���� �ִ� ���� ���μ� 
	BYTE		byFlagParty;					// ��Ƽ ���� �����ΰ�?
	BYTE		byFlagItemDrop;					// �������� ����ϴ� �����ΰ�?
	BYTE		byFlagFloorDown;				// �Ʒ������� �������鼭 �����ϴ� �����ΰ�?
	BYTE		byFlagBattleRoyal;				// ��Ʋ�ο� ���� �����ΰ�?
	BYTE		byFlagMosterKillDown;			// ���͸� ���̴� ������� �������� �����ΰ�?(���ô���)
	BYTE		byFlagExpStyle;					// ����ġ�� ȹ���ϴ� ��Ÿ��. 
	BYTE		byMonsterExpRatio;				// ���Ͱ� ����ġ �ִ� ����.
	BYTE		byBossExpRatio;					// ���� ���� ����ġ �ִ� ����.
	BYTE		byFailedExpRatio;				// ���н� ���ѱ� ����ġ�� ����
	DWORD		dwBonusMoney;					// ������ ȹ���� ī������ ��. 
	BYTE		byBonusExpRatio;				// ������ ���ʽ��� ȹ���ϴ� ����ġ�� ��
	DWORD		dwSpecialConditionStart;		// Ư���� ������ �� �̵� ���� ����.
	DWORD		dwSpecialCondition;				// Ư���� ������ �� �̵��� ����Ǵ� ����.	
};
#pragma pack(pop)


class CDungeon
{

protected:	

	BYTE					m_bDungeonKind;									// Dungeon Type (General or Novice)	
	char					m_szDungeonTitle[ MAX_DUNGEON_TITLE_LENGTH ];	// ���� Ÿ��Ʋ
	WORD					m_wID;											// �Ϲ� ������ ��� ���� ������ȣ 
	WORD					m_wDungeonUserCount;							// ���� �������� �� ������ 
	DWORD					m_dwEventClearTick;								// �̺�Ʈ ���� Ŭ���� �� �ð�.
	DWORD					m_dwEventFirstCloseTick;						// 1�� �̺�Ʈ ���� Ŭ���� �� �ð�. 
	DWORD					m_dwTemp[DUNGEON_TEMP_MAX];
	POSITION_				m_DungeonListPos;								// DungeonTable���� DungeonList�� ���������� ���� DungeonList Node Pointer
	EVENT_DUNGEON_PROPERTY* m_pEventDungeonProperty;						// �̺�Ʈ ������ ��� �̺�Ʈ ������ �Ӽ��� ��� �ȴ�.
	DB_CHANGE_DUNGEON*		m_pChangeDungeon;								// ������� ���γ��� ������ ���� ���������� ������ ���ƶ�.
	CMonster*				m_pDungeonGuardian;
	DUNGEON_DATA_EX*		m_pInfo;										// ��������
	CDungeon*				m_pPrevDungeon;
	CDungeon*				m_pNextDungeon;
	CDungeonLayer*			m_pDungeonLayer[ MAX_LAYER_PER_DUNGEON ];
	PARTYUSER_HASH*			m_pPartyUserHash;
	GUILDUSER_HASH*			m_pGuildUserHash;
	CVoidList*				m_pDungeonUserList;
	CVoidList*				m_pDefenceParty;								// ���� ��� �� �� ��Ƽ���̵�
	CVoidList*				m_pRemovePCList;								// ����� ���� ���� ����Ʈ 	

public:

	DWORD							GetTemp(DUNGEON_TEMP eTemp_Kind);
	void							SetTemp(DUNGEON_TEMP eTemp_Kind, DWORD dwValue);
	DWORD							GetEventClearTick() const;
	DWORD							GetEventFirstCloseTick() const;
	void							SetEventFirstCloseTick(DWORD dwEventFirstCloseTick);
	void							SetEventClearTick(DWORD dwEventClearTick);
	DUNGEON_DATA_EX*				GetDungeonDataEx() const;
const EVENT_DUNGEON_PROPERTY* const	GetEventDungeonProperty() const;
	void							SetEventDungeonProperty(EVENT_DUNGEON_PROPERTY* const pEventDungeonProperty);
	PARTYUSER_HASH*					GetPartyUserHash() const;
	GUILDUSER_HASH*					GetGuildUserHash() const;
	void							SetDungeonGuardian(CMonster* const pMonster);
	const CMonster*					GetDungeonGuardian();
	CVoidList*						GetDungeonUserList() const;
	CDungeon*						GetPrevDungeon() const;
	CDungeon*						GetNextDungeon() const;
	void							SetPrevDungeon(CDungeon* const pDungeon);
	void							SetNextDungeon(CDungeon* const pDungeon);
	CDungeonLayer*					GetDungeonLayer(const BYTE byLayerIndex) const;
	CVoidList*						GetRemovePCList()	const;
	BOOL							Build();	
	void							SetDungeonTitle(char* szTitle);
	WORD							GetID()	const						{	return m_wID;	}
	void							Destroy();
	void							SetDungeonID(const WORD wID)		{	m_wID = wID;	}
	void							SetDungeonKind(const BYTE bKind)	{	m_bDungeonKind = bKind;		}
	char*							GetDungeonName();
	char*							GetDungeonTitle()					{	return m_szDungeonTitle;	}
	BYTE							GetDungeonKind()					{	return m_bDungeonKind;		}
	DWORD							GetDungeonTotalUserCount()			{   return m_wDungeonUserCount; }	
	WORD							GetTotalLayer() const;
	WORD							GetDungeonLayerID(BYTE bLayer) const;
	BOOL							SetDungeonInfo(DUNGEON_DATA_EX* pInfo, BYTE bDungeonKind);
	DUNGEON_JOIN_FAIL				JoinUser(CUser* pUser, BYTE bLayerIndex = 0, VECTOR2 *pv2SpecifiedPos = NULL);
	void							ChangeLayerUser( CUser* pUser, BYTE byCurLayerIndex, int nDestLayerIndex, BYTE byDestLayerSpot ) const;
	void							ChangeLayerUser( CUser* pUser, BYTE byCurLayerIndex, BYTE byMoveSpotNum ) const;
	void							ChangeLayerMonster( CMonster* pMonster, int nDestLayerIndex, VECTOR2& v2Dest) const;
	void							InsertUser(CUser* pUser);
	void							RemoveUser(CUser* pUser);
	void							BroadCast(char* pMsg, DWORD dwLength);
	void							BroadCastInCircle(CDungeonLayer* pDungeonLayer, VECTOR2* v2Center, BYTE byRadiusTileCount, char* pMsg, DWORD dwLength);
	void							SendJoinSectionInfo( CUser* pUser , BYTE bPortal = 0 ) const;
	void							SendSiegeInfo(CUser* pUser) const;				
	void							SetDungeonListPos(POSITION_ Pos)	{	m_DungeonListPos = Pos;	}
	POSITION_						GetDungeonListPos();
	void							GotoLobbyUser(CUser* pUser) const;
	BOOL							CreateDungeonGuardian();
	BOOL							RemoveDungeonGuardian();
	void							DistinctionAttackMode();					
	void							SetDungeonOwner(DWORD dwUserIndex, DWORD dwGuildID, DWORD dwPartyID, char* pLoginID, const char* const pCharacterName);	
	void							RestartSiege();								
	BOOL							PermissionJoinDungeon(CUser* pUser);	
	void							StartSiege();							
	void							EndSiege();								
	BYTE							GetAttackMode(CUser* pUser) const;		
	void							InsertDefensePartyList(DWORD dwPartyID);
	void							RemoveAllDefencePartyList();			
	void							RemoveUserForInsidePortal( CUser* pUser ) const;		
	BOOL							SendPortalMsg( CUser* pUser, BYTE bDestLayerID, VECTOR2 *pv2Pos, BYTE bChangeDungeon ) const;
	DWORD							JoinUserForPortal( CUser* pUser, BYTE bLayerIndex);
	void							ReviveAllMonster();							
	void							RemoveAllUserMonster( CUser* pUser );
	int								GetEventDungeonType();									
	int								GetEventDungeonLevel();									
	void							ReportUserEnterEventDungeonByPortal(WORD wDungeonID);	
	void							ResetMonster(BOOL bCreateMonster = TRUE);				
	void							BanAllUser();											
	void							AddAllAccumulationExp() const;							
	void							SubAllAccumulationExp();								
	void							PlusAccumulationExp(LONG byPercent) const;				
	void							MinusAccumulationExp(BYTE byPercent);					
	void							EventFirstTimeUpProcess();								
	void							EventClearProcess();									
	void							AllUserPKMode(BOOL bIsPKMode = 1);						
	void							EventTimeUpProcess();									
	
	CDungeon();
	~CDungeon();
};

