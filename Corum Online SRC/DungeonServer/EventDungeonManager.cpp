#include "StdAfx.h"
#include "EventDungeonManager.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "../CommonServer/CommonHeader.h"
#include "OwnServer.h"
#include "User.h"
#include "Dungeon.h"
#include "DUNGEON_DATA_EX.h"
#include "Monster.h"
#include "MonsterManager.h"
#include "DungeonLayer.h"
#include "Map.h"
#include "TraderMonster.h"
#include "GameSystemNative.h"
#include "DungeonTable.h"


//======================================
// �̺�Ʈ ������ ��쿡��, �̺�Ʈ ���� �Ӽ����� ���´�.
//======================================
BOOL QueryEventDungeonProperties()
{
	if (NULL == g_pDungeonTable) return FALSE;
		
	POSITION_	pos				= g_pDungeonTable->m_pDungeonList->GetHeadPosition();
	CDungeon*	pDungeon		= NULL;
	int			iRecordCount	= 0;
	char		szQuery[0x400]	= {0,};		
		
	// DB���� ���� �о���µ� �����ϸ�, �޸𸮸� �Ҵ��ϰ�, Dungeon Object���� ������ �� �ְ� �Ѵ�.	
	EVENT_DUNGEON_PROPERTY* pEventDungeonProperty = NULL;	

	while (pos)
	{
		pDungeon = (CDungeon*)g_pDungeonTable->m_pDungeonList->GetNext(pos);

		if (NULL == pDungeon)
			continue;

		if (DUNGEON_TYPE_EVENT != pDungeon->GetDungeonDataEx()->GetDungeonType())
			continue;
		
		memset(szQuery, 0, 0x400);	
		wsprintf(szQuery, "Select [EventType], [MaxEnterNo],[StartKillMonsterCount], [FlagParty], [FlagItemDrop], [FlagFloorDown], [FlagBattleRoyal], [FlagMonsterKillDown], [FlagExpStyle], [MonsterExpRatio], [BossExpRatio], [FailedExpRatio], [BonusMoney], [BonusExpRatio], [SpecialConditionStart], [SpecialCondition] From EventDungeonProperty Where [DungeonNumber] = %d Order by [EventType]", pDungeon->GetID());

		pEventDungeonProperty = new EVENT_DUNGEON_PROPERTY;
		
		if (NULL == pEventDungeonProperty)
			continue; 

		iRecordCount = g_pDb->OpenRecord(szQuery,pEventDungeonProperty,1,GAME_DB);

		if (0 < iRecordCount)
		{
			// ���� Ŭ������ ���� �־ ������ �� �ְ� �Ѵ�. ������ ���α׷��� ����� �� ���� Ŭ������ �Ѵ�. 
			pDungeon->SetEventDungeonProperty(pEventDungeonProperty);
			pEventDungeonProperty = NULL;
		}
		else
		{
			if (NULL != pEventDungeonProperty)
			{
				delete pEventDungeonProperty;
				pEventDungeonProperty = NULL;
			}
		}		 		 
	} 
	
	return TRUE;
}


//======================================
// ���� ���� ���� �ִ� ��Ƽ�� �̾��� 
//======================================
BYTE GetMyPartyUser(CUser* pUser, CUser** pPartyUser, WORD* pPartyKillMonTotalCount, BOOL bIsMyselfExclusion)
{
	CDungeonLayer*	pMyLayer	= pUser->GetCurDungeonLayer();
	DWORD			dwPartyID	= pUser->GetPartyID();
	BYTE			byUserCount	= 0;

	Node<CUser>* pNode = pUser->GetCurDungeon()->GetPartyUserHash()->GetBucketHead(dwPartyID);

	while( pNode )
	{
		if(pNode->pData->GetCurDungeon() && pNode->pData->GetUnitStatus() != UNIT_STATUS_DEAD)
		{
			CDungeonLayer* pLayer = pNode->pData->GetCurDungeonLayer();

			if(pLayer && pLayer == pMyLayer && pNode->pData->m_dwPartyId == dwPartyID)
			{
				if( bIsMyselfExclusion )
				{
					// �� �ڽ��� ����..
					if(pUser == pNode->pData)
					{
						pNode = pNode->pNext;
						continue;
					}
				}
				
				pPartyUser[byUserCount] = pNode->pData;
				byUserCount++;

				if(pPartyKillMonTotalCount)
				{
					*pPartyKillMonTotalCount += pNode->pData->m_wKillMonsterCurrentLayer;
				}								
			}
		}
		
		pNode = pNode->pNext;
	}

	return byUserCount;
}


//==============================
// �������� ���÷��� 
//==============================
void Event_SurvivalDisplay(CUser* pUser)
{
	DSTC_EVENT_DUNGEON_SURVIVAL_DISPLAY packet;					
	packet.byDisplayMode				= SIEGEINFOWND_TYPE_EVENT_SURVIVAL;
	packet.wDungeonID					= pUser->GetCurDungeonID(); 
	packet.wCurrentMonterLayerCount		= (WORD)pUser->GetCurDungeon()->GetDungeonTotalUserCount();
	packet.wKillMonsterLayerCount		= pUser->m_wKillMonsterCurrentLayer;
	NetSendToUser(pUser->m_dwConnectionIndex, (char*)&packet,packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
}


//==============================
// ���ô��� ���÷��� 
//==============================
void Event_TraniningDisplay(CUser** pCommonUser, CUser* pUser, BYTE byUserCount, WORD wPartyKillMonTotalCount
							, const EVENT_DUNGEON_PROPERTY* pEventDungeonProperty, BOOL bIsParty)
{
	DSTC_EVENT_DUNGEON_TRAINING_DISPLAY packet;
	BYTE byStartKillMonCount = pEventDungeonProperty->byStartKillMonsterCount;

	if(bIsParty)
	{
		// ��Ƽ�� 
		if(byUserCount == 1)
		{
			BYTE byLayerIndex = pCommonUser[0]->GetCurLayerIndex();

			if(	byLayerIndex == 0 || byLayerIndex == 1 )				
				packet.wTotalKillMonSterLayerCount = byStartKillMonCount;					
			else
				packet.wTotalKillMonSterLayerCount = byStartKillMonCount - ((byLayerIndex - 1) * 5);					
						
			packet.wCurrentKillMonsterLayerCount = pCommonUser[0]->m_wKillMonsterCurrentLayer;
			packet.wDungeonID = pCommonUser[0]->GetCurDungeonID();
			NetSendToUser(pCommonUser[0]->m_dwConnectionIndex, (char*)&packet,packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			if(	!pUser->m_bInLastLayer )
			{
				if(packet.wTotalKillMonSterLayerCount <= packet.wPartyCurrentKillMonsterLayerCount)
				{
					DSTC_EVENT_DUNGEON_TRAINING_ONELAYER_CLAYER packet;
					packet.wDungeonID = pCommonUser[0]->GetCurDungeonID();
					NetSendToUser(pCommonUser[0]->m_dwConnectionIndex, (char*)&packet,packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
			}

			return;
		}

		for(BYTE i = 0; i < byUserCount; ++i)
		{
			BYTE byLayerIndex = pCommonUser[i]->GetCurLayerIndex();

			if(	byLayerIndex == 0 || byLayerIndex == 1 )
			{
				packet.wTotalKillMonSterLayerCount = byStartKillMonCount * byUserCount;
				packet.wTotalKillMonSterLayerCount += (WORD)(packet.wTotalKillMonSterLayerCount * 0.2);
			}						
			else
			{
				packet.wTotalKillMonSterLayerCount = (byStartKillMonCount - ((byLayerIndex - 1) * 5)) * byUserCount;
				packet.wTotalKillMonSterLayerCount += (WORD)(packet.wTotalKillMonSterLayerCount * 0.2);
			}

			packet.wCurrentKillMonsterLayerCount		= pCommonUser[i]->m_wKillMonsterCurrentLayer;
			packet.wPartyCurrentKillMonsterLayerCount	= wPartyKillMonTotalCount;
			packet.wDungeonID							= pCommonUser[i]->GetCurDungeonID();
			packet.bPartySameDungeonLayerUserCount		= byUserCount;

			NetSendToUser(pCommonUser[i]->m_dwConnectionIndex, (char*)&packet,packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			if(	!pCommonUser[i]->m_bInLastLayer )
			{
				if(packet.wTotalKillMonSterLayerCount <= packet.wPartyCurrentKillMonsterLayerCount)
				{
					DSTC_EVENT_DUNGEON_TRAINING_ONELAYER_CLAYER packet;
					packet.wDungeonID = pCommonUser[i]->GetCurDungeonID();
					NetSendToUser(pCommonUser[i]->m_dwConnectionIndex, (char*)&packet,packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
			}
		}				
	}
	else
	{
		// ��� 
		BYTE byLayerIndex = pUser->GetCurLayerIndex();

		if(	byLayerIndex == 0 || byLayerIndex == 1 )				
			packet.wTotalKillMonSterLayerCount = byStartKillMonCount;
		else
			packet.wTotalKillMonSterLayerCount = byStartKillMonCount - ((byLayerIndex - 1) * 5);		
		
		packet.wCurrentKillMonsterLayerCount = pUser->m_wKillMonsterCurrentLayer;
		packet.wDungeonID = pUser->GetCurDungeonID();
		NetSendToUser(pUser->m_dwConnectionIndex,(char*)&packet,packet.GetPacketSize(),FLAG_SEND_NOT_ENCRYPTION);

		if(	!pUser->m_bInLastLayer )
		{
			if(packet.wTotalKillMonSterLayerCount <= packet.wCurrentKillMonsterLayerCount)
			{
				DSTC_EVENT_DUNGEON_TRAINING_ONELAYER_CLAYER packet;
				packet.wDungeonID = pUser->GetCurDungeonID();
				NetSendToUser(pUser->m_dwConnectionIndex, (char*)&packet,packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			}
		}
	}	
}


//==============================
// ���ô��� Ŭ����  
//==============================
void Event_TraniningClear(CUser** pCommonUser, CUser* pUser, BYTE byUserCount
						  , const EVENT_DUNGEON_PROPERTY* pEventDungeonProperty, BOOL bIsParty)
{
	if(bIsParty)
	{
		for(BYTE i = 0; i < byUserCount; i++)
		{
			if(!pCommonUser[i]->m_bIsEventDungeonClear)
			{
				// ����ġ�� ���� �Դ� ����ΰ�?
				if(pEventDungeonProperty->byFlagExpStyle)
				{
					pCommonUser[i]->AddExp(pCommonUser[i]->m_dwAccumulationExp);				
					pCommonUser[i]->m_dwAccumulationExp = 0;
				}

				// ���ʽ��� �ش�.���ʽ� ����ġ�� ������ ���ʽ� ����ġ�� �԰� ��.
				if(pEventDungeonProperty->byBonusExpRatio)
				{
					DWORD dwExp = ( GetExpTableOfLevel(OBJECT_TYPE_PLAYER, pCommonUser[i]->GetLevel() + 1) -
									GetExpTableOfLevel(OBJECT_TYPE_PLAYER, pCommonUser[i]->GetLevel()) ) /
								  (100 / pEventDungeonProperty->byBonusExpRatio);

					pCommonUser[i]->m_dwAccumulationExp += dwExp;
					pCommonUser[i]->AddExp(pCommonUser[i]->m_dwAccumulationExp);				
					pCommonUser[i]->m_dwAccumulationExp = 0;
				}
				
				// ���ʽ� �Ӵϸ� ���� : �ִ��� 2005.1.21
				pCommonUser[i]->m_dwMoney += pEventDungeonProperty->dwBonusMoney;

				pCommonUser[i]->m_bIsEventDungeonClear = TRUE;
				pCommonUser[i]->ChangeGodMode(GODMODE_STATUS_MAPLOADING);

				CDungeonLayer* pLayer =	pCommonUser[i]->GetCurDungeonLayer();

				DSTC_EVENT_DUNGEON_CLEAR ClearPacket;
				ClearPacket.dwUserIndex = pCommonUser[i]->GetID();
				pLayer->BroadCastSectionMsg( (char*)&ClearPacket, ClearPacket.GetPacketSize(), pCommonUser[i]->GetPrevSectionNum() );
				
				DSTC_EVENT_DUNGEON_CLEARUSER_SEND ClearUserSendPacket;
				lstrcpy(ClearUserSendPacket.szCharName, pCommonUser[i]->GetName());
				pCommonUser[i]->GetCurDungeon()->BroadCast((char*)&ClearUserSendPacket, ClearUserSendPacket.GetPacketSize());
			}
		}
	}
	else
	{
		if(!pUser->m_bIsEventDungeonClear)
		{
			// ����ġ�� ���� �Դ� ����ΰ�?
			if (0 != pEventDungeonProperty->byFlagExpStyle)
			{
				pUser->AddExp(pUser->m_dwAccumulationExp);				
				pUser->m_dwAccumulationExp = 0;
			}

			// ���ʽ��� �ش�. ���ʽ� ����ġ�� ������ ���ʽ� ����ġ�� �԰� ��.	
			if (0 != pEventDungeonProperty->byBonusExpRatio)
			{
				DWORD dwExp = ( GetExpTableOfLevel(OBJECT_TYPE_PLAYER, pUser->GetLevel() + 1) -
								GetExpTableOfLevel(OBJECT_TYPE_PLAYER, pUser->GetLevel()) ) /
							  (100 / pEventDungeonProperty->byBonusExpRatio);

				pUser->m_dwAccumulationExp += dwExp;
				pUser->AddExp(pUser->m_dwAccumulationExp);				
				pUser->m_dwAccumulationExp = 0;
			}
			
			// ���ʽ� �Ӵϸ� ���� : �ִ��� 2005.1.21
			pUser->m_dwMoney += pEventDungeonProperty->dwBonusMoney;
			
			pUser->m_bIsEventDungeonClear = TRUE;
			pUser->ChangeGodMode(GODMODE_STATUS_EVENT_TIMER);

			CDungeonLayer* pLayer =	pUser->GetCurDungeonLayer();

			DSTC_EVENT_DUNGEON_CLEAR ClearPacket;
			ClearPacket.dwUserIndex = pUser->GetID();
			pLayer->BroadCastSectionMsg( (char*)&ClearPacket, ClearPacket.GetPacketSize(), pUser->GetPrevSectionNum());
			
			DSTC_EVENT_DUNGEON_CLEARUSER_SEND ClearUserSendPacket;
			lstrcpy(ClearUserSendPacket.szCharName,pUser->GetName());
			pUser->GetCurDungeon()->BroadCast((char*)&ClearUserSendPacket,ClearUserSendPacket.GetPacketSize());
		}
	}
}


//==============================
// �������� Ŭ����
//==============================
void Event_SurvivalClear(CDungeon* pDungeon)
{
	if( NULL == pDungeon )														return;
	if( DUNGEON_TYPE_EVENT != pDungeon->GetDungeonDataEx()->GetDungeonType() )	return;
	if( NULL == pDungeon->GetEventDungeonProperty() )							return;

	const EVENT_DUNGEON_PROPERTY* pEventDungeonProperty = pDungeon->GetEventDungeonProperty();

	if( pDungeon->GetDungeonTotalUserCount() == 1 )
	{
		DSTC_EVENT_DUNGEON_CLEAR packet;

		POSITION_	pos		= pDungeon->GetDungeonUserList()->GetHeadPosition();
		CUser*		pUser	= NULL;

		while(pos)
		{
			pUser = (CUser*)pDungeon->GetDungeonUserList()->GetNext(pos);

			if(pUser)
			{
				packet.dwUserIndex =  pUser->GetID();
				NetSendToUser(pUser->m_dwConnectionIndex,(char*)&packet,packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			}

			break;
		}

		if(NULL == pUser)	return;		

		DSTC_EVENT_DUNGEON_CLEAR_PRESENT ClearPresent;

		pUser->SetMoney(pUser->GetMoney() + pEventDungeonProperty->dwBonusMoney);
		ClearPresent.dwUserIndex	= pUser->GetID();	
		ClearPresent.wPresentMoney	= pEventDungeonProperty->dwBonusMoney;

		NetSendToUser(pUser->m_dwConnectionIndex, (char*)&ClearPresent, ClearPresent.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

		if(pEventDungeonProperty->byBonusExpRatio)
		{			
			pDungeon->PlusAccumulationExp(pEventDungeonProperty->byBonusExpRatio);
			pDungeon->AddAllAccumulationExp();
		}
	}
	else
	{
		DSTC_EVENT_DUNGEON_FAILED packet;
		pDungeon->BroadCast((char*)&packet, packet.GetPacketSize());
	}	
}


//==============================
// ���� ���������� 
//==============================
void Event_DungeonJoinUser( CUser* pUser, CDungeon* pDungeon )
{
	if( NULL == pUser )															return;
	if( NULL == pDungeon )														return;
	if( DUNGEON_TYPE_EVENT !=  pDungeon->GetDungeonDataEx()->GetDungeonType())	return;//: 050108 hwoarang
	if( NULL == pDungeon->GetEventDungeonProperty() )							return;

	const EVENT_DUNGEON_PROPERTY* pEventDungeonProperty = pDungeon->GetEventDungeonProperty();
	
	if( EVENT_DUNGEON_SURVIVAL == pEventDungeonProperty->byDungeonType )
	{
		Event_SurvivalDisplay(pUser);		
	}

	if( EVENT_DUNGEON_TRAINING == pEventDungeonProperty->byDungeonType )
	{
		if( pUser->GetPartyID() )
		{
			CUser*	pPartyUser[MAX_PARTY_USER]	= {NULL, };
			WORD	wPartyKillMonTotalCount		= 0;
			BYTE	byUserCount = GetMyPartyUser(pUser, pPartyUser, &wPartyKillMonTotalCount);
			Event_TraniningDisplay(pPartyUser, pUser, byUserCount, wPartyKillMonTotalCount, pEventDungeonProperty, TRUE);
		}
		else
		{
			Event_TraniningDisplay(NULL, pUser, NULL, NULL, pEventDungeonProperty, FALSE);	
		}
	}
}


//================================
// ������ ü���� ���̾� �Ҷ�  
//================================
BOOL Event_IsChangeLayer(CUser* pUser)
{
	if( NULL == pUser )												return FALSE;
	if( NULL == pUser->GetCurDungeon()->GetEventDungeonProperty() )	return FALSE;

	const EVENT_DUNGEON_PROPERTY* pEventDungeonProperty = pUser->GetCurDungeon()->GetEventDungeonProperty();

	if( EVENT_DUNGEON_SURVIVAL == pEventDungeonProperty->byDungeonType )
	{
		return FALSE;
	}

	if( EVENT_DUNGEON_TRAINING == pEventDungeonProperty->byDungeonType )
	{
		BYTE byLayerIndex = pUser->GetCurLayerIndex();

		if(byLayerIndex == 0)
		{
			return TRUE;
		}

		WORD wNeedKillMonCount = 0;

		if( pUser->GetPartyID() )
		{
			CUser*	pPartyUser[MAX_PARTY_USER]	= {NULL, };
			WORD	wPartyKillMonTotalCount		= 0;
			BYTE	byUserCount = GetMyPartyUser(pUser, pPartyUser, &wPartyKillMonTotalCount);

			if(byLayerIndex == 1)
			{
				wNeedKillMonCount = pEventDungeonProperty->byStartKillMonsterCount * byUserCount;
				if(byUserCount > 1)
					wNeedKillMonCount += WORD(wNeedKillMonCount * 0.2F);			
			}
			else
			{
				wNeedKillMonCount = (pEventDungeonProperty->byStartKillMonsterCount - (byLayerIndex * 5)) * byUserCount;
				if(byUserCount > 1)
					wNeedKillMonCount += WORD(wNeedKillMonCount * 0.2F);		
			}

			if(wNeedKillMonCount > wPartyKillMonTotalCount)
			{
				DSTC_EVENT_CHANGE_LAYER_RESULT packet;
				packet.bMoveResult = 2; 
				NetSendToUser( pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
				return FALSE;
			}			
		}
		else
		{
			if(	byLayerIndex == 1)
			{
				wNeedKillMonCount = pEventDungeonProperty->byStartKillMonsterCount;
			}
			else
			{
				wNeedKillMonCount = pEventDungeonProperty->byStartKillMonsterCount - (byLayerIndex * 5);
			}

			if (wNeedKillMonCount > pUser->m_wKillMonsterCurrentLayer)
			{
				DSTC_EVENT_CHANGE_LAYER_RESULT packet;
				packet.bMoveResult = 2; 
				NetSendToUser( pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
				return FALSE;
			}	
		}

		return TRUE;
	}
	
	return FALSE;
}


//==============================
// �μ�Ʈ ���̾� ���� �Ҷ� 
//==============================
void Event_InsertDungeonLayerUser(CUser* pUser, CDungeonLayer* pLayer)
{
	if( NULL == pUser )												return;
	if( NULL == pLayer )											return;
	if( FALSE == pUser->IsCurDungeonEventInLive() )					return;
	if( NULL == pUser->GetCurDungeon()->GetEventDungeonProperty() )	return;

	const EVENT_DUNGEON_PROPERTY* pEventDungeonProperty = pUser->GetCurDungeon()->GetEventDungeonProperty();

	if( EVENT_DUNGEON_SURVIVAL == pUser->GetCurDungeon()->GetEventDungeonType() )
	{
		 return;
	}

	if( EVENT_DUNGEON_TRAINING == pUser->GetCurDungeon()->GetEventDungeonType() )
	{
		// ������ ���� ���� ī��Ʈ �ʱ�ȭ 
		pUser->m_wKillMonsterCurrentLayer = 0;

		if(pUser->GetCurLayerIndex() == pLayer->GetParent()->GetTotalLayer() - 1)
		{
			pUser->m_bInLastLayer = TRUE;					

			// ������ ���̶�°��� �˷��ش�.
			DSTC_EVENT_DUNGEON_LAST_LAYER packet;		
			NetSendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}

		if( pUser->GetPartyID() )
		{
			CUser*	pPartyUser[MAX_PARTY_USER]	= {NULL, };
			WORD	wPartyKillMonTotalCount		= 0;
			BYTE	byUserCount = GetMyPartyUser(pUser, pPartyUser, &wPartyKillMonTotalCount);
			Event_TraniningDisplay(pPartyUser, pUser, byUserCount, wPartyKillMonTotalCount, pEventDungeonProperty, TRUE);
		}
		else
		{
			Event_TraniningDisplay(NULL, pUser, NULL, NULL, pEventDungeonProperty, FALSE);	
		}
	}	
}


//==============================
// ������ ���̾� ���� �Ҷ� 
//==============================
void Event_RemoveDungeonLayerUser(CUser* pUser, CDungeonLayer* pLayer)
{
	if( NULL == pUser )												return;
	if( NULL == pLayer )											return;
	if( FALSE == pUser->IsCurDungeonEventInLive() )					return;
	if( NULL == pUser->GetCurDungeon()->GetEventDungeonProperty() )	return;

	const EVENT_DUNGEON_PROPERTY* pEventDungeonProperty = pUser->GetCurDungeon()->GetEventDungeonProperty();

	if( EVENT_DUNGEON_SURVIVAL == pUser->GetCurDungeon()->GetEventDungeonType() )
	{
		 return;
	}

	if( EVENT_DUNGEON_TRAINING == pUser->GetCurDungeon()->GetEventDungeonType() )
	{
		if( pUser->GetPartyID() )
		{
			CUser*	pPartyUser[MAX_PARTY_USER]	= {NULL, };
			WORD	wPartyKillMonTotalCount		= 0;
			BYTE	byUserCount = GetMyPartyUser(pUser, pPartyUser, &wPartyKillMonTotalCount, TRUE);
			Event_TraniningDisplay(pPartyUser, pUser, byUserCount, wPartyKillMonTotalCount, pEventDungeonProperty, TRUE);
		}
		else
		{
			Event_TraniningDisplay(NULL, pUser, NULL, NULL, pEventDungeonProperty, FALSE);	
		}
	}	
}


//==============================
// ���� �׿����� 
//==============================
void Event_MonsterKillByUser( CUser* pUser, CMonster* pMonster )
{
	if( NULL == pUser )												return;
	if( NULL ==	pUser->GetCurDungeonLayer() )						return;
	if( FALSE == pUser->IsCurDungeonEventInLive() )					return;
	if( NULL == pUser->GetCurDungeon()->GetEventDungeonProperty() )	return;
	
	if( EVENT_DUNGEON_SURVIVAL == pUser->GetCurDungeon()->GetEventDungeonType() )
	{
		// ���� 
		++pUser->m_wKillMonsterCurrentLayer;
		Event_SurvivalDisplay(pUser);		
	}

	if( EVENT_DUNGEON_TRAINING == pUser->GetCurDungeon()->GetEventDungeonType() )
	{
		// ���� 
		const EVENT_DUNGEON_PROPERTY* pEventDungeonProperty = pUser->GetCurDungeon()->GetEventDungeonProperty();

		if( Event_IsClearEvent(pUser) )
		{
			// Ŭ���� �߱���...
			if( pUser->GetPartyID() )
			{
				CUser*	pPartyUser[MAX_PARTY_USER] = {NULL, };
				BYTE	byUserCount = GetMyPartyUser(pUser, pPartyUser, NULL);
				Event_TraniningClear(pPartyUser, pUser, byUserCount, pEventDungeonProperty, TRUE);
			}
			else
			{
				Event_TraniningClear(NULL, pUser, NULL, pEventDungeonProperty, FALSE);
			}
		}
		else
		{
			// ���� ����!!
			++pUser->m_wKillMonsterCurrentLayer;

			if( pUser->GetPartyID() )
			{
				CUser*	pPartyUser[MAX_PARTY_USER]	= {NULL, };
				WORD	wPartyKillMonTotalCount		= 0;
				BYTE	byUserCount = GetMyPartyUser(pUser, pPartyUser, &wPartyKillMonTotalCount);
				Event_TraniningDisplay(pPartyUser, pUser, byUserCount, wPartyKillMonTotalCount, pEventDungeonProperty, TRUE);
			}
			else
			{
				Event_TraniningDisplay(NULL, pUser, NULL, NULL, pEventDungeonProperty, FALSE);	
			}
		}		
	}		
}


//==============================
// ���Ͱ� ���� �׿����� 
//==============================
void Event_MonsterKillByMonster(CMonster* pAttackMon, CMonster* pDefenseMon)
{
	CUser* pUser = (CUser*)pAttackMon->GetLord();

	if( NULL == pAttackMon )										return;
	if( NULL == pDefenseMon )										return;
	if( NULL == pUser )												return;
	if( NULL ==	pUser->GetCurDungeonLayer() )						return;
	if( FALSE == pUser->IsCurDungeonEventInLive() )					return;
	if( NULL == pUser->GetCurDungeon()->GetEventDungeonProperty() )	return;

	if( EVENT_DUNGEON_SURVIVAL == pUser->GetCurDungeon()->GetEventDungeonType() )
	{
		// ���� 
		++pUser->m_wKillMonsterCurrentLayer;
		Event_SurvivalDisplay(pUser);
	}

	if( EVENT_DUNGEON_TRAINING == pUser->GetCurDungeon()->GetEventDungeonType() )
	{
		// ���� 
		const EVENT_DUNGEON_PROPERTY* pEventDungeonProperty = pUser->GetCurDungeon()->GetEventDungeonProperty();

		if( Event_IsClearEvent(pUser) )
		{
			// Ŭ���� �߱���...
			if( pUser->GetPartyID() )
			{
				CUser*	pPartyUser[MAX_PARTY_USER] = {NULL, };
				BYTE	byUserCount = GetMyPartyUser(pUser, pPartyUser, NULL);
				Event_TraniningClear(pPartyUser, pUser, byUserCount, pEventDungeonProperty, TRUE);
			}
			else
			{
				Event_TraniningClear(NULL, pUser, NULL, pEventDungeonProperty, FALSE);
			}
		}
		else
		{
			// ���� ����!!
			++pUser->m_wKillMonsterCurrentLayer;

			if( pUser->GetPartyID() )
			{
				CUser*	pPartyUser[MAX_PARTY_USER]	= {NULL, };
				WORD	wPartyKillMonTotalCount		= 0;
				BYTE	byUserCount = GetMyPartyUser(pUser, pPartyUser, &wPartyKillMonTotalCount);
				Event_TraniningDisplay(pPartyUser, pUser, byUserCount, wPartyKillMonTotalCount, pEventDungeonProperty, TRUE);
			}
			else
			{
				Event_TraniningDisplay(NULL, pUser, NULL, NULL, pEventDungeonProperty, FALSE);	
			}
		}		
	}		
}


//==============================
// ����ġ�� �Ѳ�����~~
//==============================
BOOL Event_GiveMeExp(CUser* pUser, DWORD dwExp)
{
	if( NULL == pUser )												return FALSE;
	if( NULL ==	pUser->GetCurDungeonLayer() )						return FALSE;
	if( FALSE == pUser->IsCurDungeonEventInLive() )					return FALSE;
	if( NULL == pUser->GetCurDungeon()->GetEventDungeonProperty() )	return FALSE;

	const EVENT_DUNGEON_PROPERTY* pEventDungeonProperty = pUser->GetCurDungeon()->GetEventDungeonProperty();

	// ���� �׿��� �� ����ġ ����ġ ���.			
	if( pEventDungeonProperty->byMonsterExpRatio )
	{
		dwExp *= (pUser->GetCurDungeon()->GetEventDungeonProperty()->byMonsterExpRatio / 100);
	}

	if(pUser->GetCurDungeon()->GetEventDungeonProperty()->byFlagExpStyle)
	{
		// Ȥ�� ����ġ�� ���� Ŭ���� �Ŀ� ���Ƽ� �Դ� ����ΰ�?
		pUser->m_dwAccumulationExp += dwExp;
	}
	else
	{
		pUser->AddExp(dwExp);
	}	

	return TRUE;
}


//==============================
// ��Ƽ
//==============================
void Event_SetPartyID(CUser* pUser)
{
	if( NULL == pUser )												return;
	if( NULL ==	pUser->GetCurDungeonLayer() )						return;
	if( FALSE == pUser->IsCurDungeonEventInLive() )					return;
	if( NULL == pUser->GetCurDungeon()->GetEventDungeonProperty() )	return;

	const EVENT_DUNGEON_PROPERTY* pEventDungeonProperty = pUser->GetCurDungeon()->GetEventDungeonProperty();

	if( EVENT_DUNGEON_SURVIVAL == pUser->GetCurDungeon()->GetEventDungeonType() )
	{
		return;
	}

	if( EVENT_DUNGEON_TRAINING == pUser->GetCurDungeon()->GetEventDungeonType() )
	{
		if( pUser->GetPartyID() )
		{
			CUser*	pPartyUser[MAX_PARTY_USER]	= {NULL, };
			WORD	wPartyKillMonTotalCount		= 0;
			BYTE	byUserCount = GetMyPartyUser(pUser, pPartyUser, &wPartyKillMonTotalCount);
			Event_TraniningDisplay(pPartyUser, pUser, byUserCount, wPartyKillMonTotalCount, pEventDungeonProperty, TRUE);
		}
		else
		{
			Event_TraniningDisplay(NULL, pUser, NULL, NULL, pEventDungeonProperty, FALSE);	
		}
	}
}


//==============================
// ���� �α׾ƿ� ������
//==============================
void Event_DisconnectUser(CUser* pUser)
{
	if( NULL == pUser )												return;
	if( NULL ==	pUser->GetCurDungeonLayer() )						return;
	if( FALSE == pUser->IsCurDungeonEventInLive() )					return;
	if( NULL == pUser->GetCurDungeon()->GetEventDungeonProperty() )	return;

	const EVENT_DUNGEON_PROPERTY* pEventDungeonProperty = pUser->GetCurDungeon()->GetEventDungeonProperty();

	if( EVENT_DUNGEON_SURVIVAL == pUser->GetCurDungeon()->GetEventDungeonType() )
	{
		return;
	}

	if( EVENT_DUNGEON_TRAINING == pUser->GetCurDungeon()->GetEventDungeonType() )
	{
		if( pUser->GetPartyID() )
		{
			CUser*	pPartyUser[MAX_PARTY_USER]	= {NULL, };
			WORD	wPartyKillMonTotalCount		= 0;
			BYTE	byUserCount = GetMyPartyUser(pUser, pPartyUser, &wPartyKillMonTotalCount, TRUE);
			Event_TraniningDisplay(pPartyUser, pUser, byUserCount, wPartyKillMonTotalCount, pEventDungeonProperty, TRUE);
		}
		else
		{
			Event_TraniningDisplay(NULL, pUser, NULL, NULL, pEventDungeonProperty, FALSE);	
		}
	}
	
	DSTWS_EVENT_DUNGEON_USER_OUT packet;
	packet.wDungeonID = WORD(pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID);
	g_pNet->SendToServer(WSINDEX, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );		
}


//================================
// �̺�Ʈ���� ���� �޽��� ����
//================================
void Event_FinishMapLoading(CUser* pUser)
{
	if( NULL == pUser )												return;
	if( NULL ==	pUser->GetCurDungeonLayer() )						return;
	if( FALSE == pUser->IsCurDungeonEventInLive() )					return;
	if( NULL == pUser->GetCurDungeon()->GetEventDungeonProperty() )	return;
//	if( 0 == pUser->GetCurLayerIndex() )							return;//: 050108 hwoarang

	const EVENT_DUNGEON_PROPERTY* pEventDungeonProperty = pUser->GetCurDungeon()->GetEventDungeonProperty();

	DSTC_EVENT_DUNGEON_ENTER EnterPacket;			
	EnterPacket.byEventType	= pEventDungeonProperty->byDungeonType; 
	EnterPacket.wDungeonID	= WORD(pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID);
	NetSendToUser(pUser->m_dwConnectionIndex, (char*)&EnterPacket, EnterPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);					
}


//================================
// �̺�Ʈ���� Ŭ���� �ߴ��� �˻� 
//================================
BOOL Event_IsClearEvent(CUser* pUser)
{
	if( NULL == pUser )												return FALSE;
	if( NULL == pUser->GetCurDungeon() )							return FALSE;
	if( NULL ==	pUser->GetCurDungeonLayer() )						return FALSE;
	if( FALSE == pUser->IsCurDungeonEventInLive() )					return FALSE;
	if( NULL == pUser->GetCurDungeon()->GetEventDungeonProperty() )	return FALSE;
	if( TRUE == pUser->m_bIsEventDungeonClear)						return FALSE;

	const EVENT_DUNGEON_PROPERTY* pEventDungeonProperty = pUser->GetCurDungeon()->GetEventDungeonProperty();
	
	if( EVENT_DUNGEON_TRAINING == pUser->GetCurDungeon()->GetEventDungeonType() )
	{
		if(FALSE == pUser->m_bInLastLayer)
			return FALSE;

		BYTE byLayerIndex		= pUser->GetCurLayerIndex();
		WORD wNeedKillMonCount	= 0;

		if( pUser->GetPartyID() )
		{
			CUser*	pPartyUser[MAX_PARTY_USER]	= {NULL, };
			WORD	wPartyKillMonTotalCount		= 0;
			BYTE	byUserCount = GetMyPartyUser(pUser, pPartyUser, &wPartyKillMonTotalCount);

			wNeedKillMonCount = (pEventDungeonProperty->byStartKillMonsterCount - (byLayerIndex * 5)) * byUserCount;

			if(byUserCount > 1)
				wNeedKillMonCount += WORD(wNeedKillMonCount * 0.2F);	
			
			if(wPartyKillMonTotalCount >= wNeedKillMonCount)
			{
				return TRUE;
			}
		}
		else
		{
			wNeedKillMonCount = pEventDungeonProperty->byStartKillMonsterCount - (byLayerIndex * 5);

			if(pUser->m_wKillMonsterCurrentLayer >= wNeedKillMonCount)
			{
				return TRUE;
			}
		}
	}	

	return FALSE;
}



