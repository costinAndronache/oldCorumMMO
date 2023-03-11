#include "stdafx.h"
#include "DUNGEON_DATA_EX.h"
#include "worlduser.h"
#include "worldusertable.h"
#include "GuildHash.h"
#include "recvmsg.h"
#include "ServerFunction.h"


DUNGEON_DATA_EX::DUNGEON_DATA_EX()
{
	memset((DUNGEON_DATA*)this, 0, sizeof(DUNGEON_DATA));
	m_pListSendToUser			= new CDugneonRefreshUserList;
	m_dwRemainSiegeStartTick	= 0;
	m_dwRemainSiegeEndTick		= 0;
	m_dwRemainLevelStartTick	= 0;
	m_dwStartLevelStartTick		= g_dwCurTick;
	
	m_pListPos					= NULL;
	m_pServer					= NULL;

	m_pNextData					= NULL;
	m_pPrevData					= NULL;

	m_bEventState				= EVENT_DUNGEON_CLOSE;
	m_dwEventLastActionTick		= 0;
	m_iEventEntered				= 0;	
}


DUNGEON_DATA_EX::~DUNGEON_DATA_EX()
{
	m_pListSendToUser->RemoveAll();
	delete m_pListSendToUser;
}


INT DUNGEON_DATA_EX::GetDungeonType()
{
	return m_lDungeonType;
}


void DUNGEON_DATA_EX::SetDungeonType(WORD wID)
{
	m_lDungeonType = DungeonIDtoType(wID);
}


WORD DUNGEON_DATA_EX::GetLevel()
{
	// ���� ����
	return WORD(m_wUpgradeLevel+m_wInitLevel);
}


WORD DUNGEON_DATA_EX::GetUpgradeTime()
{
	// ���� ���׷��̵� �ð�
	return WORD((1+(sqrt((double)m_wInitLevel))+m_wUpgradeLevel/2)*1000*60*60);
}


int	DUNGEON_DATA_EX::GetIdleTime()
{
		return (int)m_dwPieceCoolTime;	
}


DWORD DUNGEON_DATA_EX::GetEventLastActionTick()
{
	return m_dwEventLastActionTick;
}


void DUNGEON_DATA_EX::SetSiegeStartDestTime(DWORD dwTick)
{
	

	// ������ �� �ð� �����Ѵ�.
	m_bSiege = FALSE;
	m_dwRemainSiegeStartTick = g_dwCurTick + dwTick;

	
}


void DUNGEON_DATA_EX::SetSiegeEndDestTime(DWORD dwTick)
{

	m_bSiege = TRUE;
	m_dwRemainSiegeEndTick = g_dwCurTick + dwTick;
	
}


BOOL DUNGEON_DATA_EX::IsStartWarTime()
{
	
	return int(m_dwRemainSiegeStartTick - g_dwCurTick) <= 0;
}


BOOL DUNGEON_DATA_EX::IsEndWarTime()
{
	
	return int(m_dwRemainSiegeEndTick - g_dwCurTick) <= 0;
}


// ���������� ���Ź޾ƾ߸� �ϴ� �����鿡�Ը� ������.
void DUNGEON_DATA_EX::SendToUsers(char* pPacket, DWORD dwSize)
{
	POSITION_ pos = m_pListSendToUser->GetHeadPosition();

	while(pos)
	{
		CWorldUser* pUser = (CWorldUser*)m_pListSendToUser->GetNext(pos);
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, pPacket, dwSize, FLAG_SEND_NOT_ENCRYPTION);
	}
}


void DUNGEON_DATA_EX::SendStartSiege()
{
	
	SetSiegeEndDestTime(m_wBattleTimeLimit * 1000 * 60);
	
	// Ŭ���̾�Ʈ���� �������.
	WSTC_REFRESHEVENT_DUNGEON_STARTSIEGE packet;
	packet.wDungeonID		= (WORD)m_dwID;
	packet.dwSiegeEndTick	= int(m_dwRemainSiegeEndTick - g_dwCurTick);		
	SendToUsers((char*)&packet, packet.GetPacketSize());
	
	// ���� �������� �������.
	if(m_pServer && m_pServer->dwServerStatus == SERVER_STATUS_CONNECTED)	
	{
		WSTS_START_SIEGE packet;
		packet.wDungeonID = (WORD)m_dwID;		
		g_pNet->SendToServer(m_pServer->dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}


void DUNGEON_DATA_EX::SendEndSiege(BYTE bDefenseSuccess)
{

	
	SetSiegeStartDestTime( GetIdleTime() - (m_wBattleTimeLimit * 60 * 1000));//(�ֱ� - ��Ÿ��)
		
	// �������� �������.	
	WSTC_REFRESHEVENT_DUNGEON_ENDSIEGE	packet;
	packet.wDungeonID		= (WORD)m_dwID;
	packet.bDefenseSuccess	= bDefenseSuccess;
	packet.dwSiegeStartTick = int(m_dwRemainSiegeStartTick-g_dwCurTick);
	SendToUsers((char*)&packet, packet.GetPacketSize());
	SendRefreshLevelUp();	
	
	// ������������ �������.
	if(m_pServer && m_pServer->dwServerStatus == SERVER_STATUS_CONNECTED)	
	{
		WSTS_END_SIEGE packet;
		packet.wDungeonID = (WORD)m_dwID;
		g_pNet->SendToServer(m_pServer->dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}		

	// idle ���۽ð��� �����س��ƶ�.
	struct tm when;
	time_t now, result;

	time( &now );
	when = *localtime( &now );
	when.tm_min -= m_wBattleTimeLimit;
	result = mktime(&when);

	char szQuery[0xff] = {0,};
	DWORD dwStartTime = when.tm_min + when.tm_hour * 100 + when.tm_mday * 10000 + (when.tm_mon + 1) * 1000000 + (when.tm_year -100 ) * 100000000;
	wsprintf(szQuery, "update mapinfo set piecestarttime = %u where id = %d", dwStartTime, m_dwID);
	g_pDb->THExecuteSQL(szQuery, FALSE, 0, NULL, (BYTE)GAME_DB);
}


void DUNGEON_DATA_EX::SetDefenseCount(BYTE byDefenseCount)
{
	m_byDefenseCount = byDefenseCount;

	WSTC_REFRESHEVENT_DEFENSECOUNT packet;
	packet.wDungeonID		= (WORD)m_dwID;
	packet.byDefenseCount	= m_byDefenseCount;
	SendToUsers((char*)&packet, packet.GetPacketSize());
}


BOOL DUNGEON_DATA_EX::StartSiegeWarTime()
{
	// ������ �����ص� �Ǵ°�?
	if (m_bSiege == FALSE)	
	{
		// ������ �ƴҶ�.
		if (IsStartWarTime())
		{
			StopOperationTime();
			SendStartSiege();
			return TRUE;
		}
	}
	else	
	{
		// �������϶� ���� �ð������Ƿ�.
		if (IsEndWarTime())
		{
			ResumeOperationTime();
			SendEndSiege(TRUE);
		}
	}
	
	return FALSE;
}


BOOL DUNGEON_DATA_EX::IsLevelUpgradeTime()
{
	return m_dwRemainLevelStartTick <= g_dwCurTick;
}


// �������� ���۵Ǹ� ������, ��������� �ð��� ���߰� �ȴ�.
void DUNGEON_DATA_EX::StopOperationTime()
{
	m_dwStopStartTick = g_dwCurTick;
	m_dwStopLevelTick = m_dwRemainLevelStartTick - g_dwCurTick;
}


// �������� ���߰� �Ǹ� ������, ��������� �ð��� �ٽ� �帣�� �ȴ�.
void DUNGEON_DATA_EX::ResumeOperationTime()
{
	m_dwRemainLevelStartTick = m_dwStopLevelTick + g_dwCurTick;
	m_dwStartLevelStartTick += g_dwCurTick - m_dwStopStartTick;	
}


void DUNGEON_DATA_EX::SendRefreshLevelUp()
{
	WSTC_REFRESHEVENT_DUNGEON_LEVELUP packet;
	packet.wDungeonID			= (WORD)m_dwID;
	packet.dwLevelUpDestTick	= m_dwRemainLevelStartTick - g_dwCurTick;
	SendToUsers((char*)&packet, packet.GetPacketSize());
}


void DUNGEON_DATA_EX::SendRefreshOwner()
{
	// ��� �������� ������.
	WSTDS_DUNGEON_MESSAGE SendPacket;
	memset(SendPacket.szMessage, 0, sizeof(SendPacket.szMessage));
	wsprintf(SendPacket.szMessage, GetServerMessage(SERVER_MESSAGE_SIEGE_CONQUER), m_szOwner, m_szDungeonName);
	
	SendPacket.wMsgLen			= WORD(strlen(SendPacket.szMessage) +1);
	SendPacket.dwMessageColor	= 0xFF21DBFF;
	SendPacket.byType			= 0;		

	g_pServerTable->BroadCast((char*)&SendPacket, SendPacket.GetPacketSize());	

	WSTC_REFRESHEVENT_DUNGEON_CHANGE_OWNER packet;
	packet.wDungeonID	= (WORD)m_dwID;
	packet.dwUserIndex	= m_dwOwnerIndex;	
	memset(packet.szOwner, 0 , sizeof(packet.szOwner));
	memset(packet.szGuildName, 0, sizeof(packet.szGuildName));
	__lstrcpyn(packet.szOwner, m_szOwner, MAX_CHARACTER_NAME_LENGTH);
	__lstrcpyn(packet.szGuildName, m_szManager, MAX_GUILD_REAL_NAME_LENGTH);
	SendToUsers((char*)&packet, packet.GetPacketSize());	
}


void DUNGEON_DATA_EX::SendRefreshOwnerGuild()
{
	WSTC_REFRESHEVENT_DUNGEON_CHANGE_OWNER_GUILD packet;
	packet.wDungeonID = (WORD)m_dwID;
	packet.dwGuildId   = m_dwOwnerGuildNum;
	memset(packet.szGuildName, 0, sizeof(packet.szGuildName));
	__lstrcpyn(packet.szGuildName, m_szManager, MAX_GUILD_REAL_NAME_LENGTH);
	SendToUsers((char*)&packet, packet.GetPacketSize());
	
	WSTDS_REFRESH_DUNGEON_OWNER_GUILD ServerPacket;
	ServerPacket.wDungeonID	= (WORD)m_dwID;
	ServerPacket.dwGuildID		= m_dwOwnerGuildNum;
	
	if (m_pServer)
		g_pNet->SendToServer(m_pServer->dwConnectionIndex, (char*)&ServerPacket, ServerPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	
}


BOOL DUNGEON_DATA_EX::LevelUpgrade()
{
	// ���� ���� ���׷��̵��ϱ�.
	if (IsLevelUpgradeTime())
	{
		m_wUpgradeLevel = (WORD)min(m_wUpgradeLevel+1, 15);
		
		m_dwRemainLevelStartTick = GetLevelUpTickCount();
		m_dwStartLevelStartTick = g_dwCurTick;

		SendRefreshLevelUp();
		
		// ������������ �������.
		if(m_pServer && m_pServer->dwServerStatus == SERVER_STATUS_CONNECTED)	
		{
			WSTS_DUNGEON_LEVELUP packet;
			packet.wDungeonID = (WORD)m_dwID;

			g_pNet->SendToServer(m_pServer->dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}	

		return TRUE;
	}

	return FALSE;
}


void DUNGEON_DATA_EX::SendRefreshProductionInfo(WORD wItemID, BYTE byProductionCount)
{
	// ����ǰ �ƾƵ� ����.
	m_wProduction = wItemID;
	m_byProductionCount = byProductionCount;
	
	WSTC_REFRESHEVENT_DUNGEON_CREATEPRODUCTION	packet;
	packet.wDungeonID = (WORD)m_dwID;
	packet.byProductionCount = byProductionCount;
	packet.wProduction = wItemID;
	SendToUsers((char*)&packet, packet.GetPacketSize());	
}


// ������ �ð��� ƽī��Ʈ�� ����ؼ� �����Ͷ�.
DWORD DUNGEON_DATA_EX::GetLevelUpTickCount()
{
	return m_dwStartLevelStartTick + GetUpgradeTime() * 10 * 60;
}


// ���� �����
DWORD DUNGEON_DATA_EX::GetEntrancePay()
{
	return m_dwEntrance;
}


void DUNGEON_DATA_EX::SendRefreshEntrance()
{
	WSTC_REFRESHEVENT_DUNGEON_ENTRANCE_EDIT packet;
	packet.wDungeonID = (WORD)m_dwID;
	packet.dwMoney = m_dwEntrance;
	SendToUsers((char*)&packet, packet.GetPacketSize());
}


void DUNGEON_DATA_EX::SetEntrance(DWORD dwEntrance)
{
	m_dwEntrance = dwEntrance;
	SendRefreshEntrance();
}


void DUNGEON_DATA_EX::SetAccEntrance(int nPlusMoney, BOOL bSend)
{
	bSend;

	if (nPlusMoney < 0)
	{
		// �ּҰ� ����
		if (m_dwAccEntrance < (DWORD)abs(nPlusMoney))
		{
			m_dwAccEntrance = 0;	
		}
		else
		{
			m_dwAccEntrance += nPlusMoney;	
		}

	}
	else
	{
		// �ִ밪 ����
		if (0xffffffff-m_dwAccEntrance > (DWORD)nPlusMoney)
			m_dwAccEntrance += nPlusMoney;
		else
			m_dwAccEntrance = 0xffffffff;
	}
	
	WSTC_REFRESHEVENT_DUNGEON_ACCENTRANCE_ACQUISITION packet;
	packet.wDungeonID = (WORD)m_dwID;
	packet.dwMoney = m_dwAccEntrance;
	SendToUsers((char*)&packet, packet.GetPacketSize());
}


void DUNGEON_DATA_EX::SetOwner(DWORD dwOwnerIndex, DWORD dwGuildID, char* pOwnerID, char* pOwnerName)
{
#ifdef	EVENT_CONQUERDUNGEON
	char szQuery[0XFF]={0,};
	wsprintf(szQuery, "CorumGameEventConquerDungeon '%s', '%s', '%s', '%s', %d, '%s', %c"
		, m_szOwnerID, m_szOwner, pOwnerID, pOwnerName, m_dwID, m_szDungeonName, g_pThis->GetServerSetCode()[0]);
	g_pDb->THExecuteSQL(szQuery, FALSE, 0, NULL, MEMBER_DB);
#endif

	m_dwOwnerIndex		= dwOwnerIndex;
	m_dwOwnerGuildNum	= dwGuildID;
	__lstrcpyn(m_szOwner, pOwnerName, MAX_CHARACTER_NAME_REAL_LENGTH);
	__lstrcpyn(m_szOwnerID, pOwnerID, MAX_CHARACTER_NAME_REAL_LENGTH);

	GUILD_TABLE* pGuild = g_pGuildTableHash->GetData(dwGuildID);
	if (pGuild)
		__lstrcpyn(m_szManager, pGuild->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
	else
		__lstrcpyn(m_szManager, "--", MAX_GUILD_REAL_NAME_LENGTH);
}


// ���� ������ �����Ѱ�?	
BOOL DUNGEON_DATA_EX::IsConquer()
{
	return (m_dwID >= 4000) && (m_dwID < 5000);
}


void DUNGEON_DATA_EX::SetOperationMode(BYTE bOperationMode)
{
	m_bOperationType = bOperationMode;
	WSTC_REFRESHEVENT_DUNGEON_OPERATIONMODE packet;
	packet.wDungeonID = (WORD)m_dwID;
	packet.byOperationMode = m_bOperationType;
	SendToUsers((char*)&packet, packet.GetPacketSize());
}


void DUNGEON_DATA_EX::SetAccExp(DWORD dwExp)
{
	m_dwAccExp = dwExp;
	SendRefreshAccExp();
}


void DUNGEON_DATA_EX::SendRefreshAccExp()
{
	WSTC_REFRESHEVENT_DUNGEON_ACCEXP_ACQUISITION packet;
	packet.wDungeonID = (WORD)m_dwID;
	packet.dwAccExp = m_dwAccExp;
	SendToUsers((char*)&packet, packet.GetPacketSize());
}


//------------------------------------------------------------------------------
// �̺�Ʈ ������ ���� ��ȯ�� ���� �������� ó��.
//------------------------------------------------------------------------------
BOOL DUNGEON_DATA_EX::EventChangeState(BYTE byState, DWORD dwCurrentTick)
{
	if(DUNGEON_TYPE_EVENT != GetDungeonType())	return FALSE;	
	
	m_bEventState = byState;

	if(dwCurrentTick)
	{
		m_dwEventLastActionTick = dwCurrentTick;
	}

	return TRUE;
}


//------------------------------------------------------------------------------
// �̺�Ʈ ������ ������ �Ѹ� ����. �ݰ� ���� ������ �ض�.
//------------------------------------------------------------------------------
BOOL DUNGEON_DATA_EX::DoNotExistEventDungeonEntered(DWORD dwCurrentTick, EVENT_DUNGEON_PROPERTY* pProperty)
{
	if(	EVENT_DUNGEON_FIRST_CLOSE == m_bEventState 
	&&	EVENT_DUNGEON_SURVIVAL == pProperty->byDungeonType)
	{
		if(m_iEventEntered <= 1)
		{
			EventChangeState(EVENT_DUNGEON_CLOSE, dwCurrentTick);
			SendEventInfo(NULL);
			ResetEventInfo(TRUE);

			return TRUE;
		}
	}

	if(m_iEventEntered <= 0)
	{
		EventChangeState(EVENT_DUNGEON_CLOSE, dwCurrentTick);
		SendEventInfo(NULL);
		ResetEventInfo(FALSE);

		return TRUE;
	}

	return FALSE;
}


//------------------------------------------------------------------------------
// �̺�Ʈ ������ ���� ��ȭ�� ���������� �����ش�.
//------------------------------------------------------------------------------
BOOL DUNGEON_DATA_EX::SendEventInfo(DWORD dwEventEndTick)
{
	if( IS_ABLE_NATION(NC_JAPAN) )//�Ϻ��� �̺�Ʈ ���� ����: 050119 hwoarang
	{
		return FALSE;
	}

	if(NULL == g_pServerTable)					return FALSE;
	if(DUNGEON_TYPE_EVENT != GetDungeonType())	return FALSE;
	
	if(EVENT_DUNGEON_OPEN == m_bEventState)
	{
		// �̺�Ʈ ���� ���� 
		WSTDS_EVENT_DUNGEON_OPEN_INFO SendPacket;
		memset(&SendPacket.szDungeonName, 0, MAX_DUNGEON_NAME_LENGTH);
		__lstrcpyn(SendPacket.szDungeonName, m_szDungeonName, MAX_DUNGEON_NAME_LENGTH);
		SendPacket.wDungeonID		= (WORD)m_dwID;
		SendPacket.byEventState		= m_bEventState;
		SendPacket.dwEventEndTick	= dwEventEndTick;

		g_pServerTable->BroadCast((char*)&SendPacket, SendPacket.GetPacketSize());
		return TRUE;
	}

	if(EVENT_DUNGEON_OPEN_TIME_UP == m_bEventState)
	{
		// �̺�Ʈ ���� �Ա� ���� 
		WSTDS_EVENT_DUNGEON_CLOSE_INFO SendPacket;
		memset(&SendPacket.szDungeonName, 0, MAX_DUNGEON_NAME_LENGTH);
		__lstrcpyn(SendPacket.szDungeonName, m_szDungeonName, MAX_DUNGEON_NAME_LENGTH);
		SendPacket.wDungeonID	= (WORD)m_dwID;
		SendPacket.byEventState	= m_bEventState;

		g_pServerTable->BroadCast((char*)&SendPacket, SendPacket.GetPacketSize());
		return TRUE;
	}
	
	if(EVENT_DUNGEON_CLOSE == m_bEventState)
	{
		// �̺�Ʈ ���� ���� 
		WSTDS_EVENT_DUNGEON_CLOSE_INFO SendPacket;
		memset(&SendPacket.szDungeonName, 0, MAX_DUNGEON_NAME_LENGTH);
		__lstrcpyn(SendPacket.szDungeonName, m_szDungeonName, MAX_DUNGEON_NAME_LENGTH);
		SendPacket.wDungeonID	= (WORD)m_dwID;
		SendPacket.byEventState	= m_bEventState;
		
		g_pServerTable->BroadCast((char*)&SendPacket, SendPacket.GetPacketSize());
		return TRUE;
	}
	
	return TRUE;
}


//------------------------------------------------------------------------------
//	�̺�Ʈ ������ ������ �ʱ�ȭ �Ѵ�.
//------------------------------------------------------------------------------
void DUNGEON_DATA_EX::ResetEventInfo(BOOL bTimeUp)
{	
	if(DUNGEON_TYPE_EVENT != GetDungeonType())		return;
	if(NULL == m_pServer)							return;

	m_iEventEntered = 0;

	WSTDS_EVENT_DUNGEON_INIT packet;
	packet.wDungeonID	= (WORD)m_dwID;	
	packet.bTimeUp		= bTimeUp;

	g_pNet->SendToServer(m_pServer->dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);		
}


//------------------------------------------------------------------------------
//	�������� 1�� �̺�Ʈ�� ����Ǿ����� ���������� �˸���. ������ �˸��� ������ �ִ� �����鿡�� �˸���.
//------------------------------------------------------------------------------
void DUNGEON_DATA_EX::SendEventInfoFirstClose()
{
	if(DUNGEON_TYPE_EVENT != GetDungeonType())		return; 
	if(NULL == m_pServer)							return;
	
	WSTDS_EVENT_DUNGEON_FIRST_TIMEOUT SendPacket;
	memset(&SendPacket.szDungeonName, 0, MAX_DUNGEON_NAME_LENGTH);	
	__lstrcpyn(SendPacket.szDungeonName, m_szDungeonName, MAX_DUNGEON_NAME_LENGTH);	
	SendPacket.wDungeonID = (WORD)m_dwID;
	
	g_pNet->SendToServer(m_pServer->dwConnectionIndex,(char*)&SendPacket,SendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);		
}


//------------------------------------------------------------------------------
//	�̺�Ʈ ���� �����ϱ� ���� �����鿡�� �˷��ش�.
//------------------------------------------------------------------------------
void DUNGEON_DATA_EX::SendEventInfoClose()
{
	if(DUNGEON_TYPE_EVENT != GetDungeonType())	return; 	
	if(NULL == m_pServer)						return;

	WSTDS_EVENT_DUNGEON_CLOSE SendPacket;
	SendPacket.wDungeonID = (WORD)m_dwID;
	
	g_pNet->SendToServer(m_pServer->dwConnectionIndex,(char*)&SendPacket,SendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);		
}