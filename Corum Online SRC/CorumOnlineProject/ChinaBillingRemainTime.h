#ifndef __CHINA_BILLING_REMAINED_TIME_H__
#define __CHINA_BILLING_REMAINED_TIME_H__

#pragma once

// billing time type code from billing server
enum CHINA_BILLING_TIME_TYPE
{
	china_billing_time_type_none				= 0x00,
	china_billing_time_type_second				= 0x01,
	china_billing_time_type_timestamp			= 0x02,
};

// login result code from billing server
enum CHINA_BILLING_LOGIN_CODE
{
	china_billing_login_success			= 0x00,
	china_billing_login_unexpected1		= 0x01,
	china_billing_login_unexpected2		= 0x02,

	china_billing_login_no_user			= 0x03,
	china_billing_login_over_capacity	= 0x04,
	china_billing_login_timeout			= 0x05,
	china_billing_login_canceled		= 0x06,
	china_billing_login_etc				= 0x07,

	china_billing_login_time_expired	= 0x0f,
};

// kick out code from billing server
enum CHINA_BILLING_KICK_CODE
{
	china_billing_kick_expired_personal_time	= 0x01,
	china_billing_kick_expired_pcroom_time		= 0x02,
	china_billing_kick_expired_personal_period	= 0x03,
	china_billing_kick_expired_pcroom_period	= 0x04,
	china_billing_kick_dupplicated_login		= 0x05,
	china_billing_kick_etc						= 0x06,
};

// Ÿ�̸� ID
#define IDT_CHINA_BILLING_INFORM			1218
#define CHINA_BILLING_INFORM_TIMER_PERIOD	60*1000 // 1 MIN
	

struct CHINA_REMAINED_TIME
{
	BOOL	bReceived;
	DWORD	dwTimeType;
	DWORD	dwRegisteredTime;	// ���� ���� �ð��� ��ϵ� �ð�.
	DWORD	dwRemainedTime;	
	time_t	dwExpireDate;

	CHINA_REMAINED_TIME()
	{
		ZeroMemory(this, sizeof(CHINA_REMAINED_TIME));
	}
	
	BOOL	IsTimeSet()	{ return bReceived;	}
};



void	CmdLoginChinaBillRemainedTime( char* pMsg, DWORD dwLen );	// ���� ���� �˷��ִ� �޽��� ó��.
void	CmdWorldChinaBillKickCode(char* pMsg, DWORD dwLen);			// ����� Kick ���� ���� ó��.

void	OnChinaBillingLoginFail(BYTE btResult);						// �߱� ������ ���õ� �α��� ���� �� ó��.

DWORD	GetChinaBillingRemainedTime();								// ���� ��� ���� �ð��� ms ������ ����.
void	ShowChinaBillingRemainTime();								// ���� ó�� ���� ��, ���� ���� �޽��� �ڽ� �����.
LPCSTR	GetChinaBillingRemainedTimeString();						// �ܿ� �÷��� ���� �ð��� �ʴ����� �˷���.
void	SetChinaBillingRemainedTime(DWORD dwType, DWORD dwTime);	// ���� �ð��� ���� ����ü�� ����!

// ���� �ð�(1�ð�/5��) �˷��ֱ� ���� Ÿ�̸�.
void	StartChinaBillingInformTimer();
void	StopChinaBillingInformTimer();
void	CALLBACK OnChinaBillRemainTimeInform(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime); // timer function!
void	ShowChinaBillingRemainTimeInform(DWORD dwMin);




#endif // __CHINA_BILLING_REMAINED_TIME_H__