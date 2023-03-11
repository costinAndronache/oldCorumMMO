#ifndef __SERVER_TABLE_H__
#define __SERVER_TABLE_H__

#pragma once


#include "LinkedListEx.h"

struct SERVER_DATA
{
	BYTE			bServerType;		
	DWORD			dwID;							//Server ID(Port��ȣ)
	DWORD			dwConnectionIndex;	
	DWORD			dwServerStatus;	
	POSITION_		ListPos;						//LinkedList������ ��� ��ġ�� 
	DWORD			dwIPForUser;
	char			szIPForServer[MAX_IP_LENGTH];	//IP For Server
	WORD			wPort;							//��Ʈ
	BYTE			bSended;						//Server Alive Check Packet ���´����� Flag
	BYTE			bReceived;						//Server Alive Check Packet ������ �޾Ҵ����� Flag
	SERVER_DATA*	pNextData;
	SERVER_DATA*	pPrevData;
};


class CServerTable
{
	DWORD					m_dwMaxBucketNum;	
	DWORD					m_dwNum;		
	SERVER_DATA**			m_ppInfoTable;
	SERVER_DATA*			m_pLoginAgent;		
	CServerList				m_ServerList[MAX_SERVER_TYPE];	// ServerType - 1�� �ε����� �ش� ServerType�� ����Ʈ�� ����Ŵ 
		
	void					RemoveServerTable();
		
public:								  
	
	SERVER_DATA*			AllocNewServer(DWORD dwID);
	SERVER_DATA*			GetServerInfo(DWORD dwId);
	SERVER_DATA*			SetLoginAgentServer(DWORD dwConnectionIndex);
	SERVER_DATA*			GetLoginAgentServer()		{	return m_pLoginAgent;	}
	BOOL					Add(SERVER_DATA* pServer);
	void					Remove(DWORD dwConnectionIndex);
	void					Remove(SERVER_DATA* pInfo);
	void					BroadCast(char* pPacket, DWORD dwLength, BYTE bServerType = 0);		// bServerType�� �������� ��� �� ������������ Broadcast
	void					SendAgent(char* pPacket, DWORD dwLength);	
	DWORD					GetCount()	{	return m_dwNum;	}
	void					ShowAllServerStatus();	
	void					BroadCastAlivePacket();
	void					CheckAlivePacket();

	CServerTable(DWORD dwNum);
	~CServerTable();
};


extern CServerTable* g_pServerTable;


#endif 