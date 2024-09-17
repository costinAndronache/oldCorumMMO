#include "stdafx.h"
#include "Define.h"
#include "InitGame.h"
#include "GameControl.h"
#include "DungeonProcess.h"
#include "CorumResource.h"
#include "DefResource.h"
#include "GameControl.h"
#include "HashTable.h"
#include "EffectCommand.h"
#include "EffectProcess.h"
#include "ObjectManager.h"
#include "Monster.h"
#include "Npc.h"
#include "DungeonTable.h"
#include "WorldProcess.h"
#include "Interface.h"
#include "UserInterface.h"
#include "Script.h"
#include "Filter.h"
#include "ItemWnd.h"
#include "CharWnd.h"
#include "GuardianWnd.h"
#include "GameMenuWnd.h"
#include "SkillWnd.h"
#include "ChatWnd.h"
#include "CommandWnd.h"
#include "Struct.h"
#include "SkillWnd.h"
#include "ItemShopWnd.h"
#include "DSMiniMap.h"
#include "QuantityWnd.h"
#include "ExitWnd.h"
#include "StoreWnd.h"
#include "cdungeonmanagerwnd.h"
#include "cdungeoninfownd.h"
#include "BankWnd.h"
#include "GuildWnd.h"
#include "GuildExWnd.h"
#include "ChatWnd.h"
#include "GuildJoinWnd.h"
#include "GuildRankWnd.h"
#include "GMTool.h"
#include "GuildEditWnd.h"
#include "PartyJoinWnd.h"
#include "ComboBox.h"
#include "NoticeWnd.h"
#include "HelpWnd.h"
#include "ErrorWnd.h"
#include "ItemTradeWnd.h"
#include "ItemTradeShopWnd.h"
#include "message.h"
#include "OkWnd.h"
#include "GroupWnd.h"
#include "npcdungeoninfownd.h"
#include "playershopwnd.h"
#include "PartyPortalWnd.h"
#include "RequestMatchWnd.h"
#include "CodeFun.h"
#include "Party.h"
#include "Trade.h"
#include "Guild.h"
#include "Chat.h"
#include "ItemUpgrade.h"
#include "ItemUpgradeSubWnd.h"
#include "DungeonSiegeInfoWnd.h"
#include "GameNoticeWnd.h"
#include "InitItemWnd.h"
#include "IMEEdit.h"
#include "Messenger.h"
#include "Shop.h"
#include "InputManager.h"
#include "WorldWnd.h"
#include "CharDieWnd.h"
#include "ChatBackground.h"
#include "PathFinding\SearchModule_Astar.h"
#include "NetworkClient.h"
#include "Map.h"
#include "Effect.h"
#include "DamageNumber.h"
#include "ChrInfo.h"
#include "RankWnd.h"
#include "RankNpcWnd.h"
#include "EventNPCWnd.h"
#include "EventNPCSubWnd.h"
#include "special_effect.h"
#include "PartyEntryWnd.h"
#include "PartyBoardWnd.h"
#include "ItemUsed.h"
#include "ItemMakingWnd.h"
#include "EventDungeonProcess.h"
#include "TopDungeonHelpWnd.h"
#include "MinboboWork.h"
#include "ItemNative.h"
#include "BaloonChat.h"				
#include "TokenCommand.h"			
#include "RivalGuildWar.h"		
#include "DungeonProcess2nd.h"	
#include "GuildWarRequest.h"
#include "GuildWarFinalSettingWnd.h"
#include "GuildWarStatusWnd.h"
#include "ItemPickupFiltering/ItemPickupFiltering.h"

DWORD						g_dwMileHandleRefs	= 0;
LPGlobalVariable_Dungeon	g_pGVDungeon		= NULL;
SDGCHATMSG					g_sDgChatMsg[__MAX_QUEUE__];
SMSG						g_sDgChatListMsg[__MAX_CHAT_LIST_QUEUE__];
CMatchInfo					g_MatchInfo;
RECT						g_DgRect[__MAX_QUEUE__];

BOOL						g_bMagicChk		= FALSE;
BOOL						g_bRButton		= FALSE;
BOOL						g_bBeltChk		= FALSE;
BOOL						g_bKeyChkUp		= FALSE;
BOOL						g_bGuildChk		= FALSE;
BOOL						g_bLshift		= FALSE;
BOOL						g_bLCtrl		= FALSE;
BOOL						g_bLAlt			= FALSE;
BOOL						g_bPlayerShop	= FALSE;

int							g_nDgIndex;
int							g_nDgCount;
int							g_nDgChatListIndex;
int							g_nDgChatListStart;
int							g_nDgChatListEnd;
int							g_nDGZoom		= __MAX_ZOOM_DEFAULT__;
int							g_iCameraX		= __DEFAULT_CAMERA_X__;
int							g_nMsgBox[__MAX_ZOOM_OUT__+1] = {293, 279, 266, 256, 245, 237, 229, 222, 215, 209, 204, 199, 194, 190, 186, 182, 175, 171, 167, 163, 160, 157, 155, 153, 151};
int							g_nLinePos[14]	= {20, 25, 30, 35, 40, 45, 50, 55, 62, 70, 76, 80, 84, 88};
BYTE						g_bKeyItem_Large[MAX_INV_LARGE_POOL];
DWORD						g_dwCurrTick = 0;
DWORD						g_dwPrevTick = 0;
COnlyList*					g_pDyingMonsterList = NULL;
#ifdef DEVELOP_MODE
GXOBJECT_HANDLE				g_TileAttr[ MAX_KIND_OF_DEBUG_TILE ][ MAX_DEBUG_TILE_NUM ];
BYTE						g_bShowTileAttr = 0;
#endif

DWORD __stdcall AfterInterpolation(AFTER_INTERPOLATION_CALL_BACK_ARG* pArg)
{
	if (NULL == g_pMainPlayer->m_hPlayer.pHandle)
	{
		return 0;
	}
	
	if (CAMERA_MOVE_OPTION_SCREEN_FRAME == g_Camera.iCameraMoveOption)
	{
		// 0xffffffff 이 핸들 값이면 제대로된 핸들이 아니다. 
		if( 0xffffffff != (unsigned long int)g_pMainPlayer->m_hPlayer.pHandle && NULL != g_pMainPlayer->m_hPlayer.pHandle) 
		{
			VECTOR3 v3Tmp = 
			{
				g_pMainPlayer->m_v3CurPos.x + g_Camera.fCameraDistance_x + 300,
				g_pMainPlayer->m_v3CurPos.y + g_Camera.fCameraDistance_y + 150,
				g_pMainPlayer->m_v3CurPos.z + g_Camera.fCameraDistance_z
			};

			g_pExecutive->GetGeometry()->SetCameraPos( &v3Tmp, 0 );
		}		
	}

	return 0;
}


BOOL InitGameDungeon()
{
	CBankWnd::GetInstance()->Init();
	// 카메라 이동에 관련된 플래그 세팅.
	g_Camera.iCameraMoveOption = CAMERA_MOVE_OPTION_SCREEN_FRAME;

	g_pExecutive->SetAfterInterpolation(AfterInterpolation);
	SetRenderMode(RENDER_MODE_NORMAL);
	
	g_pExecutive->SetFramePerSec(30);
	
	srand( g_dwCurTick );
	
	g_pGVDungeon = new GLOBAL_VARIABLE_DUNGEON;
	memset( g_pGVDungeon, 0, sizeof( GLOBAL_VARIABLE_DUNGEON ) );
	
	g_pDyingMonsterList = new COnlyList(100);

	g_bSmallBankChk = FALSE;

	g_pInputManager->Initialize();
	
	RECT rt = {0,};
	rt.left = 8;	rt.right = 546;	rt.top = 630;	rt.bottom = 642;	
	g_pInputManager->InitializeInput(INPUT_BUFFER_19, FONT_D3D_GAME, rt, 0xffffffff, 101);

	rt.left = 8;	rt.right = 566;	rt.top = 15;	rt.bottom = 70;
	g_pInputManager->InitializeInput(INPUT_BUFFER_1, FONT_D3D_GAME, rt, 0xffffffff, 101);
	
	rt.left = 8;	rt.right = 566;	rt.top = 15;	rt.bottom = 70;
	g_pInputManager->InitializeInput(INPUT_BUFFER_PARTY_ENTRY, FONT_D3D_GAME, rt, 0xff00ffff, 100);

	::SetTimer(g_hMainWnd, 22, 5000, NULL);
	
	InitDungeonMessage();	
	
	for(int i = 0; i < __MAX_CHAT_LIST_QUEUE__; i++)
	{
		g_sDgChatListMsg[i].sdwColor = 0xffffffff;
		memset(g_sDgChatListMsg[i].szMessage, 0, sizeof(g_sDgChatListMsg[i].szMessage));
	}
	int i = 0;
	for(i = 0; i < __MAX_QUEUE__; i++)
	{
		g_DgRect[i].left	= 10;
		g_DgRect[i].right	= 500;
		g_DgRect[i].top		= 600-(i*18);
		g_DgRect[i].bottom	= 615-(i*18);
	}

	g_nDgChatListIndex = 0;

	g_pSprManager->CreateSprite(SPR_MESSAGE_BOX, 60, 100, FALSE, 100);
	SPR(SPR_MESSAGE_BOX)->SetAlpha(128);
	
	CreateChatEditBackground();

	return TRUE;
}

//------------------------------------------------------------------------
// Function		: RenderUserSelectRect
// Date			: 2004. 10. 07. ( Cut and Use by minjin. )
// Description	: 의미를 좀더 명확히 하기 위해 함수로 감쌌습니다
//------------------------------------------------------------------------
void RenderUserSelectRect()
{
	RECT		rcBox;
	VECTOR2		v2Pos1, v2Pos2;
	SetRect( &rcBox, min( g_rcSelectBox.left, g_rcSelectBox.right ),
					min( g_rcSelectBox.top, g_rcSelectBox.bottom ),
					max( g_rcSelectBox.left, g_rcSelectBox.right ),
					max( g_rcSelectBox.top, g_rcSelectBox.bottom ) );
	
	// 가로 그리기.
	v2Pos1.x = (float)rcBox.left;
	v2Pos1.y = (float)rcBox.top;
	v2Pos2.x = (float)rcBox.right;
	v2Pos2.y = (float)rcBox.top;
	g_pExecutive->GetRenderer()->RenderLine( &v2Pos1, &v2Pos2, 0xff00ff00 );

	v2Pos1.x = (float)rcBox.left;
	v2Pos1.y = (float)rcBox.bottom;
	v2Pos2.x = (float)rcBox.right;
	v2Pos2.y = (float)rcBox.bottom;
	g_pExecutive->GetRenderer()->RenderLine( &v2Pos1, &v2Pos2, 0xff00ff00 );

	// 세로 그리기.
	v2Pos1.x = (float)rcBox.left;
	v2Pos1.y = (float)rcBox.top;
	v2Pos2.x = (float)rcBox.left;
	v2Pos2.y = (float)rcBox.bottom;
	g_pExecutive->GetRenderer()->RenderLine( &v2Pos1, &v2Pos2, 0xff00ff00 );

	v2Pos1.x = (float)rcBox.right;
	v2Pos1.y = (float)rcBox.top;
	v2Pos2.x = (float)rcBox.right;
	v2Pos2.y = (float)rcBox.bottom;
	g_pExecutive->GetRenderer()->RenderLine( &v2Pos1, &v2Pos2, 0xff00ff00 );
}

//------------------------------------------------------------------------
// Function		: RenderChatMsgOnBox
// Date			: 2004. 10. 06. ( Cut and Use by minjin. )
// Description	: 의미를 좀더 명확히 하기 위해 함수로 감쌌습니다
//------------------------------------------------------------------------
void RenderChatMsgOnBox( CUser* pUser, int x, int y, int nIndex, DWORD dwColor )
{
	for( int i = 0; i < 3; i++ )
	{			
		int nLeft	= (long)(x - 57);
		int nRight	= (long)(x + 100);
		int nTop	= (long)(y + (i * 14) - g_nMsgBox[g_nDGZoom]) - ((pUser->m_byChatLine-1)*12) + 26-(pUser->m_byChatLine-1)*3;
		int nBottom	= (long)(y + (i * 14) - g_nMsgBox[g_nDGZoom]) - ((pUser->m_byChatLine-1)*12) + 38-(pUser->m_byChatLine-1)*3;

		if( pUser->m_byChatLine == 1 )
		{
			nLeft	= (long)( x + 25 )	- g_nLinePos[pUser->m_sDgChat[i].nSize / 2];
			nRight	= (long)( x + 200 ) - 80;
		}

		if(!IsEmptyString(pUser->m_sDgChat[i].szChat))
			RenderFont(pUser->m_sDgChat[i].szChat, nLeft, nRight, nTop, nBottom, nIndex, dwColor);
	}
}

//------------------------------------------------------------------------
// Function		: RenderRectChatBox
// Date			: 2004. 10. 06. ( Cut and Use by minjin. )
// Description	: 의미를 좀더 명확히 하기 위해 함수로 감쌌습니다
//------------------------------------------------------------------------
void RenderRectChatBox( CUser* pUser, VECTOR2* v2Scale, VECTOR2* v2Position, int nIndex )
{
	// 사실 여기 쓰이는 pUser->m_pBaloonChat.pSpr 도 좀.. 그렇다.. -_-;;
	g_pRenderer->RenderSprite( pUser->m_pBaloonChat.pSpr, v2Scale, 0.0, v2Position, NULL, 0xffffffff, nIndex, 0);
}

//------------------------------------------------------------------------
// Function		: SetEffectIndexAll
// Date			: 2004. 10. 06. ( Cut and Use by minjin. )
// Description	: 객체에 이펙트 넣어주는 루틴이 여러곳에 반복되어 있어서
//				: 한곳으로 옮겨버렸습니당..
//------------------------------------------------------------------------
void SetEffectIndexAll( CUser* pUser, DWORD dwEffectIndex )
{
	if(pUser == NULL)
		return;

	if(dwEffectIndex != 0xffffffff)
	{
		if(g_wEffectLevel < 1)
		{
			dwEffectIndex = 0xffffffff;
		}
		else if(g_wEffectLevel < 3 && pUser->m_dwUserIndex != g_pMainPlayer->m_dwUserIndex)
		{
			dwEffectIndex = 0xffffffff;
		}
	}

	if(pUser->m_hPlayerFlag.pHandle)
		g_pExecutive->GXOSetEffectIndex(pUser->m_hPlayerFlag.pHandle, dwEffectIndex);
	if(pUser->m_hPlayerHead.pHandle)
		g_pExecutive->GXOSetEffectIndex(pUser->m_hPlayerHead.pHandle, dwEffectIndex);
	if(pUser->m_hPlayerHelmet.pHandle)
		g_pExecutive->GXOSetEffectIndex(pUser->m_hPlayerHelmet.pHandle, dwEffectIndex);
	if(pUser->m_hPlayerEmoticon.pHandle)
		g_pExecutive->GXOSetEffectIndex(pUser->m_hPlayerEmoticon.pHandle, dwEffectIndex);						
	if(pUser->m_hPlayerShop.pHandle)
		g_pExecutive->GXOSetEffectIndex(pUser->m_hPlayerShop.pHandle, dwEffectIndex);
	if(pUser->m_hPlayer.pHandle)
		g_pExecutive->GXOSetEffectIndex(pUser->m_hPlayer.pHandle, dwEffectIndex);
	if(pUser->m_hPlayerRHand.pHandle)
		g_pExecutive->GXOSetEffectIndex(pUser->m_hPlayerRHand.pHandle, dwEffectIndex);
	if(pUser->m_hPlayerLHand.pHandle)
		g_pExecutive->GXOSetEffectIndex(pUser->m_hPlayerLHand.pHandle, dwEffectIndex);
}

void UpdateGameDungeon()
{		
	if( g_Mouse.bLDown )
	{
		int a = 0;
	}
	DWORD	dwModelIndex	= 0;
	DWORD	dwObjIndex		= 0;
	BOOL	bChk			= FALSE;

	g_hHandle = g_pExecutive->GXOGetObjectWithScreenCoord(&g_v3Temp, &dwModelIndex, &dwObjIndex, &g_fDist, &g_Mouse.MousePos, 0, PICK_TYPE_PER_COLLISION_MESH);	

	if(g_hHandle)
	{
		if(g_hEffectHandle != g_hHandle)
		{
			if(g_hEffectHandle)
			{
				LPObjectDesc pObjDesc = (LPObjectDesc)g_pExecutive->GetData(g_hEffectHandle);

				if(pObjDesc)				
				{					
					if( pObjDesc->bType == OBJECT_TYPE_PLAYER || pObjDesc->bType == OBJECT_TYPE_MAINPLAYER)
					{
						CUser* pUser = (CUser*)pObjDesc->pInfo;

						if(pUser)
						{	
							if ( pUser->m_byUpgrade == 0 )
							{
								SetEffectIndexAll( pUser, 0xffffffff );
							}
							else
							{
								bChk = TRUE;
							}
						}
					}
				}

				// sung-han 2005-01-30 특정 몬스터에게 좔좔 효과를 준다.
				BOOL bChkTempEffect = TRUE;
				if( pObjDesc )
				{
					if( pObjDesc->bType == OBJECT_TYPE_MONSTER )
					{
						CMonster* pMonster = (CMonster*)pObjDesc->pInfo;
						if(pMonster)
						{
							if( pMonster->m_dwMonsterKind == OBJECT_TYPE_MONSTER && pMonster->m_bClass == 20 )
							{
								bChkTempEffect = FALSE;
								g_pExecutive->GXOSetEffectIndex(g_hHandle, EFS_upgrade_effect_1);
							}

							// 아메리타트 구현 : 최덕석 2005.2.14
							// 아메리타트 이면
							if( pMonster->m_wModNo == 630)
							{
								// 번쩍이지 않음
								bChkTempEffect = FALSE;
							}
						}
					}
				}
				if(bChkTempEffect)
					g_pExecutive->GXOSetEffectIndex(g_hEffectHandle, 0xffffffff);
				g_hEffectHandle = NULL;				
			}

			LPObjectDesc pObjDesc = (LPObjectDesc)g_pExecutive->GetData( g_hHandle );

			if(pObjDesc)
			{
				if( pObjDesc->bType == OBJECT_TYPE_PLAYER || pObjDesc->bType == OBJECT_TYPE_MAINPLAYER )
				{
					CUser* pUser = (CUser*)pObjDesc->pInfo;

					if(pUser)
					{
						if ( pUser->m_byUpgrade == 0 )
						{
							SetEffectIndexAll( pUser, EFS_picking_effect );
						}
						else
						{
							bChk = TRUE;
						}
					}
				}								
			}			

			if(!bChk)
			{
				// sung-han 2005-01-30 특정 몬스터에게 좔좔 효과를 준다.
				BOOL bChkTempEffect = TRUE;
				if( pObjDesc )
				{
					if( pObjDesc->bType == OBJECT_TYPE_MONSTER )
					{
						CMonster* pMonster = (CMonster*)pObjDesc->pInfo;
						if(pMonster)
						{
							if( pMonster->m_dwMonsterKind == OBJECT_TYPE_MONSTER && pMonster->m_bClass == 20 )
							{
								bChkTempEffect = FALSE;
								g_pExecutive->GXOSetEffectIndex(g_hHandle, EFS_upgrade_effect_1);
							}

							// 아메리타트 구현 : 최덕석 2005.2.14
							// 아메리타트 이면
							if( pMonster->m_wModNo == 630)
							{
								// 번쩍이지 않음
								bChkTempEffect = FALSE;
							}
						}
					}
				}
				if(bChkTempEffect)
					g_pExecutive->GXOSetEffectIndex(g_hHandle, EFS_picking_effect);
				g_hEffectHandle = g_hHandle;
			}			
		}
	}
	else
	{
		if(g_hEffectHandle)
		{
			LPObjectDesc pObjDesc = (LPObjectDesc)g_pExecutive->GetData(g_hEffectHandle);

			if(pObjDesc)
			{
				if( pObjDesc->bType == OBJECT_TYPE_PLAYER || pObjDesc->bType == OBJECT_TYPE_MAINPLAYER)
				{
					CUser* pUser = (CUser*)pObjDesc->pInfo;

					if(pUser)
					{
						if ( pUser->m_byUpgrade == 0 )
						{
							SetEffectIndexAll( pUser, 0xffffffff );
						}
						else
						{
							bChk = TRUE;						
						}
					}
				}
			}	

			// sung-han 2005-01-30 특정 몬스터에게 좔좔 효과를 준다.
			BOOL bChkTempEffect = TRUE;
			if( pObjDesc )
			{
				if( pObjDesc->bType == OBJECT_TYPE_MONSTER )
				{
					CMonster* pMonster = (CMonster*)pObjDesc->pInfo;
					if(pMonster)
					{
						if( pMonster->m_dwMonsterKind == OBJECT_TYPE_MONSTER && pMonster->m_bClass == 20 )
						{
							bChkTempEffect = FALSE;
							g_pExecutive->GXOSetEffectIndex(g_hEffectHandle, EFS_upgrade_effect_1);
						}

						// 아메리타트 구현 : 최덕석 2005.2.14
						// 아메리타트 이면
						if( pMonster->m_wModNo == 630)
						{
							// 번쩍이지 않음
							bChkTempEffect = FALSE;
						}
					}
				}
			}
			if(bChkTempEffect)
				g_pExecutive->GXOSetEffectIndex(g_hEffectHandle, 0xffffffff);
			g_hEffectHandle = NULL;
		}
	}

	SearchHack();
	CheckTick();
	MouseEventDungeon();
	
	// 몬스터 잔상을 위한것..
	POSITION_ pos = g_pDyingMonsterList->GetHeadPosition();
	while(pos)
	{
		POSITION_ del = pos;
		DWORD dwMonsterIndex = (DWORD)g_pDyingMonsterList->GetNext(pos);
		CMonster* pMonster = g_pMonsterHash->GetData(dwMonsterIndex);

		if (!pMonster) continue;

		if (pMonster->m_dwTemp[MONSTER_TEMP_DYINGFRAME] < pMonster->m_dwTemp[MONSTER_TEMP_FINISHDYINGFRAME])
		{
			pMonster->m_dwTemp[MONSTER_TEMP_DYINGFRAME]++;
		}
		else
		{
			// 강제 죽이기
			if (!pMonster->m_dwTemp[MONSTER_TEMP_FRAMECOUNT])
			{
				pMonster->SetAction( MON_MOTION_TYPE_DOWN, 0, ACTION_ONCE );
				pMonster->m_hMonster.pDesc->ObjectFunc = MonsterKillFunc;
				pMonster->WithActionFunc = MonsterKillWithAction;
				pMonster->m_pDyingList = NULL;
				pMonster->m_dwTemp[MONSTER_TEMP_PUSHPOWER] = 0;
				g_pDyingMonsterList->RemoveAt(del);
			}					
		}
	}	
	
	if(g_byStatusMessenger == 0)
	{
		// 온라인에서 자리비움으로 //
		DWORD dwStatusTime	= timeGetTime();

		if(dwStatusTime - g_dwStatusTime > 60000)
		{
			g_dwStatusTime		= 1;
			g_byStatusMessenger	= 1;
						
			CTWS_MESSENGER_STATUS pPacket;
			pPacket.byType		= 1;
			pPacket.dwUserIndex	= g_pMainPlayer->m_dwUserIndex;
			g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
		}
	}
	else if(g_byStatusMessenger == 1)
	{
		// 온라인에서 자리비움으로 //
		if(g_byChkMessenger == 1)
		{
			g_byChkMessenger	= 0;
			g_dwStatusTime		= 0;
			g_byStatusMessenger	= 0;

			CTWS_MESSENGER_STATUS pPacket;
			pPacket.byType		= 0;
			pPacket.dwUserIndex	= g_pMainPlayer->m_dwUserIndex;
			g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
		}
	}

	if(g_pThisDungeon && g_pThisDungeon->IsStadium() )
	{
		DWORD	dwAlpha = 0xff;
		BOOL	bMin;
		if( g_pMainPlayer->m_dwGuildWarFlag == G_W_F_OBSERVER )
		{
			dwAlpha = 0x80;
			bMin = TRUE;

			
		}
		else
		{
			dwAlpha = 0xff;
			bMin = FALSE;
		}

		if(g_pMainPlayer->m_hPlayer.pHandle)
			MinimizePlayer( g_pMainPlayer->m_hPlayer.pHandle, bMin );
		if(g_pMainPlayer->m_hPlayer.pHandle)
			g_pExecutive->SetAlphaFlag( g_pMainPlayer->m_hPlayer.pHandle, dwAlpha );
		if(g_pMainPlayer->m_hPlayerHead.pHandle)
			g_pExecutive->SetAlphaFlag( g_pMainPlayer->m_hPlayerHead.pHandle, dwAlpha );
		if(g_pMainPlayer->m_hPlayerRHand.pHandle)
			g_pExecutive->SetAlphaFlag( g_pMainPlayer->m_hPlayerRHand.pHandle, dwAlpha );
		if(g_pMainPlayer->m_hPlayerLHand.pHandle)
			g_pExecutive->SetAlphaFlag( g_pMainPlayer->m_hPlayerLHand.pHandle, dwAlpha );
		if(g_pMainPlayer->m_hPlayerHelmet.pHandle)
			g_pExecutive->SetAlphaFlag( g_pMainPlayer->m_hPlayerHelmet.pHandle, dwAlpha );
	}
}

void ReleaseGameDungeon()
{
	if (!g_pGVDungeon->bMainUserPotalJoin)
		g_pNet->DisconnectServer(SERVER_INDEX_ZONE);

	TerminateDungeonCamera(); 
	
	g_MatchInfo.Release();

	g_pExecutive->SetAfterInterpolation(NULL);

	g_pGeometry->DisableFog(0);

	KillTimer(g_hMainWnd, 1);
	KillTimer(g_hMainWnd, 22);

	g_pThisDungeon	= NULL;
	g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;

	ReleaseDungeonEffect();
	ReleaseMainPlayerResource();
	ReleaseMainPlayerByChangeLayer();		
	RemoveAllPlayer();
	RemoveAllMonster();
	RemoveAllItem();
	RemoveAllEffectDesc();	
	ClearEventDungeonObjects();
	g_pExecutive->UnloadAllPreLoadedGXObject(UNLOAD_PRELOADED_RESOURCE_TYPE_ALL_PRELOADED);		
	CInterface* pInterface = CInterface::GetInstance();
	pInterface->Remove();
	CMiniMapWnd* pMiniMapWnd = CMiniMapWnd::GetInstance();
	pMiniMapWnd->ReleaseMap();	
	Release_Sound_Resource();

	if(g_pMainPlayer->m_pBaloonChat.pSpr)		
	{
		g_pMainPlayer->m_pBaloonChat.pSpr->Release();
		g_pMainPlayer->m_pBaloonChat.pSpr = NULL;
	}

	RELEASE_SPR(SPR_MESSAGE_BOX);

#ifdef _USE_IME
	ReleaseChatEditBackground();
#endif

	g_pExecutive->DeleteAllGXMapObjectsWitLoadMapScript();
	g_pExecutive->DeleteGXMap( NULL );
	if (g_pDyingMonsterList)
	{
		g_pDyingMonsterList->RemoveAll();
		delete g_pDyingMonsterList;
		g_pDyingMonsterList = NULL;
	}
	if( g_pGVDungeon ) 
	{
		delete g_pGVDungeon;
		g_pGVDungeon = NULL;
	}	

	if ( g_pMap )
	{
		delete g_pMap;
		g_pMap = NULL;
	}

	g_pUserHash->Delete( g_pMainPlayer, g_pMainPlayer->m_dwUserIndex );


	// 이벤트 던젼 플래그 해제
	g_pMainPlayer->m_bInEventDungeon = FALSE;
	g_pMainPlayer->m_bInEvnetDungeonFirstTimeOut = FALSE;

	IMESetEdit(0, FALSE);
}

void DungeonEnvironmentSetting(DUNGEON_DATA_EX* pDungeon)
{
	bool bEnalbeFog = true;
	if (DUNGEON_TYPE_VILLAGE != pDungeon->GetDungeonType())
	{
		// 해의 탑일경우
		if (pDungeon->m_dwID == 3057 || pDungeon->m_dwID == 3056)
		{
			g_FogDesc.fStart	= 800.0f;
			g_FogDesc.fEnd		= 5000.0f;
			g_FogDesc.dwColor	= 0xff394973;
			g_FogDesc.fDensity	= 1.0f;
		}// sung-han 2005-02-02 이아프등대일 경우 포그값을 다르게 한다.-----------------------------
		else if(pDungeon->m_dwID == 3015 || pDungeon->m_dwID == 3016 || pDungeon->m_dwID == 3017)
		{
			g_FogDesc.fStart	= 2500.0f;
			g_FogDesc.fEnd		= 5000.0f;
			g_FogDesc.dwColor	= 0xffb4e4e6;
			g_FogDesc.fDensity	= 1.0f;
		}
		else
		{
			//g_FogDesc.fStart	= 1800.0f;
			//g_FogDesc.fEnd		= 3500.0f;
			g_FogDesc.fStart	= 2500.0f;
			g_FogDesc.fEnd		= 5000.0f;
			g_FogDesc.dwColor	= 0xff000000;
			g_FogDesc.fDensity	= 1.0f;
		}
	}
	else
	{
//		if (pDungeon->m_dwID == 3)// sung-han 2005-02-25 이다마을
//		{
//			g_FogDesc.fStart	= 1800.0f;
//			g_FogDesc.fEnd		= 5000.0f;
//			g_FogDesc.dwColor	= 0xfffff8bb;
//			g_FogDesc.fDensity	= 1.0f;
//		}
//		else
		{
			g_FogDesc.fStart	= 1800.0f;
			g_FogDesc.fEnd		= 5000.0f;
			g_FogDesc.dwColor	= 0xffB4BEE6;
			g_FogDesc.fDensity	= 1.0f;
		}

		if(pDungeon->m_dwID == 1 || pDungeon->m_dwID == 3)
		{
			bEnalbeFog = false;
		}
	}

	g_pGeometry->SetFog(&g_FogDesc,0);
	if(bEnalbeFog)
	{
		g_pGeometry->EnableFog(0);
	}
	else
	{
		g_pGeometry->DisableFog(0);
	}
}

#ifdef DEVELOP_MODE
void RenderTileAttr()
{
	Section_Link_Info* pGrid = &g_pMap->m_Section[ g_pMainPlayer->m_wCurSectionNum-1 ];

	DWORD dwCount = 0;	VECTOR3 vPos;	MAP_TILE* pTile=0;
	for(float j=(pGrid->y1*125.0f); j<(pGrid->y2*125.0f); j=j+125.0f)
	{
		for(float i=(pGrid->x1*125.0f); i<(pGrid->x2*125.0f); i=i+125.0f)
		{
			pTile = g_pMap->GetTile(i, j);
			
			vPos.x = pTile->x + 62.5;
			vPos.y = 0.0f;
			vPos.z = pTile->z + 62.5;

			switch(pTile->wAttr.uOccupied)
			{
			case TILE_OCCUPIED_PC:
			case TILE_OCCUPIED_MONSTER:
				{
					HideObject(g_TileAttr[0][dwCount] );
					HideObject(g_TileAttr[1][dwCount] );
					ShowObject(g_TileAttr[2][dwCount] );

					GXSetPosition( g_TileAttr[2][dwCount], &vPos, FALSE);
				}
				break;

			case TILE_OCCUPIED_DONT_MOVE:
				{
					HideObject(g_TileAttr[0][dwCount] );
					HideObject(g_TileAttr[2][dwCount] );
					ShowObject(g_TileAttr[1][dwCount] );

					GXSetPosition( g_TileAttr[1][dwCount], &vPos, FALSE);
				}
				break;

			default:
				{
					HideObject(g_TileAttr[2][dwCount] );
					HideObject(g_TileAttr[1][dwCount] );
					ShowObject(g_TileAttr[0][dwCount] );

					GXSetPosition( g_TileAttr[0][dwCount], &vPos, FALSE);
				}
				break;

			}
				
			dwCount++;
			if(dwCount == MAX_DEBUG_TILE_NUM)	return;
		}
	}

	for(DWORD dwi=dwCount; dwi<MAX_DEBUG_TILE_NUM; dwi++)
	{
		HideObject(g_TileAttr[0][dwi] );
		HideObject(g_TileAttr[1][dwi] );
		HideObject(g_TileAttr[2][dwi] );
	}
}
#endif


DWORD __stdcall BeforeRenderGameDungeon()
{
	// 속도를 위해 지역변수로 접근하도록 수정한다. by deepdark.
	CUserInterface* pUserInterface = CUserInterface::GetInstance();
	assert(pUserInterface);

	// 메인 캐릭터 위치에 맞게 카메라 위치를 조정.
	if(pUserInterface->m_bMoveChr==TRUE && pUserInterface->m_byMoveType==6)
		GXSetPosition(pUserInterface->m_pUserMouseHandle[2].pHandle, &g_pMainPlayer->m_v3CurPos, FALSE);		

	if(pUserInterface->m_pUser)
	{
		if(pUserInterface->m_pUser->m_dwPartyId==g_pMainPlayer->m_dwPartyId)
		{
			if(pUserInterface->m_bMoveChr==TRUE && pUserInterface->m_byMoveType==4)
				GXSetPosition(pUserInterface->m_pUserMouseHandle[1].pHandle, &pUserInterface->m_pUser->m_v3CurPos, FALSE);
		}
		else
		{
			if(pUserInterface->m_bMoveChr==TRUE || pUserInterface->m_byMoveType==5)
				GXSetPosition(pUserInterface->m_pUserMouseHandle[2].pHandle, &pUserInterface->m_pUser->m_v3CurPos, FALSE);
		}
	}	

	if(pUserInterface->m_pMonster)
	{
		if(pUserInterface->m_pMonster->m_dwLordIndex==0)
		{
			if(pUserInterface->m_bMoveChr==TRUE && pUserInterface->m_byMoveType==1)
				GXSetPosition(pUserInterface->m_pUserMouseHandle[0].pHandle, &pUserInterface->m_pMonster->m_v3CurPos, FALSE);
		}
		else if(g_pMainPlayer->m_dwUserIndex==pUserInterface->m_pMonster->m_dwLordIndex)
		{
			if(pUserInterface->m_bMoveChr==TRUE && pUserInterface->m_byMoveType==2)
				GXSetPosition(pUserInterface->m_pUserMouseHandle[1].pHandle, &pUserInterface->m_pMonster->m_v3CurPos, FALSE);
		}
		else
		{
			if(pUserInterface->m_bMoveChr==TRUE &&  pUserInterface->m_byMoveType==3)
				GXSetPosition(pUserInterface->m_pUserMouseHandle[2].pHandle, &pUserInterface->m_pMonster->m_v3CurPos, FALSE);
		}
	}

	// 메치창 관련 
	CRequestMatchWnd* pRequestMatchWnd = CRequestMatchWnd::GetInstance();
	assert(pRequestMatchWnd);

	if (pRequestMatchWnd->m_bActive == TRUE)
	{ 
		// 메인 유저가 대전 신청을 받았다.
		if (pRequestMatchWnd->m_dwRequestMatchEndTick <= g_dwCurTick && 
			pRequestMatchWnd->m_dwRequestMatchEndTick)
		{
			// 자동 거절 시간이 됐다.
			// 대전 거절  
			pRequestMatchWnd->m_dwRequestMatchEndTick = 0;
			pRequestMatchWnd->SetActive(FALSE);	

			CTDS_REPLY_MATCH_MAN2MAN	packet;
			packet.bReplyCode = 2;
			packet.dwRequestUserIndex = pRequestMatchWnd->m_dwRequestMatchUserIndex ;

			g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
		}
	}
	
	if (g_dwCurTick-g_pMainPlayer->m_dwTemp[USER_TEMP_100MILLI] >= 100) 
	{
		// 쿨타임포인터 올리기.
		g_pMainPlayer->m_fCurCoolPoint = 
			(float)min(g_pMainPlayer->m_fMaxCoolPoint, g_pMainPlayer->m_fCurCoolPoint+g_pMainPlayer->m_fMaxCoolPoint/g_pMainPlayer->GetODC()/30*0.1);

		if (g_pMainPlayer->IsSkilling())
		{
			Effect* pEffect = g_pEffectLayer->GetEffectInfo(g_pMainPlayer->m_bSkillTmp);
			BYTE bSkillLevel = g_pMainPlayer->GetSkillLevel(pEffect->bID);
			if (bSkillLevel)
			{
				if (pEffect->bType == TYPE_DRIVE) // 마우스 누르고 있는 만큼만 마나 빼라.
					pUserInterface->SetDengeonMp((WORD)max(0, g_pMainPlayer->m_wMP - g_pMainPlayer->m_wMaxMP*0.1*0.1 ));
			}
		}

		if (g_pThisDungeon && g_pThisDungeon->m_bSiege)
		{
			if (g_pMainPlayer->m_bAttackMode != ATTACK_MODE_DEFENSE)
			{
				// 방어자가 아니라면.				
				POSITION_ pos = g_pMap->m_pCPList->GetHeadPosition();

				while(pos)
				{
					CP_DESC* pCPDesc = (CP_DESC*)g_pMap->m_pCPList->GetNext(pos);
					MAP_TILE* pTile = g_pMap->GetTile(g_pMainPlayer->m_v3CurPos.x, g_pMainPlayer->m_v3CurPos.z );
					BOOL bShow = FALSE;
					
					if (g_pMainPlayer->GetStatus() != UNIT_STATUS_DEAD && pCPDesc->pTile == pTile)
					{
						// cp위에 있군.
						if (!pCPDesc->bDestroy)
						{						
							VECTOR3 vecOut;
							GetScreenXYFromXYZ(g_pGeometry, 0, &pCPDesc->pEffectDesc->vecBasePosion, &vecOut);
							pCPDesc->dwStartTime+=100*((g_dwCurTick-g_pMainPlayer->m_dwTemp[USER_TEMP_100MILLI])/100);
							bShow = TRUE;
							pCPDesc->pSpr_Cp_GuageBoard->vPos.x = vecOut.x*__GAME_SIZE_X__;
							pCPDesc->pSpr_Cp_GuageBoard->vPos.y = vecOut.y*__GAME_SIZE_Y__;

							pCPDesc->pSpr_Cp_GuageBar->vPos.x = pCPDesc->pSpr_Cp_GuageBoard->vPos.x+1;
							pCPDesc->pSpr_Cp_GuageBar->vPos.y = pCPDesc->pSpr_Cp_GuageBoard->vPos.y+1;
							pCPDesc->pSpr_Cp_GuageBar->SetScaling(63-((float)pCPDesc->dwStartTime/g_pThisDungeon->GetCPTime()*63), 1);
						}									
					}
					else
					{
						pCPDesc->dwStartTime = 0;
					}
				
					pCPDesc->pSpr_Cp_GuageBar->ShowSprite(bShow);
					pCPDesc->pSpr_Cp_GuageBoard->ShowSprite(bShow);
					 
					if (bShow)	break;
				}
			}
		}

		g_pMainPlayer->m_dwTemp[USER_TEMP_100MILLI] = g_dwCurTick;
	}
	
	if (g_dwCurTick-g_pMainPlayer->m_dwTemp[USER_TEMP_1SECTICK] >= 1000  ) 
	{
		// 마나 올리기
		if (g_pMainPlayer->m_wClass == CLASS_TYPE_WARRIOR)
		{
			// 전사만 쓸수 있다.		
			if (g_pMainPlayer->IsSkilling())
			{
				Effect* pEffect = g_pEffectLayer->GetEffectInfo(g_pMainPlayer->m_bSkillTmp);
				BYTE bSkillLevel = g_pMainPlayer->GetSkillLevel(pEffect->bID);
				if (bSkillLevel)
				{
					if (pEffect->bID ==__SKILL_AURARECHARGE__) // 마우스 누르고 있는 만큼만 마나 더해라.
						pUserInterface->SetDengeonMp((WORD)min(g_pMainPlayer->m_wMaxMP, g_pMainPlayer->m_wMP + pEffect->Value[bSkillLevel-1].nMin ));
				}
			}
		}
		
		g_pMainPlayer->m_dwTemp[USER_TEMP_1SECTICK] = g_dwCurTick;
	}

	g_pMainPlayer->SetActionDummy();

	if (g_pMainPlayer->GetStatus()!=UNIT_STATUS_DEAD)
	{			
		if (int(g_pMainPlayer->m_dwTemp[USER_TEMP_5SECTICK]-g_dwCurTick) <= 0)
		{
			// 5초마다?		
			WORD	wSumWeight		= g_pMainPlayer->GetSumWeight();
			WORD	wAllSumWeight	= g_pMainPlayer->GetAllSumWeight();
			float	fPerWeight		= ((float)wSumWeight/(float)wAllSumWeight)*100;

			if(fPerWeight < WEIGTH_80PER_OVER)
			{
				pUserInterface->SetDengeonHp((WORD)min(g_pMainPlayer->m_wMaxHP, g_pMainPlayer->m_wHP+g_pMainPlayer->m_dwHealHPSec));
				
				if (g_pMainPlayer->m_wClass != CLASS_TYPE_WARRIOR) // 전사는 오라리차지로 올려야 한다. ㅡ.ㅡ
					pUserInterface->SetDengeonMp((WORD)min(g_pMainPlayer->m_wMaxMP, g_pMainPlayer->m_wMP+g_pMainPlayer->m_dwHealMPSec));
			}	
			
			g_pMainPlayer->m_dwTemp[USER_TEMP_5SECTICK] = g_dwCurTick+5000;
		}
	}
	
	return 0;
}

extern void renderAllDroppedItemsTooltips(const std::vector<ITEM*>& droppedItemTooltips);

DWORD __stdcall AfterRenderGameDungeon()
{

#ifdef DEVELOP_MODE
	if( g_bShowTileAttr ) 
		RenderTileAttr();
#endif

	g_helpLoadingSprite.SetLoadingFlag(FALSE);
	
	CInterface*				pInterface			= CInterface::GetInstance();		
	CUserInterface*			pUserInterface		= CUserInterface::GetInstance();
	CItemWnd*				pItemWnd			= CItemWnd::GetInstance();
	CSkillWnd*				pSkillWnd			= CSkillWnd::GetInstance();

	DisplayMatchInfoOnDungeon();
	
	pItemWnd->RenderItemMouse();

	if( g_bLshift && g_Mouse.bLDown )
	{
		RenderUserSelectRect();
	}

	pInterface->m_byIndex = 45;
	pInterface->Render();
	

	g_pSprManager->RenderAllSprite();
	pInterface->SetMiniMapPos();
	

	pUserInterface->SetMousePointerPos((float)g_Mouse.MousePos.x, (float)g_Mouse.MousePos.y);
	if( !g_pThisDungeon->IsStadium() || (g_pMainPlayer->m_dwGuildWarFlag != G_W_F_OBSERVER) )
	{
		pItemWnd->RenderBelt(); // 밸트의 수량 
	}
	
	pSkillWnd->RenderUsing();

	ProcessMessgae();
	
	if(g_bNoticeFlag==TRUE)
	{
		if(g_bPosType==UPDATE_GAME_PLAY)
		{
			CheckNoticeTime();
		}
	}

	ItemInfoRender();			// ItemInfo 	
	ChatRender();				// 채팅출력 	
	SkillRender();				// 스킬 출력 
	
	char szInfo[0xff] = {0,};

	if( !g_pThisDungeon->IsStadium() || (g_pMainPlayer->m_dwGuildWarFlag != G_W_F_OBSERVER) )
	{
		// Hp, Sp //	
		wsprintf(szInfo, g_Message[ETC_MESSAGE864].szMessage, g_pMainPlayer->m_wHP, g_pMainPlayer->m_wMaxHP);//"Hp:%d/%d"	
		RenderFont(szInfo, 5, 100, 740, 752, 1);

		wsprintf(szInfo, g_Message[ETC_MESSAGE865].szMessage, g_pMainPlayer->m_wMP, g_pMainPlayer->m_wMaxMP);//"Sp:%d/%d"
		RenderFont(szInfo, 5, 100, 755, 777, 1);	
	}
		
	// INPUT BUFFER 체크
	// 작업중.. by minjin. 2004. 10. 06.
	for(DWORD i = 0; i < MAX_INPUT_BUFFER_NUM; i++)	
	{			
		if(i==INPUT_BUFFER_6)
		{
			if(pInterface->m_pMenu[CHAT_WND]->GetActive()==FALSE)
				continue;
		}
		else if(i==INPUT_BUFFER_5)
		{
			if(pInterface->m_pMenu[QUANTITY_WND]->GetActive()==FALSE)
				continue;
		}
		else if(i==INPUT_BUFFER_7)
		{
			if(pInterface->m_pMenu[GUILDEX_WND]->GetActive()==FALSE)
				continue;
		}
		else if(i==INPUT_BUFFER_2)
		{
			CPlayerShopWnd* pPlayerShopWnd = CPlayerShopWnd::GetInstance();
			assert(pPlayerShopWnd);

			if(pPlayerShopWnd->GetActive()==FALSE)							
				continue;
			if(pPlayerShopWnd->m_pOwnerPlayerShop!=g_pMainPlayer && pPlayerShopWnd->m_pOwnerPlayerShop)
				continue;
		}
		
		g_pInputManager->RenderInputBuffer(i);		
	
	}

	if(!g_pUserHash)
		return 0;

	CUser*				pUser=0;
	VECTOR3				vOutPos;
	VECTOR3				vPlayerHeadPos;
	WORD				x=0, y=0;
	DWORD				dwColor=0;
	BOOL				bChk=0;
	DWORD				dwCurTick = g_dwCurTick;
	ListNode<CUser>*	pUserNode = g_pUserHash->GetHead();
	
	int					nIndex	= 0;		
	int					nSize	= 0;
	int					nSize2	= 0;
	BYTE				byIndex = 1;

	LPObjectDesc		pObjDesc = NULL;
	char*				szTemp=0;
	char*				szPrev=0;
	char				szPlayerShop[2][0xff];	

	DWORD				dwGuildWarId[MAX_GUILDWAR]={0,};
	DWORD				dwGuildWarCnt = 0;

	memset(dwGuildWarId, 0, sizeof(dwGuildWarId));
	
	if(g_hHandle)			
		pObjDesc = (LPObjectDesc)g_pExecutive->GetData(g_hHandle);

	ListNode<SGUILD_WAR>* lpGuildWarPos = g_pGuildWarHash->GetHead();

	while(lpGuildWarPos)
	{
		LP_GUILD_WAR lpGuildWar = lpGuildWarPos->pData;

		if(lpGuildWar)
		{
			dwGuildWarId[dwGuildWarCnt] = lpGuildWar->dwGuildId;
			dwGuildWarCnt++;
		}
		lpGuildWarPos = lpGuildWarPos->pNext;
	}
	
	while( pUserNode )
	{
		pUser = pUserNode->pData;
				
		vPlayerHeadPos		= pUser->m_v3CurPos;
		vPlayerHeadPos.x	= pUser->m_v3CurPos.x;
		vPlayerHeadPos.y	= pUser->m_v3CurPos.y;
		GetScreenXYFromXYZ(g_pGeometry, 0, &vPlayerHeadPos, &vOutPos);

		if(vOutPos.x >= 1.0f || vOutPos.y >= 1.0f )
			goto lb_Again;
		
		x = WORD( float(windowWidth()) * vOutPos.x );
		y = WORD( float(windowHeight()) * vOutPos.y );
		
		if(pUser->m_bPlayerShop)
		{
			BYTE byMouseChk = 0;

			if(g_hHandle)
			{				
				if(pObjDesc)
				{
					if(pObjDesc->bType==OBJECT_TYPE_MAINPLAYER || pObjDesc->bType==OBJECT_TYPE_PLAYER)
					{
						CUser*	pChkUser = (CUser*)pObjDesc->pInfo;			

						if(!pUser)
							return 0;

						if(pChkUser->m_dwUserIndex==pUser->m_dwUserIndex)
							byMouseChk = 1;					
					}
				}
			}

			VECTOR2	vPos;
			vPos.x		= float(x - 50);
			vPos.y		= float(y - g_nMsgBox[g_nDGZoom] + 50);
			g_pRenderer->RenderSprite(pUserInterface->m_pSprPlayerShop[byMouseChk], NULL, 0.0f, &vPos, NULL, 0xffffffff, 6+nIndex, RENDER_TYPE_DISABLE_TEX_FILTERING);
			nIndex++;

			dwColor = 0xffffffff;

			if(!IsEmptyString(pUser->m_szPlayerShopTitle))
			{
				szTemp	= pUser->m_szPlayerShopTitle;	
				nSize	= lstrlen(pUser->m_szPlayerShopTitle);

				memset(szPlayerShop[0], 0, sizeof(szPlayerShop[0]));
				memset(szPlayerShop[1], 0, sizeof(szPlayerShop[1]));

				// Text //
				if(nSize > 16)
				{
					szPrev	= CharPrevExA(0, szTemp, szTemp+16, 0);					
					nSize	= lstrlen(szTemp) - lstrlen(szPrev);
					nSize2	= lstrlen(szPrev);
										
					memcpy(szPlayerShop[0], szTemp, nSize);
					memcpy(szPlayerShop[1], szPrev, nSize2);

					for(int i = 0; i < byMouseChk+1; i++)
					{
						if(byMouseChk==0)
							lstrcat(szPlayerShop[i], "...");

						nSize = lstrlen(szPlayerShop[i]);
						RenderFont(szPlayerShop[i], x - 35, x - 35 + nSize*7, y - g_nMsgBox[g_nDGZoom] + 54 + i*14, y - g_nMsgBox[g_nDGZoom] + 68 + i*14, 6+nIndex);

						nIndex++;
					}
				}
				else
				{
					RenderFont(pUser->m_szPlayerShopTitle, x - 35, x - 35 + nSize*7, y - g_nMsgBox[g_nDGZoom] + 54, y - g_nMsgBox[g_nDGZoom] + 68, 6+nIndex);
					nIndex++;
				}
			}			
		}
		
		byIndex = 1;

		bChk = FALSE;

		// party
		if(g_pMainPlayer->m_dwPartyId!=0 && pUser->m_dwPartyId==g_pMainPlayer->m_dwPartyId
			&& ( g_pThisDungeon == NULL || g_pThisDungeon->IsStadium() == false ) )
		{	
			int nSize = lstrlen(pUser->m_szName);
			
			dwColor	= pUser->m_bBad ? TEXT_COLOR_PURPLE : 0xffffff00;			
			
			if(!IsEmptyString(pUser->m_szName))
			{
				RenderFont(pUser->m_szName, x - 35, x - 35 + nSize*7, y, y + 14, 6+nIndex, dwColor);
				nIndex++;
			}
			
			if(pUser->m_dwGuildId==g_pMainPlayer->m_dwGuildId)
			{
				if(!IsEmptyString(pUser->m_szNickName))
				{
					nSize = lstrlen(pUser->m_szNickName);
					RenderFont(pUser->m_szNickName, x - 35, x - 35 + nSize*7, y-byIndex*16, y-byIndex*16 + 14, 6+nIndex, dwColor);
					byIndex+=1;
					nIndex++;
				}				

				nSize = lstrlen(g_pMainPlayer->m_szGuildName);
				RenderFont(g_pMainPlayer->m_szGuildName, x - 35, x - 35 + nSize*7, y-byIndex*16, y-byIndex*16 + 14, 6+nIndex, dwColor);
				nIndex++;
			}
			bChk = TRUE;
		}

		byIndex = 1;

		if(pUser->m_dwGuildId==g_pMainPlayer->m_dwGuildId && g_pMainPlayer->m_dwGuildId!=0 && bChk==FALSE)
		{								
			dwColor = pUser->m_bBad ? TEXT_COLOR_PURPLE : 0xff00ffff;			

			int nSize = lstrlen(pUser->m_szName);

			if(!IsEmptyString(pUser->m_szName))
			{				
				RenderFont(pUser->m_szName, x - 35, x - 35 + nSize*7, y, y + 14, 6+nIndex, dwColor);
				nIndex++;
			}

			if(!IsEmptyString(pUser->m_szNickName))
			{
				int nSize = lstrlen(pUser->m_szNickName);
				RenderFont(pUser->m_szNickName, x - 35, x - 35 + nSize*7, y-byIndex*16, y-byIndex*16 + 14, 6+nIndex, dwColor);
				byIndex+=1;
				nIndex++;
			}
			
			LP_GUILD_DATA lpGuildInfo = g_pGuildInfoHash->GetData(pUser->m_dwGuildId);

			if(lpGuildInfo)
			{
				nSize = lstrlen(lpGuildInfo->szGuildName);
				RenderFont(lpGuildInfo->szGuildName, x - 35, x - 35 + nSize*7, y-byIndex*16, y-byIndex*16 + 14, 6+nIndex, dwColor);
			}
			else
			{
				if(g_pMainPlayer->m_dwUserIndex==pUser->m_dwUserIndex)
				{
					nSize = lstrlen(g_pMainPlayer->m_szGuildName);
					RenderFont(g_pMainPlayer->m_szGuildName, x - 35, x - 35 + nSize*7, y-byIndex*16, y-byIndex*16 + 14, 6+nIndex, dwColor);					
				}
			}
		}
		int i = 0;
		for(i = 0; i < dwGuildWarCnt; i++)
		{
			if(pUser->m_dwGuildId==dwGuildWarId[i])
			{
				dwColor = pUser->m_bBad ? TEXT_COLOR_PURPLE : 0xffffa2ff;			

				int nSize = lstrlen(pUser->m_szName);

				if(!IsEmptyString(pUser->m_szName))
				{
					RenderFont(pUser->m_szName, x - 35, x - 35 + nSize*7, y, y + 14, 6+nIndex, dwColor);
					nIndex++;
				}

				if(!IsEmptyString(pUser->m_szNickName))
				{
					int nSize = lstrlen(pUser->m_szNickName);
					RenderFont(pUser->m_szNickName, x - 35, x - 35 + nSize*7, y-byIndex*16, y-byIndex*16 + 14, 6+nIndex, dwColor);
					byIndex+=1;
					nIndex++;
				}
				
				LP_GUILD_DATA lpGuildInfo = g_pGuildInfoHash->GetData(pUser->m_dwGuildId);

				if(lpGuildInfo)
				{
					nSize = lstrlen(lpGuildInfo->szGuildName);
					RenderFont(lpGuildInfo->szGuildName, x - 35, x - 35 + nSize*7, y-byIndex*16, y-byIndex*16 + 14, 6+nIndex, dwColor);
				}
				else
				{
					if(g_pMainPlayer->m_dwUserIndex==pUser->m_dwUserIndex)
					{
						nSize = lstrlen(g_pMainPlayer->m_szGuildName);
						RenderFont(g_pMainPlayer->m_szGuildName, x - 35, x - 35 + nSize*7, y-byIndex*16, y-byIndex*16 + 14, 6+nIndex, dwColor);
					}
				}
				break;
			}
		}

		if(dwCurTick - pUser->m_dwChatTick > 3000)
		{
			for(int i = 0; i < 3; i++)
				memset(&pUser->m_sDgChat[i], 0, sizeof(__tag_dg_chat__));

			goto lb_Again;
		}

		if(IsEmptyString(pUser->m_sDgChat[0].szChat))
			goto lb_Again;		

		// 말 풍선 세팅 //
		pUser->m_pBaloonChat.vPos.x	= float(x - 60);
		pUser->m_pBaloonChat.vPos.y	= float(y - g_nMsgBox[g_nDGZoom] - 10 + (30 - (pUser->m_byChatLine-1)*15));


		VECTOR2 v2SclTemp;
		VECTOR2 v2PosTemp;

		v2SclTemp.x = pUser->m_pBaloonChat.v2Scaling.x;
		v2SclTemp.y = pUser->m_pBaloonChat.v2Scaling.y;

		v2PosTemp.x = pUser->m_pBaloonChat.vPos.x;
		v2PosTemp.y = pUser->m_pBaloonChat.vPos.y;

//국내도 일본과 같이 말풍선 적용 (정우창 2005.02.15)
//#if IS_JAPAN_LOCALIZING()

		// add by minjin. 2004. 10. 30.
		// RenderBaloonChatBox 이넘 Interface 안에다가 Baloonchat.cpp 넣었음

		VECTOR2 v2SclTemp_JPN;
		VECTOR2	v2PosTemp_JPN;

		// 말풍선 리소스와, 기존에 쓰던 리소스간에 픽셀 차이값 보정
		v2SclTemp_JPN.x = v2SclTemp.x * 2;
		v2SclTemp_JPN.y = v2SclTemp.y * 2;

		// 일본은 말풍선 밑에랑 꼬리때문에 살짝 위로 올린다..
		v2PosTemp_JPN.x = v2PosTemp.x;
		v2PosTemp_JPN.y = v2PosTemp.y - 20;

		RenderBaloonChatBox( &v2SclTemp_JPN, &v2PosTemp_JPN, 5 + nIndex );
/*		
#else
		RenderRectChatBox( pUser, &v2SclTemp, &v2PosTemp, 5 + nIndex );
#endif
*/

		// 채팅 출력 //

		dwColor = 0xffffffff;											// 색깔부터 정하구
		
		if( pUser->m_dwPartyId == g_pMainPlayer->m_dwPartyId )
		{
			dwColor = 0xffffffff;
		}
		else if( pUser->m_dwGuildId == g_pMainPlayer->m_dwGuildId )
		{
			dwColor = 0xffffffff;
		}

//국내도 일본과 같이 말풍선 적용 (정우창 2005.02.15)
//#if IS_JAPAN_LOCALIZING()
		// 일본은 검정색 글자..		by minjin. 2004. 10. 06.
		dwColor = 0xff111111;

		RenderChatMsgOnBox( pUser, x, y - 20, 6 + nIndex, dwColor );	// 박스 위에 메시지 출력
/*
#else
		RenderChatMsgOnBox( pUser, x, y, 6 + nIndex, dwColor );			// 박스 위에 메시지 출력
#endif
*/
		nIndex += 1;
						
	lb_Again:
		pUserNode = pUserNode->pNext;
	}
	
	if(g_hHandle)
	{
		if(!pObjDesc)
			return 0;

		byIndex = 1;

		if(pObjDesc->bType == OBJECT_TYPE_PLAYER || pObjDesc->bType == OBJECT_TYPE_MAINPLAYER)
		{
			CUser*	pUser = (CUser*)pObjDesc->pInfo;

			if(!pUser)
				return 0;
									
			vPlayerHeadPos		= pUser->m_v3CurPos;
			vPlayerHeadPos.x	= pUser->m_v3CurPos.x;
			vPlayerHeadPos.y	= pUser->m_v3CurPos.y;
			GetScreenXYFromXYZ(g_pGeometry, 0, &vPlayerHeadPos, &vOutPos);			

			x = (WORD)( __GAME_SIZE_X__ * vOutPos.x );
			y = (WORD)( __GAME_SIZE_Y__ * vOutPos.y );

			nSize = lstrlen(pUser->m_szName);			

			DWORD dwColor = pUser->m_bBad ? TEXT_COLOR_PURPLE : 0xffffffff;

			if(!IsEmptyString(pUser->m_szName))
			{			
				if(pUser->m_dwPartyId!=g_pMainPlayer->m_dwPartyId || g_pMainPlayer->m_dwPartyId==0)
				{		
					if (pUser->m_wCriminalTime == CRIMINAL_LEVEL_3)
						dwColor = 0xffFF0000;
					else if (pUser->m_wCriminalTime > CRIMINAL_LEVEL_3 && pUser->m_wCriminalTime <= CRIMINAL_LEVEL_2)
						dwColor = 0xffFF4C4C;
					else if (pUser->m_wCriminalTime > CRIMINAL_LEVEL_2 && pUser->m_wCriminalTime <= CRIMINAL_LEVEL_1)
						dwColor = 0xffFFCCCC;
					else  if (pUser->m_wCriminalTime > CRIMINAL_LEVEL_1 && pUser->m_wCriminalTime < 0)
						dwColor = 0xffFFE5E5;
					
					RenderFont(pUser->m_szName, x-35, x-35+nSize*7, y, y+14, 6+nIndex, dwColor);
					nIndex+=1;
				}
			}
			
			if(pUser->m_dwGuildId!=0)
			{
				LP_GUILD_DATA lpGuildData = g_pGuildInfoHash->GetData(pUser->m_dwGuildId);				

				if(lpGuildData)
				{
					if(!IsEmptyString(pUser->m_szNickName))
					{
						int nSize = lstrlen(pUser->m_szNickName);

						RenderFont(pUser->m_szNickName, x-35, x-35+nSize*7, y-byIndex*16, y-byIndex*16+14, 6+nIndex, dwColor);						
						byIndex+=1;
						nIndex+=1;
					}	
					
					int nSize = lstrlen(lpGuildData->szGuildName);

					RenderFont(lpGuildData->szGuildName, x-35, x-35+nSize*7, y-byIndex*16, y-byIndex*16+14, 6+nIndex, dwColor);
					nIndex+=1;
				}								
			}
		}
	}
	
	 g_Mouse.v3Mouse = GetXYZFromScreenXY( g_pGeometry, g_Mouse.MousePos.x , g_Mouse.MousePos.y , g_rcScreenRect.right, g_rcScreenRect.bottom);

#ifdef DEVELOP_MODE
	// GM 모드일때 우측 상단에 마우스 정보 표시 
	char szTempEx[0xff] = {0,};
	MAP_TILE* pTile = g_pMap->GetTile(g_Mouse.v3Mouse.x, g_Mouse.v3Mouse.z);

	
	sprintf(szTempEx, "%ld, %ld,", g_Mouse.MousePos.x, g_Mouse.MousePos.y);
	RenderFont(szTempEx, 500, 1200, 50, 90, 0);
	if (pTile)
	{
		sprintf(szTempEx, "x:%6.1f, z:%6.1f, Tile_X:%d, Tile_Z:%d, ATTR:%d", g_Mouse.v3Mouse.x, g_Mouse.v3Mouse.z, pTile->wIndex_X, pTile->wIndex_Z, pTile->wAttr.uAttr);
		//RenderFont(szTempEx, 500, 1200, 50, 90, 0);		
	}
#endif	

	renderAllDroppedItemsTooltips(selectedItemsForTooltipRendering);
	ItemPickupFiltering::sharedInstance()->render();

	return 0;
}


void OnKeyDownDungeon(WPARAM wParam, LPARAM lParam)
{	
	if (ItemPickupFiltering::sharedInstance()->handleKeyDown(wParam, lParam)) {
		return;
	}

	BOOL bHanMode = TRUE;
	
	switch( wParam )

	{	
	case 0x52:	  
		{
			if(g_bKeyChkUp==FALSE)
			{
				if(!g_pGVDungeon->bChatMode)
				{
					if( g_pMainPlayer ) 
					{
						WORD	wSumWeight		= g_pMainPlayer->GetSumWeight();
						WORD	wAllSumWeight	= g_pMainPlayer->GetAllSumWeight();
						float	fPerWeight		= ((float)wSumWeight/(float)wAllSumWeight)*100;

						if(fPerWeight >= WEIGTH_100PER_OVER)
						{
							// "포화무게한도에 도달하여 달리기가 안되며, 포션의 사용 딜레이가 증가합니다."
							DisplayMessageAdd(g_Message[ETC_MESSAGE986].szMessage, 0xffff0000);
						}
						else
						{							
							if(g_pMainPlayer->m_bMoveType == UNIT_STATUS_WALKING)
								g_pMainPlayer->m_bMoveType = UNIT_STATUS_RUNNING;
							else
								g_pMainPlayer->m_bMoveType = UNIT_STATUS_WALKING;							
							
							if(g_pMainPlayer->GetStatus()== UNIT_STATUS_WALKING || g_pMainPlayer->GetStatus()==UNIT_STATUS_RUNNING)
							{		
								WORD wAct = MOTION_TYPE_WALK;

								if( g_pMainPlayer->GetStatus()==UNIT_STATUS_WALKING )
									wAct = MOTION_TYPE_RUN;
								else if( g_pMainPlayer->GetStatus()==UNIT_STATUS_RUNNING )
									wAct = MOTION_TYPE_WALK;
								
								g_pMainPlayer->SetAction(wAct, 0, ACTION_LOOP );
								g_pMainPlayer->SetStatus(g_pMainPlayer->m_bMoveType);
								SendMovePacket();
							}
						}
					}
				}

				g_bKeyChkUp = TRUE;
			}

			bHanMode = FALSE;
		}
		break;	
	case 0x60:
	case 0x61:
	case 0x62:
	case 0x63:
	case 0x64:
	case 0x65:
	case 0x66:
	case 0x67:
	case 0x68:
	case 0x69:
		{				
			CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();
			
			if(pQuantityWnd->GetActive())
			{
				if(pQuantityWnd->m_byDrawIndex<=10)
				{										
					// 숫자 키만 사용할수있다 //
					unsigned char ch = (unsigned char)(wParam-0x30);
					memcpy(pQuantityWnd->m_szMoney + pQuantityWnd->m_byDrawIndex, &ch, 1);

					if(pQuantityWnd->m_byDrawIndex<10)							
						pQuantityWnd->m_byDrawIndex++;

					InputQuantity();

					int nSize = lstrlen(pQuantityWnd->m_szMoney);

					if(nSize!=pQuantityWnd->m_byDrawIndex)
					{
						pQuantityWnd->m_byDrawIndex--;
						g_pInputManager->GetInputRect(INPUT_BUFFER_5).left = 
							(LONG)pQuantityWnd->m_fPosX+124-(pQuantityWnd->m_byDrawIndex*6)-7-(pQuantityWnd->m_byDrawIndex/3+1)*6;
						g_pInputManager->GetInputRect(INPUT_BUFFER_5).right = 
							(LONG)pQuantityWnd->m_fPosX+124;						
					}
				}
			}

			bHanMode = FALSE;
			
			// 이모티콘 단축키 처리
			ShortKeyEmoticon((WORD)wParam, 0x60, 0x69);
		}
		break;	
	case 0x41:	// A
	case 0x42:	// B 
	case 0x43:	// C
	case 0x44:	// D
	case 0x45:	// E
	case 0x46:	// F 
	case 0x47:	// G
	case 0x48:	// H
	case 0x49:	// I
	case 0x4A:	// J
	case 0x4B:	
	case 0x4C:
	case 0x4D:
	case 0x4E:
	case 0x4F:
	case 0x50:
	case 0x51:
	case 0x53:
	case 0x54:
	case 0x55:
	case 0x56:
	case 0x57: 
	case 0x58:
	case 0x59:
	case 0x5A:
	case 0x30:
	case 0x31:
	case 0x32:
	case 0x33:
	case 0x34:
	case 0x35:
	case 0x36:
	case 0x37:
	case 0x38:
	case 0x39:
	case 0x70:
	case 0x71:
	case 0x72:
	case 0x73:
	case 0x74:
	case 0x75:
	case 0x76:
	case 0x77:
	case 0x78:		
	case 0x79:
	case 0x7A:
	case 0x7B:
	case 0xBA:
	case 0xDE:
	case 0xDB:
	case 0xDD:
	case 0xBD:
	case 0xBB:
	case 0xDC:
	case 0xBC:
	case 0xBE:
	case 0xBF:
	case 0x20:
	case __ASCII_CODE___KEY_SEE_ALL_DROPPED_ITEMS:
		{	
			if(IsGuildCreate() && wParam==0x20)
				DisplayMessageAdd(g_Message[ETC_MESSAGE1239].szMessage, 0xFFFF0000);
				
			SetKey(wParam);
			bHanMode = FALSE;
		}
		break;
	case VK_RETURN:
		{	
			if( !g_pThisDungeon->IsStadium() || g_pMainPlayer->m_dwGuildWarFlag != G_W_F_OBSERVER )
				ChatMsg();
			return;
		}
		break;
	case VK_CONTROL:
		{
			g_bLCtrl = TRUE;
		}
		break;
	case VK_SHIFT:
		{
			g_bLshift = TRUE;
		}
		break;
	case VK_HOME:
		{
#ifdef DEVELOP_MODE
			VECTOR3		v3Pos;
			g_pExecutive->GXOGetPosition( g_pMainPlayer->m_hPlayer.pHandle, &v3Pos );
			if(g_nDGZoom>-30)
			{
				SetCameraPos( &v3Pos, CAMERA_DISTANCE );
				g_nDGZoom--;
			}
#endif
		}
		break;
	case VK_END:
		{
#ifdef DEVELOP_MODE
			VECTOR3		v3Pos;
			g_pExecutive->GXOGetPosition( g_pMainPlayer->m_hPlayer.pHandle, &v3Pos );
			if(g_nDGZoom<100)
			{
				SetCameraPos( &v3Pos, CAMERA_DISTANCE*-1.f );
				g_nDGZoom++;
			}
#endif
		}
		break;
	case VK_DELETE:
		{
			VECTOR3		v3Pos;
			g_pExecutive->GXOGetPosition( g_pMainPlayer->m_hPlayer.pHandle, &v3Pos );
			SetCameraYaw( &v3Pos, DEG01 * CAMERA_Y );
		}
		break;
	case VK_NEXT:
		{
#ifdef DEVELOP_MODE
			VECTOR3		v3Pos;
			g_pExecutive->GXOGetPosition( g_pMainPlayer->m_hPlayer.pHandle, &v3Pos );
			SetCameraYaw( &v3Pos, DEG01 * CAMERA_Y * 1.0f );
#endif
		}
		break;	
	case VK_PRIOR:
		{
#ifdef DEVELOP_MODE
			VECTOR3		v3Pos;
			g_pExecutive->GXOGetPosition( g_pMainPlayer->m_hPlayer.pHandle, &v3Pos );
			
			SetCameraYaw( &v3Pos, DEG01 * CAMERA_Y * -1.0f );
#endif
		}
		break;
	case VK_INSERT:
		{
#ifdef DEVELOP_MODE
			VECTOR3		v3Pos;
			g_pExecutive->GXOGetPosition( g_pMainPlayer->m_hPlayer.pHandle, &v3Pos );
			
			SetCameraYaw( &v3Pos, DEG01 * CAMERA_Y * -1 );						
#endif
		}
		break;
	case VK_LEFT:
		{
			CChatWnd* pChatWnd = CChatWnd::GetInstance();
			
			if(pChatWnd->GetActive())
			{
				if(pChatWnd->m_byChatType>CHAT_TYPE_ALL)
					pChatWnd->m_byChatType--;				

				pChatWnd->SetActive();
			}			
		}
		break;	
	case VK_RIGHT:
		{
			CChatWnd* pChatWnd = CChatWnd::GetInstance();
			
			if(pChatWnd->GetActive())
			{
				if(pChatWnd->m_byChatType<CHAT_TYPE_FRIEND)
					pChatWnd->m_byChatType++;				

				pChatWnd->SetActive();
			}			
		}
		break;
	case VK_UP:
		{
			CChatWnd* pChatWnd = CChatWnd::GetInstance();
			
			if(pChatWnd->GetActive())
			{	
				pChatWnd->SetActive();
			}
		}
		break;
	case VK_DOWN:
		{
			CChatWnd* pChatWnd = CChatWnd::GetInstance();
			
			if(pChatWnd->GetActive())
			{				
				pChatWnd->SetActive();
			}
		}
		break;
	case VK_TAB:
		{	
			CGuildRankWnd* pGuildRankWnd = CGuildRankWnd::GetInstance();

			if(pGuildRankWnd->GetActive()==FALSE)
			{
//				CTDS_DUNGEON_CALLGARDIAN CallGuardinan;
//				CallGuardinan.bZipCode = 0;
//				g_pNet->SendMsg( (char*)&CallGuardinan, CallGuardinan.GetPacketSize(), SERVER_INDEX_ZONE );
			}						

			CChatWnd* pChatWnd = CChatWnd::GetInstance();
			
			if(g_bKeyChkUp==FALSE)
			{
				if(pChatWnd->GetActive())
				{
					if(pChatWnd->m_byChatMode==CHAT_MODE_CHAT)
						pChatWnd->m_byChatMode = CHAT_MODE_MESSAGE;
					else
						pChatWnd->m_byChatMode = CHAT_MODE_CHAT;

					pChatWnd->SetRender(SPR_OBJ_CHAT_CHAT1, FALSE);
					pChatWnd->SetRender(SPR_OBJ_CHAT_CHAT2, FALSE);
					pChatWnd->SetRender(SPR_OBJ_CHAT_MESSAGE1, FALSE);
					pChatWnd->SetRender(SPR_OBJ_CHAT_MESSAGE2, FALSE);

					pChatWnd->SetActive();
				}
				g_bKeyChkUp = FALSE;
			}			
		}
		break;
		
		//Debug Tile 정보 토글 
		#ifdef DEVELOP_MODE
		case VK_NUMLOCK:
			{
				if(g_bShowTileAttr)
				{
					g_bShowTileAttr = FALSE;
	
					for(int i=0; i<MAX_KIND_OF_DEBUG_TILE; i++)
					{
						for(int j=0; j<MAX_DEBUG_TILE_NUM; j++)
						{
							HideObject(g_TileAttr[i][j]);
						}
					}
	
				}
				else
					g_bShowTileAttr = TRUE;

				bHanMode = FALSE;
			}
			break;
		#endif		
	}

	if(bHanMode==TRUE)
	{	
		if(!g_pGVDungeon->bChatMode)
		{
			char szKey[0xff] = {0,};
			GetKeyNameText(lParam, szKey, 10);

			if(__strcmp(szKey, "R")==0)
			{		
				if(g_bKeyChkUp==FALSE)
				{
					if( g_pMainPlayer ) 
					{
						if( g_pMainPlayer->m_bMoveType == UNIT_STATUS_WALKING )
							g_pMainPlayer->m_bMoveType = UNIT_STATUS_RUNNING;
						else
							g_pMainPlayer->m_bMoveType = UNIT_STATUS_WALKING;
						
						if( g_pMainPlayer->GetStatus()==UNIT_STATUS_WALKING ||
							g_pMainPlayer->GetStatus()==UNIT_STATUS_RUNNING)
						{
							if( g_pMainPlayer->GetStatus()==UNIT_STATUS_WALKING )
							{
								g_pMainPlayer->SetAction( MOTION_TYPE_WALK, 0, ACTION_LOOP );
								g_pMainPlayer->SetStatus(g_pMainPlayer->m_bMoveType);
								SendMovePacket();
							}
							else if( g_pMainPlayer->GetStatus()==UNIT_STATUS_RUNNING)
							{
								g_pMainPlayer->SetAction( MOTION_TYPE_RUN, 0, ACTION_LOOP );								
								SendMovePacket();
							}

							g_pMainPlayer->SetStatus(g_pMainPlayer->m_bMoveType);
						}
					}

					g_bKeyChkUp = TRUE;
				}					
			}

			for(int i = 0; i < 25; i++)
			{
				if(__strcmp(szKey, g_pszKeyArray[i])==0)
				{
					ConvertKeyDown(szKey);
					break;
				}
			}
		}		
	}	
}


void OnKeyUpDungeon(WPARAM wParam, LPARAM lParam)
{	
	if (ItemPickupFiltering::sharedInstance()->handleKeyUp(wParam, lParam)) {
		return;
	}

	g_bKeyChkUp = FALSE;

	switch( LOWORD(wParam) )
	{
		case VK_CONTROL:
		{
			g_bLCtrl = FALSE;
		}

		break;

		case VK_SHIFT:
		{
			g_bLshift = FALSE;
			SetRect( &g_rcSelectBox, 0, 0, 0, 0 );
		}
		break;

		case __ASCII_CODE___KEY_SEE_ALL_DROPPED_ITEMS:
		{
			selectedItemsForTooltipRendering.clear();
		}
		break;
	}
}

BOOL OnLButtonDownInterfaceDungeon()
{
	if (ItemPickupFiltering::sharedInstance()->handleMouseDown()) {
		return TRUE;
	}

	CInterface*			pInterface		= CInterface::GetInstance();

	if( g_pThisDungeon->IsStadium() && g_pMainPlayer->m_dwGuildWarFlag == G_W_F_OBSERVER )
	{
		if ( pInterface->m_pMenu[EXIT_WND]->m_bInit != TRUE )	
			return FALSE;
		if ( pInterface->m_pMenu[EXIT_WND]->GetActive() != TRUE )			
			return FALSE;
		if ( pInterface->m_pMenu[EXIT_WND]->CheckInterface() != TRUE )		
			return FALSE;

		return FALSE;
	}
	
	CSkillWnd*			pSkillWnd		= CSkillWnd::GetInstance();
	CItemShopWnd*		pItemShopWnd	= CItemShopWnd::GetInstance();
	CStoreWnd*			pStoreWnd		= CStoreWnd::GetInstance();
	CBankWnd*			pBankWnd		= CBankWnd::GetInstance();
	CItemUpgradeWnd*	pItemUpgradeWnd = CItemUpgradeWnd::GetInstance();
	CItemMakingWnd*		pItemMakingWnd	= CItemMakingWnd::GetInstance();
		
	if(pInterface->InterfaceCheck() == -1)
	{
		if(pSkillWnd->m_bySkillIndex != 0)
		{
			pSkillWnd->m_bySkillIndex = 0;
		}
		if(pItemShopWnd->GetActive()==TRUE)
		{			
			pItemShopWnd->SetActive(FALSE);						
		}
		if(pStoreWnd->GetActive()==TRUE)
		{
			pStoreWnd->SetActive(FALSE);			
		}
		if(pBankWnd->GetActive()==TRUE)
		{
			pBankWnd->SetActive(FALSE);			
		}
		if(pItemUpgradeWnd->GetActive()==TRUE)
		{
			pItemUpgradeWnd->SetActive(FALSE);
		}
		if(pItemMakingWnd->GetActive()==TRUE)
		{
			pItemMakingWnd->SetActive(FALSE);
		}
		
		pInterface->m_bActiveChk = TRUE;		

		if(pInterface->GetMouseIcon()==FALSE)
		{
			CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_DEFAULTCLICK__);
		}

		return FALSE;
	}		
	
	if(pInterface->GetMouseIcon()==FALSE)
	{
		CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_DEFAULTCLICK__);
	}

	return TRUE;
}


void OnLButtonDownDungeon(WPARAM wParam, LPARAM lParam)
{
	if (ItemPickupFiltering::sharedInstance()->handleMouseDown()) {
		return;
	}

	CGroupWnd*		pGroupWnd		= CGroupWnd::GetInstance();
	CUserInterface* pUserInterface	= CUserInterface::GetInstance();	
	g_Mouse.dwLButtonDownTime		= g_dwCurTick;
	g_pMainPlayer->m_i64PickupItem	= 0;

	if( GetAsyncKeyState(VK_SHIFT) & 0x800000 && g_bLshift )	
	{
		g_rcSelectBox.left	= g_Mouse.MousePos.x;
		g_rcSelectBox.top	= g_Mouse.MousePos.y;
		g_rcSelectBox.right	= g_Mouse.MousePos.x;
		g_rcSelectBox.bottom= g_Mouse.MousePos.y;
		return;
	}

	if( OnLButtonDownInterfaceDungeon() )
	{
		g_pMainPlayer->SendSkill();
		return;		
	}

	if(pUserInterface->GetPointerChk() == TRUE )
	{
		pUserInterface->SetPointerChk( FALSE );	
		pUserInterface->SetMousePointerType(__MOUSE_POINTER_DEFAULT__);

		switch ( pGroupWnd->m_byStatus )
		{
			case GROUP_PARTY_CREATEADDCLK:	pGroupWnd->m_bPartyMove = TRUE;		break;
			case GROUP_GUILD_ADD:			pGroupWnd->m_bGuildMove	= TRUE;		break;
			case GROUP_FRIEND_ADD:			pGroupWnd->m_bMessengerMove = TRUE;	break;
			case GROUP_GUILDWAR_ADD:		pGroupWnd->m_bGuildWarMove = TRUE;	break;
			default:															break;
		}
	}
		
	if( g_hHandle )	
	{
		if( g_pMainPlayer->GetStatus() == UNIT_STATUS_WALKING || g_pMainPlayer->GetStatus() == UNIT_STATUS_RUNNING)
		{
			g_pMainPlayer->SetAction( MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );			
			g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
			g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;
			SendStopPacket();
		}
		
		LPObjectDesc pObjDesc = (LPObjectDesc)g_pExecutive->GetData( g_hHandle );

		if (!pObjDesc)
		{
			goto lb_move;
			return;
		}
		else if( g_pThisDungeon->IsStadium() && g_pMainPlayer->m_dwGuildWarFlag == G_W_F_OBSERVER )
			goto lb_move;

		// 픽킹된 오브젝트 타입에따라 처리.
		switch( pObjDesc->bType )
		{
			case OBJECT_TYPE_MONSTER:			PickingObjectTypeMonster(pObjDesc);			break;
			case OBJECT_TYPE_PLAYER:			PickingObjectTypePlayer(pObjDesc);			break;
			case OBJECT_TYPE_TRADER_MONSTER:	PickingObjectTypeTrader_Monster(pObjDesc);	break;
			case OBJECT_TYPE_ITEM:				PickingObjectTypeItem(pObjDesc);			break;
			default:																		break;
		}

		pUserInterface->m_bMatch = FALSE;		
		CheckAndProcForGroupWnd();
	}
	else 
	{
		pUserInterface->m_bMatch = FALSE;

		CheckAndProcForGroupWnd();
		
		// 쉬프트 키가 눌려져 있다면 몬스터들을 이동시킨다.
		if( g_bLCtrl )
		{
			CTDS_GUARDIAN_COMMAND_MOVE Move;
			Move.v2DestPos.x	= g_Mouse.v3Mouse.x / TILE_WIDTH;
			Move.v2DestPos.y	= g_Mouse.v3Mouse.z / TILE_HEIGHT;

			if( FillSelectedMonster( Move.pdwGuardianIndex, Move.pdwMonsterIndex ) != 0 )
			{
				g_pNet->SendMsg( (char*)&Move, Move.GetPacketSize(), SERVER_INDEX_ZONE );
			}

			// 강제로 l버튼 플래그를 false한다. MouseEventDungeon에 진입못하게 하기 위해서.
			g_Mouse.bLDown = false;
		}
		else
		{
			goto lb_move;
		}
	}

	// sung-han 2005-01-30 길드전 관련
	if(pGroupWnd->m_bGuildWarChallenge)
	{
		pGroupWnd->m_bGuildWarChallenge = FALSE;
	}
	
	if( !g_bLCtrl )
	{
		SkillEvent(0);
	}

	return;

lb_move:
	DungeonMove();

	return;
}


void OnLButtonUpDungeon(WPARAM wParam, LPARAM lParam)
{
	if (ItemPickupFiltering::sharedInstance()->handleMouseUp()) {
		return;
	}
	CInterface*			pInterface			= CInterface::GetInstance();
	CUserInterface*		pUserInterface		= CUserInterface::GetInstance();
	CGroupWnd*			pGroupWnd			= CGroupWnd::GetInstance();
	
	if(pUserInterface->m_bTrade)
	{
		pUserInterface->m_bTrade = FALSE;
	}

	if(pUserInterface->m_nChatChk)	
	{
		ChatMsg();
		return;
	}

	pInterface->SetUp(TRUE);	
	pInterface->InterfaceCheck();
			
	if(pInterface->GetClick())
	{
		pInterface->SetClick(FALSE);	
	}
	
	pInterface->m_bActiveChk	= FALSE;	
	pGroupWnd->m_bPartyMove		= FALSE;
	pGroupWnd->m_bGuildMove		= FALSE;	
	pGroupWnd->m_bMessengerMove = FALSE;
	pGroupWnd->m_bGuildWarMove	= FALSE;
	g_bPlayerShop				= FALSE;

	if(pInterface->GetMouseIcon()==FALSE)
	{
		if(g_hHandle)
		{
			LPObjectDesc pObjDesc = (LPObjectDesc)g_pExecutive->GetData( g_hHandle );

			if(pObjDesc)
			{
				switch( pObjDesc->bType )
				{
					case OBJECT_TYPE_MONSTER:
						CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_ATTACK__);
						break;
					case OBJECT_TYPE_TRADER_MONSTER:
						CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_NPC__);
						break;
					case OBJECT_TYPE_ITEM:
						CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_ITEM__);
						break;
					default:		
						pUserInterface->SetMousePointerType(__MOUSE_POINTER_DEFAULT__);
						break;
				}
			}
			else
			{
				pUserInterface->SetMousePointerType(__MOUSE_POINTER_DEFAULT__);
			}
		}
		else
		{
			pUserInterface->SetMousePointerType(__MOUSE_POINTER_DEFAULT__);
		}
	}

	ItemNativeOnLButtonUpProcess();

	// 셀렉트 박스에 의한 픽킹된 몬스터를 가려낸다.
	if( g_bLshift )
	{
		DWORD				i=0;
		RECT				rcBox;
		CMonster*			pMonster=0;
		LPObjectDesc		pDesc=0;
		PICK_GXOBJECT_DESC	pPickDesc[ 100 ];
		
		// 사각형의 범위를 구하고.
		SetRect( &rcBox, min( g_rcSelectBox.left, g_rcSelectBox.right ),
						 min( g_rcSelectBox.top, g_rcSelectBox.bottom ),
						 max( g_rcSelectBox.left, g_rcSelectBox.right ),
						 max( g_rcSelectBox.top, g_rcSelectBox.bottom ) );

		// 일단 모든 몬스터와 가디언을 픽되지 않은 상태로 만든다.
		for( i=0; i<MAX_USER_GUARDIAN; i++ )
		{
			if( g_pMainPlayer->m_pGuardian[i] )
			{
				SelectSummonMonster(g_pMainPlayer->m_pGuardian[i], FALSE);
			}
		}

		for( i=0; i<MAX_USER_GUARDIAN; i++ )
		{
			if( g_pMainPlayer->m_pMonster[i] )
			{
				SelectSummonMonster(g_pMainPlayer->m_pMonster[i], FALSE);
			}
		}

		DWORD dwCount = g_pExecutive->GXOGetMultipleObjectWithScreenCoordRect( pPickDesc, 100, &rcBox, 0, 0 );
		
		for( i=0; i<dwCount; i++ )
		{
			pDesc = (LPObjectDesc)g_pExecutive->GetData( pPickDesc[i].gxo );

			if (!pDesc)
				continue;

			if( ( pDesc->bType != OBJECT_TYPE_MONSTER ) )
				continue;

			pMonster = (CMonster*)pDesc->pInfo;

			if( !pMonster )
				continue;

			if( g_pMainPlayer->m_dwUserIndex == pMonster->m_dwLordIndex )
			{
				SelectSummonMonster(pMonster, TRUE);
			}
		}
	}
}


void OnRButtonDownDungeon(WPARAM wParam, LPARAM lParam)
{	
	if( g_pThisDungeon->IsStadium() && g_pMainPlayer->m_dwGuildWarFlag == G_W_F_OBSERVER )
		return;

	CInterface* pInterface	= CInterface::GetInstance();
	
	pInterface->SetUp(FALSE);
	
	if(pInterface->m_bActiveChk==FALSE)
	{
		pInterface->InterfaceCheck();					
	}
				
	if(pInterface->GetMouseIcon()==FALSE)
	{
		if(g_ItemMoveManager.GetNewItemMoveMode())
		{

		}
		else
		{
			if(g_pMainPlayer->m_MouseItem.GetID()==0)
			{
				CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_DEFAULT__);		
			}
		}		
	}	
	
	CItemWnd*	pItemWnd	= CItemWnd::GetInstance();
	int			nPosX		= int((g_Mouse.MousePos.x-(pItemWnd->m_fPosX+6))/34);
	int			nPosY		= int((g_Mouse.MousePos.y-(pItemWnd->m_fPosZ+278))/34);	
			
	if(pItemWnd->GetActive()==TRUE)	
	{
		if(pItemWnd->m_byItemChk == 1)
		{				
			int nItemIndex	= nPosY*7+nPosX;
			int nValue		= g_pMainPlayer->m_pInv_Small[nItemIndex].m_wItemID/ITEM_DISTRIBUTE;

			if(nItemIndex>=MAX_INV_SMALL_POOL)
				return;					
				
			if(nValue==ITEM_SUPPLIES_INDEX)
			{
				g_bBeltChk = TRUE;
				return;
			}				
			else if(nValue>=ITEM_SPECIALST_INDEX && nValue<=ITEM_SPECIALEND_INDEX)
			{
				g_bGuildChk = TRUE;
				return;
			}
			else if(nValue==ITEM_ZODIAC_INDEX)
			{
				g_bRButton = TRUE;
				return;
			}
			else if(nValue==ITEM_MAGICARRAY_INDEX)
			{
				g_bMagicChk = TRUE;
				return;
			}
		}
	}
	
	if(	g_Mouse.MousePos.x>=727	&&	g_Mouse.MousePos.x<=1024 &&
		g_Mouse.MousePos.y>=733	&&	g_Mouse.MousePos.y<=768)
	{
		if (g_pMainPlayer->m_bMatching)
			return;

		int nPosX	= (g_Mouse.MousePos.x-727)/37;
		int nValue	= g_pMainPlayer->m_pBelt[nPosX].m_wItemID/ITEM_DISTRIBUTE;

		if(nValue==ITEM_SUPPLIES_INDEX)
		{
			g_bBeltChk = TRUE;
			return;
		}
		else if(nValue==ITEM_ZODIAC_INDEX)
		{
			if (TRUE == g_pMainPlayer->m_bInEventDungeon)
			{			
				//"이벤트 던전내에서 조디악 카드 사용은 금지되어 있습니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE500].szMessage, 0xffff2cff);
				return;
			}

			g_bRButton = TRUE;

			return;
		}
	}
	
	for(int i = 0; i < CUR_INTERFACE; i++)
	{
		if(i==COMMAND_WND || i==INTERFACE_USER)
			continue;
		
		if(pInterface->m_pMenu[i]->GetActive()==TRUE)
		{
			int nStartPosX	= pInterface->m_pMenu[i]->GetPosX();
			int nStartPosY	= pInterface->m_pMenu[i]->GetPosY();
			int nEndPosX	= pInterface->m_pMenu[i]->GetPosX() + pInterface->m_nSize[i*2];
			int nEndPosY	= pInterface->m_pMenu[i]->GetPosY() + pInterface->m_nSize[i*2+1];						

			if(	g_Mouse.MousePos.x>=nStartPosX	&&	g_Mouse.MousePos.x<=nEndPosX	&&
				g_Mouse.MousePos.y>=nStartPosY	&&	g_Mouse.MousePos.y<=nEndPosY)
			{
				g_bRButton	= TRUE;
				return;
			}
		}		
	}

	// sung-han 2005-01-17 가디언 포션을 우클릭으로 사용시 스킬 사용되는 부분 수정, 예외처리...
	// 가디언 창이 떠있나 체크한후 영역을 체크해서 포함되면 그냥 리턴
	//--------------------------------------------------------------------------------------------
	CUserInterface* pUserInterface = CUserInterface::GetInstance();
	if( pUserInterface->m_sSprite_Data[SPR_OBJ_GUARDIAN_WND1].bRender )
	{
		int nStartPosX	= pUserInterface->m_sSprite_Data[SPR_OBJ_GUARDIAN_WND1].fPosX;
		int nStartPosY	= pUserInterface->m_sSprite_Data[SPR_OBJ_GUARDIAN_WND1].fPosY;
		int nEndPosX	= nStartPosX + 128;
		int nEndPosY	= nStartPosY + 50;

		if( g_Mouse.MousePos.x>=nStartPosX	&&	g_Mouse.MousePos.x<=nEndPosX	&&
			g_Mouse.MousePos.y>=nStartPosY	&&	g_Mouse.MousePos.y<=nEndPosY)
		{
			g_bRButton	= TRUE;
			return;
		}
	}
	//--------------------------------------------------------------------------------------------
	
	if (g_pMainPlayer->GetStatus() == UNIT_STATUS_ATTACKING	&& g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_TYPE ])
	{
		// 누군가를 때리고 있었다...스킬쏠 준비를 위해
		g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_TYPE ] = 0;
		g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_INDEX ] = 0;
		g_pMainPlayer->m_dwTemp[ USER_TEMP_LASTATTACKTICK ] = g_dwCurTick;		
	}	
	
	SkillEvent(1);
	
	if (g_pMainPlayer->GetStatus() == UNIT_STATUS_NORMAL	||
		g_pMainPlayer->GetStatus() == UNIT_STATUS_WALKING	||
		g_pMainPlayer->GetStatus() == UNIT_STATUS_RUNNING)
	{		
		GXSetPosition(CUserInterface::GetInstance()->m_pUserClickHandle.pHandle, &g_Mouse.v3Mouse, FALSE);
		SetAction(CUserInterface::GetInstance()->m_pUserClickHandle.pHandle, 1, ACTION_ONCE);
		ShowObject(CUserInterface::GetInstance()->m_pUserClickHandle.pHandle);
	}		
}


void OnRButtonUpDungeon(WPARAM wParam, LPARAM lParam)
{			
	CInterface* pInterface	= CInterface::GetInstance();
	
	pInterface->SetUp(FALSE);
	
	int nChkInter = 0;

	if(pInterface->m_bActiveChk==FALSE)
		nChkInter = pInterface->InterfaceCheck();					
				
	if(pInterface->GetMouseIcon()==FALSE)
	{
		if(g_pMainPlayer->m_MouseItem.GetID()==0)
			CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_DEFAULT__);		
	}	

	CItemWnd*	pItemWnd	= CItemWnd::GetInstance();
	int			nPosX		= int((g_Mouse.MousePos.x-(pItemWnd->m_fPosX+6))/34);
	int			nPosY		= int((g_Mouse.MousePos.y-(pItemWnd->m_fPosZ+278))/34);
	BOOL		bChk		= FALSE;
	BYTE		bInter		= FALSE;

	if(nChkInter==-1)
	{
		g_pMainPlayer->SendSkill();	
	}

	if(pItemWnd->GetActive()==TRUE)		
	{
		if(	g_Mouse.MousePos.x>=pItemWnd->m_fPosX &&
			g_Mouse.MousePos.x<=pItemWnd->m_fPosX+CItemWnd::GetInstance()->m_fMaxSizeWidth &&
			g_Mouse.MousePos.y>=pItemWnd->m_fPosZ &&
			g_Mouse.MousePos.y<=pItemWnd->m_fPosZ+CItemWnd::GetInstance()->m_fMaxSizeHeight)
		{
			if(pItemWnd->m_byItemChk == 1)
			{				
				if(g_pMainPlayer->GetStatus()!=UNIT_STATUS_DEAD)
				{
					BYTE	byZipCode	= BYTE(nPosY*7+nPosX);
				
					if(byZipCode>=MAX_INV_SMALL_POOL)
						return;

					if(!bChk)
						bChk = ItemUsedSupplies(&g_pMainPlayer->m_pInv_Small[byZipCode], byZipCode, ZODIAC_USE_TYPE_INVENTORY);
					if(!bChk)
						bChk = ItemUsedConsumable(&g_pMainPlayer->m_pInv_Small[byZipCode], byZipCode, ZODIAC_USE_TYPE_INVENTORY);
					if(!bChk)
						bChk = ItemUsedSpecial(&g_pMainPlayer->m_pInv_Small[byZipCode], byZipCode, ZODIAC_USE_TYPE_INVENTORY);
					if(!bChk)
						bChk = ItemUsedMagicArray(&g_pMainPlayer->m_pInv_Small[byZipCode], byZipCode, ZODIAC_USE_TYPE_INVENTORY);
					if(!bChk)
						bChk = ItemUsedZodiac(&g_pMainPlayer->m_pInv_Small[byZipCode], byZipCode, ZODIAC_USE_TYPE_INVENTORY);
					if(!bChk)
						bChk = ItemUsedZodianInsurance(&g_pMainPlayer->m_pInv_Small[byZipCode], byZipCode, ZODIAC_USE_TYPE_INVENTORY);

					bInter = TRUE;
				}
			}
		}
	}	
	
	if(	g_Mouse.MousePos.x>=727		&&	g_Mouse.MousePos.x<=1024	&&
		g_Mouse.MousePos.y>=733		&&	g_Mouse.MousePos.y<=768)
	{
		if(g_pMainPlayer->GetStatus()!=UNIT_STATUS_DEAD)
		{
			BYTE	byZipCode	= byte((g_Mouse.MousePos.x-727)/37);
	
			if(nPosX>=MAX_BELT_POOL)
				return;

			if(!bChk)
				bChk = ItemUsedSupplies(&g_pMainPlayer->m_pBelt[byZipCode], byZipCode, ZODIAC_USE_TYPE_BELT);
			if(!bChk)
				bChk = ItemUsedConsumable(&g_pMainPlayer->m_pBelt[byZipCode], byZipCode, ZODIAC_USE_TYPE_BELT);
			if(!bChk)
				bChk = ItemUsedZodiac(&g_pMainPlayer->m_pBelt[byZipCode], byZipCode, ZODIAC_USE_TYPE_BELT);							

			bInter = TRUE;
		}
	}
	
	g_bBeltChk	= FALSE;
	g_bGuildChk = FALSE;
	g_bRButton	= FALSE;
	g_bMagicChk	= FALSE;

	if(!bChk && !bInter && nChkInter!=-1)
	{
		// "사용할 수 있는 아이템이 없습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE791].szMessage, 0xFFFF2CFF); 
	}
}


void OnMouseMoveDungeon(WPARAM wParam, LPARAM lParam)
{	
	BOOL		bObejct		= FALSE;
	CInterface* pInterface	= CInterface::GetInstance();
	
	pInterface->SetUp(FALSE);
	
	if(pInterface->m_bActiveChk==FALSE)
	{				
		pInterface->InterfaceCheck();		
	}
				
	if(pInterface->GetMouseIcon()==FALSE)
	{
		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			if( !g_ItemMoveManager.GetMouseItemID() )
				CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_DEFAULT__);		
		}
		else
		{
			if(g_pMainPlayer->m_MouseItem.GetID()==0)
			{
				CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_DEFAULT__);		
			}
		}		
	}
	else
	{
		bObejct = TRUE;
	}

	g_pMainPlayer->m_InfoItem.m_wItemID = 0;
	g_pMainPlayer->m_dwItemInfoType		= 0;

	if(!g_Mouse.bLDown)
	{
		pInterface->m_bActiveChk = FALSE;

		CItemWnd* pItemWnd = CItemWnd::GetInstance();

		if(pItemWnd->GetActive())
		{
			switch(pItemWnd->m_sItemChkType.nItemIndex)
			{
			case __ITEM_LARGE__:
				if( g_pMainPlayer->m_pInv_Large[pItemWnd->m_sItemChkType.byItemZipCode].m_wItemID != 0)
				{
					memcpy( &g_pMainPlayer->m_InfoItem, 
						&g_pMainPlayer->m_pInv_Large[pItemWnd->m_sItemChkType.byItemZipCode], sizeof(CItem) );
					g_pMainPlayer->m_dwItemInfoType = 1;
				}
				return;

			case __ITEM_SMALL__:
				if( g_pMainPlayer->m_pInv_Small[pItemWnd->m_sItemChkType.byItemZipCode].m_wItemID != 0)
				{
					memcpy( &g_pMainPlayer->m_InfoItem, 
						&g_pMainPlayer->m_pInv_Small[pItemWnd->m_sItemChkType.byItemZipCode], sizeof(CItem) );
					g_pMainPlayer->m_dwItemInfoType = 1;
				}
				return;

			case __ITEM_EQUIP__:
				if( g_pMainPlayer->m_pEquip[pItemWnd->m_sItemChkType.byItemZipCode].m_wItemID != 0)
				{
					memcpy( &g_pMainPlayer->m_InfoItem, 
						&g_pMainPlayer->m_pEquip[pItemWnd->m_sItemChkType.byItemZipCode], sizeof(CItem) );
					g_pMainPlayer->m_dwItemInfoType = 1;
				}
				return;
			}
		}
		
		if( g_pMainPlayer->m_InfoItem.m_wItemID == 0 )
		{
			if(bObejct==FALSE)
			{				
				// 위에서 return이 안됐다면..
				// 마우스를 움직일때 아이템이 걸렸다면 아이템 정보를 출력하기 위해.				

				if(g_hHandle)
				{
					LPObjectDesc pObjDesc = (LPObjectDesc)g_pExecutive->GetData( g_hHandle );
					if( !pObjDesc ) return;

					switch( pObjDesc->bType )
					{
					case OBJECT_TYPE_ITEM:
						{
							ITEM*	pItem = (ITEM*)pObjDesc->pInfo;

							if(!pItem)
								return;

							memcpy( &g_pMainPlayer->m_InfoItem, &pItem->Item, sizeof( CItem ) );
							g_pMainPlayer->m_dwItemInfoType = 2;
							CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_ITEM__);
						}
						break;
					case OBJECT_TYPE_MONSTER:
						{
							CMonster* pMonster;
							pMonster = (CMonster*)pObjDesc->pInfo;

							if(pMonster->m_dwHP>0)
							{
								CUserInterface::GetInstance()->OpenMonsterBar(pMonster->m_szName, pMonster->m_dwHP, pMonster->m_dwMaxHP);

								if( !g_pThisDungeon->IsStadium() || g_pMainPlayer->m_dwGuildWarFlag != G_W_F_OBSERVER )
									CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_ATTACK__);
							}		
						}
						break;
					case OBJECT_TYPE_TRADER_MONSTER:
						if(pInterface->GetMouseIcon()==FALSE)
						{
							if( !g_pThisDungeon->IsStadium() || g_pMainPlayer->m_dwGuildWarFlag != G_W_F_OBSERVER )
								CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_NPC__);
						}
						break;

					case OBJECT_TYPE_PLAYER:
						{
							// 노점상 모드면 npc 처럼 마우스커서 변경해야지.
							CUser* pUser = (CUser*)pObjDesc->pInfo;
							if (pUser->m_bPlayerShop)
							{
								if( !g_pThisDungeon->IsStadium() || g_pMainPlayer->m_dwGuildWarFlag != G_W_F_OBSERVER )
									CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_NPC__);
							}

							//	sung-han 2004-12-30 : pvp 에서 캐릭터 에너지바와 엘레맨탈 에너지 바가 겹치는 부분 처리
							//-----------------------------------------------------------------------------------------
							CUserInterface * pUserInterface = CUserInterface::GetInstance();
							if( pUserInterface->m_pMatchUser && pUserInterface->m_bMatchUserHPShowFlag )
							{								
								pUserInterface->m_bMatchUserHPShowEnable = TRUE;
								pUserInterface->OpenUserBar(pUser->m_szName, pUserInterface->m_fUserHp, pUserInterface->m_fUserMaxHp);
							}
							//-----------------------------------------------------------------------------------------

							if (pUser && !g_pMainPlayer->IsAlliance(pUser))
							{
								if(g_pThisDungeon)
								{
									if(g_pThisDungeon->GetDungeonType() != DUNGEON_TYPE_VILLAGE)
									{
										// 같은 편이 아니다.
										if( !g_pThisDungeon->IsStadium() || g_pMainPlayer->m_dwGuildWarFlag != G_W_F_OBSERVER )
											CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_ATTACK__);
									}
								}
							}
						}
						break;				
					}
				}
			}
		}
	}

	if(g_Mouse.bLDown)
	{
		if( !g_bLshift )
		{
			if( g_hHandle )
			{
				LPObjectDesc pObjDesc = (LPObjectDesc)g_pExecutive->GetData( g_hHandle );
				if( !pObjDesc ) return;

				switch( pObjDesc->bType )
				{
				case OBJECT_TYPE_ITEM:	
						CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_ITEMCLICK__);
					break;
				case OBJECT_TYPE_MONSTER:
						CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_ATTACKCLICK__);
					break;
				case OBJECT_TYPE_TRADER_MONSTER:
//					CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_NPCCLICK__);
					break;
				case OBJECT_TYPE_PLAYER:
					{
						// 노점상 모드면 npc 처럼 마우스커서 변경해야지.
						CUser* pUser = (CUser*)pObjDesc->pInfo;
						if (pUser->m_bPlayerShop)
							CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_NPCCLICK__);
					}
					break;			
				}
			}
		}
		else
		{
			g_rcSelectBox.right		= g_Mouse.MousePos.x;
			g_rcSelectBox.bottom	= g_Mouse.MousePos.y;
		}

		if(CUserInterface::GetInstance()->m_bMoveChr==TRUE)
		{
			HideObject(CUserInterface::GetInstance()->m_pUserMouseHandle[0].pHandle);
			HideObject(CUserInterface::GetInstance()->m_pUserMouseHandle[1].pHandle);
			HideObject(CUserInterface::GetInstance()->m_pUserMouseHandle[2].pHandle);
			CUserInterface::GetInstance()->m_bMoveChr = FALSE;
			CUserInterface::GetInstance()->m_byMoveType = 0;
			CUserInterface::GetInstance()->m_pMonster	= NULL;
			CUserInterface::GetInstance()->m_pUser		= NULL;
		}
	}
	else
	{		
		if( g_hHandle )
		{
			LPObjectDesc pObjDesc = (LPObjectDesc)g_pExecutive->GetData( g_hHandle );
			
			if(!pObjDesc)
				return;

			switch( pObjDesc->bType )
			{			
			case OBJECT_TYPE_MONSTER:				
				{
					CMonster*	pMonster;
					pMonster	= (CMonster*)pObjDesc->pInfo;
					
					CUserInterface::GetInstance()->m_pMonster = pMonster;

					if(pMonster->m_dwLordIndex==0)
					{
						if(	CUserInterface::GetInstance()->m_bMoveChr==FALSE ||
							CUserInterface::GetInstance()->m_byMoveType!=1)
						{
							GXSetPosition(CUserInterface::GetInstance()->m_pUserMouseHandle[0].pHandle, &pMonster->m_v3CurPos, FALSE);
							SetAction(CUserInterface::GetInstance()->m_pUserMouseHandle[0].pHandle, 1, ACTION_LOOP);
							ShowObject(CUserInterface::GetInstance()->m_pUserMouseHandle[0].pHandle);
							CUserInterface::GetInstance()->m_bMoveChr = TRUE;
							CUserInterface::GetInstance()->m_byMoveType = 1;
						}						
					}
					else if(g_pMainPlayer->m_dwUserIndex==pMonster->m_dwLordIndex)
					{
						if(	CUserInterface::GetInstance()->m_bMoveChr==FALSE ||
							CUserInterface::GetInstance()->m_byMoveType!=2)
						{
							GXSetPosition(CUserInterface::GetInstance()->m_pUserMouseHandle[1].pHandle, &pMonster->m_v3CurPos, FALSE);
							SetAction(CUserInterface::GetInstance()->m_pUserMouseHandle[1].pHandle, 1, ACTION_LOOP);
							ShowObject(CUserInterface::GetInstance()->m_pUserMouseHandle[1].pHandle);
							CUserInterface::GetInstance()->m_bMoveChr = TRUE;
							CUserInterface::GetInstance()->m_byMoveType = 2;
						}						
					}					
					else
					{
						if(	CUserInterface::GetInstance()->m_bMoveChr==FALSE ||
							CUserInterface::GetInstance()->m_byMoveType!=3)
						{
							GXSetPosition(CUserInterface::GetInstance()->m_pUserMouseHandle[2].pHandle, &pMonster->m_v3CurPos, FALSE);
							SetAction(CUserInterface::GetInstance()->m_pUserMouseHandle[2].pHandle, 1, ACTION_LOOP);
							ShowObject(CUserInterface::GetInstance()->m_pUserMouseHandle[2].pHandle);
							CUserInterface::GetInstance()->m_bMoveChr = TRUE;
							CUserInterface::GetInstance()->m_byMoveType = 3;
						}						
					}
				}				
				break;
			case OBJECT_TYPE_PLAYER:
				{
					CUser*	pUser;
					pUser = (CUser*)pObjDesc->pInfo;

					CUserInterface::GetInstance()->m_pUser = pUser;

					if(pUser->m_dwPartyId==g_pMainPlayer->m_dwPartyId)
					{
						if(	CUserInterface::GetInstance()->m_bMoveChr==FALSE ||
							CUserInterface::GetInstance()->m_byMoveType!=4)
						{
							GXSetPosition(CUserInterface::GetInstance()->m_pUserMouseHandle[1].pHandle, &pUser->m_v3CurPos, FALSE);
							SetAction(CUserInterface::GetInstance()->m_pUserMouseHandle[1].pHandle, 1, ACTION_LOOP);
							ShowObject(CUserInterface::GetInstance()->m_pUserMouseHandle[1].pHandle);
							CUserInterface::GetInstance()->m_bMoveChr = TRUE;
							CUserInterface::GetInstance()->m_byMoveType = 4;
						}						
					}
					else
					{
						if(	CUserInterface::GetInstance()->m_bMoveChr==FALSE ||
							CUserInterface::GetInstance()->m_byMoveType!=5)
						{
							GXSetPosition(CUserInterface::GetInstance()->m_pUserMouseHandle[2].pHandle, &pUser->m_v3CurPos, FALSE);
							SetAction(CUserInterface::GetInstance()->m_pUserMouseHandle[2].pHandle, 1, ACTION_LOOP);
							ShowObject(CUserInterface::GetInstance()->m_pUserMouseHandle[2].pHandle);
							CUserInterface::GetInstance()->m_bMoveChr = TRUE;
							CUserInterface::GetInstance()->m_byMoveType = 5;
						}						
					}													
				}				
				break;
			case OBJECT_TYPE_MAINPLAYER:
				{		
					if(	CUserInterface::GetInstance()->m_bMoveChr==FALSE ||
						CUserInterface::GetInstance()->m_byMoveType!=6)
					{
						GXSetPosition(CUserInterface::GetInstance()->m_pUserMouseHandle[2].pHandle, &g_pMainPlayer->m_v3CurPos, FALSE);
						SetAction(CUserInterface::GetInstance()->m_pUserMouseHandle[2].pHandle, 1, ACTION_LOOP);
						ShowObject(CUserInterface::GetInstance()->m_pUserMouseHandle[2].pHandle);
						CUserInterface::GetInstance()->m_bMoveChr = TRUE;
						CUserInterface::GetInstance()->m_byMoveType = 6;
					}					
				}
				break;
			}
		}
		else
		{
			// 애니메이션 //
			if(CUserInterface::GetInstance()->m_bMoveChr==TRUE)
			{
				HideObject(CUserInterface::GetInstance()->m_pUserMouseHandle[0].pHandle);
				HideObject(CUserInterface::GetInstance()->m_pUserMouseHandle[1].pHandle);
				HideObject(CUserInterface::GetInstance()->m_pUserMouseHandle[2].pHandle);
				CUserInterface::GetInstance()->m_bMoveChr = FALSE;
				CUserInterface::GetInstance()->m_byMoveType = 0;
				CUserInterface::GetInstance()->m_pMonster	= NULL;
				CUserInterface::GetInstance()->m_pUser		= NULL;
			}
		}
	}

	// 만약 가운데 버튼이 눌려져 있는 상태라면 카메라를 업데이트 해야한다.
	if( g_Mouse.bMDown )
	{
		float		fOldPitch = 0.f, fPitch = 0.f;
		CAMERA_DESC CameraDesc;
		VECTOR3		v3Pos, v3CameraAngleRad;
		
		int x = g_Mouse.MouseOldPos.x - g_Mouse.MousePos.x;
		int y = g_Mouse.MouseOldPos.y - g_Mouse.MousePos.y;
		
		if( x != 0 )
		{
			g_pExecutive->GXOGetPosition( g_pMainPlayer->m_hPlayer.pHandle, &v3Pos );
			SetCameraYaw( &v3Pos, DEG01 * CAMERA_Y * x * -1.0f );
		}
	       	
		if( y > 0 )
		{
			g_pGeometry->GetCameraDesc( &CameraDesc, 0 );
			g_pGeometry->GetCameraAngleRad( &v3CameraAngleRad, 0 );

			fOldPitch	=  ( 180.f / PI * v3CameraAngleRad.x ) * -1.0f;
			fPitch		=  ( ( 180.f / PI * v3CameraAngleRad.x * -1.f ) - ( CAMERA_X * y ) );
#ifdef DEVELOP_MODE 
			if( fOldPitch <= -30 ) return;
			
			if( fPitch <= -30 )
			{
				fPitch = fOldPitch - -30; // ??? 이건 무슨 의미지????
			}
			else
			{
				fPitch = fOldPitch - fPitch;
			}
#else
			if( fOldPitch <= __MIN_CAMERA_X__ ) return;
			
			if( fPitch <= __MIN_CAMERA_X__ )
			{
				fPitch = fOldPitch - __MIN_CAMERA_X__;
			}
			else
			{
				fPitch = fOldPitch - fPitch;
			}
#endif
			g_pExecutive->GXOGetPosition( g_pMainPlayer->m_hPlayer.pHandle, &v3Pos );
			SetCameraPitch( &v3Pos, DEG01 * fPitch );
		}
		else if( y < 0 )
		{
			g_pGeometry->GetCameraDesc( &CameraDesc, 0 );
			g_pGeometry->GetCameraAngleRad( &v3CameraAngleRad, 0 );
			
			fOldPitch	=  ( 180.f / PI * v3CameraAngleRad.x ) * -1.0f;
			fPitch =  ( 180.f / PI * v3CameraAngleRad.x * -1.0f ) + ( CAMERA_X * y * -1.0f );
#ifdef DEVELOP_MODE
			if( fOldPitch >= 100 ) return;

			if( fPitch >= 100 )
			{
				fPitch = 100 - fOldPitch;
			}
			else
			{
				fPitch = fPitch - fOldPitch;
			}
#else
			if( fOldPitch >= __MAX_CAMERA_X__ ) return;

			if( fPitch >= __MAX_CAMERA_X__ )
			{
				fPitch = __MAX_CAMERA_X__ - fOldPitch;
			}
			else
			{
				fPitch = fPitch - fOldPitch;
			}
#endif
			
			g_pExecutive->GXOGetPosition( g_pMainPlayer->m_hPlayer.pHandle, &v3Pos );
			SetCameraPitch( &v3Pos, DEG01 * fPitch * -1.0f);
		}

		// 다음번을 위하여 마우스위치를 기억한다.
		g_Mouse.MouseOldPos.x	= g_Mouse.MousePos.x;
		g_Mouse.MouseOldPos.y	= g_Mouse.MousePos.y;

		// 카메라를 청취자로 셋팅 
		SetListener(&v3CameraAngleRad);
	}
}

void OnMouseWheelDungeon(WPARAM wParam, LPARAM lParam)
{
	// 카메라 확대 축소 회전.
	VECTOR3		v3Pos;
	g_pExecutive->GXOGetPosition( g_pMainPlayer->m_hPlayer.pHandle, &v3Pos );

	if( LOWORD( wParam ) == MK_SHIFT )	// 회전
	{
		if(((short)HIWORD(wParam)) > 0)		//wheel up
		{
			SetCameraYaw( &v3Pos, DEG01 * CAMERA_Y );					
		}
		else
		{
			SetCameraYaw( &v3Pos, DEG01 * CAMERA_Y * -1.0f );
		}
	}
	else								// 확대,축소
	{
		if(((short)HIWORD(wParam)) > 0)		//wheel up
		{
			if(g_nDGZoom>__MAX_ZOOM_IN__)
			{
				SetCameraPos( &v3Pos, CAMERA_DISTANCE );
				g_nDGZoom--;
			}
		}
		else								//wheel down
		{
			if(g_nDGZoom<__MAX_ZOOM_OUT__)
			{
				SetCameraPos( &v3Pos, CAMERA_DISTANCE * -1.f );
				g_nDGZoom++;
			}
		}	
	}
}

void OnCharDungeon(BYTE bCh)
{
	CQuantityWnd*	pQuantityWnd	= CQuantityWnd::GetInstance();
	CInterface*		pInterface		= CInterface::GetInstance();	
	CExitWnd*		pExitWnd		= CExitWnd::GetInstance();

	switch(bCh)
	{		
	case VK_ESCAPE:	
		{						
			// Item 팔때 //
			CQuantityWnd*	pQuantityWnd	= CQuantityWnd::GetInstance();
			CCharDieWnd*	pCharDieWnd		= CCharDieWnd::GetInstance();
			
			if( CGuildWarFinalSettingWnd::GetInstance()->GetActive() )
				break;

			if(pCharDieWnd->GetActive())
				break;

			if(pQuantityWnd->GetActive())
			{
				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					g_ItemMoveManager.Initialize();
				}

				if(pQuantityWnd->m_bQuantityType==__QUANTITY_STORE__)
				{								
					int		nVal = g_pMainPlayer->m_MouseItem.m_wItemID/ITEM_DISTRIBUTE;				
					BOOL	bChk = FALSE;

					CTDS_ITEM_PICKUP ItemPickup;
					ItemPickup.bSectionNum	= 1;
					ItemPickup.i64ItemID	= 0;

					if(nVal==50)
					{
						for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
						{
							if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
							{
								ItemPickup.bInv		= 14;
								ItemPickup.bZipCode	= BYTE(i);
								bChk				= TRUE;
								break;
							}
							else
							{
								if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
								{
									// 아이템에서 있는 칸을 찾는다 //
									if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<100)
									{
										ItemPickup.bInv		= 14;
										ItemPickup.bZipCode	= BYTE(i);
										bChk				= TRUE;
										break;
									}
								}															
							}
						}
						
						if(bChk==FALSE)
							ItemPickup.bInv	= 4;
						
						g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
						pQuantityWnd->m_bReq = TRUE;
					}			
				}
				else if(pQuantityWnd->m_bQuantityType==__QUANTITY_THROW__)
				{				
					int		nVal = g_pMainPlayer->m_MouseItem.m_wItemID/ITEM_DISTRIBUTE;				
					BOOL	bChk = FALSE;

					CTDS_ITEM_PICKUP ItemPickup;
					ItemPickup.bSectionNum	= 1;
					ItemPickup.i64ItemID	= 0;

//					if(nVal==50)
					{
						for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
						{
							if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
							{
								ItemPickup.bInv		= 14;
								ItemPickup.bZipCode	= BYTE(i);
								bChk				= TRUE;
								break;
							}
							else
							{
								if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
								{
									// 아이템에서 있는 칸을 찾는다 //
									if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<100)
									{
										ItemPickup.bInv		= 14;
										ItemPickup.bZipCode	= BYTE(i);
										bChk				= TRUE;
										break;
									}
								}
							}
						}
					}
					
					if(bChk==FALSE)
						ItemPickup.bInv	= 4;										
					
					g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
					pQuantityWnd->m_bReq = TRUE;
				}
				else if(pQuantityWnd->m_bQuantityType==__QUANTITY_BANKINSERT__)
				{
					int		nVal = g_pMainPlayer->m_MouseItem.m_wItemID/ITEM_DISTRIBUTE;				
					BOOL	bChk = FALSE;

					CTDS_ITEM_PICKUP ItemPickup;
					ItemPickup.bSectionNum	= 1;
					ItemPickup.i64ItemID	= 0;

					if(nVal==50)
					{
						for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
						{
							if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
							{
								ItemPickup.bInv		= 14;
								ItemPickup.bZipCode	= BYTE(i);
								bChk				= TRUE;
								break;
							}
							else
							{
								if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
								{
									// 아이템에서 있는 칸을 찾는다 //
									if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<100)
									{
										ItemPickup.bInv		= 14;
										ItemPickup.bZipCode	= BYTE(i);
										bChk				= TRUE;
										break;
									}
								}															
							}
						}
						
						if(bChk==FALSE)
							ItemPickup.bInv	= 4;
						
						g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
						pQuantityWnd->m_bReq = TRUE;
					}
				}
				else if(pQuantityWnd->m_bQuantityType==__QUANTITY_FROMBANKTOINV__)
				{
					
				}
			}
			
			BOOL bChk = FALSE;

			g_bInitSoundEnalbe = FALSE;
			for(int i = ITEM_WND; i < EXIT_WND; i++)
			{
				if(i != MINIMAP_WND && i != DUNGEONSIEGEINFO_WND)
				{
					if(bChk==FALSE)
					{
						if(pInterface->m_pMenu[i]->GetActive()==TRUE)
						{
							bChk = TRUE;
						}
					}

					pInterface->m_pMenu[i]->SetActive(FALSE);
				}				
			}
			g_bInitSoundEnalbe = TRUE;

			if(bChk==FALSE)
			{				
				pExitWnd->SetActive(!pExitWnd->GetActive());				
			}
			
			// Order //
			ChatModeOff(INPUT_BUFFER_19);

			if(GET_IMEEDIT()->IsActivated())
			{
				if(GET_IMEEDIT()->GetEditIndex()==2)
				{
					LPSTR lpszName = (LPSTR)GET_IMEEDIT()->GetImeEditString();			
					g_pInputManager->ClearInput(INPUT_BUFFER_6);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_6, lpszName, lstrlen(lpszName));
					
				}

				IMESetEdit(0);
			}
		}
		return;		
	case 8:	//BackSpace
		{
			CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();
			
			if(pQuantityWnd->GetActive())
			{				
				if(pQuantityWnd->m_byDrawIndex>0)
				{
					pQuantityWnd->m_szMoney[pQuantityWnd->m_byDrawIndex-1] = 0;
					pQuantityWnd->m_byDrawIndex -= 1;						
				}
				g_pInputManager->GetInputRect(INPUT_BUFFER_5).left = (LONG)pQuantityWnd->m_fPosX+124-(pQuantityWnd->m_byDrawIndex*6)-7-(pQuantityWnd->m_byDrawIndex/3+1)*6;
				g_pInputManager->GetInputRect(INPUT_BUFFER_5).right =(LONG)pQuantityWnd->m_fPosX+124;
				
			}

#ifdef _USE_IME
			if (GET_IMEEDIT()->IsActivated()) return;
#endif

			if(g_pGVDungeon->bChatMode)
				g_pInputManager->BackSpaceEvent();
			return;
		}
		break;

	case VK_TAB:
		{
			CGuildRankWnd* pGuildRankWnd = CGuildRankWnd::GetInstance();				

			g_pInputManager->GetInputBuffer(INPUT_BUFFER_8);
			g_pInputManager->GetInputBuffer(INPUT_BUFFER_9);
			g_pInputManager->GetInputBuffer(INPUT_BUFFER_10);
			g_pInputManager->GetInputBuffer(INPUT_BUFFER_11);
			

			POINT pChatPoint;
						
			if(pGuildRankWnd->GetActive()==TRUE)
			{
				pChatPoint.x = long(pGuildRankWnd->m_fPosX+58);

				switch(g_pInputManager->GetCurFocusInputID())
				{
				case INPUT_BUFFER_8:					
					g_pInputManager->SetFocusInput(INPUT_BUFFER_9);		
					
#ifdef	_USE_IME

					pChatPoint.y = long(pGuildRankWnd->m_fPosZ+60);
					g_pInputManager->ClearInput(INPUT_BUFFER_8);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_8, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));
					
					GET_IMEEDIT()->DeactivateIME();					
					GET_IMEEDIT()->SetImeEditString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_9));

#endif
					break;
				case INPUT_BUFFER_9:					
					g_pInputManager->SetFocusInput(INPUT_BUFFER_10);

#ifdef	_USE_IME

					pChatPoint.y = long(pGuildRankWnd->m_fPosZ+75);
					g_pInputManager->ClearInput(INPUT_BUFFER_9);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_9, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));
					
					GET_IMEEDIT()->DeactivateIME();
					GET_IMEEDIT()->SetImeEditString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_10));

#endif
					break;
				case INPUT_BUFFER_10:					
					g_pInputManager->SetFocusInput(INPUT_BUFFER_11);

#ifdef	_USE_IME

					pChatPoint.y = long(pGuildRankWnd->m_fPosZ+90);
					g_pInputManager->ClearInput(INPUT_BUFFER_10);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_10, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));
					
					GET_IMEEDIT()->DeactivateIME();					
					GET_IMEEDIT()->SetImeEditString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_11));

#endif

					break;
				case INPUT_BUFFER_11:
					g_pInputManager->SetFocusInput(INPUT_BUFFER_8);					

#ifdef	_USE_IME

					pChatPoint.y = long(pGuildRankWnd->m_fPosZ+45);
					g_pInputManager->ClearInput(INPUT_BUFFER_11);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_11, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));
					
					GET_IMEEDIT()->DeactivateIME();
					GET_IMEEDIT()->SetImeEditString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_8));

#endif
					break;					
				}

#ifdef	_USE_IME

				GET_IMEEDIT()->SetEditIndex(1);				
				GET_IMEEDIT()->ActivateIME(pChatPoint, MAX_RANK_REAL_NAME, pGuildRankWnd->GetStartOrder()+2);

#endif			
			}
			return;
		}
		break;			
	}

	if(!g_pGVDungeon->bChatMode)
	{
		return;
	}

	if(!pQuantityWnd->GetActive())
	{
		DWORD dwInputIndex = g_pInputManager->GetInputBufferLength(g_pInputManager->GetCurFocusInputID());

		switch(g_pInputManager->GetCurFocusInputID())
		{
		case INPUT_BUFFER_19:
			if(GetAsyncKeyState(VK_RETURN) & 0x800000)
				return;
			if(dwInputIndex>=__MAX_BUUFER_SIZE__) //ID와 Password 길이제한이 같다 
				return;
			break;
		case INPUT_BUFFER_7:
			if(dwInputIndex>=MAX_GUILD_NAME_REAL_LENGTH) //ID와 Password 길이제한이 같다 
				return;
			if(GetAsyncKeyState(VK_SPACE) & 0x800000)
				return;
			break;
		case INPUT_BUFFER_6:
			if(GetAsyncKeyState(VK_RETURN) & 0x800000)
				return;
			if(dwInputIndex>=__MAX_IDBUUFER_SIZE__) //ID와 Password 길이제한이 같다 
				return;
			break;
		case INPUT_BUFFER_8:
		case INPUT_BUFFER_9:
		case INPUT_BUFFER_10:
		case INPUT_BUFFER_11:
			if(GetAsyncKeyState(VK_SPACE) & 0x800000)
				return;
			if(dwInputIndex>=__MAX_RANK_SIZE__)
				return;
			break;
		case INPUT_BUFFER_12:
			if(dwInputIndex>=__MAX_IDBUUFER_SIZE__)
				return;
			if(GetAsyncKeyState(VK_SPACE) & 0x800000)
				return;
			break;
		case INPUT_BUFFER_2:
			if(dwInputIndex>=__MAX_PLAYERSHOP_MEMO__)
				return;
			
			if(GetAsyncKeyState(VK_RETURN) & 0x800000)
			{
				CPlayerShopWnd* pPlayerShopWnd = CPlayerShopWnd::GetInstance();
				
				if(pPlayerShopWnd->GetInputMode()==1)
					pPlayerShopWnd->SetInputMode(0, FALSE);

				return;
			}	
			break;
		case INPUT_BUFFER_PARTY_ENTRY:
			{
				if(dwInputIndex>=MAX_PARTYENTRY_TEXT)
					return;
			}
			break;
		}		

		if(bCh!=13)
		{
			g_pInputManager->InsertCharacter(g_pInputManager->GetCurFocusInputID(), (char*)&bCh, 1);			
		}		
	}	
}

void OnTimerEventDungeon(DWORD dwTimerIndex)
{
	switch(dwTimerIndex)
	{
	case 1:
		{
			g_pInputManager->BlinkInputCaret();
						
		}
		break;
	case 22:
		{
			//하나 지우고 
			if(g_pGVDungeon->bCurChatLine == 1)
			{
				g_pGVDungeon->bCurChatLine = 0;
				
				g_pInputManager->ClearInput(INPUT_BUFFER_1);
				KillTimer(g_hMainWnd, 22);
			}
			else if(g_pGVDungeon->bCurChatLine > 1)
			{
				//맨 먼저 온 채팅 메세지 지우고 하나씩 땅긴다.
				for(int i=0; i<g_pGVDungeon->bCurChatLine - 1; i++)
					memcpy(g_pGVDungeon->szChatDisplayBuf[ i ], g_pGVDungeon->szChatDisplayBuf[ i + 1 ],	MAX_CHAT_INPUT_LENGTH);	
					
				g_pGVDungeon->bCurChatLine--;

				//챗 버퍼 모두 지우고 				
				g_pInputManager->ClearInput(INPUT_BUFFER_1);
				int i = 0;
				//채팅 내용 이어 붙이기 
				for(i=0; i<g_pGVDungeon->bCurChatLine; i++) 
					g_pInputManager->InsertCharacter(INPUT_BUFFER_1, g_pGVDungeon->szChatDisplayBuf[ i ], lstrlen(g_pGVDungeon->szChatDisplayBuf[ i ]));
			}
		}
		break;
	}
}

void MouseEventDungeon()
{
	if (ItemPickupFiltering::sharedInstance()->isInterfaceFocused()) {
		return;
	}

	BYTE btMainPlayerStatus = g_pMainPlayer->GetStatus();
	if(	btMainPlayerStatus == UNIT_STATUS_DEAD ||
		btMainPlayerStatus == UNIT_STATUS_PLAYER_SHOP)
		return;

	CInterface*		pInterface		= CInterface::GetInstance();		
	CUserInterface*	pUserInterface	= CUserInterface::GetInstance();
	CGroupWnd*		pGroupWnd		= CGroupWnd::GetInstance();

	if(	g_bRButton==TRUE				||
		g_bBeltChk==TRUE				||
		g_bGuildChk==TRUE				||		
		g_bMagicChk==TRUE				||
		g_bPlayerShop==TRUE				||
		g_pMainPlayer->m_nItemSelect==1 ||
		pInterface->GetClick()			||
		pUserInterface->m_bTrade==TRUE	|| 
		pGroupWnd->m_bPartyMove==TRUE	||
		pGroupWnd->m_bGuildMove==TRUE	||		
		pGroupWnd->m_bMessengerMove==TRUE ||
		pGroupWnd->m_bGuildWarMove==TRUE  )
		return;

	if( g_Mouse.bLDown == true && !g_bLshift )
	{
		// 마지막으로 Mouse이벤트를 처리한 시간을 기록한다.
		g_Mouse.dwLButtonDownTime = g_dwCurTick;	
		
		if( g_hHandle )
		{
			LPObjectDesc pObjDesc = (LPObjectDesc)g_pExecutive->GetData( g_hHandle );
			if( !pObjDesc )
			{
				goto lb_move;
			}
	   		
			// 픽킹된 오브젝트에대한 처리를 한다.
			switch( pObjDesc->bType )
			{
			case OBJECT_TYPE_MONSTER:		
				{
					// 몬스터가 잡혔다면.
					CMonster* pMonster = (CMonster*)pObjDesc->pInfo;
					if (!g_pMainPlayer->IsAttack(pMonster, FALSE))
						goto lb_move;

					CTDS_DUNGEON_ATTACK_USER_MON	Attack;
					Attack.dwMonsterIndex = pMonster->m_dwMonsterIndex;
					VECTOR3_TO_VECTOR2(pMonster->m_v3CurPos, Attack.v2MonsterPos)
					VECTOR3_TO_VECTOR2(g_pMainPlayer->m_v3CurPos, Attack.v2UserPos)
					g_pNet->SendMsg( (char*)&Attack, Attack.GetPacketSize(), SERVER_INDEX_ZONE );

					// 중복해서 메시지를 보내지 않기 위해서다.
					g_pMainPlayer->SetStatus(UNIT_STATUS_ATTACKING);		
				}
				break;

			case OBJECT_TYPE_PLAYER:
				{
					CUser* pUser = (CUser*)pObjDesc->pInfo;
					if (!g_pMainPlayer->IsAttack(pUser, FALSE))
						goto lb_move;

					CTDS_DUNGEON_ATTACK_USER_USER	Attack;
					Attack.dwUserIndex		= pUser->m_dwUserIndex;
					VECTOR3_TO_VECTOR2(pUser->m_v3CurPos, Attack.v2DefenseUserPos)
					VECTOR3_TO_VECTOR2(g_pMainPlayer->m_v3CurPos, Attack.v2OffenseUserPos)
					g_pNet->SendMsg( (char*)&Attack, Attack.GetPacketSize(), SERVER_INDEX_ZONE );
					
					g_pMainPlayer->SetStatus(UNIT_STATUS_ATTACKING);		// 중복해서 메시지를 보내지 않기 위해서다.
				}
				break;

			case OBJECT_TYPE_ITEM:
				{
				}
				break;

			default:
				break;
			}
		}
		else
		{	
lb_move:
			if( (g_pThisDungeon->IsStadium() &&	(TRUE == CGuildWarFinalSettingWnd::GetInstance()->GetActive() || TRUE == CGuildWarStatusWnd::GetInstance()->IsPrepareTime())) )				
			{
				//경기장이고 프리페어 타임이거나 경기장 소환 인터페이스 떠있으면 무브 못함
				return;
			}
			else if( (g_pMainPlayer->m_bMatching == TRUE && !CUserInterface::GetInstance()->IsAllUserPVPReady()) ) 
			{
				return;
			}
			else
			{
				// 핸들이 없으면 이동으로 때운다.
				g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_TYPE ] = 0;
				g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_INDEX ] = 0;
			
				if( g_pMainPlayer->GetStatus() == UNIT_STATUS_NORMAL ||
					g_pMainPlayer->GetStatus() == UNIT_STATUS_WALKING ||
					g_pMainPlayer->GetStatus() == UNIT_STATUS_RUNNING )
				{
					if( PathFindDungeon() )
					{
						
					#ifdef GM_TOOL
						if(!g_pMainPlayer->IsGMUser() || !g_pGVGm->bQuickMove)
							SetPathFindMove();
					#else	
						SetPathFindMove();
					#endif

					}
				}
			}
		}
	}

	// 마우스의 어떤것도 안눌린 상태이다.
	// 오토 타켓팅된 것을 검사해본다.
	if( g_pMainPlayer->GetSkillKind(0) != __SKILL_ATTACK__ )		// 만약 공격이 아니라면 취소한다.
	{
		g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_TYPE ]	= 0;
		g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_INDEX ]	= 0; 
	}

	switch( g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_TYPE ] )
	{
	case OBJECT_TYPE_PLAYER:
		{
			CUser* pAutoTargetUser = g_pUserHash->GetData( g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_INDEX ] );			
			if (g_pMainPlayer->IsAttack(pAutoTargetUser, TRUE))
			{
				CTDS_DUNGEON_ATTACK_USER_USER	Attack;
				Attack.dwUserIndex = pAutoTargetUser->m_dwUserIndex;
				VECTOR3_TO_VECTOR2(pAutoTargetUser->m_v3CurPos, Attack.v2DefenseUserPos)
				VECTOR3_TO_VECTOR2(g_pMainPlayer->m_v3CurPos, Attack.v2OffenseUserPos)
				g_pNet->SendMsg( (char*)&Attack, Attack.GetPacketSize(), SERVER_INDEX_ZONE );
				
				g_pMainPlayer->SetStatus(UNIT_STATUS_ATTACKING);		// 중복해서 메시지를 보내지 않기 위해서다.
			}
		}
		break;

	case OBJECT_TYPE_MONSTER:
		{
			CMonster* pAutoTargetMonster = g_pMonsterHash->GetData( g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_INDEX ] );
			if (g_pMainPlayer->IsAttack(pAutoTargetMonster, TRUE))
			{
				CTDS_DUNGEON_ATTACK_USER_MON	Attack;
				Attack.dwMonsterIndex = pAutoTargetMonster->m_dwMonsterIndex;
				VECTOR3_TO_VECTOR2(pAutoTargetMonster->m_v3CurPos, Attack.v2MonsterPos)
				VECTOR3_TO_VECTOR2(g_pMainPlayer->m_v3CurPos, Attack.v2UserPos)
				g_pNet->SendMsg( (char*)&Attack, Attack.GetPacketSize(), SERVER_INDEX_ZONE );

				g_pMainPlayer->SetStatus(UNIT_STATUS_ATTACKING);		// 중복해서 메시지를 보내지 않기 위해서다.
			}
		}
		break;

	default:
		break;
	}
}


void InitMainPlayer( DSTC_JOIN* pJoin )
{
	g_pThisDungeon = g_pDungeonTable->GetDungeonInfo(pJoin->wMapID);

	if (DUNGEON_TYPE_EVENT == g_pThisDungeon->GetDungeonType())
	{
		g_pMainPlayer->m_bInEventDungeon				= TRUE;
		g_pMainPlayer->m_bInEvnetDungeonFirstTimeOut	= FALSE;
	}

	g_pMainPlayer->m_dwUserIndex		= pJoin->dwUserIndex;
	g_pMainPlayer->m_bCurLayer			= pJoin->bLayer;
		
	if (g_pThisDungeon->m_bSiege)
		g_pMainPlayer->SetSiegePKCount(pJoin->wSiegePKCount);
	g_pMainPlayer->m_wCriminalTime		= pJoin->wCriminal;
	
	g_pMainPlayer->DungeonOwnerEffect();
	g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
	g_pMainPlayer->m_bMoveType			= UNIT_STATUS_RUNNING;
	
	g_pUserHash->Insert( g_pMainPlayer, g_pMainPlayer->m_dwUserIndex );	//유저 인덱스 번호로 키지정후 삽입.

	g_pMainPlayer->SetSkillChangeLR(pJoin->bySelectedSkill[0], 0);
	g_pMainPlayer->SetSkillChangeLR(pJoin->bySelectedSkill[1], 1);
	g_pMainPlayer->m_i64PickupItem		= 0;

	// 나의 위치에따라 현재 속해있는 섹션 번호를 넣는다.
	VECTOR2_TO_VECTOR3(pJoin->v2CurPos, g_pMainPlayer->m_v3CurPos)
	g_pMainPlayer->m_pCurTile			= g_pMap->GetTile( pJoin->v2CurPos.x, pJoin->v2CurPos.y );
	g_pMainPlayer->m_byGuildFlag		= pJoin->byGuildFlag;

	if(g_pMainPlayer->m_hPlayerFlag.pHandle)
	{
		g_pExecutive->GXODetach(g_pMainPlayer->m_hPlayer.pHandle, g_pMainPlayer->m_hPlayerFlag.pHandle);

		DeleteHandleObject(g_pMainPlayer->m_hPlayerFlag.pHandle);
		g_pMainPlayer->m_hPlayerFlag.pHandle = NULL;
	}

	if(g_pMainPlayer->m_byGuildFlag)
	{		
		g_pMainPlayer->m_hPlayerFlag.pDesc		= AllocObjDesc();
		// sung-han 2005-02-14 길드전 혜택중 길드 깃발
		switch(g_pMainPlayer->m_byGuildFlag)
		{
		case 1:
			{
				g_pMainPlayer->m_hPlayerFlag.pHandle	= CreateHandleObject(GetFile("GuildFlag.chr", DATA_TYPE_ITEM)
					, GXPlayerPROC, g_pMainPlayer->m_hPlayerFlag.pDesc, NULL);
			}
			break;
		case 2:
			{
				g_pMainPlayer->m_hPlayerFlag.pHandle	= CreateHandleObject(GetFile("GuildFlag2.chr", DATA_TYPE_ITEM)
					, GXPlayerPROC, g_pMainPlayer->m_hPlayerFlag.pDesc, NULL);
			}
			break;
		case 3:
			{
				g_pMainPlayer->m_hPlayerFlag.pHandle	= CreateHandleObject(GetFile("GuildFlag3.chr", DATA_TYPE_ITEM)
					, GXPlayerPROC, g_pMainPlayer->m_hPlayerFlag.pDesc, NULL);
			}
			break;
		case 4:
			{
				g_pMainPlayer->m_hPlayerFlag.pHandle	= CreateHandleObject(GetFile("GuildFlag4.chr", DATA_TYPE_ITEM)
					, GXPlayerPROC, g_pMainPlayer->m_hPlayerFlag.pDesc, NULL);
			}
			break;
		case 5:
			{
				g_pMainPlayer->m_hPlayerFlag.pHandle	= CreateHandleObject(GetFile("GuildFlag5.chr", DATA_TYPE_ITEM)
					, GXPlayerPROC, g_pMainPlayer->m_hPlayerFlag.pDesc, NULL);
			}
			break;
		}

		if(g_pMainPlayer->m_hPlayerFlag.pHandle)		
			g_pExecutive->GXOAttach(g_pMainPlayer->m_hPlayerFlag.pHandle, g_pMainPlayer->m_hPlayer.pHandle, "Bip01 Spine3");
	}
	
	GXSetPosition( g_pMainPlayer->m_hPlayer.pHandle, &g_pMainPlayer->m_v3CurPos, TRUE , TRUE);
	
	// 서버에서 받아서 처리할수도 있는 구문이다.
	DWORD	x, z, i;
	Section_Link_Info*	pSection;

	x = (DWORD)( pJoin->v2CurPos.x / TILE_WIDTH );
	z = (DWORD)( pJoin->v2CurPos.y / TILE_HEIGHT );

	for( i=0; i<g_pMap->m_wTotalSectionMany; i++)
	{
		pSection = &g_pMap->m_Section[i];
		if( x >= pSection->x1 && x <= pSection->x2 && z >= pSection->y1 && z <= pSection->y2 )
		{
			g_pMainPlayer->m_wCurSectionNum = pSection->wSectionNum;
			break;
		}
	}

	//--- 여기서 바디및 여러 인텍스 번호에따라.. 모델파일을 알아오는 코드 삽입.
	
	// 임시.. 오른손.. 왼손//

	if (g_dwLayerID < 100 )	// 던전은 아니므로
		g_pMainPlayer->SetAction( MOTION_TYPE_VILLAGESTAND, 0, ACTION_LOOP );		
	else
		g_pMainPlayer->SetAction( MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );

	InitChrInfo( GetFile( "pm01000.cdt", DATA_TYPE_CDT ), 0, OBJECT_TYPE_PLAYER );
	
	LIGHT_DESC	shadowlight;

	shadowlight.dwDiffuse = 0xffffffff;
	shadowlight.v3Point.x = g_pMainPlayer->m_v3CurPos.x + 500.0f;
	shadowlight.v3Point.y = 800.0f;
	shadowlight.v3Point.z = g_pMainPlayer->m_v3CurPos.z - 600.0f;
	shadowlight.fFov = PI/2.0f;
	shadowlight.v3Up.x = 0.0f;
	shadowlight.v3Up.y = 1.0f;
	shadowlight.v3Up.z = 0.0f;

	shadowlight.fRs = 2000.0f;
	shadowlight.v3To = g_pMainPlayer->m_v3CurPos;
	
	
	g_pMainPlayer->m_hShadowLightHandle = g_pExecutive->CreateGXLight(&shadowlight, NULL, NULL, 0, NULL
		, GXLIGHT_TYPE_ENABLE_SHADOW | GXLIGHT_TYPE_DISABLE_LIGHT_COLOR);

	g_pExecutive->GXLEnableDynamicLight(g_pMainPlayer->m_hShadowLightHandle);
	
	VECTOR3	v3Offset = { 500.f, 1000.f, -300.f };
	g_pExecutive->GXOAttachLight(g_pMainPlayer->m_hPlayer.pHandle,"Bip01",g_pMainPlayer->m_hShadowLightHandle,&v3Offset,ATTACH_TYPE_LINK);	

	InitDungeonCamera(); // ResetCamera.

	VECTOR3 v3Tmp = 
	{
		g_pMainPlayer->m_v3CurPos.x + g_Camera.fCameraDistance_x,
		g_pMainPlayer->m_v3CurPos.y + g_Camera.fCameraDistance_y,
		g_pMainPlayer->m_v3CurPos.z + g_Camera.fCameraDistance_z
	};
	
	g_pGeometry->SetCameraPos( &v3Tmp, 0 );

	VECTOR3	v3ZoomPos;
	g_pExecutive->GXOGetPosition( g_pMainPlayer->m_hPlayer.pHandle, &v3ZoomPos );

	if(g_nDGZoom==__MAX_ZOOM_DEFAULT__)
		g_nDGZoom	= __MAX_ZOOM_DEFAULT__;
	else
	{
		if(g_nDGZoom<__MAX_ZOOM_DEFAULT__)
		{
			for(int i = g_nDGZoom; i < __MAX_ZOOM_DEFAULT__; i++)
				SetCameraPos( &v3ZoomPos, CAMERA_DISTANCE );
		}
		else
		{
			for(int i = __MAX_ZOOM_DEFAULT__; i < g_nDGZoom; i++)
				SetCameraPos( &v3ZoomPos, CAMERA_DISTANCE * -1.f );
		}
	}

	g_iCameraX			= __DEFAULT_CAMERA_X__;

	if(!g_pMainPlayer->m_pBaloonChat.pSpr)
	{
		g_pMainPlayer->m_pBaloonChat.pSpr	= g_pRenderer->CreateSpriteObject(FILE(SPR_SPEAKING_BOX), 0);
	}

	if (!g_MatchInfo.GetBackBoardSprite())
	{		
		g_MatchInfo.SetBackBoardSprite(g_pRenderer->CreateSpriteObject(FILE(SPR_SPEAKING_BOX), 0));
	}
	
	g_pMainPlayer->m_hShadowHandle		= CreateHandleObject(FILE(MOD_CHARACTER_SHADOW), NULL, NULL, GXOBJECT_CREATE_TYPE_EFFECT);	
	GXSetPosition(g_pMainPlayer->m_hShadowHandle, &g_pMainPlayer->m_v3CurPos, FALSE);
			
	if(CGameMenuWnd::GetInstance()->m_bShadowFlag==TRUE)
		HideObject(g_pMainPlayer->m_hShadowHandle);
	
	// 공성 정보	
	CDungeonSiegeInfoWnd::GetInstance()->SetActive(g_pThisDungeon->m_bSiege);
	CDungeonSiegeInfoWnd::GetInstance()->m_iType = SIEGEINFOWND_TYPE_SIEGE;

	InitSearchModule(g_pMap);
	g_pMainPlayer->CreateResource();
	
	if(g_pMainPlayer->m_bCurrnetHand==1)
	{
		g_pMainPlayer->m_wHandR = g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID;
		g_pMainPlayer->m_wHandL = g_pMainPlayer->m_pEquip[EQUIP_TYPE_LHAND1].m_wItemID;
	}
	else
	{
		g_pMainPlayer->m_wHandR = g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID;
		g_pMainPlayer->m_wHandL = g_pMainPlayer->m_pEquip[EQUIP_TYPE_LHAND2].m_wItemID;
	}
	
	g_pMainPlayer->m_wArmor = g_pMainPlayer->m_pEquip[EQUIP_TYPE_ARMOR].m_wItemID;
	g_pMainPlayer->m_wHelmet = g_pMainPlayer->m_pEquip[EQUIP_TYPE_HELMET].m_wItemID;
	
	g_pMainPlayer->m_dwTemp[USER_TEMP_COMMANDGUARDIAN] = 0xffffffff;

	MinimizePlayer(g_pMainPlayer->m_hPlayer.pHandle, FALSE);

	if (IsPKZone())
	{
		// "PK 가능지역입니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE850].szMessage, TEXT_COLOR_ORANGE); 
	}
	else
	{
		CUserInterface::GetInstance()->SetPKMode(FALSE);
	}
}


void InitMainPlayer( DSTC_CHANGE_LAYER* pLayer )
{	
	g_pMainPlayer->m_bMoveType		= UNIT_STATUS_WALKING;	
	g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL, TRUE);
	g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;
	g_pMainPlayer->m_hPlayer.pDesc->CrashFunc = NULL;
	g_pMainPlayer->SetAction( MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );	
	g_pMainPlayer->m_bCurLayer		= pLayer->bCurLayerIndex;
	g_pMainPlayer->m_i64PickupItem	= 0;
	
	VECTOR2_TO_VECTOR3(pLayer->v2CurPos, g_pMainPlayer->m_v3CurPos)
	g_pMainPlayer->m_pCurTile		= g_pMap->GetTile( pLayer->v2CurPos.x, pLayer->v2CurPos.y );
	g_pMainPlayer->SetAttackMode(pLayer->bAttackMode);
	g_pMainPlayer->UserObjectAlpha(255);
	
	if( g_pExecutive->IsValidHandle(g_pMainPlayer->m_hPlayer.pHandle)	== GX_MAP_OBJECT_TYPE_INVALID)		asm_int3();
	if( g_pExecutive->IsValidHandle(g_pMainPlayer->m_hPlayerHead.pHandle)	== GX_MAP_OBJECT_TYPE_INVALID)	asm_int3();

	SetUserPosition(g_pMainPlayer, &g_pMainPlayer->m_v3CurPos);	

	DWORD	x, z;
	Section_Link_Info*	pSection;

	x = (DWORD)( pLayer->v2CurPos.x / TILE_WIDTH );
	z = (DWORD)( pLayer->v2CurPos.y / TILE_HEIGHT );

	for( int i=0; i<g_pMap->m_wTotalSectionMany; i++)
	{
		pSection = &g_pMap->m_Section[i];
		if( x >= pSection->x1 && x <= pSection->x2 && z >= pSection->y1 && z <= pSection->y2 )
		{
			g_pMainPlayer->m_wCurSectionNum = pSection->wSectionNum;
			break;
		}
	}
	
	CDungeonSiegeInfoWnd::GetInstance()->SetActive(g_pThisDungeon->m_bSiege);
	CDungeonSiegeInfoWnd::GetInstance()->m_iType = SIEGEINFOWND_TYPE_SIEGE;
	
	VECTOR3 v3Tmp = {
		g_pMainPlayer->m_v3CurPos.x + g_Camera.fCameraDistance_x,
		g_pMainPlayer->m_v3CurPos.y + g_Camera.fCameraDistance_y,
		g_pMainPlayer->m_v3CurPos.z + g_Camera.fCameraDistance_z };
	
	g_pGeometry->SetCameraPos( &v3Tmp, 0 );

	if (g_dwLayerID < 100 )	// 던전은 아니므로
		g_pMainPlayer->SetAction( MOTION_TYPE_VILLAGESTAND, 0, ACTION_LOOP );		
	else
		g_pMainPlayer->SetAction( MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );	

	delete g_pSw;
	g_pSw = NULL;
	InitSearchModule(g_pMap);
	g_pMainPlayer->CreateResource();
	if (g_pThisDungeon->m_bSiege)
		g_pMainPlayer->SetSiegePKCount(g_pMainPlayer->m_wSiegePKCount);
	g_pMainPlayer->DungeonOwnerEffect();

	if (IsPKZone())
		DisplayMessageAdd(g_Message[ETC_MESSAGE850].szMessage, TEXT_COLOR_ORANGE); // "PK 가능지역입니다."
	else
		CUserInterface::GetInstance()->SetPKMode(FALSE);
}


CUser* InitPlayer( DSTC_APPEAR* pAppear )
{	
	CUser*				pPlayer		= (CUser*)LALAlloc( g_pUserPool );	
	CBaseItem*			pItemTable	= 0;
	Section_Link_Info*	pSection	= 0;
	
	memset(pPlayer, 0, sizeof(CUser));
	pPlayer->CreateResource();
	__lstrcpyn(pPlayer->m_szName, pAppear->szName, MAX_CHARACTER_NAME_REAL_LENGTH);
	
	if(pAppear->dwUserIndex==0)
		asm_int3();
	
	if(pAppear->dwGuildId!=0)
	{
		LP_GUILD_DATA lpGuildData = g_pGuildInfoHash->GetData(pAppear->dwGuildId);

		if(!lpGuildData)
		{
			CTWS_GUILD_DATA_REQUEST pSendPacket;
			pSendPacket.dwGuildId	= pAppear->dwGuildId;
			g_pNet->SendMsg((char*)&pSendPacket, pSendPacket.GetPacketSize(), SERVER_INDEX_WORLD);
		}
	}

	__lstrcpyn(pPlayer->m_szNickName, pAppear->szNickName, MAX_NICK_REAL_NAME);
	pPlayer->m_dwUserIndex			= pAppear->dwUserIndex;
	pPlayer->SetStatus(UNIT_STATUS_NORMAL);
	
	g_pUserHash->Insert(pPlayer, pPlayer->m_dwUserIndex);	//유저 인덱스 번호로 키지정후 삽입.
	
	VECTOR2_TO_VECTOR3(pAppear->v2CurPos, pPlayer->m_v3CurPos)
	VECTOR2_TO_VECTOR3(pAppear->v2Direction, pPlayer->m_v3Direction)
	pPlayer->m_wClass			= pAppear->wClass; 
	pPlayer->m_byGuildFlag			= pAppear->byGuildFlag;
	
	if(pPlayer->m_v3CurPos.x <= 0 || pPlayer->m_v3CurPos.z <= 0)
		asm_int3();

	// 유저가 속해있는 섹션을 구한다.	
	pPlayer->m_pCurTile	= g_pMap->GetTile( pAppear->v2CurPos.x, pAppear->v2CurPos.y );

	for(int i = 0; i < g_pMap->m_wTotalSectionMany; i++)
	{
		pSection = &g_pMap->m_Section[i];

		if( pPlayer->m_pCurTile->wIndex_X >= pSection->x1 && pPlayer->m_pCurTile->wIndex_X <= pSection->x2 && 
			pPlayer->m_pCurTile->wIndex_Z >= pSection->y1 && pPlayer->m_pCurTile->wIndex_Z <= pSection->y2 )
		{
			pPlayer->m_wCurSectionNum = pSection->wSectionNum;
			break;
		}
	}
	
	//--- 여기서 바디및 여러 인텍스 번호에따라.. 모델파일을 알아오는 코드 삽입.
	pPlayer->m_hPlayer.pDesc = AllocObjDesc();
	memset(pPlayer->m_hPlayer.pDesc, 0, sizeof(OBJECT_DESC));
	
	pPlayer->m_hPlayer.pDesc->bType			= OBJECT_TYPE_PLAYER;
	pPlayer->m_hPlayer.pDesc->pInfo			= (void*)pPlayer;		// 케릭터 클래스의 주소 입력.
	pPlayer->m_hPlayer.pDesc->ObjectFunc	= NULL;	

	// 플레이어에 몸통 //
	if(!pPlayer->m_hPlayer.pHandle)
	{
		if(pAppear->wArmor==0)
		{
			pPlayer->m_hPlayer.pHandle = CreateHandleObject(GetFile(RESTYPE_BASE_BODY, pAppear->wClass), GXPlayerPROC, pPlayer->m_hPlayer.pDesc, NULL);
		}
		else
		{
			LP_ITEM_RESOURCE_EX lpItemResource = g_pItemResourceHash->GetData(pAppear->wArmor);

			if(lpItemResource)
			{
				if(lpItemResource->wModCount>0)
				{
					char szInfo[0xff] = {0,};

					ItemDataName(szInfo, pAppear->wArmor, pPlayer->m_wClass-1);
					pPlayer->m_hPlayer.pHandle = CreateHandleObject(GetFile(szInfo, DATA_TYPE_CHARACTER), GXPlayerPROC, pPlayer->m_hPlayer.pDesc, NULL);
				}
			}
		}			
	}
	
	// 플레이어에 머리 //	
	if(!pPlayer->m_hPlayerHead.pHandle)
	{
		pPlayer->m_hPlayerHead.pDesc	= pPlayer->m_hPlayer.pDesc;

		if( pAppear->wClass < 4 )
		{
			pPlayer->m_hPlayerHead.pHandle = 
			CreateHandleObject( GetFile( RESTYPE_HEAD_MALE, pAppear->wHead ), NULL, pPlayer->m_hPlayerHead.pDesc, 0);			
		}
		else
		{
			pPlayer->m_hPlayerHead.pHandle = 
			CreateHandleObject( GetFile( RESTYPE_HEAD_FEMALE, pAppear->wHead ), NULL, pPlayer->m_hPlayerHead.pDesc, 0);
		}
		g_pExecutive->GXOAttach(pPlayer->m_hPlayerHead.pHandle, pPlayer->m_hPlayer.pHandle, "Bip01 Head" );
	}
	
	// 플레이어에 헬멧 //
	if(!pPlayer->m_hPlayerHelmet.pHandle)
		ItemAttach(pAppear->wHelmet, &pPlayer->m_hPlayerHelmet.pHandle, NULL, &pPlayer->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_HEAD);
	
	// 플레이어에 오른손 //
	if(pAppear->wHandR!=0)
	{
		if(!pPlayer->m_hPlayerRHand.pHandle)
			ItemAttach(pAppear->wHandR, &pPlayer->m_hPlayerRHand.pHandle, pPlayer->m_hPlayerRHand.pDesc, &pPlayer->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_RHAND);
		
		pItemTable = g_pItemTableHash_get()->GetData(pAppear->wHandR);

		if(pItemTable)
		{
			if(pItemTable->BaseItem_Weapon.bModCount==2)
			{
				if(!pPlayer->m_hPlayerLHand.pHandle)
					ItemAttach(pAppear->wHandR, &pPlayer->m_hPlayerLHand.pHandle, pPlayer->m_hPlayerLHand.pDesc, &pPlayer->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_LHAND);				
			}		
			pPlayer->m_byItemType = BYTE((pAppear->wHandR/ITEM_DISTRIBUTE)+1);
		}
	}
	
	// 플레이어에 왼손 //
	if(pAppear->wHandL!=0)
	{
		if(!pPlayer->m_hPlayerLHand.pHandle)
			ItemAttach(pAppear->wHandL, &pPlayer->m_hPlayerLHand.pHandle, pPlayer->m_hPlayerLHand.pDesc, &pPlayer->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_LHAND);
	}

	if(pPlayer->m_byGuildFlag)
	{
		if(pPlayer->m_hPlayerFlag.pHandle)			
			ItemDeleteObject(&pPlayer->m_hPlayerFlag.pHandle, &pPlayer->m_hPlayer.pHandle);		

		// sung-han 2005-02-14 길드전 혜택중 길드 깃발
		switch(pPlayer->m_byGuildFlag)
		{
		case 1:
			{
				ItemAttach(0, &pPlayer->m_hPlayerFlag.pHandle, pPlayer->m_hPlayerFlag.pDesc, &pPlayer->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_FLAG, 0, 0, GetFile("GuildFlag.chr", DATA_TYPE_ITEM));
			}
			break;
		case 2:
			{
				ItemAttach(0, &pPlayer->m_hPlayerFlag.pHandle, pPlayer->m_hPlayerFlag.pDesc, &pPlayer->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_FLAG, 0, 0, GetFile("GuildFlag2.chr", DATA_TYPE_ITEM));
			}
			break;
		case 3:
			{
				ItemAttach(0, &pPlayer->m_hPlayerFlag.pHandle, pPlayer->m_hPlayerFlag.pDesc, &pPlayer->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_FLAG, 0, 0, GetFile("GuildFlag3.chr", DATA_TYPE_ITEM));
			}
			break;
		case 4:
			{
				ItemAttach(0, &pPlayer->m_hPlayerFlag.pHandle, pPlayer->m_hPlayerFlag.pDesc, &pPlayer->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_FLAG, 0, 0, GetFile("GuildFlag4.chr", DATA_TYPE_ITEM));
			}
			break;
		case 5:
			{
				ItemAttach(0, &pPlayer->m_hPlayerFlag.pHandle, pPlayer->m_hPlayerFlag.pDesc, &pPlayer->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_FLAG, 0, 0, GetFile("GuildFlag5.chr", DATA_TYPE_ITEM));
			}
			break;
		}
	}
	
	g_pExecutive->GXOSetDirection(pPlayer->m_hPlayer.pHandle, &g_Camera.v3AxsiY, (float)(atan2(pPlayer->m_v3Direction.z, pPlayer->m_v3Direction.x) + DEG90 ) );

	pPlayer->m_wArmor	= pAppear->wArmor;
	pPlayer->m_wHandR	= pAppear->wHandR;
	pPlayer->m_wHandL	= pAppear->wHandL;	
	pPlayer->m_wHelmet	= pAppear->wHelmet;
	pPlayer->m_byItemType = BYTE((pAppear->wHandR==0) ? 0 : (pAppear->wHandR/ITEM_DISTRIBUTE)+1);
	
	pPlayer->SetAction( MOTION_TYPE_WARSTAND, g_pExecutive->GXOGetCurrentFrame( pPlayer->m_hPlayer.pHandle ), ACTION_LOOP );
	pPlayer->m_hPlayer.pDesc->ObjectFunc	= NULL;				
	pPlayer->SetStatus(UNIT_STATUS_NORMAL);
		
	pPlayer->SetAttackMode(pAppear->bAttackMode);
	pPlayer->m_dwPartyId = pAppear->dwPartyID;
	pPlayer->m_dwGuildId = pAppear->dwGuildId;
	pPlayer->m_dwGuildWarFlag = pAppear->dwGuildWarFlag;// : hwoarang 050202 
	pPlayer->m_team_index = pAppear->team_index;//팀 인덱스 : hwoarang 050202 

	LP_PARTY_USER sPartyNode = g_pPartyUserHash->GetData( pPlayer->m_dwUserIndex );

	if(sPartyNode)
	{
		CMiniMapWnd* pMiniMapWnd = CMiniMapWnd::GetInstance();

		VECTOR2 vec = pMiniMapWnd->GetRatioMinimap(pPlayer->m_v3CurPos.x, pPlayer->m_v3CurPos.z);
		
		pMiniMapWnd->SetPartyPtr(pMiniMapWnd->GetPartyPtrRt(sPartyNode->dwResourceId), 1);			

		if(pMiniMapWnd->m_bInit==TRUE)	
		{
			pMiniMapWnd->SetPosObj(sPartyNode->dwResourceId, pMiniMapWnd->m_fPosX+vec.x+8, pMiniMapWnd->m_fPosZ+vec.y+42);			
			pMiniMapWnd->SetRender(sPartyNode->dwResourceId, TRUE);
		}			
	}	
	
	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();
	pGroupWnd->SetPartyUI();

	if (g_pThisDungeon->m_bSiege)
		pPlayer->SetSiegePKCount(pAppear->wSiegePKCount);
	pPlayer->DungeonOwnerEffect();
	pPlayer->m_wCriminalTime = pAppear->wCriminalTime;
	
	//-- 기타 기초적으로 필요한 이펙트를 로드한다. -- //
	pPlayer->m_hMoveStartEffect.pDesc	= AllocObjDesc();
	pPlayer->m_hMoveStopEffect.pDesc	= AllocObjDesc();
	pPlayer->m_hPlayerShop.pDesc		= AllocObjDesc();
	
	pPlayer->m_hMoveStartEffect.pDesc->bType		= OBJECT_TYPE_EFFECT;
	pPlayer->m_hMoveStartEffect.pDesc->pInfo		= (void*)pPlayer;
	pPlayer->m_hMoveStartEffect.pDesc->ObjectFunc	= NULL;
	pPlayer->m_hMoveStopEffect.pDesc->bType			= OBJECT_TYPE_EFFECT;
	pPlayer->m_hMoveStopEffect.pDesc->pInfo			= (void*)pPlayer;
	pPlayer->m_hMoveStopEffect.pDesc->ObjectFunc	= NULL;
	pPlayer->m_hPlayerShop.pDesc->bType				= OBJECT_TYPE_EFFECT;
	pPlayer->m_hPlayerShop.pDesc->pInfo				= (void*)pPlayer;
	pPlayer->m_hPlayerShop.pDesc->ObjectFunc		= NULL;

	// 달릴때 시작 이펙트와 정지 이펙트.
	pPlayer->m_hMoveStartEffect.pHandle = CreateHandleObject( g_pObjManager->GetFile( SKILL_DUST1 ), GXPlayerPROC, pPlayer->m_hMoveStartEffect.pDesc, 0 );	
	pPlayer->m_hMoveStopEffect.pHandle	= CreateHandleObject( g_pObjManager->GetFile( SKILL_DUST2 ), GXPlayerPROC, pPlayer->m_hMoveStopEffect.pDesc, 0 );	
	pPlayer->m_hPlayerShop.pHandle		= CreateHandleObject( GetFile("playershop_sign.chr", DATA_TYPE_ITEM), GXPlayerPROC, pPlayer->m_hPlayerShop.pDesc, 0 );
	
	HideObject( pPlayer->m_hMoveStartEffect.pHandle );
	HideObject( pPlayer->m_hMoveStopEffect.pHandle );
	HideObject( pPlayer->m_hPlayerShop.pHandle );

	InitChrInfo( GetFile( "pm01000.cdt", DATA_TYPE_CDT ), 0, OBJECT_TYPE_PLAYER );

	pPlayer->m_pBaloonChat.pSpr = g_pRenderer->CreateSpriteObject(FILE(SPR_SPEAKING_BOX), 0);
	pPlayer->m_hShadowHandle	= CreateHandleObject(FILE(MOD_CHARACTER_SHADOW), NULL, NULL, GXOBJECT_CREATE_TYPE_EFFECT);		

	// Self Portal Effect Create
	pPlayer->m_hSelfPortalEffect.pDesc = AllocObjDesc();
	memset(pPlayer->m_hSelfPortalEffect.pDesc, 0, sizeof(OBJECT_DESC));
	pPlayer->m_hSelfPortalEffect.pDesc->bType = OBJECT_TYPE_EFFECT;
	pPlayer->m_hSelfPortalEffect.pDesc->pInfo = (void*)pPlayer;
	pPlayer->m_hSelfPortalEffect.pHandle = CreateHandleObject( FILE(MOD_SELF_PORTAL_EFFECT) , GXPlayerPROC, 
													pPlayer->m_hSelfPortalEffect.pDesc, NULL);
	HideObject(pPlayer->m_hSelfPortalEffect.pHandle);
	
	SetUserPosition( pPlayer, &pPlayer->m_v3CurPos );

	if(CGameMenuWnd::GetInstance()->m_bShadowFlag==TRUE)
		HideObject(pPlayer->m_hShadowHandle);	

	g_dwUserNum++;
	return pPlayer;
}


void InitMonster( DSTC_APPEAR_MON* pAppear )
{
	CMonster* pMonster = (CMonster*)LALAlloc( g_pMonsterPool );
	memset( pMonster, 0, sizeof( CMonster ) );
	pMonster->CreateResource();
	
	pMonster->m_dwLevel			= pAppear->dwLevel;
	pMonster->m_dwMonsterIndex	= pAppear->dwMonsterIndex;
	pMonster->m_bClass			= pAppear->bClass;
	pMonster->m_fSpeed			= pAppear->fSpeed;
	pMonster->m_dwHP			= pAppear->dwHP;
	pMonster->m_dwMaxHP			= pAppear->dwMaxHP;
	pMonster->m_dwMonsterKind	=	pAppear->bMonsterKind;

	if(pMonster->m_dwMonsterKind==OBJECT_TYPE_GUARDIAN)
	{		
		CUser* pUser = g_pUserHash->GetData(pAppear->dwLordIndex);

		if(!pUser)
		{
			__lstrcpyn( pMonster->m_szName
				, pAppear->szMonsterName
				, sizeof(pAppear->szMonsterName));
		}
		else
		{
			// sung-han 2005-01-13 "누구누구의 가디언" -> "누구누구의" 글자 제외
			//--------------------------------------------------------------------
			__lstrcpyn(pMonster->m_szName
				, pAppear->szMonsterName
				, sizeof(pAppear->szMonsterName) );
			/*
			__lstrcpyn(pMonster->m_szName
				, pUser->m_szName
				, MAX_CHARACTER_NAME_LENGTH );
			
			lstrcat(pMonster->m_szName
				, g_Message[ETC_MESSAGE762].szMessage); // "의 "	
			
			strncat(pMonster->m_szName
				, pAppear->szMonsterName
				, sizeof(pAppear->szMonsterName) );
			*/
			//--------------------------------------------------------------------
		}				
		
	}
	else
	{
		__lstrcpyn( pMonster->m_szName
			, pAppear->szMonsterName
			, sizeof(pAppear->szMonsterName));
	}

	if( g_pMonsterHash->Insert( pMonster, pMonster->m_dwMonsterIndex ) == -1 ) 
		asm_int3();

	VECTOR2_TO_VECTOR3(pAppear->v2CurPos, pMonster->m_v3CurPos)
	
	// 몬스터가 속해있는 섹션을 구한다.
	DWORD	x, z, i;
	Section_Link_Info*	pSection;
	x = (DWORD)(pMonster->m_v3CurPos.x / TILE_WIDTH);
	z = (DWORD)(pMonster->m_v3CurPos.z / TILE_HEIGHT);
	pMonster->m_pCurTile = g_pMap->GetTile( pAppear->v2CurPos.x, pAppear->v2CurPos.y );

	for( i=0; i<g_pMap->m_wTotalSectionMany; i++)
	{
		pSection = &g_pMap->m_Section[i];
		if( x >= pSection->x1 
			&& x <= pSection->x2 
			&& z >= pSection->y1 
			&& z <= pSection->y2 )
		{
			pMonster->m_wCurSectionNum = pSection->wSectionNum;
			break;
		}
	}

	//--- 여기서 바디및 여러 인텍스 번호에따라.. 모델파일을 알아오는 코드 삽입.
	pMonster->m_hMonster.pDesc = AllocObjDesc();
	memset(pMonster->m_hMonster.pDesc, 0, sizeof(OBJECT_DESC));
	
	if (pAppear->bMonsterKind == OBJECT_TYPE_TRADER_MONSTER)
		pMonster->m_hMonster.pDesc->bType = OBJECT_TYPE_TRADER_MONSTER;
	else
		pMonster->m_hMonster.pDesc->bType = OBJECT_TYPE_MONSTER;

	pMonster->m_hMonster.pDesc->pInfo = (void*)pMonster;		// 몬스터 클래스의 주소 입력.
	pMonster->WithActionFunc = NULL;
	
	char szBuf[100] = {0,};
	wsprintf( szBuf, "m%05d.chr", pAppear->wMonNo );
	
	pMonster->m_hMonster.pHandle = NULL;
	pMonster->m_hMonster.pHandle = CreateHandleObject( GetFile( szBuf, DATA_TYPE_MONSTER )
		, GXPlayerPROC, pMonster->m_hMonster.pDesc, 0);

	// == //
	switch(pAppear->wMonNo)
	{
	case 20:
	case 21:		
	case 30:
	case 80:
	case 81:
	case 100:
	case 220:
	case 221:
	case 222:
	case 223:
		pMonster->m_hShadowHandle = CreateHandleObject(FILE(MOD_MONSTER_SHADOW1), NULL, NULL, GXOBJECT_CREATE_TYPE_EFFECT);
		break;
	case 50:
	case 60:
	case 61:
	case 90:
	case 150:
	case 151:
	case 170:
	case 180:
	case 181:
	case 190:
	case 200:
	case 210:
	case 5030:		
	case 5060:
	case 5100:
		pMonster->m_hShadowHandle = CreateHandleObject(FILE(MOD_MONSTER_SHADOW2), NULL, NULL, GXOBJECT_CREATE_TYPE_EFFECT);
		break;
	case 10:
	case 40:
	case 70:
	case 110:
	case 111:
	case 112:
	case 120:
	case 140:
	case 160:
	case 161:
		pMonster->m_hShadowHandle = CreateHandleObject(FILE(MOD_MONSTER_SHADOW3), NULL, NULL, GXOBJECT_CREATE_TYPE_EFFECT);
		break;
	case 130:
	case 5040:
		pMonster->m_hShadowHandle = CreateHandleObject(FILE(MOD_MONSTER_SHADOW4), NULL, NULL, GXOBJECT_CREATE_TYPE_EFFECT);
		break;
	default:
		pMonster->m_hShadowHandle = CreateHandleObject(FILE(MOD_MONSTER_SHADOW1), NULL, NULL, GXOBJECT_CREATE_TYPE_EFFECT);
		break;
	}

	CGameMenuWnd* pGameMenuWnd = CGameMenuWnd::GetInstance();	

	if(pGameMenuWnd->m_bShadowFlag==TRUE)
		HideObject(pMonster->m_hShadowHandle);

	// 만약 몬스터 부르는것이 실패 했다면 뻑나지 않게 발록으로.
	if( !pMonster->m_hMonster.pHandle )
	{    
		wsprintf( szBuf, "m%05d.chr", 10 );
		pMonster->m_hMonster.pHandle = 
		CreateHandleObject( GetFile( szBuf, DATA_TYPE_MONSTER ), GXPlayerPROC, pMonster->m_hMonster.pDesc, 0);
		
		wsprintf( szBuf, "m%05d.cdt", 10 );
		InitChrInfo( GetFile( szBuf, DATA_TYPE_CDT ), 10, OBJECT_TYPE_MONSTER );

		pMonster->m_wModNo = 10;
	}
	else
	{
		wsprintf( szBuf, "m%05d.cdt", pAppear->wMonNo );
		InitChrInfo( GetFile( szBuf, DATA_TYPE_CDT ), pAppear->wMonNo, OBJECT_TYPE_MONSTER );
	}

	g_pExecutive->EnablePick(pMonster->m_hMonster.pHandle);
	g_pExecutive->SetAlphaFlag(pMonster->m_hMonster.pHandle, 0xff);
	SetMonsterPosition( pMonster, &pMonster->m_v3CurPos );

	
	pMonster->SetAction( MON_MOTION_TYPE_STAND1, 0, ACTION_LOOP );
	pMonster->SetStatus(UNIT_STATUS_NORMAL, NULL);
	pMonster->m_dwLordIndex = pAppear->dwLordIndex;	
	pMonster->m_byBaseScale = pAppear->bScale;

	// 마지막으로 가디언이라면 주인 유저에게 연결해준다.( 지금은 일단 메인 케릭터에만. )
	if( pAppear->bMonsterKind == OBJECT_TYPE_GUARDIAN && pAppear->dwLordIndex == g_pMainPlayer->m_dwUserIndex )
	{		
		if( (DWORD)pAppear->dwLordIndex == g_pMainPlayer->m_dwUserIndex )
		{
			g_pMainPlayer->m_pGuardian[ pAppear->bZipCode ] = pMonster;
			pMonster->m_bZipCode	= pAppear->bZipCode;
			pMonster->m_dwLordIndex	= g_pMainPlayer->m_dwUserIndex;
			// Interface update		
			CUserInterface* pUserInterface = CUserInterface::GetInstance();
			pUserInterface->OpenGuardianDescriptionWnd(pMonster->m_dwMonsterIndex
				, pAppear->szMonsterName
				, pAppear->dwHP
				, pAppear->dwMaxHP);
		}
	}
	else if (!pAppear->dwLordIndex && pAppear->bMonsterKind == OBJECT_TYPE_GUARDIAN)
	{
		EffectDesc* pEffectDesc = g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(EFFECT_DUNGEON_GUARDIAN), FALSE, __CHR_EFFECT_NONE__);
		pEffectDesc->byTargetObjectType[0] = OBJECT_TYPE_MONSTER;
		pEffectDesc->dwTargetIndex[0] = pMonster->m_dwMonsterIndex;
		pEffectDesc->hEffect.pDesc->ObjectFunc = EffectSkillAroundFunc;
		::SetAction(pEffectDesc->hEffect.pHandle, 1, 0, ACTION_LOOP);
		pEffectDesc->pUsingStatus = pMonster->m_pUsingStatusEffectList->AddTail(pEffectDesc);		
	}
	else if( pAppear->bMonsterKind == OBJECT_TYPE_MONSTER && pAppear->dwLordIndex == g_pMainPlayer->m_dwUserIndex )
	{
		g_pMainPlayer->m_pMonster[ pAppear->bZipCode ]	= pMonster;
		pMonster->m_bZipCode							= pAppear->bZipCode;		
	}
	
	// 메인유저의 소유의 몬스터이면 표시를 해준다.
	if( pAppear->dwLordIndex == g_pMainPlayer->m_dwUserIndex )
	{
		pMonster->m_pEffectTarget = g_pEffectLayer->CreateGXObject(GetFile( "e0704000.chr", DATA_TYPE_EFFECT ), 1, __CHR_EFFECT_NONE__);
		
		GXSetPosition( pMonster->m_pEffectTarget->hEffect.pHandle, &pMonster->m_v3CurPos, FALSE );
		::SetAction( pMonster->m_pEffectTarget->hEffect.pHandle, 2, 0, ACTION_LOOP );

		pMonster->m_pEffectSelect = g_pEffectLayer->CreateGXObject(GetFile( "e0120000.chr", DATA_TYPE_EFFECT ), 1, __CHR_EFFECT_NONE__);
		
		GXSetPosition( pMonster->m_pEffectSelect->hEffect.pHandle, &pMonster->m_v3CurPos, FALSE );
		
		SelectSummonMonster(pMonster, TRUE);
	}

	pMonster->ModelScaleByLevel();
	
	pMonster->m_wModNo			=	pAppear->wMonNo;

	// sung-han 2005-01-30 특정 몬스터에게 좔좔 효과를 준다.
	// 폭주한 몬스터에게도 추가 : 최덕석 2005.2.14
	if( pMonster->m_dwMonsterKind == OBJECT_TYPE_MONSTER && pMonster->m_bClass == 20 )
	{
		g_pExecutive->GXOSetEffectIndex(pMonster->m_hMonster.pHandle, EFS_upgrade_effect_1);
	}

	memset(pMonster->m_hSoundFile, 0, sizeof(SOUND_FILE_HANDLE)*10);

#if !defined (SOUND_NEW)
	char szBase[255] = {0, };

	if (g_pSoundLib)
	{
		lstrcpy( szBase, GetFile(szBase, DATA_TYPE_SOUND));
		
		SOUND_ERROR_CODE	ser;
		wsprintf(szBuf, "%s%d.wav", szBase, pAppear->wNeu);
		ser = g_pSoundLib->CreateSoundEffectFile(&pMonster->m_hSoundFile[MONSTER_SOUND_NEUTRAL], szBuf);

		wsprintf(szBuf, "%s%d.wav", szBase,pAppear->wAttack[0]);
		g_pSoundLib->CreateSoundEffectFile(&pMonster->m_hSoundFile[MONSTER_SOUND_ATTACK], szBuf);

		wsprintf(szBuf, "%s%d.wav", szBase,pAppear->wAttack[1]);
		g_pSoundLib->CreateSoundEffectFile(&pMonster->m_hSoundFile[MONSTER_SOUND_ATTACK+1], szBuf);

		wsprintf(szBuf, "%s%d.wav", szBase,pAppear->wDamage[0]);
		g_pSoundLib->CreateSoundEffectFile(&pMonster->m_hSoundFile[MONSTER_SOUND_GETHIT], szBuf);

		wsprintf(szBuf, "%s%d.wav", szBase, pAppear->wDamage[1]);
		g_pSoundLib->CreateSoundEffectFile(&pMonster->m_hSoundFile[MONSTER_SOUND_GETHIT+1], szBuf);

		wsprintf(szBuf, "%s%d.wav", szBase,pAppear->wDeath[0]);
		g_pSoundLib->CreateSoundEffectFile(&pMonster->m_hSoundFile[MONSTER_SOUND_DEATH], szBuf);

		wsprintf(szBuf, "%s%d.wav", szBase,pAppear->wDeath[1]);
		g_pSoundLib->CreateSoundEffectFile(&pMonster->m_hSoundFile[MONSTER_SOUND_DEATH+1], szBuf);

		wsprintf(szBuf, "%s%d.wav", szBase,pAppear->wMove[0]);
		g_pSoundLib->CreateSoundEffectFile(&pMonster->m_hSoundFile[MONSTER_SOUND_MOVE], szBuf);

		wsprintf(szBuf, "%s%d.wav", szBase,pAppear->wMove[1]);
		g_pSoundLib->CreateSoundEffectFile(&pMonster->m_hSoundFile[MONSTER_SOUND_MOVE+1], szBuf);
	}

	pMonster->wNeu				=	pAppear->wNeu;
	pMonster->wDamage[0]		=	pAppear->wDamage[0];
	pMonster->wDamage[1]		=	pAppear->wDamage[1];
	pMonster->wAttack[0]		=	pAppear->wAttack[0];
	pMonster->wAttack[1]		=	pAppear->wAttack[1];
	pMonster->wDeath[0]			=	pAppear->wDeath[0];
	pMonster->wDeath[1]			=	pAppear->wDeath[1];	
	pMonster->wMove[0]			=	pAppear->wMove[0];
	pMonster->wMove[1]			=	pAppear->wMove[1];
#endif
}


void RequestDungeonInfo( BYTE bType, DWORD dwIndex )
{
	CTDS_DUNGEON_REQUEST_INFO	Request;
	
	if( bType == 1 && dwIndex == g_pMainPlayer->m_dwUserIndex ) return;

	Request.bRequestType		= bType;
	Request.dwRequestIndex		= dwIndex;

	g_pNet->SendMsg( (char*)&Request, Request.GetPacketSize(), SERVER_INDEX_ZONE );
}


BOOL PathFindDungeon()
{
	// 만약 마우스 포인터가 맵이 좃袖별汰?찍으면 리턴 에러.
	if( g_Mouse.v3Mouse.z < 0 || g_Mouse.v3Mouse.x < 0)
		return	FALSE;

	// 오토타겟을 초기화 한다.
	g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_TYPE ]	= 0;
	g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_INDEX ]	= 0; 
	
	DWORD dwStartX, dwStartZ, dwEndX, dwEndZ; 
	dwStartX = (DWORD)( g_pMainPlayer->m_v3CurPos.x / TILE_SIZE );
	dwStartZ = (DWORD)( g_pMainPlayer->m_v3CurPos.z / TILE_SIZE );
	dwEndX = (DWORD)( g_Mouse.v3Mouse.x / TILE_SIZE );
	dwEndZ = (DWORD)( g_Mouse.v3Mouse.z / TILE_SIZE );

	if(dwEndX >= g_pMap->m_dwMapXTileMany || dwEndZ >= g_pMap->m_dwMapZTileMany)	return FALSE;
	
	g_PathFinder.dwCurveNum = g_pSw->FindShortestWay( 
								dwStartX, 
								dwStartZ,
								dwEndX,
								dwEndZ, &g_PathFinder.pLocation );

	if(g_PathFinder.dwCurveNum < 1)
		return FALSE;

	MAP_TILE*	pTile = 0, *pNextTile = 0;
	VECTOR3		v3Tmp, v3Direction, v3Dest;
	int			cnt = 1;

	v3Dest.x	= (float)( g_PathFinder.pLocation[0].dwBlock_X * TILE_WIDTH + ( TILE_WIDTH / 2.f ) );
	v3Dest.z	= (float)( g_PathFinder.pLocation[0].dwBlock_Y * TILE_HEIGHT + ( TILE_HEIGHT / 2.f ) );
	v3Dest.y	= 0;
	
	VECTOR3_SUB_VECTOR3( &v3Direction, &v3Dest, &g_pMainPlayer->m_v3CurPos );
	Normalize( &v3Tmp, &v3Direction );						// 움직일 방형의 반위백터
	pTile = g_pMap->GetTile( g_pMainPlayer->m_v3CurPos.x, g_pMainPlayer->m_v3CurPos.z );
	pNextTile = pTile;

	do
	{
		pNextTile = g_pMap->GetTile( g_pMainPlayer->m_v3CurPos.x + v3Tmp.x * cnt, g_pMainPlayer->m_v3CurPos.z + v3Tmp.z * cnt );
		cnt++;
	} while( pTile == pNextTile );

	if( !pNextTile 
		|| pNextTile->wAttr.uSection == 0 
		|| pNextTile->wAttr.uOccupied != TILE_EMPTY )
	{
		if( g_pMainPlayer->GetStatus() == UNIT_STATUS_WALKING 
			|| g_pMainPlayer->GetStatus() == UNIT_STATUS_RUNNING )
		{
			SendStopPacket();
		}

		// 이동하지 않고 이동을 취소한다.
		::SetAction(g_pMainPlayer->m_hPlayer.pHandle, MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );
		g_pMainPlayer->SetStatus( UNIT_STATUS_NORMAL );
		g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;
		return FALSE;
	}
	
	g_PathFinder.dwCurCurve = 0;

	return TRUE;
}


BOOL SetPathFindMove()
{
	if( g_PathFinder.dwCurveNum == 0 ) goto lb_stop;
	if( g_PathFinder.dwCurveNum == g_PathFinder.dwCurCurve ) goto lb_stop;
	
	// 여기서 부터 길찾기 설정이다.
	g_pMainPlayer->m_hPlayer.pDesc->vDest.x		= (float)( g_PathFinder.pLocation[g_PathFinder.dwCurCurve].dwBlock_X * TILE_WIDTH + ( TILE_WIDTH / 2.f ) );
	g_pMainPlayer->m_hPlayer.pDesc->vDest.z		= (float)( g_PathFinder.pLocation[g_PathFinder.dwCurCurve].dwBlock_Y * TILE_HEIGHT + ( TILE_HEIGHT / 2.f ) );
	g_pMainPlayer->m_pDestTile		= g_pMap->GetMap( g_PathFinder.pLocation[g_PathFinder.dwCurCurve].dwBlock_X,
													   g_PathFinder.pLocation[g_PathFinder.dwCurCurve].dwBlock_Y );
	g_pMainPlayer->m_hPlayer.pDesc->vDest.y		= 0.f;
	g_PathFinder.dwCurCurve++;	
	
	g_pMainPlayer->m_hPlayer.pDesc->dwStartTick	= g_dwCurTick;					// 현재의 ms를 구한다.
	
	VECTOR3	v3Dir, v3DirNormal;
	VECTOR3_SUB_VECTOR3( &v3Dir, &g_pMainPlayer->m_hPlayer.pDesc->vDest, &g_pMainPlayer->m_v3CurPos );
	Normalize( &v3DirNormal, &v3Dir );						// 움직일 방형의 반위백터
	g_pExecutive->GXOSetDirection( g_pMainPlayer->m_hPlayer.pHandle, &g_Camera.v3AxsiY, (float)(atan2(v3Dir.z, v3Dir.x) + DEG90 ) );

	switch( g_pMainPlayer->GetStatus())
	{
	case UNIT_STATUS_WALKING:
	case UNIT_STATUS_RUNNING:
		{
			if( EpsilonVector( &g_pMainPlayer->m_v3Direction,  &v3DirNormal, 0.1f ) ) return TRUE;	// 같은 방향이면 메시지를 보낼 필요없음.						

			// 방향이 같지 않다면 이동 메시지를 날린다.
			g_pMainPlayer->SetAction( WORD((g_pMainPlayer->m_bMoveType == UNIT_STATUS_WALKING) ? MOTION_TYPE_WALK : MOTION_TYPE_RUN)
					, g_pExecutive->GXOGetCurrentFrame( g_pMainPlayer->m_hPlayer.pHandle )
					, ACTION_LOOP );
			
			g_pMainPlayer->m_v3Direction = v3DirNormal;

			// Move Packet를 보낸다.
			SendMovePacket();			

			float	fRad = 0.f; VECTOR3 vec;
			g_pExecutive->GXOGetDirection(g_pMainPlayer->m_hPlayer.pHandle, &vec, &fRad);
			
			CMiniMapWnd* pMiniMapWnd = CMiniMapWnd::GetInstance();

			pMiniMapWnd->SetRotObj(SPR_OBJ_DSMAP_USER, -fRad+PI);
			

			g_pMainPlayer->ShowMoveStartEffect();
			g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc	= PathFindMoveFunc;
		}
		break;
	
	case UNIT_STATUS_NORMAL:
		{
			g_pMainPlayer->SetAction(WORD((g_pMainPlayer->m_bMoveType == UNIT_STATUS_WALKING) ? MOTION_TYPE_WALK : MOTION_TYPE_RUN), 0, ACTION_LOOP );
			
			g_pMainPlayer->m_v3Direction	= v3DirNormal;
			g_pMainPlayer->SetStatus(g_pMainPlayer->m_bMoveType);

			// Move Message를 보낸다.
			SendMovePacket();

			float	fRad = 0.f; VECTOR3 vec;
			g_pExecutive->GXOGetDirection(g_pMainPlayer->m_hPlayer.pHandle, &vec, &fRad);			
			
			CMiniMapWnd* pMiniMapWnd = CMiniMapWnd::GetInstance();

			pMiniMapWnd->SetRotObj(SPR_OBJ_DSMAP_USER, -fRad+PI);

			// 이펙트를 발생시킨다.
			g_pMainPlayer->ShowMoveStartEffect();
			g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc	= PathFindMoveFunc;
		}
		break;
	}
	
	return TRUE;

lb_stop:
	return FALSE;
}



void PathFindMoveFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	VECTOR3 Pos, vec;
	float fRad = 0.f;
	g_pExecutive->GXOGetPosition(handle, &Pos);				// 현재의 위치를 얻어오고.
	g_pExecutive->GXOGetDirection(handle, &vec, &fRad);
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;			// 진행 상태 얻어오고.
	
	pObjDesc->dwCurTick		= g_dwCurTick;					
	DWORD dwTime = pObjDesc->dwCurTick - pObjDesc->dwStartTick;
	float fSpeed = 0.f;

	CMainUser*	pMainUser = (CMainUser*)pData->pInfo;

	// 청취 위치 설정...
	g_pGeometry->GetCameraDesc(&g_Camera.CameraDesc, 0);
	g_pGeometry->GetCameraAngleRad(&g_Camera.v3Angle, 0);

	SetListener(&g_Camera.v3Angle);
	
	if( pMainUser->GetStatus() == UNIT_STATUS_WALKING ) 
	{
		// sung-han 2005-01-14 포박 스킬에 걸려도 걷기로 이동 가능한 버그 수정
		//---------------------------------------------------------------------
		//fSpeed = 0.20f * dwTime;
		if( pMainUser->m_fMoveSpeed == 0 )
			fSpeed = 0;
		else
			fSpeed = 0.20f * dwTime;
		//---------------------------------------------------------------------
		if( g_pChrInfoUser->GetFrameInfo(pMainUser->m_wClass-1, pMainUser->m_byItemType, MOTION_TYPE_WALK, _CHRINFO_FRAME0) == dwCurFrame || 
			g_pChrInfoUser->GetFrameInfo(pMainUser->m_wClass-1, pMainUser->m_byItemType, MOTION_TYPE_WALK, _CHRINFO_FRAME1) == dwCurFrame )
		{
			_PlaySound( GAMEPLAY_HEABYSTONE_WALK, SOUND_TYPE_GAMEPLAY, 0,  pMainUser->m_v3CurPos, FALSE );
		}
	}
	else 
	{
		fSpeed =pMainUser->m_fMoveSpeed * dwTime;
		if( g_pChrInfoUser->GetFrameInfo(pMainUser->m_wClass-1, pMainUser->m_byItemType, MOTION_TYPE_RUN, _CHRINFO_FRAME0) == dwCurFrame ||
			g_pChrInfoUser->GetFrameInfo(pMainUser->m_wClass-1, pMainUser->m_byItemType, MOTION_TYPE_RUN, _CHRINFO_FRAME1) == dwCurFrame )
		{
			_PlaySound( GAMEPLAY_HEABYSTONE_RUN, SOUND_TYPE_GAMEPLAY, 0, pMainUser->m_v3CurPos, FALSE );
		}
	}


	MAP_TILE*	pTile = NULL;
	VECTOR3		v3Tmp;

	// 도착 지점인지를 체크한다.
	if( EpsilonVector( &Pos, &pObjDesc->vDest, fSpeed ) )
	{
		SetUserPosition( pMainUser, &pObjDesc->vDest );
		if( g_PathFinder.dwCurveNum == 0 ) goto lb_stop;
		else if( g_PathFinder.dwCurveNum == g_PathFinder.dwCurCurve ) goto lb_stop;
	}
	else	
	{
		// 도착 지점이 아니라면 계속 진행한다.
		v3Tmp = pMainUser->m_v3CurPos;
		Pos.x += cosf(fRad-DEG90)*fSpeed;
		Pos.z += sinf(fRad-DEG90)*fSpeed;
		SetUserPosition( pMainUser, &Pos );
		pTile = g_pMap->GetTile( Pos.x, Pos.z );			// 이동한 다음의 타일을 받는다. 
															// 타일이 바뀌었나 체크를 위해.
		if (!pTile)
		{
			SetUserPosition( pMainUser, &v3Tmp);
			goto lb_stop;
		}
		
		if( ( g_pMainPlayer->m_pCurTile->wIndex_X != pTile->wIndex_X ) || 
			( g_pMainPlayer->m_pCurTile->wIndex_Z != pTile->wIndex_Z )	)
		{
			if( pTile->wAttr.uOccupied != TILE_EMPTY )
			{
			
				SetUserPosition( pMainUser, &v3Tmp );
				goto lb_stop;
			}

			pMainUser->m_pCurTile = pTile;
		}
		pObjDesc->dwStartTick = pObjDesc->dwCurTick;
	}

	return;

lb_stop:
	//  달리다가 정지한것이라면 이페트.
	g_pMainPlayer->ShowMoveStopEffect();
	
	if (g_dwLayerID < 100 )	// 던전은 아니므로
		::SetAction(g_pMainPlayer->m_hPlayer.pHandle, MOTION_TYPE_VILLAGESTAND, 0, ACTION_LOOP );
	else	
		::SetAction(g_pMainPlayer->m_hPlayer.pHandle, MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );

	pMainUser->SetStatus(UNIT_STATUS_NORMAL);
	pObjDesc->ObjectFunc = NULL;	// 더이상 이동 체크하지 않게하기위해서.

	// 스탑 메시지를 보낸다.
	SendStopPacket();

	// 만약 아이템이 있으면 줍기 메시지를 보낸다.
	if( g_pMainPlayer->m_i64PickupItem != 0 )
	{
		ITEM*	pItem	= g_pItemHash->GetData( g_pMainPlayer->m_i64PickupItem );
		if( !pItem ) return;

		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			g_ItemMoveManager.TileToInvPickupItem(pItem);
		}
		else
		{
			SendPickupItem(pItem, FALSE);
		}	
		
		g_pMainPlayer->m_i64PickupItem = 0;
	}

	return;
}

//-----------------------------------------------------------------------------
// 목적지 좌표까지 캐릭터를 앞으로 전진시킨다. 플레이어만 적용
//-----------------------------------------------------------------------------
void PlayerMoveFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	VECTOR3		Pos;
	MAP_TILE*	pTile=0;
	g_pExecutive->GXOGetPosition(handle, &Pos);				// 현재의 위치를 얻어오고.

	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;			// 진행 상태 얻어오고.	
	CUser*	pUser			= (CUser*)pData->pInfo;
	
	float fSpeed;
	pObjDesc->dwCurTick		= g_dwCurTick;					
	DWORD dwTime			= pObjDesc->dwCurTick - pObjDesc->dwStartTick;
	pObjDesc->dwStartTick	= pObjDesc->dwCurTick;

	// sung-han 2005-01-14 포박 스킬에 걸려도 걷기로 이동 가능한 버그 수정
	//--------------------------------------------------------------------------
	//if( pUser->GetStatus() == UNIT_STATUS_WALKING ) fSpeed = 0.20f * dwTime;
	//else fSpeed = pUser->m_fMoveSpeed * dwTime;
	if( pUser->GetStatus() == UNIT_STATUS_WALKING )
	{
		if( pUser->m_fMoveSpeed == 0 )
			fSpeed = 0;
		else
			fSpeed = 0.20f * dwTime;
	}
	else fSpeed = pUser->m_fMoveSpeed * dwTime;
	//--------------------------------------------------------------------------
	
	// 만약 이동한 타일이 이전의 타일과 다른 타일이라면 케릭터가 서있는 타일 속성을 옮긴다.
	g_pExecutive->GXOMoveForward( handle, fSpeed );	// 이동 방향으로 계속 진행한다.
	g_pExecutive->GXOGetPosition( handle, &Pos );
	
	// 현재의 위치와 옮겨가가될 위치를 검색해서 만약 중간에 못가는 타일이 있다면 세워버린다.
	pTile = g_pMap->GetTile( Pos.x, Pos.z );
	if( !pTile )
	{
		//trust
		GXSetPosition( handle, &pUser->m_v3CurPos , TRUE, TRUE);
		
		if (g_dwLayerID < 100 )	// 던전은 아니므로
			::SetAction(pUser->m_hPlayer.pHandle,  MOTION_TYPE_VILLAGESTAND, 0, ACTION_LOOP );
		else
			::SetAction(pUser->m_hPlayer.pHandle, MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );

		pUser->ShowMoveStopEffect();
		pUser->SetStatus(UNIT_STATUS_NORMAL);
		return;
	}
	
	if( pTile->wAttr.uSection == 0 )
	{
		//trust
		GXSetPosition( handle, &pUser->m_v3CurPos, TRUE , TRUE);

		if (g_dwLayerID < 100 )	// 던전은 아니므로
			::SetAction(pUser->m_hPlayer.pHandle, MOTION_TYPE_VILLAGESTAND, 0, ACTION_LOOP );
		else
			::SetAction(pUser->m_hPlayer.pHandle, MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );

		pUser->ShowMoveStopEffect();
		pUser->SetStatus(UNIT_STATUS_NORMAL);
		return;
	}


	// 시작 타일을 나갔는지를 검사한다.
	SetUserPosition( pUser, &Pos );

//	POSITION_		pos;
	LP_PARTY_USER	sPartyNode;
	
	// 파티원 위치 업데이트 //
//	BYTE byIndex = 0;
	
	//030827 KBS
	/*
	pos	= g_pPartyList->GetHeadPosition();		

	while(pos)
	{
		sPartyNode = (LP_PARTY_USER)g_pPartyList->GetNext(pos);

		if(pUser->m_dwUserIndex==sPartyNode->dwUserIndex)
		{	
			CMiniMapWnd* pMiniMapWnd = CMiniMapWnd::GetInstance();

			VECTOR2 vec = pMiniMapWnd->GetRatioMinimap(pUser->m_v3CurPos.x, pUser->m_v3CurPos.z);

			if(pMiniMapWnd->m_bInit==TRUE)	
			{
				SPR(sPartyNode->dwResourceId)->vPos.x = pMiniMapWnd->m_fPosX+vec.x+8;
				SPR(sPartyNode->dwResourceId)->vPos.y = pMiniMapWnd->m_fPosZ+vec.y+42;
			}			
		}
	}
	*/
	

	sPartyNode = g_pPartyUserHash->GetData( pUser->m_dwUserIndex );

	if(sPartyNode)
	{
		CMiniMapWnd* pMiniMapWnd = CMiniMapWnd::GetInstance();

		VECTOR2 vec = pMiniMapWnd->GetRatioMinimap(pUser->m_v3CurPos.x, pUser->m_v3CurPos.z);

		if(pMiniMapWnd->m_bInit==TRUE)	
		{
			pMiniMapWnd->SetPosObj(sPartyNode->dwResourceId, vec.x+8, vec.y+42);
			
		}			
	}
	//--
	return;
}


//-----------------------------------------------------------------------------
// File: PlayerDamageFunc()
// Desc: [ObjectFuncion]
//		 플레이어의 방어.
//-----------------------------------------------------------------------------
void PlayerDamageFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;			// 진행 상태 얻어오고.	
	CUser*	pUser = (CUser*)pData->pInfo;
	if (dwCurFrame == 0 )
	{
		
		if (pUser->UsingStatusSkill(__SKILL_AIREALCOAT__))
		{
			_PlaySound(__SKILL_AIREALCOAT__, SOUND_EFFECT_GENERAL1, 0, pUser->m_v3CurPos, FALSE);
		}
		else
		{
			_PlaySound( CHARACTER_SOUND_DAMAGE, SOUND_TYPE_CHARACTER, CHARACTER_SOUND_DAMAGE + ( pUser->m_wClass - 1 ) * SOUND_PER_CHARACTER, pUser->m_v3CurPos, FALSE );
		}
	}

	if( bFrameFlag == FRAME_FLAG_CHANGE_NEXT || bFrameFlag == FRAME_FLAG_FINISHED )
	{
		
		switch( pObjDesc->bType )
		{
		case OBJECT_TYPE_PLAYER:
		case OBJECT_TYPE_MAINPLAYER:
			{
				
				::SetAction(pUser->m_hPlayer.pHandle,  MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );
				pUser->SetStatus(UNIT_STATUS_NORMAL);
				pObjDesc->ObjectFunc		= NULL;
				
			}
			break;
		}
	}
}


//-----------------------------------------------------------------------------
// File: MonsterMoveFunc()
// Desc: [ObjectFuncion]
//		 목적지 좌표까지 몬스터를 앞으로 전진시킨다. 몬스터만 적용
//-----------------------------------------------------------------------------
void MonsterMoveFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
//	CMap*			pMap		= g_pMap;
	LPObjectDesc	pObjDesc	= (LPObjectDesc)pData;					// 진행 상태 얻어오고.	
	CMonster*		pMonster	= (CMonster*)pData->pInfo;
	VECTOR3			v3Pos;

	pObjDesc->dwCurTick		= g_dwCurTick;					
	DWORD dwTime			= pObjDesc->dwCurTick - pObjDesc->dwStartTick;
	float fSpeed			= pMonster->m_fSpeed  * dwTime;
	pObjDesc->dwStartTick	= pObjDesc->dwCurTick;
	
//	g_pExecutive->GXOGetPosition(handle, &pMonster->m_v3CurPos);		// 현재의 위치를 얻어오고.

	if(pMonster->m_v3CurPos.x==0 && pMonster->m_v3CurPos.y==0 && pMonster->m_v3CurPos.z==0)
		return;

// Sound_new test
#if !defined(SOUND_NEW)
	// neutral
	if (dwCurFrame == 1)
	{ 
		if ( 8 < GetRandom(10) )
		{
			GetSoundEffect(&pMonster->m_pSound[MONSTER_SOUND_NEUTRAL], pMonster->m_hSoundFile[MONSTER_SOUND_NEUTRAL], pMonster->m_v3CurPos);
			PlaySoundEffect(pMonster->m_pSound[MONSTER_SOUND_NEUTRAL], &pMonster->m_v3CurPos, FALSE);
			
			g_SoundEffectBasketCase.Add(pMonster->m_pSound[MONSTER_SOUND_NEUTRAL]);
			pMonster->m_pSound[MONSTER_SOUND_NEUTRAL] = NULL;

		}
	}
#endif
// ==

	
	// monster move
	int iRan = MONSTER_SOUND_MOVE;

	// sound_new test
#if !defined (SOUND_NEW)
	if( g_pChrInfoMonster->GetChrInfo(pMonster->m_wModNo) &&
		g_pChrInfoMonster->GetFrameInfo(pMonster->m_wModNo, 0, MON_MOTION_TYPE_MOVE1, _CHRINFO_FRAME0) == dwCurFrame)
	{
		GetSoundEffect(&pMonster->m_pSound[iRan], pMonster->m_hSoundFile[iRan], pMonster->m_v3CurPos);
		PlaySoundEffect(pMonster->m_pSound[MONSTER_SOUND_MOVE], &pMonster->m_v3CurPos, FALSE);

		g_SoundEffectBasketCase.Add(pMonster->m_pSound[iRan]);
		pMonster->m_pSound[iRan] = NULL;
	}
	else
	if( g_pChrInfoMonster->GetChrInfo(pMonster->m_wModNo) &&
		g_pChrInfoMonster->GetFrameInfo(pMonster->m_wModNo, 0, MON_MOTION_TYPE_MOVE2, _CHRINFO_FRAME0) == dwCurFrame)
	{
		int iRan = MONSTER_SOUND_MOVE+1;
		GetSoundEffect(&pMonster->m_pSound[iRan], pMonster->m_hSoundFile[iRan], pMonster->m_v3CurPos);
		PlaySoundEffect(pMonster->m_pSound[iRan], &pMonster->m_v3CurPos, FALSE);

		g_SoundEffectBasketCase.Add(pMonster->m_pSound[iRan]);
		pMonster->m_pSound[iRan] = NULL;
	}
#endif
	// ==

	if( EpsilonVector( &pMonster->m_v3CurPos, &pMonster->m_v3DestPos, fSpeed ) )
	{
		::SetAction(pMonster->m_hMonster.pHandle, MON_MOTION_TYPE_STAND1, 0, ACTION_LOOP );
		pMonster->SetStatus(UNIT_STATUS_NORMAL, NULL);
		
		SetMonsterPosition( pMonster, &pMonster->m_v3DestPos );	
	}
	else
	{	// 진행하다 타일이 바꾸었으면 바뀐타일을 몬스터가 서있는 타일로 만든다.
		g_pExecutive->GXOMoveForward( handle, fSpeed );
		g_pExecutive->GXOGetPosition( handle, &v3Pos );
//		pTile = pMap->GetTile( v3Pos.x, v3Pos.z );

//		if( pMonster->m_pCurTile != pTile )
//		{
//			if( pTile->wAttr.uOccupied != TILE_EMPTY )
//			{
//			}
			SetMonsterPosition( pMonster, &v3Pos );			
//		}
	}

	///// 아메리타트 연출 : 최덕석 2005.2.18
	// 아메리타트이면
	if(pMonster->m_wModNo == 630)
	{
		// 발이 땅에 떨어지는 프레임이면
		if((dwCurFrame >= 35 && dwCurFrame <= 45)
		|| (dwCurFrame >= 70 && dwCurFrame <= 80))
		{
			DWORD dwRate = (dwCurFrame >= 70 ? dwCurFrame-69 : dwCurFrame-34);
			float fDist = CalcDistance(&g_pMainPlayer->m_v3CurPos, &pMonster->m_v3CurPos);
			float fPower = 1000/max(1.0, (fDist/100.));

			VECTOR3 vecQuake;vecQuake.x = vecQuake.y = vecQuake.z = 0;

			// 짝수 프레임이면
			if (dwCurFrame % 2 == 0)
			{
				vecQuake.x = fPower/((double)pow((double)dwRate, (double)2)*0.5);
				vecQuake.x = min(vecQuake.x, 5);
			}
			// 홀수 프레임이면
			else
			{
				vecQuake.x = -fPower/((double)pow((double)dwRate, (double)2)*0.5);
				vecQuake.x = max(vecQuake.x, -5);
			}
			
			CameraQuake(vecQuake);
		}
	}

	return;
}

//-----------------------------------------------------------------------------
// 한번 애니메이션을 보여주고 사라지는 이펙트를 사용할때 씀.
//-----------------------------------------------------------------------------
void EffectOnceAndNormal( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc*	pEffectDesc = (EffectDesc*)pObjDesc->pInfo;
	if (pEffectDesc->m_sLightDescEx.LightFunc)
		pEffectDesc->m_sLightDescEx.LightFunc(pEffectDesc);
	
	if( bFrameFlag == FRAME_FLAG_FINISHED || bFrameFlag == FRAME_FLAG_CHANGE_NEXT )
	{
		goto lb_remove;
	}
	else
	{
		CMonster*	pMonster;
		CUser*		pUser;

		
		switch( pEffectDesc->byTargetObjectType[0] ) 
		{
		case OBJECT_TYPE_MONSTER:
			pMonster = g_pMonsterHash->GetData( pEffectDesc->dwTargetIndex[0] ); 
			if( !pMonster ) goto lb_remove;
			GXSetPosition( pEffectDesc->hEffect.pHandle, &pMonster->m_v3CurPos, FALSE );
			break;

		case OBJECT_TYPE_MAINPLAYER:
		case OBJECT_TYPE_PLAYER:
			pUser = g_pUserHash->GetData( pEffectDesc->dwTargetIndex[0] ); 
			if( !pUser ) goto lb_remove;
			GXSetPosition( pEffectDesc->hEffect.pHandle, &pUser->m_v3CurPos, FALSE );
			break;
		}		
	}
	return;

lb_remove:
	RemoveEffectDesc( pEffectDesc );
	
}

void EffectOnceAndHideFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{
//	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;

	if( bFrameFlag == FRAME_FLAG_FINISHED || bFrameFlag == FRAME_FLAG_CHANGE_NEXT )
	{
		HideObject( handle );
	}
}

void DamageNumberFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	
	if( dwCurFrame > 11 ) g_pExecutive->SetAlphaFlag( handle, 255 - ( dwCurFrame - 11 )*30 );
	
	if( bFrameFlag == FRAME_FLAG_FINISHED || bFrameFlag == FRAME_FLAG_CHANGE_NEXT )
	{
		LPDAMAGE_NUMBER pDamage = (LPDAMAGE_NUMBER)pObjDesc->pInfo;

		if( pObjDesc->bType == OBJECT_TYPE_DAMAGE_USER )
		{
			ClearDamageNumber( pDamage );
			FreeDamageNumber( &g_pDamageNumberPool[0], pDamage );
		}
		else if( pObjDesc->bType == OBJECT_TYPE_DAMAGE_MONSTER )
		{
			ClearDamageNumber( pDamage );
			FreeDamageNumber( &g_pDamageNumberPool[1], pDamage );
		}
	}
}

void UpdateMainPlayer( WORD wSectionNum )
{
	CorumCMap*		pMap		= g_pMap;
	CMainUser*	pMainUser	= g_pMainPlayer;
//	MAP_TILE*	pTile		= pMap->GetTile( g_pMainPlayer->m_v3CurPos.x, g_pMainPlayer->m_v3CurPos.z );
	//MAP_TILE*	pTileTemp;
	
	Section_Link_Info*		pSection=0;
	ListNode<ITEM>*			pItemNode=0;
	ITEM*					pItem=0;
	BOOL					bRemove=0;
	int						j=0;

	pMainUser->m_wCurSectionNum = wSectionNum;						// 옮겨간 섹션을 기억하고.
	pSection = &pMap->m_Section[ pMainUser->m_wCurSectionNum - 1 ];	// 그 섹션의 포인터를 얻어온다.

	if(!pSection)
		return;
		
	// 아이템을 검색해서 지울것을 지워버린다.
	pItemNode = g_pItemHash->GetHead();
	while( pItemNode )
	{
		pItem = pItemNode->pData;

		if(!pItem)
			continue;

		if( pItem->bSectionNum != pSection->wSectionNum )
		{
			bRemove = TRUE;
			for( j=0; pSection->pwSectionLinkNum[j] != 0; j++ )
			{
				if( pSection->pwSectionLinkNum[j] == pItem->bSectionNum )
				{
					bRemove = FALSE;
					break;
				}
			}
			
			if( bRemove )
			{
				pItemNode = pItemNode->pNext;
				RemoveItem( pItem );
				continue;
			}
		}
		pItemNode = pItemNode->pNext;
	}
}

void UpdatePlayer()
{
	// 만약 이동한후의 좌표의 섹션이 이전의 섹션과 들린곳이라면..
	CUser*		pUser;
	MAP_TILE*	pTile;
	CorumCMap*		pMap	= g_pMap;
	Section_Link_Info*	pSection	= &pMap->m_Section[g_pMainPlayer->m_wCurSectionNum-1];
	ListNode<CUser>*	pUserNode	= g_pUserHash->GetHead();
	BOOL		bRemove;

	while( pUserNode )
	{
		pUser	= pUserNode->pData;
		
		if( pUser == g_pMainPlayer )
		{
			pUserNode = pUserNode->pNext;
			continue;
		}
		
		pTile	= pMap->GetTile( pUser->m_v3CurPos.x, pUser->m_v3CurPos.z );

		if( pUser->m_wCurSectionNum == pTile->wAttr.uSection )
		{
			pUserNode = pUserNode->pNext;
			continue;
		}
		
		pUser->m_wCurSectionNum	=	pTile->wAttr.uSection;
		
		if( pUser->m_wCurSectionNum == pSection->wSectionNum )
		{
			pUserNode = pUserNode->pNext;
			continue;
		}
		
		bRemove = TRUE;

		for( int i=0; pSection->pwSectionLinkNum[i] != 0; i++ )
		{
			if( pSection->pwSectionLinkNum[i] == pUser->m_wCurSectionNum )	
			{
				bRemove = FALSE;
				break;
			}
		}
		
		// 여기까지 내려왔다면 유저를 지워버리자.
		pUserNode = pUserNode->pNext;
		if( bRemove ) 
		{
			RemovePlayer( pUser );
		}
	}
}

void UpdateMonster()
{
	CMonster*			pMonster;
	MAP_TILE*			pTile;
	CorumCMap*				pMap			= g_pMap;
	Section_Link_Info*	pSection		= &pMap->m_Section[g_pMainPlayer->m_wCurSectionNum-1];
	ListNode<CMonster>*	pMonsterNode	= g_pMonsterHash->GetHead();
	BOOL				bRemove;

	while( pMonsterNode )
	{
		pMonster	= pMonsterNode->pData;
		pTile		= pMap->GetTile( pMonster->m_v3CurPos.x, pMonster->m_v3CurPos.z );

		if( !( ( pMonster->GetStatus() == UNIT_STATUS_WALKING ) 
			|| ( pMonster->GetStatus() == UNIT_STATUS_RUNNING ) ) )
		{
			pMonsterNode = pMonsterNode->pNext;
			continue;
		}
		
		if( pMonster->m_wCurSectionNum == pTile->wAttr.uSection )
		{
			pMonsterNode = pMonsterNode->pNext;
			continue;
		}
		
		pMonster->m_wCurSectionNum	=	pTile->wAttr.uSection;

		// 만약 다르다면.. 음.. 메인케릭터의 섹션과 그 섹션의 링크 섹션을 검색하여 아무곳에도 포함되어있지 않으면
		// 이 몬스터를 삭제시켜 버린다.
		if( pMonster->m_wCurSectionNum == pSection->wSectionNum )
		{
			pMonsterNode = pMonsterNode->pNext;
			continue;
		}
		
		bRemove = TRUE;

		for( int i=0; pSection->pwSectionLinkNum[i] != 0; i++ )
		{
			if( pSection->pwSectionLinkNum[i] == pMonster->m_wCurSectionNum )
			{
				bRemove = FALSE;
				break;
			}
		}

		// 여기까지 내려왔다면 몬스터를 지워버리자.
		pMonsterNode = pMonsterNode->pNext;
		if( bRemove ) 
		{
//			LOGOUT( "DisAppearMonster %d", pMonster->m_dwMonsterIndex );
			RemoveMonster( pMonster );
		}
	}
}

void UpdateItem()
{

}
// sound

///////////////////////////////////////////////////////////////////////////
// Send Packet Func
void SendMovePacket()
{
	CTDS_DUNGEON_MOVE		move;
	move.bMoveType			= g_pMainPlayer->m_bMoveType;
	VECTOR3_TO_VECTOR2(g_pMainPlayer->m_v3CurPos, move.v2MoveStartPos)
	VECTOR3_TO_VECTOR2(g_pMainPlayer->m_v3Direction, move.v2MoveDirection)
	move.wDestX				= g_pMainPlayer->m_pDestTile->wIndex_X;
	move.wDestZ				= g_pMainPlayer->m_pDestTile->wIndex_Z;
	g_pNet->SendMsg( (char*)&move, move.GetPacketSize(), SERVER_INDEX_ZONE );
}

void SendStopPacket()
{
	CTDS_DUNGEON_STOP	stop;
	VECTOR3_TO_VECTOR2(g_pMainPlayer->m_v3CurPos, stop.v2StopPos)
	g_pNet->SendMsg( (char*)&stop, stop.GetPacketSize(), SERVER_INDEX_ZONE );
}


void ShopProc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{	
	LPObjectDesc	pObjDesc	= (LPObjectDesc)pData;
//	CUser*			pUser		= (CUser*)pData->pInfo;

	if(bFrameFlag==FRAME_FLAG_FINISHED || bFrameFlag==FRAME_FLAG_CHANGE_NEXT )		
	{
		pObjDesc->ObjectFunc = NULL;
		HideObject(handle);
	}
}

void PlayerShopProc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	LPObjectDesc	pObjDesc	= (LPObjectDesc)pData;
	CUser*			pUser		= (CUser*)pData->pInfo;
		
	if(bFrameFlag==FRAME_FLAG_FINISHED || bFrameFlag==FRAME_FLAG_CHANGE_NEXT )	
	{
		pUser->SetStatus(UNIT_STATUS_NORMAL);
		pObjDesc->ObjectFunc = NULL;
	}
}


void SendPKMode(BOOL bPKMode)
{
	CTDS_DUNGEON_PKMODE packet;
	packet.bPKMode = (BYTE)bPKMode;
	g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
}


void ChatRender()
{
	int nDgInx = 0;
		
	if(g_nDgCount==__MAX_QUEUE__)
	{
		if(g_nDgIndex!=0)
		{
			for(int i = g_nDgIndex-1; i >= 0; i--)
			{
				if(g_sDgChatMsg[i].sbActive==TRUE)
				{
					if(!IsEmptyString(g_sDgChatMsg[i].szDungeon))
					{						
						g_DgRect[nDgInx].right	= 12 + g_sDgChatMsg[i].snSize*6;
						RenderFont(g_sDgChatMsg[i].szDungeon, g_DgRect[nDgInx].left, g_DgRect[nDgInx].right, g_DgRect[nDgInx].top, g_DgRect[nDgInx].bottom, __ORDER_CHAT_START__, g_sDgChatMsg[i].sdwColor);
					}					
					nDgInx++;
					if( nDgInx >= g_byChatLineCnt*__MIN_QUEUE__)
						goto lb_limit_chat_line_cnt;
				}
			}

			for(int i = __MAX_QUEUE__-1; i >= g_nDgIndex; i--)
			{
				if(g_sDgChatMsg[i].sbActive==TRUE)
				{
					if(!IsEmptyString(g_sDgChatMsg[i].szDungeon))
					{
						g_DgRect[nDgInx].right	= 12 + g_sDgChatMsg[i].snSize*6;
						RenderFont(g_sDgChatMsg[i].szDungeon, g_DgRect[nDgInx].left, g_DgRect[nDgInx].right, g_DgRect[nDgInx].top, g_DgRect[nDgInx].bottom, __ORDER_CHAT_START__, g_sDgChatMsg[i].sdwColor);						
					}
					nDgInx++;
					if( nDgInx >= g_byChatLineCnt*__MIN_QUEUE__)
						goto lb_limit_chat_line_cnt;
				}
			}
		}
		else
		{
			for(int i = __MAX_QUEUE__-1; i >= 0; i--)
			{
				if(g_sDgChatMsg[i].sbActive==TRUE)
				{
					if(!IsEmptyString(g_sDgChatMsg[i].szDungeon))
					{
						g_DgRect[nDgInx].right	= 12 + g_sDgChatMsg[i].snSize*6;
						RenderFont(g_sDgChatMsg[i].szDungeon, g_DgRect[nDgInx].left, g_DgRect[nDgInx].right, g_DgRect[nDgInx].top, g_DgRect[nDgInx].bottom, __ORDER_CHAT_START__, g_sDgChatMsg[i].sdwColor);
					}
					nDgInx++;
					if( nDgInx >= g_byChatLineCnt*__MIN_QUEUE__)
						goto lb_limit_chat_line_cnt;
				}
			}
		}
	}
	else
	{
		for(int i = g_nDgIndex; i >= 0; i--)
		{		
			if(g_sDgChatMsg[i].sbActive==TRUE)
			{
				if(!IsEmptyString(g_sDgChatMsg[i].szDungeon))
				{
					g_DgRect[nDgInx].right	= 12 + g_sDgChatMsg[i].snSize * 6;									

					RenderFont(g_sDgChatMsg[i].szDungeon
						, g_DgRect[nDgInx].left
						, g_DgRect[nDgInx].right
						, g_DgRect[nDgInx].top
						, g_DgRect[nDgInx].bottom
						, __ORDER_CHAT_START__
						, g_sDgChatMsg[i].sdwColor);
				}
				nDgInx++;
				if( nDgInx >= g_byChatLineCnt*__MIN_QUEUE__)
						goto lb_limit_chat_line_cnt;
			}
		}

		for(int i = __MAX_QUEUE__-1; i > g_nDgIndex; i--)
		{
			if(g_sDgChatMsg[i].sbActive==TRUE)
			{
				if(!IsEmptyString(g_sDgChatMsg[i].szDungeon))
				{
					g_DgRect[nDgInx].right	= 12 + g_sDgChatMsg[i].snSize*6;
					RenderFont(g_sDgChatMsg[i].szDungeon, g_DgRect[nDgInx].left, g_DgRect[nDgInx].right, g_DgRect[nDgInx].top, g_DgRect[nDgInx].bottom, __ORDER_CHAT_START__, g_sDgChatMsg[i].sdwColor);
				}
				nDgInx++;
				if( nDgInx >= g_byChatLineCnt*__MIN_QUEUE__)
						goto lb_limit_chat_line_cnt;
			}
		}	
	}

lb_limit_chat_line_cnt:// sung-han 2005-03-30 노말채팅 라인수 1:5줄, 2:10줄, 3:15줄
	if(nDgInx!=0)
	{
		SPR(SPR_MESSAGE_BOX)->ShowSprite(TRUE);
		SPR(SPR_MESSAGE_BOX)->vPos.x		= 4;
		SPR(SPR_MESSAGE_BOX)->vPos.y		= float(607-nDgInx*18+4);

#if IS_JAPAN_LOCALIZING()
		SPR(SPR_MESSAGE_BOX)->v2Scaling.x = 170.0f;//178.0f;
#else
		SPR(SPR_MESSAGE_BOX)->v2Scaling.x = 178.0f;//210.0f;
#endif

		SPR(SPR_MESSAGE_BOX)->v2Scaling.y	= nDgInx*9.0f+4;
	}
	else
		SPR(SPR_MESSAGE_BOX)->ShowSprite(FALSE);
		
}

void SkillRender()
{
	CSkillWnd*	pSkillWnd = CSkillWnd::GetInstance();
//	Effect*		pEffect;
	int			nOrder	= __ORDER_USERINTERFACE_START_+__ORDER_SKILL_KEY__;
	CMonster *pGuardian = g_pMainPlayer->m_pGuardian[0];
	
	// Skill 작업 //
	if(pSkillWnd->m_bySkillIndex==1)
	{
		for(int i = 0; i < 16; i++)
		{
			if(g_pMainPlayer->m_nSkillKey[i]!=-1)
			{
				if(g_pMainPlayer->m_nSkillKey[i]==__SKILL_ATTACK__)
				{
					if(g_pMainPlayer->m_nPosSK[i]==0)
					{
						int		nPosX	= 116;
						int		nPosY	= 646;					
						char*	szKey	= KeySearch(g_sKeyConfig.snKey[18+i]);

						if(!IsEmptyString(szKey))
							RenderFont(szKey, nPosX+3, nPosX+32, nPosY+12, nPosY+26, nOrder, 0xff00ff00);
					}					
				}
				else
				{
					int nSkill_Idx = 1;

					for(int j = 0; j < g_sSkillListManager.byLeftSkillCnt; j++)
					{
						if(g_pMainPlayer->GetSkillLevel(g_sSkillListManager.byLeftSkill[j])>0)
						{
						//	int nPosX = 116+(nSkill_Idx%5)*33;
						//	int nPosY = 646-(nSkill_Idx/5)*33;
							
							if(g_pMainPlayer->m_nSkillKey[i]==g_sSkillListManager.byLeftSkill[j])
							{
								if(g_pMainPlayer->m_nPosSK[i]==0)
								{
									int		nPosX	= 116+(nSkill_Idx%5)*32;
									int		nPosY	= 646-(nSkill_Idx/5)*32;
									char*	szKey	= KeySearch(g_sKeyConfig.snKey[18+i]);																		

									if(szKey && !IsEmptyString(szKey))
										RenderFont(szKey, nPosX+3, nPosX+32, nPosY+12, nPosY+26, nOrder, 0xff00ff00);
								}										
							}
							nSkill_Idx++;
						}
					}					
				}
			}
		}
	}
	else if(pSkillWnd->m_bySkillIndex==2)
	{		
//		Effect* pEffect;

		for(int i = 0; i < 16; i++)
		{
			if(g_pMainPlayer->m_nSkillKey[i]!=-1)
			{				
				int nSkill_Idx = 0;

				for(int j = 0; j < g_sSkillListManager.byRightSkillCnt; j++)
				{
					if(g_pMainPlayer->GetSkillLevel(g_sSkillListManager.byRightSkill[j])>0)
					{
						if(g_pMainPlayer->m_nSkillKey[i]==g_sSkillListManager.byRightSkill[j])
						{
							if(g_pMainPlayer->m_nPosSK[i]==1)
							{
								int		nPosX	= 116+(nSkill_Idx%5)*32;
								int		nPosY	= 646-(nSkill_Idx/5)*32;
								char*	szKey	= KeySearch(g_sKeyConfig.snKey[18+i]);
								
								if(!IsEmptyString(szKey))
									RenderFont(szKey, nPosX+3, nPosX+32, nPosY+12, nPosY+26, nOrder, 0xff00ff00);
							}										
						}
						nSkill_Idx++;
					}				
				}
			}
		}
	}
}

BOOL BeltCollision()
{
	CInterface*		pInterface		= CInterface::GetInstance();
//	CUserInterface* pUserInterface	= CUserInterface::GetInstance();
	
	for(int i = 0; i < CUR_INTERFACE; i++)
	{				
		if(i==COMMAND_WND || i==INTERFACE_USER)
			continue;		

		if(pInterface->m_pMenu[i]->GetActive()==TRUE)
		{
			int nStartPosX	= pInterface->m_pMenu[i]->GetPosX();
			int nStartPosY	= pInterface->m_pMenu[i]->GetPosY();
			int nEndPosX	= pInterface->m_pMenu[i]->GetPosX() + pInterface->m_nSize[i*2];
		//	int nEndPosY	= pInterface->m_pMenu[i]->GetPosY() + pInterface->m_nSize[i*2+1];
			
			if(nStartPosX<=g_Mouse.MousePos.x && nEndPosX>=g_Mouse.MousePos.x && nStartPosY<=g_Mouse.MousePos.y && nStartPosY>=g_Mouse.MousePos.y)
				return FALSE;			
		}
	}
	return TRUE;
}

BOOL GuardianCollision()
{
	CInterface*		pInterface		= CInterface::GetInstance();
//	CUserInterface* pUserInterface	= CUserInterface::GetInstance();
	
	for(int i = 0; i < CUR_INTERFACE; i++)
	{				
		if(i==COMMAND_WND || i==INTERFACE_USER)
			continue;		

		if(pInterface->m_pMenu[i]->GetActive()==TRUE)
		{
			int nStartPosX	= pInterface->m_pMenu[i]->GetPosX();
			int nStartPosY	= pInterface->m_pMenu[i]->GetPosY();
		//	int nEnxPosX	= pInterface->m_pMenu[i]->GetPosX() + pInterface->m_nSize[i*2];
		//	int nEndPosY	= pInterface->m_pMenu[i]->GetPosY() + pInterface->m_nSize[i*2+1];
			
			if(nStartPosX>=93 && nStartPosX<=125 && nStartPosY>=26 && nStartPosY<=58)
				return FALSE;			
		}
	}
	return TRUE;
}


BOOL ItemCollision(int nIndex)
{
	CInterface*		pInterface		= CInterface::GetInstance();
//	CUserInterface* pUserInterface	= CUserInterface::GetInstance();
	
	for(int i = 0; i < CUR_INTERFACE; i++)
	{
		int nOrderDst = pInterface->m_pMenu[i]->GetOrderIndex();
		int nOrderSrc = pInterface->m_pMenu[nIndex]->GetOrderIndex();

		if(i==nIndex)
			continue;
		if(i==COMMAND_WND || i==INTERFACE_USER)
			continue;
		if(nOrderDst < nOrderSrc)
			continue;

		if(pInterface->m_pMenu[i]->GetActive()==TRUE)
		{
			int nStartPosX	= pInterface->m_pMenu[i]->GetPosX();
			int nStartPosY	= pInterface->m_pMenu[i]->GetPosY();
			int nEnxPosX	= pInterface->m_pMenu[i]->GetPosX() + pInterface->m_nSize[i*2];
			int nEndPosY	= pInterface->m_pMenu[i]->GetPosY() + pInterface->m_nSize[i*2+1];
			
			if(	nStartPosX	<= g_Mouse.MousePos.x	&&
				nEnxPosX	>= g_Mouse.MousePos.x	&&
				nStartPosY	<= g_Mouse.MousePos.y	&&
				nEndPosY	>= g_Mouse.MousePos.y )
			{
				return FALSE;
			}
		}
	}	

	return TRUE;
}


void ConvertKeyDown(char* szKey)
{
	char szKeyVal;
	szKeyVal = szKey[0];
	SetKey(szKeyVal);
}
/*
void SetFocusInput(int nCurFocus, int nInputType, int nOrder, int nLeft, int nRight, int nTop, int nBottom, BOOL bPos)
{	
	g_nChatModeExt							= nInputType;
	g_InputBuffer[nCurFocus].dwInputIndex	= 0;

	if(bPos==TRUE)
	{
		g_InputBuffer[nCurFocus].nOrder			= nOrder;
		g_InputBuffer[nCurFocus].Pos.left		= nLeft;
		g_InputBuffer[nCurFocus].Pos.right		= nRight;
		g_InputBuffer[nCurFocus].Pos.top		= nTop;
		g_InputBuffer[nCurFocus].Pos.bottom		= nBottom;
	}	
	memset(g_InputBuffer[nCurFocus].szBuffer, 0, MAX_INPUT_BUFFER_SIZE);

	if(nInputType==0)
	{
		g_dwCurInputFocus = 0;
		::KillTimer(g_hMainWnd, 1);
	}
	else
	{
		g_dwCurInputFocus = nCurFocus;
		::SetTimer(g_hMainWnd, 1, 500, NULL);
	}						
}
*/
//==============================================================//
//																//
//==============================================================//
#ifdef DEVELOP_MODE
void LoadDebugTile()
{
	//Debug Tile 정보 Load
	for(int k=0; k<MAX_KIND_OF_DEBUG_TILE; k++)
	{
		for(int p=0; p<MAX_DEBUG_TILE_NUM; p++)
		{
			char szFile[ _MAX_PATH ] = {0,};
			wsprintf(szFile, "test_att%d.MOD", k+1);
			g_TileAttr[k][p] = CreateHandleObject(GetFile( szFile, DATA_TYPE_DEBUG), NULL, NULL, NULL);
			HideObject(g_TileAttr[k][p]);
		}
	}
	//--
}

void UnloadDebugTile()
{
	g_bShowTileAttr = 0;
	for(int k=0; k<MAX_KIND_OF_DEBUG_TILE; k++)
	{
		for(int p=0; p<MAX_DEBUG_TILE_NUM; p++)
		{
			if(g_TileAttr[k][p])
				DeleteHandleObject(g_TileAttr[k][p]);
		}
	}
}
#endif


BOOL SendPacketRequestDungeonInfo(WORD wDungeonID)
{
	DUNGEON_DATA_EX* pDungeon = (DUNGEON_DATA_EX*)g_pDungeonTable->GetDungeonInfo(wDungeonID);
					
	if(pDungeon && !pDungeon->bReceivedInfo)
	{
		CTWS_REQUEST_DUNGEON_INFO packet;
		packet.wDungeonID = wDungeonID;
		g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_WORLD );

		pDungeon->bReceivedInfo = 2;		//0이면 아직 안받았슴, 1이면 정보 받았슴, 2이면 정보 대기중 
		return TRUE;
	}
	else if(!pDungeon)
	{
		CTWS_REQUEST_DUNGEON_INFO packet;
		packet.wDungeonID = wDungeonID;
		g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_WORLD );

		return TRUE;
	}

	return FALSE;
}


void OpenQuantityWnd(BYTE bType)
{
	// 수량창윈도우를 연다.
	// 나중에 정리해야할 함수. reyakolla
	CInterface*			pInterface		= CInterface::GetInstance();
	CQuantityWnd*		pQuantityWnd	= CQuantityWnd::GetInstance();
	CGuildExWnd*		pGuildExWnd		= CGuildExWnd::GetInstance();

	if(pQuantityWnd->m_bReq==FALSE)
	{
		CChatWnd* pChatWnd = CChatWnd::GetInstance();

		if(g_pGVDungeon->bChatMode)
			ChatModeOff(INPUT_BUFFER_19);
		
		if(pGuildExWnd->GetActive()==TRUE)
			pGuildExWnd->SetActive(FALSE);

		if(pChatWnd->GetActive()==TRUE)
			pChatWnd->SetActive(FALSE);

		g_pInputManager->ClearInput(INPUT_BUFFER_19);

		if(pQuantityWnd->GetActive()==FALSE)
			pQuantityWnd->OpenWnd(bType);
		else
			pQuantityWnd->SetActive(FALSE);

		RECT rt;
		rt.left		= (LONG)pQuantityWnd->m_fPosX+124-7;
		rt.right	= (LONG)pQuantityWnd->m_fPosX+124;
		rt.top		= (LONG)pQuantityWnd->m_fPosZ+83;
		rt.bottom	= (LONG)pQuantityWnd->m_fPosZ+95;
		int nOrder	= pQuantityWnd->GetStartOrder()+2;						

		g_pInputManager->InitializeInput(INPUT_BUFFER_5, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
		g_pInputManager->SetFocusInput(INPUT_BUFFER_5);
				
		// 재조정 //
		pInterface->SetWnd(QUANTITY_WND);
	} 
}


void ChatMsg()
{
	CQuantityWnd*	pQuantityWnd	= CQuantityWnd::GetInstance();
	CChatWnd*		pChatWnd		= CChatWnd::GetInstance();
	CGuildExWnd*	pGuildExWnd		= CGuildExWnd::GetInstance();
	CGuildRankWnd*	pGuildRankWnd	= CGuildRankWnd::GetInstance();
	CGuildEditWnd*	pGuildEditWnd	= CGuildEditWnd::GetInstance();
	CGameMenuWnd*	pGameMenuWnd = CGameMenuWnd::GetInstance();

	if(!pQuantityWnd->GetActive())
	{
		g_pInputManager->GetInputBuffer(INPUT_BUFFER_19);
		g_pInputManager->GetInputBuffer(INPUT_BUFFER_6);
								
		if(!g_pGVDungeon->bChatMode)
		{
			// 채팅 모드 설정 //
#if !IS_KOREA_LOCALIZING() // 일본판의 경우 대화모드를 기본으로 한다.
			if(GetAsyncKeyState(VK_CONTROL) & 0x80000)
				pChatWnd->m_byChatMode = CHAT_MODE_MESSAGE;
			else
				pChatWnd->m_byChatMode = CHAT_MODE_CHAT;
#else
			if(GetAsyncKeyState(VK_CONTROL) & 0x80000)
				pChatWnd->m_byChatMode = CHAT_MODE_CHAT;
			else
				pChatWnd->m_byChatMode = CHAT_MODE_MESSAGE;
#endif			
			pChatWnd->SetActive();			
			ChatModeOn(INPUT_BUFFER_19);

#ifdef _USE_IME			
			POINT pChatPoint;
			pChatPoint.x	= 10;
			pChatPoint.y	= 635;
			GET_IMEEDIT()->SetEditIndex(1);
			GET_IMEEDIT()->ActivateIME(pChatPoint, __MAX_BUUFER_SIZE__);

			ShowChatEditBackground(TRUE, GET_IMEEDIT()->GetOrder()-1);
#endif	
			if(pChatWnd->m_byClipType==1)
			{
				g_pInputManager->ClearInput(INPUT_BUFFER_19);
				g_pInputManager->InsertCharacter(INPUT_BUFFER_19, pChatWnd->m_szClip, lstrlen(pChatWnd->m_szClip));
				
				pChatWnd->m_byClipType = 0;
				return;
			}			
		}
		else
		{
			if(pGuildExWnd->GetActive()==TRUE)
			{													
				pGuildExWnd->GuildMsg();
			}			
			else if(pGuildRankWnd->GetActive()==TRUE)
			{
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_8);
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_9);
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_10);
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_11);
				
				POINT pChatPoint;

				if(pGuildRankWnd->GetActive()==TRUE)
				{
					pChatPoint.x = long(pGuildRankWnd->m_fPosX+58);

					switch(g_pInputManager->GetCurFocusInputID())
					{
					case INPUT_BUFFER_8:					
						g_pInputManager->SetFocusInput(INPUT_BUFFER_9);					
#ifdef	_USE_IME
						pChatPoint.y = long(pGuildRankWnd->m_fPosZ+60);
						g_pInputManager->ClearInput(INPUT_BUFFER_8);
						g_pInputManager->InsertCharacter(INPUT_BUFFER_8, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));
						
						GET_IMEEDIT()->DeactivateIME();					
						GET_IMEEDIT()->SetImeEditString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_9));
#endif
						break;
					case INPUT_BUFFER_9:					
						g_pInputManager->SetFocusInput(INPUT_BUFFER_10);
#ifdef	_USE_IME
						pChatPoint.y = long(pGuildRankWnd->m_fPosZ+75);
						g_pInputManager->ClearInput(INPUT_BUFFER_9);
						g_pInputManager->InsertCharacter(INPUT_BUFFER_9, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));
						
						GET_IMEEDIT()->DeactivateIME();	
						GET_IMEEDIT()->SetImeEditString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_10));
#endif
						break;
					case INPUT_BUFFER_10:					
						g_pInputManager->SetFocusInput(INPUT_BUFFER_11);
#ifdef	_USE_IME
						pChatPoint.y = long(pGuildRankWnd->m_fPosZ+90);
						g_pInputManager->ClearInput(INPUT_BUFFER_10);
						g_pInputManager->InsertCharacter(INPUT_BUFFER_10,(LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));
						
						GET_IMEEDIT()->DeactivateIME();					
						GET_IMEEDIT()->SetImeEditString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_11));
#endif
						break;
					case INPUT_BUFFER_11:
						g_pInputManager->SetFocusInput(INPUT_BUFFER_8);					
#ifdef	_USE_IME
						pChatPoint.y = long(pGuildRankWnd->m_fPosZ+45);
						g_pInputManager->ClearInput(INPUT_BUFFER_11);
						g_pInputManager->InsertCharacter(INPUT_BUFFER_11, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));
						
						GET_IMEEDIT()->DeactivateIME();
						GET_IMEEDIT()->SetImeEditString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_8));
#endif
						break;											
					}
#ifdef	_USE_IME
					GET_IMEEDIT()->SetEditIndex(1);				
					GET_IMEEDIT()->ActivateIME(pChatPoint, MAX_RANK_REAL_NAME, pGuildRankWnd->GetStartOrder()+2);
#endif			
				}				
			}
			else if(pGuildEditWnd->GetActive()==TRUE)
			{

			}
			else
			{				
				BOOL	bChk = FALSE;
				char*	pszToken = NULL;
				char	szTemp[1024] = {0,};
				
				if(g_pInputManager->GetCurFocusInputID()==INPUT_BUFFER_19)
				{
#ifdef _USE_IME
					LPCSTR lpszName = GET_IMEEDIT()->GetImeEditString();			
					g_pInputManager->ClearInput(INPUT_BUFFER_19);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_19, (LPSTR)lpszName, lstrlen(lpszName));
					GET_IMEEDIT()->ClearImeEditString();
#endif
					__lstrcpyn(szTemp, g_pInputManager->GetInputBuffer(INPUT_BUFFER_19), lstrlen(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19)));

					pszToken = strtok(szTemp, " ");

					if(pszToken==NULL)
					{

#if !IS_KOREA_LOCALIZING() // 일본어판의 경우, 입력없이 엔터를 누른 경우, 채팅창 닫히도록.. modified by deepdark.
						pChatWnd->SetActive(FALSE);
						g_pGVDungeon->bChatMode = FALSE;
						g_pInputManager->ClearInput(INPUT_BUFFER_19);
						g_pInputManager->SetFocusInput(INPUT_BUFFER_0);
						
						GET_IMEEDIT()->DeactivateIME();
						GET_IMEEDIT()->SetEditIndex(0);
						ShowChatEditBackground(FALSE, 0);
#else 
						if(pChatWnd->m_byChatMode==CHAT_MODE_MESSAGE)
						{
							pChatWnd->SetActive(FALSE);
							ChatModeOff(INPUT_BUFFER_19, 0, TRUE);
							IMESetEdit(0);	
						}
						else
							g_pInputManager->SetFocusInput(INPUT_BUFFER_19);
#endif // modified ends..				

					}
					else
					{

						int nRt = ConvString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19));

						if(nRt==1)
						{																									
							ErrMessage(g_Message[ETC_MESSAGE58].szMessage, 0xFFFF0000);
							g_pInputManager->ClearInput(INPUT_BUFFER_19);
							g_pInputManager->SetFocusInput(INPUT_BUFFER_19);
							
							return;
						}

// 중국의 경우 금지단어도 채팅에 포함 안되게 한다.... -_-
#if IS_CHINA_LOCALIZING()						
						nRt = NoConvString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19));
			
						if(nRt==1)
						{				
							ErrMessage(g_Message[ETC_MESSAGE59].szMessage, 0xFFFF0000); //"금지 단어입니다. 사용할 수 없습니다."
							g_pInputManager->ClearInput(INPUT_BUFFER_19);
							g_pInputManager->SetFocusInput(INPUT_BUFFER_19);
							return;									
						}
#endif

						if( pGameMenuWnd->m_bChatEmoticon ) // sung-han 2005-03-30 채팅시에 자동 이모티콘의 반응 여부
						{
							nRt = EmoticonString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19));

							if (nRt > 0)
							{
								CTDS_EMOTICON packet;
								packet.bWhatEmoticon = byte(nRt);
								g_pNet->SendMsg((char *)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
							}
						}

						
						CScript* pScript = CScript::GetInstance(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19));
						int nChk = pScript->ChkCommand();

						GET_TOKEN_COMMAND()->InputString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19));
						if (GET_TOKEN_COMMAND()->IsCommand())
						{	
							GET_TOKEN_COMMAND()->Execute();		
							g_pInputManager->ClearInput(INPUT_BUFFER_19);
							g_pInputManager->SetFocusInput(INPUT_BUFFER_19);	
							return;
						}
						else
						{
							// 채팅 모드 검사해서 채팅 메세지 보내기 //
							// 2005.01.31 김영대 영자 모드 귓말 안되는버그 수정 
							switch(pChatWnd->m_byChatType)
							{
								case CHAT_TYPE_ALL :
									pChatWnd->ChatDefault(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19));
									break;
								case CHAT_TYPE_PARTY :
									pChatWnd->ChatParty(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19));
									break;
								case CHAT_TYPE_GUILD :
									pChatWnd->ChatGuild(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19));
									break;
								case CHAT_TYPE_WHISPER :
									pChatWnd->ChatWhisper(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19), pScript);
									break;
								case CHAT_TYPE_FRIEND :
									pChatWnd->ChatFriend(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19));
									break;
							}
						}
// end of !NEW_COMMAND
					}
				}
				else if(g_pInputManager->GetCurFocusInputID()==INPUT_BUFFER_6)
				{
#ifdef _USE_IME

					LPCSTR lpszName = GET_IMEEDIT()->GetImeEditString();			
					
					g_pInputManager->ClearInput(INPUT_BUFFER_6);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_6, (LPSTR)lpszName, lstrlen(lpszName));					
					// sung-han 2005-01-24 귓말버그수정
					//---------------------------------------------------------------------
					pChatWnd->SetName((char*)lpszName, pChatWnd->GetActive());//sung-han
					//---------------------------------------------------------------------
										
					POINT pChatPoint;

					pChatPoint.x	= 10;
					pChatPoint.y	= 635;
	
					GET_IMEEDIT()->SetEditIndex(1);
					GET_IMEEDIT()->ClearImeEditString();
					GET_IMEEDIT()->ActivateIME(pChatPoint, __MAX_BUUFER_SIZE__);
					ShowChatEditBackground(TRUE, GET_IMEEDIT()->GetOrder() - 1);
#endif
					ChatModeOn(INPUT_BUFFER_19);
					bChk = TRUE;
				}

				if(bChk==FALSE)
				{
					if(pChatWnd->m_byChatMode==CHAT_MODE_MESSAGE && pChatWnd->GetActive())
					{
						pChatWnd->SetActive(FALSE);
						ChatModeOff(INPUT_BUFFER_19, 0, TRUE);
						IMESetEdit(0);
					}
				}
			}
		}
		g_pInputManager->ClearInput(INPUT_BUFFER_19);
		
	}
	else
	{
		pQuantityWnd->QuantityMsg();
	}
}

void ErrMessage(char* szInfo, DWORD dwColor, BYTE byType)
{
	DisplayMessageAdd(szInfo, dwColor, TRUE, byType);							
	_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
}

void InputChatName(BOOL bChk)
{
	if(bChk==FALSE)
	{
		g_pGVDungeon->bChatMode = TRUE;
		g_pInputManager->ClearInput(INPUT_BUFFER_19);
		g_pInputManager->ClearInput(INPUT_BUFFER_6);
		g_pInputManager->SetFocusInput(INPUT_BUFFER_6);
		ChangeLanguage(TRUE);
	}
	else
	{
		g_pInputManager->ClearInput(INPUT_BUFFER_19);
		g_pInputManager->SetFocusInput(INPUT_BUFFER_6);
	}
}

void SetKey(int nKey)
{
	if( g_pThisDungeon->IsStadium() && g_pMainPlayer->m_dwGuildWarFlag == G_W_F_OBSERVER )
		return;

	CUserInterface*	pUserInterface	= CUserInterface::GetInstance();
	CQuantityWnd*	pQuantityWnd	= CQuantityWnd::GetInstance();
	CInterface*		pInterface		= CInterface::GetInstance();

	// emoticon 단축키 처리  
	if (ShortKeyEmoticon((WORD)nKey, 0x30, 0x39) || ShortKeyEmoticon((WORD)nKey, 0x60, 0x69))
		return;
			
	if(pQuantityWnd->GetActive())
	{
		// 숫자 키만 사용할수있다 //
		if(nKey>=0x30 && nKey<=0x39)
		{
 			if(pQuantityWnd->m_byDrawIndex<=10)
			{					
				unsigned char ch = (unsigned char)nKey;

				memcpy(pQuantityWnd->m_szMoney + pQuantityWnd->m_byDrawIndex, &ch, 1);

				if(pQuantityWnd->m_byDrawIndex<10)							
					pQuantityWnd->m_byDrawIndex++;

				InputQuantity();

				int nSize = lstrlen(pQuantityWnd->m_szMoney);

				if(nSize!=pQuantityWnd->m_byDrawIndex)
				{
					pQuantityWnd->m_byDrawIndex--;
					g_pInputManager->GetInputRect(INPUT_BUFFER_5).left= (LONG)pQuantityWnd->m_fPosX+124-(pQuantityWnd->m_byDrawIndex*6)-7-(pQuantityWnd->m_byDrawIndex/3+1)*6;
					g_pInputManager->GetInputRect(INPUT_BUFFER_5).right=(LONG)pQuantityWnd->m_fPosX+124;					
				}
			}
		}								
	}
	else
	{			
		if(g_bKeyChkUp==FALSE)
		{
//			SPLITRESUID nUID;
//			Effect*		pEffect;				
			CSkillWnd*	pSkillWnd = CSkillWnd::GetInstance();
		
			if(!g_pGVDungeon->bChatMode)
			{				
				for(DWORD i = 0; i < 16; i++)
				{
					if(nKey==(WPARAM)g_sKeyConfig.snKey[18+i])
					{
						int nSkill_Idx = 1;

						if(pSkillWnd->m_bySkillIndex==0)
						{
							if(g_pMainPlayer->m_nSkillKey[i]!=-1)
							{
								if(g_pMainPlayer->m_nPosSK[i]==0)
									g_pMainPlayer->SetSkillChangeLR((BYTE)g_pMainPlayer->m_nSkillKey[i], 0);
								else if(g_pMainPlayer->m_nPosSK[i]==1)
									g_pMainPlayer->SetSkillChangeLR((BYTE)g_pMainPlayer->m_nSkillKey[i], 1);																								
							}
						}
						else if(pSkillWnd->m_bySkillIndex==1)
						{
							if(g_Mouse.MousePos.x>=116 && g_Mouse.MousePos.x<=116+32 &&
								g_Mouse.MousePos.y>=646 && g_Mouse.MousePos.y<=646+32)
							{
								for(int k = 0; k < 16; k++)
								{
									if(g_pMainPlayer->m_nSkillKey[k]==__SKILL_ATTACK__)
										g_pMainPlayer->m_nSkillKey[k] = -1;
								}
								g_pMainPlayer->m_nSkillKey[i] = __SKILL_ATTACK__;
								g_pMainPlayer->m_nPosSK[i] = 0;
								break;
							}						
							
							for(int j = 0; j < g_sSkillListManager.byLeftSkillCnt; j++)
							{
								if(g_pMainPlayer->GetSkillLevel(g_sSkillListManager.byLeftSkill[j])>0)
								{
									int nPosX = 116+(nSkill_Idx%5)*33;
									int nPosY = 646-(nSkill_Idx/5)*33;
									
									if(g_Mouse.MousePos.x>=nPosX && g_Mouse.MousePos.x<=nPosX+32 &&
										g_Mouse.MousePos.y>=nPosY && g_Mouse.MousePos.y<=nPosY+32)
									{
										for(int k = 0; k < 16; k++)
										{
											if(g_pMainPlayer->m_nSkillKey[k]==g_sSkillListManager.byLeftSkill[j])
												g_pMainPlayer->m_nSkillKey[k] = -1;
										}							
										g_pMainPlayer->m_nSkillKey[i] = g_sSkillListManager.byLeftSkill[j];
										g_pMainPlayer->m_nPosSK[i] = 0;
										break;
									}																						
									nSkill_Idx++;
								}
							}							
						}
						else if(pSkillWnd->m_bySkillIndex==2)
						{			
							nSkill_Idx = 0;
							
							for(int j = 0; j < g_sSkillListManager.byRightSkillCnt; j++)
							{
								if(g_pMainPlayer->GetSkillLevel(g_sSkillListManager.byRightSkill[j])>0)
								{
									int nPosX = 116+(nSkill_Idx%5)*33;
									int nPosY = 646-(nSkill_Idx/5)*33;
									
									if(g_Mouse.MousePos.x>=nPosX && g_Mouse.MousePos.x<=nPosX+32 &&
										g_Mouse.MousePos.y>=nPosY && g_Mouse.MousePos.y<=nPosY+32)
									{
										for(int k = 0; k < 16; k++)
										{
											if(g_pMainPlayer->m_nSkillKey[k]==g_sSkillListManager.byRightSkill[j])
												g_pMainPlayer->m_nSkillKey[k] = -1;
										}							
										g_pMainPlayer->m_nSkillKey[i] = g_sSkillListManager.byRightSkill[j];
										g_pMainPlayer->m_nPosSK[i] = 1;
										break;
									}																						
									nSkill_Idx++;
								}
							}
						}															
						// Key //
						if(pSkillWnd->m_bySkillIndex!=0)
						{
							CTWS_SKILL_KEY pPacket;

							for(int i = 0; i < 16; i++)
							{
								pPacket.bSkillKey[i]	= BYTE((g_pMainPlayer->m_nSkillKey[i]==-1) ? 0 : g_pMainPlayer->m_nSkillKey[i]);
								pPacket.bPosKey[i]		= BYTE(g_pMainPlayer->m_nPosSK[i]);
							}
							g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
						}
					}
				}
			}
			
			if(nKey==g_sKeyConfig.snKey[__KEY_GROUP_OPEN__])
			{
				CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();
	
				if(!g_pGVDungeon->bChatMode)
				{
					if(pGroupWnd->GetActive()==FALSE)
					{
						pGroupWnd->SetActive();
					}
					else
						pGroupWnd->SetActive(FALSE);
				}
			}

			// Belt //
			if(	nKey==g_sKeyConfig.snKey[__KEY_ITEM_BELT1__] ||
				nKey==g_sKeyConfig.snKey[__KEY_ITEM_BELT2__] ||
				nKey==g_sKeyConfig.snKey[__KEY_ITEM_BELT3__] ||
				nKey==g_sKeyConfig.snKey[__KEY_ITEM_BELT4__] ||
				nKey==g_sKeyConfig.snKey[__KEY_ITEM_BELT5__] ||
				nKey==g_sKeyConfig.snKey[__KEY_ITEM_BELT6__] ||
				nKey==g_sKeyConfig.snKey[__KEY_ITEM_BELT7__] ||
				nKey==g_sKeyConfig.snKey[__KEY_ITEM_BELT8__])
			{
				if(g_pMainPlayer->GetStatus()!=UNIT_STATUS_DEAD)
				{
					if(g_pMainPlayer->GetStatus()!=UNIT_STATUS_PORTAL_MOVING)
					{
						if(!g_pGVDungeon->bChatMode)
						{	
							if (g_pMainPlayer->m_bMatching)
							{
								//"대전중에 벨트 아이템을 사용하지 못합니다."
								DisplayMessageAdd(g_Message[ETC_MESSAGE520].szMessage, 0xFFFF0000 );
								return;
							}

							BYTE bKeyItemBelt = 0;

							if(nKey==g_sKeyConfig.snKey[__KEY_ITEM_BELT1__])
								bKeyItemBelt = __KEY_ITEM_BELT1__;
							else if(nKey==g_sKeyConfig.snKey[__KEY_ITEM_BELT2__])
								bKeyItemBelt = __KEY_ITEM_BELT2__;
							else if(nKey==g_sKeyConfig.snKey[__KEY_ITEM_BELT3__])
								bKeyItemBelt = __KEY_ITEM_BELT3__;
							else if(nKey==g_sKeyConfig.snKey[__KEY_ITEM_BELT4__])
								bKeyItemBelt = __KEY_ITEM_BELT4__;
							else if(nKey==g_sKeyConfig.snKey[__KEY_ITEM_BELT5__])
								bKeyItemBelt = __KEY_ITEM_BELT5__;
							else if(nKey==g_sKeyConfig.snKey[__KEY_ITEM_BELT6__])
								bKeyItemBelt = __KEY_ITEM_BELT6__;						
							else if(nKey==g_sKeyConfig.snKey[__KEY_ITEM_BELT7__])
								bKeyItemBelt = __KEY_ITEM_BELT7__;								
							else if(nKey==g_sKeyConfig.snKey[__KEY_ITEM_BELT8__])
								bKeyItemBelt = __KEY_ITEM_BELT8__;

							BYTE byZipCode = BYTE(bKeyItemBelt-__KEY_ITEM_BELT1__);

							BOOL bChk = FALSE;
							
							if(!bChk)
								bChk = ItemUsedSupplies(&g_pMainPlayer->m_pBelt[byZipCode], byZipCode, ZODIAC_USE_TYPE_BELT);
							if(!bChk)
								bChk = ItemUsedConsumable(&g_pMainPlayer->m_pBelt[byZipCode], byZipCode, ZODIAC_USE_TYPE_BELT);
							if(!bChk)
								bChk = ItemUsedZodiac(&g_pMainPlayer->m_pBelt[byZipCode], byZipCode, ZODIAC_USE_TYPE_BELT);
						}
					}
				}
				else
				{
					// "포탈 이동 중에는 사용할 수 없습니다."
					DisplayMessageAdd(g_Message[ETC_MESSAGE502].szMessage, 0xFFFF2CFF); 
				}
			}

			if(nKey==g_sKeyConfig.snKey[__KEY_ITEM_GUARDIAN__])
			{			
				if(g_pMainPlayer->GetStatus()!=UNIT_STATUS_PORTAL_MOVING)
				{
					// 가디언 물약 사용할 때 //
					if(CUserInterface::GetInstance()->m_bGuardian==TRUE)
					{
						int nVal = g_pMainPlayer->m_GuardianItem.m_wItemID/ITEM_DISTRIBUTE;

						if(nVal==ITEM_SUPPLIES_INDEX)
						{
							CTDS_ITEM_PICKUP	ItemPickup;
							ItemPickup.bInv			= 44;
							ItemPickup.bSectionNum	= 1;
							ItemPickup.i64ItemID	= 0;
							ItemPickup.bZipCode		= 0;				
							g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
						}
					}
				}
				else
				{
					// "포탈 이동 중에는 사용할 수 없습니다."
					DisplayMessageAdd(g_Message[ETC_MESSAGE502].szMessage, 0xFFFF2CFF); 
				}
			}

			if(nKey==g_sKeyConfig.snKey[__KEY_SKILL_OPEN__])
			{	
				if(!g_pGVDungeon->bChatMode)
				{
					pSkillWnd->SetActive(!pSkillWnd->GetActive());
					pUserInterface->m_byOrderCheck = SKILL_WND;
				}
			}
			
			if (nKey == g_sKeyConfig.snKey[__KEY_SEE_ALL_DROPPED_ITEMS])
			{
				if (!g_pGVDungeon->bChatMode)
				{
					selectedItemsForTooltipRendering.clear();

					CItemTradeShopWnd* pItemTradeShopWnd = CItemTradeShopWnd::GetInstance();

					ListNode<ITEM>* pNode;
					ITEM* pItem;

					pNode = g_pItemHash->GetHead();

					while (pNode)
					{
						pItem = (ITEM*)pNode->pData;

						if (pItem)
						{
							selectedItemsForTooltipRendering.push_back(pItem);
						}
						pNode = pNode->pNext;
					}
				}
			}

			if(nKey==g_sKeyConfig.snKey[__KEY_ITEM__])
			{	
				if(!g_pGVDungeon->bChatMode)
				{
					CItemTradeShopWnd* pItemTradeShopWnd = CItemTradeShopWnd::GetInstance();

					// 타이머 //
					g_dwCurrTick = timeGetTime();

					if((g_dwCurrTick-g_dwPrevTick)<1000)					
						return;										
					if(!((g_pMainPlayer->GetStatus()==UNIT_STATUS_NORMAL) || (g_pMainPlayer->GetStatus()==UNIT_STATUS_WALKING) || (g_pMainPlayer->GetStatus()==UNIT_STATUS_RUNNING)))
						return;
					if(pItemTradeShopWnd->GetActive()==TRUE)
						return;

					g_dwPrevTick = timeGetTime();

					memset(g_bKeyItem_Large, 0, MAX_INV_LARGE_POOL);							
					
					ListNode<ITEM>*	pNode;
					ITEM*			pItem;
					BOOL			bChk = FALSE;

					pNode = g_pItemHash->GetHead();

					while(pNode)
					{
						pItem = (ITEM*)pNode->pData;

						if(pItem)
						{
							if(g_pMainPlayer->m_dwItemInfoType==2)
							{
								if(pItem->Item.GetSerial()->i64Serial==g_pMainPlayer->m_InfoItem.GetSerial()->i64Serial)
								{
									CUserInterface* pUserInterface = CUserInterface::GetInstance();
									pUserInterface->SetMousePointerType(__MOUSE_POINTER_DEFAULT__);
									
									memset(&g_pMainPlayer->m_InfoItem, 0, sizeof( CItem ) );
									g_pMainPlayer->m_dwItemInfoType = 0;																						
								}
							}

							if(pItem->v3ItemPos.x>=g_pMainPlayer->m_v3CurPos.x-(TILE_SIZE+TILE_SIZE/2.0f) &&
								pItem->v3ItemPos.x<=g_pMainPlayer->m_v3CurPos.x+(TILE_SIZE+TILE_SIZE/2.0f))
							{
								if(pItem->v3ItemPos.z>=g_pMainPlayer->m_v3CurPos.z-(TILE_SIZE+TILE_SIZE/2.0f) &&
									pItem->v3ItemPos.z<=g_pMainPlayer->m_v3CurPos.z+(TILE_SIZE+TILE_SIZE/2.0f))
								{
									CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();

									if(pQuantityWnd->GetActive())
										return;

									if(	g_pMainPlayer->GetStatus()!=UNIT_STATUS_NORMAL &&
										g_pMainPlayer->GetStatus()!=UNIT_STATUS_WALKING &&
										g_pMainPlayer->GetStatus()!=UNIT_STATUS_RUNNING)
										return;

									// 대만은 무게 포화상태가 되면 아이템을 집을수 없게 막는다.
#if IS_TAIWAN_LOCALIZING()
									if ((g_pMainPlayer->GetAverageWeight()) >= 100.f )
									{
										// "포화무게한도에 도달하여 달리기가 안되며, 포션의 사용 딜레이가 증가합니다."
										DisplayMessageAdd(g_Message[ETC_MESSAGE986].szMessage, 0xffff0000);			

										return;
									}
#endif
									if(g_ItemMoveManager.GetNewItemMoveMode())
									{
										g_ItemMoveManager.TileToInvPickupItem(pItem);
									}
									else
									{
 										SendPickupItem(pItem, FALSE, FALSE);											
									}									

									bChk = TRUE;
								}
							}		
						}
						pNode = pNode->pNext;
					}
					
					if(bChk==FALSE)
					{
						// "아이템이 없습니다."
						DisplayMessageAdd(g_Message[ETC_MESSAGE144].szMessage, 0xFFFFC309, CGameMenuWnd::GetInstance()->m_bSystemMsgFlag); 
					}
				}
			}

			if(nKey==g_sKeyConfig.snKey[__KEY_CHARACTER_OPEN__])
			{				
				if(!g_pGVDungeon->bChatMode)
				{
					CCharWnd* pCharWnd = CCharWnd::GetInstance();		
					
					if(pCharWnd->GetActive()==FALSE)
						pCharWnd->SetActive();
					else					
						pCharWnd->SetActive(FALSE);

					pUserInterface->m_byOrderCheck = CHAR_WND;
				}
			}

			// 가디언창 오픈
			if(nKey==g_sKeyConfig.snKey[__KEY_GUARDIAN_OPEN__])
			{				
				if(!g_pGVDungeon->bChatMode)
				{
					CGuardianWnd* pGuardianWnd = CGuardianWnd::GetInstance();		
					
					if(pGuardianWnd->GetActive()==FALSE)
						pGuardianWnd->SetActive();
					else					
						pGuardianWnd->SetActive(FALSE);

					pUserInterface->m_byOrderCheck = GUARDIAN_WND;
				}
			}
			
			if(nKey==g_sKeyConfig.snKey[__KEY_OPTION_OPEN__])
			{
				if(!g_pGVDungeon->bChatMode)
				{					
					CGameMenuWnd* pGameMenuWnd = CGameMenuWnd::GetInstance();
					
					if(pGameMenuWnd->GetActive()==TRUE)
						pGameMenuWnd->SetActive(FALSE);
					else
					{
						pGameMenuWnd->SetActive();
						pGameMenuWnd->OpenWnd();
						pUserInterface->m_byOrderCheck = GAMEMENU_WND;
					}
				}
			}			

			if(nKey==g_sKeyConfig.snKey[__KEY_WEAPON_SWITCH__])
			{
				if(!g_pGVDungeon->bChatMode)
				{						
					if(g_pMainPlayer->GetStatus()==UNIT_STATUS_DEAD)
						return;

					if(g_pMainPlayer->GetStatus()==UNIT_STATUS_NORMAL)
					{
						// Item Weapon Switch.
						if(g_pMainPlayer->m_pEquip[EQUIP_TYPE_LHAND1].m_wItemID==0	&&
							g_pMainPlayer->m_pEquip[EQUIP_TYPE_LHAND2].m_wItemID==0 &&
							g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID==0	&&
							g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID==0	)
						{
							return;
						}
						else
						{					
							CItemWnd* pItemWnd = CItemWnd::GetInstance();							

							pItemWnd->m_dwCurTime = timeGetTime();
							
							if(pItemWnd->m_dwCurTime-pItemWnd->m_dwPrvTime>2000)
							{
								pItemWnd->m_dwPrvTime = pItemWnd->m_dwCurTime;							

								// SOUND_SYSTEM_WEAPONSWITCH
								_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WEAPONSWITCH, g_v3InterfaceSoundPos, FALSE);

								if(g_ItemMoveManager.GetNewItemMoveMode())
								{
									CTDS_SWITCH_WEAPON packet;
									g_pNet->SendMsg( (char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE );
								}
								else
								{
									CTDS_ITEM_PICKUP ItemPickup;
									ItemPickup.bSectionNum	= 1;
									ItemPickup.i64ItemID	= 0;
									SetItemPacket(&ItemPickup, 17, 0, 0, 0, 0);
									g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );
								}								
							}
						}
					}
				}
			}
			
			if(nKey==g_sKeyConfig.snKey[__KEY_INVENTORY_OPEN__])
			{
				if (g_pMainPlayer->m_bMatching)
					return;
				
				if(!g_pGVDungeon->bChatMode && g_pMainPlayer->GetStatus() != UNIT_STATUS_PORTAL_MOVING)
				{						
					CItemWnd* pItemWnd = CItemWnd::GetInstance();		

					if(pItemWnd->GetActive()==FALSE)
					{
						pItemWnd->SetActive(!pItemWnd->GetActive());
						pUserInterface->m_byOrderCheck = ITEM_WND;
					}
					else
					{
						if(pItemWnd->m_bItemKeyChk==FALSE)
						{
							if(pItemWnd->m_byItemChk==0)
							{
								pItemWnd->SetRender(SPR_OBJ_ITEM_CHK1, FALSE);
								pItemWnd->SetRender(SPR_OBJ_ITEM_CHK2, TRUE);								
								pItemWnd->m_byItemChk	= 1;											
								_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_BTNCLICK, g_v3InterfaceSoundPos, FALSE);
							}
							else
							{
								pItemWnd->SetRender(SPR_OBJ_ITEM_CHK1, TRUE);
								pItemWnd->SetRender(SPR_OBJ_ITEM_CHK2, FALSE);								
								pItemWnd->m_byItemChk	= 0;											
								_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_BTNCLICK, g_v3InterfaceSoundPos, FALSE);
							}
							pItemWnd->m_bItemKeyChk = TRUE;
						}
						else
						{
							pItemWnd->SetActive(FALSE);
						}									
					}
				}
			}
			
			if(nKey==g_sKeyConfig.snKey[__KEY_CHAT_CLAER__])
				InitDungeonMessage();

			// Menu Order 재조정 //
			if(!g_pGVDungeon->bChatMode)
			{																										
				for(int i = 0; i < CUR_INTERFACE; i++)
				{
					if(pInterface->m_pMenu[pUserInterface->GetChkOrderIndex()]->GetOrderIndex()<pInterface->m_pMenu[i]->GetOrderIndex())
						pInterface->m_pMenu[i]->SetOrderIndex(BYTE(pInterface->m_pMenu[i]->GetOrderIndex()-1));
				}
				pInterface->m_pMenu[pUserInterface->GetChkOrderIndex()]->SetOrderIndex(BYTE(pInterface->m_byIndex-1));

				for(int i = 0; i < CUR_INTERFACE; i++)
					pInterface->m_pMenu[i]->SetOrder();
			}
			g_bKeyChkUp = TRUE;
		}
	}
}

void ChatOff()
{
	g_pGVDungeon->bChatMode = FALSE;
	g_pInputManager->ClearInput(INPUT_BUFFER_19);
	g_pInputManager->ClearInput(INPUT_BUFFER_6);
	g_pInputManager->SetFocusInput(INPUT_BUFFER_6);
	
	ChangeLanguage(FALSE);
}

BOOL ShortKeyEmoticon(WORD wParam, int iLowKey, int iHighKey)
{
	BOOL bRet	= FALSE;
	BYTE byAlt	= 0;

	// emoticon 단축키 처리
	// Modified by minjin. 2004. 09. 18.
	// Ctrl + 0 ~ 10, Alt + 0 ~ 10 번 이모티콘 할당..
	// g_bLAlt 는 불안하다.. -_-;;
	if ( g_bLCtrl || GetAsyncKeyState(VK_MENU) & 0X80000)
	{
		if ( GetAsyncKeyState(VK_MENU) & 0X80000 )
		{
			byAlt = 10;
		}
		else
		{
			byAlt = 0;
		}

		if ( wParam >= iLowKey && wParam <= iHighKey )
		{
			CTDS_EMOTICON	packet;
			char ch = char(wParam - iLowKey);

			packet.bWhatEmoticon = BYTE((ch == 0) ? 10 + byAlt : ch + byAlt);

			g_pNet->SendMsg((char *)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
			bRet = TRUE;
		}
	}

/* 
	if (GetAsyncKeyState(VK_MENU) & 0X80000)
	{
		if (wParam >= iLowKey && wParam <= iHighKey)
		{
			CTDS_EMOTICON	packet;
			char ch = wParam - iLowKey;
		
			if (ch == 0)
				packet.bWhatEmoticon = 20;
			else
				packet.bWhatEmoticon = ch+10;

			g_pNet->SendMsg((char *)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
			bRet = TRUE;
		}
	}
*/


	return bRet;
}

void CameraAccelatingMove(void)
{
	// 카메라가 움직여야할 가속도가 아직 남아있다면 ( 이 값은, 마우스 이벤트가 끝난 직후 세팅 된다. )
	/*

	CAMERA_DESC CameraDesc;
	VECTOR3		v3CameraAngleRad;
	FLOAT		fOldPitch;
	FLOAT		fPitch;


	if (0.0f != g_Camera.fAccel_x ||  0.0f != g_Camera.fAccel_y)
	{
		// 가속도 값 만큼 카메라 위치를 보정하고

		if (0.0f != g_Camera.fAccel_x)
		{
			g_pExecutive->GXOGetPosition( g_pMainPlayer->m_hPlayer.pHandle, &v3Pos );
			SetCameraYaw( &v3Pos, DEG01 * CAMERA_Y * g_Camera.fAccel_x * -1.0f );
		}

		if (0.0f < g_Camera.fAccel_y)
		{
			g_pGeometry->GetCameraDesc( &CameraDesc, 0 );
			g_pGeometry->GetCameraAngleRad( &v3CameraAngleRad, 0 );

			fOldPitch	=  ( 180.f / PI * v3CameraAngleRad.x ) * -1.0f;
			fPitch		=  ( ( 180.f / PI * v3CameraAngleRad.x * -1.f ) - ( _CAMERA_ACCEL_MOVE_X_ * g_Camera.fAccel_y ) );
			 
			if( fOldPitch <= __MIN_CAMERA_X__ ) 
			{
				return;
			}
			
			if( fPitch <= __MIN_CAMERA_X__ )
			{
				fPitch = fOldPitch - __MIN_CAMERA_X__;
			}
			else
			{
				fPitch = fOldPitch - fPitch;
			}
			
			g_pExecutive->GXOGetPosition( g_pMainPlayer->m_hPlayer.pHandle, &v3Pos );
			SetCameraPitch( &v3Pos, DEG01 * fPitch );
		}
		else if (0.0f > g_Camera.fAccel_y) 
		{
			g_pGeometry->GetCameraDesc( &CameraDesc, 0 );
			g_pGeometry->GetCameraAngleRad( &v3CameraAngleRad, 0 );
			
			fOldPitch	=  ( 180.f / PI * v3CameraAngleRad.x ) * -1.0f;
			fPitch =  ( 180.f / PI * v3CameraAngleRad.x * -1.0f ) + ( _CAMERA_ACCEL_MOVE_X_ * g_Camera.fAccel_y * -1.0f );
			
			if( fOldPitch >= __MAX_CAMERA_X__ ) 
			{
				return;
			}

			if( fPitch >= __MAX_CAMERA_X__ )
			{
				fPitch = __MAX_CAMERA_X__ - fOldPitch;
			}
			else
			{
				fPitch = fPitch - fOldPitch;
			}
			
			g_pExecutive->GXOGetPosition( g_pMainPlayer->m_hPlayer.pHandle, &v3Pos );
			SetCameraPitch( &v3Pos, DEG01 * fPitch * -1.0f);		

		}

		// 다음번을 위하여 마우스위치를 기억한다.
		g_Mouse.MouseOldPos.x	= g_Mouse.MousePos.x;
		g_Mouse.MouseOldPos.y	= g_Mouse.MousePos.y;

		// 카메라를 청취자로 셋팅 
		SetListener(&v3CameraAngleRad);			
	}
	*/
}

BOOL FindEmptyPosNearDungeon(VECTOR3* vpDungeonPos, VECTOR3 *vpNearPos, BOOL bVillage)
{
	MAP_TILE*	pTile = 0;
	VECTOR3		vPos;
	DWORD		dwCount = 0;

	while(dwCount < 4)
	{
		vPos = *vpDungeonPos;
		dwCount++;
		
		float fDistance = (bVillage) ? 5.0f : 2.0f;

		switch(dwCount)
		{
		case 1:		vPos.x -= (TILE_SIZE_WORLD * fDistance);	break;		//서쪽방향 
		case 2:		vPos.z -= (TILE_SIZE_WORLD * fDistance);	break;		//남쪽방향
		case 3:		vPos.z += (TILE_SIZE_WORLD * fDistance);	break;		//북쪽방향
		case 4:		vPos.x += (TILE_SIZE_WORLD * fDistance);	break;		//동쪽방향 
		}
		
		pTile = g_pMap->GetTile(vPos.x, vPos.z);
		if(!pTile)
			return FALSE;

		if(pTile->wAttr.uAttr != 1)
		{
			vpNearPos->x = vPos.x;
			vpNearPos->z = vPos.z;
			g_pExecutive->GXMGetHFieldHeight(&vpNearPos->y, vPos.x, vPos.z);
			return TRUE;
		}
	}
	return FALSE;
}

void SelectSummonMonster(CMonster* pMonster, BOOL bSelect)
{
	if( bSelect )
	{
		// 반대.
		if (g_pEffectLayer->IsEffectShow(pMonster->m_pEffectSelect))
		{
			pMonster->m_pEffectSelect->bOwn = 1;
			ShowObject( pMonster->m_pEffectSelect->hEffect.pHandle );
		}
		
		pMonster->m_pEffectTarget->bOwn = 2;
		HideObject( pMonster->m_pEffectTarget->hEffect.pHandle );
		
	}
	else
	{
		// 선택되지 않은 사태로 만든다.
		if (g_pEffectLayer->IsEffectShow(pMonster->m_pEffectTarget))
		{
			pMonster->m_pEffectTarget->bOwn = 1;
			ShowObject( pMonster->m_pEffectTarget->hEffect.pHandle );
		}
		
		pMonster->m_pEffectSelect->bOwn = 2;
		HideObject( pMonster->m_pEffectSelect->hEffect.pHandle );		
	}

	pMonster->m_dwTemp[ MONSTER_TEMP_SELECTED ] = bSelect;
}

