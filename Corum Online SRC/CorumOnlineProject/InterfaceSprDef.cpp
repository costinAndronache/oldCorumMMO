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
		DecodeCDBDataResult result = DecodeCDBData(GetFile(_T("InterfaceResourceInfo.cdb"), DATA_TYPE_MANAGER));
		INTERFACE_RESOURCE_INFO	*sInterfaceResourceInfo_Back = (INTERFACE_RESOURCE_INFO*)result.buffer;

		nTotalSize = result.size;
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

	DecodeCDBDataResult result = DecodeCDBData(GetFile("InterfaceSpriteManager.cdb", DATA_TYPE_MANAGER));
	INTERFACE_SPRITE_MANAGER *sInterfaceSprite = (INTERFACE_SPRITE_MANAGER*)result.buffer;

	nTotalSize = result.size;
	nMaxNode	= nTotalSize / sizeof(INTERFACE_SPRITE_MANAGER);

	// 모든 스프라이트는 자동으로 생성한다 //
	LP_SPRITE_DATA lpSpriteData;
	INTERFACE_RESOURCE_INFO* pInterfaceResourceInfo;
	for( i = 0; i < nMaxNode; i++ )
	{		
		lpSpriteData = new SSPRITE_DATA;
		lpSpriteData->dwId	= sInterfaceSprite[i].wID;
		pInterfaceResourceInfo = &sInterfaceResourceInfo[sInterfaceSprite[i].wResourceID];

		if(!pInterfaceResourceInfo->wID)	continue;
		
		// 텍스쳐를 통체로 읽어라.
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
