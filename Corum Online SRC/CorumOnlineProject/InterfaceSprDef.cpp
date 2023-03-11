#include "InterfaceSprDef.h"
#include "InterfaceSpr.h"
#include "InitGame.h"
#include "GameControl.h"


void InterfaceSprLoad(BYTE /*byType*/)
{
	INTERFACE_RESOURCE_INFO	sInterfaceResourceInfo[MAX_INTERFACE_RESOURCE_INFO];		
	
	ZeroMemory( sInterfaceResourceInfo, sizeof(sInterfaceResourceInfo) );

	int nTotalSize;
	int nMaxNode;
	int i;

	{
		INTERFACE_RESOURCE_INFO	sInterfaceResourceInfo_Back[MAX_INTERFACE_RESOURCE_INFO];

		nTotalSize	= DecodeCDBData( GetFile( _T("InterfaceResourceInfo.cdb"), DATA_TYPE_MANAGER ), sInterfaceResourceInfo_Back );
		nMaxNode	= nTotalSize / sizeof(INTERFACE_RESOURCE_INFO);

		for( i = 0; i < nMaxNode; i++)
		{
			memcpy( 
				&sInterfaceResourceInfo[sInterfaceResourceInfo_Back[i].wID], 
				&sInterfaceResourceInfo_Back[i], 
				sizeof(INTERFACE_RESOURCE_INFO)
				);
		}
	}

	INTERFACE_SPRITE_MANAGER sInterfaceSprite[MAX_INTERFACE_SPRITE_MANAGER];		

	nTotalSize	= DecodeCDBData( GetFile("InterfaceSpriteManager.cdb", DATA_TYPE_MANAGER), sInterfaceSprite );
	nMaxNode	= nTotalSize / sizeof(INTERFACE_SPRITE_MANAGER);

	// ��� ��������Ʈ�� �ڵ����� �����Ѵ� //
	LP_SPRITE_DATA lpSpriteData;
	INTERFACE_RESOURCE_INFO* pInterfaceResourceInfo;
	for( i = 0; i < nMaxNode; i++ )
	{		
		lpSpriteData		= (LP_SPRITE_DATA)LALAlloc(g_pInterfaceSprPool);	
		lpSpriteData->dwId	= sInterfaceSprite[i].wID;
		pInterfaceResourceInfo = &sInterfaceResourceInfo[sInterfaceSprite[i].wResourceID];

		if(!pInterfaceResourceInfo->wID)	continue;
		
		// �ؽ��ĸ� ��ü�� �о��.
		if (!pInterfaceResourceInfo->byType)
		{
			lpSpriteData->pSpr	= g_pRenderer->CreateSpriteObject(
				GetFile(pInterfaceResourceInfo->szFileName, DATA_TYPE_UI), 0);
		}
		else
		{
			lpSpriteData->pSpr	= g_pRenderer->CreateSpriteObject(
				GetFile(pInterfaceResourceInfo->szFileName, DATA_TYPE_UI),
				pInterfaceResourceInfo->wLeft, pInterfaceResourceInfo->wTop, 
				pInterfaceResourceInfo->wRight, pInterfaceResourceInfo->wBottom , 0);
		}

		g_pInterfaceSprHash->Insert( lpSpriteData, sInterfaceSprite[i].wID );
		
		if (!lpSpriteData->pSpr)
		{
			MessageBox( HWND_DESKTOP, pInterfaceResourceInfo->szFileName,"InterfaceSprLoad Failed:File Does Not Find!",MB_OK|MB_ICONSTOP);
		}
	}	
}


void InterfaceSprRemove(BYTE /*byType*/)
{
	if(g_pInterfaceSprHash)
		g_pInterfaceSprHash->Destroy(TRUE);
}
