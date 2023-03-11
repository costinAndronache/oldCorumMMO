#include "StdAfx.h"
#include "HashTable.h"

void CGuildOfflineHash::DeleteData(void* pData)
{	
	GUILD_OFFLINE_USER* pGuildOfflineTable = (GUILD_OFFLINE_USER*)pData;
	
	if(!pGuildOfflineTable)
		return;	
	
	memset(pGuildOfflineTable, 0, sizeof(GUILD_OFFLINE_USER));
	LALFree(g_pGuildOfflinePool, pGuildOfflineTable);	
	pGuildOfflineTable = NULL;
}

void CGuildTableHash::DeleteData(void* pData)
{
	GUILD_TABLE* pGuildTable = (GUILD_TABLE*)pData;
		
	if(pGuildTable->pGuildOfflineHash)
	{
		pGuildTable->pGuildOfflineHash->Destroy(TRUE);
		delete pGuildTable->pGuildOfflineHash;
		pGuildTable->pGuildOfflineHash = NULL;
	}
	
	if(!pGuildTable)
		return;	
	
	LALFree(g_pGuildTablePool, pGuildTable);	
}

void CGuildWarHash::DeleteData(void* pData)
{
	GUILD_LIST* pGuildList = (GUILD_LIST*)pData;
	
	if(!pGuildList)
		return;	
	
	memset(pGuildList, 0, sizeof(GUILD_LIST));
	LALFree(g_pGuildWarPool, pGuildList);	
	pGuildList = NULL;
}