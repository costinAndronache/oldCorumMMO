#pragma once


#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4DYUCHIGRX_COMMON/math.inl"
#else
#include "./../4DYUCHIGRX_COMMON/math.inl"
#endif
#include "ProtocolDefinition.h"
#include "Item.h"


enum ITEM_NATIVE;


#pragma pack(push, _aaaa)
#pragma pack(1)
struct PLAYERSHOPINFO_NODE
{
	BYTE	index;					// ���° �������̳�.
	DWORD	m_dwCustomSellPrice;	// ������ ���鶧 �� ������ �����Ѵ�.
	CItem	cItem;					// ������ �����۸��
};
struct ITEM_NODE_FOR_MSG
{
	BYTE	index;					// ���° �������̳�.
	CItem	cItem;					// ������ �����۸��
};
struct ITEM_NODE_FOR_MSG_NATIVE
{
	BYTE		index;
	ITEM_NATIVE eSlotID;
	BYTE		bySlotIndex;	
};
#pragma pack(pop, _aaaa)


#pragma pack(push,1)


//-----------------------------------------------------------------------------
// Client�� ���� �ϸ� ���� Village������ �α��� �������� �Ȱ������� ���θ� �� ��Ŷ�� ���� �����ش�.	
//-----------------------------------------------------------------------------
struct DSTC_JOIN : public DUNGEON_PACKET_HEADER
{
	BYTE	bApproval;							// True: ����, FALSE: �ź� 	
	DWORD	dwUserIndex;						// ������ �����ε���.
	char	szName[MAX_CHARACTER_NAME_LENGTH];	// Name
	WORD	wHead;								// �Ӹ� ��ȣ.
	WORD	wClass;								// �ɸ��� Ŭ����.
	WORD	wRace;
	WORD	wGrade;
	WORD	wHP;					
	WORD	wMP;
	WORD	wPoint;								// �������� �߻��ϴ� ���ʽ� ����Ʈ.
	WORD	wPointSkill;
	DWORD	dwExp;
	DWORD	dwLevel;
	DWORD	dwHonor;
	DWORD	dwEgo;
	DWORD	dwStr;
	DWORD	dwInt;
	DWORD	dwDex;
	DWORD	dwVit;
	DWORD	dwLuck;
	BYTE	bySelectedSkill[2];	
	CItem	pEquip[ MAX_EQUIP_POOL ];
	CItem	pInv_Large[ MAX_INV_LARGE_POOL ];
	CItem	pInv_Small[ MAX_INV_SMALL_POOL ];
	CItem	pBelt[ MAX_BELT_POOL ];
	BYTE	pwSkillLevel[ MAX_SKILL ];	
	char	szGuildName[MAX_GUILD_NAME_LENGTH];
	char	szNickName[MAX_GUILD_NAME_LENGTH];	
	BYTE	byType;
	BYTE	byRank;	
	DWORD	dwGuildId;	
	CItem	pInv_Guardian[ MAX_INV_GUARDIAN_POOL ];	
	DWORD	dwMoney;	
	WORD	wMaxHP;
	WORD	wMaxMP;
	WORD	pwAttackDamage_R[2];
	WORD	pwAttackDamage_L[2];
	WORD	wAttackAcuracy;
	WORD	wAvoid;
	WORD	wBlockRate;	
	WORD	wFireResist;
	WORD	wIceResist;
	WORD	wLightResist;
	WORD	wPoiResist;
	WORD	wPhyResist;	
	float	fMoveSpeed;
	DWORD	dwHealHPSec;
	DWORD	dwHealMPSec;
	WORD	wAttackSpeed;
	DWORD	dwLayerID;
	WORD	wMapID;
	BYTE	bLayer;								// ���� 
	BYTE	bAttackMode;
	WORD	wSiegePKCount;						// �������϶� ���� ���μ�
	short	wCriminal;
	VECTOR2	v2CurPos;
	BYTE	bIsPortal;
	WORD	wClassRank;	
	DWORD	dwMatchRecords;
	DWORD	dwMatchWin;
	DWORD	dwMatchLose;		
	BYTE	byCurrentHand;	
	BYTE	byGuildFlag;
	DWORD	dwGuildWarFlag;		// �⺻ : 0. ������� : 1, ������ : 2
	size_t	team_index;	//����� �� �ε���
	
	DSTC_JOIN()				{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_JOIN;	bIsPortal = 0;}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_JOIN);	}
};


struct DSTC_MOVE : public DUNGEON_PACKET_HEADER
{
	DWORD	dwIndex;	
	BYTE	bMoveType;			// Move types..
	float	fSpeed;
	VECTOR2	v2MoveStartPos;		// Move start position..
	VECTOR2	v2MoveDirection;	// Move direction..	
	WORD	wDestX;
	WORD	wDestZ;
	
	DSTC_MOVE()				{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_MOVE;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_MOVE);	}
};


struct DSTC_STOP : public DUNGEON_PACKET_HEADER
{
	DWORD	dwUserIndex;	
	VECTOR2	v2StopPos;			// Stop position..
	
	DSTC_STOP()				{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_STOP;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_STOP);	}
};


struct DSTC_APPEAR : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwUserIndex;
	VECTOR2	v2CurPos;
	VECTOR2	v2Direction;	
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	WORD	wHead;
	WORD	wClass;	
	WORD	wArmor;
	WORD	wHandR;
	WORD	wHandL;
	WORD	wHelmet;
	short	wCriminalTime;	// ���� Ÿ��
	BYTE	bAttackMode;
	WORD	wSiegePKCount;
	DWORD	dwPartyID;	
	DWORD	dwGuildId;	
	char	szNickName[MAX_CHARACTER_NAME_LENGTH];	
	BYTE	byGuildFlag;	
	BYTE	byUpgrade;
	DWORD	dwGuildWarFlag;//����� �÷��� : ������ 050202
	size_t	team_index;//����� �� �ε��� : hwoarang 050202
	
	DSTC_APPEAR()			{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_APPEAR;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_APPEAR);	}
};


struct DSTC_DISAPPEAR : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwUserIndex;
	
	DSTC_DISAPPEAR()		{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_DISAPPEAR;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_DISAPPEAR);	}
};

struct DSTC_GUILDWAR_DIE_MSG : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwUserIndex;
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	
	DSTC_GUILDWAR_DIE_MSG()		{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_GUILDWAR_DIE_MSG;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_GUILDWAR_DIE_MSG);	}
};


struct DSTC_ATTACK_USER_MON : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwUserIndex;
	DWORD	dwMonsterIndex;	
	VECTOR2	v2UserPos;
	VECTOR2	v2MonsterPos;	
	BYTE	bType;				// ��������.. ���.
	BYTE	bStatusKind;
	DWORD	dwCurMonsterHP;		// 0�̸� �������̴�.
	DWORD	dwDamage;			// �Ҹ�� HP.
	BYTE	bAttackType;		// 3���� �ִϸ��̼� ������
	BYTE	bAttackFrame;		// 3���� ��������.
	
	DSTC_ATTACK_USER_MON()	{	memset(this, NULL, sizeof(DSTC_ATTACK_USER_MON)); bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_ATTACK_USER_MON;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_ATTACK_USER_MON);	}
};


struct DSTC_ATTACK_USER_USER : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwOffenseUserIndex;
	DWORD	dwDefenseUserIndex;	
	VECTOR2	v2OffenseUserPos;
	VECTOR2	v2DefenseUserPos;
	BYTE	bType;				// ATTACK_USER_MON �� �����ϴ�.
	BYTE	bStatusKind;		
	DWORD	dwCurDefenseUserHP;
	DWORD	dwDefenseUserMaxHP;
	DWORD	dwDamage;
	BYTE	bAttackType;		// 3���� �ִϸ��̼� ������
	BYTE	bAttackFrame;		// 3���� ��������.
	
	DSTC_ATTACK_USER_USER()	{	memset(this, NULL, sizeof(DSTC_ATTACK_USER_USER)); bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_ATTACK_USER_USER;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_ATTACK_USER_USER);	}
};


struct DSTC_ATTACK_MON_MON : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwOffenseMonIndex;
	DWORD	dwDefenseMonIndex;	
	VECTOR2 v2OffenseMonPos;
	VECTOR2	v2DefenseMonPos;
	BYTE	bType;
	BYTE	bStatusKind;
	DWORD	dwCurDefenseMonHP;
	DWORD	dwDamage;
	
	DSTC_ATTACK_MON_MON()	{	memset(this, NULL, sizeof(DSTC_ATTACK_MON_MON)); bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_ATTACK_MON_MON;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_ATTACK_MON_MON);		}
};


struct DSTC_APPEAR_MON : public DUNGEON_PACKET_HEADER
{	
	BYTE	bMonsterKind;			// Monster? or Guardain?
	BYTE	bZipCode;				// Guardian slot..
	BYTE	bClass;
	DWORD	dwLordIndex;			// When MonsterType is Guardian...
	DWORD	dwMonsterIndex;
	DWORD	dwLevel;
	char	szMonsterName[50];
	WORD	wMod;					// Use Mod Number...
	BYTE	bScale;
	VECTOR2 v2CurPos;
	DWORD	dwHP;
	DWORD	dwMaxHP;
	float	fSpeed;
	WORD	wMonNo;
	WORD	wNeu;
	WORD	wDamage[2];
	WORD	wAttack[2];
	WORD	wDeath[2];
	WORD	wMove[2];
	
	DSTC_APPEAR_MON()		{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_APPEAR_MON; }
	DWORD	GetPacketSize()	{	return sizeof(DSTC_APPEAR_MON); }
};


struct DSTC_DISAPPEAR_MON : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwMonsterIndex;
	
	DSTC_DISAPPEAR_MON()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_DISAPPEAR_MON; }
	DWORD	GetPacketSize()	{	return sizeof(DSTC_DISAPPEAR_MON); }
};


struct DSTC_KILL_MON : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwMonsterIndex;
	
	DSTC_KILL_MON()			{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_KILL_MON; }
	DWORD	GetPacketSize()	{	return sizeof( DSTC_KILL_MON ); }
};


struct DSTC_MOVE_MON : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwMonsterIndex;
	float	fSpeed;
	BYTE	bMoveType;			// Move types..
	VECTOR2	v2MoveStartPos;		// Move start position..
	VECTOR2 v2MoveStopPos;
	VECTOR2	v2MoveDirection;	// Move direction..
	
	DSTC_MOVE_MON()			{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_MOVE_MON; }
	DWORD	GetPacketSize()	{	return sizeof( DSTC_MOVE_MON ); }
};


struct DSTC_STOP_MON : public DUNGEON_PACKET_HEADER
{
	DWORD	dwMonsterIndex;	
	VECTOR2	v2StopPos;			// Stop position..
	
	DSTC_STOP_MON()				{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_STOP_MON;	}
	DWORD	GetPacketSize()		{	return sizeof(DSTC_STOP_MON);	}
};


struct DSTC_ATTACK_MON_USER : public DUNGEON_PACKET_HEADER		
{
	DWORD	dwMonsterIndex;
	DWORD	dwUserIndex;
	VECTOR2	v2MonsterPos;
	BYTE	bType;
	BYTE	bStatusKind;
	DWORD	dwCurUserHP;
	DWORD	dwDamage;
    
	DSTC_ATTACK_MON_USER()		{ memset(this, NULL, sizeof(DSTC_ATTACK_MON_USER)); bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_ATTACK_MON_USER; }
	DWORD	GetPacketSize()		{ return sizeof( DSTC_ATTACK_MON_USER ); }
};


struct	DSTC_DUNGEON_CHAT : public DUNGEON_PACKET_HEADER
{	
	BYTE	bChatType;
	DWORD	dwUserIndex;
	BYTE	bChatStrLength;
	char	szChatStr[255];
	
	DSTC_DUNGEON_CHAT()	{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_CHAT;	}
	DWORD	GetPacketSize()	{	return (sizeof(DSTC_DUNGEON_CHAT) - ( 255 - bChatStrLength ) ); }
};


struct DSTC_CHANGE_LAYER : public DUNGEON_PACKET_HEADER
{
	WORD	wDungeonIndex;
	WORD	wDungeonLayer;
	BYTE	bCurLayerIndex;	// ���� ����ȣ.
	BYTE	bAttackMode;
	VECTOR2	v2CurPos;	
	DWORD	pdwMonsterKind[10];	
	WORD	wTotalMonster;	// ���� ������.
	WORD	wCurMonster;
	
	DSTC_CHANGE_LAYER()			{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_CHANGE_LAYER;	}
	DWORD	GetPacketSize()		{ return sizeof( DSTC_CHANGE_LAYER );	}
};


struct DSTC_CHANGE_SECTION : public DUNGEON_PACKET_HEADER
{
	WORD	wChageSectionNum;
	BYTE	bDisAppearUserCount;
	BYTE	bDisAppearMonsterCount;
	DWORD	pdwDisAppearIndex[MAX_DISAPPEAR_INFO+MAX_DISAPPEAR_INFO];
	
	DSTC_CHANGE_SECTION()
	{	
		bStatus = UPDATE_GAME_PLAY; 
		bHeader = Protocol_DSTC::CMD_DUNGEON_CHANGE_SECTION;	
		bDisAppearUserCount = 0;
		bDisAppearMonsterCount = 0;
		memset( pdwDisAppearIndex, 0, sizeof( DWORD ) * (MAX_DISAPPEAR_INFO + MAX_DISAPPEAR_INFO) );
	}

	DWORD GetPacketSize()
	{ 
		return sizeof( DSTC_CHANGE_SECTION ) - sizeof( DWORD ) * 
			(MAX_DISAPPEAR_INFO + MAX_DISAPPEAR_INFO - bDisAppearUserCount - bDisAppearMonsterCount);
	}
};


struct DSTC_LEVELUP : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwUserIndex;
	DWORD	dwLevel;
	BYTE	byStatPoint;
	
	DSTC_LEVELUP()				{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_LEVELUP;			}
	DWORD	GetPacketSize()		{ return sizeof( DSTC_LEVELUP );		}
};


struct DSTC_GUARDIAN_LEVELUP : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwGuardianIndex;
	DWORD	dwLevel;
	
	DSTC_GUARDIAN_LEVELUP()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_GUARDIAN_LEVELUP;}
	DWORD	GetPacketSize()		{ return sizeof( DSTC_GUARDIAN_LEVELUP );		}
};


struct	DSTC_APPEAR_ITEM : public DUNGEON_PACKET_HEADER
{	
	BYTE	bType;				// Create?
	BYTE	bCreateByType;		
	VECTOR2 v2ItemPos;
	CItem	Item;
	
	DSTC_APPEAR_ITEM()			{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_APPEAR_ITEM;		}
	DWORD	GetPacketSize()		{ return sizeof( DSTC_APPEAR_ITEM );	}
};


struct	DSTC_DISAPPEAR_ITEM : public DUNGEON_PACKET_HEADER
{	
	__int64 i64ItemID;
	
	DSTC_DISAPPEAR_ITEM()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_DISAPPEAR_ITEM;	}
	DWORD	GetPacketSize()		{ return sizeof( DSTC_DISAPPEAR_ITEM );	}
};


struct	DSTC_ITEM_PICKUP : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwMoney;		// ���� ���� ��.
	DWORD	dwId;
	BYTE	bInv;			// ��� �����ΰ�.	1-ū����, 2-��������, 3-����â, 4-��Ʈ, 5-���콺,6-���콺->ū����, 7- ū����->���콺, 8-���콺->��������,9-��������->���콺, 8-���콺->����â, 9->���콺->��Ʈ.
	BYTE	bZipCode;		// ��� �κ��ΰ�.
	BYTE	bEquipCode;		// ���° �������� �������ΰ�.
	BYTE	bZipCode2;		// ��� �κ��ΰ�.
	BYTE	bEqiupCode2;	// ���° �������� �������ΰ�.
	CItem	Item;			// �� ������.
	
	DSTC_ITEM_PICKUP()			{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_ITEM_PICKUP; }
	DWORD	GetPacketSize()		{ return sizeof( DSTC_ITEM_PICKUP );	}
};


struct	DSTC_ITEM_PUT : public DUNGEON_PACKET_HEADER
{	
	BYTE	bInv;
	BYTE	bZipCode;
	
	DSTC_ITEM_PUT()				{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_ITEM_PUT;	}
	DWORD	GetPacketSize()		{ return sizeof( DSTC_ITEM_PUT );		}
};


struct USER_STATUS
{
	DWORD	dwCode;			// � ������ �����ΰ�.
	DWORD	dwMin;			
	DWORD	dwMax;			// ���� Min, Max �� ���� �ƴϰ� �ϳ��� ���̶�� Min�� ����Ѵ�.
};


struct	DSTC_USER_STATUS : public DUNGEON_PACKET_HEADER
{	
	BYTE		bStatusMany;		// �ݿ��� ������ ����.
	USER_STATUS	pStatus[30];		// �ִ� 30������.
	
	DSTC_USER_STATUS()
	{
		bStatus			= UPDATE_GAME_PLAY;
		bHeader			= Protocol_DSTC::CMD_DUNGEON_USER_STATUS;
		bStatusMany		= 0;
	}

	DWORD	GetPacketSize()		{ return ( sizeof(DSTC_USER_STATUS) - ( sizeof( pStatus ) - ( bStatusMany * sizeof(USER_STATUS) ) ) );	}
};


struct	DSTC_USER_SKILL_LEVEL : public DUNGEON_PACKET_HEADER
{	
	BYTE		pbSkillLevel[MAX_SKILL];
	
	DSTC_USER_SKILL_LEVEL()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_USER_SKILL_LEVEL; }
	DWORD	GetPacketSize()		{ return sizeof( DSTC_USER_SKILL_LEVEL );	}
};


struct	DSTC_DUNGEON_ERROR_CODE : public DUNGEON_PACKET_HEADER
{
	DWORD dwErrorCode;

	DSTC_DUNGEON_ERROR_CODE()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_ERROR_CODE;	}
	DWORD	GetPacketSize()		{ return sizeof( DSTC_DUNGEON_ERROR_CODE );	}
};


struct	DSTC_GUARDIAN_STATUS : public DUNGEON_PACKET_HEADER
{	
	DWORD		dwGuardianIndex;
	BYTE		bStatusMany;		// �ݿ��� ������ ����.
	USER_STATUS	pStatus[30];		// �ִ� 30������.
	
	DSTC_GUARDIAN_STATUS()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_GUARDIAN_STATUS;	}
	DWORD	GetPacketSize()		{ return ( sizeof(DSTC_USER_STATUS) - ( sizeof( pStatus ) - ( bStatusMany * sizeof(USER_STATUS) ) ) );	}
};


struct DSTC_ACCENTRANCE_ACQUISITION : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwMoney;

	DSTC_ACCENTRANCE_ACQUISITION()	{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_ACCENTRANCE_ACQUISITION;	}
	DWORD	GetPacketSize()	{	return (sizeof(DSTC_ACCENTRANCE_ACQUISITION)); }	
};


struct DSTC_GURADIAN_EGG_SUMMON_STATUS : public DUNGEON_PACKET_HEADER
{	
	INT64	i64ID;
	BYTE	byGuardianEggSummonStatus;
	BYTE	byZipCode;
	
	DSTC_GURADIAN_EGG_SUMMON_STATUS()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_GURADIAN_EGG_SUMMON_STATUS; }
	DWORD	GetPacketSize()	{	return sizeof(DSTC_GURADIAN_EGG_SUMMON_STATUS); }
};

struct DSTC_APPENDIX_GUARDIAN_INFO : public DUNGEON_PACKET_HEADER
{
	DWORD	dwMonsterIndex;
	WORD	wEgo;
	WORD	wStr;
	WORD	wInt;
	WORD	wDex;
	WORD	wVit;
	WORD	wStatPoint;
	DWORD	dwExp;
	WORD	wAttackAcuracy;		// ������
	WORD	wAvoid;				// ȸ����
	short	wFireResist;
	short	wIceResist;
	short	wLightResist;
	short	wPoiResist;
	short	wPhyResist;
	WORD	pwAttackDamage_L[2];
	DWORD	dwMaxHP;

	struct __SKILL__
	{
		BYTE bKind, bLevel;
	} pSkill[5];

	DSTC_APPENDIX_GUARDIAN_INFO()		{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_APPENDIX_GUARDIAN_INFO; }
	DWORD	GetPacketSize()	{	return sizeof(DSTC_APPENDIX_GUARDIAN_INFO); }
};


struct DSTC_DUNGEON_EMOTICON : public DUNGEON_PACKET_HEADER
{	
	BYTE		bWhatEmoticon;
	DWORD		dwUserIndex;
	
	DSTC_DUNGEON_EMOTICON()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_EMOTICON;	}
	DWORD	GetPacketSize()		{ return sizeof( DSTC_DUNGEON_EMOTICON );	}	
};


// =============
// ����� ��û�Ͽ���.
// =============
struct DSTC_DUNGEON_REQUEST_MATCH_MAN2MAN : public DUNGEON_PACKET_HEADER
{	
	DWORD		dwRequestUserIndex;
	
	DSTC_DUNGEON_REQUEST_MATCH_MAN2MAN()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_REQUEST_MATCH_MAN2MAN;	}
	DWORD	GetPacketSize()		{ return sizeof( DSTC_DUNGEON_REQUEST_MATCH_MAN2MAN);	}	
};


// =============
// ��ῡ �亯�Ͽ���.
// =============
struct DSTC_DUNGEON_REPLY_MATCH_MAN2MAN : public DUNGEON_PACKET_HEADER
{	
	BYTE		bReplyCode;			// 0 : ��Ÿ����, 1 : ����, 2 : �ź�, 3 : ������, 4 : ���� ������ , 5 : ������Ұ� ��á��, 6 : ������ ��ȯ���̴�.
	DWORD		dwMatchUserIndex;	// ��� ����;	
	
	DSTC_DUNGEON_REPLY_MATCH_MAN2MAN()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_REPLY_MATCH_MAN2MAN; bReplyCode = 0;	}
	DWORD	GetPacketSize()		{ return sizeof( DSTC_DUNGEON_REPLY_MATCH_MAN2MAN);	}	
};


// =============
// ����� ���غ� �Ǿ���.
// =============
struct DSTC_READY_MATCH_MAN2MAN : public DUNGEON_PACKET_HEADER
{	
	DWORD		dwReadyUserIndex;
	
	DSTC_READY_MATCH_MAN2MAN()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_READY_MATCH_MAN2MAN; }
	DWORD	GetPacketSize()		{ return sizeof( DSTC_READY_MATCH_MAN2MAN );	}
};


// =============
// ��� ����.
// =============
struct DSTC_START_MATCH_MAN2MAN : public DUNGEON_PACKET_HEADER
{	
	DWORD		dwMatchUserIndex;
	DWORD		dwMatchTimeTick;	// ��� �ð� (tick)
	
	DSTC_START_MATCH_MAN2MAN()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_START_MATCH_MAN2MAN; }
	DWORD	GetPacketSize()		{ return sizeof( DSTC_START_MATCH_MAN2MAN );	}
};


// =============
// ������ �Ϸ� �ƴ�.
// =============
struct DSTC_END_MATCH_MAN2MAN : public DUNGEON_PACKET_HEADER
{	
	BYTE		bJudgmentCode;		// �ǰ� ���� ���� | 0 : KO, 1 : ����(��ǵ� ����), 2 : ���º�, 3 : ������ ����, 4 : ���� �����鿡�Ը� �ǹ��ִ� �޽��� 
	char		szWinnerName[MAX_CHARACTER_NAME_LENGTH];	// ����
	char		szLoserName[MAX_CHARACTER_NAME_LENGTH];		// ����
	
	DSTC_END_MATCH_MAN2MAN()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_END_MATCH_MAN2MAN; }
	DWORD	GetPacketSize()		{ return sizeof( DSTC_END_MATCH_MAN2MAN );	}
};


// =============
// ������ ���� ������ ������.
// =============
struct DSTC_MATCH_USER_INFO : public DUNGEON_PACKET_HEADER
{	
	BYTE		bUserStatus;		// 0 ; ���� ��� ���� 
	DWORD		dwMatchUserIndex;	// ��� ���� �ε��� 
	DWORD		dwHP;
	DWORD		dwMaxHP;
	DWORD		dwMP;
	DWORD		dwMaxMP;
	
	DSTC_MATCH_USER_INFO()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_MATCH_USER_INFO; }
	DWORD	GetPacketSize()		{ return sizeof( DSTC_MATCH_USER_INFO );	}
};

//------------------------------------------------------------------------------//
// ��Ż�̵�����  ���� Join ��											
//------------------------------------------------------------------------------//
struct DSTC_PORTAL_JOIN_USER : public DUNGEON_PACKET_HEADER
{
	WORD	wDungeonID;
	WORD	wDungeonLayer;	//layer ������ȣ 
	BYTE	bCurLayerIndex;	// ���� ����ȣ.
	BYTE	bAttackMode;
	VECTOR2	v2CurPos;	
	DWORD	pdwMonsterKind[10];
	
	//-- ���� ������ DSTC_CHANGE_LAYER�� ���� ������ ������ �Ѵ�. (InitMainPlayer���ڰ�)

	BYTE	bChangeDungeon;	// ������ ��� ��� 1 , ������ ������ ���� �ٲ�� ��� 0 
	
	DSTC_PORTAL_JOIN_USER()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_PORTAL_JOIN_USER;	}
	DWORD	GetPacketSize()		{ return sizeof( DSTC_PORTAL_JOIN_USER );	}
};


//------------------------------------------------------------------------------//
// �ܺμ������� ��Ż�̵� 												
//------------------------------------------------------------------------------//
struct DSTC_PORTAL_MOVE_OUTSIDE : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwConnectIP;		
	WORD	wPort;
	WORD	wToDungeonID;		// �̵��ϰ��� �ϴ� DungeonID
	BYTE	bLayerNo;			// �� ������ LayerID
	VECTOR2	v2Pos;	
	
	DWORD	dwTargetUser;		// append by seven for GM [12/10/2004]
	
	DSTC_PORTAL_MOVE_OUTSIDE()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_PORTAL_MOVE_OUTSIDE;	}
	DWORD	GetPacketSize()		{ return sizeof( DSTC_PORTAL_MOVE_OUTSIDE );	}
};


//------------------------------------------------------------------------------//
// �ܺμ������� ��Ż�̵� ���� 											
//------------------------------------------------------------------------------//
struct DSTC_PORTAL_MOVE_OUTSIDE_FAIL : public DUNGEON_PACKET_HEADER
{	
	WORD	wToDungeonID;
	BYTE	bErrorCode;
	
	DSTC_PORTAL_MOVE_OUTSIDE_FAIL()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_PORTAL_MOVE_OUTSIDE_FAIL;	}
	DWORD	GetPacketSize()		{ return sizeof( DSTC_PORTAL_MOVE_OUTSIDE_FAIL );	}
};


struct DSTC_PORTAL_DISAPPEAR : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwUserIndex;
	BYTE	bType;
	
	DSTC_PORTAL_DISAPPEAR()		{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_PORTAL_DISAPPEAR;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_PORTAL_DISAPPEAR);	}
};


//------------------------------------------------------------------------------//
// ������ ���ָ�尡 �ߵ��ϰų� ������ �˸��� ��Ŷ					
//------------------------------------------------------------------------------//
struct DSTC_CONGESTION_MODE : public DUNGEON_PACKET_HEADER
{
	BYTE	bOn;
	DWORD	dwMonsterID;
	
	DSTC_CONGESTION_MODE()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_CONGESTION_MODE;	}
	DWORD	GetPacketSize()		{ return sizeof( DSTC_CONGESTION_MODE );	}
};


//------------------------------------------------------------------------------//
// ���� �������� Insert���� �������� �˸���.							
//------------------------------------------------------------------------------//
struct DSTC_COPY_ITEM_INSERTED : public DUNGEON_PACKET_HEADER
{
	DWORD	dwCopyItemID;
	DWORD	dwInvType;
	
	DSTC_COPY_ITEM_INSERTED()	{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_COPY_ITEM_INSERTED;	}
	DWORD	GetPacketSize()		{ return sizeof( DSTC_COPY_ITEM_INSERTED );	}
};


//-----------------------------------------------------------------------------
// �̺�Ʈ ���� Ŭ���� �߽��ϴ�.
//-----------------------------------------------------------------------------
struct DSTC_EVENT_DUNGEON_CLEAR : public DUNGEON_PACKET_HEADER
{
	DWORD	dwUserIndex;
	
	DSTC_EVENT_DUNGEON_CLEAR()			{ bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_EVENT_DUNGEON_CLEAR;	}	
	DWORD	GetPacketSize()				{ return sizeof(DSTC_EVENT_DUNGEON_CLEAR);	}
};


//-----------------------------------------------------------------------------
// �̺�Ʈ ���� ������ �� �Դϴ�.
//-----------------------------------------------------------------------------
struct DSTC_EVENT_DUNGEON_LAST_LAYER : public DUNGEON_PACKET_HEADER
{	
	DSTC_EVENT_DUNGEON_LAST_LAYER()		{ bStatus = UPDATE_GAME_PLAY;  bHeader = Protocol_DSTC::CMD_EVENT_DUNGEON_LAST_LAYER; }	
	DWORD	GetPacketSize()				{ return sizeof(DSTC_EVENT_DUNGEON_LAST_LAYER);	}
};


//-----------------------------------------------------------------------------
// �̺�Ʈ ���� Ŭ�������� ���߽��ϴ�.
//-----------------------------------------------------------------------------
struct DSTC_EVENT_DUNGEON_FAILED : public DUNGEON_PACKET_HEADER
{	
	DSTC_EVENT_DUNGEON_FAILED()		{ bStatus = UPDATE_GAME_PLAY;  bHeader = Protocol_DSTC::CMD_EVENT_DUNGEON_FAILED; }	
	DWORD	GetPacketSize()			{ return sizeof(DSTC_EVENT_DUNGEON_FAILED); }	
};


//-----------------------------------------------------------------------------
// �̺�Ʈ ���������� '����' ���.
//-----------------------------------------------------------------------------
struct DSTC_EVENT_EXCEPTION_RESULT : public DUNGEON_PACKET_HEADER
{	
	BYTE	byExceptionCode;		// 0 : ��Ż ��� �ȵ�.   1 : ����� ī�� ��� �ȵ�.
	
	DSTC_EVENT_EXCEPTION_RESULT()	{ bStatus = UPDATE_GAME_PLAY;  bHeader = Protocol_DSTC::CMD_EVENT_EXCEPTION_RESULT; }	
	DWORD	GetPacketSize()			{ return sizeof(DSTC_EVENT_EXCEPTION_RESULT); }
};


//-----------------------------------------------------------------------------
// �̺�Ʈ ������ �����Ͽ���.
//-----------------------------------------------------------------------------
struct DSTC_EVENT_DUNGEON_ENTER : public DUNGEON_PACKET_HEADER
{	
	BYTE	byEventType;
	WORD	wDungeonID;
	
	DSTC_EVENT_DUNGEON_ENTER()	{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_EVENT_DUNGEON_ENTER;	}
	DWORD	GetPacketSize()		{	return sizeof(DSTC_EVENT_DUNGEON_ENTER);	}
};


//-----------------------------------------------------------------------------
// �̺�Ʈ ������ ����ʿ� ���µǾ���.
//-----------------------------------------------------------------------------
struct DSTC_EVENT_DUNGEON_OPEN_INFO : public DUNGEON_PACKET_HEADER
{	
	char	szDungeonName[MAX_DUNGEON_NAME_LENGTH];	// ���� �̸�
	BYTE	byEventState;							// �̺�Ʈ ���� ����.
	WORD	wDungeonID;
	DWORD	dwEventEndTick;							// �̺�Ʈ ���� ���� �ð�
	
	DSTC_EVENT_DUNGEON_OPEN_INFO()	{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_EVENT_DUNGEON_OPEN_INFO;	}
	DWORD	GetPacketSize()		{	return sizeof(DSTC_EVENT_DUNGEON_OPEN_INFO);	}
};


//-----------------------------------------------------------------------------
// �̺�Ʈ ���� ���� Ŭ���� ����.
//-----------------------------------------------------------------------------
struct DSTC_EVENT_DUNGEON_LAYER_CLEAR : public DUNGEON_PACKET_HEADER
{
	WORD wRemainTime;		// �� �̵��ϱ� ���� ���� �ð�.
	BYTE byType;			// �̺�Ʈ ������ Ÿ��.	
	
	DSTC_EVENT_DUNGEON_LAYER_CLEAR()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_EVENT_DUNGEON_LAYER_CLEAR; }
	DWORD	GetPacketSize()					{ return sizeof(DSTC_EVENT_DUNGEON_LAYER_CLEAR); }
};


//-----------------------------------------------------------------------------
// �̺�Ʈ ���� Ŭ���� ����. 
//-----------------------------------------------------------------------------
struct DSTC_EVENT_CLEAR_CONDITION : public DUNGEON_PACKET_HEADER
{
	WORD	wDungeonID;	
	WORD	wNeedUserNumber;			// �̵��� ���� �ʿ��� �ִ� ������.
	WORD	wNeedToKillMonsterNumber;	// �����ؾ߾� �� ������ ��.	
	
	DSTC_EVENT_CLEAR_CONDITION()	{ bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_EVENT_CLEAR_CONDITION; }
	DWORD		GetPacketSize()		{ return sizeof(DSTC_EVENT_CLEAR_CONDITION); }
};


//-----------------------------------------------------------------------------
// ����ִ� ���� ��.
//-----------------------------------------------------------------------------
struct DSTC_EVENT_ALIVE_USER_COUNT : public DUNGEON_PACKET_HEADER
{	
	WORD	wTotalUserCount;
	WORD	wAliveUserCount;
	
	DSTC_EVENT_ALIVE_USER_COUNT()	{ bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_EVENT_ALIVE_USER_COUNT; }
	DWORD		GetPacketSize()		{ return sizeof(DSTC_EVENT_ALIVE_USER_COUNT); }
};


//-----------------------------------------------------------------------------
// �̺�Ʈ ������ ����ʿ� ������.
//-----------------------------------------------------------------------------
struct DSTC_EVENT_DUNGEON_CLOSE : public DUNGEON_PACKET_HEADER
{	
	char	szDungeonName[MAX_DUNGEON_NAME_LENGTH]; // ���� �̸�.
	BYTE	byEventState;							// �̺�Ʈ ���� ����.
	WORD	wDungeonID;
	
	DSTC_EVENT_DUNGEON_CLOSE()		{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_EVENT_DUNGEON_CLOSE; }
	DWORD	GetPacketSize()			{	return sizeof(DSTC_EVENT_DUNGEON_CLOSE);	}
};


//-----------------------------------------------------------------------------
// �̺�Ʈ ����(��������)�� 1�� �̺�Ʈ�� ����Ǿ����� Ŭ���̾�Ʈ�� �˸���.
//-----------------------------------------------------------------------------
struct DSTC_EVENT_DUNGEON_FIRST_TIMEOUT : public DUNGEON_PACKET_HEADER
{
	WORD	wDungeonID;
	char	szDungeonName[MAX_DUNGEON_NAME_LENGTH];	// ���� �̸�
	BYTE	bMonsterExpRatio;
	
	DSTC_EVENT_DUNGEON_FIRST_TIMEOUT()	{ bStatus = UPDATE_GAME_PLAY;  bHeader = Protocol_DSTC::CMD_EVENT_DUNGEON_FIRST_TIMEOUT; }	
	DWORD	GetPacketSize()				{ return sizeof(DSTC_EVENT_DUNGEON_FIRST_TIMEOUT); }
};


//-----------------------------------------------------------------------------
// �̺�Ʈ ����(���ô���)���� �������� ��ƾ� �� ������ ���� ������ �����ش�. 
//-----------------------------------------------------------------------------
struct DSTC_EVENT_DUNGEON_TRAINING_DISPLAY : public DUNGEON_PACKET_HEADER
{	
	WORD	wDungeonID;
	WORD	wTotalKillMonSterLayerCount;
	WORD	wCurrentKillMonsterLayerCount;
	WORD	wPartyCurrentKillMonsterLayerCount;
	BYTE	bPartySameDungeonLayerUserCount;
	
	DSTC_EVENT_DUNGEON_TRAINING_DISPLAY()
	{
		bStatus = UPDATE_GAME_PLAY;
		bHeader = Protocol_DSTC::CMD_EVENT_DUNGEON_TRAINING_DISPLAY;
		wPartyCurrentKillMonsterLayerCount = 0;
		bPartySameDungeonLayerUserCount = 0;
	}	
	
	DWORD	GetPacketSize()					{ return sizeof(DSTC_EVENT_DUNGEON_TRAINING_DISPLAY); }
};


//-----------------------------------------------------------------------------
// �̺�Ʈ ����(���ô���)���� �������� ��ƾ� �� ������ ä����.���������� �̵� �����ϴ�.
//-----------------------------------------------------------------------------
struct DSTC_EVENT_DUNGEON_TRAINING_ONELAYER_CLAYER : public DUNGEON_PACKET_HEADER
{
	WORD	wDungeonID;
	
	DSTC_EVENT_DUNGEON_TRAINING_ONELAYER_CLAYER()	{ bStatus = UPDATE_GAME_PLAY;  bHeader = Protocol_DSTC::CMD_EVENT_DUNGEON_TRAINING_ONELAYER_CLAYER; }	
	DWORD	GetPacketSize()							{ return sizeof(DSTC_EVENT_DUNGEON_TRAINING_ONELAYER_CLAYER); }
};


//-----------------------------------------------------------------------------
// �̺�Ʈ ����(��������)���� �������� �����ִ¸����� ���� ������ �����ش�. 
//-----------------------------------------------------------------------------
struct DSTC_EVENT_DUNGEON_SURVIVAL_DISPLAY : public DUNGEON_PACKET_HEADER
{	
	BYTE	byDisplayMode;
	WORD	wDungeonID;
	WORD	wCurrentMonterLayerCount;
	WORD	wKillMonsterLayerCount;
	
	DSTC_EVENT_DUNGEON_SURVIVAL_DISPLAY()	{ bStatus = UPDATE_GAME_PLAY;  bHeader = Protocol_DSTC::CMD_EVENT_DUNGEON_SURVIVAL_DISPLAY; }	
	DWORD	GetPacketSize()					{ return sizeof(DSTC_EVENT_DUNGEON_SURVIVAL_DISPLAY); }
};


struct DSTC_EVENT_DUNGEON_CLOSE_DISPLAY : public DUNGEON_PACKET_HEADER
{	
	WORD	wDungeonID;
	
	DSTC_EVENT_DUNGEON_CLOSE_DISPLAY()		{ bStatus = UPDATE_GAME_PLAY;  bHeader = Protocol_DSTC::CMD_EVENT_DUNGEON_CLOSE_DISPLAY; }	
	DWORD	GetPacketSize()					{ return sizeof(DSTC_EVENT_DUNGEON_CLOSE_DISPLAY); }
};


//-----------------------------------------------------------------------------
// �̺�Ʈ ���� ���尴�� �̺�Ʈ ������ �ִ� ��� �����鿡�� �˷��ش�.
//-----------------------------------------------------------------------------
struct DSTC_EVENT_DUNGEON_JOINUSER_COUNT : public DUNGEON_PACKET_HEADER
{
	WORD	wDungeonID;
	WORD	wJoinUserCount;
	
	DSTC_EVENT_DUNGEON_JOINUSER_COUNT()	{ bStatus = UPDATE_GAME_PLAY;  bHeader = Protocol_DSTC::CMD_EVENT_DUNGEON_JOINUSER_COUNT; }	
	DWORD	GetPacketSize()				{ return sizeof(DSTC_EVENT_DUNGEON_JOINUSER_COUNT); }
};


//-----------------------------------------------------------------------------
// �̺�Ʈ �������� Ŭ������ ������ ������ �ִ� �������̰� �������� �˷��ش�.
//-----------------------------------------------------------------------------
struct DSTC_EVENT_DUNGEON_CLEARUSER_SEND : public DUNGEON_PACKET_HEADER
{
	char	szCharName[MAX_CHARACTER_NAME_LENGTH];
	
	DSTC_EVENT_DUNGEON_CLEARUSER_SEND() { bStatus = UPDATE_GAME_PLAY;  bHeader = Protocol_DSTC::CMD_EVENT_DUNGEON_CLEARUSER_SEND; }	
	DWORD	GetPacketSize()				{ return sizeof(DSTC_EVENT_DUNGEON_CLEARUSER_SEND); }
};


struct	DSTC_DUNGEON_CASTING : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwUserIndex;
	DWORD	dwTargetIndex;
	BYTE	bTargetType;
	BYTE	bSkillKind;
	VECTOR2 vecTarget;

	DSTC_DUNGEON_CASTING()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_DSTC::CMD_DUNGEON_CASTING;	}
	DWORD	GetPacketSize()	{	return (sizeof(DSTC_DUNGEON_CASTING)); }
};


struct DSTC_DUNGEON_STOPSTATUSEFFECT : public DUNGEON_PACKET_HEADER
{	
	BYTE	bSkillKind;
	BYTE	bTargetType;
	DWORD	dwTargetIndex;
	DWORD	dwCurHP;		// �ߵ�Ǯ������ ���� �������� ���̱� ���ؼ� ��.��

	DSTC_DUNGEON_STOPSTATUSEFFECT()	{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_DUNGEON_STOPSTATUSEFFECT;	}
	DWORD	GetPacketSize()	{	return (sizeof(DSTC_DUNGEON_STOPSTATUSEFFECT)); }	
};


struct DSTC_DUNGEON_CHANGE_LRSKILLKIND : public DUNGEON_PACKET_HEADER
{	
	short int	shSelectSkillKind[2];

	DSTC_DUNGEON_CHANGE_LRSKILLKIND()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_CHANGE_LRSKILLKIND;	}
	DWORD	GetPacketSize()		{	return sizeof(DSTC_DUNGEON_CHANGE_LRSKILLKIND);	}	
};


struct TARGETINFO
{
	BYTE	bTargetType;
	BYTE	bType;				// miss, block, success
	DWORD	dwTargetIndex;
	DWORD	dwCurHP;
	DWORD	dwDamage;
};


struct DSTC_SKILL : public DUNGEON_PACKET_HEADER
{	
	BYTE	bSkillKind;
	char	bSkillLevel;
	DWORD	dwDestTime;		// ��ų ������ �ð�.
	BYTE	bOwnType;
	BYTE	bTargetCount;	// ������� ����� ��
	DWORD	dwOwnIndex;
	VECTOR2 vecStart;		// ��ų ������ ��ġ.
	TARGETINFO sTargetInfo[MAX_SKILL_TARGET_COUNT];
	
	DSTC_SKILL()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_SKILL;bTargetCount = 0;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_SKILL)-sizeof(sTargetInfo)+sizeof(TARGETINFO)*bTargetCount;	}
};


// ��ų������ �����.
struct DSTC_SKILL_RESULT : public DUNGEON_PACKET_HEADER
{	
	BYTE	bType;	// item : 1	, skill : 2
	WORD	wKind;
	DWORD	dwSkillResult[5];
	
	DSTC_SKILL_RESULT()	{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_SKILL_RESULT; }
	DWORD	GetPacketSize()	{ return sizeof( DSTC_SKILL_RESULT ); }
};


struct _CP_INFO_
{
	BYTE	bCPID;
	BYTE	bDestroy;
	WORD	wProperty;
};


struct DSTC_DUNGEON_SIEGEINFO : public DUNGEON_PACKET_HEADER
{
	BYTE	bCPTotalCount;
	WORD	wMonsterTotalCount;
	WORD	wMonsterCurCount;
	_CP_INFO_	sCP_Info[MAX_CP];

	DSTC_DUNGEON_SIEGEINFO()	{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_DUNGEON_SIEGEINFO;	bCPTotalCount = 0;}
	DWORD	GetPacketSize()		{	return (sizeof(DSTC_DUNGEON_SIEGEINFO) - sizeof(sCP_Info) + sizeof(sCP_Info[0]) * bCPTotalCount); }	
};


// ������ �ٲ� ���� �̹� ����Ǿ� �ִ� ��ų���� ���϶�.
struct DSTC_APEAR_SKILL : public DUNGEON_PACKET_HEADER
{	
	BYTE	bSkillKind;
	BYTE	bSkillLevel;	
	VECTOR2 vecStart;		// ��ų ������ ��ġ.
	BYTE	bTargetType;
	DWORD	dwTargetIndex;
	DWORD	dwDestTime;

	DSTC_APEAR_SKILL()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_APEAR_SKILL;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_APEAR_SKILL);	}
};


struct DSTC_ATTACK_MODE : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwUserIndex;
	BYTE	bAttackMode;	// ��������� ����������

	DSTC_ATTACK_MODE()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_ATTACK_MODE;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_ATTACK_MODE);	}
};


struct DSTC_DESTROY_CP : public DUNGEON_PACKET_HEADER
{
	_CP_INFO_	sCP_Info;
	
	DSTC_DESTROY_CP()	{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_DESTROY_CP;	}
	DWORD	GetPacketSize()	{	return (sizeof(DSTC_DESTROY_CP)); }
};


struct DSTC_PARTY : public DUNGEON_PACKET_HEADER
{
	DWORD	dwUserIndex;
	DWORD	dwPartyID;
	
	DSTC_PARTY()	{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_PARTY;	}
	DWORD	GetPacketSize()	{	return (sizeof(DSTC_PARTY)); }
};


// ������ ����
struct DSTC_PLAYERSHOP_OPEN  : public DUNGEON_PACKET_HEADER
{
	DWORD	dwUserIndex;
	char	szTitle[MAX_PLAYER_SHOP_TITLE];					// ������ ����
	
	DSTC_PLAYERSHOP_OPEN()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_DSTC::CMD_PLAYERSHOP_OPEN;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_PLAYERSHOP_OPEN);	}	
};


// ������ �ݾ�.
struct DSTC_PLAYERSHOP_CLOSE : public DUNGEON_PACKET_HEADER
{
	DWORD	dwUserIndex;
	
	DSTC_PLAYERSHOP_CLOSE()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_DSTC::CMD_PLAYERSHOP_CLOSE;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_PLAYERSHOP_CLOSE);	}	
};


// ������ ���� ������
struct DSTC_PLAYERSHOP_SHOWALL : public DUNGEON_PACKET_HEADER
{
	DWORD	dwUserIndex;
	BYTE	bItemCount;
	char	szTitle[MAX_PLAYER_SHOP_TITLE];	
	PLAYERSHOPINFO_NODE ItemNode[MAX_PLAYER_SHOP_INV];

	DSTC_PLAYERSHOP_SHOWALL()	{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_PLAYERSHOP_SHOWALL; bItemCount = 0; }
	DWORD	GetPacketSize()		{	return sizeof(DSTC_PLAYERSHOP_SHOWALL) - (MAX_PLAYER_SHOP_INV - bItemCount)*sizeof(PLAYERSHOPINFO_NODE);	}	
};


struct DSTC_FINISH_MAPLOADING : public DUNGEON_PACKET_HEADER
{
	DWORD	dwUserIndex;
	
	DSTC_FINISH_MAPLOADING()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_DSTC::CMD_FINISH_MAPLOADING;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_FINISH_MAPLOADING);	}	
};


// Ŭ���̾�Ʈ�� ��Ż�Ѵٰ� ��û�ߴ����� �ٽ� �������ش�.
struct DSTC_RESPONSE_PORTAL_JOIN_USER : public DUNGEON_PACKET_HEADER
{
	VECTOR2 vecStart;
	DWORD	dwUserIndex;
	
	DSTC_RESPONSE_PORTAL_JOIN_USER()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_RESPONSE_JOIN_USER;	}
	DWORD	GetPacketSize()		{ return sizeof( DSTC_RESPONSE_PORTAL_JOIN_USER );	}
};


struct DSTC_ITEMUPGRADE_SUCCESS : public DUNGEON_PACKET_HEADER
{
	BYTE	bResult;
	CItem	cItem;

	DSTC_ITEMUPGRADE_SUCCESS()	{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_ITEMUPGRADE_SUCCESS;	}
	DWORD	GetPacketSize()	{	return (sizeof(DSTC_ITEMUPGRADE_SUCCESS)); }	
};


struct DSTC_ITEMUPGRADE_FAIL : public DUNGEON_PACKET_HEADER
{
	BYTE	bDelete;

	DSTC_ITEMUPGRADE_FAIL()	{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_ITEMUPGRADE_FAIL;	}
	DWORD	GetPacketSize()	{	return (sizeof(DSTC_ITEMUPGRADE_FAIL)); }	
};


// pkmode set
struct	DSTC_DUNGEON_PKMODE : public DUNGEON_PACKET_HEADER
{	
	BYTE	bPKMode;
	
	DSTC_DUNGEON_PKMODE()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_PKMODE;	}
	DWORD	GetPacketSize()		{ return sizeof( DSTC_DUNGEON_PKMODE );	}
};


struct DSTC_DUNGEON_USED_POTION : public DUNGEON_PACKET_HEADER
{
	BYTE	bType;
	DWORD	dwUserIndex;
	
	DSTC_DUNGEON_USED_POTION()	{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_USED_POTION;}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_DUNGEON_USED_POTION); }	
};


// ���÷����� ���۳��̾�.
struct	DSTC_PLAYER_BAD : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwUserIndex;
	BYTE	byBad;
	
	DSTC_PLAYER_BAD()			{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_PLAYER_BAD;	}
	DWORD	GetPacketSize()		{ return sizeof(DSTC_PLAYER_BAD);	}
};


struct DSTC_PKCOUNT : public DUNGEON_PACKET_HEADER
{
	DWORD	dwUserIndex;	
	BYTE	bPKCount;
	BYTE	bPKRepeatCount;
	
	DSTC_PKCOUNT()				{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_PKCOUNT;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_PKCOUNT);	}
};


struct DSTC_BADACTION : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwUserIndex;
	BYTE	bBad;
	
	DSTC_BADACTION()		{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_BADACTION;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_BADACTION);	}
};


struct DSTC_ORIGINATIONITEMWITHEFFECTKIND : public DUNGEON_PACKET_HEADER
{
	DWORD	dwUserIndex;
	BYTE	bCreate;
	BYTE	bCount;
	BYTE	bOriginationItemWithEffectKind[0xff];
	
	DSTC_ORIGINATIONITEMWITHEFFECTKIND()				{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_ORIGINATIONITEMWITHEFFECTKIND;	bCount = 0;}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_ORIGINATIONITEMWITHEFFECTKIND)-sizeof(bOriginationItemWithEffectKind)+sizeof(bOriginationItemWithEffectKind[0])*bCount;	}
};


struct	DSTC_DUNGEON_CASTING_FAIL : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwUserIndex;
	BYTE	byErrorReason;

	DSTC_DUNGEON_CASTING_FAIL()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_DSTC::CMD_DUNGEON_CASTING_FAIL;	}
	DWORD	GetPacketSize()	{	return (sizeof(DSTC_DUNGEON_CASTING_FAIL)); }
};


struct DSTC_ITEMUPGRADE_FAIL_INSURANCE : public DUNGEON_PACKET_HEADER
{
	BYTE	bDelete;
	CItem	cItem;

	DSTC_ITEMUPGRADE_FAIL_INSURANCE()	{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_ITEMUPGRADE_FAIL_INSURANCE;	}
	DWORD	GetPacketSize()	{	return (sizeof(DSTC_ITEMUPGRADE_FAIL_INSURANCE)); }
};


struct DSTC_ITEMMAKING_SUCCESS : public DUNGEON_PACKET_HEADER
{	
	CItem	cItem;

	DSTC_ITEMMAKING_SUCCESS()	{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_ITEMMAKING_SUCCESS;	}
	DWORD	GetPacketSize()		{	return (sizeof(DSTC_ITEMMAKING_SUCCESS)); }
};


struct DSTC_ITEMMAKING_FAIL : public DUNGEON_PACKET_HEADER
{	
	DSTC_ITEMMAKING_FAIL()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_ITEMMAKING_FAIL;	}
	DWORD	GetPacketSize()		{	return (sizeof(DSTC_ITEMMAKING_FAIL)); }	
};


struct DSTC_MAKING_CANCEL : public DUNGEON_PACKET_HEADER
{	
	CItem	pItem[4];		
	BYTE	bZipCode;	
	BYTE	byType;
	
	DSTC_MAKING_CANCEL()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_ITEMMAKING_CANCEL; }
	DWORD	GetPacketSize()	{	return sizeof(DSTC_MAKING_CANCEL); }	
};


//------------------------------------------------------------------------------//
// �������� 1�� ��� ���� �޼����� ������.								
//------------------------------------------------------------------------------//
struct DSTC_GUILD_FLAG : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwPrevGuildId;
	DWORD	dwGuildId;
	BYTE	byGuildFlag;
	
	DSTC_GUILD_FLAG()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_GUILD_FLAG;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_GUILD_FLAG);	}
};


//------------------------------------------------------------------------------//
// ����,������ ��ų ������ ������ ������.									
//------------------------------------------------------------------------------//
struct DSTC_SKILL_INFO : public DUNGEON_PACKET_HEADER
{
	BOOL	bSkillInfo;	
	BYTE	bySkillIndex;
	
	DSTC_SKILL_INFO()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_SKILL_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_SKILL_INFO);	}
};


//------------------------------------------------------------------------------//
// ����,������ Status ������ ������ ������.								
//------------------------------------------------------------------------------//
struct DSTC_STATUS_INFO : public DUNGEON_PACKET_HEADER
{
	BOOL	bStatusInfo;	
	BYTE	byStatusType;
	BYTE	byObjectType;
	
	DSTC_STATUS_INFO()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_STATUS_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_STATUS_INFO);	}
};


//------------------------------------------------------------------------------//
// Mod ��ü �Ҷ� ����.													
//------------------------------------------------------------------------------//
struct DSTC_WEAPON_HAND : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwHand;
	DWORD	dwHand2;
	DWORD	dwUserIndex;	
	
	DSTC_WEAPON_HAND()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_WEAPON_HAND;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_WEAPON_HAND);	}
};


//------------------------------------------------------------------------------//
// Mod ��ü �Ҷ� ����(����).												
//------------------------------------------------------------------------------//
struct DSTC_ARMOR_ITEM : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwArmor;	
	DWORD	dwUserIndex;	
	
	DSTC_ARMOR_ITEM()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_ARMOR_ITEM;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_ARMOR_ITEM);	}
};


struct DSTC_BANK_INFO : public DUNGEON_PACKET_HEADER
{
	DWORD	dwMoney;
	BYTE	bSmallItemCount;
	BYTE	bLargeItemCount;
	ITEM_NODE_FOR_MSG Items[MAX_BANK_SMALL_POOL+MAX_BANK_LARGE_POOL];
	
	DSTC_BANK_INFO()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_BANK_INFO;	bSmallItemCount = 0; bLargeItemCount = 0;}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_BANK_INFO) - sizeof(ITEM_NODE_FOR_MSG) * (MAX_BANK_SMALL_POOL+MAX_BANK_LARGE_POOL - bSmallItemCount-bLargeItemCount);	}
};


//------------------------------------------------------------------------------//
// Mod ��ü �Ҷ� ����(���).												
//------------------------------------------------------------------------------//
struct DSTC_HELMET_ITEM : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwHelmet;	
	DWORD	dwUserIndex;	
	
	DSTC_HELMET_ITEM()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_HELMET_ITEM;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_HELMET_ITEM);	}
};


//------------------------------------------------------------------------------//
// ��� ���� ������ Ȯ�� ��Ŷ.											
//------------------------------------------------------------------------------//
struct DSTC_GUILD_ITEM : public DUNGEON_PACKET_HEADER
{	
	BYTE	bZipCode;
	BOOL	bResult;
	BYTE	bGuildType;
	
	DSTC_GUILD_ITEM()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_GUILD_RITEM;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_GUILD_ITEM);	}
};


struct DSTC_TRADE_REQUEST : public DUNGEON_PACKET_HEADER
{	
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];	
	DWORD	dwUserIndex;	
	BYTE	byCode;
	
	DSTC_TRADE_REQUEST()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_TRADE_REQUEST; }
	DWORD	GetPacketSize()	{	return sizeof(DSTC_TRADE_REQUEST); }
};


struct DSTC_TRADE_RESULT : public DUNGEON_PACKET_HEADER
{	
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];	
	DWORD	dwUserIndex;	
	BYTE	byCode;
	
	DSTC_TRADE_RESULT()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_TRADE_RESULT; }
	DWORD	GetPacketSize()	{	return sizeof(DSTC_TRADE_RESULT); }
};


struct DSTC_TRADE_CANCEL : public DUNGEON_PACKET_HEADER
{	
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];	
	BYTE	byType;
	BYTE	btItemCount;	
	BYTE	bInvType[MAX_TRADE_POOL];	
	ITEM_NODE_FOR_MSG Nodes[MAX_TRADE_POOL];
	
	DSTC_TRADE_CANCEL()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_TRADE_CANCEL; btItemCount =0;}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_TRADE_CANCEL) - sizeof(ITEM_NODE_FOR_MSG)*(MAX_TRADE_POOL-btItemCount); }
};


struct DSTC_TRADE_OK : public DUNGEON_PACKET_HEADER
{	
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];	
	BYTE	byType;		
	
	DSTC_TRADE_OK()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_TRADE_OK; }
	DWORD	GetPacketSize()	{	return sizeof(DSTC_TRADE_OK); }
};


struct DSTC_TRADE_ITEM : public DUNGEON_PACKET_HEADER
{	
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];	
	BYTE	byType;			
	DWORD	dwMoney;	
	BYTE	bItemCount;
	ITEM_NODE_FOR_MSG Nodes[MAX_TRADE_POOL];
	
	DSTC_TRADE_ITEM()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_TRADE_ITEM; bItemCount =0;}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_TRADE_ITEM) - sizeof(ITEM_NODE_FOR_MSG)*(MAX_TRADE_POOL-bItemCount); }
};


struct DSTC_TRADE_TRADE : public DUNGEON_PACKET_HEADER
{	
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];	
	BYTE	byType;		
	
	DSTC_TRADE_TRADE()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_TRADE_TRADE; }
	DWORD	GetPacketSize()	{	return sizeof(DSTC_TRADE_TRADE); }
};


struct DSTC_TRADE_RETURN : public DUNGEON_PACKET_HEADER
{
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];	
	BYTE	byType;		
	
	DSTC_TRADE_RETURN()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_TRADE_RETURN; }
	DWORD	GetPacketSize()	{	return sizeof(DSTC_TRADE_RETURN); }
};


struct DSTC_TRADE_DATA : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwMoeny;
	BYTE	btItemCount;
	ITEM_NODE_FOR_MSG Nodes[MAX_TRADE_POOL];
	
	DSTC_TRADE_DATA()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_TRADE_DATA; btItemCount = 0;}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_TRADE_DATA) - sizeof(ITEM_NODE_FOR_MSG) * (MAX_TRADE_POOL-btItemCount); }
};


struct DSTC_PARTY_PORTALMOVE : public DUNGEON_PACKET_HEADER
{	
	WORD	wDungeonID;
	BYTE	byLayerIndex;
	float	fPosX;
	float	fPosZ;
	DWORD	dwPartyId;
	
	DSTC_PARTY_PORTALMOVE()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_DSTC::CMD_PARTY_PORTALMOVE;	}
	DWORD	GetPacketSize()	{	return (sizeof(DSTC_PARTY_PORTALMOVE)); }
};


struct DSTC_PARTY_PORTALFAIL : public DUNGEON_PACKET_HEADER
{	
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	BYTE	byResult;
	
	DSTC_PARTY_PORTALFAIL()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_DSTC::CMD_PARTY_PORTALFAIL;	}
	DWORD	GetPacketSize()	{	return (sizeof(DSTC_PARTY_PORTALFAIL)); }
};


struct DSTC_PARTY_ARRAY : public DUNGEON_PACKET_HEADER
{	
	WORD	wId;	
	BYTE	bType;
	
	DSTC_PARTY_ARRAY()		{	bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_DSTC::CMD_PARTY_ARRAY; }
	DWORD	GetPacketSize()	{	return (sizeof(DSTC_PARTY_ARRAY)); }
};


struct DSTC_PARTY_MAIGCARRAYFAIL : public DUNGEON_PACKET_HEADER
{
	BYTE	byErrorCode;		
	
	DSTC_PARTY_MAIGCARRAYFAIL()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_PARTY_MAGICARRAYFAIL; }
	DWORD	GetPacketSize()		{	return sizeof(DSTC_PARTY_MAIGCARRAYFAIL); }
};

struct DSTC_PARTY_CONFIG : public DUNGEON_PACKET_HEADER
{
	PARTY_CONFIG pConfig;
	BYTE bMsg;

	DSTC_PARTY_CONFIG()			{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_PARTY_CONFIG; }
	DWORD	GetPacketSize()		{	return (sizeof(DSTC_PARTY_CONFIG)); }
};

//-----------------------------------------------------------------------------
// �̺�Ʈ ������ �� �� �̵��� ����� ������.
//-----------------------------------------------------------------------------
struct DSTC_EVENT_CHANGE_LAYER_RESULT : public DUNGEON_PACKET_HEADER
{	
	BYTE	bMoveResult;	// 0 : ���. 1 : �����ִ� ���Ͱ� ����. 2 : ���͸� �ʿ��� ��ŭ ������ ������. 3 : ���� ���� ���� ���� �ʹ� ����.
	
	DSTC_EVENT_CHANGE_LAYER_RESULT()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_EVENT_CHANGE_LAYER_RESULT;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_EVENT_CHANGE_LAYER_RESULT);	}
};


//-----------------------------------------------------------------------------
// ���� ���� ����ִ� Monster count
//-----------------------------------------------------------------------------
struct DSTC_EVENT_ALIVE_MONSTER_COUNT : public DUNGEON_PACKET_HEADER
{	
	WORD	wAliveMonsterCount;
	
	DSTC_EVENT_ALIVE_MONSTER_COUNT()	{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_EVENT_ALIVE_MONSTER_COUNT;	}
	DWORD	GetPacketSize()				{	return sizeof(DSTC_EVENT_ALIVE_MONSTER_COUNT);	}	
};


struct DSTC_GUILD_FAIL : public DUNGEON_PACKET_HEADER
{	
	CItem	cItem;
	
	DSTC_GUILD_FAIL()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_GUILD_FAIL; }
	DWORD	GetPacketSize()	{	return sizeof(DSTC_GUILD_FAIL); }
};


struct DSTC_UPGRADE_FAIL : public DUNGEON_PACKET_HEADER
{	
	CItem	pItem[4];		
	BYTE	bZipCode;	
	BYTE	byType;
	
	DSTC_UPGRADE_FAIL()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_UPGRADE_CANCEL; }
	DWORD	GetPacketSize()	{	return sizeof(DSTC_UPGRADE_FAIL); }
};


//-----------------------------------------------------------------------------
// ���� �޽���.
//-----------------------------------------------------------------------------
struct DSTC_DUNGEON_NOTICE : public DUNGEON_PACKET_HEADER
{	
	WORD	wMsgLen;
	char	szMsg[1024];
	
	DSTC_DUNGEON_NOTICE()		{ bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_DUNGEON_NOTICE; }
	DWORD		GetPacketSize()	{ return (szMsg - (char*)this) + wMsgLen; }
};


//-----------------------------------------------------------------------------
// ������ Ŭ���̾�Ʈ�鿡�� ������ �޽���. (�����ʹ� �ٸ�.)
//-----------------------------------------------------------------------------
struct DSTC_DUNGEON_MESSAGE : public DUNGEON_PACKET_HEADER
{	
	BYTE	byType;					// Ÿ��. Ȯ���ؼ� �˾Ƽ� ����Ͻñ�.
	DWORD	dwMessageColor;			// �޽����� �÷�.
	WORD	wMsgLen;
	char	szMessage[1024];	
	
	DSTC_DUNGEON_MESSAGE()	{ memset(this, 0, sizeof(*this)); bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_MESSAGE; }
	DWORD GetPacketSize()	{ return (szMessage - (char*)this) + wMsgLen; }	
};


//------------------------------------------------------------------------------//
// ���� ���� ����														
//------------------------------------------------------------------------------//
struct DSTC_DUNGEON_JOIN_FAILED : public DUNGEON_PACKET_HEADER
{
	BYTE	bErrorCode;
	
	DSTC_DUNGEON_JOIN_FAILED()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_DUNGEON_JOIN_FAILED;	}
	DWORD	GetPacketSize()		{ return sizeof( DSTC_DUNGEON_JOIN_FAILED );	}
};


struct DSTC_TRADE_CANCELFAIL : public DUNGEON_PACKET_HEADER
{
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];	
	BYTE	byType;
	
	DSTC_TRADE_CANCELFAIL()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_TRADE_CANCELFAIL; }
	DWORD	GetPacketSize()	{	return sizeof(DSTC_TRADE_CANCELFAIL); }
};


struct DSTC_STATUS_INIT : public DUNGEON_PACKET_HEADER
{
	WORD		wPoint;	
	
	DSTC_STATUS_INIT()			{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_STATUS_INIT; }
	DWORD	GetPacketSize()		{ return sizeof( DSTC_STATUS_INIT );	}
};


struct DSTC_SKILL_INIT : public DUNGEON_PACKET_HEADER
{	
	BYTE		bySkill[MAX_SKILL];
	WORD		wPoint;		
	BYTE		bySelectedSkill[2];
	
	DSTC_SKILL_INIT()			{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_SKILL_INIT; }
	DWORD	GetPacketSize()		{ return sizeof( DSTC_SKILL_INIT );	}
};


struct DSTC_GUILD_INFO : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwUserIndex;
	DWORD	dwGuildId;
	BYTE	byRank;
	char	szNickName[MAX_CHARACTER_NAME_LENGTH];
	
	DSTC_GUILD_INFO()		{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_GUILD_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_GUILD_INFO);	}
};


struct	DSTC_SHOP_FAIL : public DUNGEON_PACKET_HEADER
{	
	BYTE	byType;
	
	DSTC_SHOP_FAIL()			{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_SHOP_FAIL;	}
	DWORD	GetPacketSize()		{ return sizeof( DSTC_SHOP_FAIL);	}
};


struct DSTC_PLAYERSHOP_ITEM : public DUNGEON_PACKET_HEADER
{
	BYTE	bItemCount;
	ITEM_NODE_FOR_MSG ItemNode[MAX_PLAYER_SHOP_INV];	
	
	DSTC_PLAYERSHOP_ITEM()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_PLAYERSHOP_ITEM; bItemCount = 0;	}
	DWORD GetPacketSize()		{ return sizeof(DSTC_PLAYERSHOP_ITEM) - (MAX_PLAYER_SHOP_INV - bItemCount) * sizeof(ITEM_NODE_FOR_MSG);	}	
};


struct DSTC_PLAYERSHOP_SELL : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwMoney;
	BYTE	byZipCode;	
	CItem	cItem;	
	BYTE	byBuyRealCnt;
	
	DSTC_PLAYERSHOP_SELL()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_PLAYERSHOP_SELL;	byBuyRealCnt = 0; }
	DWORD GetPacketSize()		{ return sizeof(DSTC_PLAYERSHOP_SELL); }	
};


struct DSTC_PLAYERSHOP_SOLDOUT : public DUNGEON_PACKET_HEADER
{	
	BYTE	byType;
	
	DSTC_PLAYERSHOP_SOLDOUT()	{	bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_DSTC::CMD_PLAYERSHOP_SOLDOUT;	}
	DWORD	GetPacketSize()		{	return sizeof(DSTC_PLAYERSHOP_SOLDOUT);	}	
};

struct DSTC_USER_PORTAL : public DUNGEON_PACKET_HEADER
{	
	DUNGEON_MOVE_INFO DMI;
	
	DWORD	dwTargetUserIndex;		// append by seven for GM [12/10/2004]
	
	DSTC_USER_PORTAL()			{	bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_DSTC::CMD_USER_PORTAL;	}
	DWORD	GetPacketSize()		{	return sizeof(DSTC_USER_PORTAL);	}	
};


struct	DSTC_SHOP_ITEM : public DUNGEON_PACKET_HEADER
{
	BYTE	bItemCount;
	ITEM_NODE_FOR_MSG Nodes[MAX_PLAYER_SHOP_INV];
	
	DSTC_SHOP_ITEM()			{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_SHOP_ITEM; bItemCount = 0;	}
	DWORD	GetPacketSize()		{ return sizeof( DSTC_SHOP_ITEM ) - (MAX_PLAYER_SHOP_INV-bItemCount)*sizeof(ITEM_NODE_FOR_MSG);	}
};


struct	DSTC_PLAYER_REST : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwUserIndex;
	BYTE	byType;
	
	DSTC_PLAYER_REST()			{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_PLAYER_REST;	}
	DWORD	GetPacketSize()		{ return sizeof(DSTC_PLAYER_REST);	}
};


struct DSTC_GUILD_PORTALFAIL : public DUNGEON_PACKET_HEADER
{	
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	BYTE	byResult;
	
	DSTC_GUILD_PORTALFAIL()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_DSTC::CMD_GUILD_PORTALFAIL;	}
	DWORD	GetPacketSize()	{	return (sizeof(DSTC_GUILD_PORTALFAIL)); }
};


struct DSTC_GUILD_RANK : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwUserIndex;	
	BYTE	byFlag;	
	
	DSTC_GUILD_RANK()		{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_GUILD_RANK;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_GUILD_RANK);	}
};


struct DSTC_USER_SPEED : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwUserIndex;	
	float	fSpeed;
	
	DSTC_USER_SPEED()		{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_USER_SPEED;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTC_USER_SPEED);	}
};


struct DSTC_OVERDRIVE_CHK : public DUNGEON_PACKET_HEADER
{	
	CItem	cItem;	
	BYTE	byZipCode;
	BYTE	byExc;
	
	DSTC_OVERDRIVE_CHK()		{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_OVERDRIVE_CHK;	}
	DWORD	GetPacketSize()		{	return sizeof(DSTC_OVERDRIVE_CHK);	}
};


//-----------------------------------------------------------------------------
// ���׷��̵� üũ.
//-----------------------------------------------------------------------------
struct DSTC_UPGRADE_CHK : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwUserIndex;
	BYTE	byUpgrade;
	
	DSTC_UPGRADE_CHK()				{ bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_UPGRADE_CHK; }
	DWORD		GetPacketSize()		{ return sizeof(DSTC_UPGRADE_CHK); }
};


//-----------------------------------------------------------------------------
// ��Ƽ �Խ��� ����.
//-----------------------------------------------------------------------------
struct DSTC_PARTY_BOARD : public DUNGEON_PACKET_HEADER
{	
	DWORD	dwPartyId;
	
	DSTC_PARTY_BOARD()				{ bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_PARTY_BOARD; }
	DWORD		GetPacketSize()		{ return sizeof(DSTC_PARTY_BOARD); }
};


struct DSTC_GUILD_WAR_MESSAGE : public DUNGEON_PACKET_HEADER
{
	char	szGuildName[MAX_GUILD_NAME_LENGTH];
	char	szGuildNameDst[MAX_GUILD_NAME_LENGTH];
	BYTE	byType;
	
	DSTC_GUILD_WAR_MESSAGE()	{ bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_GUILD_WAR_MESSAGE; }
	DWORD	GetPacketSize()		{ return sizeof(DSTC_GUILD_WAR_MESSAGE); }
};


struct DSTC_GUILDUSER_FLAG : public DUNGEON_PACKET_HEADER
{
	DWORD	dwGuildId;
	DWORD	dwUserIndex;
	BYTE	byGuildFlag;
	
	DSTC_GUILDUSER_FLAG()		{ bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_DSTC::CMD_GUILDUSER_FLAG; }
	DWORD	GetPacketSize()		{ return sizeof(DSTC_GUILDUSER_FLAG); }
};


struct	DSTC_ITEM_MOVE : public DUNGEON_PACKET_HEADER
{
	BYTE	bSrc;
	BYTE	bDest;	
	BYTE	bSrcIndex;
	BYTE	bDestIndex;
	DWORD	dwUnitID;			
	INT64	i64ItemID;
	BYTE	bSectionNum;
	DWORD	dwMoney;
	CItem	Item;
	BYTE	bErrorCode;
	BYTE	bQuantity;

	DSTC_ITEM_MOVE()
	{
		bStatus		= UPDATE_GAME_PLAY;
		bHeader		= Protocol_DSTC::CMD_ITEM_MOVE;		
		bSrc		= 0xff;
		bDest		= 0xff;
		bSrcIndex	= 0xff;
		bDestIndex	= 0xff;
		dwUnitID	= 0;
		i64ItemID	= 0;
		bSectionNum = 0;
		dwMoney		= 0;
		memset(&Item,0,sizeof(CItem));
		bErrorCode	= 0xff;
		bQuantity	= 0;
	}

	DWORD	GetPacketSize()		{ return sizeof( DSTC_ITEM_MOVE );		}
};


struct DSTC_PLAYERSHOPITEM_RETURN_INV : public DUNGEON_PACKET_HEADER
{
	BYTE	bItemCount;
	ITEM_NODE_FOR_MSG_NATIVE ItemNodeNative[MAX_PLAYER_SHOP_INV];

	DSTC_PLAYERSHOPITEM_RETURN_INV()
	{
		bStatus		= UPDATE_GAME_PLAY;		
		bHeader		= Protocol_DSTC::CMD_PLAYERSHOPITEM_RETURN_INV;
		bItemCount	= 0;	
	}

	DWORD	GetPacketSize()
	{	return sizeof(DSTC_PLAYERSHOPITEM_RETURN_INV) - (MAX_PLAYER_SHOP_INV - bItemCount) * sizeof(ITEM_NODE_FOR_MSG_NATIVE);	}	
};


struct DSTC_TRADE_CANCEL_RETURN_INV : public DUNGEON_PACKET_HEADER
{
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	BYTE	byType;
	BYTE	bItemCount;
	ITEM_NODE_FOR_MSG_NATIVE ItemNodeNative[MAX_TRADE_POOL];
	
	DSTC_TRADE_CANCEL_RETURN_INV()
	{
		bStatus		= UPDATE_GAME_PLAY;
		bHeader		= Protocol_DSTC::CMD_TRADE_CANCEL_RETURN_INV;
		bItemCount = 0;
	}

	DWORD	GetPacketSize()
	{
		return sizeof(DSTC_TRADE_CANCEL_RETURN_INV) - sizeof(ITEM_NODE_FOR_MSG_NATIVE) * (MAX_TRADE_POOL - bItemCount);
	}
};


struct	DSTC_ITEM_USED_NATIVE : public DUNGEON_PACKET_HEADER
{
	BYTE	bSrc;		
	BYTE	bSrcIndex;
	BYTE	bItemUsedNativeType;
	BYTE	bItemUsedNativeValue;
	BYTE	bCommonIndex;
	BYTE	bErrorCode;
	DWORD	dwCommonId;
	DWORD	dwPortalUserIndex;
	CItem	cItem;
	
	DSTC_ITEM_USED_NATIVE()
	{
		bStatus		= UPDATE_GAME_PLAY;
		bHeader		= Protocol_DSTC::CMD_ITEM_USED_NATIVE;
		bErrorCode	= 0xff;
	}

	DWORD	GetPacketSize()		{ return sizeof(DSTC_ITEM_USED_NATIVE);	}
};


//------------------------------------------------------------------------------//
// ����� ������ ������Ʈ 												
//------------------------------------------------------------------------------//
struct DSTC_UPDATE_GUARDIAN_ITEM : public DUNGEON_PACKET_HEADER
{
	BYTE	bySlotIndex;
	CItem	cItem;

	DSTC_UPDATE_GUARDIAN_ITEM()	{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_UPDATE_GUARDIAN_ITEM; }
	DWORD	GetPacketSize()		{ return sizeof( DSTC_UPDATE_GUARDIAN_ITEM );	}
};


struct DSTC_UPGRADE_CANCAL_RETURN_INV : public DUNGEON_PACKET_HEADER
{
	BYTE	byType;
	BYTE	bItemCount;
	ITEM_NODE_FOR_MSG_NATIVE ItemNodeNative[MAX_UPGRADE_POOL];

	DSTC_UPGRADE_CANCAL_RETURN_INV() 
	{
		bStatus		= UPDATE_GAME_PLAY;
		bHeader		= Protocol_DSTC::CMD_UPGRADE_CANCEL_RETURN_INV;
		bItemCount = 0;
	}

	DWORD	GetPacketSize()
	{ return sizeof(DSTC_UPGRADE_CANCAL_RETURN_INV) - sizeof(ITEM_NODE_FOR_MSG_NATIVE) * (MAX_UPGRADE_POOL - bItemCount); }
};


struct DSTC_MAKING_CANCAL_RETURN_INV : public DUNGEON_PACKET_HEADER
{
	BYTE	byType;
	BYTE	bItemCount;
	ITEM_NODE_FOR_MSG_NATIVE ItemNodeNative[MAX_MAKING_POOL];

	DSTC_MAKING_CANCAL_RETURN_INV()
	{
		bStatus		= UPDATE_GAME_PLAY;
		bHeader		= Protocol_DSTC::CMD_MAKING_CANCEL_RETURN_INV;
		bItemCount = 0;
	}

	DWORD	GetPacketSize()
	{
		return sizeof(DSTC_MAKING_CANCAL_RETURN_INV) - sizeof(ITEM_NODE_FOR_MSG_NATIVE) * (MAX_MAKING_POOL - bItemCount);
	}
};


struct DSTC_GUARDIANSUPPLIES_CANCAL_RETURN_INV : public DUNGEON_PACKET_HEADER
{
	ITEM_NODE_FOR_MSG_NATIVE ItemNodeNative;

	DSTC_GUARDIANSUPPLIES_CANCAL_RETURN_INV()
	{
		bStatus		= UPDATE_GAME_PLAY;
		bHeader		= Protocol_DSTC::CMD_GUARDIANSUPPLIES_CANCAL_RETURN_INV;
	}

	DWORD	GetPacketSize()	{	return sizeof(DSTC_GUARDIANSUPPLIES_CANCAL_RETURN_INV);		}
};


struct	DSTC_SWITCH_WEAPON : public DUNGEON_PACKET_HEADER
{
	BYTE	bCurrnetHand;

	DSTC_SWITCH_WEAPON()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_SWITCH_WEAPON;	}
	DWORD	GetPacketSize()		{ return sizeof( DSTC_SWITCH_WEAPON );	}
};


//-----------------------------------------------------------------------------
// �̺�Ʈ ���� Ŭ���� ���� 
//-----------------------------------------------------------------------------
struct DSTC_EVENT_DUNGEON_CLEAR_PRESENT : public DUNGEON_PACKET_HEADER
{
	DWORD dwUserIndex;
	DWORD wPresentMoney;

	DSTC_EVENT_DUNGEON_CLEAR_PRESENT()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_DSTC::CMD_EVENT_DUNGEON_CLEAR_PRESENT; }
	DWORD	GetPacketSize()					{ return sizeof(DSTC_EVENT_DUNGEON_CLEAR_PRESENT); }
};


#pragma pack(pop)