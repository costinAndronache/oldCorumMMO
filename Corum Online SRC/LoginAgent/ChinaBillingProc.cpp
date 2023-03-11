#include "stdafx.h"
#include "ChinaBillingProc.h"
#include "OwnServer.h"
#include "../CommonServer/Log.h"
#include "User.h"
#include "UserTable.h"
#include "DBProc.h"
#include "ThrowQuery.h"
#include "../CommonServer/LoginPacket.h"
#include "../CommonServer/AgentServerPacket.h"
#include "WorldServerTable.h"
#include "recvmsg.h"
#include "ServerFunction.h"
#include "PacketEncrypt.h"
#include "User.h"




// function pointers for china billing message process.
void	(*g_BillPacketProc[ MAX_BILLING_PACKET_TYPE ])(char* pMsg, DWORD dwLength);


void InitializeProcFunction()
{
	ZeroMemory(g_BillPacketProc, sizeof(g_BillPacketProc));

	g_BillPacketProc[china_billing_packet_login_result]	= OnBillingMsgLoginResult;
	g_BillPacketProc[china_billing_packet_kickout]		= OnBillingMsgKickOut;
	g_BillPacketProc[china_billing_packet_server_clear]	= OnBillingMsgClear;
}

void ConnectToBillingServer()
{
	if(g_pThis->IsBillingServerConnected())
	{
		Log(LOG_JUST_DISPLAY, "@ Already connected with billing server!");
		return;
	}

	if(!g_pNet->ConnectToServerWithServerSide(	g_pThis->GetIPForBillingServer(), 
												g_pThis->GetPortForBillingServer(), 
												SuccessToConnectBillingServer, 
												FailToConnectBillingServer, NULL))
	{
		Log(LOG_IMPORTANT, "@ Failed To Connect Billing Server ...");
	}
}

void DisconnectBillingServer()
{
	g_pThis->SetBillingServerClosing(TRUE);

	// ������ ���� ���� clear �޽��� �۽��Ѵ�.
	SendChinaBillingClear();
}

void __stdcall FailToConnectBillingServer(void* pExt)
{
	Log(LOG_IMPORTANT, "@ Failed To Connect Billing Server! (IP:%s, Port:%d)", 
		g_pThis->GetIPForBillingServer(), g_pThis->GetPortForBillingServer() );

	// �翬�� �õ� ���̾��ٸ�...
	if(g_pThis->IsTryToConnectBillingServerContinuous())
	{
		g_pNet->ResumeTimer(CHINA_BILLING_CONNECT_TIMER_INDEX);
	}
}

void __stdcall SuccessToConnectBillingServer(DWORD dwConnectionIndex, void* pExt)
{
	Log(LOG_JUST_DISPLAY, "@ Billing Server Connected Successfully!");
	
	g_pThis->SetBillingServerConnectionIndex(dwConnectionIndex);
	g_pThis->SetBillingServerConnected(TRUE);

	// �翬�� �õ� ���̾��ٸ�...
	if(g_pThis->IsTryToConnectBillingServerContinuous())
	{
		g_pNet->PauseTimer(CHINA_BILLING_CONNECT_TIMER_INDEX);
		g_pThis->SetTryToConnectBillingServer(FALSE);
		
		// ���� �����Ͽ� ���� ���� ��� ������ �α��� ������ �ٽ� ������!
		SendChinaBillingLoginAll();
	}

	// ���� ���������Ƿ�, clear �޽��� ������ �˸���.
	SendChinaBillingClear();
}

void __stdcall OnDisconnectBillingServer()
{	
	// ������ �����ϱ� ���ؼ� ������ ���... 
	if(	g_pThis->IsBillingServerConnected() && 
		g_pThis->IsBillingServerConnectionClosing() )
	{
		g_pThis->SetBillingServerConnected(FALSE);
		g_pThis->SetBillingServerConnectionIndex(0);
		g_pThis->SetBillingServerClosing(FALSE);

		Log(LOG_IMPORTANT, "@ Connection With Billing Server Successfully Closed~");
		
		// ���� ������ �����Ų��!!
		TerminateServer();

		return;
	}

	// ������ �����.... �츮�� �ϴ� ���� �õ��ϰ�,
	// ���Ŀ��� ������ �����ϸ� ���� �ֱ⸶�� ������ �� ������ ������ �õ��Ѵ�.
	Log(LOG_IMPORTANT, "@ Connection with Billing Server Closed!!");
	g_pThis->SetBillingServerConnected(FALSE);

	// �翬������ �����ϰ�.. ���� �õ��Ѵ�.
	g_pThis->SetTryToConnectBillingServer(TRUE);	

	ConnectToBillingServer();
}

void __stdcall OnRecvFromBillingServerTCP(char* pMsg, DWORD dwLength)
{
	// Validation of packet type!
	if(	pMsg[0] >= MAX_BILLING_PACKET_TYPE || NULL == g_BillPacketProc[pMsg[0]] )
	{
		Log(LOG_IMPORTANT, "Invalid Billing Packet Received(Type: %d)", pMsg[0]);
		return;	
	}

	Log(LOG_IMPORTANT, "<BILLING INFO>Recv Packet Type : %d", (BYTE)*pMsg);
	// reset keep alive check timer

	// proc ����!!
	(*g_BillPacketProc[pMsg[0]])(pMsg, dwLength);
}

void __stdcall OnChinaBillingKeepAlive(DWORD dwEventIndex)
{
	dwEventIndex;

	// Keep Alive �� ���� ��Ŷ�� �����Ѵ�.
	SendChinaBillingDummy();
}

void __stdcall OnChinaBillingTryConnect(DWORD dwEventIndex)
{
	dwEventIndex;

	// �翬�� �õ��� �ð��� �ƴ�. �ٽ� �õ��϶�.
	ConnectToBillingServer();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OnBillingMsgClear(char* pMsg, DWORD dwLength)
{
	Proto_China_Billing_Server_Clear* pPacket = (Proto_China_Billing_Server_Clear*)pMsg;

	// ����� �� �������� Ȯ��!
	if(pPacket->btGameWorld != atoi(g_pThis->GetServerSetCode())) return;

	// ���� ���� ������ �ƴϸ� �����Ѵ�.
	if(!g_pThis->IsBillingServerConnectionClosing()) return;
	
		// ������ �����ϱ� ���ؼ� ������ ���... 
	if(	g_pThis->IsBillingServerConnected() && 
		g_pThis->IsBillingServerConnectionClosing() )
	{
		// ������ ������.
		g_pNet->CompulsiveDisconnectServer(g_pThis->GetBillingServerConnectionIndex());

		g_pThis->SetBillingServerConnected(FALSE);
		g_pThis->SetBillingServerConnectionIndex(0);
		g_pThis->SetBillingServerClosing(FALSE);

		Log(LOG_IMPORTANT, "@ Connection With Billing Server Successfully Closed~");

		// ���� ������ �����Ų��!!
		TerminateServer();
	}
}

void OnBillingMsgLoginResult(char* pMsg, DWORD dwLength)
{
	Proto_China_Billing_Login_Result* pPacket = (Proto_China_Billing_Login_Result*)pMsg;
	
	
	//////////////////////////////////////////////////////////////////////////
	// Log
	Log(LOG_IMPORTANT, "In OnBillingMsgLoginResult");
	//--


	// ����� �� �������� Ȯ��!
	if(pPacket->GetServerSetID() != atoi(g_pThis->GetServerSetCode())) return;

	char szUserName[0xff]={0,};
	lstrcpyn(szUserName, pPacket->GetUserName(), MAX_ID_LENGTH);
	DWORD dwUserIndex	= pPacket->GetUserIndex();	

	// ���� ���̺��� �� ���� ��ü ȹ��.
	CUser* pUser		= g_pUserTable->GetUserInfo(dwUserIndex);

	// �ϴ� �ش� ������ ������ ���¸� ...
	if(!pUser)
	{
		Log(LOG_IMPORTANT, "pUser is NULL!! in OnBillingMsgLoginResult()");

		// �α��� ������ ��쿡�� �ٽ� �α׾ƿ� �޽����� �����࿩.. �̶� ip�� �𸣴ϱ� 0����!!
		switch(pPacket->GetLoginResult())
		{
		case china_billing_login_success :
		case china_billing_login_unexpected1 :
		case china_billing_login_unexpected2 :
			SendChinaBillingLogout(dwUserIndex, szUserName, 0);
			break;
		}

		return;
	}

	// ����� ���� �α��� ó��!!
	switch(pPacket->GetLoginResult())
	{
		// �ϴ� �α��� ������Ų��.
	case china_billing_login_success :
	case china_billing_login_unexpected1 :
	case china_billing_login_unexpected2 :
		{
			// �������� ���� �ð�/�Ⱓ ���� ��Ŷ ����
			WSTC_CHINA_BILL_REMAIN_TIME Packet;
			
			Packet.SetTimeType(pPacket->GetTimeType());
			Packet.SetTime(pPacket->GetTime());

			SendToUser(pUser, (char*)&Packet, Packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			// �����۸� ���� �����ϸ鼭 ��� ����.
			
			if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//�߱����� Test : 050104 hwoarang
			{
				// China Biling Info Set //
				pUser->m_sChinaBillingData.dwChinaBillingPoint		= Packet.dwTimeValue;
				pUser->m_sChinaBillingData.eChinaBillingUserData	= china_billing_info_approval;

				LPWorldServerData pServer = g_pServerTable->GetServer( 1 );
				
				RS_CharacterLogInfo_With_Equip * pSelectedCharacter = pUser->GetSelectedCharacter();

				if(pServer && pSelectedCharacter)
				{
					Log(LOG_IMPORTANT, "<BILLING INFO> %s(%3d), Login OK!!", 
						pSelectedCharacter->szName, pSelectedCharacter->dwLevel);

					SendChinaBillingInfo(
						pUser,	
						pPacket->GetUserIndex(), 
						Billing_Info_Procedure_Type_NONE);		// normal proc type						

				}
			}
		}
		break;

		// �α��� ����!
	case china_billing_login_no_user :
	case china_billing_login_over_capacity :
	case china_billing_login_timeout :
	case china_billing_login_canceled :
	case china_billing_login_etc :
	case china_billing_login_time_expired :
		{
			if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//�߱����� Test : 050104 hwoarang
			{
				// China Biling Info Set //
				pUser->m_sChinaBillingData.dwChinaBillingPoint		= 0;
				pUser->m_sChinaBillingData.eChinaBillingUserData	= china_billing_info_limit;

				// �������� ���� �ð�/�Ⱓ ���� ��Ŷ ����
				WSTC_CHINA_BILL_REMAIN_TIME Packet;
				
				Packet.SetTimeType(pPacket->GetTimeType());
				Packet.SetTime(pPacket->GetTime());

				SendToUser(pUser, (char*)&Packet, Packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	

				LPWorldServerData pServer = g_pServerTable->GetServer( 1 );
				RS_CharacterLogInfo_With_Equip * pSelectedCharacter = pUser->GetSelectedCharacter();

				if(pServer && pSelectedCharacter)
				{
						SendChinaBillingInfo(pUser, pPacket->GetUserIndex(), Billing_Info_Procedure_Type_Normal);	// normal
						Log(LOG_IMPORTANT, "<BILLING INFO> %s(%3d), Login Fail!!", 
							pSelectedCharacter->szName, pSelectedCharacter->dwLevel);
				}
			}
			if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//�߱����� : 050104 hwoarang
			{
				// �������� �α��� ���� - �α��� ���� �޽��� �۽�.
				WSTC_LOGIN_FAIL	packet;
				packet.bResult = (BYTE)(0xf0 | pPacket->GetLoginResult());
				SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);			

				OnDisconnectUser(pUser->m_dwConnectionIndex);
				g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
				g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);
			}
		}break;
	}
}

void OnBillingMsgKickOut(char* pMsg, DWORD dwLength)
{
	Proto_China_Billing_Kickout* pPacket = (Proto_China_Billing_Kickout*)pMsg;

	// ����� �� �������� Ȯ��!
	if(pPacket->GetServerSetID() != atoi(g_pThis->GetServerSetCode())) return;
	
	char szUserName[0xff]={0,};
	lstrcpyn(szUserName, pPacket->GetUserName(), MAX_ID_LENGTH);
	DWORD dwUserIndex	= pPacket->GetUserIndex();

	// ���� ���̺��� �� ���� ��ü ȹ��.
	CUser* pUser = g_pUserTable->GetUserInfo(dwUserIndex);
	
	// �ϴ� �ش� ������ ������ ���¸� ...
	if(!pUser)
	{
		Log(LOG_IMPORTANT, "pUser is NULL!! in OnBillingMsgKickOut()");

		// ������ �α׾ƿ� �޽����� �����࿩.. �̶� ip�� �𸣴ϱ� 0����!!
		SendChinaBillingLogout(dwUserIndex, szUserName, 0);

		return;
	}

	char szInfo[0xff] = {0, };
	Log(LOG_NORMAL, "OnBillingMsgKickOut");

	if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//�߱����� : 050104 hwoarang
	{
		// China Biling Info Set //
		pUser->m_sChinaBillingData.dwChinaBillingPoint		= 0;
		pUser->m_sChinaBillingData.eChinaBillingUserData	= china_billing_info_limit;

		if(pUser->m_dwCurWorldServerPos != SERVER_POS_AGENT)
		{
			char szInfo[0xff] = {0, };
			wsprintf(szInfo, "OnBillingMsgKickOut - ID : %s, UserIndex : %u", szUserName, dwUserIndex);
			Log(LOG_NORMAL, szInfo);

			//////////////////////////////////////////////////////////////////////////
			// �ϴ� �α׿��� ��Ų��.
			ASTWS_KICK_USER	KickPacket;
			//�ٸ� ���峪 ���� ������ ������ �ִٸ� ��� ������� ������ 
			KickPacket.dwCharIndex = pUser->GetSelectedCharacter()->dwCharacterIndex;
			g_pServerTable->BroadCastWorldServer((char*)&KickPacket, KickPacket.GetPacketSize());
			
			pUser->m_dwStatus = 0;

			OnDisconnectUser(pUser->m_dwConnectionIndex);
			g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);

			//���� ���̺��� �� ���� ���� 
			g_pUserTable->Remove(pUser);		//Free�� �� 
			pUser = NULL;
			//--
		}
	}
	else
	{
		// ���� ���忡 �α��� ���� ���� ���� 
		if(pUser->m_dwCurWorldServerPos == SERVER_POS_AGENT)		
		{
			DWORD dwConnectionIndex = pUser->m_dwConnectionIndex;
			OnDisconnectUser(dwConnectionIndex);
			g_pNet->SetUserInfo(dwConnectionIndex, NULL);
			g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);

			SendChinaBillingLogout(dwUserIndex, szUserName, pUser->GetIP());
		}
		else
		{
			// ���� �������� �� ���� �α׾ƿ� ��Ű��� �޽��� ����!
			ASTWS_KICK_USER Packet;

			Packet.dwCharIndex = pUser->m_dwChar_Index;
			Packet.btKickCode = pPacket->GetKickCode();

			g_pServerTable->BroadCastWorldServer((char*)&Packet, Packet.GetPacketSize());
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////

void SendToChinaBillingServer(char* szMsg, DWORD dwLen, BOOL bTimer)
{
	if(bTimer)
	{
		g_pNet->PauseTimer(CHINA_BILLING_KEEP_ALIVE_TIMER_INDEX);

		g_pNet->SendToServer(g_pThis->GetBillingServerConnectionIndex(), szMsg, dwLen, FLAG_SEND_NOT_ENCRYPTION);

		g_pNet->ResumeTimer(CHINA_BILLING_KEEP_ALIVE_TIMER_INDEX);
	}
	else
	{
		g_pNet->SendToServer(g_pThis->GetBillingServerConnectionIndex(), szMsg, dwLen, FLAG_SEND_NOT_ENCRYPTION);
	}

	//////////////////////////////////////////////////////////////////////////
	//	log
	Log(LOG_IMPORTANT, "<BILLING INFO>Send Packet Type : %d", (BYTE)*szMsg);
	// --
}

void SendChinaBillingLogin(DWORD dwUserIndex, LPCSTR szUserId, DWORD dwIP, BOOL bTimer)
{
	if(!g_pThis->IsBillingServerConnected()) return;
	if(g_pThis->IsBillingServerConnectionClosing()) return;

	Proto_China_Billing_Login Packet;
	
	Packet.SetServerSetID(atoi(g_pThis->GetServerSetCode()));
	Packet.SetUserIndex(dwUserIndex);
	Packet.SetUserID(szUserId);	
	Packet.SetIP(dwIP);


	//////////////////////////////////////////////////////////////////////////
	// Log
	CUser * pUser = g_pUserTable->GetUserInfo(dwUserIndex);
	if (pUser)
	{
		RS_CharacterLogInfo_With_Equip* pSelectCharacter = pUser->GetSelectedCharacter();
		Log(LOG_IMPORTANT, "<BILLING INFO> TRY LOGIN %s(%d)...", 
		pSelectCharacter->szName, pSelectCharacter->dwLevel);
	}
	//--
	
	SendToChinaBillingServer((char*)&Packet, Packet.GetPacketSize(), bTimer);
}

void SendChinaBillingLogout(DWORD dwUserIndex, LPCSTR szUserId, DWORD dwIP)
{
	if(!g_pThis->IsBillingServerConnected()) return;
	if(g_pThis->IsBillingServerConnectionClosing()) return;

	Proto_China_Billing_Logout Packet;

	Packet.SetServerSetID(atoi(g_pThis->GetServerSetCode()));
	Packet.SetUserIndex(dwUserIndex);
	Packet.SetUserID(szUserId);
	Packet.SetIP(dwIP);

	//////////////////////////////////////////////////////////////////////////
	// Log
	CUser * pUser = g_pUserTable->GetUserInfo(dwUserIndex);
	if (pUser)
	{
		RS_CharacterLogInfo_With_Equip* pSelectCharacter = pUser->GetSelectedCharacter();
		Log(LOG_IMPORTANT, "<BILLING INFO> LOGOUT %S(%d)...", 
			pSelectCharacter->szName, pSelectCharacter->dwLevel);
	}
	//--

	SendToChinaBillingServer((char*)&Packet, Packet.GetPacketSize());	
}

void SendChinaBillingClear()
{
	if(!g_pThis->IsBillingServerConnected()) return;

	Proto_China_Billing_Server_Clear Packet;

	Packet.SetServerSetID(atoi(g_pThis->GetServerSetCode()));
	
	SendToChinaBillingServer((char*)&Packet, Packet.GetPacketSize());
}

void SendChinaBillingDummy()
{
	Proto_China_Billing_Dummy Packet;

	SendToChinaBillingServer((char*)&Packet, Packet.GetPacketSize());
}

void SendChinaBillingLoginAll()
{
	g_pNet->PauseTimer(CHINA_BILLING_KEEP_ALIVE_TIMER_INDEX);

	POSITION_ pos = g_pUserTable->GetHeadPosition();
	while(pos)
	{
		CUser* pUser = g_pUserTable->GetNext(pos);
		if(pUser) 
		{
			SendChinaBillingLogin(	pUser->GetPropID(), 
									pUser->GetUserID(), 
									pUser->GetIP(),
									FALSE);
		}
	}

	g_pNet->ResumeTimer(CHINA_BILLING_KEEP_ALIVE_TIMER_INDEX);
}


void SendChinaBillingInfo(CUser * pUser, DWORD dwUserIndex, enum BILLING_INFO_PROCEDURE_TYPE eProcType)
{
	ASTWS_CHINA_BILLING_INFO pChinaBillingInfoPacket;
	pChinaBillingInfoPacket.byType			= pUser->m_sChinaBillingData.eChinaBillingUserData;
	pChinaBillingInfoPacket.dwPoint			= pUser->m_sChinaBillingData.dwChinaBillingPoint;
	pChinaBillingInfoPacket.dwUserIndex		= dwUserIndex;
	pChinaBillingInfoPacket.SetProcedureType(eProcType);


	//////////////////////////////////////////////////////////////////////////
	// Log
	RS_CharacterLogInfo_With_Equip * pSelectCharacter = pUser->GetSelectedCharacter();
	Log(LOG_IMPORTANT, "SendChinaBillingInfo() %S(%d) : PROC(%d), Point(%d)", 
		pSelectCharacter->szName, pSelectCharacter->dwLevel, 
		pUser->m_sChinaBillingData.eChinaBillingUserData, 
		pUser->m_sChinaBillingData.dwChinaBillingPoint);
	//--	


	g_pServerTable->BroadCastWorldServer((char*)&pChinaBillingInfoPacket, 
		pChinaBillingInfoPacket.GetPacketSize());

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void QueryTyprLoginChinaBilling(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if(!pUser)
	{
		Log(LOG_IMPORTANT, "pUser is NULL!! in QueryTypeLogin()");
		return;
	}

	DWORD dwResult;
	memcpy(&dwResult, pResult->Query.select.pResult, sizeof(DWORD));

	switch(dwResult)
	{
	case 0:		//Login OK	//billing check ���� �ƴ� 
		{
			if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )
			{
				Log(LOG_IMPORTANT, "Query size is overflow!");
				__asm int 3
				return;
			}

			LOGIN_RESULT_WITH_PROPID	LoginResult;
			memcpy(&LoginResult, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);

			// ���� ������ �������� ������ �Ǿ� �ִ� ���°� �ƴ϶��..
			// ������ �α����� ������ ���н�Ų��.
			if(!g_pThis->IsBillingServerConnected() ||
				g_pThis->IsBillingServerConnectionClosing())
			{
				// ���� �޽��� ����!!
				WSTC_LOGIN_FAIL	packet;
				packet.bResult = (BYTE)(0xf0); // !!!!! �� ��� ���� �ڵ忡 ���� �ڵ带 �־� ������!!!! 
				SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				
				// ���� ������!
				pUser->m_dwStatus = 0;
				memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
				
				g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
				
				OnDisconnectUser(pUser->m_dwConnectionIndex);
				g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
				g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);				
			}
			
			//-------  2003�� 10�� 28�� �̺�Ʈ�� ��õ Event �ڵ� -----------------
			//if(LoginResult.dwRestrictType == EVENT_NUM_RECOM_ADD_ITEM)
				pUser->m_bEventNum = EVENT_NUM_RECOM_ADD_ITEM;
			//--------------------------------------------------------------------
			
			//���� ���̺� ���� �߰�
			pUser->m_dwStatus = USER_STATUS_LOGIN_SUCCESS;
			pUser->m_dwPropID = atoi(LoginResult.szPropID);

			//���� �����ؽ��� �ش� ���������� �����ִµ� �α���ó�� �� �� ��û�� �Ŷ�� 
			//���� �����ؽ� ������ ���� �����ִ� ó���� ���� 
						
			PrevUserProcess( pUser->m_dwPropID );

			// User ���̺� �߰�...
			BOOL bChk = g_pUserTable->Add(pUser);				

			// �ϴ� ���⿡�� ���� �������� �� ������ �������� �뺸�Ѵ�!!!!!
			// ������ ó���� ���� �����κ��� �α��� ����� �޾� �����Ѵ�.
			// WSTAS_USER_WORLD_LOGIN_SUCCESS ��Ŷ�� �޾Ƽ� ������.
			// ���忡 ���� ���� ���� ����.
			//SendChinaBillingLogin(pUser->GetPropID(), pUser->GetUserID(), pUser->GetIP());
			if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//�߱����� Test : 050104 hwoarang
			{
				ThrowItemMallQuery(pUser);	
			}
		}
		break;
	case 1:	//SQL DB ����
	case 2:	//���� ���̵�
	case 3: //��й�ȣ Ʋ��
		{
			WSTC_LOGIN_FAIL	packet;
			packet.bResult = (BYTE)dwResult;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			
			OnDisconnectUser(pUser->m_dwConnectionIndex);
			g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);
		}
		break;

	case 4:		//���� �������� ���̵�	//billing check ���� �ƴ� 
		{
			DWORD dwPropID = 0;

			//DB���� Logout ó�� 	
			if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )
			{
				Log(LOG_IMPORTANT, "Query size is overflow!");
				__asm int 3
				return;
			}

			LOGIN_RESULT_WITH_PROPID	LoginResult;
			memcpy(&LoginResult, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);
			
			int nUserServerNum = atoi(g_pThis->GetServerSetCode());
			if(LoginResult.dwPrevUserServerSet != (DWORD)nUserServerNum)
			{
				//�ٸ� ������ �ߺ� �α����� �Ǿ��ٴ� �޼����� ������. 
				WSTC_LOGIN_FAIL	AnotherPacket;
				AnotherPacket.bResult = (BYTE)LOGIN_FAIL_OVERLAPPED_ID_ANOTHER_SERVER;
				AnotherPacket.dwExData = LoginResult.dwPrevUserServerSet;
				SendToUser(pUser, (char*)&AnotherPacket, AnotherPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

				//���� Release ó�� 
				pUser->m_dwStatus = 0;
				memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
				g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
				
				OnDisconnectUser(pUser->m_dwConnectionIndex);
				g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
				g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);

				pUser = NULL;
				return;
			}
			else
			{
				//�ߺ� �α��� �Ǿ��ٴ� �޼��� ������. 
				WSTC_LOGIN_FAIL	packet;
				packet.bResult = (BYTE)LOGIN_FAIL_OVERLAPPED_ID;
				SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			}

			dwPropID = atoi(LoginResult.szPropID);
			
			CUser* pPrevUser = g_pUserTable->GetUserInfo(dwPropID);

			if(pPrevUser)
			{
				if(pPrevUser->m_dwCurWorldServerPos == SERVER_POS_AGENT)	//Agent�� ���ӵǾ� �ִ� ���� 
				{
					WSTC_LOGIN_FAIL	packet;
					packet.bResult = (BYTE)LOGIN_FAIL_OVERLAPPED_ID;
					SendToUser(pPrevUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
					
				ASTWS_KICK_USER	KickPacket;
				//�ٸ� ���峪 ���� ������ ������ �ִٸ� ��� ������� ������ 
				KickPacket.dwCharIndex = pPrevUser->m_dwChar_Index;
				g_pServerTable->BroadCastWorldServer((char*)&KickPacket, KickPacket.GetPacketSize());
				
				pPrevUser->m_dwStatus = 0;

				OnDisconnectUser(pPrevUser->m_dwConnectionIndex);
				g_pNet->SetUserInfo(pPrevUser->m_dwConnectionIndex,NULL);
				g_pNet->CompulsiveDisconnectUser(pPrevUser->m_dwConnectionIndex);

				//���� ���̺��� �� ���� ���� 
				g_pUserTable->Remove(pPrevUser);		//Free�� �� 
				pPrevUser = NULL;
			}			

			ThrowLogoutQuery(pUser);			
			
			//���� Release ó�� 
			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			//g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			
			OnDisconnectUser(pUser->m_dwConnectionIndex);
			g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);

			pUser = NULL;

		}
		break;

	case 5:		//Block ���� ���̵� //billing check ���� �ƴ� 
		{
			if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )
			{
				Log(LOG_IMPORTANT, "Query size is overflow!");
				return;
			} // if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )

			LOGIN_RESULT_WITH_PROPID	LoginResult;
			memcpy(&LoginResult, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);
	
			WSTC_LOGIN_FAIL packet;
			packet.bResult = LOGIN_FAIL_USER_BLOCK;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	
			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
				
			//g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			
			OnDisconnectUser(pUser->m_dwConnectionIndex);
			g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);
			//--
		}
		break;

	case 6:		//14���̸� �ӽð����ڷ� �θ� ���� ó�� �� ���Ӱ����մϴ�. //billing check ���� �ƴ� 
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = LOGIN_FAIL_14_OVER;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			//g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 

			OnDisconnectUser(pUser->m_dwConnectionIndex);
			g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);			

		}
		break;
		
		//�̸� ĳ�� ���� ����ڰ� �ƴմϴ�. 
	case 11:
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = 11;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			//g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			
			OnDisconnectUser(pUser->m_dwConnectionIndex);
			g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);
		}
		break;

		//�ֵ�������~ �� 18�� �̻� ���� 
	case 12:
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = 12;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			//g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			
			OnDisconnectUser(pUser->m_dwConnectionIndex);
			g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);
		}
		break;

	//�Ǹ� ������ ���� ���� 
	case 13:
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = 13;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			
			OnDisconnectUser(pUser->m_dwConnectionIndex);
			g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);
		}
		break;

	default:
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = LOGIN_FAIL_ETC;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);

			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			
			OnDisconnectUser(pUser->m_dwConnectionIndex);
			g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);
		}
		break;
	}	
}

