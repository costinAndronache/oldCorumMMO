#pragma once


#include "GameSystem.h"

#include "BaseTeamMatch.h"// : hwoarang 050202


class CDungeon;
class CMap;
class CSection;
class Sw;
struct LAYER_FORMATION;
struct GUARDIAN_INFO;
class CVoidList;
class CPcList;
class CMonsterList;
class CCreateItemList;
class CActiveSectionList;
class CUser;
class CMonster;


#pragma pack(push, 1)
struct Monster_Spot	
{
	WORD	wMonsterID;
	WORD	wPosX;
	WORD	wPosZ;
};

struct Monster_Info
{
	BOOL			bLive;				// ��� �ִ°�.
	DWORD			dwCreateTick;		// �׾����� ���� �����ð�.
	DWORD			dwKillTime;			// �׾������� �ð�.	
	float			fAttackSight;		// ���������� �������� �ٰ������ϴ� �Ÿ�.
	float			fBound;
	Monster_Spot	Spot;				// �����Ǵ� ��ǥ�� ����.
};

struct Move_Spot						// �ɸ��Ͱ� Ư�� ��ҷ� �̵��� �ٸ������� �̵��ñ��. ���� �Ʒ���..
{
	WORD	wStartSpotX;
	WORD	wStartSpotZ;
	RECT	rcMoveSpot;
	int		wDestLayerID;
	int		wDestLayerMoveSpotNum;		// ��� ���̾��� ��� ������ �̵��Ұ��ΰ�.
};
#pragma pack(pop)


class CDungeonLayer
{

	WORD					m_wTotalSectionNum;							// �ʿ� �ִ� �� Section�� ���� 
	WORD					m_wTotalMonsterInfoNum;						// ���� ������ �Ѱ���.
	CMap*					m_pMap;										// ��
	CDungeon*				m_pParent;									// �θ� ���� ������ 
	CSection*				m_pSection[MAX_SECTION_NUM_PER_LAYER];		// �� Section ������ 

public:	

	LAYER_FORMATION*		m_pLayerFormation;
	CActiveSectionList*		m_pActiveSectionList;			// ���� ������ Active�� �����ΰ��� ��Ÿ���� ��ũ����Ʈ 
	CMonsterList*			m_pMonsterList;				
	CMonsterList*			m_pResponseMonsterList;		
	CPcList*				m_pPcList;						// PC List
	CCreateItemList*		m_pCreateItemList;
	CVoidList*				m_pDeadMonsterList;				// ���� ���͵�			
	LPITEM_HASH				m_pItemHash;					// ItemHash
	BYTE					m_bLayer;						// ���� ���� 
	WORD					m_wTotalMonster;				// ��ȯ�� ���͸� ������ ���͵��� ī��Ʈ.
	WORD					m_wCurMonster;
	WORD					m_wTotalUserCount;
	WORD					m_wStartSpotNum;
	RECT*					m_pStartSpot;
	WORD					m_wMoveSpotNum;
	Move_Spot*				m_pMoveSpot;
	CDungeonLayer*			m_pPrvInfo;			
	CDungeonLayer*			m_pNextInfo;	
	Sw*						m_pSw;
	DWORD					m_dwLastFrameSec;
	DWORD					m_dwLastOwnerFrameSec;

	CBaseTeamMatch*			m_pTeamMatch;

	//PVP ��ġ �÷���
	BOOL					m_bMatching;					// ���� : TRUE,  ����� : FALSE 
	DWORD					m_dwMatchBeginTime;				// ���� ���� ƽ 
	DWORD					m_dwMatchEndTime;				// ���� ���� ƽ ( m_dwMatchBeginTime + m_dwMatchTick )

	BOOL					m_bReadyMatchNum;				// ���� �غ�� ������ 
	BOOL					m_bStartMatch;					// ���� ����
	
	DWORD					m_dwMatchTick;					// �����ð� (Ư���� ƽ)
	DWORD					m_dw30SecBeforeEndMatchTick;	// ���� ������ 30����
	DWORD					m_dwEndMatchJudgmentTick;		// ���� ������ 5�ʰ� �̷������� �ϵ� �� �Ѵ�.
	DWORD					m_dw10SecEndMatchLayerTick;		// ������ ����ǰ� 10���Ŀ� ���� ���� �÷��׸� �����Ѵ�. 

	

public:
	void			Create(DWORD dwID, BYTE bLayer, CDungeon* pParent);
	void			Destroy();	
	void			InitMatch();
	BOOL			InsertUser( CUser* pUser, const VECTOR2* pv2Start );
	BOOL			InsertUserSection( CUser* pUser, WORD wSectionNum );
	BOOL			InsertUserSection( CUser* pUser );					
	BOOL			RemoveUserSection( CUser* pUser );					
	BOOL			RemoveUser( CUser* pUser );							
	BOOL			InsertMonster( CMonster* pMonster, const VECTOR2* v2Start  );
	BOOL			InsertMonsterSection( CMonster* pMonster, WORD wSectionNum );
	BOOL			RemoveMonsterSection( CMonster* pMonster );
	BOOL			RemoveMonster( CMonster* pMonster );	
	BOOL			InsertItem( ITEM_SERVER* pItem );
	BOOL			RemoveItem( ITEM_SERVER* pItem );
	void			Process();											
	void			InitList();	
	DWORD			BroadCastMsg( char* pMsg, DWORD wLength ) const;
	DWORD			BroadCastMsgExceptSender( char* pMsg, DWORD wLength, const CUser* pSender ) const;
	DWORD			BroadCastSectionMsg( char* pMsg, DWORD wLength, const CUser* pSender ) const;
	DWORD			BroadCastSectionMsg( char* pMsg, DWORD wLength, BYTE bSectionNum ) const;
	DWORD			BroadCastSectionMsgExceptLink( char* pMsg, DWORD wLength, BYTE bSectionNum ) const;				
	DWORD			BroadCastSectionMsgExceptLink( char* pMsg, DWORD wLength, CUser* pSender ) const;
	void			IntersectSection( BYTE bPrevSectionNum, BYTE bCurSectionNum, BYTE* pIntersectSection );		
	void			BroadCastIntersectSectionInfo( BYTE	bPrevSectionNum, BYTE bCurSectionNum, CUser* pUser );	
	void			SendSectionInfo( BYTE bSectionNum, CUser* pUser , BYTE bPortal = 0 );
	CSection*		GetSection(int nSectionNum) const;					
	BOOL			AddActiveSection(int nSectionNum);					
	BOOL			AddActiveSection(CSection* pSection);				
	BOOL			RemoveActiveSection(int nSectionNum);				
	BOOL			RemoveActiveSection(CSection* pSection);			
	BOOL			MoveSection(CUser* pUser , int nFrom, int nTo);		
	BOOL			MoveSection(CMonster* pNpc , int nFrom, int nTo);	
	double			GetDistance(VECTOR2* vFrom, VECTOR2* vTo);			
	BOOL			SetSection(int nSectionNum, CSection* pSection);
	void			SetTotalSectionNum(WORD wTotalSectionNum)	{	m_wTotalSectionNum = wTotalSectionNum;	}
	CMap*			GetMap()	{	return m_pMap;	}
	BYTE			GetDungeonType();	
	DWORD			GetUserCount();
	void			CreateMonsterAll();	
	void			CreateMonster( CMonster* pMonster );
	void			CallGuardian( CUser* pUser, DWORD dwZipCode, GUARDIAN_INFO* pGuardian );
	void			ResetMonster(BOOL bCreateMonster = 1);
	void			ReviveMonster(CMonster* pMonster, DWORD dwFlag = 1);
	CMonster*		CallDungeonGuardian( GUARDIAN_INFO* pGuardian );
	CMonster*		CallMonster( CUser* pUser, DWORD dwZipCode, DWORD dwBaseMonsterNum , const VECTOR2* const v3Start);
	CMonster*		CallMonsterByGM( CUser* pUser, DWORD dwZipCode, DWORD dwBaseMonsterNum, DWORD dwItem );
	void			InitMoveSpot( DWORD dwMoveSpotNum );
	CDungeon*		GetParent()	{	return m_pParent;	}		
	VECTOR2			GetAroundEmptyTile(WORD wIndex_X, WORD wIndex_Z);		
	void			AttachMagicArrayToParty(CUser* pUser, DWORD dwItemID);	
	BOOL			GetStartTile(WORD wStartSpotID, VECTOR2* const pv2Start);

	//����ġ ����, (�Ʒ� TeamMatch ���� �Լ� ���� : hwoarang 050202
	bool			CreateTeamMatch(const FLAG_TYPE dwMatchType);//����ġ ����
	bool			IsTeamMatchRun() const ;
	bool			IsTeamMatchEnd() const;//����ġ ������?

	bool			IsTeamMatchPlayer(CUser* pUser) const;

	bool			PrepareTeamMatch(const FLAG_TYPE match_type, const FLAG_TYPE match_level,  const COUNT_TYPE player_count, const COUNT_TYPE team_count, const GUILD_WAR_TIME match_time, const bool bIsAutoMatch);

	bool			OnDie(CUser* pCaster, CUser* pTarget);//�����

	bool			CanMove();//�̵��� ���� �Ұ���

private:

	void			__ProcessTeamMatch();
	void			__ProcessMatch();
	void			__ProcessSiege();
	void			__ProcessUpdateItemOwner();
	void			__ProcessDeleteItem();
	void			__ProcessCreateItem();
	void			__ProcessRespawnMonster();
	void			__ProcessUpdateMonster();
	void			__ProcessDeadMonsterClear();
};
