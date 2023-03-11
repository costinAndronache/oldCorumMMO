#include "stdafx.h"
#include <sys/TIMEB.H>
#include "Dungeon.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "OwnServer.h"
#include "DUNGEON_DATA_EX.h"
#include "DungeonLayer.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "DungeonTable.h"
#include "AcceptTable.h"
#include "recvmsg.h"
#include "RecordSet.h"
#include "Map.h"
#include "Section.h"
#include "Monster.h"
#include "MonsterManager.h"
#include "MonsterTable.h"
#include "ServerFunction.h"
#include "GameSystemNative.h"
#include "DBProc.h"
#include "MsgProc.h"
#include "MsgProc_for_jjw6263.h"
#include "MsgProc_for_minbobo.h"
#include "MsgProc_for_Yak.h"
#include "MsgProc_for_Y2jin.h"
#include "ItemAttrLayer.h"

#define ITEM_SKILL_BOOK_START	20519
#define ITEM_SKILL_BOOK_END		20578

#define BAG_ITEM_NUM_FRIST	3600
#define BAG_ITEM_NUM_LAST	3700


#define FAIL_VALUE(x1,x2)	{ x1 = x2; __leave; }


void (*PacketProc[ MAX_UPDATE_GAME ][ MAX_PACKET_NUM ])( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );


void InitializePacketProc()
{
	memset(PacketProc, 0, sizeof(PacketProc));

	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_DUNGEON_JOIN ]					= CmdDungeonJoin;	
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_DUNGEON_MOVE ]					= CmdDungeonMove;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_DUNGEON_STOP ]					= CmdDungeonStop;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_DUNGEON_ATTACK_USER_MON ]			= CmdDungeonAttack_User_Mon;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_DUNGEON_ATTACK_USER_USER ]		= CmdDungeonAttack_User_User;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_DUNGEON_CHAT ]					= CmdDungeonChat;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_DUNGEON_CASTING ]					= CmdDungeonSkillCasting; 
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_SKILL_LEVELUP ]					= CmdSkillLevelUp;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_ITEM_PICKUP ]						= CmdPickupItem;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_ITEM_USED ]						= CmdItemUsed;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_STATUS_LEVELUP]					= CmdStatusLevelUp;	
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_DUNGEON_REQUEST_INFO ]			= CmdDungeonRequestInfo;	
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_DUNGEON_CALLDUARDIAN ]			= CmdCallGuardian;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_MONSTER_COMMAND ]					= CmdMonsterCommand;	
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_GUARDIAN_ATTACK ]					= CmdGuardianAttack;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_GUARDIAN_COMMAND ]				= CmdGuardianCommand;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_GUARDIAN_COMMAND_MOVE ]			= CmdGuardianCommandMove;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_GUARDIAN_COMMAND_ATTACK]			= CmdGuardianCommandAttack;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_GUARDIAN_SKILL_SELECT]			= CmdGuardianSkillSelect;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_GM::CMD_QUICK_MOVE ]						= CmdGMQuickMove;	
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_PORTAL_MOVE]						= CmdPortalMove;	
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_REQUEST_SECTION_INFO_FOR_PORTAL]  = CmdRequestSectionInfoForPortal;	
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_FIND_HACK ]						= CmdFindHack;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_TICK_CHECK]						= CmdTickCheck;
	
#ifndef NEW_COMMAND
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_GM_COMMAND]						= CmdGmCommand;			
#endif
						
	InitializePacketProcJJW6263();
	InitializePacketProcMinbobo();
	InitializePacketProcYak();
	InitializePacketProcY2jin();	
}


// ��Ż ��Ŷ
void CmdPortalMove( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_PORTAL_MOVE *pPacket = (CTDS_PORTAL_MOVE*)pMsg;

	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);



	if(!pUser)					return;
	if(!pUser->GetCurDungeon())	return;
	CDungeonLayer*	pLayer = pUser->GetCurDungeonLayer();

	BYTE byPortalFail = NULL;

	__try
	{
		if(	DUNGEON_TYPE_VILLAGE == DungeonIDtoType( pPacket->wToDungeonID )// hwoarang �����̶��
		&&	pPacket->bLayerNo != 255	 	// �������� �𸦶� 255�� �ֱ⶧���� �ʿ�
		&&	pPacket->bLayerNo != 0			// ���������� 0�� �̻��� ���� ���̾� �̴�.
			)
		{
			FAIL_VALUE(byPortalFail, 9);
		}

		if (DUNGEON_TYPE_EVENT == pUser->GetCurDungeon()->GetDungeonDataEx()->GetDungeonType())
			FAIL_VALUE(byPortalFail, 15);
		
		if (TRUE == pUser->GetCurDungeon()->GetDungeonDataEx()->IsEventDungeon() && 0 == pPacket->bPortalType)
			FAIL_VALUE(byPortalFail, 16);

		if (NULL == pLayer)
			FAIL_VALUE(byPortalFail, 9);

		if (UNIT_STATUS_DEAD == pUser->GetUnitStatus())
			FAIL_VALUE(byPortalFail, 7);

		if (TRUE == pUser->m_bPortalMoving)
			FAIL_VALUE(byPortalFail, 5); 

//	������� GM Ÿ�� ��Ż�� ����Ѵ�.: hwoarang 050202
//		if (Portal_Type_GM == pPacket->bPortalType && FALSE == pUser->IsGMUser())
//			FAIL_VALUE(byPortalFail, 9);

		CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo( pPacket->wToDungeonID );

		if(pDungeon)	
		{
			// ���� ��Ʈ�̴�. �� ���������� �Ҵ�� �����̸� �ٷ� ó��

			if( pDungeon->GetDungeonDataEx()->m_bSiege && !pDungeon->GetDungeonDataEx()->IsDungeonOwner(pUser) )
			{
				pUser->GoToWorld( pUser->m_bDestWorldSpot, GOTOWORLD_STATUS_GOTOWORLD );
				return;

				// ��Ī���̶��......
				// ��������̾��ٸ�
				// ����Ѵ�.
			/*	if( (pUser->m_sMatchDescInfo.m_bMatching) 
					|| (pUser->m_byGuildFlag != G_W_F_NONE && pUser->m_byGuildFlag != G_W_F_OBSERVER) 
					)
				{
					// ��Ż �ȵ˴ϴ�.��, ���ش�.
					pUser->GoToWorld( pUser->m_bDestWorldSpot, GOTOWORLD_STATUS_GOTOWORLD );
					return;
				}
				else
				{
					FAIL_VALUE(byPortalFail, 8);
				}				*/
			}

			if ( pUser->GetPortalType() != Portal_Type_GM && pUser->GetCurDungeonID() == pPacket->wToDungeonID	&& pUser->GetCurLayerIndex() == pPacket->bLayerNo )
				FAIL_VALUE(byPortalFail, 6);				
						
			if (pDungeon->GetDungeonDataEx()->IsConquer() && !pDungeon->GetDungeonDataEx()->IsDungeonOwner(pUser) )
			{
				if (pUser->GetPortalType() == Portal_Type_GM)
				{
					// GM �� ������ �ҷ� �±� ������ ������ ����.
				}
				else if (pUser->m_dwMoney >= pDungeon->GetDungeonDataEx()->GetEntrancePay())
				{
					// ����� ��ƶ�.
					pUser->m_dwMoney -= pDungeon->GetDungeonDataEx()->GetEntrancePay();
					pDungeon->GetDungeonDataEx()->SetAccEntrance(pDungeon->GetDungeonDataEx()->GetEntrancePay());
				}
				else
				{
					FAIL_VALUE(byPortalFail, 17);					
				}
			}

			// ��Ż �̵����̶�� Flag
			pUser->m_bPortalMoving = 1;	
			
			if(pUser->GetCurDungeonID() == pPacket->wToDungeonID)	
			{
				// ���� ���������� �������� �̵��϶�(�����̵�)
				
				// ������ ���̾�� ������� �ϴ� ó���� �ϰ� (Remove)
				pUser->ChangeGodMode(GODMODE_STATUS_MAPLOADING);
				pUser->GetCurDungeon()->RemoveUserForInsidePortal(pUser);

				UpdateUserDBThread(pUser);

				// �������������� ��Ż�̵���(�������� �̵��ϴ� ��Ż�� ��� ���� ����),
				// ������ ��Ż�̵����� ���� ó���� �Ѵ�. 
				pUser->GetCurDungeon()->SendPortalMsg( pUser, pPacket->bLayerNo, &pPacket->v2Pos, FALSE );
			}
			else		
			{
				// ���� �����̳� �ٸ����������� �̵��϶� 

				// �ٸ� Ŭ���̾�Ʈ �鿡�� �� ������ ������� �޽����� ������.
				DSTC_PORTAL_DISAPPEAR DisAppear;
				DisAppear.dwUserIndex	= pUser->GetID();
				DisAppear.bType			= 1;
				pLayer->BroadCastSectionMsg( (char*)&DisAppear, DisAppear.GetPacketSize(), pUser );
				
				UpdateUserForAI(pUser);
				UpdateUserDBThread(pUser);

				pDungeon->SendPortalMsg(pUser, pPacket->bLayerNo, &pPacket->v2Pos, TRUE);			
				// ���� �������� ������ ���� �ϰ� 
				// ��Ż�� ��ũ�� ���� �ʾ� ������ �߻��Ͽ� ��ġ�� ������ 2005.02.14 �迵��
				// pDungeon->SendPortalMsg(pUser, pPacket->bLayerNo, &pPacket->v2Pos, TRUE); ������ ��������Ʈ���� ����
				pUser->GetCurDungeon()->RemoveUser( pUser );
				pUser->SetCurDungeon(NULL);
			}
		}
		else	
		{
			// ��Ʈ�� Ʋ�� ����.�� ���������� �Ҵ�� ������ �ƴϸ� ����� ����� ���� 

			if(pUser->pBufForPortal)
			{
				// Portal ó�� ��ġ�� ���� �� �ѹ� �� ��� ����...
				LALFree(g_pPortalBufferPool, pUser->pBufForPortal );
				pUser->pBufForPortal = NULL;
				
				FAIL_VALUE(byPortalFail, 5);				
			}

			// ��Ż �̵����̶�� Flag
			pUser->m_bPortalMoving = 1;	

			// ���� ��û�� ������ ������ ���� PORTAL_BUCKET�� �����ϰ� ����..
			PORTAL_BUCKET* pPortal =  (PORTAL_BUCKET*)LALAlloc(g_pPortalBufferPool);	
			memset(pPortal, 0, sizeof(PORTAL_BUCKET));	
			
			pPortal->wPortalItemID	= pPacket->wPortalItemID;
			pPortal->bLayerNo		= pPacket->bLayerNo;
			pPortal->wToDungeonID	= pPacket->wToDungeonID;
			pPortal->v2Pos			= pPacket->v2Pos;
			pUser->pBufForPortal	= pPortal;
					
			// �ش� ���������� �� ������ ���� ������ ��û 
			DSWDS_REQUEST_USER_ACCEPT_FOR_PORTAL	packet;	
			packet.wFromDungeonID			= pUser->GetCurDungeonID();
			packet.wDungeonID				= pPacket->wToDungeonID;
			packet.bLayerIndex				= pPacket->bLayerNo;
			packet.dwUserIndex				= pUser->GetID();
			packet.dwUserMoney				= pUser->m_dwMoney;
			packet.dwUserLevel				= pUser->GetLevel();
			packet.bConnectionToGameRoom	= pUser->m_bConnectToGameRoom;
			packet.dwTargetUserIndex		= pPacket->dwTargetUserIndex;
			packet.bPortalType				= pPacket->bPortalType;
			g_pNet->SendToServer(WSINDEX, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}
	}
	__finally
	{
		// 5: "��Ż �̵� �߿��� ����� �� �����ϴ�."
		// 6: "���� ���� ���� ������ ��Ż�� ����� �� �����ϴ�."
		// 7: "�׾������� �̵��� �� �����ϴ�."
		// 8: "�̵��Ϸ��� ������ �������� ������ �Դϴ�. �����߿��� �̵��� �� �����ϴ�."
		// 9: "������ �����ʾ� ������ �� �����ϴ�."
		// 15: "�̺�Ʈ ���������� ��Ż����� �̿��� �� �����ϴ�."
		// 16: "�̺�Ʈ ������ �������� ���, ��Ż����� �̿��� �� �����ϴ�."
		// 17: "����ᰡ �����մϴ�."
		if(byPortalFail)
		{
			SendPortalFailMessage(pUser, pPacket->wToDungeonID, byPortalFail, pPacket->wPortalItemID);
		}
	}
}


void CmdRequestSectionInfoForPortal( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	if (!pUser)	return;

	CTDS_REQUEST_SECTION_INFO_FOR_PORTAL* pPacket = (CTDS_REQUEST_SECTION_INFO_FOR_PORTAL*)pMsg;
	
	CDungeon* pDungeon = (CDungeon*)g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if(!pDungeon)
	{
		Log(LOG_IMPORTANT, "Unknown DungeonID Received...  at CmdRequestSectionInfoForPortal() (DungeonID:%d)"
			, pPacket->wDungeonID);
		return;
	}

	CDungeonLayer*	pLayer = pDungeon->GetDungeonLayer( pPacket->bLayerIndex );
	if (!pLayer)	return;

	// ������ �ٲ�� ��� 
	if(pPacket->bJoinDungeon)
	{
		pDungeon->JoinUserForPortal(pUser, pPacket->bLayerIndex);
	}
	
	// ���� ��Ÿ�� ��ġ�� ������ �ֳ�?
	VECTOR2 v2Start = *pUser->GetReservPotalPosition();
	MAP_TILE* pTile = pLayer->GetMap()->GetTile( v2Start.x, v2Start.y );
	
	if(!pTile)
	{
		Log(LOG_IMPORTANT, "Invalid User Position at CmdRequestSectionInfoForPortal() (Name:%s)",pUser->m_szName );
		pUser->CallBackCompulsiveDisconnectUser();
		return;
	}

	if (pTile->wAttr.uOccupied)
	{
		v2Start = pLayer->GetAroundEmptyTile(pTile->wIndex_X, pTile->wIndex_Z);
	}

	// �ش� ������ �߰� 
	pLayer->InsertUser( pUser,  &v2Start);
	pUser->ChangeGodMode(GODMODE_STATUS_MAPLOADING);

	if (pDungeon->GetDungeonDataEx()->m_bSiege)
	{
		pDungeon->SendSiegeInfo(pUser);
		pUser->SetAttackMode(pDungeon->GetAttackMode(pUser));
	}
		
	// ��Ż �̵����� Flag Off
	pUser->m_bPortalMoving = 0;		

	// �̵��ϴ� ���ΰ�, �ٸ� �ֺ� �����鿡�� �ش缽�ǰ�, ��ũ���� ������ ��� ���� 
	pUser->GetCurDungeon()->SendJoinSectionInfo( pUser , TRUE);
	{
		DSTC_RESPONSE_PORTAL_JOIN_USER packet;	
		packet.vecStart		= *pUser->GetCurPosition();
		packet.dwUserIndex	= pUser->GetID();

		pLayer->BroadCastSectionMsg((char*)&packet, packet.GetPacketSize(), pUser->GetPrevSectionNum());		
	}

	if (pUser->GetCurLayerIndex()	&&
		pUser->IsCurDungeonSiege()	&&
		pUser->GetAttackMode() != ATTACK_MODE_DEFENCE &&
		!pUser->m_dwTemp[USER_TEMP_FLAG_LOBBY])
	{	
		pUser->GetCurDungeon()->GotoLobbyUser(pUser);
	}
	else if (!pUser->GetCurLayerIndex() && pUser->IsCurDungeonSiege())
	{
		pUser->m_dwTemp[USER_TEMP_FLAG_LOBBY] = 1;	
	}
	
	if (pUser->m_sPKDescInfo.m_bPKCount | pUser->m_sPKDescInfo.m_bPKRepeatCount)
	{
		pUser->m_sPKDescInfo.m_pPKList = g_pThis->m_pPKUserCheckList->AddTail(pUser);
	}
}


void CmdPickupItem( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_ITEM_PICKUP*	pItemPickup = (CTDS_ITEM_PICKUP*)pMsg;
	DSTC_GUILD_ITEM		pPacket;
	DSTC_ITEM_PICKUP	PickupItem;
	DSTC_SHOP_FAIL		pShopPacket;
	DSTC_DISAPPEAR_ITEM	DisappearItem;
	ITEM_SERVER*		pItem			= NULL;
	CDungeonLayer*		pLayer			= NULL;
	CItem				ItemLog;
	BOOL				bAppear			= TRUE;
	int					i				= 0;

	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	
	// ��Ƽ�� ī���� �й� : 2005.1.26
	CUser* pMember[MAX_PARTY_USER] = {NULL, };
	WORD wMemberCount = 0;

	if(!pUser)						return;
	if(!pUser->GetCurDungeon())		return;	//��Ż �̵��� 
	
	pLayer = pUser->GetCurDungeonLayer();
	
	if(!pLayer)						return;		
	if(pUser->m_bPortalMoving==1)	return;	
	if(pUser->m_byRest==1)			return;
	if(pItemPickup->bSectionNum==0)	return;	
	if(CheckShop(pUser))			return;

	if(!(pItemPickup->bInv>=57 && pItemPickup->bInv<=60))
		pUser->m_dwPlayserShop = 0;

	memset( &ItemLog, 0, sizeof( CItem ) );
	memset( &PickupItem, 0, sizeof( DSTC_ITEM_PICKUP ) );
		
	PickupItem.bInv		= pItemPickup->bInv;
	PickupItem.bStatus	= UPDATE_GAME_PLAY;
	PickupItem.bHeader	= Protocol_DSTC::CMD_ITEM_PICKUP;

	if(pUser->GetPartyID())
	{
		// ���� ���� ��Ƽ���� ��� ���Ѵ� : �ִ��� 2005.1.26
		Node<CUser>* pnode = pUser->GetCurDungeon()->GetPartyUserHash()->GetBucketHead(pUser->GetPartyID());
		while(pnode)
		{
			if(pnode->pData->GetCurDungeon()
			&& pnode->pData->GetUnitStatus() != UNIT_STATUS_DEAD)
			{
				if(pnode->pData->GetCurDungeonLayer() == pUser->GetCurDungeonLayer()
					&& pnode->pData->GetPartyID() == pUser->GetPartyID())
				{
					pMember[wMemberCount++] = pnode->pData;
				}
			}

			pnode = pnode->pNext;
		}
	}

	switch( pItemPickup->bInv )
	{
	case 1:		// �ٴڿ��� �ٷ� ��� ���������� �̵��� �� //
		{			

			if(!pLayer->GetSection(pItemPickup->bSectionNum))
				return;

			pItem = pLayer->GetSection(pItemPickup->bSectionNum)
						->m_pItemHash->GetData(pItemPickup->i64ItemID);

			if(!pItem || pItem->Item.m_wItemID==0)
				return;

			if(SetIsCopyItem(pUser, &PickupItem, &pItem->Item, 1, DEF_COPYITEM_TYPE_LARGE))
				goto lb_exit1;

			int nValue = GetItemIndex(&pItem->Item);
			
			if(nValue<0)
				return;
			if(GetItemType(pItem->Item.GetID())==ITEM_TYPE_SMALL)
				return;

			// ��ƼǮ ��� �������̰� �����ڰ� ��Ƽ���̸�
			if(pItem->dwPartyIndex != 0 && pUser->GetPartyID() == pItem->dwPartyIndex)
			{
				// ���� ����
			}
			// ���� �������̸�
			else if(pItem->dwOwnerIndex)
			{
				// ������ �����ڰ� �ƴϸ�
				if(pItem->dwOwnerIndex!=pUser->GetID())
				{
					// ���� �Ұ�
					return;	
				}
			}
														
			if(GetItemType(pItem->Item.GetID())==ITEM_TYPE_MONEY)
			{
				__int64 nValueMon;
				__int64 nValueItemMon;

				DWORD dwMoney = pItem->Item.m_Item_Money.dwMoney;

				if(pUser->GetPartyID() && wMemberCount > 1)
				{
					// ��Ƽ���� ���� ī������ ���� : �ִ��� 2005.1.26
					memcpy( &PickupItem.Item, &pItem->Item, sizeof( CItem ) );

					PickupItem.Item.m_Item_Money.dwMoney /= wMemberCount;

					for(int m=0;m<wMemberCount;m++)
					{
						// �ٸ� ����鿡�� ���� ����

						if(pMember[m] == pUser)
							continue;

						nValueMon		= pMember[m]->m_dwMoney;
						nValueItemMon	= PickupItem.Item.m_Item_Money.dwMoney;

						if(nValueMon+nValueItemMon>__MAX_MONEY__)
						{
							pMember[m]->m_dwMoney	= __MAX_MONEY__;
							PickupItem.dwMoney	= __MAX_MONEY__;					
							PickupItem.bZipCode	= 1;
						}
						else
						{
							pMember[m]->m_dwMoney += nValueItemMon;
							PickupItem.dwMoney	= pMember[m]->m_dwMoney;					
							PickupItem.bZipCode	= 0;
						}

						NetSendToUser( pMember[m]->GetConnectionIndex(), (char*)&PickupItem, PickupItem.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

						// ������ �α� �߰� �迵�� 2005.02.23
						SetItemLogKarz(ITEM_LOG_KARZ_PICKUP,pLayer->GetParent()->GetID(),
										pMember[m]->m_szName,NULL,PickupItem.Item.m_Item_Money.dwMoney);

						dwMoney -= PickupItem.Item.m_Item_Money.dwMoney;
					}
				}

				// ������ ���� ��� ��۷�����
				PickupItem.Item.m_Item_Money.dwMoney = dwMoney;
				pItem->Item.m_Item_Money.dwMoney = dwMoney;

				nValueMon		= pUser->m_dwMoney;
				nValueItemMon	= PickupItem.Item.m_Item_Money.dwMoney;

				if(nValueMon+nValueItemMon>__MAX_MONEY__)
				{
					pUser->m_dwMoney = __MAX_MONEY__;
					PickupItem.dwMoney	= __MAX_MONEY__;					
					PickupItem.bZipCode	= 1;
				}
				else
				{
					pUser->m_dwMoney += nValueItemMon;
					PickupItem.dwMoney	= pUser->m_dwMoney;					
					PickupItem.bZipCode	= 0;
				}

				// ������ �α� �߰� �迵�� 2005.02.23
				//SetItemLogKarz(ITEM_LOG_KARZ_PICKUP,pLayer->GetParent()->GetID(),
				//				pUser->m_szName,NULL,PickupItem.Item.m_Item_Money.dwMoney);
				// �Ʒ��� �α�ó���� ���� ��.

			}
			else if(GetItemType(pItem->Item.GetID())==ITEM_TYPE_LARGE)
			{
				//��Ƽ�� �������� �й�
				if(pUser->GetPartyID()
				&& wMemberCount > 1
				&& pUser->m_PartyConfig.item == PARTY_CONFIG_ITEM_RANDOM)
				{
                    pUser = pMember[ rand() % wMemberCount ];

					// ū �κ��丮 ĭ�� ����
					for(int zip=0;zip<MAX_INV_LARGE_POOL;zip++)
					{
						// �������
						if(pUser->m_pInv_Large[zip].m_wItemID==0)
							break;
					}

					// �κ��� ���� ����
					if(zip == MAX_INV_LARGE_POOL)
					{
						// ���� ���ϰ� �ٸ� ������� ��ȸ�� ����
						return;
					}

					pItemPickup->bZipCode = zip;
				}

#if defined TAIWAN_LOCALIZING
				// �븸�� ���� ���� 100% �̻�� �������� ������ �����Ѵ�.
				if (FALSE == pUser->IsPickUpItem())
					return;					
#endif

				if(pUser->m_pInv_Large[ pItemPickup->bZipCode ].m_wItemID==0)
				{					
					PickupItem.bZipCode = pItemPickup->bZipCode;
					memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode], &pItem->Item, sizeof(CItem));

					// ������ �α� �߰� �迵�� 2005.02.23
					SetItemLog(&PickupItem.Item,ITEM_LOG_PICKUP	, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
				}
				else
					return;
			}
			goto lb_exit1;
		}
		break;

	case 2:		// �ٴڿ��� ���� ���������� �̵��� ��.
		{
			if(!pLayer->GetSection( pItemPickup->bSectionNum))
				return;

			pItem = pLayer->GetSection(pItemPickup->bSectionNum)->m_pItemHash->GetData(pItemPickup->i64ItemID);
			
			if(!pItem)
				return;

			if(SetIsCopyItem(pUser, &PickupItem, &pItem->Item, 2, DEF_COPYITEM_TYPE_SMALL))
				goto lb_exit1;

			if(!CheckItem(&pItem->Item, ITEM_TYPE_SMALL))
				return;
			
			if(pItem->Item.GetQuantity()==0)
				return;

			// ��ƼǮ ��� �������̰� �����ڰ� ��Ƽ���̸�
			if(pItem->dwPartyIndex != 0 && pUser->GetPartyID() == pItem->dwPartyIndex)
			{
				// ���� ����
			}
			// ���� �������̸�
			else if(pItem->dwOwnerIndex)
			{
				// ������ �����ڰ� �ƴϸ�
				if(pItem->dwOwnerIndex!=pUser->GetID())
				{
					// ���� �Ұ�
					return;	
				}
			}

			// ��Ƽ�� ���� ���� : �ִ��� 2005.1.26
			if(pUser->GetPartyID() && wMemberCount > 1 && pUser->m_PartyConfig.item == PARTY_CONFIG_ITEM_RANDOM)
			{
				pUser = pMember[ rand() % wMemberCount ];

				// ���� : �ٸ� �����鿡�� �޼����� ������
				// "���� �� � �Ծ����ϴ�."
			}

			//��Ƽ�� �������� �й�
			if(pUser->GetPartyID()
			&& wMemberCount > 1
			&& pUser->m_PartyConfig.item == PARTY_CONFIG_ITEM_RANDOM)
			{
				pUser = pMember[ rand() % wMemberCount ];

				// ��ĭ�� �ִ�� ���� �� �ִ� ���� ���Ѵ�
				CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pItem->Item.m_wItemID);
				if(!pBaseItem)
					return;
				int nMaxQuantity = pBaseItem->GetMaxQuantity();

				// ���� �κ��丮 ĭ�� ����
				for(int zip=0;zip<MAX_INV_SMALL_POOL;zip++)
				{
					// ���� �� ������
					if(pUser->m_pInv_Small[zip].m_wItemID==0
					|| (pUser->m_pBelt[zip].m_wItemID == pItem->Item.m_wItemID
						&& pUser->m_pInv_Small[zip].GetQuantity() + pItem->Item.GetQuantity() < nMaxQuantity))
					{
						break;
					}
				}

				// �κ��� ��������
				if(zip == MAX_INV_SMALL_POOL)
				{
					// ���� ���ϰ� �ٸ� ������� ��ȸ�� ����
					return;
				}

				pItemPickup->bZipCode = zip;
			}

#if defined TAIWAN_LOCALIZING
			// �븸�� ���� ���� 100% �̻�� �������� ������ �����Ѵ�.
			if (FALSE == pUser->IsPickUpItem()) 	
				return;
#endif

			BYTE byCnt = 
				(BYTE)Insert_SmallInvItem(pUser, &pItem->Item, pItemPickup->bZipCode, pItem->Item.GetQuantity(), FALSE);

			if(byCnt>0)
			{
				pItem->Item.SetQuantity(byCnt);
				bAppear = FALSE;
			}
			
			CoypPickupMsg(&PickupItem, &pItem->Item, 2, pItemPickup->bZipCode);	

			// ������ �α� �߰� �迵�� 2005.02.23
			SetItemLog(&PickupItem.Item,ITEM_LOG_PICKUP	, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );

			goto lb_exit1;
		}
		break;
	case 3:		// ���콺���� ��Ʈ�� ���� �������� �̵��� �� //
		{	
			if(!CheckItem(&pUser->m_ItemMouse, ITEM_TYPE_SMALL))
				return;			

			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 3, DEF_COPYITEM_TYPE_SMALL))
				goto lb_exit2;
						
			if(pUser->m_ItemMouse.GetQuantity()==0)
				return;
							
			PickupItem.bZipCode	= pItemPickup->bZipCode;
			memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

			Insert_SmallBeltItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, pUser->m_ItemMouse.GetQuantity(),  FALSE);
		
			memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
			goto lb_exit2;
		}
		break;
	case 4:		// ���콺���� �ٴ����� ���� �������� �̵��� �� //
		{
			if(!CheckItem(&pUser->m_ItemMouse, ITEM_TYPE_SMALL))
				return;
						
			if(pUser->m_ItemMouse.GetQuantity()==0)
				return;

			//sung-han 2005-03-15 �ŷ�, ���, �����ʵǴ� ������ ó��, ����� ���..
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if( pBaseItem && pBaseItem->GetMovable() != 0 )
			{
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

				Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), TRUE);

				PickupItem.bInv		= 14;
				PickupItem.bZipCode	= 0;
								
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit2;
			}
			
			AppearItem(pUser, &pUser->m_ItemMouse, pUser->m_ItemMouse.GetQuantity(), 0, ITEM_TYPE_SMALL);

			PickupItem.bEquipCode	= 0;			
			PickupItem.bZipCode		= 0;
			memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
			goto lb_exit2;
		}
		break;
	case 5:		// �ٴڿ��� ���콺�� �̵��� �� //
		{
			if(!pLayer->GetSection( pItemPickup->bSectionNum ))
				return;

			pItem = pLayer->GetSection( pItemPickup->bSectionNum )->m_pItemHash->GetData(pItemPickup->i64ItemID);
			
			if(!pItem)
				return;

#if defined TAIWAN_LOCALIZING
				// �븸�� ���� ���� 100% �̻�� �������� ������ �����Ѵ�.				
				if (FALSE == pUser->IsPickUpItem())
					return;
#endif

			// ��Ƽ�� ��� �����۰� ��Ƽ ������ �����й��
			if(pItem->dwPartyIndex && pUser->m_PartyConfig.item == PARTY_CONFIG_ITEM_RANDOM)
			{
				// ������ �巹�� �Ұ�
				return;
			}

			// ��ƼǮ ��� �������̰� �����ڰ� ��Ƽ���̸�
			if(pItem->dwPartyIndex != 0 && pUser->GetPartyID() == pItem->dwPartyIndex)
			{
				// ���� ����
			}
			// ���� �������̸�
			else if(pItem->dwOwnerIndex)
			{
				// ������ �����ڰ� �ƴϸ�
				if(pItem->dwOwnerIndex!=pUser->GetID())
				{
					// ���� �Ұ�
					return;	
				}
			}

			PickupItem.bZipCode	= 0;
			memcpy( &pUser->m_ItemMouse, &pItem->Item, sizeof( CItem ) );
			goto lb_exit1;
		}
		break;			
	case 6:		// ���콺���� ��� ���������� �̵��� �� //
		{

			if(!CheckItem(&pUser->m_ItemMouse, ITEM_TYPE_LARGE))
				return;
			
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 6, DEF_COPYITEM_TYPE_LARGE))
				goto lb_exit2;
						
			if(pUser->m_pInv_Large[ pItemPickup->bZipCode ].m_wItemID==0)
			{				
				PickupItem.bZipCode = pItemPickup->bZipCode;
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
				memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode], &pUser->m_ItemMouse, sizeof(CItem));
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit2;
			}			
		}
		break;		
	case 7:		// ��� �����ۿ��� ���콺�� �̵��� �� //
		{
			if(!CheckItem(&pUser->m_pInv_Large[pItemPickup->bZipCode], ITEM_TYPE_LARGE))
				return;
			
			if(pUser->m_ItemMouse.m_wItemID==0)
			{				
				PickupItem.bZipCode	= pItemPickup->bZipCode;
				memcpy(&PickupItem.Item, &pUser->m_pInv_Large[pItemPickup->bZipCode], sizeof(CItem));
				memcpy(&pUser->m_ItemMouse, &pUser->m_pInv_Large[pItemPickup->bZipCode], sizeof(CItem));
				memset(&pUser->m_pInv_Large[pItemPickup->bZipCode], 0, sizeof(CItem));
				goto lb_exit2;
			}
		}
		break;
	case 8: // ������ ����������.
		{
			if(pUser->GetUnitStatus()!=UNIT_STATUS_NORMAL)
				return;					

			int nVal = GetItemIndex(&pUser->m_ItemMouse);

			if(!CheckItem(&pUser->m_ItemMouse, ITEM_TYPE_LARGE))
				return;			

			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 8, DEF_COPYITEM_TYPE_LARGE, TRUE))
				goto lb_exit2;

			// üũ Ŭ���� //
			CBaseItem * pItem = GetBaseItem(pUser->m_ItemMouse.GetID());
			if(! pItem ) return;


			// ���� ������ �߰� 
			// ���뷹���� ���� ������ ������ �����Ҽ� ����.
			// 2005.01.20 by �迵�� 
			if ( (pItem->wID >= BAG_ITEM_NUM_FRIST && pItem->wID <= BAG_ITEM_NUM_LAST) && 
				 ( pUser->GetLevel() <  pItem->BaseItem_Bag.wMin_Lev  ) )
			{
				
				PickupItem.bInv        = pItemPickup->bInv;
				PickupItem.dwMoney     =  3;					
				PickupItem.bZipCode		= pItemPickup->bZipCode;					
				PickupItem.dwId			= pItemPickup->dwId;
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
				goto lb_exit2;
			}

			BOOL bChk	= FALSE;
			BOOL bClass = pUser->CheckItem(&pUser->m_ItemMouse);
							
			if(!bClass)
			{
				for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
				{
					if(pUser->m_pInv_Large[i].GetID()==0)
					{
						PickupItem.bZipCode = i;
						bChk = TRUE;
						memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
						break;
					}
				}
				if(bChk)
				{
					PickupItem.dwMoney	= 1;					
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				}
				else
				{
					PickupItem.dwMoney	= 2;					
					AppearItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), ITEM_TYPE_LARGE);
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));						
				}						
				goto lb_exit2;
			}

			// Check //				
			if(nVal>=0 && nVal<=7)
			{					
				if((nVal>=2 && nVal<=5) || nVal==7)
				{
					// ���ڵ� //
					PickupItem.bEquipCode	= pItemPickup->bEquipCode;					
					PickupItem.bZipCode		= 0;						
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

					memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));														

					if(PickupItem.bEquipCode==EQUIP_TYPE_RHAND1)
					{								
						if(pUser->m_pEquip[EQUIP_TYPE_LHAND1].m_wItemID!=0)
						{
							BOOL bChk = FALSE;

							for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
							{
								if(pUser->m_pInv_Large[i].m_wItemID==0)
								{
									PickupItem.bZipCode	= i;
									bChk = TRUE;
									break;
								}
							}

							if(bChk==TRUE)
							{
								memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode], &pUser->m_pEquip[EQUIP_TYPE_LHAND1], sizeof(CItem));
								memset(&pUser->m_pEquip[EQUIP_TYPE_LHAND1], 0, sizeof(CItem));
							}
							else
							{
								AppearItem(pUser, &pUser->m_pEquip[EQUIP_TYPE_LHAND1], 0, 0, ITEM_TYPE_LARGE);
								memset(&pUser->m_pEquip[EQUIP_TYPE_LHAND1], 0, sizeof(CItem));
							}
						}
					}
					else if(PickupItem.bEquipCode==EQUIP_TYPE_RHAND2)
					{
						if(pUser->m_pEquip[EQUIP_TYPE_LHAND2].m_wItemID!=0)
						{
							BOOL bChk = FALSE;

							for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
							{
								if(pUser->m_pInv_Large[i].m_wItemID==0)
								{
									PickupItem.bZipCode	= i;
									bChk = TRUE;
									break;
								}
							}

							if(bChk==TRUE)
							{
								memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode], &pUser->m_pEquip[EQUIP_TYPE_LHAND2], sizeof(CItem));
								memset(&pUser->m_pEquip[EQUIP_TYPE_LHAND2], 0, sizeof(CItem));										
							}
							else
							{
								AppearItem(pUser, &pUser->m_pEquip[EQUIP_TYPE_LHAND2], 0, 0, ITEM_TYPE_LARGE);
								memset(&pUser->m_pEquip[EQUIP_TYPE_LHAND2], 0, sizeof(CItem));
							}
						}
					}
				}
				else
				{
					// ���ڵ� //
					PickupItem.bEquipCode	= pItemPickup->bEquipCode;					
					PickupItem.bZipCode		= 0;					
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
					memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				}
			}
			else if(nVal==12)
			{
				PickupItem.bEquipCode	= pItemPickup->bEquipCode;				
				PickupItem.bZipCode		= 0;
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
				memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));

				if(PickupItem.bEquipCode==EQUIP_TYPE_LHAND1)
				{								
					if(pUser->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID!=0)
					{
						int nVal = pUser->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID/ITEM_DISTRIBUTE;

						if((nVal>=2 && nVal<=5) || nVal==7)
						{
							BOOL bChk = FALSE;

							for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
							{
								if(pUser->m_pInv_Large[i].m_wItemID==0)
								{
									PickupItem.bZipCode	= i;
									bChk = TRUE;
									break;
								}
							}

							if(bChk==TRUE)
							{
								memcpy(	&pUser->m_pInv_Large[PickupItem.bZipCode],&pUser->m_pEquip[EQUIP_TYPE_RHAND1], sizeof(CItem));
								memset(&pUser->m_pEquip[EQUIP_TYPE_RHAND1], 0, sizeof(CItem));
							}
							else
							{
								AppearItem(pUser, &pUser->m_pEquip[EQUIP_TYPE_RHAND1], 0, 0, ITEM_TYPE_LARGE);
								memset(&pUser->m_pEquip[EQUIP_TYPE_RHAND1], 0, sizeof(CItem));
							}
						}
					}
				}
				else if(PickupItem.bEquipCode==EQUIP_TYPE_LHAND2)
				{
					if(pUser->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID!=0)
					{
						int nVal = pUser->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID/ITEM_DISTRIBUTE;

						if((nVal>=2 && nVal<=5) || nVal==7)
						{
							BOOL bChk = FALSE;

							for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
							{
								if(pUser->m_pInv_Large[i].m_wItemID==0)
								{
									PickupItem.bZipCode	= i;
									bChk = TRUE;
									break;
								}
							}

							if(bChk==TRUE)
							{
								memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode], &pUser->m_pEquip[EQUIP_TYPE_RHAND2], sizeof(CItem));
								memset(&pUser->m_pEquip[EQUIP_TYPE_RHAND2], 0, sizeof(CItem));										
							}
							else
							{
								AppearItem(pUser, &pUser->m_pEquip[EQUIP_TYPE_RHAND2], 0, 0, ITEM_TYPE_LARGE);
								memset(&pUser->m_pEquip[EQUIP_TYPE_RHAND2], 0, sizeof(CItem));
							}
						}
					}
				}
			}
			else
			{
				if(!CheckEquip((BYTE)nVal, pItemPickup->bEquipCode))
				{
					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[ i ].m_wItemID==0)
						{							
							PickupItem.bInv		= 6;
							PickupItem.bZipCode = i;
							memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
							memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
							memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
							goto lb_exit2;
						}
					}
					return;
				}

				PickupItem.bEquipCode	= pItemPickup->bEquipCode;				
				PickupItem.bZipCode		= 0;				
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
				memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
			}
			
			// ������ �������� ����ϰ� �뺸.
			pUser->SendAllStatus();

			// Mod ��ü �޼��� //
			if( ((PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE) >= 0 && (PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE) <=7 )
				|| PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE==12)
			{
				if(pUser->m_bCurrentHand==1)
					ItemHandMod(pUser, pUser->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID, pUser->m_pEquip[EQUIP_TYPE_LHAND1].m_wItemID);
				else
					ItemHandMod(pUser, pUser->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID, pUser->m_pEquip[EQUIP_TYPE_LHAND2].m_wItemID);
			}
			else if((PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE)==11)
			{
				ItemArmorMod(pUser, pUser->m_pEquip[EQUIP_TYPE_MAIL].m_wItemID);
			}
			else if((PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE)==10)
			{
				ItemHelmetMod(pUser, pUser->m_pEquip[EQUIP_TYPE_HELMET].m_wItemID);
			}

			goto lb_exit2;
		}
		break;
	case 9:	// �������� �������.
		{
			if(pUser->GetUnitStatus()!=UNIT_STATUS_NORMAL)
				return;

			else if(pUser->m_ItemMouse.m_wItemID==0)
				return;

			else if((pUser->m_ItemMouse.m_wItemID/ITEM_DISTRIBUTE)>MAX_LARGE_ITEM_INDEX)
				return;
						
			else if(GetItemType(pUser->m_ItemMouse.GetID())!=ITEM_TYPE_LARGE)
				return;

			else if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 9, DEF_COPYITEM_TYPE_LARGE, TRUE))
				goto lb_exit2;


			// â���� �������� �̵��� �������� ���� �������°��� ���� 
			// 2005.01.25 �迵�� 
			BOOL bChk = FALSE;
			
			for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
			{
				if(pUser->m_pInv_Large[i].GetID()==0)
				{
					bChk = TRUE;
					break;
				}
			}

			if ( bChk == FALSE)
			{
				PickupItem.bInv        = pItemPickup->bInv;
				PickupItem.dwMoney     =  4;					
				PickupItem.bZipCode		= pItemPickup->bZipCode;					
				PickupItem.dwId			= pItemPickup->dwId;
				PickupItem.bEquipCode	= 2;
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
				goto lb_exit2;
			}


			PickupItem.dwMoney	= 0;

			if(pUser->m_ItemMouse.m_wItemID!=0)
			{	
				// üũ Ŭ���� //
				BOOL bChk = FALSE;

				if(!GetBaseItem(pUser->m_ItemMouse.GetID()))
					return;

				BOOL bClass = pUser->CheckItem(&pUser->m_ItemMouse);					
				
				if(!bClass)
				{						
					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].GetID()==0)
						{
							PickupItem.bZipCode = i;
							bChk = TRUE;
							memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
							break;
						}
					}

					if(bChk)
					{
						PickupItem.dwMoney		= 1;						
						memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					}
					else
					{
						PickupItem.dwMoney		= 2;						
						memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
						AppearItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), ITEM_TYPE_LARGE);
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));							
					}						
					goto lb_exit2;
				}

				int nVal = pUser->m_ItemMouse.m_wItemID/ITEM_DISTRIBUTE;
									
				if(nVal>=0 && nVal<=7)
				{
					if((nVal>=2 && nVal<=5) || nVal==7)
					{						
						PickupItem.bEquipCode	= pItemPickup->bEquipCode;						
						PickupItem.bZipCode		= 0;
						memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

						BOOL bChk = FALSE;

						for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
						{
							if(pUser->m_pInv_Large[i].m_wItemID==0)
							{
								PickupItem.bZipCode	= i;
								bChk = TRUE;
								break;
							}
						}

						if(bChk==TRUE)
						{
							memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode], &pUser->m_pEquip[PickupItem.bEquipCode], sizeof(CItem));								
							memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
							memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
						}
						else
						{
							AppearItem(pUser, &pUser->m_pEquip[PickupItem.bEquipCode], 0, 0, ITEM_TYPE_LARGE);
							// Item //												
							memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
							memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
						}

						// ���� ������ //
						if(PickupItem.bEquipCode==EQUIP_TYPE_RHAND1)
						{								
							if(pUser->m_pEquip[EQUIP_TYPE_LHAND1].m_wItemID!=0)
							{
								BOOL bChk = FALSE;

								for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
								{
									if(pUser->m_pInv_Large[i].m_wItemID==0)
									{
										PickupItem.bZipCode2	= i;
										bChk = TRUE;
										break;
									}
								}

								if(bChk==TRUE)
								{
									memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode2], &pUser->m_pEquip[EQUIP_TYPE_LHAND1], sizeof(CItem));
									memset(&pUser->m_pEquip[EQUIP_TYPE_LHAND1], 0, sizeof(CItem));
								}
								else
								{
									AppearItem(pUser, &pUser->m_pEquip[EQUIP_TYPE_LHAND1], 0, 0, ITEM_TYPE_LARGE);
									// Item //																	
									memset(&pUser->m_pEquip[EQUIP_TYPE_LHAND1], 0, sizeof(CItem));
								}
							}
						}
						else if(PickupItem.bEquipCode==EQUIP_TYPE_RHAND2)
						{
							if(pUser->m_pEquip[EQUIP_TYPE_LHAND2].m_wItemID!=0)
							{
								BOOL bChk = FALSE;

								for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
								{
									if(pUser->m_pInv_Large[i].m_wItemID==0)
									{
										PickupItem.bZipCode2	= i;
										bChk = TRUE;
										break;
									}
								}

								if(bChk==TRUE)
								{
									memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode2], &pUser->m_pEquip[EQUIP_TYPE_LHAND2], sizeof(CItem));
									memset(&pUser->m_pEquip[EQUIP_TYPE_LHAND2], 0, sizeof(CItem));										
								}
								else
								{
									AppearItem(pUser, &pUser->m_pEquip[EQUIP_TYPE_LHAND2], 0, 0, ITEM_TYPE_LARGE);
									// Item //																	
									memset(&pUser->m_pEquip[EQUIP_TYPE_LHAND2], 0, sizeof(CItem));
								}
							}
						}
					}
					else
					{
						PickupItem.bEquipCode	= pItemPickup->bEquipCode;						
						PickupItem.bZipCode		= 0;
						memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

						BOOL bChk = FALSE;

						for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
						{
							if(pUser->m_pInv_Large[i].m_wItemID==0)
							{
								PickupItem.bZipCode	= i;
								bChk = TRUE;
								break;
							}
						}

						if(bChk==TRUE)
						{
							memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode], &pUser->m_pEquip[PickupItem.bEquipCode], sizeof(CItem));								
							memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
							memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
						}
						else
						{
							return;
							AppearItem(pUser, &pUser->m_pEquip[PickupItem.bEquipCode], 0, 0, ITEM_TYPE_LARGE);
							// Item //												
							memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
							memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
						}
					}
				}
				else if(nVal==12)
				{
					PickupItem.bEquipCode	= pItemPickup->bEquipCode;					
					PickupItem.bZipCode		= 0;
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

					BOOL bChk = FALSE;

					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].m_wItemID==0)
						{
							PickupItem.bZipCode	= i;
							bChk = TRUE;
							break;
						}
					}

					if(bChk==TRUE)
					{
						memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode], &pUser->m_pEquip[PickupItem.bEquipCode], sizeof(CItem));
						memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					}
					else
					{
						AppearItem(pUser, &pUser->m_pEquip[PickupItem.bEquipCode], 0, 0, ITEM_TYPE_LARGE);
						// Item //												
						memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					}

					// ���� ������ //
					if(PickupItem.bEquipCode==EQUIP_TYPE_LHAND1)
					{								
						if(pUser->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID!=0)
						{
							int nVal = pUser->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID/ITEM_DISTRIBUTE;

							if((nVal>=2 && nVal<=5) || nVal==7)
							{
								BOOL bChk = FALSE;

								for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
								{
									if(pUser->m_pInv_Large[i].m_wItemID==0)
									{
										PickupItem.bZipCode2	= i;
										bChk = TRUE;
										break;
									}
								}

								if(bChk==TRUE)
								{
									memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode2], &pUser->m_pEquip[EQUIP_TYPE_RHAND1], sizeof(CItem));
									memset(&pUser->m_pEquip[EQUIP_TYPE_RHAND1], 0, sizeof(CItem));
								}
								else
								{
									AppearItem(pUser, &pUser->m_pEquip[EQUIP_TYPE_RHAND1], 0, 0, ITEM_TYPE_LARGE);
									// Item //																	
									memset(&pUser->m_pEquip[EQUIP_TYPE_RHAND1], 0, sizeof(CItem));
								}
							}
						}
					}
					else if(PickupItem.bEquipCode==EQUIP_TYPE_LHAND2)
					{
						if(pUser->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID!=0)
						{
							int nVal = pUser->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID/ITEM_DISTRIBUTE;

							if((nVal>=2 && nVal<=5) || nVal==7)
							{
								BOOL bChk = FALSE;

								for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
								{
									if(pUser->m_pInv_Large[i].m_wItemID==0)
									{
										PickupItem.bZipCode2	= i;
										bChk = TRUE;
										break;
									}
								}

								if(bChk==TRUE)
								{
									memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode2], &pUser->m_pEquip[EQUIP_TYPE_RHAND2], sizeof(CItem));
									memset(&pUser->m_pEquip[EQUIP_TYPE_RHAND2], 0, sizeof(CItem));
								}
								else
								{
									AppearItem(pUser, &pUser->m_pEquip[EQUIP_TYPE_RHAND2], 0, 0, ITEM_TYPE_LARGE);
									// Item //																	
									memset(&pUser->m_pEquip[EQUIP_TYPE_RHAND2], 0, sizeof(CItem));
								}
							}
						}
					}						
				}
				else
				{
					if(!CheckEquip((BYTE)nVal, pItemPickup->bEquipCode))
					{
						for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
						{
							if(pUser->m_pInv_Large[ i ].m_wItemID==0)
							{							
								PickupItem.bInv		= 6;
								PickupItem.bZipCode = i;
								memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
								memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
								memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
								goto lb_exit2;
							}
						}
						return;
					}

					BOOL bChk = FALSE;						

					PickupItem.bEquipCode	= pItemPickup->bEquipCode;					
					PickupItem.bZipCode		= 0;
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
					
					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].m_wItemID==0)
						{
							PickupItem.bZipCode	= i;
							bChk = TRUE;
							break;
						}
					}

					if(bChk==TRUE)
					{
						memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode], &pUser->m_pEquip[PickupItem.bEquipCode], sizeof(CItem));
						memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					}
					else
					{
						AppearItem(pUser, &pUser->m_pEquip[PickupItem.bEquipCode], 0, 0, ITEM_TYPE_LARGE);
						memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					}
				}
				
				// ������ �������� ����ϰ� �뺸.
				pUser->SendAllStatus();
			}

			// Mod ��ü �޼��� //
			if(((PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE)>=0 && (PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE)<=7) ||
				PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE==12)
			{
				if(pUser->m_bCurrentHand==1)
				{
					ItemHandMod(pUser,
						pUser->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID, pUser->m_pEquip[EQUIP_TYPE_LHAND1].m_wItemID);
				}
				else
				{
					ItemHandMod(pUser,
						pUser->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID, pUser->m_pEquip[EQUIP_TYPE_LHAND2].m_wItemID);
				}
			}
			else if((PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE)==11)
			{
				ItemArmorMod(pUser, pUser->m_pEquip[EQUIP_TYPE_MAIL].m_wItemID);
			}
			else if((PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE)==10)
			{
				ItemHelmetMod(pUser, pUser->m_pEquip[EQUIP_TYPE_HELMET].m_wItemID);
			}

			goto lb_exit2;
		}
		break;
	case 10:		// �������� �ٴ����� �̵���ų ��.
		{						
			if(!CheckItem(&pUser->m_ItemMouse, ITEM_TYPE_LARGE))
				return;

			//sung-han 2005-03-15 �ŷ�, ���, �����ʵǴ� ������ ó��, ����� ���..
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if( pBaseItem && pBaseItem->GetMovable() != 0 )
			{
				for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
				{
					if(pUser->m_pInv_Large[ i ].m_wItemID==0)
					{
						PickupItem.bInv		= 6;
						PickupItem.bZipCode = i;
						memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
						memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
						goto lb_exit2;
					}
				}
				return;
			}
			
			memcpy(&ItemLog, &pUser->m_ItemMouse, sizeof(CItem));

			AppearItem(pUser
				, &pUser->m_ItemMouse
				, 0
				, 0
				, 0
				, pItemPickup->fItemX
				, 0
				, pItemPickup->fItemZ);
			
			PickupItem.bEquipCode	= 0;			
			PickupItem.bZipCode		= 0;
			memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
			goto lb_exit2;			
		}
		break;
	case 11:		// ���������� ���Կ��� ���콺�� �̵�.
		{
			int nValue = GetItemIndex(&pUser->m_pEquip[pItemPickup->bEquipCode]);
								
			if(!CheckItem(&pUser->m_pEquip[pItemPickup->bEquipCode], ITEM_TYPE_LARGE))
				return;
						
			else if(pUser->m_ItemMouse.m_wItemID!=0)
				return;			
			else if(pUser->GetUnitStatus()!=UNIT_STATUS_NORMAL)
				return;

			PickupItem.bEquipCode	= pItemPickup->bEquipCode;			
			PickupItem.bZipCode		= 0;

							
			memcpy(&PickupItem.Item, &pUser->m_pEquip[pItemPickup->bEquipCode], sizeof(CItem));
			memcpy(&pUser->m_ItemMouse, &pUser->m_pEquip[PickupItem.bEquipCode], sizeof(CItem));
			memset(&pUser->m_pEquip[PickupItem.bEquipCode], 0, sizeof(CItem));
			
			// ������ �������� ����ϰ� �뺸.
			pUser->SendAllStatus();
			// Mod ��ü �޼��� //
			nValue = PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE;

			if((nValue>=0 && nValue<=7) || nValue==12)
			{
				if(pUser->m_bCurrentHand==1)
					ItemHandMod(pUser, pUser->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID, pUser->m_pEquip[EQUIP_TYPE_LHAND1].m_wItemID);
				else
					ItemHandMod(pUser, pUser->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID, pUser->m_pEquip[EQUIP_TYPE_LHAND2].m_wItemID);
			}
			else if(nValue==11)
				ItemArmorMod(pUser, pUser->m_pEquip[EQUIP_TYPE_MAIL].m_wItemID);
			else if(nValue==10)
				ItemHelmetMod(pUser, pUser->m_pEquip[EQUIP_TYPE_HELMET].m_wItemID);


			goto lb_exit2;			
		}
		break;
	case 12:		// ���� �������� �ٴڿ��� ���콺�� �̵��� ��.
		{
			if(!pLayer->GetSection(pItemPickup->bSectionNum))
				return;

			pItem = pLayer->GetSection(pItemPickup->bSectionNum)->m_pItemHash->GetData(pItemPickup->i64ItemID);

			if(!pItem)
				return;
			
			// ��Ƽ�� ��� �������̰� ��Ƽ ������ �����й��̸�
			if(pItem->dwPartyIndex && pUser->m_PartyConfig.item == PARTY_CONFIG_ITEM_RANDOM)
			{
				// ���콺 �巡�� �Ұ�
				return;
			}
			
			// ��ƼǮ ��� �������̰� �����ڰ� ��Ƽ���̸�
			if(pItem->dwPartyIndex != 0 && pUser->GetPartyID() == pItem->dwPartyIndex)
			{
				// ���� ����
			}
			// ���� �������̸�
			else if(pItem->dwOwnerIndex)
			{
				// ������ �����ڰ� �ƴϸ�
				if(pItem->dwOwnerIndex!=pUser->GetID())
				{
					// ���� �Ұ�
					return;	
				}
			}
			
//			int nValue = GetItemIndex(&pItem->Item);

			if(!CheckItem(&pItem->Item, ITEM_TYPE_SMALL))
				return;
			
			if(pItem->Item.GetQuantity()==0)
				return;
							
			PickupItem.bZipCode	= 0;					
			memcpy(&PickupItem.Item, &pItem->Item, sizeof(CItem));
			memcpy(&pUser->m_ItemMouse, &pItem->Item, sizeof(CItem));
			goto lb_exit1;			
		}
		break;
	case 13:		// ���� ���� ��.
		{


#ifdef JAPAN_LOCALIZING		//��Ŭ�� ��û���� �Ϻ��� ���� ����߸��� �ʴ´� .. 2005.02.05 �迵��

			PickupItem.dwMoney						=	pUser->m_dwMoney;
			PickupItem.Item.m_wItemID				=	__ITEM_KARZ__;
			PickupItem.Item.m_Item_Money.dwMoney	=	pItemPickup->dwMoney;
			goto lb_exit2;
#else
			if(pUser->m_dwMoney<pItemPickup->dwMoney)
				return;

			AppearItem(pUser, NULL, 0, pItemPickup->dwMoney, ITEM_TYPE_MONEY);		
							
			pUser->m_dwMoney						-=	pItemPickup->dwMoney;			
			PickupItem.dwMoney						=	pUser->m_dwMoney;
			PickupItem.Item.m_wItemID				=	__ITEM_KARZ__;
			PickupItem.Item.m_Item_Money.dwMoney	=	pItemPickup->dwMoney;


			goto lb_exit2;
#endif
		}
		break;
	case 14:		// ���콺���� �Һ� â���� ���� �������� �̵��� ��.
		{	
//			int nValue = GetItemIndex(&pUser->m_ItemMouse);


			if(!CheckItem(&pUser->m_ItemMouse, ITEM_TYPE_SMALL))
				return;
			
			if(pUser->m_ItemMouse.GetQuantity()==0)
				return;
			
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 14, DEF_COPYITEM_TYPE_SMALL, TRUE))
				goto lb_exit2;				
			
			memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

			Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, pUser->m_ItemMouse.GetQuantity(), TRUE);
			
			PickupItem.bZipCode	= pItemPickup->bZipCode;
							
			memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
			goto lb_exit2;			
		}
		break;
	case 15:		// �Һ� â���� ���콺�� ���� �������� �̵��� ��.
		{
			int nValue = GetItemIndex(&pUser->m_pInv_Small[pItemPickup->bZipCode]);

			if(!CheckItem(&pUser->m_pInv_Small[pItemPickup->bZipCode], ITEM_TYPE_SMALL))
				return;
			
			if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()==0)
				return;
			
			if(nValue>=ITEM_CONSUMABLEST_INDEX && nValue<=ITEM_CONSUMABLEEND_INDEX)
			{
				if(pUser->m_pInv_Small[pItemPickup->bZipCode].m_Item_Consumable.bInvIndex>=1)
					return;
			}	
			
			// ������ ���� ���� �Ǿ��ִ���?? ����Ǿ� ������ ���콺�� �������� ����. 
			if(pUser->m_pInv_Small[pItemPickup->bZipCode].m_wItemID == __ITEM_INSURANCE_INDEX__)
			{
				if(pUser->m_pInv_Small[pItemPickup->bZipCode].m_Item_Zodiac.bInvIndex >=1 )
					return;
			}

			if(pUser->m_ItemMouse.m_wItemID == 0)
			{				
				PickupItem.bZipCode	= pItemPickup->bZipCode;
				memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));
				memcpy(&pUser->m_ItemMouse, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));
				memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
				goto lb_exit2;
			}
		}
		break;
	case 16:		// �Һ� �������� ��Ʈ���� ���콺�� �̵��� ��.
		{
//			int nValue = GetItemIndex(&pUser->m_pBelt[pItemPickup->bZipCode]);
			
			if(!CheckItem(&pUser->m_pBelt[pItemPickup->bZipCode], ITEM_TYPE_SMALL))
				return;
			
			if(pUser->m_pBelt[pItemPickup->bZipCode].GetQuantity()==0)
				return;
			
			if(pUser->m_ItemMouse.m_wItemID==0)
			{				
				PickupItem.bZipCode	= pItemPickup->bZipCode;
				memcpy(&PickupItem.Item, &pUser->m_pBelt[pItemPickup->bZipCode], sizeof(CItem));
				memcpy(&pUser->m_ItemMouse, &pUser->m_pBelt[pItemPickup->bZipCode], sizeof(CItem));
				memset(&pUser->m_pBelt[pItemPickup->bZipCode], 0, sizeof(CItem));
				goto lb_exit2;
			}
		}
		break;
	case 17:		// Switch Weapon �޼���.
		{							
			if(pUser->m_bCurrentHand==1)// LEFT
			{
				pUser->m_bCurrentHand	= 2;
				PickupItem.bZipCode		= 2;
			}
			else	// RIGHT
			{
				pUser->m_bCurrentHand	= 1;
				PickupItem.bZipCode		= 1;
			}					
			
			// ������ �������� ����ϰ� �뺸.
			pUser->SendAllStatus();
																	
			// Mod ��ü �޼��� //
			if(pUser->m_bCurrentHand==1)
			{
				ItemHandMod(pUser, pUser->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID
					, pUser->m_pEquip[EQUIP_TYPE_LHAND1].m_wItemID);
			}
			else
			{
				ItemHandMod(pUser, pUser->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID
					, pUser->m_pEquip[EQUIP_TYPE_LHAND2].m_wItemID);
			}
							
			goto lb_exit2;
		}
		break;
	case 18:		// ������ ������ �������� �� ��.(�Ҹ�ǰ)
		{	
			if(!CheckItem(&pUser->m_ItemMouse, ITEM_TYPE_SMALL))
				return;			

			else if(pUser->m_ItemMouse.GetQuantity()==0)
				return;
			
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 18, DEF_COPYITEM_TYPE_SMALL, TRUE))
				goto lb_exit2;
						
			CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(pUser->m_ItemMouse.m_wItemID);
			
			if(!pBaseItem)
				return;
			
			__int64 nValueMon		= pUser->m_dwMoney;
			__int64 nValuePackMon	= pItemPickup->dwMoney;
			
			PickupItem.bEquipCode		= 1;
			PickupItem.dwId				= pUser->m_ItemMouse.m_wItemID;
			PickupItem.Item.m_wItemID	= pUser->m_ItemMouse.m_wItemID;
			PickupItem.bZipCode			= 0;
								
			__int64 nValuePri = pBaseItem->GetSellPrice();
										
			if(nValueMon+(nValuePri*nValuePackMon)>__MAX_MONEY__)
			{
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

				Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, pUser->m_ItemMouse.GetQuantity(), TRUE);

				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit2;
			}								

			memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

			if(pUser->m_ItemMouse.GetQuantity()!=pItemPickup->dwMoney)
			{
				if(pUser->m_ItemMouse.GetQuantity()<pItemPickup->dwMoney)
					return;

				BYTE byPackCnt = BYTE(pUser->m_ItemMouse.GetQuantity()-pItemPickup->dwMoney);

				pUser->m_ItemMouse.SetQuantity(byPackCnt);
				Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, byPackCnt, TRUE);					
			}																
			
			pUser->m_dwMoney +=	pBaseItem->GetSellPrice()*pItemPickup->dwMoney;					
			
			PickupItem.bEquipCode	=	0;
			PickupItem.dwMoney		=	pUser->m_dwMoney;					
			PickupItem.bZipCode		=	pItemPickup->bZipCode;
			PickupItem.bZipCode2	=	BYTE(pItemPickup->dwMoney);
			
			memcpy( &ItemLog, &pUser->m_ItemMouse, sizeof( CItem ) );
			memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
			goto lb_exit2;
		}
		break;
	case 19:		// ������ ������ �������� �� ��.
		{
			CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(pItemPickup->dwId);

			if(!pBaseItem)
				break;

			if(GetItemType(WORD(pItemPickup->dwId))==ITEM_TYPE_LARGE)
				return;				

			if(!CheckStore(pItemPickup->dwId))
				return;
			
//			BOOL		bChk		= FALSE;				
//			int			nValue		= pBaseItem->wID/ITEM_DISTRIBUTE;
			__int64		nMonValue	= 0;
			
			nMonValue = pItemPickup->dwMoney * pBaseItem->GetPrice();				

			if((pUser->m_dwMoney>=nMonValue) && pItemPickup->dwMoney!=0)
			{
				CItem cItem;
				CreateItem(&cItem, WORD(pItemPickup->dwId), BYTE(pItemPickup->dwMoney));

				BYTE byPackCnt = (BYTE)Insert_SmallInvItem(pUser, &cItem, pItemPickup->bZipCode, cItem.GetQuantity(), TRUE, FALSE);
				
				memcpy(&PickupItem.Item, &cItem, sizeof(CItem));
				PickupItem.Item.SetQuantity(BYTE(pItemPickup->dwMoney-byPackCnt));
								
				PickupItem.bZipCode		= pItemPickup->bZipCode;
				PickupItem.dwId			= pItemPickup->dwId;				
				PickupItem.bEquipCode	= BYTE(pItemPickup->dwMoney);
				PickupItem.bZipCode2	= 0;

				if(byPackCnt!=0)
				{
					pUser->m_dwMoney	-=	pBaseItem->GetPrice()*(pItemPickup->dwMoney-byPackCnt);												
					PickupItem.dwMoney	=	pUser->m_dwMoney;
				}
				else
				{
					pUser->m_dwMoney	-=	pBaseItem->GetPrice()*pItemPickup->dwMoney;												
					PickupItem.dwMoney	=	pUser->m_dwMoney;
				}					
			}
			else
			{				
				PickupItem.bZipCode2	= 1;
				PickupItem.dwId			= pItemPickup->dwId;
			}
			goto lb_exit2;
		}
		break;		
	case 24:		// ���콺���� â���(��� ������).
		{
//			int nValue = GetItemIndex(&pUser->m_ItemMouse);


			if(!CheckItem(&pUser->m_ItemMouse, ITEM_TYPE_LARGE))
				return;
			
			if(pUser->m_pBank_Large[pItemPickup->bZipCode].GetID()!=0)
				return;
						
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 24, DEF_COPYITEM_TYPE_SMALL, TRUE))
				goto lb_exit2;

			//sung-han 2005-03-15 �ŷ�, ���, �����ʵǴ� ������ ó��, ����� â��..
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if( pBaseItem && pBaseItem->GetMovable() == 2 )
			{
				for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
				{
					if(pUser->m_pInv_Large[ i ].m_wItemID==0)
					{
						PickupItem.bInv		= 6;
						PickupItem.bZipCode = i;
						memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
						memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
						goto lb_exit2;
					}
				}
			}
			
			PickupItem.bZipCode	=	pItemPickup->bZipCode;
			memcpy(&pUser->m_pBank_Large[pItemPickup->bZipCode], &pUser->m_ItemMouse, sizeof(CItem));
			memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
			memset(&pUser->m_ItemMouse, 0, sizeof(CItem));

			// ������ �α� �߰� �迵�� 2005.02.23
			SetItemLog(&pUser->m_pBank_Large[pItemPickup->bZipCode],ITEM_LOG_BANK_IN, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );

			goto lb_exit2;
		}
		break;
	case 25:		// ���콺���� â���(�Һ� ������).
		{
//			int nValue = GetItemIndex(&pUser->m_ItemMouse);


			if(!CheckItem(&pUser->m_ItemMouse, ITEM_TYPE_SMALL))
				return;
						
			if(pItemPickup->bZipCode>=MAX_BANK_SMALL_POOL)
				return;

			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 25, DEF_COPYITEM_TYPE_SMALL, TRUE))
				goto lb_exit2;

			//sung-han 2005-03-15 �ŷ�, ���, �����ʵǴ� ������ ó��, ����� â��..
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if( pBaseItem && pBaseItem->GetMovable() == 2 )
			{
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

				Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), TRUE);

				PickupItem.bInv		= 14;
				PickupItem.bZipCode	= 0;
								
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit2;
			}
			
			memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
							
			if(pUser->m_ItemMouse.GetQuantity() >= pItemPickup->dwMoney)
			{
				BYTE	byPacketCount	= BYTE(pItemPickup->dwMoney);	
				BYTE	byOutCount		= BYTE(pUser->m_ItemMouse.GetQuantity()-pItemPickup->dwMoney);
									
				if(byPacketCount == pUser->m_ItemMouse.GetQuantity())
				{
					Insert_SmallBankItem(pUser
						, &pUser->m_ItemMouse
						, pItemPickup->bZipCode
						, pUser->m_ItemMouse.GetQuantity());
				}
				else
				{
					CItem cBankItem;

					memcpy(&cBankItem, &pUser->m_ItemMouse, sizeof(CItem));
					cBankItem.SetQuantity(BYTE(pItemPickup->dwMoney));
					cBankItem.SetSerialNum(g_pThis->GetNewSerial());
					
					Insert_SmallBankItem(pUser, &cBankItem, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));
					
					if(byOutCount!=0)
					{
						pUser->m_ItemMouse.SetQuantity(byOutCount);
						Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, byOutCount, TRUE);
					}
				}																			
				PickupItem.dwMoney	=	pItemPickup->dwMoney;
				PickupItem.bZipCode	=	pItemPickup->bZipCode;					
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				// ������ �α� �߰� �迵�� 2005.02.23
				SetItemLog(&PickupItem.Item,ITEM_LOG_BANK_IN, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );

				goto lb_exit2;
			}				
		}
		break;
	case 26:		// â���� ���콺��(��� ������).
		{
			if(pUser->m_nItemTradeSeriel!=0)
				return;
			
//			int nValue = GetItemIndex(&pUser->m_pBank_Large[pItemPickup->bZipCode]);

			if(!CheckItem(&pUser->m_pBank_Large[pItemPickup->bZipCode], ITEM_TYPE_LARGE))
				return;
			
			if(pUser->m_ItemMouse.GetID()!=0)
				return;
						
			PickupItem.bZipCode	=	pItemPickup->bZipCode;
			memcpy(&pUser->m_ItemMouse, &pUser->m_pBank_Large[pItemPickup->bZipCode], sizeof(CItem));
			memcpy(&PickupItem.Item, &pUser->m_pBank_Large[pItemPickup->bZipCode], sizeof(CItem));
			memset(&pUser->m_pBank_Large[pItemPickup->bZipCode], 0, sizeof(CItem));

			// ������ �α� �߰� �迵�� 2005.02.23
			SetItemLog(&PickupItem.Item,ITEM_LOG_BANK_OUT, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );

			goto lb_exit2;
		}
		break;
	case 27:		// â���� ���콺��(�Һ� ������).
		{
			if(pUser->m_nItemTradeSeriel!=0)
				return;
			
//			int nValue = GetItemIndex(&pUser->m_pBank_Small[pItemPickup->bZipCode]);

			if(!CheckItem(&pUser->m_pBank_Small[pItemPickup->bZipCode], ITEM_TYPE_SMALL))
				return;
			
			if(pUser->m_ItemMouse.GetID()!=0)
				return;
			
			PickupItem.bZipCode	=	pItemPickup->bZipCode;
			memcpy(&pUser->m_ItemMouse, &pUser->m_pBank_Small[pItemPickup->bZipCode], sizeof(CItem));				
			memcpy(&PickupItem.Item, &pUser->m_pBank_Small[pItemPickup->bZipCode], sizeof(CItem));
			memset(&pUser->m_pBank_Small[pItemPickup->bZipCode], 0, sizeof(CItem));

			// ������ �α� �߰� �迵�� 2005.02.23
			SetItemLog(&PickupItem.Item,ITEM_LOG_BANK_OUT, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );

			goto lb_exit2;
		}
		break;
	case 28:		// ���콺���� ������� �ٴ����� ������ ��(�Һ� ������).
		{
//			BOOL	bChk		= FALSE;
//			int		nValue		= pUser->m_ItemMouse.m_wItemID/ITEM_DISTRIBUTE;
			BYTE	byPacketCnt	= BYTE(pUser->m_ItemMouse.GetQuantity() - pItemPickup->dwMoney);
			BYTE	byOutCnt	= BYTE(pItemPickup->dwMoney);

			if(GetItemType(pUser->m_ItemMouse.GetID())!=ITEM_TYPE_SMALL)
				return;

			//sung-han 2005-03-15 �ŷ�, ���, �����ʵǴ� ������ ó��, ����� ���..
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if( pBaseItem && pBaseItem->GetMovable() != 0 )
			{
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

				Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), TRUE);

				PickupItem.bInv		= 14;
				PickupItem.bZipCode	= 0;
								
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit2;
			}
						
			PickupItem.dwMoney	= pItemPickup->dwMoney;
			memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

			if(pUser->m_ItemMouse.GetQuantity()==0)
				return;
			
			if(byPacketCnt<0)
				return;

			if(pUser->m_ItemMouse.GetQuantity() < pItemPickup->dwMoney)
			{
				if(GetItemType(pUser->m_ItemMouse.m_wItemID)==ITEM_TYPE_SMALL)
				{
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

					Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), TRUE);
					
					PickupItem.bZipCode	= 0;
									
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					goto lb_exit2;
				}
				return;
			}

			if(byPacketCnt==0)
			{
				pUser->m_ItemMouse.SetQuantity(byOutCnt);
				AppearItem(pUser, &pUser->m_ItemMouse, BYTE(pItemPickup->dwMoney), 0, 0, pItemPickup->fItemX, 0, pItemPickup->fItemZ);
				memcpy(&ItemLog, &pUser->m_ItemMouse, sizeof(CItem));	//kukza 041004
			}
			else
			{
				pUser->m_ItemMouse.SetQuantity(byPacketCnt);
				byPacketCnt = (BYTE)Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, byPacketCnt, TRUE, FALSE);
									
				if(byPacketCnt!=0)
					byOutCnt += byPacketCnt;
					
				if(byOutCnt!=0)
				{
					CItem cInsertITem;
					memcpy(&cInsertITem, &pUser->m_ItemMouse, sizeof(CItem));
					cInsertITem.SetSerialNum(g_pThis->GetNewSerial());
					cInsertITem.SetQuantity(byOutCnt);
					AppearItem(pUser, &cInsertITem, byOutCnt, 0, 0, pItemPickup->fItemX, 0, pItemPickup->fItemZ);
					memcpy(&ItemLog, &cInsertITem, sizeof(CItem));		//kukza 041004
				}
			}

			// ������ �α� �߰� �迵�� 2005.02.23 �Ʒ����� �α� ����
			//SetItemLog(&pUser->m_ItemMouse,ITEM_LOG_DROP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );

			memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
			goto lb_exit2;
		}
		break;		
	case 30:// ��� �����Ҷ� 
		{			
			if(pUser->m_byGroupType==__GCTYPE_NONE__)
			{	
				BOOL bChk				= FALSE;				
				PickupItem.bZipCode2	= pItemPickup->bZipCode2;

				// ������ ���� //	
				if(pItemPickup->bZipCode2==__GCTYPE_GUILD__)
				{
					if(pUser->m_pInv_Small[pItemPickup->bZipCode].m_wItemID==ITEM_GUILD_INDEX)
					{
						BYTE bQuantity = pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity();

						if(bQuantity>1)
						{
							pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(BYTE(bQuantity-1));
							PickupItem.bZipCode	= pItemPickup->bZipCode;
							memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));
						}
						else
						{
							memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
							PickupItem.bZipCode	= pItemPickup->bZipCode;
							memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));
						}
						bChk = TRUE;
					}
					else
					{
						for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
						{
							if(pUser->m_pInv_Small[i].m_wItemID==ITEM_GUILD_INDEX)
							{
								BYTE bQuantity = pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity();

								if(bQuantity>1)
								{
									pUser->m_pInv_Small[i].SetQuantity(BYTE(bQuantity-1));
									PickupItem.bZipCode	= i;
									memcpy(&PickupItem.Item, &pUser->m_pInv_Small[i], sizeof(CItem));
								}
								else
								{
									memset(&pUser->m_pInv_Small[i], 0, sizeof(CItem));
									PickupItem.bZipCode	= i;
									memcpy(&PickupItem.Item, &pUser->m_pInv_Small[i], sizeof(CItem));
								}
								bChk = TRUE;
								break;
							}
						}
					}
				}
				else if(pItemPickup->bZipCode2==__GCTYPE_CLEN__)
				{
					if(pUser->m_pInv_Small[pItemPickup->bZipCode].m_wItemID==ITEM_CLEN_INDEX)
					{
						BYTE bQuantity = pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity();

						if(bQuantity>1)
						{
							pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(BYTE(bQuantity-1));
							PickupItem.bZipCode	= pItemPickup->bZipCode;
							memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));
						}
						else
						{
							memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
							PickupItem.bZipCode	= pItemPickup->bZipCode;
							memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));
						}
						bChk = TRUE;
					}
					else
					{
						for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
						{
							if(pUser->m_pInv_Small[i].m_wItemID==ITEM_CLEN_INDEX)
							{
								BYTE bQuantity = pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity();

								if(bQuantity>1)
								{
									pUser->m_pInv_Small[i].SetQuantity(BYTE(bQuantity-1));
									PickupItem.bZipCode	= i;
									memcpy(&PickupItem.Item, &pUser->m_pInv_Small[i], sizeof(CItem));
								}
								else
								{
									memset(&pUser->m_pInv_Small[i], 0, sizeof(CItem));
									PickupItem.bZipCode	= i;
									memcpy(&PickupItem.Item, &pUser->m_pInv_Small[i], sizeof(CItem));
								}
								bChk = TRUE;
							}							
						}
					}
				}					
				
				if(bChk==TRUE)
				{											
					goto lb_exit2;
				}
				else

				{
					// ������ ���� ���� //
					pPacket.bResult		= FALSE;
					pPacket.bZipCode	= 3;
					NetSendToUser(dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	
				}
			}
			else
			{					
				pPacket.bResult	= FALSE;

				if(pUser->m_byGroupType==__GCTYPE_GUILD__)
					pPacket.bZipCode = __GCTYPE_GUILD__;
				else
					pPacket.bZipCode = __GCTYPE_CLEN__;				

				NetSendToUser(dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	
			}
		}
		break;
	case 32:		// �����ۿ��� ��ũ�� ���̵�.
		{
			if(pItemPickup->dwMoney<=pUser->m_dwMoney)
			{
				__int64 nValueBankMon	= pUser->m_dwBankMoney;
				__int64 nValueMon		= pItemPickup->dwMoney;

				if(nValueBankMon+nValueMon>__MAX_MONEY__)
				{
					DWORD dwMoney = __MAX_MONEY__ - pUser->m_dwBankMoney;

					pUser->m_dwMoney		-= dwMoney;
					pUser->m_dwBankMoney	+= dwMoney;

					// ������ �α� �߰� �迵�� 2005.02.23
					SetItemLogKarz(	ITEM_LOG_BANK_KARZ_IN,pLayer->GetParent()->GetID(),pUser->m_szName,NULL,dwMoney);
				}
				else
				{
					pUser->m_dwMoney		-= pItemPickup->dwMoney;
					pUser->m_dwBankMoney	+= pItemPickup->dwMoney;
					// ������ �α� �߰� �迵�� 2005.02.23
					SetItemLogKarz(	ITEM_LOG_BANK_KARZ_IN,pLayer->GetParent()->GetID(),pUser->m_szName,NULL,pItemPickup->dwMoney);
				}	
				
				PickupItem.dwMoney	= pUser->m_dwMoney;
				PickupItem.dwId		= pUser->m_dwBankMoney;										
				goto lb_exit2;
			}				
		}
		break;
	case 33:		// ��ũ���� ���������� ���̵�.
		{
			if(pItemPickup->dwMoney<=pUser->m_dwBankMoney)
			{
				__int64 nValueMon		= pUser->m_dwMoney;
				__int64 nValuePackMon	= pItemPickup->dwMoney;

				if(nValueMon+nValuePackMon>__MAX_MONEY__)
				{
					DWORD dwMoney = __MAX_MONEY__ - pUser->m_dwMoney;

					pUser->m_dwMoney		+= dwMoney;
					pUser->m_dwBankMoney	-= dwMoney;

					// ������ �α� �߰� �迵�� 2005.02.23
					SetItemLogKarz(	ITEM_LOG_BANK_KARZ_OUT,pLayer->GetParent()->GetID(),pUser->m_szName,NULL,dwMoney);
				}
				else
				{
					pUser->m_dwMoney		+= pItemPickup->dwMoney;
					pUser->m_dwBankMoney	-= pItemPickup->dwMoney;
					// ������ �α� �߰� �迵�� 2005.02.23
					SetItemLogKarz(	ITEM_LOG_BANK_KARZ_OUT,pLayer->GetParent()->GetID(),pUser->m_szName,NULL,pItemPickup->dwMoney);

				}									
				PickupItem.dwMoney	= pUser->m_dwMoney;
				PickupItem.dwId		= pUser->m_dwBankMoney;										
				goto lb_exit2;
			}
		}
		break;
	case 34:		// Weapon ������ ������ �� ��.
		{
			if(pUser->m_ItemMouse.m_wItemID==0)
				return;

			int nValue = GetItemIndex(&pUser->m_ItemMouse);
			
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 34, DEF_COPYITEM_TYPE_LARGE, TRUE))
				goto lb_exit2;
					
			if(nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_WEAPONEND_INDEX)				
			{
				BOOL		bChk		= FALSE;
				CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(pUser->m_ItemMouse.m_wItemID);

				if(!pBaseItem)
					break;
				
				__int64		nMoney		= pUser->m_dwMoney;
				__int64		nSellPrice	= pBaseItem->BaseItem_Weapon.dwSellPrice;

				if(nMoney+nSellPrice<=__MAX_MONEY__)
				{
					pUser->m_dwMoney		= DWORD(nMoney+nSellPrice); 
					PickupItem.dwMoney		= DWORD(nMoney+nSellPrice);					
					PickupItem.bEquipCode	= 0;						
					bChk = TRUE;
				}
				else
				{
					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].m_wItemID==0)
						{
							if(bChk==FALSE)
							{								
								PickupItem.bZipCode		= i;
								PickupItem.bEquipCode	= 1;

								memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
								memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
								bChk = TRUE;
							}
						}
					}
				
					if(bChk==FALSE)
					{
						AppearItem(pUser, &pUser->m_ItemMouse, 0, 0, ITEM_TYPE_LARGE);
						PickupItem.bEquipCode = 2;
					}
				}
				
				// ���콺���� ���� //
				memcpy( &ItemLog, &pUser->m_ItemMouse, sizeof( CItem ) );
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));

				if(bChk==TRUE)
					goto lb_exit2;
			}
		}
		break;
	case 35:		// Weapon ������ �� ��.
		{			
			int nValue = pItemPickup->dwId/ITEM_DISTRIBUTE;														
			
			if(nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_WEAPONEND_INDEX)				
			{
				BOOL		bChk		= FALSE;
				CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(pItemPickup->dwId);

				if(!pBaseItem)
					break;

				if(!CheckStore(pItemPickup->dwId))
					return;

				__int64		nMoney		= pUser->m_dwMoney;
				__int64		nPrice		= pBaseItem->BaseItem_Weapon.dwPrice;

				if(nMoney-nPrice>=0)
				{
					if(pUser->m_pInv_Large[pItemPickup->bZipCode].m_wItemID==0)
					{
						if(bChk==FALSE)
						{
							CreateItem(
								&pUser->m_pInv_Large[pItemPickup->bZipCode], 
								WORD(pItemPickup->dwId), 
								0, 
								Store_Flag_In_Store	
							);

							memcpy(&PickupItem.Item, &pUser->m_pInv_Large[pItemPickup->bZipCode], sizeof(CItem));
							pUser->m_dwMoney = DWORD(nMoney - nPrice);
							
							PickupItem.bEquipCode	= 1;
							PickupItem.bZipCode		= pItemPickup->bZipCode;
							PickupItem.dwId			= pItemPickup->dwId;
							PickupItem.dwMoney		= pUser->m_dwMoney;
							bChk = TRUE;
						}							
					}
					else
					{						
						for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
						{
							if(pUser->m_pInv_Large[i].m_wItemID==0)
							{
								CreateItem(
									&pUser->m_pInv_Large[i], 
									WORD(pItemPickup->dwId), 
									0,
									Store_Flag_In_Store
								);

								memcpy(&PickupItem.Item, &pUser->m_pInv_Large[i], sizeof(CItem));
								pUser->m_dwMoney = DWORD(nMoney - nPrice);
								bChk = TRUE;
								
								PickupItem.bZipCode		= i;
								PickupItem.bEquipCode	= 1;
								PickupItem.dwId			= pItemPickup->dwId;
								PickupItem.dwMoney		= pUser->m_dwMoney;
								break;
							}
						}
					}
				
					if(bChk==FALSE)
					{
						// ���� //						
						PickupItem.bEquipCode	= 2;
						PickupItem.dwId			= pItemPickup->dwId;
					}						
				}
				else
				{
					// ���� //					
					PickupItem.bEquipCode	= 3;
					PickupItem.dwId			= pItemPickup->dwId;
				}
				
				goto lb_exit2;
			}
		}
		break;
	case 36:		// Armor ������ ������ �� ��.
		{
			if(pUser->m_ItemMouse.m_wItemID==0)
				return;
			
			int nValue = pUser->m_ItemMouse.m_wItemID/ITEM_DISTRIBUTE;
			
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 36, DEF_COPYITEM_TYPE_LARGE, TRUE))
				goto lb_exit2;				

			if(nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX)
			{
				BOOL		bChk		= FALSE;
				CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(pUser->m_ItemMouse.m_wItemID);

				if(!pBaseItem)
					break;
				
				__int64		nMoney		= pUser->m_dwMoney;
				__int64		nSellPrice	= pBaseItem->BaseItem_Armor.dwSellPrice;

				if(nMoney+nSellPrice<=__MAX_MONEY__)
				{
					pUser->m_dwMoney		= DWORD(nMoney+nSellPrice); 
					PickupItem.dwMoney		= DWORD(nMoney+nSellPrice);					
					PickupItem.bEquipCode	= 0;
					bChk = TRUE;
				}
				else
				{
					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].m_wItemID==0)
						{
							if(bChk==FALSE)
							{								
								PickupItem.bZipCode		= i;
								PickupItem.bEquipCode	= 1;

								memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
								memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
								bChk = TRUE;
							}
						}
					}
				
					if(bChk==FALSE)
						AppearItem(pUser, &pUser->m_ItemMouse, 0, 0, ITEM_TYPE_LARGE);
				}
				
				// ���콺���� ���� //
				memcpy( &ItemLog, &pUser->m_ItemMouse, sizeof( CItem ) );
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));

				if(bChk==TRUE)
					goto lb_exit2;		
			}
		}
		break;
	case 37:		// Armor ������ �� ��.
		{				
			int nValue = pItemPickup->dwId/ITEM_DISTRIBUTE;	
			
			if((nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX) || (pItemPickup->dwId >= 3600 && pItemPickup->dwId <= 3618))				
			{
				BOOL		bChk		= FALSE;
				CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(pItemPickup->dwId);
				
				if(!pBaseItem)
					break;

				if(!CheckStore(pItemPickup->dwId))
					return;

				__int64		nMoney		= pUser->m_dwMoney;
				__int64		nPrice		= pBaseItem->BaseItem_Armor.dwPrice;

				if(nMoney-nPrice>=0)
				{
					if(pUser->m_pInv_Large[pItemPickup->bZipCode].m_wItemID==0)
					{
						CreateItem(
							&pUser->m_pInv_Large[pItemPickup->bZipCode], 
							WORD(pItemPickup->dwId), 
							0, 
							Store_Flag_In_Store
						);
						memcpy(&PickupItem.Item, &pUser->m_pInv_Large[pItemPickup->bZipCode], sizeof(CItem));
						pUser->m_dwMoney = DWORD(nMoney - nPrice);
						
						PickupItem.bEquipCode	= 1;
						PickupItem.bZipCode		= pItemPickup->bZipCode;
						PickupItem.dwId			= pItemPickup->dwId;
						PickupItem.dwMoney		= pUser->m_dwMoney;
						
						bChk = TRUE;
					}
					else
					{						
						for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
						{
							if(pUser->m_pInv_Large[i].m_wItemID==0)
							{
								if(bChk==FALSE)
								{
									CreateItem(
										&pUser->m_pInv_Large[i], 
										WORD(pItemPickup->dwId), 
										0,
										Store_Flag_In_Store
									);
									memcpy(&PickupItem.Item, &pUser->m_pInv_Large[i], sizeof(CItem));
									pUser->m_dwMoney = DWORD(nMoney - nPrice);
									bChk = TRUE;
									
									PickupItem.bZipCode		= i;
									PickupItem.bEquipCode	= 1;
									PickupItem.dwId			= pItemPickup->dwId;
									PickupItem.dwMoney		= pUser->m_dwMoney;
								}
							}
						}
					}
				
					if(bChk==FALSE)
					{
						// ���� //						
						PickupItem.bEquipCode	= 2;
						PickupItem.dwId			= pItemPickup->dwId;
					}						
				}
				else
				{
					// ���� //
					PickupItem.bEquipCode	= 3;
					PickupItem.dwId			= pItemPickup->dwId;
				}
				
				goto lb_exit2;
			}				
		}
		break;
	case 38:		// ���콺���� Guardian â���� Guardian ������ �� ��.
		{
			int nValue = GetItemIndex(&pUser->m_ItemMouse);

			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 38, DEF_COPYITEM_TYPE_SMALL, TRUE))
				goto lb_exit2;			

			if(nValue==ITEM_GUARDIAN_INDEX)
			{
				PickupItem.bZipCode	= pItemPickup->bZipCode;
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

				if(pUser->m_pInv_Guardian[PickupItem.bZipCode].m_wItemID==0)
				{
					memcpy(&pUser->m_pInv_Guardian[PickupItem.bZipCode], &pUser->m_ItemMouse, sizeof(CItem));
				}
				else
				{
					BOOL bChk = FALSE;

					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].m_wItemID==0)
						{
							memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
							PickupItem.bZipCode2 = i;								
							bChk = TRUE;
							break;
						}
					}

					if(bChk==FALSE)
					{
						AppearItem(pUser, &pUser->m_ItemMouse, 0, 0, ITEM_TYPE_LARGE);
					}
				}								
				
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit2;
			}
		}
		break;
	case 39:		// Guardian â���� ���콺�� Guardian �������� �� ��.
		{
			int nValue = GetItemIndex(&pUser->m_pInv_Guardian[pItemPickup->bZipCode]);

			if(nValue==ITEM_GUARDIAN_INDEX)
			{	
				pUser->UnSummonGuardian(0);
				
				PickupItem.bZipCode	= pItemPickup->bZipCode;
				memcpy(&PickupItem.Item, &pUser->m_pInv_Guardian[pItemPickup->bZipCode], sizeof(CItem));
				memcpy(&pUser->m_ItemMouse, &pUser->m_pInv_Guardian[pItemPickup->bZipCode], sizeof(CItem));
				memset(&pUser->m_pInv_Guardian[pItemPickup->bZipCode], 0, sizeof(CItem));
				goto lb_exit2;
			}
		}
		break;
	case 40:		// ���콺���� Guardian �������̽��� �Һ� �������� �� ��.
		{
			int		nValue		= GetItemIndex(&pUser->m_ItemMouse);
			BYTE	byPacketCnt	= 0;

			if(pUser->m_ItemMouse.GetQuantity()==0)
				return;
					
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 40, DEF_COPYITEM_TYPE_SMALL, TRUE))
				goto lb_exit2;
			
			if(nValue == ITEM_SUPPLIES_INDEX || (nValue >= ITEM_CONSUMABLEST_INDEX && nValue <= ITEM_CONSUMABLEEND_INDEX))
			{
				PickupItem.bZipCode	= 0;
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

				// ������ ������ 
				if(pUser->m_ItemGuardian.m_wItemID == 0)
				{
					memcpy(&pUser->m_ItemGuardian, &pUser->m_ItemMouse, sizeof(CItem));
				}
				else
				{
					if(pUser->m_ItemMouse.m_wItemID == pUser->m_ItemGuardian.m_wItemID)
					{
						CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemGuardian.m_wItemID);

						if(!pBaseItem)
							break;
						
						int nMaxQuantity	= 1;

						nMaxQuantity = pBaseItem->GetMaxQuantity();							

						int nItemQuantity	= nMaxQuantity - pUser->m_ItemGuardian.GetQuantity();
						int nItemMouse		= pUser->m_ItemMouse.GetQuantity();

						if(nItemQuantity>=nItemMouse)
						{
							pUser->m_ItemGuardian.SetQuantity(BYTE(pUser->m_ItemGuardian.GetQuantity()+nItemMouse));
						}
						else
						{
							pUser->m_ItemGuardian.SetQuantity(BYTE(pUser->m_ItemGuardian.GetQuantity()+nItemQuantity));
							byPacketCnt = BYTE(nItemMouse-nItemQuantity);
							
							for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
							{
								if(pUser->m_pInv_Small[i].m_wItemID==0)
								{						
									memcpy(&pUser->m_pInv_Small[i], &pUser->m_ItemMouse, sizeof(CItem));
									pUser->m_pInv_Small[i].SetQuantity(byPacketCnt);
									byPacketCnt = 0;
									break;
								}
								else if(pUser->m_pInv_Small[i].m_wItemID==pUser->m_ItemMouse.m_wItemID)
								{
									CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_pInv_Small[i].m_wItemID);

									if(!pBaseItem)
										break;

									int nMaxQuantity	= 1;

									nMaxQuantity = pBaseItem->GetMaxQuantity();										

									if(pUser->m_pInv_Small[i].GetQuantity()<nMaxQuantity)
									{
										int nCount = nMaxQuantity - pUser->m_pInv_Small[i].GetQuantity();

										if(nCount>=byPacketCnt)
										{
											pUser->m_pInv_Small[i].m_Item_Supplies.bQuantity+=byPacketCnt;
											byPacketCnt = 0;									
											break;
										}
										else
										{
											pUser->m_pInv_Small[i].m_Item_Supplies.bQuantity+=nCount;
											byPacketCnt -= nCount;
										}						
									}
								}									
							}

							if(byPacketCnt>0)
							{
								CItem cInsertItem;
								memcpy(&cInsertItem, &pUser->m_ItemMouse, sizeof(CItem));
								cInsertItem.SetQuantity(byPacketCnt);
								cInsertItem.SetSerialNum(g_pThis->GetNewSerial());
								AppearItem(pUser, &cInsertItem, byPacketCnt, 0, ITEM_TYPE_SMALL);
							}
						}
					}
					else
					{
						Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), TRUE);
					}
				}																										

				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit2;
			}			
		}
		break;
	case 41:		// Guardian �������̽����� ���콺�� �Һ� �������� �� ��.
		{
			int nValue = GetItemIndex(&pUser->m_ItemGuardian);

			if(pUser->m_ItemGuardian.GetQuantity()==0)
				return;
			if(pUser->m_ItemMouse.GetID()!=0)
				return;

			if(nValue == ITEM_SUPPLIES_INDEX || (nValue >= ITEM_CONSUMABLEST_INDEX && nValue <= ITEM_CONSUMABLEEND_INDEX))
			{						
				PickupItem.bZipCode	= 0;
				memcpy(&PickupItem.Item, &pUser->m_ItemGuardian, sizeof(CItem));
				memcpy(&pUser->m_ItemMouse, &pUser->m_ItemGuardian, sizeof(CItem));
				memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));
				goto lb_exit2;
			}
		}
		break;
	case 42:		// Guardian �������� ���콺���� ��������.
		{
			int nVal = GetItemIndex(&pUser->m_ItemMouse);
			
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 42, DEF_COPYITEM_TYPE_LARGE, TRUE))
				goto lb_exit2;
							
			if(nVal==ITEM_GUARDIAN_INDEX)
			{				
				PickupItem.bZipCode		= 0;
				PickupItem.bZipCode2	= 0;
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
				
				if(pUser->GetCurDungeon()->GetDungeonDataEx()->m_cGuardianItem.m_wItemID==0)
				{
					pUser->GetCurDungeon()->GetDungeonDataEx()->SetGuardianItem(&pUser->m_ItemMouse);										
				}
				else
				{
					BOOL bChk = FALSE;

					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].m_wItemID==0)
						{
							memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
							PickupItem.bZipCode		= i;
							PickupItem.bZipCode2	= 1;
							bChk = TRUE;
							break;
						}
					}

					if(bChk==FALSE)
					{
						AppearItem(pUser, &pUser->m_ItemMouse, 0, 0, ITEM_TYPE_LARGE);
						PickupItem.bZipCode2	= 2;
					}
				}															
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit2;
			}
		}
		break;
	case 43:	// Guardian �������� ���� ����â���� ���콺��.
		{		
			DUNGEON_DATA_EX* pDungeon = pUser->GetCurDungeon()->GetDungeonDataEx();
			
			if (!pDungeon->m_bSiege)
			{					
				if(pDungeon->m_cGuardianItem.m_wItemID!=0)
				{
					memcpy(&pUser->m_ItemMouse, &pDungeon->m_cGuardianItem, sizeof(CItem));
					
					PickupItem.bZipCode	= 0;
					memcpy(&PickupItem.Item, &pDungeon->m_cGuardianItem, sizeof(CItem));
					memset(&pDungeon->m_cGuardianItem, 0, sizeof(CItem));
					pDungeon->SetGuardianItem(&pDungeon->m_cGuardianItem);
					
					goto lb_exit2;
				}
			}
		}
		break;
	case 44:	// Guardian ���� ������ ���.
		{
			DWORD dwRand = 0;

			int nValue = GetItemIndex(&pUser->m_ItemGuardian);

			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemGuardian.m_wItemID);

			if(!pBaseItem)
				return;		
			
			if (!pUser->m_pInv_Guardian[0].GetID())
				return;			

			if(nValue==ITEM_SUPPLIES_INDEX)
			{
				int nCount = pUser->m_ItemGuardian.GetQuantity();

				if(nCount==0)
					return;				

				if (GUARDIAN_ITEM_TYPE_EGG == pUser->m_pInv_Guardian[0].GetBaseItem()->BaseItem_Guardian.bType)
				{
					if( pBaseItem->BaseItem_Supplies.bType == 5 )		
					{
						// CompleteTime
						dwRand	= ( rand()%(pBaseItem->BaseItem_Supplies.wMax - pBaseItem->BaseItem_Supplies.wMin+1) ) + pBaseItem->BaseItem_Supplies.wMin;
						
						pUser->m_pInv_Guardian[0].SetGuardianEggCompleteTime(pUser->m_pInv_Guardian[0].m_Item_Guardian.dwCompleteTime+(WORD)dwRand);
						SendToUpdate_GuardianItem(dwConnectionIndex, &pUser->m_pInv_Guardian[0]);						
					}
					else
					{
						return;
					}

					PickupItem.bZipCode	= 1;	// completeŸ���� �þ��.
					PickupItem.dwMoney = 0;
				}
				else
				{
					CMonster* pGuardian = pUser->GetGuardian(0);
					if(!pGuardian)
						return;
					
					float fPer = FLOAT(g_pBaseClassInfo[ pUser->GetClass() ].wPotion_Recovery 
						+ g_pBaseRaceInfo[ pUser->GetClass() ].wPotion_Recovery);

					if( pBaseItem->BaseItem_Supplies.bType == 1 )		// HP
					{
						dwRand	= ( rand()%(pBaseItem->BaseItem_Supplies.wMax - pBaseItem->BaseItem_Supplies.wMin+1) ) + pBaseItem->BaseItem_Supplies.wMin;
						dwRand  = DWORD((float)dwRand * (fPer / 100.f));
						pGuardian->SetHP(DWORD(pGuardian->GetHP() 
							+ ( pGuardian->GetMaxHP() * ( (float)dwRand  / 100.f ) ) ));

						PickupItem.bZipCode	= 0;	// ����� �������� ���Ǵ�.
						PickupItem.dwMoney	= pGuardian->GetHP();
					}			
					else if( pBaseItem->BaseItem_Supplies.bType == 3 ) // HP POINT
					{
						dwRand = ( rand()%(pBaseItem->BaseItem_Supplies.wMax - pBaseItem->BaseItem_Supplies.wMin+1) ) + pBaseItem->BaseItem_Supplies.wMin;
						pGuardian->SetHP( DWORD(pGuardian->GetHP() 
							+ ( dwRand * ( fPer / 100.f ) ) ));
						
						PickupItem.bZipCode	= 0;	// ����� �������� ���Ǵ�.
						PickupItem.dwMoney	= pGuardian->GetHP();
					}								
					else  if( pBaseItem->BaseItem_Supplies.bType == 6 ) // ���� ��� (�ð� ����) : �ִ��� 2005.1.17
					{
						dwRand = ( rand()%(pBaseItem->BaseItem_Supplies.wMax - pBaseItem->BaseItem_Supplies.wMin+1) ) + pBaseItem->BaseItem_Supplies.wMin;
						::SendExchangeGuardianLife(pUser, dwRand);

						PickupItem.bZipCode	= 2;	// ���� �����
						PickupItem.dwMoney	= dwRand;
					}
					else if( pBaseItem->BaseItem_Supplies.bType == 7)	// ����� ��ų�� : �ִ��� 2005.1.17
					{
						// Skill Book�� Min�� Skill Kind, Max�� Skill Level�̴�. : �ִ��� 2005.1.17
						const int iSkillKind = pBaseItem->BaseItem_Supplies.wMin;
						const int iSkillLevel = pBaseItem->BaseItem_Supplies.wMax;
						
						PickupItem.bZipCode = 3;	// ��ų�� ����
						PickupItem.bZipCode2 = 0;
						
						const int iOldSkillLevel = pGuardian->GetSkillLevel(iSkillKind);

					
						// ���� �� ���� ������ ���� ����ٸ�
						if(iOldSkillLevel >= iSkillLevel)
						{
							PickupItem.dwMoney = 2;
							goto lb_exit2;
						}

						// 0�� 1���̴� ����ó��
						if( iOldSkillLevel == 0 && iSkillLevel != 1)
						{
							PickupItem.dwMoney = 3;
							goto lb_exit2;
						}

						// �ٷ� ��(5��)�� ������ ����� �ʾҴٸ�
						if( iOldSkillLevel < iSkillLevel - 5)
						{
							PickupItem.dwMoney = 3;
							goto lb_exit2;
						}

						// �ִ�� ��� �� �ִ� ���� �Ѿ��ٸ�
						if(iSkillLevel == 1 && pGuardian->GetTemp(NPC_TEMP_SKILLMANY) >= MAX_GUARDIAN_USE_SKILL)
						{
							PickupItem.dwMoney = 1;
							goto lb_exit2;
						}
						
						// ����� ������ ���ų� �ʹ� ���ٸ�
						if( pGuardian->GetLevel() < pBaseItem->BaseItem_Supplies.wMin_Lev
						||  pGuardian->GetLevel() > pBaseItem->BaseItem_Supplies.wMax_Lev )
						{
							PickupItem.dwMoney = 4;
							goto lb_exit2;
						}
						

						PickupItem.bZipCode2 = iSkillLevel;	// ����
						PickupItem.dwMoney = iSkillKind;	
						pGuardian->SetSkillLevel(iSkillKind, iSkillLevel);

						// Ŭ���̾�Ʈ ����ȭ
						pGuardian->SendAllStatus();
					}
					else  if( pBaseItem->BaseItem_Supplies.bType == 8 ) // ���� ������ : �ִ��� 2005.3.4
					{
						pGuardian->SetDex(0);
						pGuardian->SetEgo(0);
						pGuardian->SetInt(0);
						pGuardian->SetStr(0);
						pGuardian->SetVit(0);
						pGuardian->SetStatPoint((pGuardian->GetLevel() - 1) * 3);
						
						PickupItem.bZipCode	= 4;	// ���� �ʱ�ȭ ��!
						PickupItem.dwMoney	= 0;

						// �ֽ��� ���·� �ٲٰ�, Ŭ���̾�Ʈ ����ȭ
						pGuardian->UpdateAllStatus();
						pGuardian->SendAllStatus();
					}
					else
					{
						return;
					}

					if(pGuardian->GetHP() > pGuardian->GetMaxHP())
						pGuardian->SetHP(pGuardian->GetMaxHP());
				}

				if(nCount>1)
					pUser->m_ItemGuardian.SetQuantity(BYTE(nCount-1));
				else if(nCount==1)
					memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));				
				
				memcpy(&PickupItem.Item, &pUser->m_ItemGuardian, sizeof(CItem));
				goto lb_exit2;
			}
		}
		break;
	case 45:		// Guardian�� ������� ���� ������ �̵�.
		{
			int nValue = GetItemIndex(&pUser->m_ItemGuardian);

			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 45, DEF_COPYITEM_TYPE_SMALL, TRUE))
				goto lb_exit2;
			
			if(nValue==ITEM_SUPPLIES_INDEX)
			{				
				PickupItem.bZipCode	= 0;
				memcpy(&PickupItem.Item, &pUser->m_ItemGuardian, sizeof(CItem));

//				BYTE byPacketCnt = pUser->m_ItemGuardian.GetQuantity();

				Insert_SmallInvItem(pUser, &pUser->m_ItemGuardian, 0, pUser->m_ItemGuardian.GetQuantity(), TRUE);

				memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));
			}
			goto lb_exit2;
		}
		break;
	case 46:
		{
			// sung-han 2005-03-15 �ŷ�, ���, �����ʵǴ� ������ ó��, ����� �ŷ�..
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if( pBaseItem && pBaseItem->GetMovable() != 0 )
				return;

			if(!CheckTrade(pUser->m_nItemTradeSeriel))
				return;
			
//			int nValue = GetItemIndex(&pUser->m_ItemMouse);

			if(!CheckItem(&pUser->m_ItemMouse, ITEM_TYPE_LARGE))
				return;
			
			// Item Move //
			if(pUser->m_pTradeItem[pItemPickup->bZipCode].GetID()==0)
			{				
				PickupItem.bZipCode	= pItemPickup->bZipCode;
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
				memcpy(&pUser->m_pTradeItem[pItemPickup->bZipCode], &pUser->m_ItemMouse, sizeof(CItem));
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				
				goto lb_exit2;
			}
			else
			{
				return;
			}
		}
		break;
	case 47:		// ���� �������� ���콺���� Ʈ���̵� â���� �̵�.
		{
			// sung-han 2005-03-15 �ŷ�, ���, �����ʵǴ� ������ ó��, ����� �ŷ�..
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if( pBaseItem && pBaseItem->GetMovable() != 0 )
				return;

			if(!CheckTrade(pUser->m_nItemTradeSeriel))
				return;

//			int nValue = GetItemIndex(&pUser->m_ItemMouse);

			if(pUser->m_ItemMouse.GetID()==0)
				return;

			if(pUser->m_ItemMouse.GetQuantity()==0)
				return;
			
			if(pUser->m_ItemMouse.GetQuantity() < pItemPickup->dwMoney)
			{
				if(GetItemType(pUser->m_ItemMouse.GetID())==ITEM_TYPE_SMALL)
				{
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

					Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), TRUE);
					
					PickupItem.bZipCode	= 0;
									
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					goto lb_exit2;
				}

				return;
			}
			
			if(GetItemType(pUser->m_ItemMouse.GetID())==ITEM_TYPE_SMALL)				
			{				
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

				//if(pUser->m_pTradeItem[pItemPickup->bZipCode].GetID()==0)
				{
					if(pUser->m_ItemMouse.GetQuantity()>=pItemPickup->dwMoney)
					{
						BYTE	byPacketCount	= BYTE(pItemPickup->dwMoney);	
						BYTE	byOutCount		= BYTE(pUser->m_ItemMouse.GetQuantity()-pItemPickup->dwMoney);
											
						if(byPacketCount==pUser->m_ItemMouse.GetQuantity())
						{
							Insert_TradeItem(pUser
								, &pUser->m_ItemMouse
								, pItemPickup->bZipCode
								, pUser->m_ItemMouse.GetQuantity()
								, FALSE);
						}
						else
						{
							pUser->m_ItemMouse.SetQuantity(BYTE(pItemPickup->dwMoney));

							Insert_TradeItem(pUser
								, &pUser->m_ItemMouse
								, pItemPickup->bZipCode
								, BYTE(pItemPickup->dwMoney)
								, TRUE);
							
							if(byOutCount!=0)
							{
								pUser->m_ItemMouse.SetQuantity(byOutCount);
								Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, byOutCount, TRUE);
							}
						}		
						
						PickupItem.dwMoney	=	pItemPickup->dwMoney;
						PickupItem.bZipCode	=	pItemPickup->bZipCode;					
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
						goto lb_exit2;
					}																
				}				
			}
			else
			{
				return;
			}
		}
		break;
	case 48:		// ū �������� Ʈ���̵� â���� ���콺�� �̵�.
		{
			if(!CheckTrade(pUser->m_nItemTradeSeriel))
				return;

//			int nValue = pUser->m_pTradeItem[pItemPickup->bZipCode].GetID()/ITEM_DISTRIBUTE;				
			
			if(pUser->m_ItemMouse.GetID()==0)
			{				
				if(GetItemType(pUser->m_pTradeItem[pItemPickup->bZipCode].GetID())!=ITEM_TYPE_LARGE)
					return;
				
				PickupItem.bZipCode	= pItemPickup->bZipCode;
					
				memcpy(&PickupItem.Item, &pUser->m_pTradeItem[pItemPickup->bZipCode], sizeof(CItem));
				memcpy(&pUser->m_ItemMouse, &pUser->m_pTradeItem[pItemPickup->bZipCode], sizeof(CItem));
				memset(&pUser->m_pTradeItem[pItemPickup->bZipCode], 0, sizeof(CItem));
				goto lb_exit2;						
			}
		}
		break;
	case 49:		// ���� �������� Ʈ���̵� â���� ���콺�� �̵�.
		{
			if(!CheckTrade(pUser->m_nItemTradeSeriel))
				return;

//			int nValue = GetItemIndex(&pUser->m_pTradeItem[pItemPickup->bZipCode]);

			if(pUser->m_pTradeItem[pItemPickup->bZipCode].GetQuantity()==0)
				return;
			
			if(GetItemType(pUser->m_pTradeItem[pItemPickup->bZipCode].GetID())==ITEM_TYPE_SMALL)
			{
				if(pUser->m_ItemMouse.GetID()==0)
				{					
					PickupItem.bZipCode	= pItemPickup->bZipCode;
						
					memcpy(&PickupItem.Item, &pUser->m_pTradeItem[pItemPickup->bZipCode], sizeof(CItem));
					memcpy(&pUser->m_ItemMouse, &pUser->m_pTradeItem[pItemPickup->bZipCode], sizeof(CItem));
					memset(&pUser->m_pTradeItem[pItemPickup->bZipCode], 0, sizeof(CItem));
					goto lb_exit2;					
				}
				else
					return;
			}
			else
				return;
		}
		break;
	case 50:		// ��Ż�� ��ġ ����.
		{
			int nValue = GetItemIndex(&pUser->m_pInv_Small[pItemPickup->bZipCode]);

			if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()==0)
				return;

			if(pUser->m_nItemTradeSeriel!=0)
				return;

			if(nValue==ITEM_ZODIAC_INDEX)
			{
				if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_INDEX__)
				{				
					BOOL bPortalChk = FALSE;
					
					if(!pUser->GetCurDungeon()->GetDungeonDataEx()->IsDungeonOwner(pUser))
					{
#if defined TAIWAN_LOCALIZING
						// �븸�� ��� ���� 0������ ����� ��ġ�� ����Ҽ��ִ�.					
						// ���� �� �ȵǰ�
						if( pUser->GetCurDungeon()->GetID() > 10 &&
							pUser->GetCurDungeon()->GetDungeonDataEx()->GetDungeonType() != DUNGEON_TYPE_EVENT)
#else
						if(pUser->GetCurDungeon()->GetID() >= 2000 && pUser->GetCurDungeon()->GetID() < 4000)
#endif
						{
							if(pUser->GetCurLayerIndex() == 0)
							{
								bPortalChk = TRUE;
							}
						}
					}
					else
					{
						bPortalChk = TRUE;
					}

					if(bPortalChk)
					{						
						PickupItem.bZipCode	= pItemPickup->bZipCode;						
						
						SetPositionPotalItem(&pUser->m_pInv_Small[pItemPickup->bZipCode], pUser->GetCurLayerIndex()
							,pUser->GetCurDungeonID(), (WORD)pUser->GetCurPosition()->x
							,(WORD)pUser->GetCurPosition()->y);

						memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));					
						
						goto lb_exit2;
					}
					else
					{
						return;
					}
				}							
			}
			else
			{
				return;
			}
		}
		break;
	case 51:		// ��Ż ���.
		{
			int nValue = GetItemIndex(&pUser->m_pInv_Small[pItemPickup->bZipCode]);

			if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()==0)
				return;

			if(pUser->m_nItemTradeSeriel!=0)
				return;

			if(nValue==ITEM_ZODIAC_INDEX)
			{
				if(pUser->m_pInv_Small[pItemPickup->bEquipCode].GetID() == __ITEM_PORTALUSED_INDEX__)
				{
					BOOL bPortalChk = FALSE;

					CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(
						pUser->m_pInv_Small[pItemPickup->bZipCode].m_Item_Zodiac.wMapId);

					if(pDungeon)
					{
						if(!pDungeon->GetDungeonDataEx()->IsDungeonOwner(pUser))
						{
#if defined TAIWAN_LOCALIZING
								// �븸�� ��� ���� 0������ ����� ��ġ�� ����Ҽ��ִ�.
								// ������ �ȵǰ�
								if( pUser->GetCurDungeon()->GetID() > 10 &&
									pUser->GetCurDungeon()->GetDungeonDataEx()->GetDungeonType() != DUNGEON_TYPE_EVENT)
#else
								if(pDungeon->GetID()>=2000 && pDungeon->GetID()<4000)
#endif
							{
								if(pUser->m_pInv_Small[pItemPickup->bZipCode].m_Item_Zodiac.bLayer==0)
									bPortalChk = TRUE;
							}
						}
						else
						{
							bPortalChk = TRUE;
						}
					}
					else
					{
						bPortalChk = TRUE;
					}

					if(bPortalChk)
					{						
						PickupItem.bZipCode		= pItemPickup->bEquipCode;
						PickupItem.bEquipCode	= pItemPickup->bZipCode;
						PickupItem.dwMoney		= pUser->m_pInv_Small[pItemPickup->bEquipCode].GetID();
							
						memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bEquipCode], sizeof(CItem));

						if(pUser->m_pInv_Small[pItemPickup->bEquipCode].GetQuantity()==1)
						{
							memset(&pUser->m_pInv_Small[pItemPickup->bEquipCode], 0, sizeof(CItem));
						}
						else
						{
							pUser->m_pInv_Small[pItemPickup->bEquipCode].SetQuantity(
								BYTE(pUser->m_pInv_Small[pItemPickup->bEquipCode].GetQuantity()-1));
						}
						
						if(pUser->m_ItemGuardian.GetID()!=0)
						{
							Insert_SmallInvItem(pUser
								, &pUser->m_ItemGuardian
								, 0
								, pUser->m_ItemGuardian.GetQuantity()
								, TRUE);							
							memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));
						}
							

						//  ���� ��Ż�� ������� ��ȯ �Ǿ� ������� ��ȯ��Ų�� ��Ż 
						//  �迵�� 2005.02.07
						if (pUser->IsSummonGuardian(0) ) 	pUser->UnSummonGuardian(0);

						CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(PickupItem.Item.m_Item_Zodiac.wMapId);

						if(pDungeon)
						{
							goto lb_exit2;
						}

						CheckItemSlot(pUser);
						
						goto lb_exit2;
					}
					else
					{
						return;
					}
				}							
			}
			else
			{
				return;
			}
		}
		break;
	case 52:		// ��ȯ ��ũ�� ���.
		{
			int nValue = GetItemIndex(&pUser->m_pInv_Small[pItemPickup->bZipCode]);

			if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()==0)
				return;
			
			if(pUser->m_nItemTradeSeriel!=0)
				return;

			if(nValue==ITEM_ZODIAC_INDEX)
			{
				//if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_VILL1__ || 
				//	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_VILL2__ ||
				//	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_VILL3__ ||
				//	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_VILL4__ ||
				//	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_VILL5__)
				// ���� ��ȯ ��ũ�� �߰� �迵�� 2004.02.14
				WORD wItemID = pUser->m_pInv_Small[pItemPickup->bZipCode].GetID();
				if (wItemID >= __ITEM_PORTAL_VILL1__  &&  wItemID <= __ITEM_PORTAL_VILL6__)
				{					
					PickupItem.bZipCode	= pItemPickup->bZipCode;
					PickupItem.dwMoney	= pUser->m_pInv_Small[pItemPickup->bZipCode].GetID();

											
					SetPortal(&pUser->m_pInv_Small[pItemPickup->bZipCode]);

					memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));

					if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()==1)
						memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
					else
						pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(BYTE(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()-1));
						
					if(pUser->m_ItemGuardian.GetID()!=0)
					{
						Insert_SmallInvItem(pUser, &pUser->m_ItemGuardian, 0, pUser->m_ItemGuardian.GetQuantity(), TRUE);							
						memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));
					}
					
					//  ���� ��Ż�� ������� ��ȯ �Ǿ� ������� ��ȯ��Ų�� ��Ż 
					//  �迵�� 2005.02.07
					if (pUser->IsSummonGuardian(0) ) 	pUser->UnSummonGuardian(0);


					CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(PickupItem.Item.m_Item_Zodiac.wMapId);

					if(pDungeon)
						goto lb_exit2;
						
					CheckItemSlot(pUser);

					goto lb_exit2;
				}							
			}
			else
				return;
		}
		break;
	case 53:		// ��Ʈ���� ��ȯ ��ũ�� ���.
		{
			int nValue = GetItemIndex(&pUser->m_pBelt[pItemPickup->bZipCode]);

			if(pUser->m_pBelt[pItemPickup->bZipCode].GetQuantity()==0)
				return;

			if(pUser->m_nItemTradeSeriel!=0)
				return;
			
			if(nValue==ITEM_ZODIAC_INDEX)
			{
				// ���� ��ȯ ��ũ�� �߰� �迵�� 2004.02.14
				WORD wItemID = pUser->m_pBelt[pItemPickup->bZipCode].GetID();
				if (wItemID >= __ITEM_PORTAL_VILL1__  &&  wItemID <= __ITEM_PORTAL_VILL6__)
				//if(pUser->m_pBelt[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_VILL1__ || pUser->m_pBelt[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_VILL2__ || pUser->m_pBelt[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_VILL3__ || pUser->m_pBelt[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_VILL4__ || pUser->m_pBelt[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_VILL5__)
				{					
					PickupItem.bZipCode	= pItemPickup->bZipCode;
					PickupItem.dwMoney	= pUser->m_pBelt[pItemPickup->bZipCode].GetID();
						
					SetPortal(&pUser->m_pBelt[pItemPickup->bZipCode]);

					memcpy(&PickupItem.Item, &pUser->m_pBelt[pItemPickup->bZipCode], sizeof(CItem));						

					if(pUser->m_pBelt[pItemPickup->bZipCode].GetQuantity()==1)
						memset(&pUser->m_pBelt[pItemPickup->bZipCode], 0, sizeof(CItem));
					else
						pUser->m_pBelt[pItemPickup->bZipCode].SetQuantity(BYTE(pUser->m_pBelt[pItemPickup->bZipCode].GetQuantity()-1));
					
					if(pUser->m_ItemGuardian.GetID()!=0)
					{
						Insert_SmallInvItem(pUser, &pUser->m_ItemGuardian, 0, pUser->m_ItemGuardian.GetQuantity(), TRUE);							
						memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));
					}

					//  ���� ��Ż�� ������� ��ȯ �Ǿ� ������� ��ȯ��Ų�� ��Ż 
					//  �迵�� 2005.02.07
					if (pUser->IsSummonGuardian(0) ) 	pUser->UnSummonGuardian(0);

					CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(PickupItem.Item.m_Item_Zodiac.wMapId);

					if(pDungeon)
						goto lb_exit2;

					CheckItemSlot(pUser);
					
					goto lb_exit2;
				}							
			}
			else
				return;
		}
		break;
	case 54:		// ��Ż ���.
		{
			int nValue = GetItemIndex(&pUser->m_pBelt[pItemPickup->bZipCode]);

			if(pUser->m_ItemMouse.GetID()!=0)
				return;

			if(pUser->m_pBelt[pItemPickup->bZipCode].GetQuantity()==0)
				return;

			if(pUser->m_nItemTradeSeriel!=0)
				return;
			
			if(nValue==ITEM_ZODIAC_INDEX)
			{
				// ��Ʈ���� ����� �ֵ���ī�� ���ȵǴ� ���� ���� 
				// �迵�� 2005.02.07
				if(pUser->m_pInv_Small[pItemPickup->bEquipCode].GetID()==__ITEM_PORTALUSED_INDEX__)
				{
					BOOL bPortalChk = FALSE;

					CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pUser->m_pBelt[pItemPickup->bZipCode].m_Item_Zodiac.wMapId);

					if(pDungeon)
					{
						if(!pDungeon->GetDungeonDataEx()->IsDungeonOwner(pUser))
						{
#if defined TAIWAN_LOCALIZING
								// �븸�� ��� ���� 0������ ����� ��ġ�� ����Ҽ��ִ�.	
								// ���� �� �ȵǰ�
								if( pUser->GetCurDungeon()->GetID() > 10 &&
									pUser->GetCurDungeon()->GetDungeonDataEx()->GetDungeonType() != DUNGEON_TYPE_EVENT)
#else

								if(pDungeon->GetID()>=2000 && pDungeon->GetID()<4000)
#endif
							{
								if(pUser->m_pBelt[pItemPickup->bZipCode].m_Item_Zodiac.bLayer==0)
								{
									bPortalChk = TRUE;
								}
							}
						}
						else
						{
							bPortalChk = TRUE;
						}
					}
					else
					{
						bPortalChk = TRUE;
					}

					if(bPortalChk)
					{													
						PickupItem.bZipCode		= pItemPickup->bEquipCode;
						PickupItem.bEquipCode	= pItemPickup->bZipCode;
						PickupItem.dwMoney		= pUser->m_pInv_Small[pItemPickup->bEquipCode].GetID();

						memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bEquipCode], sizeof(CItem));
						
						if(pUser->m_pInv_Small[pItemPickup->bEquipCode].GetQuantity()==1)
						{
							memset(&pUser->m_pInv_Small[pItemPickup->bEquipCode], 0, sizeof(CItem));
						}
						else
						{
							pUser->m_pInv_Small[pItemPickup->bEquipCode].SetQuantity(
								BYTE(pUser->m_pInv_Small[pItemPickup->bEquipCode].GetQuantity()-1));
						}
						
						if(pUser->m_ItemGuardian.GetID()!=0)
						{
							Insert_SmallInvItem(pUser
								, &pUser->m_ItemGuardian
								, 0
								, pUser->m_ItemGuardian.GetQuantity()
								, TRUE);							

							memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));
						}
						
						//  ���� ��Ż�� ������� ��ȯ �Ǿ� ������� ��ȯ��Ų�� ��Ż 
						//  �迵�� 2005.02.07
						if (pUser->IsSummonGuardian(0) ) 	pUser->UnSummonGuardian(0);

						CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(PickupItem.Item.m_Item_Zodiac.wMapId);

						if(pDungeon)
							goto lb_exit2;
												
						CheckItemSlot(pUser);

						goto lb_exit2;
					}
					else
						return;
				}							
			}
			else
				return;
		}
		break;
	case 55:		// ��Ʈ���� ��ġ ����.
		{
			int nValue = GetItemIndex(&pUser->m_pBelt[pItemPickup->bZipCode]);

			if(pUser->m_pBelt[pItemPickup->bZipCode].GetQuantity()==0)
				return;

 			if(pUser->m_nItemTradeSeriel!=0)
				return;
			
			if(nValue==ITEM_ZODIAC_INDEX)
			{
				if(pUser->m_pBelt[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_INDEX__)
				{					
					PickupItem.bZipCode	= pItemPickup->bZipCode;						
					
					SetPositionPotalItem(&pUser->m_pBelt[pItemPickup->bZipCode], pUser->GetCurLayerIndex()
						,pUser->GetCurDungeonID(), (WORD)pUser->GetCurPosition()->x
						,(WORD)pUser->GetCurPosition()->y);
					
					memcpy(&PickupItem.Item, &pUser->m_pBelt[pItemPickup->bZipCode], sizeof(CItem));
					
					goto lb_exit2;
				}							
			}
			else
				return;
		}
		break;
	case 56:		// ��Ƽ ������ ������(�۽��� ��Ż) ����� ��.
		{
			CUser* pSendUser = g_pUserHash->GetData(pItemPickup->dwMoney);

			if(pSendUser)
			{
				if(!pSendUser->GetCurDungeon() || !pUser->GetCurDungeon())
				{
					// ���� //
					DSTC_PARTY_PORTALFAIL pSendPacket;
					memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
					__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					pSendPacket.byResult = 2;
					NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					
					memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
					__lstrcpyn(pSendPacket.szName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					pSendPacket.byResult = 2;
					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}

				if(pSendUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID == 
					pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID)
				{
					if(pSendUser->GetCurLayerIndex() == pUser->GetCurLayerIndex())
					{
						// ���� //
						DSTC_PARTY_PORTALFAIL pSendPacket;
						memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
						__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
						pSendPacket.byResult = 5;
						NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
							, FLAG_SEND_NOT_ENCRYPTION);
						
						memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
						__lstrcpyn(pSendPacket.szName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
						pSendPacket.byResult = 5;
						NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
							, FLAG_SEND_NOT_ENCRYPTION);
						return;
					}
				}

				if(pUser->IsCurDungeonEventInLive())
				{
					// ���� //
					DSTC_PARTY_PORTALFAIL pSendPacket;
					memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
					__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					pSendPacket.byResult = 5;
					NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION);
					
					memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
					__lstrcpyn(pSendPacket.szName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					pSendPacket.byResult = 5;
					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION);
					return;
				}	

/*
					// Item Chk //
					if(	pItemPickup->bZipCode2 == 0 ||
						pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL1__ ||
						pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL2__)
					{
*/				
					// ��Ż �̵� �޼��� //
					DSTC_PARTY_PORTALMOVE pSendPacket;
					pSendPacket.wDungeonID		= WORD(pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID);
					pSendPacket.byLayerIndex	= pUser->GetCurLayerIndex();
					pSendPacket.dwPartyId		= pUser->m_dwPartyId;
					pSendPacket.fPosX			= pUser->GetCurPosition()->x;
					pSendPacket.fPosZ			= pUser->GetCurPosition()->y;						
					NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
					
					// ������ �޼��� //						
					if(pItemPickup->bZipCode2==1)
					{
						// �۽��� ����
						BYTE byCount = pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity();

						if(	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL1__ ||
							pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL2__)
						{
							if(byCount>1)
								pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(BYTE(byCount-1));
							else
								memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
						}
					}						
					
					PickupItem.bZipCode	= pItemPickup->bZipCode;
					memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));												

					CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(PickupItem.Item.m_Item_Zodiac.wMapId);

					if(pDungeon)
						goto lb_exit2;

					CheckItemSlot(pUser);
					
					if(pUser->m_ItemGuardian.GetID()!=0)
					{
						Insert_SmallInvItem(pUser, &pUser->m_ItemGuardian, 0, pUser->m_ItemGuardian.GetQuantity(), TRUE);
						memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));
					}

					goto lb_exit2;
/*
								}
								else
								{
									// ���� //
									DSTC_PARTY_PORTALFAIL pSendPacket;
									memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
									__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
									pSendPacket.byResult = 3;
									NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				
									memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
									__lstrcpyn(pSendPacket.szName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
									pSendPacket.byResult = 3;
									NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
									return;
								}*/
				
			}
			else
			{
				if(pUser->IsCurDungeonEventInLive())
				{
					// ���� //
					DSTC_PARTY_PORTALFAIL pSendPacket;
					pSendPacket.byResult = 5;
					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION);
					return;
				}	

/*
				// Item Chk //
				if(	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL1__ ||
					pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL2__)
				{*/

					// ���� ��Ż �̵� �޼��� //
					WSTDS_PARTY_PORTALMOVE pSendPacket;
					pSendPacket.wDungeonID			= WORD(pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID);
					pSendPacket.byLayerIndex		= pUser->GetCurLayerIndex();
					pSendPacket.dwPartyId			= pUser->m_dwPartyId;
					pSendPacket.fPosX				= pUser->GetCurPosition()->x;
					pSendPacket.fPosZ				= pUser->GetCurPosition()->y;
					pSendPacket.dwUserIndex			= pUser->GetID();
					pSendPacket.dwPortalUserIndex	= pItemPickup->dwMoney;
					g_pNet->SendToServer(WSINDEX, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					
					// ������ �޼��� //
					if(pItemPickup->bZipCode2==1)
					{
						// �۽��� ����
						BYTE byCount = pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity();

						if(	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL1__ ||
							pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL2__)
						{   
							if(byCount>1)
								pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(BYTE(byCount-1));
							else
								memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
						}
					}						
					
					PickupItem.bZipCode	= pItemPickup->bZipCode;
					memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));												

					CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(PickupItem.Item.m_Item_Zodiac.wMapId);

					if(pDungeon)
						goto lb_exit2;

					CheckItemSlot(pUser);
					
					if(pUser->m_ItemGuardian.GetID()!=0)
					{
						Insert_SmallInvItem(pUser, &pUser->m_ItemGuardian, 0, pUser->m_ItemGuardian.GetQuantity(), TRUE);
						memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));
					}

					goto lb_exit2;
/*
				}
				else
				{
					// ���� //
					WSTDS_PARTY_PORTALFAIL pSendServerPacket;						
					memset(pSendServerPacket.szName, 0, sizeof(pSendServerPacket.szName));
					__lstrcpyn(pSendServerPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					pSendServerPacket.byResult		= 3;
					pSendServerPacket.dwUserIndex	= pItemPickup->dwMoney;
					g_pNet->SendToServer(WSINDEX,  (char*)&pSendServerPacket, pSendServerPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}
*/
				
			}
		}
		break;
	case 57:	// ���콺���� �������� ������ �ȱ涧 
		{
//			int nValue = GetItemIndex(&pUser->m_ItemMouse);

			//sung-han 2005-03-15 �ŷ�, ���, �����ʵǴ� ������ ó��, ����� ����..
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if( pBaseItem && pBaseItem->GetMovable() != 0 )
			{
				for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
				{
					if(pUser->m_pInv_Large[ i ].m_wItemID==0)
					{
						PickupItem.bInv		= 6;
						PickupItem.bZipCode = i;
						memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
						memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
						goto lb_exit2;
					}
				}
			}

			if(pUser->m_ItemMouse.m_wItemID==0)
				return;

			if(pUser->GetCurDungeon()->GetDungeonDataEx()->GetDungeonType()!=DUNGEON_TYPE_VILLAGE)
			{
				if(GetItemType(pUser->m_ItemMouse.GetID())==ITEM_TYPE_LARGE)
				{
					if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 57, DEF_COPYITEM_TYPE_LARGE, TRUE))
						goto lb_exit2;
					
					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[ i ].m_wItemID==0)
						{
							PickupItem.bInv		= 6;
							PickupItem.bZipCode = i;
							memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
							memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
							memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
							goto lb_exit2;
						}
					}				
										
					AppearItem(pUser
						, &pUser->m_ItemMouse
						, 0
						, 0
						, 0
						, pUser->GetCurPosition()->x
						, 0
						, pUser->GetCurPosition()->y);
					
					PickupItem.bEquipCode	= 0;
					PickupItem.bInv			= 10;
					PickupItem.bZipCode		= 0;
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					goto lb_exit2;
				}
				return;
			}

			if(pUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetID()!=0)
			{
				if(g_pThis->GetServerType()==SERVER_TYPE_VILLAGE)
				{
					for(int i = 0; i < MAX_PLAYER_SHOP_INV; i++)
					{
						if(pUser->m_sPlayerShop.cItem[i].GetID()==0)
						{								
							PlayerShopLog(pUser, &pUser->m_ItemMouse, pItemPickup->bInv, pItemPickup->bZipCode, pItemPickup->dwMoney, 0, TRUE);
							return;
						}
					}
				}					

				for(i = 0; i < MAX_INV_LARGE_POOL; i++)
				{
					if(pUser->m_pInv_Large[i].GetID()==0)
					{													
						PickupItem.bZipCode		= BYTE(i);
						PickupItem.dwMoney		= 0;
						PickupItem.bZipCode2	= 2;
						memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
						memcpy(&PickupItem.Item, &pUser->m_pInv_Large[i], sizeof(CItem));
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
						goto lb_exit2;
					}						
				}												
				
				AppearItem(pUser, &pUser->m_ItemMouse, pUser->m_ItemMouse.GetQuantity(), 0, ITEM_TYPE_LARGE);
				PickupItem.bEquipCode	= 0;				
				PickupItem.bZipCode		= 0;
				memset(&PickupItem.Item, 0, sizeof(CItem));
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit1;
			}
			else
			{
				PlayerShopLog(pUser, &pUser->m_ItemMouse, pItemPickup->bInv, pItemPickup->bZipCode, pItemPickup->dwMoney, 0, TRUE);
				return;
			}								
		}
		break;
	case 58:		// �������� ���콺�� ������ �ű涧.
		{
			if(pUser->m_ItemMouse.GetID()!=0)
				return;
			if(pUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetID()==0)
				return;

			CItem cItem;
			memset(&cItem, 0, sizeof(CItem));
			PlayerShopLog(pUser, &cItem, pItemPickup->bInv, pItemPickup->bZipCode, pUser->m_sPlayerShop.dwCustomSellPrice[pItemPickup->bZipCode], 0, TRUE);
			return;								
		}
		break;
	case 59:	// ���콺���� ���� �Һ� ������.
		{				
//			int nValue = GetItemIndex(&pUser->m_ItemMouse);

			//sung-han 2005-03-15 �ŷ�, ���, �����ʵǴ� ������ ó��, ����� ����..
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if( pBaseItem && pBaseItem->GetMovable() != 0 )
			{
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

				Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), TRUE);

				PickupItem.bInv		= 14;
				PickupItem.bZipCode	= 0;
								
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit2;
			}

			if(pUser->m_ItemMouse.GetQuantity()==0)
				return;				

			if(pUser->GetCurDungeon()->GetDungeonDataEx()->GetDungeonType()!=DUNGEON_TYPE_VILLAGE)
			{
				if(GetItemType(pUser->m_ItemMouse.GetID())==ITEM_TYPE_SMALL)
				{
					if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 59, DEF_COPYITEM_TYPE_SMALL, TRUE))
						goto lb_exit2;
					
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

					Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), TRUE);

					PickupItem.bInv		= 14;
					PickupItem.bZipCode	= 0;
									
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					goto lb_exit2;
				}
				return;
			}
			
			if(GetItemType(pUser->m_ItemMouse.GetID())==ITEM_TYPE_SMALL)
			{
				CItem cItem;
				memcpy(&cItem, &pUser->m_ItemMouse, sizeof(CItem));
				cItem.SetQuantity(BYTE(pItemPickup->dwMoney));										

				if(pUser->m_ItemMouse.GetQuantity() < pItemPickup->dwMoney)
				{
					if(GetItemType(pUser->m_ItemMouse.GetID())==ITEM_TYPE_SMALL)
					{
						memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

						Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), TRUE);

						PickupItem.bInv		= 14;
						PickupItem.bZipCode	= 0;
										
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
						goto lb_exit2;
					}
					return;
				}

				PlayerShopLog(pUser, &cItem, pItemPickup->bInv, pItemPickup->bZipCode, pItemPickup->dwId, pItemPickup->dwMoney, TRUE);
			}
			else
				return;
		}
		break;
	case 60:		// �������� ���콺�� ������ �ű涧.
		{
			if(pUser->m_ItemMouse.GetID()!=0)
				return;
			if(pUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetID()==0)
				return;
			
			CItem cItem;
			memset(&cItem, 0, sizeof(CItem));
			PlayerShopLog(pUser, &cItem, pItemPickup->bInv, pItemPickup->bZipCode, pUser->m_sPlayerShop.dwCustomSellPrice[pItemPickup->bZipCode], pItemPickup->dwMoney, TRUE);
			return;								
		}
		break;
	case 61:		// ��Ƽ ������ ���.
		{
			int nValue = GetItemIndex(&pUser->m_pInv_Small[pItemPickup->bZipCode]);

			if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()==0)
				return;

			if(!pUser->GetCurDungeon())
				return;

			if(pUser->GetCurDungeon()->GetDungeonDataEx()->IsVillage())
				return;				

			if(nValue==ITEM_MAGICARRAY_INDEX)
			{
				WORD wItemId = pUser->m_pInv_Small[pItemPickup->bZipCode].GetID();

				if(	(wItemId>=__ITEM_PARTY_LIFELINK1__	&&	wItemId<=__ITEM_PARTY_LIFELINK5__)	||
					(wItemId>=__ITEM_PARTY_AMBUSH1__	&&	wItemId<=__ITEM_PARTY_AMBUSH5__)	||
					(wItemId>=__ITEM_PARTY_ST1__		&&	wItemId<=__ITEM_PARTY_ST5__)			)
				{
					if(pUser->m_pUsingMagicArrayList->GetCount()==0)
					{
						// ��Ƽ ������ ��� //						
						pLayer->AttachMagicArrayToParty(pUser, pUser->m_pInv_Small[pItemPickup->bZipCode].GetID());
						
						if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()>1)
						{
							pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(
								BYTE(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()-1));
						}
						else
						{
							memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
						}
						
						PickupItem.bZipCode		= pItemPickup->bZipCode;
						memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));
						goto lb_exit2;
					}
					else
					{
						DSTC_PARTY_MAIGCARRAYFAIL pPaccket;
						pPaccket.byErrorCode = 1;
						NetSendToUser( dwConnectionIndex, (char*)&pPaccket, pPaccket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						return;
					}						
				}
				else
				{
					return;
				}
			}
		}
		break;
	case 62:		// ��Ƽ�� ��ü ��Ż.
		{
			int nValue = GetItemIndex(&pUser->m_pInv_Small[pItemPickup->bZipCode]);

			if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()==0)
				return;

			if(!pUser->GetCurDungeon())
				return;
			
			if(pUser->IsCurDungeonSiege())
				return;				

			if(nValue==ITEM_MAGICARRAY_INDEX)
			{
				WORD wItemId = pUser->m_pInv_Small[pItemPickup->bZipCode].GetID();

				if(wItemId==__ITEM_PARTY_PORTAL2__)
				{
					if(pUser->IsCurDungeonEventInLive())
						return;

					// ���� ������ ������ //
					WSTDS_PARTY_ALLPORTAL pSendPacket;						
					pSendPacket.dwPartyId	= pUser->m_dwPartyId;						
					pSendPacket.dwUserIndex	= pUser->GetID();
					pSendPacket.byIndex		= pItemPickup->bZipCode;						
					g_pNet->SendToServer(WSINDEX, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					
					if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()>1)
					{
						pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(
							BYTE(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()-1));
					}
					else
					{
						memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
					}

					PickupItem.bZipCode	= pItemPickup->bZipCode;
					memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));
					goto lb_exit2;
				}
				else
				{
					return;
				}
			}
		}
		break;
	case 63:		// ���콺���� �������� â���� ������ �̵�
		{	
			if (pUser->GetCurDungeon()->GetDungeonDataEx()->m_bSiege)	return;
			if (!pUser->GetCurDungeon()->GetDungeonDataEx()->IsDungeonOwner(pUser))
				break;

			if(pUser->m_ItemMouse.GetQuantity()==0)
				return;
			
			int nValue = GetItemIndex(&pUser->m_ItemMouse);
			
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 63, DEF_COPYITEM_TYPE_SMALL, TRUE))
				goto lb_exit2;							
			
			if(nValue==ITEM_MATERIALEND_INDEX)
			{
				switch (pUser->m_ItemMouse.GetID())
				{
				case __ITEM_METERIALS_FIRE_PROPERTY_INDEX__:
				case __ITEM_METERIALS_ICE_PROPERTY_INDEX__:
				case __ITEM_METERIALS_LIGHT_PROPERTY_INDEX__:					
					PickupItem.bZipCode		= 0;
					PickupItem.dwMoney		= pItemPickup->dwMoney;
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));					
					
					Insert_MagicFieldArrayItem(pUser, &pUser->m_ItemMouse, BYTE(pItemPickup->dwMoney));
															
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					goto lb_exit2;					
				}																		
			}
		}
		break;
	case 64:	// ��������â���� �������� ���콺��
		{
			if (pUser->GetCurDungeon()->GetDungeonDataEx()->m_bSiege)	return;
			if (pUser->m_ItemMouse.GetID())	return;
			if (!pUser->GetCurDungeon()->GetDungeonDataEx()->IsDungeonOwner(pUser))
				break;
			
			int nValue = pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem.GetID()/ITEM_DISTRIBUTE;

			if(pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem.GetQuantity()==0)
				return;

			if(nValue==ITEM_MATERIALEND_INDEX)
			{				
				PickupItem.bZipCode		= 0;					
				memcpy(&PickupItem.Item, &pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem, sizeof(CItem));
				memcpy(&pUser->m_ItemMouse, &pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem, sizeof(CItem));
				pUser->m_ItemMouse.SetQuantity(1);

				if(pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem.GetQuantity()>1)
					pUser->m_ItemMouse.m_Serial	= *g_pThis->GetNewSerial();
				
				if(pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem.GetQuantity()>1)
					pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem.SetQuantity(BYTE(pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem.GetQuantity()-1));
				else
					memset(&pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem, 0, sizeof(CItem));
				
				pUser->GetCurDungeon()->GetDungeonDataEx()->SetMagicFieldArray(&pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem);
				goto lb_exit2;					
			}
		}
		break;
	case 65:
		{
			int nValue = GetItemIndex(&pUser->m_ItemMouse);
			BOOL bIsCorrect = TRUE;

			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if(pBaseItem)
			{
				if(GetItemType(pUser->m_ItemMouse.GetID())==ITEM_TYPE_LARGE)
				{
					if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 65, DEF_COPYITEM_TYPE_LARGE, TRUE))
						goto lb_exit2;

					if(nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_ARMOREND_INDEX)
					{
						if(nValue == ITEM_CLASS_RING || nValue == ITEM_CLASS_AMULET)
						{
							bIsCorrect = FALSE;
						}
						else if(pUser->m_ItemMouse.m_bSlot.uMaxSlot == 0)
						{
							bIsCorrect = FALSE;
						}
						else if(nValue >= ITEM_WEAPONST_INDEX && nValue <= ITEM_WEAPONEND_INDEX)
						{
							if(pBaseItem->BaseItem_Weapon.bItemGrade == 0)
							{
								bIsCorrect = FALSE;
							}
						}
						else if(nValue >= ITEM_ARMORST_INDEX && nValue <= ITEM_ARMOREND_INDEX)
						{
							if(pBaseItem->BaseItem_Armor.bItemGrade == 0)
							{
								bIsCorrect = FALSE;
							}
						}

						for(int i = 1; i < 4; i++)
						{
							CBaseItem* pBaseItemSmall = g_pBaseItemHash->GetData(pUser->m_pUpgradeItem[i].GetID());
							int nValueSmall = pUser->m_pUpgradeItem[i].GetID() / ITEM_DISTRIBUTE;

							if(pBaseItemSmall && nValueSmall == ITEM_UPGRADE_INDEX)
							{
								if(nValue >= ITEM_WEAPONST_INDEX && nValue <= ITEM_WEAPONEND_INDEX)
								{
									if( pBaseItem->BaseItem_Weapon.bItemGrade != pBaseItemSmall->BaseItem_UpGrade.wW_Grade )
									{
										bIsCorrect = FALSE;
									}
									if( pBaseItemSmall->BaseItem_UpGrade.wFormula[nValue] == 0 )
									{
										bIsCorrect = FALSE;	
									}
								}
								else if(nValue >= ITEM_ARMORST_INDEX && nValue <= ITEM_ARMOREND_INDEX)
								{
									if( pBaseItem->BaseItem_Armor.bItemGrade != pBaseItemSmall->BaseItem_UpGrade.wW_Grade )
									{
										bIsCorrect = FALSE;
									}
									if( pBaseItemSmall->BaseItem_UpGrade.wFormula[nValue] == 0 )
									{
										bIsCorrect = FALSE;
									}
								}
							}
							else if(pBaseItemSmall && nValueSmall == ITEM_LIQUID_INDEX)
							{
								if(nValue >= ITEM_WEAPONST_INDEX && nValue <= ITEM_WEAPONEND_INDEX)
								{
									if( pBaseItem->BaseItem_Weapon.bItemGrade != pBaseItemSmall->BaseItem_LiQuid.wW_Grade )
									{
										bIsCorrect = FALSE;
									}
								}
								else if(nValue >= ITEM_ARMORST_INDEX && nValue <= ITEM_ARMOREND_INDEX)
								{
									if( pBaseItem->BaseItem_Armor.bItemGrade != pBaseItemSmall->BaseItem_LiQuid.wW_Grade )
									{
										bIsCorrect = FALSE;
									}
								}
							}
							else if(pBaseItemSmall && nValueSmall == ITEM_EDITION_INDEX)
							{
								if( pBaseItemSmall->BaseItem_Edition.wFormula[nValue] == 0 )
								{
									bIsCorrect = FALSE;
								}
							}
						}
						
						if(bIsCorrect && pUser->m_pUpgradeItem[0].GetID()==0)
						{
							PickupItem.bZipCode		= 0;					
							memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
							memcpy(&pUser->m_pUpgradeItem[0], &pUser->m_ItemMouse, sizeof(CItem));										
							memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
							goto lb_exit2;						
						}
						else
						{
							for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
							{
								if(pUser->m_pInv_Large[i].GetID() == 0)
								{
									PickupItem.bZipCode = 254;
									PickupItem.bEquipCode = i;
									memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
									memcpy(&pUser->m_pInv_Large[i],&PickupItem.Item,sizeof(CItem));
									memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
									goto lb_exit2;						
								}
							}
						}
					}
				}
			}
		}
		break;
	case 66:
		{
			int nValue = GetItemIndex(&pUser->m_pUpgradeItem[0]);

			if(pUser->m_ItemMouse.GetID()!=0)
				return;
			
			if(GetItemType(pUser->m_pUpgradeItem[0].GetID())==ITEM_TYPE_LARGE)
			{
				if(nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_ARMOREND_INDEX)
				{											
					PickupItem.bZipCode		= 0;					
					memcpy(&PickupItem.Item, &pUser->m_pUpgradeItem[0], sizeof(CItem));
					memcpy(&pUser->m_ItemMouse, &pUser->m_pUpgradeItem[0], sizeof(CItem));										
					memset(&pUser->m_pUpgradeItem[0], 0, sizeof(CItem));
					goto lb_exit2;
				}
			}
		}
		break;			
	case 67:
		{
			int nValue = GetItemIndex(&pUser->m_ItemMouse);

			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if(NULL == pBaseItem)
				return;

			if(pUser->m_ItemMouse.GetQuantity()==0)
				return;
			
			if(pUser->m_pUpgradeItem[pItemPickup->bZipCode].GetID()!=0)
				return;
			
			if(GetItemType(pUser->m_ItemMouse.GetID())==ITEM_TYPE_SMALL)
			{
				if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 67, DEF_COPYITEM_TYPE_SMALL, TRUE))
					goto lb_exit2;					
				
				if( nValue == ITEM_UPGRADE_INDEX || nValue == ITEM_LIQUID_INDEX || nValue == ITEM_EDITION_INDEX )	
				{					
					PickupItem.bZipCode		= pItemPickup->bZipCode;
					PickupItem.dwMoney		= pItemPickup->dwMoney;
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

					BOOL bChk = FALSE;

					if(pUser->m_pUpgradeItem[0].GetID() != 0)
					{
						CBaseItem* pBaseItemLarge = g_pBaseItemHash->GetData(pUser->m_pUpgradeItem[0].GetID());
						int nValueLarge = pUser->m_pUpgradeItem[0].GetID() / ITEM_DISTRIBUTE;

						if(pBaseItemLarge)
						{
							if(nValue == ITEM_UPGRADE_INDEX)
							{
								if(nValueLarge >= ITEM_WEAPONST_INDEX && nValueLarge <= ITEM_WEAPONEND_INDEX)
								{
									if(pBaseItemLarge->BaseItem_Weapon.bItemGrade != pBaseItem->BaseItem_UpGrade.wW_Grade)
									{
										PickupItem.bEquipCode	= 0;
										Insert_UpgradeItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));	
										memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
										goto lb_exit2;
									}
									if(pBaseItem->BaseItem_UpGrade.wFormula[nValueLarge] == 0)
									{
										PickupItem.bEquipCode	= 0;
										Insert_UpgradeItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));	
										memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
										goto lb_exit2;
									}
								}
								else if(nValueLarge >= ITEM_ARMORST_INDEX && nValueLarge <= ITEM_ARMOREND_INDEX)
								{
									if(pBaseItemLarge->BaseItem_Armor.bItemGrade != pBaseItem->BaseItem_UpGrade.wW_Grade)
									{
										PickupItem.bEquipCode	= 0;
										Insert_UpgradeItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));	
										memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
										goto lb_exit2;
									}
									if(pBaseItem->BaseItem_UpGrade.wFormula[nValueLarge] == 0)
									{
										PickupItem.bEquipCode	= 0;
										Insert_UpgradeItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));	
										memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
										goto lb_exit2;
									}
								}
							}
							else if(nValue == ITEM_LIQUID_INDEX)
							{
								if(nValueLarge >= ITEM_WEAPONST_INDEX && nValueLarge <= ITEM_WEAPONEND_INDEX)
								{
									if( pBaseItemLarge->BaseItem_Weapon.bItemGrade != pBaseItem->BaseItem_LiQuid.wW_Grade )
									{
										PickupItem.bEquipCode	= 0;
										Insert_UpgradeItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));	
										memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
										goto lb_exit2;
									}
								}
								else if(nValueLarge >= ITEM_ARMORST_INDEX && nValueLarge <= ITEM_ARMOREND_INDEX)
								{
									if( pBaseItemLarge->BaseItem_Armor.bItemGrade != pBaseItem->BaseItem_LiQuid.wW_Grade )
									{
										PickupItem.bEquipCode	= 0;
										Insert_UpgradeItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));	
										memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
										goto lb_exit2;
									}
								}
							}
							else if(nValue == ITEM_EDITION_INDEX)
							{
								if( pBaseItem->BaseItem_Edition.wFormula[nValueLarge] == 0 )
								{
									PickupItem.bEquipCode	= 0;
									Insert_UpgradeItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));	
									memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
									goto lb_exit2;
								}
							}	
						}						
					}
					else
					{
						for(int i = 1; i < 4; i++)
						{
							if(pUser->m_pUpgradeItem[i].GetID() != 0)
							{
								int nValueSmall = pUser->m_pUpgradeItem[i].GetID() / ITEM_DISTRIBUTE;
								CBaseItem* pBaseItemSmall = g_pBaseItemHash->GetData(pUser->m_pUpgradeItem[i].GetID());

								if(pBaseItemSmall)
								{
									if(nValueSmall == ITEM_UPGRADE_INDEX && nValue == ITEM_LIQUID_INDEX)
									{
										if(pBaseItem->BaseItem_LiQuid.wW_Grade != pBaseItemSmall->BaseItem_UpGrade.wW_Grade)
										{
											PickupItem.bEquipCode	= 0;
											Insert_UpgradeItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));	
											memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
											goto lb_exit2;
										}
									}
									else if(nValueSmall == ITEM_LIQUID_INDEX && nValue == ITEM_UPGRADE_INDEX)
									{
										if(pBaseItem->BaseItem_UpGrade.wW_Grade != pBaseItemSmall->BaseItem_LiQuid.wW_Grade)
										{
											PickupItem.bEquipCode	= 0;
											Insert_UpgradeItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));	
											memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
											goto lb_exit2;
										}
									}
								}
							}
							
							if(pUser->m_pUpgradeItem[i].GetID() / ITEM_DISTRIBUTE == ITEM_UPGRADE_INDEX)
							{
								// ������ �����κ��� �������´�.
								if(nValue == ITEM_UPGRADE_INDEX)
								{
									bChk = TRUE;
									break;
								}	
							}
							if(pUser->m_pUpgradeItem[i].GetID() / ITEM_DISTRIBUTE == ITEM_LIQUID_INDEX)
							{
								// ������ �����κ��� �������´�.
								if(nValue == ITEM_LIQUID_INDEX)
								{
									bChk = TRUE;
									break;
								}
							}
							if(	pUser->m_pUpgradeItem[i].GetID() / ITEM_DISTRIBUTE == ITEM_EDITION_INDEX)
							{
								// ������ �����κ��� �������´�.
								if(nValue == ITEM_EDITION_INDEX)
								{
									bChk = TRUE;
									break;
								}	
							}
						}	
					}

					// ������ ���׷��̵� �����ۿ� �߰��Ѵ�.
					if(bChk==FALSE)
					{
						PickupItem.bEquipCode	= 0;
						Insert_UpgradeItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));
					}
					// ������ �����κ��� �������´�.
					else
					{
						PickupItem.bEquipCode	= 1;
						Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), ITEM_TYPE_SMALL, TRUE);
					}
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					goto lb_exit2;
				}
			}
		}
		break;
	case 68:
		{
			int nValue = GetItemIndex(&pUser->m_pUpgradeItem[pItemPickup->bZipCode]);

			if(pUser->m_pUpgradeItem[pItemPickup->bZipCode].GetQuantity()==0)
				return;

			if(pUser->m_ItemMouse.GetID()!=0)
				return;
			
			if(GetItemType(pUser->m_pUpgradeItem[pItemPickup->bZipCode].GetID())==ITEM_TYPE_SMALL)
			{
				if( nValue == ITEM_UPGRADE_INDEX || nValue == ITEM_LIQUID_INDEX || nValue == ITEM_EDITION_INDEX )
				{											
					PickupItem.bZipCode		= pItemPickup->bZipCode;					
					memcpy(&PickupItem.Item, &pUser->m_pUpgradeItem[pItemPickup->bZipCode], sizeof(CItem));
					memcpy(&pUser->m_ItemMouse, &pUser->m_pUpgradeItem[pItemPickup->bZipCode], sizeof(CItem));										
					memset(&pUser->m_pUpgradeItem[pItemPickup->bZipCode], 0, sizeof(CItem));
					goto lb_exit2;
				}
			}
		}
		break;
	case 69:
		{
//			int		nValue	= GetItemIndex(&pUser->m_pInv_Small[pItemPickup->bZipCode]);
			WORD	wId		= pUser->m_pInv_Small[pItemPickup->bZipCode].GetID();

			if(pUser->GetLevel() < 10)
				return;
			
			if(	wId==__ITEM_STATUS_INT__ ||
				(wId>=__ITEM_STATUS_INTSTART__ && wId<=__ITEM_STATUS_INTEND__) ||
				wId==__ITEM_SKILL_INT__ ||
				(wId>=__ITEM_SKILL_INTSTART__ && wId<=__ITEM_SKILL_INTEND__))
			{
				CBaseItem* pBaseItem = g_pBaseItemHash->GetData(wId);

				if(!pBaseItem)
					return;

				BYTE byCount = pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity();

				if(byCount>0)
				{
					memset(&ItemLog, 0, sizeof(CItem));
					memcpy(&ItemLog, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));

					if(byCount==1)
						memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
					else
						pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(BYTE(byCount-1));
					
					PickupItem.bZipCode	= pItemPickup->bZipCode;
					memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));

					if(wId==__ITEM_STATUS_INT__ || (wId>=__ITEM_STATUS_INTSTART__ && wId<=__ITEM_STATUS_INTEND__))
						pUser->InitStat(pBaseItem->BaseItem_Special.bClassGrade-50);
					else if(wId==__ITEM_SKILL_INT__ || (wId>=__ITEM_SKILL_INTSTART__ && wId<=__ITEM_SKILL_INTEND__))
						pUser->InitSkill(pBaseItem->BaseItem_Special.bClassGrade-50);
					
					goto lb_exit2;
				}					
			}				
		}
		break;
	case 70: 
		{		
			CItem cItem;
							
			PickupItem.bZipCode	= 0;

			if(pItemPickup->bZipCode==__GCTYPE_GUILD__)
			{// ��� ������ ��� ��ư �������� �Ϸ� ���� ;;					
				CreateItem(&cItem, ITEM_GUILD_INDEX, 1);
				memcpy(&PickupItem.Item, &cItem, sizeof(CItem));					
				Insert_SmallInvItem(pUser, &cItem, 0, 1, TRUE);
				goto lb_exit2;
			}
			else if(pItemPickup->bZipCode==__GCTYPE_CLEN__)
			{
				CreateItem(&cItem, ITEM_CLEN_INDEX, 1);
				memcpy(&PickupItem.Item, &cItem, sizeof(CItem));
				Insert_SmallInvItem(pUser, &cItem, 0, 1, TRUE);
				goto lb_exit2;
			}
			else
				return;
		}
		break;
	case 71:	// �ٴڿ��� ��Ʈ�� �̵�			
		{
 			if(!pLayer->GetSection( pItemPickup->bSectionNum))
				return;

			pItem = pLayer->GetSection(pItemPickup->bSectionNum)->m_pItemHash->GetData(pItemPickup->i64ItemID);
			
			// �̻��� �������̸�
			if(!pItem || pItem->Item.m_wItemID==0)
				return;

			// ���� ������ �̸�
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 71, DEF_COPYITEM_TYPE_SMALL, TRUE))
				goto lb_exit2;

			// ��ƼǮ ��� �������̰� �����ڰ� ��Ƽ���̸�
			if(pItem->dwPartyIndex != 0 && pUser->GetPartyID() == pItem->dwPartyIndex)
			{
				// ���� ����
			}
			// ���� �������̸�
			else if(pItem->dwOwnerIndex)
			{
				// ������ �����ڰ� �ƴϸ�
				if(pItem->dwOwnerIndex!=pUser->GetID())
				{
					// ���� �Ұ�
					return;	
				}
			}

			// ���� �������̸�
			if(GetItemType(pItem->Item.m_wItemID)==ITEM_TYPE_SMALL)				
			{
				if(pItem->Item.GetQuantity()==0)
					return;

				CItem cItem;
				memcpy(&cItem,&pItem->Item,sizeof(CItem));

				// ��Ƽ�� ���� ���� : �ִ��� 2005.1.26
				// ��Ƽ�� ���̰�, ��Ƽ�� �������� �̸�
				if(pUser->GetPartyID() && wMemberCount > 1 && pUser->m_PartyConfig.item == PARTY_CONFIG_ITEM_RANDOM)
				{
					// �������� ������ ���� �����ϰ� ����
					pUser = pMember[ rand() % wMemberCount ];

					// ��ĭ�� �ִ�� ���� �� �ִ� ���� ���Ѵ�
					CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pItem->Item.m_wItemID);
					if(!pBaseItem)
						return;
					int nMaxQuantity = pBaseItem->GetMaxQuantity();

					// ��Ʈ�� ��� ĭ�� ����
					for(int zip = 0;zip < MAX_BELT_POOL; zip++)
					{
						// ���� �� �ִ� ĭ�̸�
						if(	(pUser->m_pBelt[zip].m_wItemID == 0)
						|| 	(pUser->m_pBelt[zip].m_wItemID == pItem->Item.m_wItemID
							&& pUser->m_pBelt[zip].m_Item_Supplies.bQuantity + pItem->Item.GetQuantity() < nMaxQuantity))
						{
							break;
						}
					}

					// ��Ʈ�� ��������
					if(zip == MAX_BELT_POOL)
					{
						// ���� ���ϰ� �ٸ� ������� ��ȸ�� ����
						return;
					}

					pItemPickup->bZipCode = zip;

					// ���� : �ٸ� �����鿡�� �޼����� ������
					// "���� �� � �Ծ����ϴ�."
				}

#if defined TAIWAN_LOCALIZING
				// �븸�� ���� ���� 100% �̻�� �������� ������ �����Ѵ�.
				if (FALSE == pUser->IsPickUpItem()) 	
					return;
#endif

				BYTE bItemCnt = Insert_SmallBeltItem(pUser
					, &pItem->Item
					, pItemPickup->bZipCode
					, pItem->Item.GetQuantity()
					, FALSE);

				if(bItemCnt>0)
				{
					pItem->Item.SetQuantity(bItemCnt);
					bAppear = FALSE;
				}

				PickupItem.bZipCode	= pItemPickup->bZipCode;
				memcpy(&pItem->Item,&cItem,sizeof(CItem));
				memcpy(&PickupItem.Item, &pItem->Item, sizeof(CItem));

				// ������ �α� �߰� �迵�� 2005.02.23
				SetItemLog(&PickupItem.Item,ITEM_LOG_PICKUP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );

				goto lb_exit1;
			}
			// �ƴϸ�
			else
			{
				return;
			}
		}
		break;
	case 72:	// PlayerShop���� �춧    
		{							
			CUser* pShopUser = g_pUserHash->GetData(pItemPickup->dwMoney);
			
			if(!pShopUser)
			{					
				pShopPacket.byType	= 7;
				goto lb_shop;
			}

			ITEM_SHOP* pItemShop = g_pItemShopHash->GetData(pShopUser->GetID());

			if(pItemShop->byType!=1)
			{					
				pShopPacket.byType	= 8;				
				goto lb_shop;
			}

			if(pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetID()==0)
			{					
				pShopPacket.byType	= 9;										
				goto lb_shop;
			}

			if(pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetID()!=pItemPickup->dwId)
			{
				pShopPacket.byType	= 10;				
				goto lb_shop;
			}

			if(pShopUser->m_sPlayerShop.dwCustomSellPrice[pItemPickup->bZipCode]!=pItemPickup->dwShop)
			{
				pShopPacket.byType	= 19;
				goto lb_shop;
			}

			if(SetIsCopyItem(pUser
				, &PickupItem
				, &pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode]
				, 72
				, DEF_COPYITEM_TYPE_LARGE))
			{
				goto lb_exit2;
			}
			
			__int64	nMoney		= (__int64)pUser->m_dwMoney;
			__int64	nPrice		= (__int64)pShopUser->m_sPlayerShop.dwCustomSellPrice[pItemPickup->bZipCode];

			if( (nMoney - nPrice) >=0 )
			{
				__int64	nShopMoney	= pShopUser->m_dwMoney;

				if(nShopMoney + nPrice <= __MAX_MONEY__)
				{
					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].GetID() == 0)
						{						
							CItem cItem;
							memset(&cItem, 0, sizeof(CItem));

							PlayerShopPriceLog(pShopUser
								, &cItem
								, 72
								, pItemPickup->bZipCode
								, DWORD(nPrice)
								, 0
								, TRUE
								, pUser->GetID()
								, DWORD(nMoney-nPrice)
								, i
								, pItemPickup->bZipCode);

							return;								
						}
					}

					pShopPacket.byType	= 13;
					goto lb_shop;
				}
				else
				{						
					pShopPacket.byType	= 12;
					goto lb_shop;
				}
			}
			else
			{
				pShopPacket.byType	= 11;
				goto lb_shop;
			}				
		}
		break;
	case 73:
		{
			CUser* pShopUser = g_pUserHash->GetData(pItemPickup->dwMoney);
			
			if(!pShopUser)
			{					
				pShopPacket.byType	= 7;
				goto lb_shop;
			}

			ITEM_SHOP* pItemShop = g_pItemShopHash->GetData(pShopUser->GetID());

			if(pItemShop->byType!=1)
			{					
				pShopPacket.byType	= 8;				
				goto lb_shop;
			}

			if(pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetID()==0)
			{					
				pShopPacket.byType	= 9;										
				goto lb_shop;
			}

			if(pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetID()!=pItemPickup->dwId)
			{
				pShopPacket.byType	= 10;				
				goto lb_shop;
			}

			if(pShopUser->m_sPlayerShop.dwCustomSellPrice[pItemPickup->bZipCode]!=pItemPickup->dwShop)
			{
				pShopPacket.byType	= 19;
				goto lb_shop;
			}

			if(pItemPickup->bZipCode2 == 0)
			{
				pShopPacket.byType	= 21;
				goto lb_shop;
			}

			if(SetIsCopyItem(pUser, &PickupItem, &pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode], 73, DEF_COPYITEM_TYPE_SMALL))
				goto lb_exit2;

			__int64	nMoney		= (__int64)pUser->m_dwMoney;
			__int64	nPrice		= (__int64)pShopUser->m_sPlayerShop.dwCustomSellPrice[pItemPickup->bZipCode] * pItemPickup->bZipCode2;

			if((nMoney-nPrice)>=0)
			{
				__int64	nShopMoney	= pShopUser->m_dwMoney;

				if(nShopMoney+nPrice<=__MAX_MONEY__)
				{						
					CItem cItem[MAX_INV_SMALL_POOL];
					CItem cInsertItem;

					for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
					{
						memcpy(&cItem[i], &pUser->m_pInv_Small[i], sizeof(CItem));
					}

					if(pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetQuantity()<pItemPickup->bZipCode2)
					{
						pItemPickup->bZipCode2 = pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetQuantity();
					}

					memcpy(&cInsertItem, &pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode], sizeof(CItem));
					cInsertItem.SetQuantity(pItemPickup->bZipCode2);												

					if(Insert_SmallInvItem2(cItem, &cInsertItem, 0, cInsertItem.GetQuantity(), TRUE)==0)
					{
						CItem cPriceItem;

						if(pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetQuantity()==pItemPickup->bZipCode2)
						{
							memset(&cPriceItem, 0, sizeof(CItem));
						}
						else
						{
							memcpy(&cPriceItem, &pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode], sizeof(CItem));
							cPriceItem.SetQuantity(
								BYTE(pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetQuantity()-pItemPickup->bZipCode2));
						}								
						
						PlayerShopPriceLog(pShopUser
							, &cPriceItem
							, 73
							, pItemPickup->bZipCode
							, DWORD(nPrice)
							, 0
							, TRUE
							, pUser->GetID()
							, DWORD(nMoney-nPrice)
							, pItemPickup->bZipCode
							, pItemPickup->bZipCode2);

						return;							
					}
					
					pShopPacket.byType	= 13;
					goto lb_shop;
				}
				else
				{						
					pShopPacket.byType	= 12;
					goto lb_shop;
				}
			}
			else
			{
				pShopPacket.byType	= 11;
				goto lb_shop;
			}				
		}
		break;
	case 74:
		{
			CItem* pItem  = NULL;
			
			if(pItemPickup->bEquipCode==0)
			{
				if(pItemPickup->bZipCode>=MAX_INV_SMALL_POOL)
					return;

				int nValue = pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()/ITEM_DISTRIBUTE;
				
				if(nValue<ITEM_CONSUMABLEST_INDEX || nValue>ITEM_CONSUMABLEEND_INDEX)
					return;

				for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
				{
					if(pUser->m_pInv_Small[i].GetID()==pUser->m_pInv_Small[pItemPickup->bZipCode].GetID())
					{
						if(pUser->m_pInv_Small[i].m_Item_Consumable.bInvIndex>=1)							
							return;
					}

					// ���� ����� ���� �������̸�
					if(IsSamePropertyConsumableItem(&pUser->m_pInv_Small[i], &pUser->m_pInv_Small[pItemPickup->bZipCode]))
					{
						if(pUser->m_pInv_Small[i].m_Item_Consumable.bInvIndex>=1)							
							return;
					}
				}

				pItem = &pUser->m_pInv_Small[pItemPickup->bZipCode];
			}
			else
				return;
			
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pItem->GetID());
			if (pItem->m_Item_Consumable.bInvIndex || pUser->GetLevel() > pBaseItem->BaseItem_Consumable.wMax_Lev)
			{
				return;
			}		
			
			pItem->m_Item_Consumable.dwEndDate = GetDueDay(pBaseItem->BaseItem_Consumable.wLast_Time);
			pItem->m_Item_Consumable.bInvIndex = pItemPickup->bZipCode+1;
			pUser->AttachItem(pItem);
			
			//== ==//			
			PickupItem.bZipCode		= pItemPickup->bZipCode;
			PickupItem.bEquipCode	= pItemPickup->bEquipCode;
			memcpy(&PickupItem.Item, pItem, sizeof(CItem));
			goto lb_exit2;
		}
		break;
	case 75:	// ��� ���� 
		{
			int nValue = GetItemIndex(&pUser->m_pInv_Small[pItemPickup->bZipCode]);

			if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()==0)
				return;

			if(!pUser->GetCurDungeon())
				return;
			
			if(pUser->IsCurDungeonSiege())
				return;				

			if(nValue==ITEM_MAGICARRAY_INDEX)
			{
				WORD wItemId = pUser->m_pInv_Small[pItemPickup->bZipCode].GetID();

				if(wItemId==__ITEM_GUILD_RECALL_INDEX__)
				{						
					// ���� ������ ������ //
					WSTDS_GUILD_ALLPORTAL pSendPacket;						
					pSendPacket.dwGuildId	= pUser->m_dwGuildId;						
					pSendPacket.dwUserIndex	= pUser->GetID();
					pSendPacket.byIndex		= pItemPickup->bZipCode;
					
					g_pNet->SendToServer(WSINDEX, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
											
					if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()>1)
						pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(
							BYTE(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()-1));
					else
						memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));

					PickupItem.bZipCode		= pItemPickup->bZipCode;
					memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));
					goto lb_exit2;
				}
				else
				{
					return;
				}
			}
		}
		break;
	case 76:
		{
			CUser* pSendUser = g_pUserHash->GetData(pItemPickup->dwMoney);

			if(pSendUser)
			{
				if(!pSendUser->GetCurDungeon() || !pUser->GetCurDungeon())
				{
					// ���� //
					DSTC_GUILD_PORTALFAIL pSendPacket;
					memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
					__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					pSendPacket.byResult = 2;
					NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					
					memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
					__lstrcpyn(pSendPacket.szName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					pSendPacket.byResult = 2;
					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}

				if(pSendUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID == 
					pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID)
				{
					if(pSendUser->GetCurLayerIndex()==pUser->GetCurLayerIndex())
					{
						// ���� //
						DSTC_GUILD_PORTALFAIL pSendPacket;
						memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
						__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
						pSendPacket.byResult = 5;
						NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						
						memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
						__lstrcpyn(pSendPacket.szName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
						pSendPacket.byResult = 5;
						NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						return;
					}
				}
				
/*
				// Item Chk //
				if(	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL1__ ||
					pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL2__)
				{*/

					// ��Ż �̵� �޼��� //
					DSTC_PARTY_PORTALMOVE pSendPacket;
					pSendPacket.wDungeonID		= WORD(pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID);
					pSendPacket.byLayerIndex	= pUser->GetCurLayerIndex();
					pSendPacket.dwPartyId		= pUser->m_dwPartyId;
					pSendPacket.fPosX			= pUser->GetCurPosition()->x;
					pSendPacket.fPosZ			= pUser->GetCurPosition()->y;						
					NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION );
					
					// ������ �޼��� //						
					if(pItemPickup->bZipCode2==1)
					{
						BYTE byCount = pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity();

						if(	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL1__ ||
							pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL2__)
						{
							if(byCount>1)
								pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(BYTE(byCount-1));
							else
								memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
						}
					}
					
					PickupItem.bZipCode	= pItemPickup->bZipCode;
					memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));												

					CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(PickupItem.Item.m_Item_Zodiac.wMapId);

					if(pDungeon)
						goto lb_exit2;

					CheckItemSlot(pUser);
					
					if(pUser->m_ItemGuardian.GetID()!=0)
					{
						Insert_SmallInvItem(pUser, &pUser->m_ItemGuardian, 0, pUser->m_ItemGuardian.GetQuantity(), TRUE);
						memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));
					}

					goto lb_exit2;
/*
				}
				else
				{
					// ���� //
					DSTC_PARTY_PORTALFAIL pSendPacket;
					memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
					__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					pSendPacket.byResult = 3;
					NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

					memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
					__lstrcpyn(pSendPacket.szName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					pSendPacket.byResult = 3;
					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}*/

			}
			else
			{
/*
				// Item Chk //
				if(	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL1__ ||
					pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL2__)
				{*/

					// ���� ��Ż �̵� �޼��� //
					WSTDS_PARTY_PORTALMOVE pSendPacket;
					pSendPacket.wDungeonID			= WORD(pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID);
					pSendPacket.byLayerIndex		= pUser->GetCurLayerIndex();
					pSendPacket.dwPartyId			= pUser->m_dwPartyId;
					pSendPacket.fPosX				= pUser->GetCurPosition()->x;
					pSendPacket.fPosZ				= pUser->GetCurPosition()->y;
					pSendPacket.dwUserIndex			= pUser->GetID();
					pSendPacket.dwPortalUserIndex	= pItemPickup->dwMoney;
					g_pNet->SendToServer(WSINDEX, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					
					// ������ �޼��� //
					if(pItemPickup->bZipCode2==1)
					{
						BYTE byCount = pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity();

						if(	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL1__ ||
							pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL2__)
						{
							if(byCount>1)
								pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(BYTE(byCount-1));
							else
								memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
						}
					}
					
					PickupItem.bZipCode	= pItemPickup->bZipCode;
					memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));												

					CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(PickupItem.Item.m_Item_Zodiac.wMapId);

					if(pDungeon)
						goto lb_exit2;

					CheckItemSlot(pUser);
					
					if(pUser->m_ItemGuardian.GetID()!=0)
					{
						Insert_SmallInvItem(pUser, &pUser->m_ItemGuardian, 0, pUser->m_ItemGuardian.GetQuantity(), TRUE);
						memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));
					}
					goto lb_exit2;
/*
				}
				else
				{
					// ���� //
					WSTDS_GUILD_PORTALFAIL pSendServerPacket;						
					memset(pSendServerPacket.szName, 0, sizeof(pSendServerPacket.szName));
					__lstrcpyn(pSendServerPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					pSendServerPacket.byResult		= 3;
					pSendServerPacket.dwUserIndex	= pItemPickup->dwMoney;
					g_pNet->SendToServer(WSINDEX, (char*)&pSendServerPacket, pSendServerPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}*/

			}
		}
		break;
	case 77:	//������ ��ǥ ������ //�±ر� ������
		{
			CItem* pInv=0;

			if(pItemPickup->bEqiupCode2 == 1 )
				pInv = &pUser->m_pBelt[pItemPickup->bZipCode];
			else
				pInv = &pUser->m_pInv_Small[pItemPickup->bZipCode];

			int nValue = pInv->m_wItemID/ITEM_DISTRIBUTE;
			
			if(nValue==0)
				return;

			//  ��������  �迵�� 2005.02.02
			if(pInv->m_wItemID ==  __ITEM_HEROITEM_INDEX__)
			{
				WORD wItemQuantity=0;
				WORD wPresentID = UnpackPresentBox(pInv, &wItemQuantity);

				if(!wPresentID)	//�������� Ǯ�� ���� 
				{
					DSTC_ITEM_PICKUP	Error;
					memset( &Error, 0, sizeof( DSTC_ITEM_PICKUP ) );
					Error.bStatus	= UPDATE_GAME_PLAY;
					Error.bHeader	= Protocol_DSTC::CMD_ITEM_PICKUP;
					Error.bInv = 78;
					Error.bEqiupCode2 = 252;	//�������� Ǯ�� ����!!
					Log(LOG_IMPORTANT, "Pre  : Error Code 252 ");
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&Error, Error.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}
				
				DWORD dwEmpty=0;
				DWORD dwEmptyPos=0;
				nValue = wPresentID / ITEM_DISTRIBUTE;

				if(GetItemType(wPresentID)==ITEM_TYPE_SMALL)
				{
//					dwEmpty = 0;
					for(int i=0; i<MAX_INV_SMALL_POOL; i++)
					{
						if(pUser->m_pInv_Small[i].GetID() == 0) 
						{
							dwEmpty++;
							dwEmptyPos = i;
							break;
						}
					}
				}
				else if(GetItemType(wPresentID)==ITEM_TYPE_LARGE)
				{
//					dwEmpty = 0;
					for(int i=0; i<MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].GetID() == 0) 
						{
							dwEmpty++;
							dwEmptyPos = i;
							break;
						}
					}
				}

				if(!dwEmpty)	//�κ��丮�� ��ĭ�� ���� 
				{
					DSTC_ITEM_PICKUP	Error;
					memset( &Error, 0, sizeof( DSTC_ITEM_PICKUP ) );
					Error.bStatus	= UPDATE_GAME_PLAY;
					Error.bHeader	= Protocol_DSTC::CMD_ITEM_PICKUP;
					Error.bInv = 78;
					Error.bEqiupCode2 = 253;	//��ĭ����!!
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&Error, Error.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}

				char szQuery[0xff]={0,};
				wsprintf(szQuery, "CZP_CHECK_PRESENT_COUNT %d,%d,%d", wPresentID, wItemQuantity, dwEmptyPos);
				g_pDb->THOpenRecord(szQuery, DB_PRESENT_COUNT_CHECK, (void*)pUser, 1, GAME_DB);


				int nCount = pInv->GetQuantity();

				if(nCount>1)
					pInv->SetQuantity(BYTE(nCount-1));
				else if(nCount==1)
					memset(pInv, 0, sizeof(CItem));

				PickupItem.bInv		= 77;
				PickupItem.bZipCode	= pItemPickup->bZipCode;
				PickupItem.bEqiupCode2 = pItemPickup->bEqiupCode2;
				memcpy(&PickupItem.Item, pInv, sizeof(CItem));
	
				// ������ �α� �߰� �迵�� 2005.02.23
				SetItemLog(&PickupItem.Item, ITEM_LOG_SEAL_OF_HERO, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
				goto lb_exit2;
			}
		}
		break;		
	case 79:
		{
			int nValue = GetItemIndex(&pUser->m_ItemMouse);

			if(pUser->m_ItemMouse.GetQuantity()==0)
				return;
			
			if(pUser->m_pMakingItem[pItemPickup->bZipCode].GetID()!=0)			
				return;

			if(GetItemType(pUser->m_ItemMouse.GetID())==ITEM_TYPE_SMALL)
			{
				if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 79, DEF_COPYITEM_TYPE_SMALL, TRUE))
					goto lb_exit2;									
			
				if(	(nValue>=ITEM_MATERIALST_INDEX && nValue<=ITEM_MATERIALEND_INDEX) ||
					pUser->m_ItemMouse.GetID() == __ITEM_MAKING_INDEX__ )
				{					
					PickupItem.bZipCode		= pItemPickup->bZipCode;
					PickupItem.dwMoney		= pItemPickup->dwMoney;
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

					// ������ ���� �����ۿ� �߰��Ѵ�.
					PickupItem.bEquipCode	= 0;
					Insert_MakingItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));
				}						
				// ������ �����κ��� �������´�.
				else
				{
					PickupItem.bEquipCode	= 1;
					Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity()
						, ITEM_TYPE_SMALL, TRUE);
				}

				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit2;					
			}
		}
		break;
	case 80:
		{
			int nValue = GetItemIndex(&pUser->m_pMakingItem[pItemPickup->bZipCode]);

			if(pUser->m_pMakingItem[pItemPickup->bZipCode].GetQuantity()==0)
				return;

			if(pUser->m_ItemMouse.GetID()!=0)
				return;
			
			if(GetItemType(pUser->m_pMakingItem[pItemPickup->bZipCode].GetID())==ITEM_TYPE_SMALL)
			{
				if( (nValue >= ITEM_MATERIALST_INDEX && nValue <= ITEM_MATERIALEND_INDEX) ||
					pUser->m_pMakingItem[pItemPickup->bZipCode].GetID() == __ITEM_MAKING_INDEX__)
				{											
					PickupItem.bZipCode		= pItemPickup->bZipCode;					
					memcpy(&PickupItem.Item, &pUser->m_pMakingItem[pItemPickup->bZipCode], sizeof(CItem));
					memcpy(&pUser->m_ItemMouse, &pUser->m_pMakingItem[pItemPickup->bZipCode], sizeof(CItem));										
					memset(&pUser->m_pMakingItem[pItemPickup->bZipCode], 0, sizeof(CItem));
					goto lb_exit2;
				}
			}
		}
		break;
	case 81:// �����ۻ���â���� ���콺�� �̵� 
		{
			int nValue = GetItemIndex(&pUser->m_pMakingItem[0]);

			if(pUser->m_ItemMouse.GetID()!=0)
				return;
			
			if(GetItemType(pUser->m_pMakingItem[0].GetID())==ITEM_TYPE_LARGE)
			{
				// ��������� ������ �������� �ȶ������� ���� ����
				// 2005.01.25 �迵��  nValue == ITEM_BAG_INDEX �߰�
				if(	(nValue>=ITEM_WEAPONST_INDEX && nValue<= ITEM_WEAPONEND_INDEX) ||
					( nValue>=ITEM_ARMORST_INDEX && nValue<= ITEM_ARMOREND_INDEX)  ||
					nValue == ITEM_RIDE_INDEX || 
					nValue == ITEM_GUARDIAN_INDEX ||
					nValue == ITEM_BAG_INDEX ) 
				{											
					PickupItem.bZipCode		= 0;					
					memcpy(&PickupItem.Item, &pUser->m_pMakingItem[0], sizeof(CItem));
					memcpy(&pUser->m_ItemMouse, &pUser->m_pMakingItem[0], sizeof(CItem));										
					memset(&pUser->m_pMakingItem[0], 0, sizeof(CItem));
					goto lb_exit2;
				}
			}				
		}
		break;
	//case 82: �����
	case 83:	// ����� ������ ������ ���� �ȶ� 
		{
			if(pUser->m_ItemMouse.m_wItemID==0)
				return;

			int nValue = GetItemIndex(&pUser->m_ItemMouse);
			
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 34, DEF_COPYITEM_TYPE_LARGE, TRUE))
				goto lb_exit2;
					
			if(nValue==ITEM_GUARDIAN_INDEX)				
			{
				BOOL		bChk		= FALSE;
				CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(pUser->m_ItemMouse.m_wItemID);

				if(!pBaseItem)
					break;
				
				__int64		nMoney		= pUser->m_dwMoney;
				__int64		nSellPrice	= pBaseItem->BaseItem_Guardian.dwSellPrice;

				if(nMoney+nSellPrice<=__MAX_MONEY__)
				{
					pUser->m_dwMoney		= DWORD(nMoney+nSellPrice); 
					PickupItem.dwMoney		= DWORD(nMoney+nSellPrice);	
					PickupItem.bEquipCode	= 0;						
					bChk = TRUE;
				}
				else
				{
					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].m_wItemID==0)
						{
							if(bChk==FALSE)
							{								
								PickupItem.bZipCode		= i;
								PickupItem.bEquipCode	= 1;

								memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
								memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
								bChk = TRUE;
							}
						}
					}
				
					if(bChk==FALSE)
					{
						AppearItem(pUser, &pUser->m_ItemMouse, 0, 0, ITEM_TYPE_LARGE);
						PickupItem.bEquipCode = 2;
					}
				}
				
				// ���콺���� ���� //
				memcpy( &ItemLog, &pUser->m_ItemMouse, sizeof( CItem ) );
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));

				if(bChk==TRUE)
					goto lb_exit2;
			}
		}
		break;
	case 84:	// ���� ������ ������ �ȶ� 
		{
			if(pUser->m_ItemMouse.m_wItemID==0)
				return;

			int nValue = GetItemIndex(&pUser->m_ItemMouse);
			
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 34, DEF_COPYITEM_TYPE_LARGE, TRUE))
				goto lb_exit2;
					
			if(nValue==ITEM_BAG_INDEX)				
			{
				BOOL		bChk		= FALSE;
				CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(pUser->m_ItemMouse.m_wItemID);

				if(!pBaseItem)
					break;
				
				__int64		nMoney		= pUser->m_dwMoney;
				__int64		nSellPrice	= pBaseItem->BaseItem_Bag.dwSellPrice;

				if(nMoney+nSellPrice<=__MAX_MONEY__)
				{
					pUser->m_dwMoney		= DWORD(nMoney+nSellPrice); 
					PickupItem.dwMoney		= DWORD(nMoney+nSellPrice);	
					PickupItem.bEquipCode	= 0;						
					bChk = TRUE;
				}
				else
				{
					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].m_wItemID==0)
						{
							if(bChk==FALSE)
							{								
								PickupItem.bZipCode		= i;
								PickupItem.bEquipCode	= 1;

								memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
								memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
								bChk = TRUE;
							}
						}
					}
				
					if(bChk==FALSE)
					{
						AppearItem(pUser, &pUser->m_ItemMouse, 0, 0, ITEM_TYPE_LARGE);
						PickupItem.bEquipCode = 2;
					}
				}
				
				// ���콺���� ���� //
				memcpy( &ItemLog, &pUser->m_ItemMouse, sizeof( CItem ) );
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));

				if(bChk==TRUE)
					goto lb_exit2;
			}
		}
		break;	
	case 85: // ���� ������ �춧  
		{
			if(pItemPickup->dwId >= 3600 && pItemPickup->dwId <= 3618)				
			{
				BOOL		bChk		= FALSE;
				CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(pItemPickup->dwId);
				
				if(!pBaseItem)
					break;

				if(!CheckStore(pItemPickup->dwId))
					return;

				__int64		nMoney		= pUser->m_dwMoney;
				__int64		nPrice		= pBaseItem->BaseItem_Bag.dwPrice;

				if(nMoney-nPrice>=0)
				{
					if(pUser->m_pInv_Large[pItemPickup->bZipCode].m_wItemID==0)
					{
						CreateItem(&pUser->m_pInv_Large[pItemPickup->bZipCode], WORD(pItemPickup->dwId), 0, Store_Flag_In_Store);
						memcpy(&PickupItem.Item, &pUser->m_pInv_Large[pItemPickup->bZipCode], sizeof(CItem));
						pUser->m_dwMoney = DWORD(nMoney - nPrice);
						
						PickupItem.bEquipCode	= 1;
						PickupItem.bZipCode		= pItemPickup->bZipCode;
						PickupItem.dwId			= pItemPickup->dwId;
						PickupItem.dwMoney		= pUser->m_dwMoney;
						
						bChk = TRUE;
					}
					else
					{						
						for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
						{
							if(pUser->m_pInv_Large[i].m_wItemID==0)
							{
								if(bChk==FALSE)
								{
									CreateItem(&pUser->m_pInv_Large[i], WORD(pItemPickup->dwId), 0, Store_Flag_In_Store);
									memcpy(&PickupItem.Item, &pUser->m_pInv_Large[i], sizeof(CItem));
									pUser->m_dwMoney = DWORD(nMoney - nPrice);
									bChk = TRUE;
									
									PickupItem.bZipCode		= i;
									PickupItem.bEquipCode	= 1;
									PickupItem.dwId			= pItemPickup->dwId;
									PickupItem.dwMoney		= pUser->m_dwMoney;
								}
							}
						}
					}
				
					if(bChk==FALSE)
					{
						// ���� //						
						PickupItem.bEquipCode	= 2;
						PickupItem.dwId			= pItemPickup->dwId;
					}						
				}
				else
				{
					// ���� //
					PickupItem.bEquipCode	= 3;
					PickupItem.dwId			= pItemPickup->dwId;
				}
				
				goto lb_exit2;
			}				
		}
		break;
	case 100:			// ������ ���� 
		{
			CItem* pItem  = NULL;

			if(pItemPickup->bEquipCode==0)
			{
				if(pItemPickup->bZipCode>=MAX_INV_SMALL_POOL)
					return;

				if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetID() != __ITEM_INSURANCE_INDEX__)
					return;					

				for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
				{
					if(pUser->m_pInv_Small[i].GetID()==pUser->m_pInv_Small[pItemPickup->bZipCode].GetID())
					{
						if(pUser->m_pInv_Small[i].m_Item_Zodiac.bInvIndex>=1)							
							return;
					}						
				}

				pItem = &pUser->m_pInv_Small[pItemPickup->bZipCode];
			}
			else 
				return;


			pItem->m_Item_Zodiac.bInvIndex = pItemPickup->bZipCode+1;
			pUser->AttachItem(pItem);

			memset(&ItemLog, 0, sizeof(CItem));
			memcpy(&ItemLog, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));

						
			PickupItem.bZipCode		= pItemPickup->bZipCode;
			PickupItem.bEquipCode	= pItemPickup->bEquipCode;
			memcpy(&PickupItem.Item, pItem, sizeof(CItem));
			goto lb_exit2;
		}
		break;
	case 101:
		{
			int nValue = GetItemIndex(&pUser->m_ItemGuardian);

			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemGuardian.m_wItemID);

			if(!pBaseItem)
				return;		
			
			if (!pUser->m_pInv_Guardian[0].GetID())
				return;	
			
			if(nValue >= ITEM_CONSUMABLEST_INDEX && nValue <= ITEM_CONSUMABLEEND_INDEX)
			{
				if(pUser->m_ItemGuardian.GetID() >= 20519 && pUser->m_ItemGuardian.GetID() <= 20578)
				{
					int nCount = pUser->m_ItemGuardian.GetQuantity();

					if(nCount==0)
						return;	

					CMonster* pMonster = pUser->GetGuardian(0);
					if(pMonster)
					{
						for(int i = 0; i < MAX_BASEITEM_CONSUMABLE_ITEMATTR; ++i)
						{
							if(pBaseItem->BaseItem_Consumable.sItem_Attr[i].wAttr_Code)
							{
								pMonster->SetItemAttr(
													  (ITEM_ATTR_CODE)pBaseItem->BaseItem_Consumable.sItem_Attr[i].wAttr_Code
													, pBaseItem->BaseItem_Consumable.sItem_Attr[i].wAttr_Value
													);

								DWORD dwVirtualCode = 
									g_pItemAttrLayer->GetVirtualCode(
															  pBaseItem->BaseItem_Consumable.sItem_Attr[i].wAttr_Code
															, pBaseItem->BaseItem_Consumable.sItem_Attr[i].wAttr_Value
															);

								// formula 6�� �� ��ų���� �÷��ִ°��̱� ������..
								if (g_pItemAttrLayer->m_ItemAttrDefine[dwVirtualCode].wFormula == 6)
								{
									pMonster->SetSkillLevel(
														  g_pItemAttrLayer->m_ItemAttrDefine[dwVirtualCode].bSkillID
														, pMonster->GetItemAttr((ITEM_ATTR_CODE)dwVirtualCode)
														);
								}
							}
						}
						
						if(nCount>1)
							pUser->m_ItemGuardian.SetQuantity(BYTE(nCount-1));
						else if(nCount==1)
							memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));				

						memcpy(&PickupItem.Item, &pUser->m_ItemGuardian, sizeof(CItem));
						goto lb_exit2;
					}
				}
			}
		}
		break;
	}
	return;

lb_exit1: 	// Item DisAppear �� �����Ѵ�.

	// �ݱ⸦ �����ߴٸ�.
	if( PickupItem.bInv == 0 )
	{
		return;	
	}

	memcpy( &PickupItem.Item, &pItem->Item, sizeof( CItem ) );
	NetSendToUser( pUser->GetConnectionIndex(), (char*)&PickupItem, PickupItem.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

	// �������� ���������� �˷��� �Ѵ�.
	if(bAppear)
	{
		DisappearItem.i64ItemID = pItem->Item.m_Serial.i64Serial;
		pLayer->BroadCastSectionMsg( (char*)&DisappearItem, DisappearItem.GetPacketSize(), pItemPickup->bSectionNum );
	
		// ���������� �������� �����Ѵ�.
		if( pLayer->RemoveItem( pItem ) == false ) __asm int 3;
	}

	CheckSpeed(pUser);
	
	goto lb_Item_log;

lb_exit2:

	// �ݱ⸦ �����ߴٸ�.
	if( PickupItem.bInv == 0 )
	{
		return;	
	}

	NetSendToUser( pUser->GetConnectionIndex(), (char*)&PickupItem, PickupItem.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

	CheckSpeed(pUser);
	
	goto lb_Item_log;

lb_shop:
	
	NetSendToUser( pUser->GetConnectionIndex(), (char*)&pShopPacket, pShopPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

lb_Item_log:
	
	int  iCount = 0;

	// �ݱ⸦ �����ߴٸ�.
	if( PickupItem.bInv == 0 )
	{
		return;	
	}

	switch( PickupItem.bInv )
	{
	// ����.---------------------------------------------------------------------------------------
	case 1:	// �ٴڿ��� ū������ â����.
		{
//			int nValue = pItem->Item.m_wItemID/ITEM_DISTRIBUTE;

			if(GetItemType(pItem->Item.m_wItemID)==ITEM_TYPE_MONEY)
			{
				if(g_pThis->GetServerType()==SERVER_TYPE_VILLAGE)
				{
					SetItemLogKarz(	ITEM_LOG_KARZ_PICKUP,pLayer->GetParent()->GetID(),pUser->m_szName,NULL,pItem->Item.m_Item_Money.dwMoney);
				}
				else
				{
					if(pItem->Item.m_Item_Money.dwMoney >= 10000)
					{
						SetItemLogKarz(	ITEM_LOG_KARZ_PICKUP,pLayer->GetParent()->GetID(),pUser->m_szName,NULL,pItem->Item.m_Item_Money.dwMoney);
					}
				}				
			}
			else	//kukza 041004
			{
				if( pItem ) // �ϴ� �ڵ带 �� ����..
				{
					SetItemCount( pItem->Item.m_wItemID, pLayer->GetParent()->GetID(), pItem->Item.GetQuantity() );
					SetItemLog( &pItem->Item, ITEM_LOG_PICKUP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
				}
				else
				{
					SetItemCount( PickupItem.Item.m_wItemID, pLayer->GetParent()->GetID(), PickupItem.Item.GetQuantity() );
					SetItemLog( &PickupItem.Item, ITEM_LOG_PICKUP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
				}
			}
		}
		break;
	case 2:	// �ٴڿ��� ���������� â����.
	case 5: // �ٴڿ��� ���콺�� �̵�.
	case 12: // ���� �������� �ٴڿ��� ���콺���� �̵��ҋ�.
		{
			if( pItem ) // �ϴ� �ڵ带 �� ����..
			{
				SetItemCount( pItem->Item.m_wItemID, pLayer->GetParent()->GetID(), pItem->Item.GetQuantity() );
				SetItemLog( &pItem->Item, ITEM_LOG_PICKUP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
			}
			else
			{
				SetItemCount( PickupItem.Item.m_wItemID, pLayer->GetParent()->GetID(), PickupItem.Item.GetQuantity() );
				SetItemLog( &PickupItem.Item, ITEM_LOG_PICKUP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
			}
		}
		break;
	
	case 19: // �����ۻ����� �������� �싚.
	case 35: // ���� ������ �싚.
	case 37: // �� ������ �싚.
		{
			if( pItem ) // �ϴ� �ڵ带 �� ����..
			{
				SetItemCount( pItem->Item.m_wItemID, pLayer->GetParent()->GetID(), pItem->Item.GetQuantity() );
				SetItemLog( &pItem->Item, ITEM_LOG_CREATE_BY_SHOP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
			}
			else
			{
				SetItemCount( PickupItem.Item.m_wItemID, pLayer->GetParent()->GetID(), PickupItem.Item.GetQuantity() );
				SetItemLog( &PickupItem.Item, ITEM_LOG_CREATE_BY_SHOP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
			}
		}
		break;
 
	// ����.---------------------------------------------------------------------------------------	
	case 28: // ���콺���� ������ �������� �ٴڿ� ��������.		//kukza 041004
	case 10: // �������� �ٴ����� �̵�.
		{
			iCount = pUser->m_ItemMouse.GetQuantity() * -1;
			SetItemCount( ItemLog.m_wItemID, pLayer->GetParent()->GetID(), iCount );
			SetItemLog( &ItemLog, ITEM_LOG_DROP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
		}
		break;
 
	case 18: // �����ۻ����� �������� �ȋ�.
	case 34: // ���� ������ �ȋ�.
	case 36: // �� ������ �ȋ�.
	case 83: // ����� ������ �ȶ�.
		{
			if( pItem ) // �ϴ� �ڵ带 �� ����..
			{
				iCount = ItemLog.GetQuantity() * -1;
				SetItemCount( ItemLog.m_wItemID, pLayer->GetParent()->GetID(), iCount );
				SetItemLog( &ItemLog, ITEM_LOG_DELETE_BY_SHOP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
				if(ItemLog.GetID() != 0)
				{
					CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(ItemLog.GetID());
					if(pBaseItem)
					{
						DWORD dwSellPrice = pBaseItem->GetSellPrice();
						SetItemLogKarz(	ITEM_LOG_DELETE_BY_SHOP,pLayer->GetParent()->GetID(),pUser->m_szName,NULL,dwSellPrice,&ItemLog);
					}					
				}
			}
			else
			{
				iCount = ItemLog.GetQuantity() * -1;
				SetItemCount( ItemLog.m_wItemID, pLayer->GetParent()->GetID(), iCount );
				SetItemLog( &ItemLog, ITEM_LOG_DELETE_BY_SHOP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
				if(ItemLog.GetID() != 0)
				{
					CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(ItemLog.GetID());
					if(pBaseItem)
					{
						DWORD dwSellPrice = pBaseItem->GetSellPrice();
						SetItemLogKarz(	ITEM_LOG_DELETE_BY_SHOP,pLayer->GetParent()->GetID(),pUser->m_szName,NULL,dwSellPrice,&ItemLog);
					}					
				}
			}
		}
		break;

//	case 28: // ���콺���� ������ �������� �ٴڿ� ��������. 
//		break;
	case 13: // ���� ������.
		{
			if( pItem ) // �ϴ� �ڵ带 �� ����..
			{
				iCount = pItem->Item.GetQuantity() * -1;
				SetItemCount( pItem->Item.m_wItemID, pLayer->GetParent()->GetID(), iCount );
			}
			else
			{
				iCount = PickupItem.Item.GetQuantity() * -1;
				SetItemCount( PickupItem.Item.m_wItemID, pLayer->GetParent()->GetID(), iCount );

				SetItemLogKarz(	ITEM_LOG_KARZ_DROP,pLayer->GetParent()->GetID(),pUser->m_szName,NULL,pItemPickup->dwMoney);
			}
		}
		break;

	case 20: // ������ â���� ������ ����ҋ�.
	case 21: // ������ ��Ʈ���� ������ ����ҋ�.
	case 30: // ��� ���� ������ ���.
	case 44: // ����𿡰� ���� ���.
	case 51: // ����� ������ ���.
	case 52: // ��ȯ ��ũ�� ���.
	case 53: // ��Ʈ���� ��ȯ ��ũ�� ���.
	case 55: // ��Ʈ���� ����� ī�� ���.
	case 56: // ��Ƽ ��Ż �޽���.
//	case 29: // ���콺���� ������ ��ư���� ������â���� ��������.
		iCount = -1;
		if( pItem )
		{
			SetItemCount( pItem->Item.m_wItemID, pLayer->GetParent()->GetID(), iCount );
			SetItemLog( &pItem->Item, ITEM_LOG_USED, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
		}
		else
		{
			SetItemCount( PickupItem.Item.m_wItemID, pLayer->GetParent()->GetID(), iCount );
			SetItemLog( &PickupItem.Item, ITEM_LOG_USED, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
		}
		break;

	case 69: // ��ų������ �� �Ŀ�����Ȧ�� ����
		{
			iCount = -1;
			SetItemCount( ItemLog.m_wItemID, pLayer->GetParent()->GetID(), iCount );
			SetItemLog( &ItemLog, ITEM_LOG_USED, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
		}
		break;

	case 74: // ��Ŭ������ ���ϸ����� ������ ���
		iCount = -1;
		if( pItem )
		{
			SetItemCount( pItem->Item.m_wItemID, pLayer->GetParent()->GetID(), iCount );
			SetItemLog( &pItem->Item, ITEM_LOG_USED, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
		}
		else
		{
			SetItemCount( PickupItem.Item.m_wItemID, pLayer->GetParent()->GetID(), iCount );
			SetItemLog( &PickupItem.Item, ITEM_LOG_USED, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
		}
		break;
	case 100:
		{
			iCount = -1;
			SetItemCount( ItemLog.m_wItemID, pLayer->GetParent()->GetID(), iCount );
			SetItemLog( &ItemLog, ITEM_LOG_USED, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
		}
		break;		
	}
}


void CmdItemUsed( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_ITEM_USED*		pItemUsed = (CTDS_ITEM_USED*)pMsg;
	
	DSTC_ITEM_PICKUP	ItemPickUP;
	DSTC_USER_STATUS	UserStatus;
	DWORD				dwRand;
	float				fPer;
	CItem*				pItem;		// �������� �ּҸ� ������������.	
	CBaseItem*			pBaseItem;
	CUser*				pUser	= (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );

	if( !pUser )
		return;

	if ( !pUser->GetCurDungeon() )
		return;

	if( pUser->GetUnitStatus() == UNIT_STATUS_DEAD || pUser->GetUnitStatus() == UNIT_STATUS_CASTING )
		return;

	if(CheckShop(pUser))
		return;

	if(pUser->m_byRest==1)
		return;
	
	switch( pItemUsed->bInv )
	{
	case 1:	//small inv...
		ItemPickUP.bInv		= 20;

		if( pItemUsed->bZipCode >= MAX_INV_SMALL_POOL )
			return;

		pItem = &pUser->m_pInv_Small[ pItemUsed->bZipCode ];

		break;

	case 2:	//belt...
		ItemPickUP.bInv		= 21;

		if( pItemUsed->bZipCode	>= MAX_BELT_POOL )
			return;

		pItem = &pUser->m_pBelt[ pItemUsed->bZipCode ];

		break;

	default:
		return;
	}

	switch( (pItem->m_wItemID / ITEM_DISTRIBUTE) )
	{
	case 50: // ����.
		{ 
			UserStatus.bStatusMany = 0;

			int iItemDelay = g_pBaseClassInfo[ pUser->GetClass() ].wPotion_Delay
				+ g_pBaseRaceInfo[ pUser->GetClass() ].wPotion_Delay;
			
			if( g_dwTickCount - pUser->m_dwTemp[USER_TEMP_LAST_ITEM_CASTING] < (DWORD)iItemDelay )
			{
				return;
			}
			
			pUser->m_dwTemp[USER_TEMP_LAST_ITEM_CASTING] = g_dwTickCount;
			pBaseItem = g_pBaseItemHash->GetData( pItem->m_wItemID );

			if( !pBaseItem )
			{
				return;
			}

			CDungeonLayer* pLayer = pUser->GetCurDungeonLayer();

			if (pLayer)
			{
				if(pBaseItem->BaseItem_Supplies.bType == 1 || pBaseItem->BaseItem_Supplies.bType == 3)
				{
					DSTC_DUNGEON_USED_POTION packet;
					packet.dwUserIndex = pUser->GetID();
					packet.bType = BYTE(pItem->m_wItemID - 5000);
					pLayer->BroadCastSectionMsg((char*)&packet, packet.GetPacketSize(), pUser->GetPrevSectionNum());
				}				
			}
			
			fPer = FLOAT(g_pBaseClassInfo[ pUser->GetClass() ].wPotion_Recovery 
				+ g_pBaseRaceInfo[ pUser->GetClass() ].wPotion_Recovery);

			if( pBaseItem->BaseItem_Supplies.bType == 1 )		
			{
				// HP
				dwRand	= ( rand()%(pBaseItem->BaseItem_Supplies.wMax - pBaseItem->BaseItem_Supplies.wMin+1) )
					+ pBaseItem->BaseItem_Supplies.wMin;
				dwRand  = DWORD((float)dwRand * (fPer / 100.f));
				pUser->SetHP(WORD(pUser->GetHP() + ( pUser->GetMaxHP() * ( (float)dwRand  / 100.f ) ) ));
					 
				// Packet...
				UserStatus.bStatusMany			= 1;
				UserStatus.pStatus[0].dwCode	= USER_HP;
				UserStatus.pStatus[0].dwMin		= pUser->GetHP();
			}
			else if( pBaseItem->BaseItem_Supplies.bType == 2 )	
			{
				// MP
				dwRand = ( rand()%(pBaseItem->BaseItem_Supplies.wMax - pBaseItem->BaseItem_Supplies.wMin+1) ) 
					+ pBaseItem->BaseItem_Supplies.wMin;
				dwRand = DWORD((float)dwRand * (fPer / 100.f ));
				pUser->SetSP(WORD(pUser->GetSP() + ( pUser->GetMaxSP() * ( (float)dwRand / 100.f ) )));
				
				UserStatus.bStatusMany			= 1;
				UserStatus.pStatus[0].dwCode	= USER_MP;
				UserStatus.pStatus[0].dwMin		= pUser->GetSP();
			}
			else if( pBaseItem->BaseItem_Supplies.bType == 3 ) 
			{
				// HP POINT
				dwRand = ( rand()%(pBaseItem->BaseItem_Supplies.wMax - pBaseItem->BaseItem_Supplies.wMin+1) )
					+ pBaseItem->BaseItem_Supplies.wMin;
				pUser->SetHP( WORD(pUser->GetHP() + ( dwRand * ( fPer / 100.f ) ) ));

				if( pUser->GetHP() > pUser->GetMaxHP() )
				{
					pUser->SetHP((WORD)pUser->GetMaxHP());
				}

				UserStatus.bStatusMany			= 1;
				UserStatus.pStatus[0].dwCode	= USER_HP;
				UserStatus.pStatus[0].dwMin		= pUser->GetHP();
			}
			else if( pBaseItem->BaseItem_Supplies.bType == 4 ) 
			{
				// sp, hp POINT
				dwRand = ( rand()%(pBaseItem->BaseItem_Supplies.wMax - pBaseItem->BaseItem_Supplies.wMin+1) )
					+ pBaseItem->BaseItem_Supplies.wMin;
				pUser->SetHP( WORD(pUser->GetHP() + ( dwRand * ( fPer / 100.f ) ) ));

				if( pUser->GetHP() > pUser->GetMaxHP() ) 
				{
					pUser->SetHP((WORD)pUser->GetMaxHP());
				}
				
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HP;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= pUser->GetHP();

				dwRand = ( rand()%(pBaseItem->BaseItem_Supplies.wMax - pBaseItem->BaseItem_Supplies.wMin+1) )
					+ pBaseItem->BaseItem_Supplies.wMin;
				pUser->SetSP( WORD(pUser->GetSP() + ( dwRand * ( fPer / 100.f ) ) ));

				if( pUser->GetSP() > pUser->GetMaxSP()) 
				{
					pUser->SetSP((WORD)pUser->GetMaxSP());
				}

				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MP;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= pUser->GetSP();
			}
			else
			{
				return;
			}
		}
		break;

	default:
		return;
	}

	// Item Log.
	SetItemCount( pItem->m_wItemID, pUser->GetCurDungeonID(), -1 );
	SetItemLog( pItem, ITEM_LOG_USED, pUser->GetCurDungeonID(), pUser->m_szName, NULL );

	// ���������� ����� ������.
	pItem->m_Item_Supplies.bQuantity--;
	
	if( pItem->m_Item_Supplies.bQuantity == 0 )
	{
		// �������� �� �Ҹ������ �����.
		memset( pItem, 0, sizeof( CItem ) );
	}
	
	ItemPickUP.bZipCode	= pItemUsed->bZipCode;	
	memcpy( &ItemPickUP.Item, pItem, sizeof( CItem ) );

	NetSendToUser( dwConnectionIndex, (char*)&ItemPickUP, ItemPickUP.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	NetSendToUser( dwConnectionIndex, (char*)&UserStatus, UserStatus.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );	
	
	CheckSpeed(pUser);
}


void CmdDungeonChat( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_DUNGEON_CHAT* pPacket = (CTDS_DUNGEON_CHAT*)pMsg;

	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
		
	if(!pUser)					return;
	else if(!pUser->GetCurDungeon())	return;
	else if( pUser->GetCurDungeon()->GetTemp(DUNGEON_TEMP_CHAT_DISABLE) )	return;		// ���� ä�� ��� ���ϱ� �ϋ� ����.
	else if( TRUE == pUser->m_dwTemp[USER_TEMP_GM_DRIVED_SILENCE_FLAG])		return;		// GM�� ä�� ���ϰ� ��� �������� 

	DSTC_DUNGEON_CHAT		packet;
	packet.bChatType		= pPacket->bChatType;
	packet.bChatStrLength	= pPacket->bChatStrLength;
	packet.dwUserIndex		= pUser->GetID();
	memcpy(packet.szChatStr, pPacket->szChatStr, packet.bChatStrLength);
	
	CDungeonLayer*	pLayer;
	pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer ) return;

	// �븸�� GM �� ��ȭ������ �����Ѵ�.
	// seven / [12/13/2004] 
#if defined (TAIWAN_LOCALIZING)
	packet.szChatStr[packet.bChatStrLength] = 0;
	
	if (pUser->IsGMUser() ||
		0 == __strncmp(pUser->m_szName, "GM", 2))	
	{
		// GM ������ ���̸� GM ������ ��ȭ�� �����.
		SetChatLogInDungeon((CHAT_TYPE)pPacket->bChatType, 
			pUser, NULL, pPacket->szChatStr, pPacket->bChatStrLength );
	}
#endif


#ifdef JAPAN_LOCALIZING

	// �Ϻ��� ��ȭ������ ���ǿ� �������� �ʰ�,
	// �ձ� ������ �����Ѵ�		( 2004. 08. 12. minjin )
	BYTE byRadTileCount = 5;	// For BroadCast Msg in Circle ( Radius Tile Count )

	pUser->GetCurDungeon()->BroadCastInCircle( pLayer, (VECTOR2*)pUser->GetCurPosition(), byRadTileCount, (char*)&packet, packet.GetPacketSize());

#else

	pLayer->BroadCastSectionMsg( (char*)&packet, packet.GetPacketSize(), pUser->GetPrevSectionNum() );		

#endif

}


void CmdDungeonJoin(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	CTDS_DUNGEON_JOIN*  join = (CTDS_DUNGEON_JOIN*)pMsg;

	BYTE bDestWorldSpot = 0;

	if (join->wDungeonID > 2000 && join->wDungeonID < 3000)
	{
		bDestWorldSpot		= join->wDungeonID % 2 == 0;
		join->wDungeonID	-= bDestWorldSpot;				
	}

	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo( join->wDungeonID );

	if(!pDungeon)	
	{
		Log(LOG_NORMAL, "Dungeon Info is not found at CmdDungeonJoin()! (DungeonNum:%d)", join->wDungeonID);
		SendDungeonJoinFailMessage(dwConnectionIndex, DUNGEON_JOIN_FAIL_DUNGEON_INFO_IS_NOT_FOUND);
		return;
	}

	if( pDungeon->GetDungeonTotalUserCount() >= MAX_USER_PER_DUNGEON )
	{
		SendDungeonJoinFailMessage(dwConnectionIndex, DUNGEON_JOIN_FAIL_USER_OVERFLOW);
		return;
	}

	// ������� ���� ������ ������ �޾Ƶ��δ�.
	ACCEPT_USER*	pAccept				= g_pAcceptTable->GetAcceptInfo(join->dwUserIndex);
	BOOL			bConnectToGameRoom	= FALSE;
	BYTE			bUserType			= USER_TYPE_NORMAL;

	if(!pAccept)
	{
		Log(LOG_IMPORTANT, "Unauthorized User Joined! (Name:%s)", join->szChrName);
		SendDungeonJoinFailMessage(dwConnectionIndex, DUNGEON_JOIN_FAIL_UNAUTHORIZED_USER);
		return;
	}
	else
	{
		//���������� �����Ǿ����� AcceptTable���� Remove�ϰ� 
		bConnectToGameRoom = pAccept->bConnectToGameRoom;
		bUserType		   = pAccept->bUserType;
		g_pAcceptTable->Remove(pAccept);
	}	

	// ������ �����Ѵ�.
	CUser* pUser = (CUser*)CreateUnitInstance(UNIT_CLASS_ID_USER);
	
	pUser->SetCurDungeon(pDungeon);							// ���� �����Ŀ� �ʿ� �ϴ�.
	pUser->m_dwPartyId				= join->dwPartyID;		// ���� �����Ŀ� �ʿ� �ϴ�.
	pUser->m_dwConnectionIndex		= dwConnectionIndex;	
	pUser->m_bDestWorldSpot			= bDestWorldSpot;	
	pUser->m_bConnectToGameRoom		= bConnectToGameRoom;
	pUser->m_bUserType				= bUserType;
	pUser->m_wRecentJoinDungeonID	= join->wPrevDungeonID;	
	
	//��Ż Join�� ��� 
	if(join->bIsPortalJoin)
	{
		PORTAL_BUCKET* pPortal	= (PORTAL_BUCKET*)LALAlloc(g_pPortalBufferPool);
		pPortal->wToDungeonID	= join->wDungeonID;
		pPortal->bLayerNo		= join->bLayerNum;
		pPortal->v2Pos			= join->v2Pos;
		pUser->pBufForPortal	= pPortal;
	}

	g_pNet->SetUserInfo( pUser->m_dwConnectionIndex, pUser );
	
	DSTWS_CHECK_DUNGEONJOIN pCheckPacket;
	memset(pCheckPacket.szCharacerName, 0, sizeof(pCheckPacket.szCharacerName));
	__lstrcpyn(pCheckPacket.szCharacerName, join->szChrName, MAX_CHARACTER_REAL_LENGTH + 1);
	pCheckPacket.wDungeonID			= join->wDungeonID;
	pCheckPacket.dwUserIndex		= join->dwUserIndex;
	pCheckPacket.dwConnectionIndex	= dwConnectionIndex;
	g_pNet->SendToServer(WSINDEX, (char*)&pCheckPacket, pCheckPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	
	CUser* pUserChk = g_pUserChkHash->GetData(join->dwUserIndex);

	if(pUserChk)
	{
		g_pUserChkHash->Delete(pUserChk, join->dwUserIndex);
	}

	g_pUserChkHash->Insert(pUser, join->dwUserIndex);	
}


void CmdDungeonMove( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_DUNGEON_MOVE*	pMove	= (CTDS_DUNGEON_MOVE*)pMsg;
	CDungeonLayer*		pLayer	= NULL;
	CUser*				pUser	= (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );

	if(!pUser)											return;
	if(!pUser->GetCurDungeon())							return;
	if(pUser->m_enGodMode == GODMODE_STATUS_MAPLOADING)	return;	

	pLayer = pUser->GetCurDungeonLayer();

	if(!pLayer)											return;
	if(!pLayer->GetSection(pUser->GetPrevSectionNum())) return; 	

	if(!pUser->GetHP())									return;
	if(pUser->GetUnitStatus() == UNIT_STATUS_DEAD)		return;
	
	float fDirection = atan2f(FLOAT(pMove->wDestZ - pUser->GetCurMapTile()->wIndex_Z)
							, FLOAT(pMove->wDestX - pUser->GetCurMapTile()->wIndex_X));

	VECTOR2 v2Direction = {	cosf(fDirection), sinf(fDirection) };
	pUser->SetCurDirection(&v2Direction);

	if(pMove->bMoveType==UNIT_STATUS_RUNNING)
	{
		if(pUser->m_wPerWeight< WEIGTH_100PER_OVER)
		{
			pUser->SetStatus( pMove->bMoveType );
			pUser->SetLastMoveTick(g_dwTickCount);
		}
	}
	else
	{
		pUser->SetStatus( pMove->bMoveType );
		pUser->SetLastMoveTick(g_dwTickCount);
	}			
		
	DSTC_MOVE move;

	float fSpeed = 0.0f;

	if(pMove->bMoveType == UNIT_STATUS_RUNNING)
	{
		if(pUser->m_wPerWeight >= WEIGTH_80PER_OVER)
			fSpeed = pUser->GetMoveSpeed()/3;
	}		

	move.dwIndex			= pUser->GetID();
	move.bMoveType			= pMove->bMoveType;
	move.fSpeed				= (pUser->GetMoveSpeed()-fSpeed)/1000.f;
	move.v2MoveStartPos		= *pUser->GetCurPosition();
	move.v2MoveDirection	= v2Direction;
	move.wDestX				= pMove->wDestX;
	move.wDestZ				= pMove->wDestZ;
	
	// �޽��� ����.
	pLayer->BroadCastSectionMsg( (char*)&move, move.GetPacketSize(), pUser );

	// ������ ���� ���� ����.
	pUser->m_dwTemp[ USER_TEMP_LASTMOVETICK ] = g_dwTickCount;
}

void CmdDungeonStop( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{	
	CUser*	pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
	if( !pUser )	return;
	else if(!pUser->GetCurDungeon())	return;
	else if (pUser->m_enGodMode == GODMODE_STATUS_MAPLOADING) return;
	
	CDungeonLayer* pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer ) return;
	else if (!pUser->GetHP() || pUser->GetUnitStatus() == UNIT_STATUS_DEAD)	return;

	CTDS_DUNGEON_STOP*	pStop = (CTDS_DUNGEON_STOP*)pMsg;

	//���������� �����Ѵ�.
	MAP_TILE*	pStopTile = pLayer->GetMap()->GetTile( pStop->v2StopPos.x, pStop->v2StopPos.y );
	MAP_TILE*	pTile = pUser->GetCurMapTile();	

	if( !pTile || ( pTile->wAttr.uSection == 0 ) )
	{
		pUser->SetStatus( UNIT_STATUS_NORMAL );
		pUser->SendStopPacket(SEND_MODE_BROADCAST_SECTION);
		return;
	}

	if (!pStopTile || ( pStopTile->wAttr.uSection == 0 ))
	{
		pUser->SetStatus( UNIT_STATUS_NORMAL );
		pUser->SendStopPacket(SEND_MODE_BROADCAST_SECTION);
		return;
	}
	
	if (labs((int)pStopTile->wIndex_X - (int)pTile->wIndex_X) <= 1
		&& labs((int)pStopTile->wIndex_Z - (int)pTile->wIndex_Z <= 1))
	{
		pUser->SetCurPosition(&pStop->v2StopPos);
	}
	
	// Ÿ���� �ٲ������ �ٲ���� ó��.
	if( ( pUser->GetCurMapTile()->wIndex_X != pTile->wIndex_X ) || ( pUser->GetCurMapTile()->wIndex_Z != pTile->wIndex_Z ) )
	{
		// ���� �ٸ� ������ �̵��ؾ��Ҷ����..
		for( BYTE i = 0; i < pLayer->m_wMoveSpotNum; i++ )
		{			
			if (pUser->IsChangeLayer(pTile->wIndex_X, pTile->wIndex_Z, i))
			{				
				pUser->GetCurDungeon()->ChangeLayerUser(pUser, pUser->GetCurLayerIndex(), i);
				return;
			}
		}
	}
		
	if( pUser->GetPrevSectionNum() != pTile->wAttr.uSection )
	{
		int					i=0;
		BYTE				pIntersectSection[MAX_SECTION_LINK_NUM]={0,};
		CUser*				pDisAppearUser=0;
		CMonster*			pDisAppearMonster=0;
		POSITION_			UserPos;
		POSITION_			MonsterPos;
		CSection*			pSection=0;
		DSTC_CHANGE_SECTION	Section;
		DSTC_DISAPPEAR		DisAppear;
		BYTE				oldSectionNum = pUser->GetPrevSectionNum();
	
		pLayer->RemoveUserSection( pUser );
		pLayer->InsertUserSection( pUser, pTile->wAttr.uSection );
		pLayer->IntersectSection( pUser->GetPrevSectionNum(), oldSectionNum, pIntersectSection );		
		pSection = pLayer->GetSection( oldSectionNum );
		
		// �������� ������ �����鿡�� ���� �޼����� ������.
		DisAppear.dwUserIndex		= pUser->GetID();
		for( i=0; pIntersectSection[i] != 0; i++ )
		{
			pLayer->BroadCastSectionMsgExceptLink( (char*)&DisAppear, DisAppear.GetPacketSize(), pIntersectSection[i] );
		
			// ���⼭ ���������� ������ DisAppear������ �����.
			pSection = pLayer->GetSection( pIntersectSection[i] );
			if( !pSection ) continue;

			UserPos = pSection->m_pPcList->GetHeadPosition();

			while( UserPos )
			{
				pDisAppearUser = (CUser*)pSection->m_pPcList->GetNext( UserPos );
				Section.pdwDisAppearIndex[Section.bDisAppearUserCount++] = pDisAppearUser->GetID();			
			}

			MonsterPos = pSection->m_pMonsterList->GetHeadPosition();

			while( MonsterPos )
			{
				pDisAppearMonster = (CMonster*)pSection->m_pMonsterList->GetNext( MonsterPos );
				Section.pdwDisAppearIndex[Section.bDisAppearUserCount + Section.bDisAppearMonsterCount++] = pDisAppearMonster->GetID();
			}
		}
		
		Section.wChageSectionNum = pUser->GetPrevSectionNum();
		NetSendToUser( pUser->m_dwConnectionIndex, (char*)&Section, Section.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

		// �ٲ� ���ǿ��� ���� ���� ������ �޴´�.
		pLayer->BroadCastIntersectSectionInfo( oldSectionNum, pUser->GetPrevSectionNum(), pUser );
	}

	pUser->SetStatus( UNIT_STATUS_NORMAL);
	pUser->SendStopPacket(SEND_MODE_BROADCAST_EXCEPT_SENDER);
		
	if (pUser->GetEffectDesc(__SKILL_HIDING__))
		UpdateUserForAI(pUser);
}


void CmdDungeonAttack_User_Mon( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_DUNGEON_ATTACK_USER_MON*	pAttackUserMon = (CTDS_DUNGEON_ATTACK_USER_MON*)pMsg;
	DSTC_ATTACK_USER_MON	Attack;
	CDungeonLayer*			pLayer;

	CUser*		pUser	= (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
	if( !pUser ) return;

	if(!pUser->GetCurDungeon())	return;
	if (!pUser->GetHP() || pUser->GetUnitStatus() == UNIT_STATUS_DEAD)	return;

	pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer ) return;

	CMonster* pMonster= g_pMonsterTable->GetMonster( pAttackUserMon->dwMonsterIndex );

	if( pUser->m_enGodMode )
	{
		goto lbl_attack_fail;
	}
	else if(!pMonster) 
	{
		goto lbl_attack_fail;
	}
	else
	{	
		if (g_dwTickCount - pUser->m_dwTemp[ USER_TEMP_LASTATTACKTICK ] < pUser->GetAttackSpeed())
		{
			goto lbl_attack_fail;
		}
		else if (pUser->GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON) != __SKILL_ATTACK__)
		{
			goto lbl_attack_fail;
		}

		float fDist = CalcDistance( pUser->GetCurPosition(), pMonster->GetCurPosition() );
		
		// �Ƹ޸�ŸƮ�� ��ġĿ ���� ������ ���� : �ִ��� 2005.2.14
		if( fDist > (pMonster->GetBaseMonsterInfo()->dwID == 81 ? 800.0f : 400.0f) ) goto lbl_attack_fail;	

		// �븸 ���������� ������ ���緮 �ʰ� �� ���� ���Ѵ�.
#if defined TAIWAN_LOCALIZING
		if (pUser->GetAverageWeight() >= 100)
		{
			goto lbl_attack_fail;
		}
#endif 
		// ��ȯ���ʹ� pk ����϶����� �����Ҽ� �־�.
		BOOL bChk = FALSE;
		if (pMonster->GetLord())
		{
			bChk = IsGuildWar( pUser, (CUser*)pMonster->GetLord());
//						, pUser->m_dwGuildId
//						, ((CUser*)pMonster->GetLord())->m_dwGuildId
//						, pUser->m_dwPartyId
//						, ((CUser*)pMonster->GetLord())->m_dwPartyId
//						, GODMODE_STATUS_RELEASE);
		}			

		if(!pUser->IsAlliance((CUnit*)pMonster))
		{			
			if(pMonster->GetLord() && pMonster->GetLord()->GetObjectType() != OBJECT_TYPE_MONSTER)
			{
				if(!bChk)
				{
					if (!pUser->m_sPKDescInfo.m_bPKMode)				
						goto lbl_attack_fail;
				}				
			}
		}
		else
		{
			goto lbl_attack_fail;
			
		}
	}
	
	//���� �� ���Ͱ� �񼱰� �����̰� Ÿ�پ��� ��Ȳ�ϰ� �ִ³��̸� Ÿ���� �������ش�. 
	if(	pMonster->GetAI_FirstAttack() == AI_FIRST_ATTACK_NONE && !pMonster->GetUnitForAI())
	{
		pMonster->ChangeTargetObject(pUser);
	}
	
	//������ Ÿ�� �����ؾ� �ϴ� �����̸� ���ݼ������ο� ������� Ÿ���� �ٲ��ش�. 
	if( pMonster->GetAI_TargetChangeType() == AI_CHANGE_TARGET_TYPE_ABSOLUTELY_CHANGE)
	{
		pMonster->ChangeTargetObject(pUser);
	}	

	ATTACK_RESULT	AttackResult;
	if( AttackResult_Unit_Unit( pUser, pMonster, &AttackResult ) == 0 ) 
	{
		goto lbl_attack_fail;
	}
	  
	if( AttackResult.byType == 3 || AttackResult.byType == 4 )	// ������ �����ߴٸ�.
	{
		if( pMonster->GetHP() >= AttackResult.dwDamage )
		{
			pMonster->SetHP(pMonster->GetHP() - AttackResult.dwDamage, pUser);

			if (pMonster->GetLord())
			{
				SetBadAction(pUser, pMonster->GetLord());				
			}

			pMonster->ChangeTargetObject(pUser, AttackResult.dwDamage);			
		}
		else
		{
			pMonster->SetHP(0, pUser);
			if (pMonster->GetLord())
			{
				SetBadAction(pUser, pMonster->GetLord());
			}
		}
		
		// ������ ���� �Ϳ� ���� hp, sp�� ȸ���ȴ�.
		if (50 > rand()%100)//: sung han 050204
		{			
			DWORD dwHPRecovery = DWORD(AttackResult.dwDamage * pUser->GetItemAttr(ITEM_ATTR_ATTACK_DMG_TO_HP) / 100.f);
			DSTC_USER_STATUS	UserStatus;
			UserStatus.bStatusMany	= 0;

			if (dwHPRecovery)
			{
				pUser->SetHP(WORD(pUser->GetHP()+dwHPRecovery));
									
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HP;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= pUser->GetHP();			
			}

			DWORD dwMPRecovery = DWORD(AttackResult.dwDamage * pUser->GetItemAttr(ITEM_ATTR_ATTACK_DMG_TO_SP) / 100.f);

			if (dwMPRecovery)
			{
				pUser->SetSP(WORD(pUser->GetSP()+dwMPRecovery));
				
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MP;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= pUser->GetSP();
			}
			
			if (UserStatus.bStatusMany)
				NetSendToUser(pUser->m_dwConnectionIndex, (char*)&UserStatus, UserStatus.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}		
	}
	
	Attack.dwUserIndex		= pUser->GetID();
	Attack.dwMonsterIndex	= pMonster->GetID();
	Attack.v2UserPos		= *pUser->GetCurPosition();		//pAttackUserMon->v2UserPos;
	Attack.v2MonsterPos		= *pMonster->GetCurPosition();
	Attack.bType			= AttackResult.byType;
	Attack.dwCurMonsterHP	= pMonster->GetHP();
	Attack.dwDamage			= AttackResult.dwDamage;
	Attack.bStatusKind		= USER_HP;					// �� ���� ����.
	Attack.bAttackType		= pUser->m_sAttackDescInfo.m_bCurAttackType;
	Attack.bAttackFrame		= pUser->m_sAttackDescInfo.m_bCurAttackFrame;

	pLayer->BroadCastSectionMsg( (char*)&Attack, Attack.GetPacketSize(), pUser->GetPrevSectionNum() );	
	
	if(!pMonster->GetHP())	// ���ʹ� �׾���. �׾����� ����ġ�� �ش�.
	{
		MonsterKillByUser( pUser, pMonster );
		UpdateMonsterForAI( pMonster );
		
		return;
	}
	else
	{
		pUser->SendItemSkill(ITEM_SKILL_ATTACK, OBJECT_TYPE_MONSTER, pMonster);
	}

	pUser->m_dwTemp[ USER_TEMP_LASTATTACKTICK ] = g_dwTickCount;
	return;

lbl_attack_fail:
	Attack.dwUserIndex	= pUser->GetID();
	Attack.bType		= 5;
	NetSendToUser( pUser->m_dwConnectionIndex, (char*)&Attack, Attack.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);	
}
extern void DieOnGuildWar(CDungeonLayer *pLayer, CUser* pDieUser);

void CmdDungeonAttack_User_User( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_DUNGEON_ATTACK_USER_USER*	pAttackUserUser = (CTDS_DUNGEON_ATTACK_USER_USER*)pMsg;
	DSTC_ATTACK_USER_USER			Attack;
	CUser*							pOffense, *pDefense;
	CDungeonLayer*					pLayer;
	BOOL							bChk	= FALSE;
	float							fDist = 0;

	pOffense = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );

	if(!pOffense)
		return;	
	if(!pOffense->GetCurDungeon())
		return;
	if (!pOffense->GetHP() || pOffense->GetUnitStatus() == UNIT_STATUS_DEAD)	return;	
	pLayer = pOffense->GetCurDungeonLayer();
	
	if(!pLayer){return;}

	if(pOffense->GetUnitStatus()==UNIT_STATUS_DEAD)	
		goto lbl_attack_fail;
	
	pDefense = g_pUserHash->GetData( pAttackUserUser->dwUserIndex );
	
	if(!pDefense) 
		goto lbl_attack_fail;

	else if(!pDefense->GetCurDungeon())	
		goto lbl_attack_fail;	
	else if(pDefense->GetUnitStatus() == UNIT_STATUS_DEAD 
			|| pDefense->m_enGodMode) 
		goto lbl_attack_fail;
	else if (g_dwTickCount - pOffense->m_dwTemp[ USER_TEMP_LASTATTACKTICK ] 
			< pOffense->GetAttackSpeed())
		goto lbl_attack_fail;
	else if (pOffense->GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON) != __SKILL_ATTACK__)
		goto lbl_attack_fail;

	fDist = CalcDistance( pOffense->GetCurPosition(), pDefense->GetCurPosition() );
	if( fDist > 250.0f ) goto lbl_attack_fail;

	// �븸 ���������� ������ ���緮 �ʰ��� ���� ���Ѵ�.
#if defined TAIWAN_LOCALIZING
	if (pOffense->GetAverageWeight() >= 100)
	{
		goto lbl_attack_fail;	
	}
#endif 

	bChk = IsGuildWar(pOffense, pDefense);
/*			pOffense->GetCurDungeon()
			, pOffense->m_dwGuildId
			, pDefense->m_dwGuildId
			, pOffense->m_dwPartyId
			, pDefense->m_dwPartyId		
			, pDefense->m_enGodMode
		);
*/
	CDungeon* pDungeon = pOffense->GetCurDungeon();
	if(pDungeon->GetDungeonDataEx()->IsStadium() //������̰�
	&& pLayer->m_pTeamMatch )//����ġ �Ŵ����� ������
	{	//�������� �ȶ����� �ϴ� ó�� �ʿ� : hwoarang 050202
		if( !pLayer->m_pTeamMatch->OnAttack(pOffense, pDefense) )
		{
			goto lbl_attack_fail;
		}
		else
		{
			bChk = true;// ����� üũ �÷���
		}
	}

	if(pOffense->m_sPKDescInfo.m_bPKMode==0 && !bChk)
	{	// pk mode�� �ƴѰ���..
		goto lbl_attack_fail;
	}	
	
	ATTACK_RESULT	AttackResult;
	if( AttackResult_Unit_Unit( pOffense, pDefense, &AttackResult ) == 0 ) 
	{
		goto lbl_attack_fail;
	}

	if( AttackResult.byType == 3 || AttackResult.byType == 4 )	// ������ �����ߴٸ�.
	{
		Attack.bStatusKind = (BYTE)pDefense->SetDamage( pOffense->GetLevel(), AttackResult.dwDamage );		
	}

	Attack.dwDamage				= AttackResult.dwDamage;
	Attack.bType				= AttackResult.byType;
	Attack.dwDefenseUserIndex	= pDefense->GetID();
	Attack.dwOffenseUserIndex	= pOffense->GetID();
	Attack.v2DefenseUserPos		= *pDefense->GetCurPosition();
	Attack.v2OffenseUserPos		= *pOffense->GetCurPosition();
	Attack.bAttackType			= pOffense->m_sAttackDescInfo.m_bCurAttackType;
	Attack.bAttackFrame			= pOffense->m_sAttackDescInfo.m_bCurAttackFrame;

	if(	!pDefense->GetHP())//HP�� ���µ�. --> �׾��µ�.
	{
		if( !pLayer->OnDie(pOffense, pDefense) )//����� ���ó���� ���̸� �ȵȴٰ� �Ѵ�. : hwoarang 050202
		{
			if(	pDungeon->GetDungeonDataEx()->IsStadium() //������̰�
			&&	pLayer->m_pTeamMatch )//����ġ �Ŵ����� ������
			{
				DieOnGuildWar( pLayer, pDefense);
			}
			else
			{//���� ���� ���� ����
				pDefense->SetHP(1);
			}
		}
		else
		{
			if(bChk && pDefense->GetUnitStatus()!=UNIT_STATUS_DEAD)
			{
				pOffense->GuildMemberKill(pDefense);
			}
		
			// 1:1 ������ �ƴϴ�
			if (!pDefense->m_sMatchDescInfo.m_bMatching)
			{ 
				SetBadAction(pOffense, pDefense);
							
				if (pOffense->GetCurDungeon()->GetDungeonDataEx()->IsConquer() && pOffense->GetCurDungeon()->GetDungeonDataEx()->m_bSiege)
					pOffense->m_wSiegePKCount++;

				pDefense->SetStatus( UNIT_STATUS_DEAD );


//��� �۾���. ���� ���񽺿��� �߱����� �ش��.
#ifdef CHINA_LOCALIZING				
				//������̸� �ȵǰ�. PVP �϶��� �ȵǰ�.
				pDefense->PKDropItem( pOffense);
#endif

				pDefense->SendItemSkill(ITEM_SKILL_DIE, OBJECT_TYPE_PLAYER, pOffense);
			}
			else
			{	// 1:1 ����
				DSTC_END_MATCH_MAN2MAN	packet;
				packet.bJudgmentCode	= 0;	// KO
				memset(packet.szWinnerName, 0, sizeof(packet.szWinnerName));
				memset(packet.szLoserName, 0, sizeof(packet.szLoserName));
				__lstrcpyn(packet.szWinnerName, pOffense->m_szName, MAX_CHARACTER_REAL_LENGTH);
				__lstrcpyn(packet.szLoserName, pDefense->m_szName, MAX_CHARACTER_REAL_LENGTH);	
				
				// �¸� ī��Ʈ 
				MatchWinCount(pOffense);
				// �й� ī��Ʈ
				MatchLoseCount(pDefense);
				
				pOffense->GetCurDungeonLayer()->BroadCastMsg((char *)&packet
					, packet.GetPacketSize());

				CDungeonLayer*	pTopLayer = pOffense->GetCurDungeon()->GetDungeonLayer(0); // ���� 0 ���̾�
				if (pTopLayer)
					pTopLayer->BroadCastMsg((char *)&packet, packet.GetPacketSize());


				pLayer->m_dwEndMatchJudgmentTick = 0;
				pLayer->m_dwMatchEndTime = g_dwTickCount + MATCH_JUDGMENT_TERM_TICK;	// 5���Ŀ� ����Ʈ �� ������.
			}
		}
	}
	else
	{//������ �ʾҳ�.		
		SetBadAction(pOffense, pDefense);
		
		pOffense->SendItemSkill(ITEM_SKILL_ATTACK, OBJECT_TYPE_PLAYER, pDefense);
		pDefense->SendItemSkill(ITEM_SKILL_DEFENSE, OBJECT_TYPE_PLAYER, pOffense);

		BYTE bSend = FALSE;
		if (pDefense->GetItemAttr(ITEM_ATTR_DMG_REFLECT_POINT))
		{
			int nDamage = pDefense->GetItemAttr(ITEM_ATTR_DMG_REFLECT_POINT);
			
			pOffense->SetDamage(pDefense->GetLevel(), nDamage);
			if (!pOffense->GetHP())
			{
				pOffense->SetHP(1);
			}

			bSend = TRUE;
		}

		if (pDefense->GetItemAttr(ITEM_ATTR_DMG_REFLECT))
		{
			int nDamage = INT(Attack.dwDamage*pDefense->GetItemAttr(ITEM_ATTR_DMG_REFLECT)/100.F);
			pOffense->SetDamage(pDefense->GetLevel(), nDamage);
			if (!pOffense->GetHP())
			{
				pOffense->SetHP(1);
			}
			bSend = TRUE;
		}

		if (bSend)
		{
			DSTC_USER_STATUS	UserStatus;
			UserStatus.bStatusMany = 0;
			UserStatus.pStatus[UserStatus.bStatusMany].dwMin = pOffense->GetHP();
			UserStatus.pStatus[UserStatus.bStatusMany++].dwCode = USER_HP;			
			NetSendToUser(pOffense->m_dwConnectionIndex, (char*)&UserStatus, UserStatus.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}
	}		

	if( Attack.bStatusKind == USER_HP )
	{
		Attack.dwCurDefenseUserHP	= pDefense->GetHP();
	}
	else if( Attack.bStatusKind == USER_MP )
	{
		Attack.dwCurDefenseUserHP	= (pDefense->GetHP()<<16) | pDefense->GetSP();
	}

	pLayer->BroadCastSectionMsg( (char*)&Attack, Attack.GetPacketSize(), pOffense->GetPrevSectionNum() );
	pOffense->m_dwTemp[ USER_TEMP_LASTATTACKTICK ] = g_dwTickCount;

	return;

lbl_attack_fail:
	{
		Attack.dwOffenseUserIndex	= pOffense->GetID();
		Attack.bType				= 5;
		NetSendToUser( pOffense->m_dwConnectionIndex, (char*)&Attack, Attack.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	}
}

void CmdSkillLevelUp( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_SKILL_LEVELUP* pPacket = (CTDS_SKILL_LEVELUP*)pMsg;
	DSTC_SKILL_INFO		pSendPacket;

	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);

	if(!pUser)	return;	
	if(pUser->GetCurLayerIndex()	 == INVALID_DUNGEON_LAYER)	return;
	
	if(pUser->m_wPointSkill>0)
	{		
		Effect* pEffectUp	= g_pEffectLayer->GetEffectInfo((BYTE)pPacket->nSkillIndex);

		// ���� ������ �ø��� �ִ� ��ų���� �˻��ض�.
		if (!(pEffectUp->bAbleClass & (1<<(pUser->GetClass()-1))))	return;

		BYTE bSkillMastery = g_pEffectLayer->GetSkillMasteryKind((BYTE)pPacket->nSkillIndex);
		Effect* pEffectTar	= g_pEffectLayer->GetEffectInfo(bSkillMastery);

#ifndef __SKILL_MASTERY_ENABLE
	if (bSkillMastery == pEffectUp->bID)
		return;
#endif
		if(pEffectUp->dwMinMastery <=(DWORD)pEffectTar->GetMaxMastery(pUser->GetLevel(), pUser->m_pwSkillLevel[bSkillMastery], pUser->GetClass()))
		{
			
			if(pUser->m_pwSkillLevel[pPacket->nSkillIndex]<((bSkillMastery == pPacket->nSkillIndex)?255:25))
			{
				pUser->m_wPointSkill	-= 1;
				pUser->m_pwSkillLevel[pPacket->nSkillIndex] += 1;

				if (pUser->m_pwSkillLevel[bSkillMastery] < 255)
					pUser->m_pwSkillLevel[bSkillMastery] = (pUser->m_pwSkillLevel[bSkillMastery]+1);

				pSendPacket.bSkillInfo		= TRUE;
				pSendPacket.bySkillIndex	= (BYTE)pPacket->nSkillIndex;
				
				pUser->SendAllStatus();	// ��� ���¸� �����ؼ� ������.					
			}
		}				
	}
	else
	{
		pSendPacket.bySkillIndex	= pPacket->nSkillIndex;
		pSendPacket.bSkillInfo		= FALSE;	
	}

	NetSendToUser(dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}


void CmdStatusLevelUp( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{	
	CTDS_CHAR_LEVELUP*	pPacket = (CTDS_CHAR_LEVELUP*)pMsg;

	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);

	if(!pUser)	return;	
	else if (pUser->GetCurLayerIndex() == INVALID_DUNGEON_LAYER)	return;
	else if (pUser->GetUnitStatus() == UNIT_STATUS_DEAD)	return;

	CUnit* pUnit = NULL;
	if (pPacket->byObjectType == OBJECT_TYPE_MONSTER)
		pUnit = pUser->GetGuardian(0);
	else if (pPacket->byObjectType == OBJECT_TYPE_PLAYER)
		pUnit = pUser;
	
	if (pUnit)
		pUnit->AddStatusPoint((STATUS_POINT_KIND)pPacket->byStatusType);
	
}


void CmdDungeonRequestInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	CTDS_DUNGEON_REQUEST_INFO*	pInfo = (CTDS_DUNGEON_REQUEST_INFO*)pMsg;
	CDungeonLayer*				pLayer;
	
	CUser*	pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
	if( !pUser ) return;
	if(!pUser->GetCurDungeon())	return;

	pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer ) return;
	
	switch( pInfo->bRequestType)
	{
	case 1:			// User
		{
			CSection*	pSection;
			BOOL		bSendInfo = FALSE;
			CUser*		pInfoUser = g_pUserHash->GetData( pInfo->dwRequestIndex ); //pUser->GetCurDungeon()->GetUser( pInfo->dwRequestIndex ); //(CUser*)g_pNet->GetUserInfo( pInfo->dwRequestIndex );
			if( !pInfoUser ) return;
			
			pSection = pLayer->GetSection( pUser->GetPrevSectionNum() );
			if( !pSection ) return;

			if( pUser->GetPrevSectionNum() != pInfoUser->GetPrevSectionNum() )
			{
				for( int i=0; pSection->m_wLink[i] != 0; i++ )
				{
					if( pInfoUser->GetPrevSectionNum() == pSection->m_wLink[i] )
					{
						bSendInfo = TRUE;
						break;
					}
				}
			}
			else bSendInfo = TRUE;
			
			if( !bSendInfo ) return;

			DSTC_APPEAR		AppearUser;
			MakeAppearUser(&AppearUser, pInfoUser, 0);
			
			if( !(AppearUser.dwGuildWarFlag & G_W_F_OBSERVER) )//������ �������� �ƴϸ� �����ش�.
			{
				NetSendToUser( dwConnectionIndex, (char*)&AppearUser, AppearUser.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
			}

			if( pInfoUser->GetUnitStatus() == UNIT_STATUS_WALKING || pInfoUser->GetUnitStatus() == UNIT_STATUS_RUNNING )
			{
				DSTC_MOVE move;

				float fSpeed = 0.0f;

				if(pInfoUser->GetUnitStatus() == UNIT_STATUS_RUNNING)
				{
					if(pUser->m_wPerWeight >= WEIGTH_80PER_OVER)
						fSpeed = pUser->GetMoveSpeed()/3;
				}
					
				move.dwIndex			= pInfoUser->GetID();
				move.bMoveType			= (BYTE)pInfoUser->GetUnitStatus();
				move.fSpeed				= (pInfoUser->GetMoveSpeed()-fSpeed)/1000.f;
				move.v2MoveStartPos		= *pInfoUser->GetCurPosition();
				move.v2MoveDirection	= *pInfoUser->GetCurDirection();
//				move.wDestX				= pInfoUser->wDestX;
//				move.wDestZ				= pInfoUser->wDestZ;
			
				// �޽��� ����.
				NetSendToUser( dwConnectionIndex, (char*)&move, move.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
// 				pUser->GetCurDungeon()->m_pDungeonLayer[ pUser->GetCurLayerIndex() ]->BroadCastSectionMsg( (char*)&move, move.GetPacketSize(), pUser );
			}
		}
		break;

	case 2:			// Monster
		{
			CSection*	pSection;
			BOOL		bSendInfo = FALSE;
			CMonster*	pInfoMonster = g_pMonsterTable->GetMonster( pInfo->dwRequestIndex );
			if( !pInfoMonster ) return;

			pSection = pLayer->GetSection( pUser->GetPrevSectionNum() );
			if( !pSection ) return;
			if( pUser->GetPrevSectionNum() != pInfoMonster->GetPrevSectionNum() )
			{
				for( int i=0; pSection->m_wLink[i] != 0; i++ )
				{
					if( pInfoMonster->GetPrevSectionNum() == pSection->m_wLink[i] )
					{
						bSendInfo = TRUE;
						break;
					}
				}
			}
			else bSendInfo = TRUE;

			if( !bSendInfo ) return;

			DSTC_APPEAR_MON	AppearMon;
			MakeAppearMonser( &AppearMon, pInfoMonster );
			NetSendToUser( dwConnectionIndex, (char*)&AppearMon, AppearMon.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

			if( pInfoMonster->GetUnitStatus() == UNIT_STATUS_WALKING 
				|| pInfoMonster->GetUnitStatus() == UNIT_STATUS_RUNNING )
			{
				DSTC_MOVE_MON	MoveMon;
				MoveMon.fSpeed			= pInfoMonster->GetMoveSpeed();
				MoveMon.dwMonsterIndex	= pInfoMonster->GetID();
				MoveMon.v2MoveStartPos	= *pInfoMonster->GetCurPosition();
				MoveMon.v2MoveStopPos	= *pInfoMonster->GetDestPosition();
				MoveMon.v2MoveDirection	= *pInfoMonster->GetCurDirection();
				MoveMon.bMoveType		= UNIT_STATUS_WALKING;

				NetSendToUser( dwConnectionIndex, (char*)&MoveMon, MoveMon.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
//				pInfoMonster->GetCurDungeonLayer()->BroadCastSectionMsgExceptLink( (char*)&MoveMon, MoveMon.GetPacketSize(), pInfoMonster->GetPrevSectionNum() );
			}
		}
		break;
	}
}


void CmdCallGuardian(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{	
	CTDS_DUNGEON_CALLGARDIAN*	pCall = ( CTDS_DUNGEON_CALLGARDIAN* )pMsg;

	CUser*	pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );	

	if( !pUser )														return;
	else if(pUser->GetUnitStatus()==UNIT_STATUS_DEAD)					return;
	else if( pUser->GetGuardian(pCall->bZipCode) )					return;
	else if( pUser->m_pInv_Guardian[ pCall->bZipCode ].m_wItemID == 0 ) return;
	else if( pUser->m_pInv_Guardian[pCall->bZipCode].m_Item_Guardian.bCreate == GUARDIAN_CREATE_STATUS_CREATING ) return;

	char szSQL[1024] = {0,};
	LPGUARDIAN_CREATION_INFO pInfo = NULL;
	CItem* pGuardianItem = &pUser->m_pInv_Guardian[pCall->bZipCode];
	CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pGuardianItem->GetID());
	
	if ( GUARDIAN_ITEM_TYPE_EGG == pBaseItem->BaseItem_Guardian.bType  )
	{
		BOOL bSend		= TRUE;
		BOOL bSummon	= FALSE;
		
		if ( (!pGuardianItem->m_Item_Guardian.dwEndLife || IsGrowGuardianItem(pGuardianItem) ) &&
			!pUser->IsSummonGuardian(pCall->bZipCode))
		{
			// �˺�ȭ ���̸� �����Ѵ�.
			if (!pGuardianItem->m_Item_Guardian.dwEndLife)
			{
				pGuardianItem->m_Item_Guardian.dwEndLife = GetDueDay(pBaseItem->BaseItem_Guardian.dwDueDayTime);
			}
			
			pUser->AttachItem(pGuardianItem);
			bSummon = TRUE;
		}
		else if ( (!pGuardianItem->m_Item_Guardian.dwEndLife || IsGrowGuardianItem(pGuardianItem) )	&&
			pUser->IsSummonGuardian(pCall->bZipCode))
		{
			// �˺�ȭ ���̸� ����.
			pUser->UnSummonGuardian(pCall->bZipCode);
		}
		else if (IsOverDueDay(pGuardianItem->m_Item_Guardian.dwEndLife))
		{
			// ����.. ���� ����ڳ�.
			pGuardianItem->m_wItemID = pBaseItem->BaseItem_Guardian.wDieItemID;
			pUser->UnSummonGuardian(pCall->bZipCode);
		}
		else if (!IsOverDueDay(pGuardianItem->m_Item_Guardian.dwEndLife) &&
			pBaseItem->BaseItem_Guardian.wCompleteTime == pGuardianItem->m_Item_Guardian.dwCompleteTime)
		{
			// ���� ��ȭ ���ֱ�.
			pGuardianItem->m_wItemID = pBaseItem->BaseItem_Guardian.wCreatureID;
			pGuardianItem->m_Item_Guardian.dwCompleteTime = 0;	// �������ʹ� ����ġ�� �̿�ȴ�.
			pGuardianItem->m_Item_Guardian.dwEndLife = 0;
			pUser->UnSummonGuardian(pCall->bZipCode);
		}
		else 
		{
			bSend = FALSE;
		}
		
		if (bSend)
		{
			SendToUpdate_GuardianItem(dwConnectionIndex, pGuardianItem);
		}

		SendSummonEggStatus(dwConnectionIndex, bSummon, pGuardianItem);		
	}
	else if ( GUARDIAN_ITEM_TYPE_ADULT == pBaseItem->BaseItem_Guardian.bType )
	{
		if( GUARDIAN_CREATE_STATUS_ALREADYCALL == pGuardianItem->m_Item_Guardian.bCreate )
		{	
			if (IsOverDueDay(pGuardianItem->m_Item_Guardian.dwEndLife))
			{
				// ����.. ��ȥ���� ����.
				pGuardianItem->m_wItemID = pBaseItem->BaseItem_Guardian.wSoulItemID;
				pUser->UnSummonGuardian(pCall->bZipCode);
				SendToUpdate_GuardianItem(dwConnectionIndex, pGuardianItem);	
			}
			else
			{
				// �̹� ������ ������̶�� �ٷ� �ҷ�������.
				pInfo = new GUARDIAN_CREATION_INFO;
				memset( pInfo, 0, sizeof( GUARDIAN_CREATION_INFO ) );
				pInfo->pUser	= pUser;
				pInfo->bZipCode	= pCall->bZipCode;

				wsprintf( szSQL, "select * from GuardianInfo where ID=%d", pGuardianItem->m_Item_Guardian.dwID );	
				g_pDb->THOpenRecord( szSQL, DB_UUID_GUARDIAN_INFO, pInfo, 1, GAME_DB );
			}			
		}
		else if( GUARDIAN_CREATE_STATUS_NONE == pGuardianItem->m_Item_Guardian.bCreate )
		{
			// ���� �������� ���� ���¶�� �����Ѵ�.
			DWORD			dwHP, dwMP;
			LPBASEMONSTER	pBaseMonster;
			pBaseMonster = g_pBaseMonsterHash->GetData( pBaseItem->BaseItem_Guardian.wBaseGuardianID );
			if( !pBaseMonster ) return;
		
			pInfo = new GUARDIAN_CREATION_INFO;
			memset( pInfo, 0, sizeof( GUARDIAN_CREATION_INFO ) );

			pInfo->bUserGuardian	= TRUE;
			pInfo->pUser			= pUser;
			pInfo->bZipCode			= pCall->bZipCode;

			pGuardianItem->m_Item_Guardian.bCreate			= GUARDIAN_CREATE_STATUS_CREATING;	// �������̶�ǥ��.
			pGuardianItem->m_Item_Guardian.dwCompleteTime	= 0;	// �������ʹ� ����ġ�� �̿�ȴ�.
			pGuardianItem->m_Item_Guardian.dwEndLife		= GetDueDay(pBaseItem->BaseItem_Guardian.dwDueDayTime);

			// ó�� �����ɋ��� HP�� MP�� ���Ѵ�.
			dwHP = pBaseMonster->dwBaseHP	+ pBaseMonster->bLHP;
			dwMP = pBaseMonster->wBaseMP	+ pBaseMonster->bLMP;
		
			wsprintf( szSQL, "CZP_INSERT_GUARDIAN_INFO '%s', %d, %d, %d", pBaseMonster->szMonsterName_Kor
				, pBaseItem->BaseItem_Guardian.wBaseGuardianID, dwHP, dwMP );
			g_pDb->THOpenRecord( szSQL, DB_CREATE_GUARDIAN, pInfo, 1, GAME_DB );
		}
	}
}


void CmdGuardianAttack( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_GUARDIAN_ATTACK*	pAttack = ( CTDS_GUARDIAN_ATTACK* )pMsg;

	CUser*	pUser;
	pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
	if( !pUser ) return;

	if( !pUser->GetGuardian( pAttack->bZipCode ) ) return;		// ������� ���ٸ� ����.~
	
	switch( pAttack->bAttackType )
	{
	case 0:	// Ÿ���� ���ͷ� �����Ҷ�.
		{
			CMonster*	pTargetMon = g_pMonsterTable->GetMonster( pAttack->dwTargetIndex );
			if( !pTargetMon ) return;
				
			if( pTargetMon == pUser->GetGuardian(pAttack->bZipCode) ) return;	// ���ش� �ȵ�.

			pUser->GetGuardian(pAttack->bZipCode)->SetTargetAI((CUnit*) pTargetMon );
		}
		break;

	case 1:	// Ÿ���� ������ �����Ҷ�.
		{
			CUser*		pTargetUser	= g_pUserHash->GetData( pAttack->dwTargetIndex );
			if( !pTargetUser ) return;
			
			if( pTargetUser == pUser ) return; // �ðǹ�����.

			pUser->GetGuardian( pAttack->bZipCode )->SetTargetAI((CUnit*) pTargetUser );
		}
		break;
	}
}

void CmdGuardianCommand( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_GUARDIAN_COMMAND*	pCommand = ( CTDS_GUARDIAN_COMMAND* )pMsg;

	CUser*	pUser;
	pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
	if( !pUser ) return;
	if(!pUser->GetCurDirection())	return;

	if(pUser->GetUnitStatus()==UNIT_STATUS_DEAD)
		return;

	CMonster* pGuardian = pUser->GetGuardian( pCommand->bZipCode );
	if( !pGuardian ) return;

	switch( pCommand->bCommand )
	{
	case 0:			// �����.
		{
			pGuardian->ClearTargetAI();
		}
		break;

	case 1:			// Remove Guardian.
		{
			pUser->UnSummonGuardian(pCommand->bZipCode);
		}
		break;
	}
}

void CmdMonsterCommand( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_MONSTER_COMMAND*	pCommand = (CTDS_MONSTER_COMMAND*)pMsg;

	CUser*	pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
	if( !pUser ) return;
	int i;

	switch( pCommand->bCommand )
	{
	case 0:		// ���.
		{
			for( i=0; i<MAX_USER_GUARDIAN; i++ )
			{
				pUser->m_pMonster[i]->ClearTargetAI();
			}
		}
		break;

	case 1:		// ����.
		{
			if( pCommand->bAttackType == 0 )	// Attack Monster...
			{
				CMonster*	pTargetMon = g_pMonsterTable->GetMonster( pCommand->dwTargetIndex );
				if( !pTargetMon ) return;

				for( i=0; i<MAX_USER_GUARDIAN; i++ )
				{
					if( pUser->m_pMonster[i] )
					{
						if( pTargetMon == pUser->m_pMonster[i] ) return;
						if( pTargetMon == pUser->GetGuardian(BYTE(i)) ) return;
					}
				}
				
				for( i=0; i<MAX_USER_GUARDIAN; i++ )
				{
					if( pUser->m_pMonster[i] )
					{
						pUser->m_pMonster[i]->SetTargetAI((CUnit*) pTargetMon );
					}
				}
			}
			else if( pCommand->bAttackType	== 1 ) // Attack User...
			{
				CUser*		pTargetUser	= g_pUserHash->GetData( pCommand->dwTargetIndex );
				if( !pTargetUser ) return;
				
				if( pTargetUser == pUser ) return; // �ðǹ�����.

				for( i=0; i<MAX_USER_GUARDIAN; i++ )
				{
					if( pUser->m_pMonster[i] )
					{
						pUser->m_pMonster[i]->SetTargetAI((CUnit*) pTargetUser );
					}
				}
			}
		}
		break;
	}
}

void CmdGuardianCommandMove( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_GUARDIAN_COMMAND_MOVE*	pMove = (CTDS_GUARDIAN_COMMAND_MOVE*)pMsg;
	
	int i;
	CMonster*	pMonster;

	// Guardian.
	for( i=0; pMove->pdwGuardianIndex[i] != 0 || i < MAX_USER_GUARDIAN; i++ )
	{
		pMonster = g_pMonsterTable->GetMonster( pMove->pdwGuardianIndex[i] );
		if( pMonster )
		{
			if( pMonster->GetUnitStatus() == UNIT_STATUS_DEAD ) continue;
			pMonster->ScheduleMoveDestPos( &pMove->v2DestPos );
		}
	}

	// Monster.
	for( i=0; pMove->pdwMonsterIndex[i] != 0 || i < MAX_USER_GUARDIAN; i++ )
	{
		pMonster = g_pMonsterTable->GetMonster( pMove->pdwMonsterIndex[i] );
		if( pMonster )
		{
			if( pMonster->GetUnitStatus() == UNIT_STATUS_DEAD ) continue;
			pMonster->ScheduleMoveDestPos( &pMove->v2DestPos );
		}
	}
}

void CmdGuardianCommandAttack( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_GUARDIAN_COMMAND_ATTACK*	pAttack = (CTDS_GUARDIAN_COMMAND_ATTACK*)pMsg;

	int i=0;
	CMonster*	pMonster=0, *pTargetMon=0;
	CUser*		pTargetUser=0;
	CUser*	pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
	if( !pUser ) return;

	if(pUser->GetUnitStatus()==UNIT_STATUS_DEAD)
		return;

	if( pAttack->bAttackType == 0 )
	{
		pTargetMon = g_pMonsterTable->GetMonster( pAttack->dwTargetIndex );
		if( !pTargetMon ) return;
	}
	else if( pAttack->bAttackType == 1 )
	{		
		pTargetUser = g_pUserHash->GetData( pAttack->dwTargetIndex );
		
		if(!pTargetUser)
			return;
	
		if(!IsGuildWar(pUser, pTargetUser) )
		{
			CDungeon *pDungeon =pUser->GetCurDungeon();
			
			if(pUser->IsAlliance((CUnit*)pTargetUser))
			{
				return;
			}

			if( pDungeon->GetDungeonDataEx()->IsStadium() )//������� ���� �� �ְ� : hwoarang 050219 
			{//������̸�
			}
			else
			{//�����ƴϸ�
				if(pUser->m_sPKDescInfo.m_bPKMode==0)
				{
					return;
				}
			}
		}
/*
		if(!IsGuildWar(pUser->GetCurDungeon()
			, pUser->m_dwGuildId
			, pTargetUser->m_dwGuildId
			, pUser->m_dwPartyId
			, pTargetUser->m_dwPartyId
			, pTargetUser->m_enGodMode)	)					
		{
			if(pUser->m_sPKDescInfo.m_bPKMode==0)
				return;
			if(pUser->IsAlliance((CUnit*)pTargetUser))
				return;
		}		
*/	}

	// Guardian.
	for( i=0; pAttack->pdwGuardianIndex[i] != 0 || i < MAX_USER_GUARDIAN; i++ )
	{
		pMonster = g_pMonsterTable->GetMonster( pAttack->pdwGuardianIndex[i] );
		if( pMonster )
		{
			pMonster->SetStatus( UNIT_STATUS_NORMAL, TRUE );
			if( pAttack->bAttackType == 0 )
			{	
				if( pMonster->GetUnitStatus() == UNIT_STATUS_DEAD ) 
					continue;
				pMonster->SetTargetAI((CUnit*) pTargetMon );
			}
			else if( pAttack->bAttackType == 1 )
			{
				if( pMonster->GetUnitStatus() == UNIT_STATUS_DEAD ) 
					continue;
				pMonster->SetTargetAI((CUnit*) pTargetUser );
			}
		}
	}

	// Monster.
	for( i=0; pAttack->pdwMonsterIndex[i] != 0 || i < MAX_USER_GUARDIAN; i++ )
	{
		pMonster = g_pMonsterTable->GetMonster( pAttack->pdwMonsterIndex[i] );
		if( pMonster )
		{
			pMonster->SetStatus( UNIT_STATUS_NORMAL, TRUE );
			if( pAttack->bAttackType == 0 )
			{	
				if( pMonster->GetUnitStatus() == UNIT_STATUS_DEAD ) continue;
				pMonster->SetTargetAI((CUnit*) pTargetMon );
			}
			else if( pAttack->bAttackType == 1 )
			{
				if( pMonster->GetUnitStatus() == UNIT_STATUS_DEAD ) continue;
				pMonster->SetTargetAI((CUnit*) pTargetUser );
			}
		}
	}
}

// ����� ��ų ����
void CmdGuardianSkillSelect( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_GUARDIAN_SKILL_SELECT* pGuardianSkill = (CTDS_GUARDIAN_SKILL_SELECT*)pMsg;
	CUser*	pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
	CMonster *pGuardian = NULL;

	if(!pUser ) return;
	pGuardian = pUser->GetGuardian(0);
	if(!pGuardian) return;

	pGuardian->SetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON, pGuardianSkill->bSkill);
}

void CmdGMQuickMove( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_GM_QUICK_MOVE* pMove = (CTDS_GM_QUICK_MOVE*)pMsg;
	CDungeonLayer*		pLayer;

	CUser*	pUser	= (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
	if( !pUser )	return;
	if( !pUser->GetCurDungeon())	return;
	
	pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer ) return;

	CMap*		pMap = pLayer->GetMap();
	MAP_TILE*	pTile = pMap->GetTile( pMove->v2MoveStartPos.x, pMove->v2MoveStartPos.y );
	
	if( !pLayer->GetSection(pUser->GetPrevSectionNum()) ) return;
	if( pUser->GetPrevSectionNum() != pTile->wAttr.uSection )
	{
		int					i=0;
		BYTE				IntersectSection[MAX_SECTION_LINK_NUM]={0,};
		CUser*				pDisAppearUser=0;
		CMonster*			pDisAppearMonster=0;
		POSITION_			UserPos;
		POSITION_			MonsterPos;
		CSection*			pSection=0;
		DSTC_CHANGE_SECTION	Section;
		DSTC_DISAPPEAR		DisAppear;
		BYTE				oldSectionNum = pUser->GetPrevSectionNum();
	
		pLayer->RemoveUserSection( pUser );
		pLayer->InsertUserSection( pUser,  pTile->wAttr.uSection );
		
		pLayer->IntersectSection( pUser->GetPrevSectionNum(), oldSectionNum, IntersectSection );

		// �������� ������ �����鿡�� ���� �޼����� ������.
		DisAppear.dwUserIndex		= pUser->GetID();
		for( i=0; IntersectSection[i] != 0; i++ )
		{
			pLayer->BroadCastSectionMsgExceptLink( (char*)&DisAppear, DisAppear.GetPacketSize(), IntersectSection[i] );
		
			// ���⼭ ���������� ������ DisAppear������ �����.
			pSection = pLayer->GetSection( IntersectSection[i] );
			if( !pSection ) continue;

			UserPos = pSection->m_pPcList->GetHeadPosition();
			while( UserPos )
			{
				pDisAppearUser = (CUser*)pSection->m_pPcList->GetNext( UserPos );
				// mod by deepdark
				Section.pdwDisAppearIndex[Section.bDisAppearUserCount++] = pDisAppearUser->GetID();
			//	Section.pdwDisAppearUserIndex[dwUserCount] = pDisAppearUser->GetID();
			//	dwUserCount++;
			}

			MonsterPos = pSection->m_pMonsterList->GetHeadPosition();
			while( MonsterPos )
			{
				pDisAppearMonster = (CMonster*)pSection->m_pMonsterList->GetNext( MonsterPos );
				// mod by deepdark
				Section.pdwDisAppearIndex[Section.bDisAppearUserCount + Section.bDisAppearMonsterCount++] = pDisAppearMonster->GetID();
			//	Section.pdwDisAppearMonsterIndex[dwMonsterCount] = pDisAppearMonster->GetID();
			//	dwMonsterCount++;
			}
		}
		
		Section.wChageSectionNum = pUser->GetPrevSectionNum();
		NetSendToUser( pUser->m_dwConnectionIndex, (char*)&Section, Section.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		
		// �ٲ� ���ǿ��� ���� ���� ������ �޴´�.
		pLayer->BroadCastIntersectSectionInfo( oldSectionNum, pUser->GetPrevSectionNum(), pUser );
	}

	MAP_TILE* pDestTile = pMap->GetMap( pMove->wDestX, pMove->wDestZ );
	if(!pDestTile)
		__asm int 3

	// �̵��ϴ°��� �����ϰ� ��Ŷ�� ������.
	VECTOR2 v2Start = { pDestTile->x, pDestTile->z };
//	v2Start.x = pDestTile->x;
//	v2Start.y = pDestTile->z;
	pUser->SetCurPosition(&v2Start);
	pUser->SetCurDirection(&pMove->v2MoveDirection);
	pUser->SetStatus( UNIT_STATUS_NORMAL );
//	pUser->m_dwLastMoveTick	= g_dwTickCount;
	pUser->m_pTileDest		= pMap->GetMap( pMove->wDestX, pMove->wDestZ );

	DSTC_STOP	stop;
	stop.dwUserIndex	= pUser->GetID();
	stop.v2StopPos		= *pUser->GetCurPosition();
	
	// �޽��� ����.
	pLayer->BroadCastSectionMsg( (char*)&stop, stop.GetPacketSize(), pUser->GetCurMapTile()->wAttr.uSection );
}


void CmdFindHack( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_FIND_HACK*	pHack = (CTDS_FIND_HACK*)pMsg;

	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	if(!pUser)	return;

	SpeedHackLog( pUser, pHack->dwHackNum );
}

void CmdTickCheck( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_TICK_CHECK* pTick = (CTDS_TICK_CHECK*)pMsg;

	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	if(!pUser) return;

	// ó�� ���� �������� ���� ����ϰ� �׳� ��������.
	if( pUser->m_dwTemp[ USER_TEMP_LASTCLIENTTICK ] == 0  && pUser->m_dwTemp[ USER_TEMP_LASTSERVERTICK ] == 0 )
	{
		pUser->m_dwTemp[ USER_TEMP_LASTCLIENTTICK ] = pTick->dwCurClientTick;
		pUser->m_dwTemp[ USER_TEMP_LASTSERVERTICK ] = g_dwTickCount;
		return;
	}

	// ������� �Դٸ� ���� �˻縦 �غ���.

	DWORD	dwServerCheckTick;		// ���� �ð��� �󸶳� ���� �߳�.
	DWORD	dwClientCheckTick;		// Ŭ���̾�Ʈ�� �ð��� �󸶳� �����߳�.

	dwServerCheckTick = g_dwTickCount - pUser->m_dwTemp[ USER_TEMP_LASTSERVERTICK ];
	dwClientCheckTick = pUser->m_dwTemp[ USER_TEMP_LASTCLIENTTICK ] - pTick->dwCurClientTick;

	pUser->m_dwTemp[ USER_TEMP_LASTSERVERTICK ] = g_dwTickCount;
	pUser->m_dwTemp[ USER_TEMP_LASTCLIENTTICK ] = pTick->dwCurClientTick;
} 


#if defined NEW_COMMAND

// MsgProc_for_y2jinc.cpp ����..

#else	
// ! NEW_COMMAND

void CmdGmCommand( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_GM_COMMAND*	pCommand = (CTDS_GM_COMMAND*)pMsg;
	
	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	if(!pUser) return;
	
	if( !pUser->GetCurDungeon())	
		return;

#if defined TAIWAN_LOCALIZING
	if (!pUser->IsGMUser())
	{
		return;
	}
#endif

	switch( pCommand->wCommand )
	{
	case GM_COMMAND_PK_MODE:
		{
			pUser->GetCurDungeon()->SetTemp(DUNGEON_TEMP_PKMODE,pCommand->dwTemp[0]);
		}
		break;
		
	case GM_COMMAND_CHAT_DISABLE:
		{
			if( pCommand->dwTemp[0] == 1 )	// mean All.
			{
				pUser->GetCurDungeon()->SetTemp(DUNGEON_TEMP_CHAT_DISABLE, pCommand->dwTemp[1]);
			}	
		}
		break;
		
	case GM_COMMAND_CREATE:
		{
			if( pCommand->dwTemp[0] == 1 ) // mean Monster create.
			{
				pUser->GetCurDungeonLayer()->CallMonsterByGM( pUser
					, 0
					, pCommand->dwTemp[1]
					, pCommand->dwTemp[2] );
			}
			else if( pCommand->dwTemp[0] == 2 )
			{
				CreateItemByGM( pUser, pCommand->dwTemp[1] );
			}
		}
	}
}
#endif