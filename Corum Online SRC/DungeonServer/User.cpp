#include "stdafx.h"
#include "User.h"
#include "Dungeon.h"
#include "DungeonLayer.h"
#include "ClassMemoryPool.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "GameSystemNative.h"
#include "DUNGEON_DATA_EX.h"
#include "Map.h"
#include "Monster.h"
#include "Section.h"
#include "OwnServer.h"
#include "DBProc.h"
#include "MonsterManager.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "recvmsg.h"
#include "ItemNativeManager.h"
#include "MsgProc.h"
#include "MsgProc_for_jjw6263.h"
#include "EventDungeonManager.h"
#include "ItemAttrLayer.h"


CUser::CUser()
{
	CreateResource();
	m_bUserType = USER_TYPE_NORMAL;
}


CUser::~CUser()
{
	RemoveResource();
}

void* CUser::operator new(size_t size)
{
	CUser* pNew = (CUser*)g_pUserClassPool->Allocation();
	pNew->SetObjectType(OBJECT_TYPE_PLAYER);
	return pNew;
}


void CUser::operator delete(void* pDeleteUser)
{	
	g_pUserClassPool->FreeAllocation(pDeleteUser);	
}


void CUser::CreateResource()
{
	NewUsingStatusEffectList();

	if (!m_pUsingMagicArrayList)
		m_pUsingMagicArrayList	= new CVoidList;

	if (!m_pUsingItemList)
		m_pUsingItemList		= new CVoidList;

	if (!m_pItemNativeManager)
	{
		m_pItemNativeManager	= new CItemNativeManager;

		m_pItemNativeManager->Initialize(::GetItemNativePlayerBufferCount()
								, ITEM_NATIVE_PLAYER_START);

		m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_EQUIP
								, m_pEquip
								, sizeof(CItem)
								, MAX_EQUIP_POOL
								, ITEM_NATIVE_TYPE_REAL);

		m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_INV_LARGE
								, m_pInv_Large
								, sizeof(CItem)
								, MAX_INV_LARGE_POOL
								, ITEM_NATIVE_TYPE_REAL);

		m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_INV_SMALL
								, m_pInv_Small
								, sizeof(CItem)
								, MAX_INV_SMALL_POOL
								, ITEM_NATIVE_TYPE_REAL);

		m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_INV_GUARDIAN
								, m_pInv_Guardian
								, sizeof(CItem)
								, MAX_INV_GUARDIAN_POOL
								, ITEM_NATIVE_TYPE_REAL);

		m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_BELT
								, m_pBelt
								, sizeof(CItem)
								, MAX_BELT_POOL
								, ITEM_NATIVE_TYPE_REAL);

		m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_BANK_LARGE
								, m_pBank_Large
								, sizeof(CItem)
								, MAX_BANK_LARGE_POOL
								, ITEM_NATIVE_TYPE_REAL);
		
		m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_BANK_SMALL
								, m_pBank_Small
								, sizeof(CItem)
								, MAX_BANK_SMALL_POOL
								, ITEM_NATIVE_TYPE_REAL);

		m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_TRADE
								, m_pTradeItem
								, sizeof(ITEM_NATIVE_INFO)
								, MAX_TRADE_POOL
								, ITEM_NATIVE_TYPE_VIRTUAL);

		m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_UPGRADE
								, m_pUpgradeItem
								, sizeof(ITEM_NATIVE_INFO)
								, MAX_UPGRADE_POOL
								, ITEM_NATIVE_TYPE_VIRTUAL);

		m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_MAKING
								, m_pMakingItem
								, sizeof(ITEM_NATIVE_INFO)
								, MAX_MAKING_POOL
								, ITEM_NATIVE_TYPE_VIRTUAL);

		m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_GUARDIAN
								, &m_ItemGuardian
								, sizeof(CItem)
								, MAX_GUARDIAN_POOL
								, ITEM_NATIVE_TYPE_REAL);
		if( IsUltra() )
		{
			m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_PLAYERSHOP
									, m_sPlayerShopLink.cItem
									, sizeof(ITEM_NATIVE_INFO)
									, MAX_PLAYER_SHOP_INV
									, ITEM_NATIVE_TYPE_VIRTUAL);
		}
		else
		{		
			m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_PLAYERSHOP
									, m_sPlayerShop.cItem
									, sizeof(ITEM_NATIVE_INFO)
									, MAX_PLAYER_SHOP_INV
									, ITEM_NATIVE_TYPE_VIRTUAL);
		}
	}

	if(!m_pVirtualLinkItemNativeHash)
	{
		m_pVirtualLinkItemNativeHash = new CVirtualLinkItemNativeHash;
		m_pVirtualLinkItemNativeHash->InitializeHashTable(MAX_TRADE_POOL,MAX_TRADE_POOL);
	}
}

void CUser::RemoveResource()
{		
	RemoveAllSkill();
	
	if (m_pUsingMagicArrayList)
	{
		POSITION_ pos = m_pUsingMagicArrayList->GetHeadPosition();

		while(pos)
		{
			ITEM_MAGICARRAY_DESC* pItem_MagicArray_Desc = (ITEM_MAGICARRAY_DESC*)m_pUsingMagicArrayList->GetNext(pos);
			DetachMagicArray(pItem_MagicArray_Desc);
		}

		delete m_pUsingMagicArrayList;
		m_pUsingMagicArrayList = NULL;
	}

	if (m_pItemNativeManager)
	{
		delete m_pItemNativeManager;
		m_pItemNativeManager = NULL;
	}
	
	if (m_pUsingItemList)
	{
		m_pUsingItemList->RemoveAll();
		delete m_pUsingItemList;
		m_pUsingItemList = NULL;
	}

	if (m_posRemoveList)
	{
		GetCurDungeon()->GetRemovePCList()->RemoveAt(m_posRemoveList);
		m_posRemoveList = NULL;
	}
	
	if(m_pVirtualLinkItemNativeHash)
	{
		m_pVirtualLinkItemNativeHash->Destroy(TRUE);
		delete m_pVirtualLinkItemNativeHash;
		m_pVirtualLinkItemNativeHash = NULL;
	}
}

BOOL CUser::IsCurDungeonVillageInLive()
{
	return ( DUNGEON_TYPE_VILLAGE == GetCurDungeon()->GetDungeonDataEx()->GetDungeonType() );
}

BOOL CUser::IsCurDungeonEventInLive()
{
	return ( DUNGEON_TYPE_EVENT == GetCurDungeon()->GetDungeonDataEx()->GetDungeonType() );
}

BOOL CUser::IsCurDungeonMatchInLive()
{
	return ( m_sMatchDescInfo.m_bMatching );
}

void CUser::InsertToDungeon(CDungeon* pDungeon)
{	
	SetCurDungeon(pDungeon);
	CreateResource();
	SetStatus(UNIT_STATUS_NORMAL);		
}


// ������ ������ ���m�´�.
void CUser::CallBackCompulsiveDisconnectUser()
{
	OnDisconnectUser(m_dwConnectionIndex);

	g_pNet->CompulsiveDisconnectUser(m_dwConnectionIndex);								
	g_pNet->SetUserInfo(m_dwConnectionIndex, NULL);

	ListNode<CUser>* pDeleteUserPos = g_pUserHash->GetHead();
	
	while(pDeleteUserPos)
	{
		CUser* pDeleteUser = pDeleteUserPos->pData;
		
		if(pDeleteUser->GetID() == GetID())
		{
			Log(LOG_NORMAL, "CmdKickUser OnDisconnectUser Fail(Name : %s, UserIndex : %u, ConnectionIndex = %u)"
				, pDeleteUser->m_szName, pDeleteUser->GetID(), pDeleteUser->m_dwConnectionIndex);

			g_pUserHash->Delete(pDeleteUser, pDeleteUser->GetID());
			
			delete pDeleteUser;
			
			pDeleteUser = NULL;
		}

		pDeleteUserPos = pDeleteUserPos->pNext;
	}
}


// ������ �m�� �̺�Ʈ�� �ɾ��.
void CUser::SetEventCompulsiveDisconnectUser()
{
	if(m_posRemoveList)
	{
		m_dwTemp[USER_TEMP_USERDYINGSTARTTICK]		= g_dwTickCount;
		m_dwTemp[USER_TEMP_FLAG_UPDATEUSERFORAI]	= 1;		
	}

	m_dwTemp[USER_TEMP_GOTOWORLD_OUTSTAUS] = 
		(IsCurDungeonSiege() && GetCurLayerIndex() && GetCurLayerIndex() != INVALID_DUNGEON_LAYER) ?
		GOTOWORLD_STATUS_UPPER_LAYER : GOTOWORLD_STATUS_GOTOWORLD;
}

void CUser::SendStopPacket(ENUM_SEND_MODE eSendMode) const
{
	if(m_enGodMode != GODMODE_STATUS_MAPLOADING)
	{
		DSTC_STOP	Stop;
		Stop.dwUserIndex	= GetID();
		Stop.v2StopPos		= *GetCurPosition();
		
		switch (eSendMode)
		{
		case SEND_MODE_BROADCAST_SECTION:
			GetCurDungeonLayer()->BroadCastSectionMsg( (char*)&Stop, Stop.GetPacketSize(), GetPrevSectionNum() );
			break;
		case SEND_MODE_BROADCAST_EXCEPT_SENDER:
			GetCurDungeonLayer()->BroadCastSectionMsg( (char*)&Stop, Stop.GetPacketSize(), this );
			break;
		case SEND_MODE_ONE_PLAYER:
			NetSendToUser( m_dwConnectionIndex, (char*)&Stop, Stop.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
			break;
		}
	}
}

void CUser::SetStatus( BYTE bStatus, BOOL bCompulsion)
{
	if( !GetCurDungeon() )
		return;

	CDungeonLayer* pLayer = GetCurDungeonLayer();
	if( !pLayer )
		return;

	if(GetUnitStatus()==UNIT_STATUS_PLAYER_REST	&& GetUnitStatus()!=bStatus)
	{
		if(m_byRest==1)
			m_byRest = 0;
	}	
	
	if( bStatus != GetUnitStatus() )
	{		
		switch(bStatus)
		{
			// �Ѿ�� ���°� �״� ���¶��
			case UNIT_STATUS_DEAD:
			{
				if (!m_posRemoveList)
				{

					if (!m_sMatchDescInfo.m_bMatching)
					{
						Log(LOG_IMPORTANT, "[Before Die] Name : %s Criminal : %d, Level : %d exp : %u, decExpItem : %d, IncExpItem : %d"
							, m_szName, m_wCriminalPropensity,GetLevel()
							, GetExp(), GetItemAttr(ITEM_ATTR_DEAD_EXP_LOSS_DECREASE)
							, GetItemAttr(ITEM_ATTR_DEAD_EXP_LOSS_INCREASE));

#ifdef CHINA_LOCALIZING
// �߱��� XP ����(X),  HP�� ���� ����, Karz ���� : �ִ��� 2005.1.19
						SetHP(GetMaxHP()/2);
//						
#else
						SubExp( GetExpDying() );

						
#endif
						Log(LOG_IMPORTANT, "[After Die] Name : %s Criminal : %d, Level : %d exp : %u"
							, m_szName, m_wCriminalPropensity, GetLevel(), GetExp());

						char szQuery[255]={0,};
						wsprintf( szQuery, "CZP_DeathCount '%s', 1", m_szName );
						g_pDb->THExecuteSQL( szQuery, FALSE, 0, NULL, GAME_DB );

						m_dwTemp[USER_TEMP_USERDYINGSTARTTICK] = 0;
					}
					else
					{
						m_dwTemp[USER_TEMP_USERDYINGSTARTTICK]		= g_dwTickCount+15000;
						m_dwTemp[USER_TEMP_FLAG_UPDATEUSERFORAI]	= 1;						
					}

					if( pLayer->m_pTeamMatch )//: hwoarang 050202
					{//����� ���̸� �����긮��Ʈ�� �ȳ־�
					}
					else
					{
						m_posRemoveList = GetCurDungeon()->GetRemovePCList()->AddHead(this);
					}
					
					if (IsSummonGuardian(0))
					{
						UnSummonGuardian(0);
						CItem* pGuardianItem = &m_pInv_Guardian[0];
						
						if(GUARDIAN_ITEM_TYPE_EGG == pGuardianItem->GetBaseItem()->BaseItem_Guardian.bType)
						{
							pGuardianItem->SetID(pGuardianItem->GetBaseItem()->BaseItem_Guardian.wBrokenItemID);
							::SendToUpdate_GuardianItem(m_dwConnectionIndex, pGuardianItem);
						}						
					}

					CDungeonLayer* pLayer = GetCurDungeonLayer();
					if (pLayer)
					{
						pLayer->GetMap()->SetTileOccupied( GetCurMapTile()->wIndex_X, GetCurMapTile()->wIndex_Z
							, TILE_EMPTY, this);
					}
					
					::UpdateUserForAI( this );
					GetCurDungeon()->RemoveAllUserMonster( this );
					
					if (m_byRest)
					{
						SetRest(false);
					}					
				}

				SetUnitStatus((UNIT_STATUS)bStatus);
			}
			break;
		}

		if (bCompulsion || GetUnitStatus() != UNIT_STATUS_DEAD)
		{		
			if (UNIT_STATUS_WALKING == bStatus || UNIT_STATUS_RUNNING == bStatus)
			{
				if(m_enGodMode==GODMODE_STATUS_MAPLOADING)
					return;
			}

			SetUnitStatus((UNIT_STATUS)bStatus);
		}
	}	
}


DWORD CUser::GetKillMonCount()
{
	return m_dwKillMonCount;
}


void CUser::SetKillMonCount(DWORD dwKillMonCount)
{
	m_dwKillMonCount = dwKillMonCount;		
}

void CUser::BadModeReleaseTimeProcess()
{
	// �������忡 ���� �¾Ƶ� �Ҹ����� ���°� Ǯ����.
	if( m_dwTemp[USER_TEMP_BADSTARTTICK] && g_dwTickCount >= m_dwTemp[ USER_TEMP_BADSTARTTICK ] )
	{
		if (GetCurDungeon())
		{
			SetBad(false, 0, 0);
			CDungeonLayer* pLayer = GetCurDungeonLayer();
			if (pLayer)
			{
				DSTC_PLAYER_BAD packet;
				packet.byBad		= m_sPKDescInfo.m_bBad;
				packet.dwUserIndex	= GetID();
				pLayer->BroadCastSectionMsg((char*)&packet, packet.GetPacketSize(), GetPrevSectionNum());
			}						
		}
	}
}

void CUser::GodModeReleaseTimeProcess()
{
	if (m_dwTemp[USER_TEMP_LASTGODMODETICK] &&	g_dwTickCount > m_dwTemp[USER_TEMP_LASTGODMODETICK] )
	{
		SetGodMode(GODMODE_STATUS_RELEASE);
	}
}

void CUser::EffectSkillTimeProcess()
{
	if (g_dwTickCount > m_dwTemp[USER_TEMP_STATUSSKILLTICK]) 	
	{			
		POSITION_ pos = GetUsingStatusEffectList()->GetHeadPosition();		
		while(pos)
		{
			EffectDesc* pEffectDesc = (EffectDesc*)GetUsingStatusEffectList()->GetNext(pos);	
			
			switch(pEffectDesc->pEffect->bType)
			{
			case TYPE_DRIVE:// mp �Ҹ�ɶ����� ����.
			case TYPE_TIMEZERO:
				{
					if (timeGetTime() >= pEffectDesc->dwDestTick)
					{
						// �ð��� �� �Ǿ���.							
						DetachSkill(pEffectDesc);
						continue;
					}

					// ��ų�� ó���Ѵ�.
					switch( pEffectDesc->pEffect->bID )
					{
					case __SKILL_POSIONCLOUD__:
					case __SKILL_POISONING__:				// �ߵ� ó��.
						{
							// ���ʸ��� �ѹ���.
							if( g_dwTickCount - pEffectDesc->dwLastUpdateTick > 1000 )	
							{
								int	dwCount = ( g_dwTickCount - pEffectDesc->dwLastUpdateTick ) / 1000;
								int	iValue = pEffectDesc->dwTemp[EFFECT_DESC_TEMP_POISON_DAMAGE_MIN]*dwCount;
								
								if( (int)GetHP() - iValue < 1 )
								{
									SetHP(1);
									DetachSkill(pEffectDesc);
									pEffectDesc = NULL;
								}
								else
								{
									SetHP(WORD(GetHP() - iValue));
									pEffectDesc->dwLastUpdateTick = g_dwTickCount;
								}
							}
						}
						break;
					}
				}
				break;								
			}
		}

		pos = m_pUsingMagicArrayList->GetHeadPosition();
		
		while(pos)
		{
			ITEM_MAGICARRAY_DESC* pItemMagicArrayDesc = (ITEM_MAGICARRAY_DESC*)m_pUsingMagicArrayList->GetNext(pos);
						
			if (g_dwTickCount - pItemMagicArrayDesc->dwTemp[ITEM_MAGICARRAY_DESC_TEMP_STARTTICK] >=
				pItemMagicArrayDesc->pItem->BaseItem_MagicArray.dwDur1)
			{
				DetachMagicArray(pItemMagicArrayDesc);				
			}
		}

		// 1�ʸ���
		m_dwTemp[USER_TEMP_STATUSSKILLTICK] = g_dwTickCount + 1000;	
	}
}


void CUser::Update()
{
	__try
	{
		UsingItemListProcess();
		BadModeReleaseTimeProcess();
		GodModeReleaseTimeProcess();
		
		if (!GetUsingStatusEffectList())
			__leave;
		
		switch( GetUnitStatus() )
		{
			case UNIT_STATUS_NORMAL:
				break;
			case UNIT_STATUS_WALKING:
			case UNIT_STATUS_RUNNING:
				Move();
				break;
			case UNIT_STATUS_DEAD:
				__leave;
			case UNIT_STATUS_CASTING:
				{
					if (g_dwTickCount > m_dwTemp[USER_TEMP_CASTINGDESTTICK]) 	
					{
						CTDS_SKILL packet;
						
						packet.bOwnType			= m_sCastingInfo.bOwnType;
						packet.bPK				= m_sCastingInfo.bPK;
						packet.bSkillKind		= m_sCastingInfo.bSkillKind;
						packet.bTargetType		= m_sCastingInfo.bTargetType;
						packet.dwTargetIndex	= m_sCastingInfo.dwTargetIndex;
						packet.dwTime			= g_dwTickCount-m_dwTemp[USER_TEMP_CASTINGSTARTTICK];
						packet.wTileIndex_X		= m_sCastingInfo.wTileIndex_X;
						packet.wTileIndex_Z		= m_sCastingInfo.wTileIndex_Z;
						CmdSkill(m_dwConnectionIndex, (char*)&packet, sizeof(packet));
						SetStatus(UNIT_STATUS_NORMAL);
					}
				}
				break;
		}		
			
		EffectSkillTimeProcess();
		Recover5SecPer();		
	}
	__finally
	{
		// ���� DB ������Ʈ �ð��̶�� DB�� ������ ������ ����Ѵ�. �׸��� �׵��� �ö� ���ǵ��� �ʱ�ȭ.
		if( g_dwTickCount - m_dwTemp[ USER_TEMP_LASTDBUPDATE ] >= 1000 * 60 * 3 )
		{
			m_dwTemp[ USER_TEMP_HACK ]			= 0;
			m_dwTemp[ USER_TEMP_LASTDBUPDATE ]	= g_dwTickCount;
			
			UpdateUserDBThread(this);
		}
	}
}


void CUser::Recover5SecPer()
{
	// �����ð� ������ HP�� MP�� ȸ���ȴ�. 5�� ����.
	if( g_dwTickCount - m_dwTemp[ USER_TEMP_HEALTICKLAST ] >= 5000 )	
	{
		if(m_wPerWeight <=  (WEIGTH_80PER_OVER - 1) )
		{					
			// HP
			DWORD	dwHealCount = ( g_dwTickCount - m_dwTemp[ USER_TEMP_HEALTICKLAST ] ) / 1000;
			m_dwTemp[ USER_TEMP_HEALTICKLAST ] = g_dwTickCount;

			if( dwHealCount > 8 )
			{
				dwHealCount = 8;
			}
			
			if( GetHP() < GetMaxHP() )
			{
				SetHP(WORD(GetHP() + m_dwHealHPSec));
			}

			if (GetClass() != CLASS_TYPE_WARRIOR)
			{
				// ����� �ö��� ���Ѵ�.
				if( GetSP() < GetMaxSP() )
				{
					SetSP(WORD(GetSP()+m_dwHealMPSec));
				}
			}
		}
	}
}


float CUser::GetMoveSpeedByStatus() const
{
	float fSpeed = 0.f;
	// sung-han 2005-01-14 ���� ��ų�� �ɷ��� �ȱ�� �̵� ������ ���� ����
	//------------------------------------------------------------------------------------
	//if( GetUnitStatus() == UNIT_STATUS_WALKING )	fSpeed = 0.20f;
	//else if(GetUnitStatus() == UNIT_STATUS_RUNNING) fSpeed = (m_fMoveSpeed / 1000.f);
	if( GetUnitStatus() == UNIT_STATUS_WALKING )
	{
		if( m_fMoveSpeed == 0 ) 
			fSpeed = 0;
		else
			fSpeed = 0.20f;
	}
	else if(GetUnitStatus() == UNIT_STATUS_RUNNING) fSpeed = (m_fMoveSpeed / 1000.f);
	//------------------------------------------------------------------------------------

	float fPerSpeed = 0.0f;

	if(GetUnitStatus() == UNIT_STATUS_RUNNING)
	{
		if(m_wPerWeight >= WEIGTH_80PER_OVER)
			fPerSpeed = (m_fMoveSpeed / 3000.f);
	}	
	
	return (fSpeed - fPerSpeed);
}


float CUser::GetMovePerMS()
{
	// ms�� ����� 1�ʷ� �ٲ۴�.
	return float( g_dwTickCount - GetLastMoveTick() ) * (GetMoveSpeedByStatus());	
}


void CUser::Move()
{	
	CDungeonLayer*	pLayer = GetCurDungeonLayer();
	if( !pLayer )							return;
	if(!GetHP())							return;
	if(GetUnitStatus() == UNIT_STATUS_DEAD)	return;
	
	float dwMoveTime	= GetMovePerMS();
	SetLastMoveTick(g_dwTickCount);

	VECTOR2 v2Move = 
	{
		( GetCurDirection()->x * dwMoveTime ),
		( GetCurDirection()->y * dwMoveTime )
	};

	VECTOR2 v2Start = *GetCurPosition();
	v2Start.x += v2Move.x;
	v2Start.y += v2Move.y;
	
	CMap* pMap = pLayer->GetMap();
	
	MAP_TILE* pTile = pMap->GetTile( v2Start.x, v2Start.y );
	
	// ���� �̵��� ������ ������ �ƴ϶��.
	if( !pTile )
	{
		SetStatus( UNIT_STATUS_NORMAL );
		SendStopPacket(SEND_MODE_BROADCAST_SECTION);
		return;
	}
	
	if( pTile->wAttr.uSection == 0 )
	{
		SetStatus( UNIT_STATUS_NORMAL );
		SendStopPacket(SEND_MODE_BROADCAST_SECTION);
		return;
	}

	// Ÿ���� �ٲ������ üũ�Ѵ�.
	if( ( GetCurMapTile()->wIndex_X != pTile->wIndex_X ) || ( GetCurMapTile()->wIndex_Z != pTile->wIndex_Z ) )
	{
		// ���� ���� �� Ÿ���� ������ Ÿ���̸� ���� �Ѵ�.
		if( pTile->wAttr.uOccupied != TILE_EMPTY )
		{
			SetStatus( UNIT_STATUS_NORMAL);
			SendStopPacket(SEND_MODE_BROADCAST_SECTION);
			return;
		}
	}

	SetCurPosition(&v2Start);
	
	if( GetPrevSectionNum() != pTile->wAttr.uSection )
	{
		int					i=0;
		BYTE				IntersectSection[MAX_SECTION_LINK_NUM]={0,};
		CUser*				pDisAppearUser=0;
		CMonster*			pDisAppearMonster=0;
		POSITION_			UserPos=0;
		POSITION_			MonsterPos=0;
		CSection*			pSection=0;
		DSTC_CHANGE_SECTION	Section;
		DSTC_DISAPPEAR		DisAppear;
		BYTE				oldSectionNum = GetPrevSectionNum();
			
		pLayer->RemoveUserSection( this );
		pLayer->InsertUserSection( this,  pTile->wAttr.uSection );
		
		pLayer->IntersectSection( GetPrevSectionNum(), oldSectionNum, IntersectSection );

		// �������� ������ �����鿡�� ���� �޼����� ������.
		DisAppear.dwUserIndex		= GetID();
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
				Section.pdwDisAppearIndex[Section.bDisAppearUserCount++] = pDisAppearUser->GetID();
			}

			MonsterPos = pSection->m_pMonsterList->GetHeadPosition();
			while( MonsterPos )
			{
				pDisAppearMonster = (CMonster*)pSection->m_pMonsterList->GetNext( MonsterPos );
				Section.pdwDisAppearIndex[Section.bDisAppearUserCount + Section.bDisAppearMonsterCount++] = pDisAppearMonster->GetID();
			}
		}
		
		Section.wChageSectionNum = GetPrevSectionNum();
		NetSendToUser( m_dwConnectionIndex, (char*)&Section, Section.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		
		// �ٲ� ���ǿ��� ���� ���� ������ �޴´�.
		pLayer->BroadCastIntersectSectionInfo( oldSectionNum, GetPrevSectionNum(), this );
	}

	// ���� �ٸ� ������ �̵��ؾ��Ҷ����..
	for( int i=0; i<pLayer->m_wMoveSpotNum; i++ )
	{
		if (IsChangeLayer(pTile->wIndex_X, pTile->wIndex_Z, (BYTE)i))
		{
			GetCurDungeon()->ChangeLayerUser( this, GetCurLayerIndex(), (BYTE)i );
			return;
		}
	}
}

DWORD CUser::GetSumItemDex() const
{
	return GetItemAttr(ITEM_ATTR_DEX) + GetItemAttr( ITEM_ATTR_ALLSTATUS );
}


DWORD CUser::GetSumItemEgo() const
{
	return GetItemAttr( ITEM_ATTR_EGO ) + GetItemAttr( ITEM_ATTR_ALLSTATUS );
}


DWORD CUser::GetSumItemInt() const
{
	return GetItemAttr( ITEM_ATTR_INT ) + GetItemAttr( ITEM_ATTR_ALLSTATUS );
}


DWORD CUser::GetSumItemStr() const
{
	return GetItemAttr( ITEM_ATTR_STR ) + GetItemAttr( ITEM_ATTR_ALLSTATUS );
}


DWORD CUser::GetSumItemVit() const
{
	return GetItemAttr( ITEM_ATTR_VIT ) + GetItemAttr( ITEM_ATTR_ALLSTATUS );
}


DWORD CUser::GetNewestEgo() const
{
	return GetEgo() + GetSumItemEgo();
}


DWORD CUser::GetNewestStr() const
{
	return GetStr() + GetSumItemStr();
}


DWORD CUser::GetNewestInt() const
{
	return GetInt() + GetSumItemInt();
}


DWORD CUser::GetNewestDex() const
{
	return GetDex() + GetSumItemDex();
}


DWORD CUser::GetNewestVit() const
{
	return GetVit() + GetSumItemVit();
}


WORD CUser::GetOriginalMaxHP() const
{
	//(class.BHP + race.BHP + ����VIT * {Class.HP + Race.HP)  + (level-1)*(class.LHP+race.LHP)}	
	int		iBaseHP = g_pBaseClassInfo[GetClass()].iBHP + g_pBaseRaceInfo[GetRace()].iBHP;
	int		iHP		= GetVit() * ( g_pBaseClassInfo[GetClass()].iHP + g_pBaseRaceInfo[GetRace()].iHP );
	int		iLHP	= ( GetLevel()-1 )* ( g_pBaseClassInfo[GetClass()].iLHP + g_pBaseRaceInfo[GetRace()].iLHP );

	return WORD(iBaseHP+iHP+iLHP);
}


void CUser::UpdateMaxHP()
{
	//����MAX.HP * (100 + sum.%HP + ��ų.hp%)/100 + sum.pointHP + item_attr[8:vit] * (Class.HP + Race.HP) + ITEM_ATTR_LEVEL_PER_MAXHP * Char_Level
	int		iPlusHP = int(GetItemAttr(ITEM_ATTR_HP_POINT)
				+( GetItemAttr( ITEM_ATTR_VIT ) + GetItemAttr( ITEM_ATTR_ALLSTATUS ) )
				*( g_pBaseClassInfo[GetClass()].iHP + g_pBaseRaceInfo[GetRace()].iHP )
				+GetItemAttr(ITEM_ATTR_LEVEL_PER_MAXHP)*GetLevel()
				+GetAlphaStat(USER_MAXHP));
	float	fPercent= ( ( 100.f + (float)GetItemAttr(ITEM_ATTR_HP_PERCENT) + GetAlphaStat(USER_MAXHP_PERCENT)) * 0.01f );
	m_dwMaxHP = DWORD(GetOriginalMaxHP() * fPercent+iPlusHP);

	BYTE	bInfulxSkillLevel = GetSkillLevel(__SKILL_INFULX__);
	if (bInfulxSkillLevel)
		m_dwMaxHP += DWORD(m_dwMaxHP*g_pEffectLayer->GetEffectInfo(__SKILL_INFULX__)->Value[bInfulxSkillLevel-1].nMin/100.);		
}


WORD CUser::GetOriginalMaxMP() const
{
	//(class.BSP + race.BSP + ����INT * {Class.SP + Race.SP) + (level-1)*(class.LSP + race.LSP)}
	int		iBaseMP	= g_pBaseClassInfo[GetClass()].iBMP + g_pBaseRaceInfo[GetRace()].iBMP;
	int		iMP		= GetInt() * ( g_pBaseClassInfo[GetClass()].iMP + g_pBaseRaceInfo[GetRace()].iMP );
	int		iLMP	= ( GetLevel()-1 )* ( g_pBaseClassInfo[GetClass()].iLMP + g_pBaseRaceInfo[GetRace()].iLMP );

	return WORD(iBaseMP+iMP+iLMP);
}


void CUser::UpdateMaxSP()
{
	//����MAX.SP * (100 + sum.%SP + ��ų.sp%)/100 + sum.pointSP + item_attr[6:int] * (class.SP + race.HP) + ITEM_ATTR_LEVEL_PER_MAXMP * Char_Level
	int		iPlusMP = int(GetItemAttr(ITEM_ATTR_SP_POINT)
					+( GetItemAttr( ITEM_ATTR_INT )+GetItemAttr( ITEM_ATTR_ALLSTATUS ) )
					*( g_pBaseClassInfo[GetClass()].iMP + g_pBaseRaceInfo[GetRace()].iMP )
					+GetItemAttr(ITEM_ATTR_LEVEL_PER_MAXMP)*GetLevel()
					+ GetAlphaStat(USER_MAXMP));
	float	fPercent= ( ( 100.f + (float)GetItemAttr(ITEM_ATTR_SP_PERCENT) + GetAlphaStat(USER_MAXMP_PERCENT)) * 0.01f );
	m_dwMaxMP = DWORD(GetOriginalMaxMP()*fPercent+iPlusMP);
}


void CUser::UpdateMaxCoolPoint()
{
	m_fMaxCoolPoint = float(GetNewestEgo() *(100 + 5 * m_wClassRank) / 1000.);
}


void CUser::GetAttackDamage(BYTE bySelectedSkill, BYTE bSkillLevel, BYTE bWeaponKind, WORD* pwAttackDamageMin
							, WORD* pwAttackDamageMax, BYTE bLR) const
{
	// Selected ��ų�� MAX_SKILL �̸� �� ���������̴�.
	float	fDamageMin	= 0.0f;
	float	fDamageMax	= 0.0f;
	BYTE	byFA		= 0xff;
	Effect* pEffect		= NULL;

	if( bySelectedSkill != __SKILL_NONE_SELECT__ )
	{
		pEffect = g_pEffectLayer->GetEffectInfo( bySelectedSkill );
		byFA = ( pEffect->bID == 0 ) ? 1 : pEffect->bFormula;
	}

	switch( byFA )
	{
	case 1:		// �� ������ ����.
		{
			GetAttackDamageByFormula1(fDamageMin, fDamageMax, pEffect->bID, bWeaponKind, bLR, TRUE);
		}
		break;

	case 2:		
		{
			GetAttackDamageByFormula2(fDamageMin, fDamageMax, pEffect->bID, GetSkillLevel(pEffect->bID), bLR, TRUE);	
		}
		break;
		
	case 20:
		{
			GetAttackDamageByFormula20(fDamageMin, fDamageMax, pEffect->bID, bLR, (WORD)GetMaxSP());			
		}
		break;

	case 21:
		{
			GetAttackDamageByFormula1(fDamageMin, fDamageMax, pEffect->bID, bWeaponKind, bLR, FALSE);
		}
		break;

	case 22:
		{
			GetAttackDamageByFormula2(fDamageMin, fDamageMax, pEffect->bID, GetSkillLevel(pEffect->bID), bLR, FALSE);	
		}
		break;
	
	default:
		fDamageMin = 0;
		fDamageMax = 0;
		break;
	}

	*pwAttackDamageMin = (WORD)fDamageMin;
	*pwAttackDamageMax = (WORD)fDamageMax;
}


// bSkillAttack ��ų �������� ���� �Ǿ�� �ϴ���..
void CUser::GetAttackDamageByFormula1(float& fDamageMin, float& fDamageMax, BYTE bSkillKind, WORD wWeaponKind, BYTE bLR, BYTE bSkillAttack) const
{
	// ���� ������
	//minDMG = [(STR / 125 + WeaponDMG.min) * (100 + STR) / 100] * ClassAD / 2 * (100 + ���������͸�.Skill.AD + ��ų.ad) / 100 + ITEM_ATTR_ATTACK_MIN + ITEM_ATTR_LEVEL_PER_DMG_MIN * char_level
	float fSkill_Min	= 100.0f;
	float fSkill_Max	= 100.0f;
	
	if( GetSkillLevel(__SKILL_WEAPONMASTERY__) != 0 )
	{
		fSkill_Min	+= (float)g_pEffectLayer->GetEffectInfo( __SKILL_WEAPONMASTERY__ )->Value[ GetSkillLevel(__SKILL_WEAPONMASTERY__)-1 ].nMin;
		fSkill_Max	+= (float)g_pEffectLayer->GetEffectInfo( __SKILL_WEAPONMASTERY__ )->Value[ GetSkillLevel(__SKILL_WEAPONMASTERY__)-1 ].nMax;
	}	
	
	if (bLR == SELECT_ATTACK_TYPE_LBUTTON)
	{
		fSkill_Min += GetAlphaStat(USER_ATTACK_L);
		fSkill_Max += GetAlphaStat(USER_ATTACK_L);
		fSkill_Min += fSkill_Min*GetAlphaStat(USER_ATTACK_L_PERCENT)/100.f;
		fSkill_Max += fSkill_Max*GetAlphaStat(USER_ATTACK_L_PERCENT)/100.f;
	}
	else
	{
		fSkill_Min += GetAlphaStat( USER_ATTACK_R );
		fSkill_Max += GetAlphaStat( USER_ATTACK_R );
		fSkill_Min += fSkill_Min*GetAlphaStat(USER_ATTACK_R_PERCENT)/100.f;
		fSkill_Max += fSkill_Max*GetAlphaStat(USER_ATTACK_R_PERCENT)/100.f;
	}

	BYTE bCurrentHand = (m_bCurrentHand==1) ? EQUIP_TYPE_RHAND1 : EQUIP_TYPE_RHAND2;
	WORD wMinDamage = 0;
	WORD wMaxDamage = 0;
	
	if(CheckItem((CItem*)&m_pEquip[bCurrentHand])==TRUE)
	{
		wMinDamage = m_pEquip[bCurrentHand].m_Item_Weapon.wMin_Damage;
		wMaxDamage = m_pEquip[bCurrentHand].m_Item_Weapon.wMax_Damage;
	}

	float fClassRace		= float(g_pBaseClassInfo[GetClass()].iAD + g_pBaseRaceInfo[GetRace()].iAD);
	float fWeaponDamage_Min = 0.f;
	float fWeaponDamage_Max = 0.f;

	if (wWeaponKind == ITEM_CLASS_KATAL)
	{
		fWeaponDamage_Min = ( (float)GetNewestStr() / 225.0f + (float)GetNewestDex() / 225.0f+(float)wMinDamage ) * ( (float)GetNewestStr() + (float)GetNewestDex() + 200.f )/200.f;
		fWeaponDamage_Max = ( (float)GetNewestStr() / 225.0f + (float)GetNewestDex() / 225.0f+(float)wMaxDamage ) * ( (float)GetNewestStr() + (float)GetNewestDex() + 200.f )/200.f;
	}
	else
	{
		fWeaponDamage_Min = ( (float)GetNewestStr() / 125.0f + (float)wMinDamage ) * ( (float)GetNewestStr() + 100.f ) / 100.f;
		fWeaponDamage_Max = ( (float)GetNewestStr() / 125.0f + (float)wMaxDamage ) * ( (float)GetNewestStr() + 100.f ) / 100.f;	
	}

	if (bSkillAttack)
	{
		if( bSkillKind != 0 )		// ���� ��ų �������� �����Ѵ�.
		{
			
			fSkill_Min	+= g_pEffectLayer->GetFormula16(bSkillKind, BYTE(GetSkillLevel(bSkillKind)-1), GetClass());
			fSkill_Max	+= g_pEffectLayer->GetFormula19(bSkillKind, BYTE(GetSkillLevel(bSkillKind)-1), GetClass());
		}
	}

	fDamageMin = fWeaponDamage_Min * fClassRace / 2.f 
		* fSkill_Min / 100.0f+GetItemAttr(ITEM_ATTR_ATTACK_MIN) 
		+ GetItemAttr(ITEM_ATTR_LEVEL_PER_DMG_MIN) * GetLevel()
		+(2*GetLevel())*(100+GetNewestStr())/100.f;
	fDamageMax = fWeaponDamage_Max * fClassRace / 2.f 
		* fSkill_Max / 100.0f+GetItemAttr(ITEM_ATTR_ATTACK_MAX) 
		+ GetItemAttr(ITEM_ATTR_LEVEL_PER_DMG_MAX) * GetLevel()
		+(2*GetLevel())*(100+GetNewestStr())/100.f;

	DWORD dwSkillPassiveValueMin = GetSkillPassiveValueMin(bSkillKind);

	fDamageMin += fDamageMin*dwSkillPassiveValueMin/100.f;
	fDamageMax += fDamageMax*dwSkillPassiveValueMin/100.f;

	if (fDamageMin > fDamageMax)
		fDamageMin = fDamageMax-1;	
}


// ��ų �нú��� ������ �´� Min���� ���Ͷ�.
DWORD CUser::GetSkillPassiveValueMin(BYTE bSkillKind) const
{
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);

	DWORD dwSkillPassiveDamage = 0;

	switch(pEffect->wProperty % 10)
	{
	case SKILL_PROPERTY_KIND_FIRE:
		if (GetSkillLevel(__SKILL_FIREMASTERY__) != 0)
		{
			Effect* pEffectByProperty = g_pEffectLayer->GetEffectInfo(__SKILL_FIREMASTERY__);
			dwSkillPassiveDamage = pEffectByProperty->Value[GetSkillLevel(__SKILL_FIREMASTERY__)-1].nMin;
		}break;

	case SKILL_PROPERTY_KIND_ICE:
		if (GetSkillLevel(__SKILL_ICEMASTERY__) != 0)
		{
			Effect* pEffectByProperty = g_pEffectLayer->GetEffectInfo(__SKILL_ICEMASTERY__);
			dwSkillPassiveDamage = pEffectByProperty->Value[GetSkillLevel(__SKILL_ICEMASTERY__)-1].nMin;
		}break;

	case SKILL_PROPERTY_KIND_LIGHT:
		if (GetSkillLevel(__SKILL_LIGHTMASTERY__) != 0)
		{
			Effect* pEffectByProperty = g_pEffectLayer->GetEffectInfo(__SKILL_LIGHTMASTERY__);
			dwSkillPassiveDamage = pEffectByProperty->Value[GetSkillLevel(__SKILL_LIGHTMASTERY__)-1].nMin;
		}break;

	case SKILL_PROPERTY_KIND_POI:
		if (GetSkillLevel(__SKILL_POISONMASTERY__) != 0)
		{
			Effect* pEffectByProperty = g_pEffectLayer->GetEffectInfo(__SKILL_POISONMASTERY__);
			dwSkillPassiveDamage = pEffectByProperty->Value[GetSkillLevel(__SKILL_POISONMASTERY__)-1].nMin;
		}break;
	case 0:	
	case SKILL_PROPERTY_KIND_PHY:
		{
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(GetCurrentWeaponID());
			if (!pBaseItem)	return 0;
			
			// ���ľ� �� ��.��;; �ϴ� ������ ���� ���Ŀ�. 
			switch(pBaseItem->GetWeaponKind())
			{
				// ��
			case ITEM_CLASS_SWARD:
			case ITEM_CLASS_SWARD2:
				if (GetSkillLevel(__SKILL_SWORDMASTERY__) != 0)
				{
					Effect* pEffectByProperty = g_pEffectLayer->GetEffectInfo(__SKILL_SWORDMASTERY__);
					dwSkillPassiveDamage = pEffectByProperty->Value[GetSkillLevel(__SKILL_SWORDMASTERY__)-1].nMin;
				}
				break;
				// �б�
			case ITEM_CLASS_MACE:
			case ITEM_CLASS_MACE2:
				if (GetSkillLevel(__SKILL_BLUNTWEAPON_FIGTER_MASTERY__) != 0)
				{
					Effect* pEffectByProperty = g_pEffectLayer->GetEffectInfo(__SKILL_BLUNTWEAPON_FIGTER_MASTERY__);
					dwSkillPassiveDamage = pEffectByProperty->Value[GetSkillLevel(__SKILL_BLUNTWEAPON_FIGTER_MASTERY__)-1].nMin;
				}

				if (GetSkillLevel(__SKILL_BLUNTWEAPON_PREST_MASTERY__) != 0)
				{
					Effect* pEffectByProperty = g_pEffectLayer->GetEffectInfo(__SKILL_BLUNTWEAPON_PREST_MASTERY__);
					dwSkillPassiveDamage = pEffectByProperty->Value[GetSkillLevel(__SKILL_BLUNTWEAPON_PREST_MASTERY__)-1].nMin;
				}
				break;
				// â
			case ITEM_CLASS_SPEAR:
				if (GetSkillLevel(__SKILL_SPEARMASTERY__) != 0)
				{
					Effect* pEffectByProperty = g_pEffectLayer->GetEffectInfo(__SKILL_SPEARMASTERY__);
					dwSkillPassiveDamage = pEffectByProperty->Value[GetSkillLevel(__SKILL_SPEARMASTERY__)-1].nMin;
				}
				if (GetSkillLevel(__SKILL_EXTREMESPEAR__) != 0)
				{
					Effect* pEffectByProperty = g_pEffectLayer->GetEffectInfo(__SKILL_EXTREMESPEAR__);
					dwSkillPassiveDamage = pEffectByProperty->Value[GetSkillLevel(__SKILL_EXTREMESPEAR__)-1].nMin;
				} 

				// ������ ������ ��ȭ ��ų ���� �ذ� break ���� ���� ���� ��.��
				// 2005.01.30 �迵�� 
				break;
			case ITEM_CLASS_KATAL:
				if (GetSkillLevel(__SKILL_FORTIFICATIONMASTERY__) != 0)
				{
					Effect* pEffectByProperty = g_pEffectLayer->GetEffectInfo(__SKILL_FORTIFICATIONMASTERY__);
					dwSkillPassiveDamage = pEffectByProperty->Value[GetSkillLevel(__SKILL_FORTIFICATIONMASTERY__)-1].nMin;
				}
				break;
			}
		}break;
	}
	return dwSkillPassiveDamage;
}


void CUser::GetAttackDamageByFormula2(float& fDamageMin, float& fDamageMax, BYTE bSkillKind, BYTE bSkillLevel, BYTE bLR
									  , BYTE bSkillAttack) const
{
	// ���� ������	
	//Skill.MAX * (1 + ��ų.�����͸�* efficiency /2550) * (Max_��ų + Race.MD)/100 * ( 100 + �ͽ��÷���.��ų.MD) / 100
	//Skill.Min * (1 + ��ų.�����͸�* efficiency /2550) * (BaseClassSkill.Max + Race.MD)/100 * ( 100 + �ͽ��÷���.��ų.MD) / 100  * ( 100 + ITEM_ATTR_SKILL_DMG (�Ӽ�) ) / 100 + ITEM_ATTR_ATTACK(�Ӽ�).min*(1000 + ITEM_WEAPON.Mana_Damage)/1000
	if (!bSkillLevel)	return;

	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);
	float fManaMastery  = 1.f;
	float fSkill_Min	= 0.0f;
	float fSkill_Max	= 0.0f;
	
	DWORD	dwSkillPassiveDamage = 0;
	int		nClassRace = g_pEffectLayer->GetBaseClassSkillMax(GetClass(), bSkillKind);	
	
	// ��ų�������� ���� �нú� ó����.
	dwSkillPassiveDamage = GetSkillPassiveValueMin(bSkillKind);

	// Ŭ������ �г�Ƽ
	switch( pEffect->wProperty / 100)
	{	
		case CLASS_TYPE_WIZARD:
			nClassRace += g_pBaseRaceInfo[GetRace()].iMax_Magic;
			break;
		case CLASS_TYPE_WARRIOR:
			nClassRace += g_pBaseRaceInfo[GetRace()].iMax_Aura;
			break;
		case CLASS_TYPE_HUNTER:
			nClassRace += g_pBaseRaceInfo[GetRace()].iMax_Chakra;
			break;
		case CLASS_TYPE_SUMMONER:
			nClassRace += g_pBaseRaceInfo[GetRace()].iMax_Summon;
			break;
		case CLASS_TYPE_PRIEST:
			nClassRace += g_pBaseRaceInfo[GetRace()].iMax_Divine;
			break;
		default:break;
	}
	
	fSkill_Min	= pEffect->Value[ bSkillLevel-1 ].nMin * fManaMastery; 
	fSkill_Max	= pEffect->Value[ bSkillLevel-1 ].nMax * fManaMastery;

	if (bSkillAttack)
	{
		if (bLR == SELECT_ATTACK_TYPE_LBUTTON)
		{
			fSkill_Min += fSkill_Min*(GetAlphaStat(USER_SKILLATTACK_L_PERCENT))/100.f;
			fSkill_Max += fSkill_Max*(GetAlphaStat(USER_SKILLATTACK_L_PERCENT))/100.f;
		}
		else
		{
			fSkill_Min += fSkill_Min*(GetAlphaStat(USER_SKILLATTACK_R_PERCENT))/100.f;
			fSkill_Max += fSkill_Min*(GetAlphaStat(USER_SKILLATTACK_R_PERCENT))/100.f;
		}
	}	
	
	fDamageMin = fSkill_Min * nClassRace / 100.f * ( 100 + dwSkillPassiveDamage) / 100.f;
	fDamageMax = fSkill_Max * nClassRace / 100.f * ( 100 + dwSkillPassiveDamage) / 100.f;

	BYTE bCurrentHand = (m_bCurrentHand==1) ? EQUIP_TYPE_RHAND1 : EQUIP_TYPE_RHAND2;
	WORD wAttackSkillDamage = 0;
	if(CheckItem((CItem*)&m_pEquip[bCurrentHand])==TRUE)
		wAttackSkillDamage = m_pEquip[bCurrentHand].m_Item_Weapon.wMana_Damage;
		
	fDamageMin += fDamageMin*wAttackSkillDamage/1000.f;
	fDamageMax += fDamageMax*wAttackSkillDamage/1000.f;

	fDamageMin = fSkill_Min * nClassRace / 100.f * ( 100 + dwSkillPassiveDamage) / 100.f*(1000+wAttackSkillDamage+(4*GetLevel()))/1000.f;
	fDamageMax = fSkill_Max * nClassRace / 100.f * ( 100 + dwSkillPassiveDamage) / 100.f*(1000+wAttackSkillDamage+(4*GetLevel()))/1000.f;
}


void CUser::GetAttackDamageByFormula20(float& fDamageMin, float& fDamageMax, BYTE bSkillKind, BYTE bLR, WORD wSP) const
{
	// ��ũ�� ������ �ͽ��÷����� ���� ������	
	//SP * Min / 100 *BaseClassSkill.Max / 100  ~ SP * Max / 100 * BaseClassSkill.Max / 100  ������ �������� ��
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);
	float fSkill_Min	= 0.0f;
	float fSkill_Max	= 0.0f;
	BYTE bSkillLevel = GetSkillLevel(bSkillKind);
	if (!bSkillLevel)	return;

	fSkill_Min	= (float)pEffect->Value[ bSkillLevel-1 ].nMin; 
	fSkill_Max	= (float)pEffect->Value[ bSkillLevel-1 ].nMax;

	if (bLR == SELECT_ATTACK_TYPE_LBUTTON)
	{
		fSkill_Min += fSkill_Min*GetAlphaStat( USER_SKILLATTACK_L_PERCENT );
		fSkill_Max += fSkill_Min*GetAlphaStat( USER_SKILLATTACK_L_PERCENT );
	}
	else
	{
		fSkill_Min += fSkill_Min*GetAlphaStat( USER_SKILLATTACK_R_PERCENT );
		fSkill_Max += fSkill_Min*GetAlphaStat( USER_SKILLATTACK_R_PERCENT );
	}
	

	fDamageMin = wSP * fSkill_Min / 100.f*g_pEffectLayer->GetBaseClassSkillMax(GetClass(), bSkillKind)/100.f;
	fDamageMax = wSP * fSkill_Max / 100.f*g_pEffectLayer->GetBaseClassSkillMax(GetClass(), bSkillKind)/100.f;
}


void CUser::SendItemSkill(BYTE bFlag, BYTE bObjectTargetType, void* pObjectTartet)
{
	if (!GetCurDungeon())	return;
	CDungeonLayer* pLayer = GetCurDungeonLayer();
	if (!pLayer)			return;
	if (!(bFlag == ITEM_SKILL_ATTACK || bFlag == ITEM_SKILL_DEFENSE || bFlag == ITEM_SKILL_DIE))	
		return;

	// ��ų ����.
//	BYTE bCurrentHand = (m_bCurrentHand==1) ? EQUIP_TYPE_RHAND1 : EQUIP_TYPE_RHAND2;
	BYTE bKind = BYTE(GetCurrentWeaponID() / ITEM_DISTRIBUTE);
		
	for(int i = 0; i < MAX_EQUIP_POOL; ++i)
	{
		if (m_pEquip[i].GetID())
		{	
			if(m_bCurrentHand==1)
			{
				if(i==EQUIP_TYPE_RHAND2 || i==EQUIP_TYPE_LHAND2)
					continue;
			}
			else if(m_bCurrentHand==2)
			{
				if(i==EQUIP_TYPE_RHAND1 || i==EQUIP_TYPE_LHAND1)
					continue;
			}

			for(int c = 0; c < MAX_ITEM_ATTR; ++c)
			{
				if (m_pEquip[i].wAttr[c].uCode >= MAX_ITEM_KIND1_CODE)
				{				
					DWORD dwVirtualCode = g_pItemAttrLayer->GetVirtualCode(m_pEquip[i].wAttr[c].uCode, m_pEquip[i].wAttr[c].uValue);	
					
					if (g_pItemAttrLayer->m_ItemAttrDefine[dwVirtualCode].wFormula == bFlag)
					{
#ifndef	DEV_MODE
						DWORD dwValueListValue = g_pItemAttrLayer->GetValueListValue(m_pEquip[i].wAttr[c].uValue);
						DWORD dwValueListID = g_pItemAttrLayer->GetValueListID((WORD)dwVirtualCode, (WORD)dwValueListValue);
						if (g_pItemAttrLayer->m_ItemAttrValueList[dwValueListID].bPBT > rand() % 100)
#endif
						{
							short shValue = g_pItemAttrLayer->GetVirtualValue(m_pEquip[i].wAttr[c].uCode, m_pEquip[i].wAttr[c].uValue);
							WORD wMinMax[2]={0,};
							
							GetAttackDamage(g_pItemAttrLayer->m_ItemAttrDefine[dwVirtualCode].bSkillID, 
								(BYTE)shValue, bKind, &wMinMax[0], &wMinMax[1], SELECT_ATTACK_TYPE_RBUTTON);

							Effect* pEffect = g_pEffectLayer->GetEffectInfo(g_pItemAttrLayer->m_ItemAttrDefine[dwVirtualCode].bSkillID);
							if (pEffect)
							{
								if (
										((pEffect->bSkillTarget)&TARGETTYPE_ENEMY_PLAYER)
									||	((pEffect->bSkillTarget)&TARGETTYPE_FRIEND_PLAYER)
									)
								{
									if (pEffect->bCrime == CRIME_APPLY_ENEMY 
										&& bObjectTargetType == OBJECT_TYPE_PLAYER 
										&& !IsAlliance((CUser*)pObjectTartet) )
										SystemSkillToUser(pLayer, 
											g_pItemAttrLayer->m_ItemAttrDefine[dwVirtualCode].bSkillID, 
											(BYTE)shValue, 30*1000, GetCurPosition(), 
											wMinMax, OBJECT_TYPE_PLAYER, 
											GetID(), (CUser*)pObjectTartet);
									else if (pEffect->bCrime == CRIME_APPLY_FRIENDLY)
										SystemSkillToUser(pLayer, 
											g_pItemAttrLayer->m_ItemAttrDefine[dwVirtualCode].bSkillID, 
											(BYTE)shValue, 30*1000, GetCurPosition(), wMinMax, 
											OBJECT_TYPE_PLAYER, GetID(), this);
								}
								if (
										((pEffect->bSkillTarget)&TARGETTYPE_FRIEND_PLAYER)
									||	((pEffect->bSkillTarget)&TARGETTYPE_ENEMY_PLAYER)
									)
								{
									if (pEffect->bCrime == CRIME_APPLY_ENEMY 
										&& bObjectTargetType == OBJECT_TYPE_MONSTER 
										&& !IsAlliance((CMonster*)pObjectTartet) )
										SystemSkillToMonster(pLayer, 
											g_pItemAttrLayer->m_ItemAttrDefine[dwVirtualCode].bSkillID, 
											(BYTE)shValue, 30*1000, GetCurPosition(), 
											wMinMax, OBJECT_TYPE_PLAYER, 
											GetID(), (CMonster*)pObjectTartet);
									else if ((pEffect->bCrime == CRIME_APPLY_FRIENDLY && bObjectTargetType == OBJECT_TYPE_MONSTER && IsAlliance((CMonster*)pObjectTartet) )
										|| (pEffect->bID == __SKILL_REDELEMENTAL__
										|| pEffect->bID == __SKILL_BLUEELEMENTAL__
										|| pEffect->bID == __SKILL_GREENELEMENTAL__))
										SystemSkillToMonster(pLayer, 
											g_pItemAttrLayer->m_ItemAttrDefine[dwVirtualCode].bSkillID, 
											(BYTE)shValue, 30*1000, GetCurPosition(), 
											wMinMax, OBJECT_TYPE_PLAYER, 
											GetID(), (CMonster*)pObjectTartet);
								}								
							}
						}
					}					
				}
			}
		}
	}

	// �ϴ��� �̷��� �س���. ���߿� ������ �� ���� ��.��
	if (bFlag == ITEM_SKILL_DIE)
	{		
		POSITION_ pos = m_pUsingItemList->GetHeadPosition();
		POSITION_ del=0;
		
		while(pos)
		{
			del = pos;		
			CItem* pItem = (CItem*)m_pUsingItemList->GetNext(pos);
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pItem->GetID());	

			if (!pBaseItem->BaseItem_Consumable.wLast_Time)
			{
				// �̺�Ʈ���ΰ�.
				BOOL bDetachItem = TRUE;
				for(int i = 0; i < MAX_BASEITEM_CONSUMABLE_ITEMATTR; ++i)
				{					
					DWORD dwVirtualCode = g_pItemAttrLayer->GetVirtualCode(pBaseItem->BaseItem_Consumable.sItem_Attr[i].wAttr_Code, pBaseItem->BaseItem_Consumable.sItem_Attr[i].wAttr_Value);
					short shValue = g_pItemAttrLayer->GetVirtualValue(pBaseItem->BaseItem_Consumable.sItem_Attr[i].wAttr_Code, pBaseItem->BaseItem_Consumable.sItem_Attr[i].wAttr_Value);
					WORD wMinMax[2]={0,};
					
					switch (dwVirtualCode)
					{
					case ITEM_ATTR_SKILL_CASTING_WHEN_DEAD_SKILL_TREEAGION:
						{							
							GetAttackDamage(g_pItemAttrLayer->m_ItemAttrDefine[dwVirtualCode].bSkillID, 
								(BYTE)shValue, bKind, &wMinMax[0], &wMinMax[1], SELECT_ATTACK_TYPE_RBUTTON);

							Effect* pEffect = g_pEffectLayer->GetEffectInfo(g_pItemAttrLayer->m_ItemAttrDefine[dwVirtualCode].bSkillID);
							if (pEffect)
							{
								if (	(pEffect->bSkillTarget&TARGETTYPE_ENEMY_PLAYER)
									||	(pEffect->bSkillTarget&TARGETTYPE_FRIEND_PLAYER) )
								{
									if (pEffect->bCrime == CRIME_APPLY_ENEMY 
										&& bObjectTargetType == OBJECT_TYPE_PLAYER 
										&& !IsAlliance((CUser*)pObjectTartet) )
										SystemSkillToUser(pLayer, 
											g_pItemAttrLayer->m_ItemAttrDefine[dwVirtualCode].bSkillID, 
											(BYTE)shValue, 30*1000, GetCurPosition(), 
											wMinMax, OBJECT_TYPE_PLAYER, 
											GetID(), (CUser*)pObjectTartet);
									else if (pEffect->bCrime == CRIME_APPLY_FRIENDLY)
										SystemSkillToUser(pLayer, 
											g_pItemAttrLayer->m_ItemAttrDefine[dwVirtualCode].bSkillID, 
											(BYTE)shValue, 30*1000, GetCurPosition(), 
											wMinMax, OBJECT_TYPE_PLAYER, GetID(), this);
								}
								if (
										(pEffect->bSkillTarget&TARGETTYPE_FRIEND_MONSTER)
									||	(pEffect->bSkillTarget&TARGETTYPE_ENEMY_MONSTER)
									)
								{
									SystemSkillToMonster(pLayer, 
										g_pItemAttrLayer->m_ItemAttrDefine[dwVirtualCode].bSkillID, 
										(BYTE)shValue, 30*1000, GetCurPosition(), 
										wMinMax, OBJECT_TYPE_PLAYER, 
										GetID(), (CMonster*)pObjectTartet);
								}								
							}
						}break;
					case ITEM_ATTR_DEAD_EXP_LOSS_DECREASE:
						{
							// �������϶��� ���ֶ�.
							if (GetCurDungeon()->GetDungeonDataEx()->m_bSiege
							|| m_sMatchDescInfo.m_bMatching	)// : 050111 hwoarang
							{
								bDetachItem = FALSE;
							}
						}break;
					}
				}

				if (bDetachItem)
					DetachItem(del);
			}
		}
	}	
}


// ���� �������� �Ծ����� �� �Լ��� �ҷ��� �������װ��� ��������� �Ѵ�.
short CUser::GetPhyResist() const
{
	if (GetClass() == CLASS_TYPE_HUNTER)
	{
		// �������� mp �翡 ���� �������� ���Ѵ�.
		return (short)min(m_wPhyResist + max(int(GetSP()/20 - (73+22*GetLevel())/35), 0), 75);
	}
	else
	{
		return m_wPhyResist;
	}
}


void CUser::GetAttackingDamage(WORD wAttackDamageMin
							 , WORD wAttackDamageMax
							 , WORD* pwAttackDamageMin
							 , WORD* pwAttackDamageMax
							 , BYTE byLR)
{
	if (GetClass() == CLASS_TYPE_HUNTER)
	{
		// �������� mp �翡 ���� �������� ���Ѵ�.
		if (wAttackDamageMin)
		{
			if (GetSelectedSkill((SELECT_ATTACK_TYPE)byLR) == __SKILL_ATTACK__)
			{
				*pwAttackDamageMin = WORD(wAttackDamageMin + (GetSP()/30));
				*pwAttackDamageMax = WORD(wAttackDamageMax + (GetSP()/30));
			}
			else
			{
				*pwAttackDamageMin = wAttackDamageMin;
				*pwAttackDamageMax = wAttackDamageMax;
			}
		}
		else
		{
			*pwAttackDamageMin = 0;
			*pwAttackDamageMax = 0;
		}
	}
	else
	{
		*pwAttackDamageMin = wAttackDamageMin;
		*pwAttackDamageMax = wAttackDamageMax;
	}
}

WORD CUser::GetOriginalAttackAcuracy() const
{
	//����DEX*(Class.AA+ Race.AA)
	return WORD( GetDex() * (g_pBaseClassInfo[GetClass()].iAA + g_pBaseRaceInfo[GetRace()].iAA) );
}

void CUser::UpdateAttackAcuracy()
{
	//[����������Ȯ��] * [100 + sum(������.plusAA%) ] / 100 +  (Class.AA + Race.AA) * item_attr[7:dex]  +  (���� ��ų�� �ö󰡴� ��Ȯ��)  + sum(������.AApoint) + ITEM_ATTR_LEVEL_PER_AA * char_LEV
	m_wAttackAcuracy = WORD(GetOriginalAttackAcuracy()*( 100 + (float)GetItemAttr( ITEM_ATTR_AA_PERCENT ) )/100.f);
	m_wAttackAcuracy += (g_pBaseClassInfo[GetClass()].iAA+g_pBaseRaceInfo[GetRace()].iAA)*(GetItemAttr( ITEM_ATTR_DEX ) + GetItemAttr( ITEM_ATTR_ALLSTATUS ));
	m_wAttackAcuracy += WORD(GetItemAttr( ITEM_ATTR_AA_POINT )+ GetItemAttr(ITEM_ATTR_LEVEL_PER_AA)*GetLevel());
	m_wAttackAcuracy += WORD(m_wAttackAcuracy*(GetAlphaStat(USER_ATTACKACURACY_PERCENT))/100.f);
}

WORD CUser::GetOriginalAvoid() const
{
	//����DEX * (class.DP + Race.DP)
	return WORD( GetDex() * (g_pBaseClassInfo[GetClass()].iDP+g_pBaseRaceInfo[GetRace()].iDP) );
}

void CUser::UpdateAvoid()
{
	//[����ȸ�Ƿ�] * [ 100 + sum(������.DP%)] / 100 +  { (item_attr[7:dex] + SUM.�����۵��潺 / 2)  * (Class.DP + Race.DP) } + (������ų�� �ö󰡴� ȸ�Ƿ�) + sum(������.Dppoint)

	//( DEX + ( SUM.item.defence +(1+Sum.ȸ�Ƿ�(%)/100) ) / 2 ) * (class.DP + Race.DP) + �������Ͻ�.DP +����.DP + ȸ�Ƿ�.sum
	/*m_wAvoid	= ( (float)GetLastDex() +  ((float)m_dwSumArmorDefense+( 1.f +(float)m_pwItemAttr[ITEM_ATTR_AVOID_PERSENT] / 100.f )) / 2.0f ) 	
				* ( g_pBaseClassInfo[GetClass()].iDP + g_pBaseRaceInfo[GetRace()].iDP ) 
				+ m_fPlusParam[ USER_AVOID ] + m_pwItemAttr[ ITEM_ATTR_AVOID_POINT];*/

	//[�谨��������ȸ�Ƿ�]=[����ȸ�Ƿ�] * [ 100 + sum(������.DP%)] / 100 +  { (item_attr[7:dex] + SUM.�����۵��潺 / 2)  * (Class.DP + Race.DP) } + (������ų�� �ö󰡴� ȸ�Ƿ�) + sum(������.Dppoint) + ITEM_ATTR_LEVEL_PER_AVOID * char_LEV
/*	m_wAvoid = GetOriginalAvoid()*((100+m_pwItemAttr[ITEM_ATTR_AVOID_PERSENT]) / 100.f);
	m_wAvoid += (GetSumItemDex()+GetArmorDefense()/2.)*(g_pBaseClassInfo[GetClass()].iDP+g_pBaseRaceInfo[GetRace()].iDP);
	m_wAvoid += m_fPlusParam[ USER_AVOID ]+m_pwItemAttr[ ITEM_ATTR_AVOID_POINT]+m_pwItemAttr[ITEM_ATTR_LEVEL_PER_AVOID]*GetLevel();
	m_wAvoid += m_wAvoid*m_fPlusParam[ USER_AVOID_PERCENT ]/100.;

	m_wAvoid = m_wAvoid*(1-m_byRest);	// �޽��Ҷ� 0����.
*/
	//[�谨��������ȸ�Ƿ�]=[����ȸ�Ƿ�] * [ { (item_attr[7:dex] )  * (Class.DP + Race.DP) } + (������ų�� �ö󰡴� ȸ�Ƿ�) + ITEM_ATTR_LEVEL_PER_AVOID * char_LEV
	m_wAvoid = WORD(GetOriginalAvoid()*((100+GetItemAttr(ITEM_ATTR_AVOID_PERSENT)) / 100.f));
	m_wAvoid += WORD((GetSumItemDex()+GetArmorDefense()/4.f)
				*(g_pBaseClassInfo[GetClass()].iDP+g_pBaseRaceInfo[GetRace()].iDP));
	m_wAvoid += WORD(GetAlphaStat(USER_AVOID)+GetItemAttr( ITEM_ATTR_AVOID_POINT )+GetItemAttr(ITEM_ATTR_LEVEL_PER_AVOID)*GetLevel());
	m_wAvoid += WORD(m_wAvoid*GetAlphaStat(USER_AVOID_PERCENT )/100.f);

	m_wAvoid = WORD(m_wAvoid*(1-m_byRest));	// �޽��Ҷ� 0����.

	/*
	m_wAvoid = GetOriginalAvoid()*((100+m_pwItemAttr[ITEM_ATTR_AVOID_PERSENT]) / 100.f);
	m_wAvoid += GetSumItemDex()*(g_pBaseClassInfo[GetClass()].iDP+g_pBaseRaceInfo[GetRace()].iDP);
	m_wAvoid += m_fPlusParam[ USER_AVOID ]+m_pwItemAttr[ ITEM_ATTR_AVOID_POINT]+m_pwItemAttr[ITEM_ATTR_LEVEL_PER_AVOID]*GetLevel();
	m_wAvoid += m_wAvoid*m_fPlusParam[ USER_AVOID_PERCENT ]/100.;
	m_wAvoid = m_wAvoid*(1-m_byRest);	// �޽��Ҷ� 0����.*/
}


void CUser::UpdateBlockRate()
{	
	//{ (class.BR + Race.BR) * ( DEX - Class.DEX + 1) / (  DEX - Class.DEX + LV * 2 ) } + ��ų�οö󰡴º�Ȯ��
	
	//{ (class.BR + Race.BR) * DEX/LV } / 400 * (100 + ��ų.BR) /100
	/*float	iBaseBlockRate = ( g_pBaseClassInfo[GetClass()].iBR + g_pBaseRaceInfo[GetRace()].iBR ) * GetLastDex();
	m_wBlockRate = ( iBaseBlockRate / (float)GetLevel() ) * 0.04f;
	m_wBlockRate *= ( (float)( 100 + m_fPlusParam[ USER_BLOCKRATE ] ) * 0.01f );*/

	if((GetNewestDex()-g_pBaseClassInfo[GetClass()].iDEX+GetLevel()*2) == 0)
	{
		m_wBlockRate = WORD(( g_pBaseClassInfo[GetClass()].iBR + g_pBaseRaceInfo[GetRace()].iBR )
							*(GetNewestDex()-g_pBaseClassInfo[GetClass()].iDEX+1));
	}
	else
	{
		m_wBlockRate = WORD(( g_pBaseClassInfo[GetClass()].iBR + g_pBaseRaceInfo[GetRace()].iBR )
						*(GetNewestDex()-g_pBaseClassInfo[GetClass()].iDEX+1)
						/(GetNewestDex()-g_pBaseClassInfo[GetClass()].iDEX+GetLevel()*2));
	}

	m_wBlockRate += WORD(GetAlphaStat(USER_BLOCKRATE));
	m_wBlockRate += WORD(m_wBlockRate*GetAlphaStat(USER_BLOCKRATE_PERCENT));

	m_wBlockRate = WORD(m_wBlockRate * (100 + GetItemAttr(ITEM_ATTR_BLOCKRATE) ) / 100.f /2.f);
	if ((short)m_wBlockRate < 0)
		m_wBlockRate = 0;
	else if (m_wBlockRate > 75)
		m_wBlockRate = 75;

	m_wBlockRate = WORD(m_wBlockRate*(1-m_byRest));	// �޽��Ҷ� 0����
}


void CUser::UpdateFireResist()
{
	//ITEM_ATTR_FIRERESIST + ����.skill.RF + ��������̺�.skill.RF + (class.LR_Fire + race.LR_Fire) * ( LV - 1) + ITEM_ATTR_LEVEL_PER_FIRE_RESIST / 10 * char_LEV
	m_wFireResist	=	WORD(GetItemAttr( ITEM_ATTR_FIRERESIST ) 
		+ GetItemAttr( ITEM_ATTR_ALLRESIST ) 
		+ GetAlphaStat( USER_FIRE_RESIST ) 
		+ m_fPlusParamByMagicFieldArray[USER_FIRE_RESIST] 
		+ GetItemAttr(ITEM_ATTR_LEVEL_PER_ALL_MAGIC_RESIST)/10.f*GetLevel());

	m_wFireResist	+=	WORD((g_pBaseClassInfo[GetClass()].iRFire
		+g_pBaseRaceInfo[GetClass()].iRFire ) * (GetLevel() - 1)
		+GetItemAttr(ITEM_ATTR_LEVEL_PER_FIRE_RESIST)/10.f*GetLevel()
		+GetNewestInt()/15);

	m_wFireResist = min( m_wFireResist, WORD(75+GetAlphaStat(USER_FIRE_RESIST)));

	EffectDesc* pEffedtDesc = GetEffectDesc(__SKILL_IRONSKIN__);
	if(pEffedtDesc)//ö�� ��ų�϶��� 85%����
	{
		if( m_wFireResist > 85 )
			m_wFireResist = 85;
	}
	else
	{
		if( m_wFireResist > 75 )
			m_wFireResist = 75;
	}
	
	m_wFireResist -= 75*m_byRest;
}

void CUser::UpdateIceResist()
{
	//ITEM_ATTR_ICE_RESIST + ����.skill.RI + ��������̺�.skill.RI + (class.LR_Ice + race.LR_Ice) * ( LV - 1 ) + ITEM_ATTR_LEVEL_PER_ICE_RESIST / 10 * char_LEV
	m_wIceResist	=	WORD(GetItemAttr( ITEM_ATTR_ICERESIST ) 
		+ GetItemAttr( ITEM_ATTR_ALLRESIST ) 
		+ GetAlphaStat(USER_ICE_RESIST) 
		+ m_fPlusParamByMagicFieldArray[USER_ICE_RESIST] 
		+ GetItemAttr(ITEM_ATTR_LEVEL_PER_ALL_MAGIC_RESIST)/10.f*GetLevel());

	m_wIceResist	+=	WORD((g_pBaseClassInfo[GetClass()].iRIce
		+g_pBaseRaceInfo[GetClass()].iRIce ) * (GetLevel() - 1)
		+GetItemAttr(ITEM_ATTR_LEVEL_PER_ICE_RESIST)/10.f*GetLevel()
		+GetNewestInt()/15);

	m_wIceResist = min(m_wIceResist, WORD(75+GetAlphaStat(USER_ICE_RESIST)));

	EffectDesc* pEffedtDesc = GetEffectDesc(__SKILL_IRONSKIN__);
	if(pEffedtDesc)//ö�� ��ų�϶��� 85%����
	{
		if( m_wIceResist > 85 )
			m_wIceResist = 85;
	}
	else
	{
		if( m_wIceResist > 75 )
			m_wIceResist = 75;
	}

	m_wIceResist -= 75*m_byRest;
}

void CUser::UpdateLightResist()
{
	//item.RL + ����.skill.RL + ��������̺�.skill.RL + (class.LR_Lightning + race.LR_Lightning) *  ( LV - 1 ) + ITEM_ATTR_LEVEL_PER_LIGHT_RESIST / 10 * char_LEV
	m_wLightResist	=	WORD(GetItemAttr( ITEM_ATTR_LIGHTRESIST ) 
		+ GetItemAttr( ITEM_ATTR_ALLRESIST ) 
		+ GetAlphaStat(USER_LIGHT_RESIST) 
		+ m_fPlusParamByMagicFieldArray[USER_LIGHT_RESIST] 
		+ GetItemAttr(ITEM_ATTR_LEVEL_PER_ALL_MAGIC_RESIST)/10.f*GetLevel());

	m_wLightResist	+=	WORD((g_pBaseClassInfo[GetClass()].iRLighting
		+g_pBaseRaceInfo[GetClass()].iRLighting ) * (GetLevel() - 1)
		+GetItemAttr(ITEM_ATTR_LEVEL_PER_LIGHT_RESIST)/10.f*GetLevel()
		+GetNewestInt()/15);

	m_wLightResist = min(m_wLightResist, WORD(75+GetAlphaStat(USER_LIGHT_RESIST)));

	EffectDesc* pEffedtDesc = GetEffectDesc(__SKILL_IRONSKIN__);
	if(pEffedtDesc)//ö�� ��ų�϶��� 85%����
	{
		if( m_wLightResist > 85 )
			m_wLightResist = 85;
	}
	else
	{
		if( m_wLightResist > 75 )
			m_wLightResist = 75;
	}

	m_wLightResist -= 75*m_byRest;
}

void CUser::UpdatePoiResist()
{
	//ITEM_ATTR_POIRESIST + ����.skill.RP + ��������̺�.skill.R.P + (class.LR_Poison + race.LR_Poison) * ( LV - 1 ) + ITEM_ATTR_LEVEL_PER_POI_RESIST / 10 * char_LEV
	m_wPoiResist	=	WORD(GetItemAttr( ITEM_ATTR_POIRESIST ) 
		+ GetItemAttr( ITEM_ATTR_ALLRESIST ) 
		+ GetAlphaStat(USER_POI_RESIST) 
		+ m_fPlusParamByMagicFieldArray[USER_POI_RESIST] 
		+ GetItemAttr(ITEM_ATTR_LEVEL_PER_ALL_MAGIC_RESIST)/10.f*GetLevel());

	m_wPoiResist	+=	WORD((g_pBaseClassInfo[GetClass()].iRPoison
		+g_pBaseRaceInfo[GetClass()].iRPoison )
						* (GetLevel() - 1)
						+GetItemAttr(ITEM_ATTR_LEVEL_PER_POI_RESIST)/10.f*GetLevel());

	m_wPoiResist	+=	WORD(m_wPoiResist*GetAlphaStat(USER_POI_RESIST_PERCENT)
		+GetNewestInt()/15);

	m_wPoiResist	=	min(m_wPoiResist, WORD(75+GetAlphaStat(USER_POI_RESIST)));

	EffectDesc* pEffedtDesc = GetEffectDesc(__SKILL_IRONSKIN__);
	if(pEffedtDesc)//ö�� ��ų�϶��� 85%����
	{
		if( m_wPoiResist > 85 )
			m_wPoiResist = 85;
	}
	else
	{
		if( m_wPoiResist > 75 )
			m_wPoiResist = 75;
	}

	m_wPoiResist -= 75*m_byRest;
}


WORD CUser::GetArmorDefense()
{
	WORD wSumArmorDefense = 0;
	
	for(int i = 0; i < MAX_EQUIP_POOL; ++i)
	{
		if(m_bCurrentHand==1)
		{
			if(i==EQUIP_TYPE_RHAND2 || i==EQUIP_TYPE_LHAND2)
				continue;
		}
		else
		{
			if(i==EQUIP_TYPE_RHAND1 || i==EQUIP_TYPE_LHAND1)
				continue;
		}

		CItem* pItem = &m_pEquip[i];

		if(pItem)
		{		
			int iItemKind = pItem->m_wItemID / ITEM_DISTRIBUTE;

			if( iItemKind >=10 && iItemKind <= 17 )		// ItemArmor;
			{
				wSumArmorDefense += pItem->m_Item_Armor.wDefense;
			}
		}
	}

	return wSumArmorDefense;
}


void CUser::UpdatePhyResist()
{	
	////MIN [ Min { (ITEM_ATTR_PHYRESIST + ITEM_ATTR_LEVEL_PER_PHY_RESIST / 10 * char_LEV ) 
	//       * BaseClassInfo.Item_Efficiency/100
	//       + ����.skill.Rphysics + (class.LR_Physics + race.LR_Physics ) * ( LV - 1 )), 75 }
	//       + SKILL_ATTR_Defence_Mastery.min, 95 ]
	float fBaseDefense = ( g_pBaseClassInfo[GetClass()].iDR + g_pBaseRaceInfo[GetRace()].iDR ) / 10.f;
	m_wPhyResist = WORD(fBaseDefense + (GetItemAttr( ITEM_ATTR_PHYRESIST )+GetItemAttr(ITEM_ATTR_LEVEL_PER_PHY_RESIST)/10.*GetLevel())*g_pBaseClassInfo[GetClass()].byItem_Efficiency/100.f);
	m_wPhyResist	+=	WORD(GetAlphaStat(USER_PHY_RESIST)
		+m_fPlusParamByMagicFieldArray[USER_PHY_RESIST]
		+(g_pBaseClassInfo[GetClass()].iRPhisics
		+g_pBaseRaceInfo[GetClass()].iRPhisics ) * (GetLevel() - 1));

	m_wPhyResist	+=  WORD(GetAlphaStat(USER_PHY_RESIST_PERCENT)
		+GetArmorDefense()/10);
	
	m_wPhyResist	=	(WORD)min(m_wPhyResist, 75);
	if (GetSkillLevel(__SKILL_DEFENSEMASTERY__))
	{
		m_wPhyResist	+=	g_pEffectLayer->GetFormula16(__SKILL_DEFENSEMASTERY__, BYTE(GetSkillLevel(__SKILL_DEFENSEMASTERY__)-1), GetClass());
		m_wPhyResist	=	(WORD)min(m_wPhyResist, 95);
	}
	
	m_wPhyResist	-= 75*m_byRest;
}


int CUser::GetOriginalMoveSpeed() const
{
	return g_pBaseClassInfo[GetClass()].iMS + g_pBaseRaceInfo[GetRace()].iMS;
}


void CUser::UpdateMoveSpeed()
{
	// Class.MS + Race.MS + Item.MS + Skill.MS - (total.Item.weight/10)
	SetMoveSpeed( (float)GetOriginalMoveSpeed() + GetItemAttr( ITEM_ATTR_MOVE_SPEED ) );
	
	SetMoveSpeed( GetMoveSpeed() + GetAlphaStat(USER_MOVESPEED) + m_fPlusParamByMagicFieldArray[USER_MOVESPEED] );
	if( GetSkillLevel(__SKILL_SPEEDOVER__) != 0 )	// �нú� ��ų.
	{
		SetMoveSpeed( GetMoveSpeed() + GetMoveSpeed()*g_pEffectLayer->GetEffectInfo( __SKILL_SPEEDOVER__ )->Value[ GetSkillLevel(__SKILL_SPEEDOVER__)-1 ].nMin/100.f );
	}
	
	SetMoveSpeed( GetMoveSpeed() +  ( (GetMoveSpeed()*max(GetAlphaStat(USER_MOVESPEED_PERCENT), -100)) /100.f) );
}

float CUser::GetOriginalHealMPSpeed() const
{
	//(class.SPR + race.SPR) * (MAX.SP / 360 )
	return (g_pBaseClassInfo[GetClass()].iSPR + g_pBaseRaceInfo[GetRace()].iSPR)*(GetMaxSP()/3600.f);
}


float CUser::GetOriginalHealHPSpeed() const
{
	//(class.HPR + race.HPR)
	return (g_pBaseClassInfo[GetClass()].iHPR+g_pBaseRaceInfo[GetRace()].iHPR)/100.f;
}


void CUser::UpdateHealHPSec()
{
	//����HealHPSpeed + Item.HPR + �����׷��̼�.��ų.HPR
	// �ʴ� ȸ���� = (class.HPR + race.HPR ) / 100 + Item.HPR + �����׷��̼�.��ų.HPR ;
	/*float	fClassRace	= (g_pBaseClassInfo[ GetClass() ].iHPR + g_pBaseRaceInfo[ GetClass() ].iHPR) / 100.0f;
	float	fItemSkill	= m_pwItemAttr[ ITEM_ATTR_HEALSPEEDHP ];
	
	// �нú갡 �ִ��� Ȯ���Ѵ�.
	if( m_pwSkillLevel[ __SKILL_REGENERATION__ ] != 0 )
	{
		fItemSkill += g_pEffectLayer->GetEffectInfo( __SKILL_REGENERATION__ )->Value[ m_pwSkillLevel[ __SKILL_REGENERATION__ ]-1 ].nMin;
	}
	
	m_dwHealHPSec = ( fClassRace + fItemSkill + m_fPlusParamByMagicFieldArray[USER_HEALHPSPEED] );*/
	m_dwHealHPSec = DWORD(GetOriginalHealHPSpeed()+GetItemAttr( ITEM_ATTR_HEALSPEEDHP ));
	if( GetSkillLevel( __SKILL_REGENERATION__ ) != 0 )
	{
		m_dwHealHPSec += 
			g_pEffectLayer->GetFormula16(__SKILL_REGENERATION__, BYTE(GetSkillLevel( __SKILL_REGENERATION__ )-1), GetClass());
	}
	
	m_dwHealHPSec = max(m_dwHealHPSec, 1);
	m_dwHealHPSec += (DWORD)m_fPlusParamByMagicFieldArray[USER_HEALHPSPEED];
	m_dwHealHPSec = DWORD(m_dwHealHPSec*(100+GetAlphaStat(USER_HEALHPSPEED_PERCENT))/100.f);
	m_dwHealHPSec = m_dwHealHPSec*(m_byRest+1);	// �޽��Ҷ� 2��
}

void CUser::UpdateHealSPSec()
{
	//����HealMPSpeed * (100 + �޵����̼�.��ų.SPR+ item.SPR)/100 + (��������.SPR)
	// �ʴ� ȸ���� = (class.SPR + race.SPR) * (MAX.SP / 3600 ) * (100 + �޵����̼�.��ų.SPR+ item.SPR)/100 + (��������.SPR)
	/*float	fClassRace = (g_pBaseClassInfo[ GetClass() ].iSPR + g_pBaseRaceInfo[ GetRace() ].iSPR) * ( (float)GetMaxSP() / 3600.f );
	float	fItemSkill = 100 + m_pwItemAttr[ ITEM_ATTR_HEALSPEEDMP ];

	if( m_pwSkillLevel[ __SKILL_MEDITETION__ ] != 0 )
	{
		fItemSkill += g_pEffectLayer->GetEffectInfo( __SKILL_MEDITETION__ )->Value[ m_pwSkillLevel[ __SKILL_MEDITETION__ ]-1 ].nMin;
	}

	m_dwHealMPSec = max(( fClassRace * fItemSkill ) / 100.0f, 1);
	m_dwHealMPSec += m_fPlusParamByMagicFieldArray[USER_HEALMPSPEED];*/
	m_dwHealMPSec = (100+GetItemAttr( ITEM_ATTR_HEALSPEEDMP ));
	if( GetSkillLevel( __SKILL_MEDITETION__ ) != 0 )
	{		
		m_dwHealMPSec += g_pEffectLayer->GetFormula16(__SKILL_MEDITETION__, BYTE(GetSkillLevel( __SKILL_MEDITETION__ )-1), GetClass());
	}
	
	if( GetSkillLevel( __SKILL_HOLYBRAIN ) != 0 )
	{
		m_dwHealMPSec += g_pEffectLayer->GetFormula16(__SKILL_HOLYBRAIN, BYTE(GetSkillLevel( __SKILL_HOLYBRAIN )-1), GetClass());
	}
	m_dwHealMPSec = DWORD(m_dwHealMPSec*GetOriginalHealMPSpeed()/100.f);
	m_dwHealMPSec = max(m_dwHealMPSec, 1);
	m_dwHealMPSec += (DWORD)m_fPlusParamByMagicFieldArray[USER_HEALMPSPEED];

	m_dwHealMPSec = m_dwHealMPSec*(m_byRest+1);	// �޽��Ҷ� 2��	
}


void CUser::SetRest(BYTE bRest)
{
	if (m_sMatchDescInfo.m_bMatching == TRUE	&&
		GetUnitStatus() == UNIT_STATUS_SKILL	&&
		GetUnitStatus() == UNIT_STATUS_CASTING	&&
		GetUnitStatus() == UNIT_STATUS_DEAD	&&
		GetUnitStatus() == UNIT_STATUS_PORTAL_MOVING )
	{
		// 1:1 ����� �ƴϰ� ��ų ���, ĳ����, ����, ��Ż �̵��� �� �� ����.
		// Modify by seven
		return;
	}
	
	DSTC_PLAYER_REST pSendPacket;
	pSendPacket.byType		= bRest;
	pSendPacket.dwUserIndex	= GetID();
	CDungeonLayer* pLayer = GetCurDungeonLayer();
	if (pLayer)
		pLayer->BroadCastSectionMsg((char*)&pSendPacket, pSendPacket.GetPacketSize(), GetPrevSectionNum() );

	m_byRest	= bRest;
	
	UpdateHealHPSec();
	UpdateHealSPSec();
	UpdateBlockRate();
	UpdateAvoid();
	
	UpdateFireResist();
	UpdatePoiResist();
	UpdateIceResist();
	UpdateLightResist();
	UpdatePhyResist();

	DSTC_USER_STATUS UserStatus;
	UserStatus.bStatusMany = 0;
	UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HEALHPSPEED;
	UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_dwHealHPSec;
	UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HEALMPSPEED;
	UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_dwHealMPSec;
	UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_FIRE_RESIST;
	UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wFireResist;
	UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_ICE_RESIST;
	UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wIceResist;
	UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_LIGHT_RESIST;
	UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wLightResist;
	UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_POI_RESIST;
	UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wPoiResist;
	UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_PHY_RESIST;
	UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wPhyResist;
	UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_AVOID;
	UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wAvoid;

	NetSendToUser( m_dwConnectionIndex, (char*)&UserStatus, UserStatus.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);	
}


void CUser::UpdateAttackSpeed() 
{
	DWORD dwClassRace = 0;
	DWORD dwItem = 0;
	BYTE bCurrentHand = (m_bCurrentHand==1) ? EQUIP_TYPE_RHAND1 : EQUIP_TYPE_RHAND2;
	WORD wID = 0;
	
	if(CheckItem(&m_pEquip[bCurrentHand])==TRUE)
		wID = m_pEquip[bCurrentHand].GetID();

	CBaseItem* pItem = g_pBaseItemHash->GetData( wID );	
	if( pItem )
	{
		dwItem = pItem->BaseItem_Weapon.wBase_Speed;
	}

	dwClassRace = (g_pBaseClassInfo[ GetClass() ].iAS + g_pBaseRaceInfo[ GetRace() ].iAS ) +  dwItem;
	/*m_wAttackSpeed = WORD( dwClassRace 
								* ( 1.f - ( m_pwItemAttr[ ITEM_ATTR_ATTACK_SPEED_POINT ] / 100.f) ) 
								* ( 1.f + ( m_fPlusParam[ USER_ATTACKRATE_PERCENT ] / 100.f ) ) );
*/
	m_wAttackSpeed = WORD(dwClassRace*(100+GetAlphaStat(USER_ATTACKRATE_PERCENT ))/100.f);//*(100-m_pwItemAttr[ ITEM_ATTR_ATTACK_SPEED_POINT ])/100.;
	m_wAttackSpeed += WORD(m_fPlusParamByMagicFieldArray[USER_ATTACKRATE_PERCENT]);

	if (m_wAttackSpeed > GetItemAttr( ITEM_ATTR_ATTACK_SPEED_POINT )/3.f)
		m_wAttackSpeed -= WORD(GetItemAttr( ITEM_ATTR_ATTACK_SPEED_POINT )/3.f);
	else
		m_wAttackSpeed = 0;
}


DUNGEON_JOIN_FAIL CUser::IsEnterDungeon()
{
	if (GetLevel() > MAX_LEVEL)
		return DUNGEON_JOIN_FAIL_NOT_MINMAX_LEVEL;

	if (GetCurDungeon()->GetDungeonDataEx()->IsConquer())
	{
		if (!GetCurDungeon()->GetDungeonDataEx()->IsDungeonOwner(this))
		{
#if defined DEV_MODE || JAPAN_LOCALIZING
			// �Ϻ��� �������� �Ǵ�.  by minjin. 2004. 10. 27.
			if (GetCurDungeon()->GetDungeonDataEx()->m_byEntererMinLevel > GetLevel() ||
				GetCurDungeon()->GetDungeonDataEx()->m_byEntererMaxLevel < GetLevel())
			{
				// �������� �ɷǳ�?
				return DUNGEON_JOIN_FAIL_NOT_MINMAX_LEVEL;
			}
#endif
			// ����� ��ƶ�.
			if (m_dwMoney < GetCurDungeon()->GetDungeonDataEx()->GetEntrancePay())
			{
				return DUNGEON_JOIN_FAIL_NOT_ENOUGH_ENTER_KARZ;
			}
		}
	}

	return DUNGEON_JOIN_SUCCESS;
}


// �ڽ��� ��� ���¸� Ŭ���̾�Ʈ�� ������.
void CUser::SendAllStatus()
{
	UpdateAllStatus();	

	DSTC_USER_STATUS Status;
	
	Status.bStatusMany = 0;

	Status.pStatus[Status.bStatusMany].dwCode	= USER_MAXHP;
	Status.pStatus[Status.bStatusMany++].dwMin	= GetMaxHP(); 

	Status.pStatus[Status.bStatusMany].dwCode	= USER_HP;
	Status.pStatus[Status.bStatusMany++].dwMin	= GetHP(); 

	Status.pStatus[Status.bStatusMany].dwCode	= USER_MAXMP;
	Status.pStatus[Status.bStatusMany++].dwMin	= GetMaxSP(); 

	Status.pStatus[Status.bStatusMany].dwCode	= USER_MP;
	Status.pStatus[Status.bStatusMany++].dwMin	= GetSP(); 

	Status.pStatus[Status.bStatusMany].dwCode	= USER_HONOR;
	Status.pStatus[Status.bStatusMany++].dwMin	= m_dwHonor;

	Status.pStatus[Status.bStatusMany].dwCode	= USER_EGO;			
	Status.pStatus[Status.bStatusMany++].dwMin	= GetNewestEgo(); 

	Status.pStatus[Status.bStatusMany].dwCode	= USER_STR;			
	Status.pStatus[Status.bStatusMany++].dwMin	= GetNewestStr(); 

	Status.pStatus[Status.bStatusMany].dwCode	= USER_INT;			
	Status.pStatus[Status.bStatusMany++].dwMin	= GetNewestInt(); 

	Status.pStatus[Status.bStatusMany].dwCode	= USER_DEX;			
	Status.pStatus[Status.bStatusMany++].dwMin	= GetNewestDex();

	Status.pStatus[Status.bStatusMany].dwCode	= USER_VIT;			
	Status.pStatus[Status.bStatusMany++].dwMin	= GetNewestVit(); 

	Status.pStatus[Status.bStatusMany].dwCode	= USER_LUCK;			
	Status.pStatus[Status.bStatusMany++].dwMin	= m_dwLuck; 

	Status.pStatus[Status.bStatusMany].dwCode	= USER_ATTACK_R;		
	Status.pStatus[Status.bStatusMany].dwMin	= GetAttackDamageMin_R();
	Status.pStatus[Status.bStatusMany++].dwMax	= GetAttackDamageMax_R();

	Status.pStatus[Status.bStatusMany].dwCode	= USER_ATTACK_L;		
	Status.pStatus[Status.bStatusMany].dwMin	= GetAttackDamageMin_L();
	Status.pStatus[Status.bStatusMany++].dwMax	= GetAttackDamageMax_L();

	Status.pStatus[Status.bStatusMany].dwCode	= USER_AA;				
	Status.pStatus[Status.bStatusMany++].dwMin	= m_wAttackAcuracy; 

	Status.pStatus[Status.bStatusMany].dwCode	= USER_AVOID;			
	Status.pStatus[Status.bStatusMany++].dwMin	= m_wAvoid; 

	Status.pStatus[Status.bStatusMany].dwCode	= USER_BLOCKRATE;		
	Status.pStatus[Status.bStatusMany++].dwMin	= m_wBlockRate; 

	Status.pStatus[Status.bStatusMany].dwCode	= USER_FIRE_RESIST;	
	Status.pStatus[Status.bStatusMany++].dwMin	= m_wFireResist; 

	Status.pStatus[Status.bStatusMany].dwCode	= USER_ICE_RESIST;	
	Status.pStatus[Status.bStatusMany++].dwMin	= m_wIceResist;

	Status.pStatus[Status.bStatusMany].dwCode	= USER_LIGHT_RESIST;	
	Status.pStatus[Status.bStatusMany++].dwMin	= m_wLightResist;

	Status.pStatus[Status.bStatusMany].dwCode	= USER_POI_RESIST;		
	Status.pStatus[Status.bStatusMany++].dwMin	= m_wPoiResist;

	Status.pStatus[Status.bStatusMany].dwCode	= USER_PHY_RESIST;		
	Status.pStatus[Status.bStatusMany++].dwMin	= m_wPhyResist;	

	WORD	wPerWeight		= GetAverageWeight();
	BYTE	bySpeedIndex	= 0;
	BYTE	bySpeedAftIndex	= 0;
	float	fUserSpeed		= 0.0f;
	
	if(m_wPerWeight >= WEIGTH_100PER_OVER)
	{
		bySpeedIndex = 2;
	}
	else if(m_wPerWeight >= WEIGTH_80PER_OVER)
	{
		bySpeedIndex = 1;
	}

	m_wPerWeight = wPerWeight;		
	
	if(m_wPerWeight >= WEIGTH_100PER_OVER)
	{
		bySpeedAftIndex = 2;
	}
	else if(m_wPerWeight >= WEIGTH_80PER_OVER)
	{
		bySpeedAftIndex = 1;
	}

	if(bySpeedIndex != bySpeedAftIndex)
	{
		if(bySpeedAftIndex >= 1)
		{
			fUserSpeed = m_fMoveSpeed / 3;
		}

		CDungeonLayer*	pLayer = GetCurDungeonLayer();				
		if(pLayer)
		{
			DSTC_USER_SPEED	pSpeedPacket;
			pSpeedPacket.dwUserIndex	= GetID();
			pSpeedPacket.fSpeed			= (m_fMoveSpeed - fUserSpeed) / 1000.f;		
			pLayer->BroadCastSectionMsg((char*)&pSpeedPacket, pSpeedPacket.GetPacketSize(), GetPrevSectionNum());
		}		
	}
	
	if(m_wPerWeight >= WEIGTH_80PER_OVER)
	{
		fUserSpeed = m_fMoveSpeed / 3.f;
	}

	Status.pStatus[Status.bStatusMany].dwCode	= USER_MOVESPEED;	
	Status.pStatus[Status.bStatusMany++].dwMin	= DWORD(m_fMoveSpeed - fUserSpeed);
	
	Status.pStatus[Status.bStatusMany].dwCode	= USER_HEALHPSPEED;		
	Status.pStatus[Status.bStatusMany++].dwMin	= m_dwHealHPSec;

	Status.pStatus[Status.bStatusMany].dwCode	= USER_HEALMPSPEED;		
	Status.pStatus[Status.bStatusMany++].dwMin	= m_dwHealMPSec;
	
	Status.pStatus[Status.bStatusMany].dwCode	= USER_ATTACKRATE_PERCENT;
	Status.pStatus[Status.bStatusMany++].dwMin	= m_wAttackSpeed;	
	
	NetSendToUser( m_dwConnectionIndex, (char*)&Status, Status.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);

	SendAllUserSkillLevel();
}


enum PORTAL_TYPE CUser::GetPortalType() const
{
	return (enum PORTAL_TYPE)m_ePortalType;
}


void CUser::SetPortalType(enum PORTAL_TYPE	ePortalType)
{
	m_ePortalType = ePortalType;
}


void CUser::SendAllUserSkillLevel()
{
	DSTC_USER_SKILL_LEVEL	SkillLevel;

	for( int i = 0; i < MAX_SKILL; i++ )
	{
		int nLevel = GetSkillLevel( BYTE(i) );
		SkillLevel.pbSkillLevel[i] = 		nLevel;
	}

	NetSendToUser( m_dwConnectionIndex, (char*)&SkillLevel, SkillLevel.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
}


USER_PROPERTY_CODE CUser::SetDamage(DWORD dwOffenseLevel, DWORD dwDamage )
{
	USER_PROPERTY_CODE eResult = USER_HP;

	if( !GetEffectDesc(__SKILL_AIREALCOAT__ ))	// ���������� HP�� ��´�.
	{
		if (GetItemAttr(ITEM_ATTR_PHY_DMG_TO_SP))
		{
			DWORD dwTempDamage = DWORD(dwDamage*GetItemAttr(ITEM_ATTR_PHY_DMG_TO_SP)/100.f);
			if( ( (int)GetSP() - (int)dwTempDamage ) <= 0 )
			{
				SetSP(0);
			}
			else
			{
				SetSP(WORD(GetSP()-dwTempDamage));
			}
			
			eResult = USER_MP;
			dwDamage -= dwTempDamage;
		}
		
		if( ( (int)GetHP() - (int)dwDamage ) <= 0 ) 
		{
			SetHP(0);
		}
		else
		{
			SetHP(WORD(GetHP() - dwDamage));
		}
	}
	else		// SP�� ��´�.
	{
		// HP��·� : ���������� * (100 - ��ų.min) / 100
		float fDamage = dwDamage * (100-GetEffectDesc(__SKILL_AIREALCOAT__)
							->pEffect->Value[GetEffectDesc(__SKILL_AIREALCOAT__)
							->bSkillLevel].nMin) / 100.f;

		if( ( (int)GetHP() - (int)fDamage ) <= 0 )
		{
			SetHP(0);
		}
		else 
		{
			SetHP(WORD(GetHP() - fDamage));
		}
		
		// SP��·� : ���������� * ��ų.min / 100 * skill.max/100 
		//(�������������� - HP��·�) * skill.max/100 �� ���� ��..
		fDamage = (dwDamage-fDamage)*(GetEffectDesc(__SKILL_AIREALCOAT__)
					->pEffect->Value[GetEffectDesc(__SKILL_AIREALCOAT__)
					->bSkillLevel].nMax+GetItemAttr(ITEM_ATTR_PHY_DMG_TO_SP)) / 100.f;
		
		if( ( (int)GetSP() - (int)fDamage ) <= 0 ) SetSP(0);
		else SetSP(WORD(GetSP()-fDamage));

		if( GetSP() ==  0 )	// ��ų�� ����.
		{
			DetachSkill( GetEffectDesc(__SKILL_AIREALCOAT__ ) );
		}

		eResult =  USER_MP;
	}
	
	// ��ų ĳ���� ���̾� Ǯ�� ����.
	if (GetUnitStatus() == UNIT_STATUS_CASTING && GetHP() && dwDamage)
	{
		if ((GetLevel()+10)/float(dwOffenseLevel+GetLevel()+10)*100+20 
			< rand()%100)
		{
			SendSkillCastingFail(SKILL_CASTING_FAIL_REASON_GET_SLAPPED);
		}
	}

	return eResult;
}

void CUser::SendSkillCastingFail(ENUM_SKILL_CASTING_FAIL_REASON eFailReason)
{
	CDungeonLayer* pLayer = GetCurDungeonLayer();
	if( pLayer )
	{
		SetStatus(UNIT_STATUS_NORMAL);
		DSTC_DUNGEON_CASTING_FAIL packet;
		packet.dwUserIndex = GetID();
		packet.byErrorReason = (BYTE)eFailReason;
		
		pLayer->BroadCastSectionMsg((char*)&packet, packet.GetPacketSize(), GetPrevSectionNum());
	}
}

short CUser::GetAllResist() const
{
	return GetItemAttr(ITEM_ATTR_ALLRESIST);
}

void CUser::SetDamageOver(const CUnit* pUnit, DWORD dwDamage)
{
	if( g_dwTickCount - m_dwTemp[ USER_TEMP_DAMAGETICK ] > 8000 )
	{
		m_dwTemp[ USER_TEMP_DAMAGE ] = 0;
	}
		
	if( m_dwTemp[ USER_TEMP_DAMAGE ] < dwDamage )
	{
		// ���� ������� �ִٸ� �������� �����ϰ� �Ѵ�.
		for( int i=0; i<MAX_USER_GUARDIAN; i++ )
		{
			if( GetGuardian(BYTE(i)) )
			{
				// ���� �������� ����� ���ٸ�.
				if( (!GetGuardian(BYTE(i))->GetUnitForAI()) 
					&& !IsAlliance(pUnit))
				{
					GetGuardian(BYTE(i))->SetTargetAI( pUnit );
				}
			}
		}

		for( i=0; i<MAX_USER_GUARDIAN; i++ )
		{
			if( m_pMonster[i] )
			{
				// ���� �������� ����� ���ٸ�.
				if( (!m_pMonster[i]->GetUnitForAI()) 
					&& !IsAlliance(pUnit))
				{
					m_pMonster[i]->SetTargetAI( pUnit );
				}
			}
		}
	}

	m_dwTemp[ USER_TEMP_DAMAGETICK ] = g_dwTickCount;
}



/*
void CUser::SetDamageOver( CUser* pUser, DWORD dwDamage )
{
	if( g_dwTickCount - m_dwTemp[ USER_TEMP_DAMAGETICK ] > 8000 )
	{
		m_dwTemp[ USER_TEMP_DAMAGE ] = 0;
	}
	
	if( m_dwTemp[ USER_TEMP_DAMAGE ] < dwDamage )
	{
		// ���� ������� �ִٸ� �������� �����ϰ� �Ѵ�.
		for( int i=0; i<MAX_USER_GUARDIAN; i++ )
		{
			if( m_pGuardian[i] )
			{
				// ���� �������� ����� ���ٸ�.
				if( (!m_pGuardian[i]->m_pUserForAI) && (!m_pGuardian[i]->m_pMonsterForAI) 
					&& !IsAlliance(pUser))

					m_pGuardian[i]->SetTargetAI( pUser );
			}
		}

		for( i=0; i<MAX_USER_GUARDIAN; i++ )
		{
			if( m_pMonster[i] )
			{
				// ���� �������� ����� ���ٸ�.
				if( (!m_pMonster[i]->m_pUserForAI) && (!m_pMonster[i]->m_pMonsterForAI) 
					&& !IsAlliance(pUser))
					
					m_pMonster[i]->SetTargetAI( pUser );
			}
		}
	}

	m_dwTemp[ USER_TEMP_DAMAGETICK ] = g_dwTickCount;
}

void CUser::SetDamageOver( CMonster* pMonster, DWORD dwDamage )
{
	if( !pMonster ) return;
	if( pMonster->GetLord() == this ) return;

	if( g_dwTickCount - m_dwTemp[ USER_TEMP_DAMAGETICK ] > 8000 )
	{
		m_dwTemp[ USER_TEMP_DAMAGE ] = 0;
	}
	
	if( m_dwTemp[ USER_TEMP_DAMAGE ] < dwDamage )
	{
		// ���� ������� �ִٸ� �������� �����ϰ� �Ѵ�.
		for( int i=0; i<MAX_USER_GUARDIAN; i++ )
		{
			if( m_pGuardian[i] )
			{
				// ���� �������� ����� ���ٸ�.
				if( (!m_pGuardian[i]->m_pUserForAI) && (!m_pGuardian[i]->m_pMonsterForAI)
					&& !IsAlliance(pMonster) )

					m_pGuardian[i]->SetTargetAI( pMonster );
			}
		}

		for( i=0; i<MAX_USER_GUARDIAN; i++ )
		{
			if( m_pMonster[i] )
			{
				// ���� �������� ����� ���ٸ�.
				if( (!m_pMonster[i]->m_pUserForAI) && (!m_pMonster[i]->m_pMonsterForAI) 
					&& !IsAlliance(pMonster) )
					
					m_pMonster[i]->SetTargetAI( pMonster );
			}
		}
	}
                           
	m_dwTemp[ USER_TEMP_DAMAGETICK ] = g_dwTickCount;
}
*/
void CUser::SetAttackTypeAndFrame( const CUnit* pUnit, DWORD dwType )
{
	// �̻�Ȳ�̸� ������ �ʱ�ȭ.
	if( m_dwTemp[ USER_TEMP_LASTATTACKTARGET ] != 1 )
	{
		m_dwTemp[ USER_TEMP_LASTATTACKTARGET ] = 1;
		m_sAttackDescInfo.m_bAttackType		= BYTE(rand()%3);
		m_sAttackDescInfo.m_bAttackFrame	= 0;
	}
	else if( pUnit->GetID() != m_dwTemp[ USER_TEMP_LASTTARGETINDEX ] )
	{
		m_dwTemp[ USER_TEMP_LASTTARGETINDEX ] = pUnit->GetID();
		m_sAttackDescInfo.m_bAttackType		= BYTE(rand()%3);
		m_sAttackDescInfo.m_bAttackFrame	= 0;
	}
	
	// �ϴ� ���⼭ �������� ���� ��������Ѵٳ�..
	if( m_sAttackDescInfo.m_bAttackFrame == 2 )	// �������� �� ���ư��ٸ�.
	{
		m_sAttackDescInfo.m_bAttackType		= BYTE(rand()%3);
		m_sAttackDescInfo.m_bAttackFrame	= 0;
	}
	else
	{
		m_sAttackDescInfo.m_bAttackFrame++;
	}

	// �������� �������ش�.
	m_sAttackDescInfo.m_bCurAttackType	= m_sAttackDescInfo.m_bAttackType;
	m_sAttackDescInfo.m_bCurAttackFrame	= m_sAttackDescInfo.m_bAttackFrame;
	
	// ���� ������ �غ�.
	switch( dwType )                                                        
	{
	case 1:		// Block.
	case 2:		// Miss.
		{
			// ���� �׼��� �ʱ�ȭ ���ش�.
			m_sAttackDescInfo.m_bAttackType		= BYTE(rand()%3);
			m_sAttackDescInfo.m_bAttackFrame	= 0;
		}
		break;

	case 4:		// Attack.
		{

		}
		break;
	}
}

void CUser::AddExp(int iExp, BOOL bAlphaExp)
{
	if( iExp <= 0 )								return;
	if (GetUnitStatus() == UNIT_STATUS_DEAD)	return;
	if( GetLevel() == 255 )						return;
	
	if (bAlphaExp)
	{
		// ��������ȹ�����ġ * (100 + GLOBAL_EXP_GAIN + ITEM_ATTR_EXP_GAIN - ITEM_ATTR_EXP_LOSS) / 100
		m_dwExp += DWORD(iExp*(1+m_bConnectToGameRoom*g_pThis->m_GLOBAL_EXP_GAIN_DOUBLE)
			*(100+g_pThis->m_dwGLOBAL_EXP_GAIN + GetItemAttr(ITEM_ATTR_EXP_GAIN) - GetItemAttr(ITEM_ATTR_EXP_LOSS))/100.f);
	}
	else
	{
		m_dwExp += iExp;
	}
	
	// �� �������� ����ġ�� �÷��ش�.
	for( BYTE i=0; i<MAX_USER_GUARDIAN; i++ )
	{
		if( GetGuardian(i) )
		{
			GetGuardian(i)->AddExp( iExp );
		}
	}

	DSTC_USER_STATUS UserStatus;
	UserStatus.bStatusMany			= 1;
	UserStatus.pStatus[0].dwCode	= USER_EXP;
	UserStatus.pStatus[0].dwMin		= m_dwExp;
	NetSendToUser( m_dwConnectionIndex, (char*)&UserStatus, UserStatus.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
	
	while( m_dwExp >= GetExpTableOfLevel(OBJECT_TYPE_PLAYER, GetLevel() + 1) )		// Level Up^^
	{
		LevelUP();
	}
}

void CUser::SubExp( int iExp )
{
	if( iExp <= 0 ) return;

	DWORD dwLevelExp = ( GetLevel() != 1 ) ? GetExpTableOfLevel(OBJECT_TYPE_PLAYER, GetLevel() - 1 ) : 0;
	if( dwLevelExp < ( m_dwExp - iExp ) )
	{
		m_dwExp -= iExp;
	}

	DSTC_USER_STATUS	UserStatus;
	UserStatus.bStatusMany			= 1;
	UserStatus.pStatus[0].dwCode	= USER_EXP;
	UserStatus.pStatus[0].dwMin		= m_dwExp;
	NetSendToUser( m_dwConnectionIndex, (char*)&UserStatus, UserStatus.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
}



void CUser::SetExchangePKCount(int nCount, DWORD dwPKEndTime)
{	
	if (nCount < 0 && m_sPKDescInfo.m_bPKCount+nCount < 0)
		nCount = -m_sPKDescInfo.m_bPKCount;
	else if(nCount > 0 && 0xff < m_sPKDescInfo.m_bPKCount+nCount)
		nCount = 0xff-m_sPKDescInfo.m_bPKCount;

	m_sPKDescInfo.m_bPKCount+=nCount;
	m_sPKDescInfo.m_dwPKTime = dwPKEndTime;

	char szQuery[0xff]={0,};
	wsprintf(szQuery, "update characterinfo set pkcount = %d, pktime = %d where char_index = %d"
		, m_sPKDescInfo.m_bPKCount, dwPKEndTime, GetID());
	g_pDb->THExecuteSQL(szQuery, FALSE, FALSE, NULL, (BYTE)GAME_DB);

	DSTC_PKCOUNT packet;
	packet.bPKCount	= m_sPKDescInfo.m_bPKCount;
	packet.bPKRepeatCount	 = m_sPKDescInfo.m_bPKRepeatCount;
	NetSendToUser( m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
	
	if (!m_sPKDescInfo.m_pPKList && m_sPKDescInfo.m_bPKCount)
		m_sPKDescInfo.m_pPKList = g_pThis->m_pPKUserCheckList->AddTail(this);	
}

void CUser::SetExchangePKRepeatCount(int nCount, DWORD dwPKRepeatEndTime)
{
	if (nCount < 0 && m_sPKDescInfo.m_bPKRepeatCount+nCount < 0)
		nCount = -m_sPKDescInfo.m_bPKRepeatCount;
	else if(nCount > 0 && 0xff < m_sPKDescInfo.m_bPKRepeatCount+nCount)
		nCount = 0xff-m_sPKDescInfo.m_bPKRepeatCount;

	m_sPKDescInfo.m_bPKRepeatCount+=nCount;
	m_sPKDescInfo.m_dwPKRepeatTime = dwPKRepeatEndTime;
	char szQuery[0xff]={0,};
	
	wsprintf(szQuery, "update characterinfo set pkrepeatcount = %d, pkrepeattime = %d where char_index = %d"
		, m_sPKDescInfo.m_bPKRepeatCount, dwPKRepeatEndTime, GetID());
	g_pDb->THExecuteSQL(szQuery, FALSE, FALSE, NULL, (BYTE)GAME_DB);

	DSTC_PKCOUNT packet;
	packet.bPKCount	= m_sPKDescInfo.m_bPKCount;
	packet.bPKRepeatCount	 = m_sPKDescInfo.m_bPKRepeatCount;
	NetSendToUser( m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);

	if (!m_sPKDescInfo.m_pPKList && m_sPKDescInfo.m_bPKRepeatCount)
		m_sPKDescInfo.m_pPKList = g_pThis->m_pPKUserCheckList->AddTail(this);
}

void CUser::AddHackCount(BOOL bNewMethod)
{
	m_dwTemp[ USER_TEMP_HACK ]++;

	if( m_dwTemp[ USER_TEMP_HACK ] >= 3 )		// ���������� ���� ������ ����̸� ������ ���´�.
	{
		// db�� �α� �����. 
		SpeedHackLog( this, 7777 );
		
		DSTC_DUNGEON_ERROR_CODE Error;

		NetSendToUser( m_dwConnectionIndex, (char*)&Error, Error.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);

		m_dwTemp[ USER_TEMP_HACK ] = 0;
		//GoToWorld(m_bDestWorldSpot);
	}
}

void CUser::LevelUP()
{
	CDungeonLayer* pLayer = GetCurDungeonLayer();
	if( !pLayer ) return;

	SetLevel(GetLevel()+1);
	SetSP(GetMaxSP());
	SetHP(GetMaxHP());

	DSTC_LEVELUP LevelUP;
	LevelUP.dwUserIndex = GetID();
	LevelUP.dwLevel		= GetLevel();
	LevelUP.byStatPoint	= GetStatPointByLevel(GetObjectType(), GetLevel()-2);

	pLayer->BroadCastSectionMsg( (char*)&LevelUP, LevelUP.GetPacketSize(), GetPrevSectionNum() );

	SetStatPoint(WORD(GetNotUseStatPoint()+GetStatPointByLevel(GetObjectType(), GetLevel()-2)));
	m_wPointSkill	+= 1;

	WSTDS_PARTY_LEVEL pLevelPacket;
	pLevelPacket.dwLevel		= GetLevel();
	pLevelPacket.dwPartyId		= m_dwPartyId;
	pLevelPacket.dwUserIndex	= GetID();
	
	g_pNet->SendToServer(WSINDEX, (char*)&pLevelPacket, pLevelPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

	SendAllStatus();
	
	// ���� �� ������ DB������Ʈ.
	UpdateUserDBThread( this );
}


void CUser::SetGodMode(ENUM_GODMODE_STATUS eGodModeStatus)
{
	m_enGodMode = eGodModeStatus;
	m_dwTemp[USER_TEMP_LASTGODMODETICK] = 0;
}


void CUser::ChangeGodMode(ENUM_GODMODE_STATUS eGodModeStatus)
{
	SetGodMode(eGodModeStatus);

	switch (eGodModeStatus)
	{
	case GODMODE_STATUS_MAPLOADING:
		::UpdateUserForAI( this );
		SetStatus(UNIT_STATUS_NORMAL);
		break;	
	case GODMODE_STATUS_EVENT_TIMER:
		// �ٷ� Ǯ���� �ʰ� GODMODE_TIME �����̰� �����ϰ� �˴ϴ�.
		// �ٷ� Ǯ���� �Ҷ��� SetGodMode �ֿ��� �ּ���		
		m_dwTemp[USER_TEMP_LASTGODMODETICK] = g_dwTickCount+GODMODE_TIME;
		break;
	case GODMODE_STATUS_RELEASE:		
		break;
	}		
}


// pk zone�� �ֳ�?
BOOL CUser::IsPKZone()
{
#ifndef	JAPAN_LOCALIZING
	return (GetCurDungeon()->GetDungeonDataEx()->IsConquer() && GetCurLayerIndex()) || 
			(!GetCurDungeon()->GetDungeonDataEx()->IsPathWay() && 
			 !GetCurDungeon()->GetDungeonDataEx()->IsVillage() && 
			 GetCurDungeon()->GetTotalLayer()-1 == GetCurLayerIndex());
#else
	return ( g_pThis->m_bAdultMode || (GetCurDungeon()->GetDungeonDataEx()->IsConquer() && GetCurLayerIndex()) );
#endif
}


void CUser::SetPKMode(BOOL bPKMode)
{
	m_sPKDescInfo.m_bPKMode = (BYTE)bPKMode;
	DSTC_DUNGEON_PKMODE packet;
	packet.bPKMode = (BYTE)bPKMode;
	NetSendToUser(m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}


void CUser::SetMoney(DWORD dwMoney)
{
	m_dwMoney = dwMoney;
}


DWORD CUser::GetMoney()
{
	return m_dwMoney;
}


// ���¸������� ����Ǿ������� ������ ��°��� ��
void CUser::SetStatusFromSkillStatusValue(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass, float* pResetValue, BOOL bUserStatusSend)
{
	// �����Ǵ� ������� ���¸����� ���� �ɷ�ġ ��ȯ���� ���.
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);
	if ((char*)bSkillLevel<0)	return;

	int					nCount		= 0;
	DSTC_USER_STATUS	UserStatus;
	UserStatus.bStatusMany			= 0;
		
	BYTE bKind = BYTE(GetCurrentWeaponID()/ITEM_DISTRIBUTE);
		
	for (int i = 0; i < 5; ++i )
	{
		float fResult = 0;
		USER_PROPERTY_CODE wStatusID = (USER_PROPERTY_CODE)pEffect->sStatusValue[i].wStatusID;
		BYTE bFormula = pEffect->sStatusValue[i].bFormula;
		if (!wStatusID)	break;
		
		switch(wStatusID)
		{
		case USER_AVOID_PERCENT:
		case USER_AVOID:
			switch(bFormula)
			{
			case 16:
				fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass);
				break;
			case 19:
				fResult = (float)g_pEffectLayer->GetFormula19(bSkillKind, bSkillLevel, wClass);
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdateAvoid();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_AVOID;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wAvoid;
			break;
		case USER_MAXHP_PERCENT:
		case USER_MAXHP:
			switch(bFormula)
			{
			case 16:
				
				fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass);
				
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdateMaxHP();
			UpdateHealHPSec();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MAXHP;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= GetMaxHP();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HEALHPSPEED;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_dwHealHPSec;
			break;

		case USER_HP_PERCENT:
			switch(bFormula)
			{
			case 16:
				//m_fPlusParam[wStatusID] = 0;
				fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass);
				//m_fPlusParam[wStatusID] += fResult;
				break;
			}
			SetHP(WORD(GetHP() * (100+fResult)/100.f));
			if( GetHP() > GetMaxHP() ) SetHP((WORD)GetMaxHP());
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HP;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= GetHP();
			break;
		case USER_HP:
			switch(bFormula)
			{
			case 16:
				//m_fPlusParam[wStatusID] = 0;
				fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass);
				//m_fPlusParam[wStatusID] += fResult;
				break;
			}
			SetHP(WORD(GetHP() + fResult));
			if( GetHP() > GetMaxHP() ) SetHP((WORD)GetMaxHP());
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HP;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= GetHP();
			break;
		case USER_MOVESPEED_PERCENT:
			{
				switch(bFormula)
				{
				case 16:
					fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass);
					break;
				case 25:	
					fResult = (float)g_pEffectLayer->GetFormula25(bSkillKind, bSkillLevel, wClass);
					break;
				}
				SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
				UpdateMoveSpeed();
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MOVESPEED;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= (DWORD)m_fMoveSpeed;
			}break;
		case USER_MAXMP_PERCENT:
		case USER_MAXMP:
			switch(bFormula)
			{
			case 16:
				fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass);
				
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdateMaxSP();
			UpdateHealSPSec();

			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MAXMP;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= GetMaxSP();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HEALMPSPEED;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_dwHealMPSec;
			break;
	
		case USER_MP:
			switch(bFormula)
			{
			case 16:
				fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass);
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			SetSP(WORD(GetSP()+fResult));
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MP;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= GetSP();
			break;

		case USER_CASTINGTIME:
			switch(bFormula)
			{
			case 16:
				fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass);
				
				break;
			case 17:
				fResult = g_pEffectLayer->GetFormula17(bSkillKind, bSkillLevel, wClass);
				
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			break;

		case USER_COOLTIME:
			switch(bFormula)
			{
			case 16:
				fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass);
				
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			break;
		case USER_ATTACK_R_PERCENT:
		case USER_ATTACK_R:
			{
				
				switch(bFormula)
				{
				case 16:
					fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass);
					break;
				}
				SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
				WORD wMinMax[2] = {0, 0};
				GetAttackDamage(GetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON)
					, GetSkillLevel(GetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON))
					, bKind, &wMinMax[0], &wMinMax[1], SELECT_ATTACK_TYPE_RBUTTON);

				SetAttackDamage_R(wMinMax[0], wMinMax[1]);
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_ATTACK_R;
				UserStatus.pStatus[UserStatus.bStatusMany].dwMin	= GetAttackDamageMin_R();
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMax	= GetAttackDamageMax_R();
			}
			
			break;

		case USER_ATTACK_L_PERCENT:
		case USER_ATTACK_L:
			{
				switch(bFormula)
				{
				case 16:
					fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass);
					break;
				}
				SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
				WORD wMinMax[2] = {0, 0};
				
				GetAttackDamage(GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON)
					, GetSkillLevel(GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON)), bKind
					, &wMinMax[0], &wMinMax[1], SELECT_ATTACK_TYPE_LBUTTON);

				SetAttackDamage_L(wMinMax[0], wMinMax[1]);
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_ATTACK_L;
				UserStatus.pStatus[UserStatus.bStatusMany].dwMin	= GetAttackDamageMin_L();
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMax	= GetAttackDamageMax_L();
			}
				
			break;
		case USER_SKILLATTACK_R_PERCENT:
			{			
				switch(bFormula)
				{
					case 16:
					{
						fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass);
					}
					break;					
				}

				SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
				WORD wMinMax[2] = {0, 0};
				
				GetAttackDamage(GetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON)
					, GetSkillLevel(GetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON))
					, bKind, &wMinMax[0], &wMinMax[1], SELECT_ATTACK_TYPE_RBUTTON);
				SetAttackDamage_L(wMinMax[0], wMinMax[1]);
				
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_ATTACK_R;
				UserStatus.pStatus[UserStatus.bStatusMany].dwMin	= GetAttackDamageMin_R();
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMax	= GetAttackDamageMax_R();
			}
			break;
		case USER_SKILLATTACK_L_PERCENT:
			{
				switch(bFormula)
				{
					case 16:
					{
						fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass);					
					}
					break;					
				}			

				SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
		
				WORD wMinMax[2] = {0, 0};
				GetAttackDamage(GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON)
					, GetSkillLevel(GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON))
					, bKind, &wMinMax[0], &wMinMax[1], SELECT_ATTACK_TYPE_LBUTTON);

				SetAttackDamage_L(wMinMax[0], wMinMax[1]);

				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_ATTACK_L;
				UserStatus.pStatus[UserStatus.bStatusMany].dwMin	= GetAttackDamageMin_L();
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMax	= GetAttackDamageMax_L();
			}
			break;
					
		case USER_FIRE_RESIST:
			switch(bFormula)
			{
			case 15:
				fResult = g_pEffectLayer->GetFormula15(bSkillKind, bSkillLevel
					, m_wFireResist);
				
				break;
			case 16:
				fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind
					, bSkillLevel, wClass);
				
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdateFireResist();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_FIRE_RESIST;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wFireResist;
			break;

		case USER_ICE_RESIST:
			switch(bFormula)
			{
			case 15:
				fResult = g_pEffectLayer->GetFormula15(bSkillKind, bSkillLevel
					, m_wIceResist);
				
				break;
			case 16:
				fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel
					, wClass);
				
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdateIceResist();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_ICE_RESIST;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wIceResist;
			break;

		case USER_LIGHT_RESIST:
			switch(bFormula)
			{
			case 15:
				fResult = g_pEffectLayer->GetFormula15(bSkillKind, bSkillLevel
					, m_wLightResist);
				
				break;
			case 16:
				fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel
					, wClass);
				
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdateLightResist();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_LIGHT_RESIST;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wLightResist;
			break;
		case USER_POI_RESIST_PERCENT:
		case USER_POI_RESIST:
			switch(bFormula)
			{
			case 16:
				fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel
					, wClass);
				
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdatePoiResist();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_POI_RESIST;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wPoiResist;
			break;

		case USER_PHY_RESIST:
			switch(bFormula)
			{
			case 15:
				fResult = g_pEffectLayer->GetFormula15(bSkillKind, bSkillLevel
					, m_wPhyResist);
				
				break;
			case 16:
				fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel
					, wClass);
				
				break;
			case 24:
				fResult = (float)g_pEffectLayer->GetFormula24(bSkillKind, bSkillLevel
					, wClass);
				
				break;

			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdatePhyResist();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_PHY_RESIST;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wPhyResist;
			break;
		case USER_MAX_RESIST:
			switch(bFormula)
			{
			case 23:
				fResult = g_pEffectLayer->GetFormula23(bSkillLevel);
				
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MAX_RESIST;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= 
				(DWORD)GetAlphaStat(wStatusID);
			break;
		case USER_BLOCKRATE_PERCENT:
		case USER_BLOCKRATE:
			switch(bFormula)
			{
			case 16:
				fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass);
				
				break;

			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdateBlockRate();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_BLOCKRATE;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wBlockRate;
			break;
		case USER_ATTACKACURACY_PERCENT:
			switch(bFormula)
			{
			case 16:
				fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass);
				
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdateAttackAcuracy();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_ATTACKACURACY_PERCENT;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wAttackAcuracy;
			break;	

		case USER_ATTACKRATE_PERCENT:
			switch(bFormula)
			{
			case 16:
				fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass);
				
				break;
			case 19:
				fResult = (float)g_pEffectLayer->GetFormula19(bSkillKind, bSkillLevel, wClass);
				
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdateAttackSpeed();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_ATTACKRATE_PERCENT;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wAttackSpeed;
			break;
		case USER_POISONING:
			switch(bFormula)
			{
			case 16:
				fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass);
				
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_POISONING;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= (DWORD)GetAlphaStat(wStatusID);
			break;
		
		case USER_DEC_SKILLLEVEL:
			switch(bFormula)
			{
			case 16:
				fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass);
				
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_DEC_SKILLLEVEL;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= (DWORD)GetAlphaStat(wStatusID);
			// ������ε� ���� 204.02.16 �迵�� --> UpdateAllStatus() �߰� 
			UpdateAD();//UpdateAllStatus();

			break;

		case USER_OVERDRIVE_DELAY:
			switch(bFormula)
			{
			case 16:
				fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass);
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_OVERDRIVE_DELAY;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= (DWORD)GetAlphaStat(wStatusID);
			break;
		case USER_BLIND:
			switch(bFormula)
			{
			case 16:
				fResult = (float)g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass);
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_BLIND;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= (DWORD)GetAlphaStat(wStatusID);
			//UpdateSight();
			break;
		case USER_DETECTION:
			{// �����ϰ� �ִٸ� ���Žð��� �����ð��� ���Ž��� ������ Ǯ���� ���ش�.
			}break;
		}

		if (pEffect->bID == __SKILL_SILENTBRANDYSHIP__ || pEffect->bType == TYPE_DRIVE || pEffect->bType == TYPE_TIMEZERO || pEffect->bType == TYPE_PASSIVE)
		{
			if (pResetValue)
				pResetValue[nCount++] = fResult;// active������ �ٽ� ������ �ʿ� ���� ������. ���� �ʿ䰡 ����.
		}
	}
	
	if( UserStatus.bStatusMany != 0 && bUserStatusSend)
	{
		NetSendToUser( m_dwConnectionIndex, (char*)&UserStatus, UserStatus.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
	}	
}

void CUser::AttachMagicFieldArray(CItem* pItem)
{	
	if(!pItem)
		return;

	CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pItem->GetID());
	if (!pBaseItem)	return;

	memset(&m_UseMagicFieldArrayDesc, 0, sizeof(m_UseMagicFieldArrayDesc));
	m_UseMagicFieldArrayDesc.pBaseItem = pBaseItem;
	
	SetStatusFromMagicFieldArrayStatusValue(pItem);	
}

void CUser::DetachMagicFieldArray(ITEM_MAGICFIELDARRAY_DESC* pUseMagicFieldArrayDesc)
{	
	ReSetStatusFromMagicFieldArrayStatusValue(pUseMagicFieldArrayDesc);
	memset(pUseMagicFieldArrayDesc, 0, sizeof(ITEM_MAGICFIELDARRAY_DESC));
}

void CUser::SetStatusFromMagicFieldArrayStatusValue(CItem* pItem)
{
	CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pItem->GetID());
	if (!pBaseItem)	return;

	DSTC_USER_STATUS	UserStatus;
	UserStatus.bStatusMany	= 0;

	for(int i = 0; i < 5; ++i)
	{
		if (pBaseItem->BaseItem_MagicFieldArray.sValue[i].bWho 
			&& GetAttackMode() == pBaseItem->BaseItem_MagicFieldArray.sValue[i].bWho)
		{
			WORD wStatusID	= pBaseItem->BaseItem_MagicFieldArray.sValue[i].wID;
			BYTE bFormula	= pBaseItem->BaseItem_MagicFieldArray.sValue[i].bFormula;
			float fResult	= 0;

			m_UseMagicFieldArrayDesc.wStatusID[i] = wStatusID;
			
			switch(wStatusID)
			{
			case USER_FIRE_RESIST:
				switch(bFormula)
				{
				case 1:
					fResult = pBaseItem->BaseItem_MagicFieldArray.sValue[i].nValue;
					break;
				}
				m_UseMagicFieldArrayDesc.fParamPlus[i] = fResult;
				m_fPlusParamByMagicFieldArray[wStatusID] += fResult;
				UpdateFireResist();
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= wStatusID;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wFireResist;
				break;
			case USER_ICE_RESIST:
				switch(bFormula)
				{
				case 1:
					fResult = pBaseItem->BaseItem_MagicFieldArray.sValue[i].nValue;
					break;
				}
				m_UseMagicFieldArrayDesc.fParamPlus[i] = fResult;
				m_fPlusParamByMagicFieldArray[wStatusID] += fResult;
				UpdateIceResist();
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= wStatusID;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wIceResist;
				break;
			case USER_LIGHT_RESIST:
				switch(bFormula)
				{
				case 1:
					fResult = pBaseItem->BaseItem_MagicFieldArray.sValue[i].nValue;
					break;
				}
				m_UseMagicFieldArrayDesc.fParamPlus[i] = fResult;
				m_fPlusParamByMagicFieldArray[wStatusID] += fResult;
				UpdateLightResist();
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= wStatusID;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wLightResist;
				break;

			case USER_POI_RESIST:
				switch(bFormula)
				{
				case 1:
					fResult = pBaseItem->BaseItem_MagicFieldArray.sValue[i].nValue;
					break;
				}
				m_UseMagicFieldArrayDesc.fParamPlus[i] = fResult;
				m_fPlusParamByMagicFieldArray[wStatusID] += fResult;
				UpdatePoiResist();
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= wStatusID;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wPoiResist;
				break;

			case USER_PHY_RESIST:
				switch(bFormula)
				{
				case 1:
					fResult = pBaseItem->BaseItem_MagicFieldArray.sValue[i].nValue;
					break;
				}
				m_UseMagicFieldArrayDesc.fParamPlus[i] = fResult;
				m_fPlusParamByMagicFieldArray[wStatusID] += fResult;
				UpdatePhyResist();
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= wStatusID;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wPhyResist;
				break;
			
			case USER_HEALHPSPEED:
				switch(bFormula)
				{
				case 2:
					fResult = m_dwHealHPSec*pBaseItem->BaseItem_MagicFieldArray.sValue[i].nValue/100.f;
					break;
				}
				m_UseMagicFieldArrayDesc.fParamPlus[i] = fResult;
				m_fPlusParamByMagicFieldArray[wStatusID] += fResult;
				UpdateHealHPSec();
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= wStatusID;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_dwHealHPSec;
				break;
			case USER_HEALMPSPEED:
				switch(bFormula)
				{
				case 2:
					fResult = m_dwHealHPSec*pBaseItem->BaseItem_MagicFieldArray.sValue[i].nValue/100.f;
					break;
				}
				m_UseMagicFieldArrayDesc.fParamPlus[i] = fResult;
				m_fPlusParamByMagicFieldArray[wStatusID] += fResult;
				UpdateHealSPSec();
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= wStatusID;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_dwHealMPSec;
				break;
			case USER_ATTACKRATE_PERCENT:
				switch(bFormula)
				{
				case 2:
					fResult = m_wAttackSpeed*pBaseItem->BaseItem_MagicFieldArray.sValue[i].nValue/100.f;
					break;
				}
				m_UseMagicFieldArrayDesc.fParamPlus[i] = fResult;
				m_fPlusParamByMagicFieldArray[wStatusID] += fResult;

				UpdateAttackSpeed();
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= wStatusID;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wAttackSpeed;
				break;
			case USER_MOVESPEED:
				switch(bFormula)
				{
				case 2:
					fResult = GetOriginalMoveSpeed()*pBaseItem->BaseItem_MagicFieldArray.sValue[i].nValue/100.f;
					break;
				}
				m_UseMagicFieldArrayDesc.fParamPlus[i] = fResult;
				m_fPlusParamByMagicFieldArray[wStatusID] += fResult;

				UpdateMoveSpeed();
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MOVESPEED;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= (DWORD)m_fMoveSpeed;
				break;
			case USER_CASTINGTIME:
				fResult = pBaseItem->BaseItem_MagicFieldArray.sValue[i].nValue;
				
				m_UseMagicFieldArrayDesc.fParamPlus[i] = fResult;
				m_fPlusParamByMagicFieldArray[wStatusID] += fResult;
				break;
			case USER_DEC_SKILLLEVEL:
				switch(bFormula)
				{
				case 1:
					fResult = pBaseItem->BaseItem_MagicFieldArray.sValue[i].nValue;
					break;
				}
				m_UseMagicFieldArrayDesc.fParamPlus[i] = fResult;
				m_fPlusParamByMagicFieldArray[wStatusID] += fResult;
				
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= wStatusID;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= (DWORD)fResult;
				break;
			case USER_ALL_RESIST:
				switch(bFormula)
				{
				case 1:
					fResult = pBaseItem->BaseItem_MagicFieldArray.sValue[i].nValue;
					break;
				}
				m_UseMagicFieldArrayDesc.fParamPlus[i] = fResult;
				m_fPlusParamByMagicFieldArray[USER_FIRE_RESIST] += fResult;
				UpdateFireResist();
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_FIRE_RESIST;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wFireResist;

				m_fPlusParamByMagicFieldArray[USER_ICE_RESIST] += fResult;
				UpdateIceResist();
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_ICE_RESIST;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wIceResist;

				m_fPlusParamByMagicFieldArray[USER_LIGHT_RESIST] += fResult;
				UpdateLightResist();
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_LIGHT_RESIST;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wIceResist;

				m_fPlusParamByMagicFieldArray[USER_POI_RESIST] += fResult;
				UpdatePoiResist();
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_POI_RESIST;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wIceResist;

				break;
			}
		}
	}

	if( UserStatus.bStatusMany )
	{
		NetSendToUser( m_dwConnectionIndex, (char*)&UserStatus, UserStatus.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
	}
}

void CUser::ReSetStatusFromMagicFieldArrayStatusValue(ITEM_MAGICFIELDARRAY_DESC* pUseMagicFieldArrayDesc)
{
	DSTC_USER_STATUS	UserStatus;
	UserStatus.bStatusMany	= 0;

	for(int i = 0; i < 5; ++i)
	{
		WORD wStatusID = pUseMagicFieldArrayDesc->wStatusID[i];	
		
		switch(wStatusID)
		{
		case USER_FIRE_RESIST:
			m_fPlusParamByMagicFieldArray[wStatusID] -= pUseMagicFieldArrayDesc->fParamPlus[i];	
			UpdateFireResist();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= wStatusID;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wFireResist;
			break;
		case USER_ICE_RESIST:
			m_fPlusParamByMagicFieldArray[wStatusID] -= pUseMagicFieldArrayDesc->fParamPlus[i];
			UpdateIceResist();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= wStatusID;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wIceResist;
			break;
		case USER_LIGHT_RESIST:
			m_fPlusParamByMagicFieldArray[wStatusID] -= pUseMagicFieldArrayDesc->fParamPlus[i];
			UpdateLightResist();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= wStatusID;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wLightResist;
			break;

		case USER_POI_RESIST:
			m_fPlusParamByMagicFieldArray[wStatusID] -= pUseMagicFieldArrayDesc->fParamPlus[i];
			UpdatePoiResist();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= wStatusID;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wPoiResist;
			break;

		case USER_PHY_RESIST:
			m_fPlusParamByMagicFieldArray[wStatusID] -= pUseMagicFieldArrayDesc->fParamPlus[i];
			UpdatePhyResist();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= wStatusID;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wPhyResist;
			break;
		case USER_HEALHPSPEED:
			m_fPlusParamByMagicFieldArray[wStatusID] -= pUseMagicFieldArrayDesc->fParamPlus[i];
			UpdateHealHPSec();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= wStatusID;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_dwHealHPSec;
			break;
		case USER_HEALMPSPEED:
			m_fPlusParamByMagicFieldArray[wStatusID] -= pUseMagicFieldArrayDesc->fParamPlus[i];
			UpdateHealSPSec();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= wStatusID;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_dwHealMPSec;
			break;
		case USER_ATTACKRATE_PERCENT:
			m_fPlusParamByMagicFieldArray[wStatusID] -= pUseMagicFieldArrayDesc->fParamPlus[i];
			
			UpdateAttackSpeed();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= wStatusID;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wAttackSpeed;
			break;
		case USER_MOVESPEED:
			m_fPlusParamByMagicFieldArray[wStatusID] -= pUseMagicFieldArrayDesc->fParamPlus[i];	
			UpdateMoveSpeed();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MOVESPEED;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= (DWORD)m_fMoveSpeed;
			break;
		case USER_CASTINGTIME:
			m_fPlusParamByMagicFieldArray[wStatusID] -= pUseMagicFieldArrayDesc->fParamPlus[i];							
			break;
		case USER_DEC_SKILLLEVEL:
			m_fPlusParamByMagicFieldArray[wStatusID] -= pUseMagicFieldArrayDesc->fParamPlus[i];
			
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= wStatusID;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= DWORD(-pUseMagicFieldArrayDesc->fParamPlus[i]);
			break;
		case USER_ALL_RESIST:
			m_fPlusParamByMagicFieldArray[USER_FIRE_RESIST] -= pUseMagicFieldArrayDesc->fParamPlus[i];
			UpdateFireResist();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_FIRE_RESIST;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wFireResist;

			m_fPlusParamByMagicFieldArray[USER_ICE_RESIST] -= pUseMagicFieldArrayDesc->fParamPlus[i];
			UpdateIceResist();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_ICE_RESIST;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wIceResist;

			m_fPlusParamByMagicFieldArray[USER_LIGHT_RESIST] -= pUseMagicFieldArrayDesc->fParamPlus[i];
			UpdateLightResist();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_LIGHT_RESIST;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wIceResist;

			m_fPlusParamByMagicFieldArray[USER_POI_RESIST] -= pUseMagicFieldArrayDesc->fParamPlus[i];
			UpdatePoiResist();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_POI_RESIST;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wIceResist;
		}	
	}

	if( UserStatus.bStatusMany )
	{
		NetSendToUser( m_dwConnectionIndex, (char*)&UserStatus, UserStatus.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
	}
}


void CUser::AttachMagicArray(DWORD dwItemID)
{
	CBaseItem*				pItem				= g_pBaseItemHash->GetData(dwItemID);
	ITEM_MAGICARRAY_DESC*	pItemMagicArrayDesc = m_pUseMagicArrayDesc[pItem->BaseItem_MagicArray.bSubID];
	
	if(!pItem)
		return;

	if(pItem->BaseItem_MagicArray.dwDur1)
	{
		// �ð� �ִ°͸� �ð�����Ʈ�� ���־����.
		if(!pItemMagicArrayDesc)
		{
			pItemMagicArrayDesc			= new ITEM_MAGICARRAY_DESC;
			m_pUsingMagicArrayList->AddHead(pItemMagicArrayDesc);

			pItemMagicArrayDesc->pItem	= pItem;
			pItemMagicArrayDesc->pos	= m_pUsingMagicArrayList->GetHeadPosition();
			m_pUseMagicArrayDesc[pItem->BaseItem_MagicArray.bSubID]	= pItemMagicArrayDesc;
		}

		pItemMagicArrayDesc->pItem = pItem;
		pItemMagicArrayDesc->dwTemp[ITEM_MAGICARRAY_DESC_TEMP_STARTTICK] = g_dwTickCount;
	}
	
	switch(pItem->wID)
	{
	case 16041: 
	case 16042:
	case 16043:
	case 16044:
	case 16045:
		{
			// ��Ƽ���� ���Ž��ó��.
			DSTC_SKILL packet;
			DWORD dwAlphaDamage = 0;

			AttachSkill(OBJECT_TYPE_PLAYER, GetID(), __SKILL_HIDING__, CLASS_TYPE_HUNTER
				, pItem->BaseItem_MagicArray.wPoint1, packet.dwDestTime, dwAlphaDamage);

			packet.sTargetInfo[packet.bTargetCount].bTargetType		= OBJECT_TYPE_PLAYER;
			packet.sTargetInfo[packet.bTargetCount].dwTargetIndex	= GetID();
			packet.sTargetInfo[packet.bTargetCount].dwCurHP			= GetHP();
			packet.sTargetInfo[packet.bTargetCount].dwDamage		= 0;
			packet.sTargetInfo[packet.bTargetCount++].bType			= 4;
			packet.bSkillKind	= __SKILL_HIDING__;
			packet.bSkillLevel	= (BYTE)pItem->BaseItem_MagicArray.wPoint1;
			packet.dwOwnIndex	= GetID();
			packet.bOwnType		= OBJECT_TYPE_PLAYER;

			GetCurDungeonLayer()->BroadCastSectionMsg((char*)&packet, packet.GetPacketSize(), GetPrevSectionNum() );
		}
		break;
	}

	// ��Ƽ ������ �߰� 
	DSTC_PARTY_ARRAY pPacket;
	pPacket.bType	= 1;
	pPacket.wId		= pItem->wID;
	NetSendToUser(m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
}


void CUser::DetachMagicArray(ITEM_MAGICARRAY_DESC* pItemMagicArrayDesc)
{
	switch(pItemMagicArrayDesc->pItem->wID)
	{
	case 16041: 
	case 16042:
	case 16043:
	case 16044:
	case 16045:
		{
			// ��Ƽ���� ������ ���ֶ�.
			if (GetEffectDesc(__SKILL_HIDING__))
				DetachSkill(GetEffectDesc(__SKILL_HIDING__));
			
		}
		break;
	}

	m_pUseMagicArrayDesc[pItemMagicArrayDesc->pItem->BaseItem_MagicArray.bSubID] = NULL;
	m_pUsingMagicArrayList->RemoveAt(pItemMagicArrayDesc->pos);

	delete pItemMagicArrayDesc;

	// ��Ƽ ������ ���� //
	DSTC_PARTY_ARRAY pPacket;
	pPacket.bType	= 2;
	pPacket.wId		= 0;
	NetSendToUser(m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
}


void CUser::SendToUserConsumableItemEffectListAll(CUser* pUser)
{
	if (m_pUsingItemList)
	{
		// �����Ӻ� ������ �������Ʈ�� ǥ��������.
		DSTC_ORIGINATIONITEMWITHEFFECTKIND packet;
		packet.dwUserIndex	= GetID();
		packet.bCreate		= TRUE;

		POSITION_ posSkill = m_pUsingItemList->GetHeadPosition();
		while(posSkill)
		{
			CItem* pItem = (CItem* )m_pUsingItemList->GetNext(posSkill);
			pItem;
			packet.bOriginationItemWithEffectKind[packet.bCount++] = 0;
		}

		if (packet.bCount)
		{
			NetSendToUser( pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		}
	}
}

void CUser::UnSummonGuardian(BYTE bySlotIndex)
{
	if (IsSummonGuardian(bySlotIndex))
	{
		DetachItem(m_GuardianEggGrowDesc[bySlotIndex].pos);

		CTDS_ITEM_PICKUP ItemPickup;
		ItemPickup.bInv			= 45;
		ItemPickup.bSectionNum	= 1;
		ItemPickup.i64ItemID	= 0;
		ItemPickup.bZipCode		= 0;				
		
		::CmdPickupItem(m_dwConnectionIndex, (char*)&ItemPickup, ItemPickup.GetPacketSize());

		if (GetGuardian(bySlotIndex))
		{
			GetGuardian(bySlotIndex)->SetStatus(UNIT_STATUS_DEAD);
			UpdateMonsterForAI( GetGuardian(bySlotIndex) );
		}
	}
}

void CUser::GoToWorld(WORD wMoveSpotNum, GOTOWORLD_STATUS eOutStaus)
{
	if (NULL != m_sMatchDescInfo.m_dwMatchUserIndex)
	{ 
		// ���� ��û�� ������ ����� Ƥ��.
		CancelMatch(MATCH_STATUS_USER_REQUEST_AFTER_USER_GOTO_WORLD);
	}

	m_bDestWorldSpot = (BYTE)wMoveSpotNum;
	
	if (!m_posRemoveList)
	{
		if( GetCurDungeon()->GetDungeonDataEx()->IsStadium() 
		&&	GetCurDungeonLayer()->IsTeamMatchPlayer(this) 
		&& !GetCurDungeonLayer()->IsTeamMatchEnd() )// ����� Stadium�� ����� ����: hwoarang 050202
		{
			return;
		}

		m_posRemoveList = GetCurDungeon()->GetRemovePCList()->AddHead(this);
		m_dwTemp[USER_TEMP_USERDYINGSTARTTICK] = g_dwTickCount;		
		m_dwTemp[USER_TEMP_GOTOWORLD_OUTSTAUS] = eOutStaus;
	}
}


BOOL CUser::AttachSkill(BYTE bOwnType, DWORD dwOwnIndex, BYTE bSkillKind, BYTE bSkillLevel, WORD wClass, DWORD& dwDestTime
						, DWORD& dwAlphaDamage)
{
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);

	EffectDesc* pEffectDesc = GetEffectDesc(bSkillKind);
	if (!pEffectDesc)
	{
		// ���� ������.
		pEffectDesc = AllocEffectDesc(bSkillKind, bSkillLevel);
		if (bOwnType == OBJECT_TYPE_PLAYER)
			pEffectDesc->pOwnUser = g_pUserHash->GetData(dwOwnIndex);

		SetStatusFromSkillStatusValue(bSkillKind
			, bSkillLevel
			, wClass
			, pEffectDesc->fParamPlus
			, TRUE);
		
		// ��ų������ ó��.
		switch( bSkillKind )
		{
		case __SKILL_TREEAGION__:
			{
				if (GetUnitStatus() == UNIT_STATUS_DEAD)
				{
					// ������ �������.
					SetStatus(UNIT_STATUS_NORMAL, TRUE);
					SetHP((WORD)(GetMaxHP()*pEffect->Value[bSkillLevel].nMax/100.));
					GetCurDungeonLayer()->GetMap()->SetTileOccupied( 
						GetCurMapTile()->wIndex_X
						, GetCurMapTile()->wIndex_Z
						, TILE_OCCUPIED_PC
						, this 
					);

					if (m_posRemoveList)
					{
						GetCurDungeon()->GetRemovePCList()->RemoveAt(m_posRemoveList);
						m_posRemoveList = NULL;
						m_dwTemp[USER_TEMP_FLAG_UPDATEUSERFORAI] = 0;						
					}
					
					DSTC_USER_STATUS	UserStatus;
					UserStatus.bStatusMany	= 1;
					UserStatus.pStatus[0].dwCode	= USER_HP;
					UserStatus.pStatus[0].dwMin	= GetHP();
					NetSendToUser( m_dwConnectionIndex, (char*)&UserStatus
						, UserStatus.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION);					
				}
			}break;
		
		case __SKILL_HIDING__:			// ����.
			UpdateUserForAI( this );
			break;
		case __SKILL_DETECTION__:	
			if (GetEffectDesc(__SKILL_HIDING__))
				DetachSkill(GetEffectDesc(__SKILL_HIDING__));
			break;
		case __SKILL_PRAY__:
			{
				// ������ �ɸ� ���۽�ų�� ���ֶ�.
				POSITION_ pos = GetUsingStatusEffectList()->GetHeadPosition();
				while(pos)
				{
					EffectDesc* pTempEffectDesc 
						= (EffectDesc*)GetUsingStatusEffectList()->GetNext(pos);
					if (pTempEffectDesc->pEffect->bCrime == CRIME_APPLY_ENEMY)
					{
						//������ Ȯ�� * { �����̷��� * 2 / (������ ���� + �ɷ��ִ� ��ų����) }
						if (g_pEffectLayer->IsSuccessByFormula0(
								pTempEffectDesc->pEffect->bID
								, pTempEffectDesc->bSkillLevel))
						{
							DetachSkill( pTempEffectDesc );
							break;
						}
					}
				}				
			}break;
		case __SKILL_AMFLEAFIRE__:
			{
				POSITION_ pos = GetUsingStatusEffectList()->GetHeadPosition();
				while(pos)
				{
					EffectDesc* pTempEffectDesc = (EffectDesc*)GetUsingStatusEffectList()->GetNext(pos);
					if (pTempEffectDesc->pEffect->bType == TYPE_DRIVE)
					{
						DWORD dwTime = pTempEffectDesc->dwDestTick - g_dwTickCount;
						pTempEffectDesc->dwDestTick += (DWORD)(dwTime * GetAlphaStat(USER_OVERDRIVE_DELAY) / 100.);
						pTempEffectDesc->dwAmfleafireTime = dwTime;
					}
				}
				DSTC_SKILL_RESULT	skillResult;
				skillResult.bType = 2;
				skillResult.wKind = __SKILL_AMFLEAFIRE__;
				skillResult.dwSkillResult[0]	= (DWORD)GetAlphaStat(USER_OVERDRIVE_DELAY);
				NetSendToUser( m_dwConnectionIndex, (char*)&skillResult, skillResult.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
			}break;
		case __SKILL_TURNOVER__:
			{
				int nAlphaDamageCount = 0;
				POSITION_ pos = GetUsingStatusEffectList()->GetHeadPosition();
				while(pos)
				{
					EffectDesc* pTempEffectDesc = (EffectDesc*)GetUsingStatusEffectList()->GetNext(pos);
					if (pTempEffectDesc->pEffect->bType == TYPE_DRIVE)
					{
						
						if (g_pEffectLayer->IsSuccessByFormula0(pEffectDesc->pEffect->bID, pEffectDesc->bSkillLevel))
						{
							// ���� ���� ���� �˾Ƴ��� ����.
							dwAlphaDamage += (pTempEffectDesc->dwDestTick-g_dwTickCount)/1000;
							
							DetachSkill( pTempEffectDesc );
							nAlphaDamageCount++;
							if (pEffectDesc->pEffect->Value[pEffectDesc->bSkillLevel].nDuration/1000	== nAlphaDamageCount)
								break;
						}
					}
				}
				dwAlphaDamage = dwAlphaDamage * pEffectDesc->pEffect->Value[pEffectDesc->bSkillLevel].nCompass;				
			}break;
		}
	}
	
	if (pEffect->bType	== TYPE_ACTIVE)
	{
		dwDestTime = pEffect->Value[pEffectDesc->bSkillLevel].nDuration;
		DetachSkill(pEffectDesc);
		return TRUE;
	}
	else if (pEffect->bType == TYPE_DRIVE)
	{
		pEffectDesc->dwDestTick = g_dwTickCount+dwDestTime;
	}
	else if (pEffect->bType == TYPE_TIMEZERO)
	{
		dwDestTime = pEffect->Value[pEffectDesc->bSkillLevel].nDuration;
		pEffectDesc->dwDestTick = g_dwTickCount+dwDestTime;
	}		

	return TRUE;
}


void CUser::DetachSkill(EffectDesc* pEffectDesc)
{	
	CDungeonLayer* pLayer = GetCurDungeonLayer();
	if( pLayer )
	{
		DSTC_DUNGEON_STOPSTATUSEFFECT packet;
		packet.bTargetType	= 0;
		packet.bSkillKind	= pEffectDesc->pEffect->bID;
		packet.dwTargetIndex= GetID();
		packet.dwCurHP = GetHP();
		pLayer->BroadCastSectionMsg( (char*)&packet, packet.GetPacketSize(), GetPrevSectionNum() );		
	}
	FreeEffectDesc(pEffectDesc);	
	SendAllUserSkillLevel(); //���� ���ε� ���� ���� 2005.02.17  �迵�� 
}

CMonster* CUser::GetGuardian(BYTE bySlotIndex)
{
	if (!m_pGuardian[bySlotIndex] || m_pGuardian[bySlotIndex]->GetUnitStatus() == UNIT_STATUS_NONE)
		return NULL;

	return m_pGuardian[bySlotIndex];
}

void CUser::SetGuardian(BYTE bySlotIndex, CMonster* pGuardian)
{
	m_pGuardian[bySlotIndex] = pGuardian;
}

void CUser::UpdateAD()
{
	WORD wMinMax[2] = { 0, 0};

	GetAttackDamage(GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON)
		, GetSkillLevel(GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON))
		, BYTE(GetCurrentWeaponID()/ITEM_DISTRIBUTE)
		, &wMinMax[0], &wMinMax[1], SELECT_ATTACK_TYPE_LBUTTON);

	SetAttackDamage_L(wMinMax[0], wMinMax[1]);

	GetAttackDamage(GetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON)
		, GetSkillLevel(GetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON))
		, BYTE(GetCurrentWeaponID()/ITEM_DISTRIBUTE)
		, &wMinMax[0], &wMinMax[1], SELECT_ATTACK_TYPE_RBUTTON);

	SetAttackDamage_R(wMinMax[0], wMinMax[1]);

}	

void CUser::UpdateAllStatus()
{
	InitUserItemAll();
		
	UpdateMaxHP();
	UpdateMaxSP();

	if(GetMaxHP() < GetHP())
		SetHP((WORD)GetMaxHP());
	if (GetMaxSP() < GetSP())
		SetSP((WORD)GetMaxSP());

	UpdateAD();
	UpdateAttackAcuracy();
	UpdateAvoid();
	UpdateBlockRate();
	UpdateFireResist();
	UpdateIceResist();
	UpdateLightResist();
	UpdatePoiResist();
	UpdatePhyResist();
	UpdateMoveSpeed();
	UpdateHealHPSec();
	UpdateHealSPSec();
	UpdateMaxCoolPoint();
	UpdateAttackSpeed();
	m_fCurCoolPoint = m_fMaxCoolPoint;
}


BOOL CUser::IsSummonGuardian(BYTE bySlotIndex)
{
	assert(bySlotIndex < MAX_USER_GUARDIAN);

	return m_GuardianEggGrowDesc[bySlotIndex].dwLastTick;
}

DWORD CUser::GetConnectionIndex() const
{
	return m_dwConnectionIndex;
}

void CUser::GuildMemberKill(const CUser* pTargetUser)
{
	BYTE byPoint	= 0;
	BYTE byLev		= 0;
		
	if(!pTargetUser)
		return;

	if(GetLevel() <= pTargetUser->GetLevel())
	{
		byLev = (BYTE)(pTargetUser->GetLevel() - GetLevel());
		byPoint = (byLev >= 10) ? 10 : 3;
	}
	else
	{
		byLev = (BYTE)(GetLevel() - pTargetUser->GetLevel());
		byPoint = (byLev >= 10) ? 1 : 3;
	}

	SendToWorldServerForGuildPoint(GetID(), pTargetUser->GetID(), byPoint, m_dwGuildId, pTargetUser->m_dwGuildId);		
}


// ���¸����� ���� ����Ǿ��� �ɷ�ġ�� �ٽ� ����.
void CUser::ReSetStatusFromSkillStatusValue(EffectDesc* pEffectDesc)
{
	Effect* pEffect = pEffectDesc->pEffect;
	BYTE bSkillLevel = pEffectDesc->bSkillLevel;
	if ((char*)bSkillLevel<0)	return;
		
	BYTE bCurrentHand	= 0;
	BYTE bKind			= BYTE(GetCurrentWeaponID()/ITEM_DISTRIBUTE);

	int nCount = 0;
	DSTC_USER_STATUS	UserStatus;
	UserStatus.bStatusMany	= 0;
	
	for (int i = 0; i < 5; ++i )
	{
		float fResult = 0;
		USER_PROPERTY_CODE wStatusID = (USER_PROPERTY_CODE)pEffect->sStatusValue[i].wStatusID;

		if (!wStatusID)	break;
		
		switch(wStatusID)
		{
		case USER_AVOID_PERCENT:
		case USER_AVOID:
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);			
			UpdateAvoid();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_AVOID;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wAvoid;
			break;

		case USER_MAXHP_PERCENT:
		case USER_MAXHP:
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
			UpdateMaxHP();
			SetHP((WORD)min(GetHP(),GetMaxHP()));
			UpdateHealHPSec();

			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HP;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= GetHP();

			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MAXHP;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= GetMaxHP();

			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HEALHPSPEED;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_dwHealHPSec;
			break;
			
		case USER_MOVESPEED_PERCENT:
		case USER_MOVESPEED:
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
			UpdateMoveSpeed();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MOVESPEED;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= (DWORD)m_fMoveSpeed;
			break;

		case USER_HP_PERCENT:
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
			SetHP((WORD)(GetHP()/((pEffectDesc->fParamPlus[i]+100.)/100.)));
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HP;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= GetHP();
			break;

		case USER_MP:
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
			if (GetSP() < pEffectDesc->fParamPlus[i])
				SetSP(0);
			else
				SetSP((WORD)(GetSP()-pEffectDesc->fParamPlus[i]));

			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MP;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= GetSP();
			break;

		case USER_MAXMP_PERCENT:
		case USER_MAXMP:
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
			UpdateMaxSP();
			UpdateHealSPSec();
			
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MAXMP;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= GetMaxSP();

			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HEALMPSPEED;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_dwHealMPSec;
			break;

		case USER_BLIND:
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_BLIND;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= (DWORD)GetAlphaStat(wStatusID);
			//UpdateSight();
			break;
		
		case USER_CASTINGTIME:
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
			break;

		case USER_COOLTIME:
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
			break;
	
		case USER_ATTACK_R_PERCENT:
		case USER_ATTACK_R:
			{
				SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);

				bCurrentHand = (m_bCurrentHand==1) ? EQUIP_TYPE_RHAND1 : EQUIP_TYPE_RHAND2;
				WORD wMinMax[2] = {0, 0};
				
				GetAttackDamage(GetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON)
					, GetSkillLevel(GetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON)), bKind
					, &wMinMax[0], &wMinMax[1], SELECT_ATTACK_TYPE_RBUTTON);

				SetAttackDamage_R(wMinMax[0], wMinMax[1]);
				
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_ATTACK_R;
				UserStatus.pStatus[UserStatus.bStatusMany].dwMin	= GetAttackDamageMin_R();
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMax	= GetAttackDamageMax_R();
			}
			break;

		case USER_ATTACK_L_PERCENT:
		case USER_ATTACK_L:
			{
				SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
				WORD wMinMax[2] = {0, 0};
				GetAttackDamage(GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON)
					, GetSkillLevel(GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON)), bKind
					, &wMinMax[0], &wMinMax[1], SELECT_ATTACK_TYPE_LBUTTON);

				SetAttackDamage_L(wMinMax[0], wMinMax[1]);

				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_ATTACK_L;
				UserStatus.pStatus[UserStatus.bStatusMany].dwMin	= GetAttackDamageMin_L();
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMax	= GetAttackDamageMax_L();
			}
			break;
		
		case USER_SKILLATTACK_R_PERCENT:
			{
				SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
				WORD wMinMax[2] = {0, 0};
				GetAttackDamage(GetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON)
					, GetSkillLevel(GetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON)), bKind
					, &wMinMax[0], &wMinMax[1], SELECT_ATTACK_TYPE_RBUTTON);

				SetAttackDamage_R(wMinMax[0], wMinMax[1]);

				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_ATTACK_R;
				UserStatus.pStatus[UserStatus.bStatusMany].dwMin	= GetAttackDamageMin_R();
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMax	= GetAttackDamageMax_R();
			}			
			break;

		case USER_SKILLATTACK_L_PERCENT:
			{
				SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);

				WORD wMinMax[2] = {0, 0};
				GetAttackDamage(GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON)
					, GetSkillLevel(GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON)), bKind
					, &wMinMax[0], &wMinMax[1], SELECT_ATTACK_TYPE_LBUTTON);

				SetAttackDamage_L(wMinMax[0], wMinMax[1]);

				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_ATTACK_L;
				UserStatus.pStatus[UserStatus.bStatusMany].dwMin	= GetAttackDamageMin_L();
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMax	= GetAttackDamageMax_L();
			}			
			break;
					
		case USER_FIRE_RESIST:
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
			UpdateFireResist();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_FIRE_RESIST;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wFireResist;
			break;

		case USER_ICE_RESIST:
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
			UpdateIceResist();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_ICE_RESIST;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wIceResist;
			break;

		case USER_LIGHT_RESIST:
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
			UpdateLightResist();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_LIGHT_RESIST;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wLightResist;
			break;
			
		case USER_POI_RESIST_PERCENT:
		case USER_POI_RESIST:
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
			UpdatePoiResist();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_POI_RESIST;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wPoiResist;
			break;

		case USER_PHY_RESIST:
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
			UpdatePhyResist();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_PHY_RESIST;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wPhyResist;
			break;

		case USER_MAX_RESIST:
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MAX_RESIST;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= (DWORD)GetAlphaStat(wStatusID);
			break;
			
		case USER_BLOCKRATE_PERCENT:
		case USER_BLOCKRATE:
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
			UpdateBlockRate();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_BLOCKRATE;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wBlockRate;
			break;

		case USER_ATTACKACURACY_PERCENT:
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
			UpdateAttackAcuracy();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_ATTACKACURACY_PERCENT;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wAttackAcuracy;
			break;	

		case USER_ATTACKRATE_PERCENT:
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
			UpdateAttackSpeed();
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_ATTACKRATE_PERCENT;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wAttackSpeed;
			break;

		case USER_POISONING:
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_POISONING;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= (DWORD)GetAlphaStat(wStatusID);
			break;

		case USER_DEC_SKILLLEVEL:
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_DEC_SKILLLEVEL;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= (DWORD)-pEffectDesc->fParamPlus[i];
			break;
		
		case USER_OVERDRIVE_DELAY:
			{
				POSITION_  pos = GetUsingStatusEffectList()->GetHeadPosition();
				while(pos)
				{
					EffectDesc* pEffectDesc = (EffectDesc*)GetUsingStatusEffectList()->GetNext(pos);
					if (pEffectDesc->dwAmfleafireTime) 
					{
						// ������ �ð��� �ٽ� �������.
						DWORD dwTime = pEffectDesc->dwAmfleafireTime;
						pEffectDesc->dwDestTick -= (DWORD)(dwTime * GetAlphaStat(USER_OVERDRIVE_DELAY) / 100.);
						pEffectDesc->dwAmfleafireTime = 0;
					}
				}

				DSTC_SKILL_RESULT	skillResult;
				skillResult.bType = 2;
				skillResult.wKind = __SKILL_AMFLEAFIRE__;
				skillResult.dwSkillResult[0]	= (DWORD)-GetAlphaStat(USER_OVERDRIVE_DELAY);
				NetSendToUser( m_dwConnectionIndex, (char*)&skillResult, skillResult.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION );

				SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);				
			}
			break;		
		}

		if (pEffect->bType != TYPE_DRIVE || pEffect->bType != TYPE_TIMEZERO )
		{
			// active������ �ٽ� ������ �ʿ� ���� ������. ���� �ʿ䰡 ����.
			pEffectDesc->fParamPlus[nCount++] = fResult;
		}
	}
	
	if( UserStatus.bStatusMany != 0 )
	{
		NetSendToUser( m_dwConnectionIndex, (char*)&UserStatus, UserStatus.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
	}
}


// �����Ϸ� �Ѽ� �ִ� ���� �ε���  return -1 �̸� error
int CUser::GetSubordinateIndex() const
{
	int nInsertIndex = -1;

	for(int i = 0; i < 5; ++i)
	{
		if (m_pMonster[i] == NULL)
		{
			nInsertIndex = i;
			break;
		}
	}

	return nInsertIndex;
}


BYTE CUser::GetSkillLevelByBonusOption(const BYTE bSkillKind) const // : 050114 hwoarang ������ ������;;
{
	switch(bSkillKind)
	{
	case __SKILL_MEDITETION__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_MEDITETION);
		}break;
	case __SKILL_FIREMISSILE__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_FIREMISSILE);
		}break;
	case __SKILL_FIREBLAZE__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_FIREBLAZE);
		}break;
	case __SKILL_MAGMAWALL__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_MAGMAWALL);
		}break;
	case __SKILL_ICEMISSILE__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_ICEMISSILE);
		}break;
	case __SKILL_ICEWAVE__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_ICEWAVE);
		}break;
	case __SKILL_ICEWALL__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_ICEWALL);
		}break;
	case __SKILL_LIGHTNING__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_LIGHTNING);
		}break;
	case __SKILL_AIREALCOAT__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_AIREALCOAT);
		}break;
	case __SKILL_THUNDERSTORM__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_THUNDERSTORM);
		}break;
	case __SKILL_LANDINGSUPPORT__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_LANDINGSUPPORT);
		}break;
	case __SKILL_EARTHQUAKE__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_EARTHQUAKE);
		}break;
	case __SKILL_METEOR__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_METEOR);
		}break;
	case __SKILL_MANAEXPAND__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_MANAEXPAND);
		}break;
	case __SKILL_SPEEDCASTING__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_SPEEDCASTING);
		}break;
	case __SKILL_EXPLOSION__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_EXPLOSION);
		}break;
	case __SKILL_CONCENTRATION__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_CONCENTRATION);
		}break;
	case __SKILL_REGENERATION__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_REGENERATION);
		}break;
	case __SKILL_WEAPONMASTERY__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_WEAPONMASTERY);
		}break;
	case __SKILL_DEFENSEMASTERY__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_DEFENSEMASTERY);
		}break;
	case __SKILL_AURARECHARGE__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_AURARECHARGE);
		}break;
	case __SKILL_ZEAL__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_ZEAL);
		}break;
	case __SKILL_BASH__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_BASH);
		}break;
	case __SKILL_LIGHTNINGBREAK__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_LIGHTNINGBREAK);
		}break;
	case __SKILL_SILENTBRANDYSHIP__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_SILENTBRANDYSHIP);
		}break;
	case __SKILL_ANTAGONIZE__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_ANTAGONIZE);
		}break;
	case __SKILL_AURABATTLER__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_AURABATTLER);
		}break;
	case __SKILL_LIFEFORCE__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_LIFEFORCE);
		}break;
	case __SKILL_PRAY__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_PRAY);
		}break;
	case __SKILL_BLESSING__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_BLESSING);
		}break;
	case __SKILL_WINDTALES__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_WINDTALES);
		}break;
	case __SKILL_HOLDSHIELD__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_HOLDSHIELD);
		}break;
	case __SKILL_WINDFORCE__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_WINDFORCE);
		}break;
	case __SKILL_AMFLEAFIRE__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_AMFLEAFIRE);
		}break;
	case __SKILL_HOLYPRESSURE__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_HOLYPRESSURE);
		}break;
	case __SKILL_APSOLUTMAGICBARRIER__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_APSOLUTMAGICBARRIER);
		}break;
	case __SKILL_SPELLBINDING__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_SPELLBINDING);
		}break;
	case __SKILL_DOMENATION__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_DOMENATION);
		}break;
	case __SKILL_MIRACULOUSHEART__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_MIRACULOUSHEART);
		}break;
	case __SKILL_ARCANUMSKIN__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_ARCANUMSKIN);
		}break;
	case __SKILL_FRENZY__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_FRENZY);
		}break;
	case __SKILL_CALMDOWN__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_CALMDOWN);
		}break;
	case __SKILL_SLEEP__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_SLEEP);
		}break;
	case __SKILL_CONFUSE__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_CONFUSE);
		}break;
	case __SKILL_SOULBLASTER__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_SOULBLASTER);
		}break;
	case __SKILL_MINDCONTROL__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_MINDCONTROL);
		}break;
	case __SKILL_RAGE__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_RAGE);
		}break;
	case __SKILL_REDELEMENTAL__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_REDELEMENTAL);
		}break;
	case __SKILL_BLUEELEMENTAL__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_BLUEELEMENTAL);
		}break;
	case __SKILL_GREENELEMENTAL__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_GREENELEMENTAL);
		}break;
	case __SKILL_SPEEDOVER__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_SPEEDOVER);
		}break;
	case __SKILL_DODGE__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_DODGE);
		}break;
	case __SKILL_POISONING__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_POISONING);
		}break;
	case __SKILL_CHAINATTACKER__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_CHAINATTACKER);
		}break;
	case __SKILL_BLASTQUAKE__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_BLASTQUAKE);
		}break;
	case __SKILL_IRONSKIN__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_IRONSKIN);
		}break;
	case __SKILL_HIDING__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_HIDING);
		}break;
	case __SKILL_SPELLTRAP__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_SPELLTRAP);
		}break;
	case __SKILL_PRESSURE__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_PRESSURE);
		}break;
	case __SKILL_LIFEEXPLOSION__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_LIFEEXPLOSION);
		}break;
	case __SKILL_SYLPHID__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_SYLPHID);
		}break;
	case __SKILL_SHAUT__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_SHAUT);
		}break;
	case __SKILL_BERSERK__:
		{
			return GetItemAttr(ITEM_ATTR_SKILL_LEVEL_SKILL_BERSERK);
		}break;
	default:
		{
			return 0;
		}break;
	}
}

// ��ų ������ ���Ͷ�.
BYTE CUser::GetSkillLevel(const BYTE bSkillKind) const
{
	if (bSkillKind >= MAX_SKILL)
		return 0;
	
	BYTE bItemSkillLevel = 0;
	
	if (m_pwSkillLevel[bSkillKind])
	{
		BYTE byArray = static_cast<BYTE>(m_fPlusParamByMagicFieldArray[USER_DEC_SKILLLEVEL]);
		bItemSkillLevel = byArray + GetSkillLevelByItem(bSkillKind) + GetSkillLevelByBonusOption(bSkillKind);
	}

	for(int i = 0; i < MAX_EQUIP_POOL; ++i)
	{
		if (m_pEquip[i].GetID())
		{	
			if(m_bCurrentHand==1)
			{
				if(i==EQUIP_TYPE_RHAND2 || i==EQUIP_TYPE_LHAND2)
					continue;
			}
			else if(m_bCurrentHand==2)
			{
				if(i==EQUIP_TYPE_RHAND1 || i==EQUIP_TYPE_LHAND1)
					continue;
			}

			for(int c = 0; c < MAX_ITEM_ATTR; ++c)
			{
				if (m_pEquip[i].wAttr[c].uCode >= MAX_ITEM_KIND1_CODE &&
					m_pEquip[i].wAttr[c].uCode < MAX_ITEM_KIND2_CODE)
				{
					DWORD dwVirtualCode = 
						g_pItemAttrLayer->GetVirtualCode(m_pEquip[i].wAttr[c].uCode, m_pEquip[i].wAttr[c].uValue);

					// formula 6�� �� ��ų���� �÷��ִ°��̱� ������..
					if (g_pItemAttrLayer->m_ItemAttrDefine[dwVirtualCode].wFormula == 6 
						&& bSkillKind == g_pItemAttrLayer->m_ItemAttrDefine[dwVirtualCode].bSkillID)
					{
						bItemSkillLevel += GetItemAttr((ITEM_ATTR_CODE)dwVirtualCode);
					}
				}
			}

		}
	}
	
	if (bSkillKind == __SKILL_MANAMASTERY__ || bSkillKind == __SKILL_AURAMASTERY__ || bSkillKind == __SKILL_DIVINEMASTERY__ || bSkillKind == __SKILL_SUMMONMASTERY__ || bSkillKind == __SKILL_CHAKRAMASTERY__)
	{
		bItemSkillLevel = 0;
		return (BYTE)max(m_pwSkillLevel[bSkillKind]+bItemSkillLevel, 0);
	}
	else
	{
		return (BYTE)min(max(char(m_pwSkillLevel[bSkillKind]+bItemSkillLevel), 0), MAX_SKILL_LEVEL);
	}
}


// ���� �ٲ�� �ִ°�?
BOOL CUser::IsChangeLayer(WORD wIndex_X, WORD wIndex_Z, BYTE bSpotNum) const
{	
	CDungeonLayer*	pLayer = GetCurDungeonLayer();
	if( !pLayer ) return FALSE;
	
	BOOL bChangeLayer = FALSE;

	if (wIndex_X >= pLayer->m_pMoveSpot[bSpotNum].rcMoveSpot.left && 
		wIndex_X <= pLayer->m_pMoveSpot[bSpotNum].rcMoveSpot.right &&
		wIndex_Z <= pLayer->m_pMoveSpot[bSpotNum].rcMoveSpot.bottom &&
		wIndex_Z >= pLayer->m_pMoveSpot[bSpotNum].rcMoveSpot.top	)
	{
		// ���̵��Ҽ� �ִ� �����̱�.

#ifdef	DEV_MODE
	if (GetCurDungeon()->GetDungeonDataEx()->IsConquer())
		return TRUE;
#endif		
		DUNGEON_DATA_EX* pDungeon = GetCurDungeon()->GetDungeonDataEx();

		switch(pDungeon->GetDungeonType())
		{
		case DUNGEON_TYPE_EVENT:		
			{				
				if( Event_IsChangeLayer( (CUser*)this ) )
				{
					return TRUE;
				}
			}			
			break;

		case DUNGEON_TYPE_CONQUER:
			{
				CUser* pOwner = g_pUserHash->GetData(pDungeon->m_dwOwnerIndex);

				// ���� �ö󰡴°� �������
				if (!bSpotNum)
				{
					bChangeLayer = TRUE;
				}
				else
				{
					//����������						
					if (pDungeon->m_bSiege)
					{
						// �������̰�
						if (!pLayer->GetMap()->m_bCPRemainCount && !pLayer->m_wCurMonster)
						{
							// cp�� �� Ǯ���ٸ� �������, ���͵� �� ��Ҵٸ�.
							bChangeLayer = TRUE;
						}
						else
						{
							if ((pDungeon->IsDungeonOwner(this)) ||
								(pDungeon->m_dwOwnerGuildNum &&
								GetCurDungeon()->GetDungeonDataEx()->m_dwOwnerGuildNum == m_dwGuildId) ||
								(pOwner && (pOwner->m_dwPartyId && pOwner->m_dwPartyId == m_dwPartyId) ))
							{
								bChangeLayer = TRUE;
							}
						}
					}
					else
					{
						bChangeLayer = TRUE;
					}
				}
			}			
			break;

		default:
			// ���� ������ �ƴϸ� �������
			bChangeLayer = TRUE;
			break;
		}		
	}

	return bChangeLayer;				
}

BOOL CUser::IsAlliance(const CUnit* pUnit)
{
	if (!pUnit)
		return FALSE;

	switch(pUnit->GetObjectType())
	{
		case OBJECT_TYPE_PLAYER:
			return IsAlliance((CUser*)pUnit);
		case OBJECT_TYPE_MONSTER:
			return IsAlliance((CMonster*)pUnit);
	}

	return FALSE;
}


// �� ������ �����ΰ�?
BOOL CUser::IsAlliance(const CUser* pUser)
{
	CDungeon *pDungeon = GetCurDungeon();
	CDungeonLayer *pLayer = NULL;
	if(pDungeon)
	{
		pLayer = pDungeon->GetDungeonLayer(GetCurLayerIndex());
	}

	if( pLayer // : hwoarang 050218 
	&&	pLayer->m_pTeamMatch)
	{
		return !pLayer->m_pTeamMatch->OnAttack(this, pUser);
	}

	if (pUser->m_sMatchDescInfo.m_bMatching && this != pUser)
	{
		return FALSE;
	}
	else
	{
		return (this == pUser) || (m_dwPartyId && m_dwPartyId == pUser->m_dwPartyId) || (m_dwGuildId && m_dwGuildId == pUser->m_dwGuildId);
	}
}


// �� ���ʹ� �����ΰ�?
BOOL CUser::IsAlliance(const CMonster* pMonster)
{
	CDungeon *pDungeon = GetCurDungeon();
	CDungeonLayer *pLayer = NULL;
	if(pDungeon)
	{
		pLayer = pDungeon->GetDungeonLayer(GetCurLayerIndex());
	}

	if( pLayer 
	&&	pLayer->m_pTeamMatch)
	{
		return !pLayer->m_pTeamMatch->OnAttack(this, pMonster->GetLord() );
	}

	BOOL bAlliance = (pMonster->GetLord() == this) ||
		(GetAttackMode() == ATTACK_MODE_DEFENCE && pMonster->GetRace() == OBJECT_TYPE_GUARDIAN 	&& !pMonster->GetLord());

	if (!bAlliance)
	{
		CUnit* pUnit = (CUnit*)pMonster->GetLord();
		if (pUnit && pUnit->GetObjectType() == OBJECT_TYPE_PLAYER)
		{
			bAlliance = IsAlliance((CUser *)pUnit);
		}
	}
	
	return bAlliance;
}


void CUser::SetAttackMode(BYTE bAttackMode)
{
	// �����ڷ� �������� 1������ ƨ��� ���� ó��.
	if (bAttackMode == ATTACK_MODE_DEFENCE)
		m_dwTemp[USER_TEMP_FLAG_LOBBY] = 0;

	if (GetAttackMode() != bAttackMode)
	{
		// ������ ���ŵɶ��� ������.
		CUnit::SetAttackMode((ATTACK_MODE)bAttackMode);

		if (!GetCurDungeon())	return;
		CDungeonLayer*	pLayer;
		pLayer = GetCurDungeonLayer();

		if (!pLayer)	return;
		DSTC_ATTACK_MODE packet;
		packet.dwUserIndex = GetID();
		packet.bAttackMode = bAttackMode;
		pLayer->BroadCastSectionMsg( (char*)&packet, packet.GetPacketSize(), GetPrevSectionNum() );

		DetachMagicFieldArray(&m_UseMagicFieldArrayDesc);		
		AttachMagicFieldArray(&GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem);		
	}	
}


void CUser::SetBad(BOOL bBad, BYTE bCriminalKind, int nValue, BOOL bChk)
{
	if(bChk)
		return;
	
	m_sPKDescInfo.m_bBad = (BYTE)bBad;
	if (bBad)
	{
		m_dwTemp[USER_TEMP_BADSTARTTICK] = g_dwTickCount+20000;
		SetCriminalPropensity(bCriminalKind, nValue);		
	}
	else
	{
		m_dwTemp[USER_TEMP_BADSTARTTICK] = 0;
	}

	DSTC_BADACTION packet;
	packet.bBad	 = (BYTE)bBad;
	packet.dwUserIndex = GetID();
	CDungeonLayer* pLayer = GetCurDungeonLayer();
	if (pLayer)
		pLayer->BroadCastSectionMsg((char*)&packet
		, packet.GetPacketSize()
		, GetPrevSectionNum());
}

BOOL CUser::IsBadAction(CUnit* pDefenseUnit)
{
	switch(pDefenseUnit->GetObjectType())
	{
	case OBJECT_TYPE_PLAYER:
		return IsBadAction((CUser*)pDefenseUnit);
	case OBJECT_TYPE_MONSTER:
		return IsBadAction((CMonster*)pDefenseUnit);
	}
	return false;
}


// �� �������� �������� �ߴ��� �����ߴ��� üũ
BOOL CUser::IsBadAction(CUser* pDefenseUser)
{		
	// �������̰ų� 1:1 ���̰ų� �̺�Ʈ �����߿� ���������̸�	
	if (m_sMatchDescInfo.m_bMatching 
		|| IsCurDungeonSiege() 
		|| (EVENT_DUNGEON_SURVIVAL == GetCurDungeon()->GetEventDungeonType())
		|| GetCurDungeon()->GetTemp(DUNGEON_TEMP_PKMODE)	// GM�� ���ؼ� PK ���°� Ǯ�ȴ�
		|| 	IsGuildWar(this , pDefenseUser) )
//			GetCurDungeon(), m_dwGuildId, pDefenseUser->m_dwGuildId, m_dwPartyId, pDefenseUser->m_dwPartyId
//		, GODMODE_STATUS_RELEASE))
	{
		return FALSE;
	}
	else
	{
		// ���۳� �ƴϰ� �׳� ����ġ�� ���� ��������� ����Ѵٸ�
		if (g_pCharacter_Propensity[pDefenseUser->GetCriminalPropensityTableID()].bSelf_Defense 
			|| pDefenseUser->m_sPKDescInfo.m_bBad)
			return FALSE;		

		return TRUE;
	}
}


// �� ���Ϳ��� �������� �ߴ���?
BOOL CUser::IsBadAction(const CMonster* pDefenseMonster)
{
	// �������� �ƴϰ� 1:1 ���� �ƴ϶��
	if (!m_sMatchDescInfo.m_bMatching && !IsCurDungeonSiege())
	{
		if (pDefenseMonster->GetLord() && IsBadAction((CUser*)pDefenseMonster->GetLord()))
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CUser::SetSummonMonster(GAME_OBJECT_TYPE eObjectType, BYTE byIndex, CMonster* pMonster)
{
	if (OBJECT_TYPE_MONSTER == eObjectType)
	{
		m_pMonster[byIndex] = pMonster;
	}
	else if(OBJECT_TYPE_GUARDIAN == eObjectType)
	{
		SetGuardian(byIndex, pMonster);
	}
}

CMonster* CUser::GetSummonMonster(GAME_OBJECT_TYPE eObjectType, BYTE byIndex)
{
	assert(OBJECT_TYPE_MONSTER == eObjectType || OBJECT_TYPE_GUARDIAN == eObjectType);

	if (OBJECT_TYPE_MONSTER == eObjectType)
	{
		return m_pMonster[byIndex];
	}
	else if(OBJECT_TYPE_GUARDIAN == eObjectType)
	{
		return GetGuardian(byIndex);
	}

	return NULL;
}

BOOL CUser::IsServerSetChange() const
{
	return m_bInvItemCheck;
}

const char* CUser::GetLoginID() const
{
	return m_szId;
}

void CUser::SetLoginID(const char* szLoginID)
{
	__lstrcpyn(m_szId, (LPSTR)szLoginID, MAX_CHARACTER_NAME_LENGTH);
}

DWORD CUser::GetBankMoney() const
{
	return m_dwBankMoney;
}

void CUser::SetBankMoney(DWORD dwBankMoney)
{
	m_dwBankMoney = dwBankMoney;
}

BYTE CUser::GetCurrentHand() const
{
	return m_bCurrentHand;
}

PK_DESC_INFO* CUser::GetPKDescInfo() const
{
	return (PK_DESC_INFO*)&m_sPKDescInfo;
}

MATCH_DESC_INFO* CUser::GetMatchDescInfo() const
{
	return (MATCH_DESC_INFO*)&m_sMatchDescInfo;
}

short CUser::GetCriminalPropensity() const
{
	return m_wCriminalPropensity;
}

WORD CUser::GetClassRank() const
{
	return m_wClassRank;
}

void CUser::SetClassRank(WORD wClassRank)
{
	m_wClassRank = wClassRank;
}


// ����ġ ���� �Ҷ� ���̺� ���̵� ��.��
BYTE CUser::GetCriminalPropensityTableID() const
{
	BYTE byResult = 0;

	for(BYTE i = 1; i < MAX_CHARACTER_PROPENSITY; ++i)
	{
		if((short)m_wCriminalPropensity >= g_pCharacter_Propensity[i].wCriminalPointMin 
			&& m_wCriminalPropensity <= g_pCharacter_Propensity[i].wCriminalPointMax)
		{
			byResult = i;
			break;
		}
	}

	return byResult;
}


// �׾����� ���� ����ġ.
int CUser::GetExpDying()
{	
	if( GetLevel() == 255 ) return 0;
	if (GetCurDungeon() && IsCurDungeonSiege())	
		return 0;

	// �սǵǴ� ����ġ = MAX { ( ���緹����������ġ * Character_Propensity.exp_loss / 100 )
	//  *(100-ITEM_ATTR_DEAD_EXP_LOSS_DECREASE+ITEM_ATTR_DEAD_EXP_LOSS_INCREASE)/100 ), 0 }
	BYTE bCriminalLevel = GetCriminalPropensityTableID();
	
	int nCTExp = INT(((GetExpTableOfLevel(OBJECT_TYPE_PLAYER, GetLevel()+1 )-GetExpTableOfLevel(OBJECT_TYPE_PLAYER, GetLevel() ))
		*g_pCharacter_Propensity[bCriminalLevel].byExp_Loss/100.F)
		* ((100-GetItemAttr(ITEM_ATTR_DEAD_EXP_LOSS_DECREASE)+GetItemAttr(ITEM_ATTR_DEAD_EXP_LOSS_INCREASE))/100.F));
	
	if (nCTExp < 0)	nCTExp = 0;

	// ���� ����.
	if (m_dwExp<DWORD(nCTExp) || (m_dwExp-nCTExp < GetExpTableOfLevel(OBJECT_TYPE_PLAYER, GetLevel() )))
	{
		if (m_dwExp>=GetExpTableOfLevel(OBJECT_TYPE_PLAYER, GetLevel() ))
			nCTExp	 = m_dwExp-GetExpTableOfLevel(OBJECT_TYPE_PLAYER, GetLevel() );
		else
			nCTExp	 = 0;
	}
	
	return nCTExp;	
}


// ������ ���� ��������
void CUser::SetCriminalPropensity(int nKind,DWORD dwDeffenseLevel)
{	
	int nCriminal = 0;
	
	switch(nKind)
	{
		case CRIMINAL_USER_RED_LEVEL_PK:
		case CRIMINAL_MONSTER_KILL:
			
			if (GetLevel() < dwDeffenseLevel)
				nCriminal = dwDeffenseLevel- GetLevel();
			else
				nCriminal = 1;	// �ּҰ�
		
			break;	

		case CRIMINAL_USER_BLUE_LEVEL_PK:
			//Y = -[{ 200 + ( 700*INT(ĳ���� Level / 25) ) }+ ( RPC * 50)]
			nCriminal = -INT((200+(700*GetLevel()/25)+(m_sPKDescInfo.m_bPKRepeatCount*50)));
			
			break;
	}
	
	if (nKind == CRIMINAL_USER_BLUE_LEVEL_PK)
	{
		struct tm when;
		time_t now;
		time(&now);
		when = *localtime(&now);
			
		if (difftime(now, m_sPKDescInfo.m_dwPKTime) < PKREPEAT_INC_TIME)
		{
			// pk repeat count ����
			SetExchangePKRepeatCount(1, now);
		}
		
		SetExchangePKCount(1, now);		
	}

	if (nCriminal > 0)
	{
		if (m_wCriminalPropensity + nCriminal < 32767)
			m_wCriminalPropensity += nCriminal;
		else
			m_wCriminalPropensity = 32767;
	}
	else
	{
		if (m_wCriminalPropensity + nCriminal > -32767)
			m_wCriminalPropensity += nCriminal;
		else
			m_wCriminalPropensity = -32767;
	}

	if (GetCurDungeon())
	{	
		CDungeonLayer* pLayer = GetCurDungeonLayer();
		if (pLayer)
		{
			DSTC_USER_STATUS UserStatus;
			UserStatus.bStatusMany			= 1;
			UserStatus.pStatus[0].dwCode	= USER_CTTIME;
			UserStatus.pStatus[0].dwMin		= m_wCriminalPropensity;
			UserStatus.pStatus[0].dwMax		= GetID();
			pLayer->BroadCastSectionMsg((char*)&UserStatus, UserStatus.GetPacketSize(), GetPrevSectionNum());
		}
	}
}


float CUser::GetODC() const
{
	return min(max((GetNewestEgo()*2.f / GetLevel()) * (100 + 5 * m_wClassRank) / 100.F, 0.5F), 6);
}


void CUser::SetHP(DWORD dwHP, const CUnit* pAttacker)
{
	dwHP = min(dwHP, (WORD)GetMaxHP());

	if (dwHP != GetHP())
	{
		if(m_sMatchDescInfo.m_bMatching)
		{
			DSTC_MATCH_USER_INFO	packet;

			packet.bUserStatus = 0;	// ���� ����� ���� 
			packet.dwMatchUserIndex = GetID();

			packet.dwHP		= GetHP();
			packet.dwMaxHP	= GetMaxHP(); 
			packet.dwMP		= GetSP();
			packet.dwMaxMP	= GetMaxSP();

			CDungeonLayer	* pLayer = GetCurDungeonLayer();
			if (pLayer)
			{
				pLayer->BroadCastMsg((char *)&packet, packet.GetPacketSize());
			}
		}
		
		if(m_dwPartyId!=0)
		{
			// �ٸ� ��Ƽ������ ������ �� ���Ҵ��� �˷����.	// ������ �ƴϰ� 
			WSTDS_PARTY_HP pHpPacket;
			pHpPacket.dwPartyId			= m_dwPartyId;
			pHpPacket.dwUserIndex		= GetID();
			pHpPacket.wMaxHp			= (WORD)GetMaxHP();
			pHpPacket.wHP				= (WORD)dwHP;
			g_pNet->SendToServer(WSINDEX, (char*)&pHpPacket, pHpPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		}
	}
	
	m_dwHP = dwHP;
}


void CUser::SetQueryUserInfo(char* pResult)
{
	DWORD dwOffset = 0;
	
	SetID(*(DWORD*)(pResult+dwOffset));
	dwOffset+=sizeof(DWORD);
	memcpy(m_szName, pResult+dwOffset, sizeof(m_szName));
	dwOffset+=sizeof(m_szName);
	memcpy(&m_wHead, pResult+dwOffset, sizeof(m_wHead));
	dwOffset+=sizeof(m_wHead);
	SetClass(*(WORD*)(pResult+dwOffset));
	dwOffset+=sizeof(WORD);
	SetRace(*(WORD*)(pResult+dwOffset));
	dwOffset+=sizeof(WORD);
	SetGrade(*(WORD*)(pResult+dwOffset));
	dwOffset+=sizeof(WORD);
	memcpy(&m_dwHP, pResult+dwOffset, sizeof(WORD));
	dwOffset+=sizeof(WORD);
	memcpy(&m_dwSP, pResult+dwOffset, sizeof(WORD));
	dwOffset+=sizeof(WORD);
	SetStatPoint(*(WORD*)(pResult+dwOffset));
	dwOffset+=sizeof(WORD);
	memcpy(&m_wPointSkill, pResult+dwOffset, sizeof(m_wPointSkill));
	dwOffset+=sizeof(m_wPointSkill);
	memcpy(&m_dwExp, pResult+dwOffset, sizeof(m_dwExp));
	dwOffset+=sizeof(m_dwExp);
	SetLevel(*(DWORD*)(pResult+dwOffset));
	dwOffset+=sizeof(DWORD);
	memcpy(&m_dwHonor, pResult+dwOffset, sizeof(m_dwHonor));
	dwOffset+=sizeof(m_dwHonor);
	SetEgo(*(DWORD*)(pResult+dwOffset));
	dwOffset+=sizeof(DWORD);
	SetStr(*(DWORD*)(pResult+dwOffset));
	dwOffset+=sizeof(DWORD);
	SetInt(*(DWORD*)(pResult+dwOffset));
	dwOffset+=sizeof(DWORD);
	SetDex(*(DWORD*)(pResult+dwOffset));
	dwOffset+=sizeof(DWORD);
	SetVit(*(DWORD*)(pResult+dwOffset));
	dwOffset+=sizeof(DWORD);
	memcpy(&m_dwLuck, pResult+dwOffset, sizeof(m_dwLuck));
	dwOffset+=sizeof(m_dwLuck);
	SetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON, *(BYTE*)(pResult+dwOffset));
	dwOffset+=sizeof(BYTE);
	SetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON, *(BYTE*)(pResult+dwOffset));
	dwOffset+=sizeof(BYTE);
	memcpy(&m_pEquip, pResult+dwOffset, sizeof(m_pEquip));
	dwOffset+=sizeof(m_pEquip);
	memcpy(&m_pInv_Large, pResult+dwOffset, sizeof(m_pInv_Large));
	dwOffset+=sizeof(m_pInv_Large);
	memcpy(&m_pInv_Small, pResult+dwOffset, sizeof(m_pInv_Small));
	dwOffset+=sizeof(m_pInv_Small);
	memcpy(&m_pBelt, pResult+dwOffset, sizeof(m_pBelt));
	dwOffset+=sizeof(m_pBelt);
	memcpy(&m_pwSkillLevel, pResult+dwOffset, sizeof(m_pwSkillLevel));
	dwOffset+=sizeof(m_pwSkillLevel);
	memcpy(m_szGuildName, pResult+dwOffset, sizeof(m_szGuildName));
	dwOffset+=sizeof(m_szGuildName);
	memcpy(m_szNickName, pResult+dwOffset, sizeof(m_szNickName));
	dwOffset+=sizeof(m_szNickName);
	memcpy(&m_byGroupType, pResult+dwOffset, sizeof(m_byGroupType));
	dwOffset+=sizeof(m_byGroupType);
	memcpy(&m_byRank, pResult+dwOffset, sizeof(m_byRank));
	dwOffset+=sizeof(m_byRank);
	memcpy(&m_dwGuildId, pResult+dwOffset, sizeof(m_dwGuildId));
	dwOffset+=sizeof(m_dwGuildId);
	memcpy(&m_pInv_Guardian, pResult+dwOffset, sizeof(m_pInv_Guardian));
	dwOffset+=sizeof(m_pInv_Guardian);
	memcpy(&m_dwMoney, pResult+dwOffset, sizeof(m_dwMoney));
	dwOffset+=sizeof(m_dwMoney);
	memcpy(&m_wCriminalPropensity, pResult+dwOffset, sizeof(m_wCriminalPropensity));
	dwOffset+=sizeof(m_wCriminalPropensity);
	memcpy(&m_wClassRank, pResult+dwOffset, sizeof(m_wClassRank));
	dwOffset+=sizeof(m_wClassRank);
	memcpy(&m_sPKDescInfo.m_bPKCount, pResult+dwOffset, sizeof(m_sPKDescInfo.m_bPKCount));
	dwOffset+=sizeof(m_sPKDescInfo.m_bPKCount);
	memcpy(&m_sPKDescInfo.m_bPKRepeatCount, pResult+dwOffset, sizeof(m_sPKDescInfo.m_bPKRepeatCount));
	dwOffset+=sizeof(m_sPKDescInfo.m_bPKRepeatCount);
	memcpy(&m_sPKDescInfo.m_dwPKTime, pResult+dwOffset, sizeof(m_sPKDescInfo.m_dwPKTime));
	dwOffset+=sizeof(m_sPKDescInfo.m_dwPKTime);
	memcpy(&m_sPKDescInfo.m_dwPKRepeatTime, pResult+dwOffset, sizeof(m_sPKDescInfo.m_dwPKRepeatTime));
	dwOffset+=sizeof(m_sPKDescInfo.m_dwPKRepeatTime);
	memcpy(&m_sMatchDescInfo.m_dwMatchRecords, pResult+dwOffset, sizeof(m_sMatchDescInfo.m_dwMatchRecords));
	dwOffset+=sizeof(m_sMatchDescInfo.m_dwMatchRecords);
	memcpy(&m_sMatchDescInfo.m_dwMatchWin, pResult+dwOffset, sizeof(m_sMatchDescInfo.m_dwMatchWin));
	dwOffset+=sizeof(m_sMatchDescInfo.m_dwMatchWin);
	memcpy(&m_sMatchDescInfo.m_dwMatchLose, pResult+dwOffset, sizeof(m_sMatchDescInfo.m_dwMatchLose));
	dwOffset+=sizeof(m_sMatchDescInfo.m_dwMatchLose);
	memcpy(&m_bCurrentHand, pResult+dwOffset, sizeof(m_bCurrentHand));
	dwOffset+=sizeof(m_bCurrentHand);
	memcpy(&m_byLastUsedSkillItem, pResult+dwOffset, sizeof(m_byLastUsedSkillItem));
	dwOffset+=sizeof(m_byLastUsedSkillItem);
	memcpy(&m_byLastUsedStatItem, pResult+dwOffset, sizeof(m_byLastUsedStatItem));
	dwOffset+=sizeof(m_byLastUsedStatItem);
	memcpy(m_szId, pResult+dwOffset, sizeof(m_szId));
	dwOffset+=sizeof(m_szId);
	memcpy(&m_bInvItemCheck, pResult+dwOffset, sizeof(m_bInvItemCheck));
	dwOffset+=sizeof(m_bInvItemCheck);
	memcpy(&m_bBankItemCheck, pResult+dwOffset, sizeof(m_bBankItemCheck));
	dwOffset+=sizeof(m_bBankItemCheck);
}


BYTE CUser::GetPartyLeader()
{
	return m_bPartyLeader;
}

void CUser::SetPartyLeader(BYTE bPartyLeader)
{
	m_bPartyLeader = bPartyLeader;
}

DWORD CUser::GetPartyID()
{
	return m_dwPartyId;
}

void CUser::SetPartyID(DWORD dwPartyID)
{
	if(m_dwPartyId)
	{
		GetCurDungeon()->GetPartyUserHash()->Delete(this, m_dwPartyId);
		Event_SetPartyID( (CUser*)this );
	}

	m_dwPartyId = dwPartyID;
	
	if(m_dwPartyId)
	{
		GetCurDungeon()->GetPartyUserHash()->Insert(this, dwPartyID);
		Event_SetPartyID( (CUser*)this );
	}

	if(GetCurDungeon() && IsCurDungeonSiege())
	{
		GetCurDungeon()->InsertDefensePartyList(dwPartyID);

		// ���� ���γ��� ��Ƽ �ξ��ٸ�	�̳�� ��Ƽ ��������� Ȥ�ó� ���� ������ ����
		// �����ڷ� ���������̴� ���� ��Ƽ���� ����ڷ� ��ȯ�����.
		if (GetID() == GetCurDungeon()->GetDungeonDataEx()->m_dwOwnerIndex && dwPartyID)
		{
			Node<CUser>* pnode = GetCurDungeon()->GetPartyUserHash()->GetBucketHead(dwPartyID);

			while(pnode)
			{
				if (pnode->pData != this && dwPartyID == pnode->pData->m_dwPartyId)
				{
					pnode->pData->SetAttackMode(GetCurDungeon()->GetAttackMode(pnode->pData));
				}

				pnode = pnode->pNext;
			}
		}
	}	
}


WORD CUser::GetCurDungeonLayerMonsterCount()
{
	return GetCurDungeonLayer()->m_wCurMonster;
}


void CUser::SetGuildID(DWORD dwGuildID)
{
	if(m_dwGuildId)
	{
		if(GetCurDungeon() && GetCurDungeon()->GetGuildUserHash())
			GetCurDungeon()->GetGuildUserHash()->Delete(this, m_dwGuildId);
	}

	m_dwGuildId = dwGuildID;

	if(m_dwGuildId)
	{
		if(GetCurDungeon() && GetCurDungeon()->GetGuildUserHash())
			GetCurDungeon()->GetGuildUserHash()->Insert(this, dwGuildID);
	}	
}


DWORD CUser::GetPrevTickCount()				
{
	return m_dwClientPrevTick;
}

void CUser::SetPrevTickCount(DWORD dwTick)		
{
	m_dwClientPrevTick = dwTick;
}

PLAYERSHOPINFO* CUser::GetPlayerShopInfo()
{
	return &m_sPlayerShop;
}

WORD CUser::GetLeftoverStatSkillPoint() const
{
	return m_wPointSkill;
}

void CUser::SetLeftOverStatSkillPoint(WORD wSkillPoint)
{
	m_wPointSkill = wSkillPoint;
}

WORD CUser::GetHeadModelNo() const
{
	return m_wHead;
}

void CUser::SetHeadModelNo(WORD wHeadModelNo)
{
	m_wHead = wHeadModelNo;
}

DWORD CUser::GetLuck() const
{
	return m_dwLuck;
}

void CUser::SetLuck(DWORD dwLuck)
{
	m_dwLuck = dwLuck;
}

DWORD CUser::GetHonor() const
{
	return m_dwHonor;
}

void CUser::SetHonor(DWORD dwHonor)
{
	m_dwHonor = dwHonor;
}

const char*	CUser::GetName() const
{
	return m_szName;
}


BOOL CUser::OverDriveChk(BOOL bChk)
{
	if (GetItemAttr(ITEM_ATTR_FREE_OVERDRIVE_SKILL ))
		return TRUE;

	DSTC_OVERDRIVE_CHK pPacket;
		
	for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
	{
		if(m_pInv_Small[i].GetID()==__ITEM_OVERDRIVE_CHK__)
		{
			if(bChk)
			{
				pPacket.byZipCode	= BYTE(i);
				pPacket.byExc		= 1;

				if(m_pInv_Small[i].GetQuantity() > 1)
				{
					m_pInv_Small[i].SetQuantity(BYTE(m_pInv_Small[i].GetQuantity()-1));
				}
				else
				{
					memset(&m_pInv_Small[i], 0, sizeof(CItem));
				}

				memcpy(&pPacket.cItem, &m_pInv_Small[i], sizeof(CItem));
				g_pNet->SendToUser(m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION);
			}
			
			return TRUE;
		}
	}
	
	if(bChk)
	{
		pPacket.byZipCode	= 0;
		pPacket.byExc		= 0;
		memset(&pPacket.cItem, 0, sizeof(CItem));
		g_pNet->SendToUser(m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}	

	return FALSE;
}

void CUser::SetReservPotalPosition(VECTOR2* pv2Position)
{
	m_v2ReservPotalPosition = *pv2Position;
}

const VECTOR2* CUser::GetReservPotalPosition() const
{
	return &m_v2ReservPotalPosition;
}

const BYTE*	CUser::GetSkillLevelPtr() const
{
	return m_pwSkillLevel;
}


BOOL CUser::IsPickUpItem()
{
#if defined TAIWAN_LOCALIZING
	return BOOL( !(GetAverageWeight() >= 100) );
#else
	return TRUE;
#endif
}

void CUser::InitStat(int nInc)
{
	SetEgo(g_pBaseClassInfo[GetClass()].iEGO);
	SetStr(g_pBaseClassInfo[GetClass()].iSTR);
	SetInt(g_pBaseClassInfo[GetClass()].iINT);
	SetDex(g_pBaseClassInfo[GetClass()].iDEX);
	SetVit(g_pBaseClassInfo[GetClass()].iVIT);
	UpdateMaxHP();
	UpdateMaxSP();
	SetHP((WORD)GetMaxHP());
	SetSP((WORD)GetMaxSP());

	SendAllStatus();		
	
	WORD wPoint = 0;

	for(DWORD i = 0; i < GetLevel()-1; i++)	
		wPoint += GetStatPointByLevel(GetObjectType(), i);

	SetStatPoint(WORD(wPoint+nInc));

	DSTC_STATUS_INIT pPacket;	
	pPacket.wPoint	= GetNotUseStatPoint();
	NetSendToUser(m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	
	UpdateUserDBThread(this);
	Log( LOG_JUST_FILEWRITE, "UpdateUserDB Name : %s, Level : %u, Exp : %u", m_szName, GetLevel(), m_dwExp);

	char szQuery[0xff] = {0,};
	wsprintf(szQuery, "Update CharacterInfo Set LastUsedStatItem = %u where Char_Index = %u", nInc+50, GetID());
	g_pDb->THExecuteSQL(szQuery, FALSE, FALSE, NULL, GAME_DB);
}


void CUser::InitSkill(int nInc)
{
	memset(m_pwSkillLevel, 0, sizeof(m_pwSkillLevel));
	m_wPointSkill		= WORD(GetLevel()+nInc);
	SetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON, __SKILL_ATTACK__);
	SetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON, __SKILL_NONE_SELECT__);
	
	DSTC_SKILL_INIT pPacket;
	
	pPacket.wPoint = m_wPointSkill;
	memcpy(pPacket.bySkill, m_pwSkillLevel, sizeof(m_pwSkillLevel));
	NetSendToUser(m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	
	 UpdateUserDBThread(this);
	Log( LOG_JUST_FILEWRITE, "UpdateUserDB Name : %s, Level : %u, Exp : %u", m_szName, GetLevel(), m_dwExp);

	char szQuery[0xff] = {0,};
	wsprintf(szQuery, "Update CharacterInfo Set LastUsedSkillItem = %u where Char_Index = %u", nInc+50, GetID());
	g_pDb->THExecuteSQL(szQuery, FALSE, FALSE, NULL, GAME_DB);
}


//////////////////////////////////////////////////////////////////////////
// about match
void CUser::InitMatch()
{
	m_sMatchDescInfo.m_dwMatchUserIndex = 0;
	m_sMatchDescInfo.m_bMatching = 0;
}


void CUser::CancelMatch(enum MATCH_STATUS eMatchStatus)
{
	if (TRUE == m_sMatchDescInfo.m_bMatching)
	{
		// ���� ���� ���� ���׿��� ȿ���� �ִ�. 
		return;
	}

	DSTC_DUNGEON_REPLY_MATCH_MAN2MAN	ReplyPacket;

	switch (eMatchStatus) 
	{
	case MATCH_STATUS_USER_REQUEST_AFTER_USER_GOTO_WORLD:
		{
			ReplyPacket.bReplyCode = MATCH_STATUS_USER_REQUEST_AFTER_USER_GOTO_WORLD;
			goto SendPacket_Routine;
		}
		break;
	case MATCH_STATUS_USER_REQUEST_AFTER_EXIT_GAME:
		{
			ReplyPacket.bReplyCode = MATCH_STATUS_USER_REQUEST_AFTER_EXIT_GAME;
			goto SendPacket_Routine;
		}
		break;
	default:	
		return;
	}	

SendPacket_Routine:
	CUser * pUser = NULL;
	pUser = g_pUserHash->GetData(m_sMatchDescInfo.m_dwMatchUserIndex);
	if (pUser)
	{
		NetSendToUser(pUser->m_dwConnectionIndex, (char*)&ReplyPacket, ReplyPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

		// ������ ���� �ʱ�ȭ 
		pUser->InitMatch();		
	}

	// �ڽ��� ���� �ʱ�ȭ 
	InitMatch();			
}

BOOL CUser::IsGMUser() const
{
	return BOOL(USER_TYPE_GM == m_bUserType);
}

void CUser::ReLive( BYTE byFlag /*=USER_RELIVE_MODE_WITH_HP*/, DWORD dwHP /*=1*/ )
{
	DWORD dwClientHP = 0;

	if ( GetHP() == 0 )
	{
		DSTC_USER_STATUS	UserStatus;
		UserStatus.bStatusMany	= 0;

		switch ( byFlag )
		{
		case USER_RELIVE_MODE_WITH_HP:
			dwClientHP = dwHP;
			break;

		case USER_RELIVE_MODE_HALF:
			dwClientHP = GetMaxHP() / 2;
			break;

		case USER_RELIVE_MODE_FULL:
			dwClientHP = GetMaxHP();
			break;
		}

		dwClientHP = min( dwClientHP, GetMaxHP() );

		SetHP( dwClientHP );

		UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HP;
		UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= dwClientHP;
		
		NetSendToUser( m_dwConnectionIndex, (char*)&UserStatus,  UserStatus.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION );
	}
}

// �߱�, PK��� ���Ƽ, ī���� ��� : ����� 2005.4.1
// ���Լ��� �⺻������ PK �ϰ�츸 ȣ���.
void CUser::PKDropItem( CUser * pOffense ) 
{
	//��ġ ������ ���⼭ �Ұ�.
	const int MAX_PERCENT = 80; //
	const int MIN_PERCENT = 20; // 
	const int DUNGEONSIEGE_PERCENT = 20;
	const int MAX_SLOT    = 22; //(���� ����21 + 1 )�������� ���� ���Ҷ� ����.
	const int ITEM_OWNER_SEC = 35 ; //������ �����ð�


	// TODO : ����� �׼� ����, ���ΰ� ����԰� ���� : �ִ��� 2005.1.20
	/*if(AppearItem(this, NULL, 0, 2000, TRUE)) {
		m_dwMoney -= 2000;
	}*/

    

	//TODO: ��� ���� ���� ó�� �Ұ�.
	//#1. PK ��.(���ǳ�) 
	//#2. �������� �׾��� �������� �׾���. : call �ϱ� ���� �̹� ���е�. �����ϰ�� �Ϸ� ���ü� ����.
	//#3.�������ΰ�. : IsCurDungeonSiege()
	//4. ������ ó��. pOffense;
	int nDropPercent =0 ;
	if(m_sPKDescInfo.m_bBad) // 
		nDropPercent = MAX_PERCENT ;
	else
		nDropPercent = MIN_PERCENT ;

	if(IsCurDungeonSiege()) // �������ϰ��� ������ ��� ĳ�� 20 %
		nDropPercent = DUNGEONSIEGE_PERCENT ;




	if( (rand() % 100 )  > nDropPercent ) return ;


	//����� �������� ���ϱ�.
	const int nSlot = ( rand() % MAX_SLOT ) - 1 ; // 0 ~ 20 
	

	Log(LOG_IMPORTANT, " Drop Slot : %d ", nSlot);

	//�ش� ���Կ� ������ �ֳ�.
	if(0 == m_pEquip[nSlot].GetID() ) return  ;

	//================================================================|
	//1���� �ϴ� ���� �������� ���� ����.
	// �ֳĸ�, ���� ������ ������ ����â���� ����� ��Ŷ�� ������ ����.
	//================================================================|
	WORD nDropSlotList[10] = {0,1,2,3,4,5,6,10,11,12};
	bool bDrop = false;
    for( int i =0 ; i < 10 ; ++i)
	{
		if(nDropSlotList[i] == nSlot)
		{
			bDrop = true;
			break;
		}
	}	
	if( !bDrop ) return ;
	//================================================================|

	//Ư�̻���: PK�� ���Ҷ��� ������ ������ �����ڰ� �ȴ�. : �߱��� �䱸����.
	//�������� 35�� �����ȴ�.
	if(AppearItem( this, &m_pEquip[nSlot],0,0,TRUE,0,0,0,35, pOffense))
	{

		//�Ʒ� ó�� ���� �ſ� �߿���.
		WORD nValue = m_pEquip[nSlot].m_wItemID / ITEM_DISTRIBUTE; 
		memset(&m_pEquip[nSlot], 0, sizeof(CItem));
		
		SendAllStatus();

		if((nValue>=0 && nValue<=7) || nValue==12)
		{
			if(m_bCurrentHand==1)
				ItemHandMod(this, m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID, m_pEquip[EQUIP_TYPE_LHAND1].m_wItemID);
			else
				ItemHandMod(this, m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID, m_pEquip[EQUIP_TYPE_LHAND2].m_wItemID);
		}
		else if(nValue==11)
		{
			ItemArmorMod(this, m_pEquip[EQUIP_TYPE_MAIL].m_wItemID);
		}
		else if(nValue==10)
		{
			ItemHelmetMod(this, m_pEquip[EQUIP_TYPE_HELMET].m_wItemID);
		}

		//TODO :2�� �۾��� �������� �� �߰��ؼ� ������ ��ӽ� ���� â���� ���� ��� ��.
		//else if( nValue==15)
		//{
		//	�߰�ItemBootsMod(this, m .....)
		//}
		//else if( nValue==16)
		//{
		//	�߰�
		//}

	}
}

bool CUser::OnDie()
{
	DeathTime( ::timeGetTime() );//�����ð� ���
	return true;
}