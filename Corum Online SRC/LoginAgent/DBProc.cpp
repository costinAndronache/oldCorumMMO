#include "stdafx.h"
#include "DBProc.h"
#include "OwnServer.h"
#include "WorldServerTable.h"
#include "UserTable.h"
#include "recvmsg.h"
#include "ThrowQuery.h"
#include "User.h"
#include "PacketEncrypt.h"
#include "ServerFunction.h"
#include "../CommonServer/CommonHeader.h"
#include "ChinaBillingProc.h"

extern IOleDBThread*	g_pDb;
extern CTime			g_RecentCheckTime;
extern LPBASEITEM_HASH	g_pBaseItemHash;
extern MACHINENAME		g_MachineName;

void QueryTypeLoginBilling(DBRECEIVEDATA* pResult);

void QueryTypeLogin(DBRECEIVEDATA* pResult)
{
	if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//�߱����� �̸� : 050104 hwoarang
	{
		QueryTyprLoginChinaBilling(pResult);
	}
	else if( IS_ABLE_SERVICE_TYPE(ST_BILLING) )
	{
		QueryTypeLoginBilling(pResult);
	}
	else
	{
		QueryTypeLoginFree(pResult);
	}
}


void QueryTypeLoginFree(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);

	if(!pUser)
	{
		Log(LOG_IMPORTANT, "pUser is NULL!! in QueryTypeLogin()");
		return;
	}

	DWORD dwResult = *(DWORD*)(pResult->Query.select.pResult);

	switch(dwResult)
	{
	case 111: 
	case 0:		//Login OK	//billing check ���� �ƴ� 
		{
			if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )
			{
				Log(LOG_IMPORTANT, "Query size is overflow!");
				return;
			}

			LOGIN_RESULT_WITH_PROPID	LoginResult;
			ZeroMemory(&LoginResult,sizeof(LoginResult));//��� �߰�
			memcpy(&LoginResult, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);
			
			//2003�� 10�� 28�� �̺�Ʈ�� ��õ Event �ڵ� 
			pUser->m_bEventNum = EVENT_NUM_RECOM_ADD_ITEM;
		
			//���� ���̺� ���� �߰�
			pUser->m_dwStatus = USER_STATUS_LOGIN_SUCCESS;
			pUser->SetPropID(atoi(LoginResult.szPropID));

			pUser->m_ID_RegDate = LoginResult.id_regdate;//050323 ��� 

			//���� �����ؽ��� �ش� ���������� �����ִµ� �α���ó�� �� �� ��û�� �Ŷ��
			//���� �����ؽ� ������ ���� �����ִ� ó���� ���� 
			PrevUserProcess( pUser->GetPropID() );

			g_pUserTable->Add(pUser);

			if(dwResult == 111)
			{
				ThrowBlockTitleQuery(pUser, FALSE);
			}

			// �����۸����� ���� �������� �ִ��� ���� 
			ThrowItemMallQuery(pUser);
		}
		break;

	case 1:	//SQL DB ����
	case 2:	//���� ���̵�
	case 3: //��й�ȣ Ʋ��
		{
			SendToUserLoginFailPacket( pUser, (BYTE)dwResult, NULL );
			
			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;

	case 4:	//���� �������� ���̵�	//billing check ���� �ƴ� 
		{
			DWORD dwPropID = 0;

			// DB���� Logout ó�� 	
			if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )
			{
				Log(LOG_IMPORTANT, "Query size is overflow!");
				return;
			}

			LOGIN_RESULT_WITH_PROPID LoginResult;
			memcpy(&LoginResult, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);
			
			int nUserServerNum = atoi(g_pThis->GetServerSetCode());
			
			if(LoginResult.dwPrevUserServerSet != (DWORD)nUserServerNum)
			{
				SendToUserLoginFailPacket( pUser, (BYTE)LOGIN_FAIL_OVERLAPPED_ID_ANOTHER_SERVER
					, LoginResult.dwPrevUserServerSet );

				// ���� Release ó�� 
				pUser->m_dwStatus = 0;
				memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
				g_pUserTable->FreeUser(pUser);	// Alloc �ߴ� ���� �޸� Ǯ���� ���� 							
				g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
				pUser = NULL;

				return;
			}
			else
			{
				SendToUserLoginFailPacket( pUser, (BYTE)LOGIN_FAIL_OVERLAPPED_ID, NULL );
			}

			dwPropID = atoi(LoginResult.szPropID);
			
			CUser* pPrevUser = g_pUserTable->GetUserInfo(dwPropID);

			if(pPrevUser)
			{
				if(pPrevUser->m_dwCurWorldServerPos == SERVER_POS_AGENT)	
				{
					SendToUserLoginFailPacket( pPrevUser, (BYTE)LOGIN_FAIL_OVERLAPPED_ID, NULL );
				}
					
				// �ٸ� ���峪 ���� ������ ������ �ִٸ� ��� ������� ������ 
				ASTWS_KICK_USER	KickPacket;
				KickPacket.dwCharIndex = pPrevUser->m_dwChar_Index;
				g_pServerTable->BroadCastWorldServer((char*)&KickPacket, KickPacket.GetPacketSize());
				
				pPrevUser->m_dwStatus = 0;
				memset(pPrevUser->m_szLoginID, 0, MAX_ID_LENGTH);
				g_pNet->SetUserInfo(pPrevUser->GetConnectionIndex(), NULL);				
				//���� ���̺��� �� ���� ���� Free�� �� 
				g_pUserTable->Remove(pPrevUser);		
				pPrevUser = NULL;
			}

			ThrowLogoutQuery(pUser);
			
			// ���� Release ó�� 
			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);		
		}
		break;

	case 5:		//Block ���� ���̵� //billing check ���� �ƴ� 
		{
			if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )
			{
				Log(LOG_IMPORTANT, "Query size is overflow!");
				break;
			} // if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )

			LOGIN_RESULT_WITH_PROPID	LoginResult;
			memcpy(&LoginResult, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);
	
			if( IS_ABLE_NATION(NC_JAPAN))
			{
				if(LoginResult.dwCoupon == 1) // �Ϻ� block! �� ���� �о�;��Ѵ�.
				{
					ThrowBlockTitleQuery(pUser, TRUE);
					break;
				} // if(LoginResult.dwCoupon == 1) // �Ϻ� block! �� ���� �о�;��Ѵ�.
			} // if( IS_ABLE_NATION(NC_JAPAN))
		
			SendToUserLoginFailPacket( pUser, LOGIN_FAIL_USER_BLOCK, NULL );
	
			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);				
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;

	case 6:		//14���̸� �ӽð����ڷ� �θ� ���� ó�� �� ���Ӱ����մϴ�. //billing check ���� �ƴ� 
		{
			SendToUserLoginFailPacket( pUser, LOGIN_FAIL_14_OVER, NULL );

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;
		
	case 11:	//�̸� ĳ�� ���� ����ڰ� �ƴմϴ�. 
		{
			SendToUserLoginFailPacket( pUser, 11, NULL );

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;

		
	case 12:	//�ֵ�������~ �� 18�� �̻� ���� 
		{
			SendToUserLoginFailPacket( pUser, 12, NULL );

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;

	case 13:	//�Ǹ� ������ ���� ���� 
		{
			SendToUserLoginFailPacket( pUser, 13, NULL );

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;

	case LOGIN_FAIL_NOT_BETA_TESTER:	// �Ϻ�, ��Ÿ�׽��� �ƴ� 
		{
			SendToUserLoginFailPacket( pUser, LOGIN_FAIL_NOT_BETA_TESTER, NULL );

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;

	case LOGIN_FAIL_EJECT_USER_JAPAN:	//�Ϻ�, ������ 
		{
			SendToUserLoginFailPacket( pUser, LOGIN_FAIL_EJECT_USER_JAPAN, NULL );

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);			
		}
		break;

	case LOGIN_FAIL_SECESSION_USER:		//�Ϻ�, Ż���� ȸ��
		{
			SendToUserLoginFailPacket( pUser, LOGIN_FAIL_SECESSION_USER, NULL );

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);		
		}
		break;
	
	default:
		{
			SendToUserLoginFailPacket( pUser, LOGIN_FAIL_ETC, NULL );

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;
	}	
}


void QueryTypeLoginBilling(DBRECEIVEDATA* pResult)
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
	case 0:	//���� ���ÿ�! 
		{
			WSTC_LOGIN_FAIL	packet;
			packet.bResult = (BYTE)dwResult;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;
		
	case 111:	//Login OK with Warning		//Added by KBS 040309
	case 1:		//Login OK
		{
			if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )
			{
				Log(LOG_IMPORTANT, "Query size is overflow!");
				return;
			}

			LOGIN_RESULT_WITH_PROPID	LoginResult;
			memcpy(&LoginResult, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);
			
			pUser->m_bEventNum = EVENT_NUM_RECOM_ADD_ITEM;
			
			//���� ���̺� ���� �߰�
			pUser->m_dwStatus = USER_STATUS_LOGIN_SUCCESS;
			pUser->SetPropID(atoi(LoginResult.szPropID));
			pUser->m_BillingEday = LoginResult.eday;
			pUser->m_ID_RegDate = LoginResult.id_regdate;//050323 ��� 
			pUser->m_dwRemainTime = LoginResult.dwRemainTime;
			pUser->m_bBillingType = (BYTE)LoginResult.dwRestrictEx;

			//���� �����ؽ��� �ش� ���������� �����ִµ� �α���ó�� �� �� ��û�� �Ŷ�� 
			//���� �����ؽ� ������ ���� �����ִ� ó���� ���� 
			PrevUserProcess( pUser->GetPropID() );

			g_pUserTable->Add(pUser);

			// �Ϻ� ��� ������ ��� Ÿ��Ʋ ȹ��!
			if(dwResult == 111)
			{
				ThrowBlockTitleQuery(pUser, FALSE);
			}
			
			// Item Mall�� ���� ���°� �ִ��� üũ billing check ���� 
			ThrowItemMallQuery(pUser);
		}
		break;

	case 7:	//SQL DB ����
	case 2:	//���� ���̵�
	case 3: //��й�ȣ Ʋ��
		{
			WSTC_LOGIN_FAIL	packet;
			packet.bResult = (BYTE)dwResult;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);						 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;


	case 4:		//���� �������� ���̵� 
		{
			DWORD dwPropID = 0;
			
			if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )
			{
				Log(LOG_IMPORTANT, "Query size is overflow!");
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
				g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
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
	
			//�������� ���� �ִ� ������ ������ ã�Ƽ� 
			dwPropID = atoi(LoginResult.szPropID); // added by deepdark... �´°ų�??
			CUser* pPrevUser = g_pUserTable->GetUserInfo(dwPropID);

			if(pPrevUser)
			{
				if(pPrevUser->m_dwCurWorldServerPos == SERVER_POS_AGENT)	//Agent�� ���ӵǾ� �ִ� ���� 
				{
					WSTC_LOGIN_FAIL	packet;
					packet.bResult = (BYTE)LOGIN_FAIL_OVERLAPPED_ID;
					SendToUser(pPrevUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);					
				}
				
				// �ٸ� ���峪 ���� ������ ������ �ִٸ� ��� ������� ������ 
				ASTWS_KICK_USER	KickPacket;
				KickPacket.dwCharIndex = pPrevUser->m_dwChar_Index;
				g_pServerTable->BroadCastWorldServer((char*)&KickPacket, KickPacket.GetPacketSize());
				
				// ���� ���̺��� �� ���� ���� Free�� �� 
				pPrevUser->m_dwStatus = 0;
				g_pNet->SetUserInfo(pPrevUser->GetConnectionIndex(), NULL);				
				g_pUserTable->Remove(pPrevUser);		
				pPrevUser = NULL;
			}
			else
			{
				Log(LOG_NORMAL, "Login overlapped but prev userinfo is not exist!");
			}

			//DB���� ���������� �α׾ƿ� ó�� �ϰ� 
			dwPropID = atoi(LoginResult.szPropID);
			
			ThrowLogoutQuery(pUser);

			//���� Release ó�� 
			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
			pUser = NULL;

		}
		break;

	case 5:		//Block ���� ���̵� 
		{
			if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )
			{
				Log(LOG_IMPORTANT, "Query size is overflow!");
				return;
			} // if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )

			LOGIN_RESULT_WITH_PROPID	LoginResult;
			memcpy(&LoginResult, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);

			if( IS_ABLE_NATION(NC_JAPAN) )
			{
				if(LoginResult.dwCoupon == 1)
				{
					ThrowBlockTitleQuery(pUser, TRUE);
					break;
				} // if(LoginResult.dwCoupon == 1)
			} // if( IS_ABLE_NATION(NC_JAPAN))

			WSTC_LOGIN_FAIL packet;
			packet.bResult = LOGIN_FAIL_USER_BLOCK;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	
			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
				
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
			//--
				
		}
		break;

	case 6:		//14���̸� �ӽð����ڷ� �θ� ���� ó�� �� ���Ӱ����մϴ�.	//billing check ���� 
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = LOGIN_FAIL_14_OVER;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);

		}
		break;
		
		//�̸� ĳ�� ���� ����ڰ� �ƴմϴ�.  //billing check ���� 
	case 11:
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = 11;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;

		//�ֵ�������~ �� 18�� �̻� ����		//billing check ���� 
	case 12:
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = 12;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;

	//�Ǹ� ������ ���� ���� //billing check ���� 
	case 13:
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = 13;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;

		//Ż���� ȸ�� //billing check ���� 
	case 50:
		{
			WSTC_LOGIN_FAIL	packet;
			packet.bResult = (BYTE)dwResult;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
			
		}
		break;

		//�Ϻ�, ��Ÿ�׽��� �ƴ� //billing check ���� 
	case 14:
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = LOGIN_FAIL_NOT_BETA_TESTER;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;

		//�Ϻ�, ������ //billing check ���� 
	case 15:
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = LOGIN_FAIL_EJECT_USER_JAPAN;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);			
		}
		break;

		//�Ϻ�, Ż���� ȸ��//billing check ���� 
	case 16:
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = LOGIN_FAIL_SECESSION_USER;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);		
		}
		break;
	
	default://billing check ���� 
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = LOGIN_FAIL_ETC;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);

			g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;
	}	
}


void QueryTypeChrSelectInfo(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if (!pUser)	return;
	
	if(pResult->nError < 0)
	{
		Log(LOG_IMPORTANT, "Query Error: QueryTypeChrSelectInfo()...  (User: %s)", pUser->m_szLoginID);
		return;
	}
	
	RS_CharacterLogInfo_With_Equip EnumChar[ 4 ];
	memset(EnumChar, 0, sizeof(EnumChar));

	if( pResult->Query.select.dwRowCount > 4 )
		__asm int 3;
	
	memcpy(EnumChar, pResult->Query.select.pResult, sizeof(RS_CharacterLogInfo_With_Equip) * pResult->Query.select.dwRowCount );
		
	WSTC_LOGIN_SUCCESS packet;
#ifdef ADULT_MODE
	packet.bAdultMode = TRUE;
#else
	packet.bAdultMode = FALSE;
#endif
	packet.bNum = (BYTE)pResult->Query.select.dwRowCount;
	
	for(int i = 0; i < packet.bNum; i++)
	{
		if(EnumChar[i].byInvItemCheck==0)
		{
			for(int j = 0; j < MAX_EQUIP; j++)
			{
				if(EnumChar[i].Equip[j].m_wItemID!=0)
				{
					WORD wOldId = EnumChar[i].Equip[j].m_wItemID;
					EnumChar[i].Equip[j].m_wItemID = (WORD)(( wOldId / 100 * 200 ) + wOldId % 100);
				}
			}			
		}
	}

	// ( sizeof(CItem)*MAX_EQUIP );		//Equip�� ������ ����ü ������ 
	int		nSize	= sizeof(RS_CharacterLogInfo_Without_Equip);
	WORD	wIndex	= 0;

	for(int i = 0; i < packet.bNum; i++)
	{
		wIndex = EnumChar[i].wChrNum;

		//�߸��� CharacterIndex �϶� 
		if(wIndex < 1 || wIndex > 4)	
		{
			WSTC_LOGIN_FAIL	FailPacket;	
			FailPacket.bResult = LOGIN_FAIL_INVALID_CHARACTER_INDEX;	
			SendToUser(pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			memset(pUser->m_EnumChar, 0, sizeof(pUser->m_EnumChar));
			Log(LOG_IMPORTANT, "Character Overlapped! (login_id:%s)", pUser->m_szLoginID);
			return;
		}
		
		wIndex -= 1;	//�迭 ZeroBase�� 
		
		//dwStr���� ������ �̹� �� CharacterNum�� ĳ���Ͱ� �ִ°ɷ� ��������.
		if(pUser->m_EnumChar[ wIndex ].dwStr > 0)
		{
			WSTC_LOGIN_FAIL	FailPacket;	
			FailPacket.bResult = LOGIN_FAIL_CHARACTER_OVERLAPPED;	
			SendToUser(pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			memset(pUser->m_EnumChar, 0, sizeof(pUser->m_EnumChar));
			Log(LOG_IMPORTANT, "Character Overlapped! (login_id:%s)", pUser->m_szLoginID);
			return;
		}

		// ���� ����ü���� ChaNum ������� ��������!
		memcpy(&pUser->m_EnumChar[ wIndex ], &EnumChar[i], nSize);			
		memcpy(&packet.Character[i], &EnumChar[i], nSize );	
		memcpy(&pUser->m_EnumChar[ wIndex ].sCreateTime, &EnumChar[i].sCreateTime, sizeof(pUser->m_EnumChar[ wIndex ].sCreateTime));
		pUser->m_EnumChar[ wIndex ].byInvItemCheck = EnumChar[i].byInvItemCheck;
		
		BYTE byLHand = EQUIP_TYPE_LHAND1;
		BYTE byRHand = EQUIP_TYPE_RHAND1;
		
		if(EnumChar[i].dwGuildId!=0)
		{
			pUser->m_EnumChar[wIndex].dwGuildId = EnumChar[i].dwGuildId;
			pUser->m_EnumChar[wIndex].byRank	= EnumChar[i].byRank;
		}

		if(EnumChar[i].byCurrentHand==2)
		{
			byLHand = EQUIP_TYPE_LHAND2;		
			byRHand = EQUIP_TYPE_RHAND2;
		}	
		
		packet.Character[i].wLHandID	= EnumChar[i].Equip[ byLHand ].m_wItemID;			//�޼� ����
		packet.Character[i].wRHandID	= EnumChar[i].Equip[ byRHand ].m_wItemID;			//������ ����
		packet.Character[i].wHelmetID	= EnumChar[i].Equip[ EQUIP_TYPE_HELMET ].m_wItemID;	//���
		packet.Character[i].wMailID		= EnumChar[i].Equip[ EQUIP_TYPE_ARMOR ].m_wItemID;	//����
		packet.Character[i].wMantleID	= EnumChar[i].Equip[ EQUIP_TYPE_MANTLE ].m_wItemID;	//����
	}

	SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

	//Item �� ó���� �ִ� ������ ��� 
	if(pUser->m_bItemMallCount > 0)
	{
		ASTC_ITEMMALL_INFO MallPacket;
		BYTE byCharNum = 0;

		for(int i = 0; i < 4; i++)
		{
			if(pUser->m_EnumChar[i].dwCharacterIndex==0)
				byCharNum++;
		}

		if(byCharNum==4)
			pUser->m_byItemMallType = 0xff;

		Log(LOG_NORMAL, "Client ItemMall : %u, %u", pUser->m_byItemMallType, pUser->m_bItemMallCount);
	
		if(pUser->m_byItemMallType==0xff)
		{
			MallPacket.bItemCount		= pUser->m_bItemMallCount;
			MallPacket.bSuccess			= FALSE;
			MallPacket.byItemMallType	= 0xff;
		}
		else if(pUser->m_wItemMallSupplyCount >= 150)
		{
			pUser->m_wItemMallSupplyCount	= 0;
			MallPacket.bItemCount			= pUser->m_bItemMallCount;
			MallPacket.bSuccess				= FALSE;
			MallPacket.byItemMallType		= 0;
		}
		else
		{
			MallPacket.bItemAllCount	= pUser->m_bItemMallCount;
			MallPacket.bItemCount		= (BYTE)pUser->m_wItemMallSupplyCount;
			MallPacket.bSuccess			= TRUE;
			MallPacket.byItemMallType	= 0;
		}
		
		SendToUser(pUser, (char*)&MallPacket, MallPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}


void QueryTypeCheckMonitoring(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if (NULL == pUser)	return;
	
	g_pUserTable->AddMonitoringTool(pUser->GetConnectionIndex());

	Log(LOG_JUST_DISPLAY, "Monitoring Tool login.. (ID:%s, Name:%s)", pUser->m_szLoginID, pUser->GetSelectedCharacter()->szName);
	pUser->m_bIsMonitor = TRUE;	
}


void QueryTypeIsAliveUser(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if( pUser == NULL )		return;

	ASTC_IS_ALIVE_USER Packet;

	if(pResult->Query.select.dwRowCount == 0)
	{
		Packet.bIsAliveUser = 0;
		SendToUser( pUser, (char*)&Packet, Packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	}
	else
	{
		Packet.bIsAliveUser = 1;
		SendToUser( pUser, (char*)&Packet, Packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	}
}

void QueryTypeBillingCheck(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if(!pUser) return;

	if(pResult->nError < 0)
	{
		Log(LOG_IMPORTANT, "Billing Check Query Error!");
		return;
	}

	BILLING_CHECK_RESULT Result;
	memcpy(&Result, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);

	if( IS_ABLE_SERVICE_TYPE(ST_DEVELOP) )//DEV_MODE : 050104 Hwoarang
	{
		char szTemp[1024]={0,};
		wsprintf(szTemp, "PropID:%d, BillingType:%d, eDay:%d-%d-%d %d:%d:%d, RemainTime:%d, Continue:%d", Result.dwPropID, Result.nBillingType, 
					Result.eday.year, Result.eday.month, Result.eday.day, Result.eday.hour, Result.eday.minute, Result.eday.second,
					Result.dwRemainTime, Result.dwContinue);

		OutputDebugString(szTemp);
		OutputDebugString("\n\n");
	}

	//���� ���� ����. 
	if(!Result.dwContinue)
	{
		char szProc[0xff]={0,};
		wsprintf(szProc, "EXEC up_GameLogout '%s'", pUser->m_szLoginID );	
		g_pDb->THExecuteSQL(szProc, FALSE, 0, NULL, (BYTE)MEMBER_DB);	//DB���� �α��� ���� ���� 
		
		ASTWS_KICK_USER packet;
		packet.dwCharIndex = pUser->m_dwChar_Index;
		g_pServerTable->BroadCastWorldServer((char*)&packet, packet.GetPacketSize());

		g_pUserTable->Remove(pUser);
		Log(LOG_NORMAL, "User Billing Expired! (ID:%s, Name:%s)", pUser->m_szLoginID, pUser->GetSelectedCharacter()->szName);
		return;
	}

	//���� Ÿ���� ����Ǿ����� 
	if(pUser->m_bBillingType != Result.nBillingType)
	{
		ASTWS_CHANGE_BILLING_TYPE	ChangePacket;

		ChangePacket.bBillingType = (BYTE)Result.nBillingType;
		ChangePacket.dwChar_Index = pUser->m_dwChar_Index;
		ChangePacket.dwRemainTime = Result.dwRemainTime;
		ChangePacket.eDay = Result.eday;
		g_pServerTable->BroadCastWorldServer((char*)&ChangePacket, ChangePacket.GetPacketSize());

		pUser->m_dwBillingStartTick = timeGetTime();	//Ÿ�� ���濡 ���� �������۽ð� ���� 
	}

	pUser->m_bBillingType = (BYTE)Result.nBillingType;
	pUser->m_BillingEday  = Result.eday;
	pUser->m_dwRemainTime = Result.dwRemainTime;
	
	switch(Result.nBillingType)
	{
	case USER_BILLING_FLAT_RATE:
		{
			if(Result.eday.year == 0)	
			{
				Log(LOG_IMPORTANT, "eday is NULL! (USER_BILLING_FLAT_RATE)");
				return;
			}

			CTime EndDay(Result.eday.year, Result.eday.month, Result.eday.day, Result.eday.hour, Result.eday.minute, Result.eday.second);
			CTimeSpan ts = EndDay - g_RecentCheckTime;

			if(ts.GetTotalMinutes() < 10)	//������ ���� ���ڰ� 10�� �������� 
			{
				int nSecond = ts.GetTotalSeconds();

				if(nSecond >= 0)
				{
					//��� ���Ҵ� �˷���� �ȴ�. 
					ASTWS_BILLING_SMALL_TIME	packet;
					packet.bBillingType = (BYTE)Result.nBillingType;
					packet.dwChar_Index = pUser->m_dwChar_Index;
					packet.dwSecond = nSecond;
					g_pServerTable->BroadCastWorldServer((char*)&packet, packet.GetPacketSize());
				}
				else
				{
					Log(LOG_NORMAL, "Clock Mismatch!");
				}
			}
		}
		break;
		
	case PCROOM_BILLING_FLAT_RATE:
		{
			if(Result.eday.year == 0)	
			{
				Log(LOG_IMPORTANT, "eday is NULL! (PCROOM_BILLING_FLAT_RATE)");
				return;
			}

			CTime EndDay(Result.eday.year, Result.eday.month, Result.eday.day, Result.eday.hour, Result.eday.minute, Result.eday.second);
			CTimeSpan ts = EndDay - g_RecentCheckTime;

			if(ts.GetTotalHours() < 48)		//������ ���� ���ڰ� 2�� ������ �˸� 
			{
				int nSecond = ts.GetTotalSeconds();

				if(nSecond >= 0)
				{
					//��� ���Ҵ� �˷���� �ȴ�. 
					ASTWS_BILLING_SMALL_TIME	packet;
					packet.bBillingType = (BYTE)Result.nBillingType;
					packet.dwChar_Index = pUser->m_dwChar_Index;
					packet.dwSecond = nSecond;
					g_pServerTable->BroadCastWorldServer((char*)&packet, packet.GetPacketSize());
				}
				else
				{
					Log(LOG_NORMAL, "Clock Mismatch!");
				}
			}
		}
		break;
	case PCROOM_BILLING_SOMEHOUR_FREE:
		{
			if( IS_ABLE_NATION(NC_TAIWAN) )
			{//Ÿ�̿��̰� ����üũ�ϸ� �������� ����
				if( !IS_ABLE_SERVICE_TYPE(ST_BILLING) )//�Ϲݺ��� :050104 hwoarang
				{
					break;
				}
			} // if( IS_ABLE_NATION(NC_TAIWAN) )
			else
			{
				break;
			}
		}
	case USER_BILLING_1HOUR_FREE:
	case USER_BILLING_METER_RATE:
		{
			if(Result.dwRemainTime < 600)	//���� �ð��� 10�� �̸��϶� 
			{
				//��� ���Ҵ� �˷���� �ȴ�. 
				ASTWS_BILLING_SMALL_TIME	packet;
				packet.bBillingType = (BYTE)Result.nBillingType;
				packet.dwChar_Index = pUser->m_dwChar_Index;
				packet.dwSecond = Result.dwRemainTime;
				g_pServerTable->BroadCastWorldServer((char*)&packet, packet.GetPacketSize());
			}
		}
		break;

	case PCROOM_BILLING_METER_RATE:
		{
			if(Result.dwRemainTime < 18000)	//���� �ð��� 5�ð� �̸��϶� 
			{
				//��� ���Ҵ� �˷���� �ȴ�. 
				ASTWS_BILLING_SMALL_TIME	packet;
				packet.bBillingType = (BYTE)Result.nBillingType;
				packet.dwChar_Index = pUser->m_dwChar_Index;
				packet.dwSecond = Result.dwRemainTime;
				g_pServerTable->BroadCastWorldServer((char*)&packet, packet.GetPacketSize());
			}
		}
		break;
	}
}


void QueryTypeBankItemCheck(DBRECEIVEDATA* pResult)
{
	char	szQuery[QUERY_STRING_SIZE]		= {0,};
	BYTE	bBankItemCheck[4]	= {0,};
	BOOL	bIsBankItemCheck	= FALSE;

	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if(!pUser) return;

	int nRet = pResult->Query.select.dwRowCount;

	if(nRet > 0)
	{
		memcpy(bBankItemCheck,pResult->Query.select.pResult,pResult->Query.select.dwRowCount);

		for(int i = 0; i < 4; i++)
		{
			if(bBankItemCheck[i] == 1)
			{
				bIsBankItemCheck = TRUE;
				break;
			}
		}		

		wsprintf(szQuery, "Update CharacterInfo Set InvItemCheck = %u, BankItemCheck = %u Where Name = '%s'", 1, bIsBankItemCheck, pUser->GetSelectedCharacter()->szName);
		g_pDb->THExecuteSQL(szQuery, FALSE, FALSE, NULL, (BYTE)GAME_DB);	
	}	
}


void QueryTypeCreateNewChar(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if(!pUser) return;

	if(pResult->nError<0)
	{
		WSTC_CREATE_CHARACTER_FAIL	FailPacket;
		pUser->m_bClass = 0;
		FailPacket.nResultValue = -100;
		memset(pUser->GetSelectedCharacter()->szName, 0, MAX_CHARACTER_NAME_LENGTH);	// ���� �����̴� CharacterName ������ 
		pUser->m_bSelectedChrIndex = 0;
		
		SendToUser( pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	}

	CREATE_CHARACTER_RESULT Result;
	memcpy(&Result, pResult->Query.select.pResult , pResult->Query.select.dwRowSize);
			
	switch(Result.dwResult)
	{
		case 1:	//ĳ���� �߰� ���� 
		{
			pUser->m_EnumChar[ pUser->m_bSelectedChrIndex ].dwCharacterIndex = Result.dwChar_Index;

			char szQuery[QUERY_STRING_SIZE] = {0,};
						
			wsprintf(szQuery, "Select BankItemCheck From CharacterInfo Where ID = '%s'",pUser->m_szLoginID);
			g_pDb->THOpenRecord(szQuery,QUERY_TYPE_BANKITEM_CHECK,(void*)pUser->m_dwConnectionIndex,4,(BYTE)GAME_DB);
			
			//��� 050323 .
			pUser->m_dwCharCount = Result.dwCharCount; //������� ĳ���� �� ���� ����.			
			ThrowCreateCharInitBinaryQuery(pUser);
			

			ThrowInsertWebBoardQuery(pUser, pUser->GetSelectedCharacter()->szName, pUser->m_bClass);

			break;
		}
		break;

		case -1:	//�̹� �ִ� ĳ���� 
		{
			WSTC_CREATE_CHARACTER_FAIL	FailPacket;
			FailPacket.nResultValue = -1;

			SendToUser(pUser, (char*)&FailPacket, FailPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
			
			memset(pUser->m_EnumChar[ pUser->m_bTryCreateCharSlot ].szName, 0, MAX_CHARACTER_NAME_LENGTH);
			pUser->m_bTryCreateCharSlot = 0xff;			
		}
		break;

		case -2:	//�ڸ� �ߺ� 
		{
			WSTC_CREATE_CHARACTER_FAIL	FailPacket;
			FailPacket.nResultValue = -2;

			SendToUser(pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

			memset(pUser->m_EnumChar[ pUser->m_bTryCreateCharSlot ].szName, 0, MAX_CHARACTER_NAME_LENGTH);
			pUser->m_bTryCreateCharSlot = 0xff;
		}
		break;
	}
}

void QueryTypeDeleteCharacter(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData); 
	if(!pUser) return;

	if(pResult->nError<0)
	{
		WSTC_DELETE_CHARACTER_RESULT	FailPacket;
		FailPacket.bSuccess = FALSE;
		FailPacket.nResultValue = -100;		//���� ���� 
		SendToUser( pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	}

	DELETE_CHARACTER_RESULT DeleteResult;	memset(&DeleteResult, 0, sizeof(DELETE_CHARACTER_RESULT));
	memcpy(&DeleteResult, pResult->Query.select.pResult , pResult->Query.select.dwRowSize);

	switch(DeleteResult.dwResult)
	{
		case 2:		//��δ� ������ ��� 
		case 1:
		{
			for(int i = 0; i < 4; i++)
			{
				if(pUser->m_EnumChar[i].dwCharacterIndex!=0)
				{
					if(__strcmp(DeleteResult.szDeletedCharacterName, pUser->m_EnumChar[i].szName)==0)
					{
						memset(&pUser->m_EnumChar[i], 0, sizeof(pUser->m_EnumChar[i]));
						break;
					}
				}				
			}						

			WSTC_DELETE_CHARACTER_RESULT	SuccessPacket;
			SuccessPacket.bSuccess = TRUE;
			SuccessPacket.nResultValue = DeleteResult.dwResult;

			SendToUser(pUser, (char*)&SuccessPacket, SuccessPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

			ThrowDeleteWebBoardQuery(pUser, DeleteResult.szDeletedCharacterName);
		}
		break;

		case 4:		//�߸��� ĳ���� ���� �õ� 
		{
			WSTC_DELETE_CHARACTER_RESULT	Packet;
			Packet.bSuccess = FALSE;
			Packet.nResultValue = DeleteResult.dwResult;

			SendToUser(pUser, (char*)&Packet, Packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		}
		break;
	}
}


void QueryTypeJuminCheck(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if(!pUser) return;	

	if(pResult->nError<0)
	{
		WSTC_DELETE_CHARACTER_RESULT	FailPacket;
		FailPacket.bSuccess = FALSE;
		FailPacket.nResultValue = -101;		//���� ���� 
		SendToUser( pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		Log(LOG_IMPORTANT, "QueryTypeJuminCheck Failed! (ID:%s)", pUser->m_szLoginID );
		return;
	}

	JUMIN_CHECK_RESULT result;
	memcpy(&result, pResult->Query.select.pResult, sizeof(JUMIN_CHECK_RESULT));

	if(result.dwResult == 1)	//�ֹι�ȣ ���� 
	{	
		BYTE byIndex = 0;

		for(BYTE i = 0; i < 4; i++)
		{
			if(pUser->m_EnumChar[i].dwCharacterIndex!=0)
			{
				if(__strcmp(result.szCharacterName, pUser->m_EnumChar[i].szName)==0)
				{
					byIndex = i;
					break;
				}
			}
		}

		if(byIndex>=4)
		{
			WSTC_DELETE_CHARACTER_RESULT pPacket;
			pPacket.bSuccess		= FALSE;
			pPacket.nResultValue	= 11;
			SendToUser(pUser, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		}
		else
		{					
			if(pUser->m_EnumChar[byIndex].sCreateTime.year==0 || pUser->m_EnumChar[byIndex].sCreateTime.month==0 || pUser->m_EnumChar[byIndex].sCreateTime.day==0)
			{
				ThrowDeleteCharQuery(pUser, result.szCharacterName);
			}
			else
			{
				CTime cCreateTime(pUser->m_EnumChar[byIndex].sCreateTime.year, pUser->m_EnumChar[byIndex].sCreateTime.month, pUser->m_EnumChar[byIndex].sCreateTime.day, pUser->m_EnumChar[byIndex].sCreateTime.hour, pUser->m_EnumChar[byIndex].sCreateTime.minute, pUser->m_EnumChar[byIndex].sCreateTime.second);
				CTime cCurrentTime = CTime::GetCurrentTime();

				CTimeSpan ts = cCurrentTime - cCreateTime;

				if(ts.GetTotalHours()>=24)
				{
					ThrowDeleteCharQuery(pUser, result.szCharacterName);
				}
				else
				{
					WSTC_DELETE_CHARACTER_RESULT pPacket;
					pPacket.bSuccess		= FALSE;
					pPacket.nResultValue	= 10;
					SendToUser(pUser, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
				}
			}					
		}
	}
	else	//�ֹι�ȣ Ʋ�� 
	{
		WSTC_DELETE_CHARACTER_RESULT	FailPacket;
		FailPacket.bSuccess = FALSE;
		FailPacket.nResultValue = 5;		//�ֹε�Ϲ�ȣ Ʋ�� 
		SendToUser( pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	}
}


void QueryTypeCreateCharInitBinary(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if (!pUser)	return;
	
	if(pResult->nError >= 0)
	{
		WSTC_CREATE_CHARACTER_SUCCESS	packet;
		LPBASE_CLASS_INFO	pInfo = g_pUserTable->GetBaseClassInfo(pUser->m_bClass);

		packet.dwStr = pInfo->iSTR;
		packet.dwVit = pInfo->iVIT;
		packet.dwDex = pInfo->iDEX;
		packet.dwInt = pInfo->iINT;
		packet.dwEgo = pInfo->iEGO;
		SendToUser( pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	}
	else
	{
		pUser->m_bClass				= 0;
		pUser->m_bSelectedChrIndex	= 0;

		// ���� �����̴� CharacterName ������ 
		memset(pUser->GetSelectedCharacter()->szName, 0, MAX_CHARACTER_NAME_LENGTH);	

		WSTC_CREATE_CHARACTER_FAIL	FailPacket;
		SendToUser( pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );	
	}
}


void QueryTypeGMCheck(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);

	if(!pUser)
	{
		Log(LOG_NORMAL, "WorldUser Pointer is NULL in QueryTypeGMCheck!");
		return;
	}

	if(pResult->Query.select.dwRowCount == 0)
	{
		WSTC_INVALID_GM packet;
		SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		return;
	}
	
	// DB�� ��ϵ� GM IP
	char szRegisteredIP[16]={0,};
	memcpy(szRegisteredIP, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);
	
	// ���� ������ Client�� IP
	char szIp[16]={0,};	WORD wPort=0;
	g_pNet->GetUserAddress(pUser->GetConnectionIndex(), szIp, &wPort);

	// �ٸ� ��� GM�� �ƴѰɷ� ���� 
	if(!__strcmp(szIp, szRegisteredIP))
	{
		WSTC_GM_LOGIN_SUCCESS packet;
		SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
	else
	{
		WSTC_INVALID_GM packet;
		SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}	
}


void QueryTypeBlockTitle(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);

	if(!pUser)
	{
		Log(LOG_NORMAL, "WorldUser Pointer is NULL in QueryTypeBlockTitle!");
		return;
	}

	// ��� ��������̱�� �ϳ� DB�� Block ������ ���� ��� 
	if(pResult->Query.select.dwRowCount == 0)
	{
		WSTC_LOGIN_FAIL packet;
		packet.bResult = LOGIN_FAIL_USER_BLOCK;
		SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	
		pUser->m_dwStatus = 0;
		memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
				
		g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
		g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		return;
	}

	BLOCK_TITLE BlockTitle;
	memcpy(&BlockTitle, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);

	if(BlockTitle.dwType == 2)	//�α��� ���� ���� 
	{
		WSTC_BLOCK_ACCOUNT	packet;
		memcpy(packet.szReason, BlockTitle.szReason, strlen(BlockTitle.szReason) );
		packet.bLen = (BYTE)strlen(BlockTitle.szReason);
		SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}

	pUser->m_dwStatus = 0;
	memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
				
	g_pUserTable->FreeUser(pUser);							//Alloc �ߴ� ���� �޸� Ǯ���� ���� 
	g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
}


void QueryTypeWarningTitle(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);

	if(!pUser)
	{
		Log(LOG_NORMAL, "WorldUser Pointer is NULL in QueryTypeWarningTitle!");
		return;
	}

	if(pResult->Query.select.dwRowCount > 0)		//��� �ִ°�� 
	{
		BLOCK_TITLE WarningTitle;
		memcpy(&WarningTitle, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);

		WSTC_WARNING_TITLE	packet;
		memcpy(packet.szReason, WarningTitle.szReason, strlen(WarningTitle.szReason) );
		packet.bLen = (BYTE)strlen(WarningTitle.szReason);
		SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}
