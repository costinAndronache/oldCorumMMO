#pragma once


#include "LoginPacket.h"
#include "ProtocolDefinition.h"	
#include "Item.h"


#pragma pack(push,1)


//-----------------------------------------------------------------------------
// ������ ĳ���͸� �����ش�. 
//-----------------------------------------------------------------------------
struct CTWS_CHARACTER_SELECT : public CHARSELECT_PACKET_HEADER
{
	BYTE	bCharacterIndex;	//4�� �迭�� �迭 Index
	
	CTWS_CHARACTER_SELECT()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_CHARACTER_SELECT;	}
	DWORD	GetPacketSize()	{	return (DWORD)sizeof(CTWS_CHARACTER_SELECT);	}
};


//-----------------------------------------------------------------------------
// ���ο� ĳ���͸� �����Ѵ�. 
//-----------------------------------------------------------------------------
struct CTWS_CREATE_NEW_CHARACTER : public CHARSELECT_PACKET_HEADER
{
	BYTE	bChrNum;		//DB������ 1,2,3,4 �ڸ� 
	BYTE	bClass;			//Ŭ����
	WORD	wHead;			//�� ��ȣ 
	WORD	wStartMapID;	//ó�� ���� �� ��ȣ 
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];

	CTWS_CREATE_NEW_CHARACTER()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_CREATE_NEW_CHARACTER;		}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_CREATE_NEW_CHARACTER);	}
};


//-----------------------------------------------------------------------------
// ĳ���� ���� ��� ���� ��Ŷ 
//-----------------------------------------------------------------------------
struct WSTC_CREATE_CHARACTER_SUCCESS : public CHARSELECT_PACKET_HEADER
{
	DWORD	dwStr;
	DWORD	dwVit;
	DWORD	dwDex;
	DWORD	dwInt;
	DWORD	dwEgo;
	
	WSTC_CREATE_CHARACTER_SUCCESS()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_CREATE_CHARACTER_SUCCESS;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_CREATE_CHARACTER_SUCCESS);	}
};


//-----------------------------------------------------------------------------
// ĳ���͸� �����Ѵ�. 
//-----------------------------------------------------------------------------
struct CTWS_DELETE_CHARACTER : public CHARSELECT_PACKET_HEADER
{
	BYTE	bChrNum;
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	char	szJumin[7];

	CTWS_DELETE_CHARACTER()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_DELETE_CHARACTER;		}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_DELETE_CHARACTER);	}
};


//-----------------------------------------------------------------------------
// ĳ���� ���� ��� ���� ��Ŷ 
//-----------------------------------------------------------------------------
struct WSTC_CREATE_CHARACTER_FAIL : public CHARSELECT_PACKET_HEADER
{
	int		nResultValue;

	WSTC_CREATE_CHARACTER_FAIL()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_CREATE_CHARACTER_FAIL;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_CREATE_CHARACTER_FAIL);	}
};


//-----------------------------------------------------------------------------
// ������ ĳ���͸� �����ϸ� ���忡�� �ʿ��� ������ ĳ������ ������ �����ش�. 
//-----------------------------------------------------------------------------
struct WSTC_WORLD_USER_INFO : public CHARSELECT_PACKET_HEADER
{ // ���⿡ �ʵ带 �߰��Ұ�� ����Ʒ��� �߰��ؾ߸� �Ѵ�. �ɰ��ѹ����� �߻������� �𸥴�. ��� Ż�� ���� ���� �迵�� 2005.03.24
	DWORD		dwUserIndex;
	DWORD		dwRecentOutMap;
	DWORD		dwRecentOutPos;
	CItem		Equip[MAX_EQUIP_POOL];
	char		szGuildName[MAX_CHARACTER_NAME_LENGTH];
	char		szNickName[MAX_CHARACTER_NAME_LENGTH];
	BYTE		byType;
	BYTE		byRank;
	DWORD		dwLev;
	WORD		wClass;
	WORD		wHead;	
	DWORD		dwGuildId;
	DWORD		dwMoney;
	BYTE		bySkillKey[16];
	BYTE		byPosKey[16];
	short		wCriminalTime;
	BYTE		byHelpTypeA;
	BYTE		byHelpTypeB;
	BYTE		byHelpInterface;
	BYTE		byCurrentHand;
	BYTE		byMessenger;	
	DBTIMESTAMP	sGuildJoinTime;
	DWORD		dwPrevGuildId;	
	float		fStartPos_X;		//WorldMap������ ������ġ 
	float		fStartPos_Z;		//WorldMap������ ������ġ 
	BYTE		dwGuildWarFlag;		// 0 : �⺻. 1 : ����� ��, 2 : ����
	
	WSTC_WORLD_USER_INFO()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_WORLD_USER_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_WORLD_USER_INFO);	}
};


//-----------------------------------------------------------------------------
// ĳ���� ���� ���� 
//-----------------------------------------------------------------------------
struct ASTC_CHARACTER_SELECT_FAIL : public CHARSELECT_PACKET_HEADER
{
	BYTE	bErrorCode;

	ASTC_CHARACTER_SELECT_FAIL()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_CHARACTER_SELECT_FAIL;	}
	DWORD	GetPacketSize()	{	return (DWORD)sizeof(ASTC_CHARACTER_SELECT_FAIL);	}
};


//-----------------------------------------------------------------------------
// WorldServer�� �����϶�  
//-----------------------------------------------------------------------------
struct ASTC_CONNECT_WORLD_SERVER : public CHARSELECT_PACKET_HEADER
{
	DWORD	dwIp;
	WORD	wPort;
	DWORD	dwPropID;
	DWORD	dwChar_Index;
	DWORD	dwSerialCode;
	BYTE	bEventFlag;

	ASTC_CONNECT_WORLD_SERVER()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::ACMD_CONNECT_WORLD_SERVER;	}
	DWORD	GetPacketSize()	{	return (DWORD)sizeof(ASTC_CONNECT_WORLD_SERVER);	}
};


//-----------------------------------------------------------------------------
// World�� �α��� 
//-----------------------------------------------------------------------------
struct CTWS_WORLD_LOGIN : public CHARSELECT_PACKET_HEADER
{
	DWORD	dwPropID;
	DWORD	dwChar_Index;
	DWORD	dwSerialCode;
	BYTE	bEventFlag;

	CTWS_WORLD_LOGIN()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_WORLD_LOGIN;		}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_WORLD_LOGIN);	}
};


//-----------------------------------------------------------------------------
// World�� �α��� ���� 
//-----------------------------------------------------------------------------
struct WSTC_WORLD_LOGIN_FAIL : public CHARSELECT_PACKET_HEADER
{
	BYTE	bErrorCode;

	WSTC_WORLD_LOGIN_FAIL()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_WORLD_LOGIN_FAIL;		}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_WORLD_LOGIN_FAIL);	}
};


//-----------------------------------------------------------------------------
// ĳ���� ���� ��� ��Ŷ 
//-----------------------------------------------------------------------------
struct WSTC_DELETE_CHARACTER_RESULT : public CHARSELECT_PACKET_HEADER
{
	BYTE	bSuccess;		// �����̸� TRUE, ���и� FALSE
	int		nResultValue;
	
	WSTC_DELETE_CHARACTER_RESULT()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_DELETE_CHARACTER_RESULT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_DELETE_CHARACTER_RESULT);	}
};


//-----------------------------------------------------------------------------
// Item�� ó���Ҷ��� ����, ���� ���� ��Ŷ 
//-----------------------------------------------------------------------------
struct ASTC_ITEMMALL_INFO : public CHARSELECT_PACKET_HEADER
{
	BYTE	bSuccess;
	BYTE	bItemCount;
	BYTE	bItemAllCount;	
	BYTE	byItemMallType;

	ASTC_ITEMMALL_INFO()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::ACMD_ITEMMALL_INFO;	}
	DWORD	GetPacketSize()	{	return (DWORD)sizeof(ASTC_ITEMMALL_INFO);	}
};


//-----------------------------------------------------------------------------
// Client���� ������̶�� 
//-----------------------------------------------------------------------------
struct WSTC_WARNING_TITLE : public CHARSELECT_PACKET_HEADER
{
	BYTE	bLen;
	char	szReason[0xff];	

	WSTC_WARNING_TITLE()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_WARNING_TITLE; memset(szReason, 0, 0xff);}
	DWORD	GetPacketSize()	{	return 3 + bLen;	}
};


//-----------------------------------------------------------------------------
// ĳ���� ���� ����.
//-----------------------------------------------------------------------------
struct ASTC_CHR_MOVECHK : public CHARSELECT_PACKET_HEADER
{
	BYTE	byCnt;
	BYTE	byType;
		
	ASTC_CHR_MOVECHK()				{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_CHR_MOVE_CHK;		}
	DWORD	GetPacketSize()			{ return sizeof(ASTC_CHR_MOVECHK); }
};


//-----------------------------------------------------------------------------
// ĳ���� ���� ����.
//-----------------------------------------------------------------------------
struct CTAS_CHR_MOVECHK : public CHARSELECT_PACKET_HEADER
{
	BYTE	byType;
		
	CTAS_CHR_MOVECHK()				{ bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_CHR_MOVE_CHKRT;	}
	DWORD	GetPacketSize()			{ return sizeof(CTAS_CHR_MOVECHK); }
};


//-----------------------------------------------------------------------------
// ĳ���� �̵� ���� ��Ŷ 
//-----------------------------------------------------------------------------
struct ASTC_MOVE_CHARACTER_FAIL : public CHARSELECT_PACKET_HEADER
{
	BYTE	byType;
	BYTE	byNum;
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	ASTC_MOVE_CHARACTER_FAIL()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_MOVE_CHARACTER_FAIL;	}
	DWORD	GetPacketSize()		{	return sizeof(ASTC_MOVE_CHARACTER_FAIL);	}
};


//-----------------------------------------------------------------------------
// ĳ���� �̵� ���� ��Ŷ 
//-----------------------------------------------------------------------------
struct ASTC_MOVE_CHARACTER_SUCCESS : public CHARSELECT_PACKET_HEADER
{
	BYTE				byCharNum;
	BYTE				byNum;
	RS_CharacterLogInfo	Character;

	ASTC_MOVE_CHARACTER_SUCCESS()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_MOVE_CHARACTER_SUCCESS;	}
	DWORD	GetPacketSize()			{	return sizeof(ASTC_MOVE_CHARACTER_SUCCESS);	}
};


//-----------------------------------------------------------------------------
// ĳ���� ������.
//-----------------------------------------------------------------------------
struct CTAS_MOVE_CHARACTER_RESEND : public CHARSELECT_PACKET_HEADER
{
	BYTE	byType[4];
	BYTE	byNum[4];
	char	szName[4][MAX_CHARACTER_NAME_LENGTH];

	CTAS_MOVE_CHARACTER_RESEND()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_MOVE_CHARACTER_RESEND;	}
	DWORD	GetPacketSize()			{	return sizeof(CTAS_MOVE_CHARACTER_RESEND);	}
};


#pragma pack(pop)