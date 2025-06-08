#pragma once

#include "../CommonServer/CommonClientDungeon.h"
#include "../CommonServer/CommonGlobalDefine.h"
#include "../CommonServer/ItemManagerDefine.h"

#define OBJECT_CRASH_CHECK
//#define SOUND_NEW
//#define SYNC_FRAME


#define	MAX_CP_TABLE				31
#define MAX_PLAYER_PER_ROOM				8


#define MAX_ROOM_USER				8
#define MAX_ROOM_PASSWORD_LENGTH	10
#define MAX_REOM_NAME_LENGTH		20
#define MAX_ROPLY_ROOM_NUM			100			//방정보 요구할때..  응답할 방갯수 

#define PLAYER_EMPTY				245		
#define MAX_INTERFACE_SPRITE_NUM	5	


#define MAX_CHAT_INPUT_LENGTH		128			//한번에 입력할 수 있는 최대 채팅 문자 길이 
#define MAX_DISPLAY_CHAT_LINE		5			//이전 몇 줄까지 보여줄것인가...
#define MAX_CHAT_BUFFER				640			//(128*5)
#define CHAT_LINE_SPACE				10

#define MAX_MAP_NAME_LENGTH			16			//최대 맵이름 문자열 길이 
#define MAX_INVENTROY_ITEM			20			//인벤토리의 아이템 보관 갯수 

#define MAX_CHAR_KIND				5			//최대 케릭터 종류.
#define MAX_MONSTER_KIND			15000		//최대 몬스터 종류.
#define MAX_EFFECT_KIND				255			//최대 이펙트 종류.


//타일 속성의 uOccupied 타입 
#define TILE_EMPTY					0
#define TILE_OCCUPIED_PC			1
#define TILE_OCCUPIED_NPC			2
#define TILE_OCCUPIED_MONSTER		3
#define TILE_OCCUPIED_DONT_MOVE		7

// 아이템 업그레이드 배열 인덱스
//#define	ITEM_UPGRADE_WEAPON		0		
#define		ITEM_UPGRADE_ZODIAC		1
#define		ITEM_UPGRADE_JEWEL		2
#define		ITEM_UPGRDAE_ELIXIR		3


#define PC_RALLY_POINT_X			1250
#define PC_RALLY_POINT_Y			0
#define PC_RALLY_POINT_Z			3000

#define FRAME_FLAG_PLAYING			0		//프레임이 계속 진행중인 상황
#define FRAME_FLAG_FINISHED			1		//프레임 끝
#define FRAME_FLAG_FINISHED_HIDE	3		//프레임 끝
#define FRAME_FLAG_CHANGE_NEXT		2		//다음 Action으로 변하고 있는중 




#define MAX_MONSTER_NUM_PER_ROOM	100

#define MONSTER_TYPE_BALROK			10
#define MONSTER_TYPE_SKELETON		20
#define MONSTER_TYPE_CRAB			30


#define MAX_MONSTER_DEMAGE_PARTICLE_NUM		10
#define MAX_MONSTER_BLOOD_NUM				4
#define MAX_FIREBALL_EFFECT					20


#define PC_RUNNING_SPEED			0.7		//(Cm / milisec)
#define PC_WALKING_SPEED			0.1		//(Cm / milisec)

#define NPC_BALROK_SPEED			0.2
#define NPC_SKELETON_SPEED			0.1
#define NPC_CRAB_SPEED				0.4

#define NETWORK_GAP					10		//(ms)


#define ATTACK_TYPE_NORMAL			1
#define ATTACK_TYPE_FIRE_SWORD		2


#define	MAGIC_TYPE_FIREBALL			1


#define MAX_AUDIO_OBJECT_NUM		128


#define MAX_CASTING_EFFECT		5
#define MAX_CHARGE_HEIGHT		5
#define MAX_CHARGE_EFFECT		4	

#define MATCH_DUNGEON_LAYER_TOP			1		// 대전 던전 레이어 최상층 
#define MATCH_DUNGEON_LAYER_BOTTOM		20		// 대전 던전 레이어 최하층 



#define DATA_TYPE_MAP			1
#define DATA_TYPE_CHARACTER		2
#define DATA_TYPE_WEAPON		3
#define DATA_TYPE_MONSTER		4
#define DATA_TYPE_EFFECT		5
#define DATA_TYPE_UI			6
#define DATA_TYPE_SOUND			7
#define DATA_TYPE_MANAGER		8
#define DATA_TYPE_DEBUG			9
#define DATA_TYPE_BUTTON		10
#define DATA_TYPE_NPC			11
#define	DATA_TYPE_ITEM			12
#define DATA_TYPE_DAMAGENUMBER	13
#define DATA_TYPE_CDT			14

#define ITEM_KARZ_ID			25000

/// Camera 관련 상수 /////////////////////////////////////////////////////////

// 던젼과 월드에서의 카메라 시야 범위 값들임..

#define DUNGEON_CAMERA_NEAR		100.0f
#define DUNGEON_CAMERA_FAR		8000.0f

#define WORLD_CAMERA_NEAR		100.0f
#define WORLD_CAMERA_FAR		80000.0f

#ifdef DEVELOP_MODE
#define asm_int3()	 {__asm int 3 }
#else
#define asm_int3()	{;}
#endif
/*
enum USER_BILLING_FLAG
{
	USER_BILLING_FLAT_RATE				= 2,		//개인 정액
	USER_BILLING_METER_RATE				= 4,		//개인 종량 
	USER_BILLING_1HOUR_FREE				= 9,		//결제 유저 아님 
	USER_BILLING_ABSOLUTELY_FREE		= 10,		//무조건 통과 
	USER_BILLING_COUPON					= 40,		//개인 쿠폰 유저 
};
*/

/*
//PC방 과금 상태 
enum PCROOM_BILLING_FLAG
{
	PCROOM_BILLING_FLAT_RATE			= 1,		//PC방 정액
	PCROOM_BILLING_METER_RATE			= 3,		//PC방 종량 

	PCROOM_BILLING_SOMEHOUR_FREE		= 11,		//대만용 PC 방 빌링안하고 FREE TIME 적용 

	
};
*/
/*
//유저의 과금 상태 
enum USER_BILLING_FLAG
{
	USER_BILLING_COUPON					= 40,		//개인 쿠폰 유저 
	USER_BILLING_METER_RATE				= 60,		//개인 종량 
	USER_BILLING_1HOUR_FREE				= 80,		//결제 유저 아님 
	USER_BILLING_FLAT_RATE				= 100,		//개인 정액
//	USER_BILLING_FIRST_14DAY_FREE		= 120,		//신규가입 14일 무료 
	USER_BILLING_ABSOLUTELY_FREE		= 0xff
};

//PC방 과금 상태 
enum PCROOM_BILLING_FLAG
{
	PCROOM_BILLING_METER_RATE			= 90,		//PC방 종량 
	PCROOM_BILLING_FLAT_RATE			= 110,		//PC방 정액
};
*/


//----------------------------------------------------------------------------------------//
// Player의 무기별 모션 시작 번호 Define
#define MOTION_TYPE_NONE			0
#define MOTIOM_TYPE_SWORD			50
#define MOTION_TYPE_AXE				100
#define MOTION_TYPE_CUTER			150
#define MOTION_TYPE_TWOHANDSWORD	200
#define MOTION_TYPE_TWOHANDAXE		250
#define MOTION_TYPE_SPEAR			300
#define MOTION_TYPE_WAND			350
#define MOTION_TYPE_STAFF			400
//---------------------------------------------------------------------------------------//
/*
//---------------------------------------------------------------------------------------//
// 모션별 시작번호 Define
#define MOTION_TYPE_VILLAGESTAND	1
#define MOTION_TYPE_DUNGEONSTAND	2
#define MOTION_TYPE_WARSTAND		3
#define MOTION_TYPE_STAND1			4
#define MOTION_TYPE_STAND2			5
#define MOTION_TYPE_VILLAGEWALK		6
#define MOTION_TYPE_WALK			7
#define MOTION_TYPE_RUN				8
#define MOTION_TYPE_RUNSTOP			9
#define MOTION_TYPE_ATTACK1			10
#define MOTION_TYPE_ATTACK2			11
#define MOTION_TYPE_CASTINGSKILL	12
#define MOTION_TYPE_WARSTANDSKILL	13
#define MOTION_TYPE_BASH			14
///////// 15 - 25 NOT YET! ///////////
#define MOTION_TYPE_WARSTANDSTAND	26
#define MOTION_TYPE_DEFENSE			27
#define MOTION_TYPE_OFENSEFAIL1		28
#define MOTION_TYPE_OFENSEFAIL2		29
#define MOTION_TYPE_DEFENSEFAIL		30
#define MOTION_TYPE_STANDWARSTAND	31
#define MOTION_TYPE_STANDCASTING	32
#define MOTION_TYPE_WARSTANDCASTING	33
#define MOTION_TYPE_CHARGE			34
#define MOTION_TYPE_CHARGEWARSTAND	35
#define MOTION_TYPE_OVERDRIVE		36
#define MOTION_TYPE_DOWN			37
#define MOTION_TYPE_DOWNING			38
///////// 39 - 50 NOT YET! ///////////
*/
// 모션별 시작번호 Define
#define MOTION_TYPE_VILLAGESTAND	1
#define MOTION_TYPE_DUNGEONSTAND	2
#define MOTION_TYPE_WARSTAND		3
#define MOTION_TYPE_STAND1			4
#define MOTION_TYPE_STAND2			5
#define MOTION_TYPE_VILLAGEWALK		6
#define MOTION_TYPE_WALK			7
#define MOTION_TYPE_RUN				8
#define MOTION_TYPE_ATTACK1_1		9
#define MOTION_TYPE_ATTACK1_2		10
#define MOTION_TYPE_ATTACK1_3		11
#define MOTION_TYPE_ATTACK2_1		12
#define MOTION_TYPE_ATTACK2_2		13
#define MOTION_TYPE_ATTACK2_3		14
#define MOTION_TYPE_ATTACK3_1		15
#define MOTION_TYPE_ATTACK3_2		16
#define MOTION_TYPE_ATTACK3_3		17
#define MOTION_TYPE_SKILL			18
#define MOTION_TYPE_CRITICAL		19
#define MOTION_TYPE_BASH			20
#define MOTION_TYPE_BLASTQUAKE		21
#define MOTION_TYPE_RAGINGSWORD		22
#define MOTION_TYPE_LIGHTNINGBREAK	23
#define MOTION_TYPE_CHAINATTACKER	24
///////// 15 - 25 NOT YET! ///////////
#define MOTION_TYPE_WARSTANDSTAND	28
#define MOTION_TYPE_DEFENSE			29
#define MOTION_TYPE_DEFENSEFAIL		30
#define MOTION_TYPE_OFENSEFAIL2		29
#define MOTION_TYPE_DEFENSEFAIL		30
#define MOTION_TYPE_CHARGE			31
#define MOTION_TYPE_OVERDRIVE		32
#define MOTION_TYPE_DYING			33
#define MOTION_TYPE_ITEMPICKUP		34

#define MOTION_TYPE_PORTAL_DISAPPEAR 37
#define MOTION_TYPE_PORTAL_APPEAR	 38

///////// 39 - 50 NOT YET! ///////////
//---------------------------------------------------------------------------------------//
// Monster 모션 시작 번호.
#define MON_MOTION_TYPE_STAND1			1
#define MON_MOTION_TYPE_STAND2			2
#define MON_MOTION_TYPE_MOVE1			3
#define MON_MOTION_TYPE_MOVE2			4
#define MON_MOTION_TYPE_ATTACK1			5
#define MON_MOTION_TYPE_ATTACK2			6
#define MON_MOTION_TYPE_ATTACK3			7
#define MON_MOTION_TYPE_ATTACK4			8
#define MON_MOTION_TYPE_DEFENSE1		9
#define MON_MOTION_TYPE_OFENSEFAIL1		10
#define MON_MOTION_TYPE_OFENSEFAIL2		11
#define MON_MOTION_TYPE_DEFENSEFAIL1	12
#define MON_MOTION_TYPE_DEFENSEFAIL2	13
#define MON_MOTION_TYPE_DEFENSEFAIL3	14
#define MON_MOTION_TYPE_DOWN			15

//=============================================================
// DefineZ

#define		CRIMINAL_LEVEL_1	-18000		
#define		CRIMINAL_LEVEL_2	-27000		
#define		CRIMINAL_LEVEL_3	-32767

#define	MAX_TRADE_POOL					14
#define MAX_UPGRADE_POOL				4
#define MAX_MAKING_POOL					4
#define	MAX_BANK_LARGE_POOL				63
#define	MAX_BANK_SMALL_POOL				70
#define MAX_INV_GUARDIAN_POOL			5
#define MAX_PLAYERSHOP_POOL				10


enum
{
	EQUIP			= 0,
	INV_LARGE		= 1,
	INV_SMALL		= 2,
	INV_GUARDIAN	= 3,
	BELT			= 4,
	BANK_LARGE		= 5,
	BANK_SMALL		= 6,
	NPC_STORE		= 7,
	PLAYERSHOP		= 8,
	TREADE			= 9,
	UPGRADE			= 10,
	MAKING			= 11,
	TILE			= 12
};




