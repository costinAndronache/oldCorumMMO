#pragma once


#include "oledb.h"
#include "ProtocolDefinition.h"	


#pragma pack(push,1)		


//-----------------------------------------------------------------------------
// ���� �������� ������ ���� ������ �������� ��û�Ѵ�.
//-----------------------------------------------------------------------------
struct ASTWS_REQUEST_SERVER_ALIVE
{
	WORD	wHeader;
	WORD	wInfoType;	// ���� Ÿ��. ����� ������ ����.	

	ASTWS_REQUEST_SERVER_ALIVE()	{ wHeader = Protocol_Agent_Server::ACMD_REQUEST_SERVER_ALIVE; }
	DWORD	GetPacketSize()			{ return sizeof(ASTWS_REQUEST_SERVER_ALIVE); }	
};


struct WSTAS_REQUEST_CHINA_BILLING
{
	WORD	wHeader;
	
	DWORD	dwUserIndex;
	DWORD	dwPropID;

	WSTAS_REQUEST_CHINA_BILLING()	{ wHeader = Protocol_Agent_Server::ACMD_REQUEST_CHINA_BILLING; }
	DWORD	GetPacketSize()			{ return sizeof(WSTAS_REQUEST_CHINA_BILLING); }	
};


struct WSTAS_REQUEST_CHINA_BILLINGINFO
{
	WORD	wHeader;
	
	DWORD	dwUserIndex;
	DWORD	dwPropID;

	WSTAS_REQUEST_CHINA_BILLINGINFO()	{ wHeader = Protocol_Agent_Server::ACMD_REQUEST_CHINA_BILLINGINFO; }
	DWORD	GetPacketSize()				{ return sizeof(WSTAS_REQUEST_CHINA_BILLINGINFO); }	
};


//-----------------------------------------------------------------------------
// ���� �������� ���� �޼����� ������� �Ѵ�.
//-----------------------------------------------------------------------------
struct ASTWS_CMTOOL_NOTICE
{
	WORD	bHeader;	
	WORD	wMsgLen;
	char	szMsg[1024];
	
	ASTWS_CMTOOL_NOTICE()			{ bHeader = Protocol_Agent_Server::ACMD_CMTOOL_NOTICE; }
	DWORD	GetPacketSize()			{ return 4 + wMsgLen; }
};


//-----------------------------------------------------------------------------
// ��忡 ���ԵǾ�
//-----------------------------------------------------------------------------
struct ASTWS_GUILD_CHARACTER_DELETE
{
	WORD	wHeader;	
	DWORD	dwGuildId;
	BYTE	byRank;
	DWORD	dwIndex;
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];

	ASTWS_GUILD_CHARACTER_DELETE()	{ wHeader = Protocol_Agent_Server::ACMD_GUILD_CHARACTER_DELETE; }
	DWORD	GetPacketSize()			{ return sizeof(ASTWS_GUILD_CHARACTER_DELETE); }	
};


//-----------------------------------------------------------------------------
// ���� ���� �ð��� 10�� �̸��϶� �����ִ� ��Ŷ 
//-----------------------------------------------------------------------------
struct ASTWS_BILLING_SMALL_TIME
{
	WORD wHeader;
	BYTE bBillingType;		//���� ���� Ÿ�� 
	DWORD dwChar_Index;		//CharacterIndex
	DWORD dwSecond;			//�� �� ���ҳ�...

	ASTWS_BILLING_SMALL_TIME()	{ wHeader = Protocol_Agent_Server::ACMD_BILLING_SMALL_TIME;	}
	DWORD GetPacketSize()		{	return sizeof(ASTWS_BILLING_SMALL_TIME); }
};


//-----------------------------------------------------------------------------
// ���� ���� �ð��� 10�� �̸��϶� �����ִ� ��Ŷ 
//-----------------------------------------------------------------------------
struct ASTWS_CHANGE_BILLING_TYPE
{
	WORD wHeader;
	BYTE bBillingType;		//�ٲ� ���� Ÿ�� 
	DWORD dwChar_Index;		//CharacterIndex
	DWORD dwRemainTime;		//�� �� ���ҳ�...
	DBTIMESTAMP eDay;		//���� ��¥ 

	ASTWS_CHANGE_BILLING_TYPE()	{ wHeader = Protocol_Agent_Server::ACMD_CHANGE_BILLING_TYPE;	}
	DWORD GetPacketSize()		{	return sizeof(ASTWS_CHANGE_BILLING_TYPE); }
};


//-----------------------------------------------------------------------------
// ���� ������ �α��� �������� ������ ���� ������ �����´�.
//-----------------------------------------------------------------------------
struct WSTAS_SERVER_ALIVE_INFO
{
	WORD	wHeader;
	DWORD	dwServerID;
	WORD	wServerType;			// 0 : ���� ����, 1 : ���� ����.
	WORD	wConnectedUserCount;	// ���ӵǾ� �ִ� ������ ��.
	
	DWORD	dwMagicItemDropRate;	// ���� ������ ��ӷ�.
	DWORD	dwExpGainRate;			// ����ġ ȹ���.

	WSTAS_SERVER_ALIVE_INFO()		{ wHeader = Protocol_Agent_Server::ACMD_SERVER_ALIVE_INFO;	}
	DWORD	GetPacketSize()			{ return sizeof(WSTAS_SERVER_ALIVE_INFO); }
};


enum BILLING_INFO_PROCEDURE_TYPE 
{	
	Billing_Info_Procedure_Type_NONE		= 0,
	Billing_Info_Procedure_Type_Normal		= 1,
	Billing_Info_Procedure_Type_Level_Limit	= 2,
};

//-----------------------------------------------------------------------------
// Packet: ASTWS_CHINA_BILLING_INFO.
// Desc: �߱� ���ȭ ����(���� ���� ������ ���弭���� ������).
//-----------------------------------------------------------------------------
struct ASTWS_CHINA_BILLING_INFO
{
	WORD	wHeader;

	DWORD	dwUserIndex;

	BYTE	byType;
	DWORD	dwPoint;	

	BYTE	byBillingProcedureType;		// 0: not procedure, 1 : normal, 2 : detected level limit
	
	ASTWS_CHINA_BILLING_INFO()	{	wHeader = Protocol_Agent_Server::ACMD_CHINA_BILLING_INFO;	}
	DWORD	GetPacketSize()		{	return sizeof(ASTWS_CHINA_BILLING_INFO);					}

	void	SetProcedureType(enum BILLING_INFO_PROCEDURE_TYPE eProcedureType)
	{
		byBillingProcedureType = (BYTE)eProcedureType;
	}

	BYTE	GetProcedureType() const
	{
		return byBillingProcedureType;
	}
};


//-----------------------------------------------------------------------------
// ���弭���� Login Agent ������ Attach�� �õ��Ҷ� ���� ��Ŷ 
//-----------------------------------------------------------------------------
struct WSTAS_SERVER_ATTACH
{
	WORD	wHeader;
	DWORD	dwWorldServerID;			
	
	WSTAS_SERVER_ATTACH()	{	wHeader = Protocol_Agent_Server::ACMD_SERVER_ATTACH;		}
	DWORD	GetPacketSize()	{	return sizeof(WSTAS_SERVER_ATTACH);	}
};


//-----------------------------------------------------------------------------
// ���弭���� Agent �������� �ش������� ������ �����ߴٰ� �˸� 
//-----------------------------------------------------------------------------
struct WSTAS_USER_ACCEPT_ALLOWED
{
	WORD	wHeader;
	DWORD	dwPropID;			//Agent���� ������ �̾ƿ������� Key�� (MEMBER DB�� �ִ� Chr_Log_Info Index��)

	WSTAS_USER_ACCEPT_ALLOWED()	{	wHeader = Protocol_Agent_Server::ACMD_USER_ACCEPT_ALLOWED;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTAS_USER_ACCEPT_ALLOWED);	}
};


//-----------------------------------------------------------------------------
// ���弭���� Agent �������� �ش������� ������ �����ߴٰ� �˸� 
//-----------------------------------------------------------------------------
struct WSTAS_USER_ACCEPT_FAILED
{
	WORD	wHeader;
	DWORD	dwPropID;		//Agent���� ���� �����ϱ� ���� Key �� 
	BYTE	bErrorCode;

	WSTAS_USER_ACCEPT_FAILED()	{	wHeader = Protocol_Agent_Server::ACMD_USER_ACCEPT_FAILED;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTAS_USER_ACCEPT_FAILED);	}
};


//-----------------------------------------------------------------------------
// ���弭���� Agent �������� �ش������� World���� �α��ο� �����ߴٴ� �޼����� ���� 
//-----------------------------------------------------------------------------
struct WSTAS_USER_WORLD_LOGIN_SUCCESS
{
	WORD	wHeader;
	DWORD	dwPropID;			//User�� MemberDB PropID		
	DWORD	dwUserIndex;

	WSTAS_USER_WORLD_LOGIN_SUCCESS()	{	wHeader = Protocol_Agent_Server::ACMD_USER_WORLD_LOGIN_SUCCESS;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTAS_USER_WORLD_LOGIN_SUCCESS);	}
};


//-----------------------------------------------------------------------------
// ���弭������ ������ Disconnect �Ǿ��ٴ� �޼����� Agent ������ �˷��ش�. 
//-----------------------------------------------------------------------------
struct WSTAS_WORLD_USER_DISCONNECT
{
	WORD	wHeader;
	DWORD	dwPropID;			//User�� MemberDB PropID
		
	WSTAS_WORLD_USER_DISCONNECT()	{	wHeader = Protocol_Agent_Server::ACMD_WORLD_USER_DISCONNECT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTAS_WORLD_USER_DISCONNECT);	}	
};


//-----------------------------------------------------------------------------
// Agent ������ World�������� �ش� User�� ���� Logout �϶�� ���� 
//-----------------------------------------------------------------------------
struct ASTWS_KICK_USER
{
	WORD	wHeader;
	DWORD	dwCharIndex;
	BYTE	btKickCode;

	ASTWS_KICK_USER()		{	wHeader = Protocol_Agent_Server::ACMD_KICK_USER;	btKickCode = 0; 	}
	DWORD	GetPacketSize()	{	return sizeof(ASTWS_KICK_USER);	}
};


//-----------------------------------------------------------------------------
// ���弭���� Login Agent ������ Attach ��� ��Ŷ 
//-----------------------------------------------------------------------------
struct ASTWS_SERVER_ATTACH_RESULT
{
	WORD	wHeader;
	BYTE	bErrorCode;		//0�̸� Success, �ƴϸ� Error		
	BYTE	bAdultMode;	
	ASTWS_SERVER_ATTACH_RESULT()	{	wHeader = Protocol_Agent_Server::ACMD_SERVER_ATTACH_RESULT;		}
	DWORD	GetPacketSize()	{	return sizeof(ASTWS_SERVER_ATTACH_RESULT);	}
};


//-----------------------------------------------------------------------------
// Agent������ World ������ ������ �߰��϶�� Packet�� ���� 
//-----------------------------------------------------------------------------
struct BILLING_INFO
{
	BYTE		bPCRoom;				//PC�� ���� ���� 
	BYTE		bUserBillType;			//���� ���� Ÿ�� 
	BYTE		bPCRoomBillType;		//PC�� ���� Ÿ�� 	

	DWORD		dwTimePerUserMeterRate;		//���� ������ ������ Time
	DWORD		dwTimeForPCRoomMeteRate;	//PC�� ������ Time
	DWORD		dwTimeFor1HourFree;			//1�ð� ����� Time

	BYTE		bCurBillingOrderIndex;		//���� BillingOrder Index (���弭����)
	BYTE		BillingOrder[MAX_BILLING_ORDER_COUNT];	//���� ���� 
	
	DBTIMESTAMP	RecentLogout;			//�ֱ� �α׾ƿ� �ð�(��������)
	DBTIMESTAMP	UserFlatEndTime;		//���� ������ ���ᳯ¥ 
	DBTIMESTAMP	PCFlatEndTime;			//PC�� ������ ���ᳯ¥ 
};


struct ASTWS_ADD_NEW_USER
{
	WORD		wHeader;
	DWORD		dwPropID;			//Agent���� ������ �̾ƿ������� Key�� (MEMBER DB�� �ִ� Chr_Log_Info Index��)
	DWORD		dwSerialCode;		//Agent�� �ø��� �ڵ� 
	DWORD		dwChar_Index;		//������ Character�� DB���� Char_Index
	BYTE		bBillingType;		//���� Ÿ�� 
	DWORD		dwRemainTime;		//���� �ð� 
	DBTIMESTAMP	BillingEday;		//���� �����½ð� 

	BILLING_INFO	BillingInfo;//���� ���� 

	ASTWS_ADD_NEW_USER()	{	wHeader = Protocol_Agent_Server::ACMD_ADD_NEW_USER;	}
	DWORD	GetPacketSize()	{	return sizeof(ASTWS_ADD_NEW_USER);	}
};


#pragma pack(pop)

