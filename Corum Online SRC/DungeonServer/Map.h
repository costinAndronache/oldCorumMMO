//----------------------------------------------------------------------------------------------//
// Map Class
//----------------------------------------------------------------------------------------------//
#pragma once

#include "StdAfx.h"

#define TILE_WIDTH		125.0f
#define TILE_HEIGHT		125.0f
#define TILE_X(X)	(float)( X * TILE_WIDTH + (TILE_WIDTH * 0.5f ) )
#define TILE_Z(Z)	(float)( Z * TILE_HEIGHT + (TILE_HEIGHT * 0.5f ) )

#define TILE_EMPTY					0
#define TILE_OCCUPIED_PC			1
#define TILE_OCCUPIED_NPC			2
#define TILE_OCCUPIED_MONSTER		3
#define TILE_OCCUPIED_DONT_MOVE		7
#define TILE_OCCUPIED_SKILL			8

#pragma pack( push, 1 )

struct TILE_ATTR
{
	WORD uAttr		: 4;		//  0 ~ 15
	WORD uOccupied	: 4;		//	0 ~ 15
	WORD uSection	: 8;		//  0 ~ 255
};

struct MAP_TILE
{
	float	x;
	float	z;
	WORD	wIndex_X;	//�迭 Index�� 
	WORD	wIndex_Z;

	TILE_ATTR wAttr;
	LPVOID	pData;		// �����ϰ� �ִ°��� �ִٸ� �� ��ü�� �������̴�.
};
struct SPOT_INFO
{
	WORD wDestLayerID[ 10 ];
	WORD wDestSpotID[ 5 ];
};
struct SPOT_VALUE
{
	DWORD		dwMapID;
	SPOT_INFO	spot[20];
};
struct CP_DESC
{
	BYTE		bID;
	MAP_TILE*	pTile;
	DWORD		dwStartTick;	
	BOOL		bDestroy;		// ������ǰ� �ı��Ȱ���..�˱� ���ؼ�.
	WORD		wProperty;		// ��ȭ�Ǿ����� ��Ÿ���� �����ȣ.

};
#pragma pack (pop)

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
