#pragma once


#include "ProtocolDefinition.h"
#include "CommonGlobalDefine.h"


#pragma pack(push,1)


//-----------------------------------------------------------------------------
// �����̳� ���������� ���弭���� �����Ҷ� ���� ��Ŷ 
//-----------------------------------------------------------------------------
struct DSTWS_SERVER_ATTACH
{
	WORD	wHeader;
	DWORD	dwID;		
	BYTE	bType;

	DSTWS_SERVER_ATTACH()	{	wHeader = Protocol_Server::CMD_SERVER_ATTACH;		}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_SERVER_ATTACH);	}
};


//-----------------------------------------------------------------------------
// ���弭������ �ٸ� ��� ������ �����϶�� �޼����� ���� 
//-----------------------------------------------------------------------------
struct WSTS_SERVER_TERMINATE
{
	WORD	wHeader;
	
	WSTS_SERVER_TERMINATE()	{	wHeader = Protocol_Server::CMD_SERVER_TERMINATE;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTS_SERVER_TERMINATE);	}
};


//-----------------------------------------------------------------------------
// ���� or ���� �������� ���� ������ ����϶�� �޼����� ������. 
//-----------------------------------------------------------------------------
struct WSTDS_ADD_USER_ACCEPT_ALLOWED
{
	WORD	wHeader;
	WORD	wDungeonID;		// �����ϰ��� �ϴ� �ش� �����̳� ������ ID	
	BYTE	bType;			// User Accept�� �䱸�ϴ� Ÿ��, (��) ��������, ��Ż�̵����� ���� �̵�.. ��� 
	DWORD	dwUserIndex;	// ĳ���� ������ �ε��� 
	BYTE	bUserType;
	
	WSTDS_ADD_USER_ACCEPT_ALLOWED()	{	wHeader = Protocol_Server::CMD_ADD_USER_ACCEPT_ALLOWED;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_ADD_USER_ACCEPT_ALLOWED);	}
};


//-----------------------------------------------------------------------------
// ���弭������ �ش������� ������ ���Ǿ��ٰ� �˸� 
//-----------------------------------------------------------------------------
struct DSTWS_USER_ACCEPT_ALLOWED
{
	WORD	wHeader;
	BYTE	bServerType;	// ���� Ÿ�� 
	WORD	wDungeonID;		// ������ ����� ���� ID	
	BYTE	bType;			// User Accept�� �䱸�ϴ� Ÿ��, (��) ��������, ��Ż�̵����� ���� �̵�.. ��� 
	DWORD	dwUserIndex;
	
	DSTWS_USER_ACCEPT_ALLOWED()	{	wHeader = Protocol_Server::CMD_USER_ACCEPT_ALLOWED;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_USER_ACCEPT_ALLOWED);	}
};


//-----------------------------------------------------------------------------
// ���弭���� ���������� ���� ������ ��ġ�� ���� ������ ������ ����������� ���弭���� �ش� ������������ ���� ���̺��� �� ������ �����϶�� �޼���
//-----------------------------------------------------------------------------
struct WSTDS_REMOVE_USER_ACCEPT_ALLOWED
{
	WORD	wHeader;
	WORD	wDungeonID;		//�����ϰ��� �ϴ� �ش� ���� ID	
	DWORD	dwUserIndex;	
	BYTE	m_bUserType;	// USER_TYPE_GM, USER_TYPE_NORMAL
							// GM ����� �������� �ٷ� ������ �ý����̶� �� �Ӽ��� �߰��Ѵ�.
	
	WSTDS_REMOVE_USER_ACCEPT_ALLOWED()	{	wHeader = Protocol_Server::CMD_REMOVE_USER_ACCEPT_ALLOWED;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_REMOVE_USER_ACCEPT_ALLOWED);	}
};


//-----------------------------------------------------------------------------
// ���弭������ �ش������� ������ ���Ǿ��ٰ� �˸� 
//-----------------------------------------------------------------------------
struct DSTWS_USER_ACCEPT_FAILED
{
	WORD	wHeader;
	BYTE	bServerType;	// ���� Ÿ�� 
	WORD	wDungeonID;		// ������ ����� ���� ID	
	BYTE	bType;			// User Accept�� �䱸�ϴ� Ÿ��, (��) ��������, ��Ż�̵����� ���� �̵�.. ��� 
	DWORD	dwUserIndex;
	
	DSTWS_USER_ACCEPT_FAILED()	{	wHeader = Protocol_Server::CMD_USER_ACCEPT_FAILED;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_USER_ACCEPT_FAILED);	}
};


//-----------------------------------------------------------------------------
// ���弭������ ������ ��ġ �Ǿ��ٰ� �˸�.
//-----------------------------------------------------------------------------
struct DSTWS_DUNGEON_MAGICARRAY_ATTACH
{
	WORD	wHeader;
	BYTE	bServerType;	//���� Ÿ��
	WORD	wDungeonID;
	CItem	cItem;			// ������

	DSTWS_DUNGEON_MAGICARRAY_ATTACH()	{	wHeader = Protocol_Server::CMD_DUNGEON_MAGICARRAY_ATTACH;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_DUNGEON_MAGICARRAY_ATTACH);	}	
};


// ������ ������ �ð��϶� ���弭���� ������������ �����ش�.
struct WSTS_START_SIEGE
{
	WORD	wHeader;
	WORD	wDungeonID;
	
	WSTS_START_SIEGE()	{	wHeader = Protocol_Server::CMD_START_SIEGE;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTS_START_SIEGE);	}
};


// ������ ���� �ð��϶� ���弭���� ������������ �����ش�.
struct WSTS_END_SIEGE
{
	WORD	wHeader;
	WORD	wDungeonID;

	WSTS_END_SIEGE()	{	wHeader = Protocol_Server::CMD_END_SIEGE;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTS_END_SIEGE);	}
};


//-----------------------------------------------------------------------------
// ���� �������� ���������� ��Ƽ ���̵� ������.
//-----------------------------------------------------------------------------
struct WSTDS_PARTY_MSG
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwPartyId;
	
	WSTDS_PARTY_MSG()	{	wHeader = Protocol_Server::CMD_PARTY_MSG;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_PARTY_MSG);	}
};


// ���� ������ �ٲ���.
struct DSTWS_DUNGEON_OWNER_CHANGE
{
	WORD	wHeader;
	WORD	wDungeonID;				
	DWORD	dwOwnerIndex;
	DWORD	dwGuildId;
	char	szUserLoginID[MAX_CHARACTER_NAME_LENGTH ];
	char	szCharacterName[ MAX_CHARACTER_NAME_LENGTH ];

	DSTWS_DUNGEON_OWNER_CHANGE()	{	wHeader = Protocol_Server::CMD_DUNGEON_OWNER_CHANGE;	memset(szUserLoginID, 0, sizeof(szUserLoginID));	memset(szCharacterName, 0, sizeof(szCharacterName));	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_DUNGEON_OWNER_CHANGE);	}
};


struct DSTWS_DUNGEON_GUARDIANITEM_ATTACH
{
	WORD	wHeader;
	BYTE	bServerType;	//���� Ÿ��
	WORD	wDungeonID;
	CItem	cItem;			// ������

	DSTWS_DUNGEON_GUARDIANITEM_ATTACH()	{	wHeader = Protocol_Server::CMD_DUNGEON_GUARDIANITEM_ATTACH;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_DUNGEON_GUARDIANITEM_ATTACH);	}	
};


// ���忡 ���� �پ����� ���� ������ �������.
struct WSTS_DUNGEON_INFO
{
	WORD	wHeader;
	WORD	wDungeonID;
	BYTE	bSiege;		
	WORD	wUpgradeLevel;	// ���� ���׷��̵�� ����

	WSTS_DUNGEON_INFO()	{	memset(this, 0, sizeof(this)); wHeader = Protocol_Server::CMD_DUNGEON_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTS_DUNGEON_INFO);	}
};


// ��Ƽ���� �˾ƿ�
struct DSTWS_DUNGEON_GROUPEXP
{
	WORD	wHeader;
	BYTE	bType;			// 1 : party	2 : guild
	DWORD	dwExp;
	DWORD	dwMonsterLevel;
	DWORD	dwOwnUserIndex;

	DSTWS_DUNGEON_GROUPEXP()	{	wHeader = Protocol_Server::CMD_DUNGEON_GROUPEXP;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_DUNGEON_GROUPEXP);	}
};


// ���� ������
struct WSTS_DUNGEON_LEVELUP
{
	WORD	wHeader;
	WORD	wDungeonID;

	WSTS_DUNGEON_LEVELUP()	{	wHeader = Protocol_Server::CMD_DUNGEON_LEVELUP;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTS_DUNGEON_LEVELUP);	}
};


//-----------------------------------------------------------------------------
// ���� ������ ������������ Data�� Reload �϶�� ��Ŷ�� ���� 
//-----------------------------------------------------------------------------
struct WSTS_REFRESH_ALL_DUNGEON_SERVER_DATA
{
	WORD	wHeader;

	WSTS_REFRESH_ALL_DUNGEON_SERVER_DATA()		{	wHeader = Protocol_Server::CMD_REFRESH_ALL_DS_DATA;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTS_REFRESH_ALL_DUNGEON_SERVER_DATA);	}
};


// ���ɴ����� ������ ��尡 �ٲ���.
struct WSTDS_REFRESH_DUNGEON_OWNER_GUILD
{
	WORD	wHeader;	
	WORD	wDungeonID;
	DWORD	dwGuildID;

	WSTDS_REFRESH_DUNGEON_OWNER_GUILD()	{	wHeader = Protocol_Server::CMD_REFRESH_DUNGEON_OWNER_GUILD;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_REFRESH_DUNGEON_OWNER_GUILD);	}
};


// ������ ����� ���������� 
struct DSTWS_GOTOWORLD
{
	WORD	wHeader;
	DWORD	dwUserIndex;
	DWORD	dwMoney;
	short	wCriminalTime;
	WORD	wToDungeonID;		//����ʿ��� � Dungeon ������ �������ΰ� �� ���� ID
	WORD	wNowDungeonID;		//��������� ������ ���� ���� �ִ� ���� ���̵�  

	DSTWS_GOTOWORLD()	{	wHeader = Protocol_Server::CMD_GOTOWORLD;		}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_GOTOWORLD);	}
};


//-----------------------------------------------------------------------------
// ���������� �ٸ� ������������ User Accept������ ��û�Ѵ�. (DS <-> WS <-> DS)
//-----------------------------------------------------------------------------
struct DSWDS_REQUEST_USER_ACCEPT_FOR_PORTAL
{
	WORD	wHeader;
	WORD	wFromDungeonID;
	WORD	wDungeonID;		
	BYTE	bLayerIndex;		// append by seven for GM 
	DWORD	dwTargetUserIndex;	// append by seven for GM [12/10/2004]
	BYTE	bPortalType;		//enum PORTAL_TYPE ���� 
								// append by seven [12/20/2004] 

	DWORD	dwUserIndex;
	DWORD	dwUserMoney;
	DWORD	dwUserLevel;
	BYTE	bConnectionToGameRoom;

	DSWDS_REQUEST_USER_ACCEPT_FOR_PORTAL()	
	{	
		wHeader = Protocol_Server::CMD_REQUEST_USER_ACCEPT_FOR_PORTAL;	
		dwTargetUserIndex = 0;
	}
	DWORD	GetPacketSize()	{	return sizeof(DSWDS_REQUEST_USER_ACCEPT_FOR_PORTAL);	}
};


//-----------------------------------------------------------------------------
// ��Ż�� User Accept Fail �� 
//-----------------------------------------------------------------------------
struct DSWDS_USER_ACCEPT_FAILED_FOR_PORTAL
{
	WORD	wHeader;
	WORD	wFromDungeonID;
	DWORD	dwUserIndex;
	BYTE	bErrorCode;

	DSWDS_USER_ACCEPT_FAILED_FOR_PORTAL()	{	wHeader = Protocol_Server::CMD_USER_ACCEPT_FAILED_FOR_PORTAL;	}
	DWORD	GetPacketSize()	{	return sizeof(DSWDS_USER_ACCEPT_FAILED_FOR_PORTAL);	}
};


//-----------------------------------------------------------------------------
// �ش������� ������ ���Ǿ��ٰ� �˸� 
//-----------------------------------------------------------------------------
struct DSWDS_USER_ACCEPT_ALLOWED_FOR_PORTAL
{
	WORD	wHeader;
	WORD	wFromDungeonID;
	DWORD	dwUserIndex;
	DWORD	dwConnectIP;	//�����ؾ��� IP�� 
	WORD	wPort;			//Port�� ������ 

	// GM ��ɽ� �̵��� ������ ��ġ�� �𸣱⶧���� ������ ������ �ȵǾ� ������ ��ġ������ �˾ƿ´�.
	BYTE	bLayerIndex;
	VECTOR2	v2Pos;	
	
	DSWDS_USER_ACCEPT_ALLOWED_FOR_PORTAL()	
	{	
		wHeader = Protocol_Server::CMD_USER_ACCEPT_ALLOWED_FOR_PORTAL;	
		bLayerIndex = 0;
		v2Pos.x	= v2Pos.y = 0.f;
	}
	DWORD	GetPacketSize()	{	return sizeof(DSWDS_USER_ACCEPT_ALLOWED_FOR_PORTAL);	}
};


//-----------------------------------------------------------------------------
// ������������ ���弭���� ��Ƽ�� ������ ������.
//-----------------------------------------------------------------------------
struct WSTDS_PARTY_LEVEL
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwPartyId;
	DWORD	dwLevel;
	
	WSTDS_PARTY_LEVEL()	{	wHeader = Protocol_Server::CMD_PARTY_LEVEL;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_PARTY_LEVEL);	}
};


//-----------------------------------------------------------------------------
// ������������ ���弭���� ��Ƽ�� ������ ������.
//-----------------------------------------------------------------------------
struct WSTDS_PARTY_INFO
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwUserIndexRv;	
	char	szCharName[MAX_CHARACTER_NAME_LENGTH];
	
	WSTDS_PARTY_INFO()	{	wHeader = Protocol_Server::CMD_PARTY_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_PARTY_INFO);	}
};


//-----------------------------------------------------------------------------
// ������������ ���弭���� ��Ƽ�� ������ ������.
//-----------------------------------------------------------------------------
struct DSTWS_PARTY_INFO
{
	WORD	wHeader;	
	WORD	wDungeonID;
	BYTE	byLayerIndex;	
	DWORD	dwUserIndex;
	DWORD	dwUserIndexRv;	
	char	szCharName[MAX_CHARACTER_NAME_LENGTH];
	
	DSTWS_PARTY_INFO()	{	wHeader = Protocol_Server::CMD_PARTY_INFORT;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_PARTY_INFO);	}
};


//-----------------------------------------------------------------------------
// ������������ ���弭���� ��Ƽ�� ������ ������.
//-----------------------------------------------------------------------------
struct WSTDS_PARTY_HP
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwPartyId;	
	WORD	wHP;
	WORD	wMaxHp;
	
	WSTDS_PARTY_HP()	{	wHeader = Protocol_Server::CMD_PARTY_HP;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_PARTY_HP);	}
};


//-----------------------------------------------------------------------------
// ������������ ���弭���� ��Ż �޼���.
//-----------------------------------------------------------------------------
struct WSTDS_PARTY_PORTAL
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwPartalUserIndex;
	DWORD	dwPartyId;
	BYTE	byIndex;
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	
	WSTDS_PARTY_PORTAL()	{	wHeader = Protocol_Server::CMD_PARTY_PORTAL;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_PARTY_PORTAL);	}
};

struct WSTDS_PARTY_PORTALUSED
{
	WORD	wHeader;	
	DWORD	dwPartyId;
	DWORD	dwUserIndex;
	DWORD	dwPartalUserIndex;
	BYTE	dwResult;
	BYTE	byIndex;
	BYTE	byType;
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	WSTDS_PARTY_PORTALUSED()	{	wHeader = Protocol_Server::CMD_PARTY_PORTALUSED;	}
	DWORD	GetPacketSize()		{	return (sizeof(WSTDS_PARTY_PORTALUSED)); }
};


struct WSTDS_PARTY_PORTALMOVE
{
	WORD	wHeader;		
	WORD	wDungeonID;
	BYTE	byLayerIndex;
	float	fPosX;
	float	fPosZ;
	DWORD	dwPartyId;
	DWORD	dwUserIndex;
	DWORD	dwPortalUserIndex;

	WSTDS_PARTY_PORTALMOVE()	{ 	wHeader = Protocol_Server::CMD_PARTY_PORTALMOVE;}
	DWORD	GetPacketSize()		{	return (sizeof(WSTDS_PARTY_PORTALMOVE)); }
};


// ��Ƽ ���� ����ȭ : �ִ��� 2005.1.25
struct WSTDS_PARTY_CONFIG
{
	WORD wHeader;
	DWORD dwUserIndex;
	PARTY_CONFIG pConfig;
	BYTE bMsg;

	WSTDS_PARTY_CONFIG()	{ 	wHeader = Protocol_Server::CMD_PARTY_CONFIG;}
	DWORD	GetPacketSize()		{	return (sizeof(WSTDS_PARTY_CONFIG)); }
};



// ���� ���尡 �ٲ����.
struct DSTWS_DUNGEON_CHANGE_OPERATION_TYPE
{
	WORD	wHeader;
	BYTE	bOperationType;
	WORD	wDungeonID;

	DSTWS_DUNGEON_CHANGE_OPERATION_TYPE()	{	wHeader = Protocol_Server::CMD_DUNGEON_CHANGE_OPERATION_TYPE;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_DUNGEON_CHANGE_OPERATION_TYPE);	}
};


//-----------------------------------------------------------------------------
// ���弭������ ������ ���� Join�� �����ߴٴ� �޼����� ���� 
//-----------------------------------------------------------------------------
struct DSTWS_DUNGEON_JOIN_SUCCESS
{
	WORD	wHeader;
	DWORD	dwChar_Index;
	WORD	wDungeonID;

	DSTWS_DUNGEON_JOIN_SUCCESS()	{	wHeader = Protocol_Server::CMD_DUNGEON_JOIN_SUCCESS;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_DUNGEON_JOIN_SUCCESS);	}
};


// ����ǰ �����϶�.
struct DSWST_CREATEPRODUCTION
{
	WORD	wHeader;
	WORD	wDungeonID;
	WORD	wProduction;
	BYTE	byProductionCount;
	
	DSWST_CREATEPRODUCTION()	{	wHeader = Protocol_Server::CMD_CREATEPRODUCTION;	}
	DWORD	GetPacketSize()	{	return sizeof(DSWST_CREATEPRODUCTION);	}
};


// ����ǰ ȹ���ߵ�.
struct DSTWS_REFRESH_PRODUCT_ACQUISITION
{
	WORD	wHeader;
	WORD	wDungeonID;
	WORD	wProduction;

	DSTWS_REFRESH_PRODUCT_ACQUISITION()	{	wHeader = Protocol_Server::CMD_REFRESH_PRODUCT_ACQUISITION;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_REFRESH_PRODUCT_ACQUISITION);	}
};


// ����ġ ȹ���ߵ�.
struct DSTWS_REFRESH_ACCEXP_ACQUISITION
{
	WORD	wHeader;
	WORD	wDungeonID;
	DWORD	dwAccExp;

	DSTWS_REFRESH_ACCEXP_ACQUISITION()	{	wHeader = Protocol_Server::CMD_REFRESH_ACCEXP_ACQUISITION;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_REFRESH_ACCEXP_ACQUISITION);	}
};


// ����� �����ߵ�.
struct DSTWS_REFRESH_ENTRANCE_EDIT
{
	WORD	wHeader;
	WORD	wDungeonID;
	int		nMoney;

	DSTWS_REFRESH_ENTRANCE_EDIT()	{	wHeader = Protocol_Server::CMD_REFRESH_ENTRANCE_EDIT;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_REFRESH_ENTRANCE_EDIT);	}
};


// ����� ȹ���ߴ�.
struct DSTWS_REFRESH_ACCENTRANCE_ACQUISITION
{
	WORD	wHeader;
	WORD	wDungeonID;
	int		nMoney;

	DSTWS_REFRESH_ACCENTRANCE_ACQUISITION()	{	wHeader = Protocol_Server::CMD_REFRESH_ACCENTRANCE_ACQUISITION;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_REFRESH_ACCENTRANCE_ACQUISITION);	}
};


struct WSTDS_PARTY_ALLPORTAL
{
	WORD	wHeader;
	DWORD	dwUserIndex;	
	DWORD	dwPartyId;	
	BYTE	byIndex;
	
	WSTDS_PARTY_ALLPORTAL()	{	wHeader = Protocol_Server::CMD_PARTY_ALLPORTAL;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_PARTY_ALLPORTAL);	}
};


//-----------------------------------------------------------------------------
// ���� ������ ������ �϶�� ����� ������.
//-----------------------------------------------------------------------------
struct WSTDS_EVENT_DUNGEON_INIT
{
	WORD	wHeader;
	WORD	wDungeonID;
	BOOL	bTimeUp;		// �ð��� ����Ǿ� �������ϴ� ���ΰ�?

	WSTDS_EVENT_DUNGEON_INIT()	{ wHeader = Protocol_Server::CMD_EVENT_DUNGEON_INIT; }
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_EVENT_DUNGEON_INIT);	}
};


//-----------------------------------------------------------------------------
// ���弭���� �̺�Ʈ ������ ������ ���� ������ �˸���.
//-----------------------------------------------------------------------------
struct DSTWS_EVENT_DUNGEON_USER_OUT
{
	WORD	wHeader;
	WORD	wDungeonID;		// �̺�Ʈ ���� ���̵�.

	DSTWS_EVENT_DUNGEON_USER_OUT()	{ wHeader = Protocol_Server::CMD_EVENT_DUNGEON_USER_OUT; }
	DWORD	GetPacketSize()			{ return sizeof(DSTWS_EVENT_DUNGEON_USER_OUT);	}
};


//-----------------------------------------------------------------------------
// Desc: ���� ������ �پ��� ��, �̺�Ʈ ������ ���ԵǾ� �ִٸ� �̺�Ʈ ���� ������ �����ϵ��� �Ѵ�.
//-----------------------------------------------------------------------------
struct WSTDS_EVENT_DUNGEON_SET
{
	WORD	wHeader;
	WORD	wDungeonID;
	BYTE	m_byEventType;		// �̺�Ʈ ���� Ÿ��.	

	WSTDS_EVENT_DUNGEON_SET()	{ wHeader = Protocol_Server::CMD_EVENT_DUNGEON_SET; }
	DWORD	GetPacketSize()		{ return sizeof(WSTDS_EVENT_DUNGEON_SET); }
};


//-----------------------------------------------------------------------------
// �̺�Ʈ ������ ������ ���� Ŭ���� �Ǿ���.
//-----------------------------------------------------------------------------
struct DSTWS_EVENT_DUNGEON_CLEAR
{
	WORD	wHeader;
	WORD	wDungeonID;

	DSTWS_EVENT_DUNGEON_CLEAR()	{ wHeader = Protocol_Server::CMD_EVENT_DUNGEON_CLEAR; }
	DWORD	GetPacketSize()		{ return sizeof(DSTWS_EVENT_DUNGEON_CLEAR); }
};


//-----------------------------------------------------------------------------
// ���弭������ ���������� ��������� ������.
//-----------------------------------------------------------------------------
struct WSTDS_GUILD_INFO
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwGuildId;	
	BYTE	byType;
	BYTE	byRank;
	char	szGuildName[20];
	char	szNickName[20];	
	
	WSTDS_GUILD_INFO()		{	wHeader = Protocol_Server::CMD_GUILD_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_GUILD_INFO);	}
};


//-----------------------------------------------------------------------------
// ���弭������ ���������� ������ ���������� ������.
//-----------------------------------------------------------------------------
struct WSTDS_GUILD_FAIL
{
	WORD	wHeader;	
	DWORD	dwUserIndex;		
	BYTE	byType;	
	
	WSTDS_GUILD_FAIL()		{	wHeader = Protocol_Server::CMD_GUILD_FAIL;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_GUILD_FAIL);	}
};


struct WSTDS_PARTY_PORTALFAIL
{
	WORD	wHeader;
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	BYTE	byResult;
	DWORD	dwUserIndex;

	WSTDS_PARTY_PORTALFAIL(){	wHeader = Protocol_Server::CMD_PARTY_PORTALFAIL;	}
	DWORD	GetPacketSize()	{	return (sizeof(WSTDS_PARTY_PORTALFAIL));			}
};


//-----------------------------------------------------------------------------
// World ������ Dungeon �������� �ش� User�� ���� Logout �϶�� ���� 
//-----------------------------------------------------------------------------
struct WSTDS_KICK_USER
{
	WORD	wHeader;
	DWORD	dwCharIndex;

	WSTDS_KICK_USER()	{	wHeader = Protocol_Server::CMD_KICK_USER;		}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_KICK_USER);	}
};


//-----------------------------------------------------------------------------
// ���� �޽����� �����ÿ�.
//-----------------------------------------------------------------------------
struct WSTDS_NOTICE_ACTION
{
	WORD	wHeader;
	WORD	wMsgLen;
	char	szMessage[1024];

	WSTDS_NOTICE_ACTION()	{ wHeader = Protocol_Server::CMD_NOTICE_ACTION; }
	DWORD	GetPacketSize()		{  return 4 + wMsgLen; }
};


//-----------------------------------------------------------------------------
// ������ Ŭ���̾�Ʈ���� �ѷ��� �޽����� ������.
//-----------------------------------------------------------------------------
struct WSTDS_DUNGEON_MESSAGE
{
	WORD	wHeader;
	BYTE	byType;					// Ÿ��. Ȯ���ؼ� �˾Ƽ� ����Ͻñ�.
	DWORD	dwMessageColor;			// �޽����� �÷�.
	WORD	wMsgLen;
	char	szMessage[1024];	

	WSTDS_DUNGEON_MESSAGE()		{ wHeader = Protocol_Server::CMD_DUNGEON_MESSAGE; }
	DWORD	GetPacketSize()		{ return 9 + wMsgLen; }
};


//-----------------------------------------------------------------------------
// ���������� ����ִ��� �˻��Ѵ�.
//-----------------------------------------------------------------------------
struct WSTDS_REQUEST_DUNGEON_ALIVE
{
	WORD	wHeader;
	WORD	wType;					// ��û ���� Ÿ��. �ϴ� ������ ������ �ʴ´�.

	WSTDS_REQUEST_DUNGEON_ALIVE()	{ wHeader = Protocol_Server::CMD_REQUEST_DUNGEON_ALIVE; }
	DWORD	GetPacketSize()			{ return sizeof(WSTDS_REQUEST_DUNGEON_ALIVE); }
};


//-----------------------------------------------------------------------------
// ���� ������ ���弭������ ��� �ִٴ� �޽����� ������.
//-----------------------------------------------------------------------------
struct DSTWS_DUNGEON_ALIVE_INFO
{
	WORD	wHeader;
	DWORD	dwServerID;					// ���� ID
	DWORD	dwConnectedUserCount;		// �ش� ���� ������ �پ� �ִ� ���� ��.

	DSTWS_DUNGEON_ALIVE_INFO()			{ wHeader = Protocol_Server::CMD_DUNGEON_ALIVE_IFNO; }
	DWORD	GetPacketSize()				{ return sizeof(DSTWS_DUNGEON_ALIVE_INFO);			}
};


//-----------------------------------------------------------------------------
// ���� �������� ���������� ���޼����� ������.
//-----------------------------------------------------------------------------
struct WSTDS_GUILD_MSG
{
	WORD	wHeader;	
	DWORD	dwUserIndex;	
	
	WSTDS_GUILD_MSG()	{	wHeader = Protocol_Server::CMD_GUILD_MSG;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_GUILD_MSG);	}
};


//-----------------------------------------------------------------------------
// ������������ ���弭���� ��Ƽ�� ������ ������.
//-----------------------------------------------------------------------------
struct WSTDS_GUILDUSER_INFO
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwUserIndexRv;
	
	WSTDS_GUILDUSER_INFO()	{	wHeader = Protocol_Server::CMD_GUILDUSER_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_GUILDUSER_INFO);	}
};


//-----------------------------------------------------------------------------
// ������������ ���弭���� ���� ������ ������.
//-----------------------------------------------------------------------------
struct DSTWS_GUILDUSER_INFO
{
	WORD	wHeader;	
	WORD	wDungeonID;
	BYTE	byLayerIndex;	
	DWORD	dwUserIndex;
	DWORD	dwUserIndexRv;
	
	DSTWS_GUILDUSER_INFO()	{	wHeader = Protocol_Server::CMD_GUILDUSER_INFORT;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_GUILDUSER_INFO);	}
};


struct WSTDS_GUILDINFO_MSG
{
	WORD	wHeader;	
	DWORD	dwUserIndex;	
	DWORD	dwGuildId;	
	BYTE	byRank;
	char	szNickName[MAX_CHARACTER_NAME_LENGTH];
	
	WSTDS_GUILDINFO_MSG()	{	wHeader = Protocol_Server::CMD_GUILDINFO_MSG;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_GUILDINFO_MSG);	}
};


struct WSTDS_GUILD_LEVEL
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwGuildId;
	DWORD	dwLevel;
	
	WSTDS_GUILD_LEVEL()	{	wHeader = Protocol_Server::CMD_GUILD_LEVEL;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_GUILD_LEVEL);	}
};


struct WSTDS_CHARACTER_SEARCH
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwSearchUserIndex;
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	
	WSTDS_CHARACTER_SEARCH()	{	wHeader = Protocol_Server::CMD_CHARACTER_SEARCH;	}
	DWORD	GetPacketSize()		{	return sizeof(WSTDS_CHARACTER_SEARCH);	}
};


struct WSTDS_SEARCH
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwSearchUserIndex;
	char	szName[MAX_CHARACTER_NAME_LENGTH];		
	BYTE	byMsg;

	WSTDS_SEARCH()				{	wHeader = Protocol_Server::CMD_CHARACTER_SEARCHRT;	}
	DWORD	GetPacketSize()		{	return sizeof(WSTDS_SEARCH);	}
};


//-----------------------------------------------------------------------------
// GM_COMMAND
//-----------------------------------------------------------------------------
struct WSTDS_GM_COMMAND
{
	WORD	wHeader;

	WSTDS_GM_COMMAND()		{ wHeader = Protocol_Server::CMD_GM_COMMAND_WS_DS;	}
	DWORD	GetPacketSize()	{ return sizeof( WSTDS_GM_COMMAND );	}
};


struct DSTWS_GM_COMMAND
{
	WORD	wHeader;

	DSTWS_GM_COMMAND()		{ wHeader = Protocol_Server::CMD_GM_COMMAND_DS_WS;	}
	DWORD	GetPacketSize()	{ return sizeof( DSTWS_GM_COMMAND );	}
};


//-----------------------------------------------------------------------------
// ������������ ���弭���� ģ�� ������ ������.
//-----------------------------------------------------------------------------
struct WSTDS_FRIENDUSER_INFO
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwUserIndexRv;
	
	WSTDS_FRIENDUSER_INFO()	{	wHeader = Protocol_Server::CMD_FRIENDUSER_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_FRIENDUSER_INFO);	}
};


//-----------------------------------------------------------------------------
// ������������ ���弭���� ģ�� ������ ������.
//-----------------------------------------------------------------------------
struct DSTWS_FRIENDUSER_INFO
{
	WORD	wHeader;	
	WORD	wDungeonID;
	BYTE	byLayerIndex;	
	DWORD	dwUserIndex;
	DWORD	dwUserIndexRv;
	
	DSTWS_FRIENDUSER_INFO()	{	wHeader = Protocol_Server::CMD_FRIENDUSER_INFORT;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_FRIENDUSER_INFO);	}
};


//-----------------------------------------------------------------------------
// ���� ������ ���� �������� �̺�Ʈ ���� ���µ� ������ �����ش�.�̴ϸ��� �������� ���� ����� ��ġ�� ǥ���ϱ� ���� ����.
//-----------------------------------------------------------------------------
struct WSTDS_EVENT_DUNGEON_OPEN_INFO
{
	WORD	wHeader;
	WORD	wDungeonID;								// ���� ID
	char	szDungeonName[MAX_DUNGEON_NAME_LENGTH];	// ���� �̸�
	BYTE	byEventState;							// �̺�Ʈ ���� ����.
	DWORD	dwEventEndTick;							// �̺�Ʈ ���� ���� �ð�

	WSTDS_EVENT_DUNGEON_OPEN_INFO()		{ wHeader = Protocol_Server::CMD_EVENT_DUNGEON_OPEN_INFO; }
	DWORD	GetPacketSize()				{ return sizeof(WSTDS_EVENT_DUNGEON_OPEN_INFO); }
};

struct WSTDS_USER_PORTAL
{
	WORD	wHeader;

	BYTE	byGMCommand;			// enum GM_COMMAND ���� 
	DWORD	dwUserIndex;			// �̵��� �� 
	DWORD	dwTargetToUserIndex;	// ���������� ������ �� 
	
	DUNGEON_MOVE_INFO DMI;// : 050129 hwoarang 

	WSTDS_USER_PORTAL()		
	{	
		wHeader = Protocol_Server::CMD_USER_PORTAL;	
		dwTargetToUserIndex = 0;
	}

	DWORD	GetPacketSize()	{	return sizeof(WSTDS_USER_PORTAL);	}
};

//-----------------------------------------------------------------------------
// Event�� PC�� ���� 2�� ����ġ Enable ���������� Broadcasting �ϴ� ��Ŷ 
//-----------------------------------------------------------------------------
struct WSTDS_SET_DOUBLE_EXP
{
	WORD wHeader;
	BYTE bEnable;

	WSTDS_SET_DOUBLE_EXP()	{	wHeader = Protocol_Server::CMD_SET_DOUBLE_EXP;		}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_SET_DOUBLE_EXP);	}
};


//-----------------------------------------------------------------------------
// Event�� ���� ������ �巴���� �������.
//-----------------------------------------------------------------------------
struct WSTDS_SET_MAGICITEM_DROPRATE
{
	WORD wHeader;
	WORD wDropRate;

	WSTDS_SET_MAGICITEM_DROPRATE()	{	wHeader = Protocol_Server::CMD_SET_MAGICITEM_DROPRATE;		}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_SET_MAGICITEM_DROPRATE);	}
};


//-----------------------------------------------------------------------------
// Event�� EXP
//-----------------------------------------------------------------------------
struct WSTDS_SET_EXPGAIN
{
	WORD wHeader;
	WORD wEXP;

	WSTDS_SET_EXPGAIN()	{	wHeader = Protocol_Server::CMD_SET_EXPGAIN;		}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_SET_EXPGAIN);	}
};


//-----------------------------------------------------------------------------
// ����� ����Ʈ�� ������.
//-----------------------------------------------------------------------------
struct WSTDS_GUILD_LIST
{
	WORD	wHeader;	
	DWORD	dwGuildId;
	DWORD	dwDstGuildId;
	
	WSTDS_GUILD_LIST()		{	wHeader = Protocol_Server::CMD_GUILD_LIST;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_GUILD_LIST);	}
};


struct WSTDS_GUILD_ALLPORTAL
{
	WORD	wHeader;	
	DWORD	dwUserIndex;	
	DWORD	dwGuildId;	
	BYTE	byIndex;
	
	WSTDS_GUILD_ALLPORTAL()	{	wHeader = Protocol_Server::CMD_GUILD_ALLPORTAL;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_GUILD_ALLPORTAL);	}
};


struct WSTDS_GUILD_PORTALUSED
{
	WORD	wHeader;	
	DWORD	dwGuildId;
	DWORD	dwUserIndex;
	DWORD	dwPartalUserIndex;
	BYTE	dwResult;
	BYTE	byIndex;
	BYTE	byType;
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	WSTDS_GUILD_PORTALUSED()	{	wHeader = Protocol_Server::CMD_GUILD_PORTALUSED;	}
	DWORD	GetPacketSize()		{	return (sizeof(WSTDS_GUILD_PORTALUSED)); }
};


struct WSTDS_GUILD_PORTALFAIL
{
	WORD	wHeader;		
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	BYTE	byResult;
	DWORD	dwUserIndex;

	WSTDS_GUILD_PORTALFAIL(){	wHeader = Protocol_Server::CMD_GUILD_PORTALFAIL;	}
	DWORD	GetPacketSize()	{	return (sizeof(WSTDS_GUILD_PORTALFAIL));			}
};


struct DSTWS_GUILDWAR_POINT
{
	WORD	wHeader;		
	DWORD	dwGuildId;
	DWORD	dwGuildIdDst;
	BYTE	byPoint;
	DWORD	dwUserIndex;
	WORD	wDungeonID;
	BYTE	byLayerIndex;
	DWORD	dwUserIndexAttack;
	DWORD	dwUserIndexDefence;

	DSTWS_GUILDWAR_POINT()	{	wHeader = Protocol_Server::CMD_GULIDWAR_POINT;	}
	DWORD	GetPacketSize()	{	return (sizeof(DSTWS_GUILDWAR_POINT));			}
};


struct WSTDS_GUILDWAR_DELETE
{
	WORD	wHeader;		
	DWORD	dwGuildId;
	DWORD	dwGuildIdDst;	
	
	WSTDS_GUILDWAR_DELETE()	{	wHeader = Protocol_Server::CMD_GULIDWAR_DELETE;	}
	DWORD	GetPacketSize()	{	return (sizeof(WSTDS_GUILDWAR_DELETE));			}
};


//-----------------------------------------------------------------------------
// ��� ���� 1� ������ ������.
//-----------------------------------------------------------------------------
struct WSTDS_GUILD_RANK
{
	WORD	wHeader;	
	DWORD	dwGuildId;	
	
	WSTDS_GUILD_RANK()		{	wHeader = Protocol_Server::CMD_GUILD_RANK;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_GUILD_RANK);	}
};


struct WSTDS_GUILDUSER_RANK
{
	WORD	wHeader;	
	DWORD	dwGuildId;	
	DWORD	dwUserIndex;
	BYTE	byGuildFlag;	
	BYTE	byChk;
	
	WSTDS_GUILDUSER_RANK()		{	wHeader = Protocol_Server::CMD_GUILDUSER_RANK;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_GUILDUSER_RANK);	}
};


struct WSTS_SERVER_ATTACH_RESULT
{
	WORD	wHeader;	
	BYTE	bAdultMode;
	DWORD	dwEXPGAIN;
	DWORD	dwMagicDropRate;
	DWORD	dwPartyBonusEXPGAIN;
	
	WSTS_SERVER_ATTACH_RESULT()	{	wHeader = Protocol_Server::CMD_SERVER_ATTACH_RESULT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTS_SERVER_ATTACH_RESULT);	}
};


//-----------------------------------------------------------------------------
// �̺�Ʈ ������ ������ ��Ż�� ������ ��, ���弭������ ���� ī��Ʈ�� �� ���� �˸���.
//-----------------------------------------------------------------------------
struct DSTWS_EVENT_DUNGEON_USER_ENTER_BY_PORTAL
{
	WORD	wHeader;	
	WORD	wDungeonID;		// ������ ���̵�.

	DSTWS_EVENT_DUNGEON_USER_ENTER_BY_PORTAL() { wHeader = Protocol_Server::CMD_EVENT_DUNGEON_USER_ENTER_BY_PORTAL;	}
	DWORD	GetPacketSize()	{ return sizeof( DSTWS_EVENT_DUNGEON_USER_ENTER_BY_PORTAL );	}
};


//-----------------------------------------------------------------------------
// ���� ������ ���� �������� �̺�Ʈ ���� 1�� �̺�Ʈ�� ����Ǿ����� �˸���.
//-----------------------------------------------------------------------------
struct WSTDS_EVENT_DUNGEON_FIRST_TIMEOUT
{
	WORD	wHeader;
	WORD	wDungeonID;								// ���� ID
	char	szDungeonName[MAX_DUNGEON_NAME_LENGTH];	// ���� �̸�
	
	WSTDS_EVENT_DUNGEON_FIRST_TIMEOUT()		{ wHeader = Protocol_Server::CMD_EVENT_DUNGEON_FIRST_TIMEOUT; }
	DWORD	GetPacketSize()					{ return sizeof(WSTDS_EVENT_DUNGEON_FIRST_TIMEOUT); }
};


//-----------------------------------------------------------------------------
// ������ ������ ��Ƽ ������ ������.
//-----------------------------------------------------------------------------
struct WSTDS_PARTY_BOARD
{
	WORD	wHeader;
	DWORD	dwPartyId;
	WORD	wDungeonID;

	WSTDS_PARTY_BOARD()			{ wHeader = Protocol_Server::CMD_PARTY_BOARD; }
	DWORD	GetPacketSize()		{ return sizeof(WSTDS_PARTY_BOARD); }
};


//-----------------------------------------------------------------------------
// ���� ������ ���� �������� �̺�Ʈ ���� ���������� ����ɲ���°� �˸���.
//-----------------------------------------------------------------------------
struct WSTDS_EVENT_DUNGEON_CLOSE 
{
	WORD	wHeader;
	WORD	wDungeonID;

	WSTDS_EVENT_DUNGEON_CLOSE()				{ wHeader = Protocol_Server::CMD_EVENT_DUNGEON_CLOSE; }
	DWORD	GetPacketSize()					{ return sizeof(WSTDS_EVENT_DUNGEON_CLOSE); }
};


//-----------------------------------------------------------------------------
// ���� ������ ���� �������� �̺�Ʈ ���� ����� ������ �����ش�.
//-----------------------------------------------------------------------------
struct WSTDS_EVENT_DUNGEON_CLOSE_INFO
{
	WORD	wHeader;
	WORD	wDungeonID;								// ���� ID	
	char	szDungeonName[MAX_DUNGEON_NAME_LENGTH];	// ���� �̸�
	BYTE	byEventState;							// �̺�Ʈ ���� ����.

	WSTDS_EVENT_DUNGEON_CLOSE_INFO()	{ wHeader = Protocol_Server::CMD_EVENT_DUNGEON_CLOSE_INFO; }
	DWORD	GetPacketSize()				{ return sizeof(WSTDS_EVENT_DUNGEON_CLOSE_INFO); }
};


struct WSTDS_GUILDWAR_MESSAGE
{
	WORD	wHeader;
	char	szGuildName[MAX_GUILD_NAME_LENGTH];
	char	szGuildNameDst[MAX_GUILD_NAME_LENGTH];
	BYTE	byType;
	WORD	wDungeonID;

	WSTDS_GUILDWAR_MESSAGE()	{	wHeader = Protocol_Server::CMD_GUILDWAR_MESSAGE;	}
	DWORD	GetPacketSize()		{	return sizeof(WSTDS_GUILDWAR_MESSAGE);				}
};


//-----------------------------------------------------------------------------
// ���弭���� ������������ �̺�Ʈ ������ ������ ������ �ϸ� ����� �ִ��� �˷��ش�.
//-----------------------------------------------------------------------------
struct WSTDS_EVENT_JOINUSER_COUNT
{
	WORD	wHeader;
	WORD	wDungeonID;
	WORD	wJoinUserCount;

	WSTDS_EVENT_JOINUSER_COUNT()			{ wHeader = Protocol_Server::CMD_EVENT_JOINUSER_COUNT; }
	DWORD	GetPacketSize()					{ return sizeof(WSTDS_EVENT_JOINUSER_COUNT); }
};


// ���潺 ī���Ͱ� ���ŵǾ���.
struct DSTWS_REFRESH_DEFENSECOUNT
{
	WORD	wHeader;
	WORD	wDungeonID;
	BYTE	byDefenseCount;

	DSTWS_REFRESH_DEFENSECOUNT()	{	wHeader = Protocol_Server::CMD_REFRESH_DEFENSECOUNT;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_REFRESH_DEFENSECOUNT);	}
};


struct DSTWS_DUNGEON_REFRESH_OPERATIONMODE
{
	WORD	wHeader;
	BYTE	byOperationMode;
	WORD	wDungeonID;

	DSTWS_DUNGEON_REFRESH_OPERATIONMODE()	{	wHeader = Protocol_Server::CMD_REFRESH_DUNGEON_OPERATIONMODE;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_DUNGEON_REFRESH_OPERATIONMODE);	}	
};


struct DSTWS_CHECK_DUNGEONJOIN
{
	WORD	wHeader;	
	WORD	wDungeonID;
	char	szCharacerName[MAX_CHARACTER_NAME_LENGTH];
	DWORD	dwUserIndex;
	DWORD	dwConnectionIndex;
	
	DSTWS_CHECK_DUNGEONJOIN()		{	wHeader = Protocol_Server::CMD_CHECK_DUNGEON_JOIN;	}
	DWORD	GetPacketSize()			{	return sizeof(DSTWS_CHECK_DUNGEONJOIN);	}
};


struct WSTDS_CHECK_DUNGEONJOIN_RT
{
	WORD	wHeader;
	char	szCharacerName[MAX_CHARACTER_NAME_LENGTH];
	DWORD	dwUserIndex;
	BYTE	byChk;
	DWORD	dwConnectionIndex;

	WSTDS_CHECK_DUNGEONJOIN_RT()	{	wHeader = Protocol_Server::CMD_CHECK_DUNGEON_JOIN_RT;	}
	DWORD	GetPacketSize()			{	return sizeof(WSTDS_CHECK_DUNGEONJOIN_RT);	}
};


struct WSTDS_CB_LEVELCHK
{
	WORD	wHeader;	
	DWORD	dwUserIndex;

	WSTDS_CB_LEVELCHK()		{	wHeader = Protocol_Server::CMD_CB_LEVELCHK;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_CB_LEVELCHK);			}
};


//-----------------------
// ���� ��Ƽ ������ 
//-----------------------
struct WSTDS_PARTY_LEADER
{
	WORD	wHeader;
	DWORD	dwUserIndex;
	BYTE	byLeader;
	
	WSTDS_PARTY_LEADER()			{	wHeader = Protocol_Server::CMD_PARTY_LEADER;	}
	DWORD	GetPacketSize()			{	return sizeof(WSTDS_PARTY_LEADER);				}
};


//-----------------------------------------------------------------------------
// Event�� Party Bonus EXP
//-----------------------------------------------------------------------------
struct WSTDS_SET_PARTY_BONUS_EXPGAIN
{
	WORD wHeader;
	WORD wEXP;

	WSTDS_SET_PARTY_BONUS_EXPGAIN()	{	wHeader = Protocol_Server::CMD_SET_PARTY_BONUS_EXP_GAIN;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_SET_PARTY_BONUS_EXPGAIN);	}
};

typedef enum eStadiumState
{
	STADIUM_MODIFY	= 1,//���º���
	STADIUM_END		= 2,//�������
	STADIUM_POINT	= 3,//����
}STADIUM_STATE;

typedef struct tagDSTWS_STADIUM_STATE
{
	WORD	wHeader;
	
	WORD	wType;

	union
	{
		STADIUM_DATA		SD;
		STADIUM_TEAM_POINT	STP;
	}Body;

	bool bIsSuccess;

	tagDSTWS_STADIUM_STATE()	{	wHeader = Protocol_Server::CMD_STADIUM_STATE;	}
	DWORD	GetPacketSize()	
	{
		const size_t base_size = sizeof(*this) - sizeof(Body);
		switch(wType)
		{
		case STADIUM_MODIFY://���º���
		case STADIUM_END://�������
			{
				return base_size + sizeof(STADIUM_DATA) + sizeof(bool);
			}break;
		case STADIUM_POINT://����
			{
				return base_size + sizeof(STADIUM_TEAM_POINT);
			}break;
		}
		return sizeof(*this);	
	}
}DSTWS_STADIUM_STATE;

// ����ġ ����. // : hwoarang 050202
typedef struct tagWSTDS_PREPARE_TEAM_MATCH
{
	FLAG_TYPE	match_type;
	FLAG_TYPE	match_level;
	FLAG_TYPE	player_count;
	FLAG_TYPE	team_count;
	GUILD_WAR_TIME	match_time;

	DWORD		dwExFlag;//Ȯ��Ÿ��
}WSTDS_PREPARE_TEAM_MATCH;

typedef struct tagWSTDS_REG_PLAYER
{
	char		szPlayerName[MAX_CHARACTER_NAME_LENGTH];
	FLAG_TYPE	team_index;

	DWORD		dwExFlag;//Ȯ��Ÿ��
}WSTDS_REG_PLAYER;

typedef enum eWSTDS_TEAM_MATCH
{
	CMD_WSTDS_PREPARE_MATCH = 1,//����� �غ��ض�
	CMD_WSTDS_REG_PLAYER	= 2,//���� �߰�
	CMD_WSTDS_UN_REG_PLAYER = 3,
	CMD_WSTDS_END_MATCH		= 4,//����� ������
}WSTDS_TEAM_MATCH_PACKET_TYPE;

typedef struct tagWSTDS_TEAM_MATCH
{
	WORD		wHeader;

	WORD		wType;

	WORD		wDungeonID;
	BYTE		byLayerIndex;

	union
	{
		WSTDS_PREPARE_TEAM_MATCH	PTM;
		WSTDS_REG_PLAYER			RP;
	}Body;
	
	tagWSTDS_TEAM_MATCH()	{	wHeader = Protocol_Server::CMD_TEAM_MATCH;	}
	DWORD	GetPacketSize()	
	{
		const size_t base_size = sizeof(*this) - sizeof(Body);
		switch( wType)
		{
		case CMD_WSTDS_PREPARE_MATCH:
			{
				return base_size + sizeof(Body.PTM);
			}break;
		case CMD_WSTDS_REG_PLAYER:
			{
				return base_size + sizeof(Body.RP);
			}
		case CMD_WSTDS_END_MATCH:
			{
				return base_size;
			}break;
		} // switch( wType)

		return sizeof(*this);
	}

}WSTDS_TEAM_MATCH;

#pragma pack(pop)