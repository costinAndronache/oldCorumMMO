//
//	Game Notice message display window.
//
//				2003/11/13
//				Trust Pak


#include "Menu.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "Message.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "GameNoticeWnd.h"
#include "EventNPCSubWnd.h"
#include "WorldProcess.h"
#include "DungeonTable.h"
#include "CodeFun.h"
#include "Interface.h"


CEventNPCSubWnd* CEventNPCSubWnd::c_pThis = NULL;

//////////////////////////////////////////////////////////////////////////////
// Class Name : CEventNPCSubWnd
// Description :
//
//----------------------------------------------------------------------------
// Name : CEventNPCSubWnd::CEventNPCSubWnd
// Description : Constructor.
//----------------------------------------------------------------------------
CEventNPCSubWnd::CEventNPCSubWnd()
{
}

//----------------------------------------------------------------------------
// Name : CEventNPCSubWnd::~CEventNPCSubWnd
// Description : Destructor.
//----------------------------------------------------------------------------
CEventNPCSubWnd::~CEventNPCSubWnd()
{
}


//----------------------------------------------------------------------------
// Name : CEventNPCSubWnd::Init
// Description : 
//----------------------------------------------------------------------------
BOOL CEventNPCSubWnd::Init()
{		
/*
	// defualt window sprites.	
	InsertData(SPR_EVENT_NPC_SUB_WND1, SPR_INTERFACE_DF_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_EVENT_NPC_SUB_WND2, SPR_INTERFACE_DF_WND2, 0, 32, 1.0f, 10.0f, 0);
	InsertData(SPR_EVENT_NPC_SUB_WND3, SPR_INTERFACE_DF_WND3, 0, 160 + 32, 1.0f, 1.0f, 0);
	InsertData(SPR_EVENT_NPC_SUB_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_EVENT_NPC_SUB_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);

	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	*/
	// Initialize member Variables.

	m_bActive		= FALSE;
	m_bInit			= TRUE;		
	m_fWndWidth		= 0.0f;
	m_fWndHeight	= 0.0f;

	m_iTextLineNumber	= 0;
	m_iMessageType		= EVENTNPC_SUB_TYPE_CANCEL;
	m_iMoveDungeonInfoNum = 0;
	m_iSelectedIndex	= -1;

	m_bFirst		= FALSE;

	memset(m_szRenderText, 0, 16 * 256);	// m_szRenderText[16][256];		
	memset(m_aMoveDungeonInfo, 0, sizeof(m_aMoveDungeonInfo));	
	
	
	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CEventNPCSubWnd::Render
// Description : 
//----------------------------------------------------------------------------
void CEventNPCSubWnd::RenderText()
{	
	// SetText�� �ݵ�� �̸� ȣ���� �Ǿ��־�� ��.

	int		nSize=0, i = 0;			

	//"��"
	nSize = lstrlen(g_Message[ETC_MESSAGE1062].szMessage);	
	RenderFont(g_Message[ETC_MESSAGE1062].szMessage, m_fPosX+22, m_fPosX+22+nSize*6, m_fPosZ+5, m_fPosZ+19, GetStartOrder()+1);	

	// ��� ���.	
	for(i = 0; i < m_iTextLineNumber; i++)
	{
		nSize = lstrlen(m_szRenderText[i]);		
		RenderFont(m_szRenderText[i], m_fPosX+10, m_fPosX+10+(nSize*7), m_fPosZ+35+(i*15), m_fPosZ+49+(i*15), GetStartOrder()+1);
	}	

	// �̵��� �� �ִ� �������� ����Ʈ�� �ѷ���.
	DWORD	dwTextColour = 0x00000000;
	char	szOutputText[0xff]={0,};

	if (EVENTNPC_SUB_TYPE_OK_SELECT == m_iMessageType)
	{
		for (i = 0; i < m_iMoveDungeonInfoNum; i++)
		{
			dwTextColour = 0xff7f7fff;

			memset(szOutputText, 0, 0xff);

			wsprintf(szOutputText, "%s (Lv.%d ~ Lv.%d)", m_aMoveDungeonInfo[i].m_szDungeonName,
													    m_aMoveDungeonInfo[i].iLevelMin,
													    m_aMoveDungeonInfo[i].iLevelMax);
						
			if(i == m_iSelectedIndex) // ���� ���콺�� ��ġ�� �ε������ ������ �޸��Ͽ� ���.
				dwTextColour = 0xffffffff;

			nSize = lstrlen(szOutputText);
			RenderFont(szOutputText, m_fPosX+10, m_fPosX+10+(nSize*7), m_fPosZ+40+(m_iTextLineNumber*15)+(i*15), m_fPosZ+55+(m_iTextLineNumber*15)+(i*15), GetStartOrder()+1, dwTextColour);			
		}		
	}
}

//----------------------------------------------------------------------------
// Name : CEventNPCSubWnd::Remove
// Description : 
//----------------------------------------------------------------------------
void CEventNPCSubWnd::Remove()
{		
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

//----------------------------------------------------------------------------
// Name : CEventNPCSubWnd::SetOrder
// Description : 
//----------------------------------------------------------------------------
void CEventNPCSubWnd::SetOrder()
{	
}

//----------------------------------------------------------------------------
// Name : CEventNPCSubWnd::SetActive
// Description : 
//----------------------------------------------------------------------------
void CEventNPCSubWnd::SetActive(BOOL bActive)
{		
	m_bActive	= bActive;

	if(bActive == FALSE)
	{		
		m_bSoundChk	= FALSE;	

		m_iMoveDungeonInfoNum = 0;
		m_iSelectedIndex = -1;

		memset(m_aMoveDungeonInfo, 0, sizeof(m_aMoveDungeonInfo));

		ShowSpriteAll();		
	}
	else
	{	
		// Game notice message window has default position.				
		SetRender(SPR_EVENT_NPC_SUB_WND1, TRUE);
		SetRender(SPR_EVENT_NPC_SUB_WND2, TRUE);
		SetRender(SPR_EVENT_NPC_SUB_WND3, TRUE);
		SetRender(SPR_EVENT_NPC_SUB_CLOSE1, TRUE);
	}		
}

//----------------------------------------------------------------------------
// Name : CEventNPCSubWnd::CheckInterface
// Description : 
//----------------------------------------------------------------------------
int CEventNPCSubWnd::CheckInterface()
{		
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	InitValue(nRt);

	MOUSECHECK eMouseCheck = GetMouseCheck(nRt);
		
	switch(nRt)
	{		
	case 1:
		{			
			SetMouseCheck(nRt, eMouseCheck);						
		}
		break;
	case 2:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				SetActive(FALSE);				
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_EVENT_NPC_SUB_CLOSE1, FALSE);
				SetRender(SPR_EVENT_NPC_SUB_CLOSE2, TRUE);
			}						
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 3:			
		{
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}	
	
	if(MouseUp())
	{
		SetRender(SPR_EVENT_NPC_SUB_CLOSE1, TRUE);
		SetRender(SPR_EVENT_NPC_SUB_CLOSE2, FALSE);		
	}

	CheckTextSelectedIndex();

	//if (GetClick())
	if (MOUSE_DOWN == GetMouseCheck(1))
	{
		if (-1 != m_iSelectedIndex)
		{
			PortalProcess();			
		}
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
	{
		return CInterface::GetInstance()->GetSetWnd();
	}

	if(IsReturn(nRt, eMouseCheck))
	{
		return CInterface::GetInstance()->GetSetWnd();	
	}
	
	return 0;
}

//----------------------------------------------------------------------------
// Name : CEventNPCSubWnd::SetPosition
// Description :
//----------------------------------------------------------------------------
void CEventNPCSubWnd::SetPosition(float fX, float fZ)
{
	
	m_fPrevPosX = fX;
	m_fPrevPosZ = fZ;
	m_fPosX = fX;
	m_fPosZ = fZ;
	
}
//----------------------------------------------------------------------------
// Name : CEventNPCSubWnd::SetText
// Description :
//----------------------------------------------------------------------------
void CEventNPCSubWnd::SetText(int iType)
{	
	switch (iType)
	{
	case EVENTNPC_SUB_TYPE_OK_SELECT:

		// ��� ����
		// 
		// ������ ã�� ������ �ڳ׿���
		// ���� �ູ�� ���� �ϱ�.
		// ��, ���� ���� ���� �����ϰԳ�.
		//		

		memset(m_szRenderText, 0, sizeof(m_szRenderText));
		__lstrcpyn(m_szRenderText[0], g_Message[ETC_MESSAGE1012].szMessage, lstrlen(g_Message[ETC_MESSAGE1012].szMessage));	//"������ ã�� ������ �ڳ׿���"	
		__lstrcpyn(m_szRenderText[1], g_Message[ETC_MESSAGE1013].szMessage, lstrlen(g_Message[ETC_MESSAGE1013].szMessage));	//"���� �ູ�� ���� �ϱ�."	
		__lstrcpyn(m_szRenderText[2], g_Message[ETC_MESSAGE1014].szMessage, lstrlen(g_Message[ETC_MESSAGE1014].szMessage)); //"��, ���� ���� ���� �����ϰԳ�."
		
		m_iTextLineNumber = 3;	
		m_iMessageType = EVENTNPC_SUB_TYPE_OK_SELECT;

		break;
		
	case EVENTNPC_SUB_TYPE_OK_CANNOT_SELECT:

		// ��� ����
		//
		// ����... �ƽ����� ������ ���� ������ 
		// ��Ÿ���� �ʾҴٳ�. Ư���� ���������� 
		// Ư�� �ð����� ������ �� �� ����. 
		// �� �� �ð��� �Ǹ� �˷� ���״� 
		// �׶��� �Ǹ� �ٽ� �ͺ��Գ�.
		//	

		memset(m_szRenderText, 0, sizeof(m_szRenderText));
		__lstrcpyn(m_szRenderText[0], g_Message[ETC_MESSAGE1015].szMessage, lstrlen(g_Message[ETC_MESSAGE1015].szMessage)); // ����... �ƽ����� ������ ���� ������ 		
		__lstrcpyn(m_szRenderText[1], g_Message[ETC_MESSAGE1016].szMessage, lstrlen(g_Message[ETC_MESSAGE1016].szMessage));	// "��Ÿ���� �ʾҴٳ�. Ư���� ����������"	
		__lstrcpyn(m_szRenderText[2], g_Message[ETC_MESSAGE1017].szMessage, lstrlen(g_Message[ETC_MESSAGE1017].szMessage)); // "Ư�� �ð����� ������ �� �� ����."
		__lstrcpyn(m_szRenderText[3], g_Message[ETC_MESSAGE1018].szMessage, lstrlen(g_Message[ETC_MESSAGE1018].szMessage)); // "�� �� �ð��� �Ǹ� �˷� ���״�"
		__lstrcpyn(m_szRenderText[4], g_Message[ETC_MESSAGE1019].szMessage, lstrlen(g_Message[ETC_MESSAGE1019].szMessage));	// "�׶��� �Ǹ� �ٽ� �ͺ��Գ�."

		m_iTextLineNumber = 5;		
		m_iMessageType = EVENTNPC_SUB_TYPE_OK_CANNOT_SELECT;

		break;

	case EVENTNPC_LEVEL_HIGH:

		// ��系��
		//
		// �װ����� �ۻ縮�� ��ڴٴ� �ǰ�?

		memset(m_szRenderText, 0, sizeof(m_szRenderText));
		__lstrcpyn(m_szRenderText[0], g_Message[ETC_MESSAGE1063].szMessage, lstrlen(g_Message[ETC_MESSAGE1063].szMessage)); // "�װ����� �ۻ縮�� ��ڴٴ� �ǰ�?"
		
		m_iTextLineNumber = 1;
		m_iMessageType = EVENTNPC_LEVEL_HIGH;

		break;


	case EVENTNPC_LEVEL_LOW:

		// ��系��
		//
		// �̺� �ű� ��ſ��� ������.

		memset(m_szRenderText, 0, sizeof(m_szRenderText));
		__lstrcpyn(m_szRenderText[0], g_Message[ETC_MESSAGE1064].szMessage, lstrlen(g_Message[ETC_MESSAGE1064].szMessage)); // "�̺� �ű� ��ſ��� ������."
		
		m_iTextLineNumber = 1;
		m_iMessageType = EVENTNPC_LEVEL_LOW;	

		break;

		
	default:

		// ��� ����
		//
		// �̷�, �����̰� ��µ� ���ϱ�.
		// �׷��� ������� ����� 
		// ��� ������ �ϰڴ°�.. ����..
		// ��, �ȴٴ� ��¿ �� ����.
		//	

		memset(m_szRenderText, 0, sizeof(m_szRenderText));
		__lstrcpyn(m_szRenderText[0], g_Message[ETC_MESSAGE1020].szMessage, lstrlen(g_Message[ETC_MESSAGE1020].szMessage));	//"�̷�, �����̰� ��µ� ���ϱ�."	
		__lstrcpyn(m_szRenderText[1], g_Message[ETC_MESSAGE1021].szMessage, lstrlen(g_Message[ETC_MESSAGE1021].szMessage));	//"�׷��� ������� �����"	
		__lstrcpyn(m_szRenderText[2], g_Message[ETC_MESSAGE1022].szMessage, lstrlen(g_Message[ETC_MESSAGE1022].szMessage)); //"��� ������ �ϰڴ°�.. ����.."
		__lstrcpyn(m_szRenderText[3], g_Message[ETC_MESSAGE1023].szMessage, lstrlen(g_Message[ETC_MESSAGE1023].szMessage)); //"��, �ȴٴ� ��¿ �� ����."
		
		m_iTextLineNumber = 4;
		m_iMessageType = EVENTNPC_SUB_TYPE_CANCEL;

		break;
	}	

}

//----------------------------------------------------------------------------
// Description :
//----------------------------------------------------------------------------
void CEventNPCSubWnd::QueryCanMoveDungeon(void)
{
	if (NULL == g_pDungeonTable)
	{
		SetText(EVENTNPC_SUB_TYPE_OK_CANNOT_SELECT);
		return;
	}

	// ������ �ִ� ���� ������ ������� ���� ������
	// �̺�Ʈ ������ ����Ʈ�� �ѷ��ش�.

	int iMoveDungeonInfo = 0;
	memset(m_aMoveDungeonInfo, 0, sizeof(m_aMoveDungeonInfo));
	m_iMoveDungeonInfoNum = 0;

	DUNGEON_DATA_EX* pDungeon	= NULL;
	POSITION_ pos				= g_pDungeonTable->m_pEventList->GetHeadPosition();	

	while (pos)
	{
		pDungeon = (DUNGEON_DATA_EX*)g_pDungeonTable->m_pEventList->GetNext(pos);

		if (NULL != pDungeon)
		{
			if (EVENT_DUNGEON_OPEN == pDungeon->m_bEventState)
			{
				m_aMoveDungeonInfo[iMoveDungeonInfo].dwID = pDungeon->m_dwID;
				__lstrcpyn(m_aMoveDungeonInfo[iMoveDungeonInfo].m_szDungeonName
					, pDungeon->m_szDungeonName
					, MAX_DUNGEON_NAME_LENGTH);

				m_aMoveDungeonInfo[iMoveDungeonInfo].iLevelMin = pDungeon->m_byEventEnterMinLevel;
				m_aMoveDungeonInfo[iMoveDungeonInfo].iLevelMax = pDungeon->m_byEventEnterMaxLevel;

				++iMoveDungeonInfo;
				m_iMoveDungeonInfoNum = iMoveDungeonInfo;
			}
		}
	}

	// ������ �� �ִ� ������ ������ ���÷��� �� �޽����� �������ش�.

	if (0 == m_iMoveDungeonInfoNum)
	{
		SetText(EVENTNPC_SUB_TYPE_OK_CANNOT_SELECT);
	}
}

//----------------------------------------------------------------------------
// Description : 
//---------------------------------------------------------------------------
void CEventNPCSubWnd::CheckTextSelectedIndex(void)
{
	// ���콺�� ��ǥ�� ������ Ŭ���̾�Ʈ ������ �������� 
	// top�� Menu�� ���� ��ġ �̻�, 15�� �þ ������ index�� 1�� �����Ѵ�.
	// �ִ� bottom���� �Ѿ�� index�� -1�̴�.	

	const int c_iMenuTop = 78;

	int iMaxBottom = c_iMenuTop + (m_iMoveDungeonInfoNum * 15);

	if (g_Mouse.MousePos.y >= m_fPosZ + c_iMenuTop && g_Mouse.MousePos.y <= m_fPosZ + iMaxBottom &&
		g_Mouse.MousePos.x >= m_fPosX && g_Mouse.MousePos.x <= m_fPosX + m_fMaxSizeWidth)
	{
		m_iSelectedIndex = (g_Mouse.MousePos.y - (int)m_fPosZ - c_iMenuTop) / 15;
	}
	else
	{
		// ������ �ƴϸ� ������ index�� -1�̴�.
		m_iSelectedIndex = -1;	
	}
}

//---------------------------------------------------------------------------
// Description : 
//---------------------------------------------------------------------------
void CEventNPCSubWnd::PortalProcess(void)
{
	if (0 > m_iSelectedIndex || MAX_MOVE_DUNGEON_INFO <= m_iSelectedIndex)
	{
		return;
	}

	if ((int)g_pMainPlayer->m_dwLevel > m_aMoveDungeonInfo[m_iSelectedIndex].iLevelMax)
	{
		SetText(EVENTNPC_LEVEL_HIGH);
		m_iSelectedIndex = -1;

		return;
	}		
		
	if ((int)g_pMainPlayer->m_dwLevel < m_aMoveDungeonInfo[m_iSelectedIndex].iLevelMin)
	{
		SetText(EVENTNPC_LEVEL_LOW);
		m_iSelectedIndex = -1;

		return;
	}

	VECTOR3	v3Move = { 2500.0f, 0.0f, 2500.0f };
	
	CTDS_PORTAL_MOVE PortalMovePacket;

	PortalMovePacket.bLayerNo		= 0;
	PortalMovePacket.wToDungeonID	= (WORD)m_aMoveDungeonInfo[m_iSelectedIndex].dwID;
	VECTOR3_TO_VECTOR2(v3Move, PortalMovePacket.v2Pos)
	PortalMovePacket.bPortalType	= 1;		// by NPC.
	PortalMovePacket.wPrevDungeonID	= (WORD)g_pThisDungeon->m_dwID;

	if(g_pThisDungeon)
	{
		g_pMainPlayer->wPrevDungeonID = (WORD)g_pThisDungeon->m_dwID;		
	}
	
	g_pNet->SendMsg((char*)&PortalMovePacket, PortalMovePacket.GetPacketSize(), SERVER_INDEX_ZONE);
	g_pMainPlayer->SetStatus(UNIT_STATUS_PORTAL_MOVING);

	// Prevent twice to attempt to use portal to dungeon.

	SetActive(FALSE);

	m_iSelectedIndex = -1;
}

