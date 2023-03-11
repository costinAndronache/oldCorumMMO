#pragma once
#include "BaseNetworkInterface.h"
#include "Define.h"

#define		MAX_SERVER_CONNECT		4
#define		SERVER_INDEX_AGENT		0
#define		SERVER_INDEX_WORLD		1	// ���� ����.	�׻� ������ �����ؾ� ������ ���� ������ �ִ´�.
#define		SERVER_INDEX_ZONE		2	// ������ ������ ����Ȱ��� �ε���. �ϳ��� ���� ��������.

// #define		MAX_PACKET_NUM			256 //GameStatus�� �ִ� Packet�� 

#define WORLD_SERVER_CONNECT_PORT	13200	
#define LOGIN_AGENT_CONNECT_PORT	13100


//#define		WM_SOCKET_EVENT			=	WM_USER + 0x7000;

#define SIZE_PACKET 4

class CNetworkClient : public ISC_BaseNetwork
{

	// �ɹ� ���� ���� �߰� ����
public:
	DWORD	GetServerIndex( BYTE bServerType );
	
	BOOL	SendMsg( char* pszMsg, DWORD dwLength, BYTE bServerType );
	
	DWORD	ConnectServer(char* szIP, WORD wPort, BYTE bServerType );
	void	DisconnectServer( BYTE bServerType );
	int	    MakeEncPaket( char * pszPacket, DWORD dwLength,  char * szReturn,int nPacketSize);
	int	    MakeDecPaket(char *pszPacket,int nPacketSize, char * szReturn);
	int     GetBlockCount(int nPacketSize);
	BOOL	isCheckSize(int nNotEncSize, DWORD dwSize);
};

// Network ���� �Լ�
void OnConnect(DWORD dwIndex);
void OnDisConnect(DWORD dwIndex);
void ReceivedMsg(DWORD dwIndex, char* pMsg, DWORD dwLen);
void InitPacketProc();

extern CNetworkClient*	g_pNet;
extern DWORD	g_dwConnectIndex[MAX_SERVER_CONNECT];
extern void	(*PacketProc[ MAX_UPDATE_GAME ][ MAX_PACKET_NUM ])( char* pMsg, DWORD dwLen );