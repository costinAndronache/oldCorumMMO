//----------------------------------------------------------------------------------------------//
// Map Class
//----------------------------------------------------------------------------------------------//

#ifndef __MAP_H__
#define __MAP_H__

#include "StdAfx.h"
#include "../CommonServer/CommonClientDungeonWorld.h"
#include <string>

#pragma once

struct TileIndexes {
	int zIndex, xIndex;
};

class CorumCMap
{	
protected:
	
public:

	DWORD				m_dwTileNumWidth;
	DWORD				m_dwTileNumHeight;
	DWORD				dwTileSize;
	MAP_TILE*			m_pTile;	

	void				Create( DWORD dwID );
	void				Destroy();
	
	BOOL				SetMap(DWORD dwX, DWORD dwZ, MAP_TILE* pTile);
	BOOL				SetTileOccupied( DWORD dwX, DWORD dwZ, BYTE attr, LPVOID pType );
	
	MAP_TILE*			GetTileByIndexes(DWORD dwX, DWORD dwZ);
	MAP_TILE*			GetTileBy3DPosition(float fx, float fz);

	BOOL				ReadMap(DWORD dwID);

	TileIndexes			indexesFor3DPosition(float z, float x);

	std::string			debugDescription();
	CorumCMap();
	~CorumCMap();
};


extern CorumCMap*	g_pMap[ MAX_WORLD_NUM_PER_SERVER ];
extern DWORD	g_dwCurLandNum;

#endif // __MAP_H__