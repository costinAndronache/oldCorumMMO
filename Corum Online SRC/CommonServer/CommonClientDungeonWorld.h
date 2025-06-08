#pragma once
#include <windows.h>
#include <windowsx.h>

#include "CommonServerExports.h"

#define	TILE_SIZE		125.0f
#define TILE_SIZE_WORLD	80.0f
#define	TILE_HEIGHT	125.0f
#define	TILE_WIDTH	125.0f


#define GRID_WIDTH		1250.0f		
#define GRID_HEIGHT		1250.0f		


#define TILE_X(X)	(float)( X * TILE_WIDTH + (TILE_WIDTH * 0.5f ) )
#define TILE_Z(Z)	(float)( Z * TILE_HEIGHT + (TILE_HEIGHT * 0.5f ) )

#define WORLDTILE_X(X)	(float)( X * TILE_SIZE_WORLD + (TILE_SIZE_WORLD * 0.5f ) )
#define WORLDTILE_Z(Z)	(float)( Z * TILE_SIZE_WORLD + (TILE_SIZE_WORLD * 0.5f ) )

#define TILE_EMPTY					0
#define TILE_OCCUPIED_PC			1
#define TILE_OCCUPIED_NPC			2
#define TILE_OCCUPIED_MONSTER		3
#define TILE_OCCUPIED_DONT_MOVE		7
#define TILE_OCCUPIED_SKILL			8

#pragma pack( push, 1 )

struct TILE_ATTR
{
	WORD uAttr : 4;		//  0 ~ 15
	WORD uOccupied : 4;		//	0 ~ 15
	WORD uSection : 8;		//  0 ~ 255
};

struct MAP_TILE
{
	float	x;
	float	z;
	WORD	wIndex_X;	//배열 Index값 
	WORD	wIndex_Z;

	TILE_ATTR wAttr;
	LPVOID	pData;		// 점유하고 있는것이 있다면 그 객체의 포인터이다.
};
struct SPOT_INFO
{
	WORD wDestLayerID[10];
	WORD wDestSpotID[5];
};
struct SPOT_VALUE
{
	DWORD		dwMapID;
	SPOT_INFO	spot[20];
};
#pragma pack (pop)