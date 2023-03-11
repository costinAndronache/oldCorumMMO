#pragma once


#include <TIME.H>
#include "ProtocolDefinition.h"
#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4DyuchiGRX_common\\math.inl"
#else
#include "./../4DyuchiGRX_common\\math.inl"
#endif
#include "GM_Command.h"
#include "Item.h"


#pragma pack(push,1)


//-----------------------------------------------------------------------------
// ������ ���������� ���� �Ѵ�. 
//-----------------------------------------------------------------------------
struct CTWS_REQUEST_DUNGEON_INFO : public WORLD_PACKET_HEADER
{
	WORD	wDungeonID;

	CTWS_REQUEST_DUNGEON_INFO()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REQUEST_DUNGEON_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_REQUEST_DUNGEON_INFO);	}
};



//-----------------------------------------------------------------------------
// Client�� �������� �䱸�� ������ (�Ϲݴ���)
//-----------------------------------------------------------------------------
struct WSTC_REPLY_DUNGEON_INFO : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	WORD	wGroupID;
	char	szDungeonName[ MAX_DUNGEON_NAME_LENGTH ];
	WORD	wAreaNum;
	WORD	wAreaLevel_h;								//0 = ���ʺ���, 1 = �ʺ���, 2 = �߼���, 3 = �����, 4 = �����������
	WORD	wAreaLevel_l;
	WORD	wInitLevel;									// ���� �⺻ ����
	WORD	wUpgradeLevel;								// ���� ���׷��̵�� ����
	WORD	wBattleTimeLimit;							// ������ ���� �ð�.
	WORD	wProduction;								// ����ǰ ���̵�.
	BYTE	byProductionCount;
	BYTE	bSiege;		
	BYTE	bOperationType;								// ���� � ����..0 = ��ָ��, 1 = �����, 2 = ������, 3 = ���������
	WORD	wLayerCount;
	DWORD	dwOwnerIndex;
	DWORD	dwOwnerGuildNum;
	CItem	cMagicArrayItem;							// ������ ������.
	DWORD	dwAccEntrance;								// ���� �����
	DWORD	dwEntrance;									// �����
	DWORD	dwAccExp;									// ���� ����ġ
	CItem	m_cGuardianItem;							// ����� ������
	BYTE	byDefenseCount;
	BYTE	byProperty;
	BYTE	byEntererMinLevel;							// ���ó��� �ּ� ����
	BYTE	byEntererMaxLevel;							// ���ó��� �ִ� ����
	DWORD	dwRemainSiegeStartTick;						// ������ �ҷ��� ��ð� ���Ҵ°�?
	DWORD	dwRemainSiegeEndTick;						// ������ ����ϱ� ��ð� ���Ҵ°�?
	DWORD	dwRemainLevelStartTick;						// ���� ���� (�ð��� ������ ���� ������ �����Ѵ�.)
	
	char	szOwner[ MAX_CHARACTER_NAME_LENGTH ];		//������ �Ǵ� �������� 	
	char    szManager[ MAX_CHARACTER_NAME_LENGTH ];		//������ ĳ���͸�..  �������ΰ�� ������� �渶
	
	char	szSchoolName[MAX_CHARACTER_NAME_LENGTH];	// �Ϻ����� �б���� �̸��� ���� ����.

	BYTE	bMemoLength;
	char	szMemo[ MAX_DUNGEON_MEMO_LENGTH ];			//���� �������� �޸� 
	WSTC_REPLY_DUNGEON_INFO(){	memset(this, 0, sizeof(this));	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REPLY_DUNGEON_INFO;	}
	DWORD	GetPacketSize()	{	return (sizeof(WSTC_REPLY_DUNGEON_INFO) - (MAX_DUNGEON_MEMO_LENGTH - bMemoLength));	}
};


//-----------------------------------------------------------------------------
// ������ Join�� �õ� �Ѵ�. 
//-----------------------------------------------------------------------------
struct CTWS_TRY_TO_JOIN_DUNGEON : public WORLD_PACKET_HEADER
{
	WORD	wDungeonID;	
	
	CTWS_TRY_TO_JOIN_DUNGEON()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_TRY_TO_JOIN_DUNGEON;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_TRY_TO_JOIN_DUNGEON);	}
};


//-----------------------------------------------------------------------------
// ���弭���� Ŭ���̾�Ʈ���� �ش� ������ Join �� �� ���ٴ� �޼����� ���� 
//-----------------------------------------------------------------------------
struct WSTC_DUNGEON_JOIN_FAILED : public WORLD_PACKET_HEADER
{
	WORD	wDungeonID;		// �ش� ���� ID
	BYTE	bFailCode;		// 1�̸� ���� �������� �ȵ�, 2�̸� Dungeon�� Server���� ������ ����ȵ� ,3�̸� DungeonServer AcceptTable�� �߰� ���� 
							// 100 : �̺�Ʈ �Ա� �ð� ���., 101 : �̺�Ʈ �ο� �ʰ�.

	WSTC_DUNGEON_JOIN_FAILED()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_DUNGEON_JOIN_FAILED;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_DUNGEON_JOIN_FAILED);	} 
};


//-----------------------------------------------------------------------------
// ���弭���� Ŭ���̾�Ʈ���� ������ ���Ǿ����� ���� �ش� ���������� �����϶�� �޼����� ������. 
//-----------------------------------------------------------------------------
struct WSTC_CONNECT_DUNGEON_SERVER : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	DWORD	dwDungeonIP;
	
	DWORD	dwPartyID;
	WORD	wPort;
	
	WSTC_CONNECT_DUNGEON_SERVER()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_CONNECT_DUNGEON_SERVER;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_CONNECT_DUNGEON_SERVER);	}
};


// ������ ����
struct WSTC_REFRESHEVENT_DUNGEON_STARTSIEGE : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	DWORD	dwSiegeEndTick;
	
	WSTC_REFRESHEVENT_DUNGEON_STARTSIEGE()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_STARTSIEGE;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_STARTSIEGE);	}
};


// ������
struct WSTC_REFRESHEVENT_DUNGEON_LEVELUP : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	DWORD	dwLevelUpDestTick;
	WSTC_REFRESHEVENT_DUNGEON_LEVELUP()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_LEVELUP;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_LEVELUP);	}
};


// ����ǰ ����
struct WSTC_REFRESHEVENT_DUNGEON_CREATEPRODUCTION : public WORLD_PACKET_HEADER
{
	BYTE	byProductionCount;
	WORD	wDungeonID;
	WORD	wProduction;
	WSTC_REFRESHEVENT_DUNGEON_CREATEPRODUCTION()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_CREATEPRODUCTION;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_CREATEPRODUCTION);	}
};


// ������ ��
struct WSTC_REFRESHEVENT_DUNGEON_ENDSIEGE : public WORLD_PACKET_HEADER
{
	
	BYTE	bDefenseSuccess;
	WORD	wDungeonID;
	DWORD	dwSiegeStartTick;
	WSTC_REFRESHEVENT_DUNGEON_ENDSIEGE()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_ENDSIEGE;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_ENDSIEGE);	}
};


// ������ �ٲ���.
struct WSTC_REFRESHEVENT_DUNGEON_CHANGE_OWNER : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	DWORD	dwUserIndex;
	DWORD	dwGuildId;
	char	szOwner[ MAX_CHARACTER_NAME_LENGTH ];
	char	szGuildName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_REFRESHEVENT_DUNGEON_CHANGE_OWNER()
	{
		bGameStatus = UPDATE_GAME_WORLD;
		bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_CHANGE_OWNER;
		memset(szOwner, 0, sizeof(szOwner));
		memset(szGuildName, 0, sizeof(szGuildName));
	}

	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_CHANGE_OWNER);	}
};


//-----------------------------------------------------------------------------
// GM�� �������� ��Ŷ 
//-----------------------------------------------------------------------------
struct CTWS_GM_NOTICE : public WORLD_PACKET_HEADER
{
	WORD	wMsgLen;
	char	szMsg[1024];

	CTWS_GM_NOTICE()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_NOTICE;	}
	DWORD	GetPacketSize()	{	return 4 + wMsgLen;	}
};


//-----------------------------------------------------------------------------
// ��Ż ������ ���弭���� �ֱ� ���������� ���ŵǾ�� �Ҷ� 
//-----------------------------------------------------------------------------
struct CTWS_REFRESH_RECENT_DUNGEON : public WORLD_PACKET_HEADER
{
	WORD	wDungeonID;

	CTWS_REFRESH_RECENT_DUNGEON()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESH_RECENT_DUNGEON;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_REFRESH_RECENT_DUNGEON);	}
};


// ������ �ٲ���.
struct WSTC_REFRESHEVENT_DUNGEON_CHANGE_OWNER_GUILD : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	DWORD	dwGuildId;
	char	szGuildName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_REFRESHEVENT_DUNGEON_CHANGE_OWNER_GUILD()
	{
		bGameStatus = UPDATE_GAME_WORLD;
		bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_CHANGE_OWNER_GUILD;
		memset(szGuildName, 0, sizeof(szGuildName));
	}

	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_CHANGE_OWNER_GUILD);	}
};


//-----------------------------------------------------------------------------
// �̺�Ʈ ���� ����.
//-----------------------------------------------------------------------------
struct WSTC_EVENT_DUNGEON_INFO : public WORLD_PACKET_HEADER
{
	WORD	wDungeonID;								// ���� ID
	BYTE	byEventState;							// ���� ����.
	char	szDungeonName[MAX_DUNGEON_NAME_LENGTH]; // ���� �̸�.
	DWORD	dwReadyOpenTime;						// �Ա� ���� ��	������ �ð�.	
	BYTE	byWorldMapID;							// World Map Number.
	BYTE	byEventType;							// �̺�Ʈ ������ Ÿ��.
	BYTE	byEnterMinLevel;						// ���� ���� �ּ� ����.
	BYTE	byEnterMaxLevel;						// ���� ���� �ִ� ����.
	DWORD	dwEventEndTick;							// �̺�Ʈ ���� ���� �ð�
	WSTC_EVENT_DUNGEON_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_EVENT_DUNGEON_INFO; }
	DWORD	GetPacketSize()		{ return sizeof(WSTC_EVENT_DUNGEON_INFO);	}
};


struct CTWS_REQUEST_OWN_DUNGEON_INFO : public WORLD_PACKET_HEADER
{	
	CTWS_REQUEST_OWN_DUNGEON_INFO()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REQUEST_OWN_DUNGEON_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_REQUEST_OWN_DUNGEON_INFO);	}
};


//-----------------------------------------------------------------------------
// �̺�Ʈ ������ ���� ��ȭ.
//-----------------------------------------------------------------------------
struct WSTC_EVENT_DUNGEON_STATE_INFO : public WORLD_PACKET_HEADER
{
	WORD	wDungeonID;			// ���� ID
	BYTE	byEventState;		// �̺�Ʈ ���� ����.		
	DWORD	dwReadyOpenTime;	// �Ա� ���� �� ������ �ð�.
	DWORD	dwEventEndTick;		// �̺�Ʈ ���� ���� �ð�
	
	WSTC_EVENT_DUNGEON_STATE_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_EVENT_DUNGEON_STATE_INFO; }
	DWORD	GetPacketSize()			{ return sizeof(WSTC_EVENT_DUNGEON_STATE_INFO); }
};


//-----------------------------------------------------------------------------
// Event Dungeon ������ �����޶�� ��û.
//-----------------------------------------------------------------------------
struct CTWS_QUERY_EVENT_DUNGEON_INFO : public WORLD_PACKET_HEADER
{
	BYTE	bRequestType;		// ��û Ÿ��. 1 : �̺�Ʈ ������ ����, 2 : �̺�Ʈ ������ ����.

	CTWS_QUERY_EVENT_DUNGEON_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_QUERY_EVENT_DUNGEON_INFO; }
	DWORD	GetPacketSize()	{ return sizeof(CTWS_QUERY_EVENT_DUNGEON_INFO); }
};


//-----------------------------------------------------------------------------
// ���������� ���弭������ �ش� ������ �������� �����ٴ� ��ȣ�� �����ָ�(DSTC_GO_TO_WORLD)
// ���弭���� �� ������ ������� ��ġ�� �Ǻ��Ͽ� ��Ŷ�� �ְ�, Ŭ���̾�Ʈ�� ����Ŷ�� �ް� 
// ����� �ε��� �����Ѵ�. 
//-----------------------------------------------------------------------------
struct WSTC_MOVE_TO_WORLD : public WORLD_PACKET_HEADER
{
	BYTE	bWorldMapID;		// 1�� ���� ��
	WORD	wToDungeonID;
	WORD	wNowDungeonID;		// ��������� ������ ���� ���� �ִ� ���� ���̵�
	float	fStartPos_X;
	float	fStartPos_Z;

	WSTC_MOVE_TO_WORLD()	{	bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GO_TO_WORLD;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_MOVE_TO_WORLD);	}
};

const BYTE GO_TO_WORLD_ERROR_DUNGEON_NO = 1;
const BYTE GO_TO_WORLD_ERROR_DUNGEON_POS = 2;
const BYTE GO_TO_WORLD_ERROR_STADIUM = 3;
//-----------------------------------------------------------------------------
// World�� �̵� ����!
//-----------------------------------------------------------------------------
struct WSTC_GO_TO_WORLD_FAIL : public WORLD_PACKET_HEADER
{
	BYTE	bErrorCode;

	WSTC_GO_TO_WORLD_FAIL()	{	bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GO_TO_WORLD_FAIL;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GO_TO_WORLD_FAIL);	}
};


// ����� ȹ��
struct WSTC_REFRESHEVENT_DUNGEON_ACCENTRANCE_ACQUISITION : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	DWORD	dwMoney;
	WSTC_REFRESHEVENT_DUNGEON_ACCENTRANCE_ACQUISITION()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_ACCENTRANCE_ACQUISITION;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_ACCENTRANCE_ACQUISITION);	}
};


// ���� ����ġ ȹ��
struct WSTC_REFRESHEVENT_DUNGEON_ACCEXP_ACQUISITION : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	DWORD	dwAccExp;
	WSTC_REFRESHEVENT_DUNGEON_ACCEXP_ACQUISITION()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_ACCEXP_ACQUISITION;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_ACCEXP_ACQUISITION);	}
};


// ����� ����
struct WSTC_REFRESHEVENT_DUNGEON_ENTRANCE_EDIT : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	DWORD	dwMoney;
	WSTC_REFRESHEVENT_DUNGEON_ENTRANCE_EDIT()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_ENTRANCE_EDIT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_ENTRANCE_EDIT);	}
};


//-----------------------------------------------------------------------------
// ����� �̵��� ������ 
//-----------------------------------------------------------------------------
struct CTWS_GO_TO_WORLD_SUCCESS : public WORLD_PACKET_HEADER
{
	BYTE	bWorldMapId;		//����� ��ȣ 
	
	CTWS_GO_TO_WORLD_SUCCESS()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GO_TO_WORLD_SUCCESS;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GO_TO_WORLD_SUCCESS);	}
};


//-----------------------------------------------------------------------------
// ����� �̵� ��Ŷ 
//-----------------------------------------------------------------------------
struct CTC_WORLD_MOVE : public WORLD_PACKET_HEADER
{	
	DWORD	dwCharIndex;
	BYTE	bMoveType;			// Move types
	VECTOR3	v3MoveStartPos;		// Start position
	VECTOR3	v3MoveDirection;	// Direction
	
	WORD	wDestX;
	WORD	wDestZ;

	CTC_WORLD_MOVE()		{	bGameStatus = UPDATE_GAME_WORLD;		bHeader = Protocol_World::CMD_WORLD_MOVE;	}
	DWORD	GetPacketSize()	{	return sizeof(CTC_WORLD_MOVE);	}
};


//-----------------------------------------------------------------------------
// ����� �̵� ��Ŷ 
//-----------------------------------------------------------------------------
struct CTC_WORLD_STOP : public WORLD_PACKET_HEADER
{	
	DWORD	dwCharIndex;
	VECTOR3	v3StopPos;			// Stop position
	CTC_WORLD_STOP()		{	bGameStatus = UPDATE_GAME_WORLD;		bHeader = Protocol_World::CMD_WORLD_STOP;	}
	DWORD	GetPacketSize()	{	return sizeof(CTC_WORLD_STOP);	}
};


struct WORLD_PARTY_APPEAR_INFO
{
	DWORD	dwCharIndex;
	WORD	wHead;
	WORD	wClass;
	WORD	wArmor;
	WORD	wHandR;
	WORD	wHandL;
	WORD	wHelmet;	
	
	float	fAppear_X;	//��Ÿ�� ��ġ 
	float	fAppear_Z;	//��Ÿ�� ��ġ 
};


//-----------------------------------------------------------------------------
// ����� ������ Appear �ǰ� �϶� 
//-----------------------------------------------------------------------------
struct WSTC_WORLD_APPEAR : public WORLD_PACKET_HEADER
{
	WORLD_PARTY_APPEAR_INFO	AppearInfo;

	WSTC_WORLD_APPEAR()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_WORLD_APPEAR; }
	DWORD	GetPacketSize()	{ return sizeof(WSTC_WORLD_APPEAR); }
};


//-----------------------------------------------------------------------------
// ����� ������ Disappear �ǰ� �϶� 
//-----------------------------------------------------------------------------
struct WSTC_WORLD_DISAPPEAR : public WORLD_PACKET_HEADER
{
	DWORD	dwCharIndex;
	
	WSTC_WORLD_DISAPPEAR()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_WORLD_DISAPPEAR; }
	DWORD	GetPacketSize()	{ return sizeof(WSTC_WORLD_DISAPPEAR); }
};


//-----------------------------------------------------------------------------
// ������ ����㿡 �������� ���� ���ο��� �ش� ����ʿ� �־��� ��� ��Ƽ������ Appear ������ ������ 
//-----------------------------------------------------------------------------
struct WSTC_WORLD_PARTY_USER_INFO : public WORLD_PACKET_HEADER
{
	BYTE	bUserCount;		//Appear �� ���� ��� 
	WORLD_PARTY_APPEAR_INFO	AppearInfo[ MAX_PARTY_USER ];

	WSTC_WORLD_PARTY_USER_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_WORLD_PARTY_USER_INFO; }
	DWORD	GetPacketSize()	{ return ( 3 + (sizeof(WORLD_PARTY_APPEAR_INFO) * bUserCount) ); }
};


//-----------------------------------------------------------------------------
// ������ Ư�� ������ ��ġ�� �ű��. 
//-----------------------------------------------------------------------------
struct CTC_MOVE_POSITION : public WORLD_PACKET_HEADER
{
	DWORD	dwCharIndex;
	VECTOR3 v3Pos;
	float	fDirection;

	CTC_MOVE_POSITION()		{	bGameStatus = UPDATE_GAME_WORLD;		bHeader = Protocol_World::CMD_WORLD_SET_POSITION;	}
	DWORD	GetPacketSize()	{	return sizeof(CTC_MOVE_POSITION);	}
};


//-----------------------------------------------------------------------------
// ��Ƽ�� ���� �����鿡�� ������ ���������� Connect�� �õ��Ѵٴ� ����� 
// �˷��־� Ŭ���̾�Ʈ ȭ�鿡���� ��� ������� �Ѵ�. ���� Dungeon�� ������
// �����ϸ� Player�� Remove �Ѵ�. 
//-----------------------------------------------------------------------------
struct WSTC_PARTY_USER_TRY_TO_CONNECT : public WORLD_PACKET_HEADER
{
	DWORD	dwCharIndex;

	WSTC_PARTY_USER_TRY_TO_CONNECT()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_USER_TRY_TO_CONNECT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PARTY_USER_TRY_TO_CONNECT);	}
};


//-----------------------------------------------------------------------------
// Party ������ ������� ž���� 
//-----------------------------------------------------------------------------
struct CTC_AIRSHIP_RIDING : public WORLD_PACKET_HEADER
{
	DWORD	dwCharIndex;

	CTC_AIRSHIP_RIDING()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_AIRSHIP_RIDING;	}
	DWORD	GetPacketSize()	{	return sizeof(CTC_AIRSHIP_RIDING);	}
};


//-----------------------------------------------------------------------------
// Party ������ ������� ������ 
//-----------------------------------------------------------------------------
struct CTC_AIRSHIP_GETOFF : public WORLD_PACKET_HEADER
{
	DWORD	dwCharIndex;
	VECTOR3	v3Pos;			//������ ��ġ 

	CTC_AIRSHIP_GETOFF()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_AIRSHIP_GETOFF;	}
	DWORD	GetPacketSize()	{	return sizeof(CTC_AIRSHIP_GETOFF);	}
};


// ���� ������� �ٲ����.
struct WSTC_REFRESHEVENT_DUNGEON_GUARDIANITEM : public WORLD_PACKET_HEADER
{
	WORD	wDungeonID;
	CItem	cGuardianItem;
	WSTC_REFRESHEVENT_DUNGEON_GUARDIANITEM()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_GUARDIANITEM;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_GUARDIANITEM);	}
};


// ���� �������� �ٲ����.
struct WSTC_REFRESHEVENT_DUNGEON_MAGICFIELDARRAY : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	CItem	cMagicFieldArrayItem;
	WSTC_REFRESHEVENT_DUNGEON_MAGICFIELDARRAY()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_MAGICFIELDARRAYITEM;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_MAGICFIELDARRAY);	}
};


//#ifndef NEW_COMMAND

//-----------------------------------------------------------------------------
// GM Ŀ�ǵ�.
//-----------------------------------------------------------------------------
struct CTWS_GM_COMMAND : public WORLD_PACKET_HEADER
{		
	WORD	wCommand;
	char	szCharName[ MAX_CHARACTER_NAME_LENGTH ];
	DWORD	dwTemp[5];
	
	CTWS_GM_COMMAND()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GM_COMMAND;	}
	DWORD	GetPacketSize()	{	return sizeof( CTWS_GM_COMMAND );	}
};


struct CTWS_NEW_GM_COMMAND  : public WORLD_PACKET_HEADER
{
	union
	{
		GM_CMD_PARAM					sBaseGMCmdParam;
		GM_CMD_PARAM_Recall_Someone		sGMCmdParamRecallSomeone;
		GM_CMD_PARAM_Get_Move_Someone	sGMCmdParamGetMoveSomeone;
		GM_CMD_PARAM_Dungeon_Portal		sGMCmdParamDungeonPortal;
		GM_CMD_PARAM_Join_Guild			sGMCmdParamJoinGuild;
		GM_CMD_PARAM_Kick_User			sGMCmdParamKickUser;
	};
		
	CTWS_NEW_GM_COMMAND()		
	{ 
		bGameStatus = UPDATE_GAME_WORLD;	
		bHeader = Protocol_World::CMD_GM_COMMAND;	
	}

	DWORD GetPacketSize()	{ return sizeof( CTWS_NEW_GM_COMMAND );	}
};



//-----------------------------------------------------------------------------
// GM Ŀ�ǵ� �����.
//-----------------------------------------------------------------------------
struct WSTC_GM_COMMAND_RESULT : public WORLD_PACKET_HEADER
{
	WSTC_GM_COMMAND_RESULT()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GM_COMMAND_RESULT;	}
	DWORD	GetPaketSize()		{	return sizeof( WSTC_GM_COMMAND_RESULT );	}
};


//-----------------------------------------------------------------------------
// ���� ����͸� ������ üũ�� �ֽÿ�.
//-----------------------------------------------------------------------------
struct CTWS_CHECK_MONITORING : public WORLD_PACKET_HEADER
{		
	CTWS_CHECK_MONITORING()		{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_CHECK_MONITORING;	}
	DWORD	GetPacketSize()		{ return sizeof(CTWS_CHECK_MONITORING); }	
};


// ���潺 Ƚ��
struct WSTC_REFRESHEVENT_DEFENSECOUNT : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	BYTE	byDefenseCount;
	WSTC_REFRESHEVENT_DEFENSECOUNT()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DEFENSECOUNT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DEFENSECOUNT);	}
};


// ���� ����
struct WSTC_REFRESHEVENT_DUNGEON_OPERATIONMODE : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	BYTE	byOperationMode;
	WSTC_REFRESHEVENT_DUNGEON_OPERATIONMODE()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_OPERATIONMODE;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_OPERATIONMODE);	}
};


//----------------------------------------------------------------------------------
// ���ݰ��� ���� ���� ��Ŷ 
//----------------------------------------------------------------------------------
struct WSTC_BILLING_INFO : public WORLD_PACKET_HEADER
{
	BYTE		bCurBillingType;
	BYTE		bInfoType;
	DWORD		dwRemainTime;
	DBTIMESTAMP	BillingEndDate;

	WSTC_BILLING_INFO()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_BILLING_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_BILLING_INFO);	}
};


// �߱� ������. ���������� ������ kick�� �� ������ �˷��ش�.
struct WSTC_CHINA_BILL_KICK_CODE : public WORLD_PACKET_HEADER
{
	BYTE		btKickCode;

	WSTC_CHINA_BILL_KICK_CODE() { bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_CHINA_BILL_KICK_CODE; btKickCode = 0;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_CHINA_BILL_KICK_CODE);	}
};


//----------------------------------------------------------------------------------
// Ŭ���̾�Ʈ�� Ÿ�̸� üũ�� �غ� �Ǿ���.
//----------------------------------------------------------------------------------
struct CTWS_TIMER_READY
{
	BYTE	bGameStatus;
	BYTE	bHeader;

	CTWS_TIMER_READY()			{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_TIMER_READY;	}
	DWORD	GetPacketSize()		{	return sizeof(CTWS_TIMER_READY);	}
};


struct WSTC_TIMER_SYNC
{
	BYTE	bGameStatus;
	BYTE	bHeader;
	WORD	wIntervalSec;

	WSTC_TIMER_SYNC()			{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_TIMER_SYNC;	}
	DWORD	GetPacketSize()		{	return sizeof(WSTC_TIMER_SYNC);	}
};


struct CTWS_TIMER_ACK
{
	BYTE	bGameStatus;
	BYTE	bHeader;
	DWORD	dwTime;

	CTWS_TIMER_ACK()			{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_TIMER_ACK;	}
	DWORD	GetPacketSize()		{	return sizeof(CTWS_TIMER_ACK);	}
};


struct WSTC_CB_MESSAGE
{
	BYTE	bGameStatus;
	BYTE	bHeader;
	BYTE	byMessageType;

	WSTC_CB_MESSAGE()			{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_CB_MESSAGE;	}
	DWORD	GetPacketSize()		{	return sizeof(WSTC_CB_MESSAGE);	}	
};


//-----------------------------------------------------------------------------
// ��Ƽ ���� ���� ����. 
//-----------------------------------------------------------------------------
struct CTWS_PARTYUSER_WORLDINFO : public WORLD_PACKET_HEADER
{
	WORD	wHelmet;
	WORD	wHandR;
	WORD	wHandL;
	WORD	wArmor;
	BYTE	byCurrentHand;

	CTWS_PARTYUSER_WORLDINFO()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTYUSER_WORLDINFO;	}
	DWORD	GetPacketSize()			{	return sizeof(CTWS_PARTYUSER_WORLDINFO);	}	
};


struct WSTC_GUILDWAR_MESSAGE : public WORLD_PACKET_HEADER
{
	char	szGuildName[MAX_GUILD_NAME_LENGTH];	
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	char	szNameDst[MAX_CHARACTER_NAME_LENGTH];
	BYTE	byType;
	BYTE	byLayerIndex;
	WORD	wDungeonID;

	WSTC_GUILDWAR_MESSAGE()			{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDWAR_MESSAGE; }
	DWORD		GetPacketSize()		{ return sizeof(WSTC_GUILDWAR_MESSAGE); }
};


//-----------------------------------------------------------------------------
// ��Ƽ �Խ��� ���� ����.
//-----------------------------------------------------------------------------
struct CTWS_PARTY_BOARDDELETE : public WORLD_PACKET_HEADER
{
	DWORD	dwPartyId;

	CTWS_PARTY_BOARDDELETE()		{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_BOARDDELETE; }
	DWORD		GetPacketSize()		{ return sizeof(CTWS_PARTY_BOARDDELETE); }
};


struct CTWS_PARTY_INFOREQUEST : public WORLD_PACKET_HEADER
{
	DWORD	dwUserIndex;

	CTWS_PARTY_INFOREQUEST()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_INFOREQUEST;	}
	DWORD	GetPacketSize()		{	return sizeof(CTWS_PARTY_INFOREQUEST);	}
};


struct WSTC_PARTY_INFOREQUEST : public WORLD_PACKET_HEADER
{
	DWORD	dwPartyId;
	DWORD	dwLeaderUserIndex;	
	char	szCharName[MAX_CHARACTER_NAME_LENGTH];
	char	szMemo[32];
	WORD	wDungeonID;
	BYTE	byType;

	WSTC_PARTY_INFOREQUEST()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_INFOREQUESTRT;	}
	DWORD	GetPacketSize()		{	return sizeof(WSTC_PARTY_INFOREQUEST);	}
};


struct WSTC_PARTY_ENTRY : public WORLD_PACKET_HEADER
{				
	BYTE	byType;	

	WSTC_PARTY_ENTRY()			{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_ENTRYRT;	}
	DWORD	GetPacketSize()		{	return sizeof(WSTC_PARTY_ENTRY);	}
};


struct CTWS_PARTY_ENTRY : public WORLD_PACKET_HEADER
{				
	DWORD	dwUserIndex;
	DWORD	dwPartyId;	
	char	szPartyEntryMemo[MAX_PARTYENTRY_TEXT];	

	CTWS_PARTY_ENTRY()			{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_ENTRY;	}
	DWORD	GetPacketSize()		{	return sizeof(CTWS_PARTY_ENTRY);	}
};

// ��Ƽ ����, ������/����ġ �й� : �ִ��� 2005.1.25
struct CTWS_PARTY_CONFIG : public WORLD_PACKET_HEADER
{
	PARTY_CONFIG pConfig;

	CTWS_PARTY_CONFIG()			{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_CONFIG;	}
	DWORD	GetPacketSize()		{	return (sizeof(CTWS_PARTY_CONFIG)); }
};

// �ֱ� ��Ƽ�� ��� ����
struct WSTC_PARTY_FRIEND : public WORLD_PACKET_HEADER
{
	DWORD dwCount;
	PARTY_FRIEND pFriend[20];

	WSTC_PARTY_FRIEND()			{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_FRIEND;	}
	DWORD	GetPacketSize()		{	return (sizeof(WSTC_PARTY_FRIEND)); }
};

struct CTWS_PARTY_FRIEND : public WORLD_PACKET_HEADER
{
	CTWS_PARTY_FRIEND()			{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_FRIEND;	}
	DWORD	GetPacketSize()		{	return (sizeof(CTWS_PARTY_FRIEND)); }
};

struct WSTC_GUILDWAR_RANK : public WORLD_PACKET_HEADER
{
	DWORD		dwGuildId[4];
	DWORD		dwPoint[4];
	char		szName[4][MAX_GUILD_NAME_LENGTH];	
	DWORD		dwRank;
	
	WSTC_GUILDWAR_RANK()	{	bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILDWAR_RANKRT; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILDWAR_RANK); }
};


struct CTWS_GUILDWAR_RANK : public WORLD_PACKET_HEADER
{
	DWORD		dwGuildId;
	
	CTWS_GUILDWAR_RANK()	{	bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILDWAR_RANK; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILDWAR_RANK); }
};


struct WSTC_GUILDWAR_EXIT : public WORLD_PACKET_HEADER
{
	DWORD		dwGuildId;	
	DWORD		dwPoint;	
	BYTE		byType;
	
	WSTC_GUILDWAR_EXIT()	{	bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILDWAR_EXITRT; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILDWAR_EXIT); }
};


struct CTWS_GUILDWAR_EXIT : public WORLD_PACKET_HEADER
{
	DWORD		dwGuildId;
	DWORD		dwGuildIdDst;		
	
	CTWS_GUILDWAR_EXIT()	{ bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILDWAR_EXIT; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILDWAR_EXIT); }
};


struct WSTC_GUILDWAR_POINT : public WORLD_PACKET_HEADER
{
	DWORD	dwGuildId;
	WORD	wPoint1;
	WORD	wPoint2;	
	BYTE	byType;

	WSTC_GUILDWAR_POINT()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDWAR_POINT;	}
	DWORD	GetPacketSize()		{	return sizeof(WSTC_GUILDWAR_POINT);	}
};


struct CTWS_GUILDWAR_CREATE : public WORLD_PACKET_HEADER
{
	DWORD	dwGuildId;
	DWORD	dwDstGuildId;
	DWORD	dwUserIndex;

	CTWS_GUILDWAR_CREATE()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDWAR_CREATE;	}
	DWORD	GetPacketSize()		{	return sizeof(CTWS_GUILDWAR_CREATE);	}
};


struct WSTC_GUILDWAR_JOIN : public WORLD_PACKET_HEADER
{
	DWORD	dwGuildId;
	DWORD	dwDstGuildId;
	DWORD	dwUserIndex;
	char	szGuildName[MAX_GUILD_NAME_LENGTH];	

	WSTC_GUILDWAR_JOIN()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDWAR_JOIN;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILDWAR_JOIN);	}
};


struct CTWS_GUILDWAR_REQUESTRT : public WORLD_PACKET_HEADER
{
	DWORD	dwGuildId;
	DWORD	dwDstGuildId;
	DWORD	dwUserIndex;

	CTWS_GUILDWAR_REQUESTRT()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDWAR_MASTERRT;	}
	DWORD	GetPacketSize()		{	return sizeof(CTWS_GUILDWAR_REQUESTRT);	}
};


struct CTWS_GUILDWAR_ERRROR : public WORLD_PACKET_HEADER
{
	BYTE	byError;	
	DWORD	dwUserIndex;
	char	szGuildName[MAX_GUILD_NAME_LENGTH];

	CTWS_GUILDWAR_ERRROR()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDWAR_ERRORRT;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILDWAR_ERRROR);	}
};


struct WSTC_GUILDWAR_ERRROR : public WORLD_PACKET_HEADER
{
	BYTE	byError;
	char	szNameTemp[MAX_CHARACTER_NAME_LENGTH];
		
	WSTC_GUILDWAR_ERRROR()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDWAR_ERROR;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILDWAR_ERRROR);	}
};


struct WSTC_GUILDWAR_REQUEST : public WORLD_PACKET_HEADER
{
	DWORD	dwGuildId;
	DWORD	dwDstGuildId;
	DWORD	dwUserIndex;
	char	szGuildName[MAX_GUILD_NAME_LENGTH];
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_GUILDWAR_REQUEST()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDWAR_MASTER;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILDWAR_REQUEST);	}
};


struct CTWS_GUILDWAR_REQUEST : public WORLD_PACKET_HEADER
{			
	DWORD	dwGuildId;
	DWORD	dwDstGuildId;
	DWORD	dwUserIndex;

	CTWS_GUILDWAR_REQUEST()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDWAR_REQUEST;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILDWAR_REQUEST);	}
};


struct WSTC_GUILD_PORTALFAIL : public WORLD_PACKET_HEADER
{	
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	BYTE	byResult;
	
	WSTC_GUILD_PORTALFAIL()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_PORTALFAIL; }
	DWORD	GetPacketSize()	{ return sizeof(WSTC_GUILD_PORTALFAIL); }
};


struct WSTC_GUILD_PORTALUSED : public WORLD_PACKET_HEADER
{	
	DWORD	dwGuildId;
	DWORD	dwUserIndex;
	DWORD	dwPartalUserIndex;
	BYTE	dwResult;
	BYTE	byIndex;
	BYTE	byType;
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_GUILD_PORTALUSED()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_PORTALUSED;	}
	DWORD	GetPacketSize()	{	return (sizeof(WSTC_GUILD_PORTALUSED)); }
};


struct WSTC_GUILD_PORTAL : public WORLD_PACKET_HEADER
{	
	DWORD	dwGuildId;
	DWORD	dwUserIndex;
	DWORD	dwPartalUserIndex;
	BYTE	byIndex;
	BYTE	byType;
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_GUILD_PORTAL()	{ bGameStatus = UPDATE_GAME_WORLD;		bHeader = Protocol_World::CMD_GUILD_PORTAL;	}
	DWORD	GetPacketSize()	{	return (sizeof(WSTC_GUILD_PORTAL)); }
};


//-----------------------------------------------------------------------------
// ��� ���� ���� �޼���.
//-----------------------------------------------------------------------------
struct WSTC_GUILD_ERROR : public WORLD_PACKET_HEADER
{	
	BYTE		byError;
	
	WSTC_GUILD_ERROR()		{ bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_FAIL; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_ERROR); }
};


//-----------------------------------------------------------------------------
// ����� ����.
//-----------------------------------------------------------------------------
struct WSTC_GUILD_LIST : public WORLD_PACKET_HEADER
{
	char		szName[MAX_GUILD_NAME_LENGTH];
	DWORD		dwGuildId;
	WORD		wPoint1;
	WORD		wPoint2;
	time_t		sTime;	
	time_t		sCurTime;
	
	WSTC_GUILD_LIST()		{ bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_LIST; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_LIST); }
};


typedef struct __tag_sScriptex__
{
	ENCOMMAND	enCommand;
	char		szId[MAX_ID_LENGTH];
	char		szMessage[MAX_INPUT_BUFFER_SIZE];	
	char		szText[MAX_INPUT_BUFFER_SIZE];

}SSCRIPTEX, *LPSSCRIPTEX;


//-----------------------------------------------------------------------------
// ��ɾ Ŭ���̾�Ʈ���� ����(���弭��).
//-----------------------------------------------------------------------------
struct WSTC_WS_CHAT : public WORLD_PACKET_HEADER
{	
	SSCRIPTEX	cScript;	

	WSTC_WS_CHAT()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_COMMAND_SUCCESS;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_WS_CHAT);	}
};


//-----------------------------------------------------------------------------
// ���� ��ɾ ������(���弭��).
//-----------------------------------------------------------------------------
struct WSTC_WSF_CHAT : public WORLD_PACKET_HEADER
{		
	int			nFail;
	ENCOMMAND	enCommand;
	char		szId[MAX_ID_LENGTH];

	WSTC_WSF_CHAT()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_COMMAND_FAIL;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_WSF_CHAT); }
};


//-----------------------------------------------------------------------------
// Ŭ���̾�Ʈ���� �ӼӸ��� ����(���弭��).
//-----------------------------------------------------------------------------
struct WSTC_CHAT_WHISPER : public WORLD_PACKET_HEADER
{	
	int		nSuccess;
	char	szId[MAX_ID_LENGTH];
	char	szMessage[MAX_INPUT_BUFFER_SIZE];

	WSTC_CHAT_WHISPER() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_COMMAND_WHISPER;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_CHAT_WHISPER);	}
};


//-----------------------------------------------------------------------------
// Desc : ��ɾ Ŭ���̾�Ʈ���� ����(���� ����).
//-----------------------------------------------------------------------------
struct WSTC_DS_CHAT : public WORLD_PACKET_HEADER
{	
	SSCRIPTEX	cScript;

	WSTC_DS_CHAT()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_COMMAND_DSUCCESS;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_DS_CHAT);	}
};


//-----------------------------------------------------------------------------
// ������ ����(���� ����).
//-----------------------------------------------------------------------------
struct WSTC_CHAT_COMMAND : public WORLD_PACKET_HEADER
{	
	int		nSuccess;
	BYTE	byMessageLen;
	char	szId[MAX_ID_LENGTH];
	char	szMessage[MAX_INPUT_BUFFER_SIZE];

	WSTC_CHAT_COMMAND() 
	{ 
		memset(this, 0, sizeof(*this));
		bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_COMMAND_MESSAGE;	
	}

	DWORD GetPacketSize()	
	{	
		byMessageLen = BYTE(lstrlen(szMessage)+1);
		return sizeof(WSTC_CHAT_COMMAND)-sizeof(szMessage)+byMessageLen;	
	}
};


//-----------------------------------------------------------------------------
// ������ ����(���� ����).
//-----------------------------------------------------------------------------
struct CTWS_DSCHAT_COMMAND : public WORLD_PACKET_HEADER
{	
	int		nSuccess;
	BYTE	byMessageLen;
	char	szId[MAX_ID_LENGTH];
	char	szMessage[MAX_INPUT_BUFFER_SIZE];

	CTWS_DSCHAT_COMMAND() 
	{ 
		memset(this, 0, sizeof(*this));
		bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_COMMAND_MESSAGE;	
	}

	DWORD GetPacketSize()	
	{	
		byMessageLen = BYTE(lstrlen(szMessage)+1);
		return sizeof(CTWS_DSCHAT_COMMAND)-sizeof(szMessage)+byMessageLen;	
	}
};


//-----------------------------------------------------------------------------
// Ŭ���̾�Ʈ���� ���� ��û�� ����.
//-----------------------------------------------------------------------------
struct CTWS_CHAT_MESSAGERQ : public WORLD_PACKET_HEADER
{		
	char	szId[MAX_ID_LENGTH];	

	CTWS_CHAT_MESSAGERQ() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_COMMAND_MESSAGERQ;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_CHAT_MESSAGERQ);	}
};


//-----------------------------------------------------------------------------
// Ŭ���̾�Ʈ���� ���� ��û�� ����.
//-----------------------------------------------------------------------------
struct WSTC_CHAT_RQ  : public WORLD_PACKET_HEADER
{
	int			nIndex;
	int			nCount;
	BYTE		byMessageLen;
	WORD		wdTime[6];
	char		szId[MAX_ID_LENGTH];	
	char		szMessage[MAX_INPUT_BUFFER_SIZE];

	WSTC_CHAT_RQ() 
	{ 
		memset(this, 0, sizeof(*this));
		bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_COMMAND_RQ;	
	}
	DWORD	GetPacketSize()	
	{	
		byMessageLen = BYTE(lstrlen(szMessage)+1);
		return sizeof(WSTC_CHAT_RQ)-sizeof(szMessage)+byMessageLen;	
	}
};


//-----------------------------------------------------------------------------
// ä�� �޼����� Ŭ���̾�Ʈ�� �޴´�.
//-----------------------------------------------------------------------------
struct WSTC_CHAT_USER : public WORLD_PACKET_HEADER
{
	int			nIndex;
	BYTE		byMessageLen;
	char		szDstId[MAX_ID_LENGTH];
	char		szId[MAX_ID_LENGTH];
	char		szMessage[MAX_INPUT_BUFFER_SIZE];

	WSTC_CHAT_USER() 
	{ 
		memset(this, 0, sizeof(*this));
		bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_CHAT_USER; 
	}
	DWORD	GetPacketSize()	
	{	
		byMessageLen = BYTE(lstrlen(szMessage)+1);
		return sizeof(WSTC_CHAT_USER)-sizeof(szMessage)+byMessageLen; 
	}
};


//-----------------------------------------------------------------------------
// ä�� �޼����� Ŭ���̾�Ʈ�� �޴´�.
//-----------------------------------------------------------------------------
struct CTWS_DGCHAT_USER : public WORLD_PACKET_HEADER
{
	int			nIndex;
	BYTE		byMessageLen;
	char		szDstId[MAX_ID_LENGTH];
	char		szId[MAX_ID_LENGTH];
	char		szMessage[MAX_INPUT_BUFFER_SIZE];

	CTWS_DGCHAT_USER() 
	{ 
		memset(this, 0, sizeof(*this));
		bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_DGCHAT_USER; 
	}
	DWORD	GetPacketSize()	
	{	
		byMessageLen = BYTE(lstrlen(szMessage)+1);
		return sizeof(CTWS_DGCHAT_USER)-sizeof(szMessage)+byMessageLen; 
	}
};


//-----------------------------------------------------------------------------
// ���� ��ɾ ������(���� ����).
//-----------------------------------------------------------------------------
struct WSTC_DSF_CHAT : public WORLD_PACKET_HEADER
{	
	int			nFail;
	ENCOMMAND	enCommand;
	char		szId[MAX_ID_LENGTH];

	WSTC_DSF_CHAT()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_COMMAND_DGFAIL;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_DSF_CHAT);	}
};


//-----------------------------------------------------------------------------
// Ŭ���̾�Ʈ �ӼӸ��� ����(���� ����).
//-----------------------------------------------------------------------------
struct WSTC_DSCHAT_WHISPER : public WORLD_PACKET_HEADER
{
	int		nSuccess;
	BYTE	byMessageLen;
	char	szId[MAX_ID_LENGTH];
	char	szMessage[MAX_INPUT_BUFFER_SIZE];

	WSTC_DSCHAT_WHISPER() 
	{ 
		memset(this, 0, sizeof(*this));
		bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_COMMAND_DGWHISPER;	
	}
	DWORD	GetPacketSize()	
	{	
		byMessageLen = BYTE(lstrlen(szMessage)+1);
		return sizeof(WSTC_DSCHAT_WHISPER)-sizeof(szMessage)+byMessageLen;	
	}
};


//-----------------------------------------------------------------------------
// ä�� �޼����� Ŭ���̾�Ʈ�� ������.
//-----------------------------------------------------------------------------
struct WSTC_DGCHAT_USER : public WORLD_PACKET_HEADER
{
	int			nIndex;
	BYTE		byMessageLen;
	char		szDstId[MAX_ID_LENGTH];
	char		szId[MAX_ID_LENGTH];
	char		szMessage[MAX_INPUT_BUFFER_SIZE];
	
	BYTE		bSerType;

	WSTC_DGCHAT_USER() 
	{ 
		memset(this, 0, sizeof(*this));
		bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_WSDGCHAT_USER; 
	}
	DWORD	GetPacketSize()	
	{	
		byMessageLen = BYTE(lstrlen(szMessage)+1);
		return sizeof(WSTC_DGCHAT_USER)-sizeof(szMessage)+byMessageLen; 
	}
};


//-----------------------------------------------------------------------------
// ��� ������ Ŭ���̾�Ʈ�� ������.
//-----------------------------------------------------------------------------
struct WSTC_GUILD_USER : public WORLD_PACKET_HEADER
{
	char		szGuildName[MAX_GUILD_NAME_LENGTH];	
	char		szRankName[4][MAX_GUILD_NAME_LENGTH];
	BYTE		byType;
	BYTE		byCount;
	DWORD		dwGuildId;
	WORD		wLevel;
	DWORD		dwExp;
	DWORD		dwPoint;
	BYTE		byMember[4];
	BYTE		byLevelIndex;

	WSTC_GUILD_USER() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_USER; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_USER); }
};


//-----------------------------------------------------------------------------
// ���� ������ Ŭ���̾�Ʈ�� ������.
//-----------------------------------------------------------------------------
struct WSTC_GUILD_USERINFO : public WORLD_PACKET_HEADER
{
	char		szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	char		szNickName[MAX_CHARACTER_NAME_LENGTH];		
	DWORD		dwUserIndex;
	BYTE		byRank;	
	DWORD		dwLev;
	WORD		wClass;
	DWORD		dwGuildId;	
	BYTE		bGuildCount;

	WSTC_GUILD_USERINFO() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_USERINFO; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_USERINFO); }
};


//-----------------------------------------------------------------------------
// ���� ���� ������ Ŭ���̾�Ʈ�� ������.
//-----------------------------------------------------------------------------
struct WSTC_GUILD_USERINFODEL : public WORLD_PACKET_HEADER
{
	DWORD		dwUserIndex;
	char		szCharacterName[MAX_CHARACTER_NAME_LENGTH];	

	WSTC_GUILD_USERINFODEL() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_USERINFODEL; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_USERINFODEL); }
};


//-----------------------------------------------------------------------------
// ��� ���� �޼����� ������(���� ����).
//-----------------------------------------------------------------------------
struct CTWS_GUILD_CREATE : public WORLD_PACKET_HEADER
{
	char		szGuildName[MAX_CHARACTER_NAME_LENGTH];	
	BYTE		bType;
	DWORD		dwLev;
	WORD		wClass;

	CTWS_GUILD_CREATE() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_CREATE; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILD_CREATE); }	
};


//-----------------------------------------------------------------------------
// ��� ���� ����� Ŭ���̾�Ʈ�� ������.
//-----------------------------------------------------------------------------
struct WSTC_GUILD_CREATE : public WORLD_PACKET_HEADER
{
	BOOL		bCreate;		
	BYTE		bType;	

	WSTC_GUILD_CREATE() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_WSGUILD_CREATE; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_CREATE); }
};


//-----------------------------------------------------------------------------
// ���� ������ Ŭ���̾�Ʈ�� ������.
//-----------------------------------------------------------------------------
struct WSTC_DGGUILD_USERINFO : public WORLD_PACKET_HEADER
{
	char		szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	char		szNickName[MAX_CHARACTER_NAME_LENGTH];		
	BYTE		byRank;
	DWORD		dwLev;
	WORD		wClass;
	DWORD		dwGuildId;	
	DWORD		dwUserIndex;

	WSTC_DGGUILD_USERINFO() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_DGGUILD_USERINFO; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_DGGUILD_USERINFO); }
};


//-----------------------------------------------------------------------------
// ��� ������ Ŭ���̾�Ʈ�� ������.
//-----------------------------------------------------------------------------
struct WSTC_DGGUILD_USER : public WORLD_PACKET_HEADER
{
	char		szGuildName[MAX_GUILD_NAME_LENGTH];	
	char		szRankName[4][MAX_GUILD_NAME_LENGTH];
	BYTE		byType;
	BYTE		byCount;
	DWORD		dwGuildId;	
	WORD		wLevel;

	WSTC_DGGUILD_USER() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_DGGUILD_USER; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_DGGUILD_USER); }
};


//-----------------------------------------------------------------------------
// ���/Ŭ�� ���� �޼����� ������.(���� ����).
//-----------------------------------------------------------------------------
struct CTWS_GUILDJOIN_MESSAGE : public WORLD_PACKET_HEADER
{
	char		szGuildName[MAX_GUILD_NAME_LENGTH];	
	char		szCharacterName[MAX_GUILD_NAME_LENGTH];
	BYTE		bType;	
	DWORD		dwUserIndex;

	CTWS_GUILDJOIN_MESSAGE() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILDJOIN_MESSAGE; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILDJOIN_MESSAGE); }
};


//-----------------------------------------------------------------------------
// ��� ���� �޼����� Ŭ���̾�Ʈ�� ������.
//-----------------------------------------------------------------------------
struct WSTC_GUILDJOIN_MESSAGE : public WORLD_PACKET_HEADER
{
	char		szGuildName[MAX_CHARACTER_NAME_LENGTH];	
	char		szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	BYTE		bType;
	DWORD		dwUserIndex;

	WSTC_GUILDJOIN_MESSAGE() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILDJOINRECV_MESSAGE; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILDJOIN_MESSAGE); }
};


//------------------------------------------------------------------------------//
// ��� ���� �޼���.														//
//------------------------------------------------------------------------------//
struct CTWS_GUILD_JOIN : public WORLD_PACKET_HEADER
{
	BOOL	bResult;
	BYTE	bGuildType;
	DWORD	dwLev;
	WORD	wClass;
	char	szGuildName[MAX_CHARACTER_NAME_LENGTH];	
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	DWORD	dwUserIndex;

	CTWS_GUILD_JOIN()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_JOIN;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILD_JOIN);	}
};


//-----------------------------------------------------------------------------
// ��� ���� ���.
//-----------------------------------------------------------------------------
struct WSTC_GUILD_JOINRESULT : public WORLD_PACKET_HEADER
{
	BOOL		bResult;		
	BYTE		bErrorCode;
	char		szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	DWORD		dwUserIndex;

	WSTC_GUILD_JOINRESULT() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_JOINRESULT; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_JOINRESULT); }
};


//-----------------------------------------------------------------------------
// ���� ������ Ŭ���̾�Ʈ�� ������.
//-----------------------------------------------------------------------------
struct WSTC_GUILD_JOININFO : public WORLD_PACKET_HEADER
{
	char		szGuildName[MAX_GUILD_NAME_LENGTH];		
	BYTE		byType;
	BYTE		byRank;
	DWORD		dwGuildId;

	WSTC_GUILD_JOININFO() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_JOININFO; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_JOININFO); }
};


//-----------------------------------------------------------------------------
// ���� ��� ����.
//-----------------------------------------------------------------------------
struct CTWS_GUILD_RANK : public WORLD_PACKET_HEADER
{
	char	szRankName[4][MAX_CHARACTER_NAME_LENGTH];
	char	szGuildName[MAX_CHARACTER_NAME_LENGTH];
			
	CTWS_GUILD_RANK() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_RANK; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILD_RANK); }
};


//-----------------------------------------------------------------------------
// ��� ��� ����
//-----------------------------------------------------------------------------
struct WSTC_GUILD_RANK : public WORLD_PACKET_HEADER
{
	BOOL		bResult;	
	BYTE		bSerType;
	
	WSTC_GUILD_RANK() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_RANKRT; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_RANK); }
};


//-----------------------------------------------------------------------------
// ���� ���� ����.
//-----------------------------------------------------------------------------
struct CTWS_GUILD_INFO : public WORLD_PACKET_HEADER
{
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	char	szNickName[MAX_CHARACTER_NAME_LENGTH];
	BYTE	bRank;
	BYTE	bMsgType;
	DWORD	dwUserIndex;

	CTWS_GUILD_INFO() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_INFO; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILD_INFO); }
};		


//-----------------------------------------------------------------------------
// ����� ������ �������� �����ش�.
//-----------------------------------------------------------------------------
struct WSTC_GUILD_INFO : public WORLD_PACKET_HEADER
{	
	char		szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	char		szNickName[MAX_CHARACTER_NAME_LENGTH];
	BYTE		bRank;
	BYTE		bMsgType;
	BYTE		bSerType;
	DWORD		dwUserIndex;
	
	WSTC_GUILD_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_INFORT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_INFO); }
};


//-----------------------------------------------------------------------------
// ���� ����.
//-----------------------------------------------------------------------------
struct CTWS_GUILD_USERDEL : public WORLD_PACKET_HEADER
{			
	BYTE	byUserType;
	DWORD	dwGuildId;	
	DWORD	dwUserIndex;
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];

	CTWS_GUILD_USERDEL() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_USERDEL; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILD_USERDEL); }
};


//-----------------------------------------------------------------------------
// ������ ���� �Ѵ�.
//-----------------------------------------------------------------------------
struct WSTC_USER_DELETE : public WORLD_PACKET_HEADER
{	
	char		szCharacterName[MAX_CHARACTER_NAME_LENGTH];	
	DWORD		dwGuildId;
	DWORD		dwUserIndex;	
	BYTE		byMember[4];
	BYTE		byChk;

	WSTC_USER_DELETE()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_USER_DELETE;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_USER_DELETE);	}
};


//-----------------------------------------------------------------------------
// ��� ���� �޼���.
//-----------------------------------------------------------------------------
struct CTWS_GUILD_DELETE : public WORLD_PACKET_HEADER
{
	char		szGuildName[MAX_GUILD_NAME_LENGTH];	
	DWORD		dwGuildId;
	
	CTWS_GUILD_DELETE() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_DELETE; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILD_DELETE); }	
};


//-----------------------------------------------------------------------------
// ��ɾ Ŭ���̾�Ʈ���� ����(���弭��).
//-----------------------------------------------------------------------------
struct WSTC_GUILD_DELETE : public WORLD_PACKET_HEADER
{	
	DWORD	dwGuildId;
	BYTE	bServerType;

	WSTC_GUILD_DELETE()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_DELETERT; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_DELETE);	}
};


//-----------------------------------------------------------------------------
// ��Ƽ�� ���ι޴´�.
//-----------------------------------------------------------------------------
struct CTWS_PARTY_JOIN : public WORLD_PACKET_HEADER
{
	char		szName[MAX_CHARACTER_NAME_LENGTH];
	BYTE		bErrCode;	
	DWORD		dwUserIndex;

	CTWS_PARTY_JOIN() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_PARTY_JOIN; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_PARTY_JOIN); }
};


//-----------------------------------------------------------------------------
// ��Ƽ�� ���� ����� ������.
//-----------------------------------------------------------------------------
struct WSTC_PARTY_JOIN : public WORLD_PACKET_HEADER
{
	BYTE		bResult;	
	DWORD		dwUserIndex;
	char		szName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_PARTY_JOIN() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_PARTY_JOINRT; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PARTY_JOIN); }
};


//-----------------------------------------------------------------------------
// ��Ƽ�� �������� ���.
//-----------------------------------------------------------------------------
struct CTWS_PARTY_APPROVE : public WORLD_PACKET_HEADER
{
	DWORD		dwUserIndex;
	char		szName[MAX_CHARACTER_NAME_LENGTH];	

	CTWS_PARTY_APPROVE() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_PARTY_APPROVE; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_PARTY_APPROVE); }
};


//-----------------------------------------------------------------------------
// ������ ���� ���.
//-----------------------------------------------------------------------------
struct WSTC_PARTY_JOINID : public WORLD_PACKET_HEADER
{	
	DWORD		dwPartyId;
	char		szName[MAX_CHARACTER_NAME_LENGTH];
	WORD		wClass;
	BYTE		bResultCode;	
	DWORD		dwUserIndex;
	BYTE		byLeader;

	WSTC_PARTY_JOINID() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_PARTY_JOINID; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PARTY_JOINID); }
};


//-----------------------------------------------------------------------------
// ��Ƽ���� ������.
//-----------------------------------------------------------------------------
struct WSTC_PARTY_LOGOUT : public WORLD_PACKET_HEADER
{
	char		szName[MAX_CHARACTER_NAME_LENGTH];	
	BYTE		bResultCode;
	DWORD		dwUserIndex;

	WSTC_PARTY_LOGOUT() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_PARTY_LOGOUT; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PARTY_LOGOUT); }
};


//-----------------------------------------------------------------------------
// ��Ƽ�� ���� ���.
//-----------------------------------------------------------------------------
struct CTWS_PARTY_LOGOUT : public WORLD_PACKET_HEADER
{
	DWORD		dwPartyId;

	CTWS_PARTY_LOGOUT() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_PARTY_LOGOUTCL; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_PARTY_LOGOUT); }
};


//-----------------------------------------------------------------------------
// Skill Key�� ������. 
//-----------------------------------------------------------------------------
struct WSTC_SKILL_KEY : public WORLD_PACKET_HEADER
{
	BYTE	bSkillKey[16];
	BYTE	bPosKey[16];

	WSTC_SKILL_KEY()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_SKILL_KEY; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_SKILL_KEY);	}
};


//-----------------------------------------------------------------------------
// Skill Key�� ������. 
//-----------------------------------------------------------------------------
struct CTWS_SKILL_KEY : public WORLD_PACKET_HEADER
{
	BYTE	bSkillKey[16];
	BYTE	bPosKey[16];

	CTWS_SKILL_KEY()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_SKILL_KEYRT;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_SKILL_KEY);	}
};


//-----------------------------------------------------------------------------
// �Ӹ� ���, �ź� �޼���.
//-----------------------------------------------------------------------------
struct CTWS_COMMAND_MSG : public WORLD_PACKET_HEADER
{	
	BOOL		bCommand;

	CTWS_COMMAND_MSG() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_COMMAND_MSG; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_COMMAND_MSG); }
};


//-----------------------------------------------------------------------------
// �Ӹ� ���, �ź� �޼���.
//-----------------------------------------------------------------------------
struct WSTC_COMMAND_MSG : public WORLD_PACKET_HEADER
{
	BOOL		bCommand;

	WSTC_COMMAND_MSG() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_COMMAND_MSGRT; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_COMMAND_MSG); }
};


//-----------------------------------------------------------------------------
// ��Ƽ ���� �޼���.
//-----------------------------------------------------------------------------
struct CTWS_PARTY_SET : public WORLD_PACKET_HEADER
{	
	BOOL		bParty;
	
	CTWS_PARTY_SET()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_SET;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_PARTY_SET); }
};


//-----------------------------------------------------------------------------
// ��Ƽ ���� �޼���.
//-----------------------------------------------------------------------------
struct WSTC_PARTY_SET : public WORLD_PACKET_HEADER
{
	
	BOOL		bParty;
	
	WSTC_PARTY_SET()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_SETRT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PARTY_SET); }
};


struct WSTC_PARTY_LEVEL : public WORLD_PACKET_HEADER
{
	DWORD		dwPartyId;
	DWORD		dwUserIndex;
	DWORD		dwLevel;

	WSTC_PARTY_LEVEL() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_PARTY_LEVEL; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PARTY_LEVEL); }
};


//-----------------------------------------------------------------------------
// ��Ƽ ���� ����.
//-----------------------------------------------------------------------------
struct WSTC_PARTY_LEADER : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;
	char		szCharName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_PARTY_LEADER()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_LEADER;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PARTY_LEADER);	}
};


//-----------------------------------------------------------------------------
// ��Ƽ�� ����.
//-----------------------------------------------------------------------------
struct CTWS_PARTY_INFO : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;
	char		szCharName[MAX_CHARACTER_NAME_LENGTH];

	CTWS_PARTY_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_PARTY_INFO);	}
};


//-----------------------------------------------------------------------------
// ��Ƽ�� ����.
//-----------------------------------------------------------------------------
struct WSTC_PARTY_INFO : public WORLD_PACKET_HEADER
{	
	WORD		wDungeonID;
	BYTE		byLayerIndex;
	DWORD		dwUserIndex;
	char		szCharName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_PARTY_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_INFORT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PARTY_INFO);	}
};


//-----------------------------------------------------------------------------
// ��Ƽ�� ���� ���.
//-----------------------------------------------------------------------------
struct CTWS_PARTY_LOGOUTRT : public WORLD_PACKET_HEADER
{
	DWORD		dwPartyId;
	DWORD		dwUserIndex;

	CTWS_PARTY_LOGOUTRT() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_PARTY_LOGOUTUSER; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_PARTY_LOGOUTRT); }
};


//-----------------------------------------------------------------------------
// ��Ƽ HP.
//-----------------------------------------------------------------------------
struct WSTC_PARTY_HP : public WORLD_PACKET_HEADER
{
	DWORD		dwPartyId;
	DWORD		dwUserIndex;	
	WORD		wHp;
	WORD		wMaxHp;

	WSTC_PARTY_HP() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_PARTY_HP; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PARTY_HP); }
};


struct WSTC_PARTY_PORTAL : public WORLD_PACKET_HEADER
{	
	DWORD	dwPartyId;
	DWORD	dwUserIndex;
	DWORD	dwPartalUserIndex;
	BYTE	byIndex;
	BYTE	byType;
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_PARTY_PORTAL()	{ bGameStatus = UPDATE_GAME_WORLD;		bHeader = Protocol_World::CMD_PARTY_PORTAL;	}
	DWORD	GetPacketSize()	{	return (sizeof(WSTC_PARTY_PORTAL)); }
};


struct WSTC_PARTY_PORTALUSED : public WORLD_PACKET_HEADER
{	
	DWORD	dwPartyId;
	DWORD	dwUserIndex;
	DWORD	dwPartalUserIndex;
	BYTE	dwResult;
	BYTE	byIndex;
	BYTE	byType;
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_PARTY_PORTALUSED()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_PORTALUSED;	}
	DWORD	GetPacketSize()	{	return (sizeof(WSTC_PARTY_PORTALUSED)); }
};


struct WSTC_PARTY_PORTALMOVE : public WORLD_PACKET_HEADER
{		
	WORD	wDungeonID;
	BYTE	byLayerIndex;
	float	fPosX;
	float	fPosZ;
	DWORD	dwPartyId;

	WSTC_PARTY_PORTALMOVE()	{ bGameStatus = UPDATE_GAME_WORLD;		bHeader = Protocol_World::CMD_PARTY_PORTALMOVE;	}
	DWORD	GetPacketSize()	{	return (sizeof(WSTC_PARTY_PORTALMOVE)); }
};


struct WSTC_HELP_INFO : public WORLD_PACKET_HEADER
{	
	BYTE	bHelpInfo[3];	

	WSTC_HELP_INFO()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_HELP_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_HELP_INFO);	}
};


struct WSTC_PARTY_PORTALFAIL : public WORLD_PACKET_HEADER
{	
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	BYTE	byResult;
	
	WSTC_PARTY_PORTALFAIL()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_PORTALFAIL; }
	DWORD	GetPacketSize()	{ return sizeof(WSTC_PARTY_PORTALFAIL); }
};


//-----------------------------------------------------------------------------
// �̺�Ʈ ���� ���� ����.
//-----------------------------------------------------------------------------
struct WSTC_EVENT_DUNGEON_ENTER_FAILED : public WORLD_PACKET_HEADER
{
	BYTE	byErrorCode;	// 0 : ������ ������ �ƴ�.  1 : ��Ƽ ���� ������.

	WSTC_EVENT_DUNGEON_ENTER_FAILED()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_EVENT_DUNGEON_ENTER_FAILED; }	
	DWORD	GetPacketSize()				{ return sizeof(WSTC_EVENT_DUNGEON_ENTER_FAILED);	}
};


struct WSTC_GUILD_OFFLINEUSER : public WORLD_PACKET_HEADER
{
	DWORD	dwUserIndex;
	char	szName[MAX_CHARACTER_NAME_LENGTH];	
	WORD	wClass;
	DWORD	dwLevel;
	BYTE	byRank;
	char	szNickName[MAX_CHARACTER_NAME_LENGTH];		

	WSTC_GUILD_OFFLINEUSER()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_OFFLINE_USER; }
	DWORD	GetPacketSize()		{ return sizeof(WSTC_GUILD_OFFLINEUSER); }
};


struct WSTC_GUILD_OFFLINEUSER_DELETE : public WORLD_PACKET_HEADER
{
	DWORD	dwUserIndex;
	
	WSTC_GUILD_OFFLINEUSER_DELETE()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_OFFLINE_USER_DELETE; }
	DWORD	GetPacketSize()		{ return sizeof(WSTC_GUILD_OFFLINEUSER_DELETE); }
};


struct CTWS_PARTY_USERREQUEST : public WORLD_PACKET_HEADER
{
	DWORD		dwUserIndex;
	DWORD		dwReqUserIndex;
	DWORD		dwPartyId;

	CTWS_PARTY_USERREQUEST()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_USERREQUEST;	}
	DWORD	GetPacketSize()		{	return sizeof(CTWS_PARTY_USERREQUEST);	}
};


struct WSTC_PARTY_USERREQUEST : public WORLD_PACKET_HEADER
{		
	DWORD		dwUserIndex;
	DWORD		dwReqUserIndex;
	char		szName[MAX_CHARACTER_NAME_LENGTH];
	char		szRequestName[MAX_CHARACTER_NAME_LENGTH];		

	WSTC_PARTY_USERREQUEST()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_USERREQUESTWS;	}
	DWORD	GetPacketSize()		{	return sizeof(WSTC_PARTY_USERREQUEST);	}
};


struct WSTC_PARTY_USERREQUEST_FAIL : public WORLD_PACKET_HEADER
{		
	BYTE		byType;	

	WSTC_PARTY_USERREQUEST_FAIL()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_USERREQUEST_FAIL;	}
	DWORD	GetPacketSize()			{	return sizeof(WSTC_PARTY_USERREQUEST_FAIL);	}
};


struct CTWS_PARTY_USERREQUEST_FAIL : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;
	BYTE		byType;	

	CTWS_PARTY_USERREQUEST_FAIL()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_USERREQUESTWS_FAIL;	}
	DWORD	GetPacketSize()			{	return sizeof(CTWS_PARTY_USERREQUEST_FAIL);	}
};



//-----------------------------------------------------------------------------
// ���� ����.
//-----------------------------------------------------------------------------
struct CTWS_GUILDUSER_INFO : public WORLD_PACKET_HEADER
{
	DWORD		dwUserIndex;	

	CTWS_GUILDUSER_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDUSER_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILDUSER_INFO);	}
};


//-----------------------------------------------------------------------------
// ���� ����.
//-----------------------------------------------------------------------------
struct WSTC_GUILDUSER_INFO : public WORLD_PACKET_HEADER
{
	WORD		wDungeonID;
	BYTE		byLayerIndex;
	DWORD		dwUserIndex;
	char		szCharName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_GUILDUSER_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDUSER_INFORT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILDUSER_INFO);	}
};


struct CTWS_GUILD_REQUEST : public WORLD_PACKET_HEADER
{
	DWORD		dwGuildId;
	DWORD		dwUserIndex;	
	DWORD		dwRequestUserIndex;

	CTWS_GUILD_REQUEST()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_REQUEST;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILD_REQUEST);	}
};


struct WSTC_GUILD_REQUEST : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;	
	DWORD		dwRequestUserIndex;
	char		szName[MAX_CHARACTER_NAME_REAL_LENGTH];
	char		szRequestName[MAX_CHARACTER_NAME_REAL_LENGTH];

	WSTC_GUILD_REQUEST()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_REQUESTRT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_REQUEST);	}
};


struct CTWS_GUILD_USERREQUEST_FAIL : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;
	BYTE		byType;	

	CTWS_GUILD_USERREQUEST_FAIL()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_USERREQUESTWS_FAIL;	}
	DWORD	GetPacketSize()			{	return sizeof(CTWS_GUILD_USERREQUEST_FAIL);	}
};


struct WSTC_GUILD_USERREQUEST_FAIL : public WORLD_PACKET_HEADER
{		
	BYTE		byType;	

	WSTC_GUILD_USERREQUEST_FAIL()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_USERREQUEST_FAIL;	}
	DWORD	GetPacketSize()			{	return sizeof(WSTC_GUILD_USERREQUEST_FAIL);	}
};


struct CTWS_GUILD_DATA_REQUEST : public WORLD_PACKET_HEADER
{		
	DWORD		dwGuildId;

	CTWS_GUILD_DATA_REQUEST()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_DATA_REQUEST;	}
	DWORD	GetPacketSize()			{	return sizeof(CTWS_GUILD_DATA_REQUEST);	}
};


struct WSTC_GUILD_DATA_REQUEST : public WORLD_PACKET_HEADER
{		
	DWORD		dwGuildId;
	char		szName[MAX_GUILD_NAME_LENGTH];

	WSTC_GUILD_DATA_REQUEST()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_DATA_REQUESTRT;	}
	DWORD	GetPacketSize()			{	return sizeof(WSTC_GUILD_DATA_REQUEST);	}
};


struct WSTC_GUILD_COUNT : public WORLD_PACKET_HEADER
{
	BYTE	byCount;
	BYTE	byMember[4];	
	
	WSTC_GUILD_COUNT()			{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_COUNT; }
	DWORD	GetPacketSize()		{ return sizeof(WSTC_GUILD_COUNT); }
};


struct WSTC_GUILD_EXP : public WORLD_PACKET_HEADER
{	
	DWORD		dwGuildId;
	DWORD		dwExp;
	WORD		wLevel;
	BYTE		byType;
	BYTE		byIndex;
	
	WSTC_GUILD_EXP()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_EXP;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_EXP); }
};


struct WSTC_GUILD_LEVEL : public WORLD_PACKET_HEADER
{
	DWORD		dwGuildId;
	DWORD		dwUserIndex;
	DWORD		dwLevel;

	WSTC_GUILD_LEVEL() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_LEVEL; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_LEVEL); }
};


struct WSTC_RANK_FAIL : public WORLD_PACKET_HEADER
{
	BYTE		byType;
	BYTE		byRank;

	WSTC_RANK_FAIL()		{	bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_RANK_FAIL; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_RANK_FAIL); }
};


struct WSTC_MESSENGER_USER : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;
	char		szName[MAX_CHARACTER_NAME_LENGTH];
	BYTE		byType;
	WORD		wClass;	
	BYTE		byStatus;

	WSTC_MESSENGER_USER() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_MESSENGER_USER; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_MESSENGER_USER); }
};


struct WSTC_MESSENGER_USERINFO : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;
	BYTE		byType;

	WSTC_MESSENGER_USERINFO() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_MESSENGER_USERINFO; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_MESSENGER_USERINFO); }
};


struct CTWS_MESSENGER_USERADD : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;
	char		szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	
	CTWS_MESSENGER_USERADD() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_MESSENGER_USERADD; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_MESSENGER_USERADD); }
};


struct WSTC_MESSENGER_USERFAIL : public WORLD_PACKET_HEADER
{	
	BYTE		byFail;
	char		szName[MAX_CHARACTER_NAME_LENGTH];
	
	WSTC_MESSENGER_USERFAIL()	{ bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_MESSENGER_USERFAIL; }
	DWORD	GetPacketSize()		{	return sizeof(WSTC_MESSENGER_USERFAIL); }
};


struct WSTC_MESSENGER_USERADD : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;
	char		szCharacterName[MAX_CHARACTER_NAME_LENGTH];	
	
	WSTC_MESSENGER_USERADD() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_MESSENGER_USERADDRT; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_MESSENGER_USERADD); }
};


//-----------------------------------------------------------------------------
// ĳ���� ���� ���� üũ.
//-----------------------------------------------------------------------------
struct CTWS_CHARACTER_SEARCH : public WORLD_PACKET_HEADER
{	
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	CTWS_CHARACTER_SEARCH()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_CHARACTER_SEARCH;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_CHARACTER_SEARCH);	}
};


//-----------------------------------------------------------------------------
// ĳ���� ���� ���� üũ.
//-----------------------------------------------------------------------------
struct WSTC_SEARCH : public WORLD_PACKET_HEADER
{	
	BYTE	byMsg;
	BYTE	byType;	
	WORD	wDungeonID;
	DWORD	dwUserIndex;
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_SEARCH()			{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_CHARACTER_SEARCHRT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_SEARCH);	}
};


struct CTWS_MESSENGER_ANSWER : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;
	char		szName[MAX_CHARACTER_NAME_LENGTH];
	BYTE		byType;

	CTWS_MESSENGER_ANSWER() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_MESSENGER_ANSWER; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_MESSENGER_ANSWER); }
};


struct WSTC_MESSENGER_FAIL : public WORLD_PACKET_HEADER
{
	char		szName[MAX_CHARACTER_NAME_LENGTH];	

	WSTC_MESSENGER_FAIL() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_MESSENGER_FAIL; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_MESSENGER_FAIL); }
};


struct WSTC_MESSENGER_SUCCESS : public WORLD_PACKET_HEADER
{
	DWORD		dwUserIndex;
	char		szName[MAX_CHARACTER_NAME_LENGTH];
	BYTE		byType;
	WORD		wClass;

	WSTC_MESSENGER_SUCCESS() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_MESSENGER_SUCCESS; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_MESSENGER_SUCCESS); }
};


struct WSTC_GUILDUSER_LEVEL : public WORLD_PACKET_HEADER
{
	DWORD		dwUserIndex;		
	DWORD		dwLevel;

	WSTC_GUILDUSER_LEVEL()	{	bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILDUSER_LEVEL; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILDUSER_LEVEL); }
};


struct CTWS_FRIENDUSER_INFO : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;	

	CTWS_FRIENDUSER_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_FRIENDUSER_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_FRIENDUSER_INFO);	}
};


struct WSTC_FRIENDUSER_INFO : public WORLD_PACKET_HEADER
{
	WORD		wDungeonID;
	BYTE		byLayerIndex;
	DWORD		dwUserIndex;
	char		szCharName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_FRIENDUSER_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_FRIENDUSER_INFORT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_FRIENDUSER_INFO);	}
};


struct CTWS_FRIENDUSER_DELETE : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;	

	CTWS_FRIENDUSER_DELETE()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_FRIENDUSER_DELETE;	}
	DWORD	GetPacketSize()		{	return sizeof(CTWS_FRIENDUSER_DELETE);	}
};


struct WSTS_FRIENDUSER_DELETE : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;	

	WSTS_FRIENDUSER_DELETE()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_FRIENDUSER_DELETERT;	}
	DWORD	GetPacketSize()		{	return sizeof(WSTS_FRIENDUSER_DELETE);	}
};


struct CTWS_MESSENGER_STATUS : public WORLD_PACKET_HEADER
{
	BYTE		byType;
	DWORD		dwUserIndex;

	CTWS_MESSENGER_STATUS()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_MESSENGER_STATUS;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_MESSENGER_STATUS);	}
};

struct WSTC_MESSENGER_STATUS : public WORLD_PACKET_HEADER
{
	BYTE		byType;
	DWORD		dwUserIndex;

	WSTC_MESSENGER_STATUS()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_MESSENGER_STATUSRT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_MESSENGER_STATUS);	}
};


//-----------------------------------------------------------------------------
// ��帶���� �̾�.
//-----------------------------------------------------------------------------
struct WSTC_USER_MASTER : public WORLD_PACKET_HEADER
{	
	char		szCharacterName[MAX_CHARACTER_NAME_LENGTH];	
	DWORD		dwGuildId;
	DWORD		dwUserIndex;
	DWORD		dwMasterUserIndex;	
	BYTE		byMember[4];	
	BYTE		byType;

	WSTC_USER_MASTER()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_USER_MASTER;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_USER_MASTER);	}
};


struct CTWS_USER_GUILDMASTER : public WORLD_PACKET_HEADER
{
	DWORD		dwGuildId;
	DWORD		dwUserIndex;	
	
	CTWS_USER_GUILDMASTER()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_USER_GUILDMASTER;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_USER_GUILDMASTER);	}
};


struct WSTC_USER_GUILDMASTER : public WORLD_PACKET_HEADER
{
	char		szName[MAX_CHARACTER_NAME_LENGTH];
	char		szMasterName[MAX_CHARACTER_NAME_LENGTH];	
	DWORD		dwUserIndex;
	DWORD		dwMasterUserIndex;
	BYTE		byRank;
	BYTE		byMasterRank;

	WSTC_USER_GUILDMASTER()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_USER_GUILDMASTERRT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_USER_GUILDMASTER);	}
};


//-----------------------------------------------------------------------------
// CM������ ���弭������ �ش� User�� ���� Logout �϶�� ���� 
//-----------------------------------------------------------------------------
struct CTWS_KICK_USER : public WORLD_PACKET_HEADER
{
	DWORD	dwCharIndex;

	CTWS_KICK_USER()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_CM_KICK_USER;		}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_KICK_USER);	}
};

//-----------------------------------------------------------------------------
// Ŭ���̾�Ʈ�� ���弭������ ���� �ڽ��� �ʷε������� �ƴ����� �˷��ش�. 
//-----------------------------------------------------------------------------
struct CTWS_LOADING_STATUS : public WORLD_PACKET_HEADER
{
	BYTE	bIsLoading;	

	CTWS_LOADING_STATUS()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_LOADING_STATUS;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_LOADING_STATUS);	}
};


//-----------------------------------------------------------------------------
// ��ϵ� GM�� �ƴ� 
//-----------------------------------------------------------------------------
struct WSTC_INVALID_GM : public WORLD_PACKET_HEADER
{
	WSTC_INVALID_GM()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_INVALID_GM;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_INVALID_GM);	}
};


//-----------------------------------------------------------------------------
// ��ϵ� GM�� 
//-----------------------------------------------------------------------------
struct WSTC_GM_LOGIN_SUCCESS : public WORLD_PACKET_HEADER
{
	WSTC_GM_LOGIN_SUCCESS()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GM_LOGIN_SUCCESS;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GM_LOGIN_SUCCESS);	}
};


//-----------------------------------------------------------------------------
// GM�� ����ȯ���� üũ�Ѵ�. 
//-----------------------------------------------------------------------------
struct CTWS_CHECK_GM : public WORLD_PACKET_HEADER
{
	CTWS_CHECK_GM()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_CHECK_GM;		}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_CHECK_GM);	}	
};


//-----------------------------------------------------------------------------
// Client Connection Check�� KeepAlive Packet
//-----------------------------------------------------------------------------
struct ECHO_KEEP_ALIVE : public WORLD_PACKET_HEADER
{
	ECHO_KEEP_ALIVE()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_CHECK_CONNECTION;	}
	DWORD	GetPacketSize()	{	return sizeof(ECHO_KEEP_ALIVE);	}
};

#pragma pack(pop)