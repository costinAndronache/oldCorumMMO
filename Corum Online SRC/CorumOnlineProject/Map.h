//----------------------------------------------------------------------------------------------//
// Map Class
//----------------------------------------------------------------------------------------------//
#include "StdAfx.h"
#include "GameDefine.h"
#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4DyuchiGRX_Common/typedef.h"
#else
#include "./../SS3D_0719/4dyuchigx/4DYUCHIGRX_COMMON/typedef.h"
#endif
#pragma once

#include "../CommonServer/CommonClientDungeonWorld.h"


#pragma pack( push, 1 )

#define MAX_SECTION_NUM_PER_LAYER		150
#define MAX_SECTION_LINK_NUM			20		//�Ѽ��Ǵ� ��ũ�Ǵ� ������ �ִ� ����


class AppliedSkill;
class CVoidList;
struct V2_SPRITE;


struct Section_Link_Info
{
	WORD	wSectionNum;
	WORD	x1, y1, x2, y2;

	WORD	pwSectionLinkNum[ MAX_SECTION_LINK_NUM ];

	Section_Link_Info()
	{
		memset( this, 0, sizeof( Section_Link_Info ) );
	}
};
struct Move_Spot			// �ɸ��Ͱ� Ư�� ��ҷ� �̵��� �ٸ������� �̵��ñ��. ���� �Ʒ���..
{
	WORD	wStartSpotX;
	WORD	wStartSpotZ;
	RECT	rcMoveSpot;
	int		wDestLayerID;
	int		wDestLayerMoveSpotNum;		// ��� ���̾��� ��� ������ �̵��Ұ��ΰ�.
};
struct CP_DESC
{
	BYTE		bID;
	BOOL		bDestroy;
	WORD		wProperty;			// ��ȭ�Ǿ����� ��Ÿ���� �����ȣ.
	DWORD		dwStartTime;		// ������ cp�� ���ֱ� ������ �ð�.
	MAP_TILE*	pTile;
	AppliedSkill* pEffectDesc;
	V2_SPRITE*	pSpr_Cp_GuageBar;
	V2_SPRITE*	pSpr_Cp_GuageBoard;
	V2_SPRITE*	pSpr_Minimap_Dot;
};
class CorumCMap
{
	
protected:
	char				m_szMapName[ MAX_MAP_NAME_LENGTH ];		

public:
	float				m_fMapXLength;
	float				m_fMapZLength;
	DWORD				m_dwMapXTileMany;
	DWORD				m_dwMapZTileMany;

	float				m_fTileSize;
	
	float				m_fMiniMapSize;
	
	WORD				m_wTotalSectionMany;
	Section_Link_Info	m_Section[MAX_SECTION_NUM_PER_LAYER];
	
	MAP_TILE*			m_pTile;				
	CVoidList*			m_pCPList;	// cp ����Ʈ ����Ʈ
	
	BYTE				m_bCPRemainCount;		// ���־� �� cp ����.
	BOOL				SetTileOccupied( DWORD dwX, DWORD dwZ, BYTE attr, LPVOID pType );

	MAP_TILE*			GetMap(DWORD dwX, DWORD dwZ);
	BOOL				SetMap(DWORD dwX, DWORD dwZ, MAP_TILE* pTile);
	MAP_TILE*			GetTile(float fx, float fz);
	
	CorumCMap(DWORD dwWidth, DWORD dwHeight, float fTileSize);
	~CorumCMap();

};


#pragma pack (pop)

extern CorumCMap* g_pMap;