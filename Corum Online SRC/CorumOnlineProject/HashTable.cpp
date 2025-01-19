#include "stdafx.h"
#include "HashTable.h"
#include "User.h"
#include "Monster.h"		
#include "Npc.h"
#include "InitGame.h"
#include "struct.h"
#include "WorldProcess.h"

void CUserHash::DeleteData( void* pData )
{

}

void CMonsterHash::DeleteData( void* pData )
{
	
}

void CEffectHash::DeleteData( void* pData )
{	

}


void CGuildInfoHash::DeleteData( void* pData )
{	
	LP_GUILD_DATA pGuildInfoTable = (GUILD_DATA*)pData;

	if(!pGuildInfoTable)
		return;	
		
	delete pGuildInfoTable;
}


void CGroupInfoHash::DeleteData(void* pData)
{
	LP_GROUPINFO_TABLE pGroupInfoTable = (LP_GROUPINFO_TABLE)pData;

	if(!pGroupInfoTable)
		return;	
		
	delete pGroupInfoTable;
}

void CItemStoreHash::DeleteData(void* pData)
{
	LP_ITEM_STORE pItemStore = (LP_ITEM_STORE)pData;

	if(!pItemStore)
		return;	

	delete pItemStore;
}

void CHelpInfoHash::DeleteData(void* pData)
{
	LP_HELP_INFO pHelpInfoTable = (LP_HELP_INFO)pData;

	if(!pHelpInfoTable)
		return;	
		
	delete pHelpInfoTable;
}

void CItemResourceHash::DeleteData(void* pData)
{
	LP_ITEM_RESOURCE_EX pItemResource = (LP_ITEM_RESOURCE_EX)pData;

	if(!pItemResource)
		return;	
		
	if(pItemResource->pSpr)
	{
		pItemResource->pSpr->Release();
		pItemResource->pSpr = NULL;
	}

	delete pItemResource;
	pItemResource = NULL;
}

void CItemOptionHash::DeleteData(void* pData)
{
	LP_ITEM_OPTION pItemOption = (LP_ITEM_OPTION)pData;

	if(!pItemOption)
		return;	
		
	delete pItemOption;
}

void CSetItemInfoHash::DeleteData( void* pData )
{
	LPSET_ITEM_INFO pInfo = (LPSET_ITEM_INFO)pData;

	if( !pInfo ) return;

	delete pInfo;
}

void CItemMakingInfoHash::DeleteData( void* pData )
{
	LPITEM_MAKING_INFO pInfo = (LPITEM_MAKING_INFO)pData;

	if( !pInfo ) return;

	delete pInfo;
}


void CPartyUserHash::DeleteData( void* pData )
{
	LP_PARTY_USER pParty = (LP_PARTY_USER)pData;
	if(!pParty)	return;

	FreeWorldPartyUser( pParty );
	delete pParty;
}

void CGuildUserHash::DeleteData( void* pData )
{
	LP_GUILD_USER pGuildUser = (LP_GUILD_USER)pData;

	if(!pGuildUser)
		return;

	delete pGuildUser;
}

void CGuildOffLineHash::DeleteData( void* pData )
{
	LP_GUILD_OFFLINE_USER pGuildOfflineUser = (LP_GUILD_OFFLINE_USER)pData;

	if(!pGuildOfflineUser)
		return;

	delete pGuildOfflineUser;
}

void CMessengerUserHash::DeleteData( void* pData )
{
	LP_MESSENGER_USER pMessengerUser = (LP_MESSENGER_USER)pData;

	if(!pMessengerUser)
		return;

	delete pMessengerUser;
}

void CGuildListHash::DeleteData( void* pData )
{
	LP_GUILD_WAR pGuildWar = (LP_GUILD_WAR)pData;

	if(!pGuildWar)
		return;

	delete pGuildWar;
}

void CPartyBoardHash::DeleteData( void* pData )
{
	LP_PARTY_BOARD lpPartyBoard = (LP_PARTY_BOARD)pData;

	if(!lpPartyBoard)
		return;

	delete lpPartyBoard;
	memset(lpPartyBoard, 0, sizeof(SPARTY_BOARD));
	lpPartyBoard = NULL;
}
