#pragma once


#include "ProtocolDefinition.h"
#include "Item.h"

#pragma pack(push,1)

enum NATIONAL_CODE
{
	National_Code_Korea,
	National_Code_Japan,
	National_Code_China,
	National_Code_Taiwan,
};

//-----------------------------------------------------------------------------
// Client���� ������ Login�� �õ��Ѵ�.(������)
//-----------------------------------------------------------------------------
struct CTWS_LOGIN : public LOGIN_PACKET_HEADER
{
	BYTE	bySigniture[4];
	BYTE	byNationCode;
	DWORD	dwVersion;
	char	szID[MAX_ID_LENGTH];
	char	szPassword[MAX_PASSWORD_LENGTH];

#ifdef __PACKET_ENCRYPTION
	BYTE	byClientKey[10];
#endif

	CTWS_LOGIN()			
	{	
		bGameStatus = UPDATE_GAME_LOGIN;	bHeader = Protocol_Login::CMD_LOGIN;	
		bySigniture[0] = '@'; bySigniture[1] = 'S'; bySigniture[2] = 'A'; bySigniture[3] = 'D';	

		byNationCode = 
#if defined JAPAN_LOCALIZING
			National_Code_Japan;
#elif defined TAIWAN_LOCALIZING  			
			National_Code_Taiwan;
#elif defined CHINA_LOCALIZING  
			National_Code_China;
#elif defined KOREA_LOCALIZING 
			National_Code_Korea;
#else
			National_Code_Korea;
#endif			
	}

	DWORD	GetPacketSize()	{	return sizeof(CTWS_LOGIN);	}
};


//-----------------------------------------------------------------------------
// Client���� ������ Login�� �õ��Ѵ�.(�ݸ����)
//-----------------------------------------------------------------------------
struct CTWS_NETMARBLE_LOGIN : public LOGIN_PACKET_HEADER
{
	BYTE	bySigniture[4];
	DWORD	dwVersion;
	BYTE	bNetmarleLogin;
	BYTE	byNationCode;

	char	szID[MAX_ID_LENGTH];
	char	szPassword[MAX_PASSWORD_LENGTH];
	char	szCPCookie[MAX_CP_COOKIE_LENGTH];

#ifdef __PACKET_ENCRYPTION
	BYTE	byClientKey[10];
#endif

	CTWS_NETMARBLE_LOGIN()			
	{	
		bGameStatus = UPDATE_GAME_LOGIN;	bHeader = Protocol_Login::CMD_LOGIN_NETMARBLE;	
		memset(szID, 0, MAX_ID_LENGTH);
		memset(szPassword, 0, MAX_PASSWORD_LENGTH);
		bySigniture[0] = '@'; bySigniture[1] = 'S'; bySigniture[2] = 'A'; bySigniture[3] = 'D';
		byNationCode = National_Code_Korea;
	}

	DWORD	GetPacketSize()	{	return sizeof(CTWS_NETMARBLE_LOGIN);	}
};


//-----------------------------------------------------------------------------
// Client���� Login�� �����ߴٰ� �˷��ش�. 
//-----------------------------------------------------------------------------
struct WSTC_LOGIN_FAIL : public LOGIN_PACKET_HEADER
{
	BYTE	bResult;	//0: ���Ʋ��, 2: ���¾��̵� , 3: ���̵�����, 4: ���� �ִ��ο� �ʰ�, 5: �׽�Ʈ �Ⱓ�� ������ �� ���� ���̵�, 
						//6: ����Ʋ��, 7: ���� �������� �α��� �õ���, 8: ĳ���� �ڸ� ��ħ 
						//1: �α��� ����(������)
	DWORD	dwExData;	//��Ÿ ���� ������ 

	WSTC_LOGIN_FAIL()	{	bGameStatus = UPDATE_GAME_LOGIN;	bHeader = Protocol_Login::CMD_LOGIN_FAIL;	dwExData = 0; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_LOGIN_FAIL);	}
};


struct RS_CharacterLogInfo
{
	DWORD	dwCharacterIndex;						// DB�� CharacterIndex
	char	szName[ MAX_CHARACTER_NAME_LENGTH ];	// ĳ���� �̸� 
	WORD	wChrNum;								// ĳ���� ��ȣ (1 ~ 4)
	WORD	wHead;
	WORD	wClass;									// Ŭ���� 
	WORD	wGrade;									// �׷��̵� 100�̸� GM
	DWORD	dwLevel;								// ���� 
	DWORD	dwExp;									// ����ġ 
	DWORD	dwEgo;									// EGO
	DWORD	dwStr;									// STR
	DWORD	dwVit;									// VIT
	DWORD	dwDex;									// DEX
	DWORD	dwInt;									// INT		
	DWORD	dwHonor;								// ��
	DWORD	dwLuck;									// ���
	DWORD	dwRecentWorldmapID;						// �ֱ� ������ WorldMap ID
	WORD	wLHandID;			
	WORD	wRHandID;
	WORD	wHelmetID;
	WORD	wMantleID;								// ����
	WORD	wMailID;								// ����	
};


//-----------------------------------------------------------------------------
// Login�� ���������� 4���� ĳ���� ������ �Բ� �����ش�. 
//-----------------------------------------------------------------------------
struct WSTC_LOGIN_SUCCESS : public LOGIN_PACKET_HEADER
{
	BYTE				bAdultMode;		// ���θ��
	BYTE				bNum;			// ĳ���� ���� 
	RS_CharacterLogInfo	Character[4];

	WSTC_LOGIN_SUCCESS()	
	{	
		memset(Character, 0, sizeof(Character));
		bGameStatus = UPDATE_GAME_LOGIN;	
		bHeader = Protocol_Login::CMD_LOGIN_SUCCESS;	
	}

	DWORD	GetPacketSize()	{	return 4 + sizeof(RS_CharacterLogInfo)*bNum;	}	
};


//-----------------------------------------------------------------------------
// �������� ���� ������ �����ֽÿ�.
//-----------------------------------------------------------------------------
struct CTAS_REQUEST_SERVER_ALIVE : public LOGIN_PACKET_HEADER
{
	BYTE	bExpansion;

	CTAS_REQUEST_SERVER_ALIVE()		{ bGameStatus = UPDATE_GAME_LOGIN;	bHeader = Protocol_Login::CMD_REQUEST_SERVER_ALIVE;	}
	DWORD	GetPacketSize()			{ return sizeof(CTAS_REQUEST_SERVER_ALIVE); }
};


//-----------------------------------------------------------------------------
// Ŭ���̾�Ʈ���� �������� ���� ������ ������.
//-----------------------------------------------------------------------------
struct ASTC_SERVER_ALIVE_INFO : public LOGIN_PACKET_HEADER
{
	DWORD	dwServerID;				// Server ID.
	WORD	wServerType;			// 0 : World server, 1 : dungeon.
	WORD	wConnectedUserCount;	// ���� ���� ��ġ.
	DWORD	dwMagicItemDropRate;	// ���� ������ ��ӷ�.
	DWORD	dwExpGainRate;			// ����ġ ȹ���.

	ASTC_SERVER_ALIVE_INFO()		{ bGameStatus = UPDATE_GAME_LOGIN;	bHeader = Protocol_Login::CMD_SERVER_ALIVE_INFO;	}
	DWORD	GetPacketSize()			{ return sizeof(ASTC_SERVER_ALIVE_INFO); }
};


//-----------------------------------------------------------------------------
// ���� ����͸� ������ üũ�� �ֽÿ�.
//-----------------------------------------------------------------------------
struct CTAS_CHECK_MONITORING : public LOGIN_PACKET_HEADER
{
	BYTE	bExpansion;

	CTAS_CHECK_MONITORING()		{ bGameStatus = UPDATE_GAME_LOGIN;	bHeader = Protocol_Login::CMD_CHECK_MONITORING;	}
	DWORD	GetPacketSize()		{ return sizeof(CTAS_CHECK_MONITORING); }
};


//-----------------------------------------------------------------------------
// CMTool ������ �������� ��Ŷ�� �Դ�.  
//-----------------------------------------------------------------------------
struct CTAS_CMTOOL_NOTICE : public LOGIN_PACKET_HEADER
{
	WORD	wMsgLen;
	char	szMsg[1024];
	
	CTAS_CMTOOL_NOTICE()			{ bGameStatus = UPDATE_GAME_LOGIN; bHeader = Protocol_Login::CMD_CMTOOL_NOTICE; }
	DWORD	GetPacketSize()			{ return 4 + wMsgLen; }
};


//-----------------------------------------------------------------------------
// ���� ������ ���� ���θ� �ľ��Ѵ�.
//-----------------------------------------------------------------------------
struct CTAS_IS_ALIVE_USER : public LOGIN_PACKET_HEADER
{
	char	szID[MAX_ID_LENGTH];
	DWORD	Kind;
	
	CTAS_IS_ALIVE_USER()
	{
		bGameStatus = UPDATE_GAME_LOGIN;
		bHeader = Protocol_Login::CMD_IS_ALIVE_USER;
		memset(szID, 0, MAX_ID_LENGTH);
	}
	
	DWORD	GetPacketSize()			{ return sizeof(CTAS_IS_ALIVE_USER); }
};


//-----------------------------------------------------------------------------
// ���� ������ ���� ���θ� �ľ��ؼ� Ŭ���̾�Ʈ�� �����ش�.
//-----------------------------------------------------------------------------
struct ASTC_IS_ALIVE_USER : public LOGIN_PACKET_HEADER
{
	BOOL	bIsAliveUser;

	ASTC_IS_ALIVE_USER()			{ bGameStatus = UPDATE_GAME_LOGIN; bHeader = Protocol_Login::CMD_ANSWER_ALIVE_USER; }
	DWORD	GetPacketSize()			{ return sizeof(ASTC_IS_ALIVE_USER); }
};


//-----------------------------------------------------------------------------
// CM������ �α��μ������� �ش� User�� ���� Logout �϶�� ���� 
//-----------------------------------------------------------------------------
struct CTAS_KICK_USER : public LOGIN_PACKET_HEADER
{
	DWORD	dwCharIndex;
	DWORD	dwPropID;
	
	CTAS_KICK_USER()		{	bGameStatus = UPDATE_GAME_LOGIN;	bHeader = Protocol_Login::CMD_CM_KICK_USER;		}
	DWORD	GetPacketSize()	{	return sizeof(CTAS_KICK_USER);	}
};


//-----------------------------------------------------------------------------
// Client���� �������� �����̶� �α��� ���� �Ǿ��ٰ� �˷��ش�. 
//-----------------------------------------------------------------------------
struct WSTC_BLOCK_ACCOUNT : public LOGIN_PACKET_HEADER
{
	BYTE	bLen;
	char	szReason[0xff];	

	WSTC_BLOCK_ACCOUNT()	{	bGameStatus = UPDATE_GAME_LOGIN;	bHeader = Protocol_Login::CMD_LOGIN_FAIL_WITH_REASON; memset(szReason, 0, 0xff);}
	DWORD	GetPacketSize()	{	return 3 + bLen;	}
};


//-----------------------------------------------------------------------------
// �߱� ���� ��, ���� ���� �����ϸ� ���� �ð�/�Ⱓ�� ������ Ŭ���̾�Ʈ���� ���� 
//-----------------------------------------------------------------------------
struct WSTC_CHINA_BILL_REMAIN_TIME : public LOGIN_PACKET_HEADER
{
	BYTE				btTimeType;
	DWORD				dwTimeValue;

	WSTC_CHINA_BILL_REMAIN_TIME()
	{
		bGameStatus = UPDATE_GAME_LOGIN;
		bHeader		= Protocol_Login::CMD_CHINA_BILL_REMAINED_TIME;
		btTimeType	= 0xff;
		dwTimeValue = 0;
	}

	void	SetTimeType(BYTE btType){ btTimeType = btType; }
	void	SetTime(DWORD dwTime)	{ dwTimeValue = dwTime; }
	BYTE	GetTimeType()			{ return btTimeType; }
	DWORD	GetTime()				{ return dwTimeValue; }

	DWORD	GetPacketSize()			{ return sizeof(WSTC_CHINA_BILL_REMAIN_TIME); }
};


//-----------------------------------------------------------------------------
// Ŭ���̾�Ʈ���� ���� ��Ʈ Ű�� ����.
//-----------------------------------------------------------------------------
struct WSTC_ENCRYPTION_KEY : public LOGIN_PACKET_HEADER
{
	BYTE				byServerKey[10];

	WSTC_ENCRYPTION_KEY()
	{
		bGameStatus = UPDATE_GAME_LOGIN;
		bHeader		= Protocol_Login::CMD_ENCRYPTION_KEY;
	}

	LPBYTE	GetKey()				{ return byServerKey;	}
	void	SetKey(LPBYTE btKey)	{ CopyMemory(byServerKey, btKey, 10);	}

	DWORD	GetPacketSize()			{ return sizeof(WSTC_ENCRYPTION_KEY); }
};


#pragma pack(pop)