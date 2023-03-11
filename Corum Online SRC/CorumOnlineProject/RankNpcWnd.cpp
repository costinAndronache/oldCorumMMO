//======================================================//
// Code by Jang.							2004.3.10	//
//======================================================//
#include "RankNpcWnd.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "InitGame.h"
#include "DSMiniMap.h"
#include "RankWnd.h"
#include "CodeFun.h"

CRankNpcWnd* CRankNpcWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CRankNpcWnd::CRankNpcWnd()
{ 
	m_byLine	= 0;
	m_bBtn		= FALSE;
}

CRankNpcWnd::~CRankNpcWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CRankNpcWnd::Init()
{	
/*
	InsertData(SPR_OBJ_RANKNPC_WND1, SPR_INTERFACE_DF_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_RANKNPC_WND2, SPR_INTERFACE_DF_WND2, 0, 32, 1.0f, 13.0f, 0);
	InsertData(SPR_OBJ_RANKNPC_WND3, SPR_INTERFACE_DF_WND3, 0, 240, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_RANKNPC_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_RANKNPC_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);	
	InsertData(SPR_OBJ_RANKNPC_RANK1, SPR_INTERFACE_RANKNPC_RANK1, 176, 230, 1.0f, 1.0f, 2);	
	InsertData(SPR_OBJ_RANKNPC_RANK2, SPR_INTERFACE_RANKNPC_RANK2, 176, 230, 1.0f, 1.0f, 2);	
	InsertData(SPR_OBJ_RANKNPC_RANK3, SPR_INTERFACE_RANKNPC_RANK3, 176, 230, 1.0f, 1.0f, 2);	

	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(176, 230, 240, 246, 4, CHECK_BTN);
*/
	m_bInit	= TRUE;

	return TRUE;
}

void CRankNpcWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CRankNpcWnd::SetOrder()
{
}

void CRankNpcWnd::SetText()
{
	char szName[0xff] = {0, };
	char szText[4][0xff];

	BYTE byIndex = 0;

	for(int i = 0; i < 4; i++)
		memset(szText[i], 0, sizeof(szText[i]));
	
	if(CMiniMapWnd::GetInstance()->m_wMapId==1)
	{
		lstrcpy(szName, g_Message[ETC_MESSAGE931].szMessage); // "�ѽ� ����ũ"

		if(m_dwGuildId[0]!=0)
		{
			int nRan = rand()%3;

			switch(nRan)
			{
			case 0:
				wsprintf(szText[0], g_Message[ETC_MESSAGE932].szMessage, szName); // "%s : ���� \"Ż��\" �� ������ �ʴ±�."
				wsprintf(szText[1], g_Message[ETC_MESSAGE933].szMessage, m_szGuildName[0]); // "�� �������� ���� %s ��尡 �� �ϴ븦 ��� �ִ� �Ҹ��� ���� �����ϴܸ���."
				wsprintf(szText[2], g_Message[ETC_MESSAGE934].szMessage, m_szGuildName[0]); // "�Ͽ��� ������~�� �ϴ뿡�� %s ��带 �𸣸� ��ø����. �ڳ� Ȥ ��ø �ƴѰ�? ������"
				byIndex = 3;
				break;
			case 1:
				wsprintf(szText[0], g_Message[ETC_MESSAGE935].szMessage, szName, m_szGuildName[0]); // "%s : ���� �� ��ó�� ���͵��� �� ������ �ʾ�.. Ȥ�� �� %s ��� ������ �ƴұ�?"
				lstrcpy(szText[1], g_Message[ETC_MESSAGE936].szMessage); // "ū ���͵��� ����ִ� ������ ���� ���� ���� ������."
				wsprintf(szText[2], g_Message[ETC_MESSAGE937].szMessage, m_szGuildName[0]); // "�ڳ� Ȥ %s ����� ������� ������ ���ٰ� �����ְ�."
				lstrcpy(szText[3], g_Message[ETC_MESSAGE938].szMessage); // "�� ���� �ٺ��� ���̾�. �̰��� �� ���� ���ŵ�."
				byIndex = 4;
				break;
			case 2:
				wsprintf(szText[0], g_Message[ETC_MESSAGE939].szMessage, szName, m_szGuildName[0]); // "%s : �ֵ��� �׷��µ�.. %s ��尡.. ���� �ټ� �ֵ����� �峲�̶��."
				lstrcpy(szText[1], g_Message[ETC_MESSAGE940].szMessage); // "���� �Ӹ��� ���� ��.. �ƴ�.. �׷��ϱ�.. �ڲ� ��õ���� �����±�."
				lstrcpy(szText[2], g_Message[ETC_MESSAGE941].szMessage); // "��.. �Ѹ���� �������� �Ի��� ����� �츮 �����鵵 ���� �� ���� �Կ� �ø� ������ ������ ����� �Ҹ���.."
				lstrcpy(szText[3], g_Message[ETC_MESSAGE942].szMessage); // "���� ���� ���� ���� �ߴ���..?"
				byIndex = 4;
				break;
			}
		}		
		else
		{
			wsprintf(szText[0], g_Message[ETC_MESSAGE1066].szMessage, szName); // "%s : �̷� ������ ������ ���� ������ ���� �ɱ�? ������ �̾��� ������"
			lstrcpy(szText[1], g_Message[ETC_MESSAGE1067].szMessage); // "������ ��Ű�� �͸����ε� ������.. ��..."
			byIndex = 2;
		}		
	}	
	else if(CMiniMapWnd::GetInstance()->m_wMapId==3)
	{
		lstrcpy(szName, g_Message[ETC_MESSAGE943].szMessage); // "�ξ� ����ũ"

		if(m_dwGuildId[0]!=0)
		{
			int nRan = rand()%3;

			switch(nRan)
			{
			case 0:
				wsprintf(szText[0], g_Message[ETC_MESSAGE944].szMessage, szName, m_szGuildName[0]); // "%s : ���󿡼� �ֽ��ϰ� �ִٴ� �ҹ��� �ִ���.. �� ���̳ı�? %s ��� ���ϼ�."
				lstrcpy(szText[1], g_Message[ETC_MESSAGE945].szMessage); // "�ʹ� ������ �����ϱ� ��� �ϴ���.. �Ϲ��ε�ν�� ���͵� ����ְ� ġ�������� ������ ������,"
				lstrcpy(szText[2], g_Message[ETC_MESSAGE946].szMessage); // "������ ���忡���� ���� ���Ű� ��. ��.. �����ϴ� ���� �ƴϰھ�?"
				byIndex = 3;
				break;
			case 1:
				wsprintf(szText[0], g_Message[ETC_MESSAGE947].szMessage, szName, m_szGuildName[0]); // "%s : %s ����� �������� ���͸��� �˰ɼ�. �Ƹ޸�ŸƮ�� ž �ȿ� ������ ��¼�� ����̾�~"
				wsprintf(szText[1], g_Message[ETC_MESSAGE948].szMessage, m_szGuildName[0]); // "%s ����� �������� ������ ���� �͸��ص� �� �巡���� õ������. �ٵ� �Ƹ޸�ŸƮ�� ���İ�?"
				lstrcpy(szText[2], g_Message[ETC_MESSAGE949].szMessage); // "�ƴ� �� ȯ���� �巡���� �𸥴� ���ΰ�~ �ڳ� ���ʺ��α�~!!"
				byIndex = 3;
				break;
			case 2:
				wsprintf(szText[0], g_Message[ETC_MESSAGE950].szMessage, szName); // "%s : ���� �׷��µ�.. �츮 ���� �ѽ��̶�� ���ν��� �־�."
				wsprintf(szText[1], g_Message[ETC_MESSAGE951].szMessage, m_szGuildName[0]); // "�ٵ� �� ���� ������ ������ �����Ծ�. %s ����� ������� �� �� ������ �鸦�ŷ�~"
				lstrcpy(szText[2], g_Message[ETC_MESSAGE952].szMessage); // "���� ���� �α� �Ÿ��� ���̾�. ����. �� �ٸ� ����鿡�Ե� �˷������."
				byIndex = 3;
				break;
			}
		}
		else
		{
			wsprintf(szText[0], g_Message[ETC_MESSAGE1068].szMessage, szName); // "%s : ������� �Ѽ� �Ҹ��� �ϴÿ� ���� �ʴ� �ɱ�? �ŵ� �� �����Ͻ���."
			lstrcpy(szText[1], g_Message[ETC_MESSAGE1069].szMessage); // "��.. ���� ���ͼ� �ϴ� �Ҹ���, �̷� �ñ⿡ ���� ���� ������ �����ϴ� �� ����."
			byIndex = 2;
		}		
	}
	else if(CMiniMapWnd::GetInstance()->m_wMapId==4)
	{
		lstrcpy(szName, g_Message[ETC_MESSAGE953].szMessage); // "�׽��� ����ũ"

		if(m_dwGuildId[0]!=0)
		{
			int nRan = rand()%3;

			switch(nRan)
			{
			case 0:
				wsprintf(szText[0], g_Message[ETC_MESSAGE954].szMessage, szName); // "%s : ��.. ���� �츮 ��° �� ������ ����ھ�~"
				wsprintf(szText[1], g_Message[ETC_MESSAGE955].szMessage, m_szGuildName[0]); // "�۽� ���Ϸ� �� �� ������ ���� ã�ƿԳ� �;����� �� �濡 �ִ� %s ��� ��θ��̵带 �� �������ݾ�~!!!"
				lstrcpy(szText[2], g_Message[ETC_MESSAGE956].szMessage); // "���� �޾Ƴ��� �ͱ���.. ������."
				wsprintf(szText[3], g_Message[ETC_MESSAGE957].szMessage, m_szGuildName[0]); // "���� ���� �츮 �� ���� �����~!!! %s ��尡 ���� �̰��� �ٽ� �ô���.."
				byIndex = 4;
				break;
			case 1:
				wsprintf(szText[0], g_Message[ETC_MESSAGE958].szMessage, szName, m_szGuildName[0]); // "%s : �츮 ��° ���� ���� �Ⱦ�. ���� ���� %s ��� �渶�� �����ļ� �Ǽ��� �޾Ҵµ�,"
				lstrcpy(szText[1], g_Message[ETC_MESSAGE959].szMessage); // "�ʹ� ���ݽ������� �� ���� ���� �Ⱦľ��ŵ�. �ٵ� ���� �����ٰ� �� �ڴ� ���̿� ���� �İ� ���� �ž�."
				lstrcpy(szText[2], g_Message[ETC_MESSAGE960].szMessage); // "�� �� ���������� ���� �ڶ��̾��ٰ�~!! ����..�츮 ������ ���ڴ�ξ�~!!!"
				byIndex = 3;
				break;
			case 2:
				wsprintf(szText[0], g_Message[ETC_MESSAGE961].szMessage, szName, m_szGuildName[0]); // "%s : �츮 ������ �׷��� ���� ������. %s ��尡 �츮 ���� �� �� ������ �����༭ ������ �� �־���."
				lstrcpy(szText[1], g_Message[ETC_MESSAGE962].szMessage); // "�� ������ �ٴٶ�� �ϴµ� ���� �������� ���ϰ� �ְŵ�? �� �Ȱ��� ���� �� ���Ƶ� �ڼ��� ���� ���� ���� �Ϳ���."
				wsprintf(szText[2], g_Message[ETC_MESSAGE963].szMessage, m_szGuildName[0]); // "%s ����� ����� ���׾�? ��? �ǳ� �� ����ٰ�~? �׷��� �����~"
				byIndex = 3;
				break;
			}			
		}		
		else
		{
			wsprintf(szText[0], g_Message[ETC_MESSAGE1070].szMessage, szName); // "%s : ����� �� ������ �� ���� �ִٸ� �� ��� ������ �ٵ� ����."
			lstrcpy(szText[1], g_Message[ETC_MESSAGE1071].szMessage); // "���, �뺴�� �� ������ �ѹ� �����غ�."
			byIndex = 2;
		}
	}
	else if(CMiniMapWnd::GetInstance()->m_wMapId==5)
	{
		lstrcpy(szName, g_Message[ETC_MESSAGE964].szMessage); // "���� ����ũ"

		if(m_dwGuildId[0]!=0)
		{
			int nRan = rand()%3;

			switch(nRan)
			{
			case 0:
				wsprintf(szText[0], g_Message[ETC_MESSAGE965].szMessage, szName, m_szGuildName[0]); // "%s : ���̾� �ٸ� ������. %s ��尡 �ƴϸ� ���͵��� ���� �� ����ڴ°�?"
				lstrcpy(szText[1], g_Message[ETC_MESSAGE966].szMessage); // "�� ���� ������ �� ���� ���͵��� ���� ���� ����������?"
				wsprintf(szText[2], g_Message[ETC_MESSAGE967].szMessage, m_szGuildName[0]); // "%s ���� �پ ������� �����ϱ� ������ ���Ͱ� �Ҹ�� ���� ���� ����������.."
				lstrcpy(szText[3], g_Message[ETC_MESSAGE968].szMessage); // "�׷��� �������� �ʾ�?"
				byIndex = 4;
				break;
			case 1:
				wsprintf(szText[0], g_Message[ETC_MESSAGE969].szMessage, szName, m_szGuildName[0]); // "%s : �츮 ��° ���� �׷��µ� %s ����� �������� ���󿡼� ����ϰ� �ִٴ���."
				lstrcpy(szText[1], g_Message[ETC_MESSAGE970].szMessage); // "�ٵ� ���� �𸣴°� �־�~"
				lstrcpy(szText[2], g_Message[ETC_MESSAGE971].szMessage); // "�̰� �������� ���󿡼� ��ī��Ʈ �Ϸ��� ��и��� ���Ǹ� ���� �ִٴ� �ҹ��� ���ٰ�.."
				wsprintf(szText[3], g_Message[ETC_MESSAGE972].szMessage, m_szGuildName[0]); // "�� � �� ����̵� %s ����� �ְ��� �ö󰡰� �ִ� �� ����̾�."
				byIndex = 4;
				break;
			case 2:
				wsprintf(szText[0], g_Message[ETC_MESSAGE973].szMessage, szName, m_szGuildName[0]); // "%s : ���� �ִٰ� �� �ְ�� �ƴ����� ����. %s ����� �����°� �Ŀ��� Ž�� �� �ϰڴ���."
				wsprintf(szText[1], g_Message[ETC_MESSAGE974].szMessage, m_szGuildName[0]); // "������ �氡�� %s ����� ����� �����µ� ����. �ٸ� ����� ���� ���� �Ծ ��½ �ϴ°ž�."
				lstrcpy(szText[2], g_Message[ETC_MESSAGE975].szMessage); // "�׷��� �����̶� �ϴ� �Ű���?"
				byIndex = 3;
				break;
			}
		}
		else
		{
			wsprintf(szText[0], g_Message[ETC_MESSAGE1072].szMessage, szName); // "%s : ���� ���� ��ŭ ������ ������ ���ٴ� �� ���� ���̾�."
			wsprintf(szText[1], g_Message[ETC_MESSAGE1073].szMessage); // "���赵 ���� ȥŹ������, ���� ������� ���� ������ ����?"
			byIndex = 2;
		}
	}
	else if(CMiniMapWnd::GetInstance()->m_wMapId==6)
	{
		lstrcpy(szName, g_Message[ETC_MESSAGE976].szMessage); // "�ٴ� ����ũ"

		if(m_dwGuildId[0]!=0)
		{
			int nRan = rand()%3;

			switch(nRan)
			{
			case 0:
				wsprintf(szText[0], g_Message[ETC_MESSAGE977].szMessage, szName, m_szGuildName[0]); // "%s : ��.. %s ����� ���� ���� �̰��� �ٳబ���."
				lstrcpy(szText[1], g_Message[ETC_MESSAGE978].szMessage); // "�׵��� ������� �����⵵ ������ ���� ������ ������ ���� �ִٴϱ��."
				lstrcpy(szText[2], g_Message[ETC_MESSAGE979].szMessage); // "�ϱ� �׷��� ������� ������ �Ű���. ī.��.��.�� ����?"
				byIndex = 3;
				break;
			case 1:
				wsprintf(szText[0], g_Message[ETC_MESSAGE980].szMessage, szName); // "%s : ���� �� �� ��ġ�� �˱� �ӿ��� �� ����� �þ��."
				wsprintf(szText[1], g_Message[ETC_MESSAGE981].szMessage, m_szGuildName[0]); // "%s ����� ���� �̶����. ���� �Ӹ��� �λ����� �˰��̾���."
				lstrcpy(szText[2], g_Message[ETC_MESSAGE982].szMessage); // "�ٽ� ���� �Ǹ� �� ���� �޾Ƽ� ��° ������ ���� �ֱ�� ����߾��."
				byIndex = 3;
				break;
			case 2:
				wsprintf(szText[0], g_Message[ETC_MESSAGE983].szMessage, szName, m_szGuildName[0]); // "%s : ������ ������ ������ �е��� ���� ������ �Ǵµ�, %s ��� �е��� ���ƿ�."
				lstrcpy(szText[1], g_Message[ETC_MESSAGE984].szMessage); // "ģ������ ������ ���� �λ�� �ϴ� ��������."
				lstrcpy(szText[2], g_Message[ETC_MESSAGE985].szMessage); // "�׷��� ���� ������ ���� �η�������."
				byIndex = 3;
				break;
			}
		}		
		else
		{
			wsprintf(szText[0], g_Message[ETC_MESSAGE1074].szMessage, szName); // "%s : ���� ���� �������� �����ϴ� �ñ⿣ �Ҹ��� �������� �ֺ� ���µ鰣��"
			wsprintf(szText[1], g_Message[ETC_MESSAGE1075].szMessage); // "�����̳� ���Ϳ� ���� ���а��� ���� �ʿ䰡 ������."
			byIndex = 2;
		}
	}

	m_byLine = 0;
	
	for(i = 0; i < 20; i++)
		memset(m_szRenderText[i], 0, sizeof(m_szRenderText[i]));

	for(i = 0; i < byIndex; i++)
	{
		char* szTemp = NULL;

		int nSize = lstrlen(szText[i]);

		szTemp = szText[i];

		while(nSize>0)
		{
			if(nSize>36)
			{
				for(;;)
				{
					if(nSize > 40)
					{
						char* szPrev = CharPrevExA(0, szTemp, szTemp+40, 0);
						nSize = lstrlen(szTemp) - lstrlen(szPrev); 

						__lstrcpyn(m_szRenderText[m_byLine], szTemp, nSize);
						
						szTemp = szPrev; 
						m_byLine++;
						nSize = lstrlen(szTemp);
					}
					else
					{
						// ���� ���ڿ��� ä�� ���ο� ����, ���̸� ����Ѵ�.
						lstrcpy(m_szRenderText[m_byLine], szTemp);
						m_byLine++;								
						nSize = 0;
						break;
					}
				}
			}
			else
			{
				lstrcpy(m_szRenderText[m_byLine], szTemp);				
				m_byLine++;
				nSize = 0;							
			}		
		}
	}
}

void CRankNpcWnd::RenderText()
{	
	char szInfo[0xff] = {0, };

	lstrcpy(szInfo, g_Message[ETC_MESSAGE916].szMessage); // "��ŷ �Խ���"
	
	int nSize = lstrlen(szInfo);
	RenderFont(szInfo, m_fPosX+22, m_fPosX+22+nSize*7, m_fPosZ+5, m_fPosZ+19, GetStartOrder()+1);

	for(int i = 0; i < m_byLine; i++)
	{
		nSize = lstrlen(m_szRenderText[i]);
		RenderFont(m_szRenderText[i], m_fPosX+10, m_fPosX+10+nSize*7, m_fPosZ+35+i*15, m_fPosZ+49+i*15, GetStartOrder()+1);
	}		
}

void CRankNpcWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		ShowSpriteAll();

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_RANKNPC_WND1, TRUE);
		SetRender(SPR_OBJ_RANKNPC_WND2, TRUE);
		SetRender(SPR_OBJ_RANKNPC_WND3, TRUE);
		SetRender(SPR_OBJ_RANKNPC_CLOSE1, TRUE);
		SetRender(SPR_OBJ_RANKNPC_RANK1, TRUE);			

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);		
	}
}

int CRankNpcWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();
		
	if(nRt!=4 && m_bBtn)
	{
		m_bBtn = TRUE;
		SetRender(SPR_OBJ_RANKNPC_RANK1, TRUE);
		SetRender(SPR_OBJ_RANKNPC_RANK2, FALSE);
		SetRender(SPR_OBJ_RANKNPC_RANK3, FALSE);
	}

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
				SetActive(FALSE);	
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_RANKNPC_CLOSE1, FALSE);
				SetRender(SPR_OBJ_RANKNPC_CLOSE2, TRUE);
			}						
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 3:			
		{
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 4:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				CRankWnd* pRankWnd = CRankWnd::GetInstance();

				SetRender(SPR_OBJ_RANKNPC_RANK1, FALSE);
				SetRender(SPR_OBJ_RANKNPC_RANK2, TRUE);
				SetRender(SPR_OBJ_RANKNPC_RANK3, FALSE);
				m_bBtn = TRUE;

				for(int i = 0; i < 4; i++)
				{
					pRankWnd->m_dwGuildId[i]	= m_dwGuildId[i];
					pRankWnd->m_dwPoint[i]	= m_dwPoint[i];
					memset(pRankWnd->m_szGuildName[i], 0, sizeof(pRankWnd->m_szGuildName[i]));
					__lstrcpyn(pRankWnd->m_szGuildName[i], m_szGuildName[i], MAX_GUILD_NAME_REAL_LENGTH);
				}
				pRankWnd->m_dwRank	= m_dwRank;
				pRankWnd->SetActive();
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{							
				m_bBtn = TRUE;
				SetRender(SPR_OBJ_RANKNPC_RANK1, FALSE);
				SetRender(SPR_OBJ_RANKNPC_RANK2, FALSE);
				SetRender(SPR_OBJ_RANKNPC_RANK3, TRUE);				
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				m_bBtn = TRUE;
				SetRender(SPR_OBJ_RANKNPC_RANK1, FALSE);
				SetRender(SPR_OBJ_RANKNPC_RANK2, TRUE);
				SetRender(SPR_OBJ_RANKNPC_RANK3, FALSE);				
			}
			SetMouseCheck(nRt, eMouseCheck);
		}		
		break;
	}		
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_RANKNPC_CLOSE1, TRUE);
		SetRender(SPR_OBJ_RANKNPC_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
		return 1;
	
	if(IsReturn(nRt, eMouseCheck))
		return 1;		

	return 0;
}
//======================================================//
// End.													//
//======================================================//
