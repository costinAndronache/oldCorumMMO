#include "stdafx.h"
#include "ChinaBillingRemainTime.h"
#include "message.h"
#include "InitGame.h"
#include "ErrorWnd.h"
#include "GameDefaultWnd.h"
#include "WorldWnd.h"
#include "LoginProcess.h"
#include "InputManager.h"
#include "Interface.h"

CHINA_REMAINED_TIME g_ChinaBillingRemainedTime;



void SetChinaBillingRemainedTime(DWORD dwType, DWORD dwTime)
{
	g_ChinaBillingRemainedTime.bReceived = TRUE;

	g_ChinaBillingRemainedTime.dwRegisteredTime = timeGetTime();
	g_ChinaBillingRemainedTime.dwTimeType = dwType;
	
	switch(dwType)
	{
	case china_billing_time_type_none		:
		g_ChinaBillingRemainedTime.dwExpireDate = -1;
		g_ChinaBillingRemainedTime.dwRemainedTime = (DWORD)-1;
		break;

	case china_billing_time_type_second		:
		g_ChinaBillingRemainedTime.dwExpireDate = -1;
		g_ChinaBillingRemainedTime.dwRemainedTime = dwTime;
		break;

	case china_billing_time_type_timestamp	:
		g_ChinaBillingRemainedTime.dwExpireDate = dwTime;
		g_ChinaBillingRemainedTime.dwRemainedTime = (DWORD)-1;
		break;
	}
}

DWORD GetChinaBillingRemainedTime()
{
	if(!g_ChinaBillingRemainedTime.IsTimeSet()) return (DWORD)-1;

	switch(g_ChinaBillingRemainedTime.dwTimeType) 
	{
	case china_billing_time_type_none:
		return (DWORD)-1;
		break;

	case china_billing_time_type_second:
		{
			// ���� �ð��� ���Ѵ�. ������ ��!
			DWORD dwPastTime = (timeGetTime() - g_ChinaBillingRemainedTime.dwRegisteredTime)/1000; 
			return (g_ChinaBillingRemainedTime.dwRemainedTime - dwPastTime < 0) ? 
				0 : g_ChinaBillingRemainedTime.dwRemainedTime - dwPastTime;
		}
		break;

	case china_billing_time_type_timestamp:
		{
			time_t CurrentDate; // ������.
			time(&CurrentDate);

			double time = difftime(g_ChinaBillingRemainedTime.dwExpireDate, CurrentDate);
			return (time < 0) ? 0 : (DWORD)time;
		}
		break;
	}
	return 0;
}

LPCSTR GetChinaBillingRemainedTime(BOOL bRemainedTime)
{
	static char szRemainTimeMsg[0xff];

	if(g_ChinaBillingRemainedTime.IsTimeSet())	return NULL;

	switch(g_ChinaBillingRemainedTime.dwTimeType) 
	{
	case china_billing_time_type_none:
		lstrcpy(szRemainTimeMsg, g_Message[ETC_MESSAGE1191].szMessage); // "���ϴ� ���� ���̽ʴϴ�."
		break;

	case china_billing_time_type_second:
		{
			// ���� �ð��� ���Ѵ�. ������ ��!
		//	DWORD dwPastTime = (timeGetTime() - g_ChinaBillingRemainedTime.dwRegisteredTime)/1000; 
			DWORD dwTime = GetChinaBillingRemainedTime();

			// ������ ���� �ð� ���.
			DWORD dwDay		= dwTime / (60*60*24)	; dwTime -= (60*60*24) * dwDay;
			DWORD dwHour	= dwTime / (60*60)		; dwTime -= (60*60) * dwHour;
			DWORD dwMin		= dwTime / (60)			; dwTime -= 60 * dwMin;

			wsprintf(szRemainTimeMsg, g_Message[ETC_MESSAGE1192].szMessage, dwDay, dwHour, dwMin, dwTime); // "%u�� %u�ð� %u�� %u��"
		}
		break;

	case china_billing_time_type_timestamp:
		{
			// �Ⱓ���� �ϴ��� ���� �ð� ���.
			if(bRemainedTime)
			{
				time_t CurrentDate; // ������.
				time(&CurrentDate);

				DWORD dwTime = GetChinaBillingRemainedTime();

				DWORD dwDay		= dwTime / (60*60*24)	; dwTime -= (60*60*24) * dwDay;
				DWORD dwHour	= dwTime / (60*60)		; dwTime -= (60*60) * dwHour;
				DWORD dwMin		= dwTime / (60)			; dwTime -= 60 * dwMin;

				wsprintf(szRemainTimeMsg, g_Message[ETC_MESSAGE1192].szMessage, dwDay, dwHour, dwMin, dwTime); // "%u�� %u�ð� %u�� %u��"
			}
			// ���� �Ⱓ�� ���������� �� ���.
			else
			{
				tm* pExpireDate = NULL;
				char* szAMPM = g_Message[ETC_MESSAGE1193].szMessage; // "����";
				
				pExpireDate = localtime(&g_ChinaBillingRemainedTime.dwExpireDate);
				if(pExpireDate->tm_hour > 12)
				{
					pExpireDate->tm_hour -= 12;
					lstrcpy(szAMPM, g_Message[ETC_MESSAGE1194].szMessage); // "����"
				}

				if(pExpireDate->tm_hour == 0)
					pExpireDate->tm_hour = 12;

				wsprintf(szRemainTimeMsg, g_Message[ETC_MESSAGE1195].szMessage, // "%u�� %u�� %u�� %s %u�� %u�� %u��"
					pExpireDate->tm_year, 
					pExpireDate->tm_mon, 
					pExpireDate->tm_mday,
					szAMPM,
					pExpireDate->tm_hour,
					pExpireDate->tm_min,
					pExpireDate->tm_sec);
			}			
		}
		break;
	}

	return szRemainTimeMsg;
}


void CmdLoginChinaBillRemainedTime( char* pMsg, DWORD dwLen )
{
	WSTC_CHINA_BILL_REMAIN_TIME* pPacket = (WSTC_CHINA_BILL_REMAIN_TIME*)pMsg;

	// ���� ������ ���� �ð��� ����Ѵ�.... :)
	SetChinaBillingRemainedTime(pPacket->GetTimeType(), pPacket->GetTime());

	// Ÿ�̸� ���� ����!
	StartChinaBillingInformTimer();
}

void CmdWorldChinaBillKickCode(char* pMsg, DWORD dwLen)
{
	WSTC_CHINA_BILL_KICK_CODE* pPacket = (WSTC_CHINA_BILL_KICK_CODE*)pMsg;

	DWORD dwMsgID = 0;

	switch(pPacket->btKickCode)
	{
	case china_billing_kick_expired_personal_time	:	dwMsgID = ETC_MESSAGE1201;	break;
	case china_billing_kick_expired_pcroom_time		:	dwMsgID = ETC_MESSAGE1202;	break;
	case china_billing_kick_expired_personal_period	:	dwMsgID = ETC_MESSAGE1203;	break;
	case china_billing_kick_expired_pcroom_period	:	dwMsgID = ETC_MESSAGE1204;	break;
	case china_billing_kick_dupplicated_login		:	dwMsgID = ETC_MESSAGE1205;	break;
	case china_billing_kick_etc						:	dwMsgID = ETC_MESSAGE1206;	break;
	}

	// Kick�� ���� �޽��� �ڽ� ����. 
	CErrorWnd* pErrorWnd = CErrorWnd::GetInstance();	
	
	pErrorWnd->SetError(g_Message[dwMsgID].szMessage, g_Message[ETC_MESSAGE594].szMessage , "", 2);				
	pErrorWnd->SetActive(TRUE);
	pErrorWnd->m_byType		= 1;
	pErrorWnd->m_byTime		= 5;
	pErrorWnd->m_dwCurTime	= timeGetTime();
	pErrorWnd->m_dwPevTime	= timeGetTime();

	CInterface::GetInstance()->SetWnd(ERROR_WND);
}

void ShowChinaBillingRemainTime()
{	
	if(!g_ChinaBillingRemainedTime.IsTimeSet()) return;

	// ȭ�鿡 �޽��� �Ѹ���~~~ (���忡�� �ؾ� �Ѵ�!!)
	LPCSTR szRemainTimeMsg = GetChinaBillingRemainedTime(FALSE);

	if(NULL == szRemainTimeMsg) return;

	CWorldWnd* pWorldWnd = CWorldWnd::GetInstance();

	memset(pWorldWnd->szMessageLine, 0, sizeof(pWorldWnd->szMessageLine));

	switch(g_ChinaBillingRemainedTime.dwTimeType) 
	{
	case china_billing_time_type_none		:
		lstrcpy(pWorldWnd->szMessageLine[0], GetChinaBillingRemainedTime(TRUE));
		break;

	case china_billing_time_type_second		:
		lstrcpy(pWorldWnd->szMessageLine[0], g_Message[ETC_MESSAGE1196].szMessage); // "������ ���� ������ �ð���"
		lstrcpy(pWorldWnd->szMessageLine[1], GetChinaBillingRemainedTime(TRUE));
		lstrcpy(pWorldWnd->szMessageLine[2], g_Message[ETC_MESSAGE1197].szMessage); // "���ҽ��ϴ�."
		break;

	case china_billing_time_type_timestamp	:
		lstrcpy(pWorldWnd->szMessageLine[0], g_Message[ETC_MESSAGE1198].szMessage); // "������ ���� ��������"
		lstrcpy(pWorldWnd->szMessageLine[1], GetChinaBillingRemainedTime(FALSE));
		lstrcpy(pWorldWnd->szMessageLine[2], g_Message[ETC_MESSAGE1199].szMessage); // "�Դϴ�."
		break;
	}

	pWorldWnd->m_bInfoType = 0;
	pWorldWnd->SetActive();	
}

LPCSTR GetChinaBillingRemainedTimeString()
{
	static char szMsg[0xff];

	if(!g_ChinaBillingRemainedTime.IsTimeSet())
	{
		lstrcpy(szMsg, "No Billing Info.");
	}
	else
	{
		if(g_ChinaBillingRemainedTime.dwTimeType == china_billing_time_type_none) 
		{
			lstrcpy(szMsg, GetChinaBillingRemainedTime(TRUE));
		}
		else
		{
			// "������ ���� ������ �ð��� %s ���ҽ��ϴ�."
			wsprintf(szMsg, g_Message[ETC_MESSAGE1200].szMessage, GetChinaBillingRemainedTime(TRUE));
		}
	}

	return szMsg;
}

void OnChinaBillingLoginFail(BYTE btResult)
{
	CGameDefaultWnd* pGameDefaultWnd = CGameDefaultWnd::GetInstance();

	g_pGVLogin->bIsLoging = FALSE;

	btResult &= 0x0f;

	switch(btResult)
	{
	case china_billing_login_success		: // �� �ڵ�� ������ �ƴϴ�!!! �ٸ� �뵵�� ��������~ :)
		// "���� ������ �������� ���߽��ϴ�."
		// "��� �� �ٽ� �õ��� �ּ���."
		pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, 
			g_Message[ETC_MESSAGE1183].szMessage, 
			g_Message[ETC_MESSAGE339].szMessage, "", 402, 459, 2, TRUE);									
		break;

	case china_billing_login_no_user		:
		// "������ ���� ������ ã�� ���߽��ϴ�.";
		// ����� �����ϼ���."
		pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, 
			g_Message[ETC_MESSAGE1184].szMessage, 
			g_Message[ETC_MESSAGE483].szMessage, "", 402, 459, 2, TRUE);	
		break;

	case china_billing_login_over_capacity	:
		// "���� ������ �ִ� ó������ �Ѿ����ϴ�.";
		// "��� �� �ٽ� �õ��� �ּ���."
		pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, 
			g_Message[ETC_MESSAGE1185].szMessage, 
			g_Message[ETC_MESSAGE339].szMessage, "", 402, 459, 2, TRUE);	
		break;

	case china_billing_login_timeout		:
		// "���� �������� Ÿ�Ӿƿ� �Ǿ����ϴ�.";
		pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, 
			g_Message[ETC_MESSAGE1186].szMessage, "", "", 402, 459, 1, TRUE);	
		break;

	case china_billing_login_canceled		:
		// "���� ������ ���� ��ҵǾ����ϴ�.";
		pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, 
			g_Message[ETC_MESSAGE1187].szMessage, "", "", 402, 459, 1, TRUE);	
		break;

	case china_billing_login_etc			:
		// "��Ÿ ������ ���� ������ ���� �źεǾ����ϴ�.";
		// "����� �����ϼ���."
		pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, 
			g_Message[ETC_MESSAGE1188].szMessage, 
			g_Message[ETC_MESSAGE483].szMessage, "", 402, 459, 2, TRUE);	
		break;

	case china_billing_login_time_expired	:
		// "������ ������ ����Ǿ����ϴ�.";
		// "�ٽ� ������ �ֽñ� �ٶ��ϴ�."
		pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, 
			g_Message[ETC_MESSAGE1189].szMessage, 
			g_Message[ETC_MESSAGE1190].szMessage, "", 402, 459, 2, TRUE);	
		break;
	}

	pGameDefaultWnd->SetActive();
	LoginInterface(FALSE);

	g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_ID);
	g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
	g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
}

void StartChinaBillingInformTimer()
{
	SetTimer(g_hMainWnd, IDT_CHINA_BILLING_INFORM, CHINA_BILLING_INFORM_TIMER_PERIOD, OnChinaBillRemainTimeInform);
}

void StopChinaBillingInformTimer()
{
	KillTimer(g_hMainWnd, IDT_CHINA_BILLING_INFORM);
}

void CALLBACK OnChinaBillRemainTimeInform(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	static bool bOneHour = false;
	static bool bFiveMin = false;

	// ���� ���� �ð��� �����´�.
	DWORD dwRemainTicks = GetChinaBillingRemainedTime();

	// �ϴ� �д����� ����Ѵ�.
	DWORD dwRemainMin = dwRemainTicks / (1000*60);

	// 60���� ������.
	if(!bOneHour && dwRemainMin == 60)
	{
		bOneHour = true;

		ShowChinaBillingRemainTimeInform(60);
	}

	// 5���� ������..
	else if(!bFiveMin && dwRemainMin == 5)
	{
		bFiveMin = true;

		// ���̻� �ʿ������ Ÿ�̸ӵ� ������!
		StopChinaBillingInformTimer();

		ShowChinaBillingRemainTimeInform(5);
	}	
}

void ShowChinaBillingRemainTimeInform(DWORD dwMin)
{
	CErrorWnd* pErrorWnd = CErrorWnd::GetInstance();

	if(dwMin == 60)
	{
		pErrorWnd->SetError(g_Message[ETC_MESSAGE1207].szMessage, "" , "", 1);
	}
	else if(dwMin == 5)
	{
		pErrorWnd->SetError(g_Message[ETC_MESSAGE1208].szMessage, "", "", 1);
	}
	else 
	{
		return;
	}

	pErrorWnd->SetActive(TRUE);
	pErrorWnd->m_byType		= 77;
	
//	pErrorWnd->m_byTime		= 5;
//	pErrorWnd->m_dwCurTime	= timeGetTime();
//	pErrorWnd->m_dwPevTime	= timeGetTime();

	CInterface::GetInstance()->SetWnd(ERROR_WND);
}
