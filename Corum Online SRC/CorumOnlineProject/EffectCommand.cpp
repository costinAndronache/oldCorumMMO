#include "effectcommand.h"
#include "EffectProcess.h"
#include "InitGame.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "DSMiniMap.h"
#include "message.h"
#include "Chat.h"
#include "ObjectManager.h"
#include "PlayerShopWnd.h"
#include "ItemUpgrade.h"
#include "ItemUpgradeSubWnd.h"
#include "DungeonProcess.h"
#include "Effect.h"
#include "Map.h"
#include "ChrInfo.h"
#include "CorumResource.h"
#include "DungeonSiegeInfoWnd.h"
#include "CodeFun.h"

void CmdSkillResult( char* pMsg, DWORD dwLen )
{
	DSTC_SKILL_RESULT*	pResult = (DSTC_SKILL_RESULT*)pMsg;
	
	CMonster*	pMonster;
	if (pResult->bType == 1)	// �����ۿ� ���� �����.
	{
		switch(pResult->wKind )
		{
		case ITEM_ATTR_DMG_REFLECT_POINT:
			{
				pMonster = g_pMonsterHash->GetData( pResult->dwSkillResult[0] );
				if (!pMonster)		return;

				pMonster->m_dwHP = pResult->dwSkillResult[1];
			}
			break;
		}
	}
	else if (pResult->bType == 2)	// ��ų�� ���� �����.
	{
		switch( pResult->wKind )
		{
		case  __SKILL_SUMMONACCELERATOR__:
			{
				pMonster = g_pMonsterHash->GetData( pResult->dwSkillResult[0] );
				if (!pMonster)		return;

				pMonster->m_dwMaxHP = pResult->dwSkillResult[1];
				pMonster->m_dwHP	= pResult->dwSkillResult[2];
				
			}break;
		case __SKILL_AMFLEAFIRE__:
			{
				POSITION_ pos = g_pMainPlayer->m_pUsingStatusEffectList->GetHeadPosition();
				EffectDesc* pEffectDesc = NULL;
				int nTemp = 0;
				int nAmple = int(pResult->dwSkillResult[0])/100;
				while(pos)
				{
					pEffectDesc = (EffectDesc*)g_pMainPlayer->m_pUsingStatusEffectList->GetNext(pos);
					if (pEffectDesc->pEffect->bType == TYPE_DRIVE)
					{
						if (nAmple > 0)
						{
							pEffectDesc->dwTemp[SKILL_TEMP_AMFLEAFIRETIME] = pEffectDesc->GetRemainTime(g_dwCurTick)*1000;
							nTemp = int(pEffectDesc->dwTemp[SKILL_TEMP_AMFLEAFIRETIME]) * nAmple;
						}
						else
						{
							nTemp = int(pEffectDesc->dwTemp[SKILL_TEMP_AMFLEAFIRETIME]) * nAmple;
							pEffectDesc->dwTemp[SKILL_TEMP_AMFLEAFIRETIME] = pEffectDesc->GetRemainTime(g_dwCurTick)*1000;
						}
						
						if (nTemp >= 0 || 
							(nTemp < 0 && pEffectDesc->dwTemp[SKILL_TEMP_AMFLEAFIRETIME]+nTemp >= 0))
							
							pEffectDesc->hEffect.pDesc->dwDestTime += nTemp;
						else
							pEffectDesc->hEffect.pDesc->dwDestTime = g_dwCurTick;
					}
				}
			}break;
		case __SKILL_MINDEXPLOSION__:
		case __SKILL_MINDCONTROL__:
			{
				CUser* pUser = g_pUserHash->GetData(pResult->dwSkillResult[2]);
				if (!pUser)	return;				
				
				pMonster = g_pMonsterHash->GetData( pResult->dwSkillResult[1] );
				if( !pMonster ) return;
				
				if( pResult->dwSkillResult[0] == 1 )		// ���ε� ����� ����.
				{
					// ���͸� �����ϰ� ����Ʈ�� �ε��Ѵ�.
					pUser->m_pMonster[ pResult->dwSkillResult[3] ]	= pMonster;
					pMonster->m_bZipCode	= (BYTE)pResult->dwSkillResult[3];
					pMonster->m_dwLordIndex	= pUser->m_dwUserIndex;
					pMonster->m_dwTemp[ MONSTER_TEMP_SELECTED ] = 0;
					
					pMonster->m_pEffectTarget = g_pEffectLayer->CreateGXObject(GetFile( "e0704000.chr", DATA_TYPE_EFFECT ), 1, __CHR_EFFECT_NONE__);
					
					GXSetPosition( pMonster->m_pEffectTarget->hEffect.pHandle, &pMonster->m_v3CurPos, FALSE );
					SetAction( pMonster->m_pEffectTarget->hEffect.pHandle, WORD((pMonster->m_dwLordIndex==pUser->m_dwUserIndex)+1), 0, ACTION_LOOP );
						
					if( pUser == g_pMainPlayer )
					{						
						pMonster->m_pEffectSelect = g_pEffectLayer->CreateGXObject( GetFile( "e0120000.chr", DATA_TYPE_EFFECT ), 1, __CHR_EFFECT_NONE__);
						
						GXSetPosition( pMonster->m_pEffectSelect->hEffect.pHandle, &pMonster->m_v3CurPos, FALSE );
						
						pMonster->m_pEffectSelect->bOwn = 2;
						HideObject( pMonster->m_pEffectSelect->hEffect.pHandle );
					}
				}
				else										// ����.
				{
					pUser->m_pMonster[ pResult->dwSkillResult[3] ]	= NULL;
					pMonster->m_bZipCode	= 0;
					pMonster->m_dwLordIndex	= 0;
					pMonster->m_dwTemp[ MONSTER_TEMP_SELECTED ] = 0;

					if( pMonster->m_pEffectTarget )
					{
						RemoveEffectDesc(pMonster->m_pEffectTarget);
						pMonster->m_pEffectTarget = NULL;
					}

					if( pUser == g_pMainPlayer )
					{						
						if( pMonster->m_pEffectSelect )
						{ 
							RemoveEffectDesc(pMonster->m_pEffectSelect);
							pMonster->m_pEffectSelect = NULL;
						}
					}
				}				
			}
			break;
		}
	}	
}

void CmdSkillCastingFail( char* pMsg, DWORD dwLen )
{
	DSTC_DUNGEON_CASTING_FAIL* pPacket = (DSTC_DUNGEON_CASTING_FAIL*)pMsg;
	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);
	if (!pUser)	return;

	pUser->SetSkillFailAction((ENUM_SKILL_CASTING_FAIL_REASON)pPacket->byErrorReason);
}

void CmdSkill( char* pMsg, DWORD dwLen )
{	
	DSTC_SKILL* pPacket = (DSTC_SKILL*)pMsg;
	BYTE bSkillKind = pPacket->bSkillKind;
	char bSkillLevel = pPacket->bSkillLevel;
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);
	VECTOR3 vecObjectPos={0.f,};
	GXOBJECT_HANDLE handle=0;
	BOOL bOwnSkill = FALSE;	// ���� ��� ��ų�ΰ�?
	CUser* pOwnUser = NULL;

	char szOwnName[MAX_CHARACTER_NAME_LENGTH];	
	memset(szOwnName, 0, sizeof(szOwnName));

	if (pPacket->bOwnType == OBJECT_TYPE_PLAYER)
	{
		// ����� �÷��̾�
		pOwnUser = g_pUserHash->GetData(pPacket->dwOwnIndex);
		
		if (!pOwnUser)
		{
			RequestDungeonInfo( 1, pPacket->dwOwnIndex );			
		}
		else
		{
			vecObjectPos = pOwnUser->m_v3CurPos;
			handle = pOwnUser->m_hPlayer.pHandle;
			bOwnSkill = pOwnUser==g_pMainPlayer;
			
			memcpy(szOwnName,pOwnUser->m_szName,MAX_CHARACTER_NAME_LENGTH);

			if (pOwnUser->GetStatus() != UNIT_STATUS_PLAYER_REST)
			{			
				// ��ų ��³��� ����
				pOwnUser->m_bSkillTmp				= bSkillKind;
				pOwnUser->SetActionSkill(bSkillKind);
				pOwnUser->SetStatus(UNIT_STATUS_SKILL);
				pOwnUser->m_hPlayer.pDesc->ObjectFunc		= PlayerSkillFunc;
			}
		}
	}
	else if(pPacket->bOwnType == OBJECT_TYPE_MONSTER) // ����� ����.
	{
		CMonster* pOwnMonster = g_pMonsterHash->GetData(pPacket->dwOwnIndex);
		if (!pOwnMonster)
		{
			RequestDungeonInfo( 2, pPacket->dwOwnIndex );			
		}
		else
		{
			// ��ų ��� ���� ����.		
			vecObjectPos = pOwnMonster->m_v3CurPos;
			handle = pOwnMonster->m_hMonster.pHandle;			

			pOwnMonster->m_bSkillTmp = bSkillKind;
			pOwnMonster->SetActionSkill(bSkillKind);
			pOwnMonster->SetStatus(UNIT_STATUS_SKILL, NULL);
			pOwnMonster->m_hMonster.pDesc->ObjectFunc = MonsterSkillFunc;
		}				
	}
	else if (pPacket->bOwnType == OBJECT_TYPE_SKILL)
	{		
		vecObjectPos.x = (pPacket->dwOwnIndex>>16)*TILE_WIDTH+TILE_WIDTH/2;
		vecObjectPos.z = (pPacket->dwOwnIndex&0x0000ffff)*TILE_WIDTH+TILE_WIDTH/2;
		vecObjectPos.y = 0;
	}
	// Ÿ�ϰ���� �Ű���.
	//pPacket->vecStart.x = pPacket->vecStart.x/TILE_SIZE*TILE_SIZE+TILE_SIZE/2;
	//pPacket->vecStart.z = pPacket->vecStart.z/TILE_SIZE*TILE_SIZE+TILE_SIZE/2;
	
	EffectDesc* pEffectDesc = NULL;

	switch(bSkillKind)
	{// ���� ��ƾ..
	case __SKILL_FIREMISSILE__:
	case __SKILL_DETONATION__:
	case __SKILL_ICEMISSILE__:
	case __SKILL_SOULBLASTER:
	case __SKILL_ROUNDRANGE__:
	case __SKILL_FLASHSHOCK__:
	//�迵�� �߻� 
	//case __SKILL_POSIONCLOUD__:
		{
			if (!pPacket->bTargetCount)
			{
				if (pOwnUser)	
					pOwnUser->SetSkillFailAction(SKILL_CASTING_FAIL_NONE_REASEON);
				return;
			}	
			
		//	BYTE bCount = 0;
			float ang[] = {0, 0.5, -0.5, 1.0, -1.0, 1.5, -1.5, 3.0}; // ���ư� ����
			int nCompass = pEffect->Value[bSkillLevel].nCompass;
			if (bSkillKind == __SKILL_FLASHSHOCK__)
				nCompass = pEffect->Value[bSkillLevel].nDuration;

			VECTOR3 v3Start;
			for(int i = 0; i < nCompass; ++i)
			{//  ��������
				float tempAng = ang[i];
				if (bSkillLevel+1 >= MAX_SKILL_LEVEL/2) // ���� �ְ�.
					tempAng = DEG360/nCompass*i;
				VECTOR2_TO_VECTOR3(pPacket->vecStart, v3Start)
				pEffectDesc = SkillSubProcess_FireBall(bSkillKind
					, bSkillLevel, pPacket->bOwnType, pPacket->dwOwnIndex
					, pPacket->bTargetCount, v3Start, pPacket->sTargetInfo
					, tempAng);
				pEffectDesc->vecBasePosion = vecObjectPos;
				pEffectDesc->vecBasePosion.y+=100;
			
				pEffectDesc->dwTemp[SKILL_TEMP_CREATECOUNT] = i; // ���° ����.
				
				for(int nCount=0; nCount < pPacket->bTargetCount; ++nCount)
				{
					pEffectDesc->byTargetObjectType[nCount]	= pPacket->sTargetInfo[nCount].bTargetType;
					pEffectDesc->dwTargetIndex[nCount]		= pPacket->sTargetInfo[nCount].dwTargetIndex;					
				}
				/*
				bCount = rand()%pPacket->bTargetCount;
				DWORD dwTemp = pEffectDesc->byTargetObjectType[0];
				pEffectDesc->byTargetObjectType[0]=pPacket->sTargetInfo[bCount].bTargetType;
				pPacket->sTargetInfo[bCount].bTargetType = dwTemp;
				dwTemp = pEffectDesc->dwTargetIndex[0];
				pEffectDesc->dwTargetIndex[0]=pPacket->sTargetInfo[bCount].dwTargetIndex;
				pPacket->sTargetInfo[bCount].dwTargetIndex=dwTemp;*/
				pEffectDesc->dwCount = pPacket->bTargetCount;				
			}
			
		}break;
	
	case __SKILL_MAGMAWALL__:
		{
			BYTE bCount = 0;
			VECTOR3 v3Start;
			for(int i = 0; i < pEffect->Value[bSkillLevel].nCompass; ++i)
			{//  ��������
				for(int j = 0; j < pEffect->Value[bSkillLevel].nCompass; ++j)
				{
					VECTOR2_TO_VECTOR3(pPacket->vecStart, v3Start)
					pEffectDesc = SkillSubProcess_MagmaWall(bSkillKind
						, bSkillLevel, pPacket->bOwnType, pPacket->dwOwnIndex
						, pPacket->bTargetCount, v3Start
						, pPacket->sTargetInfo);
					pEffectDesc->dwCount = 0;
					pEffectDesc->dwTemp[SKILL_TEMP_CREATECOUNT] = i*pEffect->Value[bSkillLevel].nCompass+j; // ���° ����.
					pEffectDesc->f_Radian = (float)atan2(pPacket->vecStart.y-vecObjectPos.z, pPacket->vecStart.x-vecObjectPos.x);
					if (bCount < pPacket->bTargetCount)	
					{//  ������ ���� ��ü�� ������.
						pEffectDesc->byTargetObjectType[0]	= pPacket->sTargetInfo[bCount].bTargetType;
						pEffectDesc->dwTargetIndex[0]		= pPacket->sTargetInfo[bCount].dwTargetIndex;
						
						pEffectDesc->dwCount = 1;
						
						++bCount;
					}
				}				
			}
		}break;
	case __SKILL_ICEWALL__:
		{
			BYTE bCount = 0;
			VECTOR3 v3Start;
			for(int i = 0; i < (pEffect->Value[bSkillLevel].nCompass)*8+1; ++i)
			{
				VECTOR2_TO_VECTOR3(pPacket->vecStart, v3Start)
				pEffectDesc = SkillSubProcess_IceWall(bSkillKind, bSkillLevel
					, pPacket->bOwnType, pPacket->dwOwnIndex
					, pPacket->bTargetCount, v3Start
					, pPacket->sTargetInfo);
				pEffectDesc->dwCount = 0;
				pEffectDesc->dwTemp[SKILL_TEMP_CREATECOUNT] = i; // ���° ������.
				if (bCount < pPacket->bTargetCount)	
				{//  ������ ���� ��ü�� ������.
					pEffectDesc->byTargetObjectType[0]	= pPacket->sTargetInfo[bCount].bTargetType;
					pEffectDesc->dwTargetIndex[0]		= pPacket->sTargetInfo[bCount].dwTargetIndex;
					
					pEffectDesc->dwCount = 1;
					
					++bCount;
				}
			}			
		}break;
	case __SKILL_BLASTQUAKE__:
		{
			BYTE bCount = 0;
			VECTOR3 v3Start;
			int nMax[] = {1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78, 91, 105, 120, 136, 153, 171, 190};
			for(int i = 0; i < nMax[pEffect->Value[bSkillLevel].nCompass]; ++i)
			{// ���׸� ��������
				VECTOR2_TO_VECTOR3(pPacket->vecStart, v3Start)
				pEffectDesc = SkillSubProcess_BlastQuake(bSkillKind, bSkillLevel
					, pPacket->bOwnType, pPacket->dwOwnIndex
					, pPacket->bTargetCount, v3Start
					, pPacket->sTargetInfo);
				pEffectDesc->vecBasePosion = vecObjectPos;
				pEffectDesc->dwCount = 0;
				pEffectDesc->dwTemp[SKILL_TEMP_CREATECOUNT] = i; // ���° ���׸�����.
				pEffectDesc->f_Radian = (float)atan2(pPacket->vecStart.y-vecObjectPos.z, pPacket->vecStart.x-vecObjectPos.x);
				if (bCount < pPacket->bTargetCount)	
				{// ���׸��� ������ ���� ��ü�� ������.
					pEffectDesc->byTargetObjectType[0]	= pPacket->sTargetInfo[bCount].bTargetType;
					pEffectDesc->dwTargetIndex[0]		= pPacket->sTargetInfo[bCount].dwTargetIndex;
					
					pEffectDesc->dwCount = 1;
					
					++bCount;
				}
			}
			
		}break;
	case __SKILL_CHAINATTACKER__:
		{
			VECTOR3 v3Start;
			for(int i = 0; i < pPacket->bTargetCount; ++i)
			{
				VECTOR2_TO_VECTOR3(pPacket->vecStart, v3Start)
				// ���� ��ü�� ������ ���.
				pEffectDesc = SkillSubProcess_General(bSkillKind, bSkillLevel
					, BYTE(bSkillLevel/10), pPacket->bOwnType, pPacket->dwOwnIndex
					, pPacket->bTargetCount, v3Start
					, pPacket->sTargetInfo);
				
				pEffectDesc->byTargetObjectType[0]	= pPacket->sTargetInfo[i].bTargetType;
				pEffectDesc->dwTargetIndex[0]		= pPacket->sTargetInfo[i].dwTargetIndex;
				pEffectDesc->dwCount = 1;
				pEffectDesc->vecBasePosion = vecObjectPos;	// ������� ���� ������ �Ѵ�.
				pEffectDesc->vecBasePosion.y+=300;
			}			
		}break;
	default:
		{
			VECTOR3 v3Start={0.f,};
			if (bSkillKind == __SKILL_HEAL__ 
				|| bSkillKind == __SKILL_PRAY__
				|| bSkillKind == __SKILL_ORDSLUM__
				|| bSkillKind == __SKILL_SUITSLUM__)
			{
				// ���������̸鼭 ������ ����Ʈ�� ��� �Ұ��. ���߿� �÷��׸� ������ ��.��
				if (!pPacket->bTargetCount)	
				{
					if (pOwnUser)	
						pOwnUser->SetSkillFailAction(SKILL_CASTING_FAIL_NONE_REASEON);
					return;
				}
				
				for(int i = 0; i < pPacket->bTargetCount; ++i)
				{
					// ���� ��ü�� ������ ���.
					if (pPacket->sTargetInfo[i].bTargetType ==OBJECT_TYPE_MONSTER)
					{
						CMonster*	pMonster = g_pMonsterHash->GetData( pPacket->sTargetInfo[i].dwTargetIndex);
						if (pMonster)
							v3Start = pMonster->m_v3CurPos;
					}
					else if (pPacket->sTargetInfo[i].bTargetType ==OBJECT_TYPE_PLAYER)
					{
						CUser*	pUser = g_pUserHash->GetData( pPacket->sTargetInfo[i].dwTargetIndex);
						if (pUser)
							v3Start = pUser->m_v3CurPos;
					}	

					pEffectDesc = SkillSubProcess_General(bSkillKind, bSkillLevel
						, 0, pPacket->bOwnType, pPacket->dwOwnIndex
						, pPacket->bTargetCount, v3Start, pPacket->sTargetInfo);
					
					pEffectDesc->byTargetObjectType[0]	= pPacket->sTargetInfo[i].bTargetType;
					pEffectDesc->dwTargetIndex[0]		= pPacket->sTargetInfo[i].dwTargetIndex;
					pEffectDesc->dwCount = 1;
				}				
			}
			else
			{
				if (pEffect->bSkillType/10%10 == 0
					&& !pPacket->bTargetCount && pOwnUser)
				{
					pOwnUser->SetSkillFailAction(SKILL_CASTING_FAIL_NONE_REASEON);
					return;
				}
				
				VECTOR2_TO_VECTOR3(pPacket->vecStart, v3Start)

				pEffectDesc = SkillSubProcess_General(bSkillKind, bSkillLevel
					, 0, pPacket->bOwnType, pPacket->dwOwnIndex
					, pPacket->bTargetCount, v3Start
					, pPacket->sTargetInfo);
				for(int i = 0; i < pPacket->bTargetCount; ++i)
				{// ���� ��ü�� ������ ���.
					pEffectDesc->byTargetObjectType[i]	= pPacket->sTargetInfo[i].bTargetType;
					pEffectDesc->dwTargetIndex[i]		= pPacket->sTargetInfo[i].dwTargetIndex;
				}
				pEffectDesc->dwCount = pPacket->bTargetCount;
				if (bSkillKind == __SKILL_DRAGONICFIREBLAST__)
				{// �뿡 �Կ��� ������ ��
					pEffectDesc->vecBasePosion = vecObjectPos;
				}
				
			}			
		}		
	}
	if (!pEffectDesc)
	{
		if (pOwnUser)
			pOwnUser->SetSkillFailAction(SKILL_CASTING_FAIL_REASON_LOW);
		return;
	}
	switch(pPacket->bSkillKind)
	{// ������ ��� �������� ��ų ������ Ʋ���.
	
	case __SKILL_RAGINGSWORD__:
		{		
			float fRad;	VECTOR3 vec;
			g_pExecutive->GXOGetDirection(handle, &vec, &fRad);
			g_pExecutive->GXOSetDirection( pEffectDesc->hEffect.pHandle, &g_Camera.v3AxsiY, -fRad );
			pEffectDesc->vecBasePosion = vecObjectPos;
		}break;
	
	case __SKILL_LIGHTNINGBREAK__:
		{	
			float fRad;	VECTOR3 vec;
			g_pExecutive->GXOGetDirection(handle, &vec, &fRad);
			g_pExecutive->GXOSetDirection( pEffectDesc->hEffect.pHandle, &vec, fRad );
			pEffectDesc->vecBasePosion = vecObjectPos;
		}break;			
	}
	
	if (pEffectDesc->pEffect->bSkillType == TARGETTYPE_LINE)
	{ 	//��ų ���⸸ Ʋ���.
		VECTOR3 v3Start;
		VECTOR2_TO_VECTOR3(pPacket->vecStart, v3Start)
		VECTOR3	v3DirMon;
		VECTOR3_SUB_VECTOR3( &v3DirMon, &v3Start, &vecObjectPos );
		g_pExecutive->GXOSetDirection( pEffectDesc->hEffect.pHandle, &g_Camera.v3AxsiY, (float)(atan2(v3DirMon.z, v3DirMon.x) + DEG90 ) );		
	}
	
	for(int i = 0; i < pPacket->bTargetCount; ++i)
	{// ���� ���͸�ŭ...		
		if (pPacket->sTargetInfo[i].bTargetType ==OBJECT_TYPE_MONSTER)
		{
			CMonster*	pMonster = g_pMonsterHash->GetData( pPacket->sTargetInfo[i].dwTargetIndex);			
			if( !pMonster )
			{
				RequestDungeonInfo( 2, pPacket->sTargetInfo[i].dwTargetIndex );
				continue;
			}

			pMonster->m_dwTemp[MONSTER_TEMP_DAMAGE_TYPE]	= pPacket->sTargetInfo[i].bType;
			pMonster->m_dwTemp[MONSTER_TEMP_DAMAGE] = pPacket->sTargetInfo[i].dwDamage;
			pMonster->m_dwHP =  pPacket->sTargetInfo[i].dwCurHP;
	
			// ƨ������ ���� ����
			VECTOR3	v3DirMon;			
			pMonster->m_vecTemp = pMonster->m_v3CurPos;
			if (bSkillKind == __SKILL_ZEAL__ 
				|| bSkillKind == __SKILL_BASH__ 
				|| bSkillKind == __SKILL_RAGINGSWORD__)
			{
				VECTOR3_SUB_VECTOR3( &v3DirMon, &pMonster->m_v3CurPos, &vecObjectPos );	
			}
			else
			{
				VECTOR3 v3Start;
				VECTOR2_TO_VECTOR3(pPacket->vecStart, v3Start)
				VECTOR3_SUB_VECTOR3( &v3DirMon, &pMonster->m_v3CurPos, &v3Start );
			}
			
			float pfRad = (float)atan2(v3DirMon.z, v3DirMon.x);
			pMonster->m_radian = pfRad;
					
			if( pMonster->m_dwHP == 0 )
			{
				if (pPacket->bOwnType == OBJECT_TYPE_PLAYER)
				{
					CUser* pKiller = g_pUserHash->GetData(pPacket->dwOwnIndex);
					
					pMonster->InitDie(pKiller, bOwnSkill, min(pMonster->m_dwTemp[MONSTER_TEMP_DAMAGE]/10, 50)+GetRandom(20), g_pChrInfoEffect->GetFrameInfo(pEffectDesc->wChrNum, 0, 1, _CHRINFO_FRAME0)+GetSkillStartActionFinishCount(bSkillKind, pKiller, pPacket->bOwnType));
				}
				else if (pPacket->bOwnType == OBJECT_TYPE_MONSTER)
				{
					CMonster* pOwnMonster = g_pMonsterHash->GetData(pPacket->dwOwnIndex);
					if (pOwnMonster)
					{
						CUser* pKiller = g_pUserHash->GetData(pOwnMonster->m_dwLordIndex);
						pMonster->InitDie(pKiller, bOwnSkill, min(pMonster->m_dwTemp[MONSTER_TEMP_DAMAGE]/10, 50)+GetRandom(20), g_pChrInfoEffect->GetFrameInfo(pEffectDesc->wChrNum, 0, 1, _CHRINFO_FRAME0)+GetSkillStartActionFinishCount(bSkillKind, pKiller, pPacket->bOwnType));
					}
					else
					{
						pMonster->InitDie(NULL, bOwnSkill, min(pMonster->m_dwTemp[MONSTER_TEMP_DAMAGE]/10, 50)+GetRandom(20), g_pChrInfoEffect->GetFrameInfo(pEffectDesc->wChrNum, 0, 1, _CHRINFO_FRAME0)+GetSkillStartActionFinishCount(bSkillKind, NULL, pPacket->bOwnType));
					}
				}
				else if (pPacket->bOwnType == OBJECT_TYPE_SKILL)
				{
					pMonster->InitDie(NULL, FALSE, min(pMonster->m_dwTemp[MONSTER_TEMP_DAMAGE]/10, 50)+GetRandom(20), g_pChrInfoEffect->GetFrameInfo(pEffectDesc->wChrNum, 0, 1, _CHRINFO_FRAME0)+GetSkillStartActionFinishCount(bSkillKind, NULL, pPacket->bOwnType));
				}
			}
			pMonster->WithActionFunc = pEffect->MonsterSKillDamageWithAction;
			if (pMonster->m_dwTemp[MONSTER_TEMP_DAMAGE_TYPE] == 4)
			{
				if(pEffect->bType == TYPE_DRIVE || pEffect->bType == TYPE_TIMEZERO)			
				{
					pMonster->AttachSkill(pEffect->bID, bSkillLevel, pPacket->dwDestTime);
					if (pMonster->m_pEffectDesc[pEffect->bID])
						HideObject(pMonster->m_pEffectDesc[pEffect->bID]->hEffect.pHandle);
				}
			}
			if (bSkillKind == __SKILL_POISONING__ 
				&& 	pMonster->m_pEffectDesc[__SKILL_POISONING__])
				pMonster->m_pEffectDesc[__SKILL_POISONING__]->dwTemp[SKILL_TEMP_POSONING] = pPacket->sTargetInfo[i].dwDamage;
			else if (bSkillKind == __SKILL_POSIONCLOUD__
				&& pMonster->m_pEffectDesc[__SKILL_POSIONCLOUD__])
				pMonster->m_pEffectDesc[__SKILL_POSIONCLOUD__]->dwTemp[SKILL_TEMP_POSONING] = pPacket->sTargetInfo[i].dwDamage;		
			
		}// IF
		else if(pPacket->sTargetInfo[i].bTargetType ==OBJECT_TYPE_PLAYER)
		{
			CUser* pTargetUser = (CUser*)g_pUserHash->GetData(pPacket->sTargetInfo[i].dwTargetIndex);
			if (!pTargetUser)
			{
				RequestDungeonInfo( 1, pPacket->sTargetInfo[i].dwTargetIndex );
				continue;
			}
			if (g_pMainPlayer == pTargetUser)
			{
				CUserInterface::GetInstance()->SetDengeonHp((WORD)pPacket->sTargetInfo[i].dwCurHP);
			}

			pTargetUser->m_dwTemp[USER_TEMP_DAMAGE_TYPE] = pPacket->sTargetInfo[i].bType;
			pTargetUser->m_dwTemp[USER_TEMP_DAMAGE] = pPacket->sTargetInfo[i].dwDamage;pTargetUser->m_dwTemp[USER_TEMP_DAMAGE] = pPacket->sTargetInfo[i].dwDamage;
			
			// modified by deepdark :)
			if( pPacket->sTargetInfo[i].dwCurHP == 0 )
			{
				pTargetUser->SetStatus(UNIT_STATUS_DEAD);
				char szTemp[0xff]={0,};

				if(bSkillKind == __SKILL_TREEAGION__)
				{
					wsprintf(szTemp, g_Message[ETC_MESSAGE930].szMessage, pTargetUser->m_szName); // "%s���� ��Ȱ�� �����߽��ϴ�"
				}
				else
				{	
					if (pPacket->bOwnType == OBJECT_TYPE_PLAYER)
						wsprintf(szTemp, g_Message[ETC_MESSAGE505].szMessage, szOwnName, pTargetUser->m_szName);//"%s���� %s���� �׿����ϴ�."
				}

				DisplayMessageAdd(szTemp, 0xffFF0000);
			}

			if (pTargetUser->m_dwTemp[USER_TEMP_DAMAGE_TYPE] == 4)
			{
				if(pEffect->bType == TYPE_DRIVE || pEffect->bType == TYPE_TIMEZERO)			
				{
					pTargetUser->AttachSkill(pEffect->bID, bSkillLevel, pPacket->dwDestTime);
					HideObject(pTargetUser->m_pEffectDesc[pEffect->bID]->hEffect.pHandle);
				}
			}

			if (bSkillKind == __SKILL_POISONING__ 
				&& pTargetUser->m_pEffectDesc[__SKILL_POISONING__])
				pTargetUser->m_pEffectDesc[__SKILL_POISONING__]->dwTemp[SKILL_TEMP_POSONING] = pPacket->sTargetInfo[i].dwDamage;
			else if (bSkillKind == __SKILL_POSIONCLOUD__
				&& pTargetUser->m_pEffectDesc[__SKILL_POSIONCLOUD__])
				pTargetUser->m_pEffectDesc[__SKILL_POSIONCLOUD__]->dwTemp[SKILL_TEMP_POSONING] = pPacket->sTargetInfo[i].dwDamage;			
		}
	}// FOR	
}


// ���� �ٲ����� ���ο��������� ���ܾ�.. �׶� �̹� ����Ǿ� �ִ� ��ų�� ����..
void CmdApearSkill( char* pMsg, DWORD dwLen )
{
	DSTC_APEAR_SKILL* pPacket = (DSTC_APEAR_SKILL*)pMsg;
	if (pPacket->bTargetType == OBJECT_TYPE_PLAYER)
	{
		CUser* pTargetUser = g_pUserHash->GetData(pPacket->dwTargetIndex);
		if (pTargetUser)
		{			
			pTargetUser->AttachSkill(pPacket->bSkillKind, pPacket->bSkillLevel, pPacket->dwDestTime);			
		}
	}
	else if(pPacket->bTargetType == OBJECT_TYPE_MONSTER)
	{
		CMonster* pTargetMonster = g_pMonsterHash->GetData(pPacket->dwTargetIndex);
		if (pTargetMonster)
		{			
			pTargetMonster->AttachSkill(pPacket->bSkillKind, pPacket->bSkillLevel, pPacket->dwDestTime);
		}
	}
}


// ������ ���� ���( ��������� ���������� )
void CmdAttackMode( char* pMsg, DWORD dwLen )
{
	DSTC_ATTACK_MODE* pPacket = (DSTC_ATTACK_MODE*)pMsg;
	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);
	if (pUser)
	{
		pUser->SetAttackMode(pPacket->bAttackMode);		
	}
}


void CmdPlayerBadMode( char* pMsg, DWORD dwLen )
{
	DSTC_PLAYER_BAD* pPacket = (DSTC_PLAYER_BAD*)pMsg;
	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);
	if (pUser)
	{
		pUser->m_bBad = (BOOL)pPacket->byBad;
	}	
}


void CmdPKMode( char* pMsg, DWORD dwLen )
{
	DSTC_DUNGEON_PKMODE* pPacket = (DSTC_DUNGEON_PKMODE*)pMsg;
	CUserInterface::GetInstance()->SetPKMode(pPacket->bPKMode);
}


void CmdSkillCasting( char* pMsg, DWORD dwLen )
{
	DSTC_DUNGEON_CASTING *pPacket = (DSTC_DUNGEON_CASTING*)pMsg;
	
	CUser* pOwnUser = g_pUserHash->GetData(pPacket->dwUserIndex);
	
	VECTOR3 vecTarget;
	VECTOR2_TO_VECTOR3(pPacket->vecTarget, vecTarget)
	BOOL bDirection = TRUE;

	if(!pOwnUser)
	{
		RequestDungeonInfo( 1, pPacket->dwUserIndex );
		return;
	}

	if (OBJECT_TYPE_MONSTER == pPacket->bTargetType)
	{
		CMonster* pMonster = g_pMonsterHash->GetData(pPacket->dwTargetIndex);
		if( !pMonster )
		{
			RequestDungeonInfo( 2, pPacket->dwTargetIndex );
			return;
		}
	}
	else if (OBJECT_TYPE_PLAYER == pPacket->bTargetType)
	{
		CUser* pTargetUser = g_pUserHash->GetData(pPacket->dwTargetIndex);
		if( !pTargetUser )
		{
			RequestDungeonInfo( 1, pPacket->dwTargetIndex );
			return;
		}
		bDirection = !(pOwnUser == pTargetUser);
	}

	if (pPacket->bSkillKind)
	{
		pOwnUser->SetActionCasting(pPacket->bSkillKind, vecTarget, bDirection);
	}
	else	// fail
	{
		if (g_pMainPlayer == pOwnUser)
			pOwnUser->SetStatus(UNIT_STATUS_NORMAL);
	}	
}


void CmdSkillStopStatusEffect( char* pMsg, DWORD dwLen )
{
	DSTC_DUNGEON_STOPSTATUSEFFECT * pPacket = (DSTC_DUNGEON_STOPSTATUSEFFECT*)pMsg;

	if (pPacket->bTargetType==0)
	{
		// �÷��̾��� ���¸��� ����
		CUser* pUser = g_pUserHash->GetData(pPacket->dwTargetIndex);		
		if (!pUser)	
		{
			RequestDungeonInfo( 1, pPacket->dwTargetIndex );
			return;
		}

		EffectDesc* pEffectDesc = pUser->m_pEffectDesc[pPacket->bSkillKind];
		if (pEffectDesc)
		{
			if (pUser == g_pMainPlayer)
				CUserInterface::GetInstance()->SetDengeonHp((WORD)pPacket->dwCurHP);
			
			if (pEffectDesc->bEffectInfoNum == __SKILL_HIDING__)
			{
				// �����ϰ�� ���� ���̵�.
				pUser->UserObjectEnable(TRUE);
				pUser->UserObjectAlpha(255);				
			}

			if (pEffectDesc->pEffect->bType == TYPE_DRIVE)
				pUser->m_bOverDriveCount--;

			pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_FINISH] = 1; // ���� ���� ����Ʈ ������� �÷���.

			pUser->m_pUsingStatusEffectList->RemoveAt(pEffectDesc->pUsingStatus);

			pUser->m_pEffectDesc[pEffectDesc->pEffect->bID] = NULL;
			RemoveEffectDesc(pEffectDesc);
			
		}

		POSITION_ pos = pUser->m_pUsingStatusEffectList->GetHeadPosition();
		while (pos)
		{
			pEffectDesc = (EffectDesc*)pUser->m_pUsingStatusEffectList->GetNext(pos);
			if (pEffectDesc->pEffect && pEffectDesc->pEffect->bType == TYPE_DRIVE)
			{			
				if (g_pEffectLayer->IsEffectShow(pEffectDesc))
					ShowObject(pEffectDesc->hEffect.pHandle);

				if (pUser->m_bOverDriveCount>1)
					::SetAction(pEffectDesc->hEffect.pHandle, (WORD)min(pUser->m_bOverDriveCount, 3), 0, ACTION_LOOP);
				else
					::SetAction(pEffectDesc->hEffect.pHandle, 1, 0, ACTION_LOOP);

				break;
			}
		}
	}
	else
	{
		// ������ ���¸��� ����
		CMonster* pMonster = g_pMonsterHash->GetData(pPacket->dwTargetIndex);
		if (!pMonster)	
		{
			RequestDungeonInfo( 2, pPacket->dwTargetIndex );
			return;
		}
		
		EffectDesc* pEffectDesc = pMonster->m_pEffectDesc[pPacket->bSkillKind];
		if (pEffectDesc)
		{
			pMonster->m_dwHP = pPacket->dwCurHP;
			if (pEffectDesc->pEffect->bType == TYPE_DRIVE)
				pMonster->m_bOverDriveCount--;
			pMonster->m_pUsingStatusEffectList->RemoveAt(pEffectDesc->pUsingStatus);
			pMonster->m_pEffectDesc[pEffectDesc->pEffect->bID] = NULL;
			RemoveEffectDesc(pEffectDesc);
			
			pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_FINISH] = 1; // ���� ���� ����Ʈ ������� �÷���.
		}

		POSITION_ pos = pMonster->m_pUsingStatusEffectList->GetHeadPosition();
		while (pos)
		{
			pEffectDesc = (EffectDesc*)pMonster->m_pUsingStatusEffectList->GetNext(pos);
			if (pEffectDesc->pEffect && pEffectDesc->pEffect->bType == TYPE_DRIVE)
			{			
				if (g_pEffectLayer->IsEffectShow(pEffectDesc))
					ShowObject(pEffectDesc->hEffect.pHandle);

				if (pMonster->m_bOverDriveCount>1)
					::SetAction(pEffectDesc->hEffect.pHandle, (WORD)min(pMonster->m_bOverDriveCount, 3), 0, ACTION_LOOP);
				else
					::SetAction(pEffectDesc->hEffect.pHandle, 1, 0, ACTION_LOOP);

				break;
			}
		}
	}	
}

void CmdSkill_ChangeLR( char* pMsg, DWORD dwLen )
{
//	DSTC_DUNGEON_CHANGE_LRSKILLKIND* pPacket = (DSTC_DUNGEON_CHANGE_LRSKILLKIND*)pMsg;
}

void CmdDestroyCP( char* pMsg, DWORD dwLen )
{
	DSTC_DESTROY_CP* pPacket = (DSTC_DESTROY_CP*)pMsg;
	
	POSITION_ pos = g_pMap->m_pCPList->GetHeadPosition();
	while(pos)
	{
		CP_DESC* pCPDesc = (CP_DESC*)g_pMap->m_pCPList->GetNext(pos);
		if (pCPDesc->bID == pPacket->sCP_Info.bCPID)
		{
			::SetAction( pCPDesc->pEffectDesc->hEffect.pHandle, 2, 0, ACTION_ONCE );
			pCPDesc->pEffectDesc->hEffect.pDesc->ObjectFunc = EffectOnceAndHideFunc;
			pCPDesc->pSpr_Minimap_Dot->ShowSprite(FALSE);
			pCPDesc->bDestroy = 1;
			--g_pMap->m_bCPRemainCount;
			
#ifdef DEVELOP_MODE
			char szTemp[0xff]={0,};
			wsprintf(szTemp, "CP Name : %s Description : %s", g_pCPTable[pPacket->sCP_Info.wProperty].szKorName, g_pCPTable[pPacket->sCP_Info.wProperty].szDescription);
			DisplayMessageAdd(szTemp, 0xFFFFC309);
#endif

			if (g_pCPTable[pPacket->sCP_Info.wProperty].bClass == 2)
			{
				// ������ ��� �ִ� cp�� �������ױ�. ��.��				
				EffectDesc* pEffectDesc = g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(EFFECT_CP_RELEASE1), 1, __CHR_EFFECT_NONE__);
				::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE );
				GXSetPosition(pEffectDesc->hEffect.pHandle, &pCPDesc->pEffectDesc->vecBasePosion, FALSE);
				pEffectDesc->hEffect.pDesc->ObjectFunc = EffectOnceAndRemoveFunc;
			}
			else if (g_pCPTable[pPacket->sCP_Info.wProperty].bClass == 1)
			{				
				EffectDesc* pEffectDesc = g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(EFFECT_CP_RELEASE2), 1, __CHR_EFFECT_NONE__);
				::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE );
				GXSetPosition(pEffectDesc->hEffect.pHandle, &pCPDesc->pEffectDesc->vecBasePosion, FALSE);
				pEffectDesc->hEffect.pDesc->ObjectFunc = EffectOnceAndRemoveFunc;
			}
		}
	}
}

void CmdSiegeInfo( char* pMsg, DWORD dwLen )
{
	// ���� ����
	DSTC_DUNGEON_SIEGEINFO* pPacket = (DSTC_DUNGEON_SIEGEINFO*)pMsg;
	CDungeonSiegeInfoWnd::GetInstance()->m_dwKillMonsterLayerCount = pPacket->wMonsterCurCount;
	CDungeonSiegeInfoWnd::GetInstance()->m_dwCurrentMonterLayerCount = pPacket->wMonsterTotalCount;
	g_pMap->m_bCPRemainCount = pPacket->bCPTotalCount;
	
	for(int i = 0; i < pPacket->bCPTotalCount; ++i)
	{
		POSITION_ pos = g_pMap->m_pCPList->GetHeadPosition();
		while(pos)
		{
			CP_DESC* pCPDesc = (CP_DESC*)g_pMap->m_pCPList->GetNext(pos);
			if (pPacket->sCP_Info[i].bCPID == pCPDesc->bID)
			{
				if (pPacket->sCP_Info[i].bDestroy)
				{
					// cp �ı� �Ǿ���.
					pCPDesc->pSpr_Minimap_Dot->ShowSprite(FALSE);
					pCPDesc->pSpr_Cp_GuageBar->ShowSprite(FALSE);
					pCPDesc->pSpr_Cp_GuageBoard->ShowSprite(FALSE);
					HideObject(pCPDesc->pEffectDesc->hEffect.pHandle);
					--g_pMap->m_bCPRemainCount;					
				}
				else
				{
					pCPDesc->pSpr_Minimap_Dot->ShowSprite(TRUE);
					ShowObject(pCPDesc->pEffectDesc->hEffect.pHandle);
					::SetAction( pCPDesc->pEffectDesc->hEffect.pHandle, 1, 0, ACTION_LOOP );					
				}

				pCPDesc->bDestroy = pPacket->sCP_Info[i].bDestroy;
				pCPDesc->wProperty = pPacket->sCP_Info[i].wProperty;
				pCPDesc->pEffectDesc->hEffect.pDesc->ObjectFunc = NULL;
				break;
			}
		}
	}
}

void CmdPKCount( char* pMsg, DWORD dwLen )
{
	DSTC_PKCOUNT* pPacket = (DSTC_PKCOUNT*)pMsg;
	char szTemp[0xff]={0,};
	//"PKī���� %d, �ݺ�PKī���� %d, ����PKī���� %d�� �Ǿ����ϴ�."
	wsprintf(szTemp, g_Message[ETC_MESSAGE551].szMessage, pPacket->bPKCount, pPacket->bPKRepeatCount, pPacket->bPKCount+pPacket->bPKRepeatCount);
	DisplayMessageAdd(szTemp, 0xFFFF0000);
}

void CmdBadAction( char* pMsg, DWORD dwLen )
{
	DSTC_BADACTION* pPacket = (DSTC_BADACTION*)pMsg;
	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);
	if (pUser)
		pUser->m_bBad = (BOOL)pPacket->bBad;
}

void CmdFinishMapLoading( char* pMsg, DWORD dwLen )
{
	DSTC_FINISH_MAPLOADING* pPacket = (DSTC_FINISH_MAPLOADING*)pMsg;

	CUser * pReadyUser = g_pUserHash->GetData(pPacket->dwUserIndex);
	if ( !pReadyUser )
	{
		return;	
	}

	if (pReadyUser->m_bMatching )
	{
		char szBuf[255]={0,};
		wsprintf(szBuf, g_Message[ETC_MESSAGE537].szMessage, pReadyUser->m_szName); // "%s ���� ��� �غ� �Ǿ����ϴ�."
		DisplayMessageAdd(szBuf, 0xFF00FF1E);
	}
	
	// seven
	g_helpLoadingSprite.ReleaseSprites();
	
	// ����� �ƹ��͵� ���ڷ� �ȳ־ �ȴ�.
	SetListener(NULL);
}

void CmdAccEntranceAcquisition( char* pMsg, DWORD dwLen )
{
	DSTC_ACCENTRANCE_ACQUISITION* pPacket = (DSTC_ACCENTRANCE_ACQUISITION*)pMsg;
	g_pMainPlayer->m_dwMoney = pPacket->dwMoney;
}

void CmdUsedPotion( char* pMsg, DWORD dwLen )
{
	DSTC_DUNGEON_USED_POTION* pPacket = (DSTC_DUNGEON_USED_POTION*)pMsg;
	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);
	if (!pUser)	return;

	EffectDesc* pEffectDesc = g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(EFFECT_HPPOTION), g_pMainPlayer == pUser, __CHR_EFFECT_NONE__);
	
	switch(pPacket->bType)
	{
	case 0:	// ����
		pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_ALPHA] = 100;
		break;
	case 1: // �̵��
		pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_ALPHA] = 150;
		break;
	case 2: // hp ����
		pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_ALPHA] = 200;
		break;
	case 3: // ���� hp ����
		pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_ALPHA] = 220;
		break;
	case 4: // ����Ʈ hp ����
		pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_ALPHA] = 240;
		break;
	case 16: // ���ݷ�
		pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_ALPHA] = 255;
		break;
	default:
		pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_ALPHA] = 255;
		break;
	}

	ShowObject(pEffectDesc->hEffect.pHandle);
	::SetAction(pEffectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE);
	pEffectDesc->hEffect.pDesc->ObjectFunc = EffectPotionAroundRemoveFunc;
	pEffectDesc->dwTargetIndex[0] = pUser->m_dwUserIndex;
	pEffectDesc->byTargetObjectType[0] = OBJECT_TYPE_PLAYER;
	g_pExecutive->SetAlphaFlag(pEffectDesc->hEffect.pHandle, pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_ALPHA]);
	
	// Play Sound for Using Potion
	// by minjin. 2004. 10. 29.
	// pUser �� MainPlayer ���� �˻��ϴ� �Լ��� �߰������� ���ڴµ�..
	// ��� �߰��ϴ°� ��������.. ;;;
	if ( pUser->m_dwUserIndex == g_pMainPlayer->m_dwUserIndex )
	{
		// ������ ���� ������ �Ҹ��� �鸮�� �Ѵ�. �ٸ���� �������� �鸮�� �Ž����ܴ�.. -_-;;;
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_HPRECOVER, g_pMainPlayer->m_v3CurPos, FALSE);
	}

}

void CmdMoveMonster( char* pMsg, DWORD dwLen )
{
	DSTC_MOVE_MON* pMoveMon = (DSTC_MOVE_MON*)pMsg;
	//MAP_TILE*	pTile;

	CMonster*	pMonster = g_pMonsterHash->GetData( pMoveMon->dwMonsterIndex );
	if( !pMonster ) 
	{
#ifdef DEVELOP_MODE
		LOGOUT( g_Message[ETC_MESSAGE72].szMessage, pMoveMon->dwMonsterIndex );	// MSG_ID : 72 ; MonsterMove Error! MonsterID : %d
#endif
		RequestDungeonInfo( 2, pMoveMon->dwMonsterIndex );
		return;
	}
//	if( !pMonster->m_dwHP )	return;
	
//	LOGOUT( "MonsterMove  : %d, %u, %u, %u, %u", pMoveMon->dwMonsterIndex, pMoveMon->v3MoveStartPos.x, pMoveMon->v3MoveStartPos.z,
//		pMoveMon->v3MoveStopPos.x, pMoveMon->v3MoveStopPos.y );
	// �̵� ������ �Է��ϰ�
	
	VECTOR3 v3MoveStartPos;
	VECTOR2_TO_VECTOR3(pMoveMon->v2MoveStartPos, v3MoveStartPos)
	SetMonsterPosition( pMonster, &v3MoveStartPos );
	VECTOR2_TO_VECTOR3(pMoveMon->v2MoveStopPos, pMonster->m_v3DestPos)
	VECTOR2_TO_VECTOR3(pMoveMon->v2MoveDirection, pMonster->m_v3Direction)
	pMonster->m_fSpeed		= pMoveMon->fSpeed;
	pMonster->m_hMonster.pDesc->dwStartTick	= g_dwCurTick;
	
	DWORD dwMotionIndex	= g_pExecutive->GXOGetCurrentMotionIndex( pMonster->m_hMonster.pHandle );
	DWORD dwMotionFrame = g_pExecutive->GetCurrentGameFrame();
	
	switch(dwMotionIndex)
	{	
	case MON_MOTION_TYPE_MOVE1:	pMonster->SetAction( MON_MOTION_TYPE_MOVE1, dwMotionFrame ); 	break;	//�ȱ� 
	case MON_MOTION_TYPE_MOVE2: pMonster->SetAction( MON_MOTION_TYPE_MOVE2, dwMotionFrame ); 	break;	//�ٱ� 
	default:
		{
			if(pMoveMon->bMoveType == UNIT_STATUS_WALKING)
				pMonster->SetAction( MON_MOTION_TYPE_MOVE1);	//�ȱ� 	
			else
				pMonster->SetAction( MON_MOTION_TYPE_MOVE2);	//�ٱ� 
		}
		break;
	}

/*
	if (dwMotionIndex == MON_MOTION_TYPE_MOVE1)
		pMonster->SetAction( MON_MOTION_TYPE_MOVE1, dwMotionFrame );
	else
		pMonster->SetAction( MON_MOTION_TYPE_MOVE1);
*/	
	pMonster->SetStatus(pMoveMon->bMoveType, MonsterMoveFunc);

	VECTOR3_SUB_VECTOR3( &pMonster->m_v3Direction, &pMonster->m_v3DestPos, &pMonster->m_v3CurPos );
	if (pMonster->m_v3Direction.x ||
		pMonster->m_v3Direction.y || 
		pMonster->m_v3Direction.z)
	{
		Normalize( &pMonster->m_v3Direction, &pMonster->m_v3Direction );
	}
	else
	{
		pMonster->m_v3Direction.z = -1; 
	}
	g_pExecutive->GXOSetDirection( pMonster->m_hMonster.pHandle, &g_Camera.v3AxsiY, (float)(atan2(pMonster->m_v3Direction.z, pMonster->m_v3Direction.x) + DEG90 ) );

	// seven �켱 �ȱ� Ÿ���������� ������������ �̰ɷ� ��ü 
	// �� �����ϰ� �ȱ� �Ҷ� �Ҹ��� ���ش�.
	if( GetRandom(2) )
	{	
//		PlaySoundEffect(pMonster->m_pSound[MONSTER_SOUND_NEUTRAL], &pMonster->m_v3CurPos, FALSE);
//		_PlaySound( MONSTER_SOUND_NEUTRAL, SOUND_TYPE_MONSTER, pMonster->wNeu );
	}		
}
