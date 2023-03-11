#if !defined(AFX_DUNGEON_DATA_EX_H__71BCF2A8_A1AD_4074_A8D1_A703ECB4C452__INCLUDED_)
#define AFX_DUNGEON_DATA_EX_H__71BCF2A8_A1AD_4074_A8D1_A703ECB4C452__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#include "ServerTable.h"
#include "OnlyList.h"
#include "dungeon_data.h"
#include "EventDungeonManager.h"
#include "../CommonServer/CommonDungeon.h"


#define SERVER_MESSAGE_SIEGE_CONQUER		1	// "%s���� %s������ �����߽��ϴ�." 

struct EVENT_DUNGEON_PROPERTY;


class DUNGEON_DATA_EX : public DUNGEON_DATA
{
	
public:

	LONG						m_lDungeonType;				// ���� Ÿ��.
	DWORD						m_dwRemainSiegeStartTick;	// ������ �ҷ��� ��ð� ���Ҵ°�?
	DWORD						m_dwRemainSiegeEndTick;		// ������ ����ϱ� ��ð� ���Ҵ°�?
	DWORD						m_dwRemainLevelStartTick;	// ���� ���� (�ð��� ������ ���� ������ �����Ѵ�.)
	DWORD						m_dwStopLevelTick;			// ���߾����� �������۵Ǳ������ �����ð�.
	DWORD						m_dwStopStartTick;
	DWORD						m_dwStartLevelStartTick;	// ���� �ٲ� �����ð��� �˾ƾ� �ϱ⿡.
	DWORD						m_dwEventLastActionTick;	// �̺�Ʈ ���� ���������� �׼� ������ �ð�.
	LONG						m_iEventEntered;			// �̺�Ʈ ������ �� ���� ��.
	BYTE						m_bEventState;				// �̺�Ʈ ���� ����.

	char						m_szSchoolName[MAX_CHARACTER_NAME_LENGTH]; // �Ϻ����� ���, ���� �б� ��� �̸�
	
	POSITION_					m_pListPos;
	SERVER_DATA*				m_pServer;					// ������ �ִ� ���� ���� 		
	DUNGEON_DATA_EX*			m_pNextData;
	DUNGEON_DATA_EX*			m_pPrevData;
	CDugneonRefreshUserList*	m_pListSendToUser;			// ���� ������ ���ŵǾ����� �����鿡�� ������ �Ѵ�.
	
	void						SetDungeonType(WORD wID);
	INT							GetDungeonType();
	WORD						GetLevel();
	WORD						GetUpgradeTime();
	int							GetIdleTime();
	void						SetSiegeStartDestTime(DWORD dwTick);
	void						SetSiegeEndDestTime(DWORD dwTick);
	BOOL						StartSiegeWarTime();
	BOOL						LevelUpgrade();
	void						SendRefreshProductionInfo(WORD wItemID, BYTE byProductionCount);
	BOOL						IsConquer();		
	BOOL						IsStartWarTime();
	BOOL						IsEndWarTime();
	BOOL						IsLevelUpgradeTime();
	DWORD						GetEntrancePay();	
	void						SetOwner(DWORD dwOwnerIndex, DWORD dwGuildID, char* pOwnerID, char* pOwnerName);
	void						SendStartSiege();
	void						SendEndSiege(BYTE bDefenseSuccess);
	void						SendToUsers(char* pPacket, DWORD dwSize);
	void						SetOperationMode(BYTE bOperationMode);
	void						StopOperationTime();		
	void						ResumeOperationTime();		
	void						SetAccEntrance(int nPlusMoney, BOOL bSend = FALSE);
	DWORD						GetLevelUpTickCount();
	void						SendRefreshLevelUp();
	void						SendRefreshOwner();
	void						SetAccExp(DWORD dwExp);
	void						SendRefreshAccExp();
	void						SendRefreshEntrance();
	void						SetEntrance(DWORD dwEntrance);
	void						SendRefreshOwnerGuild();
	BOOL						EventChangeState(BYTE byState, DWORD dwCurrentTick);	
	BOOL						SendEventInfo(DWORD dwEventEndTick = 0);
	void						SendEventInfoClose();
	void						SendEventInfoFirstClose();
	DWORD						GetEventLastActionTick();
	BOOL						DoNotExistEventDungeonEntered(DWORD dwCurrentTick, EVENT_DUNGEON_PROPERTY* pProperty);
	void						ResetEventInfo(BOOL bTimeUp);	
	BOOL						CheckEventTimes();
	void						SetDefenseCount(BYTE byDefenseCount);	

	bool						IsStadium()const{ return (DUNGEON_TYPE_MATCH_STATIDUM == m_lDungeonType);}//����� ���̳�?: hwoarang 050202
	bool						IsVilliage()const{ return (DUNGEON_TYPE_VILLAGE == m_lDungeonType);}//���� ���̳�?: hwoarang 050202

	DUNGEON_DATA_EX();
	virtual ~DUNGEON_DATA_EX();
};


#endif 
