#pragma once

#include "stdafx.h"
#include "Matcher.h"
#include "WorldUserTable.h"

#include "DungeonTable.h"

struct MATCH_CONDITION_PARTY
{
	BYTE bLevel;	// ����
	BYTE bExp;		// ����ġ �й�
	BYTE bItem;		// ������ �й�
};

bool operator<(const MATCH_CONDITION_PARTY& f, const MATCH_CONDITION_PARTY& b);

class CMatcherActionParty
{

// ���׸� ������ ���� �˻��� �� ��Ī�� : �ִ��� 2005.1.29
public:
	// ���� �˻� �� ��Ī��
	void DoMatch(DWORD dwOwnUserIndex, DWORD dwGuestUserIndex, DWORD dwMatchIndex)
	{
		CWorldUser* pOwnUser = (CWorldUser*)g_pUserTable->GetUserInfo(dwOwnUserIndex);
		CWorldUser* pGuestUser = (CWorldUser*)g_pUserTable->GetUserInfo(dwGuestUserIndex);

		if(pOwnUser == NULL || pGuestUser == NULL)
			return;

		// �� ��󿡰� ���� ��ȣ�� �˷���
		pOwnUser->m_dwPMatchIndex = dwMatchIndex;
		pGuestUser->m_dwPMatchIndex = dwMatchIndex;

		// ���忡�� ��Ī�� �³��� ���
		WSTC_PMATCH_REQUEST PMatchRequest;
		PMatchRequest.bLevel = (BYTE)pGuestUser->m_dwLevel;
		PMatchRequest.bClass = (BYTE)pGuestUser->m_wClass;
		strcpy(PMatchRequest.szName, pGuestUser->m_szCharacterName);
		PMatchRequest.bMaster = 1;

		g_pNet->SendToUser(pOwnUser->m_dwConnectionIndex, (char*)&PMatchRequest, PMatchRequest.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
	}

	// (�����) ���� ������
	bool MakeCondition(DWORD dwUserIndex, MATCH_CONDITION_PARTY& cnd, MATCH_CONDITION_PARTY& out)
	{
		CWorldUser* pUser = (CWorldUser*)g_pUserTable->GetUserInfo(dwUserIndex);

		if(pUser == NULL)
			return false;

		// ����� �Ƿ� ����(����)�� ������ �Ѿ��ٸ�
		if(pUser->m_PMatchCondition.bLevel < pUser->m_bLevelDist)
		{
			// ���� ������ �ʱ�ȭ
			pUser->m_bLevelDist = 0;
			pUser->m_iSign = 1;

			// ���̻� (�����) ������ ������ �˷���
			return false;
		}

		// ���� ���� ������ �˷���
		out = cnd;
		
		out.bLevel = (BYTE)(pUser->m_dwLevel + pUser->m_bLevelDist * pUser->m_iSign);
		
		pUser->m_iSign *= -1;
		if(pUser->m_iSign == -1)
			pUser->m_bLevelDist++;

		return true;
	}

	// ��Ī ��� �ڵ鷯
	void CancelMatch(DWORD dwOwnUserIndex, DWORD dwGuestUserIndex)
	{
		CWorldUser* pOwnUser = (CWorldUser*)g_pUserTable->GetUserInfo(dwOwnUserIndex);
		CWorldUser* pGuestUser = (CWorldUser*)g_pUserTable->GetUserInfo(dwGuestUserIndex);

		if(pOwnUser == NULL)
		{
			pOwnUser->m_dwPMatchIndex = 0;
		}

		if(pGuestUser == NULL)
		{
			pGuestUser->m_dwPMatchIndex = 0;
		}
	}

	// ��Ī �������� �˾Ƴ�
	bool CanMatch(DWORD dwOwnUserIndex, DWORD dwGuestUserIndex)
	{
		
		CWorldUser* pOwnUser = (CWorldUser*)g_pUserTable->GetUserInfo(dwOwnUserIndex);
		CWorldUser* pGuestUser = (CWorldUser*)g_pUserTable->GetUserInfo(dwGuestUserIndex);
		DUNGEON_DATA_EX *pDungeonForOwn, *pDungeonForGuest;

		// ��Ŷ �սǷ�, ���� ����� ��Ī �Ϸ���
		if(dwOwnUserIndex == dwGuestUserIndex)
			return false;

		// ���� ��� ������ �����ٸ�
		if(pOwnUser == NULL || pGuestUser == NULL)
			return false;

		// �Խ�Ʈ�� ������ ���Ѵٸ�
		if(pGuestUser->m_PMatchCondition.bMaster == 1)
			return false;

		// �Խ�Ʈ�� ��Ƽ�� ���� ���̸�
		if(pGuestUser->m_dwPartyId != 0)
			return false;

		// �������� ���
		int iLevelDist = (int)abs((int)pOwnUser->m_dwLevel - (int)pGuestUser->m_dwLevel);
		
		// ���ϴ� �������� �������
		if(iLevelDist > pOwnUser->m_PMatchCondition.bLevel
		|| iLevelDist > pGuestUser->m_PMatchCondition.bLevel)
			return false;
	
		// ���� ������ �˾Ƴ�
		pDungeonForOwn = g_pDungeonTable->GetDungeonInfo((WORD)pOwnUser->m_dwDungeonID);
		pDungeonForGuest = g_pDungeonTable->GetDungeonInfo((WORD)pGuestUser->m_dwDungeonID);

		// ������ ���忡 �ִٸ�
		if( pDungeonForOwn == NULL ||  pDungeonForGuest == NULL)
			return false;

		// ����� ���̸�
		if(pDungeonForOwn->IsStadium() || pDungeonForGuest->IsStadium())
			return false;

		// �̺�Ʈ �����̸�
		if(pDungeonForOwn->GetDungeonType() == DUNGEON_TYPE_EVENT || pDungeonForGuest->GetDungeonType() == DUNGEON_TYPE_EVENT)
			return false;

		return true;
	}
};

extern CMatcher<MATCH_CONDITION_PARTY, DWORD, CMatcherActionParty> g_PartyMatcher;