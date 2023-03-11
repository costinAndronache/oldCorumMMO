#pragma once

#include "../CommonServer/TeamMatcher.h"
#include ".\GuildMatchHandler.h"
#include ".\StadiumMgr.h"

extern CStadiumMgr< CStadium >		g_StadiumMgr;

class CGuildMatcher
	:	public CTeamMatcher< G_MC_DEFAULT , CGuildMatchHandler >
{
	typedef CCondition<  CMatchCondition , size_t >	G_MC_READY;//����� ������ ������ ����Ʈ���� Key
	typedef map<  G_MC_READY , HANDLER_ARRAY >		READY_HASH;

public:
	CGuildMatcher(void){}
	virtual ~CGuildMatcher(void){}

	virtual bool HandleEvent()
	{	//Ready ������ handler�� �̰�
		//Condition �� ���� handler�� vector�� ���� //�ο����� ����. G_MC_READY
		//�ο����� ������? condition �� �־ ó���ϳ�?
		//randomshuffle�� ���ϰ� �տ��� ���� �� ���� ��ŭ ������ ����. 1���� 2���� -->
		READY_HASH mapReady;
		mapReady.clear();
{
		HANDLER_HASH_ITOR hash_Itor = m_hash.begin();
		while( m_hash.end() != hash_Itor ) //ready �� �ֵ鳢�� �׷�ȭ
		{
			T_HANDLER_FOR_HASH &pHandler	= (*hash_Itor).second;
			G_MC_DEFAULT	&r_match_cond	= const_cast< G_MC_DEFAULT& >((*hash_Itor).first);

			size_t minplayerCount;

			switch( r_match_cond.Second().TeamType() )
			{
			case T_T_GUILD:
				minplayerCount = g_min_player_count;
				break;			
			case T_T_S_GUILD:
				++hash_Itor;
				continue;			
			case T_T_RANDOM:
			default:
				minplayerCount = g_min_player_count_AutoMatch;
				break;
			}

			if( pHandler->IsReady( minplayerCount ) )//Ready�̳�?
			{
				G_MC_READY ready_cond;
				
				ready_cond.First( r_match_cond.First() );
				ready_cond.Second( pHandler->ReadyPlayerCount() );

				READY_HASH::iterator ready_itor =  mapReady.find( ready_cond );//�ش� ������� �ִ��� ã��
				
				if( mapReady.end() != ready_itor )
				{//�̹� HANDLER_ARRAY�� �ִٸ� �ű⿡ iterator�� �߰��Ѵ�.
					HANDLER_ARRAY& rvecHandler = (*ready_itor).second;
					
					READY_ELEMENT Ready(r_match_cond,pHandler) ;
					rvecHandler.push_back( Ready );
				}
				else
				{
					mapReady.insert( make_pair( ready_cond, HANDLER_ARRAY() ) );
					
					READY_HASH::iterator after_itor =  mapReady.find( ready_cond );
					if( mapReady.end() != after_itor )
					{
						HANDLER_ARRAY& rvecHandler = (*after_itor).second;

						READY_ELEMENT Ready(r_match_cond, pHandler) ;

						rvecHandler.push_back( Ready );
					}
					else
					{
						assert( NULL && "Can't Insert GuildMatcher ReadyMap");
					}
				}
			}
			++hash_Itor;
		} // while( m_hash.end() != Itor )
}//�Ʒ��� while�� ���� �ȵ� �õ� ���� �� ��.
{
		//map<  G_MC_READY , HANDLER_ARRAY >		READY_HASH; //���� ������ Key�� ������ �ڵ鷯�� �������� �迭�� ������ ��
		READY_HASH::iterator ready_itor = mapReady.begin();//Ready�� 
		
		while( mapReady.end()  != ready_itor )
		{//Ready �� �ֵ��� �ٿ���
			G_MC_READY &ready_cond = const_cast<G_MC_READY&>( (*ready_itor).first );
			//���� ����� ��� ���Ҵ��� Ȯ�� �ؾ� �Ѵ�.
	
			HANDLER_ARRAY& rvecHandler = (*ready_itor).second;//ready ������ handler��
			
			if( rvecHandler.size() >= MIN_MATCH_TEAM )// n:n ����ġ�� �ϱ� ���ؼ��� while�� ī��Ʈ�� �ٲٸ� �ȴ�
			{//������ �ϱ����� �ּ����� �� ���ڰ� �����ȴٸ�
				CStadium *pSD = g_StadiumMgr.PreparedStadium(
					ready_cond.First().MatchType() ,	ready_cond.First().MatchLevel(),
					MIN_MATCH_TEAM,						ready_cond.Second(),
					GWT_TEN,							true );//�غ�� ����� �޴´�
				if(!pSD)//�ش� ���ǿ� ��� ������
				{
					++ready_itor;
					continue;
				}

				random_shuffle( rvecHandler.begin(), rvecHandler.end() );//�� ����.
				
				//rvecHandler�ȿ��� handler ���� ��� �ְ� �ű⼭ ��ġ�� ��ų Handler�� �����.
				
				HANDLER_ARRAY vecAttached;//���� ���� ���� ������

				for(size_t i = 0; MIN_MATCH_TEAM > i; ++i)// n:n ����ġ�� �ϱ� ���ؼ��� for�� ī��Ʈ�� �ٲٸ� �ȴ�
				{
					if( !rvecHandler.empty())
					{
						HANDLER_ARRAY::reverse_iterator rItor = rvecHandler.rbegin() ;
						vecAttached.push_back( (*rItor) );
						rvecHandler.pop_back();
					}
				} // for(size_t i = 0; MIN_MATCH_TEAM > i; ++i)

				//��Ī �޼����� ������.
				HANDLER_ARRAY::iterator attach_itor = vecAttached.begin();

				vector< size_t > vec_team_index;
				vec_team_index.clear();

				size_t team_index_pos = 0;//����ڴ� 1�� ����

				vec_team_index.push_back(1);//�� �ε���. ������ 4�� �ۿ� �����ϱ�. �̷��� �س��´�.
				vec_team_index.push_back(2);
				vec_team_index.push_back(3);
				vec_team_index.push_back(4);

				random_shuffle( vec_team_index.begin(), vec_team_index.end() );

				while( vecAttached.end() != attach_itor )
				{
					READY_ELEMENT &ReadyElement = (*attach_itor);
					
					G_MC_DEFAULT &rCond			= ReadyElement.m_Cond;
					T_HANDLER_FOR_HASH pHandler = ReadyElement.m_pHandler;
					
					const size_t ready_count = ready_cond.Second();

					CMatcherResult::NotifyResult NR;

					NR.player_count = ready_count;

					NR.match_type		= ready_cond.First().MatchType();
					NR.pStadium			= pSD;
					NR.team_index		= vec_team_index.at(team_index_pos);
					NR.dwGuildWarFlag	= G_W_F_PLAYER;
					NR.team_type		= rCond.Second().TeamType();
//					NR.DMI.byLayerIndex = pSD->byLayerIndex;
//					NR.DMI.wDungeonID = pSD->dwDungeonID;
//					NR.DMI.wX = pSD->aPos[team_index].x;//������ MAX_MATCH_TEAM ��ŭ�� üũ �ؼ� �ֱ� ������ index ���� �˻縦 �� �ʿ�� ����.
//					NR.DMI.wY = pSD->aPos[team_index].y;
					
					pHandler->OnMatchNotify( NR );//�Ű��ְ����� �������� �ֵ� ����Ʈ�� �־�����.
					++attach_itor;
					++team_index_pos;
				}
			}
			++ready_itor;
		} // while( m_MapReady.end()  != ready_itor )
}
		return false;
	}
	
	T_HANDLER* GetSTypeHandler(G_MC_DEFAULT &rGMC)
	{
		HANDLER_HASH_ITOR hash_Itor = m_hash.begin();
		while( m_hash.end() != hash_Itor ) //�츮�� ����Ʈ �ޱ�
		{
			const G_MC_DEFAULT &rFirst =  (*hash_Itor).first ;
			if( rFirst == rGMC)
			{
				return (*hash_Itor).second;
			}
			++hash_Itor;
		}
		return NULL;
	}
};

extern CGuildMatcher g_GuildMatcher;