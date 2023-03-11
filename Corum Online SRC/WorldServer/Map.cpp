#include "stdafx.h"
#include "Map.h"
#include "Section.h"
#include "OwnServer.h"
#include <crtdbg.h>


CMap*	g_pMap[ MAX_WORLD_NUM_PER_SERVER ] = {0,};
DWORD	g_dwCurLandNum = 0; 


CMap::CMap()
{
	memset(this, 0, sizeof(CMap));
	
}

CMap::~CMap()
{

	Destroy();
}


//-----------------------------------------------------------------------------
// �� ���Ϸ� ���� ��(Ÿ��)�� �����Ѵ�. �� ���� : �Ϲ� ���������� ��쿡�� ����Ǿ�� �Ѵ�. �ʺ��� ���������� ��� �̸� Load�س��� Data�� Copy�Ͽ� �ʱ�ȭ �Ѵ�. 
//-----------------------------------------------------------------------------
void CMap::Create(  DWORD dwID )
{
	if(ReadMap(dwID))
	{
		Log(LOG_JUST_DISPLAY, "@ Worldmap file read Successfully!");
	}
	else
	{
		Log(LOG_NORMAL, "Fail to read %d.ttb files!", dwID);
	}
}


//-----------------------------------------------------------------------------
// ���� ���� �Ѵ�. �� ���� : �Ϲ� ���������� ��쿡�� ����Ǿ�� �Ѵ�. �ʺ��� ���������� ��� �޸�Ǯ�� ���� �޸𸮸� �����Ѵ�. 
//-----------------------------------------------------------------------------
void CMap::Destroy()
{
	if(m_pTile)
	{
		delete [] m_pTile;
		m_pTile = NULL;
	}
}


//-----------------------------------------------------------------------------
// �Ӽ� ������ �о�帰��. 
//-----------------------------------------------------------------------------
BOOL CMap::ReadMap(DWORD dwID)
{
	// ���⼭ ���� �� ���� �о� ���̴� �۾� ...
	HANDLE	hFile = NULL;
	DWORD dwRead = 0,	dwTotalReadByte=0; 
	BOOL bResult = 0;

	char szFile[ _MAX_PATH ]={0,};
	wsprintf(szFile, "%s\\%d.ttb", g_pThis->GetServerDataPath(), dwID);
	
	hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		Log(LOG_IMPORTANT,"Fail to Open TTB File!(%d.ttb)", dwID );
		return FALSE;
	}
	
	DWORD dwTileSize, dwObjNum;
			
	//Read XTileMany				
	bResult = ReadFile(hFile, &m_dwTileNumWidth, sizeof(m_dwTileNumWidth), &dwRead, NULL);
	if(!bResult || dwRead != sizeof(m_dwTileNumWidth))	__asm int 3
		
	//Read ZTileMany				
	bResult = ReadFile(hFile, &m_dwTileNumHeight, sizeof(m_dwTileNumHeight), &dwRead, NULL);
	if(!bResult || dwRead != sizeof(m_dwTileNumHeight))	__asm int 3

	//Read TileSize
	bResult = ReadFile(hFile, &dwTileSize, sizeof(dwTileSize), &dwRead, NULL);
	if(!bResult || dwRead != sizeof(dwTileSize))	__asm int 3
		
	//Read ObjectNum
	bResult = ReadFile(hFile, &dwObjNum, sizeof(dwObjNum), &dwRead, NULL);
	if(!bResult || dwRead != sizeof(dwObjNum))	__asm int 3
	

	//Tile Memory Allocation
	m_pTile = new MAP_TILE[ m_dwTileNumWidth * m_dwTileNumHeight ];
	memset( m_pTile, 0x0, sizeof(MAP_TILE) * (m_dwTileNumWidth * m_dwTileNumHeight) );
	
	//Read TileAttribute
	MAP_TILE* pTile = NULL;
	
	for(DWORD i=0; i< (m_dwTileNumWidth * m_dwTileNumHeight); i++)
	{
		pTile = m_pTile + i;

		bResult = ReadFile(hFile, &pTile->wAttr, 2, &dwRead, NULL);
		if(!bResult || dwRead != 2)	__asm int 3

		if( pTile->wAttr.uAttr == 1 )	//�� 
			pTile->wAttr.uOccupied = TILE_OCCUPIED_DONT_MOVE;
	}
	
	//Read SectionMany, �� ���� ���� 								
	WORD	wTotalSectionNum;
	bResult = ReadFile(hFile, &wTotalSectionNum, sizeof(wTotalSectionNum), &dwRead, NULL);
	if(!bResult || dwRead != sizeof(wTotalSectionNum))	__asm int 3
	
	//Read Section Information
	for(i=0; i<wTotalSectionNum; i++)
	{
		dwTotalReadByte = 0;

		CSection* pSection = new CSection;

		//Read SectionMany								
		bResult = ReadFile(hFile, pSection, sizeof(Section_Link_Info), &dwRead, NULL);
		if(!bResult || dwRead != sizeof(Section_Link_Info))	__asm int 3		
	}	

	CloseHandle(hFile);

	return TRUE;
}

//-----------------------------------------------------------------------------
// �������� �ε����� Ÿ���� ������. 
//-----------------------------------------------------------------------------
BOOL CMap::SetMap(DWORD dwX, DWORD dwZ, MAP_TILE* pTile)
{
	if(dwX >= m_dwTileNumWidth || dwZ >= m_dwTileNumHeight)	
		return FALSE;
	
	memcpy( m_pTile + (m_dwTileNumWidth * dwZ + dwX), pTile, sizeof(MAP_TILE));  
	return TRUE;
}


BOOL CMap::SetTileOccupied( DWORD dwX, DWORD dwZ, BYTE attr, LPVOID pType )
{
	if(dwX >= m_dwTileNumWidth || dwZ >= m_dwTileNumHeight)
	{	
		return FALSE;
	}
	
	if( m_pTile[ m_dwTileNumWidth * dwZ + dwX ].wAttr.uOccupied == TILE_OCCUPIED_DONT_MOVE ) 
	{
		return FALSE;
	}

	if( m_pTile[ m_dwTileNumWidth * dwZ + dwX ].pData == pType ) goto lb_set;
	if( m_pTile[ m_dwTileNumWidth * dwZ + dwX ].pData == NULL ) goto lb_set; 

	return FALSE;

lb_set:	

	// ����
	if( attr == TILE_EMPTY )
	{
		m_pTile[ m_dwTileNumWidth * dwZ + dwX ].pData = NULL;
	}
	else 
	{
		m_pTile[ m_dwTileNumWidth * dwZ + dwX ].pData = pType;
	}
	m_pTile[ m_dwTileNumWidth * dwZ + dwX ].wAttr.uOccupied = attr;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Ÿ�� �ε��� ��ǥ�� Ÿ�� �����͸� ���� 
//-----------------------------------------------------------------------------
MAP_TILE* CMap::GetMap(DWORD dwX, DWORD dwZ)
{
	if( dwX >= m_dwTileNumWidth || dwZ >= m_dwTileNumHeight )	
		return NULL;

	return ( m_pTile + (m_dwTileNumWidth * dwZ + dwX) );
}

//-----------------------------------------------------------------------------
// ��ǥ�� Ÿ�� �����͸� ����. 
//-----------------------------------------------------------------------------
MAP_TILE* CMap::GetTile(float fx, float fz)
{
	if(fx < 0 || fz < 0)	return NULL;

	DWORD x = (DWORD)( fx / TILE_WIDTH );
	DWORD z = (DWORD)( fz / TILE_HEIGHT );
	
	return GetMap(x, z);
}