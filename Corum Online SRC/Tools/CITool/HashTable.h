#pragma once

#include "GuildInitView.h"
#include "AbyssHash.h"

class CGuildOfflineHash : public CAbyssHash<GUILD_OFFLINE_USER>
{
public:
	
	void DeleteData( void* pData );
};

class CGuildTableHash : public CAbyssHash<GUILD_TABLE>
{
public:
	
	void DeleteData( void* pData );
};

class CGuildWarHash : public CAbyssHash<GUILD_LIST>
{
public:
	
	void DeleteData( void* pData );
};