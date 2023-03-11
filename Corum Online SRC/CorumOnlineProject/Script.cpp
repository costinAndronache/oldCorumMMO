//======================================================//
// Code by Jang.							2002.12.06	//
//======================================================//
#include "stdafx.h"
#include "Script.h"
#include "GameControl.h"
#include "Interface.h"
#include "UserInterface.h"
#include "ChatWnd.h"
#include "message.h"
#include "cmd_message.h"
#include "Chat.h"
#include "Dungeon_data_ex.h"
#include "GmTool.h"
#include "InputManager.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "map.h"
#include "ChinaBillingRemainTime.h"


//======================================================//
// Construction/Destruction.							//
//======================================================//
CScript::CScript()
{
	//memset(m_szName, 0, sizeof(m_szName));
}

CScript::~CScript()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
int CScript::ChkCommand()
{
	int nIndex = INPUT_BUFFER_1;

	if(GetGameStatus()==UPDATE_GAME_PLAY)
		nIndex = INPUT_BUFFER_19;
	
	int	nChk	= __CHAT_MSG__;
	int	nSize	= lstrlen(g_pInputManager->GetInputBuffer(nIndex));
	int	nPos	= strcspn(g_pInputManager->GetInputBuffer(nIndex), "/" );

	// �˻� //						
	if(nPos==0)
	{
		nChk = __CHAT_COMMAND__;

		for(int i = 1; i < nSize; i++)
		{
			if(g_pInputManager->GetInputBuffer(nIndex)[i]=='/')					
			{
				nChk = __CHAT_MSG__;
				break;
			}
		}
	}
	else
	{				
		if(nSize!=nPos)
		{
			nChk = __CHAT_COMMAND__;

			for(int i = 0; i < nPos; i++)
			{
				if(g_pInputManager->GetInputBuffer(nIndex)[i]!=' ')
				{
					nChk = __CHAT_MSG__;
					break;
				}
			}

			if(nChk==__CHAT_COMMAND__)
			{
				for(int i = nPos+1; i < nSize; i++)
				{
					if(g_pInputManager->GetInputBuffer(nIndex)[i]=='/')
					{
						nChk = __CHAT_MSG__;
						break;
					}
				}
			}
		}					
	}
	return nChk;
}

int CScript::MsgCheck()
{
	int		nIndex		= 0;
	int		nCommand	= -1;
	char	szSeps[]	= " ,/";
	char*	szToken;

	char szCommand[__MAX_COMMAND__][__MAX_COMMANDSIZE__] = {0,};
//		{""}, {"�Ӹ�"}, {""}, {"�Ӹ��ź�"}, {"�Ӹ����"}, {"����"}, {"���"}, {"���Ͱ���"}, {"���͹��"},
//		{"��Ƽ"}, {"���"}, {":"}, {"@"}, {"#"}, {"��Ƽ���"}, {"��Ƽ�ź�"}, {"$"}, {"?"}
		// MSG(ID) :	�Ӹ�(374), �Ӹ��ź�(375), �Ӹ����(376), ����(377), ���(378), ���Ͱ���(379), ���͹��(380)
		//			��Ƽ(381), ���(285)

/*0*/	lstrcpy(szCommand[nIndex++], "");
/*1*/	lstrcpy(szCommand[nIndex++], g_CmdMessage[CMD_MESSAGE0].szMessage);	// CMD_MSG_ID : 0 ; �Ӹ� 
/*2*/	lstrcpy(szCommand[nIndex++], "");
/*3*/	lstrcpy(szCommand[nIndex++], g_CmdMessage[CMD_MESSAGE1].szMessage);	// CMD_MSG_ID : 1 ; �Ӹ��ź�
/*4*/	lstrcpy(szCommand[nIndex++], g_CmdMessage[CMD_MESSAGE2].szMessage);	// CMD_MSG_ID : 2 ; �Ӹ����
/*5*/	lstrcpy(szCommand[nIndex++], g_CmdMessage[CMD_MESSAGE3].szMessage);	// CMD_MSG_ID : 3 ; ����
/*6*/	lstrcpy(szCommand[nIndex++], g_CmdMessage[CMD_MESSAGE4].szMessage);	// CMD_MSG_ID : 4 ; ���
/*7*/	lstrcpy(szCommand[nIndex++], g_CmdMessage[CMD_MESSAGE5].szMessage);	// CMD_MSG_ID : 5 ; ���Ͱ���
/*8*/	lstrcpy(szCommand[nIndex++], g_CmdMessage[CMD_MESSAGE6].szMessage);	// CMD_MSG_ID : 6 ; ���͹��
/*9*/	lstrcpy(szCommand[nIndex++], g_CmdMessage[CMD_MESSAGE7].szMessage);	// CMD_MSG_ID : 7 ; ��Ƽ
/*10*/	lstrcpy(szCommand[nIndex++], g_CmdMessage[CMD_MESSAGE8].szMessage);	// CMD_MSG_ID : 8 ; ���
/*11*/	lstrcpy(szCommand[nIndex++], ":");
/*12*/	lstrcpy(szCommand[nIndex++], "@");	
/*13*/	lstrcpy(szCommand[nIndex++], "#");
/*14*/	lstrcpy(szCommand[nIndex++], g_CmdMessage[CMD_MESSAGE9].szMessage);	// CMD_MSG_ID : 9 ; ��Ƽ���
/*15*/	lstrcpy(szCommand[nIndex++], g_CmdMessage[CMD_MESSAGE10].szMessage);	// CMD_MSG_ID : 10 ; ��Ƽ�ź�
/*16*/	lstrcpy(szCommand[nIndex++], "$");	
/*17*/	lstrcpy(szCommand[nIndex++], "?");
/*18*/	lstrcpy(szCommand[nIndex++], "PK" );									// PK Mode.. 	
/*19*/	lstrcpy(szCommand[nIndex++], "CharacterInfo" );
/*20*/	lstrcpy(szCommand[nIndex++], g_CmdMessage[CMD_MESSAGE11].szMessage ); // CMD_MSG_ID : 11 ; "ħ��"
/*21*/	lstrcpy(szCommand[nIndex++], g_CmdMessage[CMD_MESSAGE12].szMessage ); // CMD_MSG_ID : 12 ; "����"
/*22*/	lstrcpy(szCommand[nIndex++], g_CmdMessage[CMD_MESSAGE13].szMessage ); // CMD_MSG_ID : 13 ; "����"
/*23*/	lstrcpy(szCommand[nIndex++], g_CmdMessage[CMD_MESSAGE14].szMessage ); // CMD_MSG_ID : 14 ; "�̵�"
/*24*/	lstrcpy(szCommand[nIndex++], g_CmdMessage[CMD_MESSAGE15].szMessage ); // CMD_MSG_ID : 15 ; "��ȯ" 
/*25*/	lstrcpy(szCommand[nIndex++], g_CmdMessage[CMD_MESSAGE16].szMessage ); // CMD_MSG_ID : 16 ; "���"
	
// Begin - minjin 2004. 08. 25.
// Debugging for Chatting Message Error
// When User Input '/' , Error is occured
// The Reason why is CHINA LOCALIZING SPECIAL CODE is here..
#if IS_CHINA_LOCALIZING()
	lstrcpy(szCommand[nIndex++], g_CmdMessage[CMD_MESSAGE17].szMessage ); // CMD_MSG_ID : 17 ; "" �߱� ���� ���� �ð�.
#endif
// End - minjin

	memset(&m_sScript, 0, sizeof(SSCRIPT));

	szToken = strtok(m_szText, szSeps);
	if( !szToken )	return __SCRT_FAIL__;			// ����.
	
	int i;
	
	// � Ŀ�ǵ� ������ �˾Ƴ���.
	for( i=0; i<__MAX_COMMAND__; i++ )
	{
		if( __stricmp( szToken, szCommand[i] ) == 0 )
			nCommand = i;
	}
	if( nCommand == -1 )	return __SCRT_FAIL__;	// ��ġ �ϴ� Ŀ�ǵ� ����.

	switch( nCommand )
	{
	case COMMAND_GUARDIAN_ATTACK:							// ����� ���� Ŀ�ǵ�.
		{
			m_sScript.enCommand	= COMMAND_GUARDIAN_ATTACK;
			szToken = strtok(NULL, szSeps);
			if( !szToken ) return __SCRT_FAIL__;

			__lstrcpyn( m_sScript.szParam[0], szToken, sizeof(szToken));
			
			for( i=0; i<MAX_USER_GUARDIAN; i++ )	// Attack Command �� ���� Guardian�� ã�´�.
			{
				if( g_pMainPlayer->m_pGuardian[i] )
				{
					g_pMainPlayer->m_pGuardian[i]->m_szName;
					if( __stricmp( g_pMainPlayer->m_pGuardian[i]->m_szName, m_sScript.szParam[0] ) == 0 )
					{
						// ���� ����� ���� ������� �ε��� ��ȣ�� ����Ѵ�.
						g_pMainPlayer->m_dwTemp[ USER_TEMP_COMMANDGUARDIAN ] = i;	// ������ ����س��´�.
						g_pInputManager->ClearInput(nIndex);
						CChatWnd::GetInstance()->SetActive(FALSE);
						return __SCRT_SUCCESS__;
					}
				}
			}
			g_pInputManager->ClearInput(nIndex);
			CChatWnd::GetInstance()->SetActive(FALSE);
			return __SCRT_FAIL__;
		}
		break;

	case COMMAND_GUARDIAN_DEFENSE:
		{
			m_sScript.enCommand	= COMMAND_GUARDIAN_DEFENSE;
			szToken = strtok(NULL, szSeps);
			if( !szToken ) return __SCRT_FAIL__;

			__lstrcpyn( m_sScript.szParam[0], szToken, sizeof(szToken));
			
			for( i=0; i<MAX_USER_GUARDIAN; i++ )	// Attack Command �� ���� Guardian�� ã�´�.
			{
				if( g_pMainPlayer->m_pGuardian[i] )
				{
					g_pMainPlayer->m_pGuardian[i]->m_szName;
					if( __stricmp( g_pMainPlayer->m_pGuardian[i]->m_szName, m_sScript.szParam[0] ) == 0 )
					{
						// ���� ����� ���� ������� �ε��� ��ȣ�� ����Ѵ�.
						g_pMainPlayer->m_dwTemp[ USER_TEMP_COMMANDGUARDIAN ] = i;	// ������ ����س��´�.
						g_pInputManager->ClearInput(nIndex);
						CChatWnd::GetInstance()->SetActive(FALSE);
						return __SCRT_SUCCESS__;
					}
				}
			}
			g_pInputManager->ClearInput(nIndex);
			CChatWnd::GetInstance()->SetActive(FALSE);
			return __SCRT_FAIL__;
		}
		break;

	case COMMAND_MONSTER_ATTACK:
		{
			m_sScript.enCommand	= COMMAND_MONSTER_ATTACK;
//			szToken = strtok(NULL, szSeps);
//			if( !szToken ) return __SCRT_FAIL__;
			
			g_pInputManager->ClearInput(nIndex);
			CChatWnd::GetInstance()->SetActive(FALSE);
			return __SCRT_SUCCESS__;
		}
		break;

	case COMMAND_MONSTER_DEFENSE:
		{
			m_sScript.enCommand	= COMMAND_MONSTER_DEFENSE;
//			szToken = strtok(NULL, szSeps);
//			if( !szToken ) return __SCRT_FAIL__;
			return __SCRT_SUCCESS__;
		}
		break;
	case COMMAND_REFUSAL:
		{
			// �ӼӸ� �ź� //
			m_sScript.enCommand	= COMMAND_REFUSAL;

			szToken = strtok(NULL, szSeps);
			
			if(szToken)
				return __SCRT_FAIL__;

			return __SCRT_SUCCESS__;			
		}
		break;	
	case COMMAND_APPROVAL:
		{
			// �ӼӸ� ��� //
			m_sScript.enCommand	= COMMAND_APPROVAL;

			szToken = strtok(NULL, szSeps);
			
			if(szToken)
				return __SCRT_FAIL__;

			return __SCRT_SUCCESS__;
		}
		break;
	case COMMAND_PARTY:
	case COMMAND_PARTY2:
		{
			// ��Ƽ //
			m_sScript.enCommand	= COMMAND_PARTY;

			szToken = strtok(NULL, "");
			
			if(szToken)
			{
				lstrcat(m_sScript.szMessage, szToken);
				lstrcat(m_sScript.szMessage, " ");

				szToken = strtok(NULL, " ");
			
				while(szToken)
				{
					if(szToken)
					{
						lstrcat(m_sScript.szMessage, szToken);
						lstrcat(m_sScript.szMessage, " ");
					}
					
					szToken = strtok(NULL, " ");											
				}
				return __SCRT_SUCCESS__;
			}
			return __SCRT_FAIL__;			
		}
		break;
	case COMMAND_GUILD:
	case COMMAND_GUILD2:
		{
			// ��� //
			m_sScript.enCommand	= COMMAND_GUILD;

			szToken = strtok(NULL, "");
			
			if(szToken)
			{
				lstrcat(m_sScript.szMessage, szToken);
				lstrcat(m_sScript.szMessage, " ");

				szToken = strtok(NULL, " ");
			
				while(szToken)
				{
					if(szToken)
					{
						lstrcat(m_sScript.szMessage, szToken);
						lstrcat(m_sScript.szMessage, " ");
					}
					
					szToken = strtok(NULL, " ");
				}
				return __SCRT_SUCCESS__;
			}
			return __SCRT_FAIL__;
		}
		break;
	case COMMAND_WHISPER3:
		{
			// �Ӹ� //
			m_sScript.enCommand	= COMMAND_WHISPER;

			if(__stricmp(m_szName, "")!=0)
				__lstrcpyn(m_sScript.szId, m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
			else
			{
				DisplayMessageAdd(g_Message[ETC_MESSAGE639].szMessage, 0xFFFFB894); // "���������� ���� ����� �����ϴ�."
				_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				
				return __SCRT_FAIL__;
			}

			szToken = strtok(NULL, " ");
			
			if(szToken)
			{
				lstrcat(m_sScript.szMessage, szToken);
				lstrcat(m_sScript.szMessage, " ");

				szToken = strtok(NULL, " ");
			
				while(szToken)
				{
					if(szToken)
					{
						lstrcat(m_sScript.szMessage, szToken);
						lstrcat(m_sScript.szMessage, " ");
					}
					szToken = strtok(NULL, " ");					
				}
				return __SCRT_SUCCESS__;
			}
			return __SCRT_FAIL__;
		}
		break;
	case COMMAND_WHISPER:
	case COMMAND_WHISPER2:
		{
			// �Ӹ� //
			m_sScript.enCommand	= COMMAND_WHISPER;

			szToken = strtok(NULL, " ");
			
			if(szToken)
			{
				__lstrcpyn(m_sScript.szId, szToken, MAX_CHARACTER_NAME_REAL_LENGTH);

				szToken = strtok(NULL, " ");
			
				while(szToken)
				{
					if(szToken)
					{
						lstrcat(m_sScript.szMessage, szToken);
						lstrcat(m_sScript.szMessage, " ");
					}
					
					szToken = strtok(NULL, " ");

					if(!szToken)
						return __SCRT_SUCCESS__;
				}
				return __SCRT_FAIL__;
			}
			return __SCRT_FAIL__;
		}
		break;
	case COMMAND_PARTYAPPROVE:	
		{
			// ��Ƽ ��� //
			m_sScript.enCommand	= COMMAND_PARTYAPPROVE;

			szToken = strtok(NULL, "");
			
			if(szToken)			
				return __SCRT_FAIL__;

			return __SCRT_SUCCESS__;
		}
		break;
	case COMMAND_PARTYREFUSAL:
		{
			// ��Ƽ �ź� //
			m_sScript.enCommand	= COMMAND_PARTYREFUSAL;

			szToken = strtok(NULL, "");
			
			if(szToken)			
				return __SCRT_FAIL__;

			return __SCRT_SUCCESS__;
		}
		break;
	case COMMAND_HELP:
		{
			// ���� //
			m_sScript.enCommand	= COMMAND_HELP;

			szToken = strtok(NULL, "");
			
			if(szToken)			
				return __SCRT_FAIL__;

			return __SCRT_SUCCESS__;
		}
		break;

#ifdef GM_TOOL
	case COMMAND_GM_PK_MODE:
		{
			m_sScript.enCommand = COMMAND_GM_PK_MODE;

			szToken = strtok(NULL, " " );
			if( szToken )
			{
				__lstrcpyn( m_sScript.szParam[0], szToken, lstrlen( szToken ) );

				szToken = strtok( NULL, " " );

				if( !szToken )
					return __SCRT_SUCCESS__;
			}
			return __SCRT_FAIL__;
		}
		break;

	case COMMAND_GM_CHAT_DISABLE:
		{
			m_sScript.enCommand = COMMAND_GM_CHAT_DISABLE;

			szToken = strtok(NULL, " " );
			if( szToken )
			{
				__lstrcpyn( m_sScript.szParam[0], szToken, lstrlen( szToken ) );
				szToken = strtok( NULL, " " );

				if( !szToken )
					return __SCRT_SUCCESS__;
			}
			return __SCRT_FAIL__;
		}
		break;

	case COMMAND_GM_CREATE:
		{
			m_sScript.enCommand = COMMAND_GM_CREATE;

			szToken = strtok(NULL, " " );
			if( szToken )
			{
				__lstrcpyn( m_sScript.szParam[0], szToken, lstrlen( szToken ) );		// ����, ������, ....
				szToken = strtok( NULL, " " );
				
				if( szToken )
				{
					__lstrcpyn( m_sScript.szParam[1], szToken, lstrlen( szToken ) );	// ��ȣ..
					szToken = strtok( NULL, " " );
				}
				else 
					return __SCRT_FAIL__;
				
				lstrcpy( m_sScript.szParam[2], "0" );		// �⺻�� �־��ְ�.

				if( szToken )	
				{
					__lstrcpyn( m_sScript.szParam[2], szToken, lstrlen( szToken ) );	// ��ȣ..
					szToken = strtok( NULL, " " );
				}

				if( !szToken )
					return __SCRT_SUCCESS__;
			}
			return __SCRT_FAIL__;
		}
		break;

	case COMMAND_GM_REMOVE:
		{
			__asm nop;
		}
		break;

	case COMMAND_GM_POTAL:
		{
			m_sScript.enCommand = COMMAND_GM_POTAL;

			szToken = strtok( NULL, " " );
			if( szToken )
			{
				__lstrcpyn( m_sScript.szParam[0], szToken, lstrlen( szToken ) );		// GM, ��ȯ, ���.
				
				szToken = strtok( NULL, " " );
				if( szToken )
				{
					__lstrcpyn( m_sScript.szParam[1], szToken, lstrlen( szToken ) );	// �ʹ�ȣ.
				}
				else
					return __SCRT_FAIL__;
				
				szToken = strtok( NULL, " " );
				if( szToken )
				{
					__lstrcpyn( m_sScript.szParam[2], szToken, lstrlen( szToken ) );	// ����ȣ.
				}
				else
					return __SCRT_FAIL__;

				szToken = strtok( NULL, " " );
				if( szToken )
				{
					__lstrcpyn( m_sScript.szParam[3], szToken, lstrlen( szToken ) );	// X.
				}
				else
					return __SCRT_FAIL__;
				
				szToken = strtok( NULL, " " );
				if( szToken )
				{
					__lstrcpyn( m_sScript.szParam[4], szToken, lstrlen( szToken ) );	// Y.
				}
				else
					return __SCRT_FAIL__;
				
				szToken = strtok( NULL, " " );
				if( !szToken )
					return __SCRT_SUCCESS__;
			}
			return __SCRT_FAIL__;
		}
		break;
		
	case COMMAND_GM_RECALL:
		{
			m_sScript.enCommand = COMMAND_GM_RECALL;

			szToken = strtok( NULL, " " );
			if( szToken )
			{
				__lstrcpyn( m_sScript.szParam[0], szToken, lstrlen( szToken ) );		// ���̵�
				
				szToken = strtok( NULL, " " );
				if( !szToken )
					return __SCRT_SUCCESS__;
			}

			return __SCRT_FAIL__;
		}
		break;

	case COMMAND_GM_USERPOTAL:
		{
			m_sScript.enCommand = COMMAND_GM_USERPOTAL;

			szToken = strtok( NULL, " " );
			if( szToken )
			{
				__lstrcpyn( m_sScript.szParam[0], szToken, lstrlen( szToken ) );		// ���̵�
				
				szToken = strtok( NULL, " " );
				if( szToken )
				{
					__lstrcpyn( m_sScript.szParam[1], szToken, lstrlen( szToken ) );	// �ʹ�ȣ.
				}
				else
					return __SCRT_FAIL__;
				
				szToken = strtok( NULL, " " );
				if( szToken )
				{
					__lstrcpyn( m_sScript.szParam[2], szToken, lstrlen( szToken ) );	// ����ȣ.
				}
				else
					return __SCRT_FAIL__;

				szToken = strtok( NULL, " " );
				if( szToken )
				{
					__lstrcpyn( m_sScript.szParam[3], szToken, lstrlen( szToken ) );	// X.
				}
				else
					return __SCRT_FAIL__;

				szToken = strtok( NULL, " " );
				if( szToken )
				{
					__lstrcpyn( m_sScript.szParam[4], szToken, lstrlen( szToken ) );	// Y.
				}
				else
					return __SCRT_FAIL__;
				
				szToken = strtok( NULL, " " );
				if( !szToken )
					return __SCRT_SUCCESS__;
			}

			return __SCRT_FAIL__;
		}
		break;
#endif

	case COMMNAD_CHARACTERINFO:
		{
#ifdef DEVELOP_MODE
			// �Ӹ� //
			m_sScript.enCommand	= COMMNAD_CHARACTERINFO;

			szToken = strtok(NULL, " ");
			
			if(szToken)
			{
				__lstrcpyn(m_sScript.szId, szToken, MAX_CHARACTER_NAME_REAL_LENGTH);

				szToken = strtok(NULL, " ");
			
				if(!szToken)				
					return __SCRT_SUCCESS__;
			}
			return __SCRT_FAIL__;
#else 
			return __SCRT_FAIL__;
#endif
		}
		break;

#if IS_CHINA_LOCALIZING()		
	case COMMAND_CHINA_BILL_REMAINTIME:
		{
			m_sScript.enCommand	= COMMAND_CHINA_BILL_REMAINTIME;

			return __SCRT_SUCCESS__;
		}
		break;
#endif
	}


	g_pInputManager->ClearInput(nIndex);
	//CChatWnd::GetInstance()->SetActive(FALSE);
	return __SCRT_FAIL__;	
}
	
int CScript::MsgExecute(int nServer)
{
	// ��ɾ� ���� ó�� //
	switch(m_sScript.enCommand)
	{
	case COMMAND_GUARDIAN_ATTACK:
		{
			CUserInterface::GetInstance()->SetPointerChk( TRUE );
			CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_GUARDIAN__);
		}
		break;

	case COMMAND_GUARDIAN_DEFENSE:
		{
			CTDS_GUARDIAN_COMMAND Command;
			Command.bZipCode	= (BYTE)g_pMainPlayer->m_dwTemp[ USER_TEMP_COMMANDGUARDIAN ];
			Command.bCommand	= 0;
			g_pNet->SendMsg( (char*)&Command, Command.GetPacketSize(), SERVER_INDEX_ZONE );
		}
		break;

	case COMMAND_MONSTER_ATTACK:
		{
			CUserInterface::GetInstance()->SetPointerChk( TRUE );
			CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_GUARDIAN__);
		}
		break;

	case COMMAND_MONSTER_DEFENSE:
		{
			CTDS_MONSTER_COMMAND	MonsterCommand;
			MonsterCommand.bCommand	= 0;
			g_pNet->SendMsg( (char*)&MonsterCommand, MonsterCommand.GetPacketSize(), SERVER_INDEX_ZONE );
		}
		break;
	case COMMAND_REFUSAL:
		{
			CTWS_COMMAND_MSG pPacket;
			pPacket.bCommand = FALSE;			
			g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
		}
		break;
	case COMMAND_APPROVAL:
		{
			CTWS_COMMAND_MSG pPacket;
			pPacket.bCommand = TRUE;			
			g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
		}
		break;
	case COMMAND_PARTY:
		{						
			// ��Ƽ //						
			if(GetGameStatus()==UPDATE_GAME_WORLD)
			{			
				if(g_pMainPlayer->m_dwPartyId!=0)
				{
					WSTC_CHAT_USER pPacket;
					pPacket.nIndex = 3;
					memset(pPacket.szId, 0, sizeof(pPacket.szId));
					memset(pPacket.szMessage, 0, sizeof(pPacket.szMessage));
					__lstrcpyn(pPacket.szId, g_pMainPlayer->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
					__lstrcpyn(pPacket.szMessage, m_sScript.szMessage, sizeof(m_sScript.szMessage));
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
				}
				else
				{
					// MSG_ID : 69 ; �������� ��Ƽ�� �����ϴ�.
					DisplayMessageAdd(g_Message[ETC_MESSAGE69].szMessage, 0xffffff00);	
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}
			}
			else
			{
			//	CChatWnd* pChatWnd = CChatWnd::GetInstance();
				
				if(g_pMainPlayer->m_dwPartyId!=0)
				{
					CTWS_DGCHAT_USER pPacket;
					pPacket.nIndex = 3;
					memset(pPacket.szId, 0, sizeof(pPacket.szId));
					memset(pPacket.szMessage, 0, sizeof(pPacket.szMessage));
					__lstrcpyn(pPacket.szId, g_pMainPlayer->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
					__lstrcpyn(pPacket.szMessage, m_sScript.szMessage, sizeof(m_sScript.szMessage));
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
				}
				else
				{
					// MSG_ID : 69 ; �������� ��Ƽ�� �����ϴ�.
					DisplayMessageAdd(g_Message[ETC_MESSAGE69].szMessage, 0xffffff00);	
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}
			}													
		}
		break;
	case COMMAND_GUILD:
		{
			// ��� //
			if(GetGameStatus()==UPDATE_GAME_WORLD)
			{				
				if(g_pMainPlayer->m_dwGuildId!=0 && g_pGuildUserHash->GetCount()!=0)
				{
					WSTC_CHAT_USER pPacket;
					pPacket.nIndex = 2;
					memset(pPacket.szId, 0, sizeof(pPacket.szId));
					memset(pPacket.szMessage, 0, sizeof(pPacket.szMessage));
					__lstrcpyn(pPacket.szId, g_pMainPlayer->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
					__lstrcpyn(pPacket.szMessage, m_sScript.szMessage, sizeof(m_sScript.szMessage));
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
				}
				else
				{
					char szInfo[0xff] = {0,};

					if(g_pMainPlayer->m_dwGuildId==0)
						lstrcpy(szInfo, g_Message[ETC_MESSAGE67].szMessage);		// MSG_ID : 67 ; ���Ե� ��尡 �����ϴ�.
					if(g_pGuildUserHash->GetCount()==0)
						lstrcpy(szInfo, g_Message[ETC_MESSAGE68].szMessage);	// MSG_ID : 68 ; �α����� ������ �����ϴ�.

					DisplayMessageAdd(szInfo, 0xff00ffff);
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}
			}
			else
			{
			//	CChatWnd* pChatWnd = CChatWnd::GetInstance();
				
				if(g_pMainPlayer->m_dwGuildId!=0 && g_pGuildUserHash->GetCount()!=0)
				{
					CTWS_DGCHAT_USER pPacket;
					pPacket.nIndex = 2;	
					memset(pPacket.szId, 0, sizeof(pPacket.szId));
					memset(pPacket.szMessage, 0, sizeof(pPacket.szMessage));
					__lstrcpyn(pPacket.szId, g_pMainPlayer->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
					__lstrcpyn(pPacket.szMessage, m_sScript.szMessage, sizeof(m_sScript.szMessage));
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
				}
				else
				{
					char szInfo[0xff] = {0,};

					if(g_pMainPlayer->m_dwGuildId==0)
						lstrcpy(szInfo, g_Message[ETC_MESSAGE67].szMessage);		// MSG_ID : 67 ; ���Ե� ��尡 �����ϴ�.
					if(g_pGuildUserHash->GetCount()==0)
						lstrcpy(szInfo, g_Message[ETC_MESSAGE68].szMessage);	// MSG_ID : 68 ; �α����� ������ �����ϴ�.

					DisplayMessageAdd(szInfo, 0xff00ffff);
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);						
				}
			}
		}
		break;
	case COMMAND_WHISPER:
		{
			if(GetGameStatus()==UPDATE_GAME_WORLD)
			{
				if(__stricmp(g_pMainPlayer->m_szName, m_sScript.szId)==0)
				{
					DisplayMessageAdd(g_Message[ETC_MESSAGE64].szMessage, 0xffff9a63);	// MSG_ID : 64 ; �ڱ� �ڽſ��Դ� �ӼӸ��� �Ҽ������ϴ�.
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}
				else
				{
					char	szText[0xff] = {0,};
					char	szText2[0xff] = {0,};
					char*	szToken=0;
					char	szSeps[]	= "',?";

					// Id �˻� //
					memset(szText, 0, sizeof(szText));
					memset(szText2, 0, sizeof(szText2));

					__lstrcpyn(szText, m_sScript.szId, MAX_CHARACTER_NAME_REAL_LENGTH);
					__lstrcpyn(szText2, m_sScript.szMessage, sizeof(m_sScript.szMessage));

					szToken = strtok(szText, szSeps);

					if(szToken)
					{
						if(__stricmp(szToken, m_sScript.szId)!=0)
						{
							DisplayMessageAdd(g_Message[ETC_MESSAGE57].szMessage, 0xFFFFB894);	// MSG_ID : 57 ; ���� ���ڸ� ����Ҽ��� �����ϴ�(', ?).
							_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);														
							return 0;
						}
					}
					else
					{
						DisplayMessageAdd(g_Message[ETC_MESSAGE57].szMessage, 0xFFFFB894);	// MSG_ID : 57 ; ���� ���ڸ� ����Ҽ��� �����ϴ�(', ?).
						_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);											
						return 0;
					}

					WSTC_WS_CHAT packet;
					memset(packet.cScript.szId, 0, sizeof(packet.cScript.szId));
					memset(packet.cScript.szMessage, 0, sizeof(packet.cScript.szMessage));
					memset(packet.cScript.szText, 0, sizeof(packet.cScript.szText));				

					szToken = strtok(szText2, szSeps);
					memset(&packet.cScript, 0, sizeof(packet.cScript));

					if(szToken)
					{
						if(__stricmp(szToken, m_sScript.szMessage)==0)
						{
 							__lstrcpyn(szText, m_sScript.szMessage, sizeof(m_sScript.szMessage));
							__lstrcpyn(packet.cScript.szText, szText2, sizeof(szText2));
						}
						else
						{
							memset(szText, 0, sizeof(szText));

							lstrcat(szText, szToken);
							lstrcat(szText, " ");

							while(szToken)
							{
								szToken = strtok(NULL, szSeps);

								if(szToken)
								{
									lstrcat(szText, szToken);
									lstrcat(szText, " ");
								}												
							}
							__lstrcpyn(packet.cScript.szText, szText, sizeof(szText));
						}													
					}									
					packet.cScript.enCommand = COMMAND_WHISPER;
					__lstrcpyn(packet.cScript.szId, m_sScript.szId, MAX_CHARACTER_NAME_REAL_LENGTH);
					__lstrcpyn(packet.cScript.szMessage, m_sScript.szMessage, sizeof(m_sScript.szMessage));
 					g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_WORLD);
				}			
			}
			else
			{														
				if(__stricmp(g_pMainPlayer->m_szName, m_sScript.szId)==0)
				{
					DisplayMessageAdd(g_Message[ETC_MESSAGE64].szMessage, 0xffff9a63);	// MSG_ID : 64 ; �ڱ� �ڽſ��Դ� �ӼӸ��� �Ҽ������ϴ�.
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}
				else
				{	
					char	szText[0xff] = {0,};
					char	szText2[0xff] = {0,};					
					char*	szToken=0;
					char	szSeps[]	= "',?";

					memset(szText, 0, sizeof(szText));
					memset(szText2, 0, sizeof(szText2));
					// Id �˻� //
					__lstrcpyn(szText, m_sScript.szId, MAX_CHARACTER_NAME_REAL_LENGTH);
					__lstrcpyn(szText2, m_sScript.szMessage, sizeof(m_sScript.szMessage));

					szToken = strtok(szText, szSeps);

					if(szToken)
					{													
						if(__stricmp(szToken, m_sScript.szId)!=0)
						{
							DisplayMessageAdd(g_Message[ETC_MESSAGE57].szMessage, 0xFFFFB894);	// MSG_ID : 57 ; ���� ���ڸ� ����Ҽ��� �����ϴ�(', ?).
							_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);														
							return 0;
						}						
					}
					else
					{
						DisplayMessageAdd(g_Message[ETC_MESSAGE57].szMessage, 0xFFFFB894);	// MSG_ID : 57 ; ���� ���ڸ� ����Ҽ��� �����ϴ�(', ?).
						_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);	
						return 0;
					}

					WSTC_DS_CHAT packet;
					memset(packet.cScript.szId, 0, sizeof(packet.cScript.szId));
					memset(packet.cScript.szMessage, 0, sizeof(packet.cScript.szMessage));
					memset(packet.cScript.szText, 0, sizeof(packet.cScript.szText));

					szToken = strtok(szText2, szSeps);
					memset(&packet.cScript, 0, sizeof(packet.cScript));

					if(szToken)
					{
						if(__stricmp(szToken, m_sScript.szMessage)==0)
						{
							__lstrcpyn(szText, m_sScript.szMessage, sizeof(m_sScript.szMessage));
							__lstrcpyn(packet.cScript.szText, szText2, sizeof(szText2));
						}
						else
						{
							memset(szText, 0, sizeof(szText));

							lstrcat(szText, szToken);
							lstrcat(szText, " ");

							while(szToken)
							{
								szToken = strtok(NULL, szSeps);

								if(szToken)
								{
									lstrcat(szText, szToken);
									lstrcat(szText, " ");
								}
							}
							__lstrcpyn(packet.cScript.szText, szText, sizeof(szText));
						}
					}
					packet.cScript.enCommand = COMMAND_WHISPER;
					__lstrcpyn(m_szName, m_sScript.szId, MAX_CHARACTER_NAME_REAL_LENGTH);
					__lstrcpyn(packet.cScript.szId, m_sScript.szId,  MAX_CHARACTER_NAME_REAL_LENGTH);
					__lstrcpyn(packet.cScript.szMessage, m_sScript.szMessage, sizeof(m_sScript.szMessage));
					g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_WORLD);
				}
			}
		}
		break;
	case COMMAND_PARTYAPPROVE:
		{
			// ��Ƽ ��� //
			CTWS_PARTY_SET pPacket;
			pPacket.bParty = TRUE;
			g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);									
		}
		break;
	case COMMAND_PARTYREFUSAL:
		{
			// ��Ƽ �ź� //
			CTWS_PARTY_SET pPacket;
			pPacket.bParty = FALSE;
			g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
			CUserInterface::GetInstance()->m_bParty	= FALSE;
		}
		break;		
	case COMMAND_HELP:
		{
			char szInfo[0xff] = {0,};

			CUserInterface* pUserInterface = CUserInterface::GetInstance();
				
			if(pUserInterface->m_bParty==TRUE)
				lstrcpy(szInfo, g_Message[ETC_MESSAGE640].szMessage); // "���� ���"
			else
				lstrcpy(szInfo, g_Message[ETC_MESSAGE641].szMessage); // "���� �ź�"

			if(pUserInterface->m_bCommand==TRUE)
				lstrcpy(szInfo, g_Message[ETC_MESSAGE642].szMessage); // "�Ӹ� ���"
			else
				lstrcpy(szInfo, g_Message[ETC_MESSAGE643].szMessage); // "�Ӹ� �ź�"

			DisplayMessageAdd(szInfo, 0xffff9a63);
		}
		break;
	case COMMNAD_CHARACTERINFO:
		{
#ifdef DEVELOP_MODE			
			CTWS_CHARACTER_SEARCH pSendPacket;
			memset(&pSendPacket.szName, 0, sizeof(pSendPacket.szName));
			__lstrcpyn(pSendPacket.szName, m_sScript.szId, MAX_CHARACTER_NAME_REAL_LENGTH);
			g_pNet->SendMsg((char*)&pSendPacket, pSendPacket.GetPacketSize(), SERVER_INDEX_WORLD);
#endif
		}
		break;

#if IS_CHINA_LOCALIZING()
	case COMMAND_CHINA_BILL_REMAINTIME:
		{
			DisplayMessageAdd((LPSTR)GetChinaBillingRemainedTimeString(), TEXT_COLOR_GREEN);
		}
		break;
#endif

#ifdef GM_TOOL
	case COMMAND_GM_PK_MODE:
		{
			CTDS_GM_COMMAND Command;
			Command.wCommand	= GM_COMMAND_PK_MODE;
			
			if( __stricmp( m_sScript.szParam[0], g_Message[ETC_MESSAGE262].szMessage ) == 0 ) // "�ϱ�"
			{
				Command.dwTemp[0]	= 1;
				g_pNet->SendMsg( (char*)&Command, Command.GetPacketSize(), SERVER_INDEX_ZONE );
			}
			else if( __stricmp( m_sScript.szParam[0], g_Message[ETC_MESSAGE261].szMessage ) == 0 ) // "���ϱ�"
			{
				Command.dwTemp[0]	= 0;
				g_pNet->SendMsg( (char*)&Command, Command.GetPacketSize(), SERVER_INDEX_ZONE );
			}
		}
		break;

	case COMMAND_GM_CHAT_DISABLE:
		{
			CTDS_GM_COMMAND Command;
			Command.wCommand	= GM_COMMAND_CHAT_DISABLE;

			if( __stricmp( m_sScript.szParam[0], g_Message[ETC_MESSAGE260].szMessage ) == 0 ) // "����ϱ�"
			{
				Command.dwTemp[0]	= 1;		// mean All.
				Command.dwTemp[1]	= 1;		// Enable.
				g_pNet->SendMsg( (char*)&Command, Command.GetPacketSize(), SERVER_INDEX_ZONE );
			}
			else if( __stricmp( m_sScript.szParam[0], g_Message[ETC_MESSAGE259].szMessage ) == 0 ) // "��ξ��ϱ�"
			{
				Command.dwTemp[0]	= 1;		// mean All.
				Command.dwTemp[1]	= 0;			// Enable.
				g_pNet->SendMsg( (char*)&Command, Command.GetPacketSize(), SERVER_INDEX_ZONE );
			}
		}
		break;

	case COMMAND_GM_CREATE:
		{
			CTDS_GM_COMMAND	Command;
			Command.wCommand	= GM_COMMAND_CREATE;

			// ���� ����.
			if( __stricmp( m_sScript.szParam[0], g_Message[ETC_MESSAGE750].szMessage ) == 0 )	// "����"
			{
				Command.dwTemp[0]	= 1;							// mean Create monster.
				Command.dwTemp[1]	= atoi( m_sScript.szParam[1] );	// BaseMonster ID;
//				Command.dwTemp[2]	= atoi( m_sScript.szParam[2] );	// BaseItem ID;
				Command.dwTemp[2]	= 0;	// BaseItem ID;
				g_pNet->SendMsg( (char*)&Command, Command.GetPacketSize(), SERVER_INDEX_ZONE );
			}
			else if( __stricmp( m_sScript.szParam[0], g_Message[ETC_MESSAGE751].szMessage ) == 0 ) // "������"
			{
				Command.dwTemp[0]	= 2;							// mean Create ITEM.
				Command.dwTemp[1]	= atoi( m_sScript.szParam[1] );	// ITEM ID;
				g_pNet->SendMsg( (char*)&Command, Command.GetPacketSize(), SERVER_INDEX_ZONE );
			}
		}
		break;

	case COMMAND_GM_POTAL:
		{
			if( __stricmp( m_sScript.szParam[0], "GM" ) == 0 )		// GM �� �ٷ� �̵��Ѵ�.
			{
				CTDS_PORTAL_MOVE packet;			
				packet.wPortalItemID			= 0;
				packet.wToDungeonID			= atoi( m_sScript.szParam[1] );
				packet.bLayerNo					= atoi( m_sScript.szParam[2] );
				packet.v2Pos.x					= (float)atoi( m_sScript.szParam[3] );
				packet.v2Pos.y					= (float)atoi( m_sScript.szParam[4] );

				g_pNet->SendMsg( (char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE );
			}
		}
		break;

	case COMMAND_GM_RECALL:
		{
			CTWS_GM_COMMAND	Command;
			Command.wCommand	= GM_COMMAND_RECALL;
			__lstrcpyn( Command.szCharName, m_sScript.szParam[0], 20 ); 
			Command.dwTemp[0] = g_pThisDungeon->m_dwID;
			Command.dwTemp[1] = g_pMainPlayer->m_bCurLayer;
			Command.dwTemp[2] = g_pMainPlayer->m_pCurTile->wIndex_X;
			Command.dwTemp[3] = g_pMainPlayer->m_pCurTile->wIndex_Z;

			g_pNet->SendMsg( (char*)&Command, Command.GetPacketSize(), SERVER_INDEX_WORLD );
		}
		break;

	case COMMAND_GM_USERPOTAL:
		{
			CTWS_GM_COMMAND	Command;
			Command.wCommand	= GM_COMMAND_USERPOTAL;

			__lstrcpyn( Command.szCharName, m_sScript.szParam[0], 20 ); 
			Command.dwTemp[0] = atoi( m_sScript.szParam[1] );
			Command.dwTemp[1] = atoi( m_sScript.szParam[2] );
			Command.dwTemp[2] = atoi( m_sScript.szParam[3] );
			Command.dwTemp[3] = atoi( m_sScript.szParam[4] );

			g_pNet->SendMsg( (char*)&Command, Command.GetPacketSize(), SERVER_INDEX_WORLD );
		}
		break;
#endif
	}	
	return 0;
}
//======================================================//
// End.													//
//======================================================//