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
#include "CodeFun.h"
#include "GlobalVariableLogin.h"
#include "ChinaBillingRemainTime.h"
#include "Interface.h"
#include "PacketEncrypt.h"
#include "GuildWarFinalSettingWnd.h"


LPGlobalVariable_Login	g_pGVLogin = NULL;

BOOL g_bExitChk		= FALSE;
BOOL g_bSprChk		= FALSE;
BOOL g_bsprExitChk	= FALSE;

void InitPacketProcLogin()
{
	PacketProc[ UPDATE_GAME_LOGIN ][ Protocol_Login::CMD_LOGIN_SUCCESS ]			= CmdLoginSuccess;	
	PacketProc[ UPDATE_GAME_LOGIN ][ Protocol_Login::CMD_LOGIN_FAIL ]				= CmdLoginFail;	
	PacketProc[ UPDATE_GAME_LOGIN ][ Protocol_Login::CMD_LOGIN_FAIL_WITH_REASON ]	= CmdLoginFailWithReason;
	PacketProc[ UPDATE_GAME_LOGIN ][ Protocol_Login::CMD_CHINA_BILL_REMAINED_TIME ]	= CmdLoginChinaBillRemainedTime;

// Begin - minjin. 2004. 09. 22.
// #ifdef __PACKET_ENCRYPTION	// enum ���ǿ� ������ �Ǿ��ִ�.. ��� ����..
	
	// �α��� ��ȣȭ ����..
	PacketProc[ UPDATE_GAME_LOGIN ][ Protocol_Login::CMD_ENCRYPTION_KEY ]			= CmdEncryptionKey;
// #endif
// End - minjin.
}

BOOL InitGameLogin()
{
	g_byStatusMessenger	= 0;	
	g_byChkMessenger	= 0;
	g_dwStatusTime		= 0;

	// set not-chat mode!
	g_pInputManager->SetFocusInput(INPUT_BUFFER_0);
	g_nChatModeExt		= 0;	
	
	LoadTotalDungeonInfo();

	IMESetEdit(0, TRUE, FALSE, TRUE);
	
	if( g_pMainPlayer )
	{
		//g_pUserHash->Delete( g_pMainPlayer, g_pMainPlayer->m_dwUserIndex );
		ReleaseMainPlayerResource();
		ReleaseMainPlayer();
	}
	
	SetRenderMode(RENDER_MODE_MULTIPLE);

	if(!g_pGVLogin)
	{
		g_pGVLogin = new GLOBAL_VARIABLE_LOGIN;
		memset(g_pGVLogin, 0, sizeof(GLOBAL_VARIABLE_LOGIN));
	}

	// by deepdark. �߱����� connect.ini ó���� ���� ������ ��.
	if(!g_pGVLogin->LoadConnectionInfo()) return FALSE;
	
	//wsprintf(szFile, "%s\\Data\\Sound\\%s", g_szBasePath, FILE(BGM_TITLE));
//	g_pDigitalAudioSet->OpenFileStream(FILE(BGM_TITLE));
//	g_pDigitalAudioSet->PlayStream();
	CGameMenuWnd * pGameMenuWnd = CGameMenuWnd::GetInstance();
	
	if(pGameMenuWnd->m_bBGMFlag)
	{
		PlayBGM( GetFile( FILE(BGM_TITLE), DATA_TYPE_SOUND ) );
		pGameMenuWnd->m_dwBGMId = BGM_TITLE;
	}
	g_pGVLogin->bIntroFlag = INTRO_FLAG_WINDOW_FADE_IN;//INTRO_FLAG_DEMO_SHOWING;	//Ÿ��Ʋ ���� ���  
	CInterface*	pInterface	= CInterface::GetInstance();
	pInterface->InitInterface(0);	

	g_pSprManager->CreateSprite(SPR_LOGIN_BACKGROUND, 0.0f, 0.0f, TRUE, 254); // resourceid 344
#if IS_JAPAN_LOCALIZING() || IS_KOREA_LOCALIZING()//�ϴ� �Ϻ��ϱ� �ѱ�, ���߿� ����....
	g_pSprManager->CreateSprite(SPR_LOGIN_SERVERSELECT_WINDOW, 402, 459, FALSE);// resourceid 345
#else
	g_pSprManager->CreateSprite(SPR_LOGIN_SERVERSELECT_WINDOW, 402, 380, FALSE);// resourceid 345
#endif
	V2_SPRITE* pSpr = g_pSprManager->CreateSprite(SPR_CS_BLACK_LINE, 0, 0, TRUE, 255 );// resourceid 346
	pSpr->v2Scaling.x = 8.0f;
	pSpr->v2Scaling.y = 6.0f;

	

	g_pSprManager->CreateSprite(SPR_LOGIN_LOGIN_WINDOW, 402, 459, FALSE);// resourceid 347

	g_pSprManager->CreateSprite(BUTTON_OK		, 523, 568, 0, 0, 64, 16, FALSE, 1);// resourceid 348
	g_pSprManager->CreateSprite(BUTTON_OK_OVER	, 523, 568, 0, 16, 64, 16, FALSE, 1);// resourceid 349
	g_pSprManager->CreateSprite(BUTTON_OK_DOWN  , 523, 568, 0, 32, 64, 16, FALSE, 1);// resourceid 350

	g_pSprManager->CreateSprite(BUTTON_CANCEL		, 588, 568, 64, 0, 64, 16, FALSE, 1);// resourceid 351
	g_pSprManager->CreateSprite(BUTTON_CANCEL_OVER	, 588, 568, 64, 16, 64, 16, FALSE, 1);// resourceid 352
	g_pSprManager->CreateSprite(BUTTON_CANCEL_DOWN  , 588, 568, 64, 32, 64, 16, FALSE, 1);// resourceid 353
	
#if IS_JAPAN_LOCALIZING() || IS_KOREA_LOCALIZING()//�ϴ� �Ϻ��ϱ� �ѱ�, ���߿� ����....
	g_pSprManager->CreateSprite(SPR_LOGIN_EXIT1, 578, 568, 128, 0, 64, 16, FALSE, 4);// resourceid 354
	g_pSprManager->CreateSprite(SPR_LOGIN_EXIT2, 578, 568, 128, 16, 64, 16, FALSE, 4);// resourceid 355
	g_pSprManager->CreateSprite(SPR_LOGIN_EXIT3, 578, 568, 128, 32, 64, 16, FALSE, 4);	// resourceid 356
#else
	g_pSprManager->CreateSprite(SPR_LOGIN_EXIT1, 578, 618, 128, 0, 64, 16, FALSE, 4);// resourceid 354
	g_pSprManager->CreateSprite(SPR_LOGIN_EXIT2, 578, 618, 128, 16, 64, 16, FALSE, 4);// resourceid 355
	g_pSprManager->CreateSprite(SPR_LOGIN_EXIT3, 578, 618, 128, 32, 64, 16, FALSE, 4);	// resourceid 356
#endif

	SetMousePointerType(__MOUSE_POINTER_DEFAULT__);
	
	//Login����ǲ���� ����
	g_pInputManager->Initialize();	
	
	RECT rt;
#if IS_CHINA_LOCALIZING()
	rt.top = 492; rt.bottom = rt.top + 14; rt.left = 476; rt.right  = rt.left + 120;
	g_pInputManager->InitializeInput(INPUT_BUFFER_LOGIN_ID, FONT_SS3D_GAME, rt, 0xffffffff, 3);

	rt.top = 509; rt.bottom = rt.top + 14; rt.left = 476; rt.right  = rt.left + 200;
	g_pInputManager->InitializeInput(INPUT_BUFFER_LOGIN_PASSWORD, FONT_SS3D_GAME, rt, 0xffffffff, 3);
#else
	rt.top = 492; rt.bottom = rt.top + 14; rt.left = 485; rt.right  = rt.left + 120;
	g_pInputManager->InitializeInput(INPUT_BUFFER_LOGIN_ID, FONT_SS3D_GAME, rt, 0xffffffff, 3);

	rt.top = 509; rt.bottom = rt.top + 14; rt.left = 485; rt.right  = rt.left + 120;
	g_pInputManager->InitializeInput(INPUT_BUFFER_LOGIN_PASSWORD, FONT_SS3D_GAME, rt, 0xffffffff, 3);
#endif

	VECTOR3		From;
	VECTOR3		Rotate;
/*	
	From.x = -48.0f;							
	From.y = 145.71f;							
	From.z = 23.98f;	
*/
	From.x = -210.88f;
	From.y = -80.90f;
	From.z = 200.34f;

	//g_pGeometry->ResetCamera(&From, 100.0f, 80000.0f, DEG45, 0 );
	g_pGeometry->ResetCamera(&From, 100.0f, 80000.0f, 0.90000000000000000000f, 0 );

	//Rotate.x = -0.40f;	Rotate.y = 6.28f;	Rotate.z = 0.0f;
	Rotate.x = 0.14f;	Rotate.y = 3.56f;	Rotate.z = 0.0f;
	g_pExecutive->GetGeometry()->RotateCameraRad(&Rotate, 0);	
	g_pObjManager->CreateObject(CHR_MAIN_TITLE, 0.0f, 0.0f, 0.0f, ACTION_LOOP, true, 0);

//	g_pExecutive->GXOSetEffectIndex(pDesc->pHandle, 0);
/*
	g_FogDesc.fStart	= 500.0f;
	g_FogDesc.fEnd		= 8000.0f;
	g_FogDesc.dwColor	= 0xffffcccc;
	g_FogDesc.fDensity	= 1.0f;
	g_pGeometry->SetFog(&g_FogDesc,0);
	g_pGeometry->EnableFog(0);7
*/
/*
	DIRECTIONAL_LIGHT_DESC	dirLight;
	dirLight.v3Dir.x	= -1.0f;
	dirLight.v3Dir.y	= 1.0f;
	dirLight.v3Dir.z	= -1.0f;
	dirLight.dwSpecular = 0xffffffff;
	dirLight.dwDiffuse	= 0xffc8c8c8;
	dirLight.dwAmbient	= 0x00b4b4b4;

	g_pGeometry->SetDirectionalLight(&dirLight,0);
	g_pGeometry->EnableDirectionalLight(0);
*/

	LIGHT_DESC	shadowlight1;
	GXLIGHT_HANDLE hLight1 = NULL;

	shadowlight1.dwAmbient = 0xffffffdd;
	shadowlight1.dwDiffuse = 0xffffffdd;
	shadowlight1.dwSpecular = 0;
	shadowlight1.v3Point.x = 1113.84f;
	shadowlight1.v3Point.y = 395.69f;
	shadowlight1.v3Point.z = -443.95f;
	shadowlight1.fFov = PI;
	//shadowlight1.v3Up.x = 0.0f;
	shadowlight1.v3Up.x = 1.0f;
	shadowlight1.v3Up.y = 1.0f;
	shadowlight1.v3Up.z = 0.0f;

	shadowlight1.fRs = 10000.0f;
	shadowlight1.v3To.x = 0.0f;
	shadowlight1.v3To.y = 0.0f;
	shadowlight1.v3To.z = 0.0f;

	hLight1 = g_pExecutive->CreateGXLight(&shadowlight1,NULL,0,1, NULL, 0);
	g_pExecutive->GXLEnableDynamicLight(hLight1);		

	CGameDefaultWnd*	pGameDefaultWnd = CGameDefaultWnd::GetInstance();
	CBlockWnd*			pBlockWnd		= CBlockWnd::GetInstance();
	
	pGameDefaultWnd->Init();	
	pGameDefaultWnd->SetPos(384, 320);
	pBlockWnd->Init();	
	pBlockWnd->SetPos(384, 320);

	if(g_pAllChatList)
		g_pAllChatList->RemoveAllData();
	if(g_pPartyChatList)
		g_pPartyChatList->RemoveAllData();
	if(g_pFriendChatList)
		g_pFriendChatList->RemoveAllData();
	if(g_pGuildChatList)
		g_pGuildChatList->RemoveAllData();
	if(g_pWhisperChatList)
		g_pWhisperChatList->RemoveAllData();	
	if(g_pPartyUserHash)
		g_pPartyUserHash->Destroy(TRUE);	
	if(g_pGuildUserHash)
		g_pGuildUserHash->Destroy(TRUE);
	if(g_pGuildOfflineHash)
		g_pGuildOfflineHash->Destroy(TRUE);
	if(g_pMessengerOffLineHash)	
		g_pMessengerOffLineHash->Destroy(TRUE);
	if(g_pMessengerOnLineHash)
		g_pMessengerOnLineHash->Destroy(TRUE);
	if(g_pGuildWarHash)
		g_pGuildWarHash->Destroy(TRUE);
	if(g_pPartyBoardHash)
		g_pPartyBoardHash->Destroy(TRUE);
	
	
	return TRUE;
}

static void UpdateServerSetInfoToUI()
{
	g_pGVLogin->bIntroFlag = INTRO_FLAG_SERVER_SELECT;

	RECT rt;

	for(DWORD i=0; i<g_pGVLogin->GetTotalSetNumber(); i++)
	{
		//Server Set Display Rect - 01 Line
#if IS_JAPAN_LOCALIZING() || IS_KOREA_LOCALIZING()//�ϴ� �Ϻ��ϱ� �ѱ�, ���߿� ����....
		rt.left = 484;	rt.right = rt.left + 100;	rt.top = 492-1 + (i*19);	rt.bottom = rt.top + 14;
#else
		rt.left = 484;	rt.right = rt.left + 100;	rt.top = 412 + (i*19);	rt.bottom = rt.top + 14;
#endif
		g_pInputManager->InitializeInput(INPUT_BUFFER_LOGIN_SERVER_LIST+i, FONT_SS3D_LOGIN, rt, 0xffffffff, 2+i);
		g_pInputManager->InsertCharacter(INPUT_BUFFER_LOGIN_SERVER_LIST+i, 
			g_pGVLogin->GetSetInfo(i)->szSetName, lstrlen(g_pGVLogin->GetSetInfo(i)->szSetName));			
	}
	
	SPR(SPR_LOGIN_SERVERSELECT_WINDOW)->ShowSprite(true);
	SPR(SPR_CS_BLACK_LINE)->SetAlpha(0);
}

static void ServerSelectFunc()
{
	if(g_bExecuteType == EXECUTE_TYPE_CORUM)	//�ڷ��϶��� 
	{			
		SPR(SPR_LOGIN_EXIT1)->ShowSprite(TRUE);
		g_bsprExitChk = TRUE;
	    // �迵�� >>>>>>>>>>>>>>>>
		UpdateServerSetInfoToUI();
	}
	else	//�ݸ����϶� 
	{
		g_pGVLogin->bIntroFlag = INTRO_FLAG_CONTINUE_SHOWING;	
	}
}

static void SetMousePointerOnList(WORD x, WORD y, BYTE btOnType, BYTE btOutType)
{
	BOOL bChk = FALSE;

	for(DWORD i = 0; i < g_pGVLogin->GetTotalSetNumber(); i++)
	{
		bChk = g_pInputManager->PtInRect(INPUT_BUFFER_LOGIN_SERVER_LIST+i, x, y);
		if (bChk) break;
	}

	SetMousePointerType(bChk ? btOnType : btOutType);
}

inline BOOL IsExitButton(WORD x, WORD y)
{
#if IS_JAPAN_LOCALIZING() || IS_KOREA_LOCALIZING()//�ϴ� �Ϻ��ϱ� �ѱ�, ���߿� ����....
	return (x>=578 && x<=642 && y>=568 && y<=584);
#else
	return (x>=578 && x<=642 && y>=618 && y<=634);
#endif
}

static void SetMousePointerOnAccountInput(WORD x, WORD y, BYTE btOnType, BYTE btOutType)
{
	if(	(x>=523 && x<=587 && y>=568 && y<=584) ||
		(x>=588 && x<=652 && y>=568 && y<=584) ||
		(x>=485 && x<=585 && y>=509 && y<=521) ||
		(x>=485 && x<=585 && y>=492 && y<=504) )
		SetMousePointerType(btOnType);
	else
		SetMousePointerType(btOutType);
}

static void InputBufferColorSetting()
{
	for(DWORD i=0; i<g_pGVLogin->GetTotalSetNumber(); i++)
	{
		if(g_pGVLogin->GetSelectedSet() == i && g_pGVLogin->GetSelectedSet() != 0xffffffff)
			g_pInputManager->SetInputColor(INPUT_BUFFER_LOGIN_SERVER_LIST+i, 0xffff00ff);
		else
			g_pInputManager->SetInputColor(INPUT_BUFFER_LOGIN_SERVER_LIST+i, 0xffffffff);
			
		g_pInputManager->RenderInputBuffer(INPUT_BUFFER_LOGIN_SERVER_LIST+i);			
	}
}

void UpdateGameLogin()
{ 
	switch(g_pGVLogin->bIntroFlag) 
	{
	case INTRO_FLAG_WINDOW_FADE_IN:
		{
			BYTE bAlpha = *( ( (BYTE*)&(SPR(SPR_CS_BLACK_LINE)->dwAlpha) ) + 3 );

			if(bAlpha >= 3)
			{
				bAlpha -= 2;
				SPR(SPR_CS_BLACK_LINE)->SetAlpha(bAlpha);
				return;
			}
			ServerSelectFunc();
		}
		break;
	}
}

void ReleaseGameLogin()
{
	g_pGVLogin->ReleaseConnectionInfo();

	g_pSprManager->RemoveBackground();	
	g_pSprManager->RemoveSprite(SPR_LOGIN_SERVERSELECT_WINDOW);
	
	g_pObjManager->RemoveObject(CHR_MAIN_TITLE);		//Intro Demo ����� 
	g_pSprManager->RemoveSprite(SPR_LOGIN_BACKGROUND);
	g_pSprManager->RemoveSprite(SPR_CS_BLACK_LINE);
		

	g_pGVLogin->bIsLoginInputMode = 0;

	SPR(SPR_LOGIN_LOGIN_WINDOW)->ShowSprite(FALSE);

	SPR(BUTTON_OK)->ShowSprite(FALSE);
	SPR(BUTTON_OK)->vPos.x		= 123;
	SPR(BUTTON_OK_OVER)->vPos.x = 123;		
	SPR(BUTTON_OK_DOWN)->vPos.x = 123;		

	SPR(BUTTON_CANCEL)->ShowSprite(FALSE);
	SPR(BUTTON_OK)->vPos.y		= 654;
	SPR(BUTTON_OK_OVER)->vPos.y = 654;
	SPR(BUTTON_OK_DOWN)->vPos.y = 654;

	SPR(BUTTON_CANCEL)->vPos.x		= 190;
	SPR(BUTTON_CANCEL_OVER)->vPos.x = 190;		
	SPR(BUTTON_CANCEL_DOWN)->vPos.x = 190;

	SPR(BUTTON_CANCEL)->vPos.y		= 654;
	SPR(BUTTON_CANCEL_OVER)->vPos.y = 654;
	SPR(BUTTON_CANCEL_DOWN)->vPos.y = 654;

	g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_ID);
	g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
	g_pInputManager->SetFocusInput(INPUT_BUFFER_0);
	
	RELEASE_SPR(SPR_LOGIN_EXIT1);
	RELEASE_SPR(SPR_LOGIN_EXIT2);
	RELEASE_SPR(SPR_LOGIN_EXIT3);
	
	CGameDefaultWnd*	pGameDefaultWnd = CGameDefaultWnd::GetInstance();
	CBlockWnd*			pBlockWnd		= CBlockWnd::GetInstance();
	
	pGameDefaultWnd->Remove();		
	pBlockWnd->Remove();	
}

DWORD __stdcall AfterRenderGameLogin()
{
	g_pSprManager->RenderAllSprite();	

	CGameDefaultWnd*	pGameDefaultWnd = CGameDefaultWnd::GetInstance();	
	CBlockWnd*			pBlockWnd		= CBlockWnd::GetInstance();

	if(pBlockWnd->GetActive() == TRUE)
		pBlockWnd->RenderText();	
	if(pGameDefaultWnd->GetActive()==TRUE)
	{
		pGameDefaultWnd->Render();
		pGameDefaultWnd->RenderText();		
	}
	else
	{
		g_pInputManager->RenderInputBuffer(INPUT_BUFFER_LOGIN_ID);
		
		// ���� ���콺�� ��ġ�� ���� ���ڻ��� ����.
		InputBufferColorSetting();

		g_pInputManager->RenderConvertToSpecialCharacter(INPUT_BUFFER_LOGIN_PASSWORD, '*');		
	}

//	CInterface::GetInstance()->Render();
	
	return 0;
}

DWORD __stdcall BeforeRenderGameLogin()
{
	g_pSprManager->RenderBackground();	
	return 0;
}

void OnSprButtonProcessLogin(DWORD dwSprID)
{
	switch(dwSprID)
	{
	case BUTTON_OK:
		{
			CGameDefaultWnd* pGameDefaultWnd = CGameDefaultWnd::GetInstance();

			if(pGameDefaultWnd->GetActive()==TRUE)
				return;
			
			if(g_pInputManager->GetCurFocusInputID() == INPUT_BUFFER_LOGIN_ID)
			{
				g_pInputManager->ClearInput(INPUT_FOCUS_PASSWORD);
				g_pInputManager->SetFocusInput(INPUT_FOCUS_PASSWORD);				
			}
			else
			{
				LoginProcess();			//LC-01	
			}
		}
		break;
	case BUTTON_CANCEL:
		{
			CGameDefaultWnd* pGameDefaultWnd = CGameDefaultWnd::GetInstance();

			if(pGameDefaultWnd->GetActive()==TRUE)
				return;

			g_pNet->DisconnectServer(SERVER_INDEX_AGENT);

#ifdef __PACKET_ENCRYPTION
			// �α��� �������� ������ �����Ƿ�.. Ű�� �ٽ� �����Ѵ�.
			// modified by minjin. 2004. 11. 05.
			ResetEncryptionKey();
#endif
			
			//�ٽ� ServerSelect ���� 
			SPR(SPR_LOGIN_SERVERSELECT_WINDOW)->ShowSprite(TRUE);
			SPR(SPR_LOGIN_LOGIN_WINDOW)->ShowSprite(FALSE);

			SPR(BUTTON_OK)->ShowSprite(FALSE);
			SPR(BUTTON_OK_DOWN)->ShowSprite(FALSE);
			SPR(BUTTON_OK_OVER)->ShowSprite(FALSE);
			
			SPR(BUTTON_CANCEL)->ShowSprite(FALSE);
			SPR(BUTTON_CANCEL_DOWN)->ShowSprite(FALSE);
			SPR(BUTTON_CANCEL_OVER)->ShowSprite(FALSE);

			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_ID);
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			
			// �迵�� >>>>>>>>>>>>>>>>
			UpdateServerSetInfoToUI();
			
			g_pGVLogin->bIsLoginInputMode = 0; 
			g_pInputManager->SetFocusInput(INPUT_BUFFER_0);
			
			g_pGVLogin->bIsLoging = FALSE;
		}
		break;
	}
}

void LoginProcess()
{
	CGameDefaultWnd* pGameDefaultWnd = CGameDefaultWnd::GetInstance();			

	//���� �α��� ���̶�� �޼��� �ڽ� �ٿ�� ���� 
	if(g_pGVLogin->bIsLoging)	
	{
		pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE316].szMessage, "", "", 402, 459, 1);		// MSG_ID : 316 ; �α������Դϴ�. ��ø� ��ٸ�����!
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);				
		pGameDefaultWnd->SetActive();
		LoginInterface(FALSE);
		return;
	}

	int ch = '\'';	

	if( strchr( g_pInputManager->GetInputBuffer(INPUT_BUFFER_LOGIN_ID), ch) )
	{
		pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE317].szMessage, "", "", 402, 459, 1);	// MSG_ID : 317 ; ��ϵ��� ���� �����Դϴ�.
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);				
		pGameDefaultWnd->SetActive();
		LoginInterface(FALSE);		
		return;
	}
	
	if( strchr( g_pInputManager->GetInputBuffer(INPUT_BUFFER_LOGIN_PASSWORD), ch) )
	{
		pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE318].szMessage, "", "", 402, 459, 1);		// MSG_ID : 318 ; ��й�ȣ�� Ʋ�Ƚ��ϴ�.
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);				
		pGameDefaultWnd->SetActive();		
		LoginInterface(FALSE);

		if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)
			pGameDefaultWnd->m_bExit = TRUE;

		return; 
	}
	
	if(g_pInputManager->GetCurFocusInputID() == INPUT_BUFFER_LOGIN_ID)	//021203
	{
		g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
		g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);		
	}
	else
	{
		::KillTimer(g_hMainWnd, 1);	//Cursor ������ ���̰� 		
		
		if(!g_dwConnectIndex[SERVER_INDEX_AGENT])		//������ ���� ���Ѱ�� 
		{
			if( !g_pNet->ConnectServer( g_pGVLogin->ServerSetInfo.pServerInfo[ g_pGVLogin->bConnectServerIndex ].szIp, LOGIN_AGENT_CONNECT_PORT, SERVER_INDEX_AGENT ) )	
			{
				pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE319].szMessage, "", "", 402, 459, 1);	// MSG_ID : 319 ; Fail to connect World Server!
				_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);				
				pGameDefaultWnd->SetActive();
				LoginInterface(FALSE);
				/*char szBuf[ 0xff ];
				wsprintf(szBuf, "Fail to connect World Server! (%d, %s, %d)",  SERVER_INDEX_AGENT, g_pGVLogin->ServerSetInfo.pServerInfo[ g_pGVLogin->bConnectServerIndex ].szIp, WORLD_SERVER_CONNECT_PORT );*/								

				if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)
					pGameDefaultWnd->m_bExit = TRUE;
				
				return;
			}
		}		
			
		char* szTemp = strtok(g_pInputManager->GetInputBuffer(INPUT_BUFFER_LOGIN_ID), " ");
		
		if(!szTemp)
		{			
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE320].szMessage, "", "", 402, 459, 1);	// MSG_ID : 320 ; ������ �Է��� �ֽʽÿ�.
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);				
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			// ��й�ȣ�� Focus �ְ� �����̰� //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_ID);	
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);	
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
						
			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)
				pGameDefaultWnd->m_bExit = TRUE;

			return;
		}	
		
		szTemp = strtok(g_pInputManager->GetInputBuffer(INPUT_BUFFER_LOGIN_PASSWORD), " ");

		if(!szTemp)
		{			
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE321].szMessage, "", "", 402, 459, 1);	// MSG_ID : 321 ; ��й�ȣ�� �Է��� �ֽʽÿ�.
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);				
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			// ��й�ȣ�� Focus �ְ� �����̰� //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);	
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)
				pGameDefaultWnd->m_bExit = TRUE;

			return;
		}
		
		CTWS_LOGIN	packet;
		memset(packet.szID, 0, MAX_ID_LENGTH);
		memset(packet.szPassword, 0, MAX_PASSWORD_LENGTH);		
		__lstrcpyn(packet.szID, g_pInputManager->GetInputBuffer(INPUT_BUFFER_LOGIN_ID), MAX_ID_REAL_LENGTH);				//ID
		__lstrcpyn(packet.szPassword, g_pInputManager->GetInputBuffer(INPUT_BUFFER_LOGIN_PASSWORD), MAX_PASSWORD_REAL_LENGTH);	//Password		
		packet.dwVersion = g_dwVersion;
//		g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_AGENT);
//		g_pGVLogin->bIsLoging = TRUE;

#ifdef __PACKET_ENCRYPTION

		// Ŭ���̾�Ʈ ��Ʈ Ű�� �����Ѵ�.
		RandomKeyGenerator(packet.byClientKey);
		
		SetClientKey(packet.byClientKey);

#endif // __PACKET_ENCRYPTION
		
		
		// �α��� ��Ŷ ���⼭ ������.
		
		//	by minjin. 2004. 10. 25.		
		SendMsgWithBlowfish((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_AGENT);
		//g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_AGENT);

		g_pGVLogin->bIsLoging = TRUE;		
	}
}

void OnKeyDownLogin(WPARAM wParam, LPARAM lParam)
{
	_CHECK_MEMORY();
}

void OnKeyUpLogin(WPARAM wParam, LPARAM lParam)
{
}

static void NetMarbleLogin()
{
	if(g_NetMarble.bLoging)
		return;
	
	if(!g_dwConnectIndex[SERVER_INDEX_AGENT])		//������ ���� ���Ѱ�� 
	{
		if( !g_pNet->ConnectServer( g_NetMarble.szIP, (WORD)g_NetMarble.dwPort, SERVER_INDEX_AGENT ) )	
		{
			CGameDefaultWnd* pGameDefaultWnd = CGameDefaultWnd::GetInstance();			
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE319].szMessage, "", "", 402, 459, 1);	// MSG_ID : 319 ; Fail to connect World Server!
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);				
			pGameDefaultWnd->SetActive(); 
			LoginInterface(FALSE);
			
			if(g_bExecuteType==EXECUTE_TYPE_NETMARBLE)
				pGameDefaultWnd->m_bExit = TRUE;

			return;
		}
	}		

	// seven 
	// �ݸ����� ��Ű ���� �ѱ��.
#ifdef J0_WORK_SECTION
	asm_int3();
#endif
	CTWS_NETMARBLE_LOGIN	packet;
	packet.bNetmarleLogin	= TRUE;
	packet.szID[0]			= '\0';
	packet.szPassword[0]	= '\0';
	memset(packet.szCPCookie, 0, MAX_CP_COOKIE_LENGTH );

	packet.dwVersion = g_dwVersion;
	__lstrcpyn(packet.szCPCookie, g_NetMarble.szCPCookie, lstrlen(g_NetMarble.szCPCookie) );

	//	by minjin. 2004. 10. 25.
	SendMsgWithBlowfish((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_AGENT);
	//g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_AGENT);

	g_NetMarble.bLoging = TRUE;
}

void OnLButtonDownLogin(WPARAM wParam, LPARAM lParam)
{	
	// ������ �׽�Ʈ
//	CGuildWarFinalSettingWnd::GetInstance()->CheckInterface();
	// ������ �׽�Ʈ
	CBlockWnd*			pBlockWnd		= CBlockWnd::GetInstance();
	CGameDefaultWnd*	pGameDefaultwnd = CGameDefaultWnd::GetInstance();

	if(pBlockWnd->GetActive()==TRUE)
	{
		pBlockWnd->SetUp(FALSE);
		pBlockWnd->CheckInterface();			
		return;
	}	

	// �ݸ��� �α� ó��
	if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)
	{
		NetMarbleLogin();
		return;
	}
	
	if(pGameDefaultwnd->GetActive()==TRUE)
	{
		pGameDefaultwnd->SetUp(FALSE);
		
		if(pGameDefaultwnd->CheckInterface()==1)
		{
			// LoginInterface(TRUE);
		}		
		return;
	}

	WORD x = LOWORD(lParam);
	WORD y = HIWORD(lParam);

	if(IsExitButton(x, y))
	{
		if(g_Mouse.bLDown && g_bSprChk && g_bsprExitChk)
		{
			SPR(SPR_LOGIN_EXIT1)->ShowSprite(FALSE);
			SPR(SPR_LOGIN_EXIT2)->ShowSprite(FALSE);
			SPR(SPR_LOGIN_EXIT3)->ShowSprite(TRUE);
		}				
	}	

	/*
	if(g_pGVLogin->bIntroFlag == INTRO_FLAG_DEMO_SHOWING)
	{
		g_pSprManager->GetSprite(SPR_LOGIN_BACKGROUND)->SetAlpha(0);
		g_pSprManager->GetSprite(SPR_LOGIN_BACKGROUND)->ShowSprite(true);
		g_pGVLogin->bIntroFlag = INTRO_FLAG_WINDOW_FADE_IN;
		return;
	}
	*/

	// �����̳� ���� ���� ���� ��� ���콺 ������ Ÿ�� ����.
	if(	g_pGVLogin->bIntroFlag == INTRO_FLAG_SERVER_SELECT)
	{
		SetMousePointerOnList(x, y, __MOUSE_POINTER_BUTTONCLICK__, __MOUSE_POINTER_DEFAULTCLICK__);
	}
	else	//���̵�� ��й�ȣ �Է��ϴ� ��� 	
	{
		SetMousePointerOnAccountInput(x, y, __MOUSE_POINTER_BUTTONCLICK__, __MOUSE_POINTER_DEFAULTCLICK__);
	}

	//ID, Password �Է�ĭ Ŭ���ؼ� ��Ŀ�� �ִ� ó�� 
	if(g_pGVLogin->bIsLoginInputMode)
	{
		for(int i=INPUT_BUFFER_LOGIN_ID; i<=INPUT_BUFFER_LOGIN_PASSWORD; i++)
		{
			if(g_pInputManager->PtInRect(i, x, y))
			{				
				g_pInputManager->SetFocusInput(i);
				goto lb_continue;
			}
		}
	}
	

lb_continue:

	V2_SPRITE* pSprOKDown = g_pSprManager->GetSprite(BUTTON_OK_DOWN);
	V2_SPRITE* pSprOKOver = g_pSprManager->GetSprite(BUTTON_OK_OVER);
	V2_SPRITE* pSprCancelDown = g_pSprManager->GetSprite(BUTTON_CANCEL_DOWN);
	V2_SPRITE* pSprCancelOver = g_pSprManager->GetSprite(BUTTON_CANCEL_OVER);

	if( pSprOKOver->PtInSpr(x, y) )
	{
		pSprOKOver->ShowSprite(FALSE);
		pSprOKDown->ShowSprite(TRUE);
	}

	if(pSprCancelOver->PtInSpr(x, y))
	{
		pSprCancelOver->ShowSprite(FALSE);
		pSprCancelDown->ShowSprite(TRUE);
	}

	if(g_pGVLogin->bIsLoginInputMode)	
		return;

	// ���� �����ϴ� ���̶��, Ŭ���� ���� �������� �����Ѵ�.
	// �׸���, �������̽��� �����Ѵ�.
	if(g_pGVLogin->bIntroFlag == INTRO_FLAG_SERVER_SELECT)
	{
		for(DWORD i=0; i<g_pGVLogin->GetTotalSetNumber(); i++)
		{
			if(g_pInputManager->PtInRect(INPUT_BUFFER_LOGIN_SERVER_LIST+i, x, y))
			{
				g_pGVLogin->bConnectServerIndex = BYTE(i);
				g_pGVLogin->SetCurSet(i);
				
				for(DWORD j=0; j<g_pGVLogin->GetTotalSetNumber(); j++)
					g_pInputManager->ClearInput(j+INPUT_BUFFER_LOGIN_SERVER_LIST);
				
				g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
				g_pGVLogin->bIsLoginInputMode = 1; 
				g_pGVLogin->bIntroFlag = INTRO_FLAG_LOGIN_INPUT;

				g_bsprExitChk = FALSE;

				SPR(SPR_LOGIN_EXIT1)->ShowSprite(FALSE);
				SPR(SPR_LOGIN_EXIT2)->ShowSprite(FALSE);
				SPR(SPR_LOGIN_EXIT3)->ShowSprite(FALSE);				

				g_pSprManager->GetSprite(SPR_LOGIN_SERVERSELECT_WINDOW)->ShowSprite(FALSE);
				g_pSprManager->GetSprite(SPR_LOGIN_LOGIN_WINDOW)->ShowSprite(TRUE);

				g_pSprManager->GetSprite(BUTTON_OK)->ShowSprite(TRUE);
				g_pSprManager->GetSprite(BUTTON_OK_DOWN)->ShowSprite(FALSE);
				g_pSprManager->GetSprite(BUTTON_OK_OVER)->ShowSprite(FALSE);
				
				g_pSprManager->GetSprite(BUTTON_CANCEL)->ShowSprite(TRUE);
				g_pSprManager->GetSprite(BUTTON_CANCEL_DOWN)->ShowSprite(FALSE);
				g_pSprManager->GetSprite(BUTTON_CANCEL_OVER)->ShowSprite(FALSE);			

				return;
			}
		}
	}
}

void OnLButtonUpLogin(WPARAM wParam, LPARAM lParam)
{

	// ������ �׽�Ʈ
//	CGuildWarFinalSettingWnd::GetInstance()->CheckInterface();
	// ������ �׽�Ʈ

	if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)	return;

	CGameDefaultWnd*	pGameDefaultwnd = CGameDefaultWnd::GetInstance();
	CBlockWnd*			pBlockWnd		= CBlockWnd::GetInstance();
		
	if(pBlockWnd->GetActive()==TRUE)
	{
		pBlockWnd->SetUp(TRUE);

		if(pBlockWnd->CheckInterface()==TRUE)
		{
		}
		return;
	}
	
	if(pGameDefaultwnd->GetActive()==TRUE)
	{
		pGameDefaultwnd->SetUp(TRUE);
		
		if(pGameDefaultwnd->CheckInterface()==1)
		{
			if(g_bExitChk==TRUE)
			{
				g_bIsRunning = FALSE;
				return; 
			}

			if(pGameDefaultwnd->m_byType==167)
			{
				LoginInterface(FALSE);
				g_bsprExitChk = TRUE;
			}
			else
				LoginInterface(TRUE);
		}
		return;
	}

	if(g_pGVLogin->bIntroFlag == INTRO_FLAG_WINDOW_FADE_IN)
	{
		SPR(SPR_LOGIN_EXIT1)->ShowSprite(TRUE);
		g_bsprExitChk = TRUE;

		// �迵�� >>>>>>>>>>>>>>>>
		UpdateServerSetInfoToUI();
		return;
	}

	WORD x = LOWORD(lParam);
	WORD y = HIWORD(lParam);

	if(IsExitButton(x, y))
	{
		if(g_bsprExitChk)
		{
			g_bIsRunning = FALSE;
			return;		
		}
	}	
		
	V2_SPRITE* pSprOK	  =	g_pSprManager->GetSprite(BUTTON_OK);
	V2_SPRITE* pSprOKDown = g_pSprManager->GetSprite(BUTTON_OK_DOWN);
	V2_SPRITE* pSprOKOver = g_pSprManager->GetSprite(BUTTON_OK_OVER);
	V2_SPRITE* pSprCancel = g_pSprManager->GetSprite(BUTTON_CANCEL);
	V2_SPRITE* pSprCancelDown = g_pSprManager->GetSprite(BUTTON_CANCEL_DOWN);
	V2_SPRITE* pSprCancelOver = g_pSprManager->GetSprite(BUTTON_CANCEL_OVER);

	if(g_pGVLogin->bIsLoginInputMode==0)
	{
		SetMousePointerOnList(x, y, __MOUSE_POINTER_BUTTON__, __MOUSE_POINTER_DEFAULT__);
	}
	else
	{
		SetMousePointerOnAccountInput(x, y, __MOUSE_POINTER_BUTTON__, __MOUSE_POINTER_DEFAULT__);
	}

	if( pSprOKDown->PtInSpr(x, y) || pSprOK->PtInSpr(x, y) || pSprOKOver->PtInSpr(x, y) )
	{
		pSprOKDown->ShowSprite(FALSE);
		pSprOKOver->ShowSprite(TRUE);

		OnSprButtonProcessLogin(BUTTON_OK);		
	}

	if(pSprCancelDown->PtInSpr(x, y) || pSprCancel->PtInSpr(x, y) || pSprCancelOver->PtInSpr(x, y) )
	{
		pSprCancelDown->ShowSprite(FALSE);
		pSprCancelOver->ShowSprite(TRUE);
		
		OnSprButtonProcessLogin(BUTTON_CANCEL);

		SPR(SPR_LOGIN_EXIT1)->ShowSprite(TRUE);		
		g_bsprExitChk = TRUE;
	}
}

void OnRButtonDownLogin(WPARAM wParam, LPARAM lParam)
{	
}

void OnRButtonUpLogin(WPARAM wParam, LPARAM lParam)
{	
}

void OnMouseMoveLogin(WPARAM wParam, LPARAM lParam)
{
	// ������ �׽�Ʈ
//	CGuildWarFinalSettingWnd::GetInstance()->CheckInterface();
	// ������ �׽�Ʈ
	CGameDefaultWnd*	pGameDefaultwnd = CGameDefaultWnd::GetInstance();
	CBlockWnd*			pBlockWnd		= CBlockWnd::GetInstance();

	if(pBlockWnd->GetActive()==TRUE)
	{
		pBlockWnd->SetUp(FALSE);		
		pBlockWnd->CheckInterface();			
		return;
	}
	
	if(pGameDefaultwnd->GetActive()==TRUE)
	{
		pGameDefaultwnd->SetUp(FALSE);
		
		if(pGameDefaultwnd->CheckInterface()==1)
			LoginInterface(TRUE);
		
		return;
	}
	

	WORD x = LOWORD( lParam );
	WORD y = HIWORD( lParam );

	V2_SPRITE* pSprOK			= g_pSprManager->GetSprite(BUTTON_OK);
	V2_SPRITE* pSprOKOver		= g_pSprManager->GetSprite(BUTTON_OK_OVER);
	V2_SPRITE* pSprCancel		= g_pSprManager->GetSprite(BUTTON_CANCEL);
	V2_SPRITE* pSprCancelOver	= g_pSprManager->GetSprite(BUTTON_CANCEL_OVER);		
	
	if(IsExitButton(x, y))
	{
		if(g_bsprExitChk==TRUE)
		{
			SPR(SPR_LOGIN_EXIT1)->ShowSprite(FALSE);
			SPR(SPR_LOGIN_EXIT2)->ShowSprite(TRUE);
			SPR(SPR_LOGIN_EXIT3)->ShowSprite(FALSE);
			g_bSprChk = TRUE;
		}		
	}
	else
	{
		if(g_bSprChk==TRUE && g_bsprExitChk==TRUE) 
		{
			SPR(SPR_LOGIN_EXIT1)->ShowSprite(TRUE);
			SPR(SPR_LOGIN_EXIT2)->ShowSprite(FALSE);
			SPR(SPR_LOGIN_EXIT3)->ShowSprite(FALSE);
			g_bSprChk = FALSE;
		}		
	}

	if(g_Mouse.bLDown)
	{
		if(g_pGVLogin->bIsLoginInputMode==0)
		{
			BOOL bChk = FALSE;

			for(DWORD i = 0; i < g_pGVLogin->GetTotalSetNumber(); i++)
			{
				bChk = g_pInputManager->PtInRect(INPUT_BUFFER_LOGIN_SERVER_LIST+i, x, y);
				if (bChk)	break;
			}

			// modified by deepdark.
			if(g_bSprChk || bChk)
				SetMousePointerType(__MOUSE_POINTER_BUTTON__);
			else
				SetMousePointerType(__MOUSE_POINTER_DEFAULT__);
		}
		else
		{			
			SetMousePointerOnAccountInput(x, y, __MOUSE_POINTER_BUTTONCLICK__, __MOUSE_POINTER_DEFAULT__);
		}

		if(!g_pGVLogin->bIsLoginInputMode)
			return;

		if(!pSprOKOver->PtInSpr(x, y))
		{
			V2_SPRITE* pSprOKDown = g_pSprManager->GetSprite(BUTTON_OK_DOWN);
			pSprOK->ShowSprite(TRUE);
			pSprOKOver->ShowSprite(FALSE);
			pSprOKDown->ShowSprite(FALSE);
		}

		if(!pSprCancelOver->PtInSpr(x, y))
		{
			V2_SPRITE* pSprCancelDown = g_pSprManager->GetSprite(BUTTON_CANCEL_DOWN);
			pSprCancel->ShowSprite(TRUE);
			pSprCancelOver->ShowSprite(FALSE);
			pSprCancelDown->ShowSprite(FALSE);			
		}
	}
	else
	{
		if(g_pGVLogin->bIsLoginInputMode==0)
		{
			SetMousePointerOnList(x, y, __MOUSE_POINTER_BUTTON__, __MOUSE_POINTER_DEFAULT__);
		}
		else
		{
			SetMousePointerOnAccountInput(x, y, __MOUSE_POINTER_BUTTON__, __MOUSE_POINTER_DEFAULT__);
		}
		
		//OK��ư���� Mouse Over ������ 
		if( pSprOK->PtInSpr(x, y) )
		{			
			pSprOK->ShowSprite(FALSE);
			pSprOKOver->ShowSprite(TRUE);
			return;
		}
		else if( !pSprOKOver->PtInSpr(x, y) && pSprOKOver->bRenderFlag )
		{
			pSprOK->ShowSprite(TRUE);
			pSprOKOver->ShowSprite(FALSE);			
		}
		
		//Cancel��ư���� Mouse Over ������ 
		if( pSprCancel->PtInSpr(x, y) )
		{			
			pSprCancel->ShowSprite(FALSE);
			pSprCancelOver->ShowSprite(TRUE);
			return;
		}
		else if( !pSprCancelOver->PtInSpr(x, y) && pSprCancelOver->bRenderFlag )
		{
			pSprCancel->ShowSprite(TRUE);
			pSprCancelOver->ShowSprite(FALSE);			
		}

		for(DWORD i=0; i<g_pGVLogin->GetTotalSetNumber(); i++)
		{
			if(g_pInputManager->PtInRect(INPUT_BUFFER_LOGIN_SERVER_LIST+i, x, y))
			{
				g_pGVLogin->SetCurSet(i);
				return;
			}

			if(i == g_pGVLogin->GetTotalSetNumber()-1)
				g_pGVLogin->SetCurSet(0xffffffff);
		}	
	}		
}

void OnMouseWheelLogin(WPARAM wParam, LPARAM lParam)
{
}

void OnCharLogin(BYTE bCh)
{
	if(!g_pGVLogin->bIsLoginInputMode)	return;

	CGameDefaultWnd* pGameDefaultWnd = CGameDefaultWnd::GetInstance();

	if(pGameDefaultWnd->GetActive()==TRUE)
	{
		if(bCh==13)
		{
			pGameDefaultWnd->SetActive(FALSE);
			LoginInterface(TRUE);
		}
		return;
	}		

	switch(bCh)
	{
	case VK_ESCAPE: // added by deepdark!
	case VK_SPACE:	return;	
	case 8:	//BackSpace
		{
			g_pInputManager->BackSpaceEvent();
			return;
		}
		break;
	case 9:	//Tab
		{
//lb_Switch:			

			g_pInputManager->GetInputBuffer(g_pInputManager->GetCurFocusInputID());
			
			g_pInputManager->SetFocusInput(g_pInputManager->GetCurFocusInputID()+1);
			
			if(g_pInputManager->GetCurFocusInputID() > INPUT_BUFFER_LOGIN_PASSWORD)
			{
				g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);	//�ٽ� ��Ŀ���� ID ĭ���� 
			}
			else
			{
				ChangeLanguage(FALSE);
			}
			return;

		}
		break;
	case 13:	//Enter
		{
			LoginProcess();		//LC-01	
			return;
		}
		break;
	}
	// ����� �α��� �Է�â�� ID, PASSWORD �̴�. 
	// �߰��� ���� �������� �߰��ȴٸ� ������ �κ��� �ִ�.
	// comment by seven
	if (g_pInputManager->GetCurFocusInputID() == 1) 
	{
		// ID ��Ŀ���̴�.
		if (g_pInputManager->GetInputBufferLength(g_pInputManager->GetCurFocusInputID()) >= MAX_ID_REAL_LENGTH )
			return;
	}
	else	
	{
		// PASSWORD ��Ŀ���̴�. 
		if (g_pInputManager->GetInputBufferLength(g_pInputManager->GetCurFocusInputID()) >= MAX_PASSWORD_REAL_LENGTH )
			return;
	}
	g_pInputManager->InsertCharacter(g_pInputManager->GetCurFocusInputID(), (char*)&bCh, 1);
}

void OnTimerEventLogin(DWORD dwTimerIndex)
{
	switch(dwTimerIndex)
	{
	case 1:
		{
			g_pInputManager->BlinkInputCaret();
		}
		break;
	}
}

void LoginInterface(BOOL bActive)
{
	SPR(BUTTON_OK_OVER)->ShowSprite(FALSE);
	SPR(BUTTON_OK_DOWN)->ShowSprite(FALSE);
	SPR(BUTTON_CANCEL_OVER)->ShowSprite(FALSE);
	SPR(BUTTON_CANCEL_DOWN)->ShowSprite(FALSE);	
	SPR(BUTTON_OK)->ShowSprite(bActive);
	SPR(BUTTON_CANCEL)->ShowSprite(bActive);	
}

/*{
	SPR(SPR_LOGIN_LOGIN_WINDOW)->ShowSprite(bActive);
}*/