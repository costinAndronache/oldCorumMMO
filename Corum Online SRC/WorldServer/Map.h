//----------------------------------------------------------------------------------------------//
// Map Class
//----------------------------------------------------------------------------------------------//

#ifndef __MAP_H__
#define __MAP_H__

#include "StdAfx.h"
#include "../CommonServer/CommonClientDungeonWorld.h"

#pragma once


class CorumCMap
{	
protected:
	
public:

	DWORD				m_dwTileNumWidth;
	DWORD				m_dwTileNumHeight;
		
	MAP_TILE*			m_pTile;	

	void				Create( DWORD dwID );
	void				Destroy();
	
	BOOL				SetMap(DWORD dwX, DWORD dwZ, MAP_TILE* pTile);
	BOOL				SetTileOccupied( DWORD dwX, DWORD dwZ, BYTE attr, LPVOID pType );
	
	MAP_TILE*			GetMap(DWORD dwX, DWORD dwZ);
	MAP_TILE*			GetTile(float fx, float fz);

	BOOL				ReadMap(DWORD dwID);

	CorumCMap();
	~CorumCMap();
};


extern CorumCMap*	g_pMap[ MAX_WORLD_NUM_PER_SERVER ];
extern DWORD	g_dwCurLandNum;

#endif // __MAP_H__