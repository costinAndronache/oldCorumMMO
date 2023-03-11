#include "StdAfx.h"
#include "RivalGuildWar.h"
#include "Dungeon.h"
#include "DungeonLayer.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "DUNGEON_DATA_EX.h"
#include "Map.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "MonsterTable.h"
#include "Monster.h"
#include "DungeonTable.h"
#include "OwnServer.h"
#include "recvmsg.h"

//����� : hwoarang 050205

void RECV_CMD_TEAM_MATCH(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{//����� ���� �޴� ����
	WSTDS_TEAM_MATCH *pPacket = (WSTDS_TEAM_MATCH*)pMsg;

	CBaseTeamMatch *pTeamMatch = NULL;
	CDungeonLayer *pLayer	= NULL;
	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo( pPacket->wDungeonID);		
	if(pDungeon)
	{
		pLayer = pDungeon->GetDungeonLayer( pPacket->byLayerIndex );
		if(	pLayer
		&&	pLayer->m_pTeamMatch)
		{
			pTeamMatch = pLayer->m_pTeamMatch;
		}
	}

	switch(pPacket->wType)
	{
	case CMD_WSTDS_PREPARE_MATCH:
		{
			WSTDS_PREPARE_TEAM_MATCH &rPTM = pPacket->Body.PTM;
			
			if(pLayer)
			{//���̾� �ִ�.
				if( !pLayer->IsTeamMatchRun() )
				{//����ġ ���� ����? //���� �ߴ� �����̱� ������ ������ ���� ���� �Դٰ� ���弭���� 1���̱� ������ ����ȭ�� �Ű� �� �ʿ䵵 ����.
					pLayer->PrepareTeamMatch(rPTM.match_type , rPTM.match_level, rPTM.player_count,  rPTM.team_count, rPTM.match_time, rPTM.dwExFlag?true:false );
				}
			} // if(pDungeon)
		}break;
	case CMD_WSTDS_REG_PLAYER:
		{
			WSTDS_REG_PLAYER &rRP = pPacket->Body.RP;
			
			if(pTeamMatch)
			{
				Log(LOG_NORMAL, "! ----- RegTeamMatchPlayer %s", rRP.szPlayerName);
				pTeamMatch->RegTeamMatchPlayer(rRP.szPlayerName, rRP.team_index , rRP.dwExFlag);
			}
		}break; // if(pDungeon)
	case CMD_WSTDS_UN_REG_PLAYER:
		{
			WSTDS_REG_PLAYER &rRP = pPacket->Body.RP;
			
			if(pTeamMatch)
			{
				Log(LOG_NORMAL, "! ----- UnRegTeamMatchPlayer %s", rRP.szPlayerName);
				pTeamMatch->UnRegTeamMatchPlayer(rRP.szPlayerName );

			}
		}break;
	case CMD_WSTDS_END_MATCH:
		{
			if(pTeamMatch)
			{//���̾� �ִ�.
				pTeamMatch->Terminate();
			} // if(pDungeon)
		}break;
	}    
	return;
}