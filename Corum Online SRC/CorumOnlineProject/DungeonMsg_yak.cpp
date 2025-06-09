#include "DungeonMsg_yak.h"
#include "InitGame.h"
#include "GameControl.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "message.h"
#include "CodeFun.h"
#include "CorumResource.h"
#include "DungeonMsg.h"
#include "EffectProcess.h"
#include "DungeonTable.h"
#include "UserInterface.h"
#include "GameMenuWnd.h"
#include "ItemWnd.h"
#include "DSMiniMap.h"
#include "User.h"
#include "Monster.h"
#include "ObjectManager.h"
#include "Map.h"
#include "ChrInfo.h"
#include "ItemMove.h"
#include "Interface.h"
#include "SkillWnd.h"
#include "../CommonServer/CommonClientDungeon.h"


void InitPacketProcDungeonYak()
{
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_MOVE]					= CmdMoveDungeon;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_STOP]					= CmdStopDungeon;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_APPEAR]					= CmdAppearDungeon;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_DISAPPEAR]				= CmdDisappearDungeon;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_ATTACK_USER_MON]		= CmdAttack_User_Mon;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_ATTACK_USER_USER]		= CmdAttack_User_User;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_ATTACK_MON_MON]			= CmdAttack_Mon_Mon;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_SKILL]							= CmdSkill;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_APEAR_SKILL]					= CmdApearSkill;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ATTACK_MODE]					= CmdAttackMode;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_BADACTION]						= CmdBadAction;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DESTROY_CP]						= CmdDestroyCP; 
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_CASTING]				= CmdSkillCasting; 
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_STOPSTATUSEFFECT]		= CmdSkillStopStatusEffect; 
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_CHANGE_LRSKILLKIND]		= CmdSkill_ChangeLR; 
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_SIEGEINFO]				= CmdSiegeInfo; 	
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_CASTING_FAIL]			= CmdSkillCastingFail;	
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_GUARDIAN_STATUS]		= CmdGuardianStatus;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_USER_SKILL_LEVEL]		= CmdUserSkillLevel;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_USER_STATUS]			= CmdUserStatus;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_GUARDIAN_LEVELUP]		= CmdGuardianLevelUP;	
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_SKILL_RESULT]					= CmdSkillResult;		
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PKCOUNT]						= CmdPKCount;				
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_FINISH_MAPLOADING]				= CmdFinishMapLoading;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_RESPONSE_JOIN_USER]				= CmdResponsePotalJoinUser;			
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PKMODE]							= CmdPKMode;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PLAYER_BAD]						= CmdPlayerBadMode;	
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_USED_POTION]			= CmdUsedPotion;			
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ACCENTRANCE_ACQUISITION]		= CmdAccEntranceAcquisition;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ORIGINATIONITEMWITHEFFECTKIND]	= CmdOriginationItemWithEffectKind;		
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_APPEAR_MON]				= CmdAppearMonster;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_DISAPPEAR_MON]			= CmdDisAppearMonster;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_KILL_MON]				= CmdKillMonster;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_MOVE_MON]				= CmdMoveMonster;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_STOP_MON]				= CmdStopMonster;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_ATTACK_MON_USER]		= CmdAttack_Mon_User;	
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_CONGESTION_MODE]				= CmdMonsterCongestionMode;		
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_GURADIAN_EGG_SUMMON_STATUS]		= CmdGuardianEggSummonStatus;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_UPDATE_GUARDIAN_ITEM]			= CmdUpdateGuardianItem;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_APPENDIX_GUARDIAN_INFO]	= CmdAppendixGuardianInfo;
}


void CmdAppendixGuardianInfo(char* pMsg, DWORD dwLen)
{
	DSTC_APPENDIX_GUARDIAN_INFO* pPacket = (DSTC_APPENDIX_GUARDIAN_INFO*)pMsg;
	CMonster*	pMonster = g_pMonsterHash->GetData( pPacket->dwMonsterIndex );
	
	if (!pMonster)
		return;

	pMonster->m_wEgo = pPacket->wEgo;
	pMonster->m_wStr = pPacket->wStr;
	pMonster->m_wDex = pPacket->wDex;
	pMonster->m_wInt = pPacket->wInt;
	pMonster->m_wVit = pPacket->wVit;
	pMonster->m_wStatPoint = pPacket->wStatPoint;
	pMonster->m_dwExp = pPacket->dwExp;
	memcpy(pMonster->m_pwAttackDamage_L, pPacket->pwAttackDamage_L, sizeof(WORD)*2);
	pMonster->m_wAvoid = pPacket->wAvoid;
	pMonster->m_wAttackAcuracy = pPacket->wAttackAcuracy;
	pMonster->m_wFireResist = pPacket->wFireResist;
	pMonster->m_wIceResist = pPacket->wIceResist;
	pMonster->m_wLightResist = pPacket->wLightResist;
	pMonster->m_wPhyResist = pPacket->wPhyResist;
	pMonster->m_wPoiResist = pPacket->wPoiResist;
	pMonster->m_dwMaxHP = pPacket->dwMaxHP;

	for(int i = 0; i < MAX_GUARDIAN_USE_SKILL; ++i)
	{
		pMonster->m_Skill[i].wSkill = pPacket->pSkill[i].bKind;
		pMonster->m_Skill[i].bSkillLevel = pPacket->pSkill[i].bLevel;
	}

	// 왼쪽 상단의 가디언 창을 업데이트 : 최덕석 2005.2.16
	CUserInterface* pUserInterface = CUserInterface::GetInstance();
	pUserInterface->SetGauge(pMonster->m_dwHP, pMonster->m_dwMaxHP);				
}

void CmdUpdateGuardianItem( char* pMsg, DWORD dwLen )
{
	DSTC_UPDATE_GUARDIAN_ITEM* pUpdate = (DSTC_UPDATE_GUARDIAN_ITEM*)pMsg;

	ITEM_NATIVE_INFO ItemNativeInfo;
	ItemNativeInfo.eSlotID		= ITEM_NATIVE_INV_GUARDIAN;
	ItemNativeInfo.bySlotIndex	= pUpdate->bySlotIndex;
	
	const CItem* pItemNative = GetItemNative(&ItemNativeInfo);

	CItem cItem;
	memcpy(&cItem, pItemNative, sizeof(CItem));

	if( !memcmp(cItem.GetSerial(), pUpdate->cItem.GetSerial(), sizeof(ITEM_SERIAL)) )
	{
		memcpy(&cItem, &pUpdate->cItem, sizeof(CItem));		
		CBaseItem* lpItemTable = g_pItemTableHash_get()->GetData(cItem.m_wItemID);
		CUserInterface* pUserInterface = CUserInterface::GetInstance();
		pUserInterface->SetGauge(cItem.m_Item_Guardian.dwCompleteTime, lpItemTable->BaseItem_Guardian.wCompleteTime);				
	}
}

void CmdGuardianLevelUP( char* pMsg, DWORD dwLen )
{
	DSTC_GUARDIAN_LEVELUP*	pLevelUP = ( DSTC_GUARDIAN_LEVELUP* )pMsg;

	CMonster*	pMonster = g_pMonsterHash->GetData( pLevelUP->dwGuardianIndex );
	if( !pMonster ) 
	{
		RequestDungeonInfo( 2, pLevelUP->dwGuardianIndex );
		return;
	}

	// 레벨업 사운드 출력
	_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_LEVELUP, g_v3InterfaceSoundPos, FALSE);

	// 레벨업 이펙트를 뿌림
	EffectDesc*	pEffectDesc =  g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(SKILL_LEVELUP), TRUE, __CHR_EFFECT_NONE__);
	
	pEffectDesc->byTargetObjectType[0]	= OBJECT_TYPE_MONSTER; 
	pEffectDesc->dwTargetIndex[0]		= pMonster->m_dwMonsterIndex;
	
	GXSetPosition( pEffectDesc->hEffect.pHandle, &pMonster->m_v3CurPos, FALSE );
	::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE );
	pEffectDesc->hEffect.pDesc->ObjectFunc	= EffectOnceAndNormal;
	
	// 가디언 스케일을 레벨에 맞게 조정
	pMonster->ModelScaleByLevel();
	
	// 자기 소유의 가디언이라면
	if(g_pMainPlayer->m_pGuardian[0]
	&& pMonster->m_dwMonsterIndex == g_pMainPlayer->m_pGuardian[0]->m_dwMonsterIndex)
	{
		// 가디언 레벨을 동기화
		pMonster->m_dwLevel = GetGuardianItemLevel(pMonster->m_dwExp);

		// 가디언 HP 게이지를 만땅 시킨다
		CUserInterface* pUserInterface = CUserInterface::GetInstance();
		pUserInterface->SetGauge(g_pMainPlayer->m_pGuardian[0]->m_dwMaxHP, g_pMainPlayer->m_pGuardian[0]->m_dwMaxHP);

		// "가디언이 레벨업하였습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE1408].szMessage, 0xFFFFC309);
	
		{	// "가디언의 수명이 %d시간 연장되었습니다."
			char szTemp[255];
			
			CItem* pGuardianItem = &g_pMainPlayer->m_pInv_Guardian[0];
			wsprintf(szTemp, 
				g_Message[ETC_MESSAGE1409].szMessage,
				pGuardianItem->GetBaseItem()->BaseItem_Guardian.wLvLifeUp / 60);
			DisplayMessageAdd(szTemp, 0xFFFFC309);
		}
	}
}


void CmdMoveDungeon( char* pMsg, DWORD dwLen )
{
	CInterface* pInterface = CInterface::GetInstance();		
	
	if(pInterface->GetClick())
		return;

	DSTC_MOVE*	pMove = (DSTC_MOVE*)pMsg;

	if( g_pMainPlayer->m_dwUserIndex == pMove->dwIndex )		// 나 자신에게 나자신의 이동 메시지가 왔다면
		return;

	CUser*	pUser = g_pUserHash->GetData( pMove->dwIndex );
	if( !pUser ) 
	{
		RequestDungeonInfo( 1, pMove->dwIndex );
		return;
	}

	int nFrame = 0;	// 걷거나 달리고 있는 상태 였다면 프래임을 부드럽게 이여주기 위해...
	if( pUser->GetStatus() == UNIT_STATUS_WALKING 
		|| pUser->GetStatus() == UNIT_STATUS_RUNNING ) 
		nFrame = g_pExecutive->GXOGetCurrentFrame( pUser->m_hPlayer.pHandle );

	switch( pMove->bMoveType )
	{
	case UNIT_STATUS_WALKING:
		pUser->SetAction( MOTION_TYPE_WALK, nFrame, ACTION_LOOP );
		break;

	case UNIT_STATUS_RUNNING:
		pUser->SetAction( MOTION_TYPE_RUN, nFrame, ACTION_LOOP );
		// 달리기이면 이펙트도 같이 ...
		pUser->ShowMoveStartEffect();
		break;
	}

	pUser->m_fMoveSpeed					= pMove->fSpeed;
	pUser->m_hPlayer.pDesc->ObjectFunc	= PlayerMoveFunc;
	pUser->m_hPlayer.pDesc->dwStartTick	= g_dwCurTick;
	
	VECTOR3 v3MoveStartPos;
	VECTOR2_TO_VECTOR3(pMove->v2MoveStartPos, v3MoveStartPos)
	
	SetUserPosition( pUser, &v3MoveStartPos );
	
	// 이동 정보를 입력하고
	pUser->SetStatus(pMove->bMoveType);

	VECTOR2_TO_VECTOR3(pMove->v2MoveDirection, pUser->m_v3Direction)
	pUser->m_pDestTile		= g_pMap->GetMap( pMove->wDestX, pMove->wDestZ );
	pUser->m_pStartTile		= pUser->m_pCurTile;
	
	g_pExecutive->GXOSetDirection( pUser->m_hPlayer.pHandle, 
		&g_Camera.v3AxsiY, 
		(float)(atan2(pUser->m_v3Direction.z, pUser->m_v3Direction.x) + DEG90 ) );
	GXSetPosition(pUser->m_hShadowHandle, &pUser->m_v3CurPos, FALSE, TRUE);	
	
}

void CmdStopDungeon( char* pMsg, DWORD dwLen )
{
	DSTC_STOP*	pStop = (DSTC_STOP*)pMsg;

	CUser*	pUser = g_pUserHash->GetData( pStop->dwUserIndex );

	if( !pUser ) 
	{
		RequestDungeonInfo( 1, pStop->dwUserIndex );
		return;
	}
	
	if(pUser->GetStatus() == UNIT_STATUS_PORTAL_MOVING)
		return;
	
	VECTOR3 v3StopPos;
	VECTOR2_TO_VECTOR3(pStop->v2StopPos , v3StopPos)
	SetUserPosition( pUser, &v3StopPos );
	
	// Player Shop 상태와, Rest 상태는 동작 재설정을 하지 않는다. added by minjin. 2004. 11. 4.
	if ( pUser->GetStatus() == UNIT_STATUS_PLAYER_SHOP )		return;
	if ( pUser->GetStatus() == UNIT_STATUS_PLAYER_REST )		return;

	if (g_dwLayerID < 100 )	// 던전은 아니므로
		pUser->SetAction( MOTION_TYPE_VILLAGESTAND, 0, ACTION_LOOP );
	else
		pUser->SetAction( MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );
	
	if( pUser->GetStatus() != UNIT_STATUS_DEAD )
	{
		pUser->SetStatus(UNIT_STATUS_NORMAL);
		pUser->m_hPlayer.pDesc->ObjectFunc	= NULL;
	}

	// 달리다기 정지 했다면 이펙트를 뿌려준다.
	pUser->ShowMoveStopEffect();	
}

void CmdAppearDungeon( char* pMsg, DWORD dwLen )
{
	if(!g_pThisDungeon)	return;
	DSTC_APPEAR*	pAppear = (DSTC_APPEAR*)pMsg;

	if(	g_pThisDungeon->IsStadium() //경기장인데
	&&	!(pAppear->dwGuildWarFlag & G_W_F_PLAYER) )//선수가 아니면
	{
		return;
	}

	CUser* pUser = (CUser*)g_pUserHash->GetData( pAppear->dwUserIndex );
	if( !pUser )
	{
		pUser = InitPlayer( pAppear );
		LOGOUT( "Appear User %d", pAppear->dwUserIndex );
				
		if(pUser->m_dwUserIndex==g_pMainPlayer->m_dwUserIndex)
			return;

		pUser->m_byUpgrade = pAppear->byUpgrade;

		if(pAppear->byUpgrade==0)
		{
			SetEffectIndexAll(pUser, 0xffffffff);
		}
		else
		{
			SetEffectIndexAll(pUser, pAppear->byUpgrade-6+1);
		}
	}
	else
	{
		LOGOUT( "same Appear User %d", pAppear->dwUserIndex );
	}
}

void CmdDisappearDungeon( char* pMsg, DWORD dwLen )
{
	DSTC_DISAPPEAR*	pDisAppear	= (DSTC_DISAPPEAR*)pMsg;
	CUser*	pUser = g_pUserHash->GetData( pDisAppear->dwUserIndex );
	if( !pUser ) return;
	if( pUser == g_pMainPlayer ) return;

	RemovePlayer( pUser );

	LOGOUT( "Disappear User %d", pDisAppear->dwUserIndex );
}

void CmdAttack_User_Mon( char* pMsg, DWORD dwLen )
{
	DSTC_ATTACK_USER_MON*	pAttackUserMon = (DSTC_ATTACK_USER_MON*)pMsg;
	
	// 먼저 나의 공격 에러 검사를 한다.
	if( g_pMainPlayer->m_dwUserIndex == pAttackUserMon->dwUserIndex )
	{
		if( pAttackUserMon->bType == 5 )	// Error
		{
			g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
			return;
		}
	}

	CMonster*	pMonster= g_pMonsterHash->GetData( pAttackUserMon->dwMonsterIndex );
	if( !pMonster ) 
	{
		RequestDungeonInfo( 2, pAttackUserMon->dwMonsterIndex );
		if( g_pMainPlayer->m_dwUserIndex == pAttackUserMon->dwUserIndex && pAttackUserMon->bType == 5 )	// Error
		{
			g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
		}
		return;
	}
	
	// 나의 공격.
	if( g_pMainPlayer->m_dwUserIndex == pAttackUserMon->dwUserIndex )
	{
		if( pAttackUserMon->bType == 5 )	// Error
		{
			g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
			return;
		}
		
		g_pMainPlayer->m_dwTemp[ USER_TEMP_TARGET_TYPE ]	= OBJECT_TYPE_MONSTER;
		g_pMainPlayer->m_dwTemp[ USER_TEMP_TARGET_INDEX ]	= pMonster->m_dwMonsterIndex;

		VECTOR3	v3DirMon;
		VECTOR3_SUB_VECTOR3( &v3DirMon, &pMonster->m_v3CurPos, &g_pMainPlayer->m_v3CurPos );
		g_pExecutive->GXOSetDirection( g_pMainPlayer->m_hPlayer.pHandle, &g_Camera.v3AxsiY, (float)(atan2(v3DirMon.z, v3DirMon.x) + DEG90 ) );		
		
		g_pMainPlayer->m_wAttackType = (WORD)( MOTION_TYPE_ATTACK1_1 + ( pAttackUserMon->bAttackType * 3 + pAttackUserMon->bAttackFrame ) );

		if (MOTION_TYPE_ATTACK3_3 < g_pMainPlayer->m_wAttackType)	asm_int3();

		g_pMainPlayer->SetAction( g_pMainPlayer->m_wAttackType, 0, ACTION_ONCE );
		g_pMainPlayer->SetStatus(UNIT_STATUS_ATTACKING);
		g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc	= PlayerAttackFunc;
				
		//몬스터 상태 저장.
		pMonster->m_dwTemp[MONSTER_TEMP_DAMAGE_TYPE]	= pAttackUserMon->bType;
		pMonster->m_dwTemp[MONSTER_TEMP_DAMAGE]			= pAttackUserMon->dwDamage;
		pMonster->m_dwHP								=  pAttackUserMon->dwCurMonsterHP;						
		
		// 나의 공격이라면 오토 타겟팅을 만들어준다.
		g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_TYPE ]	= OBJECT_TYPE_MONSTER;
		g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_INDEX ]	= pMonster->m_dwMonsterIndex; 

		if( pMonster->m_dwHP == 0 )
		{
			pMonster->InitDie(g_pMainPlayer, TRUE, min(pAttackUserMon->dwDamage/10, 50)+GetRandom(20)
				, g_pChrInfoUser->GetFrameInfo(g_pMainPlayer->m_wClass-1, g_pMainPlayer->m_byItemType, g_pMainPlayer->m_wAttackType, _CHRINFO_FRAME1) );							
		}
		else
			CUserInterface::GetInstance()->OpenMonsterBar(pMonster->m_szName, pMonster->m_dwHP, pMonster->m_dwMaxHP);
	}
	else	// 다른 유저의 공격.
	{
		CUser* pUser = g_pUserHash->GetData( pAttackUserMon->dwUserIndex );
		if( !pUser ) 
		{
			RequestDungeonInfo( 1, pAttackUserMon->dwUserIndex );
			return;
		}
 
		pUser->m_dwTemp[ USER_TEMP_TARGET_TYPE ]	= OBJECT_TYPE_MONSTER;
		pUser->m_dwTemp[ USER_TEMP_TARGET_INDEX ]	= pMonster->m_dwMonsterIndex;

		VECTOR3		v3DirMon;
		VECTOR2_TO_VECTOR3(pAttackUserMon->v2UserPos, v3DirMon)
		SetUserPosition( pUser, &v3DirMon );
		VECTOR3_SUB_VECTOR3( &v3DirMon, &pMonster->m_v3CurPos, &pUser->m_v3CurPos );
		g_pExecutive->GXOSetDirection( pUser->m_hPlayer.pHandle, &g_Camera.v3AxsiY, (float)(atan2(v3DirMon.z, v3DirMon.x) + DEG90 ) );		
			
		pUser->m_wAttackType	= (WORD)( MOTION_TYPE_ATTACK1_1 + ( pAttackUserMon->bAttackType * 3 + pAttackUserMon->bAttackFrame ) );
		pUser->SetAction( pUser->m_wAttackType, 0, ACTION_ONCE );
		pUser->SetStatus( UNIT_STATUS_ATTACKING );
		pUser->m_hPlayer.pDesc->ObjectFunc	= PlayerAttackFunc;
		
		int nDamage = pAttackUserMon->dwDamage;

		//몬스터 상태 저장.
		// sung-han 2005-01-18 pvp시 엘레멘탈 대미지 0나오는거 수정
		//-------------------------------------------------------------------------------
		pMonster->m_dwTemp[MONSTER_TEMP_DAMAGE_TYPE]	= pAttackUserMon->bType;
		pMonster->m_dwTemp[MONSTER_TEMP_DAMAGE]			= pAttackUserMon->dwDamage;
		//-------------------------------------------------------------------------------
		pMonster->m_dwHP =  pAttackUserMon->dwCurMonsterHP;
		
		if( pMonster->m_dwHP == 0 )
		{
			pMonster->InitDie(pUser, FALSE, min(nDamage/10, 50)+GetRandom(20)
				, g_pChrInfoUser->GetFrameInfo(pUser->m_wClass-1, pUser->m_byItemType, pUser->m_wAttackType, _CHRINFO_FRAME0) );
		}
	}
}

void CmdAttack_User_User( char* pMsg, DWORD dwLen )
{

	DSTC_ATTACK_USER_USER*	pAttackUserUser = (DSTC_ATTACK_USER_USER*)pMsg;

	CUser* pOffense, *pDefense;

	pOffense = g_pUserHash->GetData( pAttackUserUser->dwOffenseUserIndex );
	if( !pOffense ) 
	{
		RequestDungeonInfo( 1, pAttackUserUser->dwOffenseUserIndex );
		return;
	}		
	
	// 나의 공격이라면 에러 검사를 한다.
	if( pOffense == g_pMainPlayer )	// 공격자가 나라면.
	{
		if( pAttackUserUser->bType == 5 && g_pMainPlayer->GetStatus() != UNIT_STATUS_DEAD )	// Error
		{
			g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
			return;
		}
	}

	pDefense = g_pUserHash->GetData( pAttackUserUser->dwDefenseUserIndex );
	if( !pDefense ) 
	{
		RequestDungeonInfo( 1, pAttackUserUser->dwDefenseUserIndex );
		if( pOffense == g_pMainPlayer && pAttackUserUser->bType == 5 )	// Error
		{
			g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
		}
		return;
	}

	if( pOffense == g_pMainPlayer )	// 공격자가 나라면.
	{
		if( pAttackUserUser->bType == 5 && g_pMainPlayer->GetStatus() != UNIT_STATUS_DEAD )	// Error
		{
			g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
			return;
		}

		if( g_pMainPlayer->GetStatus() != UNIT_STATUS_DEAD )
		{
			VECTOR3	v3DirMon;
			VECTOR3_SUB_VECTOR3( &v3DirMon, &pDefense->m_v3CurPos, &g_pMainPlayer->m_v3CurPos );
			g_pExecutive->GXOSetDirection( g_pMainPlayer->m_hPlayer.pHandle, &g_Camera.v3AxsiY, (float)(atan2(v3DirMon.z, v3DirMon.x) + DEG90 ) );	
			
			g_pMainPlayer->m_dwTemp[ USER_TEMP_TARGET_TYPE ]	= OBJECT_TYPE_PLAYER;
			g_pMainPlayer->m_dwTemp[ USER_TEMP_TARGET_INDEX ]	= pDefense->m_dwUserIndex;

			g_pMainPlayer->m_wAttackType	= (WORD)( MOTION_TYPE_ATTACK1_1 + ( pAttackUserUser->bAttackType * 3 + pAttackUserUser->bAttackFrame ) );
			g_pMainPlayer->SetAction( g_pMainPlayer->m_wAttackType, 0, ACTION_ONCE );
			g_pMainPlayer->SetStatus(UNIT_STATUS_ATTACKING);
			g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc	= PlayerAttackFunc;	
		}

		// 나의 공격이라면 오토 타겟팅을 만들어준다.
		g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_TYPE ]	= OBJECT_TYPE_PLAYER;
		g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_INDEX ]	= pDefense->m_dwUserIndex;
	}
	else							// 공격자가 다른 유저이다.
	{ 
		pOffense->m_dwTemp[ USER_TEMP_TARGET_TYPE ]		= OBJECT_TYPE_PLAYER;
		pOffense->m_dwTemp[ USER_TEMP_TARGET_INDEX ]	= pDefense->m_dwUserIndex;

		VECTOR3		v3DirMon;
		VECTOR2_TO_VECTOR3(pAttackUserUser->v2OffenseUserPos, v3DirMon)
		SetUserPosition( pOffense, &v3DirMon );
		VECTOR3_SUB_VECTOR3( &v3DirMon, &pDefense->m_v3CurPos, &pOffense->m_v3CurPos );
		g_pExecutive->GXOSetDirection( pOffense->m_hPlayer.pHandle, &g_Camera.v3AxsiY, (float)(atan2(v3DirMon.z, v3DirMon.x) + DEG90 ) );
		
		pOffense->m_wAttackType	= (WORD)( MOTION_TYPE_ATTACK1_1 + ( pAttackUserUser->bAttackType * 3 + pAttackUserUser->bAttackFrame ) );
		pOffense->SetAction( pOffense->m_wAttackType, 0, ACTION_ONCE );
		pOffense->SetStatus(UNIT_STATUS_ATTACKING);
		pOffense->m_hPlayer.pDesc->ObjectFunc	= PlayerAttackFunc;
	}

	if( pDefense == g_pMainPlayer ) // 방어자가 나라면.
	{
		g_pMainPlayer->m_dwTemp[USER_TEMP_DAMAGE_TYPE] = pAttackUserUser->bType;
		g_pMainPlayer->m_dwTemp[USER_TEMP_DAMAGE] = pAttackUserUser->dwDamage;		

		char szTemp[0xff] = {0,};
		wsprintf(szTemp, g_Message[ETC_MESSAGE504].szMessage, pOffense->m_szName); // "%s로부터 공격을 받았습니다."
		DisplayMessageAdd(szTemp, 0xffFF0000, CGameMenuWnd::GetInstance()->m_bSystemMsgFlag);
	}
	else							// 방어자가 다른 유저이다.
	{
		pDefense->m_dwTemp[USER_TEMP_DAMAGE_TYPE] = pAttackUserUser->bType;
		pDefense->m_dwTemp[USER_TEMP_DAMAGE] = pAttackUserUser->dwDamage;
	}

	if( pAttackUserUser->bType == 3 || pAttackUserUser->bType == 4 )
	{
		if( pAttackUserUser->bStatusKind == USER_HP )
		{
			if (pDefense == g_pMainPlayer)
				CUserInterface::GetInstance()->SetDengeonHp(WORD(pAttackUserUser->dwCurDefenseUserHP));

			if (pAttackUserUser->dwCurDefenseUserHP == 0  )
			{
				_PlaySound( CHARACTER_SOUND_DEAD, SOUND_TYPE_CHARACTER, CHARACTER_SOUND_DEAD + ( pDefense->m_wClass - 1 ) * SOUND_PER_CHARACTER
					, pDefense->m_v3CurPos, FALSE );

				pDefense->SetStatus(UNIT_STATUS_DEAD);	

				if (g_pThisDungeon->m_bSiege)
					pOffense->SetSiegePKCount(pOffense->m_wSiegePKCount+1);

				char szTemp[0xff] = {0,};
				// "%s님이 %s님을 죽였습니다."
				wsprintf(szTemp, g_Message[ETC_MESSAGE505].szMessage, pOffense->m_szName, pDefense->m_szName); 
				DisplayMessageAdd(szTemp, 0xffFF0000);
				
			}
		}
		else if( pAttackUserUser->bStatusKind == USER_MP )
		{
			if (pDefense == g_pMainPlayer)
			{
				CUserInterface::GetInstance()->SetDengeonHp(WORD(pAttackUserUser->dwCurDefenseUserHP>>16));
				CUserInterface::GetInstance()->SetDengeonMp(WORD(pAttackUserUser->dwCurDefenseUserHP&0xffff));
			}
			
			if (pAttackUserUser->dwCurDefenseUserHP>>16 == 0  )
			{
				
				_PlaySound( CHARACTER_SOUND_DEAD, SOUND_TYPE_CHARACTER, CHARACTER_SOUND_DEAD + ( pDefense->m_wClass - 1 ) * SOUND_PER_CHARACTER
					, pDefense->m_v3CurPos, FALSE );

				pDefense->SetStatus(UNIT_STATUS_DEAD);	

				if (g_pThisDungeon->m_bSiege)
					pOffense->SetSiegePKCount(pOffense->m_wSiegePKCount+1);
				
				char szTemp[0xff] = {0,};
				// "%s님이 %s님을 죽였습니다."
				wsprintf(szTemp, g_Message[ETC_MESSAGE505].szMessage, pOffense->m_szName, pDefense->m_szName); 
				DisplayMessageAdd(szTemp, 0xffFF0000);				
			}
		}		
	}
}

void CmdAttack_Mon_Mon( char* pMsg, DWORD dwLen )
{
	DSTC_ATTACK_MON_MON*	pAttackMonMon = (DSTC_ATTACK_MON_MON*)pMsg;

	CMonster*	pOffenseMon, *pDefenseMon;

	pOffenseMon = g_pMonsterHash->GetData( pAttackMonMon->dwOffenseMonIndex );
	pDefenseMon = g_pMonsterHash->GetData( pAttackMonMon->dwDefenseMonIndex );

	if( !pOffenseMon ) 
	{
		RequestDungeonInfo( 2, pAttackMonMon->dwOffenseMonIndex );
		return;
	}

	if( !pDefenseMon )
	{
		RequestDungeonInfo( 2, pAttackMonMon->dwDefenseMonIndex );
		return;
	}

	VECTOR3 v3MonPos;
	VECTOR2_TO_VECTOR3(pAttackMonMon->v2OffenseMonPos, v3MonPos)

	// 위치교정.
	SetMonsterPosition( pOffenseMon, &v3MonPos );
	VECTOR2_TO_VECTOR3(pAttackMonMon->v2DefenseMonPos, v3MonPos)
	SetMonsterPosition( pDefenseMon, &v3MonPos );

	VECTOR3	v3DirMon;
	VECTOR3_SUB_VECTOR3( &v3DirMon, &pDefenseMon->m_v3CurPos, &pOffenseMon->m_v3CurPos );
	g_pExecutive->GXOSetDirection( pOffenseMon->m_hMonster.pHandle, &g_Camera.v3AxsiY, (float)(atan2(v3DirMon.z, v3DirMon.x) + DEG90 ) );

	pOffenseMon->SetActionNext( MON_MOTION_TYPE_ATTACK1, MON_MOTION_TYPE_STAND1, ACTION_ONCE, 0 );
	pOffenseMon->SetStatus(UNIT_STATUS_ATTACKING, MonsterAttackFunc);
		
	// 유저를 세팅한다.
	pOffenseMon->m_dwTemp[ MONSTER_TEMP_TARGET_TYPE ]		= OBJECT_TYPE_MONSTER;
	pOffenseMon->m_dwTemp[ MONSTER_TEMP_TARGET_INDEX ]		= pDefenseMon->m_dwMonsterIndex;
		
	pOffenseMon->m_dwTemp[MONSTER_TEMP_MONMON_DAMAGE_TYPE]	= pAttackMonMon->bType;
	pOffenseMon->m_dwTemp[MONSTER_TEMP_MONMON_DAMAGE]		= pAttackMonMon->dwDamage;
	pDefenseMon->m_dwHP										= pAttackMonMon->dwCurDefenseMonHP;

	if( pDefenseMon->m_dwMonsterKind == OBJECT_TYPE_GUARDIAN )
	{
		CUserInterface* pUserInterface = CUserInterface::GetInstance();
		if( g_pMainPlayer->m_pGuardian[0] )
		{
			if( g_pMainPlayer->m_pGuardian[0]->m_dwMonsterIndex == pDefenseMon->m_dwMonsterIndex )
			{
				pUserInterface->SetGauge(g_pMainPlayer->m_pGuardian[0]->m_dwHP, g_pMainPlayer->m_pGuardian[0]->m_dwMaxHP);
			}
		}
	}
 
	if( pDefenseMon->m_dwHP == 0 )
	{
		CUser* pKiller = g_pUserHash->GetData(pOffenseMon->m_dwLordIndex);
		pDefenseMon->InitDie(pKiller, FALSE, min(pAttackMonMon->dwDamage/10, 50)+GetRandom(20)
			, g_pChrInfoMonster->GetFrameInfo(pOffenseMon->m_wModNo, 0, MON_MOTION_TYPE_ATTACK1, _CHRINFO_FRAME0) );		
	}
}

void CmdGuardianEggSummonStatus( char* pMsg, DWORD dwLen )
{
	DSTC_GURADIAN_EGG_SUMMON_STATUS* pPacket = (DSTC_GURADIAN_EGG_SUMMON_STATUS*)pMsg;

	if(pPacket->byGuardianEggSummonStatus)
	{
		CItem* pItem = &g_pMainPlayer->m_pInv_Guardian[pPacket->byZipCode];

		CBaseItem* pItemInfo = g_pItemTableHash_get()->GetData(pItem->GetID());

		CUserInterface::GetInstance()->OpenGuardianDescriptionWnd(
			pItem->GetSerial()->i64Serial
			, pItemInfo->szItemName_Kor
			, pItem->m_Item_Guardian.dwCompleteTime
			, pItemInfo->BaseItem_Guardian.wCompleteTime);
	}
	else
	{
		CUserInterface::GetInstance()->CloseGuardianDescriptionWnd();
	}
}

//특정몬스터의 폭주모드가 On되거나 Off 된다는 패킷 
void CmdMonsterCongestionMode( char* pMsg, DWORD dwLen )
{
	DSTC_CONGESTION_MODE* pPacket = (DSTC_CONGESTION_MODE*)pMsg;
	char szBuf[0xff] = {0,};
#ifdef DEVELOP_MODE
	if(pPacket->bOn)
		wsprintf(szBuf, g_Message[ETC_MESSAGE1008].szMessage, pPacket->dwMonsterID); // "폭주모드 On: MonsterID:%d"
	else
		wsprintf(szBuf, g_Message[ETC_MESSAGE1009].szMessage, pPacket->dwMonsterID); // "폭주모드 Off: MonsterID:%d"

	DisplayMessageAdd(szBuf, 0xff00ffff);
#endif

	// 폭주모드 적용 : 최덕석 2005.2.14
	CMonster*	pMonster = g_pMonsterHash->GetData( pPacket->dwMonsterID );
	pMonster->m_bCongestionMode = pPacket->bOn;

/*	if(pPacket->bOn)
		wsprintf(szBuf, "Krrrrrrrrrrrrrrr!!!", pPacket->dwMonsterID);
	else
		wsprintf(szBuf, "Huuuuuuuuuuuuuuu...", pPacket->dwMonsterID);
	
	DisplayMessageAdd(szBuf, 0xff00ffff);
*/
}

void CmdAppearMonster( char* pMsg, DWORD dwLen )
{
	DSTC_APPEAR_MON* pAppearMon = (DSTC_APPEAR_MON*)pMsg;

	if(GetGameStatus()!=UPDATE_GAME_PLAY)
		return;
	
	if( !g_pMonsterHash->bSameKey( pAppearMon->dwMonsterIndex ) )
	{
		InitMonster( pAppearMon );
	}
	else
	{
#ifdef DEVELOP_MODE
		LOGOUT( g_Message[ETC_MESSAGE71].szMessage, pAppearMon->dwMonsterIndex );	// MSG_ID : 71 ; AppearMonster Error! MonsterID : %d
#endif
		CMonster*	pMonster = g_pMonsterHash->GetData( pAppearMon->dwMonsterIndex );
		g_pExecutive->EnablePick( pMonster->m_hMonster.pHandle );
		g_pExecutive->SetAlphaFlag(pMonster->m_hMonster.pHandle, 0xff);
		pMonster->m_dwHP = pAppearMon->dwHP;
		SetMonsterPosition( pMonster, &pMonster->m_v3CurPos, TRUE );
	}
}

void CmdDisAppearMonster( char* pMsg, DWORD dwLen )
{
	DSTC_DISAPPEAR_MON*	pDisAppear = (DSTC_DISAPPEAR_MON*)pMsg;

	CMonster*	pMonster;
	pMonster = g_pMonsterHash->GetData( pDisAppear->dwMonsterIndex );
	if( !pMonster ) return;

	// 나의 가디언이 없어졌다 적당한 처리를 해준다.
	if( pMonster->m_dwMonsterKind == OBJECT_TYPE_GUARDIAN
		&& pMonster->m_dwLordIndex == g_pMainPlayer->m_dwUserIndex )
	{
		CUserInterface::GetInstance()->CloseGuardianDescriptionWnd();

		// HP가 0으로 소환 해제라면 (죽었다면)
		if(pMonster->m_dwHP == 0)
		{
			// "가디언이 사망하였습니다."
			DisplayMessageAdd(g_Message[ETC_MESSAGE1407].szMessage, 0xFFFFC309);
			
			/*
			{	// "가디언의 수명이 %d시간 감소되었습니다."
				char szTemp[255];
		
				CItem* pGuardianItem = &g_pMainPlayer->m_pInv_Guardian[0];
				wsprintf(szTemp, 
					g_Message[ETC_MESSAGE1410].szMessage,
					pGuardianItem->GetBaseItem()->BaseItem_Guardian.wDyingPenalty / 60);
				DisplayMessageAdd(szTemp, 0xFFFFC309);
			}
			*/
		}

		// Item 이동 //
		if(g_pMainPlayer->m_GuardianItem.m_wItemID!=0)
		{
			CTDS_ITEM_PICKUP ItemPickup;
			ItemPickup.bInv			= 45;
			ItemPickup.bSectionNum	= 1;
			ItemPickup.i64ItemID	= 0;
			ItemPickup.bZipCode		= 0;				
			g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
		}
	}

	RemoveMonster( pMonster );
}

void CmdKillMonster( char* pMsg, DWORD dwLen )
{
/*	DSTC_KILL_MON* pKillMon = (DSTC_KILL_MON*)pMsg;

	CMonster*	pMonster = g_pMonsterHash->GetData( pKillMon->dwMonsterIndex );
	if( !pMonster ) __asm int 3

	pMonster->SetAction( MON_MOTION_TYPE_DOWN, 0, ACTION_ONCE );
	
	pMonster->m_hMonster.pDesc->ObjectFunc	= MonsterKillFunc;
	pMonster->m_hMonster.pDesc->dwStartTick	= g_dwCurTick;*/
}

void CmdStopMonster( char* pMsg, DWORD dwLen )
{
	DSTC_STOP_MON*	pStopMon = (DSTC_STOP_MON*)pMsg;
	//MAP_TILE*		pTile;

	CMonster*	pMonster = g_pMonsterHash->GetData( pStopMon->dwMonsterIndex );
	if( !pMonster ) 
	{
		RequestDungeonInfo( 2, pStopMon->dwMonsterIndex );
		return;
	}
	VECTOR3 v3StopPos;
	VECTOR2_TO_VECTOR3(pStopMon->v2StopPos, v3StopPos)
	SetMonsterPosition( pMonster, &v3StopPos );
	pMonster->m_v3DestPos	= v3StopPos;
	
	if( pMonster->GetStatus() == UNIT_STATUS_WALKING )
	{
		
		pMonster->SetAction( MON_MOTION_TYPE_STAND1, ACTION_LOOP );
		pMonster->SetStatus( UNIT_STATUS_NORMAL, NULL );
	}
}

void CmdAttack_Mon_User( char* pMsg, DWORD dwLen )
{
	DSTC_ATTACK_MON_USER* pAttack = (DSTC_ATTACK_MON_USER*)pMsg;
	
	CMonster*	pMonster;		// 공격
	CUser*		pUser = g_pUserHash->GetData( pAttack->dwUserIndex );
	
	pMonster = g_pMonsterHash->GetData( pAttack->dwMonsterIndex );
	if( !pMonster ) 
	{
		RequestDungeonInfo( 2, pAttack->dwMonsterIndex );
		return;
	}
	
	if( g_pMainPlayer->m_dwUserIndex == pAttack->dwUserIndex )
	{
		VECTOR3 v3MonsterPos;
		VECTOR2_TO_VECTOR3(pAttack->v2MonsterPos, v3MonsterPos)
		SetMonsterPosition( pMonster, &v3MonsterPos );
					
		// 방향을 케릭터의 방향으로 만든다.
		VECTOR3	v3DirMon;
		VECTOR3_SUB_VECTOR3( &v3DirMon, &g_pMainPlayer->m_v3CurPos, &pMonster->m_v3CurPos );
		g_pExecutive->GXOSetDirection( pMonster->m_hMonster.pHandle, &g_Camera.v3AxsiY
			, (float)(atan2(v3DirMon.z, v3DirMon.x) + DEG90 ) );		
		
		pMonster->SetActionNext( MON_MOTION_TYPE_ATTACK1, MON_MOTION_TYPE_STAND1, ACTION_ONCE, 0 );
		pMonster->SetStatus(UNIT_STATUS_ATTACKING, MonsterAttackFunc);

		// 유저를 세팅한다.
		pMonster->m_dwTemp[ MONSTER_TEMP_TARGET_TYPE ]	= OBJECT_TYPE_MAINPLAYER;
		pMonster->m_dwTemp[ MONSTER_TEMP_TARGET_INDEX ]	= g_pMainPlayer->m_dwUserIndex;		

		CUser* pLoader = g_pUserHash->GetData(pMonster->m_dwLordIndex);
		if (pLoader)
		{
			char szTemp[0xff] = {0,};
			wsprintf(szTemp, g_Message[ETC_MESSAGE506].szMessage, pLoader->m_szName, pMonster->m_szName); // "%s의 소환물 %s로부터 공격을 받았습니다."
			DisplayMessageAdd(szTemp, 0xffFF0000);
		}
	}
	else 
	{
		if( !pUser )
		{
			RequestDungeonInfo( 1, pAttack->dwUserIndex );
			return;
		}

		if( pMonster->GetStatus() != UNIT_STATUS_DAMAGING )
		{
			VECTOR3 v3MonsterPos;
			VECTOR2_TO_VECTOR3(pAttack->v2MonsterPos, v3MonsterPos)
			SetMonsterPosition( pMonster, &v3MonsterPos );

			// 방향을 케릭터의 방향으로 만든다.
			VECTOR3	v3DirMon;
			VECTOR3_SUB_VECTOR3( &v3DirMon, &pUser->m_v3CurPos, &pMonster->m_v3CurPos );
			g_pExecutive->GXOSetDirection( pMonster->m_hMonster.pHandle, &g_Camera.v3AxsiY, (float)(atan2(v3DirMon.z, v3DirMon.x) + DEG90 ) );		
			
			if( pMonster->GetStatus() != UNIT_STATUS_ATTACKING )
			{
				
				pMonster->SetActionNext( MON_MOTION_TYPE_ATTACK1, MON_MOTION_TYPE_STAND1, ACTION_ONCE, 0 );
				pMonster->SetStatus(UNIT_STATUS_ATTACKING, MonsterAttackFunc);		
				
				// 유저를 세팅한다.
				pMonster->m_dwTemp[ MONSTER_TEMP_TARGET_TYPE ]	= OBJECT_TYPE_PLAYER;
				pMonster->m_dwTemp[ MONSTER_TEMP_TARGET_INDEX ]	= pUser->m_dwUserIndex;
			}
		}	
	}

	pMonster->m_dwTemp[MONSTER_TEMP_MONMON_DAMAGE_TYPE]	= pAttack->bType;
	pMonster->m_dwTemp[MONSTER_TEMP_MONMON_DAMAGE]	= pAttack->dwDamage;
	
	// 인터페이스 작업.
	
	if( pAttack->bStatusKind == USER_HP )
	{
		if (pUser == g_pMainPlayer)
			CUserInterface::GetInstance()->SetDengeonHp(WORD(pAttack->dwCurUserHP));
		if (pAttack->dwCurUserHP == 0  )
		{
			_PlaySound( CHARACTER_SOUND_DEAD, SOUND_TYPE_CHARACTER, CHARACTER_SOUND_DEAD + ( pUser->m_wClass - 1 ) * SOUND_PER_CHARACTER, pUser->m_v3CurPos, FALSE );
			pUser->SetStatus(UNIT_STATUS_DEAD);	
			
		}
	}
	else if( pAttack->bStatusKind == USER_MP )
	{
		if (pUser == g_pMainPlayer)
		{
			CUserInterface::GetInstance()->SetDengeonHp(WORD(pAttack->dwCurUserHP>>16));
			CUserInterface::GetInstance()->SetDengeonMp(WORD(pAttack->dwCurUserHP&0xffff));
		}
		if (pAttack->dwCurUserHP>>16 == 0  )
		{
			_PlaySound( CHARACTER_SOUND_DEAD, SOUND_TYPE_CHARACTER, CHARACTER_SOUND_DEAD + ( pUser->m_wClass - 1 ) * SOUND_PER_CHARACTER, pUser->m_v3CurPos, FALSE );
			pUser->SetStatus(UNIT_STATUS_DEAD);	
			
		}
	}
}

void CmdUserStatus( char* pMsg, DWORD dwLen )
{
	CUserInterface* pUserInterface = CUserInterface::GetInstance();

	int					i;
	char				szInfo[0xff] = {0,};
	DSTC_USER_STATUS*	pUserStatus = ( DSTC_USER_STATUS* )pMsg;

	for( i=0; i<pUserStatus->bStatusMany; i++ )
	{
		switch( pUserStatus->pStatus[i].dwCode )
		{
		case USER_MAXHP:
			g_pMainPlayer->m_wMaxHP	= (DWORD)pUserStatus->pStatus[i].dwMin;
			break;
		
		case USER_MAXMP:
			g_pMainPlayer->m_wMaxMP = (DWORD)pUserStatus->pStatus[i].dwMin;
			break;
		
		case USER_HONOR:
			g_pMainPlayer->m_dwHonor	= pUserStatus->pStatus[i].dwMin;
			break;
		
		case USER_EGO:
			g_pMainPlayer->m_dwEgo	= pUserStatus->pStatus[i].dwMin;
			break;
		
		case USER_STR:
			g_pMainPlayer->m_dwStr	= pUserStatus->pStatus[i].dwMin;
			break;
		
		case USER_INT:
			g_pMainPlayer->m_dwInt	= pUserStatus->pStatus[i].dwMin;
			break;
		
		case USER_DEX:
			g_pMainPlayer->m_dwDex	= pUserStatus->pStatus[i].dwMin;
			break;
		
		case USER_VIT:
			g_pMainPlayer->m_dwVit	= pUserStatus->pStatus[i].dwMin;
			break;
		
		case USER_LUCK:
			g_pMainPlayer->m_dwLuck	= pUserStatus->pStatus[i].dwMin;
			break;
		
		case USER_ATTACK_R:
			g_pMainPlayer->m_pwAttackDamage_R[0]	= (DWORD)pUserStatus->pStatus[i].dwMin;
			g_pMainPlayer->m_pwAttackDamage_R[1]	= (DWORD)pUserStatus->pStatus[i].dwMax;
			break;
		
		case USER_ATTACK_L:
			g_pMainPlayer->m_pwAttackDamage_L[0]	= (DWORD)pUserStatus->pStatus[i].dwMin;
			g_pMainPlayer->m_pwAttackDamage_L[1]	= (DWORD)pUserStatus->pStatus[i].dwMax;
			break;
		
		case USER_AA:
			g_pMainPlayer->m_wAttackAcuracy	= (DWORD)pUserStatus->pStatus[i].dwMin;
			break;
		
		case USER_AVOID:
			g_pMainPlayer->m_wAvoid	= (DWORD)pUserStatus->pStatus[i].dwMin;
			break;
		
		case USER_BLOCKRATE:
			g_pMainPlayer->m_wBlockRate	= (DWORD)pUserStatus->pStatus[i].dwMin;
			break;
		
		case USER_FIRE_RESIST:
			g_pMainPlayer->m_wFireResist	= (DWORD)pUserStatus->pStatus[i].dwMin;
			break;
		
		case USER_ICE_RESIST:
			g_pMainPlayer->m_wIceResist		= (DWORD)pUserStatus->pStatus[i].dwMin;
			break;
		
		case USER_LIGHT_RESIST:
			g_pMainPlayer->m_wLightResist		= (DWORD)pUserStatus->pStatus[i].dwMin;
			break;
		
		case USER_POI_RESIST:
			g_pMainPlayer->m_wPoiResist		= (DWORD)pUserStatus->pStatus[i].dwMin;
			break;
		
		case USER_PHY_RESIST:
			g_pMainPlayer->m_wPhyResist		= (DWORD)pUserStatus->pStatus[i].dwMin;
			break;		
		case USER_MOVESPEED:
			// 일단 이건 생각좀 해봐야 하는것.
			g_pMainPlayer->m_fMoveSpeed		= (float)pUserStatus->pStatus[i].dwMin / 1000.f;
			break;
		case USER_EXP:			
			{
				CGameMenuWnd* pGameMenuWnd = CGameMenuWnd::GetInstance();

				if (pUserStatus->pStatus[i].dwMin > g_pMainPlayer->m_dwExp)
				{
					// MSG_ID : 91 ; %u 의 경험치를 획득하였습니다
					wsprintf(szInfo, g_Message[ETC_MESSAGE91].szMessage
						, pUserStatus->pStatus[i].dwMin-g_pMainPlayer->m_dwExp); 
				}
				else if (pUserStatus->pStatus[i].dwMin < g_pMainPlayer->m_dwExp)
				{
					// "%u 의 경험치를 잃었습니다"
					wsprintf(szInfo, g_Message[ETC_MESSAGE851].szMessage
						, g_pMainPlayer->m_dwExp-pUserStatus->pStatus[i].dwMin); 
				}

				DisplayMessageAdd(szInfo, 0xFFFF2CFF, pGameMenuWnd->m_bSystemMsgFlag);

				g_pMainPlayer->m_dwExp = pUserStatus->pStatus[i].dwMin;				

				pUserInterface->SetDengeonExpDefInc();			
			}
			break;		
		case USER_HP:
			//_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_HPRECOVER, g_pMainPlayer->m_v3CurPos, FALSE);
			// comment by minjin. 2004. 10. 29.
			// 소리는, 포션 썼을때 내자.
			pUserInterface->SetDengeonHp((pUserStatus->pStatus[i].dwMin));
			break;		
		case USER_MP:
			//_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_HPRECOVER, g_pMainPlayer->m_v3CurPos, FALSE);
			// comment by minjin. 2004. 10. 29.
			// 소리는, 포션 썼을때 내자.

			pUserInterface->SetDengeonMp((pUserStatus->pStatus[i].dwMin));
			break;
		case USER_HEALHPSPEED:
			g_pMainPlayer->m_dwHealHPSec = pUserStatus->pStatus[i].dwMin;
			break;
		
		case USER_HEALMPSPEED:
			g_pMainPlayer->m_dwHealMPSec = pUserStatus->pStatus[i].dwMin;
			break;

		case USER_POISONING:
			g_pMainPlayer->m_nPoisonSec = pUserStatus->pStatus[i].dwMin;
			break;

		case USER_DEC_SKILLLEVEL:
			{
				int nLevel = (int)pUserStatus->pStatus[i].dwMin;
				
				for(int s = 0; s < MAX_SKILL; ++s)
				{
					int nSkillLevel = g_pMainPlayer->sSkillTable[s].bSKillLevel;

					if(nSkillLevel+nLevel<0)
					{
						g_pMainPlayer->m_nDecLevel[s]				+=	nLevel-nSkillLevel;
						g_pMainPlayer->sSkillTable[s].bSKillLevel	=	0;
					}
					else
					{
						g_pMainPlayer->sSkillTable[s].bSKillLevel	+=	nLevel;
						g_pMainPlayer->sSkillTable[s].bSKillLevel	+=	g_pMainPlayer->m_nDecLevel[s];
						g_pMainPlayer->m_nDecLevel[s]				=	0;
					}
				}				
			}
			
			break;
		case USER_CTTIME:
			{
				// 따로 패킷 만들기도  그렇고 다른 유저들도 올수 있다.
				CUser* pUser = g_pUserHash->GetData(pUserStatus->pStatus[i].dwMax);
				if (pUser)
					pUser->m_wCriminalTime = (short)pUserStatus->pStatus[i].dwMin;
			}
			break;
			
		case USER_ATTACKRATE_PERCENT:
			g_pMainPlayer->m_wAttackSpeed = (WORD)pUserStatus->pStatus[i].dwMin;
			g_pMainPlayer->m_dwTemp[ USER_TEMP_LASTATTACKTICK ] = g_dwCurTick;
			break;
			
		case USER_ATTACKACURACY_PERCENT:
			g_pMainPlayer->m_wAttackAcuracy	= (WORD)pUserStatus->pStatus[i].dwMin;
			break;
			
		case USER_MAX_RESIST:
			g_pMainPlayer->m_bMaxResist	 = (BYTE)pUserStatus->pStatus[i].dwMin;
			break;
			
		case USER_BLIND:
			{

#ifndef DEVELOP_MODE
				FOG_DESC fog = g_FogDesc;
				fog.fStart += fog.fStart*max((int)pUserStatus->pStatus[i].dwMin, -100)/100.;
				fog.fEnd += fog.fEnd*max((int)pUserStatus->pStatus[i].dwMin, -100)/100.;
				
				g_pGeometry->SetFog(&fog, 0);
#endif				
				
			}			
			break;
		}
		
	}
	g_pMainPlayer->GetCheckUpgrade();

}

void CmdUserSkillLevel( char* pMsg, DWORD dwLen )
{
	DSTC_USER_SKILL_LEVEL*	pSkillLevel = ( DSTC_USER_SKILL_LEVEL* )pMsg;

	// 서버로부터 넘어온 스킬레벨을 적용한다.
	for( int i = 0; i < MAX_SKILL; i++)
		g_pMainPlayer->sSkillTable[i].bSKillLevel	= pSkillLevel->pbSkillLevel[i];

	// 스킬창 버그 수정 2005.02.16 김영대 
	CSkillWnd*	pSkillWnd = CSkillWnd::GetInstance();
	if ( pSkillWnd->GetActive() ) 	pSkillWnd->SetSkillUpBtn();

}

void CmdGuardianStatus( char* pMsg, DWORD dwLen )
{
	DSTC_GUARDIAN_STATUS* pGuardianStatus = ( DSTC_GUARDIAN_STATUS* )pMsg;

	CMonster* pMonster = g_pMonsterHash->GetData( pGuardianStatus->dwGuardianIndex );
	if( !pMonster ) return;
	if( pMonster->m_dwLordIndex != g_pMainPlayer->m_dwUserIndex ) return;	// 나의 가디언인가? 꼭 필요하진 않지만.
	
	char				szInfo[0xff] = {0,};

	for( int i=0; i<pGuardianStatus->bStatusMany; i++ )
	{
		switch( pGuardianStatus->pStatus[i].dwCode )
		{
		case MONSTER_MAXHP:
			pMonster->m_dwMaxHP = pGuardianStatus->pStatus[i].dwMin;
			break;

		case MONSTER_MAXMP:
			break;

		case MONSTER_HP:
			pMonster->m_dwHP	= pGuardianStatus->pStatus[i].dwMin;
			
			break;

		case MONSTER_MP:
			break;

		case MONSTER_EXP:
			//"%s이(가) %d의 경험치를 획득하였습니다"
			CGameMenuWnd* pGameMenuWnd = CGameMenuWnd::GetInstance();				
			if(pGuardianStatus->pStatus[i].dwMin - pMonster->m_dwExp > 0)
			{
				wsprintf(szInfo, g_Message[ETC_MESSAGE509].szMessage, pMonster->m_szName, pGuardianStatus->pStatus[i].dwMin - pMonster->m_dwExp );
				DisplayMessageAdd(szInfo, 0xFFFF2CFF, pGameMenuWnd->m_bSystemMsgFlag);
				pMonster->m_dwExp = pGuardianStatus->pStatus[i].dwMin;
				g_pMainPlayer->m_pInv_Guardian[0].m_Item_Guardian.dwCompleteTime = pMonster->m_dwExp;
			}

			break;
		}
	}
}

void CmdOriginationItemWithEffectKind(char* pMsg, DWORD dwLen)
{
	// 아이템 몰에서 산 아이템을 사용중일때 부수 적인 효과가 나타남을 위한 함수.	
	DSTC_ORIGINATIONITEMWITHEFFECTKIND* pPacket = (DSTC_ORIGINATIONITEMWITHEFFECTKIND*)pMsg;
	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);
	if (!pUser)	return;

	for(int i = 0; i < pPacket->bCount; ++i)
	{
		switch(pPacket->bOriginationItemWithEffectKind[i])
		{
		case 1:
			VECTOR3 vec = {20.f, 20.f, 20.f};
			g_pExecutive->GXOSetScale(pUser->m_hPlayerRHand.pHandle, &vec);			
			break;
		}
	}
}