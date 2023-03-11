/*#####################################################################//
//##################### 2004. 11. Ŭ����ȭ ����.. #####################
//#####################################################################
//
//	Version		: Dev/Develop
//	ProjectName	: CorumOnlineProject
//	FileName	: RivalGuildWarLib.cpp
//	Date		: 2004. 10. 18. ( minjin )
//	Description	: Define Rival Guild War Global Function
//				  For Assistant to Set or Get Information About Guild
//
//#####################################################################*/

#include "StdAfx.h"
#include "Guild.h"
#include "GuildHash.h"
#include "RivalGuildWarLib.h"

/*	minjin.
 *	�켱��, ���̹� ����� ���� �Լ��� ����ٰ� ��Ƴ��´�.
 *	���Ÿ� �׳� ������ ����, ��ư ����� ��ƴ� ����,
 *	���߿� Ŭ������ �ٲܶ�.. �� ���ƾ��������.. -_-;;;
 */

//------------------------------------------------------------------------
// Date			: 2004. 10. 19. ( Written by minjin )
// Description	: ���̹� ����� �Ҽ� �ִ��� ������ �˻�
//				  �� �� ������ RGWE_GUILD_WAR_SUCCESS
//				  �Ҽ� ������, EN_RivalGuildWarErrorCode �� ����
//------------------------------------------------------------------------
// ����� ����
// 1. ��� ������ 5 �̻��̾�� ��
// 2. ��� �����Ͱ� ���ӵǾ� �־�� ��
// 3. ��帶���͸� ������ ������ ������ �ּ� 5�� �̻� �־�� ��
// 4. �й��� �� 24 �ð��� ������ ���ӿ� ���� ����
//------------------------------------------------------------------------
/* 
BYTE RGWLibCheckCondition( DWORD dwMyGuildIdx, DWORD dwOpGuildIdx )
{
//	LP_GUILD_LIST	lpGuildList		= g_pGuildWarHash->GetData(dwMyGuildIdx);	// ��û�� ���
//	LP_GUILD_LIST	lpGuildListDst	= g_pGuildWarHash->GetData(dwOpGuildIdx);	// ��� ���

	LP_GUILD_TABLE	lpGuildTable	= g_pGuildTableHash->GetData(dwMyGuildIdx);	// ��û�� ���
	LP_GUILD_TABLE	lpGuildTableDst	= g_pGuildTableHash->GetData(dwOpGuildIdx);	// ��� ���


/ *	if( !lpGuildList )			// ��û�� ����� ��帮��Ʈ.. �̻�..?
	{
		Log( LOG_NORMAL, "[minjin] lpGuildList is Invalidate. Check the GuildList TABLE" );
		return RGWE_GUILD_DATA_ERROR;
	}

	if( !lpGuildListDst )		// ��� ����� ��帮��Ʈ.. �̻�..?
	{
		Log( LOG_NORMAL, "[minjin] lpGuildListDst is Invalidate. Check the GuildList TABLE" );
		return RGWE_GUILD_DATA_ERROR;
	}* /


	if( !lpGuildTable )			// ��û�� ����� ������̺��� �̻�..?
	{
		Log( LOG_NORMAL, "[minjin] lpGuildTable is Invalidate. Check the GuildList TABLE" );
		return RGWE_GUILD_DATA_ERROR;
	}

	if( !lpGuildTableDst )		// ��� ����� ������̺��� �̻�..?
	{
		Log( LOG_NORMAL, "[minjin] lpGuildTableDst is Invalidate. Check the GuildList TABLE" );
		return RGWE_GUILD_DATA_ERROR;
	}

	CWorldUser* pMyMasterUser = IsThereGuildMaster( lpGuildTable->pGuildUserHash->GetHead() );

	if( !pMyMasterUser )														return RGWE_NOT_GUILD_MASTER;
//	if( lpGuildTable->sGuild.wLevel < MIN_RGW_GUILD_LEVEL )						return RGWE_MUST_LEVEL5;

	
#if defined(JAPAN_LOCALIZING)
	if( lpGuildTable->pGuildUserHash->GetCount() <= MIN_RGW_GUILD_MEMBER )		return RGWE_MORE_GUILD_MEMBER;
#endif


	CWorldUser* pOpMasterUser = IsThereGuildMaster( lpGuildTableDst->pGuildUserHash->GetHead() );

	if( !pOpMasterUser )														return RGWE_VS_NOT_GUILD_MASTER;
//	if( lpGuildTableDst->sGuild.wLevel < MIN_RGW_GUILD_LEVEL )					return RGWE_VS_MUST_LEVEL5;

	
#if defined(JAPAN_LOCALIZING)
	if( lpGuildTableDst->pGuildUserHash->GetCount() <= MIN_RGW_GUILD_MEMBER )	return RGWE_VS_MORE_GUILD_MEMBER;
#endif


	// ��� �������.
	// ���̹� ����� �ض�..
	return RGWE_GUILD_WAR_SUCCESS;
}*/


VOID	ConfirmDestRGWDungeon( DWORD* dwDungeonID, DWORD* dwDestLayer )
{
	// �켱 5500 ������ �Ѵ�!
	*dwDungeonID = 5500;
	*dwDestLayer = 0;
}
/*
//------------------------------------------------------------------------
// Date			: 2004. 09. 24. ( Written by minjin )
// Description	: ��� �����Ͱ� ������ �渶 Pointer / ������ FALSE
//------------------------------------------------------------------------
CWorldUser* IsThereGuildMaster(ListNode<CWorldUser>* pGuildUserPos)
{
	CWorldUser*	pMasterUser	= NULL;

	while ( pGuildUserPos ) 
	{
		pMasterUser = pGuildUserPos->pData;

		if ( pMasterUser ) 
		{
			if ( pMasterUser->m_byRank == 1 ) 
			{
				return pMasterUser;		// �渶 Pointer ����!
			}
		}
		else
		{
			return FALSE;
		}
		pGuildUserPos = pGuildUserPos->pNext;
	}

	// ����Ʈ ������ �˻��ѰŰ�����.. �׷��ٸ�..
	// ���!! �渶�� ����..!!! -_-;;
	return FALSE;
}//*/
