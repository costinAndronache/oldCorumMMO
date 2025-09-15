#include "StdAfx.h"
#include "DUNGEON_DATA_EX.h"
#include "OwnServer.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "User.h"


DUNGEON_DATA_EX::DUNGEON_DATA_EX()
{
	memset((CZP_REQUEST_DUNGEON_INFO_DG_ResultRow*)this, 0, sizeof(CZP_REQUEST_DUNGEON_INFO_DG_ResultRow));
}

DUNGEON_DATA_EX::~DUNGEON_DATA_EX()
{

}

WORD DUNGEON_DATA_EX::GetLevel() const
{
	// 던전 레벨
	return WORD(m_wUpgradeLevel+m_wInitLevel);
}

WORD DUNGEON_DATA_EX::GetUpgradeTime()
{
	// 던전 업그레이드 시간
	return WORD((1+(sqrt((double)m_wInitLevel))+m_wUpgradeLevel/2)*1000*60*60);
}

int	DUNGEON_DATA_EX::GetIdleTime()
{
	// 공성전 할때까지의 휴식기간.
	return (m_wUpgradeLevel/2+12)*1000*60*60;
}

DWORD DUNGEON_DATA_EX::GetEntrancePay()
{
	return entranceFee;
}

DWORD DUNGEON_DATA_EX::GetMaxEntrancePay()
{
	return DWORD(sqrt((double)(GetLevel()*10)))*30 + GetLevel()*10;
}
	
BOOL DUNGEON_DATA_EX::LevelUpgrade()
{
	m_wUpgradeLevel = (WORD)min(m_wUpgradeLevel+1, 15);
	return TRUE;
}

void DUNGEON_DATA_EX::DecisionDungeonType(DWORD dwID)
{
	m_lDungeonType = DungeonIDtoType(dwID);
}

int DUNGEON_DATA_EX::GetDungeonType() const
{
	return m_lDungeonType;
}

int DUNGEON_DATA_EX::GetRemainTime(DWORD dwDestTime, DWORD dwCurTick, DWORD dwFlag)
{
	DWORD dwSec = abs((double)(dwDestTime - dwCurTick)) / 1000;

	int nHour = dwSec / 3600;
	int nMinute = ((dwSec % 3600) / 60) % 60;
	int nSec = (dwSec - (3600*nHour + 60 * nMinute)) % 60;

	switch(dwFlag)
	{
	case TIME_HOUR:
		return nHour;
		
	case TIME_MIN:
		return nMinute;
		
	case TIME_SEC:
		return nSec;
	}
	return -1;
}

DWORD DUNGEON_DATA_EX::GetCPTime() const
{
	return GetLevel()/10+10*1000;
}

BOOL DUNGEON_DATA_EX::IsStartWarTime()
{
	return int(m_dwRemainSiegeStartTick - timeGetTime()) <= 0;
}

BOOL DUNGEON_DATA_EX::IsEndWarTime()
{
	return int(m_dwRemainSiegeEndTick - timeGetTime()) <= 0;
}

BOOL DUNGEON_DATA_EX::IsLevelUpgradeTime()
{
	return int(m_dwRemainLevelStartTick - timeGetTime()) <= 0;
}

BOOL DUNGEON_DATA_EX::IsCreateProductionTime()
{
	return int(m_dwRemainGradeCraeteTick - timeGetTime()) <= 0;
}

void DUNGEON_DATA_EX::SetOwner(DWORD dwOwnerIndex, DWORD dwGuildID, char* pOwnerID, char* pOwnerName)
{
	ownerUserID = dwOwnerIndex;
	ownerGuildID = dwGuildID;
	
	__lstrcpyn(m_szOwner, pOwnerName, MAX_CHARACTER_REAL_LENGTH);
	
	if (pOwnerID)
	{
		DSTWS_DUNGEON_OWNER_CHANGE packet;
		packet.wDungeonID = (WORD)m_dwID;
		packet.dwOwnerIndex = dwOwnerIndex;
		packet.dwGuildId = dwGuildID;

		memset(packet.szUserLoginID, 0, MAX_CHARACTER_REAL_LENGTH);
		memset(packet.szCharacterName, 0, MAX_CHARACTER_REAL_LENGTH);

		__lstrcpyn(packet.szUserLoginID, pOwnerID,MAX_CHARACTER_REAL_LENGTH );
		__lstrcpyn(packet.szCharacterName, pOwnerName, MAX_CHARACTER_REAL_LENGTH);
		g_pNet->SendToServer(WSINDEX, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	}
}

void DUNGEON_DATA_EX::SetDefenseCount(BYTE byDefenseCount, BOOL bDB_Update)
{
	m_byDefenseCount = byDefenseCount;
	DSTWS_REFRESH_DEFENSECOUNT ServerPacket;
	ServerPacket.wDungeonID = (WORD)m_dwID;
	// 획득한 돈
	ServerPacket.byDefenseCount = byDefenseCount;
	g_pNet->SendToServer(WSINDEX, (char*)&ServerPacket, ServerPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	
	if (bDB_Update)
	{
		char szQuery[0xff]={0,};
		wsprintf(szQuery, "update mapinfo set DefenseCount = %d where id = %d", m_byDefenseCount, m_dwID);
		g_pDb->THExecuteSQL(szQuery, FALSE, FALSE, NULL, (BYTE)GAME_DB);
	}	
}

void DUNGEON_DATA_EX::SetEntrance(DWORD dwEntrance)
{
	entranceFee = dwEntrance;

	char szQuery[0xff]={0,};
	wsprintf(szQuery, "update mapinfo set Entrance = %d where id = %d", entranceFee, m_dwID);
	g_pDb->THExecuteSQL(szQuery, FALSE, FALSE, NULL, (BYTE)GAME_DB);

	DSTWS_REFRESH_ENTRANCE_EDIT ServerPacket;
	ServerPacket.wDungeonID = (WORD)m_dwID;
	// 획득한 돈
	ServerPacket.nMoney = entranceFee; 
	g_pNet->SendToServer(WSINDEX, (char*)&ServerPacket, ServerPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}

int DUNGEON_DATA_EX::SetAccEntrance(int nPlusMoney)
{
	DSTWS_REFRESH_ACCENTRANCE_ACQUISITION ServerPacket;
	ServerPacket.wDungeonID = (WORD)m_dwID;
	// 획득한 돈
	ServerPacket.nMoney = nPlusMoney; 
	
	g_pNet->SendToServer(WSINDEX, (char*)&ServerPacket, ServerPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

	if (nPlusMoney < 0)
	{
		if (accumulatedEntranceFees < (DWORD)abs(nPlusMoney))
			accumulatedEntranceFees = 0;	
		else
			accumulatedEntranceFees += nPlusMoney;
	}
	else
	{
		if (0xffffffff-accumulatedEntranceFees > (DWORD)nPlusMoney)
			accumulatedEntranceFees += nPlusMoney;
		else
			accumulatedEntranceFees = 0xffffffff;
	}
	
	char szQuery[0xff]={0,};
	wsprintf(szQuery, "update mapinfo set AccEntrance = %u where id = %d", accumulatedEntranceFees, m_dwID);
	g_pDb->THExecuteSQL(szQuery, FALSE, FALSE, NULL, (BYTE)GAME_DB);
	
	return nPlusMoney;
}

WORD DUNGEON_DATA_EX::ConvertProduction()
{
	WORD wProduction = g_DungeonProductionItemMinMax[siegeDungeonPrize_productionSchemeIndex].wItemIDMin;
	BYTE byRandom = BYTE(g_DungeonProductionItemMinMax[siegeDungeonPrize_productionSchemeIndex].wItemIDMax-wProduction);

	return WORD( (byRandom) ? (wProduction + (rand() % byRandom)) : wProduction);
}

void DUNGEON_DATA_EX::InitializeOperationMode()
{
	if (m_cMagicFieldArrayItem.GetQuantity())
	{
		// 원소 속성석이 아니라면
		if (g_DungeonProductionItemMinMax[siegeDungeonPrize_productionSchemeIndex].wItemIDDefault != siegeDungeonPrize_itemID)
			SetOperationMode(DUNGEON_OPERATIONTYPE_CONVERSION);
	}
	else
	{
		// 완성했다.
		if (siegeDungeonPrize_currentMakingStep >= 3)
			SetOperationMode(DUNGEON_OPERATIONTYPE_PRODUCTION);
		else
			SetOperationMode(DUNGEON_OPERATIONTYPE_PRECOCIOUS);
	}	
}

WORD DUNGEON_DATA_EX::CreateProduction(BOOL bWin)
{
	// 원소 속성석 하나 생성해줘라.
	if (bWin)
	{	
		// 원소 속성석이었다면
		if (g_DungeonProductionItemMinMax[siegeDungeonPrize_productionSchemeIndex].wItemIDDefault == siegeDungeonPrize_itemID)
		{
			// 갯수를 증가해줘라.
			if (!siegeDungeonPrize_currentMakingStep)
			{
				siegeDungeonPrize_currentMakingStep = 1;
				SetOperationMode(DUNGEON_OPERATIONTYPE_PRECOCIOUS);
			}
			// 다음 생산품으로 변해라.
			else
			{
				siegeDungeonPrize_itemID = ConvertProduction();
				siegeDungeonPrize_currentMakingStep = 0;
				SetOperationMode(DUNGEON_OPERATIONTYPE_PRECOCIOUS);
			}
		}
		else
		{
			// 완성도를 멈추고, 생산품을 바꿔줘라.
			if (current_DUNGEON_OPERATIONTYPE == DUNGEON_OPERATIONTYPE_CONVERSION)
			{
				siegeDungeonPrize_itemID = ConvertProduction();
			}
			// 완성도를 증가해라.
			else
			{
				if (siegeDungeonPrize_currentMakingStep < 3)
					siegeDungeonPrize_currentMakingStep+=1;				
			}
			
			InitializeOperationMode();
		}
	}
	else
	{
		//완성도를 줄여라.
		if (g_DungeonProductionItemMinMax[siegeDungeonPrize_productionSchemeIndex].wItemIDDefault == siegeDungeonPrize_itemID)
		{
			siegeDungeonPrize_currentMakingStep = 0;
			SetOperationMode(DUNGEON_OPERATIONTYPE_PRECOCIOUS);			
		}
		else
		{
			if (siegeDungeonPrize_currentMakingStep > 0)
			{
				siegeDungeonPrize_currentMakingStep-=1;
			}
			else
			{
				siegeDungeonPrize_currentMakingStep = 1;
				siegeDungeonPrize_itemID = g_DungeonProductionItemMinMax[siegeDungeonPrize_productionSchemeIndex].wItemIDDefault;
			}
			SetOperationMode(DUNGEON_OPERATIONTYPE_PRECOCIOUS);
		}
	}

	DSWST_CREATEPRODUCTION packet;
	packet.wDungeonID = (WORD)m_dwID;
	packet.byProductionCount = siegeDungeonPrize_currentMakingStep;
	packet.wProduction = siegeDungeonPrize_itemID;
	g_pNet->SendToServer(WSINDEX, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	
	char szQuery[0xff]={0,};
	wsprintf(szQuery, "update mapinfo set Production = %d, ProductionCount = %d where id = %d", siegeDungeonPrize_itemID, siegeDungeonPrize_currentMakingStep, m_dwID);
	g_pDb->THExecuteSQL(szQuery, FALSE, FALSE, NULL, (BYTE)GAME_DB);

	return siegeDungeonPrize_itemID;
}

void DUNGEON_DATA_EX::SetOperationMode(BYTE byOperationMode)
{
	current_DUNGEON_OPERATIONTYPE = byOperationMode;
	DSTWS_DUNGEON_REFRESH_OPERATIONMODE packet;
	packet.wDungeonID = (WORD)m_dwID;
	packet.byOperationMode = byOperationMode;
	g_pNet->SendToServer(WSINDEX, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	
	char szQuery[0xff]={0,};
	wsprintf(szQuery, "update mapinfo set OperationType = %d where id = %d", current_DUNGEON_OPERATIONTYPE, m_dwID);
	g_pDb->THExecuteSQL(szQuery, FALSE, FALSE, NULL, (BYTE)GAME_DB);
}

const CItem* DUNGEON_DATA_EX::GetGuardianItem()
{
	return &m_cGuardianItem;
}


CItem* DUNGEON_DATA_EX::GetMagicFieldArrayItem()
{
	return &m_cMagicFieldArrayItem;
}

void DUNGEON_DATA_EX::SetMagicFieldArray(CItem* pItem)
{
	if(!pItem)
		return;
		
	memcpy(&m_cMagicFieldArrayItem, pItem, sizeof(CItem));

	InitializeOperationMode();
	
	// 월드서버에 알려줘라.
	DSTWS_DUNGEON_MAGICARRAY_ATTACH packet;
	
	memcpy(&packet.cItem, &m_cMagicFieldArrayItem, sizeof(CItem));
	packet.wDungeonID = (WORD)m_dwID;
	g_pNet->SendToServer(WSINDEX, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

	char	szQuery[1024]	={0,};
	int		nColumn			= 1;	
	int		col				= 0;
	DWORD	dwParamOffset	= 0;
	DBBINDING* pBinding = g_pDb->THCreateParamInfo((WORD)nColumn);
	
	if(!pBinding)
	{
		Log(LOG_IMPORTANT, "ParameterPool is Full!!  in SetMagicFieldArray Function()!");
		return;
	}

	pBinding[col].obValue = dwParamOffset;
	pBinding[col].cbMaxLen = sizeof(m_cMagicFieldArrayItem);
	pBinding[col++].wType = DBTYPE_BYTES;					
	dwParamOffset += sizeof(m_cMagicFieldArrayItem);

	wsprintf( szQuery, "update mapinfo set MagicArrayItem = ? where id = %d ", m_dwID );
	g_pDb->THExecuteSQLByParam(szQuery, pBinding, &m_cMagicFieldArrayItem, sizeof(CItem), (BYTE)nColumn, FALSE, NULL, NULL, (BYTE)GAME_DB);
}


void DUNGEON_DATA_EX::SetGuardianItem(CItem* pItem)
{
	if(!pItem)
		return;

	memcpy(&m_cGuardianItem, pItem, sizeof(CItem));

	char	szQuery[1024]={0,};
	int		nColumn = 1;	
	int		col = 0;
	DWORD	dwParamOffset = 0;
	DBBINDING* pBinding = g_pDb->THCreateParamInfo((WORD)nColumn);

	if(!pBinding)
	{
		Log(LOG_IMPORTANT, "ParameterPool is Full!!  in SetGuardianItem Function()!");
		return;
	}

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(m_cGuardianItem);
	pBinding[col++].wType	= DBTYPE_BYTES;					
	dwParamOffset += sizeof(m_cGuardianItem);	
	
	wsprintf( szQuery, "update mapinfo set GuardianItem = ? where id = %d ", m_dwID );
	g_pDb->THExecuteSQLByParam(szQuery, pBinding, &m_cGuardianItem, sizeof(CItem), (BYTE)nColumn, FALSE, NULL, NULL, (BYTE)GAME_DB);

	// 월드서버에 알려줘라.
	DSTWS_DUNGEON_GUARDIANITEM_ATTACH packet;
	
	memcpy(&packet.cItem, &m_cGuardianItem, sizeof(CItem));
	packet.wDungeonID = (WORD)m_dwID;
	g_pNet->SendToServer(WSINDEX, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}

void DUNGEON_DATA_EX::SetAccExp(DWORD dwExp)
{
	accumulatedEXPForOwner = dwExp;

	DSTWS_REFRESH_ACCEXP_ACQUISITION ServerPacket;
	ServerPacket.wDungeonID = (WORD)m_dwID;
	ServerPacket.dwAccExp = accumulatedEXPForOwner;	
	g_pNet->SendToServer(WSINDEX, (char*)&ServerPacket, ServerPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

	char szQuery[0xff]={0,};
	wsprintf(szQuery, "update mapinfo set accexp = %d where id = %d",accumulatedEXPForOwner, m_dwID);
	g_pDb->THExecuteSQL(szQuery, FALSE, FALSE, NULL, (BYTE)GAME_DB);
}

BOOL DUNGEON_DATA_EX::IsDungeonOwner(const CUser* const pUser) const
{
	return (ownerUserID == pUser->GetID() || (ownerGuildID && pUser->m_dwGuildId == ownerGuildID && pUser->m_byRank == 1));
}

BOOL DUNGEON_DATA_EX::IsPathWay()const
{
	return (DUNGEON_TYPE_TUNNEL== m_lDungeonType);
}

BOOL DUNGEON_DATA_EX::IsEventDungeon()const
{
	return (DUNGEON_TYPE_EVENT == m_lDungeonType);
}
BOOL DUNGEON_DATA_EX::isSiegeDungeon()const
{
	return (DUNGEON_TYPE_CONQUER == m_lDungeonType);
}

BOOL DUNGEON_DATA_EX::IsVillage()const
{
	return (DUNGEON_TYPE_VILLAGE == m_lDungeonType);
}

BOOL DUNGEON_DATA_EX::IsStadium() const//길드전 맵이냐?: hwoarang 050202
{
	return (DUNGEON_TYPE_MATCH_STATIDUM == m_lDungeonType);
}
