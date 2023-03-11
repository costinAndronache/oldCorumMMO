#pragma once

#include "Localizing.h"
#include <crtdbg.h>


#define _CAMERA_ACCEL_MOVE_X_ 0.2f 
#define _CAMERA_ACCEL_MOVE_Y_ 0.2f
#define	CORUM_AUTOPATCH_EXE_FILE	"CorumAutopatch.exe"	//kjk(41119)
#define ITEMINFO_RENDER_POSITION	15
#define EXECUTE_TYPE_CORUM				0
#define EXECUTE_TYPE_NETMARBLE			1
#define MAX_RANDOM_CODE_FOR_NETMARBLE	5
#define MAX_USER_PER_GIRD				30	//Added by KBS 030107
#define MAX_MOVE_SPOT_NUM				10
#define GM_ARMOR					1142
#define	MAX_LEVEL					255
#define	MAX_MESSENGER_USER			32
#define MAX_ID_LENGTH					20
#define MAX_PASSWORD_LENGTH				20
#define MAX_CHARACTER_NAME_LENGTH		20
#define MAX_CHARACTER_REAL_NICKNAME_LENGTH	16
#define MAX_CHARACTER_NAME_REAL_LENGTH  16
#define MAX_ID_REAL_LENGTH				16
#define MAX_PASSWORD_REAL_LENGTH2		16
#define MAX_GUILD_NAME_LENGTH			20
#define MAX_GUILD_NAME_REAL_LENGTH		10
#define MAX_DUNGEON_MEMO_LENGTH			32
#define MAX_DUNGEON_NAME_LENGTH			32
#define MAX_RANK_REAL_NAME				8
#define MAX_NICK_REAL_NAME				16
#if IS_CHINA_LOCALIZING()
	#undef	MAX_PASSWORD_REAL_LENGTH
	#define	MAX_PASSWORD_REAL_LENGTH			20
#endif
#define	MAX_MONEY_PLAYERSHOP			4000000000

/*
enum DUNGEON_JOIN_FAIL
{
	DUNGEON_JOIN_SUCCESS							= 0,
	DUNGEON_JOIN_FAIL_SERVER_IS_NOT_CONNECTED		= 1,
	DUNGEON_JOIN_FAIL_SERVER_INFO_IS_NOT_FOUND		= 2,
	DUNGEON_JOIN_FAIL_USER_ACCEPT_FAIL				= 3,
	DUNGEON_JOIN_FAIL_NOT_ENOUGH_ENTER_KARZ			= 4,
	DUNGEON_JOIN_FAIL_UNKNOWN_DUNGEOIN_ID			= 5,
	DUNGEON_JOIN_FAIL_NOT_MINMAX_LEVEL				= 6,
	DUNGEON_JOIN_FAIL_EVENT_DUNGEON_OPEN_FULL		= 100,
	DUNGEON_JOIN_FAIL_EVENT_DUNGEON_OPEN_TIME_UP	= 101,
	DUNGEON_JOIN_FAIL_INVALID_EVENT_ENTER_LEVEL		= 0xff,
	DUNGEON_JOIN_FAIL_DUNGEON_INFO_IS_NOT_FOUND		= 200,
	DUNGEON_JOIN_FAIL_USER_OVERFLOW					= 201,
	DUNGEON_JOIN_FAIL_UNAUTHORIZED_USER				= 202,
	DUNGEON_JOIN_FAIL_INVALID_CLASS_OR_HEAD			= 203,
	DUNGEON_JOIN_FAIL_INVALID_LAYERINDEX			= 204,
	DUNGEON_JOIN_FAIL_INVALID_START_SPOT			= 205,
	DUNGEON_JOIN_FAIL_INSERTLAYERUSER_FAILED		= 206,
	DUNGOEN_JOIN_FAIL_IMPOSSIBILITY_AREA			= 207,
};
*/

#define FONT_D3D_LOGIN					0
#define FONT_D3D_GAME					1
#define FONT_SS3D_LOGIN					2
#define FONT_SS3D_GAME					3


#define DEG01		0.017453293005625f		
#define DEG45		PI_DIV_4
#define DEG90		PI_DIV_2
#define DEG180		PI
#define DEG270		PI + DEG90
#define DEG360		PI_MUL_2


#define CAMERA_DISTANCE				40.0f		// ���ѹ��� ����� ����.


#define __MAX_CAMERA_X__			70.0f
#define __MIN_CAMERA_X__			30.0f
//#define __MAX_CAMERA_X__			80.0f
//#define __MIN_CAMERA_X__			5.0f
#define __DEFAULT_CAMERA_X__		45

#define CAMERA_X					0.4f		// ���ѹ��� ī�޶� ȭ���� ��.
#define CAMERA_Y					0.5f

#define		MAX_TRADE			14

#define MAX_SERVER_SET_NAME_LENGTH	32
#define MAX_IP_LENGTH				16
#define MAX_CHAR_SELECT_NUM			4
#define	MAX_KIND_OF_CLASS			5	//Ŭ���� ���� 
#define MAX_KIND_OF_HEAD			22
#define MAX_KIND_OF_HEAD_MALE		22	//���� �Ӹ� ���� 	
#define MAX_KIND_OF_HEAD_FEMALE		22	//���� �Ӹ� ���� 


//021203
#define INPUT_FOCUS_CHAT			0
#define INPUT_FOCUS_ID				1
#define INPUT_FOCUS_PASSWORD		2
//--

// ���� ���� ����.
// Modify seven
#define SOUND_WORLD_PLAYING			0
#define SOUND_DUNGEON_PLAYING		1

#define	SOUND_NUMBER_CHARACTER		3001		// �ɸ��� ���� ����.

// sound character.
#define SOUND_PER_CHARACTER			10
/*
#define CHARACTER_SOUND_ATTACK_FIGHTER	1 // ������, ������Ʈ, ���ӳ�, ������, �Ҽ����� ���� 
#define CHARACTER_SOUND_ATTACK_PRIST	2 
#define CHARACTER_SOUND_ATTACK_SUMMONER	3
#define CHARACTER_SOUND_ATTACK_RANGER	4 
#define CHARACTER_SOUND_ATTACK_SORCERESS 5	
*/

#define CHARACTER_SOUND_ATTACK		0
#define	CHARACTER_SOUND_DAMAGE		3
#define CHARACTER_SOUND_DEAD		6

// sound weapon attack
#define SOUND_NUMBER_WEAPON			3101
#define SOUND_PER_WEAPON			10

// sound weapon damage
#define SOUND_NUMBER_WEAPONDAMAGE	3201
#define SOUND_PER_WEAPONDAMAGE		8
#define WEAPONDAMAGE_SOUND_DAMAGE	0
#define WEAPONDAMAGE_SOUND_CRI		3
#define WEAPONDAMAGE_SOUND_BLOCK	5

#define SOUND_TYPE_PLAYER			10
#define SOUND_TYPE_MONSTER			11
#define SOUND_TYPE_GAMEPLAY			12
#define SOUND_TYPE_CHARACTER		13
#define SOUND_TYPE_WEAPONATTACK		14
#define SOUND_TYPE_WEAPONDAMAGE		15

#define	SOUND_TYPE_SYSTEM			16
#define	SOUND_TYPE_ITEM				17

#define	SOUND_SYSTEM_WNDOPEN		1
#define	SOUND_SYSTEM_WNDCLOSE		2
#define	SOUND_SYSTEM_BTNCLICK		3
#define	SOUND_SYSTEM_ITEMCLICK		4
#define	SOUND_SYSTEM_ITEMUPSUCESS	5
#define	SOUND_SYSTEM_ITEMUPFAIL		6
#define	SOUND_SYSTEM_LEVELUP		7
#define	SOUND_SYSTEM_ATTPOINTUP		8
#define	SOUND_SYSTEM_SKILLPOINTUP	9
#define	SOUND_SYSTEM_SIEGNOTICE		10
#define	SOUND_SYSTEM_SIEGSTART		11
#define	SOUND_SYSTEM_REQUEST		12
#define	SOUND_SYSTEM_TRADEREADY		13
#define	SOUND_SYSTEM_TRADEEND		14
#define	SOUND_SYSTEM_ERRORMSG		15
#define	SOUND_SYSTEM_CHARREAPPEAR	16
#define	SOUND_SYSTEM_PORTALSETTING	17
#define	SOUND_SYSTEM_PORTALOPEN		18
#define	SOUND_SYSTEM_PORTALENTER	19
#define	SOUND_SYSTEM_CCPOPEN		20
#define	SOUND_SYSTEM_CCPCLOSE		21
#define	SOUND_SYSTEM_DOOROPEN		22
#define	SOUND_SYSTEM_DOORCLOSE		23
#define	SOUND_SYSTEM_GETITEM		24
#define	SOUND_SYSTEM_HPRECOVER		25
#define SOUND_SYSTEM_WEAPONSWITCH	26
#define SOUND_SYSTEM_CHARSELBTNON		27
#define SOUND_SYSTEM_CHARSELBTNCLICK	28
#define SOUND_SYSTEM_CHARSELROTATE		29
#define SOUND_SYSTEM_CHARSELSPOT		30
#define SOUND_SYSTEM_GUILDWAR_COUNT		31
#define SOUND_SYSTEM_GUILDWAR_START		32
#define SOUND_SYSTEM_GUILDWAR_WIN		33
#define SOUND_SYSTEM_GUILDWAR_LOSE		34


// ������ ���� //
#define	SOUND_ITEM_SWORD			1
#define	SOUND_ITEM_MACE				2
#define	SOUND_ITEM_KATAR			3
#define	SOUND_ITEM_SWORD2			4
#define	SOUND_ITEM_MACE2			5
#define	SOUND_ITEM_SPEAR			6
#define	SOUND_ITEM_WAND				7
#define	SOUND_ITEM_STAFF			8
#define	SOUND_ITEM_HELM				9
#define	SOUND_ITEM_ARMOR			10
#define	SOUND_ITEM_SHIELD			11
#define	SOUND_ITEM_BELT				12
#define	SOUND_ITEM_GLOVE			13
#define	SOUND_ITEM_BOOTS			14
#define	SOUND_ITEM_RING				15
#define	SOUND_ITEM_AMULET			16
#define	SOUND_ITEM_LEATHER			17
#define	SOUND_ITEM_WOOD				18
#define	SOUND_ITEM_METAL			19
#define	SOUND_ITEM_ZODIAC			20
#define	SOUND_ITEM_GEM				21
#define	SOUND_ITEM_ELIXIR			22
#define	SOUND_ITEM_JEWEL			23
#define	SOUND_ITEM_POSION			24
#define SOUND_ITEM_TELE_GATE_OPEN	25
#define SOUND_ITEM_TELEPORT			26

#define SOUND_ITEM_THROW_BOTARI		101	// ������ �ٴڿ� ����߸��� 
#define SOUND_ITEM_THROW_JEWEL_BOX	102	// �������ڸ� �ٴڿ� ����߷��� �� 
#define SOUND_ITEM_THROW_MAGICITEM	103	// ������������ �ٴڿ� ����߷��� �� 
#define SOUND_ITEM_THROW_SETITEM	104	// ��Ʈ �������� �ٴڿ� ����߷��� �� 
#define SOUND_ITEM_THROW_POTION		105	// ������ �ٴڿ� ����߷��� �� 
#define SOUND_ITEM_THROW_MONEY		106	// ���� �ٴڿ� ����߷��� ��
#define SOUND_ITEM_THROW_MONSTERBYMAGICITEM 107	// ���Ͱ� ���� �������� ��������.

#define SOUND_EFFECT_CASTING1		0	// �� ������ ���ڴ� �ٲ�� ����. �迭�� ���� �ɲ���.
#define SOUND_EFFECT_CASTING2		1	// �� ������ ���ڴ� �ٲ�� ����.
#define SOUND_EFFECT_CASTING3		2	// �� ������ ���ڴ� �ٲ�� ����.
#define SOUND_EFFECT_START			3	// �� ������ ���ڴ� �ٲ�� ����.
#define SOUND_EFFECT_MID			4	// �� ������ ���ڴ� �ٲ�� ����.
#define SOUND_EFFECT_END			5	// �� ������ ���ڴ� �ٲ�� ����.
#define SOUND_EFFECT_GENERAL1		6	// �� ������ ���ڴ� �ٲ�� ����.
#define SOUND_EFFECT_GENERAL2		7	// �� ������ ���ڴ� �ٲ�� ����.
#define MAX_NPC_NAME_LENGTH			32
#define MAX_NPC_MSG_LENGTH			256
#define MAX_NPC_NUM_PER_MAP			12
#define MAX_NPC_NUM_PER_VILLAGE		60



#define MAX_SKILL_NODE				20


#define SKILL_TYPE_MASTERY			0
#define SKILL_TYPE_ACTIVE			1
#define SKILL_TYPE_OVERDIRVE		2
#define SKILL_TYPE_PASSIVE			3


#define MAX_CHATLIST_TEXT			58


#define	__MAX_MSG_CNT__				256
#define	__MAX_MESSGAE__				256
#define	__MAX_COMMAND__				100
#define	__MAX_COMMANDSIZE__			256
#define	__CHAT_MSG__				0
#define	__CHAT_COMMAND__			1
#define	__MAX_INDEX__				4
#define	__SERVER_WORLD__			1
#define	__SERVER_DUNGEON__			2
#define	__SCRT_FAIL__				0
#define	__SCRT_SUCCESS__			1
#define	__SCRT_ID__					2
#define	__ITEM_WORD_FAIL__			1
#define	__CHAR_WORD_FAIL__			2
#define	__SKILL_WORD_FAIL__			3
#define	__OPTION_WORD_FAIL__		4
#define	__MENU_OFF_FAIL__			5
#define	__CHAT_WORD_FAIL__			6
#define	__MAX_INPUT_BUFFER_SIZE__	256
#define	__MAX_CHAT__				5
#define	__MAX_READ_MESSAGE__		10
#define	__MAX_WORD_QUEUE__			256
#define	__MAX_IDBUUFER_SIZE__		16
#define	__MAX_CHAT_INDEX__			5
#define	__MAX_CHAT_SCROLL__			251

//�Ϻ��� ���� ��� ä��ť�� ũ�⸦ 10���� ����(����â 2005.03.03) 
/*
#if IS_JAPAN_LOCALIZING() // �Ϻ����� ��� ä��ť�� ũ�⸦ 8�� ����.
#define	__MAX_QUEUE__				8 // �Ϻ��� 8�� �Ǿ� �־��� 
#else

#define	__MAX_QUEUE__				10 //(����â 2005.03.02) - ä��ť(�����ϴܿ� ä�ñ� �ö���� �κ�) ���� �κ� 
#endif 
*/
// sung-han 2005-03-30 ���ä�� ǥ�ü�
#define __MIN_QUEUE__				5
#define	__MAX_QUEUE__				(__MIN_QUEUE__ * 3)

#define	__MAX_CHAT_LIST_QUEUE__		256
#define	MAX_SKILL					151

#define	__MAX_PLAYERSHOP_MEMO__		30

//== Skill Define ==//
#define	__SKILL_TYPE_COUNT__			5

#define	__SKILL_TYPE_MANA__				0
#define	__SKILL_TYPE_AURA__				1
#define	__SKILL_TYPE_DIVINE__			2
#define	__SKILL_TYPE_SUMMON__			3
#define	__SKILL_TYPE_CHAKRA__			4

#define	__KEY_SKILL_OPEN__				0				// ��ųâ ����.
#define	__KEY_GROUP_OPEN__				1				// �׷�â ����.
#define	__KEY_WEAPON_SWITCH__			2				// ���� ����ġ.
#define	__KEY_CHARACTER_OPEN__			3				// ĳ����â ����.
#define	__KEY_INVENTORY_OPEN__			4				// �κ��丮â ����.
#define	__KEY_CHAT_CLAER__				5				// ä�� �����.
#define	__KEY_OPTION_OPEN__				6				// �ɼ�â ����.
#define	__KEY_AUTO_CASTING_TOGGLE1__	7				// ����ĳ���� ���1.
#define	__KEY_AUTO_CASTING_TOGGLE2__	8				// ����ĳ���� ���2.
#define __KEY_ITEM_BELT1__				9				// ������ ��Ʈ1.
#define __KEY_ITEM_BELT2__				10				// ������ ��Ʈ2.
#define __KEY_ITEM_BELT3__				11				// ������ ��Ʈ3.
#define __KEY_ITEM_BELT4__				12				// ������ ��Ʈ4.
#define __KEY_ITEM_BELT5__				13				// ������ ��Ʈ5.
#define __KEY_ITEM_BELT6__				14				// ������ ��Ʈ6.
#define __KEY_ITEM_BELT7__				15				// ������ ��Ʈ7.
#define __KEY_ITEM_BELT8__				16				// ������ ��Ʈ8.
#define	__KEY_ITEM_GUARDIAN__			17				// ������ �����.
#define __KEY_SKILL1__					18				// ��ų1.
#define __KEY_SKILL2__					19				// ��ų2.
#define __KEY_SKILL3__					20				// ��ų3.
#define __KEY_SKILL4__					21				// ��ų4.
#define __KEY_SKILL5__					22				// ��ų5.
#define __KEY_SKILL6__					23				// ��ų6.
#define __KEY_SKILL7__					24				// ��ų7.
#define __KEY_SKILL8__					25				// ��ų8.
#define __KEY_SKILL9__					26				// ��ų9.
#define __KEY_SKILL10__					27				// ��ų10.
#define __KEY_SKILL11__					28				// ��ų11.
#define __KEY_SKILL12__					29				// ��ų12.
#define __KEY_SKILL13__					30				// ��ų13.
#define __KEY_SKILL14__					31				// ��ų14.
#define __KEY_SKILL15__					32				// ��ų15.
#define __KEY_SKILL16__					33				// ��ų16.
#define __KEY_ITEM__					34				// ������ �ݱ�.
#define __KEY_GUARDIAN_OPEN__			35				// �����â ����.

#define CHAT_MODE_CHAT					0
#define CHAT_MODE_MESSAGE				1

#define CHAT_TYPE_ALL					0
#define CHAT_TYPE_PARTY					1
#define CHAT_TYPE_GUILD					2
#define CHAT_TYPE_WHISPER				3
#define CHAT_TYPE_FRIEND				4

#define	__MAX_ZOOM_IN__					0
#define __MAX_ZOOM_OUT__				24	// 24 , �ִ���
#define __MAX_ZOOM_DEFAULT__			16

#define	__ITEM_LARGE__					1
#define	__ITEM_SMALL__					2
#define	__ITEM_EQUIP__					3

#define __MAX_ITEM_MOD__				5
#define __MAX_IVICON_COUNT__			6
#define __MAX_BELTICON_COUNT__			8

#define	__GAME_SIZE_X__					1024
#define	__GAME_SIZE_Y__					768

// ������ ��ġ ���� //
#define	EXITWND_SIZE_X				256
#define	EXITWND_SIZE_Y				128
#define	EXITWND_START_X				256
#define	EXITWND_START_Y				256
/*
#define	CHARWND_SIZE_X				256
#define	CHARWND_SIZE_Y				384
#define	CHARWND_START_X				384
#define	CHARWND_START_Y				128

#define	CHATLISTWND_SIZE_X			420
#define	CHATLISTWND_SIZE_Y			136
#define CHATLISTWND_START_X			604
#define CHATLISTWND_START_Y			512

#define CHATWND_SIZE_X				256
#define CHATWND_SIZE_Y				39
#define	CHATWND_START_X				384
#define	CHATWND_START_Y				729

#define	COMMANDWND_SIZE_X			520
#define	COMMANDWND_SIZE_Y			256
#define	COMMANDWND_START_X			128
#define	COMMANDWND_START_Y			128

#define DSMINIMAP_SIZE_X			159
#define DSMINIMAP_SIZE_Y			186
#define DSMINIMAP_START_X			867
#define DSMINIMAP_START_Y			0

#define GAMEMENUWND_SIZE_X			256
#define GAMEMENUWND_SIZE_Y			128
#define GAMEMENUWND_START_X			384
#define GAMEMENUWND_START_Y			384

#define ITEMSHOPWND_SIZE_X			256
#define ITEMSHOPWND_SIZE_Y			128
#define ITEMSHOPWND_START_X			384
#define ITEMSHOPWND_START_Y			128

#define ITEMWND_SIZE_X				256
#define ITEMWND_SIZE_Y				640
#define ITEMWND_START_X				128
#define ITEMWND_START_Y				128

#define	BLOCKWND_SIZE_X				256
#define	BLOCKWND_SIZE_Y				128
#define	BLOCKWND_START_X			256
#define	BLOCKWND_START_Y			256

#define SKILLWND_SIZE_X				320
#define SKILLWND_SIZE_Y				512
#define SKILLWND_START_X			640
#define SKILLWND_START_Y			128

#define	NPCDUNGEONINFOWND_SIZE_X	256
#define	NPCDUNGEONINFOWND_SIZE_Y	256
#define	NPCDUNGEONINFOWND_START_X	384
#define	NPCDUNGEONINFOWND_START_Y	128

#define	QUANTITYWND_SIZE_X			128
#define	QUANTITYWND_SIZE_Y			128
#define	QUANTITYWND_START_X			384
#define	QUANTITYWND_START_Y			128

#define	CHARMOVEWND_SIZE_X			256
#define	CHARMOVEWND_SIZE_Y			128
#define	CHARMOVEWND_START_X			256
#define	CHARMOVEWND_START_Y			256


#define	PARTYBOARDWND_SIZE_X		256
#define	PARTYBOARDWND_SIZE_Y		136
#define	PARTYBOARDWND_START_X		256
#define	PARTYBOARDWND_START_Y		256

#define	PARTYENTRYWND_SIZE_X		256
#define	PARTYENTRYWND_SIZE_Y		136
#define	PARTYENTRYWND_START_X		256
#define	PARTYENTRYWND_START_Y		256

#define	STOREWND_SIZE_X				256
#define	STOREWND_SIZE_Y				384
#define	STOREWND_START_X			128
#define	STOREWND_START_Y			128

#define	BANKWND_SIZE_X				256
#define	BANKWND_SIZE_Y				432
#define	BANKWND_START_X				384
#define	BANKWND_START_Y				128

#define	DUNGEONMANAGER_SIZE_X		256
#define	DUNGEONMANAGER_SIZE_Y		256
#define	DUNGEONMANAGER_START_X		384
#define	DUNGEONMANAGER_START_Y		128

#define	ITEMUPGRADE_SIZE_X			256
#define	ITEMUPGRADE_SIZE_Y			192
#define	ITEMUPGRADE_START_X			384
#define	ITEMUPGRADE_START_Y			128

#define	MATCHWND_SIZE_X				256
#define	MATCHWND_SIZE_Y				256
#define	MATCHWND_START_X			256
#define	MATCHWND_START_Y			256

#define	DUNGEONINFOWND_SIZE_X		416
#define	DUNGEONINFOWND_SIZE_Y		256
#define	DUNGEONINFOWND_START_X		384
#define	DUNGEONINFOWND_START_Y		128

#define	DUNGEONSIEGEINFOWND_SIZE_X		256
#define	DUNGEONSIEGEINFOWND_SIZE_Y		128
#define	DUNGEONSIEGEINFOWND_START_X		200
#define	DUNGEONSIEGEINFOWND_START_Y		0

#define	GUILDWND_SIZE_X				256
#define	GUILDWND_SIZE_Y				128
#define	GUILDWND_START_X			256
#define	GUILDWND_START_Y			256

#define	OKWND_SIZE_X				256
#define	OKWND_SIZE_Y				128
#define	OKWND_START_X				256
#define	OKWND_START_Y				256

#define	MESSENGERWND_SIZE_X			256
#define	MESSENGERWND_SIZE_Y			128
#define	MESSENGERWND_START_X		256
#define	MESSENGERWND_START_Y		256

#define	GUILDEXWND_SIZE_X			256
#define	GUILDEXWND_SIZE_Y			128
#define	GUILDEXWND_START_X			256
#define	GUILDEXWND_START_Y			256

#define	GUILDJOINWND_SIZE_X			256
#define	GUILDJOINWND_SIZE_Y			128
#define	GUILDJOINWND_START_X		256
#define	GUILDJOINWND_START_Y		256

#define	GUILDRANKWND_SIZE_X			256
#define	GUILDRANKWND_SIZE_Y			128
#define	GUILDRANKWND_START_X		256
#define	GUILDRANKWND_START_Y		256

#define	GUILDEDITWND_SIZE_X			256
#define	GUILDEDITWND_SIZE_Y			128
#define	GUILDEDITWND_START_X		256
#define	GUILDEDITWND_START_Y		256

#define	PARTYJOINWND_SIZE_X			256
#define	PARTYJOINWND_SIZE_Y			128
#define	PARTYJOINWND_START_X		256
#define	PARTYJOINWND_START_Y		256
		
#define	INITITEMWND_SIZE_X			256
#define	INITITEMWND_SIZE_Y			128
#define	INITITEMWND_START_X			256
#define	INITITEMWND_START_Y			256

#define	PARTYREQUESTWND_SIZE_X		256
#define	PARTYREQUESTWND_SIZE_Y		128
#define	PARTYREQUESTWND_START_X		256
#define	PARTYREQUESTWND_START_Y		256

#define DEFAULTWND_SIZE_X			256
#define DEFAULTWND_SIZE_Y			128
#define DEFAULTWND_START_X			256
#define DEFAULTWND_START_Y			256

#define	GAMEDEFAULTWND_SIZE_X		256
#define	GAMEDEFAULTWND_SIZE_Y		128
#define	GAMEDEFAULTWND_START_X		256
#define	GAMEDEFAULTWND_START_Y		256

#define	HELPWND_SIZE_X				256
#define	HELPWND_SIZE_Y				256
#define	HELPWND_START_X				700
#define	HELPWND_START_Y				300

#define	PARTYHELPWND_SIZE_X			480
#define	PARTYHELPWND_SIZE_Y			488
#define	PARTYHELPWND_START_X		300
#define	PARTYHELPWND_START_Y		300

#define	PARTYBOARDINFO_WND_SIZE_X	480
#define	PARTYBOARDINFO_WND_SIZE_Y	256
#define	PARTYBOARDINFO_WND_START_X	300
#define	PARTYBOARDINFO_WND_START_Y	300

#define	CHARMOVERTWND_SIZE_X		256
#define	CHARMOVERTWND_SIZE_Y		256
#define	CHARMOVERTWND_START_X		700
#define	CHARMOVERTWND_START_Y		300

#define	CHARMOVEEDITWND_SIZE_X		256
#define	CHARMOVEEDITWND_SIZE_Y		256
#define	CHARMOVEEDITWND_START_X		700
#define	CHARMOVEEDITWND_START_Y		300

#define	RANKNPCWND_SIZE_X			256
#define	RANKNPCWND_SIZE_Y			256
#define	RANKNPCWND_START_X			700
#define	RANKNPCWND_START_Y			300

#define	RANKWND_SIZE_X				256
#define	RANKWND_SIZE_Y				256
#define	RANKWND_START_X				700
#define	RANKWND_START_Y				300

#define	WORLDWND_SIZE_X				256
#define	WORLDWND_SIZE_Y				256
#define	WORLDWND_START_X			700
#define	WORLDWND_START_Y			300

#define	NOTICEWND_SIZE_X			256
#define	NOTICEWND_SIZE_Y			256
#define	NOTICEWND_START_X			200
#define	NOTICEWND_START_Y			200

#define	TOPDUNGEONHELPWND_SIZE_X			256
#define	TOPDUNGEONHELPWND_SIZE_Y			256
#define	TOPDUNGEONHELPWND_START_X			200
#define	TOPDUNGEONHELPWND_START_Y			200

#define	ERRORWND_SIZE_X				256
#define	ERRORWND_SIZE_Y				128
#define	ERRORWND_START_X			256
#define	ERRORWND_START_Y			256

#define	CHARDIEWND_SIZE_X			256
#define	CHARDIEWND_SIZE_Y			128
#define	CHARDIEWND_START_X			256
#define	CHARDIEWND_START_Y			256

#define	EVENTDUNGEONWND_SIZE_X		256
#define	EVENTDUNGEONWND_SIZE_Y		128
#define	EVENTDUNGEONWND_START_X		256
#define	EVENTDUNGEONWND_START_Y		256

#define	ITEMTRADEWND_SIZE_X			256
#define	ITEMTRADEWND_SIZE_Y			128
#define	ITEMTRADEWND_START_X		256
#define	ITEMTRADEWND_START_Y		256

#define	ITEMTRADESHOPWND_SIZE_X		512
#define	ITEMTRADESHOPWND_SIZE_Y		256
#define	ITEMTRADESHOPWND_START_X	256
#define	ITEMTRADESHOPWND_START_Y	256

#define	GROUPWND_SIZE_X				256
#define	GROUPWND_SIZE_Y				512
#define	GROUPWND_START_X			256
#define	GROUPWND_START_Y			256

#define	PLAYERSHOPWND_SIZE_X		256
#define	PLAYERSHOPWND_SIZE_Y		256
#define	PLAYERSHOPWND_START_X		256
#define	PLAYERSHOPWND_START_Y		256

#define	PARTYPORTALWND_SIZE_X		256
#define	PARTYPORTALWND_SIZE_Y		128
#define	PARTYPORTALWND_START_X		256
#define	PARTYPORTALWND_START_Y		256

#define	GUILDPORTALWND_SIZE_X		256
#define	GUILDPORTALWND_SIZE_Y		128
#define	GUILDPORTALWND_START_X		256
#define	GUILDPORTALWND_START_Y		256

#define	GUILDREQUESTWND_SIZE_X		256
#define	GUILDREQUESTWND_SIZE_Y		128
#define	GUILDREQUESTWND_START_X		256
#define	GUILDREQUESTWND_START_Y		256

#define	GUILDDELETEWND_SIZE_X		256
#define	GUILDDELETEWND_SIZE_Y		128
#define	GUILDDELETEWND_START_X		256
#define	GUILDDELETEWND_START_Y		256

#define	GUILDWARWND_SIZE_X			256
#define	GUILDWARWND_SIZE_Y			128
#define	GUILDWARWND_START_X			256
#define	GUILDWARWND_START_Y			256

#define	GUILDMASTERWND_SIZE_X		256
#define	GUILDMASTERWND_SIZE_Y		128
#define	GUILDMASTERWND_START_X		256
#define	GUILDMASTERWND_START_Y		256

#define	MESSENGERDELETEWND_SIZE_X	256
#define	MESSENGERDELETEWND_SIZE_Y	128
#define	MESSENGERDELETEWND_START_X	256
#define	MESSENGERDELETEWND_START_Y	256

#define GAMENOTICEWND_SIZE_X		256
#define GAMENOTICEWND_SIZE_Y		128
#define GAMENOTICEWND_START_X		900
#define GAMENOTICEWND_START_Y		0

#define EVENTNPCWND_SIZE_X			256
#define EVENTNPCWND_SIZE_Y			128
#define EVENTNPCWND_START_X			256
#define EVENTNPCWND_START_Y			256

#define EVENTNPCSUBWND_SIZE_X		256
#define EVENTNPCSUBWND_SIZE_Y		128
#define EVENTNPCSUBWND_START_X		256
#define EVENTNPCSUBWND_START_Y		256
*/

#define __MAP_SIZE_X__					144
#define __MAP_SIZE_Y__					144

// ����Ʈ �ɼ� ���� //
#define __EFFECT_OPTION__OFF__			0
#define __EFFECT_OPTION__LOW__			1
#define __EFFECT_OPTION__MIDIUM__		2
#define __EFFECT_OPTION__HIGH__			3
#define __EFFECT_OPTION__MAX__			4

#define GROUP_TAB_PARTY			0
#define GROUP_TAB_GUILD			1
#define GROUP_TAB_FRIEND		2
#define GROUP_TAB_GUILDWAR		3

#define GROUP_PARTY_DEFAULT			   0
#define GROUP_PARTY_CREATEADD		   1
#define GROUP_PARTY_CREATEADDCLK	   2
#define GROUP_PARTY_JOIN			   3
#define GROUP_PARTY_SELECT			   4
#define GROUP_GUILD_DEFAULT			   5
#define GROUP_GUILD_ADD				   6
#define GROUP_GUILD_SELECT			   7
#define GROUP_FRIEND_DEFAULT		   8
#define GROUP_FRIEND_ADD			   9
#define GROUP_FRIEND_SELECT			   10
#define GROUP_GUILDWAR_DEFAULT		   11
#define GROUP_GUILDWAR_ADD			   12
#define GROUP_GUILDWAR_SELECT		   13
//��Ƽ ��ȭ �ý��� (����â 2005.01.25)
#define GROUP_PARTY_PARTYUSERINFO1			   14 //������ ������ ���        
#define GROUP_PARTY_PARTYUSERINFO2			   15 //������ ���콺 up���� ��� 
#define GROUP_PARTY_EXPITEMCONDITION1		   16 //������ ������ ��� 
#define GROUP_PARTY_EXPITEMCONDITION2          17
#define GROUP_PARTY__CONDITIONLIST_EXP1		   18 //��ӹڽ� ������ DOWN Ŭ���� ����Ʈ ����.	
#define GROUP_PARTY__CONDITIONLIST_EXP2		   19 //ó�� ��ӹڽ� ������ DOWN->UP���� ��� ����Ʈ ���� ���·� �����ϰ� ��ӹڽ� �����ܸ� ����  
#define GROUP_PARTY__CONDITIONLIST_EXP3		   20 //�ٽ� ��ӹڽ� ������ DOWN Ŭ���� ����Ʈ ����. 
#define GROUP_PARTY__CONDITIONLIST_EXP4		   21 //��ӹڽ� �����ܿ� over���� ��� 
#define GROUP_PARTY__CONDITIONLIST_ITEM1	   22 //��ӹڽ� ������ DOWN Ŭ���� ����Ʈ ����.	
#define GROUP_PARTY__CONDITIONLIST_ITEM2	   23 //ó�� ��ӹڽ� ������ DOWN->UP���� ��� ����Ʈ ���� ���·� �����ϰ� ��ӹڽ� �����ܸ� ����  
#define GROUP_PARTY__CONDITIONLIST_ITEM3	   24 //�ٽ� ��ӹڽ� ������ DOWN Ŭ���� ����Ʈ ����. 
#define GROUP_PARTY__CONDITIONLIST_ITEM4	   25 //��ӹڽ� �����ܿ� over���� ��� 
#define GROUP_PARTY_PMATCHCONDITION1		   26 //������ ������ ��� 	 
#define GROUP_PARTY_PMATCHCONDITION2		   27
#define GROUP_PARTY_PMATCHCONDITION_LEV1	   28 //��ӹڽ� ������ DOWN Ŭ���� ����Ʈ ����.	
#define GROUP_PARTY_PMATCHCONDITION_LEV2	   29 //ó�� ��ӹڽ� ������ DOWN->UP���� ��� ����Ʈ ���� ���·� �����ϰ� ��ӹڽ� �����ܸ� ����  
#define GROUP_PARTY_PMATCHCONDITION_LEV3	   30 //�ٽ� ��ӹڽ� ������ DOWN Ŭ���� ����Ʈ ����. 
#define GROUP_PARTY_PMATCHCONDITION_LEV4	   31 //��ӹڽ� �����ܿ� over���� ��� 
#define GROUP_PARTY_PMATCHCONDITION_EXP1	   32 //��ӹڽ� ������ DOWN Ŭ���� ����Ʈ ����.	
#define GROUP_PARTY_PMATCHCONDITION_EXP2	   33 //ó�� ��ӹڽ� ������ DOWN->UP���� ��� ����Ʈ ���� ���·� �����ϰ� ��ӹڽ� �����ܸ� ����  
#define GROUP_PARTY_PMATCHCONDITION_EXP3	   34 //�ٽ� ��ӹڽ� ������ DOWN Ŭ���� ����Ʈ ����. 
#define GROUP_PARTY_PMATCHCONDITION_EXP4	   35 //��ӹڽ� �����ܿ� over���� ��� 
#define GROUP_PARTY_PMATCHCONDITION_ITEM1	   36 //��ӹڽ� ������ DOWN Ŭ���� ����Ʈ ����.	
#define GROUP_PARTY_PMATCHCONDITION_ITEM2	   37 //ó�� ��ӹڽ� ������ DOWN->UP���� ��� ����Ʈ ���� ���·� �����ϰ� ��ӹڽ� �����ܸ� ����  
#define GROUP_PARTY_PMATCHCONDITION_ITEM3	   38 //�ٽ� ��ӹڽ� ������ DOWN Ŭ���� ����Ʈ ����. 
#define GROUP_PARTY_PMATCHCONDITION_ITEM4	   39 //��ӹڽ� �����ܿ� over���� ��� 
#define GROUP_PARTY_PMATCHCONDITION_PMAINCHO1  40 //��ӹڽ� ������ DOWN Ŭ���� ����Ʈ ����.	
#define GROUP_PARTY_PMATCHCONDITION_PMAINCHO2  41 //ó�� ��ӹڽ� ������ DOWN->UP���� ��� ����Ʈ ���� ���·� �����ϰ� ��ӹڽ� �����ܸ� ����  
#define GROUP_PARTY_PMATCHCONDITION_PMAINCHO3  42 //�ٽ� ��ӹڽ� ������ DOWN Ŭ���� ����Ʈ ����. 
#define GROUP_PARTY_PMATCHCONDITION_PMAINCHO4  43 //��ӹڽ� �����ܿ� over���� ��� 
#define GROUP_PARTY__CONDITIONLIST_EXP5        44 //GROUP_PARTY_EXPITEMCONDITION1 ��ư�� ��ӹڽ� ����Ʈ ���ý� ����Ʈ ������ ������ ���� 
#define GROUP_PARTY__CONDITIONLIST_ITEM5	   45 //GROUP_PARTY_EXPITEMCONDITION1 ��ư�� ��ӹڽ� ����Ʈ ���ý� ����Ʈ ������ ������ ���� 
#define GROUP_PARTY_PMATCHCONDITION_LEV5	   46 //GROUP_PARTY_PMATCHCONDITION1  ��ư�� ��ӹڽ� ����Ʈ ���ý� ����Ʈ ������ ������ ����
#define GROUP_PARTY_PMATCHCONDITION_EXP5       47 //GROUP_PARTY_PMATCHCONDITION1  ��ư�� ��ӹڽ� ����Ʈ ���ý� ����Ʈ ������ ������ ����
#define GROUP_PARTY_PMATCHCONDITION_ITEM5      48 //GROUP_PARTY_PMATCHCONDITION1  ��ư�� ��ӹڽ� ����Ʈ ���ý� ����Ʈ ������ ������ ����
#define GROUP_PARTY_PMATCHCONDITION_PMAINCHO5  49 //GROUP_PARTY_PMATCHCONDITION1  ��ư�� ��ӹڽ� ����Ʈ ���ý� ����Ʈ ������ ������ ����
#define GROUP_PARTY__EXPITEMCONDITI_OK		   50 //EXPITEMCONDITI Ȯ�� ��ư
#define GROUP_PARTY__EXPITEMCONDITI_CANCEL     51 //EXPITEMCONDITI ��� ��ư 
//#define GROUP_PARTY__EXPITEMCONDITI_OK_UP	   52 //EXPITEMCONDITI Ȯ�� ��ư UP ���� ���
#define GROUP_PARTY_PMATCHCONDITION_OK_ON      53 //��Ī ���� �� Ȯ�� ������ ��� Ȱ��ȭ 
#define GROUP_PARTY_PMATCHCONDITION_OK_OFF     54 //��Ī ���� �� Ȯ�� ���� �Ŀ� ��� �� ��� �� Ȱ��ȭ  
#define GROUP_PARTY_FRIEND_SELECT			   55 //��Ƽ��Ȳ (����â 2005.03.03) ��Ƽ��Ȳ ����Ʈ���� �¶��� �������� �Ӹ������� ���� ��ư Ȱ��ȭ 	

// sung-han 2005-01-29 ���̹� ����� ���� ������
#define GROUP_GUILDWAR_RANK					53
#define GROUP_GUILDWAR_LIST					54
#define GROUP_GUILDWAR_MATCH				55



#define GROUP_RENDER_NONE				0
#define GROUP_RENDER_PARTYCREATE		1
#define GROUP_RENDER_PARTYCREATEADD		2
#define GROUP_RENDER_PARTYUSERADD		3
#define GROUP_RENDER_PARTYUSERADDCLK	4
#define GROUP_RENDER_PARTYUSERLOGOUT	5
#define GROUP_RENDER_PARTYUSERINFO		6
#define GROUP_RENDER_PARTYUSERDEL		7
#define GROUP_RENDER_PARTYMSG			8
//��Ƽ ��ȭ �ý��� (����â 2005.01.25)
#define GROUP_RENDER_PARTYUSERINFO_LIST		27  
#define GROUP_RENDER_EXPITEMCONDITION_MSG	28
#define GROUP_RENDER_PMATCHCONDITION_MSG	29
//��Ƽ ��ȭ �ý��� (����â 2005.02.07)
#define GROUP_RENDER_PMATCHBUTTON_ON_MSG	30   //��ư�� Ŭ��(��Ȱ��ȭ)�ϸ� ��Ƽ ��Ī�� �����մϴ�.
#define GROUP_RENDER_PMATCHBUTTON_OFF_MSG	31   //��ư�� Ŭ��(Ȱ��ȭ)�ϸ� ��Ƽ ��Ī�� �����մϴ�.
#define GROUP_RENDER_PMATCHSTART_MSG		32   //��Ƽ ��Ī�� ���� �Ǿ����ϴ�.
#define GROUP_RENDER_PMATCHEND_MSG			33   //��Ƽ ��Ī�� ���� �Ǿ����ϴ�.




#define GROUP_RENDER_GUILDUSERADD		9
#define GROUP_RENDER_GUILDUSERADDCLK	10
#define GROUP_RENDER_GUILDUSERONOFF		11
#define GROUP_RENDER_GUILDDELETE		12
#define GROUP_RENDER_GUILDMSG			13
#define GROUP_RENDER_GUILDUSERINFO		14
#define GROUP_RENDER_GUILDUSERDELETE	15
#define GROUP_RENDER_GUILDUSERRANK		16
#define GROUP_RENDER_GUILDUSERMASTER	17

#define GROUP_RENDER_FRIENDUSERADD		17
#define GROUP_RENDER_FRIENDUSERADDCLK	18
#define GROUP_RENDER_FRIENDUSERONOFF	19
#define GROUP_RENDER_FRIENDUSERMSG		20
#define GROUP_RENDER_FRIENDSERINFO		21
#define GROUP_RENDER_FRIENDUSERDELETE	22

#define GROUP_RENDER_GUILDWARUSERADD	23
#define GROUP_RENDER_GUILDWARUSERADDCLK	24

#define GROUP_RENDER_PARTYBOARDCREATE	25
#define GROUP_RENDER_PARTYBOARDDESTROY	26

#define MAX_GUILDWAR					8

// by deepdark.
#if IS_JAPAN_LOCALIZING()
#	define GUILDWAR_TIME				24
#else
#	define GUILDWAR_TIME				72
#endif

#define GUILD_USER_ONLINE				1
#define GUILD_USER_OFFLINE				2

#define FRIEND_USER_ONLINE				1
#define FRIEND_USER_OFFLINE				2

// ���� ���� //
#define	__ORDER_USERINTERFACE_START_	0
#define	__ORDER_NOTICE_START__			100			
#define	__ORDER_USER_BLET__				400
#define	__ORDER_USERCHAT_START__		500					
#define	__ORDER_CHAT_START__			1000
#define	__ORDER_INTERFACE_START__		2000
#define	__ORDER_MOUSE_ITEM__			3000
#define __ORDER_CLICK_INTERFACE__		4000
#define	__ORDER_MOUSE_POINTER__			5000
#define	__ORDER_ITEM_DESC__				6000
#define	__ORDER_LOAD_INTERFACE__		10000

#define	__ORDER_SKILL_KEY__				5

#define __MENU_TYPE_NONE__				0
#define __MENU_TYPE_CHARACTER__			1
#define __MENU_TYPE_WORLD__				2
#define __MENU_TYPE_DEFAULT__			3

#define __MAX_INTERFACE_MESSAGE			1024		//	modified by minjin with y2jinc. 2004. 11. 17.
// NPC ���� //
#define	__NPC_VILL_SUPPLIES__			1
#define	__NPC_VILL_WEAPON__				2
#define	__NPC_VILL_ARMOR__				3
#define	__NPC_VILL_BANK__				4
#define	__NPC_VILL_SIN__				5
#define	__NPC_VILL_SU__					6
#define __NPC_VILL_KAI__				7
#define __NPC_DUNGEON_TALK__			8
#define __NPC_VILL_GUILD__				9
#define __NPC_VILL_EVENT__				10
#define __NPC_PARTY_BOARD__				11
#define __NPC_WORLD_MOVE1__				12
#define __NPC_WORLD_MOVE2__				17

#define	__MAX_BANK_COUNT__				64

#define	__MAX_BANKLINE_WIDTH__			10
#define	__MAX_BANKLINE_HEIGHT__			7

#define	__QUANTITY_MONEY__					1
#define	__QUANTITY_THROW__					2
#define	__QUANTITY_STORE__					3
#define	__QUANTITY_BUY__					4
#define	__QUANTITY_BANKINSERT__				5
#define __QUANTITY_DUNGEONMANAGEINSERT__	6
#define	__QUANTITY_BNAKMONEYINSERT__		7
#define	__QUANTITY_BNAKMONEYPOP__			8
#define __QUANTITY_TRADE__					9
#define __QUANTITY_TRADEMONEY__				10
#define __QUANTITY_PLAYERSHOP__				11
#define __QUANTITY_PLAYERSHOPCNT__			12
#define __QUANTITY_PLAYERSHOPSELLCNT__		13
#define __QUANTITY_DUNGEON__				14
#define __QUANTITY_JUMIN_NUMBER__			15
#define __QUANTITY_FROMBANKTOINV__			16


#define	__MAX_MOUSE_POINTER__			19


// ���콺 ������ ���� //
#define	__MOUSE_POINTER_DEFAULT__		1
#define	__MOUSE_POINTER_DEFAULTCLICK__	2
#define	__MOUSE_POINTER_BUTTON__		4
#define	__MOUSE_POINTER_BUTTONCLICK__	5
#define	__MOUSE_POINTER_ITEM__			6
#define	__MOUSE_POINTER_ITEMCLICK__		7
#define	__MOUSE_POINTER_ATTACK__		9
#define	__MOUSE_POINTER_ATTACKCLICK__	10
#define	__MOUSE_POINTER_NPC__			12
#define	__MOUSE_POINTER_NPCCLICK__		13
#define	__MOUSE_POINTER_GUARDIAN__		17
#define	__MOUSE_POINTER_GUARDIANCLICK__	18

#define	__MAX_BANK_SC__					4
#define	__MAX_SKILL_UP__				25
#define	__MAX_MASTERYSKILL_UP__			255

#define __GCTYPE_NONE__					0
#define __GCTYPE_GUILD__				1
#define __GCTYPE_CLEN__					2
#define MAX_GUILD_FILENAME_LENGTH		50

//#define	MAX_GUILD_USER					200

#define	__MAX_MONEY__					4200000000

#define	__PK_NONEMODE__					0
#define	__PK_MODE__						1

#define	__GUILD_NONE__					0
#define	__GUILD_MASTER__				1
#define	__GUILD_MASTER2__				2
#define	__GUILD_USER2__					4

#define	__ITEM_RESOURCETYPE_MOD__		0
#define	__ITEM_RESOURCETYPE_CHR__		1
#define __ITEM_RESOURCETYPE_PRE__		2

#define	__MAX_CLICKINTERFAE_DISTANCE__	400

#define	__HAND_INDEX_ONE__				1
#define	__HAND_INDEX_TWO__				2

#define	__MAX_ITEM_ICON__				279

#define	__ITEM_LARGE_START__			0
#define	__ITEM_SMALL_START__			21
#define	__ITEM_EQUIP_START__			63
#define	__ITEM_BELT_START__				84
#define	__ITEM_MOUSE_START__			92
#define	__ITEM_BANKLARGE_START__		93
#define	__ITEM_BANKSMALL_START__		156
#define	__ITEM_GUARDIAN_START__			226
#define __ITEM_TRADE_START__			230
#define __ITEM_PLAYERSHOP_START__		258

#define	__MAPID_DUNGEON_START__			2001



#define	__MAX_VIEW_GUILD_USER__			8
#define	__MAX_GUILD_USER__				100

#define	__DG_IB_RANK1__					8
#define	__DG_IB_RANK2__					9
#define	__DG_IB_RANK3__					10
#define	__DG_IB_RANK4__					11
#define	__DG_IB_NICKNAME__				12

#define	__MAX_RANK_SIZE__				8
#define	__MAX_GUILD_USER__				100
#define	__MAX_PARTY_UER__				8

#define	__GUILDINFO_MSG_NICKNAME__		1
#define	__GUILDINFO_MSG_RANK__			2
#define	__GUILDINFO_MSG_NICKNAMERANK__	3


#define	MAX_PARTY						10
#define	MAX_PARTY_USER					8

#define	STORE_TYPE_WEAPON				1
#define	STORE_TYPE_ARMOR				2
#define	STORE_TYPE_SUSPZO				3
#define STORE_TYPE_BAG					4

#define	STORE_MAX_ITEM_COUNT			56
#define	STORE_MAX_ITEM_COUNT2			28

#define	TEXT_RENDER_BUFFER				128

#define	__STORE_PAGE_PREV__				1
#define	__STORE_PAGE_NEXT__				2

#define	__GUILD_PAGE_CURR__				0
#define	__GUILD_PAGE_PREV__				1
#define	__GUILD_PAGE_NEXT__				2

#define	__FRIEND_PAGE_CURR__			0
#define	__FRIEND_PAGE_PREV__			1
#define	__FRIEND_PAGE_NEXT__			2

#define __GUILD_MAX_USER_DISPLAY__		8

#define __FRIEND_MAX_USER_DISPLAY__		8

#define	__MAX_SKILLINFO_VAL__			6

#define	MAX_SKILLWND_DPCOUNT			10

#define	MAX_NOTICE_SIZE					40
#define	MAX_NOTICE_COUNT				10
#define	MAX_NOTICE_TIME					16000

#define	MAX_FONT_WIDTH					8
#define	MAX_FONT_HEIGHT					14
//-------------------------------------------------------------------------------------------//
// Status
#define MAX_STATUS						5

#define MAX_KEY							36


//-------------------------------------------------------------------------------------------//
#define		MAX_USER_GUARDIAN	5
#define	MAX_PLAYER_SHOP_INV			10

#if IS_JAPAN_LOCALIZING()
#define	MAX_PLAYER_SHOP_TITLE		30
#elif IS_CHINA_LOCALIZING()
#define	MAX_PLAYER_SHOP_TITLE		28
#else
#define	MAX_PLAYER_SHOP_TITLE		40
#endif

#define	MAX_REAL_PLAYER_SHOP_TITLE	30

#define	MAX_PLAYER_SHOP_DESCRIPTION	50



//////////////////////////////////////////////////////////////////////////
// Dungeon ����
// by seven
#define TOP_OF_THE_DUNGEON_LAYER		0



// emoticon ���� 
#define EMOTICON_1			1
#define EMOTICON_2			2
#define EMOTICON_3			3
#define EMOTICON_4			4
#define EMOTICON_5			5
#define EMOTICON_6			6
#define EMOTICON_7			7
#define EMOTICON_8			8
#define EMOTICON_9			9
#define EMOTICON_10			10
#define EMOTICON_11			11
#define EMOTICON_12			12
#define EMOTICON_13			13




#define _SEVEN_MEMORY_CHECK

#ifdef DEBUG
#	ifdef _SEVEN_MEMORY_CHECK
#		define _CHECK_MEMORY()	if(_CrtCheckMemory()==NULL) { asm_int3() }
#	else
#		define _CHECK_MEMORY()
#	endif
#else
#	define _CHECK_MEMORY()	
#endif




