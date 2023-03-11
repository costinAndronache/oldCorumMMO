#pragma once
#include "Define.h"
#include "dungeon_data_ex.h"
#define MAX_EVENT_DUNGEON_NUMBER				16
#define MAX_OCCUPIED_DUNGEON_NUM_PER_SERVERSET	400
#define MAX_BYGROUP	130
#define MAX_GROUPLIST	10
#define MAX_WORLD_LAND_NUM		2				//�ִ� ����� ������� 
class COnlyList;
class CDungeonTable
{
	DWORD					m_dwMaxBucketNum;	
	DWORD					m_dwNum;	
	DUNGEON_DATA_EX**		m_ppInfoTable;	
	
	BOOL					Add(DUNGEON_DATA_EX* pDungeon);
	
	BYTE					m_bCurrentWorldMapID;		//���� ������� �Ǵ� ������ ���� ����� �����ȣ�� ��Ÿ�� 
public:	
	
	void					HideAllDungeonGroupEffect();
	void					ShowEffectDungeonGroup(WORD wGroupID, BOOL bShow);
	void					AddListDungeonGroup(DUNGEON_DATA_EX* pDungeon, WORD wGroupID);
	
	void					RemoveDungeonTable();

	COnlyList*				m_pInfoList[ MAX_WORLD_LAND_NUM + 1 ];		//����� �����ȣ���� ����Ʈ �迭�� ���� 0�� �迭�� ������. 
	COnlyList*				m_pReceivedDungeonList;		//���� ������ ���� ���������� ���� ���ɴ����� ����Ʈ 
	COnlyList*				m_pGroupList[MAX_BYGROUP];		//�׷캰�� ����Ʈ�� �����.		

	DUNGEON_DATA_EX*		AllocDungeonInfo();
	DUNGEON_DATA_EX*		GetDungeonInfo(DWORD dwId);
	BOOL					AddDungeonInfo(DUNGEON_DATA_EX* pDungeon);

	void					Remove(DUNGEON_DATA_EX* pDungeon);
	DWORD					GetCount()	{	return m_dwNum;		}

	void					RemoveAllSprGuildMark();
	void					RemoveDungeonEffect(DUNGEON_DATA_EX* pDungeon);
	

	COnlyList*				m_pEventList;
	BOOL					AddEventDungeon(DUNGEON_DATA_EX* pDungeon);	
	BOOL					LoadEventDungeonDescription(char* pszFileName);

	void					SetCurrentWorldMapID( BYTE bWorldMapID )	{	m_bCurrentWorldMapID = bWorldMapID;	}
	BYTE					GetCurrentWorldMapID()						{	return m_bCurrentWorldMapID;	}

	void					DeleteEventDungeonResource();
	

	CDungeonTable(DWORD dwNum);
	~CDungeonTable();
};


