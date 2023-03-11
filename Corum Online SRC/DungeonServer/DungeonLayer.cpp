#include "stdafx.h"
#include <math.h>
#include <crtdbg.h>
#include "DungeonLayer.h"
#include "OwnServer.h"
#include "Section.h"
#include "Dungeon.h"
#include "DungeonTable.h"
#include "MonsterTable.h"
#include "MonsterManager.h"
#include "effect.h"
#include "recvmsg.h"
#include "Map.h"
#include "LayerFormation.h"
#include "PathFinding\\SearchModule_Astar.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "Monster.h"
#include "DUNGEON_DATA_EX.h"
#include "user.h"
#include "DBProc.h"
#include "GameSystemNative.h"
#include "EventDungeonManager.h"
#include "../CommonServer/CommonHeader.h"

#include ".\tm_killingfield.h"

//-----------------------------------------------------------------------------
// ������ ����(Layer)�� �����Ѵ�(�Ϲ� ���������� ���) 
//	
//�� ���� : �Ϲݴ����� ��쿡�� ���� �ʱ� ����� ����ȴ�. ���� ���ư��� �ִ� ����
//			������� ����..    �ܺ� ���� �о�帮�� new�� �޸� �Ҵ��ؼ� ����.�Ѥ� 
//			�ʺ��� ������ ��� COwnServer::SpawnNewSession()�� ���� �޸�Ǯ�� �Ҵ��Ѵ�.
//-----------------------------------------------------------------------------
void CDungeonLayer::Create(DWORD dwID, BYTE bLayer, CDungeon* pParent)
{
	memset(this, 0, sizeof(CDungeonLayer));	
	m_pMap = new CMap;
	m_pLayerFormation = new LAYER_FORMATION;
	
	m_bLayer	= bLayer;		
	m_pParent	= pParent;
	
	m_pTeamMatch = NULL;//����ġ �ʱ�ȭ : hwoarang 050202

	InitList();
	
	//�Ӽ�����, ����, ���� Spawn �������� ���� �о�帲 By *.ttb ���� 
	m_pMap->Create(dwID, this);
	
	m_dwLastFrameSec		= 0;
	m_dwLastOwnerFrameSec	= 0;
	
	// ���� �ʱⰪ.
	InitMatch();
	
	// PathFinder
	m_pSw = new Sw;
	m_pSw->Initialize( 20, 20, m_pMap->m_dwTileNumWidth, m_pMap->m_dwTileNumHeight, sizeof(MAP_TILE), (PVOID)&( m_pMap->GetTile(0, 0)->wAttr ) );
	
	// Make Layer Formation.
	char szQuery[ 0xff ]={0,};
	wsprintf(szQuery, "Select DungeonID, Layer, MonsterFormation, MonsterKind from LayerFormation where DungeonID=%d and Layer=%d", m_pParent->GetID(), bLayer );
	int nRet = 	g_pDb->OpenRecord(szQuery, m_pLayerFormation, 1, GAME_DB);

	if(nRet < 0) Log(LOG_IMPORTANT, "Make Layer Formation Failed! Dungeon %d, Layer %d", m_pParent->GetID(), bLayer );
	
	CreateMonsterAll();

	//����� ���̾ �� ��ġ ����: hwoarang 050202
	DUNGEON_DATA_EX* pDungeonData = pParent->GetDungeonDataEx();
	if(pDungeonData)
	{
		if( pDungeonData->IsStadium() )
		{//Layer
			FLAG_TYPE match_type= M_T_KILLING_FIELD;
			CreateTeamMatch( match_type );
			return;
		}
	}
	else
	{
		assert( NULL && "!!! Created Layer Can't Get DungeonData");
	}
}

bool CDungeonLayer::CreateTeamMatch(const FLAG_TYPE dwMatchType)
{
	switch (dwMatchType & M_T_CMP_BASE)
	{
	case M_T_DEAH_MATCH:
	case M_T_KING_OF_THE_HILL:
	case M_T_CAPTURE_THE_FLAG:
	case M_T_LEADER_KILL:
		{
			m_pTeamMatch = NULL;
		}break;
	case M_T_KILLING_FIELD:
		{
			m_pTeamMatch = new CTM_KillingField( this->m_pParent->GetID(), this->m_bLayer);
		}break;
	} // switch (dwMatchType & M_T_CMP_BASE)

	return false;
}


//-----------------------------------------------------------------------------
// ������ ����(Layer)�� �����Ѵ�.(�Ϲ� ���������� ���) 
//	
//�� ���� : �Ϲݴ����� ��쿡�� ���� ����� ����ȴ�.
//			�ʺ��� ���������ϰ�� �̸��� ������ ū�ϳ���. �Ѥ�;
//-----------------------------------------------------------------------------
void CDungeonLayer::Destroy()
{
	for(int i=0; i<m_wTotalSectionNum; i++)
	{
		if( m_pSection[i]->m_pItemHash )
		{
			m_pSection[i]->m_pItemHash->Destroy();
			delete m_pSection[i]->m_pItemHash;
			m_pSection[i]->m_pItemHash = NULL;
		}

		delete m_pSection[i];
		m_pSection[i] = NULL;
	}
	
	if( m_pItemHash )
	{
		m_pItemHash->Destroy();
		delete m_pItemHash;
		m_pItemHash = NULL;
	}
	
	m_pMap->Destroy();

	if( m_pMoveSpot )
	{
		delete[] m_pMoveSpot;
		m_pMoveSpot = NULL;
	}
	
	if( m_pStartSpot )
	{
		delete [] m_pStartSpot;
		m_pStartSpot = NULL;
	}
	
	if( m_pSw )
	{
		delete m_pSw;
		m_pSw = NULL;
	}

	if (m_pPcList)
	{
		m_pPcList->RemoveAll();
		delete m_pPcList;
		m_pPcList = NULL;
	}

	if (m_pMonsterList)
	{
		m_pMonsterList->RemoveAll();
		delete m_pMonsterList;
		m_pMonsterList	= NULL;
	}

	if (m_pResponseMonsterList)
	{
		m_pResponseMonsterList->RemoveAll();
		delete m_pResponseMonsterList;
		m_pResponseMonsterList = NULL;
	}

	if (m_pCreateItemList)
	{
		m_pCreateItemList->RemoveAll();
		delete m_pCreateItemList;
		m_pCreateItemList = NULL;
	}

	if (m_pActiveSectionList)
	{
		m_pActiveSectionList->RemoveAll();
		delete m_pActiveSectionList;
		m_pActiveSectionList = NULL;
	}

	if (m_pDeadMonsterList)
	{
		m_pDeadMonsterList->RemoveAll();
		delete m_pDeadMonsterList;
		m_pDeadMonsterList = NULL;
	}

	if (m_pMap)
	{
		delete m_pMap;
		m_pMap = NULL;
	}

	if (m_pLayerFormation)
	{
		delete m_pLayerFormation;
		m_pLayerFormation = NULL;
	}	
}

DWORD CDungeonLayer::GetUserCount()	
{	
	return m_pPcList->GetCount();	
}

void CDungeonLayer::InitMatch()
{
	m_bMatching					= FALSE;
	m_bStartMatch				= FALSE;
	m_dwMatchBeginTime			= 0;
	m_dwMatchTick				= MATCH_TIME;		// 5�� ����(300��)
	m_dwMatchEndTime			= 0;
	m_bReadyMatchNum			= 0;
	m_dwEndMatchJudgmentTick	= 0;
	m_dw10SecEndMatchLayerTick	= 0;
}

BOOL CDungeonLayer::SetSection(int nSectionNum, CSection* pSection)
{
	if(nSectionNum < 1 || !pSection)
	{
		__asm int 3
		return FALSE;
	}
	
	// ������ �ؽ��� �������ش�.
	pSection->m_pItemHash = new ITEM_HASH;
	pSection->m_pItemHash->InitializeHashTable( 5000, 5000 );

	m_pSection[ nSectionNum-1 ] = pSection;
	m_pSection[ nSectionNum-1 ]->m_pActiveListPos	= NULL;
	
	return TRUE;
}

//-----------------------------------------------------------------------------
// ������ ����(Layer)�� ��� �������� �޼����� Broadcast �� 
//-----------------------------------------------------------------------------
DWORD CDungeonLayer::BroadCastMsg( char* pMsg, DWORD wLength ) const
{
	DWORD dwCounter = 0;

	CUser* pUser=0;
	POSITION_ pos = m_pPcList->GetHeadPosition();
	
	while(pos)
	{
		 pUser = (CUser*)m_pPcList->GetNext(pos);
	     NetSendToUser(pUser->m_dwConnectionIndex, pMsg, wLength, FLAG_SEND_NOT_ENCRYPTION);
		 ++dwCounter;
	}

	return dwCounter;
}

//-----------------------------------------------------------------------------
// ������ ����(Layer)�� Sender�� ������ ��� �������� �޼����� Broadcast  
//-----------------------------------------------------------------------------
DWORD CDungeonLayer::BroadCastMsgExceptSender( char* pMsg, DWORD wLength, const CUser* pSender ) const
{
	DWORD dwCounter = 0;

	CUser* pUser=0;
	POSITION_ pos = m_pPcList->GetHeadPosition();
	
	while(pos)
	{
		 pUser = (CUser*)m_pPcList->GetNext(pos);
		 if(pUser == pSender) continue;
	     NetSendToUser(pUser->m_dwConnectionIndex, pMsg, wLength, FLAG_SEND_NOT_ENCRYPTION);
		 ++dwCounter;
	}

	return dwCounter;
}

//-----------------------------------------------------------------------------
// ������ ���ǰ� �׼����� ��ũ ���ǿ� �����ִ� �������� �޽����� ������.  ���� ����
//-----------------------------------------------------------------------------
DWORD CDungeonLayer::BroadCastSectionMsg( char* pMsg, DWORD wLength, const CUser* pSender ) const
{
	DWORD dwCounter = 0;

	CSection*	pSection = GetSection( pSender->GetPrevSectionNum() );
	if( !pSection ) return 0;	

	// ������ ������ �������� �޽�������.
	CUser*		pUser	= NULL;
	POSITION_	pos		= pSection->m_pPcList->GetHeadPosition();

	while(pos)
	{
		pUser = (CUser*)pSection->m_pPcList->GetNext(pos);
		if(pUser == pSender ) continue;
		NetSendToUser( pUser->m_dwConnectionIndex, pMsg, wLength, FLAG_SEND_NOT_ENCRYPTION );
		++dwCounter;
	}

	// ������ ������ ��ũ�� ������ �������� �޽��� ����.
	CSection* pLinkSection = NULL;

	for( int i=0; pSection->m_wLink[i] != 0; i++ )
	{
		pLinkSection = GetSection(pSection->m_wLink[i]);
		if( !pLinkSection ) continue;

		pos = pLinkSection->m_pPcList->GetHeadPosition();
		while(pos)
		{
			pUser = (CUser*)pLinkSection->m_pPcList->GetNext(pos);
			NetSendToUser( pUser->m_dwConnectionIndex, pMsg, wLength, FLAG_SEND_NOT_ENCRYPTION );
			++dwCounter;
		}
	}
	
	return dwCounter;
}

//-----------------------------------------------------------------------------
// ���ǰ� �׼����� ��ũ ���ǿ� �����ִ� �������� �޽����� ������.  
//-----------------------------------------------------------------------------
DWORD CDungeonLayer::BroadCastSectionMsg( char* pMsg, DWORD wLength, BYTE bSectionNum ) const
{
	DWORD dwCounter = 0;

	// ���� �۽��ڰ� �����ִ� ������ ��ȣ.
	CSection*	pSection = GetSection( bSectionNum );		
	if( !pSection ) return dwCounter;	

	// ������ ������ �������� �޽�������.
	CUser*		pUser	= NULL;
	POSITION_	pos = pSection->m_pPcList->GetHeadPosition();
	while(pos)
	{
		pUser = (CUser*)pSection->m_pPcList->GetNext(pos);
		NetSendToUser( pUser->m_dwConnectionIndex, pMsg, wLength, FLAG_SEND_NOT_ENCRYPTION );
		++dwCounter;
	}

	// ������ ������ ��ũ�� ������ �������� �޽��� ����.
	CSection* pLinkSection = NULL;

	for( int i=0; pSection->m_wLink[i] != 0; i++ )
	{
		pLinkSection = GetSection(pSection->m_wLink[i]);
		if( !pLinkSection ) continue;

		pos = pLinkSection->m_pPcList->GetHeadPosition();
		while(pos)
		{
			pUser = (CUser*)pLinkSection->m_pPcList->GetNext(pos);
			NetSendToUser( pUser->m_dwConnectionIndex, pMsg, wLength, FLAG_SEND_NOT_ENCRYPTION );
			++dwCounter;
		}
	}
	
	return dwCounter;
}

//-----------------------------------------------------------------------------
// ���ڷ� ���� ���ǿ� �޽����� ������ ��ũ ���ǿ��� ������ �ʴ´�. ��ũ ������ �������� �ʰ� �޽����� ������. 
//-----------------------------------------------------------------------------
DWORD CDungeonLayer::BroadCastSectionMsgExceptLink( char* pMsg, DWORD wLength, BYTE bSectionNum ) const
{
	DWORD dwCounter = 0;

	CSection*	pSection = GetSection( bSectionNum );
	if( !pSection ) return dwCounter;

	CUser*		pUser	= NULL;
	POSITION_ pos = pSection->m_pPcList->GetHeadPosition();
	while( pos )
	{
		pUser = (CUser*)pSection->m_pPcList->GetNext(pos);
		NetSendToUser( pUser->m_dwConnectionIndex, pMsg, wLength, FLAG_SEND_NOT_ENCRYPTION );
		++dwCounter;
	}

	return dwCounter;
}

DWORD CDungeonLayer::BroadCastSectionMsgExceptLink( char* pMsg, DWORD wLength, CUser* pSender ) const
{
	pMsg;wLength;pSender;

	// ���� ������ ������ ���� �ڵ� ���Ѵ�.. �� �� �� 
	return 0;
}


//-----------------------------------------------------------------------------
// �������� ����� �� ���ǵ鿡 ���ϴ� �������� �޽����� ������. ������ �������� ���� �����Ѵ�.
//-----------------------------------------------------------------------------
void CDungeonLayer::IntersectSection( BYTE bPrevSectionNum, BYTE bCurSectionNum, BYTE* pIntersectSection )
{
	if( !pIntersectSection ) return;

	CSection*	pPrevSection	= GetSection( bPrevSectionNum );
	CSection*	pCurSection		= GetSection( bCurSectionNum );	
	
	if( !pPrevSection ) return;
	if( !pCurSection ) return;

	memset( pIntersectSection, 0, sizeof(BYTE) * MAX_SECTION_LINK_NUM ); 

	int			i=0, j=0, cnt=0;
	BOOL		bInter=0;
	
	// ������ �����.
	for( i=0; pCurSection->m_wLink[i] != 0; i++ )
	{
		if( pCurSection->m_wLink[i] == bPrevSectionNum ) continue;
		
		bInter = TRUE;

		for( j=0; pPrevSection->m_wLink[j] != 0; j++ )
		{
			if( pPrevSection->m_wLink[j] == pCurSection->m_wLink[i] )	// ���⿡ �ɷȴٸ� ���ܽ�Ų��.
			{
				bInter = FALSE;
				break;
			}
		}

		if( bInter )		// �������� ����.
		{
			pIntersectSection[cnt] = (BYTE)pCurSection->m_wLink[i];
			++cnt;
		}
	}
}


// ������ ������ �ٲ������� ���Լ��� �ٸ��������� ������ ��ȯ�Ѵ�.
void CDungeonLayer::BroadCastIntersectSectionInfo( BYTE bPrevSectionNum, BYTE bCurSectionNum, CUser* pOwnUser )
{
	if( !pOwnUser ) return;
	
	int		i=0;
	BYTE	pbIntersectSection[MAX_SECTION_LINK_NUM]={0,};		// ������ ������ ������ �迭.
	
	IntersectSection( bPrevSectionNum, bCurSectionNum, pbIntersectSection );

	for( i=0; pbIntersectSection[i] != 0; i++ )
	{
		SendSectionInfo( pbIntersectSection[i], pOwnUser );
	}
}

void CDungeonLayer::SendSectionInfo( BYTE bSectionNum, CUser* pOwnUser, BYTE bPortal )
{
	CDungeonLayer* pLayer = pOwnUser->GetCurDungeonLayer();
	if( !pLayer ) return;
	
	CSection* pSection = pLayer->GetSection( bSectionNum );
	if( !pSection ) return;

	// pOwnUser�� ������ �ۼ��Ѵ�.
	DSTC_APPEAR	AppearMe;	
	MakeAppearUser(&AppearMe, pOwnUser, bPortal);

	DSTC_APPEAR			AppearUser;

	CUser*						pUser=0;
	ITEM_SERVER*				pItem=0;
	CMonster*					pMonster=0;	
	POSITION_					pos=0;
	ListNode<ITEM_SERVER>*		pItemNode=0;

	pos = pSection->m_pPcList->GetHeadPosition();

	POSITION_ del=0;
	POSITION_ posSkill = NULL;

	while( pos )
	{
		del = pos;
		pUser = (CUser*)pSection->m_pPcList->GetNext( pos );
		
		if (!pUser->GetID()  || !pUser->GetClass() || !pUser->GetHeadModelNo())
		{
			pSection->m_pPcList->RemoveAt(del);
			continue;
		}

		MakeAppearUser(&AppearUser, pUser, 0);
		
		// ������ �ִ���鿡�� ���� ������, ���� �׳���� �޾ƿ´�.
		if(	pUser->GetID() != pOwnUser->GetID() )
		{
			// �����ѳ��� ���� �ٸ������� 	
			if( !(AppearMe.dwGuildWarFlag & G_W_F_OBSERVER) )//������ �������� �ƴϸ� �����ش�.
			{
				NetSendToUser( pUser->m_dwConnectionIndex, (char*)&AppearMe, AppearMe.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );	
			}

			// �����ѳ� 
			NetSendToUser( pOwnUser->m_dwConnectionIndex, (char*)&AppearUser, AppearUser.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION  );	

			if (pUser->GetPlayerShopInfo()->m_bPlayerShopMode)
			{
				// �����ϴ³𿡰� ������ �ִ� ���� ��������� ��������� �˷������.
				DSTC_PLAYERSHOP_OPEN packet;
				memset(packet.szTitle, 0, sizeof(packet.szTitle));
				memcpy(packet.szTitle, pUser->GetPlayerShopInfo()->szTitle, sizeof(packet.szTitle));
				packet.dwUserIndex = pUser->GetID();
				NetSendToUser( pOwnUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );		
			}
			
			if(pUser->m_byRest==1)
			{
				DSTC_PLAYER_REST pSendPacket;
				pSendPacket.byType		= pUser->m_byRest;
				pSendPacket.dwUserIndex	= pUser->GetID();
				NetSendToUser( pOwnUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );		
			}
			
			if (pUser->m_sPKDescInfo.m_bBad)
			{
				DSTC_PLAYER_BAD pSendPacket;
				pSendPacket.byBad		= pUser->m_sPKDescInfo.m_bBad;
				pSendPacket.dwUserIndex	= pUser->GetID();
				NetSendToUser( pOwnUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
			}			
			
			// ������ �ֶ� ���� ���� ��ų�� �ٽ� �������.
			if (pUser->GetUsingStatusEffectList())
			{
				posSkill = pUser->GetUsingStatusEffectList()->GetHeadPosition();
				while(posSkill)
				{					
					EffectDesc* pEffectDesc = (EffectDesc*)pUser->GetUsingStatusEffectList()->GetNext(posSkill);
					g_pEffectLayer->SendSkillStatusUserUser(pOwnUser, pUser, pEffectDesc->pEffect->bID, pEffectDesc->bSkillLevel, pEffectDesc->dwDestTick-g_dwTickCount);
				}
			}
			
			pUser->SendToUserConsumableItemEffectListAll(pOwnUser);
		}
		
		posSkill = pOwnUser->GetUsingStatusEffectList()->GetHeadPosition();
		while(posSkill)
		{
			// �ڱⲬ ������ 			
			EffectDesc* pEffectDesc = (EffectDesc*)pOwnUser->GetUsingStatusEffectList()->GetNext(posSkill);
			g_pEffectLayer->SendSkillStatusUserUser(pUser, pOwnUser, pEffectDesc->pEffect->bID, pEffectDesc->bSkillLevel, pEffectDesc->dwDestTick-g_dwTickCount);	
		}		
	}

	// ���� ������ �޾ƾ��Ѵ�.
	DSTC_APPEAR_MON		AppearMon;	
	DSTC_MOVE_MON		MoveMon;

	pos = pSection->m_pMonsterList->GetHeadPosition();	
	while( pos )
	{
		del = pos;
		pMonster = (CMonster*)pSection->m_pMonsterList->GetNext( pos );

		if (pMonster->GetUnitStatus() == UNIT_STATUS_DEAD)
		{			
			continue;
		}

		MakeAppearMonser( &AppearMon, pMonster );		

		NetSendToUser( pOwnUser->m_dwConnectionIndex, (char*)&AppearMon, AppearMon.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		
		if( pMonster->GetUnitStatus() == UNIT_STATUS_WALKING )
		{
			MoveMon.dwMonsterIndex	= pMonster->GetID();
			MoveMon.v2MoveStartPos	= *pMonster->GetCurPosition();
			MoveMon.v2MoveStopPos	= *pMonster->GetDestPosition();
			MoveMon.v2MoveDirection	= *pMonster->GetCurDirection();
			MoveMon.bMoveType		= UNIT_STATUS_WALKING;
			MoveMon.fSpeed			= pMonster->GetMoveSpeed();
			NetSendToUser( pOwnUser->m_dwConnectionIndex, (char*)&MoveMon, MoveMon.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		}

		// ������ �ִ� ���� ��ų�� �ٽ� �������.
		// Modify By Minbobo.. ���� �����Ҷ� �ٿ�Ǽ� ��������.

		if(pMonster->GetUsingStatusEffectList())
		{
			posSkill = pMonster->GetUsingStatusEffectList()->GetHeadPosition();
			
			while(posSkill)
			{
				
				EffectDesc* pEffectDesc = (EffectDesc*)pMonster->GetUsingStatusEffectList()->GetNext(posSkill);
				g_pEffectLayer->SendSkillStatusUserMon(pOwnUser, pMonster, pEffectDesc->pEffect->bID, pEffectDesc->bSkillLevel, pEffectDesc->dwDestTick-g_dwTickCount);		
			}
		}		
	}
	
	// ���� ������ �������� ������ �޾ƾ��Ѵ�.
	DSTC_APPEAR_ITEM	AppearItem;
	pItemNode = pSection->m_pItemHash->GetHead();
	while( pItemNode )
	{
		pItem = pItemNode->pData;
		
		if(!pItem)
			return;

		AppearItem.bType		= 0;
		AppearItem.v2ItemPos	= pItem->v2ItemPos;
		memcpy( &AppearItem.Item, &pItem->Item, sizeof( CItem ) );
		NetSendToUser( pOwnUser->m_dwConnectionIndex, (char*)&AppearItem, AppearItem.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

		pItemNode = pItemNode->pNext;
	}	
}

//-----------------------------------------------------------------------------
// �������� ���ǹ�ȣ�� ���� �����͸� �����Ѵ�. 
//-----------------------------------------------------------------------------
CSection* CDungeonLayer::GetSection(int nSectionNum) const
{	
	if(nSectionNum <= 0 || nSectionNum > this->m_wTotalSectionNum )	
	{
		return NULL;	
	}

	return m_pSection[ nSectionNum - 1 ];
}


//-----------------------------------------------------------------------------
// �������� ������ �������� Ȱ��ȭ �Ǿ��ִ� ���� List�� �߰��Ѵ�. �������� Ȱ��ȭ �Ǿ��ִ� ���� List�� �߰��Ѵ�.
//-----------------------------------------------------------------------------
BOOL CDungeonLayer::AddActiveSection(int nSectionNum)
{
	if(nSectionNum < 1 || nSectionNum > MAX_SECTION_NUM_PER_LAYER)
	{
		__asm int 3
		return FALSE;
	}
	
	CSection* pSection = GetSection( nSectionNum );
	if( !pSection )
	{
		__asm int 3
		return FALSE;
	}

	pSection->m_pActiveListPos = m_pActiveSectionList->AddTail( pSection );

	if(!pSection->m_pActiveListPos)
	{
		__asm int 3
		return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// �������� ������ �������� Ȱ��ȭ �Ǿ��ִ� ���� List�� �߰��Ѵ�. �������� Ȱ��ȭ �Ǿ��ִ� ���� List�� �߰��Ѵ�.
//-----------------------------------------------------------------------------
BOOL CDungeonLayer::AddActiveSection(CSection* pSection)
{
	if(!pSection)	return FALSE;

	pSection->m_pActiveListPos = m_pActiveSectionList->AddTail( pSection );

	if(!pSection->m_pActiveListPos)
	{
		__asm int 3
		return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// �������� ������ �������� Ȱ��ȭ �Ǿ��ִ� ���� List���� ���� ����. �������� Ȱ��ȭ �Ǿ��ִ� ���� List���� ���� ����.
//-----------------------------------------------------------------------------
BOOL CDungeonLayer::RemoveActiveSection(int nSectionNum)
{
	if(nSectionNum < 1 || nSectionNum > MAX_SECTION_NUM_PER_LAYER)
	{
		__asm int 3
		return FALSE;
	}
	
	CSection*	pSection = GetSection( nSectionNum );

	if( !pSection )
	{
		__asm int 3
		return FALSE;
	}

	m_pActiveSectionList->RemoveAt( pSection->m_pActiveListPos );
	pSection->m_pActiveListPos = NULL;
	
	return TRUE;
}


//-----------------------------------------------------------------------------
// �������� ������ �������� Ȱ��ȭ �Ǿ��ִ� ���� List���� ���� ����. �������� Ȱ��ȭ �Ǿ��ִ� ���� List���� ���� ����.
//-----------------------------------------------------------------------------
BOOL CDungeonLayer::RemoveActiveSection(CSection* pSection)
{
	if(!pSection)	return FALSE;

	m_pActiveSectionList->RemoveAt( pSection->m_pActiveListPos );
	pSection->m_pActiveListPos = NULL;

	return TRUE;
}


//-----------------------------------------------------------------------------
// ���� ������ ������ġ�� �̵���Ŵ. ���� ������ ������ġ�� �̵���Ŵ
//-----------------------------------------------------------------------------
BOOL CDungeonLayer::MoveSection(CUser* pUser , int nFrom, int nTo)
{
	if(nFrom < 1 || nFrom > MAX_SECTION_NUM_PER_LAYER || nTo < 1 || nTo > MAX_SECTION_NUM_PER_LAYER)
	{
		__asm int 3
		return FALSE;
	}

	CSection* pSection = GetSection(nFrom);
	if( !pSection ) return FALSE;
	
	RemoveActiveSection( pSection );

	for(int k=0; k<MAX_SECTION_LINK_NUM; k++)	//Link �Ǿ��ִ� ���ǵ� ��� Disable ��Ű�� 
	{
		if(pSection->m_wLink[k] && GetSection( pSection->m_wLink[k] )->m_pActiveListPos )
			RemoveActiveSection( pSection->m_wLink[ k ] );
	}

	pSection->m_pPcList->RemoveAt( pUser->m_pPrevSectionNode );	//Section�� �ִ� PCList���� ���� ����
	pUser->m_pPrevSectionNode = NULL;
		
	//���ο� �������� Activate
	pSection = GetSection( nTo );
	if( !pSection ) return FALSE;

	AddActiveSection( pSection );
	
	for(k=0; k<MAX_SECTION_LINK_NUM; k++)	//��ũ�� �ʵ� ��� Activate
	{
		if(pSection->m_wLink[ k ])
			AddActiveSection( pSection->m_wLink[k] );
	}

	pUser->m_pPrevSectionNode = pSection->m_pPcList->AddTail( pUser );

	return TRUE;
}


//-----------------------------------------------------------------------------
// ���� ������ ������ġ�� �̵���Ŵ. ���� ������ ������ġ�� �̵���Ŵ
//-----------------------------------------------------------------------------
BOOL CDungeonLayer::MoveSection(CMonster* pMonster , int nFrom, int nTo)
{
	if(nFrom < 1 || nFrom > MAX_SECTION_NUM_PER_LAYER || nTo < 1 || nTo > MAX_SECTION_NUM_PER_LAYER)
	{
		__asm int 3
		return FALSE;
	}

	CSection* pSection = GetSection( nFrom );
	RemoveActiveSection( pSection );

	for(int k=0; k<MAX_SECTION_LINK_NUM; k++)	//Link �Ǿ��ִ� ���ǵ� ��� Disable ��Ű�� 
	{
		if(pSection->m_wLink[ k ] && GetSection(pSection->m_wLink[ k ])->m_pActiveListPos)
			RemoveActiveSection( pSection->m_wLink[ k ]);
	}

	pMonster->RemoveCurSection();
		
	//���ο� �������� Activate
	pSection = GetSection( nTo );
	AddActiveSection( nTo );

	for(k=0; k<MAX_SECTION_LINK_NUM; k++)	//��ũ�� �ʵ� ��� Activate
	{
		if(pSection->m_wLink[ k ])
			AddActiveSection(pSection->m_wLink[k]);
	}

	pMonster->SetCurSection(pSection);
	
	return TRUE;
}


//-----------------------------------------------------------------------------
// �κ��� ������ �Ÿ��� ���� 
//-----------------------------------------------------------------------------
double CDungeonLayer::GetDistance(VECTOR2* vFrom, VECTOR2* vTo)
{
	return sqrt( (vTo->x - vFrom->x) * (vTo->x - vFrom->x) + (vTo->y - vFrom->y) * (vTo->y - vFrom->y));
}


//-----------------------------------------------------------------------------
// ���� ���� Ÿ���� ������. 
//-----------------------------------------------------------------------------
BYTE CDungeonLayer::GetDungeonType()
{
	return m_pParent->GetDungeonKind();
}

BOOL CDungeonLayer::InsertUser( CUser* pUser, const VECTOR2* pv2Start )
{
	if( !pUser ) return FALSE;

	// ���� ���̾�� ���ǿ� ������ �߰��ϰ� �ش缽�ǰ� ��ũ�� ������ Ȱ��ȭ ��Ų��.
	pUser->SetLayerIndex(m_bLayer);
	pUser->m_pLayerUserListNode = m_pPcList->AddTail( pUser );	// ���̾ ���� �߰�.
	pUser->SetCurPosition(pv2Start);
	
	// ���� ���ǿ� ����� ���� �ߴٸ�..
	if( !InsertUserSection( pUser ) )						
	{
		m_pPcList->RemoveAt( pUser->m_pLayerUserListNode );
		pUser->m_pLayerUserListNode = NULL;

		if (pUser->GetCurMapTile())
		{
			m_pMap->SetTileOccupied( pUser->GetCurMapTile()->wIndex_X, pUser->GetCurMapTile()->wIndex_Z, TILE_EMPTY, NULL );
		}
		
		return FALSE;
	}	
	
	++m_wTotalUserCount;
	
	if(!pUser->GetHP())
	{
		pUser->SetHP(1);
	}

	Event_InsertDungeonLayerUser( pUser, this );	

	return TRUE;
}

	
// Layer���� ������ �����.
BOOL CDungeonLayer::RemoveUser( CUser* pUser )
{
	Event_RemoveDungeonLayerUser( pUser, this );	
		
	// ��� ����� ���͸� �����.
	m_pParent->RemoveAllUserMonster( pUser );		

	if (pUser->GetCurMapTile())
	{
		m_pMap->SetTileOccupied( pUser->GetCurMapTile()->wIndex_X, pUser->GetCurMapTile()->wIndex_Z, TILE_EMPTY, pUser );
	}

	RemoveUserSection( pUser );

	try
	{
		m_pPcList->RemoveAt( pUser->m_pLayerUserListNode );
	}
	catch(...)
	{
		Log(LOG_IMPORTANT, "Exception from RemoveUser, RemoveAt");
	}

	pUser->m_pLayerUserListNode = NULL;
	pUser->SetLayerIndex(INVALID_DUNGEON_LAYER);

	--m_wTotalUserCount;
	
	if (m_bMatching)
	{
		if (m_bReadyMatchNum > 0)
		{
			m_bReadyMatchNum --;
		}
	}	

	return TRUE;
}


// �ش� ���ǿ� ������ �߰��Ѵ�.
BOOL CDungeonLayer::InsertUserSection( CUser* pUser, WORD wSectionNum )
{
	if( !pUser ) return false;
	if( wSectionNum > m_wTotalSectionNum ) return false;
	
	CSection* pSection = GetSection( wSectionNum );
	if( !pSection ) return false;
	
	pUser->m_pPrevSectionNode = pSection->m_pPcList->AddTail( pUser );

	// �̰� ���κ��̽��� �ƴѰ��� ����.
	pUser->SetPrevSectionNum(pSection->m_bSectionNum);	
	
	if( !pSection->m_pActiveListPos )	// Ȱ��ȭ�� �ƴϸ� Ȱ��ȭ�� �߰�.
	{
		pSection->m_pActiveListPos = m_pActiveSectionList->AddTail( pSection );
	}
	
	// ���� �������� ����.
	CSection* pActiveSection = 0;

	for( int i=0; (pSection->m_wLink[i] != 0); i++ )
	{
		pActiveSection = GetSection(pSection->m_wLink[i]);
		if( !pActiveSection ) continue;

		if( !pActiveSection->m_pActiveListPos ) // ��ũ�Ȱ��� Ȱ��ȭ�� �ƴ϶��
		{
			pActiveSection->m_pActiveListPos = m_pActiveSectionList->AddTail( pActiveSection );
		}
	}

	return true;
}


// �ɸ����� ������ ��ǥ�� ������ ������ ���ǿ� �߰��Ѵ�.
BOOL CDungeonLayer::InsertUserSection( CUser* pUser )
{
	if( !pUser ) return false;
	
	int		i=0;
	short	wSectionNum = 0;

	const MAP_TILE*	const pTile = pUser->GetCurMapTile();
	if( !pTile || pTile->wAttr.uSection == 0 ) return false;
	
	CSection* pActiveSection = 0;
	wSectionNum	= pTile->wAttr.uSection;

	CSection* pSection = GetSection( wSectionNum );
	if( !pSection ) return false;

	// ������ ������ ã������ ���ǿ� �߰��Ѵ�.
	pUser->m_pPrevSectionNode = pSection->m_pPcList->AddTail( pUser );

	// �̰� ���κ��̽��� �ƴѰ��� ����.
	pUser->SetPrevSectionNum(pSection->m_bSectionNum);				
	
	// Ȱ��ȭ�� �ƴϸ� Ȱ��ȭ�� �߰�.
	if( !pSection->m_pActiveListPos )							
	{
		pSection->m_pActiveListPos = m_pActiveSectionList->AddTail( pSection );
	}
	
	// ���� �������� ����.
	for( i=0; (pSection->m_wLink[i] != 0); i++ )
	{
		pActiveSection = GetSection( pSection->m_wLink[i] );
		if( !pActiveSection ) continue;

		if( !pActiveSection->m_pActiveListPos ) // ��ũ�Ȱ��� Ȱ��ȭ�� �ƴ϶��
		{
			pActiveSection->m_pActiveListPos = m_pActiveSectionList->AddTail( pActiveSection );
		}
	}	
		
	return true;
}


// �ش� ���ǿ��� ������ �����Ѵ�.
BOOL CDungeonLayer::RemoveUserSection( CUser* pUser )
{
	if( !pUser ) return false;
	if( pUser->GetPrevSectionNum() == INVALID_DUNGEON_SECTION ) return false;
	
	CSection* pSection = GetSection( pUser->GetPrevSectionNum() );
	if( !pSection ) return false;
	
	// �ϴ� �ش� ���ǿ��� ������ �����..
	pSection->m_pPcList->RemoveAt( pUser->m_pPrevSectionNode );
	pUser->m_pPrevSectionNode = NULL;
	if( !pSection->m_pPcList->IsEmpty() ) return true;	// �ٸ� ������ �ִٸ� ���� �ʿ����..
	
	BOOL		bFlag = TRUE;
	CSection*	pLinkSection=0;
	CSection*	pLinkLink=0;
	int			i=0, j=0;

	for( i = 0; (pSection->m_wLink[i] != 0); i++ )
	{
		pLinkSection = GetSection( pSection->m_wLink[i] );	// ��ũ ������ ������.
		if( !pLinkSection ) continue;

		if( !pLinkSection->m_pPcList->IsEmpty() )
		{
			bFlag = FALSE;	// ��ũ�� �����߿� �Ѱ��̶� ������ �ִٸ� FALSE ����.
		}
		else	//����ٸ�..
		{
			m_pActiveSectionList->RemoveAt( pLinkSection->m_pActiveListPos );
			pLinkSection->m_pActiveListPos = NULL;	// �ϴ� ��Ȱ��ȭ �س���..
			for( j=0; (pLinkSection->m_wLink[j] != 0); j++ )
			{
				pLinkLink = GetSection(pLinkSection->m_wLink[j]);
				if( !pLinkLink ) continue;

				if( !pLinkLink->m_pPcList->IsEmpty() )	// ��ũ������ ��ũ���ǿ� ����� �ִٸ�.
				{
					pLinkSection->m_pActiveListPos = m_pActiveSectionList->AddTail( pLinkSection );
					break;
				}
			}
		}		
	}

	if( bFlag )	// ��ũ�� ���ǵ鵵 �� ����ٸ�.
	{
		m_pActiveSectionList->RemoveAt( pSection->m_pActiveListPos );
		pSection->m_pActiveListPos = NULL;
	}
	
	pUser->SetPrevSectionNum(INVALID_DUNGEON_SECTION);

	return true;
}

BOOL CDungeonLayer::InsertMonster( CMonster* pMonster, const VECTOR2* v2Start )
{
	if( !pMonster ) return false;

	CMonster* pHashMonster = g_pMonsterTable->GetMonster(pMonster->GetID());

	if(!pHashMonster)
		Log(LOG_JUST_FILEWRITE, "g_MonsterTable = NULL");
	
	pMonster->SetCurDungeon(m_pParent);
	pMonster->SetLayerIndex(m_bLayer);
	pMonster->SetCurDungeonLayer(this);
	pMonster->SetCurPosition(v2Start);
	pMonster->SetPrevSectionNum(0);
	
	// �˸��� ������ ã���� ���ǿ��� ����.
	DWORD dwCurX = (DWORD)(pMonster->GetCurPosition()->x / TILE_WIDTH);
	DWORD dwCurZ = (DWORD)(pMonster->GetCurPosition()->y / TILE_HEIGHT);
	
	for( int i=0; i<m_wTotalSectionNum; i++ )
	{
		if( (dwCurX >= m_pSection[i]->x1) && (dwCurX <= m_pSection[i]->x2 ) && (dwCurZ >= m_pSection[i]->y1) && (dwCurZ <= m_pSection[i]->y2 ) )
		{
			pMonster->SetPrevSectionNum(m_pSection[i]->m_bSectionNum);
			break;
		}
	}
			
	// �ش� ���ǿ� ����.
	if( !InsertMonsterSection( pMonster, pMonster->GetPrevSectionNum() ) )
	{
		return false;
	}

	pMonster->CreateResource();

	return true;
}

BOOL CDungeonLayer::InsertMonsterSection( CMonster* pMonster, WORD wSectionNum )
{
	if( !pMonster ) return false;
	if( wSectionNum > m_wTotalSectionNum ) return false;
	
	CSection* pSection = GetSection( wSectionNum );
	if( !pSection ) return false;

	pMonster->SetCurSection(pSection);
	
	return true;
}

BOOL CDungeonLayer::RemoveMonsterSection( CMonster* pMonster )
{
	if( !pMonster ) 
		return false;

	pMonster->RemoveCurSection();
	
	return true;
}


BOOL CDungeonLayer::InsertItem( ITEM_SERVER* pItem )
{
	if( !pItem )
		return true;

	if( pItem->dwSectionNum == 0 )
		return true;
	
	if( m_pItemHash->Insert( pItem, pItem->Item.m_Serial.i64Serial ) == -1 )
		return true;

	if( m_pSection[pItem->dwSectionNum-1]->m_pItemHash->Insert( pItem, pItem->Item.m_Serial.i64Serial ) == -1)
		return true;
	
	return true;
}

BOOL CDungeonLayer::RemoveItem( ITEM_SERVER* pItem )
{
	if( !pItem ) return false;
	
	m_pItemHash->Delete( pItem, pItem->Item.m_Serial.i64Serial );
	m_pSection[pItem->dwSectionNum-1]->m_pItemHash->Delete( pItem, pItem->Item.m_Serial.i64Serial );
	
	LALFree( g_pItemPool, (void*)pItem );

	return true;
}

void CDungeonLayer::__ProcessDeadMonsterClear()
{
	POSITION_ Del = 0;
	POSITION_ MonsterPos = m_pDeadMonsterList->GetHeadPosition();

	while(MonsterPos)
	{
		try
		{
			Del = MonsterPos;
			CMonster* pMonster = (CMonster*)m_pDeadMonsterList->GetNext(MonsterPos);
			RemoveMonster(pMonster);
			m_pDeadMonsterList->RemoveAt(Del);
		}
		catch(...)
		{
			m_pDeadMonsterList->RemoveAt(Del);
			Log(LOG_IMPORTANT, "Exception from __ProcessDeadMonsterClear");
		}
	}
}

void CDungeonLayer::__ProcessUpdateMonster()
{
	POSITION_ SectionPos = m_pActiveSectionList->GetHeadPosition();
	while( SectionPos )
	{
		CSection* pSection = (CSection*)m_pActiveSectionList->GetNext( SectionPos );

		POSITION_ MonsterPos = pSection->m_pMonsterList->GetHeadPosition();
		while( MonsterPos )
		{
			CMonster* pMonster = (CMonster*)pSection->m_pMonsterList->GetNext( MonsterPos );			
			if( pMonster->GetUnitStatus() != UNIT_STATUS_DEAD )
			{
				try
				{
					pMonster->Update();
				}
				catch(...)
				{
					Log(LOG_IMPORTANT, "Exception from CMonster::Update");
					Log(LOG_IMPORTANT, "Monster will be dead!!");
					pMonster->SetStatus(UNIT_STATUS_DEAD);
					UpdateMonsterForAI(pMonster);
				}
			}
		}	
	}
}

void CDungeonLayer::__ProcessRespawnMonster()
{
	POSITION_ MonsterPos = m_pResponseMonsterList->GetHeadPosition();

	while( MonsterPos )
	{
		CMonster* pMonster = (CMonster*)m_pResponseMonsterList->GetNext( MonsterPos );
		ReviveMonster(pMonster);
	}
}

void CDungeonLayer::__ProcessCreateItem()
{
	POSITION_ ItemPos = m_pCreateItemList->GetHeadPosition();
	
	while(ItemPos)
	{
		ITEM_CREATE* pCreateItem = (ITEM_CREATE*)m_pCreateItemList->GetNext( ItemPos );
		if(!pCreateItem)
			continue;
		
		if( g_dwTickCount - pCreateItem->dwCreateTick <= 1000 )
			break;

		// �������� �����ؾ��Ѵ�.
		DSTC_APPEAR_ITEM AppearItem;
		AppearItem.bType	= 1;
		AppearItem.v2ItemPos= pCreateItem->v2ItemPos;

		memcpy( &AppearItem.Item, &pCreateItem->Item, sizeof(CItem) );
		
		ITEM_SERVER* pItem = (ITEM_SERVER*)LALAlloc( g_pItemPool );

		MakeItemStruct( pItem, &pCreateItem->Item, &pCreateItem->v2ItemPos, pCreateItem->dwSectionNum, pCreateItem->dwOwnerIndex, pCreateItem->dwPartyIndex, 0, 0 );

		m_pCreateItemList->RemoveAt( pCreateItem->DelPos );	// ���������� �����.
		LALFree( g_pCreateItemPool, (void*)pCreateItem );
		
		if( InsertItem( pItem ) == false ) 
		{
			Log(LOG_ALL, "incorrect item was drop : %d", pItem->Item.GetID());
			continue;//���� �ٿ� ����: hwoarang 050202 
		}

		BroadCastSectionMsg( (char*)&AppearItem, AppearItem.GetPacketSize(), (BYTE)pItem->dwSectionNum );
	}
}

void CDungeonLayer::__ProcessDeleteItem()
{
	// 10Sec
	if( g_dwTickCount - m_dwLastFrameSec > 10000 )	
	{
		//������ �����
		m_dwLastFrameSec = g_dwTickCount;

		ITEM_SERVER*				pItem=0;
		ListNode<ITEM_SERVER>*		pItemNode=0;
		DSTC_DISAPPEAR_ITEM			DisAppearItem;
		pItemNode = m_pItemHash->GetHead();

		while( pItemNode )
		{
			pItem		= pItemNode->pData;
			pItemNode	= pItemNode->pNext;

			pItem->dwCurFramePerSec += 10;

			if(pItem->dwCurFramePerSec>=70)
			{
				// ���� ����� �������̶��.
				if( ( pItem->Item.m_wItemID / ITEM_DISTRIBUTE ) == 19 )			
				{
					// ���� �����Ǿ��� ������̶�� ��񿡼��� �����.
					if( GUARDIAN_CREATE_STATUS_ALREADYCALL == pItem->Item.m_Item_Guardian.bCreate )	
					{
						char szSQL[255] = {0,};
						wsprintf( szSQL, "delete from GuardianInfo where ID = %d", pItem->Item.m_Item_Guardian.dwID );
						g_pDb->THExecuteSQL( szSQL, FALSE, 0, NULL, GAME_DB );
					}
				}

				RemoveItem( pItem );
				DisAppearItem.i64ItemID = pItem->Item.m_Serial.i64Serial;
				BroadCastSectionMsg( (char*)&DisAppearItem, DisAppearItem.GetPacketSize(), (BYTE)pItem->dwSectionNum );
			}
		}
	}
}

void CDungeonLayer::__ProcessUpdateItemOwner()
{
	// ������ ������ ������ üũ �� 5�ʰ� �����ٸ�
	if( g_dwTickCount - m_dwLastOwnerFrameSec > 5000 )	
	{
		// ������ ������ ������ üũ �ð��� ����
		m_dwLastOwnerFrameSec = g_dwTickCount;

		ITEM_SERVER*				pItem=0;
		ListNode<ITEM_SERVER>*		pItemNode=0;
		pItemNode = m_pItemHash->GetHead();

		// ��� �����ۿ� ����
		while( pItemNode )
		{
			pItem = pItemNode->pData;
			pItemNode = pItemNode->pNext;
			
			// ������ ���� �ð��� ���� �ȳ�������
			if( pItem->dwOwnerFramePerSec < MAX_ITEM_OWNER_FRAME )
			{
				// ������ ���� �ð��� 5�ʰ� �ø�
				pItem->dwOwnerFramePerSec += 5;
			}
			// �ð��� ������ ������ Ȥ�� ������Ƽ�� �ִ� �������̸�
			else if( pItem->dwOwnerIndex != 0 || pItem->dwPartyIndex != 0)
			{
				// �����ڿ� ������Ƽ�� �����ش�
				pItem->dwOwnerIndex = 0;
				pItem->dwPartyIndex = 0;
			}
		}
	}
}

void CDungeonLayer::__ProcessSiege()
{
	if (m_pParent->GetDungeonDataEx()->m_bSiege)
	{
		// �������϶��� �˻��ؾ� �Ѵ�.
		DSTC_DESTROY_CP packet;
		CVoidList* pCPList = m_pMap->m_pCPList;
		POSITION_ pos = pCPList->GetHeadPosition();

		while(pos)
		{
			// cp�� �˻��Ѵ�.
			CP_DESC* pCPDesc = (CP_DESC*)pCPList->GetNext(pos);

			if (!pCPDesc->bDestroy)
			{	
				if (pCPDesc->pTile->wAttr.uOccupied != TILE_OCCUPIED_PC)
				{
					//�����ؾ� �ϹǷ�.
					pCPDesc->dwStartTick = 0;
				}
				else
				{
					// cp�� user�� ���ֱ�.
					CUser* pCP_User = (CUser*)pCPDesc->pTile->pData;
					if (pCP_User->GetAttackMode() == ATTACK_MODE_DEFENCE)
						continue;// ����ڴ� Ǯ�� ���Ѵ�.
					
					if (!pCPDesc->dwStartTick)	
					{
						pCPDesc->dwStartTick = g_dwTickCount;
						continue; // ���� ������ �ɱ�?
					}
					
					if (g_dwTickCount - pCPDesc->dwStartTick 
						>= m_pParent->GetDungeonDataEx()->GetCPTime())
					{
						// cp �ı��ض�.
						pCPDesc->dwStartTick = 0;
						pCPDesc->bDestroy = 1;
						GetMap()->m_bCPRemainCount--;
						
						// ������ ���� ������ cp�� ���� ���� �ߴٸ�.��ȣ������� ��ȯ�ض�.
						if (m_pParent->GetTotalLayer()-1 == pCP_User->GetCurLayerIndex()
							&& !GetMap()->m_bCPRemainCount)
						{
							if (m_pParent->GetDungeonDataEx()->m_byCallGuardian)
							{
								m_pParent->CreateDungeonGuardian();
							}
							else
							{
								m_pParent->SetDungeonOwner(pCP_User->GetID()
									, pCP_User->m_dwGuildId
									, pCP_User->m_dwPartyId
									, pCP_User->m_szId
									, pCP_User->GetName() );
							}
						}						
						
						packet.sCP_Info.bCPID		= pCPDesc->bID;
						packet.sCP_Info.bDestroy	= 1;
						packet.sCP_Info.wProperty	= pCPDesc->wProperty;
						
						BroadCastMsg( (char*)&packet, packet.GetPacketSize() );

						for(int i = 0; i < 5; ++i)
						{
							if (g_pCPTable[pCPDesc->wProperty].sCPTable_Value[i].wID 
									&& g_pCPTable[pCPDesc->wProperty].sCPTable_Value[i].wID < 10000)
							{
								// skill �����ؾ��Ѵ�.
								VECTOR2 vecStartPosition = {pCPDesc->pTile->x, pCPDesc->pTile->z};
								WORD wMinMax[2] = {0, 0};

								Effect* pEffect = g_pEffectLayer->GetEffectInfo((BYTE)g_pCPTable[pCPDesc->wProperty].sCPTable_Value[i].wID);

								if (pEffect->bFormula == 2	||
									pEffect->bFormula == 20 ||
									pEffect->bFormula == 21 ||
									pEffect->bFormula == 22)
								{
									wMinMax[0] = 100;
									wMinMax[1] = 300;
								}

								SystemSkillToUser(this
									, (BYTE)g_pCPTable[pCPDesc->wProperty].sCPTable_Value[i].wID
									, (BYTE)g_pCPTable[pCPDesc->wProperty].sCPTable_Value[i].wValue
									, g_pCPTable[pCPDesc->wProperty].wApply_Time*1000
									, &vecStartPosition
									, wMinMax
									, OBJECT_TYPE_SKILL
									, 0
									, pCP_User
								);
							}
						}
					}				
				}		
			}
		}
	}
}

void CDungeonLayer::__ProcessMatch()
{
	if (m_bMatching)
	{
		if (m_dwMatchBeginTime && g_dwTickCount >= (m_dwMatchBeginTime+MATCH_START_TERM_TICK) )
		{ 
			m_dwMatchBeginTime = 0;
			
			DSTC_START_MATCH_MAN2MAN packet;
			m_bStartMatch = TRUE;

			POSITION_ pos = m_pPcList->GetHeadPosition();

			while(pos)
			{
				CUser * pUserTemp = (CUser*) m_pPcList->GetNext(pos);
								
				if (pUserTemp && pUserTemp->m_sMatchDescInfo.m_bMatching == TRUE)
				{  
					// ���� ī��Ʈ
					MatchRecordCount(pUserTemp);

					CUser * pUser = g_pUserHash->GetData(pUserTemp->m_sMatchDescInfo.m_dwMatchUserIndex);

					packet.dwMatchUserIndex = (pUser) ? pUser->GetID() : 0;// ��������� �ε����� �־�д�.

					// ���� �ð�
					packet.dwMatchTimeTick = MATCH_TIME;

					NetSendToUser(pUserTemp->m_dwConnectionIndex, (char *)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);				
				}
			}

			m_dwMatchEndTime = 0;
			m_dwEndMatchJudgmentTick = g_dwTickCount + MATCH_TIME;
		}		
		else if (m_dwEndMatchJudgmentTick && g_dwTickCount >= m_dwEndMatchJudgmentTick /* m_dwMatchEndTime */ )
		{ 
			// Time Out
			m_dwEndMatchJudgmentTick = 0;

			DSTC_END_MATCH_MAN2MAN	packet;
			float	fRemainHPRate[2]	= {0.f, };	// �����ִ� HP �ۼ�Ƽ��	
			CUser * pMatchUser[2]		= {NULL, };
			packet.bJudgmentCode		= 1;		// Time over

			int iMatchUser = 0;
			POSITION_ pos = m_pPcList->GetHeadPosition();

			// �����ϱ����ؼ� ������ ���� ����Ѵ�.
			while(pos)
			{
				CUser * pUserTemp = (CUser*) m_pPcList->GetNext(pos);

				if (pUserTemp->m_sMatchDescInfo.m_bMatching == TRUE)
				{
					pMatchUser[iMatchUser] = pUserTemp;
					fRemainHPRate[iMatchUser] = 
						(WORD)pMatchUser[iMatchUser]->GetHP() / (WORD)pMatchUser[iMatchUser]->GetMaxHP() * 100.f;	// ������ �ۼ�Ƽ�� 

					iMatchUser++;
				}
			}

			if (!pMatchUser[0] || !pMatchUser[1])
			{
				// ���߿� �ƹ��� ������ ������ ���� ������ �������ʴ´�.
				m_dwMatchEndTime = g_dwTickCount + MATCH_JUDGMENT_TERM_TICK;
			}
			else
			{ 
				memset(packet.szWinnerName, 0, MAX_CHARACTER_REAL_LENGTH);
				memset(packet.szLoserName, 0, MAX_CHARACTER_REAL_LENGTH); 

				// ����
				if (fRemainHPRate[0] > fRemainHPRate[1])
				{ 
					__lstrcpyn(packet.szWinnerName, (LPSTR)pMatchUser[0]->GetName(), MAX_CHARACTER_REAL_LENGTH);
					__lstrcpyn(packet.szLoserName, (LPSTR)pMatchUser[1]->GetName(), MAX_CHARACTER_REAL_LENGTH); 

					// �¸� ī��Ʈ 
					MatchWinCount(pMatchUser[0]);

					// �й� ī��Ʈ
					MatchLoseCount(pMatchUser[1]);
				}
				else if (fRemainHPRate[0] < fRemainHPRate[1]) 
				{ 
					__lstrcpyn(packet.szWinnerName, (LPSTR)pMatchUser[1]->GetName(), MAX_CHARACTER_REAL_LENGTH);
					__lstrcpyn(packet.szLoserName, (LPSTR)pMatchUser[0]->GetName(), MAX_CHARACTER_REAL_LENGTH); 

					// �¸� ī��Ʈ 
					MatchWinCount(pMatchUser[1]);

					// �й� ī��Ʈ
					MatchLoseCount(pMatchUser[0]);
				}
				else
				{ // ���º�
					packet.bJudgmentCode	= 2;	// ���º� 
					__lstrcpyn(packet.szWinnerName, (LPSTR)pMatchUser[0]->GetName(), MAX_CHARACTER_REAL_LENGTH);
					__lstrcpyn(packet.szLoserName, (LPSTR)pMatchUser[1]->GetName(), MAX_CHARACTER_REAL_LENGTH); 
				}
				
				if (pMatchUser[0])
					pMatchUser[0]->GetCurDungeon()->GetDungeonLayer(0)->BroadCastMsg((char *)&packet, packet.GetPacketSize());
				else if (pMatchUser[1])
					pMatchUser[1]->GetCurDungeon()->GetDungeonLayer(0)->BroadCastMsg((char *)&packet, packet.GetPacketSize());			
				
				BroadCastMsg((char*)&packet, packet.GetPacketSize());
				
				m_dwMatchEndTime = g_dwTickCount + MATCH_JUDGMENT_TERM_TICK; // 5�� �� ������ �����϶�
			}
		}
		else if ( m_dwMatchEndTime && g_dwTickCount >= m_dwMatchEndTime )
		{
			m_dwMatchEndTime = 0;
			CUser * pMatchUser[2]	= {0,};
			int		iMatchUser		= 0;

			POSITION_ pos = m_pPcList->GetHeadPosition();

			while(pos)
			{
				CUser * pUserTemp = (CUser*) m_pPcList->GetNext(pos);
				if ( pUserTemp )
				{	
					if (pUserTemp->m_sMatchDescInfo.m_bMatching == TRUE)
					{
						// �������϶� ������ �̸��� �´�. 
						pMatchUser[iMatchUser] = pUserTemp;
						pMatchUser[iMatchUser]->m_sMatchDescInfo.m_bMatching = FALSE;
						pMatchUser[iMatchUser]->m_sMatchDescInfo.m_dwMatchUserIndex = 0;
						
						// ������� ������ ��� ���ش�.
						// ������� ��ų�� ġ���.
						pMatchUser[iMatchUser]->RemoveAllDetachSkill();

#if defined JAPAN_LOCALIZING
// �Ϻ� ���������� ����� �ǻ�Ƴ��� ��ü HP �� 50 % ȸ��
// ( 2004. 09. 12. minjin )
						if ( pMatchUser[iMatchUser]->GetHP() == 0 )
						{
							DSTC_USER_STATUS	UserStatus;
							UserStatus.bStatusMany	= 0;

							// ���� �� HP �� �ݶ� ä���ش�.
							pMatchUser[iMatchUser]->SetStatus(UNIT_STATUS_NORMAL, TRUE);
							pMatchUser[iMatchUser]->SetHP( pMatchUser[iMatchUser]->GetMaxHP() / 2 );

							// HP �ݶ� ä���ش�.(Ŭ���̾�Ʈ�� �˸���.)
							UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HP;
							UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= pMatchUser[iMatchUser]->GetMaxHP() / 2;
							
							NetSendToUser(pMatchUser[iMatchUser]->m_dwConnectionIndex, (char*)&UserStatus,  UserStatus.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
						}
#endif
						pMatchUser[iMatchUser]->SetStatus(UNIT_STATUS_NORMAL, TRUE);
						pMatchUser[iMatchUser]->GetCurDungeon()->ChangeLayerUser(pMatchUser[iMatchUser], pMatchUser[iMatchUser]->GetCurLayerIndex(), 0, 0);

						iMatchUser++;
					}
				}
			}
		}

		if (m_bStartMatch)
		{ 
			// ���� �����ϰ� �ƹ��� ����.
			if (m_pPcList->IsEmpty())
			{
				InitMatch();
			}
		}
	}
}

// ���� ���� ������Ʈ...
void CDungeonLayer::Process()
{
	__ProcessDeadMonsterClear();	// ���� �ѵ��� ���δ�.
	__ProcessUpdateMonster();		// ���� ������Ʈ
	__ProcessRespawnMonster();		// ���� ������
	__ProcessCreateItem();			// ������ ����
	__ProcessDeleteItem();			// ������ ����(�������� ���� �����ϴ� �� ���� �ʳ�? �۽�..)
	__ProcessUpdateItemOwner();		// ������ ������ ������Ʈ
	__ProcessSiege();				// ���� ó��
	__ProcessMatch();				// 1:1 ��� ó��

	__ProcessTeamMatch();// ����ġ : hwoarang 050202 
}


void CDungeonLayer::InitList()
{
	m_pPcList				= new CPcList;
	m_pMonsterList			= new CMonsterList;
	m_pResponseMonsterList	= new CMonsterList;
	m_pCreateItemList		= new CCreateItemList;
	m_pActiveSectionList	= new CActiveSectionList;
	m_pDeadMonsterList		= new CVoidList;

	m_pItemHash				= new ITEM_HASH;
	m_pItemHash->InitializeHashTable( 5000, 5000 );
}


// �̷��̾ ������ �ִ� ���͸� ���� �����. ó���� ���
void CDungeonLayer::CreateMonsterAll()
{
	int					i=0;
	CMonster*			pMonster=0;
	LPBASEMONSTER		pBaseMonster=0;
	VECTOR2				v2Start = {0.f, 0.f};

	for( i = 0; i < MAX_MONSTER_FORMATION && m_pLayerFormation->pMonsterFormation[i].dwMonsterID != 0; i++ )
	{		
		pBaseMonster = g_pBaseMonsterHash->GetData( m_pLayerFormation->pMonsterFormation[i].dwMonsterID );
		if( !pBaseMonster )
			continue;

		if (pBaseMonster->bRace == 0)
			pMonster = g_pMonsterTable->Add(UNIT_CLASS_ID_TRADER_MONSTER);
		else	
			pMonster = g_pMonsterTable->Add(UNIT_CLASS_ID_MONSTER);
		
		pMonster->SetInfoIndex( WORD(i) );
		pMonster->Initialize(pBaseMonster, NULL, NULL, 0, pBaseMonster->wBaseLevel, pBaseMonster->dwExp);
				
		v2Start.x = TILE_X( m_pLayerFormation->pMonsterFormation[i].wX );
		v2Start.y = TILE_Z( m_pLayerFormation->pMonsterFormation[i].wY );

		pMonster->SetDestPosition(&v2Start);
		
		// Insert Monster.
		InsertMonster( pMonster, &v2Start );

		++m_wCurMonster;
		++m_wTotalMonster;						
	}
}


void CDungeonLayer::CreateMonster( CMonster* pMonster )
{	
	pMonster->RemoveResponList();

	if (pMonster->IsNormalMonster())
		++m_wCurMonster;

	VECTOR2 v2Start = 
	{
		TILE_X( m_pLayerFormation->pMonsterFormation[pMonster->GetInfoIndex()].wX ),
		TILE_Z( m_pLayerFormation->pMonsterFormation[pMonster->GetInfoIndex()].wY ) 
	};

	pMonster->SetCurPosition(&v2Start);
	
	pMonster->SetHP(pMonster->GetMaxHP());
	pMonster->SetTemp(NPC_TEMP_CREATETYPE, 0);	// �̳��� ������ �����.	
	pMonster->SetStatus(UNIT_STATUS_NORMAL, FALSE, TRUE);
	pMonster->SetTemp(NPC_TEMP_LASTATTACKTICK, g_dwTickCount);
	pMonster->SetTemp(NPC_TEMP_LASTSTATUSTICK, g_dwTickCount);	

	InsertMonster( pMonster, &v2Start );	
	
	int			iRandX=0, iRandZ=0, iSignX=0, iSignZ=0;
	int         iCnt=0;
	BOOL		bSpot= TRUE;
	MAP_TILE*	pTile = NULL;

	do
	{
		// X, Z �׸��� ��ȣ�� ���Ѵ�.
		iRandX = rand()%8 + 1;
		iRandZ = rand()%8 + 1;
		iSignX = rand()%2;
		iSignZ = rand()%2;
		if( iSignX ) iRandX *= -1;
		if( iSignZ ) iRandZ *= -1;
		
		pTile = m_pMap->GetTile( pMonster->GetCurPosition()->x + ( (float)iRandX * TILE_WIDTH ), 
								pMonster->GetCurPosition()->y + ( (float)iRandZ * TILE_HEIGHT ) );
		
		if( iCnt >= 30 ) 
		{
			bSpot = FALSE;
			break;
		}

		iCnt++;

	} while( ( (pTile == NULL) || ( pTile->wAttr.uSection == 0 ) ) || ( pTile->wAttr.uOccupied == TILE_OCCUPIED_DONT_MOVE) );
	
	if( bSpot )
	{
		v2Start.x = TILE_X( pTile->wIndex_X );
		v2Start.y = TILE_Z( pTile->wIndex_Z );
		pMonster->SetCurPosition(&v2Start);
	}
	
	pMonster->SetDestPosition(pMonster->GetCurPosition());
	
	// ���� ������ �˸���.
	DSTC_APPEAR_MON AppearMon;
	MakeAppearMonser( &AppearMon, pMonster );
	CDungeonLayer *pLayer = pMonster->GetCurDungeonLayer();
	if(pLayer)
	{
		pLayer->BroadCastSectionMsg( (char*)&AppearMon, AppearMon.GetPacketSize(), pMonster->GetPrevSectionNum() );
	}
}


void CDungeonLayer::CallGuardian( CUser* pUser, DWORD dwZipCode, GUARDIAN_INFO* pGuardian )
{
	CMonster* pMonster = g_pMonsterTable->Add(UNIT_CLASS_ID_MONSTER); // <-- ������� �� ������.

	if (!pMonster)
	{
		Log(LOG_IMPORTANT, "CDungeonLayer::CallGuardian MonsterPool(%d)", g_pMonsterTable->GetCount());
		return;
	}
		
	// ������� ����Ÿ�� �����.	
	LPBASEMONSTER pBaseMonster = g_pBaseMonsterHash->GetData( pGuardian->dwKind + (pGuardian->bOldGuardian?80:0) );
	if( !pBaseMonster ) __asm int 3;

	pMonster->SetDestPosition(pUser->GetCurPosition());
	InsertMonster( pMonster, pUser->GetCurPosition() );
	pGuardian->dwExp = min(pGuardian->dwExp, GetExpTableOfLevel(OBJECT_TYPE_MONSTER, 200));
	pMonster->Initialize(pBaseMonster, pGuardian, pUser, (BYTE)dwZipCode
		, GetGuardianItemLevel(pGuardian->dwExp), pGuardian->dwExp);
	
	if (!pUser->IsSummonGuardian((BYTE)dwZipCode))
	{
		pUser->AttachItem(&pUser->m_pInv_Guardian[dwZipCode]);
	}

	// ���� ������ �˸���.
	DSTC_APPEAR_MON	AppearMon;
			MakeAppearMonser( &AppearMon, pMonster );
	CDungeonLayer *pLayer = pMonster->GetCurDungeonLayer();
	if(pLayer)
	{
		pLayer->BroadCastSectionMsg((char*)&AppearMon, AppearMon.GetPacketSize(), pMonster->GetPrevSectionNum() );
	}

	pMonster->SendAllStatus();
}


CMonster* CDungeonLayer::CallDungeonGuardian( GUARDIAN_INFO* pGuardian )
{
	CMonster* pMonster = g_pMonsterTable->Add(UNIT_CLASS_ID_MONSTER);
	if (!pMonster)
	{
		Log(LOG_IMPORTANT, "CDungeonLayer::CallDungeonGuardian MonsterPool(%d)", g_pMonsterTable->GetCount());
		return 0;
	}
		
	// ���� ��ġ�� ���Ѵ�.	 
	if( !m_pMap->m_pGuardianTile ) return NULL;

		
	// ������� ����Ÿ�� �����.
	LPBASEMONSTER pBaseMonster = g_pBaseMonsterHash->GetData( pGuardian->dwKind );
	if( !pBaseMonster ) __asm int 3;

	// CP�ڸ��� ������� ���� ��Ų��.
	VECTOR2 v2Start = 
	{
		TILE_X( m_pMap->m_pGuardianTile->wIndex_X ),
		TILE_Z( m_pMap->m_pGuardianTile->wIndex_Z ) 
	};
	pMonster->SetDestPosition(&v2Start);
	m_pParent->SetDungeonGuardian(pMonster);

	InsertMonster( pMonster, &v2Start );

	pMonster->Initialize(pBaseMonster, pGuardian, NULL, 0, pGuardian->dwLevel, 0);
	pMonster->SetMonsterAI( 5 );

	// ���� ������ �˸���.
	DSTC_APPEAR_MON	AppearMon;
	MakeAppearMonser( &AppearMon, pMonster );
	CDungeonLayer *pLayer = pMonster->GetCurDungeonLayer();
	if(pLayer)
	{
		pLayer->BroadCastSectionMsg((char*)&AppearMon, AppearMon.GetPacketSize(), pMonster->GetPrevSectionNum() );
	}
	
	return pMonster;
}


void CDungeonLayer::ReviveMonster(CMonster* pMonster, DWORD dwFlag /*���� �ִٸ� ������ �ִٴ� �Ҹ���.*/)
{
	// �̺�Ʈ(����)���� 1�� �̺�Ʈ ���� �Ǿ����� ���Ͱ� �����Ǵ°� ���� ���� 
	if(	m_pParent &&
		m_pParent->GetEventDungeonType() == EVENT_DUNGEON_SURVIVAL &&
		m_pParent->GetEventFirstCloseTick() > 0 ||
		m_pParent->GetEventClearTick() > 0)
	{
		return;		
	}

	if(	dwFlag && (pMonster->IsNormalMonster() || pMonster->IsNPC()) &&
		pMonster->GetTemp(NPC_TEMP_KILLTICK) != 0xffffffff && ( g_dwTickCount - pMonster->GetTemp(NPC_TEMP_KILLTICK) ) 
		>= ( (DWORD)m_pLayerFormation->pMonsterFormation[pMonster->GetInfoIndex()].wResponse * 1000 + 10000) ) 
	{
		// �⺻ 5��
		CreateMonster( pMonster );
	}
	else if (!dwFlag)
	{
		// ������ �¾��.
		CreateMonster( pMonster );
	}
}


CMonster* CDungeonLayer::CallMonster( CUser* pUser, DWORD dwZipCode, DWORD dwBaseMonsterNum, const VECTOR2* const v2Start)
{
	LPBASEMONSTER pBaseMonster = g_pBaseMonsterHash->GetData( dwBaseMonsterNum );
	if( !pBaseMonster )	
	{
		Log( LOG_JUST_DISPLAY, "CallMonster, Invalid BaseMonster Index!" );
		return	NULL;
	}

	CMonster* pMonster = g_pMonsterTable->Add(UNIT_CLASS_ID_MONSTER);	
	if (!pMonster)
	{
		Log(LOG_IMPORTANT, "CDungeonLayer::CallMonster MonsterPool(%d)", g_pMonsterTable->GetCount());
		return 0;
	}

	pMonster->SetDestPosition(v2Start);
	InsertMonster( pMonster, v2Start );	
		
	pMonster->Initialize(pBaseMonster, NULL, pUser, (BYTE)dwZipCode, pUser->GetLevel(), 0);
	
	((CUser*)pMonster->GetLord())->m_pMonster[ dwZipCode ]	= pMonster;
	
	// ��ȯ���� �ɷ�ġ�� �������Ѷ�.
	DWORD dwSummonPassiveSkill[] = {__SKILL_MIRACULOUSHEART__, __SKILL_FRENZY__, __SKILL_RAGE__};

	for(int i = 0; i < sizeof(dwSummonPassiveSkill)/4; ++i)
	{
		BYTE bSkillLevel = pUser->GetSkillLevel((BYTE)dwSummonPassiveSkill[i]);

		if (bSkillLevel)
			pMonster->SetStatusFromSkillStatusValue((BYTE)dwSummonPassiveSkill[i], BYTE(bSkillLevel-1), pUser->GetClass(), NULL);
	}	

	// ���� ������ �˸���.
	DSTC_APPEAR_MON AppearMon;
	MakeAppearMonser( &AppearMon, pMonster );
	CDungeonLayer *pLayer = pMonster->GetCurDungeonLayer();
	if(pLayer)
	{
		pLayer->BroadCastSectionMsg( (char*)&AppearMon, AppearMon.GetPacketSize(), pMonster->GetPrevSectionNum() );
	}

	return pMonster;
}

CMonster* CDungeonLayer::CallMonsterByGM( CUser* pUser, DWORD dwZipCode, DWORD dwBaseMonsterNum, DWORD dwItem )
{
	dwZipCode;

	LPBASEMONSTER pBaseMonster = g_pBaseMonsterHash->GetData( dwBaseMonsterNum );
	if(!pBaseMonster)	
	{
		Log( LOG_JUST_DISPLAY, "CallMonster, Invalid BaseMonster Index!" );
		return	NULL;
	}

	CMonster* pMonster = g_pMonsterTable->Add(UNIT_CLASS_ID_MONSTER);
	if(!pMonster)
	{
		Log( LOG_JUST_DISPLAY, "CallMonster, No Free Pool Object!" );
		return NULL;
	}
	
	if (OBJECT_TYPE_GUARDIAN == pBaseMonster->bRace)
	{
		Log( LOG_JUST_DISPLAY, "CallMonster, Don't Call Guardian!" );
		return NULL;
	}

	if (6 == pBaseMonster->bRace)
	{
		// ���� �����̴�.
		Log( LOG_JUST_DISPLAY, "CallMonster, Don't Call Elemental!" );
		return NULL;
	}

	pMonster->SetDestPosition(pUser->GetCurPosition());
	InsertMonster( pMonster, pUser->GetCurPosition() );

	pMonster->SetTemp(NPC_TEMP_CREATETYPE, 1);
	pMonster->SetTemp(NPC_TEMP_ITEM, dwItem);

	pMonster->Initialize(pBaseMonster, NULL, NULL, 0, pUser->GetLevel(), 0);
	
	// �ϴ� ���ΰ��� ��ġ�� ���ļ� ������. �������..
	
	pMonster->SetInfoIndex( 0xffff );		// ��
	
	// ��ȯ���� �ɷ�ġ�� �������Ѷ�.
	DWORD dwSummonPassiveSkill[] = {__SKILL_MIRACULOUSHEART__, __SKILL_ARCANUMSKIN__, __SKILL_FRENZY__};

	for(int i = 0; i < sizeof(dwSummonPassiveSkill)/4; ++i)
	{
		BYTE bSkillLevel = pUser->GetSkillLevel((BYTE)dwSummonPassiveSkill[i]);

		if (bSkillLevel)
			pMonster->SetStatusFromSkillStatusValue((BYTE)dwSummonPassiveSkill[i], BYTE(bSkillLevel-1)
			, pUser->GetClass(), NULL);
	}
		 
	// ���� ������ �˸���.
	DSTC_APPEAR_MON AppearMon;
	MakeAppearMonser( &AppearMon, pMonster );
	CDungeonLayer *pLayer = pMonster->GetCurDungeonLayer();
	if(pLayer)
	{
		pLayer->BroadCastSectionMsg( (char*)&AppearMon, AppearMon.GetPacketSize(), pMonster->GetPrevSectionNum() );
	}

	return pMonster;
}

void CDungeonLayer::InitMoveSpot( DWORD dwMoveSpotNum )
{
	if( dwMoveSpotNum <= 0 ) return;

	if( m_pMoveSpot )
	{
		delete [] m_pMoveSpot;
		m_pMoveSpot = NULL;
	}

	m_wMoveSpotNum = (WORD)dwMoveSpotNum;
	m_pMoveSpot = new Move_Spot[dwMoveSpotNum];
}


// �� ���ǿ� ���Ե� ��Ƽ���鿡�� ���� ������ ������Ѷ�.
void CDungeonLayer::AttachMagicArrayToParty(CUser* pUser, DWORD dwItemID)
{
	CSection* pSection = GetSection( pUser->GetPrevSectionNum() );

	if(!pSection)
	{
		return;	
	}

	// ������ ������ �������� �޽�������.
	POSITION_	pos			= NULL;
	CUser*		pTargetUser	= NULL;

	pos = pSection->m_pPcList->GetHeadPosition();

	while(pos)
	{
		pTargetUser = (CUser*)pSection->m_pPcList->GetNext(pos);
		
		if(pTargetUser->m_dwPartyId && pTargetUser->m_dwPartyId == pUser->m_dwPartyId )
		{
			// ���� ��Ƽ���̱�
			pTargetUser->AttachMagicArray(dwItemID);			
		}			
	}
	
	// ������ ������ ��ũ�� ������ �������� �޽��� ����.
	CSection* pLinkSection = NULL;
	
	for(int i = 0; pSection->m_wLink[i] != 0; i++)
	{
		pLinkSection = GetSection(pSection->m_wLink[i]);

		if( !pLinkSection )
		{
			continue;
		}

		pos = pLinkSection->m_pPcList->GetHeadPosition();

		while(pos)
		{
			pTargetUser = (CUser*)pLinkSection->m_pPcList->GetNext(pos);
			pTargetUser->AttachMagicArray(dwItemID);			
		}
	}
}


// ��ŸƮ Ÿ�����ǿ� ����ִ� Ÿ���� �����Ͷ�.
VECTOR2 CDungeonLayer::GetAroundEmptyTile(WORD wIndex_X, WORD wIndex_Z)
{
	VECTOR2 vecDest = {0.f, 0.f};
	int zz = -1;
	int xx = -1;

	for(;;)
	{		
		for(int z = zz; z < -zz; ++z)
		{
			for(int x = xx; x < -xx; ++x)
			{
				MAP_TILE* pTileTemp = m_pMap->GetMap(wIndex_X+x, wIndex_Z+z);

				if (pTileTemp)
				{
					if (pTileTemp->wAttr.uOccupied == TILE_EMPTY &&  !pTileTemp->pData)
					{
						// ���� ��ġ ������.
						vecDest.x = pTileTemp->x+TILE_SIZE/2;
						vecDest.y = pTileTemp->z+TILE_SIZE/2;
						
						return vecDest;
					}
				}				
			}
		}

		zz-=2;
		xx-=2;
	}
}


BOOL CDungeonLayer::GetStartTile(WORD wStartSpotID, VECTOR2* const pv2Start)
{
//	assert(wStartSpotID < m_wStartSpotNum);

	if( !m_wStartSpotNum )
	{
		pv2Start->x = 0;
		pv2Start->y = 0;
		return FALSE;
	}

	// ���ϴ� ���� ����Ʈ�� ���� �� �̸�
	if( wStartSpotID >= m_wStartSpotNum)
	{
		// �⺻ ������ ����϶�	
		wStartSpotID = 0;
	}

	MAP_TILE* pTile=0;
	for(int x = m_pStartSpot[ wStartSpotID ].left; x <= m_pStartSpot[ wStartSpotID ].right; x++)
	{
		for(int z = m_pStartSpot[ wStartSpotID ].top; z <= m_pStartSpot[ wStartSpotID ].bottom; z++)
		{
			pTile = GetMap()->GetMap(x, z);
			
			if(!pTile) // Ÿ���� ������..
			{
				Log(LOG_IMPORTANT, "Start Spot is invalid range! (x:%d, z:%d)", x, z);
				continue;
			}

			// �� Ÿ���� �ƴϸ�...
			if(pTile->wAttr.uOccupied != TILE_EMPTY || pTile->pData)
				continue;

			pv2Start->x = TILE_X(x);	
			pv2Start->y = TILE_Z(z);

			return TRUE;
		}
	}

	// ��Ÿ���� ã�Ƽ� �ѷ���.
	VECTOR2 vec = GetAroundEmptyTile((WORD)m_pStartSpot[ wStartSpotID ].left,(WORD)m_pStartSpot[ wStartSpotID ].bottom);
	*pv2Start = vec;

	return TRUE;
}


void CDungeonLayer::ResetMonster(BOOL bCreateMonster)
{
	// �켱 ������ ���͵��� ���� �����Ѵ�.	
	POSITION_	MonsterPos	= NULL;	
	CMonster*	pMonster	= NULL;
	
	MonsterPos = m_pMonsterList->GetHeadPosition();

	while( MonsterPos )
	{
		pMonster = (CMonster*)m_pMonsterList->GetNext( MonsterPos );				

		if (pMonster->IsNormalMonster())
		{
			pMonster->SetStatus(UNIT_STATUS_DEAD);
			RemoveMonster(pMonster);
		}		
	}

	m_pDeadMonsterList->RemoveAll();

	// �׸��� �ٽ� ������ ����.
	if(bCreateMonster)
	{
		MonsterPos = m_pResponseMonsterList->GetHeadPosition();
		while( MonsterPos )
		{
			pMonster = (CMonster*)m_pResponseMonsterList->GetNext( MonsterPos );	
			CreateMonster( pMonster );
		}
	}
	
	// �ϴ� ¯������ ���� ���� �ִ� ������ ���� �˾� ����. ��.��	
	m_wCurMonster = 0;
	POSITION_ pos = m_pMonsterList->GetHeadPosition();
	while(pos)
	{
		CMonster* pMonster = (CMonster*)m_pMonsterList->GetNext(pos);
		
		if (pMonster->IsNormalMonster() )
		{
			m_wCurMonster++;
		}
	}	
}


BOOL CDungeonLayer::RemoveMonster( CMonster* pMonster )
{
	if( !pMonster )
		return false;

	pMonster->RemoveResource();
	RemoveMonsterSection( pMonster );
	
	pMonster->RemoveCurDungeonLayer();
		
	if( pMonster->IsNormalMonster() || pMonster->IsNPC() )
	{	
		if( pMonster->IsMindControll() )
		{
			((CUser*)pMonster->GetLord())->m_pMonster[pMonster->GetTemp(NPC_TEMP_SLOT)] = NULL;
			pMonster->AddResponList();
		}
		else if(pMonster->GetLord())
		{
			// ���ϰ� ������ ���忡�� "�� �׾���" �Ѵ�. �Ƹ޸�ŸƮ�� : �ִ��� 2005.1.26
			// ���ϴ� ������ ��ȯ�ϹǷ� ResponList�� ���� ����
            pMonster->GetLord()->SetStaff(NULL);
		}
		else {
			// �� ���� ������ ������ �Ǳ⸦ ��� : �ִ��� 2005.1.26
			pMonster->AddResponList();
		}
	}
	else
	{
		// ������ ����� ������ �����̴�.
		switch(pMonster->IsGuardian())
		{
		case GUARDIAN_TYPE_NORMAL:	// �����ִ� ����� ������� ������ �� Update�Ѵ�.
			{
				UpdateGuardianDB( pMonster );
				CUser* pMasterUser = ((CUser*)pMonster->GetLord());

				if (pMasterUser)
				{
					pMasterUser->SetGuardian((BYTE)pMonster->GetTemp(NPC_TEMP_SLOT), NULL);
					pMasterUser->UnSummonGuardian((BYTE)pMonster->GetTemp(NPC_TEMP_SLOT));
				}
			}
			break;

		case GUARDIAN_TYPE_DUNGEON:
			m_pParent->SetDungeonGuardian(NULL);
			break;
		}

		if (pMonster->IsElemental())
		{
			// ���Ӵ� ����
			if (pMonster->GetTemp(NPC_TEMP_SLOT) < MAX_USER_GUARDIAN)
			{
				((CUser*)pMonster->GetLord())->m_pMonster[pMonster->GetTemp(NPC_TEMP_SLOT)] = NULL;
			}
		}

		DSTC_DISAPPEAR_MON	DisAppear;
		DisAppear.dwMonsterIndex = pMonster->GetID();
		CDungeonLayer *pLayer = pMonster->GetCurDungeonLayer();
		if(pLayer)
		{
			pLayer->BroadCastSectionMsg((char*)&DisAppear, DisAppear.GetPacketSize(), pMonster->GetPrevSectionNum());
		}

		g_pMonsterTable->Remove( pMonster );
	}

	return true;
}


void CDungeonLayer::__ProcessTeamMatch()
{//���� ����� ������ �ð��� ���� �ȴ�
	if( m_pTeamMatch )
	{
		m_pTeamMatch->Process();//���ο��� OnEnd���� ���忡 �����϶�.
	}
}

//����ġ ���ΰ�(�Ʒ� TeamMatch ���� �Լ� ���� : hwoarang 050202
bool CDungeonLayer::IsTeamMatchRun() const 
{
	if( m_pTeamMatch ) 
	{
		return m_pTeamMatch->IsRun();
	}
	return false;
}

bool CDungeonLayer::IsTeamMatchEnd() const//����ġ ������?
{
//	if( m_dwTeamMatchEndTime <= g_dwTickCount)
//	{
//		return true;
//	}
	return false;
}

bool CDungeonLayer::OnDie(CUser* pCaster, CUser* pTarget)//�����
{
	pTarget->OnDie();
	if(m_pTeamMatch)
	{
		return m_pTeamMatch->OnDie( pCaster, pTarget );//������ false �� �����ϸ� ������ �ʴ´�.
	}
	return true;
}

bool CDungeonLayer::PrepareTeamMatch(const FLAG_TYPE match_type, const FLAG_TYPE match_level,  const COUNT_TYPE player_count, const COUNT_TYPE team_count, const GUILD_WAR_TIME match_time, const bool bIsAutoMatch)
{//Prepare������ �޾����Ƿ� ���ð��� �ְ� ������ �̵��� ���ϰ� �Ѵ�.
	if(m_pTeamMatch)
	{
		m_pTeamMatch->Prepare(match_type, match_level, player_count, team_count, match_time, bIsAutoMatch);
	}
	return false;
}

bool CDungeonLayer::IsTeamMatchPlayer(CUser* pUser) const
{

	/*
	if(m_pTeamMatch)
	{
<<<<<<< DungeonLayer.cpp
		return (bool)(G_W_F_PLAYER & m_pTeamMatch->GetTeamMatchFlag( pUser ) ) ;
=======
		return ((G_W_F_PLAYER & m_pTeamMatch->GetTeamMatchFlag( pUser )) == G_W_F_PLAYER) ? true : false;
>>>>>>> 1.18
	}
	*/

	
	// ����� ��û�� ���Ѽ���, 4�� 6��, PM 6:52, ���� �ּ������� �����Ͽ��� �� �κ���.-------------------
	if(m_pTeamMatch) 
	{
		return ((G_W_F_PLAYER & m_pTeamMatch->GetTeamMatchFlag( pUser )) == G_W_F_PLAYER) ? true : false;
	}
	// --------------------------------------------------------------------------------------------------


	return false;//����ġ �� �ƴϸ� �÷��̾� �� ���� ����.
}

bool CDungeonLayer::CanMove()//�̵��� ���� �Ұ���
{
	if( m_pTeamMatch )
	{
		return m_pTeamMatch->CanMove();
	}
	return true;
}