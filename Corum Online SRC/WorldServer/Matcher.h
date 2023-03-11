#pragma once

#include <map>
#include <deque>
#include <algorithm>

using namespace std;


template<typename T_CONDITION, typename T_USER, class T_ACTION>
class CMatcher
{
//////////////
// ���׸� ���� : �ִ��� 2005.1.29
//
// ������ �κ�� �����Ű��
// ��� �´� ��븦 ã���� �׵��� ������ ������ش�
// ��Ī�� �����ϸ� ������ ���常 �κ�� ���������� ������ ���ش�.
// ��Ī�� �����ϸ� �׵��� �κ�� ���������� ������ ���ش�.

	typedef deque<T_USER> V_USER;
	typedef map<T_CONDITION, V_USER> M_LOBBY;

	struct SReadyRoom
	{
		T_CONDITION cndOwn;
		T_USER usrOwn;
		T_CONDITION cndGuest;
		T_USER usrGuest;
		DWORD dwCreateTime;
	};

	typedef map<int, SReadyRoom> M_READYROOM;

protected:
	M_LOBBY m_Lobby;					// ��븦 ��ٸ��� �κ�
	M_READYROOM m_ReadyRoom;		// ��Ī ������ ��ٸ��� ����
	T_ACTION m_Handler;				// ��Ī��

public:
	// �κ�� ����
	int Insert(T_CONDITION& cnd, T_USER& usr, bool bMatching = true)
	{
		T_CONDITION cndOwn;
		T_USER usrOwn;
		
		// �κ� ����ϸ鼭 �ٷ� ��븦 ã�����
		if(bMatching && Search(cnd, usr, cndOwn, usrOwn))
		{
			// ���� ��Ī�� �ּ�
			Match(cndOwn, usrOwn, cnd, usr);
			return true;
		}
		
		// ���ϴ� ������ �κ� ���ٸ�
		M_LOBBY::iterator i = m_Lobby.find(cnd);
		if(i == m_Lobby.end())
		{
			// �� �κ� �����
			i = m_Lobby.insert(m_Lobby.end(), make_pair(cnd, deque<T_USER>()));
		}

		// �κ� ����� ���
		i->second.push_back(usr);
		
		return true;
	}

	// �κ񿡼� ����
	bool Remove(T_CONDITION& cnd, T_USER& usr)
	{
		M_LOBBY::iterator i =  m_Lobby.find(cnd);

		// ������ �κ� ���ٸ�
		if(i == m_Lobby.end())
		{
			// ����
			return false;
		}

		// �κ񿡼� ����� ã��
		V_USER& vecUser = i->second;
		V_USER::iterator j = find(vecUser.begin(), vecUser.end(), usr);
		
		// ����� �ִٸ�
		if(j != vecUser.end())
		{
			// �κ񿡼� ����� Ż��
			vecUser.erase(j);
			return true;
		}
		// ����� ���ٸ�
		else
		{
			// ����
			return false;
		}
	}

	// ��Ī ��븦 ã��
	bool Search(T_CONDITION& cndGuest, T_USER& usrGuest, T_CONDITION& cndOwn, T_USER& usrOwn/*out*/)
	{
		M_LOBBY::iterator i;
		T_CONDITION cnd;
		
		// ����� ���ǵ� ��ο� ����
		while(m_Handler.MakeCondition(usrGuest, cndGuest, cnd))
		{
			// ����� ������ �κ� ã�ƶ�
			i = m_Lobby.find(cnd);

			// �κ� �ְ� ��뵵 �ִٸ�
			if(i != m_Lobby.end() && i->second.size() > 0 && m_Handler.CanMatch(i->second.front(), usrGuest))
			{
				// ã�� �κ�� ��븦 �˷���
				cndOwn = cnd;
				usrOwn = i->second.front();
				
				return true;
			}
		}

		// ã�� ������
		return false;
	}

	// �� ����� ��Ī ���Ѷ�
	int Match(T_CONDITION& cndOwn, T_USER& usrOwn, T_CONDITION& cndGuest, T_USER& usrGuest)
	{
		static int matchId = 1;

		// �� ���� ��ȣ�� ����
		matchId++;

		// ������ ����� �� ����� ���� ��Ŵ
		SReadyRoom room = {cndOwn, usrOwn, cndGuest, usrGuest, g_dwCurTick};
		m_ReadyRoom.insert(make_pair(matchId, room));

		// ��Ī�� ��Ű��
		m_Handler.DoMatch(usrOwn, usrGuest, matchId);

		// �κ񿡼� �� ����� Ż��
		Remove(cndOwn, usrOwn);
		Remove(cndGuest, usrGuest);

		return matchId;
	}

	// ���� ������ �˾Ƴ�
	bool GetReadyDB(int matchId, T_USER& usrOwn, T_USER& usrGuest)
	{
		M_READYROOM::iterator i = m_ReadyRoom.find(matchId);
		
		// ������ ���ٸ�
		if(i == m_ReadyRoom.end())
		{
			// ����
			return false;
		}
		
		// ���� ������ �˷���
		SReadyRoom& room = i->second;
		usrOwn = room.usrOwn;
		usrGuest = room.usrGuest;

		return true;
	}

	// ��Ī �Ϸ�
	bool MatchSuccess(int matchId)
	{
		M_READYROOM::iterator i = m_ReadyRoom.find(matchId);
		
		// ������ ���ٸ�
		if(i == m_ReadyRoom.end())
		{
			// ����
			return false;
		}

		// ������ �κ�� ������ ������ ����
		SReadyRoom& room = i->second;
		Insert(room.cndOwn, room.usrOwn, false);
		m_ReadyRoom.erase(i);

		return true;
	}

	// ��Ī ����
	bool MatchFail(int matchId)
	{
		M_READYROOM::iterator i = m_ReadyRoom.find(matchId);
		
		// ������ ���ٸ�
		if(i == m_ReadyRoom.end())
		{
			// ����
			return false;
		}


		// ������ �� ����� �κ�� ������ ������ ����
		SReadyRoom& room = i->second;
		Insert(room.cndOwn, room.usrOwn, false);
		Insert(room.cndGuest, room.usrGuest, false);

		m_Handler.CancelMatch(room.usrOwn, room.usrGuest);

		m_ReadyRoom.erase(i);

		return true;
	}

	// �ڵ� ��Ī
	void Process(int iCount)
	{
		M_READYROOM::iterator r;
		M_LOBBY::iterator i;
		static int flag = 0;
	
		
		//// ���� ���� ����, ��� ��Ű��
		r = m_ReadyRoom.begin();
		// ��� ���ǿ� ����
		for(;r != m_ReadyRoom.end();r++)
		{
			SReadyRoom& room = r->second;
			
			// ������ ����� ���� 10�ʰ� �����ٸ�
			if(g_dwCurTick - room.dwCreateTime > 10000)
			{
                // ��Ī�� ��� ��Ŵ
				MatchFail(r->first);

				// ���ͷ����Ͱ� �����Ƿ� �ٽ� ����
				r = m_ReadyRoom.begin();
			}
		}

		i = m_Lobby.begin();

		for(int n = 0;n<flag && i != m_Lobby.end();n++)
			i++;

		for(;i != m_Lobby.end();i++, flag++)
		{
			if(i->second.size() == 0)
				continue;

			T_CONDITION cnd, cndOwn;
			T_USER usr, usrOwn;

			cnd = i->first;
			usr = i->second.front();
			i->second.pop_front();

			if(Search(cnd, usr, cndOwn, usrOwn))
			{
				// ���� ��Ī�� �ּ�
				Match(cndOwn, usrOwn, cnd, usr);
			}
			else
			{
				// �ڷ� ����, ��ȸ�� �յ�
				i->second.push_back(usr);
			}

			iCount--;

			if(iCount == 0)
				break;
		}

		if(i == m_Lobby.end())
		{
			flag = 0;
		}
	}
};