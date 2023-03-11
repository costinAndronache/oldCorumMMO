#include "stdafx.h"
#include "ServerFunction.h"
#include "ServerTable.h"
#include "WorldUserTable.h"
#include "recvmsg.h"
#include "GuildHash.h"
#include "PartyHash.h"
#include "MessengerHash.h"
#include "AcceptTable.h"
#include "DungeonTable.h"
#include "EventDungeonManager.h"
#include "Map.h"
#include "ConnectionDistributionTable.h"
#include <afxdisp.h>
#include "GuildHash.h"
#include "Guild.h"
#include "GlobalDefine.h"
#include "SchoolGuildDungeon.h"
#include "GuildMatcher.h"
#include "GuildMatchHandler.h"
#include "StadiumMgr.h"

#include "../Commonserver/RivalGuildWarPacket.h"

#include "PartyMatcher.h"

extern void Recv_CTWS_TM_INSERT(CWorldUser* pUser, char* pMsg, DWORD dwLength);
extern void Recv_CTWS_TM_REMOVE(CWorldUser* pUser);

const int MAX_RANDOM_TEAM_COUNT = 2;

CGuildMatcher	g_GuildMatcher;
CStadiumMgr< CStadium >	 g_StadiumMgr;

void __stdcall WorldMatchProc(DWORD dwVal)
{
	g_GuildMatcher.HandleEvent(); // ����� ���Ͼ�ϱ� �켱 �ּ�
	g_PartyMatcher.Process(20); // ��Ƽ �ڵ� ��Ī : �ִ��� 2005.2.15

	static SYSTEMTIME PrevTime;

	SYSTEMTIME NowTime;
	GetLocalTime(&NowTime);

	//9�� 10�� 11�� �� �޼��� ������ ( ����� ���� �϶�� ) 
	if( (PrevTime.wHour == 20 || PrevTime.wHour == 21 || PrevTime.wHour == 22 ) //9 ~ 11 �� �� ��� �϶��
	&&	(NowTime.wHour == PrevTime.wHour+1 ) )	
	{	
		GUILD_WAR_BBS	packet;
		g_pUserTable->BroadCast((char*)&packet, packet.GetPacketSize() );
	}
	PrevTime = NowTime;
}


CWorldUser* GetCanMatchGuildLeader(const DWORD dwGuildID, const BYTE bLimitRank)//: 050202 hwoarang �渶 �ܿ� 2��ޱ��� �޴´�
{
	LP_GUILD_TABLE	lpGuildTable	= g_pGuildTableHash->GetData( dwGuildID );		// ��û�� ���

	ListNode<CWorldUser>* pGuildUserPos = lpGuildTable->pGuildUserHash->GetHead();
	
	CWorldUser*	pMasterUser	= NULL;

	while ( pGuildUserPos ) 
	{
		pMasterUser = pGuildUserPos->pData;

		if ( pMasterUser ) 
		{
			DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pMasterUser->m_dwDungeonID);//������ �ִ³��̾����

			if( pMasterUser->m_byRank && pMasterUser->m_byRank <= bLimitRank ) 
			{
				if( pDungeon &&  !g_StadiumMgr.GetUserStadium( pMasterUser->m_szCharacterName ) )
				{
					Log ( LOG_NORMAL, "IsThereGuildMaster() Pointer : %p", pMasterUser );
					return pMasterUser;
				}
			}
		}
		else
		{
			return NULL;
		}
		pGuildUserPos = pGuildUserPos->pNext;
	}

	return NULL;
}

void BroadCastGuildUser(const DWORD dwGuildID, char* pPacket, const DWORD dwLen)//: 050202 hwoarang �渶 �ܿ� 2��ޱ��� �޴´�
{
	LP_GUILD_TABLE	lpGuildTable	= g_pGuildTableHash->GetData( dwGuildID );		// ��û�� ���

	ListNode<CWorldUser>* pGuildUserPos = lpGuildTable->pGuildUserHash->GetHead();
	
	CWorldUser*	pGuildUser	= NULL;

	while ( pGuildUserPos ) 
	{
		pGuildUser = pGuildUserPos->pData;

		if( pGuildUser ) 
		{
			DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo( (WORD)pGuildUser->m_dwDungeonID );//������ �ִ³��̾����

			if( pDungeon )//������� �ƴϾ�� �Ѵ�.
			{
				g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, pPacket, dwLen, FLAG_SEND_NOT_ENCRYPTION);
			}
		}
		else
		{
			return;
		}
		pGuildUserPos = pGuildUserPos->pNext;
	}
	return;
}

FLAG_TYPE GetRandomMatchType()
{
	const FLAG_TYPE aMatchType[] = 
		{
			M_T_DEAH_MATCH,	
			M_T_KING_OF_THE_HILL,	
			M_T_CAPTURE_THE_FLAG,	
			M_T_LEADER_KILL, 
			M_T_KILLING_FIELD
		};

	const size_t array_count = sizeof(aMatchType)/sizeof(FLAG_TYPE);

	const int iIndex = rand()%array_count;//0 �Ǹ� ���

	return aMatchType[iIndex];
}

COUNT_TYPE GetRandomBasePlayerCount()
{
	const COUNT_TYPE base_array_count = sizeof( g_aBasePlayerCount )/sizeof(COUNT_TYPE);//0 �Ǹ� ���

	const int iIndex = rand()%base_array_count;

	return g_aMatchTeamCount[iIndex];
}


template< typename T_IN_TYPE >
FLAG_TYPE ConvertMatchType( const T_IN_TYPE InType )
{
	switch(InType)
	{
	case M_T_DEAH_MATCH:
	case M_T_KING_OF_THE_HILL:
	case M_T_CAPTURE_THE_FLAG:
	case M_T_LEADER_KILL:
	case M_T_KILLING_FIELD:
		{
			return static_cast<FLAG_TYPE>(InType);
		}break;

	case M_T_ALL:
		{
			return GetRandomMatchType();
		}break;
	} // switch(InType)

	
	return M_T_NONE;
}

FLAG_TYPE GetTeamType(const CWorldUser* pUser)
{
	if(!pUser)
	{
		return T_T_NONE;
	}

	if(0 != pUser->m_dwGuildId )
	{
		return T_T_GUILD;
	}

	return T_T_RANDOM;
}

FLAG_TYPE GetMatchLevel(const CWorldUser* pUser)
{
	if(!pUser)
	{
		return M_L_NONE;
	}

	if( pUser->m_dwLevel > 60 )
	{
		return M_L_HIGH;
	}

	return M_L_LOW;
}

//COUNT_TYPE GetTeamIndex(const CWorldUser* pUser)
//{
//	if( !pUser->m_dwGuildId )
//	{
//		return rand()%MAX_RANDOM_TEAM_INDEX;//��� ���̵� ���� �ֵ��� �����ϰ� �� �ε����� ����
//	}
//	return pUser->m_dwGuildId;
//}

void MakeGMC(CWorldUser* pUser, const FLAG_TYPE match_type, const FLAG_TYPE team_type = T_T_NONE, const bool bDoInsert = true)
{
	const FLAG_TYPE	eMT = ConvertMatchType(match_type);//ALLŸ���� Ÿ���ɰ�(���۸� ���� �ʴ´ٴ� �ǹ�
	FLAG_TYPE	eML	= GetMatchLevel(pUser);
	
	FLAG_TYPE	eTT	= GetTeamType(pUser);
	
	if(	T_T_S_GUILD == team_type 
	&& T_T_GUILD	== eTT )//��尡 �ְ�
	{
		eTT = T_T_S_GUILD;
		eML = M_L_HIGH;//������ ������� ������ ��ġ ������ High�̴�
	}
	
	if( T_T_RANDOM == team_type )
	{
		eTT = T_T_RANDOM;
	}

	if(	eMT == M_T_NONE
	||	eTT == T_T_NONE )
	{
		Log(LOG_ALL, "@Recv_CTWS_TM_INSERT Illigal Type!");
		return;
	}

	G_MC_DEFAULT GMD;

	CMatchCondition *pMC = const_cast<CMatchCondition*>(&GMD.First());
	CTeamInfo *pTI = const_cast<CTeamInfo*>(&GMD.Second());
	
	pMC->MatchType(eMT);
	pMC->MatchLevel(eML);//���� �̱�.

	pTI->TeamType(eTT);//��Ÿ�� ����
//	pTI->TeamIndex( GetTeamIndex(pUser) );
	
//	pTI->TeamType(T_T_GUILD);//��Ÿ�� ����

	const int randomteamindex = rand()%MAX_RANDOM_TEAM_COUNT;
	if( eTT == T_T_RANDOM )
	{
		pTI->TeamIndex( randomteamindex );
	}
	else
	{
		pTI->TeamIndex( pUser->m_dwGuildId );
	}
	
	pUser->m_GMC = GMD;//������ ����

	if( bDoInsert)
	{
		CPlayerInfo Player( pUser );
		g_GuildMatcher.Insert( GMD, Player );
		Log(LOG_ALL, "@Recv_CTWS_TM_INSERT name[%s] match_type[%d], match_level[%d], team_type[%d], team_index[%d]", pUser->m_szCharacterName, eMT, eML, eTT, pTI->TeamIndex() );
	}
}

void Recv_CMD_TEAM_MATCH(CWorldUser* pCaster, char* pMsg, DWORD dwLength)
{//������ ���� �޾Ҵ�.
	TEAM_MATCH_PACKET* pPacket = reinterpret_cast<TEAM_MATCH_PACKET*>(pMsg);
	
	const WORD wType = pPacket->wType;
	switch(wType)
	{
	case CTWS_TM_INSERT:
		{
			Recv_CTWS_TM_INSERT(pCaster, pMsg, dwLength);
		}
		break;
	case CTWS_TM_REMOVE:
		{
			Recv_CTWS_TM_REMOVE(pCaster);
		}
		break;
	case CTWS_TM_LADDER_LIST:
		{
			// ��� ���� ������ Ŭ���̾�Ʈ�� �����Ѵ�.
			TCHAR	szQuery[0xFF];
			::wsprintf( szQuery, _T("EXEC SP_GET_GUILD_LADDER_INFO %u, %u"), pPacket->Body.m_GuildLadderInfoPage.m_uCurPage, pCaster->m_dwGuildId );
			g_pDb->THOpenRecord( szQuery, QUERY_TYPE_LADDER_INFO, reinterpret_cast< VOID* >(pCaster->m_dwConnectionIndex), 20, static_cast< BYTE>(GAME_DB) );
		}
		break;
	case CTWS_TM_GUILD_WAR_LIST:
		{//����� ����Ʈ ����
			size_t cur_page = pPacket->Body.PageNum.iPageNumber;
//
			TEAM_MATCH_PACKET packet;
			packet.wType = CTWS_TM_GUILD_WAR_LIST;
			GUILD_WAR_DATA_LIST &rGWDL = packet.Body.GuildWarList;
			g_StadiumMgr.MakeGuildWarList(rGWDL, cur_page);
			g_pNet->SendToUser(pCaster->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}break;
	case CTWS_TM_GUILD_WAR_REQUEST:
		{
			pCaster->m_bGuildWarRequestApply = FALSE;
			CWorldUser* pTarget = g_pUserTable->GetUserInfoByName(pPacket->Body.GuildWarRequest.szTargetName);

			if(!pTarget)
			{
				break;
			}

			if( !pCaster->m_dwGuildId 
			||	!pTarget->m_dwGuildId )
			{//��� ID �� ���ų�
				break;
			}

			if( pCaster->m_dwGuildId == pTarget->m_dwGuildId )
			{//���� �����
				break;
			}

			switch(pPacket->Body.GuildWarRequest.Sequence)
			{
			case GWRS_NONE:
				{

					CWorldUser* pGuildMaster = GetCanMatchGuildLeader( pTarget->m_dwGuildId, 2 );
					
					if( pTarget->m_byRank &&  pTarget->m_byRank <= 2 )
					{
						pGuildMaster = pTarget;
					}

					if(pGuildMaster)//�渶 ã�Ҵ�.
					{
						TEAM_MATCH_PACKET SendPacket;
						SendPacket.wType = CTWS_TM_GUILD_WAR_REQUEST;
						SendPacket.Body.GuildWarRequest.Sequence = GWRS_NONE;
						SendPacket.Body.GuildWarRequest.MatchType = M_T_NONE;
						SendPacket.Body.GuildWarRequest.MatchTime = GWT_TEN;
						SendPacket.Body.GuildWarRequest.MatchNumber = GWN_SIX;
						
						::strcpy( SendPacket.Body.GuildWarRequest.szTargetName, pGuildMaster->m_szCharacterName );

						//SendPacket.Body.GuildWarRequest.LadderData	// ���� ����� ���� ����Ÿ�� �ִ� �κ�
						ZeroMemory(&SendPacket.Body.GuildWarRequest.LadderData, sizeof(SendPacket.Body.GuildWarRequest.LadderData));
						g_pNet->SendToUser(pCaster->m_dwConnectionIndex, (char*)&SendPacket, SendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						::Log(LOG_ALL, "GWRS_NONE GuildMaster GuildID[%d], MasterName[%s]", pTarget->m_dwGuildId, pGuildMaster->m_szCharacterName);
					}
					else
					{
						TEAM_MATCH_PACKET SendPacket;
						SendPacket.wType = CTWS_TM_GUILD_WAR_REQUEST;
						SendPacket.Body.GuildWarRequest.Sequence = GWRS_REJECT;
						SendPacket.Body.GuildWarRequest.MatchType = M_T_NONE;//�ӽ÷� ����.. �渶�� �ƴϾ ����Ʈ�ɶ�
						SendPacket.Body.GuildWarRequest.MatchTime = GWT_TEN;
						SendPacket.Body.GuildWarRequest.MatchNumber = GWN_SIX;
						//SendPacket.Body.GuildWarRequest.LadderData	// ���� ����� ���� ����Ÿ�� �ִ� �κ�
						ZeroMemory(&SendPacket.Body.GuildWarRequest.LadderData, sizeof(SendPacket.Body.GuildWarRequest.LadderData));
						g_pNet->SendToUser(pCaster->m_dwConnectionIndex, (char*)&SendPacket, SendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);


						::Log(LOG_ALL, "GWRS_NONE Can't Find GuildMaster GuildID[%d]", pTarget->m_dwGuildId);
					}
				}
				break;
			case GWRS_REQUEST:
			case GWRS_REREQUEST:
				{
					if( (pCaster->m_byRank == 1 || pCaster->m_byRank == 2)
					&&	(pTarget->m_byRank == 1 || pTarget->m_byRank == 2) )
					{
						memcpy(&pCaster->m_RequestGuildWarRequest, &pPacket->Body.GuildWarRequest, sizeof(pPacket->Body.GuildWarRequest));
						TEAM_MATCH_PACKET SendPacket;
						SendPacket.wType = CTWS_TM_GUILD_WAR_REQUEST;
						memcpy(&SendPacket.Body.GuildWarRequest, &pPacket->Body.GuildWarRequest, sizeof(pPacket->Body.GuildWarRequest));
						//SendPacket.Body.GuildWarRequest.LadderData //�ڱ� ����� ���� ����Ÿ �ִ� �κ�
						ZeroMemory(&SendPacket.Body.GuildWarRequest.LadderData, sizeof(SendPacket.Body.GuildWarRequest.LadderData));
						memcpy(&SendPacket.Body.GuildWarRequest.LadderData, &pPacket->Body.GuildWarRequest.LadderData, sizeof(pPacket->Body.GuildWarRequest.LadderData));
						g_pNet->SendToUser(pTarget->m_dwConnectionIndex, (char*)&SendPacket, SendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					}
					else
					{
						Log(LOG_ALL, "GWRS_REQUEST Incorrect GuildRank  Caster[Rank%d], Target[Rank%d]", pCaster->m_byRank,  pTarget->m_byRank);
					}

				}
				break;
			case GWRS_APPLY:
				{
					if( (pCaster->m_byRank == 1 || pCaster->m_byRank == 2)
					&&	(pTarget->m_byRank == 1 || pTarget->m_byRank == 2) 
					&&	!g_StadiumMgr.GetUserStadium( pCaster->m_szCharacterName ) //������� �ִ� ���̸� �ȵȴ� .
					&&	!g_StadiumMgr.GetUserStadium( pTarget->m_szCharacterName ) 
					)
					{
						GUILD_WAR_REQUEST &rGWR = pPacket->Body.GuildWarRequest;

						const FLAG_TYPE match_type	= rGWR.MatchType;
						const FLAG_TYPE match_level = M_L_NONE;
						const COUNT_TYPE max_player = rGWR.MatchNumber;
						const GUILD_WAR_TIME match_time = rGWR.MatchTime;

						size_t count = g_StadiumMgr.EmptyStadiumCount( match_type );
						if(	(0 < count )
						&&	(0 != pCaster->m_dwGuildId)
						&&	(0 != pTarget->m_dwGuildId)
						&&	(pCaster->m_dwGuildId != pTarget->m_dwGuildId) )
						{
							CStadium *pStadium = g_StadiumMgr.PreparedStadium( match_type, match_level, MIN_MATCH_TEAM, max_player, match_time, false);

							DUNGEON_MOVE_INFO DMI;

							DMI.wDungeonID = pStadium->wDungeonID;
							DMI.byLayerIndex = pStadium->byLayerIndex;
						
							MakeGMC(pCaster, match_type, T_T_S_GUILD, true);
							MakeGMC(pTarget, match_type, T_T_S_GUILD, true);
						
							
							size_t team_index_pos = 0;//����ڴ� 1�� ����
							
							vector< size_t > vec_team_index;
							vec_team_index.clear();
							vec_team_index.push_back(1);//�� �ε���. ������ 4�� �ۿ� �����ϱ�. �̷��� �س��´�.
							vec_team_index.push_back(2);
							vec_team_index.push_back(3);
							vec_team_index.push_back(4);
							random_shuffle( vec_team_index.begin(), vec_team_index.end() );
							
							{
							const size_t result_index = vec_team_index.at(team_index_pos);
							
							DMI.wX = static_cast< WORD >(pStadium->aPos[result_index].x);
							DMI.wY = static_cast< WORD >(pStadium->aPos[result_index].y);

							STADIUM_PLAYER S_P(match_type, DMI, result_index, G_W_F_MASTER, false);
							pStadium->InsertUser( pCaster, S_P );//����忡 ���
							MoveToStadium( pCaster, DMI, result_index, G_W_F_MASTER);//��� �����Ͱ� �ȴ�

							::Log(LOG_ALL, "GWRS_APPLY Name[%s] Type[%d] TeamIndex[%d] MovePos = %d %d", pCaster->m_szCharacterName, match_type, result_index, pStadium->aPos[result_index].x, pStadium->aPos[result_index].y);
							}
							++team_index_pos;
							{
							const size_t result_index = vec_team_index.at(team_index_pos);
							DMI.wX = static_cast< WORD >(pStadium->aPos[result_index].x);
							DMI.wY = static_cast< WORD >(pStadium->aPos[result_index].y);
							
							STADIUM_PLAYER S_P(match_type, DMI, result_index, G_W_F_MASTER, false);
							pStadium->InsertUser( pTarget, S_P );//����忡 ���
							MoveToStadium( pTarget, DMI, result_index, G_W_F_MASTER);//��� �����Ͱ� �ȴ�
							
							::Log(LOG_ALL, "GWRS_APPLY Name[%s] Type[%d] TeamIndex[%d] MovePos = %d %d", pTarget->m_szCharacterName, match_type, result_index, pStadium->aPos[result_index].x, pStadium->aPos[result_index].y);
							}
						}
					}
				}break;
			case GWRS_REJECT:
				{
					TEAM_MATCH_PACKET SendPacket;
					SendPacket.wType = CTWS_TM_GUILD_WAR_REQUEST;
					SendPacket.Body.GuildWarRequest.Sequence = GWRS_REJECT;
					SendPacket.Body.GuildWarRequest.MatchType = M_T_ALL;//�ӽ÷� ����.. �׳� �����϶�
					SendPacket.Body.GuildWarRequest.MatchTime = GWT_TEN;
					SendPacket.Body.GuildWarRequest.MatchNumber = GWN_SIX;
					//SendPacket.Body.GuildWarRequest.LadderData	// ���� ����� ���� ����Ÿ�� �ִ� �κ�
					ZeroMemory(&SendPacket.Body.GuildWarRequest.LadderData, sizeof(SendPacket.Body.GuildWarRequest.LadderData));
					g_pNet->SendToUser(pTarget->m_dwConnectionIndex, (char*)&SendPacket, SendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
				break;
			}

			if( pCaster->m_bGuildWarRequestApply && pTarget->m_bGuildWarRequestApply )
			{
				TEAM_MATCH_PACKET SendPacket;
				SendPacket.wType = CTWS_TM_GUILD_WAR_REQUEST;
				SendPacket.Body.GuildWarRequest.Sequence = GWRS_END;
				g_pNet->SendToUser(pCaster->m_dwConnectionIndex, (char*)&SendPacket, SendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				g_pNet->SendToUser(pTarget->m_dwConnectionIndex, (char*)&SendPacket, SendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				//����� ��û
				//pCaster->m_RequestGuildWarRequest �� �ش� ����Ÿ�� �������
			}
		}
		break;
	case CTWS_TM_GUILD_WAR_OBSERVE:
		{	
//			int i = 0;
			GUILD_WAR_OBSERVE &rGWO = pPacket->Body.ObserveIndex;
			//�������
			
			CStadium *pStadium = g_StadiumMgr.GetStadium( rGWO.match_type, rGWO.wDungeonID, rGWO.byLayerIndex );
			if( pStadium )
			{
				if( TMS_RUN == pStadium->byState )
				{
					DUNGEON_MOVE_INFO DMI;
					DMI.wDungeonID		= rGWO.wDungeonID;
					DMI.byLayerIndex	= rGWO.byLayerIndex;
					DMI.wX				= static_cast< WORD >(pStadium->aPos[0].x);
					DMI.wY				= static_cast< WORD >(pStadium->aPos[0].y);

//					STADIUM_PLAYER S_P(rGWO.match_type, DMI, 0, G_W_F_OBSERVER, false);
//					pStadium->InsertUser( pCaster, S_P );//����忡 ���
					//�������� ����忡 ��� �ϸ� �ȵǴµ�.
					MoveToStadium( pCaster, DMI, 0, G_W_F_OBSERVER, false);
				}				
			} // if( pStadium )
			else
			{
				::Log(LOG_ALL, "CTWS_TM_GUILD_WAR_OBSERVE MatchType[%d] DungeonID[%d] LayerIndex[%d]", rGWO.match_type, rGWO.wDungeonID, rGWO.byLayerIndex);
			}
		}break;
	case CTWS_TM_GUILD_WAR_OBSERVE_CANCEL:
		{
			STADIUM_PLAYER *pSP = g_StadiumMgr.GetUserStadium( pCaster->m_szCharacterName );
			if(pSP)
			{
				MoveToPrevDungeon(pCaster, pSP->DMI.wDungeonID, false);
			}
		}break;
	}
}

// ������
// ������ ���ÿ� ���� ������ ��⿡ ���ִ�.
void CmdGuildMatchBeginSetting(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{// T_T_S_GUILD �� ���
	GUILD_MATCHBEGIN_SETTING*	pPacket = reinterpret_cast< GUILD_MATCHBEGIN_SETTING* >(pMsg);

	GUILD_MATCHBEGIN_SETTING send_packet;

	switch( pPacket->eType )
	{
	case GMS_TYPE_REQUEST_MEMBER_LIST:
		{	// �������Ʈ ������
			GUILD_MATCHBEGIN_MEMBER_LIST &rGMML = send_packet.Body.m_MemberList;
			ZeroMemory( &rGMML, sizeof(rGMML) );
			
			send_packet.eType = GMS_TYPE_MEMBER_LIST;

			CGuildMatchHandler *pHandler = g_GuildMatcher.GetSTypeHandler( pUser->m_GMC );

			if(pHandler)
			{
				const size_t total_size = pHandler->List().size();
				if( total_size )
				{//������ ������ �ִ´�� ������.
					CGuildMatchHandler::ELEMENT_LIST &rList = pHandler->List();
					CGuildMatchHandler::ELEMENT_LIST::iterator itor =  rList.begin();
					
					size_t temp_count = 0;
					while ( rList.end() != itor)
					{
						STADIUM_PLAYER *pSP = g_StadiumMgr.GetUserStadium( (*itor).Name() );
						rGMML.m_aMemberList[temp_count].byCalled = ((pSP)?1:0);

						rGMML.m_aMemberList[temp_count].byClass = (*itor).Class();
						strcpy( rGMML.m_aMemberList[temp_count].szName, (*itor).Name() );
						rGMML.m_aMemberList[temp_count].wLevel = static_cast< WORD >((*itor).LV());

						++temp_count;
						++itor;

						if(	MAX_MEMBER_LIST == temp_count
						||	rList.end() == itor)//�������̸� ������
						{
							if( rList.end() == itor )
							{
								rGMML.m_wIsEnd = true;
							}
							else
							{
								rGMML.m_wIsEnd = false;
							}

							rGMML.m_wListSize = static_cast< WORD >(temp_count);

							g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&send_packet, send_packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
							temp_count = 0;//�ʱ�ȭ
						}
					}
				} // if( total_size )
				else
				{//�ο��� ����
					rGMML.m_wIsEnd = true;
					rGMML.m_wListSize = 0;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&send_packet, send_packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
			}
		}break;
	case GMS_TYPE_REQUEST_LADDER_INFO:
		{
		// GUILD_MATCHBEGIN_LADDER_INFO	m_LadderInfo;
		// ���� ���� ������.
			CStadium *pStadium= g_StadiumMgr.GetStadium( pUser );
			if(pStadium)
			{
				pStadium->SendLadderInfoToUser( pUser );
			}
		}break;
	case GMS_TYPE_REQUEST_USER_CALL:
		// GUILD_MATCHBEGIN_USER_CALL		m_UserCall;
		// ���� ��ȯ����
		{
			char *szName = pPacket->Body.m_UserCall.szName;
			CWorldUser* pTargetUser = g_pUserTable->GetUserInfoByName( szName );

			CGuildMatchHandler *pHandler = g_GuildMatcher.GetSTypeHandler( pUser->m_GMC );
			if(pHandler)
			{
				CPlayerInfo Player(pTargetUser);
				CPlayerInfo *pPlayer = pHandler->List().Find(Player);
				if(pPlayer)//�ڵ鷯�� �ִ� ����
				{
					STADIUM_PLAYER *pSP =  g_StadiumMgr.GetUserStadium( pUser->m_szCharacterName );//�� �ѻ���� ���� ��Ÿ���
					
					STADIUM_PLAYER *pTargetSP =  g_StadiumMgr.GetUserStadium( pTargetUser->m_szCharacterName );//��ȯ ���� ����� ���� ��Ÿ���

					if(pSP && !pTargetSP)//��ȯ�ڴ� ������� �־�� �ϰ�, ��ȯ �� ����� ����忡 �־�� �ȵȴ�.
					{
						CStadium *pStadium = g_StadiumMgr.GetStadium( pUser->m_GMC.First().MatchType(), pSP->DMI.wDungeonID, pSP->DMI.byLayerIndex);
						
						if( pStadium )//������� �޾Ƽ�
						{
							STADIUM_PLAYER S_P( pStadium->match_type, pSP->DMI, pSP->team_index, G_W_F_PLAYER, false);
							pStadium->InsertUser( pTargetUser, S_P );//����忡 ���
							MoveToStadium(pTargetUser, pSP->DMI, pSP->team_index, G_W_F_PLAYER, false);//call �ϴ� �ֵ��� ������ ����
						}
					}
				}
			}
		}break;
	case GMS_TYPE_REQUEST_USER_KICK:
		// GUILD_MATCHBEGIN_USER_CALL		m_UserCall;
		// ���� ű����
		{ //Call �� �ֵ��� Master�� ����� �������� Stadium ���� Remove ���� �ʴ´�! 
			char *szName = pPacket->Body.m_UserCall.szName;
			CWorldUser* pTargetUser = g_pUserTable->GetUserInfoByName( szName );
			
			CGuildMatchHandler *pHandler = g_GuildMatcher.GetSTypeHandler( pUser->m_GMC );
			if(pHandler)
			{
				CPlayerInfo Player(pTargetUser);
				CPlayerInfo *pPlayer = pHandler->List().Find(Player);
				if(pPlayer)
				{
					STADIUM_PLAYER *pSP =  g_StadiumMgr.GetUserStadium( pUser->m_szCharacterName );
					CStadium *pStadium = g_StadiumMgr.GetStadium( pUser->m_GMC.First().MatchType(),pSP->DMI.wDungeonID, pSP->DMI.byLayerIndex );
					if(pStadium)
					{
						pStadium->RemoveUser(pTargetUser);//call �ϴ� �ֵ��� ������ ����
						MoveToPrevDungeon( pTargetUser, pStadium->wDungeonID, false);
					}
				}
			}
		}break;
	case GMS_TYPE_REQUEST_SET_READY:
		{// ���� �ϱ�.
		}break;
	case GMS_TYPE_REQUEST_RESET_READY:
		{// ���� Ǯ��.
		}break;
	case GMS_TYPE_REQUEST_CANCEL:
		{// ��� ���.
			STADIUM_PLAYER *pPlayer = g_StadiumMgr.GetUserStadium( pUser->m_szCharacterName );
			if(pPlayer)
			{
				g_StadiumMgr.SendEndTeamMatch(pPlayer->DMI.wDungeonID,pPlayer->DMI.byLayerIndex);
				g_StadiumMgr.LogGuildWar( g_StadiumMgr.GetStadium( pUser ) , false);
			}
		}break;
	case GMS_TYPE_REQUEST_INVITATION:
		{	// �۾��ʿ�
			GUILD_WAR_INVITE_PACKET packet;
			BroadCastGuildUser(pUser->m_dwGuildId, (char*)&packet, packet.GetPacketSize() );
		}break;

	} // switch( pPacket->eType )
}

void CmdGuildMatchStatusRequest(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	GUILD_MATCH_STATUS_REQUEST *pPacket = reinterpret_cast< GUILD_MATCH_STATUS_REQUEST* >(pMsg);

	CStadium *pStadium = NULL;
		 
	const FLAG_TYPE ret = pPacket->dwGuildWarFlag & G_W_F_CMP_BASE;
	switch(ret) 
	{
	case G_W_F_OBSERVER:
		{
			pStadium = g_StadiumMgr.GetStadium( pPacket->match_type, pPacket->wDungeonID, pPacket->byLayerIndex );
		}break;
	case G_W_F_PLAYER:
	case G_W_F_MASTER:
		{
			pStadium = g_StadiumMgr.GetStadium( pUser );
		}break;
	case G_W_F_NONE:
	default:
		{
			::Log(LOG_ALL, "CmdGuildMatchStatusRequest Incorrect GuildWarFlag[%d]",ret);
			return;
		}break;
	} // switch(pPacket->dwGuildWarFlag  & G_W_F_CMP_BASE)

	if( pStadium )
	{
		pStadium->SendTeamInfoToUser(pUser);
	}
}

void CmdGuildMatchCancelObserver(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{//���� ���̵�

	if(! g_StadiumMgr.GetUserStadium( pUser->m_szCharacterName ) )
	{
		GUILD_MATCH_CANCEL_OBSERVER *pPacket = reinterpret_cast< GUILD_MATCH_CANCEL_OBSERVER* >(pMsg);

		if( DUNGEON_TYPE_MATCH_STATIDUM  == DungeonIDtoType(pPacket->wDungeonID))
		{
			MoveToPrevDungeon( pUser, pPacket->wDungeonID, false);
		}
		else
		{
			::Log(LOG_ALL, "!! CmdGuildMatchCancelObserver There is Not Stadium DungeonID[%d]", pPacket->wDungeonID);
		}
	} // if(! g_StadiumMgr.GetUserStadium( pUser->m_szCharacterName ) )
	else
	{
		::Log(LOG_ALL, "!! CmdGuildMatchCancelObserver User is Player");
	}
}

bool CheckHaveTeamMatch(CWorldUser* pUser)
{
	STADIUM_PLAYER *pSP = g_StadiumMgr.GetUserStadium( pUser->m_szCharacterName );
	if( pSP )//�̹� ���� �ϴ����̾�����
	{// ���̵� 
		Log(LOG_ALL, "! CheckHaveTeamMatch User Have Game name[%s]", pUser);
		CStadium *pStadium = g_StadiumMgr.GetStadium( pUser );
		if(!pStadium->bIsAutoMatch)
		{// ������������ ������ ���� �ڵ� ��� ���ش�.
			if( pUser->IsHaveGMC() )//���� ��� �Ǿ� �ִٸ� �����
			{
				Recv_CTWS_TM_REMOVE(pUser);
			}
			MakeGMC(pUser, pSP->match_type, T_T_S_GUILD);//GMC�� �����
		}

		MoveToStadium( pUser, pSP->DMI, pSP->team_index );
		return true;
	} // if( pSP )//�̹� ���� �ϴ����̾�����
	return false;
}

void Recv_CTWS_TM_INSERT(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	TEAM_MATCH_PACKET* pPacket = reinterpret_cast<TEAM_MATCH_PACKET*>(pMsg);
	TEAM_MATCH_CONDITION &TMC = pPacket->Body.Condtion;

	if( DUNGEON_TYPE_MATCH_STATIDUM == DungeonIDtoType(pUser->m_dwDungeonID) )
	{//����� �ʿ� ������ Insert ����� ���� �ȵ�
		Log(LOG_ALL, "@ pUser In Stadium [%s]!", pUser->m_szCharacterName );
		return;
	} // if( DUNGEON_TYPE_MATCH_STATIDUM == DungeonIDtoType(pUser->m_dwDungeonID) )

	if(CheckHaveTeamMatch(pUser))
	{
		return;
	}
	
	if( pUser->IsHaveGMC() )
	{//�̹� ��� ����
		Log(LOG_ALL, "@ Already Regist AutoMatch System!");
		return;
	}

	MakeGMC(pUser, TMC.iMatchType ,TMC.iMatchGuildType, true);//�⺻�̴ϱ� �� Ÿ���� NONE
}

void Recv_CTWS_TM_REMOVE(CWorldUser* pUser)
{
	if( pUser->IsHaveGMC() )
	{
		Log(LOG_ALL, "! Recv_CTWS_TM_REMOVE name[%s]", pUser->m_szCharacterName);
		
		CPlayerInfo Player( pUser );
		g_GuildMatcher.Remove(pUser->m_GMC, Player);
		pUser->ClearGuildWarData();//���ϱ� Ŭ����
	}
}

void Recv_CMD_STADIUM_STATE(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{//���������� ���� �޴´�.
	DSTWS_STADIUM_STATE* pPacket = (DSTWS_STADIUM_STATE*)pMsg;

	if( pPacket->GetPacketSize() != dwLength )
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	} // if( pPacket->GetPacketSize() != dwLength )

	switch( pPacket->wType )
	{
	case STADIUM_END:
		{//���� ����.
			STADIUM_DATA &rSD = pPacket->Body.SD;
			CStadium *pStadium = g_StadiumMgr.GetStadium(rSD.match_type, rSD.wDungeonID, rSD.byLayerIndex);
			if(pStadium)
			{
				g_StadiumMgr.LogGuildWar
					( pStadium, pPacket->bIsSuccess );
				pStadium->SendStateBroadCast();
				pStadium->ClearUser();//Clear ���� �ϸ� �ȵ˴ϴ�
			}
		}//break; //Break �Ϻη� ���̴ϴ�.
	case STADIUM_MODIFY:
		{
			Log(LOG_ALL, "@------- Stadium State Modifyed! [%d] [%d:%d]", pPacket->Body.SD.byState, pPacket->Body.SD.aPos[0].x, pPacket->Body.SD.aPos[0].y);
		
			STADIUM_DATA &rSD = pPacket->Body.SD;

			CStadium S( rSD );
			g_StadiumMgr.ModifyStadiumState( S );

			CStadium *pStadium = g_StadiumMgr.GetStadium(rSD.match_type, rSD.wDungeonID, rSD.byLayerIndex);
			if(pStadium)
			{//���� ����� ���� ������
				pStadium->SendPointToUser(rSD.byState, NULL);//��ο���
				if(rSD.byState == TMS_PREPARE)//��� �غ� ���
				{//������ ���
					pStadium->SendStateBroadCast();
				}
			}
		}break;
	case STADIUM_POINT:
		{
			STADIUM_TEAM_POINT &rSTP = pPacket->Body.STP;
			
			CStadium *pStadium = g_StadiumMgr.GetStadium(rSTP.match_type, rSTP.wDungeonID, rSTP.byLayerIndex);
			if(pStadium)
			{
				pStadium->dwRemainBeginTime = rSTP.dwRemainBeginTime;//�ð� ���� 
				pStadium->dwRemainEndTime	= rSTP.dwRemainEndTime;
				pStadium->ModifyPoint( rSTP.team_index, rSTP.point, pStadium->byState);
			}
			else
			{
				Log(LOG_ALL, "@------- Try Modify Point where No Stadium!   Warning!!!");
			}
		}break;
	}
	
}


void Send_WSTDS_TEAM_MATCH(CWorldUser *pUser, const DUNGEON_MOVE_INFO &rDMI , const size_t team_index, const DWORD dwGuildWarFlag)
{
	WSTDS_TEAM_MATCH TM_packet;
			
	TM_packet.wType = CMD_WSTDS_REG_PLAYER;
	
	TM_packet.wDungeonID	= rDMI.wDungeonID;
	TM_packet.byLayerIndex = rDMI.byLayerIndex;

	::strcpy( TM_packet.Body.RP.szPlayerName, pUser->m_szCharacterName);
	
	TM_packet.Body.RP.team_index =	team_index;
	TM_packet.Body.RP.dwExFlag = dwGuildWarFlag;
	
	DUNGEON_DATA_EX* pDungeonStadium = g_pDungeonTable->GetDungeonInfo((WORD)rDMI.wDungeonID);//�̵��� �� �� �����.
	if(pDungeonStadium)
	{
		g_pNet->SendToServer( pDungeonStadium->m_pServer->dwConnectionIndex, (char*)&TM_packet, TM_packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
	else
	{
		Log(LOG_IMPORTANT, "There Is No Stadium(Name:%s)", pUser->m_szCharacterName);
	}
}

bool MoveToStadium( CWorldUser *pUser, const DUNGEON_MOVE_INFO &rDMI , const size_t team_index, const DWORD dwGuildWarFlag , const bool bIsGMCClear)//�⺻������ �÷��̾��
{//���϶��� ��� �ϳ�?;;
	if(pUser)
	{//���� �̵� 
		WSTDS_USER_PORTAL packet;

		packet.byGMCommand = GM_Command_Dungeon_Portal;			// enum GM_COMMAND ���� 
		packet.dwTargetToUserIndex = 0;//�� Ÿ��
		
		DUNGEON_MOVE_INFO &rPacketDMI = packet.DMI;

		rPacketDMI	= rDMI;//Notify�� ��ǥ ����� �̵�
		
		packet.dwUserIndex		= pUser->m_dwUserIndex;
		
		if( G_W_F_MASTER != (G_W_F_CMP_BASE & dwGuildWarFlag) )
		{//�����Ͱ� �ƴҶ��� �����. �����ʹ� GMC�� ��� �ϹǷ�
			if( bIsGMCClear )
			{
				pUser->ClearGuildWarData();//�̵� �ϹǷ� GMCŬ����
			}
		}

		DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pUser->m_dwDungeonID);
		if(pDungeon)
		{//������ �ִ� ������ //������ ��� �߰��� ������ ������ ����� ��.
			Send_WSTDS_TEAM_MATCH(pUser, rDMI, team_index, dwGuildWarFlag);//���� ���

			const WORD x = static_cast< WORD >((rand()%800)-400);//��ǥ ����
			const WORD y = static_cast< WORD >((rand()%800)-400);

			rPacketDMI.wX += x;
			rPacketDMI.wY += y;

			Log(LOG_IMPORTANT, "User Move To Dungeon (Name:%s) X[%d],Y[%d]", pUser->m_szCharacterName, rPacketDMI.wX ,rPacketDMI.wY);

			g_pNet->SendToServer( pDungeon->m_pServer->dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return true;
		} // if(pDungeon)
		else
		{//������ ���� ���忡 �ִ�.
			Log(LOG_IMPORTANT, "User don't have Dungeon (Name:%s)", pUser->m_szCharacterName);
		}
	} // if(pUser)

	return false;
}

bool MoveToPrevDungeon( CWorldUser *pUser, const WORD wNowStadiumID, const bool bIsGMCClear)//�⺻������ �÷��̾��
{//���� �������� ����
	if(pUser)//����� �����ٰ� �޼��� ������.
	{//���� �̵� 
		WSTDS_USER_PORTAL packet;

		packet.byGMCommand = GM_Command_Dungeon_Portal;			// enum GM_COMMAND ���� 
		packet.dwTargetToUserIndex = 0;//�� Ÿ��
		
		DUNGEON_MOVE_INFO &rPacketDMI = packet.DMI;

		packet.dwUserIndex		= pUser->m_dwUserIndex;

		if( bIsGMCClear )
		{
			CPlayerInfo Player( pUser );
			g_GuildMatcher.Remove( pUser->m_GMC, Player);
			pUser->ClearGuildWarData();//�̵� �ϹǷ� GMCŬ����
		}

		rPacketDMI.wDungeonID = static_cast< WORD >(pUser->m_dwPrevDungeonId);
		rPacketDMI.byLayerIndex = 0;
		rPacketDMI.wX = 0;
		rPacketDMI.wY = 0;

		DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(wNowStadiumID);

		if(pDungeon)
		{//������ �ִ� ������ //������ ��� �߰��� ������ ������ ����� ��.
			g_pNet->SendToServer( pDungeon->m_pServer->dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return true;
		} // if(pDungeon)
		else
		{//������ ���� ���忡 �ִ�.
			Log(LOG_IMPORTANT, "User don't have Dungeon (Name:%s)", pUser->m_szCharacterName);
		}
	} // if(pUser)
	return false;
}