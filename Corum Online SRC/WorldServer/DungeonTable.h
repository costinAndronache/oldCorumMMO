#ifndef __DUNGEON_TABLE_H__
#define __DUNGEON_TABLE_H__

#pragma once

#include "stdafx.h"
#include "ServerTable.h"
#include "OnlyList.h"
#include "dungeon_data_ex.h"


class CDungeonTable
{
	
	DWORD					m_dwNum;

	void					RemoveDungeonTable();
		
public:								  

	DUNGEON_DATA_EX**		m_ppInfoTable;
	DWORD					m_dwMaxBucketNum;		
	COnlyList*				m_pDungeonList;					//���ɴ��� ����Ʈ, �����ɰ� ����, ��δ� Add ��Ű�� �ʴ´�. 	
	COnlyList*				m_pDungeonVillList;				// �����ɰ� ����, ��θ� Add ��Ų��.
	
	DUNGEON_DATA_EX*		AllocNewDungeon(WORD wID);
	DUNGEON_DATA_EX*		GetDungeonInfo(DWORD wId);
	BOOL					Add(DUNGEON_DATA_EX* pDungeon);
	void					Remove(DUNGEON_DATA_EX* pInfo);	
	DWORD					GetCount()	{	return m_dwNum;		}
	
	CDungeonTable(DWORD dwNum);
	~CDungeonTable();
};

extern CDungeonTable*	g_pDungeonTable;

#endif 