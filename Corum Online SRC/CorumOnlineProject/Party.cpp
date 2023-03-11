//==============================================================//
// Code by Jang.									2003.08.01	//
//==============================================================//
#include "Party.h"
#include "GameControl.h"
#include "../CommonServer/CommonHeader.h"
#include "User.h"
#include "Message.h"
#include "PartyPortalWnd.h"
#include "HashTable.h"
#include "PartyJoinWnd.h"
#include "Interface.h"
#include "GroupWnd.h"
#include "DSMiniMap.h"
#include "Chat.h"
#include "UserInterface.h"
#include "PartyRequestWnd.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "DUNGEON_DATA_EX.h"
#include "PartyBoardInfoWnd.h"
#include "ItemNative.h"
#include "PartyEntryWnd.h"
#include "PartyMatchingWnd.h"


//==============================================================//
// ��Ƽ ��û �޼��� �޾����� ó��.								//
//==============================================================//
void CmdPartyUserRequest(char* pMsg, DWORD dwLen)
{
	WSTC_PARTY_USERREQUEST* pPacket = (WSTC_PARTY_USERREQUEST*)pMsg;

	char szInfo[0xff] = {0,};

	CPartyRequestWnd* pPartyRequestWnd = CPartyRequestWnd::GetInstance();
	memset(pPartyRequestWnd->m_szName, 0, sizeof(pPartyRequestWnd->m_szName));
	memset(pPartyRequestWnd->m_szRequestName, 0, sizeof(pPartyRequestWnd->m_szRequestName));
	pPartyRequestWnd->m_bChk				= TRUE;
	pPartyRequestWnd->m_dwUserIndex			= pPacket->dwUserIndex;
	pPartyRequestWnd->m_dwRequestUserIndex	= pPacket->dwReqUserIndex;
	__lstrcpyn(pPartyRequestWnd->m_szName, pPacket->szName, MAX_CHARACTER_NAME_REAL_LENGTH );
	__lstrcpyn(pPartyRequestWnd->m_szRequestName, pPacket->szRequestName, MAX_CHARACTER_NAME_REAL_LENGTH );	
	pPartyRequestWnd->SetActive();
		
	// "%s �Կ��Լ� ��Ƽ ��û �޼����� �Խ��ϴ�."
	wsprintf(szInfo, g_Message[ETC_MESSAGE709].szMessage, pPacket->szName); 
	DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
}


//==============================================================//
// ��Ƽ ��û ���� �Ǵ� ���� �޼���.								//
//==============================================================//
void CmdPartyUserRequestFail(char* pMsg, DWORD dwLen)
{
	WSTC_PARTY_USERREQUEST_FAIL* pPacket = (WSTC_PARTY_USERREQUEST_FAIL*)pMsg;

	DWORD dwMsgIndex = 0;
	switch(pPacket->byType)
	{
	case 1: dwMsgIndex = ETC_MESSAGE710; break;		// "��Ƽ ���Կ�û �޼����� �����Ͽ����ϴ�."
	case 2: dwMsgIndex = ETC_MESSAGE711; break;		// "���Կ�û�� ĳ���Ͱ� �����ϴ�."
	case 3: dwMsgIndex = ETC_MESSAGE712; break;		// "��Ƽ���� �Բ��� ���� ��û�� �����ϼ̽��ϴ�."
	case 5: dwMsgIndex = ETC_MESSAGE1238; break;	// "����� �������̶� ��Ƽ�� �Ͻ� �� �����ϴ�."
	case 4: return;
	}

	DisplayMessageAdd(g_Message[dwMsgIndex].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
}


//==============================================================//
// ��Ƽ ������ ���� �޼���.										//
//==============================================================//
void CmdPartyMagicArrayFail(char* pMsg, DWORD dwLen)
{
	DSTC_PARTY_MAIGCARRAYFAIL* pPacket = (DSTC_PARTY_MAIGCARRAYFAIL*)pMsg;

	if(pPacket->byErrorCode==1)
	{
		// "���� ��Ƽ �������� ����� �Դϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE619].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); 
	}
}


//==============================================================//
// ��Ƽ ������ ����.											//
//==============================================================//
void CmdPartyArray(char* pMsg, DWORD dwLen)
{
	DSTC_PARTY_ARRAY* pPacket = (DSTC_PARTY_ARRAY*)pMsg;

	if(pPacket->bType==1)
		CUserInterface::GetInstance()->m_dwMagicArray	= pPacket->wId;
	else
		CUserInterface::GetInstance()->m_dwMagicArray	= 0;		
}


//==============================================================//
// ��Ƽ ��Ż ����.												//
//==============================================================//
void CmdPartyPotalFail(char* pMsg, DWORD dwLen)
{
	DSTC_PARTY_PORTALFAIL* pPacket = (DSTC_PARTY_PORTALFAIL*)pMsg;

	char szInfo[0xff] = {0,};

	if(pPacket->byResult==1)
	{
		// ������� ��� ���� ��� //
		// "%s ���� ��Ƽ��Ż�� �ź��ϼ̽��ϴ�."
		wsprintf(szInfo, g_Message[ETC_MESSAGE620].szMessage, pPacket->szName); 
		DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
	}
	else if(pPacket->byResult==2)
	{
		// ���� ����, ���� ���� ���� ��� //
		// "���� ����, ���� �������� ��Ż�� ����Ҽ� �����ϴ�."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE621].szMessage); 
		DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
	}
	else if(pPacket->byResult==3)
	{
		// �������� ���� ��� //
		// "%s ���� �������� �����ϴ�."
		wsprintf(szInfo, g_Message[ETC_MESSAGE622].szMessage, pPacket->szName); 
		DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
	}
	else if(pPacket->byResult==4)
	{
		// �������� ���� ��� //
		//"���������� ��Ż�� ����Ҽ� �����ϴ�."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE623].szMessage);
		DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
	}
	else if(pPacket->byResult==5)
	{
		// �������� ���� ��� //
		//"��Ż������ ����Ҽ� �����ϴ�."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE713].szMessage); 
		DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
	}
}


//==============================================================//
// ��Ƽ ��Ż ���� �޼���.										//
//==============================================================//
void CmdPartyPotalMove(char* pMsg, DWORD dwLen)
{
	DSTC_PARTY_PORTALMOVE* pPacket = (DSTC_PARTY_PORTALMOVE*)pMsg;
	
	g_pMainPlayer->SetAction(36);
	g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;	
	SendStopPacket();

	CTDS_PORTAL_MOVE packet;	
	packet.wPortalItemID	= 0;
	packet.wToDungeonID		= pPacket->wDungeonID;					
	packet.bLayerNo			= pPacket->byLayerIndex;
	packet.v2Pos.x			= pPacket->fPosX;
	packet.v2Pos.y			= pPacket->fPosZ;

	if(g_pThisDungeon->GetDungeonType() != DUNGEON_TYPE_EVENT)
	{
		g_pMainPlayer->wPrevDungeonID = (WORD)g_pThisDungeon->m_dwID;
	}
	
	g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
	g_pMainPlayer->SetStatus(UNIT_STATUS_PORTAL_MOVING);	
}


//==============================================================//
// ��Ƽ ��Ż ���� �޼���(����).									//
//==============================================================//
void CmdPartyPotalWorldMove(char* pMsg, DWORD dwLen)
{
	WSTC_PARTY_PORTALMOVE* pPacket = (WSTC_PARTY_PORTALMOVE*)pMsg;
	
	g_pMainPlayer->SetAction(36);
	g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;	
	SendStopPacket();

	CTDS_PORTAL_MOVE packet;	
	packet.wPortalItemID	= 0;
	packet.wToDungeonID		= pPacket->wDungeonID;					
	packet.bLayerNo			= pPacket->byLayerIndex;
	packet.v2Pos.x			= pPacket->fPosX;
	packet.v2Pos.y			= pPacket->fPosZ;

	if(g_pThisDungeon->GetDungeonType() != DUNGEON_TYPE_EVENT)
	{
		g_pMainPlayer->wPrevDungeonID = (WORD)g_pThisDungeon->m_dwID;
	}
	
	g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
	g_pMainPlayer->SetStatus(UNIT_STATUS_PORTAL_MOVING);	
}


//==============================================================//
// ��Ƽ ��Ż ��� ó��.											//
//==============================================================//
void CmdPartyPotalUsed(char* pMsg, DWORD dwLen)
{
	WSTC_PARTY_PORTALUSED* pPacket = (WSTC_PARTY_PORTALUSED*)pMsg;
	
	if(pPacket->dwResult == 0)
	{
		// �ź� //
		char szInfo[0xff] = {0,};
		// "%s ���� ��Ƽ��Ż�� �ź��ϼ̽��ϴ�."
		wsprintf(szInfo, g_Message[ETC_MESSAGE620].szMessage, pPacket->szName); 
		DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
	}
	else if(pPacket->dwResult==1)
	{
		// ���� //	
		
		if(pPacket->byType==1)
		{
			// �۽��� ����
			if(pPacket->byIndex != 255 && g_pMainPlayer->m_pInv_Small[pPacket->byIndex].GetID()!=__ITEM_PARTY_PORTAL1__)
			{
				// �������� ���� ��� //
				// "��Ƽ ��Ż �������� �����ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE624].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); 

				CTDS_PARTY_PORTALFAIL pSendPacket;
				pSendPacket.dwPartalUserIndex	= pPacket->dwPartalUserIndex;
				pSendPacket.dwUserIndex			= pPacket->dwUserIndex;
				pSendPacket.byResult			= 1;				
				g_pNet->SendMsg((char*)&pSendPacket, pSendPacket.GetPacketSize(), SERVER_INDEX_ZONE );
			}
			else
			{
				CTDS_ITEM_PICKUP ItemPickup;
				ItemPickup.bSectionNum	= 1;
				ItemPickup.i64ItemID	= 0;
				ItemPickup.dwMoney		= pPacket->dwPartalUserIndex;				
				SetItemPacket(&ItemPickup, 56, pPacket->byIndex, 0, 0, 0);
				ItemPickup.bZipCode2	= pPacket->byType;
				g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );
				return;
			}
		}
		else
		{
			// ��Ƽ ���� ����
			CTDS_ITEM_PICKUP ItemPickup;
			ItemPickup.bSectionNum	= 1;
			ItemPickup.i64ItemID	= 0;
			ItemPickup.dwMoney		= pPacket->dwPartalUserIndex;			
			SetItemPacket(&ItemPickup, 56, pPacket->byIndex, 0, 0, 0);
			ItemPickup.bZipCode2	= pPacket->byType;
			g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );
			return;
		}		
	}
}


//==============================================================//
// ��Ƽ ��Ż ��û�� �޾Ƽ� ó��.								//
//==============================================================//
void CmdPartyPotal(char* pMsg, DWORD dwLen)
{
	WSTC_PARTY_PORTAL* pPacket = (WSTC_PARTY_PORTAL*)pMsg;

	CPartyPortalWnd* pPartyPortalWnd = CPartyPortalWnd::GetInstance();
	
	if(pPartyPortalWnd->GetActive() == FALSE)
	{
		if(pPacket->dwPartalUserIndex == g_pMainPlayer->m_dwUserIndex)
		{
			if(g_pMainPlayer->m_dwPartyId == pPacket->dwPartyId)
			{
				LP_PARTY_USER pPartyUserNode = g_pPartyUserHash->GetData( pPacket->dwUserIndex );
		
				if(pPartyUserNode)
				{
					if(GetGameStatus()==UPDATE_GAME_PLAY)
					{				
						pPartyPortalWnd->m_byIndex	= pPacket->byIndex;
						pPartyPortalWnd->m_byType	= pPacket->byType;
						pPartyPortalWnd->m_dwUserIndex	= pPacket->dwUserIndex;
						memset(pPartyPortalWnd->m_szName, 0, sizeof(pPartyPortalWnd->m_szName));
						__lstrcpyn(pPartyPortalWnd->m_szName, pPacket->szName, MAX_CHARACTER_NAME_REAL_LENGTH);

						pPartyPortalWnd->SetActive();
						pPartyPortalWnd->m_bActiveChk = TRUE;
					}				
				}				
			}
		}
	}	
}


//==============================================================//
// ��Ƽ ��Ż ���� �Ǵ� �ź�.									//
//==============================================================//
void CmdServerPartyPortalFail( char* pMsg, DWORD dwLen )
{
	WSTC_PARTY_PORTALFAIL* pPacket = (WSTC_PARTY_PORTALFAIL*)pMsg;

	char szInfo[0xff] = {0,};

	if(pPacket->byResult==1)
	{
		// ������� ��� ���� ��� //
		// "%s ���� ��Ƽ��Ż�� �ź��ϼ̽��ϴ�."
		wsprintf(szInfo, g_Message[ETC_MESSAGE620].szMessage, pPacket->szName); 
		DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
	}
	else if(pPacket->byResult==2)
	{
		// ���� ����, ���� ���� ���� ��� //
		// "���� ����, ���� �������� ��Ż�� ����Ҽ� �����ϴ�."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE621].szMessage); 
		DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
	}
	else if(pPacket->byResult==3)
	{
		// �������� ���� ��� //
		// "%s ���� �������� �����ϴ�."
		wsprintf(szInfo, g_Message[ETC_MESSAGE622].szMessage, pPacket->szName); 
		DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
	}
	else if(pPacket->byResult==4)
	{
		// �������� ���� ��� //
		// "���������� ��Ż�� ����Ҽ� �����ϴ�."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE623].szMessage); 
		DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
	}
}


//==============================================================//
// ��Ƽ�� HP ����.												//
//==============================================================//
void CmdPartyHP( char* pMsg, DWORD dwLen )
{
	WSTC_PARTY_HP*	pPacket		= (WSTC_PARTY_HP*)pMsg;
	CGroupWnd*		pGroupWnd	= CGroupWnd::GetInstance();

	if(g_pMainPlayer->m_dwPartyId!=0)
	{
		LP_PARTY_USER lpPartyNode = g_pPartyUserHash->GetData( pPacket->dwUserIndex );

		if(lpPartyNode)
		{
			lpPartyNode->wHp	= pPacket->wHp;
			lpPartyNode->wMaxHp	= pPacket->wMaxHp;
		}		
		pGroupWnd->SetPartyUI();
	}
}


//==============================================================//
// ��Ƽ ���� ����.												//
//==============================================================//
void CmdPartyLeader( char* pMsg, DWORD dwLen )
{
	WSTC_PARTY_LEADER*	pPacket		= (WSTC_PARTY_LEADER*)pMsg;
	CGroupWnd*			pGroupWnd	= CGroupWnd::GetInstance();

	if(g_pMainPlayer->m_dwPartyId!=0)
	{
		if(g_pMainPlayer->m_dwUserIndex == pPacket->dwUserIndex)
		{
			g_pMainPlayer->m_bPartyLeader = 1;

			// "��Ƽ�� ������ �Ǿ����ϴ�."
			DisplayMessageAdd(g_Message[ETC_MESSAGE625].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); 		
		}
		else
		{
			LP_PARTY_USER lpPartyNode = g_pPartyUserHash->GetData( pPacket->dwUserIndex );
			
			if(lpPartyNode)
			{
				lpPartyNode->byLeader = 1;
				
				char szInfo[0xff] = {0,};
				// "%s ���� ��Ƽ�� ������ �Ǿ����ϴ�."
				wsprintf(szInfo, g_Message[ETC_MESSAGE626].szMessage, pPacket->szCharName); 
				DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
			}
		}
				
		if(pGroupWnd->GetActive()==TRUE)
		{
			pGroupWnd->SetActive();
		}
	}
}


//==============================================================//
// ��Ƽ�� ���� ����.											//
//==============================================================//
void CmdPartyInfo( char* pMsg, DWORD dwLen )
{
	WSTC_PARTY_INFO*	pPacket		= (WSTC_PARTY_INFO*)pMsg;
	CGroupWnd*			pGroupWnd	= CGroupWnd::GetInstance();
	
	if(pGroupWnd->GetActive()==TRUE)
	{
		memset(pGroupWnd->m_szName, 0, sizeof(pGroupWnd->m_szName));
		__lstrcpyn(pGroupWnd->m_szName, pPacket->szCharName, MAX_CHARACTER_NAME_REAL_LENGTH);
		pGroupWnd->m_wDungeonID		= pPacket->wDungeonID;
		pGroupWnd->m_byLayerIndex		= pPacket->byLayerIndex;
		pGroupWnd->m_bInfo				= TRUE;
		pGroupWnd->m_dwPrevTime			= timeGetTime();
		pGroupWnd->m_dwCurTime			= timeGetTime();
	}
}


//==============================================================//
// ��Ƽ�� ������ ����.											//
//==============================================================//
void CmdPartyLevel( char* pMsg, DWORD dwLen )
{
	WSTC_PARTY_LEVEL* pPacket = (WSTC_PARTY_LEVEL*)pMsg;

	if(pPacket->dwPartyId==g_pMainPlayer->m_dwPartyId)
	{
		LP_PARTY_USER sPartyNode = g_pPartyUserHash->GetData( pPacket->dwUserIndex );

		if(sPartyNode)
		{
			sPartyNode->wLevel	= (WORD)pPacket->dwLevel;
			return;
		}
	}
}


//==============================================================//
// ��Ƽ�� �α� �ƿ� ó��.										//
//==============================================================//
void CmdPartyLogout( char* pMsg, DWORD dwLen )
{
	char				szInfo[0xff]	= {0,};
	WSTC_PARTY_LOGOUT*	pPacket			= (WSTC_PARTY_LOGOUT*)pMsg;
	CMiniMapWnd*		pMiniMapWnd		= CMiniMapWnd::GetInstance();
	CGroupWnd*			pGroupWnd		= CGroupWnd::GetInstance();		

	if(pPacket->bResultCode==0)
	{
		LP_PARTY_USER lpPartyNode = g_pPartyUserHash->GetData( pPacket->dwUserIndex );
		
		if(lpPartyNode)
		{			
			pMiniMapWnd->SetPartyPtr(lpPartyNode->dwResourceId);
			
			if(pMiniMapWnd->GetActive()==TRUE)
				pMiniMapWnd->SetRender(lpPartyNode->dwResourceId, FALSE);

			memset(lpPartyNode->szName, 0, sizeof(lpPartyNode->szName));	
			lpPartyNode->wClass			= 0;
			lpPartyNode->dwUserIndex	= 0;
			lpPartyNode->dwResourceId	= 0;
			lpPartyNode->byLeader		= 0;
			lpPartyNode->wLevel			= 0;
			g_pPartyUserHash->Delete(lpPartyNode, pPacket->dwUserIndex, TRUE);
		}		
		pGroupWnd->m_nPartyUserIndex = -1;
		pGroupWnd->SetPartyUI();		

		if(pGroupWnd->GetActive()==TRUE)
			pGroupWnd->SetActive();

		// MSG_ID : 116 ; %s ���� ��Ƽ���� �����̽��ϴ�. 
		wsprintf(szInfo, g_Message[ETC_MESSAGE116].szMessage, pPacket->szName);	
	}
	else		
	{
		// ��Ƽ�� ���� //
		g_pPartyUserHash->Destroy(TRUE);

		g_pMainPlayer->m_dwPartyId = 0;

		pGroupWnd->m_nPartyUserIndex = -1;
		pGroupWnd->SetPartyUI();		
					
		if(pGroupWnd->GetActive()==TRUE)
			pGroupWnd->SetActive();

		for(int i = 0; i < MAX_PARTY; i++)
			pMiniMapWnd->m_bPartyPtr[i] = 0;

		if(GetGameStatus()==UPDATE_GAME_PLAY)
		{
			pMiniMapWnd->SetRender(SPR_OBJ_PARTY_RED, FALSE);
			pMiniMapWnd->SetRender(SPR_OBJ_PARTY_BLUE, FALSE);
			pMiniMapWnd->SetRender(SPR_OBJ_PARTY_WHITE, FALSE);
			pMiniMapWnd->SetRender(SPR_OBJ_PARTY_GREEN, FALSE);
			pMiniMapWnd->SetRender(SPR_OBJ_PARTY_BORA, FALSE);
			pMiniMapWnd->SetRender(SPR_OBJ_PARTY_YELLOW, FALSE);
			pMiniMapWnd->SetRender(SPR_OBJ_PARTY_ORANGE, FALSE);
			pMiniMapWnd->SetRender(SPR_OBJ_PARTY_PINK, FALSE);
			
		
		}		

		// MSG_ID : 117 ; ��Ƽ�� ����Ǿ����ϴ�.
		lstrcpy(szInfo, g_Message[ETC_MESSAGE117].szMessage);	
	}

	// �޼��� ��� //
	DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
}


//==============================================================//
// ��Ƽ ���� �Ǵ� ����.											//
//==============================================================//
void CmdPartyJoinId( char* pMsg, DWORD dwLen )
{
	char				szInfo[0xff]	= {0,};
	WSTC_PARTY_JOINID*	pPacket			= (WSTC_PARTY_JOINID*)pMsg;	
	CMiniMapWnd*		pMiniMapWnd		= CMiniMapWnd::GetInstance();
	CGroupWnd*			pGroupWnd		= CGroupWnd::GetInstance();

	if(g_pMainPlayer->m_dwUserIndex!=pPacket->dwUserIndex)	
	{
		BYTE	byIndex		= pMiniMapWnd->GetPartyPtr();
		DWORD	dwResource	= 0;
		
		if(byIndex<MAX_PARTY)
		{
			dwResource = pMiniMapWnd->GetPartyResource(byIndex);
			pMiniMapWnd->SetPartyPtr(byIndex, 1);
		}
		
		LP_PARTY_USER lpPartyUser = (LP_PARTY_USER)LALAlloc(g_pPartyPool);
		memset(lpPartyUser, 0, sizeof(lpPartyUser));
		lpPartyUser->wClass			= pPacket->wClass;
		lpPartyUser->dwUserIndex	= pPacket->dwUserIndex;
		lpPartyUser->dwResourceId	= dwResource;
		lpPartyUser->byLeader		= pPacket->byLeader;
		__lstrcpyn(lpPartyUser->szName, pPacket->szName, MAX_CHARACTER_NAME_REAL_LENGTH);				
		g_pPartyUserHash->Insert(lpPartyUser, lpPartyUser->dwUserIndex);
				
		if(g_pMainPlayer->m_dwPartyId==0)
			g_pMainPlayer->m_dwPartyId = pPacket->dwPartyId;

		if(pPacket->byLeader==1)
			g_pMainPlayer->m_bPartyLeader = 0;
		
		if(pGroupWnd->GetActive()==TRUE)
			pGroupWnd->SetActive();

		// ��Ƽ�� ��ġ ǥ�� //
		CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);

		if(pUser)
		{
			VECTOR2 vec = pMiniMapWnd->GetRatioMinimap(pUser->m_v3CurPos.x, pUser->m_v3CurPos.z);
			pMiniMapWnd->SetPosObjX(dwResource, vec.x+8);
			pMiniMapWnd->SetPosObjY(dwResource, vec.y+42);
			pMiniMapWnd->SetRender(dwResource, TRUE);
		}

		pGroupWnd->SetPartyUI();		
	}

	if(pPacket->bResultCode==0)
	{
		// MSG_ID : 118 ; %s �԰� ��Ƽ�� �ξ����ϴ�.	
		wsprintf(szInfo, g_Message[ETC_MESSAGE118].szMessage, pPacket->szName);	
	}
	else if(pPacket->bResultCode==1)		
	{
		// MSG_ID : 119 ; %s ���� ��Ƽ ��û�� �޾Ƶ��̼̽��ϴ�.
		wsprintf(szInfo, g_Message[ETC_MESSAGE119].szMessage, pPacket->szName);	
	}
	else if(pPacket->bResultCode==2)
	{
		// MSG_ID : 120 ; %s ���� ��Ƽ�� ���� �ϼ̽��ϴ�.
		wsprintf(szInfo, g_Message[ETC_MESSAGE120].szMessage, pPacket->szName);	
	}
	
	if(pPacket->bResultCode>=0 && pPacket->bResultCode<=2)
	{
		DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
	}

}


//==============================================================//
// ��Ƽ ��û �޼���.											//
//==============================================================//
void CmdPartyJoin( char* pMsg, DWORD dwLen )
{
	WSTC_PARTY_JOIN*	pPacket			= (WSTC_PARTY_JOIN*)pMsg;
	CPartyJoinWnd*		pPartyJoinWnd	= CPartyJoinWnd::GetInstance();
	char				szInfo[0xff]	= {0,};

	if(pPacket->bResult==0)
	{
		// �����϶�� �޼��� //
		if(pPartyJoinWnd->GetActive()==FALSE)
		{	
			memset(pPartyJoinWnd->m_szName, 0, sizeof(pPartyJoinWnd->m_szName));
			__lstrcpyn(pPartyJoinWnd->m_szName, pPacket->szName, MAX_CHARACTER_NAME_REAL_LENGTH);
			pPartyJoinWnd->m_dwUserIndex = pPacket->dwUserIndex;
			pPartyJoinWnd->SetActive(TRUE);												
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_REQUEST, g_v3InterfaceSoundPos, FALSE);
			
			// MSG_ID : 121 ; %s ���� ��Ƽ�� ��û�ϼ̽��ϴ�.		
			wsprintf(szInfo, g_Message[ETC_MESSAGE121].szMessage, pPacket->szName);	
			DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
		}
		else
		{
			// �̹� ���� �޼����� ���� ��� //
			CTWS_PARTY_JOIN pPacketSend;
			pPacketSend.bErrCode	= 1;
			pPacketSend.dwUserIndex	= pPacket->dwUserIndex;
			__lstrcpyn(pPacketSend.szName, pPacket->szName, MAX_CHARACTER_NAME_REAL_LENGTH);
			g_pNet->SendMsg((char*)&pPacketSend, pPacketSend.GetPacketSize(), SERVER_INDEX_WORLD);
		}
	}
	else
	{
		if(pPacket->bResult==1)
		{
			// MSG_ID : 122 ; %s ���� �ٸ� ��Ƽ�� ���ԵǾ��ֽ��ϴ�.
			wsprintf(szInfo, g_Message[ETC_MESSAGE122].szMessage, pPacket->szName);	
		}
		else if(pPacket->bResult==2)
		{
			// MSG_ID : 123 ; ���̻� ��Ƽ �ο��� �߰� �Ҽ��� �����ϴ�.
			lstrcpy(szInfo, g_Message[ETC_MESSAGE123].szMessage);					
		}
		else if(pPacket->bResult==3)
		{
			// MSG_ID : 124 ; %s ���� �̹�  ��Ƽ ��û�� �����̽��ϴ�.
			wsprintf(szInfo, g_Message[ETC_MESSAGE124].szMessage, pPacket->szName);	
		}
		else if(pPacket->bResult==4)
		{
			// MSG_ID : 125 ; %s ���� ��Ƽ ��û�� �����ϼ̽��ϴ�.
			wsprintf(szInfo, g_Message[ETC_MESSAGE125].szMessage, pPacket->szName);	
		}
		else if(pPacket->bResult==5)
		{
			// MSG_ID : 455 ; %s ���� ��Ƽ�ź� �����Դϴ�.
			wsprintf(szInfo, g_Message[ETC_MESSAGE455].szMessage, pPacket->szName);	
		}
		else if(pPacket->bResult==6)
		{
			// MSG_ID : 456 ; %s ���� �α׾ƿ� �����Դϴ�.
			wsprintf(szInfo, g_Message[ETC_MESSAGE456].szMessage, pPacket->szName);	
		}
		else if(pPacket->bResult==7)
		{
			// "��Ƽ�� �����ϴ�."
			lstrcpy(szInfo, g_Message[ETC_MESSAGE627].szMessage);	
		}

		DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
	}
}


//==============================================================//
// ��Ƽ ���� ����.												//
//==============================================================//
void CmdParty( char* pMsg, DWORD dwLen )
{
	// ��Ƽ�� �����ض�.
	DSTC_PARTY*	pPacket = (DSTC_PARTY*)pMsg;
	CUser*		pUser	= g_pUserHash->GetData(pPacket->dwUserIndex);
	
	if(pUser)
		pUser->m_dwPartyId = pPacket->dwPartyID;			
}


//==============================================================//
// ��Ƽ �Խ��� ��� ����, ���� �޼���.							//
//==============================================================//
void CmdPartyEntry( char* pMsg, DWORD dwLen )
{
	WSTC_PARTY_ENTRY* pPacket = (WSTC_PARTY_ENTRY*)pMsg;

	if(pPacket->byType==0)
	{
		CPartyEntryWnd* pPartyEntryWnd	= CPartyEntryWnd::GetInstance();
		pPartyEntryWnd->m_byEntryType	= PARTYENTRY_RESULT;
		pPartyEntryWnd->SetActive();
	}
	else if(pPacket->byType==1)
		DisplayMessageAdd(g_Message[ETC_MESSAGE1141].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); // "��Ƽ�� �������� ����Ҽ� �ֽ��ϴ�."
	else if(pPacket->byType==2)
		DisplayMessageAdd(g_Message[ETC_MESSAGE1142].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); // "��Ƽ �Խ��ǿ� ����� �� �����ϴ�."
	else if(pPacket->byType==3)
		DisplayMessageAdd(g_Message[ETC_MESSAGE1143].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); // "�����ϰ� �ִ� ��Ƽ�� �����ϴ�."
	else if(pPacket->byType==4)
		DisplayMessageAdd(g_Message[ETC_MESSAGE1144].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); // "��ϵ� ��Ƽ�� �����ϴ�."
	else if(pPacket->byType==5)
		DisplayMessageAdd(g_Message[ETC_MESSAGE1145].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); // "��Ƽ �Խ��ǿ� ��� �Ǿ��ֽ��ϴ�."
	else if(pPacket->byType==6)
		DisplayMessageAdd(g_Message[ETC_MESSAGE1146].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); // "��Ƽ �Խ��ǿ� ����� �����Ǿ����ϴ�."
}


//==============================================================//
// ��Ƽ �Խ��ǿ� ��� ó��.										//
//==============================================================//
void CmdPartyInfoRequest(char* pMsg, DWORD dwLen)
{
	WSTC_PARTY_INFOREQUEST* pPacket				= (WSTC_PARTY_INFOREQUEST*)pMsg;
	CGroupWnd*				pGroupWnd			= CGroupWnd::GetInstance();
	CPartyBoardInfoWnd*		pPartyBoardInfoWnd	= CPartyBoardInfoWnd::GetInstance();	
	LP_PARTY_BOARD			lpPartyBoard		= (LP_PARTY_BOARD)LALAlloc(g_pPartyBoardPool);

	if(lpPartyBoard)
	{		
		memset(lpPartyBoard->szMemo, 0, sizeof(lpPartyBoard->szMemo));
		memset(lpPartyBoard->szName, 0, sizeof(lpPartyBoard->szName));
		__lstrcpyn(lpPartyBoard->szMemo, pPacket->szMemo, 32);
		__lstrcpyn(lpPartyBoard->szName, pPacket->szCharName, MAX_CHARACTER_NAME_REAL_LENGTH);
		lpPartyBoard->wDungeonID	= pPacket->wDungeonID;
		lpPartyBoard->dwPartyId		= pPacket->dwPartyId;
		lpPartyBoard->dwUserIndex	= pPacket->dwLeaderUserIndex;
		g_pPartyBoardHash->Insert(lpPartyBoard, lpPartyBoard->dwPartyId);
		
		if(pGroupWnd->GetActive())
			pGroupWnd->SetActive();
	}
	
	if(pPacket->byType==1)	
		pPartyBoardInfoWnd->SetActive();
}


//==============================================================//
// ��Ƽ �Խ��ǿ��� ����.										//
//==============================================================//
void CmdPartyBoard(char* pMsg, DWORD dwLen)
{
	DSTC_PARTY_BOARD*	pPacket			= (DSTC_PARTY_BOARD*)pMsg;
	CGroupWnd*			pGroupWnd		= CGroupWnd::GetInstance();
	LP_PARTY_BOARD		lpPartyBoard	= g_pPartyBoardHash->GetData(pPacket->dwPartyId);

	if(lpPartyBoard)
	{
		g_pPartyBoardHash->Delete(lpPartyBoard, lpPartyBoard->dwPartyId);
		memset(lpPartyBoard, 0, sizeof(SPARTY_BOARD));
		lpPartyBoard = NULL;
	}

	if(pGroupWnd->GetActive())
		pGroupWnd->SetActive();
}


//==============================================================//
// ��Ƽ �޼��� ���/�ź� ����.									//
//==============================================================//
void CmdPartySet(char* pMsg, DWORD dwLen)
{
	WSTC_PARTY_SET* pPacket			= (WSTC_PARTY_SET*)pMsg;
	char			szInfo[0xff]	= {0,};
	
	if(pPacket->bParty==TRUE)
		lstrcpy(szInfo, g_Message[ETC_MESSAGE670].szMessage); // "��Ƽ��� ���·� ����Ǿ����ϴ�."
	else
		lstrcpy(szInfo, g_Message[ETC_MESSAGE671].szMessage); // "��Ƽ�ź� ���·� ����Ǿ����ϴ�."
	
	DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);

	CUserInterface::GetInstance()->m_bParty = pPacket->bParty;
}

//==============================================================//
// ��Ƽ ���� : �ִ���,����â  2005.1.24									//
//==============================================================//
void CmdPartyConfig(char *pMsg, DWORD dwLen)
{
	DSTC_PARTY_CONFIG* pPacket = (DSTC_PARTY_CONFIG*)pMsg;

	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();

	pGroupWnd->m_nItemRouting  = pPacket->pConfig.item; 
	pGroupWnd->m_nExpDistrib   = pPacket->pConfig.exp;
	
	if(pPacket->bMsg==1)
	{
		DisplayMessageAdd(g_Message[ETC_MESSAGE1311].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); // "����ġ �� ������ �й� ����� �ٲ�����ϴ�."
	}



}


//==============================================================//
// ��Ƽ ��Ī : �ִ��� 2005.1.24									//
//==============================================================//
// ����� ��� ���� ����
void CmdPMatchRegisterResult(char* pMsg, DWORD dwLen)
{
	WSTC_PMATCH_REGISTER_RESULT* pPacket	= (WSTC_PMATCH_REGISTER_RESULT*)pMsg;

	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();

	pGroupWnd->m_nPMatchResult = pPacket->bResult;

}

// ����� Ż�� ���� ����
void CmdPMatchEscapeResult(char* pMsg, DWORD dwLen)
{
	WSTC_PMATCH_ESCAPE_RESULT* pPacket		= (WSTC_PMATCH_ESCAPE_RESULT*)pMsg;

	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();

	pGroupWnd->m_nPMatchEscapeResult = pPacket->bResult;

}

// ��Ī �Ƿ�
void CmdPMatchRequest(char* pMsg, DWORD dwLen)
{
	WSTC_PMATCH_REQUEST* pPacket			= (WSTC_PMATCH_REQUEST*)pMsg;

	CPartyMatchingWnd* pPartMatchingWnd = CPartyMatchingWnd::GetInstance();
	PARTY_MATCHING_DATA PartyMatchData;
	ZeroMemory(PartyMatchData.szName, sizeof(PartyMatchData.szName));
	PartyMatchData.byKind = pPacket->bMaster;             
	PartyMatchData.byClass = pPacket->bClass;
	PartyMatchData.nLevel =  pPacket->bLevel;
	lstrcpy(PartyMatchData.szName, pPacket->szName);
	pPartMatchingWnd->SetPartyMatchingWndData(PartyMatchData);
	pPartMatchingWnd->SetActive(TRUE); 

	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();

	pGroupWnd->m_bMasterMatchStatus = pPacket->bMaster;


}
// �ֱ� ��Ƽ�� ���
void CmdPartyFriend(char* pMsg, DWORD dwLen)
{
	WSTC_PARTY_FRIEND* pPacket			= (WSTC_PARTY_FRIEND*)pMsg;

	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();

	pGroupWnd->m_dwPartyCount = pPacket->dwCount;
	ZeroMemory(&pGroupWnd->m_nPartyFriend, sizeof(pGroupWnd->m_nPartyFriend));
	memcpy(&pGroupWnd->m_nPartyFriend, &pPacket->pFriend, sizeof(PARTY_FRIEND)*pPacket->dwCount);
}

// ���� ���� ���� ����
void CmdPMatchChangeResult(char* pMsg, DWORD dwLen)
{
	WSTC_PMATCH_CHANGE_RESULT* pPacket				= (WSTC_PMATCH_CHANGE_RESULT*)pMsg;
} 

//==============================================================//
// End.															//
//==============================================================//