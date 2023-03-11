#include "stdafx.h"
#include "LicensingProc.h"
#include "OwnServer.h"
#include "User.h"
#include "UserTable.h"
#include "WorldServerTable.h"
#include "LicensingPacket.h"
#include "ServerFunction.h"
#include "ThrowQuery.h"
#include "recvmsg.h"


DWORD				g_dwLastHeartbeatTick = 0;
extern MACHINENAME	g_MachineName;


void	(*g_LicensingPacketProc[ MAX_LICENSING_PACKET_TYPE ])(char* pMsg, DWORD dwLength);


void InitializeProcFunctionForLicensing()
{
	ZeroMemory(g_LicensingPacketProc, sizeof(g_LicensingPacketProc));

	g_LicensingPacketProc[Proto_Licensing_Heartbeat]			= OnLicensingHeartbeat;
	g_LicensingPacketProc[Proto_Licensing_AuthenticaionResult]	= OnLicensingAuthenticationResult;
	g_LicensingPacketProc[Proto_Licensing_Command]				= OnLicensingCommand;
}

BOOL ConnectToLicensingServer()
{
	if(g_pThis->IsLicensingServerConnected())
	{
		Log(LOG_IMPORTANT, "@ Already connected with Licensing server!");
		return FALSE;
	}

	Log(LOG_IMPORTANT, "@ NOW Try to connect to Licensing server!");
	if(!g_pNet->ConnectToServerWithServerSide(	LICENSING_SERVER_IP, 
												LICENSING_SERVER_PORT, 
												SuccessToConnectLicensingServer, 
												FailToConnectLicensingServer, NULL))
	{
		Log(LOG_IMPORTANT, "@ Failed To Connect Licensing Server ...");
		return FALSE;
	}

	return TRUE;
}

void DisconnectLicensingServer()
{
	g_pThis->SetAuthenticated(FALSE);
	g_pThis->SetLicensingServerClosing(TRUE);

	// ������ �����ϱ� ���ؼ� ������ ���... 
	if(	g_pThis->IsLicensingServerConnected() && 
		g_pThis->IsLicensingServerConnectionClosing() )
	{
		// ������ ������.
		g_pNet->CompulsiveDisconnectServer(g_pThis->GetLicensingServerConnectionIndex());

		g_pThis->SetLicensingServerConnected(FALSE);
		g_pThis->SetLicensingServerConnectionIndex(0);
		g_pThis->SetLicensingServerClosing(FALSE);

		Log(LOG_IMPORTANT, "@ Connection With Licensing Server Successfully Closed~");
	}
}

void __stdcall FailToConnectLicensingServer(void* pExt)
{
	pExt;

	Log(LOG_IMPORTANT, "@ Failed To Connect License Server!");

	// �翬�� �õ� ���̾��ٸ�...
	if(g_pThis->IsTryToConnectLicensingServerContinuous())
	{
		g_pNet->ResumeTimer(LICENSING_CONNECT_TIMER_INDEX);
	}	
}

void __stdcall SuccessToConnectLicensingServer(DWORD dwConnectionIndex, void* pExt)
{
	pExt;

	Log(LOG_IMPORTANT, "@ Licensing Server Connected Successfully!");
	
	g_pThis->SetLicensingServerConnectionIndex(dwConnectionIndex);
	g_pThis->SetLicensingServerConnected(TRUE);

	// �翬�� �õ� ���̾��ٸ�...
	if(g_pThis->IsTryToConnectLicensingServerContinuous())
	{
		g_pNet->PauseTimer(LICENSING_CONNECT_TIMER_INDEX);
		g_pThis->SetTryToConnectLicensingServer(FALSE);
	}

	// ���� ���������Ƿ�, ���� ��û �޽��� ������.
	SendLicensingAuthentication();
}

void __stdcall OnDisconnectLicensingServer()
{
	// ������ �����ϱ� ���ؼ� ������ ���... 
	if(	g_pThis->IsLicensingServerConnected() && g_pThis->IsLicensingServerConnectionClosing() )
	{
		g_pThis->SetLicensingServerConnected(FALSE);
		g_pThis->SetLicensingServerConnectionIndex(0);
		g_pThis->SetLicensingServerClosing(FALSE);
		g_pThis->SetAuthenticated(FALSE);

		Log(LOG_IMPORTANT, "@ Connection With Licensing Server Successfully Closed~");
		
		// ���� ������ �����Ų��!!
		//TerminateServer();

		return;
	}

	// ������ �����.... �츮�� �ϴ� ���� �õ��ϰ�,
	// ���Ŀ��� ������ �����ϸ� ���� �ֱ⸶�� ������ �� ������ ������ �õ��Ѵ�.
	Log(LOG_IMPORTANT, "@ Connection with Licensing Server Closed!!");
	g_pThis->SetLicensingServerConnected(FALSE);

	// �翬������ �����ϰ�.. ���� �õ��Ѵ�.
	g_pThis->SetTryToConnectLicensingServer(TRUE);	

	ConnectToLicensingServer();	
}


void __stdcall OnRecvFromLicensingServerTCP(char* pMsg, DWORD dwLength)
{
	// Validation of packet type!
	if(	pMsg[0] >= MAX_LICENSING_PACKET_TYPE || NULL == g_LicensingPacketProc[pMsg[0]] )
	{
		Log(LOG_IMPORTANT, "Invalid Licensing Packet Received(Type: %d)", pMsg[0]);
		return;	
	}

	// proc ����!!
	(*g_LicensingPacketProc[pMsg[0]])(pMsg, dwLength);	
}

void __stdcall OnLicensingHeartbeat(DWORD dwEventIndex)
{
	dwEventIndex;

	// ������ ���� ���� �ð��� üũ�Ѵ�.
	if(0 != g_dwLastHeartbeatTick)
	{
		DWORD dwPassedTick = timeGetTime() - g_dwLastHeartbeatTick;

		// 5 �⵿ֱ�� Heartbeat�� ���޾Ҵٸ�!!!!
		if(dwPassedTick > LICENSING_HEARTBEAT_SEND_PERIOD * 5) // 5 min!!
		{
			Log(LOG_IMPORTANT, "OnLicensingHeartbeat() : DisconnectLicensingServer()");
			DisconnectLicensingServer();
			return;
		}
	}

	// HeartBeat ��Ŷ�� �����Ѵ�.
	SendLicensingHeartbeat();
}

void __stdcall OnLicensingTryConnect(DWORD dwEventIndex)
{
	dwEventIndex;

	Log(LOG_IMPORTANT, "try connctto lic");
	// �翬�� �õ��� �ð��� �ƴ�. �ٽ� �õ��϶�.
	ConnectToLicensingServer();
}

void __stdcall OnLicensingSendInfo(DWORD dwEventIndex)
{
	dwEventIndex;

	if(!g_pUserTable) return;

	// ���� ���� �۽��϶�!!
	SendLicensingCurrentUser(g_pUserTable->GetCount());
}


void OnLicensingHeartbeat(char* pMsg, DWORD dwLength)
{
	Proto_Licensing_Packet_Heartbeat* pPacket = (Proto_Licensing_Packet_Heartbeat*)pMsg;
	if(pPacket->GetPacketSize() != dwLength) 	
	{
		IllegalSizePacketProc(pPacket->byPacketType, dwLength);
		return;
	}

	// ������ Tick�� �����Ѵ�.
	g_dwLastHeartbeatTick = timeGetTime();
}

void OnLicensingAuthenticationResult(char* pMsg, DWORD dwLength)
{
	Proto_Licensing_Packet_AuthenticaionResult* pPacket =
		(Proto_Licensing_Packet_AuthenticaionResult*)pMsg;

	if(pPacket->GetPacketSize() != dwLength) 	
	{
		IllegalSizePacketProc(pPacket->byPacketType, dwLength);
		return;
	}

	if(pPacket->byResult != 1)
	{
		Log(LOG_IMPORTANT,"OnLicensingAuthenticationResult()");
		TerminateServer();
	}
	else
	{
		// ���� �����ߴ�... ������ �����ϵ��� �Ѵ�....
		g_pThis->SetAuthenticated(TRUE);

		// �ֱ������� ���� ���� �������� ����.
		g_pNet->ResumeTimer(LICENSING_SEND_INFO_INDEX); 
	}
}

void OnLicensingCommand(char* pMsg, DWORD dwLength)
{
	Proto_Licensing_Packet_Command* pPacket = (Proto_Licensing_Packet_Command*)pMsg;
	if(pPacket->GetPacketSize() != dwLength) 	
	{
		IllegalSizePacketProc(pPacket->byPacketType, dwLength);
		return;
	}

	switch(pPacket->byCommand)
	{
	case Command_Send_Heartbeat:			// Heartbeat�� ������� ���.
		SendLicensingHeartbeat(); 
		break;

	case Command_Send_CurrentUser:			// ���� ���� ������ ������� ���.
		if(g_pUserTable) SendLicensingCurrentUser(g_pUserTable->GetCount());
		break;

	case Command_Send_BillingInformation:	// ���� ������ ������� ���. ����μ� �ƹ��� �ൿ ����....
		break;

	case Command_Suicide:					// ������� ���.
		{
			Log(LOG_IMPORTANT,"OnLicensingCommand()");
			TerminateServer();
		}
		break;
	
	case Command_DropAllUsers:				// ��� ������ ������ ���� ������� ���.
		KickAllUser();
		break;
	}
}

void KickAllUser()
{
	if(!g_pUserTable) return;
	
	POSITION_ pos = g_pUserTable->GetHeadPosition();
	while(pos)
	{
		CUser* pPrevUser = (CUser*)g_pUserTable->GetNext(pos);		

		if(pPrevUser->m_dwChar_Index)
		{
			// �ٸ� ���峪 ���� ������ ������ �ִٸ� ��� ������� ������ 
			ASTWS_KICK_USER	KickPacket;
			KickPacket.dwCharIndex = pPrevUser->m_dwChar_Index;
			g_pServerTable->BroadCastWorldServer((char*)&KickPacket, KickPacket.GetPacketSize());
		}
		
		// Agent�� ���ӵǾ� �ִ� ���� 
		if(pPrevUser->m_dwCurWorldServerPos == SERVER_POS_AGENT)	
		{
			DWORD dwConnectionIndex = pPrevUser->GetConnectionIndex();
			OnDisconnectUser(dwConnectionIndex);
			g_pNet->SetUserInfo(dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);			
		}
		else
		{
			ThrowLogoutQuery(pPrevUser);

			// ���� ���̺��� �� ���� ���� 
			g_pUserTable->Remove(pPrevUser); // Free�� �� 
		}
	}
}

// send functions..
void SendToLicensingServer(char* szMsg, DWORD dwLen, BOOL bTimer)
{
	if(!g_pThis->IsLicensingServerConnected()) return;

	if(bTimer)
	{
		g_pNet->PauseTimer(LICENSING_HEARTBEAT_TIMER_INDEX);

		g_pNet->SendToServer(g_pThis->GetLicensingServerConnectionIndex(), szMsg, dwLen, FLAG_SEND_NOT_ENCRYPTION);

		g_pNet->ResumeTimer(LICENSING_HEARTBEAT_TIMER_INDEX);
	}
	else
	{
		g_pNet->SendToServer(g_pThis->GetLicensingServerConnectionIndex(), szMsg, dwLen, FLAG_SEND_NOT_ENCRYPTION);
	}
}

void SendLicensingHeartbeat()
{
	if(!g_pThis->IsLicensingServerConnected()) return;
	if(g_pThis->IsLicensingServerConnectionClosing()) return;

	Proto_Licensing_Packet_Heartbeat Packet;

	SendToLicensingServer((char*)&Packet, Packet.GetPacketSize());
}

void SendLicensingAuthentication()
{
	if(!g_pThis->IsLicensingServerConnected()) return;
	if(g_pThis->IsLicensingServerConnectionClosing()) return;

	Proto_Licensing_Packet_Authenticaion Packet;

	SendToLicensingServer((char*)&Packet, Packet.GetPacketSize());
}

void SendLicensingCurrentUser(DWORD dwCCU)
{
	if(!g_pThis->IsLicensingServerConnected()) return;
	if(g_pThis->IsLicensingServerConnectionClosing()) return;

	Proto_Licensing_Packet_CurrentUser Packet;
	Packet.dwCurrentUserCount = dwCCU;

	SendToLicensingServer((char*)&Packet, Packet.GetPacketSize());
}

void SendLicensingBillingInformation(char* pData, DWORD dwSize)
{
	if(!g_pThis->IsLicensingServerConnected()) return;
	if(g_pThis->IsLicensingServerConnectionClosing()) return;

	Proto_Licensing_Packet_BillingInformation Packet;

	if(dwSize > sizeof(Packet.BillingInformation)) return;
	{
	//	Log(LOG_IMPORTANT, "BillingInformation is larger than packet!! %u", dwSize);
		return;
	}

	CopyMemory(Packet.BillingInformation, pData, dwSize);

	SendToLicensingServer((char*)&Packet, Packet.GetPacketSize());
}

