//----------------------------------------------------------------------------------------------//
// Map Class
//----------------------------------------------------------------------------------------------//
#pragma once

#include "StdAfx.h"
#include "../CommonServer/CommonClientDungeonWorld.h"

#pragma pack(push, 1)
struct CP_DESC
{
	BYTE		bID;
	MAP_TILE* pTile;
	DWORD		dwStartTick;
	BOOL		bDestroy;		// 현재시피가 파괴된건지..알기 위해서.
	WORD		wProperty;		// 중화되었을때 나타나는 현상번호.

};
#pragma pack(pop)

class CDungeonLayer;
struct PRELOAD_DATA;
class CVoidList;
class CMap
{
	
protected:
	

public:

	DWORD				m_dwTileNumWidth;
	DWORD				m_dwTileNumHeight;
		
	CDungeonLayer*		m_pParentLayer;		//이 맵의 소유 클래스 
	MAP_TILE*			m_pTile;	
	MAP_TILE*			m_pGuardianTile;	// 가디언 소환될 타일.	
	CVoidList*			m_pCPList;			// cp 포인트 리스트
	BYTE				m_bCPRemainCount;	// 없애야 할 cp 갯수.

	void				Create( DWORD dwID, CDungeonLayer* pParent);
	void				Destroy();
	
	BOOL				SetMap(DWORD dwX, DWORD dwZ, MAP_TILE* pTile);
	BOOL				SetTileOccupied( DWORD dwX, DWORD dwZ, BYTE attr, LPVOID pType );
	
	MAP_TILE*			GetMap(DWORD dwX, DWORD dwZ);
	MAP_TILE*			GetTile(float fx, float fz);

	BOOL				ReadMap(DWORD dwID);

	CMap();
	~CMap();

};
