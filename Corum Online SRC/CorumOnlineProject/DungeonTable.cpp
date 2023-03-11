#include "Stdafx.h"
#include "DungeonTable.h"
#include "SpriteManager.h"
#include "corumresource.h"
#include "objectmanager.h"
#include "GameControl.h"
#include "onlylist.h"	// Added by ClassView
#include "WorldProcess.h"
#include "InitGame.h"
#include "CodeFun.h"

CDungeonTable::CDungeonTable(DWORD dwNum)
{
	memset(this, 0, sizeof(CDungeonTable));

	m_dwMaxBucketNum = dwNum;

	m_ppInfoTable = NULL;
	m_ppInfoTable = new DUNGEON_DATA_EX*[ dwNum ];		
	memset(m_ppInfoTable, 0, sizeof(DUNGEON_DATA_EX*)*dwNum);

	for(int i=1; i<=MAX_WORLD_LAND_NUM; i++)
	{
		m_pInfoList[i] = new COnlyList(MAX_DUNGEON_NUM_PER_SERVERSET);	
	}
	
	m_pReceivedDungeonList = new COnlyList(MAX_OCCUPIED_DUNGEON_NUM_PER_SERVERSET);

	// Initialize Event dungeon information.
	m_pEventList = new COnlyList(MAX_EVENT_DUNGEON_NUMBER);		

	// �׷� ����Ʈ ����.
	for(i = 0; i < MAX_BYGROUP; ++i)
	{
		m_pGroupList[i] = new COnlyList(MAX_GROUPLIST);
	}

	m_dwNum = 0;	
}

//New�� �޸� �Ҵ��ؼ� ����..      �Ϲ� ������ ��� 
DUNGEON_DATA_EX* CDungeonTable::AllocDungeonInfo()
{
	DUNGEON_DATA_EX* pDungeon = new DUNGEON_DATA_EX;
	
	return pDungeon;
}

BOOL CDungeonTable::AddDungeonInfo(DUNGEON_DATA_EX* pDungeon)
{
	if(!pDungeon)	return FALSE;

	pDungeon->SetDungeonType(pDungeon->m_dwID);

	//List�� �߰� 
	//if (TRUE == pDungeon->IsEvent())
	if (DUNGEON_TYPE_EVENT == pDungeon->GetDungeonType())
	{
		AddEventDungeon(pDungeon);		
	}
	else
	{
		pDungeon->pListPos = m_pInfoList[ pDungeon->m_wWorldMapID ]->AddTail(pDungeon);
	}	

	DWORD dwIndex = pDungeon->m_dwID % m_dwMaxBucketNum;		
	m_dwNum++;

	if (!m_ppInfoTable[dwIndex])	//m_ppInfoTable[dwIndex] �� ������ ������ ��ȣ ������.
	{
		m_ppInfoTable[dwIndex] = pDungeon;
		pDungeon->pNext = NULL;
		pDungeon->pPrev = NULL;
		return TRUE;
	}

	DUNGEON_DATA_EX* cur = m_ppInfoTable[dwIndex];
	DUNGEON_DATA_EX* prv = NULL;
	while (cur)
	{
		prv = cur;
		cur = cur->pNext;
	}
	
	cur = prv->pNext = pDungeon;
	cur->pPrev = prv;
	cur->pNext = NULL;

	return TRUE;
}

DUNGEON_DATA_EX* CDungeonTable::GetDungeonInfo(DWORD dwID)
{
	DWORD dwIndex = dwID%m_dwMaxBucketNum;

	DUNGEON_DATA_EX* cur = m_ppInfoTable[dwIndex];

	while (cur)
	{
		if (cur->m_dwID == dwID)
		{
			return cur;
		}
		cur = cur->pNext;
	}
	return NULL;
}

void CDungeonTable::RemoveDungeonEffect(DUNGEON_DATA_EX* pDungeon)
{
	// ���� ����Ʈ�� ������.
	if(pDungeon->m_hOwnDungeon )
	{
		DeleteHandleObject( pDungeon->m_hOwnDungeon );
		pDungeon->m_hOwnDungeon = NULL;
	}
	if(pDungeon->m_hGroupDungeon )
	{
		DeleteHandleObject( pDungeon->m_hGroupDungeon );
		pDungeon->m_hGroupDungeon = NULL;
	}
	if(pDungeon->m_hSiegingDungeon )
	{
		DeleteHandleObject( pDungeon->m_hSiegingDungeon );
		pDungeon->m_hSiegingDungeon = NULL;
	}
}

void CDungeonTable::Remove(DUNGEON_DATA_EX* pDungeon)
{
	if(!pDungeon)
	{
		return;
	}

	//if (TRUE == pDungeon->IsEvent())
	if (DUNGEON_TYPE_EVENT == pDungeon->GetDungeonType())
	{
		if (NULL != pDungeon->pListPos)
		{
			m_pEventList->RemoveAt(pDungeon->pListPos);
		}
	}
	else
	{
		if (NULL != pDungeon->pListPos)
		{
			m_pInfoList[ pDungeon->m_wWorldMapID ]->RemoveAt( pDungeon->pListPos );
		}	
	}

	pDungeon->pListPos = NULL;
	
	if(pDungeon->pReceivedDungeonPos)
	{
		m_pReceivedDungeonList->RemoveAt( pDungeon->pReceivedDungeonPos );
	}

	pDungeon->pReceivedDungeonPos = NULL;
	
	RemoveDungeonEffect(pDungeon);
	
	DWORD dwIndex = pDungeon->m_dwID%m_dwMaxBucketNum;

	DUNGEON_DATA_EX* cur = m_ppInfoTable[dwIndex];
	DUNGEON_DATA_EX* next = NULL;
	DUNGEON_DATA_EX* prv = NULL;
	
	while (cur)
	{
		prv = cur->pPrev;
		next = cur->pNext;
		if (cur->m_dwID == pDungeon->m_dwID)
		{
			if (!prv)	// if head
				m_ppInfoTable[dwIndex] = next;	
			else 
				prv->pNext = next;
			
			if (next)
				next->pPrev = prv;

			if(cur)	
				delete cur;
												
			cur = NULL;
			m_dwNum--;
			return;

		}
		cur = cur->pNext;
	}
	return;
}

CDungeonTable::~CDungeonTable()
{	
	RemoveDungeonTable();
	
	for(int i=1; i<=MAX_WORLD_LAND_NUM; i++)
	{
		if(m_pInfoList[i])
		{
			m_pInfoList[i]->RemoveAll();
			delete m_pInfoList[i];
			m_pInfoList[i] = NULL;
		}
	}
	
	if(m_pReceivedDungeonList)
	{
		m_pReceivedDungeonList->RemoveAll();
		delete m_pReceivedDungeonList;
		m_pReceivedDungeonList = NULL;
	}

	if (NULL != m_pEventList)
	{
		m_pEventList->RemoveAll();		
		delete m_pEventList;
		m_pEventList = NULL;
	}

	for(i = 0; i < MAX_BYGROUP; ++i)
	{
		if (m_pGroupList[i])
		{
			m_pGroupList[i]->RemoveAll();
			delete m_pGroupList[i];
			m_pGroupList[i] = NULL;
		}
	}
	
	if (m_ppInfoTable)
	{
		delete [] m_ppInfoTable;
		m_ppInfoTable = NULL;
	}
}

void CDungeonTable::RemoveDungeonTable()
{
	DeleteEventDungeonResource();
	RemoveAllSprGuildMark();

	for(int a = 0; a < MAX_BYGROUP; ++a)
		m_pGroupList[a]->RemoveAll();
	
	DUNGEON_DATA_EX* cur = NULL;
	DUNGEON_DATA_EX* next = NULL;

	for (DWORD i=0; i<m_dwMaxBucketNum; i++)
	{
		cur = m_ppInfoTable[i];
		while (cur)
		{
			next = cur->pNext;

			Remove(cur);// ã�Ƽ� ������ �ϱ� ������ �� ������...   !@re			
			cur = next;			
		}
		
		m_ppInfoTable[i] = NULL;
	}
	
	g_bReceiveEventInfo = FALSE;
}

//
void CDungeonTable::RemoveAllSprGuildMark()
{
	for(int i=1; i<=MAX_WORLD_LAND_NUM; i++)
	{
		if(!m_pInfoList[i])	continue;

		POSITION_ pos = m_pInfoList[i]->GetHeadPosition();

		while(pos)
		{
			DUNGEON_DATA_EX* pInfo = (DUNGEON_DATA_EX*)m_pInfoList[i]->GetNext(pos);
			g_pSprManager->RemoveSprite(pInfo->pSprMark);
		}
	}
}

void CDungeonTable::AddListDungeonGroup(DUNGEON_DATA_EX *pDungeon, WORD wGroupID)
{// ���� �׷� ������ �°� ����Ʈ�� �߰� �ض�.
	if (wGroupID)
	{
		POSITION_ pos = m_pGroupList[wGroupID]->GetHeadPosition();
		while(pos)
		{// �̹� �߰� �ߴٸ� �߰� ���� ���ƶ�.
			DUNGEON_DATA_EX* pTempDungeon = (DUNGEON_DATA_EX*)m_pGroupList[wGroupID]->GetNext(pos);
			if (pTempDungeon == pDungeon)
				return;
		}
		
		m_pGroupList[wGroupID]->AddHead(pDungeon);

		if (!g_pThisDungeon)
		{
			pDungeon->m_hGroupDungeon = CreateHandleObject( g_pObjManager->GetFile(DWORD(DUNGEON_GROUP)), NULL, NULL, GXOBJECT_CREATE_TYPE_EFFECT|GXOBJECT_CREATE_TYPE_DEFAULT_PROC );
			GXSetPosition(pDungeon->m_hGroupDungeon, &pDungeon->vPos, FALSE);
			// �׷������ �ϴ� �����.
			HideObject(pDungeon->m_hGroupDungeon);
		}

		// �ڱ� ��������..
		if (!g_pThisDungeon && pDungeon->IsDungeonOwner(g_pMainPlayer)) 
		{
			// ���忡�� ������ ����Ʈ ����.
			pDungeon->m_hOwnDungeon = CreateHandleObject( g_pObjManager->GetFile(DWORD(DUNGEON_OWNS)), NULL, NULL, GXOBJECT_CREATE_TYPE_EFFECT|GXOBJECT_CREATE_TYPE_DEFAULT_PROC );
			GXSetPosition(pDungeon->m_hOwnDungeon, &pDungeon->vPos, FALSE);
			ShowObject(pDungeon->m_hOwnDungeon);
		}		
	}
	
	if (!g_pThisDungeon && pDungeon->IsConquer())
	{
		pDungeon->m_hSiegingDungeon = CreateHandleObject( g_pObjManager->GetFile(DWORD(DUNGEON_SIEGING)), NULL, NULL, GXOBJECT_CREATE_TYPE_EFFECT|GXOBJECT_CREATE_TYPE_DEFAULT_PROC );
		GXSetPosition(pDungeon->m_hSiegingDungeon, &pDungeon->vPos, FALSE);
		// ����������.
		if (pDungeon->m_bSiege)	ShowObject(pDungeon->m_hSiegingDungeon);
		else					HideObject(pDungeon->m_hSiegingDungeon);		
	}
}

void CDungeonTable::ShowEffectDungeonGroup(WORD wGroupID, BOOL bShow)
{// ���� ���� �׷��� ã�Ƽ� ����Ʈ�� ǥ�������.
	if (!wGroupID) return;//0���� ���ٴ°��̴�.
	
	POSITION_ pos = m_pGroupList[wGroupID]->GetHeadPosition();
	while(pos)
	{
		DUNGEON_DATA_EX* pTempDungeon = (DUNGEON_DATA_EX*)m_pGroupList[wGroupID]->GetNext(pos);
		if (bShow)	ShowObject(pTempDungeon->m_hGroupDungeon);
		else		HideObject(pTempDungeon->m_hGroupDungeon);		
	}	
}

void CDungeonTable::HideAllDungeonGroupEffect()
{// ��� ���ܶ�.
	
	for(WORD i = 1; i < MAX_BYGROUP; ++i)
	{	
		ShowEffectDungeonGroup(i, 0);
	}
}

BOOL CDungeonTable::AddEventDungeon(DUNGEON_DATA_EX* pDungeon)
{
	if (NULL == pDungeon)
	{
		return FALSE;
	}

	if (NULL == m_pEventList)
	{
		return FALSE;
	}

	// ����Ʈ�� �߰�.

	pDungeon->pListPos = m_pEventList->AddTail(pDungeon);

	return TRUE;
}

void CDungeonTable::DeleteEventDungeonResource()
{
	// ��.... �� �� ���� �߰��� �Լ�.
	
	if (NULL == m_pEventList)
	{
		return;
	}

	DUNGEON_DATA_EX* pDungeon	= NULL;
	POSITION_ pos				= m_pEventList->GetHeadPosition();

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
}

