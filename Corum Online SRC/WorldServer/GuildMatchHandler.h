#pragma once
#include "../CommonServer/CommonServer.h"
#include "../CommonServer/TeamMatchHandler.h"

#include "../CommonServer/STLSupport.h"

#include "windows.h"
#include "mmsystem.h"
#pragma comment (lib,"winmm.lib")

typedef size_t COUNT_TYPE;

const COUNT_TYPE g_aMatchTeamCount[] =  {2,3,4}; //�ݵ�� ������� ���������� �� ��
const COUNT_TYPE g_min_team_count = g_aMatchTeamCount[0];

// Remark
// �����Ī ������� �ϱ� ���� �ο� ����Ʈ
const COUNT_TYPE g_aBasePlayerCount[] =  
{
	6,
	9,
	12,
	15,
	18
}; //�ݵ�� ������� ���������� �� ��
// ���� ������� �ϱ� ���� �ּ��ο�
const COUNT_TYPE g_min_player_count = g_aBasePlayerCount[0];
// ������� ������� �ϱ� ���� �ּ��ο�
#if defined(GUILDMATCH_TEST)
const COUNT_TYPE g_min_player_count_AutoMatch = 1;
#else
const COUNT_TYPE g_min_player_count_AutoMatch = 6;
#endif

const DWORD	g_dwGuildWarDelayTime = 5000;//1�� ���� ��ĥ�� 60000

extern bool MoveToStadium( CWorldUser *pUser, const DUNGEON_MOVE_INFO &rDMI , const size_t team_index, const DWORD dwGuildWarFlag = G_W_F_PLAYER, const bool bIsGMCClear =true);//�⺻������ �÷��̾��

class CPlayerInfo
{
public:
	CPlayerInfo(CWorldUser *pUser)
	{
		Name( pUser->m_szCharacterName );
//		Class( pUser->m_eClass );
//		LV( pUser->m_dwLevel );
		m_pUser = pUser;
//		m_bIsCalled = false;
	}

	virtual ~CPlayerInfo()
	{
		m_pUser = NULL;
	}

public:
//	int Index() const {return m_iIndex;}
//	void Index(const int iIndex) {m_iIndex = iIndex;}

	const char* Name() const {return m_strName.c_str();}
	void Name(const string &rstrName) {m_strName = rstrName;}

//	CLASS_TYPE	Class() const {return m_eClass;}
//	void Class(const CLASS_TYPE eCT) {m_eClass = eCT;}
	
//	DWORD	LV()const {return m_dwLv;}
//	void	LV(const DWORD dwLv){m_dwLv = dwLv;}

	WORD	Class() const {return m_pUser->m_wClass;}
	DWORD	LV()const {return m_pUser->m_dwLevel;}

//	void Call(const bool bIsCalled){m_bIsCalled = bIsCalled;}
//	bool IsCalled() const {return m_bIsCalled;}


	bool operator == (const CPlayerInfo& rRight)const 
	{
		if(	//this->Index() == rRight.Index()
		this->m_strName == rRight.m_strName )//Name() ���� �ϸ� �����Ͱ� ���ƿ´�.
		{
			return true;
		}
		return false;
	}

protected:	
	CWorldUser	*m_pUser;
	string		m_strName;
//	bool		m_bIsCalled;
//	CLASS_TYPE	m_eClass;
//	DWORD		m_dwLv;
};

class CGuildMatchHandler
	: public CTeamMatchHandler< CPlayerInfo >// ������
{
public:
	CGuildMatchHandler(void)
	{
		WaitTime(0);
		ReadyPlayerCount(0);
	}
	
	virtual ~CGuildMatchHandler(void)
	{
	}

public:	
	virtual bool Open()
	{
		cout << "Open Called[ "<< typeid(this).name()<<" ]"<< endl;
		return true;
	}
	virtual void OnDestroy(){return;}

	virtual bool handle_insert()//insert �� 
	{
		return true;
	}

	virtual bool handle_remove()//insert ��
	{
		return true;
	}

	virtual void OnInsert()//insert �Ϸ�
	{//�Ѹ��� �߰� �� ��, Ÿ�Ժ� �ο����� �´� ����� ������ ��ٸ��� �ð��� �߰����ش�.
		cout << "OnInsert Called[ "<< typeid(this).name() <<" ]"<<  endl;
		cout << "Now Size [ "<< List().size() <<" ]"<<  endl;
				
		const size_t array_count = sizeof(g_aBasePlayerCount)/sizeof(COUNT_TYPE);

		const size_t prev_ready_count = ReadyPlayerCount();
		const size_t now_real_count = List().size();

		for(size_t i = 0; array_count > i; ++i)
		{
			const size_t &r_base_count = g_aBasePlayerCount[i];
			if(	prev_ready_count < r_base_count
			&&	now_real_count >= r_base_count )
			{
				ReadyPlayerCount( r_base_count );
				break;
			}
		} // for(size_t i = 0; array_count > i; ++i)

		if( prev_ready_count != ReadyPlayerCount() )//�����ܰ�� ���� �ɶ����� wait Ÿ���� �ɸ���.
		{
			WaitTime( ::timeGetTime() + g_dwGuildWarDelayTime );
		}
	}

	virtual void OnRemove()//remove �Ϸ�
	{
		cout << "OnRemove Called[ "<< typeid(this).name()<<" ]"<< endl;
		cout << "Now Size [ "<< List().size() <<" ]"<<  endl;

		const size_t array_count = sizeof(g_aBasePlayerCount)/sizeof(COUNT_TYPE);

		const size_t now_count = List().size();

		ReadyPlayerCount(0);//0���� �ʱ�ȭ �ؾ߸� min ���� ���� ��� ������ ���� �ȴ�.

		for(size_t i = 0; array_count > i; ++i)
		{
			const size_t &r_base_count = g_aBasePlayerCount[i];
			if(	now_count >= r_base_count )
			{
				ReadyPlayerCount( r_base_count );
				break;
			}
		} // for(size_t i = 0; array_count > i; ++i)
	}
	
	virtual bool IsReady( size_t minplayerCount ) CONST
	{
		if( WaitTime() <= ::timeGetTime() )//����Ʈ Ÿ���� �ɸ��� �ʰ�
		{
			if( minplayerCount <= ReadyPlayerCount() )
			{
				return true;
			}
		}
		return false;
	}

	virtual bool OnMatchNotify( const CMatcherResult::NotifyResult& rNR )
	{//
		cout << "!!! OnMatchNotify Called[ "<< typeid(this).name()<<" ]"<< "TeamIndex[" << rNR.team_index<< "]" <<endl;
		const size_t target_count = rNR.player_count;//���¥�� ������.
		size_t count = 0;

		ELEMENT_LIST::iterator Itor = List().begin();
		
		while( List().end() != Itor)
		{
			T_PLAYER rPlayer = (*Itor);

			CWorldUser* pUser = g_pUserTable->GetUserInfoByName( rPlayer.Name() );

			if(pUser)
			{//���� �̵�
				DUNGEON_MOVE_INFO DMI;
				DMI.wDungeonID = rNR.pStadium->wDungeonID;
				DMI.byLayerIndex = rNR.pStadium->byLayerIndex;
				
				DMI.wX = static_cast< WORD >(rNR.pStadium->aPos[rNR.team_index].x);
				DMI.wY = static_cast< WORD >(rNR.pStadium->aPos[rNR.team_index].y);

				STADIUM_PLAYER S_P(rNR.match_type, DMI,rNR.team_index, rNR.dwGuildWarFlag, rNR.team_type );
				rNR.pStadium->InsertUser( pUser, S_P );
//				rNR.pStadium->PlayerHash().Insert(rPlayer.Name(), S_P );//��忡 ��� �ְ�
				if( ::MoveToStadium(pUser, DMI, rNR.team_index , rNR.dwGuildWarFlag) )
				{
					Itor = List().erase(Itor);//Erase ���� �̵� �����ϸ� �����
				}
			} // if(pUser)
			else
			{
				++Itor;
			}

			if( target_count == (++count) )
			{
				break;
			}
		} // while( List().end() != Itor)

		OnRemove();

		return true;
	}//������ ���� �̵� ��Ű�� �κ�

	DWORD WaitTime() CONST 
	{
		return m_dwWaitTime;
	}
	void WaitTime(const DWORD dwWaitTime){m_dwWaitTime = dwWaitTime;}
	
	size_t ReadyPlayerCount() const {return m_ready_player;}
	void ReadyPlayerCount(const size_t ready_player)
	{
		cout << "ReadyPlayerCount [ "<< ready_player <<" ]"<< endl;
		m_ready_player = ready_player;
	}
private:
	DWORD m_dwWaitTime;
	size_t m_ready_player;
};