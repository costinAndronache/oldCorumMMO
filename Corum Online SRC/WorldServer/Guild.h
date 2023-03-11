#ifndef __GUILD_H__
#define __GUILD_H__

#pragma once

#include "WorldUser.h"
#include "GuildHash.h"

extern char g_szGuildBattleText[MAX_GUILD_TEXT][0xff];

#define	MIN_GUILD_LEVEL				3

#if defined(JAPAN_LOCALIZING)
// Remark
#define	MIN_GUILD_MEMBER			1
#else
#define	MIN_GUILD_MEMBER			1
#endif

enum GUILD_WAR_ERRCODE
{
	GWE_VS_NOT_EXIST_MASTER		= 0,	// "��� ��帶���Ͱ� ���ӿ� ������ ���� �ʽ��ϴ�."
	GWE_VS_OUTOFBOUNDS_MAX_WAR	= 1,	// "��� ��尡 �ִ� 8���� �ٸ� ���� ����� ���Դϴ�."
	GWE_VS_MUST_LEVEL3			= 2,	// "��� ��� ���� 3�̻��̾�� ������� �����մϴ�."
	GWE_VS_MORE_GUILD_MEMBER	= 3,	// "��� ��帶���͸� ������ ������ �ּ� 1�� �̻� �����ؾ� �մϴ�."
	GWE_VS_NOT_ACCEPT_WAR		= 4,	// "%s ��� �����Ͱ� ����� ���� ��û�� �����Ͽ����ϴ�."
	GWE_NOT_GUILD_MASTER		= 5,	// "��� �����Ͱ� �ƴմϴ�."
	GWE_OUTOFBOUNDS_MAX_WAR		= 6,	// "��尡 �ִ� 8���� �ٸ� ���� ����� ���Դϴ�."
	GWE_MUST_LEVEL3				= 7,	// "��� ���� 3�̻��̾�� ������� �����մϴ�."
	GWE_MORE_GUILD_MEMBER		= 8,	// "��帶���͸� ������ ������ �ּ� 1�� �̻� �����ؾ� �մϴ�."
	GWE_NOW_ON_GUILD_WAR		= 9,	// "���� ������� ����ǰ� �ֽ��ϴ�."
	GWE_SAME_GUILD				= 10,	// "���� �������� ������� ��û�� �� �����ϴ�."
	GWE_CANNOT_REMOVE_GUILD		= 11,	// "����� �� �������� �Ұ��� �մϴ�."
	GWE_NOW_WAIT_ACCEPT			= 12,	// "���� ����� �������Դϴ�. ��� �Ŀ� �ٽ� �õ����ּ���."
	GWE_BEFORE_END_TIME			= 13,	// "����� �¶��� 3��(72�ð�)���Ŀ� ���ᰡ �����մϴ�."
	GWE_GUILD_DATA_ERROR		= 100,	// ����� ������ ������.. -_-;;;
	GWE_GUILD_WAR_SUCCESS		= 255
};

void CmdGuildUserDel(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdGuildInfo(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdGuildRank(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdGuildJoin(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdGuildCreate(CWorldUser* pUser ,char* pMsg, DWORD dwLength);
void CmdUserGuildMaster(CWorldUser* pUser, char* pMsg, DWORD dwcLength);
void CmdGuildDelete(CWorldUser* pUser, char* pMsg, DWORD dwcLength);
void CmdGuildJoinMessage(CWorldUser* pUser ,char* pMsg, DWORD dwLength);
void CmdGuildRequest(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdGuildUserInfo(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdGuildDataRequest(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdGuildUserRequestFail(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdGuildWarRequest(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdGuildWarErrorRt(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdGuildWarMasterRt(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdGuildWarCreate(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdGuildWarExit(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdGuildWarRank(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void SendToUserGuildWarFail(CWorldUser* pUser, BYTE byErrorCode);
int	 GuildWarCheckCondition( DWORD dwGuildID, DWORD dwDstGuildID );

BOOL InitGuildInformations();

BOOL IsGuildWar(DWORD dwOffenceId, DWORD dwDefenseID);

void GuildWarTime(LP_GUILDWAR_TIME lpGuildWarTime, DWORD dwGuildId, DWORD dwGuildIdDst);

void GuildPoint(DWORD dwGuildId);

extern CWorldUser* IsThereGuildMaster(ListNode<CWorldUser>* pGuildUserPos, const bool bIsOnlyGuildMaster = true);//: 050202 hwoarang �渶 �ܿ� 2��ޱ��� �޴´�

#endif	// __GUILD_H__