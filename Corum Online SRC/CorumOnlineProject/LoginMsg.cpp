#include "stdafx.h"
#include "LoginProcess.h"
#include "InitGame.h"
#include "CorumResource.h"
#include "DefResource.h"
#include "GameControl.h"
#include "SpriteManager.h"
#include "develop.h"
#include "ObjectManager.h"
#include "HashTable.h"
#include "../CommonServer/AbyssHash.h"
#include "GameDefaultWnd.h"
#include "CharSelectProcess.h"
#include "WorldProcess.h"
#include "DungeonProcess.h"
#include "GlobalSoundHandles.h"
#include "message.h"
#include "GameMenuWnd.h"
#include "ErrorWnd.h"
#include "IMEEdit.h"
#include "InputManager.h"
#include "NetworkClient.h"
#include "BlockWnd.h"
#include "LoginMsg.h"
#include "ChinaBillingRemainTime.h"
#include "PacketEncrypt.h"

void CmdLoginFail( char* pMsg, DWORD dwLen )
{
	WSTC_LOGIN_FAIL*	pPacket			= (WSTC_LOGIN_FAIL*)pMsg;

	// �߱� ���� ���� ���� �α��� ���� ó��!!
	if(pPacket->bResult >= 0xf0)
	{
		OnChinaBillingLoginFail(pPacket->bResult);
		return;
	}

	CGameDefaultWnd*	pGameDefaultWnd = CGameDefaultWnd::GetInstance();

	g_pGVLogin->bIsLoging = FALSE;

	if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)
	{
		g_NetMarble.bLoging = FALSE;
	}

	//0: ���Ʋ��, 2: ���¾��̵� , 3: ���̵�����, 4: ���� �ִ��ο� �ʰ� 
	//1: �α��� ����(������)
	switch(pPacket->bResult)
	{
	case LOGIN_FAIL_NO_PAY:
		{
			//"������ ���������� �����ϴ�."	//
			//"���� ��뼭�� ���Դϴ�."		//
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE832].szMessage, g_Message[ETC_MESSAGE833].szMessage, "", 402, 459, 2, TRUE);									
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_ID);
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
		}
		break;
	case LOGIN_FAIL_EJECT_USER:		//Ż���� ȸ�� 
		{
			// "Ż���� ȸ���Դϴ�."	//
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE834].szMessage, "", "", 402, 459, 1, TRUE);								
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}			
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_ID);
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
		}
		break;	
	case LOGIN_FAIL_SQL_ERROR:
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE592].szMessage, "", "", 402, 459, 1, TRUE); // "SQL �����Դϴ�."				
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
			// ��й�ȣ�� Focus �ְ� �����̰� //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);			
		}
		break;
	case LOGIN_FAIL_INVALID_PASSWORD:	
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE318].szMessage, "", "", 402, 459, 1, TRUE);	// MSG_ID : 318 ; ��й�ȣ�� Ʋ�Ƚ��ϴ�.			
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
			// ��й�ȣ�� Focus �ְ� �����̰� //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);
		}		
		break;
	case LOGIN_FAIL_NO_ID:	
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE317].szMessage, "", "", 402, 459, 1, TRUE); // MSG_ID : 317 ; ��ϵ��� ���� �����Դϴ�.			
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
			// ��й�ȣ�� Focus �ְ� �����̰� //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_ID);
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
		}			
		break;
	case LOGIN_FAIL_OVERLAPPED_ID_ANOTHER_SERVER:
		{
			char szServerSetName[32]	= {0,};
			char szMsg1[0xff]			= {0,};

			switch(pPacket->dwExData)
			{
			case 1:	lstrcpy(szServerSetName, g_Message[ETC_MESSAGE216].szMessage);	break; // "ī�̿�"
			case 2:	lstrcpy(szServerSetName, g_Message[ETC_MESSAGE217].szMessage);	break; // "�̽�����"
			case 3:	lstrcpy(szServerSetName, g_Message[ETC_MESSAGE218].szMessage);	break; // "������"
			case 4:	lstrcpy(szServerSetName, g_Message[ETC_MESSAGE219].szMessage);	break; // "���̽���"
			case 5:	lstrcpy(szServerSetName, g_Message[ETC_MESSAGE220].szMessage);	break; // "�Ƹ��׶�"
			case 6:	lstrcpy(szServerSetName, g_Message[ETC_MESSAGE221].szMessage);	break; // "�ƴ���"
			case 7:	lstrcpy(szServerSetName, g_Message[ETC_MESSAGE222].szMessage);	break; // "���Ϲ�Ʈ"
			case 8:	lstrcpy(szServerSetName, g_Message[ETC_MESSAGE223].szMessage);	break; // "����"
			case 9:	lstrcpy(szServerSetName, g_Message[ETC_MESSAGE224].szMessage);	break; // "������"
			}

			wsprintf(szMsg1, g_Message[ETC_MESSAGE277].szMessage, szServerSetName); // "���̵� %s �������� ����� "

			// "�α��� ����"
			// "�Դϴ�. �ش缭���� �α����� �õ�"
			// "�Ͽ� ������ ���� �� �ֽ��ϴ�."
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE276].szMessage, szMsg1, g_Message[ETC_MESSAGE275].szMessage, g_Message[ETC_MESSAGE274].szMessage,  402, 459, 3, TRUE); 			
			pGameDefaultWnd->m_byType = 167;
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			// ��й�ȣ�� Focus �ְ� �����̰� //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_ID);
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
		}
		break;
	case LOGIN_FAIL_OVERLAPPED_ID:	
		{
			if(GetGameStatus()==UPDATE_GAME_CHAR_SELECT)
			{				
				// "�ߺ� �α����� �Ǿ����ϴ�." , "5�� �Ŀ� �����ϰڽ��ϴ�."

				CErrorWnd* pErrorWnd = CErrorWnd::GetInstance();				
				pErrorWnd->SetError(g_Message[ETC_MESSAGE593].szMessage, g_Message[ETC_MESSAGE594].szMessage , "", 2);				
				pErrorWnd->SetActive(TRUE);
				pErrorWnd->m_byType		= 1;
				pErrorWnd->m_byTime		= 5;
				pErrorWnd->m_dwCurTime	= timeGetTime();
				pErrorWnd->m_dwPevTime	= timeGetTime();				
			}
			else
			{
				// "���� ������� �����̰ų� �����������", "���� ������ �����ֽ��ϴ�.", "���α׷� ������ ������ ���ּ���."
				// MSG_ID : 325 ; ���� ������� �����Դϴ�.
				pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE595].szMessage, g_Message[ETC_MESSAGE596].szMessage, g_Message[ETC_MESSAGE597].szMessage, 402, 459, 3, TRUE);																			
				pGameDefaultWnd->SetActive();
				pGameDefaultWnd->m_bExit		= TRUE;
				pGameDefaultWnd->m_dwCurTime	= timeGetTime();
				pGameDefaultWnd->m_dwPevTime	= timeGetTime();
				LoginInterface(FALSE);

				if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
				{
					pGameDefaultWnd->m_bExit = TRUE;
					return;
				}
				g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_ID);
				g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
				g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);				
			}
		}		
		break;
	case LOGIN_FAIL_USER_OVERFLOW:	
		{
			// "�����Ͻ� ������ �ο��� �ʰ��Ͽ�"	//
			//"���ϴ�. ����� �ٽ� ������ �ֽñ�",	//
			//"�ٶ��ϴ�.",							//
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE326].szMessage, g_Message[ETC_MESSAGE598].szMessage, g_Message[ETC_MESSAGE328].szMessage, 402, 459, 3, TRUE);				 							
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
		}				
		break;				
	case LOGIN_FAIL_USER_NOT_TEST_ID:
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE329].szMessage, g_Message[ETC_MESSAGE330].szMessage, "", 402, 459, 2, TRUE);	// MSG_ID : 329, 330			
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}			
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
		}
		break;
	case LOGIN_FAIL_INVALID_VERSION:
		{

#if IS_JAPAN_LOCALIZING()
			// MSG_ID : 331 ; ���������� Ŭ���̾�Ʈ�Դϴ�., 332 ; �������� Ŭ���̾�Ʈ�� �������ּ���!
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage
				, g_Message[ETC_MESSAGE331].szMessage, g_Message[ETC_MESSAGE332].szMessage
				, g_Message[ETC_MESSAGE249].szMessage,402, 459, 2, TRUE);	
#else
			// MSG_ID : 331 ; ���������� Ŭ���̾�Ʈ�Դϴ�., 332 ; �������� Ŭ���̾�Ʈ�� �������ּ���!
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage
				, g_Message[ETC_MESSAGE331].szMessage, g_Message[ETC_MESSAGE332].szMessage, "",402, 459, 2, TRUE);	
#endif				
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);			
			g_bExitChk = TRUE;

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}		
		break;		
	case LOGIN_FAIL_NOW_TRY_TO_LOGIN:
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE333].szMessage, g_Message[ETC_MESSAGE334].szMessage, "", 402, 459, 2);	// MSG_ID : 333 ; Now try to login..., 334 ; Please Wait
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);				
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;

	case LOGIN_FAIL_USER_BLOCK:
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE335].szMessage, "", "",402, 459, 1, TRUE);	// MSG_ID : 334 ; ����������� �����Դϴ�.			
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
			g_bExitChk = TRUE;
		
			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}		
		break;		
	case LOGIN_FAIL_NEXT:
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE337].szMessage, "", "",402, 459, 1, TRUE);	// MSG_ID : 337 ; ���� PC�� ���º��� ���Ӱ����մϴ�.			
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
			g_bExitChk = TRUE;

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;
	case LOGIN_FAIL_ETC:
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE338].szMessage, g_Message[ETC_MESSAGE339].szMessage, "",402, 459, 2, TRUE);	//MSG_ID : 338 ; �α��ο� �����߽��ϴ�., 339 ; ����� �ٽ� �õ����ּ���!			
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
			g_bExitChk = TRUE;

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;
	case LOGIN_FAIL_CHARACTER_OVERLAPPED:
		{
			// "ĳ���� �ߺ� �����Դϴ�."	//
			// "��ڿ��� �����ϼ���!"		//
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE599].szMessage, g_Message[ETC_MESSAGE600].szMessage, "",402, 459, 3, TRUE);	
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
			g_bExitChk = TRUE;

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;
	case LOGIN_FAIL_INVALID_CHARACTER_INDEX:
		{
			// "ĳ���� �ε��� �����Դϴ�."	//
			//"��ڿ��� �����ϼ���!"		//
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE601].szMessage, g_Message[ETC_MESSAGE600].szMessage, "",402, 459, 3, TRUE);
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
			g_bExitChk = TRUE;

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;
	case LOGIN_FAIL_14_OVER:
		{
			// "14���̸� �ӽð����ڷ� �θ�"		//
			// "���� ó�� �� ���Ӱ����մϴ�."	//
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE602].szMessage, g_Message[ETC_MESSAGE603].szMessage, "", 402, 459, 3, TRUE);				
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			// ��й�ȣ�� Focus �ְ� �����̰� //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);
			
			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)							
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;		
	case LOGIN_FAIL_NOT_MIRI_USER:	//�̸� ���� ����� 
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, 
				g_Message[ETC_MESSAGE604].szMessage, // "ĳ���͸� �̸� ������ �� �ִ� "
				g_Message[ETC_MESSAGE605].szMessage, // "����ڰ� �ƴմϴ�."
				"", 402, 459, 3, TRUE);	
			
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			// ��й�ȣ�� Focus �ְ� �����̰� //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)							
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;		
	case LOGIN_FAIL_18_OVER:	//18�� �̻� ���� 
		{
			// "�̼����� �������� ������ "			//
			// "��18�� �̻� ������ ��밡���մϴ�."	//
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE606].szMessage, g_Message[ETC_MESSAGE607].szMessage, "", 402, 459, 2, TRUE);	
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			// ��й�ȣ�� Focus �ְ� �����̰� //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)							
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;
	case LOGIN_FAIL_NOT_REAL_NAME:
		{
			//"���� �Ǹ������� ���� �����̽��ϴ�.",			//
			//"Ȩ���������� �Ǹ����� �� ���� �����մϴ�.",	//
			// "http://www.corumonline.co.kr",				//
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE608].szMessage, g_Message[ETC_MESSAGE609].szMessage, g_Message[ETC_MESSAGE610].szMessage, 402, 459, 3, TRUE);	
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			// ��й�ȣ�� Focus �ְ� �����̰� //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)							
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;
	case LOGIN_FAIL_NOT_BETA_TESTER:
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE926].szMessage, g_Message[ETC_MESSAGE927].szMessage, "", 402, 459, 3, TRUE);	
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			// ��й�ȣ�� Focus �ְ� �����̰� //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)							
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;
	case LOGIN_FAIL_EJECT_USER_JAPAN:
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE928].szMessage, g_Message[ETC_MESSAGE600].szMessage, "", 402, 459, 3, TRUE);	
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			// ��й�ȣ�� Focus �ְ� �����̰� //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)							
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;
	case LOGIN_FAIL_SECESSION_USER:
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE929].szMessage, g_Message[ETC_MESSAGE600].szMessage,  "", 402, 459, 3, TRUE);	
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			// ��й�ȣ�� Focus �ְ� �����̰� //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)							
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;
	default:
		{
			// Show Error Code by minjin. 2004. 10. 29.
			char szTemp[1024] = {0,};
			wsprintf( szTemp, "[Error Code : %d]", pPacket->bResult );

			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, "Cannot login with some reason", szTemp, "", 402, 459, 3, TRUE);			
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);


			// ��й�ȣ�� Focus �ְ� �����̰� //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)							
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
	}

	// When Login is Failed..
	// Disconnect from Login Server
	// for Packet Encryption with deepdark
	// 2004. 08. 20.
	// Begin - minjin

	// If User Failed to Login, then
	// Server Side User Object Reset..
	// So, Key must be Reseted

#ifdef __PACKET_ENCRYPTION
			ResetEncryptionKey();
#endif

	// End - minjin

}

void CmdLoginSuccess( char* pMsg, DWORD dwLen )
{	
	WSTC_LOGIN_SUCCESS* pPacket			= (WSTC_LOGIN_SUCCESS*)pMsg;
	CGameDefaultWnd*	pGameDefaultWnd = CGameDefaultWnd::GetInstance();	
	
	int		i		= 0;
	WORD	wIndex	= 0;
	
	g_bAdultMode = pPacket->bAdultMode;
	g_byCharacterAllCount	= pPacket->bNum;
	
	pGameDefaultWnd->SetActive(FALSE);
	
#ifndef DEVELOP_MODE
	g_pGVLogin->bIsLoging = FALSE;	
#endif

	if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)
	{
		g_NetMarble.bLoging = FALSE;
	}

	V2_SPRITE* pSprOKOver = g_pSprManager->GetSprite(BUTTON_OK_OVER);

	for(i = 0; i < pPacket->bNum; i++)
	{
		if(IsEmptyString(pPacket->Character[i].szName))
			continue;

		wIndex = WORD(pPacket->Character[i].wChrNum - 1);

		if(wIndex > 3 || wIndex > 256)
		{			
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE322].szMessage, "", "", 402, 459, 1, TRUE);	// MSG_ID : 322 ; Invalid Character Index Received!			
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);			
			PostQuitMessage(0);
	
			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)
			{
				g_bIsRunning = FALSE;
				return;
			}
		}

		if(!IsEmptyString(g_pGVLogin->ChrSelectInfo[ wIndex ].ReceivedChrInfo.szName))
		{			
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE323].szMessage, "", "", 402, 459, 1, TRUE);	// MSG_ID : 323 ; Character Overlapped!			
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);						
			PostQuitMessage(0);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		memcpy(&g_pGVLogin->ChrSelectInfo[ wIndex ], &pPacket->Character[i], sizeof(RS_CharacterLogInfo) );		
		g_pGVLogin->ChrSelectInfo[ wIndex ].bByItemType = BYTE((pPacket->Character[i].wRHandID/ITEM_DISTRIBUTE) + 1);		
	}
	pSprOKOver->ShowSprite(FALSE);
	SetGameStatus( UPDATE_GAME_CHAR_SELECT );
	return;
}

void CmdLoginFailWithReason( char* pMsg, DWORD dwLen )
{
	WSTC_BLOCK_ACCOUNT*	pPacket = (WSTC_BLOCK_ACCOUNT*)pMsg;
	CBlockWnd*			pWnd	= CBlockWnd::GetInstance();

	g_pGVLogin->bIsLoging = FALSE;

	if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)	//��������ϰ�� 
	{
		g_NetMarble.bLoging = FALSE;
	}

	pWnd->m_bInfoType = UPDATE_GAME_CHAR_SELECT;

	char*	szTemp				= 0;
	char	szReason[1024]		= {0,};	
	int		nMaxLineCharacter	= 34;

	memset(szReason, 0, 1024);
	memcpy(szReason, pPacket->szReason, pPacket->bLen);

	szTemp	= szReason;

	int nSize	= pPacket->bLen;
	int nCount	= 1;
	
	for(int i = 0; i < MAX_BLOCK_MESSAGE_LINE_COUNT; i++)
		memset(pWnd->szMessageLine[i], 0, 255);

	lstrcpy(pWnd->szMessageLine[0], g_Message[ETC_MESSAGE923].szMessage); // "<< ���ϴ� �������� �����Դϴ�. >>"

	while(nSize > 0)
	{
		if(nSize > nMaxLineCharacter)
		{
			for(;;)
			{
				if(nSize > nMaxLineCharacter)
				{
					char* szPrev = CharPrevExA(0, szTemp, szTemp+nMaxLineCharacter, 0);
					nSize = lstrlen(szTemp) - lstrlen(szPrev);
					
					memset(pWnd->szMessageLine[nCount], 0, 255);
					__lstrcpyn(pWnd->szMessageLine[nCount], szTemp, nSize);
					
					szTemp = szPrev;
					nCount++;
					nSize = lstrlen(szTemp);
				}
				else
				{
					// ���� ���ڿ��� ä�� ���ο� ����, ���̸� ����Ѵ�.
					lstrcpy(pWnd->szMessageLine[nCount], szTemp);
					nCount++;								
					nSize = 0;
					break;
				}
			}			
		}
		else
		{	
			memset(pWnd->szMessageLine[nCount], 0, nMaxLineCharacter);
			__lstrcpyn(pWnd->szMessageLine[nCount], szTemp, lstrlen(szTemp));	
			nCount++;
			nSize = 0;
		}		
	}
	
	pWnd->m_fPosX		= 402;
	pWnd->m_fPrevPosX	= pWnd->m_fPosX;
	pWnd->m_fPosZ		= 330;
	pWnd->m_fPrevPosZ	= pWnd->m_fPosZ;
	pWnd->SetActive();

	if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
		return;
	
	g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_ID);
	g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
	g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);	
}

// added by deepdark. �α��� �������� ��ȣȭ ���� Ű�� �����ߴ�...
void CmdEncryptionKey(char* pMsg, DWORD dwLen)
{
	WSTC_ENCRYPTION_KEY* pPacket = (WSTC_ENCRYPTION_KEY*)pMsg;

	BYTE btEncryptionKey[ENCRYPT_KEY_LEN]={0,};
	CalculateKey(GetClientKey(), pPacket->byServerKey, btEncryptionKey);

/*
//	Output Key Strings For Packet Encryption
//	2004. 08. 20.
//	minjin with deepdark

	char szTemp[4096];
	LPBYTE szTempClient = GetClientKey();

	sprintf(szTemp, "[minjin with deepdark] Client Key : %d,%d,%d,%d,%d,%d,%d,%d,%d,%d \n",
		szTempClient[0],	szTempClient[1],	szTempClient[2],	szTempClient[3],	szTempClient[4],
		szTempClient[5],	szTempClient[6],	szTempClient[7],	szTempClient[8],	szTempClient[9]);
	OutputDebugString(szTemp);

	sprintf(szTemp, "[minjin with deepdark] Server Key : %d,%d,%d,%d,%d,%d,%d,%d,%d,%d \n",
		pPacket->byServerKey[0],	pPacket->byServerKey[1],	pPacket->byServerKey[2],	pPacket->byServerKey[3],	pPacket->byServerKey[4],
		pPacket->byServerKey[5],	pPacket->byServerKey[6],	pPacket->byServerKey[7],	pPacket->byServerKey[8],	pPacket->byServerKey[9]);
	OutputDebugString(szTemp);

	sprintf(szTemp, "[minjin with deepdark] Encrypted Key : %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d \n",
		btEncryptionKey[0],		btEncryptionKey[1],		btEncryptionKey[2],		btEncryptionKey[3],
		btEncryptionKey[4],		btEncryptionKey[5],		btEncryptionKey[6],		btEncryptionKey[7],
		btEncryptionKey[8],		btEncryptionKey[9],		btEncryptionKey[10],	btEncryptionKey[11],
		btEncryptionKey[12],	btEncryptionKey[13],	btEncryptionKey[14],	btEncryptionKey[15]);
	OutputDebugString(szTemp);

//*/
	
	SetEncryptionKey(btEncryptionKey);
}