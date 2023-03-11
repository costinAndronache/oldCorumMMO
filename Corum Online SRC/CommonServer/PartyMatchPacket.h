#pragma once

#include "ProtocolDefinition.h"

// ��Ī ����
struct PMATCH_CONDITION
{
	BYTE bLevel;		// ������ 3, 5, 10
	BYTE bExp;		// ����ġ �й� ��� : 1 ���� , 2 ����, 3 ���ʽ��� �й�, 4 ��� ����
	BYTE bItem;		// ������ �γ� ��� : 1 ������, 2 ������ Ÿ����, 3 ����, 4 ��� ����
	BYTE bMaster;	// ��Ƽ������ ���� ���� : 1 ��Ƽ������, 0 ��Ƽ������(��Ƽ���� �� ���� ����)
};

// ����� ��� �Ƿ�
struct CTWS_PMATCH_REGISTER : WORLD_PACKET_HEADER
{
	PMATCH_CONDITION condition;		// ��Ī ����
	CTWS_PMATCH_REGISTER()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PMATCH_REGISTER;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_PMATCH_REGISTER);	}
};

// ����� ��� ���
struct WSTC_PMATCH_REGISTER_RESULT : WORLD_PACKET_HEADER
{
	BYTE bResult;	// 0 : ����, �ƴϸ� �����ڵ�
	WSTC_PMATCH_REGISTER_RESULT()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PMATCH_REGISTER_RESULT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PMATCH_REGISTER_RESULT);	}
};

// ����� Ż��
struct CTWS_PMATCH_ESCAPE : WORLD_PACKET_HEADER
{
	CTWS_PMATCH_ESCAPE()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PMATCH_ESCAPE;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_PMATCH_ESCAPE);	}
};

// ����� Ż�� ���
struct WSTC_PMATCH_ESCAPE_RESULT : WORLD_PACKET_HEADER
{
	BYTE bResult;	// 0 : ����, �ƴϸ� �����ڵ�
	WSTC_PMATCH_ESCAPE_RESULT()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PMATCH_ESCAPE_RESULT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PMATCH_ESCAPE_RESULT);	}
};

// ��Ī �Ƿ�
struct WSTC_PMATCH_REQUEST : WORLD_PACKET_HEADER
{
	BYTE	bLevel;									// ����
	BYTE	bClass;									// Ŭ����
	char	szName[ MAX_CHARACTER_NAME_LENGTH ];	// �̸�
	BYTE	bMaster;								// ����
	WSTC_PMATCH_REQUEST()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PMATCH_REQUEST;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PMATCH_REQUEST);	}
};

// ��Ī �Ƿ� ���
struct CTWS_PMATCH_REQUEST_RESULT : WORLD_PACKET_HEADER
{
	BYTE bResult;	// 0 : ����, �ƴϸ� �����ڵ�
	CTWS_PMATCH_REQUEST_RESULT()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PMATCH_REQUEST_RESULT;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_PMATCH_REQUEST_RESULT);	}
};

// ��Ī ���� ����
struct CTWS_PMATCH_CHANGE : WORLD_PACKET_HEADER
{
	PMATCH_CONDITION condition;		// ���ο� ��Ī ����
	CTWS_PMATCH_CHANGE()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PMATCH_CHANGE;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_PMATCH_CHANGE);	}
};

// ��Ī ���� ���� ���
struct WSTC_PMATCH_CHANGE_RESULT : WORLD_PACKET_HEADER
{
	BYTE bResult;	// 0 : ����, �ƴϸ� �����ڵ�
	WSTC_PMATCH_CHANGE_RESULT()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PMATCH_CHANGE_RESULT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PMATCH_CHANGE_RESULT);	}
};