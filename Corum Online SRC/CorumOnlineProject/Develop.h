//------------------------------------------------------------------------------
//#define PARTY_BOARD

//------------------------------------------------------------------------------
// ���� �������� ���������� �����ϴ� ��츸 Ǯ���.
// �̰� Ǯ�������� ������� �Ұ��ϴ�!!!!
// �׷��� ������ EXE ���忡�� �ݵ�� Ǯ�� �� ��!!!!
//#define __RELEASE_BUILD	
//------------------------------------------------------------------------------

/*==============================================================================*/
/*						FOR DEVELOPER DEFINITIONS								*/
//#define INNER_TEST_SERVER_CONNECT
/*==============================================================================*/

/*==============================================================================*/
/*						THE GAME SYSTEM DEFINITIONS								*/
#define	EQUIP_CHK
/*==============================================================================*/


// ���ο��� �� ���� �; 
#if defined(DEVELOP_MODE)
	#define __PACKAGE_MODE__
#endif

#if !defined(DEVELOP_MODE)
	#ifndef __PACKAGE_MODE__
		#define __PACKAGE_MODE__
	#endif
#endif

#define	__RANDOM_MEMORY_ALLOCATION			// ���� ĳ���� ���� ��, �޸� ��ġ ����ȭ!!
//#define	__USE_CLIENT_SPEEDHACK_CHECKER	// Ŭ���̾�Ʈ ���ǵ��� üũ Ȱ��ȭ!
//#define	__WITH_EXCEPTION_FILTER			// ũ���� ������ ����� ���ο� Ŭ���̾�Ʈ ���� �� Ǯ���.
//#define	__PACKET_ENCRYPTION				// ��Ŷ ��ȣȭ�� ���ؼ��� Ǭ��.
//#define	__TRAFFIC_LOG					// Ŭ���̾�Ʈ�� �ۼ��� Ʈ������ ����ϰ� ������ Ǯ���~!
											// 1 �� ������ Ʈ������ ����Ѵ�! by deepdark~!


#pragma once


#include "Localizing.h"
#include "define.h"


#if IS_CHINA_LOCALIZING()
//#	define __SKILL_MASTERY_ENABLE	// ��ų �����͸��� Ȱ��ȭ�Ұ��ΰ�? �ϰ� ������ Ǯ�����.
#	define __ITEM_DROPBOX_ENABLE
#	define __USE_STAT_LIMIT
#endif

#if IS_JAPAN_LOCALIZING()
	#ifndef __PACKAGE_MODE__
		#define __PACKAGE_MODE__
	#endif
	#define __USE_STAT_LIMIT
#endif


#define MODE_FOR_DEBUG				100


//#define J0_WORK_SECTION

#define DEVELOP_START_NORMAL		0
#define DEVELOP_START_WORLD			1
#define DEVELOP_START_MAP			2


struct DEVELOP_VARIABLE
{
	BYTE	bBeginType;
	BYTE	bFullScreen;
	BYTE	bRenderFramePerSec;
	WORD	wWorldPort;
	WORD	wBeginMapID;

	char szID[MAX_ID_LENGTH];
	char szPassword[MAX_PASSWORD_LENGTH];
	char szWorldIP[16];
	char szCharacterName[ MAX_CHARACTER_NAME_LENGTH ];
	char szDevIniPath[ _MAX_PATH ];
};


struct Protocol_Debug
{
	enum 
	{
		CMD_LOGIN					= 0
	};
};


#pragma pack(push,1)

//-----------------------------------------------------------------------------
// Packet: DEBUG_LOGIN
// Desc: �����ڿ� LoginPacket
//-----------------------------------------------------------------------------
struct DEBUG_LOGIN
{
	BYTE	bGameStatus;
	BYTE	bHeader;
	char	szID[ MAX_ID_LENGTH ];
	char	szPassword[ MAX_PASSWORD_LENGTH ];


	DEBUG_LOGIN()	{	bGameStatus = MODE_FOR_DEBUG;	bHeader = Protocol_Debug::CMD_LOGIN;	}
	DWORD	GetPacketSize()	{	return sizeof(DEBUG_LOGIN);	}
};

#pragma pack(pop)





















