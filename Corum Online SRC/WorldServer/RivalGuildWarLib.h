/*#####################################################################//
//##################### 2004. 11. Ŭ����ȭ ����.. #####################
//#####################################################################
//
//	Version		: Dev/Develop
//	ProjectName	: CorumOnlineProject
//	FileName	: RivalGuildWarLib.h
//	Date		: 2004. 10. 18. ( minjin )
//	Description	: Declare Rival Guild War Global Function
//				  For Assistant to Set or Get Information About Guild
//
//#####################################################################*/

#ifndef __RIVAL_GUILD_WAR_LIB__
#define __RIVAL_GUILD_WAR_LIB__

#include "WorldUser.h"
#include "../CommonServer/AbyssHash.h"

//#define MIN_RGW_GUILD_LEVEL		5
//#define MIN_RGW_GUILD_MEMBER	5

enum EN_RivalGuildWarErrorCode
{
	RIVAL_GUILD_WAR_ERRCODE_DUMMY	= 0,	// ���̹� ����� �����ڵ� ����.. -_-;;;

	RGWE_VS_NOT_GUILD_MASTER		= 1,	// "������ ��帶���Ͱ� �ƴմϴ�."
	RGWE_VS_MUST_LEVEL5				= 2,	// "��� ��� ���� 5�̻��̾�� ���̹� ������� �����մϴ�."
	RGWE_VS_MORE_GUILD_MEMBER		= 3,	// "��� ��帶���͸� ������ ������ �ּ� 5�� �̻� �����ؾ� �մϴ�."
	RGWE_VS_NOT_ACCEPT_WAR			= 4,	// "%s ��� �����Ͱ� ���̹� ����� ���� ��û�� �����Ͽ����ϴ�."

	RGWE_NOT_GUILD_MASTER			= 11,	// "��� �����͸� ���̹� ������� ��û�� �� �ֽ��ϴ�."
	RGWE_MUST_LEVEL5				= 12,	// "��� ���� 5�̻��̾�� ���̹� ������� �����մϴ�."
	RGWE_MORE_GUILD_MEMBER			= 13,	// "��帶���͸� ������ ������ �ּ� 5�� �̻� �����ؾ� �մϴ�."

	RGWE_NOW_ON_GUILD_WAR			= 21,	// "���� ���̹� ������� ����ǰ� �ֽ��ϴ�."
	RGWE_SAME_GUILD					= 22,	// "���� �������� ���̹� ������� ��û�� �� �����ϴ�."
	RGWE_CANNOT_REMOVE_GUILD		= 23,	// "���̹� ����� �� �������� �Ұ��� �մϴ�."
	RGWE_NOW_WAIT_ACCEPT			= 24,	// "���� ���̹� ����� �������Դϴ�. ��� �Ŀ� �ٽ� �õ����ּ���."

	RGWE_GUILD_DATA_ERROR			= 100,	// ���̹� ����� ������ ������.. -_-;;;

	RGWE_GUILD_WAR_SUCCESS			= 255,
};

//BYTE	RGWLibCheckCondition( DWORD dwMyGuildIdx, DWORD dwOpGuildIdx );

VOID	ConfirmDestRGWDungeon( DWORD* dwDungeonID, DWORD* dwDestLayer );

/*
CWorldUser* IsThereGuildMaster(ListNode<CWorldUser>* pGuildUserPos);
//*/


#endif
