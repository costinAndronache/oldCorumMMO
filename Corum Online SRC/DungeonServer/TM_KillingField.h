#pragma once
#include "stdAfx.h"
#include "baseteammatch.h"

class CTM_KillingField 
	: public CBaseTeamMatch
{
public:
	CTM_KillingField(const WORD wDungeonID, const BYTE byLayerIndex)
		: CBaseTeamMatch(wDungeonID,byLayerIndex)
	{
	}
	virtual ~CTM_KillingField(void)
	{
	}

	virtual bool OnDie(CUnit *pCaster, CUnit *pTarget)
	{
		const FLAG_TYPE caster_is_player = this->GetTeamMatchFlag( pCaster );
		const FLAG_TYPE target_is_player = this->GetTeamMatchFlag( pTarget );
		if(	caster_is_player 
		&&	target_is_player)
		{//�Ѵ� ������.	//���� ó�� ����.
			TEAM_INFO *pTeamInfo = m_mapPlayer.Find( pCaster->Name() );

			if( pTeamInfo )
			{
				MATCH_DATA *pMD = m_mapMatchData.Find(pTeamInfo->team_index);
				if( pMD )
				{
					pMD->IncPoint();

					Send_DSTWS_STADIUM_TEAM_POINT(this, pTeamInfo->team_index, pMD->point );
				}
			}
		}
		return false;
	}

	virtual bool OnAttack(const CUnit *pCaster, const CUnit *pTarget)
	{
		if( !pCaster || !pTarget)
		{//�������� ������.
			return false;
		}

		if( pCaster != pTarget)
		{
			const FLAG_TYPE caster_flag = GetTeamMatchFlag( pCaster );
			const FLAG_TYPE target_flag = GetTeamMatchFlag( pTarget );

			if( (G_W_F_PLAYER & caster_flag )//���ΰ��� �÷��̾�߸� ���� �� �ִ�.
			&&	(G_W_F_PLAYER & target_flag ) )
			{
				const size_t caster_team = GetTeamIndex( pCaster );
				const size_t target_team = GetTeamIndex( pTarget );
				if( caster_team != target_team )
				{
					const DWORD dwNowTime		= ::timeGetTime();
					const DWORD dwTargetTime	= pTarget->DeathTime() + 10000;
					if( dwTargetTime > dwNowTime )//10�ʵ��� ������.
					{
						return false;
					}

					return true;
				}
			}
		} // if( pCaster != pTarget)

		return false;;
	}

	virtual bool OnMove(CUnit *pCaster){return true;}

	virtual FLAG_TYPE GetMatchType(){return M_T_KILLING_FIELD;}

	virtual void OnEnd()
	{//������Ʈ�� ���⼭ 
		Log(LOG_IMPORTANT, "!! GuildWar End '' ");
	}

	virtual bool OnProcess()
	{	//���⼭ ����� ������ ������
		typedef map< COUNT_TYPE, COUNT_TYPE >	COUNT_HASH;
		typedef COUNT_HASH::iterator			COUNT_ITOR;

		PLAYER_ITOR player_itor = m_mapPlayer.begin();

		map< COUNT_TYPE, COUNT_TYPE > mapTempCount;
		mapTempCount.clear();
		
		while( m_mapPlayer.end() != player_itor)
		{
			TEAM_INFO &rTD = (*player_itor).second;

			if( 0 != rTD.team_index )
			{
				COUNT_ITOR count_itor = mapTempCount.find( rTD.team_index );
				if(mapTempCount.end()  == count_itor)
				{
					mapTempCount.insert( make_pair( rTD.team_index, 1 ) );
				}
				else
				{
					++((*count_itor).second);
				}
			}
			++player_itor;
		} // while( m_mapPlayer.end() != player_itor)

		if( MIN_MATCH_TEAM > mapTempCount.size() )//��� ���ڶ�� ����
		{//2���̸� üũ�� ���������� �ټ��� �Ǹ� üũ�� ��������� ����
			return false;	
		}

		COUNT_ITOR count_itor = mapTempCount.begin();
		while( mapTempCount.end()  != count_itor )
		{//�� ���ڶ�� �˽�.
			if( (*count_itor).second != PlayerCount() )
			{
				return false;
			}
			++count_itor;
		}

		return true;
		//Start ������
	}

	virtual VECTOR2 GetPos( const size_t team_index, const bool bIsAddRandomPos )
	{
		static const VECTOR2 aVec[] = 
				{	{4700,4700},//0 ������

					{4800,1350},//3	4��
					{7850,4650},//1	2��
					{4600,7850},//4 1��				
					{1350,4650},//2	3��
					
					{4700,4700},//5 ������
					{4700,4700},//6 ������
					{4700,4700}//7 ������
				} ;
		if(	0 <= team_index 
		&&	(sizeof(aVec)/sizeof(VECTOR2)) >team_index)
		{
			if(bIsAddRandomPos)
			{
				const float x = (rand()%800)-400;
				const float y = (rand()%800)-400;

				VECTOR2 vec = aVec[team_index];

				vec.x += x;
				vec.y += y;

				return vec;
			} // if(bIsAddRandomPos)
			return aVec[team_index];
		}

		return aVec[0];
	}
};
