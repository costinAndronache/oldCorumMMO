/////////////////////////////////////
//
//		 yakolla
//
/////////////////////////////////////
#include "stdafx.h"
#include "effect.h"
#include "EffectProcess.h"
#include "GameControl.h"
#include "DungeonProcess.h"
#include "CorumResource.h"
#include "DefResource.h"
#include "GameControl.h"
#include "HashTable.h"
#include "CodeFun.h"
#include "ObjectManager.h"
#include "InitGame.h"
#include "GameMenuWnd.h"
#include "DSMiniMap.h"
#include "UserInterface.h"
#include "message.h"
#include "InitGame.h"
#include "PlayerShopWnd.h"
#include "Chat.h"
#include "DungeonTable.h"
#include "ItemUpgrade.h"
#include "CharDieWnd.h"
#include "ItemWnd.h"
#include "NetworkClient.h"
#include "Map.h"
#include "ChrInfo.h"
#include "SoundControl.h"
#include "special_effect.h"


float	g_fWind	= 0.0f;


void CreateDamageEffect( BYTE bObjectType, DWORD dwIndex )
{	
	BYTE	bOwn = 0;
	VECTOR3 vec;
	WORD	bAction = 1;
	DWORD	dwResource = SKILL_DAMAGE1;

	switch(bObjectType)
	{
	case OBJECT_TYPE_MAINPLAYER:
	case OBJECT_TYPE_PLAYER:
		{
			CUser* pUser = g_pUserHash->GetData(dwIndex);
			if (!pUser)		return;
			vec = pUser->m_v3CurPos;
			
			if (bObjectType == OBJECT_TYPE_MAINPLAYER)
				bOwn = 1;			
		}break;

	case OBJECT_TYPE_MONSTER:
		{
			CMonster* pMonster = g_pMonsterHash->GetData(dwIndex);
			if (!pMonster)	return;
			vec = pMonster->m_v3CurPos;
			dwResource = SKILL_DAMAGE2;
			bAction = GetRandom(4)+1;
		}break;
	}

	EffectDesc*	pEffectDesc =  g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(dwResource), bOwn, __CHR_EFFECT_NONE__);
	pEffectDesc->byTargetObjectType[0]	= bObjectType;
	pEffectDesc->dwTargetIndex[0]		= dwIndex;

	GXSetPosition( pEffectDesc->hEffect.pHandle, &vec, FALSE );
	pEffectDesc->vecBasePosion = vec;

	::SetAction( pEffectDesc->hEffect.pHandle, bAction, 0, ACTION_ONCE );
	pEffectDesc->hEffect.pDesc->ObjectFunc	= EffectOnceAndNormal;
	g_pEffectLayer->AttachLight(pEffectDesc, 7, LightTimeOff);
	pEffectDesc->m_sLightDescEx.m_nDestTick = g_dwCurTick+50;	
}

void EffectSkillStartFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{// ����Ʈ ������ Ÿ�̹� �����ϴ� ���.	
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc*	pEffectDesc = (EffectDesc*)pObjDesc->pInfo;

	if (!pEffectDesc->dwFrameCount)
	{
		if (pEffectDesc->dwOwnType == OBJECT_TYPE_PLAYER)
		{
			// �������� ���ֶ�.
			CUser* pUser = g_pUserHash->GetData(pEffectDesc->dwOwnIndex);
			if (pUser)
				pEffectDesc->dwFrameFinish  = GetSkillStartActionFinishCount(pEffectDesc->pEffect->bID, pUser, pEffectDesc->dwOwnType);
		}
	}
	
	if( pEffectDesc->dwFrameFinish == pEffectDesc->dwFrameCount++ )
	{
		// ��ų �� Ÿ�̹�.
		VECTOR3 vecStart = pEffectDesc->vecBasePosion;

		if (pEffectDesc->dwOwnType == OBJECT_TYPE_PLAYER)
		{
			// �������� ���ֶ�.
			CUser* pUser = g_pUserHash->GetData(pEffectDesc->dwOwnIndex);
			if (pUser)
			{
				pUser->DeleteMagicArray();							
			}			
		}
		
		pEffectDesc->dwFrameCount = 0;
		if (g_pEffectLayer->IsEffectShow(pEffectDesc))
			g_pExecutive->EnableRender(handle);

		switch(pEffectDesc->pEffect->bID)
		{
		case __SKILL_FIREMISSILE__:
		case __SKILL_DETONATION__:
			
		case __SKILL_ICEMISSILE__:
		case __SKILL_SOULBLASTER:
		case __SKILL_ROUNDRANGE__:
		case __SKILL_FLASHSHOCK__:
		//�迵�� �߻� 
		//case __SKILL_POSIONCLOUD__:
			{
				::SetAction( handle, 1, 0, ACTION_LOOP );
				if (pEffectDesc->dwSkillLevel+1 >= MAX_SKILL_LEVEL/2)
					pObjDesc->ObjectFunc = FireBallHeadFunctionBySlowboatEightDirection;
				else
					pObjDesc->ObjectFunc = pEffectDesc->pEffect->SkillFunc;
				
			}break;

		case __SKILL_CHAINATTACKER__:
			{
				::SetAction( handle, 1, 0, ACTION_LOOP );
				pObjDesc->ObjectFunc = pEffectDesc->pEffect->SkillFunc;
			}break;

		case __SKILL_BLASTQUAKE__:
			{
				::SetAction( handle, 1, 0, ACTION_ONCE );				
				
				int nMin[] = {1, 2, 4, 7, 11, 16, 22, 29, 37, 46, 56, 67, 79, 92, 106, 121, 137, 154, 172, 191};
				int nMax[] = {1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78, 91, 105, 120, 136, 153, 171, 190};
				int nIndex = pEffectDesc->dwTemp[SKILL_TEMP_CREATECOUNT]+1;

				for(int i = 0; i < sizeof(nMin)/sizeof(nMin[0]); ++i)
				{
					if (nMin[i] <= nIndex && nMax[i] >= nIndex)
					{
						pEffectDesc->dwFrameCount = i*4+1 ;
						float fradian = (nIndex-nMin[i])*DEG180/(i+1)-DEG45;
						vecStart.x += cosf(pEffectDesc->f_Radian+fradian)*TILE_SIZE/2*(i+1);
						vecStart.z += sinf(pEffectDesc->f_Radian+fradian)*TILE_SIZE/2*(i+1);
						break;
					}
				}
				
				pObjDesc->ObjectFunc = pEffectDesc->pEffect->SkillFunc;
				g_pExecutive->DisableRender(handle);
			}break;

		case __SKILL_MAGMAWALL__:
			{
				::SetAction( handle, 1, 0, ACTION_ONCE );
				
				pEffectDesc->dwFrameCount = 1 + 5*pEffectDesc->dwTemp[SKILL_TEMP_CREATECOUNT]/5;
				
				// ������ �ʹ� ����� ������� ��.��
				int nIndex = (pEffectDesc->dwTemp[SKILL_TEMP_CREATECOUNT]/pEffectDesc->pEffect->Value[pEffectDesc->dwSkillLevel].nCompass);
				if (nIndex%2)
				{
					vecStart.x += cosf(pEffectDesc->f_Radcount)*TILE_SIZE*nIndex/2;
					vecStart.z += sinf(pEffectDesc->f_Radcount)*TILE_SIZE*nIndex/2;
				}
				else
				{
					vecStart.x -= cosf(pEffectDesc->f_Radcount)*TILE_SIZE*nIndex/2;
					vecStart.z -= sinf(pEffectDesc->f_Radcount)*TILE_SIZE*nIndex/2;
				}
				
				vecStart.x += cosf(pEffectDesc->f_Radian)*TILE_SIZE*(pEffectDesc->dwTemp[SKILL_TEMP_CREATECOUNT]%pEffectDesc->pEffect->Value[pEffectDesc->dwSkillLevel].nCompass);
				vecStart.z += sinf(pEffectDesc->f_Radian)*TILE_SIZE*(pEffectDesc->dwTemp[SKILL_TEMP_CREATECOUNT]%pEffectDesc->pEffect->Value[pEffectDesc->dwSkillLevel].nCompass);
				pObjDesc->ObjectFunc = pEffectDesc->pEffect->SkillFunc;
				g_pExecutive->DisableRender(handle);				
			}break;

		case __SKILL_ICEWALL__:
			{
				::SetAction( handle, 1, 0, ACTION_ONCE );
				
				pEffectDesc->dwFrameCount = pEffectDesc->dwTemp[SKILL_TEMP_CREATECOUNT]/9*8+1 ;
				VECTOR2 array[] = 
				{
					{1.0, -1.0},{1.0, 0.0},{1.0, 1.0},
					{0.0, 1.0},	{-1.0, 1.0},{-1.0, 0.0},
					{-1.0, -1.0},{0.0, -1.0},
					{0.0, 0.0}
				};

				int arrayIndex = 8;
				if (pEffectDesc->dwTemp[SKILL_TEMP_CREATECOUNT])
				{
					arrayIndex = (pEffectDesc->dwTemp[SKILL_TEMP_CREATECOUNT])%8;
				
					float costemp = array[arrayIndex].x*TILE_SIZE;
					float sintemp = array[arrayIndex].y*TILE_SIZE;
					vecStart.x += costemp*((pEffectDesc->dwTemp[SKILL_TEMP_CREATECOUNT]-1)/8+1);
					vecStart.z += sintemp*((pEffectDesc->dwTemp[SKILL_TEMP_CREATECOUNT]-1)/8+1);
				}
				
				pObjDesc->ObjectFunc = pEffectDesc->pEffect->SkillFunc;
				g_pExecutive->DisableRender(handle);
			}break;

		case __SKILL_DRAGONICFIREBLAST__:
			{
				CMonster* pMonster = g_pMonsterHash->GetData(pEffectDesc->dwOwnIndex);
				if (!pMonster)	goto lbl_remove;

				VECTOR3 vAxis;
				float fRadian=0.f;
				g_pExecutive->GXOGetDirection(pMonster->m_hMonster.pHandle, &vAxis, &fRadian);
				
				vecStart.z = pMonster->m_v3CurPos.z+sinf(fRadian-DEG90) * 600.f;
				vecStart.x = pMonster->m_v3CurPos.x+cosf(fRadian-DEG90) * 600.f;
				
				::SetAction(handle, 1, 0, ACTION_ONCE);
				pObjDesc->ObjectFunc = DragonicFireblast;
			}break;

		default:
			{			
				::SetAction(handle, 1, 0, ACTION_ONCE);
				pObjDesc->ObjectFunc = pEffectDesc->pEffect->SkillFunc;
			}
		}
		
		g_pExecutive->EnableSchedule(handle);
		GXSetPosition( handle, &vecStart, FALSE );
		pEffectDesc->vecBasePosion = vecStart;		
	}

	return;

lbl_remove:
	RemoveEffectDesc(pEffectDesc);
}

//-----------------------------------------------------------------------------
// File: PlayerSkillFunc()
// Desc: [ObjectFuncion]
//		 �÷��̾��� ���.
//-----------------------------------------------------------------------------
void PlayerSkillFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;			// ���� ���� ������.	
	CUser*	pUser			= (CUser*)pData->pInfo;
//	Effect* pEffect			= g_pEffectLayer->GetEffectInfo(pUser->m_bSkillTmp); // �̳� �Ⱦ��� �� ����.
	
	if( pUser->GetStatus() == UNIT_STATUS_SKILL 
		&& (bFrameFlag == FRAME_FLAG_CHANGE_NEXT 
			|| bFrameFlag == FRAME_FLAG_FINISHED) )
	{
		::SetAction(pUser->m_hPlayer.pHandle, MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );
		pUser->SetStatus(UNIT_STATUS_NORMAL);
		pObjDesc->ObjectFunc		= NULL;
	}
}

void MonsterSkillFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;			// ���� ���� ������.	
	CMonster*	 pMonster	= (CMonster*)pData->pInfo;
//	Effect*		 pEffect	= g_pEffectLayer->GetEffectInfo(pMonster->m_bSkillTmp);
	
	if( pMonster->GetStatus() == UNIT_STATUS_SKILL 
		&& (bFrameFlag == FRAME_FLAG_CHANGE_NEXT 
			|| bFrameFlag == FRAME_FLAG_FINISHED) )
	{
		::SetAction(pMonster->m_hMonster.pHandle, MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );
		pMonster->SetStatus(UNIT_STATUS_NORMAL, NULL);
		pObjDesc->ObjectFunc		= NULL;
	}	
}

// ����Ʈ �¾������� ���� ��ƾ.
void EffectSubCommonProcess(EffectDesc* pEffectDesc)
{
	for(DWORD i = 0; i < pEffectDesc->dwCount; ++i)
	{	
		switch( pEffectDesc->byTargetObjectType[i] )
		{
		case OBJECT_TYPE_MONSTER:
			{
				CMonster*	pMonster = g_pMonsterHash->GetData( pEffectDesc->dwTargetIndex[i] );
				if( !pMonster ) break;
				
				CUser* pUser = NULL;
				if (OBJECT_TYPE_MONSTER == pEffectDesc->dwOwnType)
				{
					CMonster* pOwnMonster = g_pMonsterHash->GetData(pEffectDesc->dwOwnIndex);
					if (pOwnMonster)
					{
						pUser = g_pUserHash->GetData(pOwnMonster->m_dwLordIndex);
					}
				}
				else if (OBJECT_TYPE_PLAYER == pEffectDesc->dwOwnType)
				{
					pUser = g_pUserHash->GetData(pEffectDesc->dwOwnIndex);
				}
				
				if (pUser == g_pMainPlayer)
				{
					if( pMonster->m_dwTemp[MONSTER_TEMP_DAMAGE_TYPE] == 4 )	// ������ �����ߴٸ�.
					{
						// sung-han 2005-01-13 íũ���ͽ��÷��� ����� ǥ�õ��� �ʴ� �κ� ����
						//------------------------------------------------------------
						if (pEffectDesc->pEffect->bFormula == 22	||
							pEffectDesc->pEffect->bFormula == 2		||
							pEffectDesc->pEffect->bFormula == 1		||
							pEffectDesc->pEffect->bFormula == 20)
						/*
						if (pEffectDesc->pEffect->bFormula == 22	||
							pEffectDesc->pEffect->bFormula == 2		||
							pEffectDesc->pEffect->bFormula == 1)
						*/
						//------------------------------------------------------------
						{
							pMonster->SetDamageIndex( pMonster->m_dwTemp[MONSTER_TEMP_DAMAGE] );
						}
					}
					else
					{
						pMonster->SetDamageChar( pMonster->m_dwTemp[MONSTER_TEMP_DAMAGE_TYPE]-1 );
					}
				}				

				if( pMonster->m_dwHP != 0 )
				{
					if(pMonster->GetStatus() == UNIT_STATUS_NORMAL)
					{
						::SetActionNext( pMonster->m_hMonster.pHandle, MON_MOTION_TYPE_DEFENSEFAIL1, MON_MOTION_TYPE_STAND1, ACTION_LOOP, 0 );
						
						pMonster->SetStatus( UNIT_STATUS_DAMAGING, pEffectDesc->pEffect->MonsterDamageFunc);
						pMonster->WithActionFunc = pEffectDesc->pEffect->MonsterSKillDamageWithAction;
						CUserInterface::GetInstance()->OpenMonsterBar(pMonster->m_szName, pMonster->m_dwHP, pMonster->m_dwMaxHP);
					}
					
					if(CUserInterface::GetInstance()->m_bGuardian==TRUE)
					{
						if(pMonster)
						{
							if(pMonster->m_dwHP>0)
							{
								if(g_pMainPlayer->m_dwUserIndex==pMonster->m_dwLordIndex)
								{
									CUserInterface* pUserInterface = CUserInterface::GetInstance();

									pUserInterface->SetGauge(pMonster->m_dwHP, pMonster->m_dwMaxHP);									
								}
							}
						}
					}
				}
				else
				{
					pMonster->SetAction(MON_MOTION_TYPE_DOWN, 0, ACTION_ONCE);
					pMonster->SetStatus(UNIT_STATUS_DEAD, NULL);// �¿��� �Ҷ� ������ ��ŵó�� ���� ������..
					
					pMonster->SetStatus(UNIT_STATUS_DEAD, pEffectDesc->pEffect->MonsterKillFunc);
					pMonster->WithActionFunc = pEffectDesc->pEffect->MonsterKillWithAction;
				}
			}
			break;

		case OBJECT_TYPE_PLAYER:
		case OBJECT_TYPE_MAINPLAYER:
			{
				CUser*	pUser = g_pUserHash->GetData( pEffectDesc->dwTargetIndex[i] );
				if( !pUser )
				{
					break;
				}
				
				if( pUser->m_dwTemp[USER_TEMP_DAMAGE_TYPE] == 4 )	// ������ �����ߴٸ�.
				{
					// sung-han 2005-01-13 íũ���ͽ��÷��� ����� ǥ�õ��� �ʴ� �κ� ����
					//------------------------------------------------------------
					if (pEffectDesc->pEffect->bFormula == 2 || 
						pEffectDesc->pEffect->bFormula == 1 ||		// Comment by jin wouk
						pEffectDesc->pEffect->bFormula == 22||		// 1:1 ������ ������ �ѹ� ������ �ʾƼ� �߰���.
						pEffectDesc->pEffect->bFormula == 20 )
					/*
					if (pEffectDesc->pEffect->bFormula == 2 || 
						pEffectDesc->pEffect->bFormula == 1 ||		// Comment by jin wouk
						pEffectDesc->pEffect->bFormula == 22 )		// 1:1 ������ ������ �ѹ� ������ �ʾƼ� �߰���.
					*/
					//------------------------------------------------------------
					{
						pUser->SetDamageIndex( pUser->m_dwTemp[USER_TEMP_DAMAGE] );
					}
				}
				else
				{
					pUser->SetDamageChar( pUser->m_dwTemp[USER_TEMP_DAMAGE_TYPE]-1 );
				}

				if (pUser->m_dwTemp[USER_TEMP_DAMAGE_TYPE] == 4 && pEffectDesc->pEffect->bID == __SKILL_TREEAGION__)
				{
					// ������ ��Ƴ��� �ض�.
					pUser->SetAction(MOTION_TYPE_WARSTAND, 0, ACTION_LOOP);
					pUser->SetStatus(UNIT_STATUS_NORMAL, TRUE);
					pUser->m_hPlayer.pDesc->ObjectFunc = NULL;
					pUser->m_v3CurPos.y = 0;
					CCharDieWnd::GetInstance()->SetActive(FALSE);					
					
					g_pExecutive->SetAlphaFlag( pUser->m_hPlayer.pHandle, 255 );
					GXSetPosition(pUser->m_hPlayer.pHandle, &pUser->m_v3CurPos, FALSE );					
				}
				else
				{
					if (pUser->GetStatus()==UNIT_STATUS_DEAD)
					{
						pUser->SetStatus(UNIT_STATUS_DEAD);
					}
				}
			}
			break;					
		}
	}
}

// �ش� ������ ���� ���ĺ����� ���ش�.
void EffectAlphaFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc*	pEffectDesc = (EffectDesc*)pObjDesc->pInfo;

	++pEffectDesc->dwFrameCount;

	BYTE byAlpha = 0XFF-0XFF*float(pEffectDesc->dwFrameCount)/pEffectDesc->dwFrameFinish;
	g_pExecutive->SetAlphaFlag(handle, byAlpha);

	if (!byAlpha)
		RemoveEffectDesc(pEffectDesc);
}

// �ش� ������ ���� 0���� �������� ��������.
void EffectFallingFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc*	pEffectDesc = (EffectDesc*)pObjDesc->pInfo;
	float fPower = 35;	
		
	if (pEffectDesc->vecBasePosion.y > 10)
	{
		pEffectDesc->vecBasePosion.x += cos(pEffectDesc->f_Radian)*fPower;
		pEffectDesc->vecBasePosion.z += sin(pEffectDesc->f_Radian)*fPower;
		pEffectDesc->vecBasePosion.y -= fPower;
	}
	else
	{
		if (++pEffectDesc->dwFrameCount == pEffectDesc->dwTemp[SKILL_TEMP_HOAMINGRANDOM])
			goto lbl_remove;		
	}

	GXSetPosition(handle, &pEffectDesc->vecBasePosion, FALSE);	

	if (pEffectDesc->dwFrameCount > 300)
		goto lbl_remove;

	return;

lbl_remove:
	pEffectDesc->dwFrameCount = 0;
	MOTION_DESC		MotionDesc;
	g_pExecutive->GXOGetMotionDesc(pEffectDesc->hEffect.pHandle, &MotionDesc, 1,0);
	pEffectDesc->dwFrameFinish = MotionDesc.dwLastFrame;
	pEffectDesc->hEffect.pDesc->ObjectFunc = EffectAlphaFunc;
}


// ��ų - �������.
void EffectChainAttackerFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc*	pEffectDesc = (EffectDesc*)pObjDesc->pInfo;
	
	VECTOR3 vecDest = {0,}, vPos = {0,};
	float fRad = 0.f;

	if (pEffectDesc->byTargetObjectType[0] == OBJECT_TYPE_MONSTER)
	{
		CMonster* pMonster = g_pMonsterHash->GetData(pEffectDesc->dwTargetIndex[0]);
		if (!pMonster)	goto lbl_remove;

		vecDest = pMonster->m_v3CurPos;
	}
	else if (pEffectDesc->byTargetObjectType[0] == OBJECT_TYPE_PLAYER)
	{
		CUser* pUser = g_pUserHash->GetData(pEffectDesc->dwTargetIndex[0]);
		if (!pUser)	goto lbl_remove;

		vecDest = pUser->m_v3CurPos;
	}
	
	{
		if (pEffectDesc->dwFrameCount == 0)
		{
			pEffectDesc->f_Radcount = CalcDistance( &vecDest, &pEffectDesc->vecBasePosion );
			pEffectDesc->f_Radcount = pEffectDesc->f_Radcount < 0 ? -pEffectDesc->f_Radcount : pEffectDesc->f_Radcount;
			VECTOR3 v3DirMon;
			VECTOR3_SUB_VECTOR3( &v3DirMon, &pEffectDesc->vecBasePosion, &vecDest );
			
			pEffectDesc->f_Radian = (float)(atan2(v3DirMon.z, v3DirMon.x)+3.14 );
			g_pExecutive->GXOSetDirection( pEffectDesc->hEffect.pHandle, &g_Camera.v3AxsiY,  pEffectDesc->f_Radian);
		}
		pEffectDesc->dwFrameCount++;		
		
		fRad = pEffectDesc->f_Radian;		
		
		float fPower = 35;
		pEffectDesc->vecBasePosion.x += cos(fRad)*fPower;
		pEffectDesc->vecBasePosion.z += sin(fRad)*fPower;
		if (pEffectDesc->vecBasePosion.y > 10)
			pEffectDesc->vecBasePosion.y -= fPower;
		
		GXSetPosition(handle, &pEffectDesc->vecBasePosion, FALSE);
		vPos = pEffectDesc->vecBasePosion;		
		
		// �Ÿ��� ��������� �浹!
		if (vecDest.y > -10 && sqrt(pow(vecDest.z - vPos.z, 2)+pow(vecDest.x - vPos.x, 2)) < 100)
		{
			// ���� �̹��� ���
			EffectDesc* effectDesc = g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(EFFECT_CHAIN_EXPLODE), pEffectDesc->bOwn, __CHR_EFFECT_NONE__);
			vPos.y = 0;
			GXSetPosition( effectDesc->hEffect.pHandle, &vPos, FALSE);
			::SetAction(effectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE);
			effectDesc->dwFrameCount = 0;
			MOTION_DESC		MotionDesc;
			g_pExecutive->GXOGetMotionDesc(effectDesc->hEffect.pHandle, &MotionDesc, 1,0);
			effectDesc->dwFrameFinish = MotionDesc.dwLastFrame*2;
			effectDesc->hEffect.pDesc->ObjectFunc = EffectAlphaFunc;
			
			// ���� ƨ���� ���� ����.
			if (pEffectDesc->byTargetObjectType[0] == OBJECT_TYPE_MONSTER)
			{
				VECTOR3	v3DirMon;
				VECTOR3_SUB_VECTOR3( &v3DirMon, &vPos, &vecDest);
				float fRad = (float)atan2(v3DirMon.z, v3DirMon.x);
				CMonster* pMonster = 	g_pMonsterHash->GetData(pEffectDesc->dwTargetIndex[0]); 
				pMonster->m_vecTemp = vecDest;
				pMonster->m_radian = fRad+3.14;
			
			}
			EffectSubCommonProcess(pEffectDesc);
			pEffectDesc->hEffect.pDesc->ObjectFunc = EffectFallingFunc;
			pEffectDesc->dwFrameCount = 0;
			pEffectDesc->dwTemp[SKILL_TEMP_HOAMINGRANDOM] = 90;
		}	
	}

	return;

lbl_remove:
	RemoveEffectDesc(pEffectDesc);
}

// ����� �׾����� ��Ÿ���� ����Ʈ ����𿡼� ���� ���γ𿡰� ����Ʈ�� �̵��Ѵ�.
void EffectGuardianKillFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc*	pEffectDesc = (EffectDesc*)pObjDesc->pInfo;

	if( bFrameFlag == FRAME_FLAG_FINISHED )
		goto lbl_remove;
	else
	{		
		if (g_pChrInfoEffect->GetFrameInfo(pEffectDesc->wChrNum, 0, 1, _CHRINFO_FRAME0) > dwCurFrame)
		{
			// ù��° ������ ������ �ִٰ�
			if (pEffectDesc->dwOwnType == OBJECT_TYPE_MONSTER)
			{
				CMonster* pMonster = g_pMonsterHash->GetData(pEffectDesc->dwOwnIndex);
				if (pMonster)
				{
					VECTOR3 vec = pMonster->m_v3CurPos;
					vec.y = 0;
					GXSetPosition(handle, &vec, FALSE);

					VECTOR3 vAfterPos;
					vAfterPos.x = vAfterPos.y = vAfterPos.z = 0;
						
					if (dwCurFrame % 2 == 0)
						vAfterPos.x += 10;
					else
						vAfterPos.x -= 10;
					
					CameraQuake(vAfterPos);						
				}				
			}
			else if(pEffectDesc->dwOwnType == OBJECT_TYPE_PLAYER)
			{
				CUser* pUser = g_pUserHash->GetData(pEffectDesc->dwOwnIndex);
				if (pUser)		GXSetPosition(handle, &pUser->m_v3CurPos, FALSE);				
			}
		}
		else 
		{				
			DWORD dwOwnIndex=0, dwOwnType=0;
			VECTOR3	vecDest = {0,};
			
			g_pExecutive->GXOGetPosition(handle, &pEffectDesc->vecBasePosion);
			float fRad;
			if (pEffectDesc->byTargetObjectType[0] == OBJECT_TYPE_MONSTER)
			{
				CMonster* pMonster = g_pMonsterHash->GetData(pEffectDesc->dwTargetIndex[0]);
				if (pMonster)
				{
					vecDest = pMonster->m_v3CurPos;
					dwOwnIndex = pMonster->m_dwMonsterIndex;
					dwOwnType = OBJECT_TYPE_MONSTER;
				}
				else			goto lbl_remove;
			}
			else if(pEffectDesc->byTargetObjectType[0] == OBJECT_TYPE_PLAYER)
			{
				CUser* pUser = g_pUserHash->GetData(pEffectDesc->dwTargetIndex[0]);
				if (pUser)
				{
					vecDest = pUser->m_v3CurPos;
					dwOwnIndex = pUser->m_dwUserIndex;
					dwOwnType = OBJECT_TYPE_PLAYER;
				}
				else			goto lbl_remove;
			}

			if (g_pChrInfoEffect->GetFrameInfo(pEffectDesc->wChrNum, 0, 1, _CHRINFO_FRAME1) > dwCurFrame)
			{
				// �ι�° ���ۺ��� �ش簴ü�� �����̱� �����ؼ�
				pEffectDesc->dwFrameCount++;
				SetHoamingDirection(&vecDest,&pEffectDesc->vecBasePosion,handle,0.3f,0,pEffectDesc);
				fRad = pEffectDesc->f_Radian;
				
				float fDist = fabs(vecDest.z - pEffectDesc->vecBasePosion.z) + fabs(vecDest.x - pEffectDesc->vecBasePosion.x);
				float fPower = fDist/(g_pChrInfoEffect->GetFrameInfo(pEffectDesc->wChrNum, 0, 1, _CHRINFO_FRAME0)-pEffectDesc->dwFrameCount);
				pEffectDesc->vecBasePosion.x += cos(fRad+DEG90+3.14)*fPower;
				pEffectDesc->vecBasePosion.z += sin(fRad+DEG90+3.14)*fPower;
				GXSetPosition(handle, &pEffectDesc->vecBasePosion, FALSE);				
			}
			else if (g_pChrInfoEffect->GetFrameInfo(pEffectDesc->wChrNum, 0, 1, _CHRINFO_FRAME1) == dwCurFrame)
			{				
				pEffectDesc->vecBasePosion.x = vecDest.x;
				pEffectDesc->vecBasePosion.y = 0;
				pEffectDesc->vecBasePosion.z = vecDest.z;
				GXSetPosition(handle, &pEffectDesc->vecBasePosion, FALSE);

				{
					// �����ڿ��� ������ ����Ʈ
					EffectDesc* pEffectDesc = g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(EFFECT_DUNGEON_CAPTURE), 1, __CHR_EFFECT_NONE__);
					pEffectDesc->hEffect.pDesc->ObjectFunc = EffectSkillUserAroundRemoveFunc;
					pEffectDesc->dwTargetIndex[0] = dwOwnIndex;
					pEffectDesc->byTargetObjectType[0] = dwOwnType;
					pEffectDesc->vecBasePosion = vecDest;
					GXSetPosition(pEffectDesc->hEffect.pHandle, &pEffectDesc->vecBasePosion, FALSE );
				} 
			}
		}		
	}

	return;

lbl_remove:
	RemoveEffectDesc(pEffectDesc);
}


void EffectEventDungeonFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc*	pEffectDesc = (EffectDesc*)pObjDesc->pInfo;

	CUser* pTargetUser = g_pUserHash->GetData(pEffectDesc->dwTargetIndex[0]);
	if (pTargetUser == NULL)
	{
		RemoveEffectDesc(pEffectDesc);
		return;
	}
	
	GXSetPosition( handle, &pTargetUser->m_v3CurPos, FALSE );
	
	if( bFrameFlag == FRAME_FLAG_FINISHED )
	{
		if(++pTargetUser->m_byEventEffectCount >= 3)
		{
			// ���ô��� 
			if(g_pThisDungeon->m_byEventType == EVENT_DUNGEON_TRAINING)
			{
				RemoveEffectDesc(pEffectDesc);
				
				if((CUser*)g_pMainPlayer == pTargetUser )
				{
					CTDS_EVENT_CLEAR_EFFECT_FINISH packet;
					g_pNet->SendMsg((char*)&packet,packet.GetPacketSize(),SERVER_INDEX_ZONE);
					pTargetUser->m_byEventEffectCount = 0;
				}													
			}
		}
	}
}


//������ ������ �������� ����Ʈ.
void EffectOnceAndRemoveFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{ 
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc*	pEffectDesc = (EffectDesc*)pObjDesc->pInfo;	
	
	switch(pEffectDesc->bEffectInfoNum)
	{
	case __SKILL_ZEAL__:
		break;
	case __SKILL_FIREMISSILE__:
	case __SKILL_DETONATION__:
	case __SKILL_ICEMISSILE__:
	case __SKILL_FLASHSHOCK__:
	case __SKILL_SOULBLASTER:
	// �迵�� �߻� 
	//case __SKILL_POSIONCLOUD__:
		{
			if (pEffectDesc->bJoint == 6 && g_pChrInfoEffect->GetFrameInfo(pEffectDesc->wChrNum, 0, 1, _CHRINFO_FRAME0) == dwCurFrame)
				PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_START], &pEffectDesc->vecBasePosion, FALSE);				
			
		}break;

	case __SKILL_THUNDERSTORM__:
	{
		VECTOR3	v3CameraAngleRad;
		// ī�޶� ���� �ٲ��� ���鸸 ����.
		g_pGeometry->GetCameraAngleRad( &v3CameraAngleRad, 0 );
		g_pExecutive->GXOSetDirection( pEffectDesc->hEffect.pHandle, &g_Camera.v3AxsiY,  -v3CameraAngleRad.y);
	}

	default:
		if (pEffectDesc->pEffect)
		{// ��ų�� �����°� �ƴϹǷ�.
			if (pEffectDesc->bJoint == 0 && 0 == dwCurFrame)
				PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_START], &pEffectDesc->vecBasePosion, FALSE);
			else if (pEffectDesc->bJoint == 0 && g_pChrInfoEffect->GetFrameInfo(pEffectDesc->wChrNum, 0, 1, _CHRINFO_FRAME0) == dwCurFrame)
				PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_MID], &pEffectDesc->vecBasePosion, FALSE);
			else if (pEffectDesc->bJoint == 0 && g_pChrInfoEffect->GetFrameInfo(pEffectDesc->wChrNum, 0, 1, _CHRINFO_FRAME1) == dwCurFrame)
				PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_END], &pEffectDesc->vecBasePosion, FALSE);
		}
	}	
	
	if (pEffectDesc->pEffect && g_pChrInfoEffect->GetFrameInfo(pEffectDesc->wChrNum, 0, 1, _CHRINFO_FRAME0) == dwCurFrame)
	{		
		EffectSubCommonProcess(pEffectDesc);		
	}

	if (pEffectDesc->pEffect && pEffectDesc->pEffect->bSkillType/10%10 == 0)
	{
		// ��ü ���� �ٳ�� �Ѵ�.
		switch(pEffectDesc->byTargetObjectType[0])
		{
		case OBJECT_TYPE_PLAYER:
			{
				CUser* pTargetUser = g_pUserHash->GetData(pEffectDesc->dwTargetIndex[0]);
				if (pTargetUser)
					GXSetPosition( handle, &pTargetUser->m_v3CurPos, FALSE );
				
			}break;

		case OBJECT_TYPE_MONSTER:
			{
				CMonster* pTargetMonster = g_pMonsterHash->GetData(pEffectDesc->dwTargetIndex[0]);
				if (pTargetMonster)
					GXSetPosition( handle, &pTargetMonster->m_v3CurPos, FALSE );
			}break;
		}		
	}

	if( bFrameFlag == FRAME_FLAG_FINISHED || bFrameFlag == FRAME_FLAG_CHANGE_NEXT )
	{
		if(pEffectDesc->bEffectInfoNum == __SKILL_EARTHQUAKE__ || pEffectDesc->bEffectInfoNum == __SKILL_DRAGONICVIBRATION__)
		{// ������ ������ ���ķ� ������� ���ؼ�.
		
			VECTOR3 vec;
			g_pExecutive->GXOGetPosition(handle, &vec);
			
			DWORD alpha_value = g_pExecutive->GetAlphaFlag(handle);
			if( alpha_value > 0 )
			{
				GXSetPosition( handle, &vec, FALSE );
				g_pExecutive->SetAlphaFlag( handle, alpha_value-5 );
				return;
			}
		}
		
		for(int i = 0; i < pEffectDesc->dwCount; ++i )
		{			
			if(pEffectDesc->pEffect->bType == TYPE_DRIVE || pEffectDesc->pEffect->bType == TYPE_TIMEZERO)
			{// ���� ����̺� �����̱�.
								
				switch(pEffectDesc->byTargetObjectType[i])
				{
				case OBJECT_TYPE_PLAYER:
					{
						CUser* pTargetUser = g_pUserHash->GetData(pEffectDesc->dwTargetIndex[i]);
						if (pTargetUser && pTargetUser->m_dwTemp[USER_TEMP_DAMAGE_TYPE] == 4) // ������������ �ٿ���.
						{
							if (pTargetUser->m_pEffectDesc[pEffectDesc->pEffect->bID])
							{
								if (g_pEffectLayer->IsEffectShow(pEffectDesc))
									ShowObject(pTargetUser->m_pEffectDesc[pEffectDesc->pEffect->bID]->hEffect.pHandle);
								else
									g_pExecutive->EnableSchedule(pTargetUser->m_pEffectDesc[pEffectDesc->pEffect->bID]->hEffect.pHandle);
								//pTargetUser->m_pEffectDesc[pEffectDesc->pEffect->bID]->dwTemp[SKILL_TEMP_POSONING] = pEffectDesc->dwTemp[SKILL_TEMP_POSONING];
							}
						}
					}break;

				case OBJECT_TYPE_MONSTER:
					{
						CMonster* pTargetMonster = g_pMonsterHash->GetData(pEffectDesc->dwTargetIndex[i]);
						if (pTargetMonster && pTargetMonster->m_dwTemp[MONSTER_TEMP_DAMAGE_TYPE] == 4) // ������������ �ٿ���.
						{
							if (pTargetMonster->m_pEffectDesc[pEffectDesc->pEffect->bID])
							{
								if (g_pEffectLayer->IsEffectShow(pEffectDesc))
									ShowObject(pTargetMonster->m_pEffectDesc[pEffectDesc->pEffect->bID]->hEffect.pHandle);
								else 
									g_pExecutive->EnableSchedule(pTargetMonster->m_pEffectDesc[pEffectDesc->pEffect->bID]->hEffect.pHandle);
								//pTargetMonster->m_pEffectDesc[pEffectDesc->pEffect->bID]->dwTemp[SKILL_TEMP_POSONING] = pEffectDesc->dwTemp[SKILL_TEMP_POSONING];
							}							
						}
					}break;
				}				
			}
		}
		
		RemoveEffectDesc(pEffectDesc);		
	}
	else
	{ // ī�޶� ��ģ���� ����
		EffectSkillCameraQuake(pEffectDesc, dwCurFrame);		
	}
}

void EffectSkillCameraQuake(EffectDesc* pEffectDesc, DWORD dwCurFrame)
{// ����Ʈ ī�޶� ���ó�� ��Ƴ����� ��.��;;;
	
	VECTOR3 v3Effect;g_pExecutive->GXOGetPosition(pEffectDesc->hEffect.pHandle, &v3Effect);		
			
	float fDist = CalcDistance( &g_pMainPlayer->m_v3CurPos, &v3Effect );
	if (fDist > 1000)	return;

	VECTOR3 vecQuake;vecQuake.x = vecQuake.y = vecQuake.z = 0;	

	switch(pEffectDesc->bEffectInfoNum)
	{
		// ��ų ���������� ����.
	case __SKILL_EARTHQUAKE__:
	case __SKILL_THUNDERSTORM__:
	case __SKILL_METEOR__:
	case __SKILL_DRAGONICVIBRATION__:	
		{						
			if (g_pChrInfoEffect->GetFrameInfo(pEffectDesc->wChrNum, 0, 1, _CHRINFO_FRAME0) <= dwCurFrame)
			{
				++pEffectDesc->dwTemp[SKILL_TEMP_CAMERA_FRAMECOUNT];
				float fPower = 1000/max(1.0, (fDist/100.));
				if (dwCurFrame % 2 == 0)
				{
					vecQuake.x = fPower/(pow(static_cast<int>(pEffectDesc->dwTemp[SKILL_TEMP_CAMERA_FRAMECOUNT]), 2)*0.5);
					vecQuake.x = min(vecQuake.x, 5);
				}
				else
				{
					vecQuake.x = -fPower/(pow(static_cast<int>(pEffectDesc->dwTemp[SKILL_TEMP_CAMERA_FRAMECOUNT]), 2)*0.5);
					vecQuake.x = max(vecQuake.x, -5);
				}				
			}
			
		}break;
		// ��� ����.
	case __SKILL_LIGHTNINGBREAK__:
	case __SKILL_DRAGONICFIREBLAST__:
		{
			if (g_pChrInfoEffect->GetFrameInfo(pEffectDesc->wChrNum, 0, 1, _CHRINFO_FRAME0) <= dwCurFrame)
			{
				if (++pEffectDesc->dwTemp[SKILL_TEMP_CAMERA_FRAMECOUNT] < 5)
				{				
					float fPower = 1000/max(1.0, (fDist/100.));
					if (dwCurFrame % 2 == 0)
					{
						vecQuake.x = fPower/(pow(static_cast<int>(pEffectDesc->dwTemp[SKILL_TEMP_CAMERA_FRAMECOUNT]), 2)*0.5);
						vecQuake.x = min(vecQuake.x, 10);
					}
					else
					{
						vecQuake.x = -fPower/(pow(static_cast<int>(pEffectDesc->dwTemp[SKILL_TEMP_CAMERA_FRAMECOUNT]), 2)*0.5);
						vecQuake.x = max(vecQuake.x, -10);
					}					
				}
				
			}
			
		}break;
	}

	CameraQuake(vecQuake);
}

void CameraQuake(VECTOR3& vecQuake)
{	
	VECTOR3 vAfterPos = g_pMainPlayer->m_v3CurPos;

	vAfterPos.x += g_Camera.fCameraDistance_x+vecQuake.x;
	vAfterPos.y += g_Camera.fCameraDistance_y+vecQuake.y;
	vAfterPos.z += g_Camera.fCameraDistance_z+vecQuake.z;
	
	g_pGeometry->SetCameraPos(&vAfterPos, 0 );

	g_Camera.iCameraMoveOption = !(int(vecQuake.x) || int(vecQuake.y) || int(vecQuake.z));	
}

void PlayerAttackFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;			// ���� ���� ������.	
	CUser*		pUser		= (CUser*)pData->pInfo;

	//if( pUser->GetStatus()== PC_STATUS_ATTACKING )
	{
		if ( 0 == dwCurFrame)
		{ // ���� �Ҹ� 
			_PlaySound( CHARACTER_SOUND_ATTACK, SOUND_TYPE_CHARACTER, CHARACTER_SOUND_ATTACK + ( pUser->m_wClass - 1 ) * SOUND_PER_CHARACTER, pUser->m_v3CurPos, FALSE );
		}
		
		// Ÿ���� �÷���.
		if( g_pChrInfoUser->GetFrameInfo(pUser->m_wClass-1, pUser->m_byItemType, pUser->m_wAttackType, _CHRINFO_FRAME1) == dwCurFrame)
		{
			DWORD dwWeapon = pUser->m_wHandR / 100+1;

			if(pUser->m_dwUserIndex==g_pMainPlayer->m_dwUserIndex)
			{
				if(g_pMainPlayer->m_bCurrnetHand==1)
					pUser->m_wHandR = g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND1].GetID()/100+1;
				else
					pUser->m_wHandR = g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND2].GetID()/100+1;

				dwWeapon = pUser->m_wHandR / 100+1;
			}
		
			if( pUser->m_wHandR == 0 )
			{	
				_PlaySound( 0, SOUND_TYPE_WEAPONATTACK, 0, pUser->m_v3CurPos, FALSE);	// �Ǽ�.
			}
			else
			{
				_PlaySound( 0, SOUND_TYPE_WEAPONATTACK, dwWeapon * SOUND_PER_WEAPON, pUser->m_v3CurPos, FALSE);
			}
		}

		if( g_pChrInfoUser->GetFrameInfo(pUser->m_wClass-1, pUser->m_byItemType, pUser->m_wAttackType, _CHRINFO_FRAME0) == dwCurFrame)
		{
			switch( pUser->m_dwTemp[ USER_TEMP_TARGET_TYPE ] )
			{
			case OBJECT_TYPE_MONSTER:
				{
					CMonster* pMonster = g_pMonsterHash->GetData( pUser->m_dwTemp[ USER_TEMP_TARGET_INDEX ] );
					if( !pMonster ) goto lb_exit;

					VECTOR3	v3DirMon;
					VECTOR3_SUB_VECTOR3( &v3DirMon, &pUser->m_v3CurPos, &pMonster->m_v3CurPos);
					float fRad = (float)atan2(v3DirMon.z, v3DirMon.x);
					pMonster->m_vecTemp = pMonster->m_v3CurPos;
					pMonster->m_radian = fRad+3.14;

					if( pMonster->m_dwHP )
					{	
						if (pMonster->GetStatus() == UNIT_STATUS_NORMAL)
						{
							// �Ƹ޸�ŸƮ�� ���� ������ ������� ���� ���� : �ִ��� 2005.2.14
							if(pMonster->m_wModNo != 630)
							{
								g_pExecutive->GXOSetDirection( pMonster->m_hMonster.pHandle, &g_Camera.v3AxsiY, fRad + DEG90 );
								pMonster->SetActionNext( MON_MOTION_TYPE_DEFENSEFAIL1, MON_MOTION_TYPE_STAND1, ACTION_ONCE, 0 );
							}

							pMonster->SetStatus(UNIT_STATUS_DAMAGING, MonsterDamageFunc);
						}
					}
					else
					{
						pUser->m_dwTemp[ USER_TEMP_TARGET_INDEX ] = 0xFFFFFFFF;

						::SetAction(pMonster->m_hMonster.pHandle, MON_MOTION_TYPE_DOWN, 0, ACTION_ONCE );
						pMonster->SetStatus(UNIT_STATUS_DEAD, MonsterKillFunc);
						pMonster->WithActionFunc = MonsterKillWithAction;
					}
					
					if( ( pUser == g_pMainPlayer ) || ( pMonster->m_dwLordIndex == g_pMainPlayer->m_dwUserIndex	 ) )
					{
						if( pMonster->m_dwTemp[MONSTER_TEMP_DAMAGE_TYPE] == 4 )	// ������ �����ߴٸ�.
						{
							pMonster->SetDamageIndex( pMonster->m_dwTemp[MONSTER_TEMP_DAMAGE] );
						}
						else pMonster->SetDamageChar( pMonster->m_dwTemp[MONSTER_TEMP_DAMAGE_TYPE]-1 );
					}

					CreateDamageEffect( OBJECT_TYPE_MONSTER, pMonster->m_dwMonsterIndex );
					
					DWORD dwAttack = pUser->m_wHandR / 100;
					if( pMonster->m_dwTemp[MONSTER_TEMP_DAMAGE_TYPE] == 4 )
					{
						if( ( dwAttack == 0 && pUser->m_wHandR != 0 ) || dwAttack == 1 || dwAttack == 2 || dwAttack == 3 || dwAttack == 5 )
						{	// Į���϶�.
							_PlaySound( WEAPONDAMAGE_SOUND_DAMAGE, SOUND_TYPE_WEAPONDAMAGE, WEAPONDAMAGE_SOUND_DAMAGE, pMonster->m_v3CurPos, FALSE );
						}
						else
						{	// �б� �϶�.
							_PlaySound( WEAPONDAMAGE_SOUND_DAMAGE, SOUND_TYPE_WEAPONDAMAGE, WEAPONDAMAGE_SOUND_DAMAGE + SOUND_PER_WEAPONDAMAGE, pMonster->m_v3CurPos, FALSE );
						}
					}
					else if( pMonster->m_dwTemp[MONSTER_TEMP_DAMAGE_TYPE] == 1 )	// block
					{
						if( ( dwAttack == 0 && pUser->m_wHandR != 0 ) || dwAttack == 1 || dwAttack == 2 || dwAttack == 3 || dwAttack == 5 )
						{	// Į���϶�.
							_PlaySound( WEAPONDAMAGE_SOUND_BLOCK, SOUND_TYPE_WEAPONDAMAGE, WEAPONDAMAGE_SOUND_DAMAGE, pMonster->m_v3CurPos, FALSE );
						}
						else
						{	// �б� �϶�.
							_PlaySound( WEAPONDAMAGE_SOUND_BLOCK, SOUND_TYPE_WEAPONDAMAGE, WEAPONDAMAGE_SOUND_DAMAGE + SOUND_PER_WEAPONDAMAGE, pMonster->m_v3CurPos, FALSE );
						}
					}					
				}
				break;

			case OBJECT_TYPE_PLAYER:
				{
					CUser* pDefense = g_pUserHash->GetData( pUser->m_dwTemp[ USER_TEMP_TARGET_INDEX ] );
					if( !pDefense ) goto lb_exit;
					
					if( pDefense->GetStatus()!= UNIT_STATUS_DEAD || pDefense->GetStatus()!=0)
					{	
						if (pDefense->GetStatus()== UNIT_STATUS_NORMAL)
						{
							pDefense->SetActionNext( MOTION_TYPE_DEFENSEFAIL, MOTION_TYPE_WARSTAND, ACTION_ONCE, 0 );
							pDefense->SetStatus(UNIT_STATUS_DAMAGING);
							pDefense->m_hPlayer.pDesc->ObjectFunc = PlayerDamageFunc;
						}
					}
					else
					{
						pUser->m_dwTemp[ USER_TEMP_TARGET_INDEX ] = 0xFFFFFFFF;
					}

					if( pUser == g_pMainPlayer )
					{
						if( pDefense->m_dwTemp[USER_TEMP_DAMAGE_TYPE] == 4 )	// ������ �����ߴٸ�.
						{
							pDefense->SetDamageIndex( pDefense->m_dwTemp[USER_TEMP_DAMAGE] );
						}
						else 
						{
							pDefense->SetDamageChar( pDefense->m_dwTemp[USER_TEMP_DAMAGE_TYPE]-1 );
						}
					}

					if( pDefense == g_pMainPlayer )
					{
						if( g_pMainPlayer->m_dwTemp[USER_TEMP_DAMAGE_TYPE] == 4 )	// ������ �����ߴٸ�.
						{
							g_pMainPlayer->SetDamageIndex( g_pMainPlayer->m_dwTemp[USER_TEMP_DAMAGE] );
						}
						else g_pMainPlayer->SetDamageChar( g_pMainPlayer->m_dwTemp[USER_TEMP_DAMAGE_TYPE]-1 );
					}

					CreateDamageEffect(OBJECT_TYPE_PLAYER, pDefense->m_dwUserIndex);

					DWORD dwAttack = pUser->m_wHandR / 100;
					if( pDefense->m_dwTemp[USER_TEMP_DAMAGE_TYPE] == 4 )
					{
						if( ( dwAttack == 0 && pUser->m_wHandR != 0 ) || dwAttack == 1 || dwAttack == 2 || dwAttack == 3 || dwAttack == 5 )
						{	// Į���϶�.
							_PlaySound( WEAPONDAMAGE_SOUND_DAMAGE, SOUND_TYPE_WEAPONDAMAGE, WEAPONDAMAGE_SOUND_DAMAGE, pUser->m_v3CurPos, FALSE );
						}
						else
						{	// �б� �϶�.
							_PlaySound( WEAPONDAMAGE_SOUND_DAMAGE, SOUND_TYPE_WEAPONDAMAGE, WEAPONDAMAGE_SOUND_DAMAGE + SOUND_PER_WEAPONDAMAGE, pUser->m_v3CurPos, FALSE );
						}
					}
					else if( pDefense->m_dwTemp[USER_TEMP_DAMAGE_TYPE] == 1 )	// block
					{
						if( ( dwAttack == 0 && pUser->m_wHandR != 0 ) || dwAttack == 1 || dwAttack == 2 || dwAttack == 3 || dwAttack == 5 )
						{	// Į���϶�.
							_PlaySound( WEAPONDAMAGE_SOUND_DAMAGE, SOUND_TYPE_WEAPONDAMAGE, WEAPONDAMAGE_SOUND_DAMAGE, pUser->m_v3CurPos, FALSE );
						}
						else
						{	// �б� �϶�.
							_PlaySound( WEAPONDAMAGE_SOUND_DAMAGE, SOUND_TYPE_WEAPONDAMAGE, WEAPONDAMAGE_SOUND_DAMAGE + SOUND_PER_WEAPONDAMAGE, pUser->m_v3CurPos, FALSE );
						}
					}
				}
				break;
			}	 
		}
	}
 
lb_exit:
	if ( bFrameFlag == FRAME_FLAG_FINISHED || bFrameFlag == FRAME_FLAG_CHANGE_NEXT )
	{
		::SetAction(pUser->m_hPlayer.pHandle, MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );
		pUser->SetStatus(UNIT_STATUS_NORMAL);
		pObjDesc->ObjectFunc	= NULL;

		// ������ ���� ƽ�� ����Ѵ�.
		if( pUser == g_pMainPlayer )
		{
			g_pMainPlayer->m_dwTemp[ USER_TEMP_LASTATTACKTICK ] = g_dwCurTick;
		}
	}
}

void MonsterAttackFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	CMonster*	 pMonster	= (CMonster*)pData->pInfo;
	
	if( pMonster->GetStatus() == UNIT_STATUS_ATTACKING )
	{			
		if( g_pChrInfoMonster->GetChrInfo(pMonster->m_wModNo) &&
			g_pChrInfoMonster->GetFrameInfo(pMonster->m_wModNo, 0, MON_MOTION_TYPE_ATTACK1, _CHRINFO_FRAME0) == dwCurFrame )
		{
			// ���⼭���� ������ Ÿ�������� �����̴�.
#if !defined(SOUND_NEW)
			int iRan = MONSTER_SOUND_ATTACK + GetRandom(2);
			GetSoundEffect(&pMonster->m_pSound[iRan], pMonster->m_hSoundFile[iRan], pMonster->m_v3CurPos);
			PlaySoundEffect(pMonster->m_pSound[iRan], &pMonster->m_v3CurPos, FALSE);

			g_SoundEffectBasketCase.Add(pMonster->m_pSound[iRan]);
			pMonster->m_pSound[iRan] = NULL;
#endif
			switch( pMonster->m_dwTemp[ USER_TEMP_TARGET_TYPE ] )
			{
			case OBJECT_TYPE_PLAYER:
				{
					CUser* pUser = g_pUserHash->GetData( pMonster->m_dwTemp[ MONSTER_TEMP_TARGET_INDEX ] );
					if( !pUser ) goto lb_exit;

					//if( pUser->m_bStatus != PC_STATUS_ATTACKING )
					switch(pUser->GetStatus())
					{
					case UNIT_STATUS_NORMAL:
					case UNIT_STATUS_WALKING:
					case UNIT_STATUS_RUNNING:
						{
							// ������ ������ �������� �����ش�.
							VECTOR3	v3DirMon;
							VECTOR3_SUB_VECTOR3( &v3DirMon, &pMonster->m_v3CurPos, &pUser->m_v3CurPos );
							g_pExecutive->GXOSetDirection( pUser->m_hPlayer.pHandle, &g_Camera.v3AxsiY, (float)(atan2(v3DirMon.z, v3DirMon.x) + DEG90 ) );							
							
							pUser->SetActionNext( MOTION_TYPE_DEFENSEFAIL, MOTION_TYPE_WARSTAND, ACTION_ONCE, 0 );
							pUser->SetStatus(UNIT_STATUS_DAMAGING);
							pUser->m_hPlayer.pDesc->ObjectFunc = PlayerDamageFunc;
						}
						break;

					case UNIT_STATUS_DEAD:
						{
							if (pUser->m_hPlayer.pDesc->ObjectFunc != PlayerKillFunc)
							{
								pUser->SetStatus(UNIT_STATUS_DEAD);			
							}	
						}
						break;
					}
					
					CreateDamageEffect( OBJECT_TYPE_PLAYER, pUser->m_dwUserIndex );

					if( pMonster->m_dwTemp[MONSTER_TEMP_MONMON_DAMAGE_TYPE] == 4 )
					{
						// �� ���Ͱ� �ٸ� �÷��̾ ģ��.
						if( g_pMainPlayer->m_dwUserIndex == pMonster->m_dwLordIndex )
						{
							pUser->SetDamageIndex( pMonster->m_dwTemp[MONSTER_TEMP_MONMON_DAMAGE] );
						}
					}
					else 
					{
						// �� ���Ͱ� �ٸ� �÷��̾ ģ��.
						if( g_pMainPlayer->m_dwUserIndex == pMonster->m_dwLordIndex )
						{
							pUser->SetDamageChar( pMonster->m_dwTemp[MONSTER_TEMP_MONMON_DAMAGE_TYPE] - 1);
						}
					}
				}
				break;

			case OBJECT_TYPE_MAINPLAYER:
				{	
					// ���� �÷��̾ �Ȱų� �ٰ� �־��ٸ�.
					switch( g_pMainPlayer->GetStatus())
					{
					case UNIT_STATUS_NORMAL:
					case UNIT_STATUS_WALKING:
					case UNIT_STATUS_RUNNING:
						{
#if IS_CHINA_LOCALIZING() //�߱��� ��� �̽��ϰ�� �¾����� ������ ����  �迵��  2005.03.14
							if( pMonster->m_dwTemp[MONSTER_TEMP_MONMON_DAMAGE_TYPE] != 2 )
							{
								if (g_pMainPlayer->GetStatus()== UNIT_STATUS_WALKING || g_pMainPlayer->GetStatus()== UNIT_STATUS_RUNNING)
									SendStopPacket();
								// ������ ������ �������� �����ش�.
								VECTOR3	v3DirMon;
								VECTOR3_SUB_VECTOR3( &v3DirMon, &pMonster->m_v3CurPos, &g_pMainPlayer->m_v3CurPos );
								g_pExecutive->GXOSetDirection( g_pMainPlayer->m_hPlayer.pHandle, &g_Camera.v3AxsiY, (float)(atan2(v3DirMon.z, v3DirMon.x) + DEG90 ) );

								g_pMainPlayer->SetActionNext( MOTION_TYPE_DEFENSEFAIL, MOTION_TYPE_WARSTAND, ACTION_ONCE, 0 );
								g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = PlayerDamageFunc;
								g_pMainPlayer->SetStatus(UNIT_STATUS_DAMAGING);
							}
#else
							if (g_pMainPlayer->GetStatus()== UNIT_STATUS_WALKING || g_pMainPlayer->GetStatus()== UNIT_STATUS_RUNNING)
								SendStopPacket();
							// ������ ������ �������� �����ش�.
							VECTOR3	v3DirMon;
							VECTOR3_SUB_VECTOR3( &v3DirMon, &pMonster->m_v3CurPos, &g_pMainPlayer->m_v3CurPos );
							g_pExecutive->GXOSetDirection( g_pMainPlayer->m_hPlayer.pHandle, &g_Camera.v3AxsiY, (float)(atan2(v3DirMon.z, v3DirMon.x) + DEG90 ) );

							g_pMainPlayer->SetActionNext( MOTION_TYPE_DEFENSEFAIL, MOTION_TYPE_WARSTAND, ACTION_ONCE, 0 );
							g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = PlayerDamageFunc;
							g_pMainPlayer->SetStatus(UNIT_STATUS_DAMAGING);


#endif 
						}
						break;
					case UNIT_STATUS_DEAD:
						{
							if (g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc != PlayerKillFunc)
							{
								g_pMainPlayer->SetStatus(UNIT_STATUS_DEAD);
							}
							
						}break;
					}										
					
					CreateDamageEffect(OBJECT_TYPE_MAINPLAYER, g_pMainPlayer->m_dwUserIndex);

					if( pMonster->m_dwTemp[MONSTER_TEMP_MONMON_DAMAGE_TYPE] == 4 )
					{
						g_pMainPlayer->SetDamageIndex( pMonster->m_dwTemp[MONSTER_TEMP_MONMON_DAMAGE] );
					}
					else 
					{
						g_pMainPlayer->SetDamageChar( pMonster->m_dwTemp[MONSTER_TEMP_MONMON_DAMAGE_TYPE] - 1);
					}
				}
				break;
				
			case OBJECT_TYPE_MONSTER:
				{
					CMonster* pDefenseMon = g_pMonsterHash->GetData( pMonster->m_dwTemp[ MONSTER_TEMP_TARGET_INDEX ] );
					if( !pDefenseMon ) goto lb_exit;
					
					VECTOR3	v3DirMon;
					VECTOR3_SUB_VECTOR3( &v3DirMon, &pMonster->m_v3CurPos, &pDefenseMon->m_v3CurPos);
					float fRad = (float)atan2(v3DirMon.z, v3DirMon.x);
					pDefenseMon->m_vecTemp	= pDefenseMon->m_v3CurPos;
					pDefenseMon->m_radian	= fRad+3.14;
					
					switch( pDefenseMon->GetStatus() )
					{
					case UNIT_STATUS_DEAD:
						{							
							::SetAction(pDefenseMon->m_hMonster.pHandle, MON_MOTION_TYPE_DOWN, 0, ACTION_ONCE );
							pDefenseMon->SetStatus(UNIT_STATUS_DEAD, MonsterKillFunc);
							pDefenseMon->WithActionFunc = MonsterKillWithAction;							
						}
						break;

					case UNIT_STATUS_NORMAL:
						{
							g_pExecutive->GXOSetDirection( pDefenseMon->m_hMonster.pHandle, &g_Camera.v3AxsiY, fRad + DEG90 );
							pDefenseMon->SetActionNext( MON_MOTION_TYPE_DEFENSEFAIL1, MON_MOTION_TYPE_STAND1, ACTION_ONCE, 0 );
							pDefenseMon->SetStatus(UNIT_STATUS_DAMAGING, MonsterDamageFunc);
						}
						break;
					}
					
					if( pMonster->m_dwTemp[MONSTER_TEMP_MONMON_DAMAGE_TYPE] == 4 )
					{
						// ���� ������ �����̰ų� ���� ���Ͱ� ����̶��.
						if( ( g_pMainPlayer->m_dwUserIndex == pMonster->m_dwLordIndex ) || ( g_pMainPlayer->m_dwUserIndex == pDefenseMon->m_dwLordIndex ) )
						{
							pDefenseMon->SetDamageIndex( pMonster->m_dwTemp[MONSTER_TEMP_MONMON_DAMAGE] );
						}
					}
					else 
					{
						// ���� ������ �����̰ų� ���� ���Ͱ� ����̶��.
						if( ( g_pMainPlayer->m_dwUserIndex == pMonster->m_dwLordIndex ) || ( g_pMainPlayer->m_dwUserIndex == pDefenseMon->m_dwLordIndex ) )
						{
							pDefenseMon->SetDamageChar( pMonster->m_dwTemp[MONSTER_TEMP_MONMON_DAMAGE_TYPE] - 1);
						}
					}

					CreateDamageEffect(OBJECT_TYPE_MONSTER, pDefenseMon->m_dwMonsterIndex);
				}
				break;
			}
		}
	}

lb_exit:
	if( bFrameFlag == FRAME_FLAG_CHANGE_NEXT || bFrameFlag == FRAME_FLAG_FINISHED )
	{		
		::SetAction(pMonster->m_hMonster.pHandle, MON_MOTION_TYPE_STAND1, 0, ACTION_LOOP);
		pMonster->SetStatus(UNIT_STATUS_NORMAL, NULL);		
	}
}


void MonsterDisappear( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{
	CMonster* pMonster = (CMonster*)pData->pInfo;

	DWORD dwAlpha = 0; 
	g_pExecutive->SetAlphaFlag(handle,dwAlpha);

	if( bFrameFlag == FRAME_FLAG_FINISHED || bFrameFlag == FRAME_FLAG_CHANGE_NEXT )
	{
		RemoveMonster( pMonster );
	}
}

void MonsterKillFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{
	CMonster* pMonster = (CMonster*)pData->pInfo;

#if !defined(SOUND_NEW)	
	if( 0 == dwCurFrame )
	{
		ReleaseBoundingVolume( pMonster->m_hMonster.pHandle );
		int	iRan = MONSTER_SOUND_DEATH + GetRandom(2);
		GetSoundEffect(&pMonster->m_pSound[iRan], pMonster->m_hSoundFile[iRan], pMonster->m_v3CurPos);
		PlaySoundEffect(pMonster->m_pSound[iRan], &pMonster->m_v3CurPos, FALSE);
		g_SoundEffectBasketCase.Add(pMonster->m_pSound[iRan]);
		pMonster->m_pSound[iRan] = NULL;
	}
#endif
	
	if (pMonster->m_dwTemp[MONSTER_TEMP_PUSHING])
	{
		if( bFrameFlag == FRAME_FLAG_FINISHED || bFrameFlag == FRAME_FLAG_CHANGE_NEXT )
		{
			if( pMonster->m_v3CurPos.y >= -80.0f )
			{
				pMonster->m_v3CurPos.y -= 1.0f;
				GXSetPosition( pMonster->m_hMonster.pHandle, &pMonster->m_v3CurPos, FALSE );
				g_pExecutive->SetAlphaFlag( pMonster->m_hMonster.pHandle, (DWORD)( 180 + ( pMonster->m_v3CurPos.y*2) ) );

				if (pMonster->m_v3CurPos.y == -10)
				{
					if (pMonster->m_dwMonsterKind == OBJECT_TYPE_GUARDIAN && !pMonster->m_dwLordIndex)
					{
						// ��ȣ������� �׾�����
						EffectDesc* pEffectDesc = g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(EFFECT_GUARDIAN_DYING),TRUE, __CHR_EFFECT_GUARDIAN_DYING__);
						pEffectDesc->dwOwnIndex = pMonster->m_dwMonsterIndex;
						pEffectDesc->dwOwnType = OBJECT_TYPE_MONSTER;
						pEffectDesc->byTargetObjectType[0] = OBJECT_TYPE_PLAYER;
						::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE );
						
						pEffectDesc->hEffect.pDesc->ObjectFunc = EffectGuardianKillFunc;
						CUser* pKiller = g_pUserHash->GetData(pMonster->m_dwTemp[MONSTER_TEMP_KILLERINDEX]);
						
						if (pKiller)	pEffectDesc->dwTargetIndex[0] = pKiller->m_dwUserIndex;
					}
				}
			}	
			else
			{
				// ���� ������� �׾��ٸ�.
				if( pMonster->m_dwLordIndex == g_pMainPlayer->m_dwUserIndex )
				{
					if( pMonster->m_dwMonsterKind == OBJECT_TYPE_GUARDIAN )
					{
						// ����� �������̽� �ʱ�ȭ.
						CUserInterface::GetInstance()->CloseGuardianDescriptionWnd();

						// Item �̵� //
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
				}

				RemoveMonster( pMonster );	
			}
		}			
	}
	else
	{
		if (pMonster->WithActionFunc)
			pMonster->WithActionFunc(pMonster);
	}		
}


// ���������� ���� �ٴϸ鼭 ���ĸ� ���δ�....(���� ����Ʈ ����)
void EffectPotionAroundRemoveFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc* pEffectDesc = (EffectDesc*)pObjDesc->pInfo;
	VECTOR3 vecStart;
	
	switch(pEffectDesc->byTargetObjectType[0])
	{
	case OBJECT_TYPE_PLAYER:
		{
			CUser* pUser = (CUser*)g_pUserHash->GetData(pEffectDesc->dwTargetIndex[0]);
			if (!pUser)
			{ 
				RemoveEffectDesc(pEffectDesc);
				return;
			}

			vecStart = pUser->m_v3CurPos;
			if (pEffectDesc->dwFrameCount > 20)
				g_pExecutive->SetAlphaFlag(handle, pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_ALPHA]-(pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_ALPHA]/30*(pEffectDesc->dwFrameCount-20)));
		}break;
		
	}
	
	if( bFrameFlag == FRAME_FLAG_FINISHED)
		RemoveEffectDesc(pEffectDesc);	
	else
		GXSetPosition( handle, &vecStart, FALSE );

	pEffectDesc->dwFrameCount++;
}

// ���������� ���� �ٴѴ�.
void EffectSkillUserAroundRemoveFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc* pEffectDesc = (EffectDesc*)pObjDesc->pInfo;
	VECTOR3 vecStart;

	switch(pEffectDesc->byTargetObjectType[0])
	{
	case OBJECT_TYPE_PLAYER:
		{
			CUser* pUser = (CUser*)g_pUserHash->GetData(pEffectDesc->dwTargetIndex[0]);
			if (!pUser) return;

			vecStart = pUser->m_v3CurPos;
		}break;
	case OBJECT_TYPE_MONSTER:
		{
		}break;
	case OBJECT_TYPE_SUMMON:
		{
			DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pEffectDesc->dwTargetIndex[0]);
			if(!pDungeon) return;

			vecStart = pDungeon->vPos;
		}break;
	}
	
	if( bFrameFlag == FRAME_FLAG_FINISHED)
		RemoveEffectDesc(pEffectDesc);	
	else
		GXSetPosition( handle, &vecStart, FALSE );	
}


// ���������� ���� �ٴѴ�.
void EffectSkillAroundFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc* pEffectDesc = (EffectDesc*)pObjDesc->pInfo;
	VECTOR3 vecStart;

	switch(pEffectDesc->byTargetObjectType[0])
	{
	case OBJECT_TYPE_PLAYER:
		{
			CUser* pUser = (CUser*)g_pUserHash->GetData(pEffectDesc->dwTargetIndex[0]);
			if (!pUser) return;

			vecStart = pUser->m_v3CurPos;
		}break;
	case OBJECT_TYPE_MONSTER:
		{
			CMonster* pMonster = (CMonster*)g_pMonsterHash->GetData(pEffectDesc->dwTargetIndex[0]);
			if (!pMonster) return;

			vecStart = pMonster->m_v3CurPos;
		}break;
	case OBJECT_TYPE_SUMMON:
		{
			DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pEffectDesc->dwTargetIndex[0]);
			if(!pDungeon) return;

			vecStart = pDungeon->vPos;
		}break;
	}
	
	GXSetPosition( handle, &vecStart, FALSE );	
}


// �����ð����� ���ӵǴ� ����Ʈ�� ���..
void EffectSkillUserStatusTopFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{	
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc* pEffectDesc = (EffectDesc*)pObjDesc->pInfo;
	
	CUser* pUser = (CUser*)g_pUserHash->GetData(pEffectDesc->dwTargetIndex[0]);
	if (!pUser) return;	
	
	if (pObjDesc->dwDestTime - g_dwCurTick <= 30000 && pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_30SEC]==0)	// 30�� ���Ҵٸ�
	{
		pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_30SEC] = 1;
		PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_GENERAL1], &pEffectDesc->vecBasePosion, FALSE);
	}
	else if (pObjDesc->dwDestTime - g_dwCurTick <= 5000 && pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_5SEC]==0)	// 5�� ���ҵ���
	{
		pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_5SEC] = 1;
		PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_GENERAL2], &pEffectDesc->vecBasePosion, FALSE);
	}

	GXSetPosition( handle, &pUser->m_v3CurPos, FALSE );
	
	if (bFrameFlag == FRAME_FLAG_FINISHED_HIDE)
		g_pExecutive->DisableRender(handle);
}

void EffectSkillUserStatusCenterFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{	
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc* pEffectDesc = (EffectDesc*)pObjDesc->pInfo;
	
	CUser* pUser = (CUser*)g_pUserHash->GetData(pEffectDesc->dwTargetIndex[0]);
	if (!pUser)	return;

	if (pObjDesc->dwDestTime - g_dwCurTick <= 30000 && pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_30SEC]==0)	// 30�� ���Ҵٸ�
	{
		pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_30SEC] = 1;
		PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_GENERAL1], &pEffectDesc->vecBasePosion, FALSE);
	}
	else if (pObjDesc->dwDestTime - g_dwCurTick <= 5000 && pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_5SEC]==0)	// 5�� ���ҵ���
	{
		pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_5SEC] = 1;
		PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_GENERAL2], &pEffectDesc->vecBasePosion, FALSE);
	}

	switch(pEffectDesc->pEffect->bID )
	{
	case __SKILL_POSIONCLOUD__:
	case __SKILL_POISONING__:
		DWORD dwTick = g_dwCurTick - pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_1SEC];
		if (dwTick >= 1000)
		{
			// ���� 1�ʸ��� ���̴� �������� ǥ������� �Ѵ�.			
			int  nDamage = pEffectDesc->dwTemp[SKILL_TEMP_POSONING];
			if (pUser == g_pMainPlayer)
			{				
				if (g_pMainPlayer->m_wHP-nDamage >= 1)
					CUserInterface::GetInstance()->SetDengeonHp(g_pMainPlayer->m_wHP-nDamage);
				else
					CUserInterface::GetInstance()->SetDengeonHp(1);				
			}

			pUser->SetDamageIndex( nDamage );
			pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_1SEC] = g_dwCurTick;
		}
		break;
	}
	
	GXSetPosition( handle, &pUser->m_v3CurPos, FALSE );	
}

void EffectSkillUserStatusBottomFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{	
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc* pEffectDesc = (EffectDesc*)pObjDesc->pInfo;
	
	CUser* pUser = (CUser*)g_pUserHash->GetData(pEffectDesc->dwTargetIndex[0]);
	if (!pUser)	return;

	pEffectDesc->dwFrameCount++; 
	
	if (pEffectDesc->bEffectInfoNum == __SKILL_HIDING__)
	{
		if(g_pMainPlayer->IsAlliance(pUser))
		{
			pUser->UserObjectEnable(TRUE);
			
			if (pUser->GetStatus() == UNIT_STATUS_WALKING || pUser->GetStatus()== UNIT_STATUS_RUNNING)
			{
				pUser->UserObjectAlpha(100);  //(����â 2005.03.07) - ���� ���¿��� �ٰų� ������ ���Ż��¸� ���� ��Ŵ 255���� 100���� ���� 				
			}
			else     
			{
				pUser->UserObjectAlpha(100);
			}			
		}
		else
		{
			if (pUser->GetStatus() == UNIT_STATUS_WALKING || pUser->GetStatus() == UNIT_STATUS_RUNNING)
			{
				pUser->UserObjectEnable(FALSE);	//(����â 2005. 03.17) - ���� ������ ��� �ٰų� ������ �ٸ� �������׵� ������ ���� 	
			}
			else
			{
				pUser->UserObjectEnable(FALSE);
			}			
		}

		if (pObjDesc->dwDestTime - g_dwCurTick <= 5000 && pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_5SEC]==0)	// 5�� ���ҵ���
		{
			pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_5SEC] = 1;
			PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_GENERAL1], &pEffectDesc->vecBasePosion, FALSE);
		}		
	}
	else
	{
		// ���Ŷ�����..������ 5�ʸ� �ִ�. �׿��� ���� 30��, 5�ʸ��� Ÿ�̸�.
		if (pObjDesc->dwDestTime - g_dwCurTick <= 30000 && pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_30SEC]==0)	// 30�� ���Ҵٸ�
		{
			pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_30SEC] = 1;
			PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_GENERAL1], &pEffectDesc->vecBasePosion, FALSE);
		}
		else if (pObjDesc->dwDestTime - g_dwCurTick <= 5000 && pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_5SEC]==0)	// 5�� ���ҵ���
		{
			pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_5SEC] = 1;
			PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_GENERAL2], &pEffectDesc->vecBasePosion, FALSE);
		}
	}

	GXSetPosition( handle, &pUser->m_v3CurPos, FALSE );	
}


// �����ð����� ���ӵǴ� ����Ʈ�� ���..
void EffectSkillMonsterStatusTopFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{	
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc* pEffectDesc = (EffectDesc*)pObjDesc->pInfo;
	
	CMonster* pMonster = g_pMonsterHash->GetData(pEffectDesc->dwTargetIndex[0]);
	if (!pMonster)	return;
	
	if (pObjDesc->dwDestTime - g_dwCurTick <= 30000 && pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_30SEC]==0)	// 30�� ���Ҵٸ�
	{
		pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_30SEC] = 1;
		PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_GENERAL1], &pEffectDesc->vecBasePosion, FALSE);
	}
	else if (pObjDesc->dwDestTime - g_dwCurTick <= 5000 && pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_5SEC]==0)	// 5�� ���ҵ���
	{
		pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_5SEC] = 1;
		PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_GENERAL2], &pEffectDesc->vecBasePosion, FALSE);
	}

	COLLISION_MESH_OBJECT_DESC meshdesc;
	g_pExecutive->GXOGetCollisionMesh(pMonster->m_hMonster.pHandle,  &meshdesc);
	VECTOR3 vec = pMonster->m_v3CurPos; vec.y = meshdesc.boundingBox.v3Oct[0].y;
	GXSetPosition( handle, &vec, FALSE );
	
	if (bFrameFlag == FRAME_FLAG_FINISHED_HIDE)
		g_pExecutive->DisableRender(handle);
	
	if (pEffectDesc->pEffect && __SKILL_DETONATION__ == pEffectDesc->pEffect->bID)
		g_pExecutive->GXOSetEffectIndex(pMonster->m_hMonster.pHandle, EFS_upgrade_effect_6);
}

void EffectSkillMonsterStatusCenterFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{	
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc* pEffectDesc = (EffectDesc*)pObjDesc->pInfo;
	
	CMonster* pMonster = g_pMonsterHash->GetData(pEffectDesc->dwTargetIndex[0]);
	if (!pMonster)	return;
	
	if (pObjDesc->dwDestTime - g_dwCurTick <= 30000 && pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_30SEC]==0)	// 30�� ���Ҵٸ�
	{
		pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_30SEC] = 1;
		PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_GENERAL1], &pEffectDesc->vecBasePosion, FALSE);
	}
	else if (pObjDesc->dwDestTime - g_dwCurTick <= 5000 && pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_5SEC]==0)	// 5�� ���ҵ���
	{
		pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_5SEC] = 1;
		PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_GENERAL2], &pEffectDesc->vecBasePosion, FALSE);
	}

	switch(pEffectDesc->pEffect->bID )
	{
	case __SKILL_POSIONCLOUD__:
	case __SKILL_POISONING__:
		DWORD dwTick = g_dwCurTick - pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_1SEC];
		if (dwTick >= 1000)
		{
			// ���� 1�ʸ��� ���̴� �������� ǥ������� �Ѵ�.
			
			int  nDamage = pEffectDesc->dwTemp[SKILL_TEMP_POSONING];
			
			if ((int)pMonster->m_dwHP-nDamage >= 1)
				pMonster->m_dwHP -= nDamage;
			else
				pMonster->m_dwHP = 1;
			
			pMonster->SetDamageIndex( nDamage );
			pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_1SEC] = g_dwCurTick;
		}
		break;
	}
	
	GXSetPosition( handle, &pMonster->m_v3CurPos, FALSE );	
}

void EffectSkillMonsterStatusBottomFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{	
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc* pEffectDesc = (EffectDesc*)pObjDesc->pInfo;
	
	CMonster* pMonster = g_pMonsterHash->GetData(pEffectDesc->dwTargetIndex[0]);
	if (!pMonster)	return;
	
	if (pObjDesc->dwDestTime - g_dwCurTick <= 30000 && pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_30SEC]==0)	// 30�� ���Ҵٸ�
	{
		pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_30SEC] = 1;
		PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_GENERAL1], &pEffectDesc->vecBasePosion, FALSE);
	}
	else if (pObjDesc->dwDestTime - g_dwCurTick <= 5000 && pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_5SEC]==0)	// 5�� ���ҵ���
	{
		pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_5SEC] = 1;
		PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_GENERAL2], &pEffectDesc->vecBasePosion, FALSE);
	}
	
	switch(pEffectDesc->pEffect->bID)
	{
	case __SKILL_REDELEMENTAL__:
	case __SKILL_BLUEELEMENTAL__:
	case __SKILL_GREENELEMENTAL__:
		{
			if( bFrameFlag == FRAME_FLAG_FINISHED || bFrameFlag == FRAME_FLAG_CHANGE_NEXT ) 
			{
				pEffectDesc->bOwn = 2;	// �ɼ��� �ٽ� ���������� ������ ���ƾ� �ϴ� ����Ʈ�ϰ��
				HideObject(handle);
			}
		}break;
	}

	GXSetPosition( handle, &pMonster->m_v3CurPos, FALSE );
}

void MonsterKillWithAction1( GXOBJECT_HANDLE handle)
{
	// ��� ���ý� �и� ó��.
	CMonster*	 pMonster = (CMonster*) handle;
	if (pMonster == NULL) return;	

	float time = pMonster->m_dwTemp[MONSTER_TEMP_FRAMECOUNT]/2.0;

	if (time < 10)
	{
		// �и��� ó��.		
		VECTOR3 vecDest = pMonster->m_v3CurPos;
		MAP_TILE* pTile = g_pMap->GetTile(vecDest.x, vecDest.z);
		if (pTile)	
		{
			if (pTile->wAttr.uOccupied != TILE_OCCUPIED_DONT_MOVE)	
			{
				float v0 = pMonster->m_dwTemp[MONSTER_TEMP_PUSHPOWER]*time;
				float ttime = time*time;
				
				vecDest.x = pMonster->m_vecTemp.x+ v0*cosf(pMonster->m_radian)-sqrt(ttime)*0.5;
				vecDest.z = pMonster->m_vecTemp.z+ v0*sinf(pMonster->m_radian)-sqrt(ttime)*0.5;
				vecDest.y = pMonster->m_vecTemp.y+ sinf(0.55f)*v0-0.5*9.888*(ttime);
				vecDest.y = max(vecDest.y, 0);
				g_pExecutive->GXOSetDirection(pMonster->m_hMonster.pHandle, &g_Camera.v3AxsiY, pMonster->m_fDir+=0.5);		
				SetMonsterPosition(pMonster, &vecDest, 2);		
			}
		}
	}
	else
	{
		pMonster->m_dwTemp[MONSTER_TEMP_PUSHING] = 1; // finish;
	}

	pMonster->m_dwTemp[MONSTER_TEMP_FRAMECOUNT]++;
}


void MonsterKillWithAction( GXOBJECT_HANDLE handle)
{
	// �ڷ� ��~~ �и��鼭 �״� �׼� �Լ�.
	CMonster*	 pMonster = (CMonster*) handle;
	if (pMonster == NULL) return;	

	float time = pMonster->m_dwTemp[MONSTER_TEMP_FRAMECOUNT]/2.0;	
	
	{
		// �и��� ó��.		
		VECTOR3 vecDest = pMonster->m_v3CurPos;		

		MAP_TILE* pTile = g_pMap->GetTile(vecDest.x, vecDest.z);
		if (pTile)	
		{
			float v0 = pMonster->m_dwTemp[MONSTER_TEMP_PUSHPOWER]*time;
			float ttime = time*time;
			if (pTile->wAttr.uOccupied != TILE_OCCUPIED_DONT_MOVE)	
			{		
				float vx = v0*cosf(pMonster->m_radian);
				float vz = v0*sinf(pMonster->m_radian);
		
				vecDest.x = pMonster->m_vecTemp.x+ vx-sqrt(ttime)*0.5;
				vecDest.z = pMonster->m_vecTemp.z+ vz-sqrt(ttime)*0.5;			
			}	

			vecDest.y = pMonster->m_vecTemp.y+ sinf(0.55f)*v0-0.5*9.888*(ttime);
			vecDest.y = max(vecDest.y, 0);
			g_pExecutive->GXOSetDirection(pMonster->m_hMonster.pHandle, &g_Camera.v3AxsiY, pMonster->m_fDir+=0.5);
			SetMonsterPosition(pMonster, &vecDest, 2);
		}			
	}

	if (time && pMonster->m_v3CurPos.y == 0)
		pMonster->m_dwTemp[MONSTER_TEMP_PUSHING] = 1; // finish;

	pMonster->m_dwTemp[MONSTER_TEMP_FRAMECOUNT]++;
}


void MonsterDamageFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{
	CMonster* pMonster = (CMonster*)pData->pInfo;
	
#if !defined (SOUND_NEW)
	if( 0 == dwCurFrame )
	{
		int iRandom = MONSTER_SOUND_GETHIT + GetRandom(2);
		GetSoundEffect(&pMonster->m_pSound[iRandom], pMonster->m_hSoundFile[iRandom], pMonster->m_v3CurPos);
		PlaySoundEffect(pMonster->m_pSound[iRandom], &pMonster->m_v3CurPos, FALSE);

		g_SoundEffectBasketCase.Add(pMonster->m_pSound[iRandom]);
		pMonster->m_pSound[iRandom] = NULL;
	}
#endif

	if( bFrameFlag == FRAME_FLAG_CHANGE_NEXT || bFrameFlag == FRAME_FLAG_FINISHED )
	{
		::SetAction(pMonster->m_hMonster.pHandle, MON_MOTION_TYPE_STAND1, 0, ACTION_LOOP);
		pMonster->SetStatus(UNIT_STATUS_NORMAL, NULL);
	}
	else
	{
		if (pMonster->WithActionFunc)
			pMonster->WithActionFunc(pMonster);
	}
}

DWORD	GetSkillStartActionFinishCount(BYTE bSkillKind, void* pObject, BYTE bObjectType)
{
	WORD wClass = 0;
	BYTE byItemType = 0;

	if (bObjectType == OBJECT_TYPE_MONSTER)
	{
		return 21;// ������ ��ų �׼ǿ��ٰ� ������ ���� �ϴ� �ٺ��� ��.��
	}
	else if (bObjectType == OBJECT_TYPE_PLAYER)
	{
		CUser* pUser = (CUser*)pObject;
		if (pUser)
		{
			wClass = pUser->m_wClass-1;
			byItemType = pUser->m_byItemType;
		}		
	}	

	switch(bSkillKind)
	{
		// Ÿ�������� ���߷���.
	case __SKILL_LIGHTNINGBREAK__:
	case __SKILL_RAGINGSWORD__:
		return 0;
		
	case __SKILL_BASH__:
		{
			return g_pChrInfoUser->GetFrameInfo(wClass, byItemType, MOTION_TYPE_BASH, _CHRINFO_FRAME0);			
		}break;
	case __SKILL_BLASTQUAKE__:
		{
			return g_pChrInfoUser->GetFrameInfo(wClass, byItemType, MOTION_TYPE_BLASTQUAKE, _CHRINFO_FRAME0);
		}break;
	case __SKILL_ZEAL__:
		{
			return g_pChrInfoUser->GetFrameInfo(wClass, byItemType, MOTION_TYPE_ATTACK1_1, _CHRINFO_FRAME0);			
		}
		break;
	case __SKILL_CHAINATTACKER__:
		return g_pChrInfoUser->GetFrameInfo(wClass, byItemType, MOTION_TYPE_CHAINATTACKER, _CHRINFO_FRAME0);
	default:
		return g_pChrInfoUser->GetFrameInfo(wClass, byItemType, MOTION_TYPE_SKILL, _CHRINFO_FRAME0);
	}	
}

EffectDesc* SkillSubProcess_General(BYTE bSkillKind, BYTE bSkillLevel, BYTE bJoint, DWORD dwOwnType, DWORD dwOwnIndex, DWORD dwCount, VECTOR3 vecStart, TARGETINFO* pTargetInfo )
{		
	EffectDesc*	pEffectDesc = g_pEffectLayer->CreateEffect(bSkillKind, bJoint, dwOwnType==OBJECT_TYPE_PLAYER && g_pMainPlayer->m_dwUserIndex == dwOwnIndex);
	
	pEffectDesc->vecBasePosion = vecStart;
	pEffectDesc->dwOwnIndex = dwOwnIndex;
	pEffectDesc->dwOwnType = dwOwnType;
	pEffectDesc->dwSkillLevel = bSkillLevel;
	pEffectDesc->dwFrameCount = 0;
	::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_PAUSE );
	g_pExecutive->DisableRender(pEffectDesc->hEffect.pHandle);
	pEffectDesc->hEffect.pDesc->ObjectFunc = EffectSkillStartFunc;
	
	return pEffectDesc;
}

EffectDesc* SkillSubProcess_FireBall(BYTE bSkillKind, BYTE bSkillLevel, DWORD dwOwnType, DWORD dwOwnIndex, DWORD dwCount, VECTOR3 vecStart, TARGETINFO* pTargetInfo, float fRad )
{
	EffectDesc*	pEffectDesc = CreateEffectBall(bSkillKind, fRad, dwOwnType, dwOwnIndex);
	pEffectDesc->vecBasePosion = vecStart;
	pEffectDesc->dwOwnIndex = dwOwnIndex;
	pEffectDesc->dwOwnType = dwOwnType;
	pEffectDesc->dwSkillLevel = bSkillLevel;
	pEffectDesc->dwFrameCount = 0;
	::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_PAUSE );
	g_pExecutive->DisableRender(pEffectDesc->hEffect.pHandle);
	pEffectDesc->hEffect.pDesc->ObjectFunc = EffectSkillStartFunc;
	
	return pEffectDesc;
}

EffectDesc* SkillSubProcess_MagmaWall(BYTE bSkillKind, BYTE bSkillLevel, DWORD dwOwnType, DWORD dwOwnIndex, DWORD dwCount, VECTOR3 vecStart, TARGETINFO* pTargetInfo )
{
	EffectDesc*	pEffectDesc = g_pEffectLayer->CreateEffect(bSkillKind, 0 , dwOwnType==OBJECT_TYPE_PLAYER && g_pMainPlayer->m_dwUserIndex == dwOwnIndex);
		
	pEffectDesc->vecBasePosion = vecStart;
	pEffectDesc->dwOwnIndex = dwOwnIndex;
	pEffectDesc->dwOwnType = dwOwnType;
	pEffectDesc->dwSkillLevel = bSkillLevel;
	pEffectDesc->dwFrameCount = 0;
	::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_PAUSE );
	g_pExecutive->DisableRender(pEffectDesc->hEffect.pHandle);
	pEffectDesc->hEffect.pDesc->ObjectFunc = EffectSkillStartFunc;
	
	VECTOR3	v3DirMon = {0,};
	
	if (dwOwnType == OBJECT_TYPE_PLAYER)
	{
		CUser* pUser = g_pUserHash->GetData(dwOwnIndex);
		VECTOR3_SUB_VECTOR3( &v3DirMon, &vecStart, &pUser->m_v3CurPos );
	}
	else if( dwOwnType == OBJECT_TYPE_MONSTER)
	{
		CMonster* pMonster = g_pMonsterHash->GetData(dwOwnIndex);
		VECTOR3_SUB_VECTOR3( &v3DirMon, &vecStart, &pMonster->m_v3CurPos );
	}
		
	// ������ ������ �̷����� �Ѵ�.
	pEffectDesc->f_Radcount = (float)(atan2(v3DirMon.z, v3DirMon.x) + DEG90 );

	return pEffectDesc;	
}

EffectDesc* SkillSubProcess_IceWall(BYTE bSkillKind, BYTE bSkillLevel, DWORD dwOwnType, DWORD dwOwnIndex, DWORD dwCount, VECTOR3 vecStart, TARGETINFO* pTargetInfo )
{
	EffectDesc*	pEffectDesc = g_pEffectLayer->CreateEffect(bSkillKind, 0 , dwOwnType==OBJECT_TYPE_PLAYER && g_pMainPlayer->m_dwUserIndex == dwOwnIndex);
		
	pEffectDesc->vecBasePosion = vecStart;
	pEffectDesc->dwOwnIndex = dwOwnIndex;
	pEffectDesc->dwOwnType = dwOwnType;
	pEffectDesc->dwSkillLevel = bSkillLevel;
	pEffectDesc->dwFrameCount = 0;
	::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_PAUSE );
	g_pExecutive->DisableRender(pEffectDesc->hEffect.pHandle);
	pEffectDesc->hEffect.pDesc->ObjectFunc = EffectSkillStartFunc;
	
	VECTOR3	v3DirMon = {0,};
	if (dwOwnType == OBJECT_TYPE_PLAYER)
	{ 
		CUser* pUser = g_pUserHash->GetData(dwOwnIndex);
		VECTOR3_SUB_VECTOR3( &v3DirMon, &vecStart, &pUser->m_v3CurPos );
	}
	else if (dwOwnType == OBJECT_TYPE_MONSTER)
	{
		CMonster* pMonster = g_pMonsterHash->GetData(dwOwnIndex);
		VECTOR3_SUB_VECTOR3( &v3DirMon, &vecStart, &pMonster->m_v3CurPos );
	}
	
	// ������ ������ �̷����� �Ѵ�.
	pEffectDesc->f_Radian = (float)(atan2(v3DirMon.z, v3DirMon.x));
	return pEffectDesc;	
}

EffectDesc* SkillSubProcess_BlastQuake(BYTE bSkillKind, BYTE bSkillLevel, DWORD dwOwnType, DWORD dwOwnIndex, DWORD dwCount, VECTOR3 vecStart, TARGETINFO* pTargetInfo )
{
	EffectDesc*	pEffectDesc = g_pEffectLayer->CreateEffect(bSkillKind, 0 , dwOwnType==OBJECT_TYPE_PLAYER && g_pMainPlayer->m_dwUserIndex == dwOwnIndex);
		
	pEffectDesc->vecBasePosion = vecStart;
	pEffectDesc->dwOwnIndex = dwOwnIndex;
	pEffectDesc->dwOwnType = dwOwnType;
	pEffectDesc->dwSkillLevel = bSkillLevel;
	pEffectDesc->dwFrameCount = 0;
	::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_PAUSE );
	g_pExecutive->DisableRender(pEffectDesc->hEffect.pHandle);
	pEffectDesc->hEffect.pDesc->ObjectFunc = EffectSkillStartFunc;

	// ������ ������ �̷����� �Ѵ�.
	VECTOR3	v3DirMon = {0,};

	if (dwOwnType == OBJECT_TYPE_PLAYER)
	{
		CUser* pUser = g_pUserHash->GetData(dwOwnIndex);
		VECTOR3_SUB_VECTOR3( &v3DirMon, &vecStart, &pUser->m_v3CurPos );		
	}
	else if(dwOwnType == OBJECT_TYPE_MONSTER)
	{
		CMonster* pMonster = g_pMonsterHash->GetData(dwOwnIndex);
		VECTOR3_SUB_VECTOR3( &v3DirMon, &vecStart, &pMonster->m_v3CurPos );		
	}
	else if(dwOwnType == OBJECT_TYPE_SKILL)
	{
		VECTOR3 vecTemp;
		vecTemp.x = (dwOwnIndex>>16)*TILE_WIDTH+TILE_WIDTH/2;
		vecTemp.z = (dwOwnIndex&0x0000ffff)*TILE_WIDTH+TILE_WIDTH/2;

		VECTOR3_SUB_VECTOR3( &v3DirMon, &vecStart, &vecTemp );
	}

	pEffectDesc->f_Radcount = (float)(atan2(v3DirMon.z, v3DirMon.x) + DEG90 );

	return pEffectDesc;	
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// �ӽ� ��¥ �ӽ�. slowboat
void MakingNullEffect(BYTE bSkillKind,VECTOR3 vPos, BYTE bJoint, BOOL bOwn)
{	
	EffectDesc*	effectDesc = g_pEffectLayer->CreateEffect(bSkillKind, bJoint, bOwn);
	
	effectDesc->byTargetObjectType[0]	= OBJECT_TYPE_EFFECT;	// 
	effectDesc->dwTargetIndex[0]		= 0;	// 
	effectDesc->dwFrameCount		=	0; //������ ī���� 0���� ����
	
	GXSetPosition( effectDesc->hEffect.pHandle, &vPos, FALSE);
	::SetAction( effectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE );
	effectDesc->hEffect.pDesc->ObjectFunc	= EffectOnceAndNormal;	
}

// �����̻����Լ� . �����Ǵ� �̻����� ������ �����ϴ� �Լ��̴�.
// ���ڴ� Ÿ���� ��ġ, ���� ���� ��ġ, ���� �ڵ�, �ִ� ���̴°���, ������, ����Ʈ ����ü ������
void SetHoamingDirection(VECTOR3* vTarget, VECTOR3* vMine, HANDLE hHandle, float fpower, DWORD dwRandom,EffectDesc*	pEffectDesc)
{
	float fNewdirection = atan2(vMine->z - vTarget->z , vMine->x - vTarget->x);
	float fCurrentDirection=0.f;
	
	// ��ǥ���� �Ÿ��� ����ﶧ�� �����̴� ������ ũ�� �ؼ� �ٷ� ã�ư��� �����.
	float fDelta = fabs(vMine->z - vTarget->z) + fabs(vMine->x - vTarget->x); 

	if (fDelta <= 200 ) fpower+=0.3f;
	else if (fDelta <= 250 ) fpower+=0.2f;
	else if (fDelta <= 300 ) fpower+=0.1f;
	
	//g_pExecutive->GXOGetDirection(hHandle,&vecAxis,&fCurrentDirection);
	fCurrentDirection = pEffectDesc->f_Radian;
	
	fNewdirection-=DEG90;
	if (fNewdirection < 0 ) fNewdirection+=DEG360;
	if (fNewdirection > DEG360 ) fNewdirection-=DEG360;
	if (fCurrentDirection < 0 ) fCurrentDirection+=DEG360;
	if (fCurrentDirection > DEG360 ) fCurrentDirection-=DEG360;
	
	// ����� �������. �� �ٷ� �����Ǵ°� �ƴ϶� �巷ū���̾� ���� ����.
	if (dwRandom)
	{
		if (pEffectDesc->dwTemp[SKILL_TEMP_HOAMINGRANDOM]) // �̹� ��������������̸�
		{
			if (pEffectDesc->dwTemp[SKILL_TEMP_HOAMINGARROW]) fCurrentDirection+=fpower;
			else fCurrentDirection-=fpower;
			//g_pExecutive->GXOSetDirection(hHandle,&vecAxis,fCurrentDirection);
			pEffectDesc->f_Radian = fCurrentDirection;
			pEffectDesc->dwTemp[SKILL_TEMP_HOAMINGFRAMECOUNT]++;
			// �� ���� �巷ū���� �󸶳� ������ ���� ��Ÿ����. ���� �����ϸ� �� ���� ����.
			if (pEffectDesc->dwTemp[SKILL_TEMP_HOAMINGFRAMECOUNT] > 2+ GetRandom(5) )
				pEffectDesc->dwTemp[SKILL_TEMP_HOAMINGRANDOM] = 0;
			
			return;
		}
		// �巷ū ���� �ʱ⿡��, �׸��� �����ϰ�, ��ǥ���� �Ÿ��� ����̻� �ɶ��� �Ѵ�.
		else if (pEffectDesc->dwFrameCount < 30 &&  GetRandom(dwRandom) == 0 && fDelta > 200)
		{
			if (pEffectDesc->dwTemp[SKILL_TEMP_HOAMINGDRUNKENCOUNT] < 2)
			{
				pEffectDesc->dwTemp[SKILL_TEMP_HOAMINGDRUNKENCOUNT]++;	// �ִ� 3�������� �巷ū ���� �����Ѵ�.
				pEffectDesc->dwTemp[SKILL_TEMP_HOAMINGFRAMECOUNT] = 0;
				pEffectDesc->dwTemp[SKILL_TEMP_HOAMINGRANDOM] = 1;
				if ( rand()%2 == 1) pEffectDesc->dwTemp[SKILL_TEMP_HOAMINGARROW] = 1; //ȸ���� �������� ���������� ����.
				else pEffectDesc->dwTemp[SKILL_TEMP_HOAMINGARROW] = 0;
			}
		}
	}
	
	// ���� ���� ����.
	if (fNewdirection >= 0 && fNewdirection <= DEG180)
	{
		if (fCurrentDirection >= fNewdirection && fCurrentDirection <= fNewdirection+DEG180)
			fCurrentDirection-=fpower;
		else fCurrentDirection+=fpower;
	}
	else
	{
		if (fNewdirection >= fCurrentDirection && fNewdirection <= fCurrentDirection+DEG180)
			fCurrentDirection+=fpower;
		else fCurrentDirection-=fpower;
	}
	
	//g_pExecutive->GXOSetDirection(hHandle,&vecAxis,fCurrentDirection);
	pEffectDesc->f_Radian = fCurrentDirection;	
}

// �巡���� �Կ��� ������ ���̾� ����Ʈ
void DragonicFireblast( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc*	pEffectDesc = (EffectDesc*)pObjDesc->pInfo;
	pEffectDesc->dwFrameCount++;	// counting
	VECTOR3 vPos, vecDest;	

	// Ÿ�� ������ ��ġ�� �˾ƺ���
	if (pEffectDesc->byTargetObjectType[0] == OBJECT_TYPE_MONSTER)	
	{	
		CMonster* pMonster = 	g_pMonsterHash->GetData(pEffectDesc->dwTargetIndex[0]); 
		
		if (!pMonster) goto lb_remove;

		vecDest = pMonster->m_v3CurPos;		
	}
	else
	{
		CUser* pUser = g_pUserHash->GetData(pEffectDesc->dwTargetIndex[0] );
		if (!pUser)	goto lb_remove;
		
		vecDest = pUser->m_v3CurPos;
	}
	
	g_pExecutive->GXOGetPosition(handle,&vPos);
	
	if (dwCurFrame == 0)
	{		
		VECTOR3	v3DirMon;
		
		VECTOR3_SUB_VECTOR3( &v3DirMon, &vecDest, &vPos );
		pEffectDesc->f_Radian = (float)(atan2(v3DirMon.z, v3DirMon.x));
		float fDist = CalcDistance( &vecDest, &vPos );
		
		pEffectDesc->f_Radcount = fDist/(g_pChrInfoEffect->GetFrameInfo(pEffectDesc->wChrNum, 0, 1, _CHRINFO_FRAME0));
	}
	
	if (g_pChrInfoEffect->GetFrameInfo(pEffectDesc->wChrNum, 0, 1, _CHRINFO_FRAME0) == dwCurFrame)
	{		
		EffectSubCommonProcess(pEffectDesc);
	}
	else if (g_pChrInfoEffect->GetFrameInfo(pEffectDesc->wChrNum, 0, 1, _CHRINFO_FRAME0) > dwCurFrame)
	{
		vPos.x += cos(pEffectDesc->f_Radian)*pEffectDesc->f_Radcount;
		vPos.z += sin(pEffectDesc->f_Radian)*pEffectDesc->f_Radcount;
		
		GXSetPosition(handle, &vPos, FALSE);
	}
	
	if( bFrameFlag == FRAME_FLAG_FINISHED)
		goto lb_remove;

	EffectSkillCameraQuake(pEffectDesc, dwCurFrame);
	return;

lb_remove:
	
	RemoveEffectDesc(pEffectDesc);
}

// �����Ǵ� ȭ�̾, ����� �߻�
void FireBallHeadFunctionBySlowboatEightDirection( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc*	pEffectDesc = (EffectDesc*)pObjDesc->pInfo;
	pEffectDesc->dwFrameCount++;	// counting
	VECTOR3 vPos, vecDest;
	
	g_pExecutive->GXOGetPosition(handle,&vPos);

	// sung-han 2005-03-02 ��ų�����ִ�(25)�϶� �Ҹ� �ȳ��� �κ� ����, 25�϶��� ����Ʈ ó�� �Լ��� Ʋ����.
	//-----------------------------------------------------------------------------------------------------
	if (pEffectDesc->dwFrameCount == 0)			// ó�� �߻� �ɶ� �Ҹ� ����
		PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_START], &pEffectDesc->vecBasePosion, FALSE);
	else if (5 == pEffectDesc->dwFrameCount)	// ���� ���鼭 �Ҹ�����
		PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_START], &pEffectDesc->vecBasePosion, FALSE);
	//-----------------------------------------------------------------------------------------------------
	
	// ó�� �ѹ��� ���õǴ� �Լ� 
	if (pEffectDesc->dwFrameCount == 1)
	{
		VECTOR3 vPosTemp;
		float	pfRadtemp;
		
		g_pExecutive->GXOGetDirection(g_pMainPlayer->m_hPlayer.pHandle,&vPosTemp,&pfRadtemp);
		pfRadtemp+=pEffectDesc->f_Radcount;
		g_pExecutive->GXOSetDirection(handle,&vPosTemp,pfRadtemp);		
	}

	// Ÿ���� ��ġ�� �˾ƺ���
	if (pEffectDesc->byTargetObjectType[0] == OBJECT_TYPE_MONSTER)	
	{	
		CMonster* pMonster = g_pMonsterHash->GetData(pEffectDesc->dwTargetIndex[0]); 		
		if (!pMonster) goto lb_remove;

		vecDest = pMonster->m_v3CurPos;		
	}
	else
	{
		CUser* pUser = g_pUserHash->GetData(pEffectDesc->dwTargetIndex[0] );
		if (!pUser)	goto lb_remove;
		
		vecDest = pUser->m_v3CurPos;
	}

	// Ÿ�� ������ ��ġ�� �˾ƺ���
	if (pEffectDesc->dwFrameCount >= 10)
	{
		// �Ÿ��� ��������� �浹!
		if (vecDest.y > -10 && fabs(vecDest.z - vPos.z) + fabs(vecDest.x - vPos.x) < 100)
		{
			// ���� �̹��� ���
			EffectDesc* effectDesc = g_pEffectLayer->CreateEffect(pEffectDesc->bEffectInfoNum,6, pEffectDesc->bOwn );
			effectDesc->dwCount = pEffectDesc->dwCount;
			memcpy(effectDesc->dwTargetIndex, pEffectDesc->dwTargetIndex, effectDesc->dwCount*sizeof(DWORD));
			memcpy(effectDesc->byTargetObjectType, pEffectDesc->byTargetObjectType, effectDesc->dwCount*sizeof(DWORD));
			
			effectDesc->dwOwnIndex = pEffectDesc->dwOwnIndex;
			effectDesc->dwOwnType = pEffectDesc->dwOwnType;
			
			GXSetPosition( effectDesc->hEffect.pHandle, &vPos, FALSE);
			::SetAction( effectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE );
			effectDesc->hEffect.pDesc->ObjectFunc = EffectOnceAndRemoveFunc;

			for(int i = 0; i < effectDesc->dwCount; ++i)
			{			
				// ���� ƨ���� ���� ����.
				if (pEffectDesc->byTargetObjectType[i] == OBJECT_TYPE_MONSTER)
				{
					VECTOR3	v3DirMon;
					VECTOR3_SUB_VECTOR3( &v3DirMon, &vPos, &vecDest);
					float fRad = (float)atan2(v3DirMon.z, v3DirMon.x);
					CMonster* pMonster = 	g_pMonsterHash->GetData(pEffectDesc->dwTargetIndex[i]); 
					if (pMonster)
					{
						pMonster->m_vecTemp = vecDest;
						pMonster->m_radian = fRad+3.14;
					}				
				}
			}
			goto lb_remove;
		}

		// ������� �����ϴٰ� �����Ǳ� �����Ѵ�
		SetHoamingDirection(&vecDest,&vPos,handle,1.0f,0,pEffectDesc);
		g_pExecutive->GXOSetDirection(handle, &g_Camera.v3AxsiY, pEffectDesc->f_Radian);				
	}

	// ��������, ���� 2�����Ӹ��� �ϳ��� �����Ѵ�.
	if (pEffectDesc->dwFrameCount % 4 == 0)
	{// ��������, ���� 2�����Ӹ��� �ϳ��� �����Ѵ�.
		EffectDesc* effectDesc = g_pEffectLayer->CreateEffect(pEffectDesc->bEffectInfoNum, GetRandom(5)+1, pEffectDesc->bOwn);
		
		GXSetPosition( effectDesc->hEffect.pHandle, &vPos, FALSE);
		::SetAction( effectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE );
		effectDesc->hEffect.pDesc->ObjectFunc = EffectOnceAndRemoveFunc;		
	}

	// 20�����ӱ��� ��������ϴٰ� �޼� �����Ѵ�.
	if (pEffectDesc->dwFrameCount > 20) g_pExecutive->GXOMoveForward(handle, 65.0f+ 2*(pEffectDesc->dwFrameCount - 20));
	else								g_pExecutive->GXOMoveForward(handle, 65.0f);

	// ������� ���Ƶ� ���� ���ϸ� ����
	if (pEffectDesc->dwFrameCount > 100)
		goto lb_remove;

	return;

lb_remove:
	RemoveEffectDesc(pEffectDesc);	
}


// �����Ǵ� ȭ�̾
void FireBallHeadFunctionBySlowboat( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc*	pEffectDesc = (EffectDesc*)pObjDesc->pInfo;

	if (pEffectDesc->dwFrameCount == 0)			// ó�� �߻� �ɶ� �Ҹ� ����
		PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_START], &pEffectDesc->vecBasePosion, FALSE);
	else if (5 == pEffectDesc->dwFrameCount)	// ���� ���鼭 �Ҹ�����
		PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_START], &pEffectDesc->vecBasePosion, FALSE);

	pEffectDesc->dwFrameCount++;	// counting
	VECTOR3 vPos, vecDest;

	g_pExecutive->GXOGetPosition(handle,&vPos);	
	
	// Ÿ�� ������ ��ġ�� �˾ƺ���
	if (pEffectDesc->byTargetObjectType[0] == OBJECT_TYPE_MONSTER)	
	{	
		CMonster* pMonster = g_pMonsterHash->GetData(pEffectDesc->dwTargetIndex[0]); 		
		if (!pMonster) goto lb_remove;

		vecDest = pMonster->m_v3CurPos;		
	}
	else
	{
		CUser* pUser = g_pUserHash->GetData(pEffectDesc->dwTargetIndex[0] );
		if (!pUser)	goto lb_remove;
		
		vecDest = pUser->m_v3CurPos;
	}	 
	
	if (pEffectDesc->dwFrameCount % 4 == 0)
	{// ��������, ���� 2�����Ӹ��� �ϳ��� �����Ѵ�.
		EffectDesc* effectDesc = g_pEffectLayer->CreateEffect(pEffectDesc->bEffectInfoNum, GetRandom(5)+1, pEffectDesc->bOwn);
		
		GXSetPosition( effectDesc->hEffect.pHandle, &vPos, FALSE);
		::SetAction( effectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE );
		effectDesc->hEffect.pDesc->ObjectFunc = EffectOnceAndRemoveFunc;
	}
	
	// �Ÿ��� ��������� �浹!
	if (vecDest.y > -10 && fabs(vecDest.z - vPos.z) + fabs(vecDest.x - vPos.x) < 100)
	{
		// ���� �̹��� ���
		EffectDesc* effectDesc = g_pEffectLayer->CreateEffect(pEffectDesc->bEffectInfoNum,6, pEffectDesc->bOwn );
		effectDesc->dwCount = pEffectDesc->dwCount;
		memcpy(effectDesc->dwTargetIndex, pEffectDesc->dwTargetIndex, effectDesc->dwCount*sizeof(DWORD));
		memcpy(effectDesc->byTargetObjectType, pEffectDesc->byTargetObjectType, effectDesc->dwCount*sizeof(DWORD));
		
		effectDesc->dwOwnIndex = pEffectDesc->dwOwnIndex;
		effectDesc->dwOwnType = pEffectDesc->dwOwnType;
		
		GXSetPosition( effectDesc->hEffect.pHandle, &vPos, FALSE);
		::SetAction( effectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE );
		effectDesc->hEffect.pDesc->ObjectFunc = EffectOnceAndRemoveFunc;

		for(int i = 0; i < effectDesc->dwCount; ++i)
		{	
			// ���� ƨ���� ���� ����.
			if (pEffectDesc->byTargetObjectType[i] == OBJECT_TYPE_MONSTER)
			{
				VECTOR3	v3DirMon;
				VECTOR3_SUB_VECTOR3( &v3DirMon, &vPos, &vecDest);
				float fRad = (float)atan2(v3DirMon.z, v3DirMon.x);
				CMonster* pMonster = 	g_pMonsterHash->GetData(pEffectDesc->dwTargetIndex[i]); 
				if (pMonster)
				{
					pMonster->m_vecTemp = vecDest;
					pMonster->m_radian = fRad+3.14;
				}			
			}
		}
		goto lb_remove;
	}

	// ������� �����ϴٰ� �����Ǳ� �����Ѵ�
	if (pEffectDesc->dwFrameCount > 3) 
	{
		SetHoamingDirection(&vecDest,&vPos,handle,1.0f,0,pEffectDesc);
		g_pExecutive->GXOSetDirection(handle, &g_Camera.v3AxsiY, pEffectDesc->f_Radian);
	}

	// 20�����ӱ��� ��������ϴٰ� �޼� �����Ѵ�.
	if (pEffectDesc->dwFrameCount > 20) g_pExecutive->GXOMoveForward(handle, 65.0f+ 2*(pEffectDesc->dwFrameCount - 20));
	else								g_pExecutive->GXOMoveForward(handle, 65.0f);

	// ������� ���Ƶ� ���� ���ϸ� ����
	if (pEffectDesc->dwFrameCount > 100)
		goto lb_remove;

	return;

lb_remove:
	RemoveEffectDesc(pEffectDesc);
}


// ȭ�̾ �̹����� �ɷ��� �ֺ����� ȸ���ϴ� �Լ�
// �ϳ��� �㸮�뿡�� ����� �����ϰ� ���� �ΰ��� y������ ���ϸ鼭 ����
void FireBallCoatFunctionBySlowboat( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc*	pEffectDesc = (EffectDesc*)pObjDesc->pInfo;
	pEffectDesc->dwFrameCount++;	// counting
	VECTOR3 vPos;
	VECTOR3 vPosEffect;
//	float	pfRad;
	vPos = g_pMainPlayer->m_v3CurPos;
	g_pExecutive->GXOGetPosition(handle,&vPosEffect);

	// ��ġ ����
	pEffectDesc->f_Radcount+=0.2f;	// ���� �ӵ�
	vPos.z+=50*sin(pEffectDesc->f_Radcount);
	vPos.x+=80*cos(pEffectDesc->f_Radcount);	// ����� ������
	vPos.y = 70.0f;				// ����, �㸮����

	if (pEffectDesc->f_Radcount > DEG360) pEffectDesc->f_Radcount = 0;

	if (pEffectDesc->dwTargetIndex[0] == 2)	// 
	{
		// ���̵� ������ ����
		vPos.y = vPosEffect.y;
		// ��ġ ����, �������� �⺻��ġ�� �� �´�
		vPos.x+=5.0f;
		vPos.z-=30.0f;
		if (pEffectDesc->f_Radcount < DEG180) vPos.y+=8.0f; //���̰� ��ȭ
		else if (pEffectDesc->f_Radcount > DEG180) vPos.y-=8.0f;
	}
	else if (pEffectDesc->dwTargetIndex[0] == 3)	// 
	{
		// ���̵� ������ ����
		vPos.y = vPosEffect.y;
		// ��ġ ����, �������� �⺻��ġ�� �� �´�
		vPos.x+=5.0f;
		vPos.z-=30.0f;
		if (pEffectDesc->f_Radcount < DEG180) vPos.y-=8.0f;
		else if (pEffectDesc->f_Radcount > DEG180) vPos.y+=8.0f;
	}

	GXSetPosition( handle, &vPos, FALSE );

	// ������ ����
	if (pEffectDesc->dwFrameCount % 8 == 1)
		MakingNullEffect(pEffectDesc->bEffectInfoNum, vPos, GetRandom(5), pEffectDesc->bOwn);

	// �ð��ֱ�. ����� 1000 ������ ������ ��������
	if (pEffectDesc->dwFrameCount > 1000)
		goto lb_remove;

	return;

lb_remove:
	RemoveEffectDesc(pEffectDesc);
}

// ȭ�̾ �̹����� �ɷ��� �ֺ����� ȸ���ϴ� �Լ�
// �ϳ��� �㸮�뿡�� ����� �����ϰ� ���� �ΰ��� y������ ���ϸ鼭 ����
void	MakeCoatFunctionBySlowboat(DWORD dwPara)
{
	// �� ���α��� �Ӹ� ���� 
	// �̰��� �㸮���� xz��鿡 �����ϰ� ȸ���ϴ� ��.
	EffectDesc*	pEffectDesc = g_pEffectLayer->CreateEffect(__SKILL_FIREMISSILE__, 0, 1);
	VECTOR3 vPos = g_pMainPlayer->m_v3CurPos;
	vPos.y+=70.0f;
	
	pEffectDesc->byTargetObjectType[0]	= OBJECT_TYPE_EFFECT;	// 
	pEffectDesc->dwTargetIndex[0]		= 1;	// 
	
	pEffectDesc->dwFrameCount		=	0; //������ ī���� 0���� ����
	pEffectDesc->f_Radcount			=	DEG90;	
	
	GXSetPosition( pEffectDesc->hEffect.pHandle, &vPos, FALSE );
	::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_LOOP );
	pEffectDesc->hEffect.pDesc->ObjectFunc	= FireBallCoatFunctionBySlowboat;
	// �������� 111��� 
	pEffectDesc = g_pEffectLayer->CreateEffect(__SKILL_FIREMISSILE__, 0, 1);
	pEffectDesc->byTargetObjectType[0]	= OBJECT_TYPE_EFFECT; // 
	pEffectDesc->dwTargetIndex[0]		= 2;	// 
	pEffectDesc->dwFrameCount		=	0; //������ ī���� 0���� ����
	pEffectDesc->f_Radcount			=	0;	
	
	GXSetPosition( pEffectDesc->hEffect.pHandle, &vPos, FALSE );
	::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_LOOP );
	pEffectDesc->hEffect.pDesc->ObjectFunc	= FireBallCoatFunctionBySlowboat;

	// �������� 1-11���
	pEffectDesc = g_pEffectLayer->CreateEffect(__SKILL_FIREMISSILE__, 0,1);
	pEffectDesc->byTargetObjectType[0]	= OBJECT_TYPE_EFFECT; // 
	pEffectDesc->dwTargetIndex[0]		= 3; // 
	
	pEffectDesc->dwFrameCount		=	0; //������ ī���� 0���� ����
	pEffectDesc->f_Radcount			=	DEG180;	
	
	GXSetPosition( pEffectDesc->hEffect.pHandle, &vPos, FALSE );
	::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_LOOP );
	pEffectDesc->hEffect.pDesc->ObjectFunc	= FireBallCoatFunctionBySlowboat;
}

EffectDesc*	CreateEffectBall(BYTE bSkillKind, float fRadian, DWORD dwOwnType, DWORD dwOwnIndex)
{
	EffectDesc* pEffectDesc = g_pEffectLayer->CreateEffect(bSkillKind, 0, dwOwnType==OBJECT_TYPE_PLAYER && g_pMainPlayer->m_dwUserIndex == dwOwnIndex);
	pEffectDesc->f_Radcount			=	fRadian;
		
	VECTOR3 vPosTemp = g_Camera.v3AxsiY;
	float	pfRadtemp = 0;
		
	if (dwOwnType == OBJECT_TYPE_PLAYER)
	{
		CUser* pUser = g_pUserHash->GetData(dwOwnIndex);
		g_pExecutive->GXOGetDirection(pUser->m_hPlayer.pHandle,&vPosTemp,&pfRadtemp);
	}
	else if (dwOwnType == OBJECT_TYPE_MONSTER)
	{
		CMonster* pMonster = g_pMonsterHash->GetData(dwOwnIndex);
		g_pExecutive->GXOGetDirection(pMonster->m_hMonster.pHandle,&vPosTemp,&pfRadtemp);
	}

	pfRadtemp+=pEffectDesc->f_Radcount;
	g_pExecutive->GXOSetDirection(pEffectDesc->hEffect.pHandle, &vPosTemp,pfRadtemp);
	
	//pEffectDesc->dwTemp[0] = 0;
	
	return pEffectDesc;
}
void EffectWall( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{	// 
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc*	pEffectDesc = (EffectDesc*)pObjDesc->pInfo;
	
	if (pEffectDesc->dwFrameCount > 0)
	{// �� �ϳ��ϳ� ���� Ÿ�̹�.
		if (pEffectDesc->dwFrameCount == 1)
		{
			::SetAction(handle, 1, 0, ACTION_ONCE);
			if (g_pEffectLayer->IsEffectShow(pEffectDesc))
			{// �׼��� 3���̴�. ���⼱ ù��° �׼ǿ� ����Ʈ ���̱�.
				if (pEffectDesc->pEffect->bID == __SKILL_MAGMAWALL__)
					g_pEffectLayer->AttachLight(pEffectDesc, 7, LightTimeOnOff);
				else
					g_pEffectLayer->AttachLight(pEffectDesc, 6, LightTimeOnOff);
				pEffectDesc->m_sLightDescEx.m_nDestTick = g_dwCurTick+50;
				
				ShowObject(handle);
			}
		}
		pEffectDesc->dwFrameCount--;
		return;
	}
	if (pEffectDesc->m_sLightDescEx.LightFunc)
		pEffectDesc->m_sLightDescEx.LightFunc(pEffectDesc);
	if (pEffectDesc->bJoint == 0 && dwCurFrame == 0)
		PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_START], &pEffectDesc->vecBasePosion, FALSE);
	
	if (pEffectDesc->bJoint == 0 && g_pChrInfoEffect->GetFrameInfo(pEffectDesc->wChrNum, 0, 1, _CHRINFO_FRAME0) == dwCurFrame) 
	{
		EffectSubCommonProcess(pEffectDesc);		
	}	
	
	if (pEffectDesc->bJoint == 0)
	{
		if( bFrameFlag == FRAME_FLAG_FINISHED || bFrameFlag == FRAME_FLAG_CHANGE_NEXT )
		{			
			EffectDesc*	effectDesc = g_pEffectLayer->CreateEffect(pEffectDesc->bEffectInfoNum, 1, pEffectDesc->bOwn);
			
			effectDesc->pEffect = pEffectDesc->pEffect;	// ù��° ����Ʈ ������ �״�� �˰� �־�� �Ѵ�.
			effectDesc->dwCount = pEffectDesc->dwCount;
			memcpy(effectDesc->dwTargetIndex, pEffectDesc->dwTargetIndex, 4*effectDesc->dwCount);
			memcpy(effectDesc->byTargetObjectType, pEffectDesc->byTargetObjectType, 4*effectDesc->dwCount);
			
			
			GXSetPosition( effectDesc->hEffect.pHandle, &pEffectDesc->vecBasePosion, FALSE );
			effectDesc->vecBasePosion = pEffectDesc->vecBasePosion;
			
			::SetAction( effectDesc->hEffect.pHandle, 1, 0, ACTION_LOOP );
			effectDesc->hEffect.pDesc->ObjectFunc = pObjDesc->ObjectFunc;
			effectDesc->hEffect.pDesc->dwStartTick = g_dwCurTick;
			effectDesc->hEffect.pDesc->dwDestTime = g_dwCurTick+1000;
			
			PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_MID], &pEffectDesc->vecBasePosion, FALSE);
			
			// �׼��� 3���̴�. ���⼱ �ι�° �׼ǿ� ����Ʈ ���̱�.
			if (pEffectDesc->pEffect->bID == __SKILL_MAGMAWALL__)
				g_pEffectLayer->AttachLight(effectDesc, 7, LightTimeOnOff);
			else
				g_pEffectLayer->AttachLight(effectDesc, 6, LightTimeOnOff);
			
			effectDesc->m_sLightDescEx.m_nDestTick = g_dwCurTick+50;
			
			goto lbl_remove;
		}
	}
	else if(pEffectDesc->bJoint  == 1)
	{
		if (g_dwCurTick >= pObjDesc->dwDestTime)
		{		
			EffectDesc*	effectDesc = g_pEffectLayer->CreateEffect(pEffectDesc->bEffectInfoNum, 2, pEffectDesc->bOwn);
			effectDesc->pEffect = pEffectDesc->pEffect;	// ù��° ����Ʈ ������ �״�� �˰� �־�� �Ѵ�.
			VECTOR3 vec;
			g_pExecutive->GXOGetPosition(handle, &vec);
			GXSetPosition( effectDesc->hEffect.pHandle, &vec, FALSE );
			
			::SetAction( effectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE );
			effectDesc->hEffect.pDesc->ObjectFunc = EffectOnceAndRemoveFunc;
			
			PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_END], &pEffectDesc->vecBasePosion, FALSE);
			
			goto lbl_remove;
		}		
	}
	else if(pEffectDesc->bJoint == 2)
	{
		if( bFrameFlag == FRAME_FLAG_FINISHED || bFrameFlag == FRAME_FLAG_CHANGE_NEXT )
			goto lbl_remove;
	}

	return;

lbl_remove:
	RemoveEffectDesc(pEffectDesc);
}

void EffectBlastQuake( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{	// 
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc*	pEffectDesc = (EffectDesc*)pObjDesc->pInfo;
	//VECTOR3 v3From;
	
	if (pEffectDesc->dwFrameCount > 0)
	{
		if (pEffectDesc->dwFrameCount == 1)
		{
			::SetAction(handle, 1, 0, ACTION_ONCE);
			if (g_pEffectLayer->IsEffectShow(pEffectDesc)) ShowObject(handle);
		}
		pEffectDesc->dwFrameCount--;
		return;
	}

	if (pEffectDesc->bJoint == 0 && dwCurFrame == 0)
		PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_START], &pEffectDesc->vecBasePosion, FALSE);
		
	if (pEffectDesc->bJoint == 0 && g_pChrInfoEffect->GetFrameInfo(pEffectDesc->wChrNum, 0, 1, _CHRINFO_FRAME0) == dwCurFrame) // ���Ͱ� Ÿ�� �������� �������̶�� �����ϸ� �ȴ�.
	{
		EffectSubCommonProcess(pEffectDesc);
		
	}
	
	if( bFrameFlag == FRAME_FLAG_FINISHED || bFrameFlag == FRAME_FLAG_CHANGE_NEXT )
	{
		RemoveEffectDesc(pEffectDesc);
	}	
}

void MonsterSKillDamageWithAction1( GXOBJECT_HANDLE handle)
{
	CMonster* pMonster = (CMonster*) handle;
	if (pMonster == NULL) return;
	
	float time	= pMonster->m_dwTemp[MONSTER_TEMP_FRAMECOUNT]/2.0;
	int act		= g_pExecutive->GXOGetCurrentMotionIndex(pMonster->m_hMonster.pHandle);

	if (act < MON_MOTION_TYPE_DEFENSEFAIL1)	return;
	if (time < 10)
	{
		// �и��� ó��.
	}

	pMonster->m_dwTemp[MONSTER_TEMP_FRAMECOUNT]++;
}

void MonsterSKillDamageWithAction2( GXOBJECT_HANDLE handle)
{
	// ���̽� ��
	CMonster*	 pMonster = (CMonster*) handle;
	if (pMonster == NULL) return;
		
	pMonster->m_dwTemp[MONSTER_TEMP_FRAMECOUNT]++;
}

void MonsterSKillDamageWithAction3( GXOBJECT_HANDLE handle)
{
	// �����ũ �¾������� �׼�
	CMonster*	 pMonster = (CMonster*) handle;
	if (pMonster == NULL) return;
	
	pMonster->m_dwTemp[MONSTER_TEMP_FRAMECOUNT]++;
}

void MonsterKillWithAction2( GXOBJECT_HANDLE handle)
{
	// ���׿��� �¾� ������ ó��.
	CMonster*	 pMonster = (CMonster*) handle;
	if (pMonster == NULL) return;

	float time = pMonster->m_dwTemp[MONSTER_TEMP_FRAMECOUNT]/2.0;	

	{
		// �и��� ó��.		
		VECTOR3 vecDest = pMonster->m_v3CurPos;
		float v0 = pMonster->m_dwTemp[MONSTER_TEMP_PUSHPOWER]*time;

		MAP_TILE* pTile = g_pMap->GetTile(vecDest.x, vecDest.z);
		if (pTile)	
		{
			if (pTile->wAttr.uOccupied != TILE_OCCUPIED_DONT_MOVE)	
			{			
				vecDest.x = pMonster->m_vecTemp.x+ cosf(pMonster->m_radian)*v0;
				vecDest.z = pMonster->m_vecTemp.z+ sinf(pMonster->m_radian)*v0;				
			}
			vecDest.y = pMonster->m_vecTemp.y+ sinf(1.57f)*pMonster->m_dwTemp[MONSTER_TEMP_PUSHPOWER1]*time-0.5*9.888*(time*time);
			vecDest.y = max(vecDest.y, 0);
			g_pExecutive->GXOSetDirection(pMonster->m_hMonster.pHandle, &g_Camera.v3AxsiY, pMonster->m_fDir+=0.5);
		}

		SetMonsterPosition(pMonster, &vecDest, 2);			
	}

	if (time && pMonster->m_v3CurPos.y == 0)
		pMonster->m_dwTemp[MONSTER_TEMP_PUSHING] = 1; // finish

	pMonster->m_dwTemp[MONSTER_TEMP_FRAMECOUNT]++;
}

void MonsterKillWithAction3( GXOBJECT_HANDLE handle)
{// ���̽��� �¾����� ���� ���ö� �׾��.
	CMonster*	 pMonster = (CMonster*) handle;
	if (pMonster == NULL) return;

	float time = pMonster->m_dwTemp[MONSTER_TEMP_FRAMECOUNT]/2.0;
	//float pfRad;
	//VECTOR3 Pos;
	
	{// �и��� ó��.
		VECTOR3 vecDest = pMonster->m_v3CurPos;
		vecDest.y = pMonster->m_vecTemp.y+ sinf(1.57f)*70*time-0.5*9.888*(time*time);
		vecDest.y = max(vecDest.y, 0);
		g_pExecutive->GXOSetDirection(pMonster->m_hMonster.pHandle, &g_Camera.v3AxsiY, pMonster->m_fDir+=0.5);
		SetMonsterPosition(pMonster, &vecDest, 2);		
	}

	if (time > 10 && pMonster->m_v3CurPos.y == 0)// ���� ��û�� äũ..
		pMonster->m_dwTemp[MONSTER_TEMP_PUSHING] = 1; // finish

	pMonster->m_dwTemp[MONSTER_TEMP_FRAMECOUNT]++;
}

void SkillDamageFunc1( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{
	CMonster* pMonster = (CMonster*)pData->pInfo;
	
	if (dwCurFrame == 0)
		pMonster->m_vecTemp = pMonster->m_v3CurPos;

	if( g_pChrInfoMonster->GetFrameInfo(pMonster->m_wModNo, 0, MON_MOTION_TYPE_DEFENSEFAIL1, _CHRINFO_FRAME0) == dwCurFrame )
	{
//		_PlaySound( MONSTER_SOUND_GETHIT, SOUND_TYPE_MONSTER, pMonster->wDamage[0] );
	}

	if( ( bFrameFlag == FRAME_FLAG_FINISHED || bFrameFlag == FRAME_FLAG_CHANGE_NEXT  ) && pMonster->m_v3CurPos.y == 0)
	{
//		pMonster->SetAction( MON_MOTION_TYPE_STAND1 );
		pMonster->SetStatus(UNIT_STATUS_NORMAL, NULL);
	}
	else
	{
		if (pMonster->WithActionFunc)
			pMonster->WithActionFunc(pMonster);
	}
}

void PlayerKillFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	CUser*	pUser = (CUser*)pData->pInfo;
	if (!pUser)	return;

	if( bFrameFlag == FRAME_FLAG_FINISHED || bFrameFlag == FRAME_FLAG_CHANGE_NEXT  )
	{
		if( pUser->m_v3CurPos.y > -5.0f )
		{
			pUser->m_v3CurPos.y -= 0.1f;
			GXSetPosition( pUser->m_hPlayer.pHandle, &pUser->m_v3CurPos, FALSE , TRUE);
			g_pExecutive->SetAlphaFlag( pUser->m_hPlayer.pHandle, (DWORD)( 180 + ( pUser->m_v3CurPos.y*2) ) );
		}
		else
		{
			pUser->m_hPlayer.pDesc->ObjectFunc = NULL;
		}
	}

	if( bFrameFlag == FRAME_FLAG_FINISHED )
	{
		if(pUser->m_bInEventDungeon)
		{
			CTDS_USER_DIE_OK packet;
			g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
		}
	}
}

void LightTimeOff(EffectDesc* pEffectDesc)
{
	if (pEffectDesc->m_sLightDescEx.m_nDestTick < g_dwCurTick)
	{
		if (pEffectDesc->m_sLightDescEx.m_handle)
			g_pEffectLayer->DetachLight(pEffectDesc);		
	}
}

void LightTimeOnOff(EffectDesc* pEffectDesc)
{
	if (pEffectDesc->m_sLightDescEx.m_nDestTick < g_dwCurTick)
	{
		pEffectDesc->m_sLightDescEx.m_nDestTick+=100;
		if (!pEffectDesc->m_sLightDescEx.m_dwTemp[LIGHT_DESCEX_TEMP_SHOW])
		{
			pEffectDesc->m_sLightDescEx.m_dwTemp[LIGHT_DESCEX_TEMP_SHOW] = 1;
			g_pExecutive->GXLDisableStaticShadow(pEffectDesc->m_sLightDescEx.m_handle);
		}
		else
		{
			pEffectDesc->m_sLightDescEx.m_dwTemp[LIGHT_DESCEX_TEMP_SHOW] = 0;
			g_pExecutive->GXLEnableStaticShadow(pEffectDesc->m_sLightDescEx.m_handle);
		}
	}
}

BOOL LoadNpcInfo()
{	
	char* pFile = NULL;
	pFile = GetFile("npctable.cdb", DATA_TYPE_MANAGER);

	if(!pFile)
	{
		return FALSE;
	}

	DWORD dwTotalRead = DecodeCDBData( pFile, g_NPCTable, DECODE_KEY, DECODE_SUBKEY);
	DWORD dwNum = dwTotalRead / sizeof(NPC_TABLE);
	dwNum;

	return TRUE;
}


BOOL SkillEvent(BYTE bLR)
{
	BOOL bSkill = FALSE;
	CUserInterface*	pUserInterface	= CUserInterface::GetInstance();
	
	if ((g_pMainPlayer->GetStatus() == UNIT_STATUS_NORMAL
		|| g_pMainPlayer->GetStatus() == UNIT_STATUS_WALKING 
		|| g_pMainPlayer->GetStatus() == UNIT_STATUS_RUNNING )
		&& g_pMainPlayer->GetSkillKind(bLR) < __SKILL_ATTACK__ 
		&& g_pMainPlayer->GetSkillKind(bLR) >= __SKILL_MANAMASTERY__)
	{		
#if IS_TAIWAN_LOCALIZING()
		if (g_pMainPlayer->GetAverageWeight() >= 100.f)
		{
			//"��ȭ�����ѵ��� �����Ͽ� �޸��Ⱑ �ȵǸ�, ������ ��� �����̰� �����մϴ�."
			DisplayMessageAdd(g_Message[ETC_MESSAGE986].szMessage, 0xffff0000);	
			return FALSE;
		}
#endif		
		
		g_Mouse.v3Mouse = GetXYZFromScreenXY( g_pGeometry, g_Mouse.MousePos.x , g_Mouse.MousePos.y
			, g_rcScreenRect.right, g_rcScreenRect.bottom);
		g_pMainPlayer->SetAction( MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );		
		g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
		g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;
		SendStopPacket();
		
		BYTE bSkillKind = g_pMainPlayer->GetSkillKind(bLR);
		Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);
		
		
		VECTOR3 vecStart; memset(&vecStart, 0, sizeof(VECTOR3));
		LPObjectDesc pObjDesc = NULL;
		if (g_hHandle && ( pObjDesc = (LPObjectDesc)g_pExecutive->GetData( g_hHandle ))
			&& (
					( pObjDesc->bType == OBJECT_TYPE_PLAYER 
						|| pObjDesc->bType == OBJECT_TYPE_MAINPLAYER) 
					|| ( pObjDesc->bType == OBJECT_TYPE_MONSTER)
				)
			)
		{// Ÿ���� ������ ������ ��ų����Ʈ�� ������ ��ġ�� ��Ȯ�ϰ� �ϱ� ���ؼ���. ��ų �����µ��ȿ� ��ü�� �����ϼ��� �����ϱ�..
			
			if( pObjDesc->bType == OBJECT_TYPE_PLAYER 
				|| pObjDesc->bType == OBJECT_TYPE_MAINPLAYER)
			{// Ŭ���Ѱ��� �÷��̾.
				CUser*	pUser = (CUser*)pObjDesc->pInfo;
				//if (pEffect->bCrime == CRIME_APPLY_ENEMY && g_pMainPlayer->IsAlliance(pUser))
				//	return FALSE;// ���� ��ų�ε� �������̸� ����
					
				if (pEffect->dwRange == 0)
				{// �ڱ� �ڽ����� ���� ������ ��.
					if ( g_pEffectLayer->IsEffectUse(bSkillKind
							, &g_pMainPlayer->m_v3CurPos 
							, g_pMainPlayer->m_dwStartSkillTick[bSkillKind]) )
					{
						vecStart = pUser->m_v3CurPos;
						goto lbl_skill_to_tile;
					}
				}
				else
				{
					BOOL bChk = FALSE;
					ListNode<SGUILD_WAR>* lpGuildWarPos = g_pGuildWarHash->GetHead();

					while(lpGuildWarPos)
					{
						LP_GUILD_WAR lpGuildWar = lpGuildWarPos->pData;

						if(lpGuildWar)
						{
							if(lpGuildWar->dwGuildId==pUser->m_dwGuildId)
							{
								bChk = TRUE;
								break;
							}
							lpGuildWarPos = lpGuildWarPos->pNext;
						}
					}
					
					if( !g_pMainPlayer->IsAlliance( pUser ) )//���̸� : hwoarang 050202 
					{
						bChk = TRUE;
					}

					if(!bChk)
					{
						if (!IsSkillUserUser(pEffect->bID, g_pMainPlayer, pUser, pUserInterface->m_nPK))
							return FALSE;
					}
					
					{// �÷��̾ ���� �ߵ��Ѵ�.
						
						// ���������̶�� ����ġ�� ���� �ϹǷ�.
						if (pEffect->IsRangeSkill())
						{
							vecStart = pUser->m_v3CurPos;
							goto lbl_skill_to_tile;
						}
						else
						{
							if (g_pEffectLayer->IsEffectUse(bSkillKind, 
								&pUser->m_v3CurPos, 
								g_pMainPlayer->m_dwStartSkillTick[bSkillKind]))
							{
								g_pMainPlayer->SetPacketSkillUser(pUser, bLR);
								bSkill = TRUE;
							}
						}
					}					
				}								
			}
			else if( pObjDesc->bType == OBJECT_TYPE_MONSTER)
			{
				CMonster*	pMonster = (CMonster*)pObjDesc->pInfo;					

				if (!IsSkillUserMon(pEffect->bID, g_pMainPlayer, pMonster))
					return FALSE;				

				if (pEffect->dwRange == 0)
				{// �ڱ� �ڽ����� ���� ������ ��.
					if (g_pEffectLayer->IsEffectUse(bSkillKind, &g_pMainPlayer->m_v3CurPos, g_pMainPlayer->m_dwStartSkillTick[bSkillKind]))
					{
						vecStart = pMonster->m_v3CurPos;
						goto lbl_skill_to_tile;
					}
				}
				else
				{
					if (
							(pEffect->bSkillTarget & TARGETTYPE_ENEMY_MONSTER)
						||	(pEffect->bSkillTarget & TARGETTYPE_FRIEND_MONSTER)
						)
					{// ���͸� ���� �ߵ��Ѵ�.
						// ���������̶�� ����ġ�� ���� �ϹǷ�.
						if (pEffect->IsRangeSkill())
						{
							vecStart = pMonster->m_v3CurPos;
							goto lbl_skill_to_tile;
						}
						else 
						{
							if (g_pEffectLayer->IsEffectUse(bSkillKind, &pMonster->m_v3CurPos, g_pMainPlayer->m_dwStartSkillTick[bSkillKind]))
							{
								g_pMainPlayer->SetPacketSkillMonster(pMonster, bLR);
								bSkill = TRUE;
							}
						}
					}
				}
			}			
		}
		else
		{
			if (pEffect->dwRange == 0)
			{// �ڱ� �ڽ����� ���� ������ ��.
				if (g_pEffectLayer->IsEffectUse(bSkillKind, &g_pMainPlayer->m_v3CurPos, g_pMainPlayer->m_dwStartSkillTick[bSkillKind]))
				{
					vecStart.x = DWORD(g_Mouse.v3Mouse.x/TILE_WIDTH)*TILE_WIDTH+TILE_WIDTH/2;
					vecStart.z = DWORD(g_Mouse.v3Mouse.z/TILE_WIDTH)*TILE_WIDTH+TILE_WIDTH/2;
					goto lbl_skill_to_tile;
				}
			}
			else
			{				
				if (pEffect->bSkillTarget & TARGETTYPE_TILE  )
				{// Ÿ���� ���� �ߵ��Ѵ�.
					vecStart.x = DWORD(g_Mouse.v3Mouse.x/TILE_WIDTH)*TILE_WIDTH+TILE_WIDTH/2;
					vecStart.z = DWORD(g_Mouse.v3Mouse.z/TILE_WIDTH)*TILE_WIDTH+TILE_WIDTH/2;
				}
lbl_skill_to_tile:
					
				if (g_pEffectLayer->IsEffectUse(bSkillKind, 
						&vecStart, 
						g_pMainPlayer->m_dwStartSkillTick[bSkillKind]))
				{
					g_pMainPlayer->SetPacketSkillTile(vecStart.x/TILE_WIDTH, vecStart.z/TILE_WIDTH, bLR);
					bSkill = TRUE;
				}
			}
		}			
	}

	return bSkill;
}

void InitSnowEffect(EffectDesc* pEffectDesc)
{
	pEffectDesc->vecBasePosion.x = g_pMainPlayer->m_v3CurPos.x;
	pEffectDesc->vecBasePosion.x += rand() % int(__GAME_SIZE_X__*1.5);
	pEffectDesc->vecBasePosion.x -= rand() % int(__GAME_SIZE_X__*1.5);
	pEffectDesc->vecBasePosion.y = 1500;
	pEffectDesc->vecBasePosion.z = g_pMainPlayer->m_v3CurPos.z;
	pEffectDesc->vecBasePosion.z += rand() % 1000;
	pEffectDesc->vecBasePosion.z -= rand() % 2000;
	GXSetPosition(pEffectDesc->hEffect.pHandle, &pEffectDesc->vecBasePosion, FALSE);
	pEffectDesc->dwFrameCount = 0;
	pEffectDesc->f_Radcount = 10+rand()%10;
	pEffectDesc->f_Radian = (rand()%2) ? (rand()%4+ rand()%10/10.f) : (-(rand()%4+rand()%10/10.f));
	pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_ALPHA] = 255;
	g_pExecutive->SetAlphaFlag(pEffectDesc->hEffect.pHandle, 255);
	if (rand() % 500 == 0)
	{
		g_fWind = (rand()%2) ? (rand()%4 + rand()%10/10.f) : (-(rand()%4 + rand()%10/10.f));
	}	
}

void CreateSnowEffect()
{
	// �Ѱ��� �����ؼ� ��������.	
	if (g_pThisDungeon->m_dwID == 4)
	{
		for(int i = 0; i < MAX_SNOW_EFFECT; ++i)
		{
			EffectDesc* pEffectDesc = g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(EFFECT_SNOW), 1, __CHR_EFFECT_NONE__);			
			
			InitSnowEffect(pEffectDesc);
			
			pEffectDesc->hEffect.pDesc->ObjectFunc = EffectSnowFunc;
			pEffectDesc->vecBasePosion.y = 1500+rand()%1000;
			GXSetPosition(pEffectDesc->hEffect.pHandle, &pEffectDesc->vecBasePosion, FALSE);
			ShowObject(pEffectDesc->hEffect.pHandle);
			::SetAction(pEffectDesc->hEffect.pHandle, 1, 0, ACTION_LOOP);
			VECTOR3 vecScale;vecScale.x = 1.5;vecScale.z = 1.5;vecScale.y = 1.5;
			g_pExecutive->GXOSetScale(pEffectDesc->hEffect.pHandle, &vecScale);
		}
		
		g_fWind = 0.0f;
	}	
}

void EffectSnowFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag )
{
	LPObjectDesc pObjDesc	= (LPObjectDesc)pData;
	EffectDesc*	pEffectDesc = (EffectDesc*)pObjDesc->pInfo;
	
	if (pEffectDesc->dwFrameCount>0)
	{
		--pEffectDesc->dwFrameCount;
		return;
	}
	
	if (pEffectDesc->vecBasePosion.y <= 5)
	{
		if (pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_ALPHA] >= 10)
		{
			g_pExecutive->SetAlphaFlag(handle, pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_ALPHA]-=10);			
		}
		else
		{
			InitSnowEffect(pEffectDesc);
		}
	}
	else
	{
		pEffectDesc->vecBasePosion.y -= pEffectDesc->f_Radcount;
		if (pEffectDesc->vecBasePosion.y < 5)pEffectDesc->vecBasePosion.y = 5;
		
		pEffectDesc->vecBasePosion.x+=((cosf(pEffectDesc->f_Radian+g_fWind))*(3+rand()%3+0.5));
		pEffectDesc->vecBasePosion.z+=((sinf(pEffectDesc->f_Radian+g_fWind))*(3+rand()%3+0.5));	
	}
	
	GXSetPosition(handle, &pEffectDesc->vecBasePosion, FALSE);
	::SetAction(handle, 1, 0, ACTION_LOOP);
}

ISoundEffect*	PlaySound(CSoundControl* pSoundControl, char* pszFileName, VECTOR3* v3Pos, BOOL bLoop)
{
	ISoundEffect * pSound = pSoundControl->GetSoundEffect(GetFile(pszFileName, DATA_TYPE_SOUND), v3Pos, bLoop);

	if (pSound)	pSound->Play(FALSE);

	return pSound;
}

// object�� ���� ���
void ObjectSoundPlay(LPObjectDesc pObjDesc, DWORD dwMotionIndex, BYTE bFrameIndex)
{
#ifdef SOUND_NEW
	char szBase[0xff];
	CSoundControl* pSoundControl = NULL;
	VECTOR3	v3Pos = {0.f};

	switch(pObjDesc->bType)
	{
	case OBJECT_TYPE_MONSTER:
		{
			CMonster*		pMonster	= (CMonster*)pObjDesc->pInfo;
			ChrInfo* pChrInfo = g_pChrInfoMonster->GetChrInfo(pMonster->m_wModNo);
			if (!pChrInfo)	return;
			v3Pos = pMonster->m_v3CurPos;
			SSOUNDINFO* pSoundInfo = g_pChrInfoMonster->GetSoundInfo(pMonster->m_wModNo, 0, dwMotionIndex);
			pSoundControl = pMonster->m_pSoundControl;
			
			if (pSoundInfo[0].dwFrame == bFrameIndex)
			{
				wsprintf(szBase, "%d.wav",pSoundInfo[0].dwSoundNum);
				PlaySound(pSoundControl, szBase, &v3Pos, FALSE);
				
			}

			if(pSoundInfo[1].dwFrame == bFrameIndex)
			{
				wsprintf(szBase, "%d.wav",pSoundInfo[1].dwSoundNum);
				PlaySound(pSoundControl, szBase, &v3Pos, FALSE);
			}

			if(pSoundInfo[2].dwFrame == bFrameIndex)
			{
				if (GetRandom(750) == 1)
				{
					wsprintf(szBase, "%d.wav",pSoundInfo[2].dwSoundNum);
					PlaySound(pSoundControl, szBase, &v3Pos, FALSE);
				}
				else if (GetRandom(751) == 1)
				{
					wsprintf(szBase, "%d.wav",pSoundInfo[3].dwSoundNum);
					PlaySound(pSoundControl, szBase, &v3Pos, FALSE);
				}
			}

			if(pSoundInfo[4].dwFrame == bFrameIndex)
			{
				if (GetRandom(750) == 1)
				{
					wsprintf(szBase, "%d.wav",pSoundInfo[4].dwSoundNum);
					PlaySound(pSoundControl, szBase, &v3Pos, FALSE);
				}
				else if (GetRandom(751) == 1)
				{
					wsprintf(szBase, "%d.wav",pSoundInfo[5].dwSoundNum);
					PlaySound(pSoundControl, szBase, &v3Pos, FALSE);
				}
			}
			
			if (pSoundInfo[6].dwFrame == bFrameIndex)
			{
				if (GetRandom(2))
				{
					wsprintf(szBase, "%d.wav",pSoundInfo[6].dwSoundNum);
					PlaySound(pSoundControl, szBase, &v3Pos, FALSE);
				}
				else
				{
					wsprintf(szBase, "%d.wav",pSoundInfo[7].dwSoundNum);
					PlaySound(pSoundControl, szBase, &v3Pos, FALSE);
				}
			}
			if (pSoundInfo[8].dwFrame == bFrameIndex)
			{
				if (GetRandom(2))
				{
					wsprintf(szBase, "%d.wav",pSoundInfo[8].dwSoundNum);
					PlaySound(pSoundControl, szBase, &v3Pos, FALSE);
				}
				else
				{
					wsprintf(szBase, "%d.wav",pSoundInfo[9].dwSoundNum);
					PlaySound(pSoundControl, szBase, &v3Pos, FALSE);
				}
			}
		}break;
	default:
		return;
	}
	
#endif
}


int		IsSkillMon(BYTE bSkillKind, CMonster* pMonster)
{
	return (pMonster->m_dwHP && pMonster->GetStatus() != UNIT_STATUS_DEAD);
}


int		IsSkillUser(BYTE bSkillKind, CUser* pUser)
{
	BYTE bUnitStatus = UNIT_STATUS_DEAD; 
	return	(bUnitStatus != (pUser->GetStatus()	&& __SKILL_TREEAGION__ != bSkillKind ) ||
			(bUnitStatus == pUser->GetStatus() 	&& __SKILL_TREEAGION__ == bSkillKind));
}

int		IsSkillUserUser(BYTE bSkillKind, CMainUser *pOwnUser, CUser* pTargetUser, BYTE bPK)
{
	int nResult = 1;
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);

	if ( (pEffect->bCrime == CRIME_APPLY_ENEMY || 
		pEffect->bCrime == CRIME_APPLY_ENEMY_FRIENDLY_MONSTER)
		&& !bPK)
		nResult = 0;//�������ϴ� �����̶�� pk��尡 Ȱ��ȭ ���־�� �Ѵ�.
	else if (pEffect->bCrime == CRIME_APPLY_FRIENDLY &&
		!(pEffect->bSkillTarget & TARGETTYPE_ENEMY_PLAYER)
		&& (!pOwnUser->IsAlliance(pTargetUser) 
		&& pEffect->bSkillTarget & TARGETTYPE_FRIEND_PLAYER)
		)
		nResult = 0;//�Ʊ��� �����ϴ� �����̴�.
	else if (pEffect->bCrime == CRIME_APPLY_ENEMY && (pOwnUser->IsAlliance(pTargetUser)))
		nResult = 0;// �������ϴ� �����ε� �츮��, �Ÿ���γ�� ����.
	else if (!IsSkillUser(bSkillKind, pTargetUser))
		nResult = 0;// �׾����� ��ų ������ �ִ°��� ���� Ʈ�������»��̴�.

	return nResult;
}

int		IsSkillUserMon(BYTE bSkillKind, CMainUser *pOwnUser, CMonster* pTargetMonster)
{
	int nResult = 1;
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);
	
	if (!pTargetMonster)	nResult = 0;
	else if (!IsSkillMon(bSkillKind, pTargetMonster))
		nResult = 0;
	else if (pEffect->bCrime == CRIME_APPLY_ENEMY && pOwnUser->IsAlliance(pTargetMonster))
		nResult = 0;// �������ϴ� �����ε� �츮���� ������.
	else if (!pOwnUser->IsAlliance(pTargetMonster) && pTargetMonster->m_dwLordIndex && pEffect->bCrime == CRIME_APPLY_ENEMY && !(CUserInterface::GetInstance()->m_nPK == __PK_MODE__))
		nResult = 0;
	else if (//pEffect->bCrime == CRIME_APPLY_FRIENDLY &&
		!(pEffect->bSkillTarget & TARGETTYPE_ENEMY_MONSTER)
		&& (!pOwnUser->IsAlliance(pTargetMonster) 
		&& pEffect->bSkillTarget & TARGETTYPE_FRIEND_MONSTER)
		)
		nResult = 0;

	return nResult;
}