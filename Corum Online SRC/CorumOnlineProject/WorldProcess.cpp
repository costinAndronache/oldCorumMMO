#include "stdafx.h"
#include "WorldProcess.h"
#include "DungeonProcess.h"
#include "InitGame.h"
#include "GameControl.h"
#include "DungeonTable.h"
#include "ObjectManager.h"
#include "HashTable.h"
#include "Map.h"
#include "Script.h"
#include "Filter.h"
#include "CommandWnd.h"
#include "Interface.h"
#include "Define.h"
#include "GameMenuWnd.h"
#include "DSMiniMap.h"
#include "CorumResource.h"
#include "DefResource.h"
#include "CDungeonInfoWnd.h"
#include "LoginProcess.h"
#include "ExitWnd.h"
#include "HelpWnd.h"
#include "UserInterface.h"
#include "message.h"
#include "GlobalSoundHandles.h"
#include "Chat.h"
#include "OkWnd.h"
#include "DungeonSiegeInfoWnd.h"
#include "GameNoticeWnd.h"
#include "Guild.h"
#include "Resource.h"
#include "GmTool.h"
#include "ErrorWnd.h"
#include "Messenger.h"
#include "InputManager.h"
#include "WorldWnd.h"
#include "message.h"
#include "EffectProcess.h"
#include "PathFinding\BlockLineManager.h"
#include "PathFinding\SearchModule_Astar.h"
#include "NetworkClient.h"
#include "Effect.h"
#include "Party.h"
#include "WorldMsg.h"
#include "IMEEdit.h"
#include "CodeFun.h"
#include "ChinaBillingRemainTime.h"
#include "RivalGuildWar.h"			//	Rival Guild War Operation.. KJK, LMJ is Working 
//	deathrain
#include "NetworkTimeObserver.h"

BYTE					g_byLen[10];
SMESSAGERQ				g_sMessageRQ[__MAX_READ_MESSAGE__];
SWSCHATMSG				g_sWordChatMsg[__MAX_WORD_QUEUE__];
RECT					g_rt[__MAX_CHAT__];
char					g_sNoticeMessage[MAX_NOTICE_COUNT][MAX_NOTICE_SIZE];

BYTE					g_bPosType			= 0;
BYTE					g_byNoticeCount		= 0;
BOOL					g_bNoticeFlag		= FALSE;
DWORD					g_dwPrevTime		=  0;
DWORD					g_dwNextTime		=  0;

int						g_nScrollPos		= 0;
int						g_nWordStart		= 0;
int						g_nWordEnd			= 0;
int						g_nWrIndex			= 0;
int						g_nWordIndex		= 0;
int						g_nPGSelect			= 0;
int						g_nChatDst			= 0;
int						g_nChatModeExt		= 0;
int						g_nChatId			= 0;
int						g_nMsgInx			= 0;
int						g_nCurScrollPos		= 0;

BOOL					g_bRideChk			= FALSE;
BOOL					g_bDownChk			= FALSE;
BOOL					g_bWorldKeyChkUp	= FALSE;

LPGlobalVariable_World	g_pGVWorld			= NULL;

void InitPacketProcWorld()
{
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_REPLY_DUNGEON_INFO ]							= CmdReplyDungeonInfo;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_DUNGEON_JOIN_FAILED ]							= CmdDungeonJoinFailed;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_CONNECT_DUNGEON_SERVER ]						= CmdConnectDungeonServer;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_COMMAND_FAIL ]									= CmdCommandFail;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_COMMAND_WHISPER]								= CmdWhisper;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_COMMAND_MESSAGE]								= CmdCommandMessage;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_COMMAND_RQ]									= CmdCommandMessageRQ;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_CHAT_USER]										= CmdChatUser;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_REFRESHEVENT_DUNGEON_STARTSIEGE]				= CmdRefreshEventDungeon_StartSiege;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_REFRESHEVENT_DUNGEON_LEVELUP]					= CmdRefreshEventDungeon_LevelUp;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_REFRESHEVENT_DUNGEON_CREATEPRODUCTION]			= CmdRefreshEventDungeon_CreateProduction;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_REFRESHEVENT_DUNGEON_ENDSIEGE]					= CmdRefreshEventDungeon_EndSiege;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_REFRESHEVENT_DUNGEON_CHANGE_OWNER]				= CmdRefreshEventDungeon_Change_Owner;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_REFRESHEVENT_DUNGEON_CHANGE_OWNER_GUILD]		= CmdRefreshEventDungeon_Change_Owner_Guild;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILD_USER]									= CmdGuildUser;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILD_USERINFO]								= CmdGuildUserInfo;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILD_USERINFODEL]								= CmdGuildUserInfoDel;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILD_INFORT]									= CmdGuildInfo;				
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_SKILL_KEY]										= CmdSkillKey;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_NOTICE]										= CmdNotice;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_COMMAND_MSGRT]									= CmdCommandMsg;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PARTY_SETRT]									= CmdPartySet;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_EVENT_DUNGEON_INFO]							= CmdEventDungeonInfo;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_EVENT_DUNGEON_STATE_INFO]						= CmdEventStateChange;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_REFRESHEVENT_DUNGEON_ACCENTRANCE_ACQUISITION]	= CmdRefreshEventDungeon_AccEntrance_Acquisition;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_REFRESHEVENT_DUNGEON_OPERATIONMODE]			= CmdRefreshEventDungeon_OperationMode;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_REFRESHEVENT_DUNGEON_ACCEXP_ACQUISITION]		= CmdRefreshEventDungeon_AccExp_Acquisition;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_REFRESHEVENT_DUNGEON_ENTRANCE_EDIT]			= CmdRefreshEventDungeon_Entrance_Edit;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_WORLD_APPEAR ]									= CmdWorldAppear;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_WORLD_MOVE ]									= CmdWorldMove;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_WORLD_STOP ]									= CmdWorldStop;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_WORLD_PARTY_USER_INFO ]						= CmdWorldPartyUserInfo;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_WORLD_SET_POSITION ]							= CmdWorldSetPosition;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PARTY_USER_TRY_TO_CONNECT ]					= CmdPartyUserTryToConnect;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_WORLD_DISAPPEAR ]								= CmdWorldDisappear;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_AIRSHIP_RIDING ]								= CmdAirshipRiding;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_AIRSHIP_GETOFF ]								= CmdAirshipGetoff;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_CHECK_CONNECTION ]								= CmdCheckConnection;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_REFRESHEVENT_DUNGEON_GUARDIANITEM ]			= CmdRefreshEventDungeon_GuardianItem;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_REFRESHEVENT_DUNGEON_MAGICFIELDARRAYITEM ]		= CmdRefreshEventDungeon_MagicFieldArrayItem;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_EVENT_DUNGEON_ENTER_FAILED ]					= CmdEvnetDungeonEnterFailed;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILD_OFFLINE_USER]							= CmdGuildoffLine;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILD_OFFLINE_USER_DELETE]						= CmdGuildoffLineDelete;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_MESSENGER_STATUSRT]							= CmdFriendStaus;		
	//	deathrain
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_TIMER_SYNC]									= CmdTimerSync;
	//
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_BILLING_INFO ]									= CmdBillingInfo;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PARTY_ENTRYRT]									= CmdPartyEntry;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PARTY_INFOREQUESTRT]							= CmdPartyInfoRequest;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILDWAR_MESSAGE]								= CmdGuildWarMessageGuild;
	
#ifdef GM_TOOL

	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_INVALID_GM]									= CmdInvalidGM;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GM_LOGIN_SUCCESS]								= CmdGMLoginSuccess;
	
#endif
	
	// GM COMMAND //
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GM_COMMAND_RESULT]								= CmdGmCommandResult;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_REFRESHEVENT_DEFENSECOUNT]						= CmdRefreshDefenseCount;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_TEAM_MATCH ]									= Recv_CMD_TEAM_MATCH;

	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_CB_MESSAGE]									= CmdCBMessage;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILD_MATCHBEGIN_SETTING]							= CmdGuildMatchBeginSetting;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILD_MATCH_BEGIN]									= CmdGuildMatchBegin;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILD_MATCH_STATUS]								= CmdGuildMatchStatus;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILD_MATCH_STATUS_RESPON]						= CmdGuildMatchStatusRespon;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILD_WAR_INVITE]									= CmdGuildWarInvite;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILD_WAR_BBS]									= CmdGuildWarBBS;
}

BOOL InitGameWorld()
{	
	g_bLoadingChk	= FALSE;
	g_nWrIndex		= 0;
	g_pGVWorld		= new GLOBAL_VARIABLE_WORLD;
	memset(g_pGVWorld, 0, sizeof(GLOBAL_VARIABLE_WORLD));
	SetRenderMode(RENDER_MODE_NORMAL);	

#ifdef _USE_IME

	GET_IMEEDIT()->SetEditIndex(0);
	GET_IMEEDIT()->EnableSpaceKey(TRUE);

#endif

	_CHECK_MEMORY()	
	g_Camera.v3AxsiY.y = 1.0f;
	
	LoadWorldMap(g_pMainPlayer->m_dwRecentOutMap);	
	InitSearchModule(g_pMap);
	
	//���� ������ ���� ����� �����ȣ�� ���� 
	g_pDungeonTable->SetCurrentWorldMapID( (BYTE)g_pMainPlayer->m_dwRecentOutMap );
	
	UpdateMainPlayerToMap();
	g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;

	MinimizePlayer(g_pMainPlayer->m_hPlayer.pHandle);	//ĳ���� ��� 
	g_pExecutive->GXOEnableHFieldApply(g_pMainPlayer->m_hPlayer.pHandle);



	g_pGeometry->SetShadowFlag( ENABLE_PROJECTION_SHADOW | ENABLE_PROJECTION_TEXMAP );

//	LPObjectDesc	pDesc	= (LPObjectDesc)g_pExecutive->GetData(g_pMainPlayer->m_hPlayer.pHandle);
	LPObjectDesc	pDescM	= (LPObjectDesc)g_pExecutive->GetData( g_pMainPlayer->m_hPlayer.pHandle );

	pDescM->pInfo		= g_pMainPlayer;
	pDescM->CrashFunc	= OnCrashUser;
	g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL, TRUE);
	SetBoundingVolume(g_pMainPlayer->m_hPlayer.pHandle, 80.0f);

	VAR->bDisplayDungeonInfo = 1;

	g_pSprManager->CreateSprite(SPR_WORLD_UI_TOP, 0, 0, TRUE, 1);
	g_pSprManager->CreateSprite(SPR_WORLD_UI_MENU, 0, 32, TRUE, 1);

	switch(g_pMainPlayer->m_dwRecentOutMap)
	{
	case 1:		g_pSprManager->CreateSprite(SPR_WORLD_UI_SMALL_MAP, 793, 0, TRUE, 120);	break;
	case 2:		g_pSprManager->CreateSprite(SPR_WORLD_UI_SMALL_MAP2, 793, 0, TRUE, 120);break;
	default:	asm_int3();
	}

	g_pSprManager->CreateSprite(SPR_WORLD_UI_RIGHT, 1004, 221, TRUE, 1);
	g_pSprManager->CreateSprite(SPR_WORLD_UI_LEFT, 0, 554, TRUE, 1);
	g_pSprManager->CreateSprite(SPR_WORLD_UI_BOTTOM, 0, 608, TRUE, 1);
	g_pSprManager->CreateSprite(SPR_WORLD_UI_CHAT, 0, 640, TRUE, 1);	

	V2_SPRITE* pSpr = g_pSprManager->CreateSprite(SPR_SPEAKING_BOX, 100, 100, FALSE, 100);

#if IS_JAPAN_LOCALIZING()
	pSpr->SetScaling(150, 100);
#else
	pSpr->SetScaling(130, 100);
#endif

	//Light�� Attach�� Object�� Vector �Ÿ� ���� 
	VAR->vLightDistance.x = 500.0f;
	VAR->vLightDistance.y = 1000.0f;
	VAR->vLightDistance.z = 600.0f;
	
	POSITION_ pos = g_pDungeonTable->m_pReceivedDungeonList->GetHeadPosition();

	while(pos)
	{
		// ���� ���Դٴ� �Ҹ��� ���� ���ٰ� �ٽ� ����� �Դٴ°�.
		// ��������Ʈ���� �ٽ� �����ض�.
		DUNGEON_DATA_EX* pDungeon =	(DUNGEON_DATA_EX*)g_pDungeonTable->m_pReceivedDungeonList->GetNext(pos);

		if(pDungeon->IsDungeonOwner(g_pMainPlayer))
		{
			if (!pDungeon->m_hOwnDungeon)
			{
				pDungeon->m_hOwnDungeon = 
					CreateHandleObject( g_pObjManager->GetFile((DWORD)DUNGEON_OWNS), NULL, NULL, GXOBJECT_CREATE_TYPE_EFFECT|GXOBJECT_CREATE_TYPE_DEFAULT_PROC);//KUK
			}

			ShowObject(pDungeon->m_hOwnDungeon);
			GXSetPosition(pDungeon->m_hOwnDungeon, &pDungeon->vPos, FALSE);
		}
		
		if(pDungeon->m_bSiege)
		{
			if(!pDungeon->m_hSiegingDungeon)
			{
				pDungeon->m_hSiegingDungeon =
					CreateHandleObject( g_pObjManager->GetFile((DWORD)DUNGEON_SIEGING), NULL, NULL, GXOBJECT_CREATE_TYPE_EFFECT|GXOBJECT_CREATE_TYPE_DEFAULT_PROC );
			}

			ShowObject(pDungeon->m_hSiegingDungeon);
			GXSetPosition(pDungeon->m_hSiegingDungeon, &pDungeon->vPos, FALSE);
		}
		
		if(pDungeon->m_wGroupID)
		{
			if(!pDungeon->m_hGroupDungeon)
			{
				pDungeon->m_hGroupDungeon = 
					CreateHandleObject( g_pObjManager->GetFile((DWORD)DUNGEON_GROUP), NULL, NULL, GXOBJECT_CREATE_TYPE_EFFECT|GXOBJECT_CREATE_TYPE_DEFAULT_PROC );
			}

			HideObject(pDungeon->m_hGroupDungeon);
			GXSetPosition(pDungeon->m_hGroupDungeon, &pDungeon->vPos, FALSE);
		}
	}
	
	VECTOR3 From;
	From.x = 0.0f;							
	From.y = WORLD_CAMERA_DISTANCE_Y;							
	From.z = 0.0f;	
	g_pGeometry->ResetCamera(&From, 100.0f, 80000.0f, DEG45, 0 );

	g_pMainPlayer->SetAction(MOTION_TYPE_STAND1, 0, ACTION_LOOP);
	
	g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
	
	VECTOR3 vPos;
	g_pExecutive->GXOGetPosition(g_pMainPlayer->m_hPlayer.pHandle, &vPos);
	g_pExecutive->GXMGetHFieldHeight(&vPos.y, vPos.x, vPos.z);
	GXSetPosition(g_pMainPlayer->m_hPlayer.pHandle, &vPos, FALSE);

	g_pMainPlayer->m_v3CurPos.x = vPos.x;
	g_pMainPlayer->m_v3CurPos.y = vPos.y;
	g_pMainPlayer->m_v3CurPos.z = vPos.z;	

	g_pMainPlayer->m_pCurTile	= g_pMap->GetTile( g_pMainPlayer->m_v3CurPos.x, g_pMainPlayer->m_v3CurPos.z );

	// ����� ������Ʈ ���� //
	LP_ITEM_RESOURCE_EX lpItemResource;

#ifdef DEVELOP_MODE
	g_pMainPlayer->m_pEquip[ EQUIP_TYPE_RIDE ].m_wItemID = 4000;
#endif
	int				nVal = g_pMainPlayer->m_pEquip[ EQUIP_TYPE_RIDE ].m_wItemID/ITEM_DISTRIBUTE;

	VECTOR3			v3Pos = { 0, };
	char			szInfo[0xff]={0,};

	if(nVal==ITEM_RIDE_INDEX)
	{
		lpItemResource = g_pItemResourceHash->GetData(g_pMainPlayer->m_pEquip[ EQUIP_TYPE_RIDE ].m_wItemID);

		if(lpItemResource)
		{
			if(lpItemResource->wModCount>0)
			{
				g_pMainPlayer->m_RideObject.pDesc = AllocObjDesc();
				memset(g_pMainPlayer->m_RideObject.pDesc, 0, sizeof(OBJECT_DESC));
					
				g_pMainPlayer->m_RideObject.pDesc->bType = OBJECT_TYPE_PLANE;	
				g_pMainPlayer->m_RideObject.pDesc->bActionFlag = ACTION_LOOP;
			
				ItemDataName(szInfo, g_pMainPlayer->m_pEquip[ EQUIP_TYPE_RIDE ].m_wItemID);
				
				g_pMainPlayer->m_RideObject.pHandle = CreateHandleObject(GetFile(szInfo, DATA_TYPE_ITEM), GXPlayerPROC, g_pMainPlayer->m_RideObject.pDesc, 0);

				SetBoundingVolume(g_pMainPlayer->m_RideObject.pHandle, 80.0f);
				g_pExecutive->GXOEnableHFieldApply(g_pMainPlayer->m_RideObject.pHandle);
							
				v3Pos.x = g_pMainPlayer->m_v3CurPos.x-200;
				v3Pos.y = 0.0f;
				v3Pos.z = g_pMainPlayer->m_v3CurPos.z;
				GXSetPosition(g_pMainPlayer->m_RideObject.pHandle, &v3Pos, FALSE);
			}	
		}					
	}
				
	//����ʿ� ĳ���� ���� ��ġ ǥ���� �� 
	VAR->pMinimapPos = g_pSprManager->CreateSprite(SPR_WORLD_RED_DOT, 0, 0, TRUE, 255);
	SetMinimapPos(v3Pos);
	
	//MainPlayer ��ġ�� ī�޶� ���� 	
	g_Camera.fCameraDistance_z = WORLD_CAMERA_DISTANCE_Z;
	g_Camera.fCameraDistance_y = WORLD_CAMERA_DISTANCE_Y;
	SetCameraTarget(g_pMainPlayer->m_hPlayer.pHandle);
	
	VECTOR3 Rotate;
	Rotate.x =  WORLD_CAMERA_ANGLE;	Rotate.y = 0.0f;	Rotate.z = 0.0f;
	g_pGeometry->SetCameraAngleRad(&Rotate, 0);
	g_Camera.fCameraAngle = Rotate.x;				// Normal��� ī�޶� Angle�� ���� //-0.808261

	//-------- ����Ʈ , �׸��� ���� 
	//g_pGeometry->CreateShadowMap( NULL, 1, 256, 0 );
	//g_pGeometry->CreateShadowMap(NULL, 4, 256, 0);

	g_pGeometry->SetShadowFlag(ENABLE_PROJECTION_SHADOW |	ENABLE_PROJECTION_TEXMAP);

	LIGHT_DESC	shadowlight;

	shadowlight.dwDiffuse	= 0xffffffff;
	shadowlight.v3Point.x	= g_pMainPlayer->m_v3CurPos.x + 500.0f;
	shadowlight.v3Point.y	= 1000.0f;
	shadowlight.v3Point.z	= g_pMainPlayer->m_v3CurPos.z + 600.0f;
	shadowlight.fFov		= PI/2.0f;
	shadowlight.v3Up.x		= 0.0f;
	shadowlight.v3Up.y		= 1.0f;
	shadowlight.v3Up.z		= 0.0f;
	shadowlight.fRs			= 2000.0f;
	shadowlight.v3To		= g_pMainPlayer->m_v3CurPos;
	
	VAR->hMainSpotWorld = g_pExecutive->CreateGXLight(&shadowlight,NULL,NULL,0,NULL, GXLIGHT_TYPE_ENABLE_SHADOW | GXLIGHT_TYPE_DISABLE_LIGHT_COLOR);
	g_pExecutive->GXLEnableDynamicLight(VAR->hMainSpotWorld);

	g_pExecutive->GXOAttachLight(g_pMainPlayer->m_hPlayer.pHandle,"Bip01",VAR->hMainSpotWorld, &VAR->vLightDistance,ATTACH_TYPE_LINK);
	g_pGeometry->SetShadowFlag(1);

	//���� ó�� ���ö� �⺻ �ȱ�� ...
	g_pMainPlayer->m_bMoveType = UNIT_STATUS_RUNNING;
	
	g_pInputManager->Initialize();
	
	RECT rt;
	// ��ȭ �Է�â //
	rt.left = 8;	rt.right = 546;	rt.top = 752;	rt.bottom = 764;
	g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_CHAT, FONT_SS3D_GAME, rt, 0xffffffff, 88);
	rt.left = 8;	rt.right = 566;	rt.top = 646;	rt.bottom = 735;	
	// ��ȭ�� �Է�â //	
	rt.left = 458;	rt.right = 600;	rt.top = 752;	rt.bottom = 774;
	g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME, FONT_SS3D_GAME, rt, 0xffffffff, 88);
	//���콺 �����Ϳ��� ���� ���� ǥ���� ä�� �ڽ����� DungeonName
	rt.left = 0;	rt.right = 0;	rt.top = 0;	rt.bottom = 0;
	g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_DUNGEON_NAME, FONT_D3D_GAME, rt, 0xffffff00, 100);
	//������ 	//���콺 �����Ϳ��� ���� ���� ǥ���� ä�� �ڽ����� ���� ����	
	rt.left = 0;	rt.right = 0;	rt.top = 0;	rt.bottom = 0;
	g_pInputManager->InitializeInput(INPUT_BUFFER_8, FONT_D3D_GAME, rt, 0xffffffff, 100);
	//���콺 �����Ϳ��� ���� ���� ǥ���� ä�� �ڽ����� ���� ����
	rt.left = 0;	rt.right = 0;	rt.top = 0;	rt.bottom = 0;
	g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_DUNGEON_SIEGE_TIME, FONT_D3D_GAME, rt, 0xffff00ff, 100);
	//���콺 �����Ϳ��� ���� ���� ǥ���� ä�� �ڽ����� ���� �ð�
	rt.left = 0;	rt.right = 0;	rt.top = 0;	rt.bottom = 0;
	g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_DUNGEON_LEVELUP_TIME, FONT_D3D_GAME, rt, 0xffffffff, 100);
	//���� �޸�����  ǥ���� ���� 
	rt.left = 640;	rt.right = 1040;	rt.top = 655;	rt.bottom = 673;
	g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_DUNGEON_MANAGER, FONT_D3D_GAME, rt, 0xffffff00, 10);
	//���� �޸𳻿� ǥ���� ���� 
	rt.left = 640;	rt.right = 740;	rt.top = 688;	rt.bottom = 740;
	g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_DUNGEON_MEMO, FONT_D3D_GAME, rt, 0xffffffff, 11);
	//���ɴ����� �ƴ� ���� ���� ǥ�ÿ� 
	rt.left = 0;	rt.right = 0;	rt.top = 0;	rt.bottom = 0;
	g_pInputManager->InitializeInput(INPUT_BUFFER_13, FONT_D3D_GAME, rt, 0xff0000ff, 100);
	//���ɴ����� �ƴ� ���� ���� ǥ�ÿ� 
	rt.left = 0;	rt.right = 0;	rt.top = 0;	rt.bottom = 0;
	g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_DUNGEON_LEVELTYPE, FONT_D3D_GAME, rt, 0xffffffff, 100);
	//�������� (����)
	rt.left = 0;	rt.right = 0;	rt.top = 0;	rt.bottom = 0;
	g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_DUNGEON_LEVEL, FONT_D3D_GAME, rt, 0xffffffff, 100);
	//�������� (����)
	rt.left = 0;	rt.right = 0;	rt.top = 0;	rt.bottom = 0;
	g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_DUNGEON_LAYER_COUNT, FONT_D3D_GAME, rt, 0xffffffff, 100);
	//�������� (�������)
	rt.left = 0;	rt.right = 0;	rt.top = 0;	rt.bottom = 0;
	g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_DUNGEON_GUILD, FONT_D3D_GAME, rt, 0xffffffff, 100);
	//�������� (������)
	rt.left = 0;	rt.right = 0;	rt.top = 0;	rt.bottom = 0;
	g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_DUNGEON_MANAGER2, FONT_D3D_GAME, rt, 0xffffffff, 100);
	//�������� (����ǰ)
	rt.left = 0;	rt.right = 0;	rt.top = 0;	rt.bottom = 0;
	g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_DUNGEON_ITEM, FONT_D3D_GAME, rt, 0xffffffff, 100);
	
	for(int i = 0; i < __MAX_CHAT__; i++)
	{
		g_rt[i].left	= 10;
		g_rt[i].right	= 600;
		g_rt[i].top		= 650+i*20;
		g_rt[i].bottom	= 662+i*20;
	}

	g_pSprManager->GetSprite(SPR_LOADING_BACK)->ShowSprite(FALSE);	
	g_pSprManager->CreateSprite(BUTTON_WDCHAT1, 443, 750, 32, 0, 8, 16, TRUE, 24);
	g_pSprManager->CreateSprite(BUTTON_WDCHAT2, 578, 734, 224, 16, 8, 32, TRUE, 24);
	g_pSprManager->CreateSprite(BUTTON_WDCHAT3, 590, 734, 232, 16, 8, 32, TRUE, 24);	
	g_pSprManager->CreateSprite(SPR_CHAT_SC, 589, 643, 213, 16, 10, 16, TRUE, 24);
	g_pSprManager->GetSprite(BUTTON_WDCHAT1)->ShowSprite(FALSE);
	g_pSprManager->GetSprite(BUTTON_WDCHAT2)->ShowSprite(TRUE);
	g_pSprManager->GetSprite(BUTTON_WDCHAT3)->ShowSprite(FALSE);

	g_nPGSelect	= 3;
	g_nChatId	= 0;
	
	g_pInputManager->SetFocusInput(INPUT_BUFFER_WORLD_CHAT);
		
	CInterface*		pInterface	= CInterface::GetInstance();
	CMiniMapWnd*	pMiniMapWnd = CMiniMapWnd::GetInstance();
	CCommandWnd*	pCommandWnd	= CCommandWnd::GetInstance();

	pInterface->InitInterface(__SERVER_TYPE_WORLD__);	
	pMiniMapWnd->m_bTemp = TRUE;		
	
	pCommandWnd->m_byMaxIndex = 0;
	memset(pCommandWnd->m_sMsg, 0, sizeof(pCommandWnd->m_sMsg));

	if(pCommandWnd->m_bMessageChk==FALSE)
	{
		CTWS_CHAT_MESSAGERQ pPacket;
		memset(pPacket.szId, 0, sizeof(pPacket.szId));
		__lstrcpyn(pPacket.szId, g_pMainPlayer->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
		g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
	}	

	SelectBGM(WORD(g_pMainPlayer->m_dwRecentOutMap+10000), 0, 0);

	SetMousePointerType(__MOUSE_POINTER_DEFAULT__);
	
	// ���弭������ �̺�Ʈ ���� ���� ��û��.
	SendQueryEventDungeonInfo();	

	if(g_pMainPlayer->m_byHelp[0]==0 && g_pMainPlayer->m_byHelp[1]==0)
	{		
		// ����� �⺻ ���� //
		CHelpWnd* pHelpWnd = CHelpWnd::GetInstance();
		pHelpWnd->m_byType	= 0;
		pHelpWnd->SetClear();
		pHelpWnd->SetActive();		
		// ������ �޼��� ������ //
		g_pMainPlayer->m_byHelp[0]	= 1;
		g_pMainPlayer->m_byHelp[1]	= 1;
		
		WSTC_HELP_INFO pHelpInfoPacket;
		pHelpInfoPacket.bHelpInfo[0] = g_pMainPlayer->m_byHelp[0];
		pHelpInfoPacket.bHelpInfo[1] = g_pMainPlayer->m_byHelp[1];
		pHelpInfoPacket.bHelpInfo[2] = g_pMainPlayer->m_byHelp[2];	
		g_pNet->SendMsg((char*)&pHelpInfoPacket, pHelpInfoPacket.GetPacketSize(), SERVER_INDEX_WORLD);
	}

	SetEffectIndex(g_pMainPlayer->m_hPlayerFlag.pHandle);
	SetEffectIndex(g_pMainPlayer->m_hPlayerHead.pHandle);
	SetEffectIndex(g_pMainPlayer->m_hPlayerHelmet.pHandle);
	SetEffectIndex(g_pMainPlayer->m_hPlayerEmoticon.pHandle);
	SetEffectIndex(g_pMainPlayer->m_hPlayerShop.pHandle);
	SetEffectIndex(g_pMainPlayer->m_hPlayer.pHandle);
	SetEffectIndex(g_pMainPlayer->m_hPlayerLHand.pHandle);
	SetEffectIndex(g_pMainPlayer->m_hPlayerRHand.pHandle);
	
#if IS_CHINA_LOCALIZING()
	// �߱� ������ ���,.. ó��.
	ShowChinaBillingRemainTime();
#endif

	return TRUE;
}

// Send Packet Func
void SendWorldMovePacket()
{
	CTC_WORLD_MOVE move;
	move.bMoveType			= g_pMainPlayer->m_bMoveType;
	move.v3MoveStartPos		= g_pMainPlayer->m_v3CurPos;
	move.v3MoveDirection	= g_pMainPlayer->m_v3Direction;
	move.wDestX				= g_pMainPlayer->m_pDestTile->wIndex_X;
	move.wDestZ				= g_pMainPlayer->m_pDestTile->wIndex_Z;
	g_pNet->SendMsg( (char*)&move, move.GetPacketSize(), SERVER_INDEX_WORLD );
}

void SendWorldStopPacket()
{
	CTC_WORLD_STOP	stop;
	stop.v3StopPos		= g_pMainPlayer->m_v3CurPos;
	g_pNet->SendMsg( (char*)&stop, stop.GetPacketSize(), SERVER_INDEX_WORLD );
}

BOOL EpsilonVectorByTwoMatrix(VECTOR3*	v3Vec1, VECTOR3* v3Vec2, float fEpsilon )
{
	double dx = pow((double)(v3Vec2->x - v3Vec1->x), 2);
	double dz = pow((double)(v3Vec2->z - v3Vec1->z), 2);
	double fA = (float)sqrt(dx + dz);

	return ( fA < fEpsilon ) ? TRUE : FALSE;
}

void PathFindMoveFuncWorld(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	VECTOR3 Pos;
	g_pExecutive->GXOGetPosition(handle, &Pos);				// ������ ��ġ�� ������.

	
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;			// ���� ���� ������.
	
	pObjDesc->dwCurTick		= g_dwCurTick;					
	DWORD dwTime = pObjDesc->dwCurTick - pObjDesc->dwStartTick;
	float fSpeed;

	CMainUser*	pMainUser = (CMainUser*)pData->pInfo;
	
	if( pMainUser->GetStatus() == UNIT_STATUS_WALKING ) 
		fSpeed = WORLD_WALKING_SPEED_RATIO * dwTime;		
	else 
		fSpeed = WORLD_RUNNING_SPEED_RATIO * dwTime;		
	
//	CMap*		pMap	= g_pMap;
	MAP_TILE*	pTile=0;

	// ���� ���������� üũ�Ѵ�.
	if(EpsilonVectorByTwoMatrix(&Pos, &pObjDesc->vDest, fSpeed))
	{
		GXSetPosition(handle, &pObjDesc->vDest, FALSE);
		pMainUser->m_v3CurPos = pObjDesc->vDest;
		
		if(!SetPathFindMoveWorld())
			goto lb_stop;
	}
	else	// ���� ������ �ƴ϶�� ��� �����Ѵ�.
	{
		g_pExecutive->GXOMoveForward( handle, fSpeed );
		g_pExecutive->GXOGetPosition( handle, &Pos );			
		pTile = g_pMap->GetTile( Pos.x, Pos.z );			// �̵��� ������ Ÿ���� �޴´�. 
															// Ÿ���� �ٲ���� üũ�� ����.

		if(!pTile)
			return;

		if( ( g_pMainPlayer->m_pCurTile->wIndex_X != pTile->wIndex_X ) || 
			( g_pMainPlayer->m_pCurTile->wIndex_Z != pTile->wIndex_Z )	)
		{
			if( pTile->wAttr.uOccupied != TILE_EMPTY )
			{
				// ������ Ÿ���̸� �ٽ� ���ڸ��� �������� �����Ѵ�.
				if (g_dwLayerID < 100 )	// ������ �ƴϹǷ�
					::SetAction(pMainUser->m_hPlayer.pHandle, MOTION_TYPE_VILLAGESTAND, 0, ACTION_LOOP );
				else	
					::SetAction(pMainUser->m_hPlayer.pHandle, MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );

				GXSetPosition( handle, &pMainUser->m_v3CurPos, FALSE );			
				goto lb_stop;
			}

			pMainUser->m_pCurTile = pTile;
		}
		pObjDesc->dwStartTick = pObjDesc->dwCurTick;
		pMainUser->m_v3CurPos = Pos;
	}

	// ���� ���ΰ��̶�� ī�޶� ���� �̵���Ų��.
	if( g_pMainPlayer->m_hPlayer.pHandle == handle )
	{
		SetCameraTarget(g_pMainPlayer->m_hPlayer.pHandle);		
	}
	return;

lb_stop:
	
	if (g_dwLayerID < 100 )	// ������ �ƴϹǷ�
		::SetAction(pMainUser->m_hPlayer.pHandle, MOTION_TYPE_VILLAGESTAND, 0, ACTION_LOOP );
	else	
		::SetAction(pMainUser->m_hPlayer.pHandle, MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );

	pMainUser->SetStatus(UNIT_STATUS_NORMAL);
	pObjDesc->ObjectFunc = NULL;	// ���̻� �̵� üũ���� �ʰ��ϱ����ؼ�.

	// ��ž �޽����� ������.
	SendWorldStopPacket();	

	return;
}

BOOL SetPathFindMoveWorld()
{
	VECTOR3	v3Dir, v3DirNormal;
	float fDir;
//	char temp[0xff];
	if(g_PathFinder.dwCurveNum==0)							goto lb_stop;
	if(g_PathFinder.dwCurveNum==g_PathFinder.dwCurCurve)	goto lb_stop;
	
	// ���⼭ ���� ��ã�� �����̴�.
	g_pMainPlayer->m_hPlayer.pDesc->vDest.x		= (float)( g_PathFinder.pLocation[g_PathFinder.dwCurCurve].dwBlock_X * TILE_SIZE_WORLD + ( TILE_SIZE_WORLD / 2.f ) );
	g_pMainPlayer->m_hPlayer.pDesc->vDest.z		= (float)( g_PathFinder.pLocation[g_PathFinder.dwCurCurve].dwBlock_Y * TILE_SIZE_WORLD + ( TILE_SIZE_WORLD / 2.f ) );
	g_pMainPlayer->m_pDestTile					= g_pMap->GetMap( g_PathFinder.pLocation[g_PathFinder.dwCurCurve].dwBlock_X,
																	g_PathFinder.pLocation[g_PathFinder.dwCurCurve].dwBlock_Y );
	g_pMainPlayer->m_hPlayer.pDesc->vDest.y		= 0.f;
	g_PathFinder.dwCurCurve++;
		
	g_pMainPlayer->m_hPlayer.pDesc->dwStartTick	= g_dwCurTick;					// ������ ms�� ���Ѵ�.
		
	VECTOR3_SUB_VECTOR3( &v3Dir, &g_pMainPlayer->m_hPlayer.pDesc->vDest, &g_pMainPlayer->m_v3CurPos );

	Normalize( &v3DirNormal, &v3Dir );						// ������ ������ ��������
	fDir = (float)(atan2(v3Dir.z, v3Dir.x) + DEG90 );
	g_pExecutive->GXOSetDirection( g_pMainPlayer->m_hPlayer.pHandle, &g_Camera.v3AxsiY,  fDir);
	
	switch( g_pMainPlayer->GetStatus())
	{
	case UNIT_STATUS_WALKING:
	case UNIT_STATUS_RUNNING:
		{
			if( EpsilonVectorByTwoMatrix( &g_pMainPlayer->m_v3Direction,  &v3DirNormal, 0.1f ) ) return TRUE;	// ���� �����̸� �޽����� ���� �ʿ����.						

			//�ȱ�/�ٱ� ��� ��忡 ����
			if(g_pMainPlayer->m_bMoveType == UNIT_STATUS_WALKING)
			{
				if(g_pMainPlayer->GetStatus()!= UNIT_STATUS_WALKING)
					::SetAction(g_pMainPlayer->m_hPlayer.pHandle, MOTION_TYPE_WALK, 0, ACTION_LOOP);
				
				g_pMainPlayer->SetStatus(UNIT_STATUS_WALKING);
			}
			else if(g_pMainPlayer->m_bMoveType == UNIT_STATUS_RUNNING)
			{
				if(g_pMainPlayer->GetStatus()!= UNIT_STATUS_RUNNING)
					::SetAction(g_pMainPlayer->m_hPlayer.pHandle, MOTION_TYPE_RUN, 0, ACTION_LOOP);

				g_pMainPlayer->SetStatus(UNIT_STATUS_RUNNING);
			}
			else
				asm_int3();
						
			g_pMainPlayer->m_v3Direction = v3DirNormal;
			// Move Packet�� ������.
			SendWorldMovePacket();			

			float	fRad; VECTOR3 vec;
			g_pExecutive->GXOGetDirection(g_pMainPlayer->m_hPlayer.pHandle, &vec, &fRad);
			
			CMiniMapWnd* pMiniMapWnd = CMiniMapWnd::GetInstance();

			pMiniMapWnd->SetRotObj(SPR_OBJ_DSMAP_USER, -fRad+PI);					
		}
		break;
	
	case UNIT_STATUS_NORMAL:
		{
			g_pMainPlayer->m_v3Direction	= v3DirNormal;

			
			//�ȱ�/�ٱ� ��� ��忡 ����
			if(g_pMainPlayer->m_bMoveType == UNIT_STATUS_WALKING)
			{
				if(g_pMainPlayer->GetStatus()!= UNIT_STATUS_WALKING)
					::SetAction(g_pMainPlayer->m_hPlayer.pHandle, MOTION_TYPE_WALK, 0, ACTION_LOOP);
				
				g_pMainPlayer->SetStatus(UNIT_STATUS_WALKING);
			}
			else if(g_pMainPlayer->m_bMoveType == UNIT_STATUS_RUNNING)
			{
				if(g_pMainPlayer->GetStatus()!= UNIT_STATUS_RUNNING)
					::SetAction(g_pMainPlayer->m_hPlayer.pHandle, MOTION_TYPE_RUN, 0, ACTION_LOOP);

				g_pMainPlayer->SetStatus(UNIT_STATUS_RUNNING);
			}
			else
				asm_int3();

			// Move Message�� ������.
			SendWorldMovePacket();		

			float	fRad; VECTOR3 vec;
			g_pExecutive->GXOGetDirection(g_pMainPlayer->m_hPlayer.pHandle, &vec, &fRad);			
			
			// ����Ʈ�� �߻���Ų��.			
		}
		break;
	}
	g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc	= PathFindMoveFuncWorld;
	return TRUE;

lb_stop:
	return FALSE;
}

BOOL PathFindWorld()
{
	g_Mouse.v3Mouse = GetXYZFromScreenXY( g_pGeometry, g_Mouse.MousePos.x , g_Mouse.MousePos.y , g_rcScreenRect.right, g_rcScreenRect.bottom);

	if( g_Mouse.v3Mouse.z < 0 || g_Mouse.v3Mouse.x < 0)	
		return	FALSE;
	
	DWORD dwStartX	= (DWORD)( g_pMainPlayer->m_v3CurPos.x / TILE_SIZE_WORLD );
	DWORD dwStartZ	= (DWORD)( g_pMainPlayer->m_v3CurPos.z / TILE_SIZE_WORLD );
	DWORD dwEndX	= (DWORD)( g_Mouse.v3Mouse.x / TILE_SIZE_WORLD );
	DWORD dwEndZ	= (DWORD)( g_Mouse.v3Mouse.z / TILE_SIZE_WORLD );
		
	if(dwEndX >= g_pMap->m_dwMapXTileMany || dwEndZ >= g_pMap->m_dwMapZTileMany)
		return FALSE;
		
	g_PathFinder.dwCurveNum = g_pSw->FindShortestWay( dwStartX, dwStartZ, dwEndX, dwEndZ, &g_PathFinder.pLocation );

	if( g_PathFinder.dwCurveNum >= 1 )	// ��ã�⸦ �ؾ��Ѵ�.
	{
		MAP_TILE*	pTile, *pNextTile;
		VECTOR3		v3Tmp, v3Direction, v3Dest;
		int			cnt = 1;

		v3Dest.x	= (float)( g_PathFinder.pLocation[0].dwBlock_X * TILE_SIZE_WORLD + ( TILE_SIZE_WORLD / 2.f ) );
		v3Dest.z	= (float)( g_PathFinder.pLocation[0].dwBlock_Y * TILE_SIZE_WORLD + ( TILE_SIZE_WORLD / 2.f ) );
		v3Dest.y	= 0;
		
		VECTOR3_SUB_VECTOR3( &v3Direction, &v3Dest, &g_pMainPlayer->m_v3CurPos );
		Normalize( &v3Tmp, &v3Direction );						// ������ ������ ��������
		pTile = g_pMap->GetTile( g_pMainPlayer->m_v3CurPos.x, g_pMainPlayer->m_v3CurPos.z );
		pNextTile = pTile;

		do
		{
			pNextTile = g_pMap->GetTile( g_pMainPlayer->m_v3CurPos.x + v3Tmp.x * cnt, g_pMainPlayer->m_v3CurPos.z + v3Tmp.z * cnt );
			cnt++;
		}while( pTile == pNextTile );

		//if( !pNextTile || pNextTile->wAttr.uSection == 0 || pNextTile->wAttr.uOccupied != TILE_EMPTY )
		if( !pNextTile || pNextTile->wAttr.uOccupied != TILE_EMPTY )
		{
			if( g_pMainPlayer->GetStatus() == UNIT_STATUS_WALKING || g_pMainPlayer->GetStatus() == UNIT_STATUS_RUNNING )
			{
				SendWorldStopPacket();	
			}

			// �̵����� �ʰ� �̵��� ����Ѵ�.
			::SetAction(g_pMainPlayer->m_hPlayer.pHandle,  MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );
			g_pMainPlayer->SetStatus( UNIT_STATUS_NORMAL );
			g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;
			return FALSE;
		}		
		g_PathFinder.dwCurCurve = 0;
		return TRUE;
	}
	else
		return FALSE;
}

void MouseEventWorld()
{	
	CInterface* pInterface = CInterface::GetInstance();
	
	if(pInterface->GetClick())
		return;
	
	if( g_Mouse.bLDown == true )
	{		
		g_Mouse.dwLButtonDownTime = g_dwCurTick;	// ���������� Mouse�̺�Ʈ�� ó���� �ð��� ����Ѵ�.		
			
		if( g_pMainPlayer->GetStatus()==UNIT_STATUS_NORMAL	||
			g_pMainPlayer->GetStatus()==UNIT_STATUS_WALKING	||
			g_pMainPlayer->GetStatus()==UNIT_STATUS_RUNNING	)
		{
			if(PathFindWorld())
				SetPathFindMoveWorld();
		}
		
	}
}

void UpdateGameWorld()
{
	DWORD dwModelIndex	= 0;
	DWORD dwObjIndex	= 0;

	g_hHandle = g_pExecutive->GXOGetObjectWithScreenCoord(&g_v3Temp, &dwModelIndex, &dwObjIndex, &g_fDist, &g_Mouse.MousePos, 0, PICK_TYPE_PER_COLLISION_MESH);	
	
	if(g_byStatusMessenger==0)
	{
		// �¶��ο��� �ڸ�������� //
		DWORD dwStatusTime	= timeGetTime();

		if(dwStatusTime-g_dwStatusTime>60000)
		{
			g_dwStatusTime = 1;
			
			// ��Ŷ ������ //			
			CTWS_MESSENGER_STATUS pPacket;
			pPacket.byType		= 1;
			pPacket.dwUserIndex	= g_pMainPlayer->m_dwUserIndex;
			g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
		}
	}
	else if(g_byStatusMessenger)
	{
		// �¶��ο��� �ڸ�������� //
		if(g_byChkMessenger==1)
		{
			g_byChkMessenger	= 0;
			g_dwStatusTime		= 0;
			g_byStatusMessenger	= 0;

			// ��Ŷ ������ //
			CTWS_MESSENGER_STATUS pPacket;
			pPacket.byType		= 0;
			pPacket.dwUserIndex	= g_pMainPlayer->m_dwUserIndex;
			g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
		}
	}

	//Dungeon Join Time out 
	if(g_pMainPlayer->GetStatus()==UNIT_STATUS_AWAIT_DUNGEON_RESPONSE)
	{
		if( (g_dwCurTick - VAR->dwCrashDungeonTick) > JOIN_TIME_OUT )
		{
			g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_JOIN_FAIL);
			g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_JOIN_FAIL, g_Message[ETC_MESSAGE417].szMessage, lstrlen(g_Message[ETC_MESSAGE417].szMessage)); // MSG_ID : 417 ; [Dungeon Join Failed!]
						
			if(g_pMainPlayer->m_bReceivedConnectMsg)
				DisplayMessageAdd(g_Message[ETC_MESSAGE675].szMessage, 0xFFFFC309); // "���弭���� �������� �ʽ��ϴ�! ������� �����ϼ���!"
			else
				DisplayMessageAdd(g_Message[ETC_MESSAGE676].szMessage, 0xFFFFC309); // "�������� �α��ο� �����߽��ϴ�.! ������� �����ϼ���!"

			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_Camera.v3CameraPos, FALSE);
			MoveToUserPrevPos();
			
			g_pMainPlayer->SetAction(MOTION_TYPE_VILLAGESTAND, 0, ACTION_LOOP);
			g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
		}
	}
	
	MouseEventWorld();
					
	if(g_hHandle)
	{		
		DWORD dwID = g_pExecutive->GetID(g_hHandle);

		if (dwID > EVENT_DUNGEON_EFFECT_INCRESEMENT)
		{
			dwID -= EVENT_DUNGEON_EFFECT_INCRESEMENT;
		}		
		VAR->pCurSelectDungeon = g_pDungeonTable->GetDungeonInfo(dwID);
	}
	else
	{
		VAR->pCurSelectDungeon = NULL;		
	}

	//---------------------------------  ���� ������� ���� ���� �˾�â �ٿ�� --------------------------------------	
	//	V2_SPRITE*	pSpr = SPR(SPR_SPEAKING_BOX);
	
	switch(g_pMainPlayer->GetStatus())
	{
	case UNIT_STATUS_RIDE_AIRPLANE:	
	case UNIT_STATUS_RIDE_AIRPLANE_DOWN:
	case UNIT_STATUS_AIRPLANE_DOWNING:
	case UNIT_STATUS_AIRPLANE_FLOATING:
		goto lb_Process;
	}
	
lb_Process:

	switch(g_pMainPlayer->GetStatus())
	{
	case UNIT_STATUS_RIDE_AIRPLANE:	
		MoveProcessPlane();
		return;			
	default:
		return;
	}
}

void ReleaseGameWorld()
{
	g_pGeometry->DisableFog(0);

	CInterface* pInterface = CInterface::GetInstance();
	pInterface->Remove();

	RELEASE_SPR(BUTTON_WDCHAT1);
	RELEASE_SPR(BUTTON_WDCHAT2);
	RELEASE_SPR(BUTTON_WDCHAT3);
	RELEASE_SPR(SPR_CHAT_SC);
	
	InitWorldMessage();
	StopBGM();
		
	RELEASE_SPR(SPR_WORLD_UI_TOP);
	RELEASE_SPR(SPR_WORLD_UI_MENU);
	RELEASE_SPR(SPR_WORLD_UI_SMALL_MAP);
	RELEASE_SPR(SPR_WORLD_UI_SMALL_MAP2);
	RELEASE_SPR(SPR_WORLD_UI_RIGHT);
	RELEASE_SPR(SPR_WORLD_UI_LEFT);
	RELEASE_SPR(SPR_WORLD_UI_BOTTOM);
	RELEASE_SPR(SPR_WORLD_UI_CHAT);
	RELEASE_SPR(SPR_WORLD_RED_DOT);
	RELEASE_SPR(SPR_SPEAKING_BOX);

	g_pObjManager->RemoveObject(MOD_WORLD_WATER);	

	//����� Object ���� 
	if( g_pMainPlayer->m_RideObject.pHandle )
	{
		DeleteHandleObject(g_pMainPlayer->m_RideObject.pHandle);
		FreeObjDesc( g_pMainPlayer->m_RideObject.pDesc);
		g_pMainPlayer->m_RideObject.pHandle = NULL;
		g_pMainPlayer->m_RideObject.pDesc = NULL;
	}
	//
	
	g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;
	g_pExecutive->DeleteGXLight(VAR->hMainSpotWorld);
	
	//��� ��Ƽ���� 
	ListNode<PARTY_USER>* pNode = g_pPartyUserHash->GetHead();

	while(pNode)
	{
		LP_PARTY_USER pPartyUser = pNode->pData;
		FreeWorldPartyUser(pPartyUser);
		pNode = pNode->pNext;
	}

	DeleteDungeonObjects();

	POSITION_ pos = g_pDungeonTable->m_pReceivedDungeonList->GetHeadPosition();
	
	while(pos)
	{
		// ��������Ʈ���� ���ܶ�.
		DUNGEON_DATA_EX* pDungeon =	(DUNGEON_DATA_EX*)g_pDungeonTable->m_pReceivedDungeonList->GetNext(pos);

		DeleteObj(&pDungeon->m_hGroupDungeon);
		DeleteObj(&pDungeon->m_hOwnDungeon);
		DeleteObj(&pDungeon->m_hSiegingDungeon);
	}
	g_pInputManager->Initialize();
		
	//LoadMapScript�� ���� ������ GXObject Release 
	for(int i = 0; i < VAR->wMapObjectNum; i++)
		DeleteHandleObject(VAR->hMapObjHandle[i]);

	g_pExecutive->DeleteAllGXLights();	

	if(g_pMap)
	{
		g_pExecutive->DeleteAllGXMapObjectsWitLoadMapScript();
		g_pExecutive->DeleteGXMap( NULL );
		delete g_pMap;
		g_pMap = NULL;
	}

	if(VAR)	
	{
		delete VAR;
		VAR = NULL;
	}	

	IMESetEdit(0, FALSE);
}

void OnFloatingPlane(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	if(bFrameFlag == FRAME_FLAG_FINISHED || bFrameFlag == FRAME_FLAG_CHANGE_NEXT )
	{
		
		::SetAction(handle, 3, 0, ACTION_LOOP);
		pData->ObjectFunc = NULL;
		VECTOR3 Result;
		Result.x = WORLD_CAMERA_PLANE_ANGLE;		
		Result.y = 0.0f;
		Result.z = 0.0f;

		g_pGeometry->SetCameraAngleRad(&Result, 0);
		
		g_Camera.fCameraDistance_y = WORLD_PLANE_CAMERA_DISTANCE_Y;
		g_Camera.fCameraDistance_z = WORLD_PLANE_CAMERA_DISTANCE_Z;
		SetCameraTarget(handle);

		g_pMainPlayer->SetStatus(UNIT_STATUS_RIDE_AIRPLANE);

		PlayBGM(GetFile( "Map_World1_fly.mp3", DATA_TYPE_SOUND ));

		VAR->bDisplayDungeonInfo = FALSE;

		//�ٿ�� ���� ���� 
		ReleaseBoundingVolume(handle);
	}
	else
	{	
		VECTOR3 Result;
		Result.x = 0.007f;		//0.21 ��ŭ 
		Result.y = 0.0f;
		Result.z = 0.0f;

		g_pGeometry->RotateCameraRad(&Result, 0);

		g_Camera.fCameraDistance_y += 15;
		g_Camera.fCameraDistance_z -= 50;
		SetCameraTarget(handle);
	}
}

void OnSettledownPlane(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	if(bFrameFlag == FRAME_FLAG_FINISHED || bFrameFlag == FRAME_FLAG_CHANGE_NEXT )
	{
		::SetAction(handle, 1, 0, ACTION_LOOP);
		pData->ObjectFunc = NULL;
		
		VECTOR3 Result;
		VECTOR3 vPos;

		Result.x = WORLD_CAMERA_ANGLE;
		Result.y = 0.0f;
		Result.z = 0.0f;
		g_pGeometry->SetCameraAngleRad(&Result, 0);
		
		g_Camera.fCameraDistance_y = WORLD_CAMERA_DISTANCE_Y;
		g_Camera.fCameraDistance_z = WORLD_CAMERA_DISTANCE_Z;
		SetCameraTarget(handle);

		g_pMainPlayer->SetStatus(UNIT_STATUS_RIDE_AIRPLANE_DOWN);

		StopBGM();
		SelectBGM(WORD(g_pMainPlayer->m_dwRecentOutMap+10000), 0, 0);

		VAR->bDisplayDungeonInfo = TRUE;
		SetBoundingVolume(handle, 80.0f);
		
		g_pExecutive->GXOEnableHFieldApply( g_pMainPlayer->m_hPlayer.pHandle );
		
		g_pExecutive->GXOGetPosition(g_pMainPlayer->m_RideObject.pHandle, &vPos);
		g_pMainPlayer->m_pEquip[ EQUIP_TYPE_RIDE ].m_Item_RideObject.fWorldPos_x = vPos.x;
		g_pMainPlayer->m_pEquip[ EQUIP_TYPE_RIDE ].m_Item_RideObject.fWorldPos_z = vPos.z;

	}
	else
	{
		VECTOR3 Result;
		Result.x = -0.007f;		//0.21 ��ŭ 
		Result.y = 0.0f;
		Result.z = 0.0f;
		g_pGeometry->RotateCameraRad(&Result, 0);

		g_Camera.fCameraDistance_y -= 15;
		g_Camera.fCameraDistance_z += 50;
		SetCameraTarget(handle);		
	}
}

DWORD __stdcall AfterRenderGameWorld()
{
	
	g_helpLoadingSprite.SetLoadingFlag(FALSE);
	
	g_pSprManager->RenderAllSprite();

	if(g_bNoticeFlag==TRUE)
	{
		if(g_bPosType==UPDATE_GAME_WORLD)
		{
			//RenderNotice();
			CheckNoticeTime();
		}
	}

	int		nIdx = 0;
	
	CInterface*				pInterface			= CInterface::GetInstance();
//	CExitWnd*				pExitWnd			= CExitWnd::GetInstance();
//	CHelpWnd*				pHelpWnd			= CHelpWnd::GetInstance();
//	COkWnd*					pOkWnd				= COkWnd::GetInstance();
//	CDungeonInfoWnd*		pDungeonInfoWnd		= CDungeonInfoWnd::GetInstance();
//	CGameNoticeMessageWnd*	pNoticeMessageWnd	= CGameNoticeMessageWnd::GetInstance();

	pInterface->Render();		
	
	// ä�� //
	if(g_nWordIndex==__MAX_WORD_QUEUE__)
	{		
		if(g_nWordStart>g_nWordEnd)
		{
			if(g_nCurScrollPos==__MAX_CHAT_SCROLL__)
			{
				nIdx = 0;

				if(g_nWordEnd>=4)
				{
					for(int k = g_nWordEnd-4; k <= g_nWordEnd; k++)
					{
						if(g_sWordChatMsg[k].snSize!=0)
							RenderFont(g_sWordChatMsg[k].szDungeon, g_rt[nIdx].left, g_rt[nIdx].right, g_rt[nIdx].top, g_rt[nIdx].bottom, 5+nIdx, g_sWordChatMsg[k].sdwChatColor);									
						
						nIdx++;						
					}
				}
				else
				{
					for(int k = __MAX_WORD_QUEUE__ - (4-g_nWordEnd); k < __MAX_WORD_QUEUE__; k++)
					{	
						if(g_sWordChatMsg[k].snSize!=0)
							RenderFont(g_sWordChatMsg[k].szDungeon, g_rt[nIdx].left, g_rt[nIdx].right, g_rt[nIdx].top, g_rt[nIdx].bottom, 5+nIdx, g_sWordChatMsg[k].sdwChatColor);		
						
						nIdx++;					
					}
					for(k = 0; k <= g_nWordEnd; k++)
					{
						if(g_sWordChatMsg[k].snSize!=0)
							RenderFont(g_sWordChatMsg[k].szDungeon, g_rt[nIdx].left, g_rt[nIdx].right, g_rt[nIdx].top, g_rt[nIdx].bottom, 5+nIdx, g_sWordChatMsg[k].sdwChatColor);		

						nIdx++;
					}
				}
			}
			else
			{
				if(g_nWordStart+g_nCurScrollPos+4<__MAX_WORD_QUEUE__)
				{
					for(int k = g_nWordStart+g_nCurScrollPos; k <= g_nWordStart+g_nCurScrollPos+4; k++)
					{
						if(g_sWordChatMsg[k].snSize!=0)
							RenderFont(g_sWordChatMsg[k].szDungeon, g_rt[(k-g_nCurScrollPos-g_nWordStart)].left, g_rt[(k-g_nCurScrollPos-g_nWordStart)].right, g_rt[(k-g_nCurScrollPos-g_nWordStart)].top, g_rt[(k-g_nCurScrollPos-g_nWordStart)].bottom, 5+nIdx, g_sWordChatMsg[k].sdwChatColor);
					}
				}
				else
				{
					nIdx = 0;

					for(int k = g_nWordStart+g_nCurScrollPos; k < __MAX_WORD_QUEUE__; k++)
					{
						if(g_sWordChatMsg[k].snSize!=0)
							RenderFont(g_sWordChatMsg[k].szDungeon, g_rt[(k-g_nCurScrollPos-g_nWordStart)].left, g_rt[(k-g_nCurScrollPos-g_nWordStart)].right, g_rt[(k-g_nCurScrollPos-g_nWordStart)].top, g_rt[(k-g_nCurScrollPos-g_nWordStart)].bottom, 5+nIdx, g_sWordChatMsg[k].sdwChatColor);

						nIdx++;
					}
					
					for(k = 0; k < 5-nIdx; k++)
					{
						if(g_sWordChatMsg[k].snSize!=0)
							RenderFont(g_sWordChatMsg[k].szDungeon, g_rt[(k-g_nCurScrollPos-g_nWordStart)].left, g_rt[(k-g_nCurScrollPos-g_nWordStart)].right, g_rt[(k-g_nCurScrollPos-g_nWordStart)].top, g_rt[(k-g_nCurScrollPos-g_nWordStart)].bottom, 5+nIdx, g_sWordChatMsg[k].sdwChatColor);
					}
				}				
			}
		}
		else
		{
			for(int k = g_nCurScrollPos; k <= g_nCurScrollPos+4; k++)
			{
				if(g_sWordChatMsg[k].snSize!=0)
					RenderFont(g_sWordChatMsg[k].szDungeon, g_rt[(k-g_nCurScrollPos-g_nWordStart)].left, g_rt[(k-g_nCurScrollPos-g_nWordStart)].right, g_rt[(k-g_nCurScrollPos-g_nWordStart)].top, g_rt[(k-g_nCurScrollPos-g_nWordStart)].bottom, 5+nIdx, g_sWordChatMsg[k].sdwChatColor);
			}
		}
	}
	else
	{
		for(int k = g_nCurScrollPos; k <= g_nCurScrollPos+4; k++)
		{
			if(g_sWordChatMsg[k].snSize!=0 && g_nWordIndex>k)
				RenderFont(g_sWordChatMsg[k].szDungeon, g_rt[(k-g_nCurScrollPos-g_nWordStart)].left, g_rt[(k-g_nCurScrollPos-g_nWordStart)].right, g_rt[(k-g_nCurScrollPos-g_nWordStart)].top, g_rt[(k-g_nCurScrollPos-g_nWordStart)].bottom, 4+nIdx, g_sWordChatMsg[k].sdwChatColor);
		}
	}
	//===========================================================================//
	//g_pInputManager->RenderInputBufferAll();
	int nFontIndex = 0;
	//ID ��Ʈ ���

	for(int i=0; i<MAX_INPUT_BUFFER_NUM; i++)
	{
#ifdef _USE_IME

		if(GET_IMEEDIT()->GetEditIndex()==1)
		{
			if(i!=0)
			{
				if(!g_pInputManager->GetInputBufferLength(i))	continue;

#if IS_JAPAN_LOCALIZING()
				// ���� ������ �޸� ���� ���.. 
				if(i == INPUT_BUFFER_11)
				{
					if(!g_pInputManager->GetInputBufferLength(i))	continue;
				}
#endif
				g_pInputManager->RenderInputBuffer(i);
				
			}
		}
		else if(GET_IMEEDIT()->GetEditIndex()==2)
		{
			if(i!=6)
			{
				if(!g_pInputManager->GetInputBufferLength(i))	continue;

#if IS_JAPAN_LOCALIZING()
				// ���� ������ �޸� ���� ���.. 
				if(i == INPUT_BUFFER_11)
				{
					if(!g_pInputManager->GetInputBufferLength(i+1)) continue;
				}
#endif		
				g_pInputManager->RenderInputBuffer(i);
				
			}
		}
		else
		{
			if(!g_pInputManager->GetInputBufferLength(i))	continue;
			
#if IS_JAPAN_LOCALIZING()
			// ���� ������ �޸� ���� ���.. 
			if(i == INPUT_BUFFER_11)
			{
				if(!g_pInputManager->GetInputBufferLength(i+1))	continue;
			}
#endif
		
			g_pInputManager->RenderInputBuffer(i);
		}
			
#else
		if(!g_pInputManager->GetInputBufferLength(i))	continue;
		
		g_pInputManager->RenderInputBuffer(i);
#endif

		nFontIndex++;
	}

#ifdef _USE_IME

	GET_IMEEDIT()->RenderSprite(59999); // ���忡�� IME ǥ�� �ȵǴ� ������ ���� BY DEEPDARK.

#endif
	
	DisplayDungeonInfoOnWorldMap(VAR->pCurSelectDungeon);
	RECT rect;

	if(VAR->bDisplayDungeonInfo)
	{
		VECTOR3 vOutPos;
		
		WORD x,y;
		char szBuf[ 0xff ]={0,};
		DUNGEON_DATA_EX* pDungeon;
		BYTE bCurrentWorldMapID = g_pDungeonTable->GetCurrentWorldMapID();
		POSITION_ pos = g_pDungeonTable->m_pInfoList[ bCurrentWorldMapID ]->GetHeadPosition();
		while(pos)
		{
			pDungeon = (DUNGEON_DATA_EX*)g_pDungeonTable->m_pInfoList[ bCurrentWorldMapID ]->GetNext(pos);

			GetScreenXYFromXYZ( g_pGeometry, 0, &pDungeon->vPos, &vOutPos);


			//if(vOutPos.x >= 1.0f || vOutPos.y < 0.4f || vOutPos.y >= 0.8f )		continue;
			if(vOutPos.x >= 1.0f || vOutPos.y < 0.1f || vOutPos.y >= 0.8f )		continue;

		//	if(vOutPos.y >= 0.8f || vOutPos.x >= 1.0f)	continue;		//�ý����� �� �ɶ��� �� ���ͷ� ����.. �Ѥ� 
		//	if(vOutPos.x > 0.8 && vOutPos.y < 0.4f)	continue;
									
			x = WORD(__GAME_SIZE_X__ * vOutPos.x);
			y = WORD(__GAME_SIZE_Y__ * vOutPos.y);
			
			if (SendPacketRequestDungeonInfo(WORD(pDungeon->m_dwID)))
				continue;
			
			if(pDungeon->IsConquer())	//���� ���� ���� 
			{
				wsprintf(szBuf, "%s\n%s", pDungeon->m_szDungeonName,	pDungeon->m_szOwner);

				int nLen = lstrlen(szBuf);

				rect.left	= x - (nLen/2)*8;
				rect.right	= rect.left + nLen*16;
				rect.top	= y - 100;
				rect.bottom = rect.top + 30;
				
				if(pDungeon->pSprMark)
				{
					pDungeon->pSprMark->vPos.x = float(rect.left - 34);
					pDungeon->pSprMark->vPos.y = float(rect.top - 3);
					g_pRenderer->RenderSprite(pDungeon->pSprMark->pSpr, NULL, 0.0, &pDungeon->pSprMark->vPos, NULL,
																pDungeon->pSprMark->dwAlpha, pDungeon->pSprMark->nZOrder, 0);	
				}				
				RenderFont(szBuf, rect.left, rect.right, rect.top, rect.bottom, 5);
			}
			else	//������ ���� 
			{				
#if IS_JAPAN_LOCALIZING()
				int nLen = 0;
				if(lstrlen(pDungeon->m_szSchoolName))
				{
					nLen = lstrlen(pDungeon->m_szSchoolName);
					lstrcpy(szBuf, pDungeon->m_szSchoolName);
				}
				else
				{
					nLen = lstrlen(pDungeon->m_szDungeonName);
					lstrcpy(szBuf, pDungeon->m_szDungeonName);
				}
#else
				int nLen = lstrlen(pDungeon->m_szDungeonName);
				lstrcpy(szBuf, pDungeon->m_szDungeonName);
#endif

				rect.left	= x - (nLen/2)*8;
				rect.right	= rect.left + nLen*8;
				rect.top	= y - 75;				
				rect.bottom = rect.top + 14;

				RenderFont(szBuf, rect.left, rect.right, rect.top, rect.bottom, 6, 0xffffff78);				
			}			
		}

		// �̺�Ʈ ���� �̸�.
		DrawEventDungeonName();	
	}

	VECTOR3 vPos;

	if(g_pMainPlayer->GetStatus()!=UNIT_STATUS_RIDE_AIRPLANE && g_pMainPlayer->GetStatus()!=UNIT_STATUS_RIDE_AIRPLANE_DOWN && g_pMainPlayer->GetStatus()!=UNIT_STATUS_AIRPLANE_FLOATING && g_pMainPlayer->GetStatus()!=UNIT_STATUS_AIRPLANE_DOWNING)
		g_pExecutive->GXOGetPosition(g_pMainPlayer->m_hPlayer.pHandle, &vPos);
	else 
	{	
		if(!g_pMainPlayer->m_RideObject.pHandle)
			return 0;

		g_pExecutive->GXOGetPosition(g_pMainPlayer->m_RideObject.pHandle, &vPos);
	}
	
	MAP_TILE* pTile = g_pMap->GetTile(vPos.x, vPos.z);

	if(pTile)
		SetMinimapPos(vPos);	

	//  ��Ƽ�� �̸� ��� 	
//	DWORD dwModelIndex = 0;
//	DWORD dwObjIndex = 0;	
	
	if( g_hHandle )
	{
		
		LPObjectDesc pObjDesc = (LPObjectDesc)g_pExecutive->GetData(g_hHandle);
		
		if( !pObjDesc ) return 0;

		if(pObjDesc->bType == OBJECT_TYPE_PLAYER )
		{
		
			LP_PARTY_USER	pUser = (LP_PARTY_USER)pObjDesc->pInfo;			
			
			VECTOR3	vPlayerHeadPos, vOutPos;
			WORD	x, y;
			vPlayerHeadPos = pUser->m_v3CurPos;
			vPlayerHeadPos.y = pUser->m_v3CurPos.y + 140;
			GetScreenXYFromXYZ(g_pGeometry, 0, &vPlayerHeadPos, &vOutPos);

			x = (WORD)( __GAME_SIZE_X__ * vOutPos.x );
			y = (WORD)( __GAME_SIZE_Y__ * vOutPos.y );

			rect.top	= (long)y; 
			rect.left	= (long)( x - 25 );
			rect.right	= (long)( rect.left + 100 );
			rect.bottom = (long)( rect.top + 12 );

			if(!IsEmptyString(pUser->szName))
				RenderFont(pUser->szName, rect.left, rect.right, rect.top, rect.bottom, 0);
		}
	}
	//
	//wsprintf(szTemp, "x:%6.1f, z:%6.1f, Tile_X:%d, Tile_Z:%d, ATTR:%d",vPos.x, vPos.z, pTile->wIndex_X, pTile->wIndex_Z, pTile->wAttr.uAttr);
	
	//RECT rect2;
	//rect2.left = 500;
	//rect2.top = 50;
	//rect2.right = 1200;
	//rect2.bottom = 90;

	//g_pGeometry->RenderFont( GetFont(), szTemp, lstrlen(szTemp),&rect2,0xffffffff,CHAR_CODE_TYPE_ASCII, 0, 0);		
	//rect2.top = 60;

#ifdef DEVELOP_MODE
	//Display Cursor Position For Debug
	LPMouseState	pMouse = GetMouse();
	char	szTemp[ 0xff ]={0,};	
	wsprintf(szTemp, "X: %d, Y:%d", pMouse->MousePos.x , pMouse->MousePos.y);
	//wsprintf(szTemp, "%s", m_szInputBuffer);
//	g_pGeometry->RenderFont( GetFont(), szTemp, lstrlen(szTemp),&rect2,0xffffffff,CHAR_CODE_TYPE_ASCII, 0, 0);
	
	//--
	{
		char szTemp[ 0xff ]={0,};
		rect.left = 550;
		rect.top = 10;
		rect.right = 1200;
		rect.bottom = 50;

		// sound count 
		if (g_pSoundLib)
		{
			wsprintf(szTemp, "AllocatedSoundFileNum : %ld | Allocated SounfEffectNum : %ld", 
				g_pSoundLib->GetAllocatedSoundFileCount(), g_pSoundLib->GetAllocatedSoundEffectCount() );

			RenderFont(szTemp, rect.left, rect.right, rect.top, rect.bottom, 255);			
		}
	}
#endif

	//Display Camera Position For Debug
	//CAMERA_DESC desc;
	//g_pGeometry->GetCameraDesc(&desc, 0);
	
	//VECTOR3 vRad;
	//g_pGeometry->GetCameraAngleRad(&vRad, 0);
	//wsprintf(szTemp, "From.x:%7.1f From.z:%7.1f To.x:%7.1f To.z:%7.1f Rad:%11.7f", desc.v3From.x, desc.v3From.z, desc.v3To.x, desc.v3To.z, vRad.x);
	//g_pGeometry->RenderFont( GetFont(), szTemp, lstrlen(szTemp),&rect,0xffffffff,CHAR_CODE_TYPE_ASCII);

	return 0;
}

DWORD __stdcall BeforeRenderGameWorld()
{
	return 0;
}

void SetMinimapPos(VECTOR3 vPos)
{	
	WORD wX, wY;
	float fX, fY;
	
	fX = float(vPos.x * MINIMAP_RATIO_X);
	fY = float(vPos.z * MINIMAP_RATIO_Y);
	fY = float(180.0 - fY);

	wX = WORD(/*800*/798 + fX);
	wY = WORD(3 + fY);

	VAR->pMinimapPos->vPos.x = wX;
	VAR->pMinimapPos->vPos.y = wY;
}

void OnKeyDownWorld(WPARAM wParam, LPARAM lParam)
{
	BOOL bHanMode = TRUE;

	switch(wParam)
	{	
#ifdef	DEVELOP_MODE
	case 192:
		{
			if(g_bKeyChkUp==FALSE)
			{				
				CDungeonInfoWnd::GetInstance()->SetActive();								
				g_bKeyChkUp = TRUE;				
			}
			bHanMode = FALSE;
		}
		break;
#endif
	case VK_F1:
		{
		  	if(VAR->bDisplayDungeonInfo)
				VAR->bDisplayDungeonInfo = 0;
			else
				VAR->bDisplayDungeonInfo = 1;

			bHanMode = FALSE;
		}
		break;

	case VK_UP:	 
		{
			bHanMode = FALSE;

			switch(g_pMainPlayer->GetStatus())
			{
			case UNIT_STATUS_RUNNING:
			case UNIT_STATUS_RIDE_AIRPLANE_DOWN:
			case UNIT_STATUS_RIDE_AIRPLANE:
			case UNIT_STATUS_AIRPLANE_DOWNING:
		  	case UNIT_STATUS_AIRPLANE_FLOATING:				
				break;
			}
		}
		break;

	case VK_DOWN:
		{
			bHanMode = FALSE;

			switch(g_pMainPlayer->GetStatus())
			{
			case UNIT_STATUS_RUNNING:
			case UNIT_STATUS_RIDE_AIRPLANE_DOWN:
			case UNIT_STATUS_RIDE_AIRPLANE:
			case UNIT_STATUS_AIRPLANE_DOWNING:
			case UNIT_STATUS_AIRPLANE_FLOATING:
				break;		
			}
		}
		break;

	case VK_RIGHT:
		{
			bHanMode = FALSE;
			
			switch(g_pMainPlayer->GetStatus())
			{
			case UNIT_STATUS_RUNNING:
			case UNIT_STATUS_RIDE_AIRPLANE_DOWN:
			case UNIT_STATUS_RIDE_AIRPLANE:
			case UNIT_STATUS_AIRPLANE_DOWNING:
			case UNIT_STATUS_AIRPLANE_FLOATING:
				break;
			}
		}
		break;

	case VK_LEFT:
		{
			bHanMode = FALSE;
			
			switch(g_pMainPlayer->GetStatus())
			{
			case UNIT_STATUS_RUNNING:
			case UNIT_STATUS_RIDE_AIRPLANE_DOWN:
			case UNIT_STATUS_RIDE_AIRPLANE:
			case UNIT_STATUS_AIRPLANE_DOWNING:
			case UNIT_STATUS_AIRPLANE_FLOATING:
				break;		
			}
		}
		break;

	case VK_RETURN:
		{		
			bHanMode = FALSE;

			if(VAR->bChatMode)	return;		//ä�ø��� ���� 
			
			if(g_pMainPlayer->GetStatus()== UNIT_STATUS_RIDE_AIRPLANE_DOWN)	//����� ���� 
			{
				g_bRideChk = TRUE;

				VECTOR3 vPos;
				g_pExecutive->GXOGetPosition(g_pMainPlayer->m_RideObject.pHandle, &vPos);

				MAP_TILE* pTile;
				pTile = g_pMap->GetTile(vPos.x + 250.0f, vPos.z);
				if(!pTile->wAttr.uAttr)
				{
					vPos.x += 250.0f;
					GXSetPosition(g_pMainPlayer->m_hPlayer.pHandle, &vPos, FALSE);
					g_pMainPlayer->m_v3CurPos = vPos;
					g_pMainPlayer->SetAction(MOTION_TYPE_VILLAGESTAND, 0, ACTION_LOOP);					
					ShowObject(g_pMainPlayer->m_hPlayer.pHandle);
					SetBoundingVolume(g_pMainPlayer->m_hPlayer.pHandle, 80.0f);

					g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
					VAR->bMainPlayerJumping = 0;

					CTC_AIRSHIP_GETOFF GetoffPacket;
					GetoffPacket.v3Pos = vPos;
					g_pNet->SendMsg( (char*)&GetoffPacket, GetoffPacket.GetPacketSize(), SERVER_INDEX_WORLD );
					
					//ĳ���Ϳ��� Light Attach
					if(!g_pExecutive->GXODetachLight(g_pMainPlayer->m_RideObject.pHandle, VAR->hMainSpotWorld))
						asm_int3();
					
					LIGHT_DESC	shadowlight;

					shadowlight.dwDiffuse = 0xffffffff;
					shadowlight.v3Point.x = g_pMainPlayer->m_v3CurPos.x + 500.0f;
					shadowlight.v3Point.y = 1000.0f;
					shadowlight.v3Point.z = g_pMainPlayer->m_v3CurPos.z + 600.0f;
					shadowlight.fFov = PI/2.0f;
					shadowlight.v3Up.x = 0.0f;
					shadowlight.v3Up.y = 1.0f;
					shadowlight.v3Up.z = 0.0f;

					shadowlight.fRs = 2000.0f;
					shadowlight.v3To = g_pMainPlayer->m_v3CurPos;

					g_pExecutive->GXLSetLightDesc(VAR->hMainSpotWorld,&shadowlight);

					if(!g_pExecutive->GXOAttachLight(g_pMainPlayer->m_hPlayer.pHandle,"Bip01",VAR->hMainSpotWorld, &VAR->vLightDistance,ATTACH_TYPE_LINK))
						asm_int3();

					g_pExecutive->GXOEnableHFieldApply( g_pMainPlayer->m_hPlayer.pHandle );

					SetCameraTarget(g_pMainPlayer->m_hPlayer.pHandle);
					return;
				}
			}
		}
		break;	
	case VK_SPACE:
		{
			bHanMode = FALSE;

			if(VAR->bChatMode)	//ä�ø��� ���� 
				return;		

			switch(g_pMainPlayer->GetStatus())
			{
			case UNIT_STATUS_RIDE_AIRPLANE_DOWN:
				{
					// �̷� 
					_PlaySound(0, SOUND_TYPE_GAMEPLAY, GAMEPLAY_AIRPLANE_LANDING, g_Camera.v3CameraPos, FALSE);

					LPObjectDesc pDesc = (LPObjectDesc)g_pExecutive->GetData(g_pMainPlayer->m_RideObject.pHandle);

				    if( g_pExecutive->GXOGetCurrentMotionIndex(g_pMainPlayer->m_RideObject.pHandle) == 1 )	//������ �ִ� ��� 
					{
						::SetAction(g_pMainPlayer->m_RideObject.pHandle, 2, 0, ACTION_ONCE);
						pDesc->ObjectFunc = OnFloatingPlane;

						g_pMainPlayer->SetStatus(UNIT_STATUS_AIRPLANE_FLOATING);	//�̷��� 
					}							
				}
				break;

			case UNIT_STATUS_RIDE_AIRPLANE:
				{
					// ���� 
					_PlaySound(0, SOUND_TYPE_GAMEPLAY, GAMEPLAY_AIRPLANE_TAKE_OFF, g_Camera.v3CameraPos, FALSE);
					LPObjectDesc pDesc = (LPObjectDesc)g_pExecutive->GetData(g_pMainPlayer->m_RideObject.pHandle);

					if( g_pExecutive->GXOGetCurrentMotionIndex(g_pMainPlayer->m_RideObject.pHandle) == 3 )	//���ִ°�� 
					{
						VECTOR3 vAirShipPos;	g_pExecutive->GXOGetPosition(g_pMainPlayer->m_RideObject.pHandle, &vAirShipPos);
						MAP_TILE* pTile = g_pMap->GetTile(vAirShipPos.x, vAirShipPos.z );
						
						if(pTile->wAttr.uOccupied == TILE_EMPTY)
						{
							::SetAction(g_pMainPlayer->m_RideObject.pHandle, 4, 0, ACTION_ONCE);
							pDesc->ObjectFunc = OnSettledownPlane;

							g_pMainPlayer->SetStatus(UNIT_STATUS_AIRPLANE_DOWNING);	//�̷��� 
						}
					}
				}
				break;
			}
		}
		break;		
	}
	
	if(bHanMode==TRUE)
	{	
		if(!VAR->bChatMode)
		{
			char szKey[0xff]={0,};
			GetKeyNameText(lParam, szKey, 10);

			if(__strcmp(szKey, "R")==0)
			{						
				if( g_pMainPlayer->m_bMoveType == UNIT_STATUS_WALKING ) g_pMainPlayer->m_bMoveType = UNIT_STATUS_RUNNING;
					else g_pMainPlayer->m_bMoveType = UNIT_STATUS_WALKING;
					
				if( g_pMainPlayer->GetStatus()== UNIT_STATUS_WALKING || g_pMainPlayer->GetStatus()== UNIT_STATUS_RUNNING )
				{
					g_pMainPlayer->SetStatus(g_pMainPlayer->m_bMoveType);
					if( g_pMainPlayer->GetStatus()== UNIT_STATUS_WALKING )
					{
						g_pMainPlayer->SetAction(MOTION_TYPE_WALK, 0, ACTION_LOOP);						
					}
					else if( g_pMainPlayer->GetStatus()== UNIT_STATUS_RUNNING )
					{
						g_pMainPlayer->SetAction(MOTION_TYPE_RUN, 0, ACTION_LOOP);
					}
				}
			}			
		}			
	}
}

void OnKeyUpWorld(WPARAM wParam, LPARAM lParam)
{
}

void OnLButtonDownWorld(WPARAM wParam, LPARAM lParam)
{			
	CInterface*	 pInterface	= CInterface::GetInstance();
	
	if(g_bLoadingChk)
		goto lbl_fail;
		
	if(pInterface->InterfaceCheck()==-1)
	{	
		pInterface->m_bActiveChk = TRUE;
		
		//goto lbl_fail;
	}
	else
	{		
		 goto lbl_fail;
	}
	
	// Moue Pointer //
	if(g_Mouse.MousePos.x>=0 && g_Mouse.MousePos.y>=747 && g_Mouse.MousePos.x<=439 && g_Mouse.MousePos.y<=767)
		SetMousePointerType(__MOUSE_POINTER_BUTTONCLICK__);
	else if(g_Mouse.MousePos.x>=578 && g_Mouse.MousePos.y>=734 && g_Mouse.MousePos.x<=586 && g_Mouse.MousePos.y<=766)
		SetMousePointerType(__MOUSE_POINTER_BUTTONCLICK__);			
	else if(g_Mouse.MousePos.x>=590 && g_Mouse.MousePos.y>=734 && g_Mouse.MousePos.x<=598 && g_Mouse.MousePos.y<=766)
		SetMousePointerType(__MOUSE_POINTER_BUTTONCLICK__);
	else if(g_Mouse.MousePos.x>=443 && g_Mouse.MousePos.y>=750 && g_Mouse.MousePos.x<=574 && g_Mouse.MousePos.y<=766)
		SetMousePointerType(__MOUSE_POINTER_BUTTONCLICK__);	
	else if(g_Mouse.MousePos.x>589 && g_Mouse.MousePos.y>641 && g_Mouse.MousePos.x<599 && g_Mouse.MousePos.y<731)
	{
		if(SPR(SPR_CHAT_SC)->vPos.y<=g_Mouse.MousePos.y && SPR(SPR_CHAT_SC)->vPos.y+12>=g_Mouse.MousePos.y)
			SetMousePointerType(__MOUSE_POINTER_BUTTONCLICK__);
		else
			SetMousePointerType(__MOUSE_POINTER_DEFAULTCLICK__);				
	}
	else
		SetMousePointerType(__MOUSE_POINTER_DEFAULTCLICK__);

	if(g_Mouse.MousePos.x>=0 && g_Mouse.MousePos.y>=747 && g_Mouse.MousePos.x<=439 && g_Mouse.MousePos.y<=767)
	{
#ifdef _USE_IME

		if(GET_IMEEDIT()->GetEditIndex()==2)
		{
			LPSTR lpszName = (LPSTR)GET_IMEEDIT()->GetImeEditString();			
					
			g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME);
			g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME, lpszName, lstrlen(lpszName));								
			
		}

		POINT pChatPoint;
		pChatPoint.x	= 10;
		pChatPoint.y	= 752;
		GET_IMEEDIT()->SetEditIndex(1);
		GET_IMEEDIT()->ClearImeEditString();
		GET_IMEEDIT()->ActivateIME(pChatPoint, __MAX_BUUFER_SIZE__);

				
#endif

		g_pInputManager->SetFocusInput(INPUT_BUFFER_WORLD_CHAT);
		
		VAR->bChatMode		= 1;
		g_nChatModeExt		= VAR->bChatMode;

		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_CHAT);
		g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME);

		g_Mouse.bLDown =false;
		goto lbl_fail;

	}
	else if(g_Mouse.MousePos.x>=578 && g_Mouse.MousePos.y>=734 && g_Mouse.MousePos.x<=586 && g_Mouse.MousePos.y<=766)
	{
		g_pSprManager->GetSprite(BUTTON_WDCHAT1)->ShowSprite(FALSE);
		g_pSprManager->GetSprite(BUTTON_WDCHAT2)->ShowSprite(TRUE);
		g_pSprManager->GetSprite(BUTTON_WDCHAT3)->ShowSprite(FALSE);
		g_nPGSelect	= 3;		

		g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME);

#ifdef _USE_IME

		if(GET_IMEEDIT()->GetEditIndex()==2)
		{
			LPSTR lpszName = (LPSTR)GET_IMEEDIT()->GetImeEditString();			
			g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME);
			g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME, lpszName, lstrlen(lpszName));								
			
		}

		POINT pChatPoint;
		pChatPoint.x	= 10;
		pChatPoint.y	= 752;
		GET_IMEEDIT()->SetEditIndex(1);
		GET_IMEEDIT()->ClearImeEditString();
		GET_IMEEDIT()->ActivateIME(pChatPoint, __MAX_BUUFER_SIZE__);

#endif

		g_pInputManager->SetFocusInput(INPUT_BUFFER_WORLD_CHAT);
		g_nChatId			= 0;

		if(!VAR->bChatMode)
		{
			VAR->bChatMode	= 1;
			g_nChatModeExt	= VAR->bChatMode;

		}
		g_Mouse.bLDown =false;
		goto lbl_fail;
	}
	else if(g_Mouse.MousePos.x>=590 && g_Mouse.MousePos.y>=734 && g_Mouse.MousePos.x<=598 && g_Mouse.MousePos.y<=766)
	{
		g_pSprManager->GetSprite(BUTTON_WDCHAT1)->ShowSprite(FALSE);
		g_pSprManager->GetSprite(BUTTON_WDCHAT2)->ShowSprite(FALSE);
		g_pSprManager->GetSprite(BUTTON_WDCHAT3)->ShowSprite(TRUE);
		g_nPGSelect	= 2;
		
		g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME);
		g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT);
		

#ifdef _USE_IME

		if(GET_IMEEDIT()->GetEditIndex()==2)
		{
			LPSTR lpszName = (LPSTR)GET_IMEEDIT()->GetImeEditString();			
					
			g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME);
			g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME, lpszName, lstrlen(lpszName));								
			
		}

		POINT pChatPoint;
		pChatPoint.x	= 10;
		pChatPoint.y	= 752;
		GET_IMEEDIT()->SetEditIndex(1);
		GET_IMEEDIT()->ClearImeEditString();
		GET_IMEEDIT()->ActivateIME(pChatPoint, __MAX_BUUFER_SIZE__);

#endif
		g_pInputManager->SetFocusInput(INPUT_BUFFER_WORLD_CHAT);
		g_nChatId			= 0;

		if(!VAR->bChatMode)
		{
			VAR->bChatMode	= 1;
			g_nChatModeExt	= VAR->bChatMode;

		}
		g_Mouse.bLDown =false;
		goto lbl_fail;
	}
	else if(g_Mouse.MousePos.x>=443 && g_Mouse.MousePos.y>=750 && g_Mouse.MousePos.x<=574 && g_Mouse.MousePos.y<=766)
	{		
		g_pSprManager->GetSprite(BUTTON_WDCHAT1)->ShowSprite(TRUE);
		g_pSprManager->GetSprite(BUTTON_WDCHAT2)->ShowSprite(FALSE);
		g_pSprManager->GetSprite(BUTTON_WDCHAT3)->ShowSprite(FALSE);		
		g_nPGSelect	= 0;

		if(!VAR->bChatMode)
		{
			VAR->bChatMode = 1;

		}

#ifdef _USE_IME

		if(GET_IMEEDIT()->GetEditIndex()!=2)
		{
			POINT pChatPoint;
			pChatPoint.x	= 458;
			pChatPoint.y	= 752;
			GET_IMEEDIT()->SetEditIndex(2);
			GET_IMEEDIT()->SetImeEditString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME));		
			GET_IMEEDIT()->ActivateIME(pChatPoint, __MAX_IDBUUFER_SIZE__);		
		}
		
#endif

		g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT);
		
		g_pInputManager->SetFocusInput(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME);
		g_nChatId			= 1;
		g_nChatModeExt		= VAR->bChatMode;

		g_Mouse.bLDown =false;
		goto lbl_fail;
	}
	else if(g_Mouse.MousePos.x>589 && g_Mouse.MousePos.y>641 && g_Mouse.MousePos.x<599 && g_Mouse.MousePos.y<731)
	{
		float fPos = float(g_Mouse.MousePos.y-641);
		
		if(g_nScrollPos>0)
		{
			g_nCurScrollPos	= int(fPos / (float)(90.0f/(float)(g_nScrollPos+1)));

			fPos = float(g_Mouse.MousePos.y-8);

			if(fPos>730-16)
				fPos = 730-16;
			else if(fPos<643)
				fPos = 643;

			g_pSprManager->GetSprite(SPR_CHAT_SC)->vPos.y = fPos;

			g_bDownChk = TRUE;
		}		

		g_Mouse.bLDown =false;
		goto lbl_fail;
	}	

	// ������������ ���� ��Ÿ���� �����̾����µ�..	
	
	if( PathFindWorld() )
	{		
		SetPathFindMoveWorld();
	}
	return;
lbl_fail:
		
;
// g_Mouse.bLDown =false;	// ��ã�⸦ ���� ����� 
	
}

void OnLButtonUpWorld(WPARAM wParam, LPARAM lParam)
{
	if(g_bLoadingChk)
		return;
	CInterface*	pInterface	= CInterface::GetInstance();
	
	pInterface->SetUp(TRUE);	
	pInterface->InterfaceCheck();
			
	if(pInterface->GetClick())
		pInterface->SetClick(FALSE);

	pInterface->m_bActiveChk	= FALSE;
	
	if(g_bDownChk)
		g_bDownChk = FALSE;

	// Moue Pointer //	
	if(g_Mouse.MousePos.x>=0 && g_Mouse.MousePos.y>=747 && g_Mouse.MousePos.x<=439 && g_Mouse.MousePos.y<=767)
		SetMousePointerType(__MOUSE_POINTER_BUTTON__);
	else if(g_Mouse.MousePos.x>=578 && g_Mouse.MousePos.y>=734 && g_Mouse.MousePos.x<=586 && g_Mouse.MousePos.y<=766)
		SetMousePointerType(__MOUSE_POINTER_BUTTON__);			
	else if(g_Mouse.MousePos.x>=590 && g_Mouse.MousePos.y>=734 && g_Mouse.MousePos.x<=598 && g_Mouse.MousePos.y<=766)
		SetMousePointerType(__MOUSE_POINTER_BUTTON__);
	else if(g_Mouse.MousePos.x>=443 && g_Mouse.MousePos.y>=750 && g_Mouse.MousePos.x<=574 && g_Mouse.MousePos.y<=766)
		SetMousePointerType(__MOUSE_POINTER_BUTTON__);	
	else if(g_Mouse.MousePos.x>589 && g_Mouse.MousePos.y>641 && g_Mouse.MousePos.x<599 && g_Mouse.MousePos.y<731)
	{
		if(SPR(SPR_CHAT_SC)->vPos.y<=g_Mouse.MousePos.y && SPR(SPR_CHAT_SC)->vPos.y+12>=g_Mouse.MousePos.y)
			SetMousePointerType(__MOUSE_POINTER_BUTTON__);
		else
			SetMousePointerType(__MOUSE_POINTER_DEFAULT__);		
	}
	else
		SetMousePointerType(__MOUSE_POINTER_DEFAULT__);
}

void OnRButtonDownWorld(WPARAM wParam, LPARAM lParam)
{
	if(g_bLoadingChk)
		return;
	
	switch(g_pMainPlayer->GetStatus())
	{
	case UNIT_STATUS_RIDE_AIRPLANE_DOWN:
		{
			LPObjectDesc pDesc = (LPObjectDesc)g_pExecutive->GetData(g_pMainPlayer->m_RideObject.pHandle);

			if( g_pExecutive->GXOGetCurrentMotionIndex(g_pMainPlayer->m_RideObject.pHandle) == 1 )	//������ �ִ� ��� 
			{
				::SetAction(g_pMainPlayer->m_RideObject.pHandle, 2, 0, ACTION_ONCE);
				pDesc->ObjectFunc = OnFloatingPlane;

				g_pMainPlayer->SetStatus(UNIT_STATUS_AIRPLANE_FLOATING);	//�̷��� 
			}									
		}
		break;

	case UNIT_STATUS_RIDE_AIRPLANE:
		{
			LPObjectDesc pDesc = (LPObjectDesc)g_pExecutive->GetData(g_pMainPlayer->m_RideObject.pHandle);

			if( g_pExecutive->GXOGetCurrentMotionIndex(g_pMainPlayer->m_RideObject.pHandle) == 3 )	//���ִ°�� 
			{

				VECTOR3 vAirShipPos;	g_pExecutive->GXOGetPosition(g_pMainPlayer->m_RideObject.pHandle, &vAirShipPos);
				MAP_TILE* pTile = g_pMap->GetTile(vAirShipPos.x, vAirShipPos.z );
				
				if(pTile->wAttr.uOccupied == TILE_EMPTY)
				{
					::SetAction(g_pMainPlayer->m_RideObject.pHandle, 4, 0, ACTION_ONCE);
					pDesc->ObjectFunc = OnSettledownPlane;

					g_pMainPlayer->SetStatus(UNIT_STATUS_AIRPLANE_DOWNING);	//�̷��� 
				}
			}
		}
		break;
	}
}

void OnRButtonUpWorld(WPARAM wParam, LPARAM lParam)
{
	if(g_bLoadingChk)
		return;
}

void OnMouseMoveWorld(WPARAM wParam, LPARAM lParam)
{		
	if(g_bLoadingChk)
		return;
	
	CInterface* pInterface	= CInterface::GetInstance();
	
	pInterface->SetUp(FALSE);
	
	if(pInterface->m_bActiveChk==FALSE)	
		pInterface->InterfaceCheck();	
	
	if(!g_Mouse.bLDown)	
		pInterface->m_bActiveChk = FALSE;

	if(g_Mouse.bLDown==TRUE)
	{	
		// Moue Pointer //
		if(g_Mouse.MousePos.x>=0 && g_Mouse.MousePos.y>=747 && g_Mouse.MousePos.x<=439 && g_Mouse.MousePos.y<=767)
			SetMousePointerType(__MOUSE_POINTER_BUTTONCLICK__);
		else if(g_Mouse.MousePos.x>=578 && g_Mouse.MousePos.y>=734 && g_Mouse.MousePos.x<=586 && g_Mouse.MousePos.y<=766)
			SetMousePointerType(__MOUSE_POINTER_BUTTONCLICK__);			
		else if(g_Mouse.MousePos.x>=590 && g_Mouse.MousePos.y>=734 && g_Mouse.MousePos.x<=598 && g_Mouse.MousePos.y<=766)
			SetMousePointerType(__MOUSE_POINTER_BUTTONCLICK__);
		else if(g_Mouse.MousePos.x>=443 && g_Mouse.MousePos.y>=750 && g_Mouse.MousePos.x<=574 && g_Mouse.MousePos.y<=766)
			SetMousePointerType(__MOUSE_POINTER_BUTTONCLICK__);	
		else if(g_Mouse.MousePos.x>589 && g_Mouse.MousePos.y>641 && g_Mouse.MousePos.x<599 && g_Mouse.MousePos.y<731)
		{
			if(SPR(SPR_CHAT_SC)->vPos.y<=g_Mouse.MousePos.y && SPR(SPR_CHAT_SC)->vPos.y+12>=g_Mouse.MousePos.y)
				SetMousePointerType(__MOUSE_POINTER_BUTTONCLICK__);
			else
				SetMousePointerType(__MOUSE_POINTER_DEFAULTCLICK__);		
		}
		else
			SetMousePointerType(__MOUSE_POINTER_DEFAULTCLICK__);
	}
	else
	{	
		// Moue Pointer //
		if(g_Mouse.MousePos.x>=0 && g_Mouse.MousePos.y>=747 && g_Mouse.MousePos.x<=439 && g_Mouse.MousePos.y<=767)
			SetMousePointerType(__MOUSE_POINTER_BUTTON__);
		else if(g_Mouse.MousePos.x>=578 && g_Mouse.MousePos.y>=734 && g_Mouse.MousePos.x<=586 && g_Mouse.MousePos.y<=766)
			SetMousePointerType(__MOUSE_POINTER_BUTTON__);			
		else if(g_Mouse.MousePos.x>=590 && g_Mouse.MousePos.y>=734 && g_Mouse.MousePos.x<=598 && g_Mouse.MousePos.y<=766)
			SetMousePointerType(__MOUSE_POINTER_BUTTON__);
		else if(g_Mouse.MousePos.x>=443 && g_Mouse.MousePos.y>=750 && g_Mouse.MousePos.x<=574 && g_Mouse.MousePos.y<=766)
			SetMousePointerType(__MOUSE_POINTER_BUTTON__);	
		else if(g_Mouse.MousePos.x>589 && g_Mouse.MousePos.y>641 && g_Mouse.MousePos.x<599 && g_Mouse.MousePos.y<731)
		{
			if(SPR(SPR_CHAT_SC)->vPos.y<=g_Mouse.MousePos.y && SPR(SPR_CHAT_SC)->vPos.y+12>=g_Mouse.MousePos.y)
				SetMousePointerType(__MOUSE_POINTER_BUTTON__);
			else
				SetMousePointerType(__MOUSE_POINTER_DEFAULT__);		
		}
		else
			SetMousePointerType(__MOUSE_POINTER_DEFAULT__);
	}

	if(g_Mouse.MousePos.x>589 && g_Mouse.MousePos.y>641 && g_Mouse.MousePos.x<599 && g_Mouse.MousePos.y<731)
	{
		if(g_Mouse.bLDown)
		{
			float fPos = float(g_Mouse.MousePos.y-641);
		
			if(g_nScrollPos>0)
			{
				g_nCurScrollPos	= int(fPos / (float)(90.0f/(float)(g_nScrollPos+1)));

				fPos = float(g_Mouse.MousePos.y-8);

				if(fPos>730-16)
					fPos = 730-16;
				else if(fPos<643)
					fPos = 643;

				g_pSprManager->GetSprite(SPR_CHAT_SC)->vPos.y = fPos;
			}
		}
	}
	else if(g_bDownChk==TRUE)	
	{
		if(g_Mouse.bLDown)
		{
			float fMousePos = float(g_Mouse.MousePos.y);

			if(fMousePos<641)
				fMousePos = 641;
			else if(fMousePos>=731)
				fMousePos = 730;

			float fPos = fMousePos-641;
		
			if(g_nScrollPos>0)
			{
				g_nCurScrollPos	= int(fPos / (float)(90.0f/(float)(g_nScrollPos+1)));

				fPos = fMousePos-8;

				if(fPos>730-16)
					fPos = 730-16;
				else if(fPos<643)
					fPos = 643;

				g_pSprManager->GetSprite(SPR_CHAT_SC)->vPos.y = fPos;			
			}
		}
	}	
}

void OnMouseWheelWorld(WPARAM wParam, LPARAM lParam)
{
}

void OnCharWorld(BYTE bCh)
{
	switch(bCh)
	{
		case VK_ESCAPE:	
		{	
			CExitWnd*			pExitWnd		= CExitWnd::GetInstance();
			CCommandWnd*		pCommandWnd		= CCommandWnd::GetInstance();
			CHelpWnd*			pHelpWnd		= CHelpWnd::GetInstance();
			COkWnd*				pOkWnd			= COkWnd::GetInstance();
			CDungeonInfoWnd*	pDungeonInfoWnd	= CDungeonInfoWnd::GetInstance();
						
			if(pDungeonInfoWnd->GetActive()==TRUE)
				pDungeonInfoWnd->SetActive(FALSE);
			if(pCommandWnd->GetActive()==TRUE)
				pCommandWnd->SetActive(FALSE);
			if(pHelpWnd->GetActive()==TRUE)
				pHelpWnd->SetActive(FALSE);
			if(pOkWnd->GetActive()==TRUE)			
				pOkWnd->SetActive(FALSE);
						
			pExitWnd->SetActive(!pExitWnd->GetActive());			
		}
		return;	
	case 8:	//BackSpace
		{
#ifdef _USE_IME
			if (GET_IMEEDIT()->IsActivated()) return;
#endif
			g_pInputManager->BackSpaceEvent();
			return;
		}
		break;
	case VK_RETURN:
		{		
			if(g_pMainPlayer->GetStatus()== UNIT_STATUS_RIDE_AIRPLANE)
				return;

			if(g_bRideChk==TRUE)
			{
				g_bRideChk = FALSE;
				return;
			}

			g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT);
			g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME);
			
			if(VAR->bChatMode==FALSE)
			{
#ifdef _USE_IME
				POINT pChatPoint;
				pChatPoint.x	= 10;
				pChatPoint.y	= 752;
				GET_IMEEDIT()->SetEditIndex(1);
			//	GET_IMEEDIT()->ClearImeEditString(); // ���� �Է��� �����!
				GET_IMEEDIT()->ActivateIME(pChatPoint, __MAX_BUUFER_SIZE__);
#endif
				g_pInputManager->SetFocusInput(INPUT_BUFFER_WORLD_CHAT);
				VAR->bChatMode		= 1;
				g_nChatModeExt		= VAR->bChatMode;

				g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_CHAT);
				return;
			}
			else
			{
				if(g_pInputManager->GetCurFocusInputID()==INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME)
				{
#ifdef _USE_IME
					LPSTR lpszName = (LPSTR)GET_IMEEDIT()->GetImeEditString();			
					
					g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME,lpszName, lstrlen(lpszName));					
					

					POINT pChatPoint;
					pChatPoint.x	= 10;
					pChatPoint.y	= 752;
					GET_IMEEDIT()->SetEditIndex(1);
					GET_IMEEDIT()->ClearImeEditString();
					GET_IMEEDIT()->ActivateIME(pChatPoint, __MAX_BUUFER_SIZE__);
#endif
					g_pInputManager->SetFocusInput(INPUT_BUFFER_WORLD_CHAT);
					VAR->bChatMode		= 1;
					g_nChatModeExt		= VAR->bChatMode;

					g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_CHAT);
					return;
				}
				else
				{

#ifdef _USE_IME
					LPSTR lpszName = (LPSTR)GET_IMEEDIT()->GetImeEditString();			
					
					g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_CHAT);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_CHAT, lpszName, lstrlen(lpszName));
					

					GET_IMEEDIT()->ClearImeEditString();
#endif
					char szBuffer[0xff];	memset(szBuffer, 0, sizeof(szBuffer));
					
					__lstrcpyn(szBuffer, g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT), lstrlen(g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT)));

					char* szTemp = strtok(szBuffer, " ");

					if(!szTemp)
					{
						IMESetEdit(0);
						
						g_pInputManager->SetFocusInput(INPUT_BUFFER_WORLD_CHAT);
						VAR->bChatMode		= 0;
						g_nChatModeExt		= VAR->bChatMode;
						::KillTimer(g_hMainWnd, 1);
						g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_CHAT);
						return;
					}

					int nRt = ConvString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT));

					if(nRt==1)
					{				
						// MSG_ID : 58 ; ��Ӿ �弳�� ����� �� �����ϴ�.
						DisplayMessageAdd(g_Message[ETC_MESSAGE58].szMessage, 0xFFFF0000);	
						g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_CHAT);
						_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_Camera.v3CameraPos, FALSE);
						return;
					}

#if IS_CHINA_LOCALIZING() // �߱��� ��� �����ܾ ä�� ���ϰ� �ؾ� �Ѵ�!!
					nRt = NoConvString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT));
					
					if(nRt==1)
					{				
						// MSG_ID : 58 ; ��Ӿ �弳�� ����� �� �����ϴ�.
						DisplayMessageAdd(g_Message[ETC_MESSAGE59].szMessage, 0xFFFF0000);	
						g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_CHAT);
						_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_Camera.v3CameraPos, FALSE);
						return;
					}
#endif

					CScript* pScript = CScript::GetInstance(g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT));
					
					int nChk = pScript->ChkCommand();
					
					if(nChk==__CHAT_COMMAND__)			
					{
						// ��ɾ� �˻� //
						int nRt = pScript->MsgCheck();

						if(nRt==__SCRT_SUCCESS__)				
						{
						//	int nRtEx = pScript->MsgExecute(__SERVER_WORLD__);
							
							g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_CHAT);
							return;
						}
						else
						{
							// "������ �߸��Ǿ����ϴ�."
							DisplayMessageAdd(g_Message[ETC_MESSAGE669].szMessage, 0xFFFF0000); 
							_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_Camera.v3CameraPos, FALSE);

							g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_CHAT);
							return;
						}
					}
					
					if(nChk==__CHAT_COMMAND__)
					{
						if(nRt!=__SCRT_FAIL__)
						{
							g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_CHAT);
							return;
						}
					}
					
					if(g_nPGSelect==0)
					{
						if(g_pInputManager->GetInputBufferLength(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME)!=0)
						{
							if(g_pInputManager->GetInputBufferLength(INPUT_BUFFER_WORLD_CHAT)!=0)
							{
								if(__strcmp(g_pMainPlayer->m_szName, g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME))==0)
								{
									DisplayMessageAdd(g_Message[ETC_MESSAGE64].szMessage, 0xffff9a63);	// MSG_ID : 64 ; �ڱ� �ڽſ��Դ� �ӼӸ��� �Ҽ������ϴ�.
									_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_Camera.v3CameraPos, FALSE);
								}
								else									
								{	
									char	szText[0xff]={0,};																										
									char	szText2[0xff]={0,};									
									char*	szToken=0;
									char	szSeps[]	= "',?";

									// Id �˻� //
									__lstrcpyn(szText, g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME), MAX_CHARACTER_NAME_REAL_LENGTH);
									__lstrcpyn(szText2, g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT), lstrlen(g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT))+1);

									szToken = strtok(szText, szSeps);

									if(szToken)
									{
										if(__strcmp(szToken, g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME))!=0)
										{
											DisplayMessageAdd(g_Message[ETC_MESSAGE57].szMessage, 0xFFFFB894);	// MSG_ID : 57 ; ���� ���ڸ� ����Ҽ��� �����ϴ�(', ?).
											_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_Camera.v3CameraPos, FALSE);
											
											g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_CHAT);
											g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME);
											
											return;
										}
									}
									else
									{
										DisplayMessageAdd(g_Message[ETC_MESSAGE57].szMessage, 0xFFFFB894);	// MSG_ID : 57 ; ���� ���ڸ� ����Ҽ��� �����ϴ�(', ?).
										_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_Camera.v3CameraPos, FALSE);
										
										g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_CHAT);
										g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME);
										return;
									}

									WSTC_WS_CHAT packet;
									memset(packet.cScript.szId, 0, sizeof(packet.cScript.szId));
									memset(packet.cScript.szMessage, 0, sizeof(packet.cScript.szMessage));
									memset(packet.cScript.szText, 0, sizeof(packet.cScript.szText));				

									szToken = strtok(szText2, szSeps);
									memset(&packet.cScript, 0, sizeof(packet.cScript));

									if(szToken)
									{
										if(__strcmp(szToken, g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT))==0)
										{
											__lstrcpyn(szText, g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT), lstrlen(g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT)));
											__lstrcpyn(packet.cScript.szText, g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT),lstrlen(g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT)));
										}
										else
										{
											memset(szText, 0, sizeof(szText));

											lstrcat(szText, szToken);
											lstrcat(szText, " ");

											while(szToken)
											{
												szToken = strtok(NULL, szSeps);

												if(szToken)
												{
													lstrcat(szText, szToken);
													lstrcat(szText, " ");
												}												
											}
											__lstrcpyn(packet.cScript.szText, szText, sizeof(szText));
										}													
									}									
									packet.cScript.enCommand = COMMAND_WHISPER;
									__lstrcpyn(pScript->m_szName, g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME), MAX_CHARACTER_NAME_REAL_LENGTH);
									__lstrcpyn(packet.cScript.szId, g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME), MAX_CHARACTER_NAME_REAL_LENGTH);
									__lstrcpyn(packet.cScript.szMessage, g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME), lstrlen(g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME)));
 									g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_WORLD);
								}
							}
							else
							{								
								DisplayMessageAdd(g_Message[ETC_MESSAGE65].szMessage, 0xffff9a63);	// MSG_ID : 65 ; ������ �޼����� �����ϴ�.
								_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_Camera.v3CameraPos, FALSE);
							}
						}
						else
						{
							DisplayMessageAdd(g_Message[ETC_MESSAGE66].szMessage, 0xffff9a63);	// MSG_ID : 66 ; ������ ���� ĳ���Ϳ� �̸��� �����ϴ�.
							_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_Camera.v3CameraPos, FALSE);
						}
					}
					else
					{	
						//  ���, ��Ƽ //
						if(g_pInputManager->GetInputBufferLength(INPUT_BUFFER_WORLD_CHAT)!=0)
						{
							if(g_nPGSelect==2)
							{
								if(g_pMainPlayer->m_dwGuildId!=0 && g_pGuildUserHash->GetCount()!=0)
								{
									WSTC_CHAT_USER pPacket;
									memset(pPacket.szId, 0, sizeof(pPacket.szId));
									memset(pPacket.szMessage, 0, sizeof(pPacket.szMessage));
									pPacket.nIndex = g_nPGSelect;				
									__lstrcpyn(pPacket.szId, g_pMainPlayer->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
									__lstrcpyn(pPacket.szMessage, g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT), lstrlen(g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT)));
									g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
								}
								else
								{
									if(g_pMainPlayer->m_dwGuildId==0)
										DisplayMessageAdd(g_Message[ETC_MESSAGE67].szMessage, 0xff00ffff); // MSG_ID : 67 ; ���Ե� ��尡 �����ϴ�.
									if(g_pGuildUserHash->GetCount()==0)
										DisplayMessageAdd(g_Message[ETC_MESSAGE68].szMessage, 0xff00ffff);	// MSG_ID : 68 ; �α����� ������ �����ϴ�.
								}
							}
							else if(g_nPGSelect==3)
							{
								if(g_pMainPlayer->m_dwPartyId!=0)
								{
									WSTC_CHAT_USER pPacket;
									memset(pPacket.szId, 0, sizeof(pPacket.szId));
									memset(pPacket.szDstId, 0, sizeof(pPacket.szDstId));
									memset(pPacket.szMessage, 0, sizeof(pPacket.szMessage));
									
									pPacket.nIndex = g_nPGSelect;				
									__lstrcpyn(pPacket.szId, g_pMainPlayer->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
									__lstrcpyn(pPacket.szMessage, g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT), lstrlen(g_pInputManager->GetInputBuffer(INPUT_BUFFER_WORLD_CHAT)));				
									g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
								}
								else
									DisplayMessageAdd(g_Message[ETC_MESSAGE69].szMessage, 0xffffff00);	// MSG_ID : 69 ; �������� ��Ƽ�� �����ϴ�.
							}
						}
						else
						{
							DisplayMessageAdd(g_Message[ETC_MESSAGE65].szMessage, 0xffffff00);	// MSG_ID : 65 ; ������ �޼����� �����ϴ�.
							_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_Camera.v3CameraPos, FALSE);
						}
					}	
					g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_CHAT);
					
				}
			}	
			return;
		}
		break;
	}

	if(!VAR->bChatMode)
		return;

	if(g_pInputManager->GetCurFocusInputID()==INPUT_BUFFER_WORLD_CHAT)
	{
		if(g_pInputManager->GetInputBufferLength(INPUT_BUFFER_WORLD_CHAT)>=__MAX_BUUFER_SIZE__) //ID�� Password ���������� ���� 
			return;
	}
	if(g_pInputManager->GetCurFocusInputID()==INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME)
	{
		if(g_pInputManager->GetInputBufferLength(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME)>=__MAX_IDBUUFER_SIZE__) //ID�� Password ���������� ���� 
			return;
	}
	g_pInputManager->InsertCharacter(g_pInputManager->GetCurFocusInputID(), (char*)&bCh, 1);
}


void OnTimerEventWorld(DWORD dwTimerIndex)
{
	g_pInputManager->BlinkInputCaret();
}

void MoveProcessWalk()
{
	float fSpeed;	// = 15.0f;

	if(g_pMainPlayer->GetStatus()== UNIT_STATUS_RUNNING)// || VAR->bMainPlayerJumping)
		fSpeed = WORLD_RUNNING_SPEED;
	else
		fSpeed = WORLD_WALK_SPEED;

	VECTOR3 v3Pos;
	if(g_KeyPress.IsUp() && g_KeyPress.IsRight() )
	{
		g_pExecutive->GXOSetDirection(g_pMainPlayer->m_hPlayer.pHandle, &g_Camera.v3AxsiY, DEG90 + DEG45);
		g_pExecutive->EstimatedMoveForward(g_pMainPlayer->m_hPlayer.pHandle, &v3Pos, fSpeed);
		MAP_TILE* pTile = g_pMap->GetTile(v3Pos.x, v3Pos.z);
		if(pTile->wAttr.uAttr != 1)
		{
			g_pExecutive->GXOMoveForward(g_pMainPlayer->m_hPlayer.pHandle, fSpeed);
			SetCameraTarget(g_pMainPlayer->m_hPlayer.pHandle);
		}
		
		return;
	}

	if(g_KeyPress.IsDown() && g_KeyPress.IsRight() )
	{
		g_pExecutive->GXOSetDirection(g_pMainPlayer->m_hPlayer.pHandle, &g_Camera.v3AxsiY, DEG45);
		g_pExecutive->EstimatedMoveForward(g_pMainPlayer->m_hPlayer.pHandle, &v3Pos, fSpeed);
		MAP_TILE* pTile = g_pMap->GetTile(v3Pos.x, v3Pos.z);
		if(pTile->wAttr.uAttr != 1)
		{
			g_pExecutive->GXOMoveForward(g_pMainPlayer->m_hPlayer.pHandle, fSpeed);
			SetCameraTarget(g_pMainPlayer->m_hPlayer.pHandle);
		}
		return;
	}

	if(g_KeyPress.IsDown() && g_KeyPress.IsLeft() )
	{
		g_pExecutive->GXOSetDirection(g_pMainPlayer->m_hPlayer.pHandle, &g_Camera.v3AxsiY, DEG270 + DEG45);
		g_pExecutive->EstimatedMoveForward(g_pMainPlayer->m_hPlayer.pHandle, &v3Pos, fSpeed);
		MAP_TILE* pTile = g_pMap->GetTile(v3Pos.x, v3Pos.z);
		if(pTile->wAttr.uAttr != 1)
		{
			g_pExecutive->GXOMoveForward(g_pMainPlayer->m_hPlayer.pHandle, fSpeed);
			SetCameraTarget(g_pMainPlayer->m_hPlayer.pHandle);
		}
		return;
	}

	if(g_KeyPress.IsUp() && g_KeyPress.IsLeft() )
	{
		g_pExecutive->GXOSetDirection(g_pMainPlayer->m_hPlayer.pHandle, &g_Camera.v3AxsiY, DEG180 + DEG45);
		g_pExecutive->EstimatedMoveForward(g_pMainPlayer->m_hPlayer.pHandle, &v3Pos, fSpeed);
		MAP_TILE* pTile = g_pMap->GetTile(v3Pos.x, v3Pos.z);
		if(pTile->wAttr.uAttr != 1)
		{
			g_pExecutive->GXOMoveForward(g_pMainPlayer->m_hPlayer.pHandle, fSpeed);
			SetCameraTarget(g_pMainPlayer->m_hPlayer.pHandle);
		}
		return;
	}

	if(g_KeyPress.IsUp())
	{
		g_pExecutive->GXOSetDirection(g_pMainPlayer->m_hPlayer.pHandle, &g_Camera.v3AxsiY, DEG180);
		g_pExecutive->EstimatedMoveForward(g_pMainPlayer->m_hPlayer.pHandle, &v3Pos, fSpeed);
		MAP_TILE* pTile = g_pMap->GetTile(v3Pos.x, v3Pos.z);
		if(pTile->wAttr.uAttr != 1)
		{
			g_pExecutive->GXOMoveForward(g_pMainPlayer->m_hPlayer.pHandle, fSpeed);
			SetCameraTarget(g_pMainPlayer->m_hPlayer.pHandle);
		}
		return;
	}
	
	if( g_KeyPress.IsRight() )
	{
		g_pExecutive->GXOSetDirection(g_pMainPlayer->m_hPlayer.pHandle, &g_Camera.v3AxsiY, DEG90);
		g_pExecutive->EstimatedMoveForward(g_pMainPlayer->m_hPlayer.pHandle, &v3Pos, fSpeed);
		MAP_TILE* pTile = g_pMap->GetTile(v3Pos.x, v3Pos.z);
		if(pTile->wAttr.uAttr != 1)
		{
			g_pExecutive->GXOMoveForward(g_pMainPlayer->m_hPlayer.pHandle, fSpeed);
			SetCameraTarget(g_pMainPlayer->m_hPlayer.pHandle);
		}
		return;
	}
	
	if( g_KeyPress.IsLeft() )
	{
		g_pExecutive->GXOSetDirection(g_pMainPlayer->m_hPlayer.pHandle, &g_Camera.v3AxsiY, DEG270);
		g_pExecutive->EstimatedMoveForward(g_pMainPlayer->m_hPlayer.pHandle, &v3Pos, fSpeed);
		MAP_TILE* pTile = g_pMap->GetTile(v3Pos.x, v3Pos.z);
		if(pTile->wAttr.uAttr != 1)
		{
			g_pExecutive->GXOMoveForward(g_pMainPlayer->m_hPlayer.pHandle, fSpeed);
			SetCameraTarget(g_pMainPlayer->m_hPlayer.pHandle);
		}
		return;
	}
	
	if( g_KeyPress.IsDown() )
	{
		g_pExecutive->GXOSetDirection(g_pMainPlayer->m_hPlayer.pHandle, &g_Camera.v3AxsiY, DEG360);
		g_pExecutive->EstimatedMoveForward(g_pMainPlayer->m_hPlayer.pHandle, &v3Pos, fSpeed);
		MAP_TILE* pTile = g_pMap->GetTile(v3Pos.x, v3Pos.z);
		if(pTile->wAttr.uAttr != 1)
		{
			g_pExecutive->GXOMoveForward(g_pMainPlayer->m_hPlayer.pHandle, fSpeed);
			SetCameraTarget(g_pMainPlayer->m_hPlayer.pHandle);
		}
		return;
	}
}

void MoveProcessPlane()
{
	float fSpeed = 50.0f;
	VECTOR3 v3Pos;
	if(g_KeyPress.IsUp() && g_KeyPress.IsRight() )
	{
		g_pExecutive->GXOSetDirection(g_pMainPlayer->m_RideObject.pHandle, &g_Camera.v3AxsiY, DEG90 + DEG45);
		g_pExecutive->EstimatedMoveForward(g_pMainPlayer->m_RideObject.pHandle, &v3Pos, fSpeed);
		MAP_TILE* pTile = g_pMap->GetTile(v3Pos.x, v3Pos.z);
		if(pTile)
		{
			g_pExecutive->GXOMoveForward(g_pMainPlayer->m_RideObject.pHandle, fSpeed);
			SetCameraTargetWithHeight(g_pMainPlayer->m_RideObject.pHandle);
		}

		return;
	}

	if(g_KeyPress.IsDown() && g_KeyPress.IsRight() )
	{
		g_pExecutive->GXOSetDirection(g_pMainPlayer->m_RideObject.pHandle, &g_Camera.v3AxsiY, DEG45);
		g_pExecutive->EstimatedMoveForward(g_pMainPlayer->m_RideObject.pHandle, &v3Pos, fSpeed);
		MAP_TILE* pTile = g_pMap->GetTile(v3Pos.x, v3Pos.z);
		if(pTile)
		{
			g_pExecutive->GXOMoveForward(g_pMainPlayer->m_RideObject.pHandle, fSpeed);
			SetCameraTargetWithHeight(g_pMainPlayer->m_RideObject.pHandle);
		}
		return;
	}

	if(g_KeyPress.IsDown() && g_KeyPress.IsLeft() )
	{
		g_pExecutive->GXOSetDirection(g_pMainPlayer->m_RideObject.pHandle, &g_Camera.v3AxsiY, DEG270 + DEG45);
		g_pExecutive->EstimatedMoveForward(g_pMainPlayer->m_RideObject.pHandle, &v3Pos, fSpeed);
		MAP_TILE* pTile = g_pMap->GetTile(v3Pos.x, v3Pos.z);
		if(pTile)
		{
			g_pExecutive->GXOMoveForward(g_pMainPlayer->m_RideObject.pHandle, fSpeed);
			SetCameraTargetWithHeight(g_pMainPlayer->m_RideObject.pHandle);
		}
		return;
	}

	if(g_KeyPress.IsUp() && g_KeyPress.IsLeft() )
	{
		g_pExecutive->GXOSetDirection(g_pMainPlayer->m_RideObject.pHandle, &g_Camera.v3AxsiY, DEG180 + DEG45);
		g_pExecutive->EstimatedMoveForward(g_pMainPlayer->m_RideObject.pHandle, &v3Pos, fSpeed);
		MAP_TILE* pTile = g_pMap->GetTile(v3Pos.x, v3Pos.z);
		if(pTile)
		{
			g_pExecutive->GXOMoveForward(g_pMainPlayer->m_RideObject.pHandle, fSpeed);
			SetCameraTargetWithHeight(g_pMainPlayer->m_RideObject.pHandle);
		}
		return;
	}

	if(g_KeyPress.IsUp())
	{
		g_pExecutive->GXOSetDirection(g_pMainPlayer->m_RideObject.pHandle, &g_Camera.v3AxsiY, DEG180);
		g_pExecutive->EstimatedMoveForward(g_pMainPlayer->m_RideObject.pHandle, &v3Pos, fSpeed);
		MAP_TILE* pTile = g_pMap->GetTile(v3Pos.x, v3Pos.z);
		if(pTile)
		{
			g_pExecutive->GXOMoveForward(g_pMainPlayer->m_RideObject.pHandle, fSpeed);
			SetCameraTargetWithHeight(g_pMainPlayer->m_RideObject.pHandle);
		}
		return;
	}
	
	if( g_KeyPress.IsRight() )
	{
		g_pExecutive->GXOSetDirection(g_pMainPlayer->m_RideObject.pHandle, &g_Camera.v3AxsiY, DEG90);
		g_pExecutive->EstimatedMoveForward(g_pMainPlayer->m_RideObject.pHandle, &v3Pos, fSpeed);
		MAP_TILE* pTile = g_pMap->GetTile(v3Pos.x, v3Pos.z);
		if(pTile)
		{
			g_pExecutive->GXOMoveForward(g_pMainPlayer->m_RideObject.pHandle, fSpeed);
			SetCameraTargetWithHeight(g_pMainPlayer->m_RideObject.pHandle);
		}
		return;
	}
	
	if( g_KeyPress.IsLeft() )
	{
		g_pExecutive->GXOSetDirection(g_pMainPlayer->m_RideObject.pHandle, &g_Camera.v3AxsiY, DEG270);
		g_pExecutive->EstimatedMoveForward(g_pMainPlayer->m_RideObject.pHandle, &v3Pos, fSpeed);
		MAP_TILE* pTile = g_pMap->GetTile(v3Pos.x, v3Pos.z);
		if(pTile)
		{
			g_pExecutive->GXOMoveForward(g_pMainPlayer->m_RideObject.pHandle, fSpeed);
			SetCameraTargetWithHeight(g_pMainPlayer->m_RideObject.pHandle);
		}
		return;
	}
	
	if( g_KeyPress.IsDown() )
	{
		g_pExecutive->GXOSetDirection(g_pMainPlayer->m_RideObject.pHandle, &g_Camera.v3AxsiY, DEG360);
		g_pExecutive->EstimatedMoveForward(g_pMainPlayer->m_RideObject.pHandle, &v3Pos, fSpeed);
		MAP_TILE* pTile = g_pMap->GetTile(v3Pos.x, v3Pos.z);
		if(pTile)
		{
			g_pExecutive->GXOMoveForward(g_pMainPlayer->m_RideObject.pHandle, fSpeed);
			SetCameraTargetWithHeight(g_pMainPlayer->m_RideObject.pHandle);
		}
		return;
	}
	
}

void CheckNoticeTime(void)
{
#if !IS_JAPAN_LOCALIZING()
	g_dwNextTime = timeGetTime();

	if((g_dwNextTime-g_dwPrevTime) > MAX_NOTICE_TIME)
	{
		g_byNoticeCount = 0;
		g_bNoticeFlag	= FALSE;

		CGameNoticeMessageWnd* pNoticeWnd = CGameNoticeMessageWnd::GetInstance();
		pNoticeWnd->SetActive(FALSE);

		return;
	}
#endif
}

void RenderNotice()
{	
	// 2004�� 2�� 9�� ���� �� �Լ��� ȣ����� �ʴ´�. 
	// ������ ����, ���������� �������̽� â�� ���� �ʰ� ������� 
	// ������ �� ��Ȳ�� ����Ͽ� ����ϱ� ���ؼ� �������� �ʰ� ���ܵд�. 
	
	RECT rtPos;

	g_dwNextTime = timeGetTime();

	if((g_dwNextTime-g_dwPrevTime) > MAX_NOTICE_TIME)
	{
		g_byNoticeCount = 0;
		g_bNoticeFlag	= FALSE;
		return;
	}	
	
	for(int i = 0; i < g_byNoticeCount; i++)
	{
		int nSize = (g_byLen[i] / 2) * 8;

		rtPos.left		= 512 - nSize;
		rtPos.right		= rtPos.left + g_byLen[i] * 8;
		rtPos.top		= 30 + (i * 18);
		rtPos.bottom	= 44 + (i * 18);
		RenderFont(g_sNoticeMessage[i], rtPos.left, rtPos.right, rtPos.top, rtPos.bottom, __ORDER_NOTICE_START__+i);
	}
}

void MoveToUserPrevPos()
{
	//char szMsg[ 0xff ];

	VECTOR3 vPos;	BYTE bFlag = 0;	float fDistance = 100.0f;
	MAP_TILE* pTile;

	float fRad;
	g_pExecutive->GXOGetDirection(g_pMainPlayer->m_hPlayer.pHandle, &g_Camera.v3AxsiY, &fRad);
	g_pExecutive->GXOSetDirection(g_pMainPlayer->m_hPlayer.pHandle, &g_Camera.v3AxsiY, fRad + DEG180);	//�ڵ��ƺ��� �ϰ� 

lb_TileAgain:
	g_pExecutive->EstimatedMoveForward(g_pMainPlayer->m_hPlayer.pHandle, &vPos, fDistance);
	switch(bFlag)
	{
	case 0:	vPos.x -= 200.0f; bFlag++;	break;
	case 1:	vPos.x += 200.0f; bFlag++;	break;
	case 2:	vPos.z -= 200.0f; bFlag++;	break;
	case 3:	vPos.z += 200.0f; bFlag++;	break;
	case 4: 
		{
			DisplayMessageAdd(g_Message[ETC_MESSAGE416].szMessage, 0xFFFFC309); // MSG_ID : 416 ; [���� ���� ����] �̵������� ������ �����ϴ�! ����˴ϴ�.
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_Camera.v3CameraPos, FALSE);
			g_bIsRunning = FALSE;
			return;
		}					
		break;
	}

	pTile = g_pMap->GetTile(vPos.x, vPos.z);
 	if(pTile->wAttr.uAttr == 1)
	{
		goto lb_TileAgain;
	}

	g_pExecutive->GXOMoveForward(g_pMainPlayer->m_hPlayer.pHandle, fDistance);
	g_pExecutive->GXOGetDirection(g_pMainPlayer->m_hPlayer.pHandle, &g_Camera.v3AxsiY, &fRad);
	g_pExecutive->GXOSetDirection(g_pMainPlayer->m_hPlayer.pHandle, &g_Camera.v3AxsiY, fRad - DEG180);	//�ٽ� �ڵ��ƺ��� �ϰ� 

	//���� ���� 
	float y;
	g_pExecutive->GXOGetPosition(g_pMainPlayer->m_hPlayer.pHandle, &vPos);
	g_pExecutive->GXMGetHFieldHeight(&y, vPos.x, vPos.z);
	vPos.y = y;
	GXSetPosition(g_pMainPlayer->m_hPlayer.pHandle, &vPos, FALSE);

	//�ٸ� ����� ��Ƽ ������ ��ġ ���� 
	CTC_MOVE_POSITION	MovePacket;
	MovePacket.v3Pos = vPos;
	MovePacket.fDirection =  (fRad - DEG180);
	g_pNet->SendMsg((char*)&MovePacket, MovePacket.GetPacketSize(), SERVER_INDEX_WORLD	);

	//�ٽ� ����ȭ������ 
	g_pSprManager->GetSprite(SPR_LOADING_BACK)->ShowSprite(FALSE);
	VAR->bDisplayDungeonInfo = 1;

	LPObjectDesc pDesc = (LPObjectDesc)g_pExecutive->GetData(g_pMainPlayer->m_hPlayer.pHandle);// = OnCrashUser;
	pDesc->CrashFunc = OnCrashUser;
	
	g_pMainPlayer->SetAction( MOTION_TYPE_VILLAGESTAND, 0, ACTION_LOOP );
	g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);	//���� ���·� 
	pDesc->ObjectFunc = NULL;	
	
	VAR->bMainPlayerJumping = 0;
	VAR->dwDownMoveTick = 0;
	VAR->dwRightMoveTick = 0;
	VAR->dwLeftMoveTick = 0;
	VAR->dwUpMoveTick = 0;
	
	g_pExecutive->GXOEnableHFieldApply(g_pMainPlayer->m_hPlayer.pHandle);
}

BOOL InitWorldPlayer( WORLD_PARTY_APPEAR_INFO* pAppear, LP_PARTY_USER pPlayer )
{
	LP_ITEM_RESOURCE_EX	lpItemResource;
//	LP_ITEM_TABLE		pItemTable;
	CBaseItem*		pItemTable;
	char				szInfo[0xff]={0,};

	//--- ���⼭ �ٵ�� ���� ���ؽ� ��ȣ������.. �������� �˾ƿ��� �ڵ� ����.
	pPlayer->m_hPlayer.pDesc = AllocObjDesc();
	memset(pPlayer->m_hPlayer.pDesc, 0, sizeof(OBJECT_DESC));
	
	pPlayer->m_hPlayer.pDesc->bType			= OBJECT_TYPE_PLAYER;
	pPlayer->m_hPlayer.pDesc->pInfo			= (void*)pPlayer;		// �ɸ��� Ŭ������ �ּ� �Է�.
	pPlayer->m_hPlayer.pDesc->ObjectFunc	= NULL;	

	// �÷��̾ ���� //
	if(!pPlayer->m_hPlayer.pHandle)
	{
		if(pAppear->wArmor==0)
			pPlayer->m_hPlayer.pHandle = CreateHandleObject(GetFile(RESTYPE_BASE_BODY, pAppear->wClass), GXPlayerPROC, pPlayer->m_hPlayer.pDesc, NULL);
		else
		{			
			lpItemResource = g_pItemResourceHash->GetData(pAppear->wArmor);

			if(lpItemResource)
			{
				if(lpItemResource->wModCount>0)					
				{
					ItemDataName(szInfo, pAppear->wArmor, pAppear->wClass-1);
									
					pPlayer->m_hPlayer.pHandle = CreateHandleObject(GetFile(szInfo, DATA_TYPE_CHARACTER), GXPlayerPROC, pPlayer->m_hPlayer.pDesc, NULL);					
				}			
			}
		}
	}

	// �÷��̾ �Ӹ� //	
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

	// �÷��̾ ��� //
	if(!pPlayer->m_hPlayerHelmet.pHandle)
	{
		if(pAppear->wHelmet!=0)
		{
			lpItemResource = g_pItemResourceHash->GetData(pAppear->wHelmet);

			if(lpItemResource)
			{
				if(lpItemResource->wModCount>0)
					ItemAttach(pAppear->wHelmet, &pPlayer->m_hPlayerHelmet.pHandle, pPlayer->m_hPlayerHelmet.pDesc, &pPlayer->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_HEAD, 0);
			}
		}		
	}

	// �÷��̾ ������ //
	if(pAppear->wHandR!=0)
	{
		if(!pPlayer->m_hPlayerRHand.pHandle)
		{
			lpItemResource = g_pItemResourceHash->GetData(pAppear->wHandR);
			
			if(lpItemResource)
			{
				if(lpItemResource->wModCount>0)
					ItemAttach(pAppear->wHandR, &pPlayer->m_hPlayerRHand.pHandle, pPlayer->m_hPlayerRHand.pDesc, &pPlayer->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_RHAND, 0);
			}
		}
		 
		pItemTable = g_pItemTableHash->GetData(pAppear->wHandR);

		if(pItemTable)
		{
			if(pItemTable->BaseItem_Weapon.bModCount==2)
			{
				if(!pPlayer->m_hPlayerLHand.pHandle)
				{
					lpItemResource = g_pItemResourceHash->GetData(pAppear->wHandR);

					if(lpItemResource)
					{
						if(lpItemResource->wModCount>0)
							ItemAttach(pAppear->wHandR, &pPlayer->m_hPlayerLHand.pHandle, pPlayer->m_hPlayerLHand.pDesc, &pPlayer->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_LHAND, 0);
					}
				}
			}		
		}
		pPlayer->m_byItemType = BYTE((pAppear->wHandR/ITEM_DISTRIBUTE)+1);
	}
	
	// �÷��̾ �޼� //
	if(pAppear->wHandL!=0)
	{
		if(!pPlayer->m_hPlayerLHand.pHandle)
		{
			lpItemResource = g_pItemResourceHash->GetData(pAppear->wHandL);
			
			if(lpItemResource)
			{
				if(lpItemResource->wModCount>0)
					ItemAttach(pAppear->wHandL, &pPlayer->m_hPlayerLHand.pHandle, pPlayer->m_hPlayerLHand.pDesc, &pPlayer->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_LHAND, 0);
			}
		}
	}	

	VECTOR3 v3Pos;
	v3Pos.x = pAppear->fAppear_X;
	v3Pos.z = pAppear->fAppear_Z;
	g_pExecutive->GXMGetHFieldHeight(&v3Pos.y, v3Pos.x, v3Pos.z);

//	DWORD dwResult = g_pExecutive->IsValidHandle( pPlayer->m_hPlayer.pHandle );
	g_pExecutive->GXOEnableHFieldApply( pPlayer->m_hPlayer.pHandle );
	GXSetPosition(pPlayer->m_hPlayer.pHandle, &v3Pos, FALSE);
	
	MinimizePlayer(pPlayer->m_hPlayer.pHandle, TRUE);

	return TRUE;
}


void SetActionForPartyUser( GXOBJECT_HANDLE handle, WORD wAct, int nframe, BYTE bFlag )
{
	LPObjectDesc	pDesc = (LPObjectDesc)g_pExecutive->GetData( handle );

	if(!pDesc)
	{
		// MSG_ID : 278 ; Fail to ChangeAction!
		MessageBox(g_hMainWnd,g_Message[ETC_MESSAGE278].szMessage,NULL,NULL);
		return;
	}  

	if(pDesc->bType==OBJECT_TYPE_PLAYER || pDesc->bType==OBJECT_TYPE_MAINPLAYER)
	{
		LP_PARTY_USER pUser = (LP_PARTY_USER)pDesc->pInfo;

		if(pUser)
		{
			wAct%=max(pUser->m_byItemType*50, 50);
			wAct = WORD(pUser->m_byItemType*50+wAct);
		}
	}
	 
	pDesc->bActionFlag	= bFlag;

	if( nframe > 0 )
	{
		pDesc->nFrame	= 0;
		g_pExecutive->GXOSetCurrentFrame( handle, nframe );
		pDesc->nCurFrame = nframe;
	}
	else
	{
		pDesc->nFrame		= 0;
		g_pExecutive->GXOSetCurrentFrame( handle, 0 );
		pDesc->nCurFrame = 0;
	}

	g_pExecutive->GXOSetCurrentMotionIndex( handle, wAct );
}

void FreeWorldPartyUser( LP_PARTY_USER pPartyUser )
{
	DeleteObj(&pPartyUser->m_hPlayerHead.pHandle);
	DeleteObj(&pPartyUser->m_hPlayerRHand.pHandle);
	DeleteObj(&pPartyUser->m_hPlayerLHand.pHandle);
	DeleteObj(&pPartyUser->m_hPlayerHelmet.pHandle);
	
	DeleteObj(&pPartyUser->m_hPlayer.pHandle);
	ObjDeleteDesc(&pPartyUser->m_hPlayer.pDesc);
}

void PlayerWorldMoveFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	VECTOR3		Pos;
	MAP_TILE*	pTile;
	g_pExecutive->GXOGetPosition(handle, &Pos);				// ������ ��ġ�� ������.

	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;			// ���� ���� ������.	
	LP_PARTY_USER pUser		= (LP_PARTY_USER)pData->pInfo;
	
	float fSpeed;
	pObjDesc->dwCurTick		= g_dwCurTick;					
	DWORD dwTime			= pObjDesc->dwCurTick - pObjDesc->dwStartTick;
	pObjDesc->dwStartTick	= pObjDesc->dwCurTick;
	
	if( pUser->GetStatus() == UNIT_STATUS_WALKING ) 
		fSpeed = WORLD_WALKING_SPEED_RATIO * dwTime;
	else 
		fSpeed = WORLD_RUNNING_SPEED_RATIO * dwTime;
	
	// ���� �̵��� Ÿ���� ������ Ÿ�ϰ� �ٸ� Ÿ���̶�� �ɸ��Ͱ� ���ִ� Ÿ�� �Ӽ��� �ű��.
	g_pExecutive->GXOMoveForward( handle, fSpeed );	// �̵� �������� ��� �����Ѵ�.
	g_pExecutive->GXOGetPosition( handle, &Pos );
	
	// ������ ��ġ�� �Űܰ����� ��ġ�� �˻��ؼ� ���� �߰��� ������ Ÿ���� �ִٸ� ����������.
	pTile = g_pMap->GetTile( Pos.x, Pos.z );
	if( !pTile )
	{
		GXSetPosition( handle, &pUser->m_v3CurPos, FALSE );
		
		::SetAction(pUser->m_hPlayer.pHandle, MOTION_TYPE_VILLAGESTAND, 0, ACTION_LOOP );		
		pUser->SetStatus(UNIT_STATUS_NORMAL);
		return;
	}

	//������ Ÿ���̸�  
	if( pTile->wAttr.uAttr )
	{
		GXSetPosition( handle, &pUser->m_v3CurPos , FALSE);
		
		::SetAction(pUser->m_hPlayer.pHandle, MOTION_TYPE_VILLAGESTAND, 0, ACTION_LOOP );		
		pUser->SetStatus(UNIT_STATUS_NORMAL);
		return;
	}

	SetWorldUserPosition( pUser, &Pos );	
	return;
}

void SetWorldUserPosition( LP_PARTY_USER pUser, VECTOR3* pv3Pos, BOOL bFlag /* = TRUE  */ )
{
	MAP_TILE* pTile = g_pMap->GetTile( pv3Pos->x, pv3Pos->z );

	if (!pTile)
		return;
	
	pUser->m_v3CurPos.x = pv3Pos->x;
	pUser->m_v3CurPos.y	= pv3Pos->y;
	pUser->m_v3CurPos.z = pv3Pos->z;
	
	if(pUser->m_v3CurPos.x < 0)
		pUser->m_v3CurPos.x = 0;
	else if(pUser->m_v3CurPos.x > g_pMap->m_fMapXLength)
		pUser->m_v3CurPos.x = g_pMap->m_fMapXLength-1;
	
	if(pUser->m_v3CurPos.z < 0)
		pUser->m_v3CurPos.z = 0;
	else if(pUser->m_v3CurPos.z > g_pMap->m_fMapZLength)
		pUser->m_v3CurPos.z = g_pMap->m_fMapZLength-1;

	if(bFlag)
	{
		g_pExecutive->GXMGetHFieldHeight(&pUser->m_v3CurPos.y, pUser->m_v3CurPos.x, pUser->m_v3CurPos.z);
		GXSetPosition( pUser->m_hPlayer.pHandle, &pUser->m_v3CurPos, FALSE );

		if( g_pMainPlayer->m_dwUserIndex != pUser->dwUserIndex )
		{
			if( pUser->m_pCurTile != pTile )
			{			
				pUser->m_pCurTile = pTile;
			}
		}
	}

	if( g_pMainPlayer->m_dwUserIndex == pUser->dwUserIndex )
	{
		SetCameraTarget( g_pMainPlayer->m_hPlayer.pHandle);	
	}
}

void InitWorldMessage()
{
	for(int i = 0; i < __MAX_WORD_QUEUE__; i++)
	{
		memset(g_sWordChatMsg[i].szDungeon, 0, sizeof(g_sWordChatMsg[i].szDungeon));
		g_sWordChatMsg[i].sdwChatColor	= 0xffffffff;
		g_sWordChatMsg[i].snSize		= 0;
	}
	g_nWordIndex	= 0;
	g_nWordStart	= 0;
	g_nWordEnd		= 0;
	g_nMsgInx		= 0;
	g_nCurScrollPos	= 0;
	g_nScrollPos	= 0;
}
//----------------------------------------------------------------------------
// Name : DeleteEventDungeonObjects
// Description : 
//----------------------------------------------------------------------------
bool DeleteDungeonObjects(void)
{
	if (NULL == g_pDungeonTable)
	{
		return false;
	}

	if (NULL == g_pDungeonTable->m_pEventList)
	{
		return FALSE;
	}

	DUNGEON_DATA_EX* pDungeon	= NULL;
	POSITION_ pos				= g_pDungeonTable->m_pEventList->GetHeadPosition();

	while (pos)
	{
		pDungeon = (DUNGEON_DATA_EX*)g_pDungeonTable->m_pEventList->GetNext(pos);

		// �̺�Ʈ ���� �׷��� ���ҽ� ����.
		if (NULL != pDungeon->m_hEventDungeonModel)
		{
			g_pExecutive->ReleaseID(pDungeon->m_hEventDungeonModel);
			DeleteHandleObject(pDungeon->m_hEventDungeonModel);
			pDungeon->m_hEventDungeonModel = NULL;
		}

		if (NULL != pDungeon->m_hEventDungeonEffect)
		{
			g_pExecutive->ReleaseID(pDungeon->m_hEventDungeonEffect);
			DeleteHandleObject(pDungeon->m_hEventDungeonEffect);
			pDungeon->m_hEventDungeonEffect = NULL;
		}
	}
	return true;
}
//----------------------------------------------------------------------------
// Name : DrawEventDungeonName
// Description : ȭ�鿡 �̺�Ʈ ���� �̸����� ���.
//----------------------------------------------------------------------------
void DrawEventDungeonName(void)
{	
	VECTOR3 vOutPos;
	WORD	x,y;
	RECT	rect;

	DUNGEON_DATA_EX* pDungeon;
	char szBuf[ 0xff ] = {0,};

	POSITION_ pos = g_pDungeonTable->m_pEventList->GetHeadPosition();
	
	while (pos)
	{
		pDungeon = (DUNGEON_DATA_EX*)g_pDungeonTable->m_pEventList->GetNext(pos);

		if (NULL == pDungeon)
		{
			continue;
		}		

		// ���� �������, �̺�Ʈ ������ ���� ������϶��� ó��.
		if (pDungeon->m_wWorldMapID != g_pDungeonTable->GetCurrentWorldMapID())
		{
			return;
		}		

		GetScreenXYFromXYZ( g_pGeometry, 0, &pDungeon->vPos, &vOutPos);

		x = WORD(__GAME_SIZE_X__ * vOutPos.x);
		y = WORD(__GAME_SIZE_Y__ * vOutPos.y);

		if (vOutPos.x >= 1.0f || vOutPos.y < 0.1f || vOutPos.y >= 0.8f )
		{
			continue;
		}

		if (DUNGEON_TYPE_EVENT == pDungeon->GetDungeonType() &&
			EVENT_DUNGEON_OPEN == pDungeon->m_bEventState)
			
		{
			int nLen = lstrlen(pDungeon->m_szDungeonName);

			rect.left	= x - (nLen/2)*8;
			rect.right	= rect.left + nLen*8;
			rect.top	= y - 50;				
			rect.bottom = rect.top + 14;

			lstrcpy(szBuf, pDungeon->m_szDungeonName);
			RenderFont(szBuf, rect.left, rect.right, rect.top, rect.bottom, 6);			
		}
	}
}
//----------------------------------------------------------------------------
// Name : CmdEventStateChange
// Description : 
//----------------------------------------------------------------------------
void ShowEventDungeonModel(DUNGEON_DATA_EX* pDungeon)
{	
	if (NULL == pDungeon)
	{
		return;
	}	

	if (DUNGEON_TYPE_EVENT != pDungeon->GetDungeonType())
	{
		return;
	}

	// ���� �������, �̺�Ʈ ������ ���� ������϶��� ó��.
	if (pDungeon->m_wWorldMapID != g_pDungeonTable->GetCurrentWorldMapID())
	{
		return;
	}
/*
	char szOutMessage[0xff] = {0,};
	
	switch (pDungeon->m_bEventState)
	{
		case EVENT_DUNGEON_CLOSE:
			break;
		case EVENT_DUNGEON_READY:
			break;
		case EVENT_DUNGEON_OPEN:
			memset(szOutMessage, 0, 0xff);			
			wsprintf(szOutMessage, g_Message[ETC_MESSAGE1050].szMessage, pDungeon->m_szDungeonName); // "[ %s ] �̺�Ʈ ������ ���µǾ����ϴ�."
			DisplayMessageAdd(szOutMessage, 0xFFFFC309);
			break;
		case EVENT_DUNGEON_OPEN_FULL:
		case EVENT_DUNGEON_OPEN_TIME_UP:			
			//"�̺�Ʈ ���� �Ա��� �������ϴ�."
			wsprintf(szOutMessage,g_Message[ETC_MESSAGE1051].szMessage,pDungeon->m_szDungeonName);
			DisplayMessageAdd(szOutMessage,0xffffffff);
			break;
		default:
			break;
	}*/
}
//----------------------------------------------------------------------------
// Name : SendQueryEventDungeonInfo
// Description : ���� ������ �̺�Ʈ ���� ������ ��û��.
//----------------------------------------------------------------------------
void SendQueryEventDungeonInfo(void)
{
	CTWS_QUERY_EVENT_DUNGEON_INFO packet;

	if (FALSE == g_bReceiveEventInfo)
	{
		packet.bRequestType = 1;
		g_bReceiveEventInfo = TRUE;
	}
	else
	{
		packet.bRequestType = 2;
	}

	g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_WORLD);	

	//"GM" �˻�
#ifdef GM_TOOL// : 050111 hwoarang
	CTWS_CHECK_GM CheckGmPacket;
	g_pNet->SendMsg((char*)&CheckGmPacket, CheckGmPacket.GetPacketSize(), SERVER_INDEX_WORLD);
#endif

}
//----------------------------------------------------------------------------
// Name : ClearEventInfo
// Description :
//----------------------------------------------------------------------------
void ClearEventInfo(void)
{
	int iTotalNode = g_pDungeonTable->m_pEventList->GetCount();
	DUNGEON_DATA_EX* pDungeon = NULL;
	
	for(int i = 0; i < iTotalNode; i++)
	{
		pDungeon = (DUNGEON_DATA_EX*)g_pDungeonTable->m_pEventList->GetHead();		
		g_pDungeonTable->Remove(pDungeon);
	}
}

void DisplayDungeonInfoOnWorldMap(DUNGEON_DATA_EX* pDungeon)
{	
	V2_SPRITE*	pSpr = SPR(SPR_SPEAKING_BOX);	
	VAR->pCurSelectDungeon = pDungeon;		//������ ���� 
	g_pDungeonTable->HideAllDungeonGroupEffect();
	if(pDungeon)
	{
		g_pDungeonTable->ShowEffectDungeonGroup(pDungeon->m_wGroupID, 1);

		VECTOR3 vOutPos;
		GetScreenXYFromXYZ( g_pGeometry, 0, &pDungeon->vPos, &vOutPos);

		LONG x = LONG(__GAME_SIZE_X__ * vOutPos.x);
		LONG y = LONG(__GAME_SIZE_Y__ * vOutPos.y);
		RECT rt;
		rt.left = x+10;	rt.right = x+250;	rt.top = y+10;	rt.bottom = y+30;
		g_pInputManager->InitializeInput(INPUT_BUFFER_7, FONT_D3D_GAME, rt, 0xffffff00, 101);
		rt.left = x+10;	rt.right = x+350;	rt.top = y+10;	rt.bottom = y+30;
		g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_DUNGEON_TYPE, FONT_D3D_GAME, rt, 0xffffffff, 101);
		rt.left = x+10;	rt.right = x+250;	rt.top = y+35;	rt.bottom = y+55;
		g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_DUNGEON_LEVELTYPE, FONT_D3D_GAME, rt, 0xffffffff, 101);
		rt.left = x+10;	rt.right = x+250;	rt.top = y+35;	rt.bottom = y+55;
		g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_DUNGEON_LEVEL, FONT_D3D_GAME, rt, 0xffffffff, 101);
		rt.left = x+10;	rt.right = x+250;	rt.top = y+60;	rt.bottom = y+80;
		g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_DUNGEON_LAYER_COUNT, FONT_D3D_GAME, rt, 0xffffffff, 101);
		rt.left = x+10;	rt.right = x+250;	rt.top = y+85;	rt.bottom = y+105;
		g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_DUNGEON_GUILD, FONT_D3D_GAME, rt, 0xffffffff, 101);
		rt.left = x+10;	rt.right = x+250;	rt.top = y+110;	rt.bottom = y+130;
		g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_DUNGEON_MANAGER2, FONT_D3D_GAME, rt, 0xffffffff, 101);
		rt.left = x+10;	rt.right = x+250;	rt.top = y+135;	rt.bottom = y+155;
		g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_DUNGEON_SIEGE_DEFENSECOUNT, FONT_D3D_GAME, rt, 0xffffffff, 101);

#if IS_JAPAN_LOCALIZING()
		rt.left = x+10;	rt.right = x+350;	rt.top = y+160;	rt.bottom = y+180;
#else
		rt.left = x+10;	rt.right = x+350;	rt.top = y+160;	rt.bottom = y+180;
#endif		
		g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_DUNGEON_ITEM, FONT_D3D_GAME, rt, 0xffffffff, 101);
		rt.left = x+10;	rt.right = x+250;	rt.top = y+185;	rt.bottom = y+205;
		g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_DUNGEON_SIEGE_TIME, FONT_D3D_GAME, rt, 0xffff00ff, 101);
		rt.left = x+10;	rt.right = x+250;	rt.top = y+210;	rt.bottom = y+230;
		g_pInputManager->InitializeInput(INPUT_BUFFER_WORLD_DUNGEON_LEVELUP_TIME, FONT_D3D_GAME, rt, 0xffffffff, 101);
				
		pSpr->ShowSprite(TRUE);
		pSpr->vPos.x  = float(x);
		pSpr->vPos.y  = float(y);
						
	}
	else
	{
		g_pInputManager->ClearInput(INPUT_BUFFER_7);
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_SIEGE_DEFENSECOUNT);
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_SIEGE_TIME);
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_LEVELUP_TIME);
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_MANAGER);
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_MEMO);
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_TYPE);
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_LEVELTYPE);
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_LEVEL);
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_LAYER_COUNT);
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_GUILD);
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_MANAGER2);
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_ITEM);
		pSpr->ShowSprite(FALSE);
		VAR->pCurSelectDungeon = NULL;
		
		return;
	}
				
	int nHour;
	int nMinute;
	int nSec;
	char szTemp[0xff]={0,};

	if(pDungeon->IsConquer())
	{
		// ���� ���� ����.
		pSpr->SetScaling(130, 105);
		wsprintf(szTemp, g_Message[ETC_MESSAGE1174].szMessage, pDungeon->m_szDungeonName); // "�̸� : %s"

		switch(pDungeon->m_byProperty % 3)
		{
		case 2:
			lstrcat(szTemp, g_Message[ETC_MESSAGE1175].szMessage); // "(���̾�)"
			break;
		case 1:
			lstrcat(szTemp, g_Message[ETC_MESSAGE1176].szMessage); // "(����)"
			break;
		case 0:
			lstrcat(szTemp, g_Message[ETC_MESSAGE1177].szMessage); // "(����Ʈ)"
			break;
		}
		
		g_pInputManager->ClearInput(INPUT_BUFFER_7);
		g_pInputManager->InsertCharacter(INPUT_BUFFER_7, szTemp, lstrlen(szTemp));		
		
//		LP_ITEM_TABLE pItem = g_pItemTableHash->GetData( pDungeon->m_wProduction );
		CBaseItem* pItem = g_pItemTableHash->GetData( pDungeon->m_wProduction );

		char szItemName[MAX_PATH]={0,};

		if(!pItem)
			lstrcpy(szItemName, g_Message[ETC_MESSAGE399].szMessage);	// MSG_ID : 399 ; None
		else
			lstrcpy(szItemName, pItem->szItemName_Kor);
		
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_SIEGE_DEFENSECOUNT);
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_LEVEL);
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_LAYER_COUNT);
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_GUILD);
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_MANAGER2);
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_ITEM);
		
		// MSG_ID : 400 ;  ����: Lv.%d\n\n ����: ����%d��\n\n �������: %s\n\n ������: %s\n\n ����ǰ: %s (%d)
		wsprintf(szTemp, g_Message[ETC_MESSAGE347].szMessage, pDungeon->GetLevel()); // "����: Lv.%d"
		g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_LEVEL, szTemp, lstrlen(szTemp));
		wsprintf(szTemp, g_Message[ETC_MESSAGE665].szMessage, pDungeon->m_wLayerCount); // "����: ����%d��"
		g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_LAYER_COUNT, szTemp, lstrlen(szTemp));
		wsprintf(szTemp, g_Message[ETC_MESSAGE666].szMessage, pDungeon->m_szManager); // "�������: %s"
		g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_GUILD, szTemp, lstrlen(szTemp));
		wsprintf(szTemp, g_Message[ETC_MESSAGE729].szMessage, pDungeon->m_szOwner); // "������: %s"
		g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_MANAGER2, szTemp, lstrlen(szTemp));

		if (pDungeon->m_wProduction != g_DungeonProductionItemMinMax[pDungeon->m_byProperty].wItemIDDefault)
			wsprintf(szTemp, g_Message[ETC_MESSAGE1178].szMessage, szItemName, (pDungeon->m_byProductionCount+1)*25); // "����ǰ: %s (%d%%)"
		else
			wsprintf(szTemp, g_Message[ETC_MESSAGE667].szMessage, szItemName, pDungeon->m_byProductionCount); // "����ǰ: %s (%d)"
		g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_ITEM, szTemp, lstrlen(szTemp));
		wsprintf(szTemp, g_Message[ETC_MESSAGE1179].szMessage, pDungeon->m_byDefenseCount); // "��� ���� Ƚ�� : %d"
		g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_SIEGE_DEFENSECOUNT, szTemp, lstrlen(szTemp));

		if(pDungeon->m_bSiege)
		{// ������.
			nHour = pDungeon->GetRemainTime(pDungeon->m_dwRemainSiegeEndTick, g_dwCurTick, TIME_HOUR);
			nMinute = pDungeon->GetRemainTime(pDungeon->m_dwRemainSiegeEndTick, g_dwCurTick, TIME_MIN);
			nSec = pDungeon->GetRemainTime(pDungeon->m_dwRemainSiegeEndTick, g_dwCurTick, TIME_SEC);
			wsprintf(szTemp, g_Message[ETC_MESSAGE352].szMessage, nHour, nMinute, nSec);	// MSG_ID : 352 ;  %02d�ð� %02d�� %02d���Ŀ� ���� ��
			g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_SIEGE_TIME);
			g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_SIEGE_TIME, szTemp, lstrlen(szTemp));

//			nHour = pDungeon->GetRemainTime(pDungeon->m_dwRemainLevelStartTick, pDungeon->m_dwStopStartTick, TIME_HOUR);
//			nMinute = pDungeon->GetRemainTime(pDungeon->m_dwRemainLevelStartTick, pDungeon->m_dwStopStartTick, TIME_MIN);
//			nSec = pDungeon->GetRemainTime(pDungeon->m_dwRemainLevelStartTick, pDungeon->m_dwStopStartTick, TIME_SEC);
//			wsprintf(szTemp, g_Message[ETC_MESSAGE401].szMessage, nHour, nMinute, nSec);	// MSG_ID : 401 ;  %02d�ð� %02d�� %02d���Ŀ� ������
//			g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_LEVELUP_TIME);
//			g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_LEVELUP_TIME, szTemp, lstrlen(szTemp));
		}
		else
		{// idle
			
			nHour = pDungeon->GetRemainTime(pDungeon->m_dwRemainSiegeStartTick, g_dwCurTick, TIME_HOUR);
			nMinute = pDungeon->GetRemainTime(pDungeon->m_dwRemainSiegeStartTick, g_dwCurTick, TIME_MIN);
			nSec = pDungeon->GetRemainTime(pDungeon->m_dwRemainSiegeStartTick, g_dwCurTick, TIME_SEC);
			wsprintf(szTemp, g_Message[ETC_MESSAGE353].szMessage, nHour, nMinute, nSec);	// MSG_ID : 353 ;  %02d�ð� %02d�� %02d���Ŀ� ���� ����
			g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_SIEGE_TIME);
			g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_SIEGE_TIME, szTemp, lstrlen(szTemp));

//			nHour = pDungeon->GetRemainTime(pDungeon->m_dwRemainLevelStartTick, g_dwCurTick, TIME_HOUR);
//			nMinute = pDungeon->GetRemainTime(pDungeon->m_dwRemainLevelStartTick, g_dwCurTick, TIME_MIN);
//			nSec = pDungeon->GetRemainTime(pDungeon->m_dwRemainLevelStartTick, g_dwCurTick, TIME_SEC);
//			wsprintf(szTemp, g_Message[ETC_MESSAGE401].szMessage, nHour, nMinute, nSec);	// MSG_ID : 401 ;  %02d�ð� %02d�� %02d���Ŀ� ������
//			g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_LEVELUP_TIME);
//			g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_LEVELUP_TIME, szTemp, lstrlen(szTemp));
		}
		
		wsprintf(szTemp, g_Message[ETC_MESSAGE402].szMessage, pDungeon->m_szManager);	// MSG_ID : 402 ; ���� ������ [%s]���� �޸�
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_MANAGER);
		g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_MANAGER, szTemp, lstrlen(szTemp));

		lstrcpy(szTemp, pDungeon->m_szMemo);
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_MEMO);
		g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_MEMO, szTemp, lstrlen(szTemp));		
	}
	else	//���ɴ����� �ƴ� ��� 
	{
		pSpr->v2Scaling.x = 100.0f;
		pSpr->v2Scaling.y = 25.0f;	

		if(pDungeon->m_dwID < 4000 && pDungeon->m_dwID > 3000)	//������ ���� 
		{
			g_pInputManager->SetInputColor(INPUT_BUFFER_WORLD_DUNGEON_TYPE, 0xff0000ff);
			
#if IS_JAPAN_LOCALIZING()
			// �б� ��� ���� ����!!
			if (lstrlen(pDungeon->m_szSchoolName))
			{
				lstrcpy(szTemp, g_Message[ETC_MESSAGE247].szMessage);
				g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_TYPE);
				g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_TYPE, szTemp, lstrlen(szTemp));
			}
			else
			{
				lstrcpy(szTemp, g_Message[ETC_MESSAGE403].szMessage);
				g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_TYPE);
				g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_TYPE, szTemp, lstrlen(szTemp));
				
			}
#else
			lstrcpy(szTemp, g_Message[ETC_MESSAGE403].szMessage);	// MSG_ID : 403 ; [������ �����Դϴ�]
			g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_TYPE);
			g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_TYPE, szTemp, lstrlen(szTemp));
#endif
			
			switch(pDungeon->m_wAreaLevel_h)	
			{
			case 301:	lstrcpy(szTemp, g_Message[ETC_MESSAGE1079].szMessage);	break; // "�ּ� Lv.20 �ִ� Lv.50"
			case 302:	lstrcpy(szTemp, g_Message[ETC_MESSAGE1080].szMessage);	break; // "�ּ� Lv.30 �ִ� Lv.60"
			case 303:	lstrcpy(szTemp, g_Message[ETC_MESSAGE1081].szMessage);	break; // "�ּ� Lv.30 �ִ� Lv.70"
			case 304:	lstrcpy(szTemp, g_Message[ETC_MESSAGE1082].szMessage);	break; // "�ּ� Lv.40 �ִ� Lv.70"
			case 305:	lstrcpy(szTemp, g_Message[ETC_MESSAGE1083].szMessage);	break; // "�ּ� Lv.50 �ִ� Lv.80"
			case 306:	lstrcpy(szTemp, g_Message[ETC_MESSAGE1273].szMessage);	break; // "�ּ� Lv.100 �ִ� Lv.130"
			// sung-han 2005-02-02 ����ʿ��� ���� �������� ���̰� �ϴ� �κ�-----
			case 307:	lstrcpy(szTemp, g_Message[ETC_MESSAGE1315].szMessage);	break; // "�ּ� Lv.140 �ִ� Lv.150"
			case 308:	lstrcpy(szTemp, g_Message[ETC_MESSAGE1316].szMessage);	break; // "�ּ� Lv.150 �ִ� Lv.160"
			case 309:	lstrcpy(szTemp, g_Message[ETC_MESSAGE1317].szMessage);	break; // "�ּ� Lv.160 �ִ� Lv.180"
			//-------------------------------------------------------------------
			case 401:	lstrcpy(szTemp, g_Message[ETC_MESSAGE1084].szMessage);	break; // "�ʺ��� �ּ� Lv.1 �ִ� Lv.35"
			case 402:	lstrcpy(szTemp, g_Message[ETC_MESSAGE1085].szMessage);	break; // "�ʺ��� �ּ� Lv.3 �ִ� Lv.35"
			case 403:	lstrcpy(szTemp, g_Message[ETC_MESSAGE1171].szMessage);	break; // "�ʺ��� �ּ� Lv.1 �ִ� Lv.20"
			case 404:	lstrcpy(szTemp, g_Message[ETC_MESSAGE1169].szMessage);	break; // "�ʺ��� �ּ� Lv.20 �ִ� Lv.50"
			default:	memset(szTemp, 0, sizeof(szTemp));			
			}						
			g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_LEVELTYPE);
			g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_LEVELTYPE, szTemp, lstrlen(szTemp));
		}
		else if(pDungeon->IsPathWay())	//��� ����
		{			
			lstrcpy(szTemp, g_Message[ETC_MESSAGE409].szMessage);	// MSG_ID : 409 ; [��� �����Դϴ�]
			g_pInputManager->SetInputColor(INPUT_BUFFER_WORLD_DUNGEON_TYPE, 0xffff00ff);
			g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_TYPE);
			g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_TYPE, szTemp, lstrlen(szTemp));

			int nValue = pDungeon->m_wAreaLevel_h - 501;
			WORD wLevel = WORD(40 + (10*nValue));		
			wsprintf(szTemp, g_Message[ETC_MESSAGE1077].szMessage, wLevel); // "���� Lv.%d"
						
			g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_LEVELTYPE);
			g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_LEVELTYPE, szTemp, lstrlen(szTemp));
			
		}
		//else if(pDungeon->IsEvent())	//�̺�Ʈ�� ���� 
		else if (DUNGEON_TYPE_EVENT == pDungeon->GetDungeonType())
		{
			if (EVENT_DUNGEON_READY == pDungeon->m_bEventState)
			{
				pSpr->v2Scaling.x = 150.0f;
				pSpr->v2Scaling.y = 45.0f;	
				
				
				DWORD dwTick = timeGetTime();

				int iHour = pDungeon->GetRemainTime(pDungeon->m_dwEndOpenReadyTime, dwTick, TIME_HOUR);
				int iMin  = pDungeon->GetRemainTime(pDungeon->m_dwEndOpenReadyTime, dwTick, TIME_MIN);
				int iSec  = pDungeon->GetRemainTime(pDungeon->m_dwEndOpenReadyTime, dwTick, TIME_SEC);
				wsprintf(szTemp, g_Message[ETC_MESSAGE668].szMessage, iHour, iMin, iSec); // "�̺�Ʈ ���� �������� %02d:%02d:%02d ���ҽ��ϴ�."
				g_pInputManager->SetInputColor(INPUT_BUFFER_WORLD_DUNGEON_TYPE, 0xffffffff);
				g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_TYPE);
				g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_TYPE, szTemp, lstrlen(szTemp));
				
				wsprintf(szTemp, g_Message[ETC_MESSAGE776].szMessage, pDungeon->m_byEventEnterMinLevel); // "���� �ּ� ���� : Lv.%d"
				g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_LEVEL);
				g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_LEVEL, szTemp, lstrlen(szTemp));
				
				wsprintf(szTemp, g_Message[ETC_MESSAGE777].szMessage, pDungeon->m_byEventEnterMaxLevel); // "���� �ִ� ���� : Lv.%d"
				g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_LAYER_COUNT);
				g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_LAYER_COUNT, szTemp, lstrlen(szTemp));

			}
			else
			{			
				pSpr->v2Scaling.y = 45.0f;	
				
				lstrcpy(szTemp, pDungeon->m_szDungeonName);
				g_pInputManager->SetInputColor(INPUT_BUFFER_WORLD_DUNGEON_TYPE, 0xffFB7422);
				g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_TYPE);
				g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_TYPE, szTemp, lstrlen(szTemp));
				
				wsprintf(szTemp, g_Message[ETC_MESSAGE776].szMessage, pDungeon->m_byEventEnterMinLevel); // "���� �ּ� ���� : Lv.%d"
				g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_LEVEL);
				g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_LEVEL, szTemp, lstrlen(szTemp));
				
				wsprintf(szTemp, g_Message[ETC_MESSAGE777].szMessage, pDungeon->m_byEventEnterMaxLevel); // "���� �ִ� ���� : Lv.%d"
				g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_LAYER_COUNT);
				g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_LAYER_COUNT, szTemp, lstrlen(szTemp));
			}
		}
		else	//���� 
		{
			pSpr->v2Scaling.y = 15.0f;	
			lstrcpy(szTemp, g_Message[ETC_MESSAGE410].szMessage);	// MSG_ID : 410 ; [�����Դϴ�]
						
			g_pInputManager->SetInputColor(INPUT_BUFFER_WORLD_DUNGEON_TYPE, 0xff09EA19);
			g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_TYPE);
			g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_TYPE, szTemp, lstrlen(szTemp));
			
			g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_LEVELTYPE);
		}
	}
}

void DungeonEnter(WORD wDungeonID)
{
	LPObjectDesc pDesc = (LPObjectDesc)g_pExecutive->GetData(g_pMainPlayer->m_hPlayer.pHandle);// = OnCrashUser;
	pDesc->CrashFunc = NULL;

	CTWS_TRY_TO_JOIN_DUNGEON packet;
	packet.wDungeonID = wDungeonID;
	g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_WORLD);
	g_pMainPlayer->m_bReceivedConnectMsg = 0;

	g_pSprManager->GetSprite(SPR_LOADING_BACK)->ShowSprite(TRUE);
	g_pGVWorld->dwCrashDungeonTick = g_dwCurTick;		//������ �浹�������� Tick ��� 
	g_pGVWorld->bDisplayDungeonInfo = 0;
	g_pMainPlayer->SetStatus(UNIT_STATUS_AWAIT_DUNGEON_RESPONSE);
	g_pGeometry->DisableFog(0);
}

void OnCrashUser(GXOBJECT_HANDLE handle, LPObjectDesc pData, GXSCHEDULE_PROC_MSG_COLLISION_ARG* pArg)
{
	LPObjectDesc pToData = (LPObjectDesc)g_pExecutive->GetData(pArg->gxHandle);
	
	COkWnd* pOKWnd = COkWnd::GetInstance();

	if(!pToData)
	{
		//������ �ε��� ��� 
		DWORD dwID = g_pExecutive->GetID(pArg->gxHandle);

		DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(dwID);
	
		if(!pDungeon)	
		{
			DisplayMessageAdd(g_Message[ETC_MESSAGE394].szMessage, 0xFFFFC309);	// MSG_ID : 394 ; �ش� ���������� ã�� �� �����ϴ�.
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_Camera.v3CameraPos, FALSE);
			return;
		}	

		if (TRUE == pDungeon->IsConquer() && pDungeon->m_dwOwnerIndex != g_pMainPlayer->m_dwUserIndex)
		{			
			pOKWnd->m_bType = __OKWND_TYPE_POTAL_ENTRANCE;
			pOKWnd->m_dwDungeonID = pDungeon->m_dwID;			
			pOKWnd->SetActive(TRUE);			
			pData->ObjectFunc = NULL;
		}
		else
		{
			pOKWnd->m_bType = __OKWND_TYPE_DUNGEON_ENTER;
			pOKWnd->m_dwDungeonID = pDungeon->m_dwID;			
			pOKWnd->SetActive(TRUE);			
			pData->ObjectFunc = NULL;
		}
		return;
	}
	
	switch(pToData->bType)
	{
	case OBJECT_TYPE_PLANE:
		{		
			HideObject(handle);
			ReleaseBoundingVolume(handle);
			
			CTC_AIRSHIP_RIDING RidingPacket;
			g_pNet->SendMsg((char*)&RidingPacket, RidingPacket.GetPacketSize(), SERVER_INDEX_WORLD );
		
			g_pMainPlayer->SetStatus(UNIT_STATUS_RIDE_AIRPLANE_DOWN);
			pData->ObjectFunc = NULL;

			//��������� Light Attach
			if(!g_pExecutive->GXODetachLight(g_pMainPlayer->m_hPlayer.pHandle, VAR->hMainSpotWorld))
				asm_int3();

			VECTOR3		vAirShipPos;
			VECTOR3		v3Offset;
			LIGHT_DESC	shadow;
			
			g_pExecutive->GXOGetPosition(g_pMainPlayer->m_RideObject.pHandle, &vAirShipPos);
			g_pExecutive->GXLGetLightDesc(VAR->hMainSpotWorld,&shadow);
						
			v3Offset.x = 0.0f;
			v3Offset.y = 800.0f;
			v3Offset.z = 0.0f;
	
			g_pExecutive->GXLSetLightDesc(VAR->hMainSpotWorld,&shadow);
			
			if(!g_pExecutive->GXOAttachLight(g_pMainPlayer->m_RideObject.pHandle,"ship_obj", VAR->hMainSpotWorld, &v3Offset, ATTACH_TYPE_LINK))
				asm_int3();

			g_pExecutive->GXOEnableHFieldApply(g_pMainPlayer->m_RideObject.pHandle);

			g_pInputManager->ClearInput(INPUT_BUFFER_7);
			g_pInputManager->ClearInput(INPUT_BUFFER_8);
			g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_SIEGE_TIME);
			g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_LEVELUP_TIME);
			g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_MANAGER);
			g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_MEMO);
			g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_TYPE);
			g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_LEVELTYPE);			

			OutputDebugString("Attach!\n");	// MSG_ID :	395 ; Attach!\n
		}
		break;
	}
}