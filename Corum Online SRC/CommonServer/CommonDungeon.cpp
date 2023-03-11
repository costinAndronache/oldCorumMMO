#include "stdafx.h"
#include "CommonServer.h"
#include "CommonDungeon.h"

DUNGEON_TYPE DECLSPECIFIER DungeonIDtoType(const DWORD dwID)
{
	if ( (1 <= dwID) && (10 >= dwID) )				
	{	// ����
		return DUNGEON_TYPE_VILLAGE;
	}
	else if ( (2000 <= dwID) && (3000 > dwID) ) 
	{	// �ͳ� ����
		return DUNGEON_TYPE_TUNNEL;
	}
	else if ( (3000 <= dwID) && (4000 > dwID) ) 
	{	// ������ ����	
		return DUNGEON_TYPE_NOT_CONQUER;
	}
	else if ( (4000 <= dwID) && (5000 > dwID ) ) 
	{	// ���� ����
		return DUNGEON_TYPE_CONQUER;
	}
	else if ( (5000 <= dwID) && (6000 > dwID ) )  
	{	// �̺�Ʈ ����.
		return DUNGEON_TYPE_EVENT;
	}
	else if ( (6000 <= dwID) && (7000 > dwID ) )  
	{	// ���̹� ����� ���� 
		return DUNGEON_TYPE_MATCH_STATIDUM;
	}
	
	return DUNGEON_TYPE_NOT_CONQUER;//�⺻������ �����ɴ����� �ȴ�.
}