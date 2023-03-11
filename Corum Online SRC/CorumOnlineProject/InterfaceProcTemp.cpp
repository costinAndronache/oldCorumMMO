#include "GameControl.h"
#include "User.h"
#include "Chat.h"
#include "InitGame.h"
#include "message.h"
#include "NetworkClient.h"
#include "UserInterface.h"
#include "Interface.h"
#include "DSMiniMap.h"			//	BeltProcForInterface()
#include "CDungeonInfoWnd.h"	//	BeltProcForInterface()
#include "ItemNative.h"			//	BeltProcForInterface()
#include "DUNGEON_DATA_EX.h"	//	BeltProcForInterface()
#include "DungeonTable.h"		//	BeltProcForInterface()
#include "PlayerShopWnd.h"		//	PlayerShopProcForInterface()
#include "ItemTradeShopWnd.h"	//	PlayerShopProcForInterface()
#include "ItemWnd.h"			//	PlayerShopProcForInterface()
#include "DungeonProcess.h"		//	RestProcForInterface()
#include "QuantityWnd.h"		//	ChattingProcForInterface()
#include "ChatWnd.h"			//	ChattingProcForInterface()
#include "InputManager.h"		//	ChattingProcForInterface()
#include "ChatBackground.h"		//	ChattingProcForInterface()

int CUserInterface::BeltProcForInterface()
{
	if ( g_Mouse.bLDown )
	{
		if ( g_pMainPlayer->m_bMatching )
		{
			// "�����߿� ��Ʈ �������� ������� ���մϴ�."
			DisplayMessageAdd(g_Message[ETC_MESSAGE520].szMessage, 0xFFFF0000 );
			return 0;
		}

		// ��Ʈ���� ������ ����� ��� //
		int nPosX	= (g_Mouse.MousePos.x-727)/37;
		int nValue	= g_pMainPlayer->m_pBelt[nPosX].m_wItemID/ITEM_DISTRIBUTE;
		
		if ( nValue == 0 )
		{
			SetPointer( __MOUSE_POINTER_DEFAULTCLICK__ );
			return 0;
		}

		CTDS_ITEM_PICKUP		ItemPickup;
		ItemPickup.bInv			= 16;
		ItemPickup.bSectionNum	= 1;
		ItemPickup.i64ItemID	= 0;
		ItemPickup.bZipCode		= (BYTE)nPosX;
		ItemPickup.bEquipCode	= 0;
		g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
		
		if ( g_ItemMoveManager.GetNewItemMoveMode() )
		{
			if ( g_ItemMoveManager.GetNativeSrc() == 0xff )
			{
				WORD wID = g_pMainPlayer->m_pBelt[nPosX].m_wItemID;
				g_ItemMoveManager.SetLButtonDownItem( ITEM_NATIVE_BELT, BYTE(nPosX), wID, 0 );
			}
		}
	}
	else if ( g_Mouse.bRDown && !m_bSmall )
	{
		if(	g_pMainPlayer->GetStatus() != UNIT_STATUS_DEAD &&
			g_pMainPlayer->GetStatus() != UNIT_STATUS_CASTING )
		{
			if(g_pMainPlayer->m_bMatching)
			{
				// "��� �߿��� ����� �� �����ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE501].szMessage, 0xFFFF2CFF);
				return 0;
			}

			CTDS_ITEM_USED pPacket;
			pPacket.bInv	= 2;
			
			int nPosX	= ( g_Mouse.MousePos.x - 727 ) / 37;
			int nValue	= g_pMainPlayer->m_pBelt[nPosX].m_wItemID / ITEM_DISTRIBUTE;

			if(nValue == 50)
			{
				BOOL	bChk			= TRUE;
				WORD	wSumWeight		= g_pMainPlayer->GetSumWeight();
				WORD	wAllSumWeight	= g_pMainPlayer->GetAllSumWeight();
				float	fPerWeight		= ((float)wSumWeight/(float)wAllSumWeight)*100;

				if(fPerWeight >= WEIGTH_100PER_OVER)
				{
					time_t now;
				
					time(&now);
					localtime(&now);

					int nTime = (int)difftime(now, g_pMainPlayer->m_PotionTime);																		

					if( !( nTime >= fPerWeight * 0.03 ) )
					{		
						bChk = FALSE;
						//"��ȭ�����ѵ��� �����Ͽ� �޸��Ⱑ �ȵǸ�, ������ ��� �����̰� �����մϴ�."
						DisplayMessageAdd(g_Message[ETC_MESSAGE986].szMessage, 0xffff0000);								
					}					
				}						

				if(bChk)
				{
					// ���� ����ϱ� //
					pPacket.bZipCode = BYTE(nPosX);
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);

					time(&g_pMainPlayer->m_PotionTime);
					localtime(&g_pMainPlayer->m_PotionTime);
				}
			}
			else if(nValue==ITEM_ZODIAC_INDEX)
			{					
				// ����� ī�� ����ϱ� //
				// ���� ��ȯ ��ũ�� �߰� 2005.02.14 �迵�� 
				WORD wItemID = g_pMainPlayer->m_pBelt[nPosX].GetID();
				if(wItemID >= __ITEM_PORTAL_VILL1__ && wItemID <= __ITEM_PORTAL_VILL6__)
				{
					CMiniMapWnd* pMiniMapWnd = CMiniMapWnd::GetInstance();

					if(	(pMiniMapWnd->m_wMapId!=g_pMainPlayer->m_pBelt[nPosX].m_Item_Zodiac.wMapId) ||
						(pMiniMapWnd->m_byMapLayer!=g_pMainPlayer->m_pBelt[nPosX].m_Item_Zodiac.bLayer))
					{
						CTDS_ITEM_PICKUP pPacket;
						pPacket.bSectionNum	= 1;
						pPacket.i64ItemID	= 0;
						SetItemPacket(&pPacket, 53, BYTE(nPosX), 0, 0, 0);
						g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);
					}
					else 
					{
						// "���� ���������� �̵��Ҽ��� �����ϴ�."
						DisplayMessageAdd(g_Message[ETC_MESSAGE503].szMessage, 0xFFFF2CFF); 
					}
				}
				else if(g_pMainPlayer->m_pBelt[nPosX].GetID()==__ITEM_PORTAL_INDEX__)
				{ 
					if(g_pMainPlayer->m_pBelt[nPosX].m_Item_Zodiac.bType==0)
					{
						if(g_pMainPlayer->m_pBelt[nPosX].m_wItemID==__ITEM_PORTAL_INDEX__)
						{		
							if(g_pMainPlayer->m_pBelt[nPosX].m_Item_Zodiac.bType==0)
							{

// ����� ī�� ���� ���� 
// ����� �ֵ��̸� ��Ʈ �� �÷� ���� ����ϸ� ��ǥ ���� �Ǵ� ���� ����
// 2005.01.31 �迵�� 
								int nZodiacMapId = g_pMainPlayer->m_pBelt[nPosX].m_Item_Zodiac.wMapId;

#if IS_TAIWAN_LOCALIZING()	
								if (nZodiacMapId > 10)
#else
								if (nZodiacMapId >=2000 && nZodiacMapId < 4000)
#endif
								{
									if(g_pMainPlayer->m_pBelt[nPosX].m_Item_Zodiac.bLayer !=0)
									{
										//"����� �������δ� ���� �̵��� �Ұ����մϴ�."
										DisplayMessageAdd(g_Message[ETC_MESSAGE988].szMessage, 0xffff0000);
									}
									else
									{
										CTDS_ITEM_PICKUP pPacket;
										pPacket.bSectionNum	= 1;
										pPacket.i64ItemID	= 0;
										SetItemPacket(&pPacket, 55, BYTE(nPosX), 0, 0, 0);
										g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);							
									}
								}
								else
								{
									//"����� �������δ� ���� �̵��� �Ұ����մϴ�."
									DisplayMessageAdd(g_Message[ETC_MESSAGE988].szMessage, 0xffff0000);
								}
							}
							else
							{
								// "�̰��� �����ī�忡 ��ġ�� ����ų �� �����ϴ�."
								DisplayMessageAdd(g_Message[ETC_MESSAGE987].szMessage, 0xffff0000);
							}							
						}
						else
						{
							// "�������� �����ϴ�."
							DisplayMessageAdd(g_Message[ETC_MESSAGE144].szMessage, 0xFF21DBFF); 
						}
					}
					else if(g_pMainPlayer->m_pBelt[nPosX].m_Item_Zodiac.bType==1)
					{
						CMiniMapWnd* pMiniMapWnd = CMiniMapWnd::GetInstance();

						if(g_pMainPlayer->m_pBelt[nPosX].GetID()==__ITEM_PORTAL_INDEX__)
						{
							for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
							{
								if(g_pMainPlayer->m_pInv_Small[i].GetID()==__ITEM_PORTALUSED_INDEX__)
								{
									if( ( pMiniMapWnd->m_wMapId != g_pMainPlayer->m_pBelt[nPosX].m_Item_Zodiac.wMapId )
									 || ( pMiniMapWnd->m_byMapLayer != g_pMainPlayer->m_pBelt[nPosX].m_Item_Zodiac.bLayer ) )
									{
									//	CMiniMapWnd*		pMiniMapWnd		= CMiniMapWnd::GetInstance();
										CDungeonInfoWnd*	pDungeonInfoWnd = CDungeonInfoWnd::GetInstance();

										BOOL bPortalChk = FALSE;														
										BOOL bOnwerChk	= FALSE;

										POSITION_ pos = pDungeonInfoWnd->m_pPageList->GetHeadPosition();

										while(pos)
										{
											DUNGEON_DATA_EX* pDungeon = (DUNGEON_DATA_EX*)g_pDungeonTable->m_pReceivedDungeonList->GetNext(pos);

											if(pDungeon)
											{
												if(pDungeon->IsDungeonOwner(g_pMainPlayer))
													bOnwerChk = TRUE;
											}
										}

										if(!bOnwerChk)
										{
											if(	g_pMainPlayer->m_pBelt[nPosX].m_Item_Zodiac.wMapId >= 2000 &&
												g_pMainPlayer->m_pBelt[nPosX].m_Item_Zodiac.wMapId < 4000)
											{
												if ( g_pMainPlayer->m_pBelt[nPosX].m_Item_Zodiac.bLayer == 0 )
												{
													bPortalChk = TRUE;
												}
												else
												{
													//"����� �������δ� ���� �̵��� �Ұ����մϴ�."
													DisplayMessageAdd(g_Message[ETC_MESSAGE988].szMessage, 0xffff0000);
												}
											}
											else
											{
												// "����� �������δ� ���� �̵��� �Ұ����մϴ�."
												DisplayMessageAdd(g_Message[ETC_MESSAGE988].szMessage, 0xffff0000);
											}
										}
										else
										{
											bPortalChk = TRUE;
										}

										if ( bPortalChk )
										{
											CTDS_ITEM_PICKUP pPacket;
											pPacket.bSectionNum	= 1;
											pPacket.i64ItemID	= 0;
											SetItemPacket(&pPacket, 54, BYTE(nPosX), BYTE(i), 0, 0);
											g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);
										}
										break;
									}
									else
									{
										// "���� ���������� �̵��� �Ҽ��� �����ϴ�."
										DisplayMessageAdd(g_Message[ETC_MESSAGE503].szMessage, 0xFFFF2CFF); 
									}
								}
							}
						}
						else
						{
							// "����� �� �ִ� �������� �����ϴ�."
							DisplayMessageAdd(g_Message[ETC_MESSAGE52].szMessage, 0xFFFF2CFF); 
						}
					}
				}
				else
				{
					// "����� �� �ִ� �������� �����ϴ�."
					DisplayMessageAdd(g_Message[ETC_MESSAGE52].szMessage, 0xFFFF2CFF); 
				}
			}

			m_bSmall = TRUE;
		}
	}
	else if(!g_Mouse.bLDown)
	{
		int nPosX	= (g_Mouse.MousePos.x-727)/37;
		int nVal	= g_pMainPlayer->m_pBelt[nPosX].m_wItemID/ITEM_DISTRIBUTE;

		if(nVal==0)
			SetPointer(__MOUSE_POINTER_DEFAULT__);
		else
			SetPointer(__MOUSE_POINTER_ITEM__);
	}
	
	return 1;
}


int CUserInterface::GuardianProcForInterface()
{
	if ( g_Mouse.bLDown )
	{			
		int nVal = g_pMainPlayer->m_GuardianItem.m_wItemID / ITEM_DISTRIBUTE;

		if(ITEM_SUPPLIES_INDEX == nVal)
		{
			if ( g_ItemMoveManager.GetNewItemMoveMode() )
			{
				WORD wID = g_pMainPlayer->m_GuardianItem.m_wItemID;
				g_ItemMoveManager.SetLButtonDownItem( ITEM_NATIVE_GUARDIAN, 0, wID, 0, 0 );
			}
			else
			{
				if ( g_pMainPlayer->m_MouseItem.GetID() != 0 )		return 0;

				CTDS_ITEM_PICKUP		ItemPickup;
				ItemPickup.bInv			= 41;
				ItemPickup.bSectionNum	= 1;
				ItemPickup.i64ItemID	= 0;
				ItemPickup.bZipCode		= 0;				
				g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );
			}				
		}
		else if(ITEM_CONSUMABLEST_INDEX <= nVal && ITEM_CONSUMABLEEND_INDEX >= nVal)
		{
			if ( g_ItemMoveManager.GetNewItemMoveMode() )
			{
				WORD wID = g_pMainPlayer->m_GuardianItem.m_wItemID;
				g_ItemMoveManager.SetLButtonDownItem( ITEM_NATIVE_GUARDIAN, 0, wID, 0, 0 );
			}
			else
			{
				if ( g_pMainPlayer->m_MouseItem.GetID() != 0 )		return 0;

				CTDS_ITEM_PICKUP		ItemPickup;
				ItemPickup.bInv			= 41;
				ItemPickup.bSectionNum	= 1;
				ItemPickup.i64ItemID	= 0;
				ItemPickup.bZipCode		= 0;				
				g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );
			}				
		}
	}
	else if ( g_Mouse.bRDown && !m_bSmall )
	{
		// ����� ���� ����� �� //
		int nVal = g_pMainPlayer->m_GuardianItem.m_wItemID/ITEM_DISTRIBUTE;

		if ( FALSE == m_bGuardian )				return 0;

		if(ITEM_SUPPLIES_INDEX == nVal)
		{
			CTDS_ITEM_PICKUP		ItemPickup;		
			ItemPickup.bInv			= 44;
			ItemPickup.bSectionNum	= 1;
			ItemPickup.i64ItemID	= 0;
			ItemPickup.bZipCode		= 0;				
			g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );
			g_Mouse.bRDown			= FALSE;
		}
		else if(ITEM_CONSUMABLEST_INDEX <= nVal && ITEM_CONSUMABLEEND_INDEX >= nVal)
		{
			CTDS_ITEM_PICKUP		ItemPickup;		
			ItemPickup.bInv			= 101;
			ItemPickup.bSectionNum	= 1;
			ItemPickup.i64ItemID	= 0;
			ItemPickup.bZipCode		= 0;				
			g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );
			g_Mouse.bRDown			= FALSE;
		}		
	}
	else if ( !g_Mouse.bLDown )
	{
		if ( g_pMainPlayer->m_GuardianItem.m_wItemID != 0 )
			SetPointer(__MOUSE_POINTER_ITEM__);			
	}

	return 1;
}

int CUserInterface::PlayerShopProcForInterface()
{
	CPlayerShopWnd*		pPlayerShopWnd		= CPlayerShopWnd::GetInstance();
	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();
	
	if ( FALSE == g_pThisDungeon->IsVillage() )
	{
		// "���������� ����ϽǼ� �ֽ��ϴ�."
		DisplayMessageAdd( g_Message[ETC_MESSAGE786].szMessage, 0xFFFF0000 ); 
		return 0;
	}

	if ( pItemTradeShopWnd->GetActive() )
	{
		// "��ȯ�߿��� �ٸ� �ൿ�� �Ͻ� �� �����ϴ�."
		DisplayMessageAdd( g_Message[ETC_MESSAGE1006].szMessage, 0xFFFF2CFF ); 
		return 0;
	}

	if ( g_pMainPlayer->m_bMatching )
	{
		// "��� �߿��� ����� �� �����ϴ�."
		DisplayMessageAdd( g_Message[ETC_MESSAGE501].szMessage, 0xFFFF2CFF ); 
		return 0;
	}

	if ( UNIT_STATUS_PLAYER_REST == g_pMainPlayer->GetStatus() )
	{
		// "�ɱ��߿��� ����� �� �����ϴ�."
		DisplayMessageAdd( g_Message[ETC_MESSAGE797].szMessage, 0xFFFF0000 ); 
		return 0;
	}

	if ( UNIT_STATUS_PORTAL_MOVING == g_pMainPlayer->GetStatus() )
	{
		// "��Ż�̵� �߿��� ���������� �Ͻ� �� �����ϴ�."
		DisplayMessageAdd( g_Message[ETC_MESSAGE1044].szMessage, 0xffff2cff ); 
		return 0;
	}

	if(pPlayerShopWnd->GetActive()==FALSE)
	{
		CItemWnd* pItemWnd = CItemWnd::GetInstance();
		
		pPlayerShopWnd->m_pOwnerPlayerShop = g_pMainPlayer;
		pPlayerShopWnd->SetActive();
		
		if(!pItemWnd->GetActive())
			pItemWnd->SetActive();
	}			
	else
	{
		if(!pPlayerShopWnd->GetActive())
		{
			pPlayerShopWnd->SetActive(TRUE);
		}
		else
		{
			if(pPlayerShopWnd->m_byMode==1)
			{
				g_pMainPlayer->SetPlayerShop(0);
			
				CTDS_PLAYERSHOP_CLOSE packet;
				g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);

				pPlayerShopWnd->m_byMode = 0;

				g_byStatusMessenger	= 0;
							
				// ��Ŷ ������ //			
				CTWS_MESSENGER_STATUS pPacket;
				pPacket.byType		= 0;
				pPacket.dwUserIndex	= g_pMainPlayer->m_dwUserIndex;
				g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
			}
			
			pPlayerShopWnd->m_pOwnerPlayerShop	= NULL;
			memset(pPlayerShopWnd->m_sPlayerShopInfo.szTitle, 0,
				sizeof(pPlayerShopWnd->m_sPlayerShopInfo.szTitle));
			pPlayerShopWnd->SetActive(FALSE);
		}
	}

	return 1;
}

int CUserInterface::RestProcForInterface()
{
	if( g_pMainPlayer->m_MouseItem.GetID() != 0 )
	{
		return 0;
	}

	if ( g_pMainPlayer->GetStatus() == UNIT_STATUS_PLAYER_SHOP )
	{
		// "�����߿��� ����� �� �����ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE798].szMessage, 0xFFFF0000); 
		return 0;
	}

	if( g_pMainPlayer->GetStatus() == UNIT_STATUS_PORTAL_MOVING )
	{
		// "��Ż�߿��� ����� �� �����ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE502].szMessage, 0xFFFF0000); 
		return 0;
	}

	CTDS_PLAYER_REST pPacket;

	if( g_pMainPlayer->GetStatus() == UNIT_STATUS_PLAYER_REST )
	{
		// �̹� �����ִ� ���¿�����,
		pPacket.dwUserIndex	= g_pMainPlayer->m_dwUserIndex;
		pPacket.byType		= 0;
		g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);
		return 0;
	}

	if ( g_pMainPlayer->m_bMatching != TRUE						&&
		 g_pMainPlayer->GetStatus() != UNIT_STATUS_SKILL		&&
		 g_pMainPlayer->GetStatus() != UNIT_STATUS_CASTING		&&
		 g_pMainPlayer->GetStatus() != UNIT_STATUS_DEAD			&&
		 g_pMainPlayer->GetStatus() != UNIT_STATUS_PORTAL_MOVING )
	{ 
		// 1:1 ����� �ƴϰ� ��ų ���, ĳ����, ����, ��Ż �̵��� �� �ƴ� ��쿡�� ���� �ִ�.  						
		SendStopPacket();

		pPacket.dwUserIndex	= g_pMainPlayer->m_dwUserIndex;
		pPacket.byType		= 1;
		g_pNet->SendMsg( (char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE );
	}

	return 1;
}

int CUserInterface::MatchProcForInterface()
{
	if ( g_pThisDungeon->IsKaien() )
	{
		// "ī�̿��� �������� ����� �� �� �����ϴ�."
		DisplayMessageAdd( g_Message[ETC_MESSAGE1241].szMessage, 0xFFFF0000 ); 
		return 0;
	}

	if ( g_pMainPlayer->GetStatus() == UNIT_STATUS_PLAYER_SHOP )
	{
		// "�����߿��� ����� �� �����ϴ�."
		DisplayMessageAdd( g_Message[ETC_MESSAGE798].szMessage, 0xFFFF0000 ); 
		return 0;
	}

	if ( g_pMainPlayer->GetStatus() == UNIT_STATUS_PLAYER_REST )
	{
		// "�ɱ��߿��� ����� �� �����ϴ�."
		DisplayMessageAdd( g_Message[ETC_MESSAGE797].szMessage, 0xFFFF0000 ); 
		return 0;
	}
	
	if ( g_pMainPlayer->m_bMatching )
	{
		// "�����߿� ������û�� �Ͻ� �� �����ϴ�."
		DisplayMessageAdd( g_Message[ETC_MESSAGE664].szMessage, 0xFFFF0000 ); 
		return 0;
	}

	SetPointer( __MOUSE_POINTER_GUARDIAN__ );
	SetPointerChk( TRUE );

	m_bMatch = TRUE;	

	return 1;
}

int CUserInterface::ChattingProcForInterface()
{
	CQuantityWnd*	pQuantityWnd	= CQuantityWnd::GetInstance();
	CChatWnd*		pChatWnd		= CChatWnd::GetInstance();

	g_pInputManager->ClearInput( INPUT_BUFFER_19 );
	g_pInputManager->GetInputBuffer( INPUT_BUFFER_6 );
	
	if ( pQuantityWnd->GetActive() )
	{
		return 0;
	}

	pChatWnd->SetActive( !pChatWnd->GetActive() );		// Toggle Chatting Window Active Status
/*		for more simple sentence
	if( pChatWnd->GetActive() == TRUE )
		pChatWnd->SetActive( FALSE );
	else
		pChatWnd->SetActive( TRUE );	//*/

	m_byOrderCheck	= CHAT_WND;
	m_nChatChk		= 1;				
	
	if( g_pGVDungeon->bChatMode )
	{
		ChatModeOff( INPUT_BUFFER_0, 0, TRUE, TRUE );
		return 0;
	}

	ChatModeOn( INPUT_BUFFER_19 );

#ifdef _USE_IME
	POINT pChatPoint;
	pChatPoint.x	= 10;
	pChatPoint.y	= 635;
	GET_IMEEDIT()->ActivateIME( pChatPoint, __MAX_BUUFER_SIZE__ );
	ShowChatEditBackground( TRUE, GET_IMEEDIT()->GetOrder() - 1 );
#endif						
	return 1;
}

int CUserInterface::DungeonInfoProcForInterface()
{
	CDungeonInfoWnd* pDungeonInfoWnd = CDungeonInfoWnd::GetInstance();

	pDungeonInfoWnd->SetActive( !pDungeonInfoWnd->GetActive() );

	CTWS_REQUEST_OWN_DUNGEON_INFO packet;
	g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_WORLD);

	return 1;
}
