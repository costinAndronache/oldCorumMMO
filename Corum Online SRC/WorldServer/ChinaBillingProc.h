#ifndef __CHINA_BILLING_PROC_H__
#define __CHINA_BILLING_PROC_H__

#pragma once

enum ECHINA_BILLING_CHECK
{
	enum_level_approval		= 1,
	enum_level_free			= 2,
	enum_level_limit		= 3,
};

struct SCHINA_BILLING_INFO
{
	ECHINA_BILLING_CHECK	eType;
	DWORD					dwPoint;
};

// Kick ����.
struct CHINA_KICK_USER_DATA
{
	DWORD dwCharacterIndex;
	DWORD dwKickDecistionTime;
};

// Kick üũ Ÿ�ӿ� ����Ǵ� callback �Լ�.
void __stdcall OnChinaKickOut(DWORD dwEventIndex);

// ���� ���� �� �������� Kick �ϴ� �Լ�.
void ChinaBillingKickOutProc(DWORD dwCharacterIndex);

// Kick ���� ���
void AddChinaBillingKickList(DWORD dwCharacterIndex);

void AcmdChinaBillingInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

#endif // __CHINA_BILLING_PROC_H__
