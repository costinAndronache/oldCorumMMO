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
	BOOL		bDestroy;		// ������ǰ� �ı��Ȱ���..�˱� ���ؼ�.
	WORD		wProperty;		// ��ȭ�Ǿ����� ��Ÿ���� �����ȣ.

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
		
	CDungeonLayer*		m_pParentLayer;		//�� ���� ���� Ŭ���� 
	MAP_TILE*			m_pTile;	
	MAP_TILE*			m_pGuardianTile;	// ����� ��ȯ�� Ÿ��.	
	CVoidList*			m_pCPList;			// cp ����Ʈ ����Ʈ
	BYTE				m_bCPRemainCount;	// ���־� �� cp ����.

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
