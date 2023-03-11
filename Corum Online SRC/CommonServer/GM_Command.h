#ifndef _GM_COMMAND_H_
#define _GM_COMMAND_H_


#pragma once


#define SUMMON_MONSTER_MIN_COUNT		1
#define SUMMON_MONSTER_MAX_COUNT		50


enum GM_COMMAND
{
	GM_Command_Empty				= -1,
	GM_Command_Create_Item			= 0,	// ������ ���� ��ɾ�. create item �ڿ� ������ų �������� id�� �Է�
	GM_Command_Summon_Monster		= 1,	// ���� ��ȯ ��ɾ�(���� ��ȯ). 
	GM_Command_Silence				= 2,
	GM_Command_Get_Recall_Someone	= 3,	// ĳ���͸� �����´�.
	GM_Command_Get_Move_Someone		= 4,	// Ư�� �ɸ��� �ִ°����� GM �̵�
	GM_Command_Dungeon_Portal		= 5,
	GM_Command_PK_Mode				= 6,	// pk mode (on, off)
	GM_Command_Observe				= 7,	// GM ĳ���Ͱ� �þ߿��� �����. (on, off)
	GM_Command_Join_Guild			= 8,
	GM_Command_Change_Monster		= 9,	// GM ĳ������ ���°� ���ͷ� ����(�̺�Ʈ��)
	GM_Command_Where_Is_Someone		=10,	// Ư��ĳ������ ��ġ�� �˾ƿ´�.
	GM_Command_Kick_User			=11,	// Ư�� ������ ű�Ѵ�.

	
	// ����ٰ� ����ϼ��� 
	GM_Command_Max_Count
};


#pragma pack(push,1)

struct GM_CMD_PARAM
{
	enum GM_COMMAND		eGMCommand;
};


//
// GM_CMD_PARAM_Create_Item 
//
struct GM_CMD_PARAM_Create_Item : public GM_CMD_PARAM
{
	WORD wItemID;	
};

//
// GM_CMD_PARAM_Summon_Monster
//
struct GM_CMD_PARAM_Summon_Monster : public GM_CMD_PARAM
{
	WORD	wMonsterID;
	WORD	wMonsterCount;
};

//
// GM_CMD_PARAM_Silence 
//
struct GM_CMD_PARAM_Silence : public GM_CMD_PARAM
{
	BYTE	bFlag;
	BYTE	bAll;
	
	DWORD	dwUserIndex;
};


//
// GM_CMD_PARAM_Recall_Someone 
//
struct GM_CMD_PARAM_Recall_Someone : public GM_CMD_PARAM
{
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	WORD	wDungeonID;
	BYTE	bLayer;

	WORD	wX;
	WORD	wZ;
};


//
// GM_CMD_PARAM_Get_Move_Someone 
//
struct GM_CMD_PARAM_Get_Move_Someone : public GM_CMD_PARAM
{
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];		

};

//
// GM_CMD_PARAM_Kick_User 
//
struct GM_CMD_PARAM_Kick_User : public GM_CMD_PARAM
{
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];
};


//
// GM_CMD_PARAM_Dungeon_Portal
//
struct GM_CMD_PARAM_Dungeon_Portal : public GM_CMD_PARAM
{
	WORD	wDungeonID;		// �������			
	BYTE	bLayerFloor;	// �� ��

};

//
// GM_CMD_PARAM_PK_Mode 
//
struct GM_CMD_PARAM_PK_Mode : public GM_CMD_PARAM
{
	BYTE	bPKMode;
	char	szCharacterName[20];
};


//
// GM_CMD_PARAM_Observe 
//
struct GM_CMD_PARAM_Observe : public GM_CMD_PARAM
{
	BYTE	bObserveFlag;		// TRUE : disappear, FALSE : appear
};


//
// GM_CMD_PARAM_Join_Guild
//
struct GM_CMD_PARAM_Join_Guild : public GM_CMD_PARAM
{
	char	szGuildName[10];
};


//
// GM_CMD_PARAM_Change_Monster 
//
struct GM_CMD_PARAM_Change_Monster : public GM_CMD_PARAM
{
	WORD	wChangeMonsterID;
};


#pragma pack(pop)


#endif


