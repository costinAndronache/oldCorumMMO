#ifndef _EVENT_DUNGEON_MANAGER_
#define _EVENT_DUNGEON_MANAGER_


#pragma once


#include "DUNGEON_DATA_EX.h"


#define EVENT_DUNGEON_POS_TYPE_NORMAL	0
#define EVENT_DUNGEON_POS_TYPE_SPECIAL	1
#define MAX_EVENT_DUNGEON_NUMBER		16
#define MAX_EVENT_POS_NUMBER			128
#define MAX_EVENT_DUNGEON_KIND			16

class DUNGEON_DATA_EX;


#pragma pack(push, 1)
struct EVENT_DUNGEON_PROPERTY
{
	DWORD		dwDungeonNumber;		// �� ���̺����� ���� ��ȣ.
	BYTE		byDungeonType;			// ������ Ÿ��.
	BYTE		byEnterLevelMin;		// ���� �ּ� ����
	BYTE		byEnterLevelMax;		// ���� �ִ� ����
	DWORD		dwOpenTime;				// ���� �ִ� ������ ���µǱ� ���� ������.
	DWORD		dwReadyOpenTime;		// '�Ա� ���� ���� �غ�����' ������ '�Ա��� ����'���� ������.
	DWORD		dwOpenTimeUpTime;		// '�Ա� ����'������ '�Ա� �ݱ�'���� ������.
	DWORD		dwFirstCloseTime;		// ���������϶� 1���̺�Ʈ ���� ������ 
	DWORD		dwCloseTime;			// '�Ա� ����'������ '������'���� (�Ա��� �ƴ�) ������.
	BYTE		byMaxEnterNo;			// �ִ� ���� �ο���.
	BYTE		byPositionType;			// ���� ��ġ ������ ���� Ÿ��.
};
#pragma pack(pop)


class CEventDungeonManager
{

public:

	CEventDungeonManager();
	~CEventDungeonManager();

	BOOL					AddEventDungeon(DUNGEON_DATA_EX* pDungeon);
	EVENT_DUNGEON_PROPERTY* GetEventProperty(DWORD dwDungeonNumber) const;
	void					Process();						
	void					ProcessRelationWithCreate();
	BOOL					QueryEventDungeonProperties();

	COnlyList*				m_pEventDungeonList;
	CVoidDontDeleteList*	m_pPropertyList;

private:

	void					Clear();	
	BOOL					ClearPropertyInfo();	
};


extern CEventDungeonManager* g_pEventDungeonManager;


#endif 