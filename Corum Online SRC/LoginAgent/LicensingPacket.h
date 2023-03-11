#ifndef __LICENSING_PROTOCOL_H__
#define __LICENSING_PROTOCOL_H__


#pragma once


enum Proto_Licensing
{
	Proto_Licensing_Heartbeat			= 0, // LoginAgent���� 1�а� ������ ��Ŷ�� ���� ���, �۽�. ���̼��� ������ ��۽�.
	Proto_Licensing_Authenticaion		= 1, // �������� �α��μ������� Ȯ�� ��û.
	Proto_Licensing_AuthenticaionResult = 2, // ���� ����� ����.
	Proto_Licensing_CurrentUser			= 3, // ���� ���ڸ� �˸�.
	Proto_Licensing_BillingInformation	= 4, // ���� ���� ���� �۽�.
	Proto_Licensing_Command				= 5, // ���̼��� ������ ��� ����.
};


enum Licensing_Command
{
	Command_Send_Heartbeat				= 0, // Heartbeat�� ������� ���.
	Command_Send_CurrentUser			= 1, // ���� ���� ������ ������� ���.
	Command_Send_BillingInformation		= 2, // ���� ������ ������� ���.
	Command_Suicide						= 3, // ������� ���.
	Command_DropAllUsers				= 4, // ��� ������ ������ ���� ������� ���.	
};


struct Proto_Licensing_Packet_Heartbeat
{
	BYTE	byPacketType;

	Proto_Licensing_Packet_Heartbeat()				{ byPacketType = Proto_Licensing_Heartbeat;	}
	WORD	GetPacketSize()							{ return sizeof(Proto_Licensing_Packet_Heartbeat);	}
};

struct Proto_Licensing_Packet_Authenticaion
{
	BYTE	byPacketType;

	Proto_Licensing_Packet_Authenticaion()			{ byPacketType = Proto_Licensing_Authenticaion;	}
	WORD	GetPacketSize()							{ return sizeof(Proto_Licensing_Packet_Authenticaion);	}
};

struct Proto_Licensing_Packet_AuthenticaionResult
{
	BYTE	byPacketType;
	BYTE	byResult;	// 1 if succeeded!

	Proto_Licensing_Packet_AuthenticaionResult()	{ byPacketType = Proto_Licensing_AuthenticaionResult; byResult = 0;	}
	WORD	GetPacketSize()							{ return sizeof(Proto_Licensing_Packet_AuthenticaionResult);	}
};

struct Proto_Licensing_Packet_CurrentUser
{
	BYTE	byPacketType;
	DWORD	dwCurrentUserCount;

	Proto_Licensing_Packet_CurrentUser()			{ byPacketType = Proto_Licensing_CurrentUser; dwCurrentUserCount = 0;	}
	WORD	GetPacketSize()							{ return sizeof(Proto_Licensing_Packet_CurrentUser);	}
};

struct Proto_Licensing_Packet_BillingInformation
{
	BYTE	byPacketType;
	BYTE	BillingInformation[2048];

	Proto_Licensing_Packet_BillingInformation()		{ byPacketType = Proto_Licensing_BillingInformation; memset(&BillingInformation, 0, sizeof(BillingInformation));	}
	WORD	GetPacketSize()							{ return sizeof(Proto_Licensing_Packet_BillingInformation);	}
};

struct Proto_Licensing_Packet_Command
{
	BYTE	byPacketType;
	BYTE	byCommand;

	Proto_Licensing_Packet_Command()				{ byPacketType = Proto_Licensing_Command; byCommand = 0;	}
	WORD	GetPacketSize()							{ return sizeof(Proto_Licensing_Packet_Command);	}
};


#endif 