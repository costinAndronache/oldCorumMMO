#pragma once

#include <windows.h>
#include <windowsx.h>

#include "CommonAllServers.h"
#include "CommonServerExports.h"
#include "DungeonPacket.h"
#include <functional>

inline DWORD secondsToMilliseconds(float seconds) { return seconds * 1000; }
inline float millisecondsToSeconds(DWORD milliseconds) { return float(milliseconds) / 1000.f; }

struct CPTable_Value
{
	WORD	wID;
	WORD	wValue;
};

struct CPTable
{
	WORD	wID;
	char	szKorName[50];
	char	szEngName[35];
	char	szDescription[128];
	BYTE	bClass;
	WORD	wRate;
	WORD	wApply_Time;
	BYTE	bPartyUse;
	CPTable_Value	sCPTable_Value[5];
};


struct DUNGEONPRODUCTIONITEMMINMAX
{
	BYTE	byID;
	WORD	wItemIDMin;
	WORD	wItemIDMax;
	WORD	wItemIDDefault;
};

enum SELECT_ATTACK_TYPE {
	SELECT_ATTACK_TYPE_LBUTTON = 0,
	SELECT_ATTACK_TYPE_RBUTTON = 1,
};

enum UNIT_STATUS
{
	UNIT_STATUS_NONE = 0,
	UNIT_STATUS_NORMAL = 10,
	UNIT_STATUS_WALKING = 11,
	UNIT_STATUS_RUNNING = 12,
	UNIT_STATUS_ATTACKING = 13,
	UNIT_STATUS_DAMAGING = 14,
	UNIT_STATUS_SKILL = 15,
	UNIT_STATUS_CASTING = 16,
	UNIT_STATUS_WAITING = 17,
	UNIT_STATUS_DEAD = 18,
	UNIT_STATUS_READY = 19,
	UNIT_STATUS_PORTAL_MOVING = 20,
	UNIT_STATUS_PLAYER_SHOP = 21,
	UNIT_STATUS_PLAYER_REST = 22,
	UNIT_STATUS_DELAYTIME = 23,
	UNIT_STATUS_WORLD = 128,
	UNIT_STATUS_RIDE_AIRPLANE = 130,
	UNIT_STATUS_RIDE_AIRPLANE_DOWN = 131,
	UNIT_STATUS_AIRPLANE_DOWNING = 134,
	UNIT_STATUS_AIRPLANE_FLOATING = 135,
	UNIT_STATUS_AWAIT_DUNGEON_RESPONSE = 136,	//던전에 조인하기 위해 던전(월드)서버의 응답을 기다리는중 
};

enum STATUS_POINT_KIND {
	STATUS_POINT_KIND_EGO = 0,
	STATUS_POINT_KIND_STR = 1,
	STATUS_POINT_KIND_INT = 2,
	STATUS_POINT_KIND_DEX = 3,
	STATUS_POINT_KIND_VIT = 4,
	STATUS_POINT_KIND_MAX
};

enum ATTACK_MODE {
	ATTACK_MODE_NONE = 0,
	ATTACK_MODE_OFFENSE = 1,		// 공격자
	ATTACK_MODE_DEFENSE = 2,		// 방어자
};

#define		WEIGTH_100PER_OVER					101		// 소지품 무게가 100 퍼 이상인경우 
#define		WEIGTH_80PER_OVER					81		// 소지품 무게가 80 퍼 이상인경우 

enum ENUM_SKILL_CASTING_FAIL_REASON
{
	SKILL_CASTING_FAIL_REASON_LOW = 0,
	SKILL_CASTING_FAIL_REASON_GET_SLAPPED = 1,	// 캐스팅중 얻어 맞았다.
	SKILL_CASTING_FAIL_REASON_LACK_SP = 2,	// SP 부족
	SKILL_CASTING_FAIL_REASON_LACK_ZODIAC_ITEM = 3,	// 황소 카드 부족
	SKILL_CASTING_FAIL_NONE_REASEON = 4,	// 실패하긴 했지만 이유가 없다.
	SKILL_CASTING_FAIL_REASON_OVER_RANGE = 5,	// 사정거리를 넘겼군.
};

enum USER_TYPE
{
	USER_TYPE_NORMAL = 0,
	USER_TYPE_GM,
	USER_TYPE_PC_ROOM,
};

enum PORTAL_TYPE
{
	Portal_Type_General,		// 일반 아템 사용 
	Portal_Type_NPC,			// NPC 에 의해  
	Portal_Type_GM,				// GM 에 의해 
};

enum MATCH_STATUS
{
	MATCH_STATUS_USER_REQUEST_AFTER_USER_GOTO_WORLD = 10,	// 유저가 대결 신청하고 월드로 나가버렸네 쓰글..
	MATCH_STATUS_USER_REQUEST_AFTER_EXIT_GAME = 11,	// 유저가 대결 신청하고 게임 종료 했네 둑일넘..
};

#define ITEM_SKILL_BOOK_START	20519
#define ITEM_SKILL_BOOK_END		20578

#define BAG_ITEM_NUM_FRIST	3600
#define BAG_ITEM_NUM_LAST	3700

#define		SIEGEINFOWND_TYPE_SIEGE				0
#define		SIEGEINFOWND_TYPE_EVENT_TRAINING	1
#define		SIEGEINFOWND_TYPE_EVENT_SURVIVAL	2

#define		MONSTER_MAXHP			1
#define		MONSTER_MAXMP			2
#define		MONSTER_HP				3
#define		MONSTER_MP				4
#define		MONSTER_EXP				5

#define		ITEM_UPGRADE_WEAPON		0		// 아이템 업그레이드 배열 인덱스
#define		ITEM_UPGRADE_UPGRADE	1
#define		ITEM_UPGRADE_LIQUID		2
#define		ITEM_UPGRDAE_EDITION	3

#define		ITEM_MAKING_RESULT		0
#define		ITEM_MAKING_MATERIALS1	1 
#define		ITEM_MAKING_MATERIALS2	2
#define		ITEM_MAKING_ZODIAC		3


#define		ITEM_UPGRADE_SUCCESS_TYPE_AD	1		// 공격력 성공
#define		ITEM_UPGRADE_SUCCESS_TYPE_DP	2		// 방어력 성공
#define		ITEM_UPGRADE_SUCCESS_TYPE_OP	4		// 옵션 성공
#define		ITEM_UPGRADE_SUCCESS_TYPE_SL	8		// 슬롯증가 성공
#define		ITEM_UPGRADE_WAITING			127		// 업그레이드 결과 기다리는중
#define		ITEM_UPGRADE_FAIL				128		// 실패

#define MAX_NPC_NUM_PER_VILLAGE		120

#define MAX_SKILL_LEVEL				50
#define ITI_TYPE_BASESKILL			3000
#define MAX_KIND_OF_CLASS_EX		MAX_KIND_OF_CLASS + 2	// 클레스에 속하지 않는 이펙트가 필요 해서.
#define MAX_STATUS_SKILLVALUE		5		// 상태스킬에 의한 변화할수 있는값.

#define TARGETTYPE_NONE		0
#define TARGETTYPE_TILE		1
#define TARGETTYPE_ENEMY_MONSTER  2	// 대상이 몬스터
#define TARGETTYPE_FRIEND_MONSTER  4	// 대상이 몬스터
#define	TARGETTYPE_ENEMY_PLAYER	8		// 대상이 플레이어
#define	TARGETTYPE_FRIEND_PLAYER	16		// 대상이 플레이어

#define TARGETTYPE_OBJECT	100		// 개체
#define	TARGETTYPE_LINE		110		// 직선
#define	TARGETTYPE_CIRCLE	120		// 원안에 있는 놈들
#define	TARGETTYPE_X		130		// X 모양에 있는놈들
#define TARGETTYPE_BOX		 140	// 박스
#define TARGETTYPE_MULTICOLLITION 150	// 3중 충돌
#define TARGETTYPE_TIMEZEROTILE 160	// 타일에다가 지뢰를 박는다. 시간제한.
#define TARGETTYPE_SUMMONTILE	170 // 타일에다가 소환을 한다.
#define	TARGETTYPE_ARC		180		// 부채꼴

#define	TYPE_ACTIVE		1
#define	TYPE_DRIVE		2
#define	TYPE_PASSIVE	3
#define TYPE_TIMEZERO	4

#define MAX_SKILL_COUNT				255

#define __SKILL_MANAMASTERY__			1
#define	__SKILL_FIREMISSILE__			2
#define	__SKILL_FIREBLAZE__				3
#define	__SKILL_MAGMAWALL__				4
#define	__SKILL_ICEMISSILE__			5
#define	__SKILL_ICEWAVE__				6
#define	__SKILL_ICEWALL__				7

#define	__SKILL_EARTHQUAKE__			9
#define	__SKILL_METEOR__				10
#define	__SKILL_LIGHTNING__				11
#define	__SKILL_AIREALCOAT__			12
#define	__SKILL_THUNDERSTORM__			13
#define	__SKILL_MANAEXPAND__			14
#define	__SKILL_SPEEDCASTING__			15
#define	__SKILL_EXPLOSION__				16
#define	__SKILL_MEDITETION__			17
#define	__SKILL_MULTICASTING__			18
#define	__SKILL_CONCENTRATION__			19
#define __SKILL_BLASTING__				20
#define __SKILL_FREEZING__				21
#define __SKILL_FLASHSHOCK__			22
#define __SKILL_DISPEL__				23
#define __SKILL_FIREMASTERY__			24
#define __SKILL_ICEMASTERY__			25
#define __SKILL_LIGHTMASTERY__			26
#define __SKILL_DRAGONICFIREBLAST__		28
#define __SKILL_DRAGONICVIBRATION__		29
// 오라 계열
#define	__SKILL_AURAMASTERY__			31
#define	__SKILL_AURARECHARGE__			32
#define	__SKILL_ZEAL__					33
#define	__SKILL_BASH__					34
#define __SKILL_LIGHTFLING				35
#define	__SKILL_RAGINGSWORD__			36
#define __SKILL_LIFEBALANCE__			37
#define	__SKILL_LIGHTNINGBREAK__		38
#define	__SKILL_SILENTBRANDYSHIP__		39
#define	__SKILL_ANTAGONIZE__			41
#define	__SKILL_AURABATTLER__			42
#define	__SKILL_LIFEFORCE__				43
#define	__SKILL_REGENERATION__			44
#define	__SKILL_WEAPONMASTERY__			45
#define	__SKILL_DEFENSEMASTERY__		46
#define __SKILL_SWORDMASTERY__			47
#define __SKILL_BLUNTWEAPON_FIGTER_MASTERY__	48
#define __SKILL_SPEARMASTERY__			49
#define __SKILL_INFULX__				50
#define __SKILL_AMPLIFY__				51
#define __SKILL_BERSERKER__				52

// 디바인 계열
#define	__SKILL_DIVINEMASTERY__			61
#define	__SKILL_HEAL__					62
#define	__SKILL_PRAY__					63
#define	__SKILL_BLESSING__				64
#define	__SKILL_WINDTALES__				65
#define	__SKILL_HOLDSHIELD__			66
#define	__SKILL_WINDFORCE__				67
#define	__SKILL_TREEAGION__				68
#define	__SKILL_AMFLEAFIRE__			69
#define	__SKILL_HOLYPRESSURE__			70
#define	__SKILL_APSOLUTMAGICBARRIER__	71
#define	__SKILL_SPELLBINDING__			72
#define	__SKILL_DETECT__				73
#define	__SKILL_AROSE__					74
#define	__SKILL_HOLYDIVINE__			75
#define __SKILL_ORDRISING__				77
#define __SKILL_SUITRISING__			78
#define __SKILL_ORDSLUM__				79
#define __SKILL_SUITSLUM__				80
#define __SKILL_REMEDY__				81
#define __SKILL_PROTECTSHIELD__			82
#define __SKILL_FRAIL__					83
#define __SKILL_DETECTION__				84
#define __SKILL_ROUNDRANGE__			85
#define __SKILL_BLUNTWEAPON_PREST_MASTERY__	86
#define __SKILL_HOLYBRAIN				87
#define __SKILL_INCREASING				88
#define	__SKILL_LANDINGSUPPORT__		89
// 서몬 계열
#define	__SKILL_SUMMONMASTERY__			91
#define	__SKILL_CALMDOWN__				92
#define	__SKILL_SLEEP__					93
#define	__SKILL_CONFUSE__				94
#define	__SKILL_MINDCONTROL__			95
#define	__SKILL_RAGE__					96		//kk
#define	__SKILL_REDELEMENTAL__			97
#define __SKILL_SOULBLASTER__			98
#define	__SKILL_MINDEXPLOSION__			99
#define	__SKILL_SOULETER__				100
#define	__SKILL_SUMMONACCELERATOR__		101
#define	__SKILL_DOMENATION__			102
#define	__SKILL_TAMINGMASTERY__			103
#define	__SKILL_BEYONDLIMIT__			104
#define __SKILL_BLUEELEMENTAL__			105
#define __SKILL_GREENELEMENTAL__		106
#define __SKILL_MIRACULOUSHEART__		107
#define __SKILL_ARCANUMSKIN__			108
#define __SKILL_FRENZY__				109
#define __SKILL_DETONATION__			110
#define __SKILL_TURNOVER__				111
#define __SKILL_DETONATION_MONSTER__	112
// 차크라 계열
#define	__SKILL_CHAKRAMASTERY__			121
#define	__SKILL_POISONING__				123
#define	__SKILL_CHAINATTACKER__			124
#define	__SKILL_IRONSKIN__				125
#define	__SKILL_SNIPING__				126
#define	__SKILL_SHADOWSQUARE__			127
#define	__SKILL_HIDING__				128
#define	__SKILL_SPELLTRAP__				129
#define	__SKILL_PRESSURE__				130
#define	__SKILL_SYLPHID__				131
#define	__SKILL_SHAUT__					132
#define	__SKILL_BERSERK__				133
#define	__SKILL_SPEEDOVER__				134
#define	__SKILL_DODGE__					135
#define	__SKILL_TRAINING__				136
#define	__SKILL_BLASTQUAKE__			137
#define	__SKILL_LIFEEXPLOSION__			138
#define __SKILL_DEATHBLOW__				139
#define __SKILL_VAMPIRE__				140
#define __SKILL_POSIONCLOUD__			141
#define __SKILL_DARK__					143
#define __SKILL_FORTIFICATIONMASTERY__	144
#define __SKILL_AMPLIFICATIONMASTERY__	145
#define __SKILL_POISONMASTERY__			146
#define __SKILL_EXTREMESPEAR__				147
#define __SKILL_ATTACK__				151
#define __SKILL_NONE_SELECT__			255

#define EFFECT_TYPE_STATUS_TOP		1		
#define EFFECT_TYPE_STATUS_CENTER	2		
#define EFFECT_TYPE_STATUS_BOTTOM	4

#pragma pack(push, 1)
struct VALUE
{
	int		nMin;
	int		nMax;
	int		nMana;
	int		nCompass;
	int		nDuration;
	int		nProbability;
};

struct STATUSVALUE
{
	WORD			wStatusID;	// 상태마법을 썻을때 변화되는 필드 플래그.
	BYTE			bFormula;	// 식에 의한 값을 계산하기 위해서.
};
#pragma pack(pop)

#define	STORE_TYPE_WEAPON				1
#define	STORE_TYPE_ARMOR				2
#define	STORE_TYPE_SUSPZO				3
#define STORE_TYPE_BAG					4

#define	MAX_MONEY_PLAYERSHOP			4000000000
#define	__MAX_MONEY__					4200000000

#define	MAX_REAL_PLAYER_SHOP_TITLE	30

#define		__GCTYPE_NONE__				0
#define		__GCTYPE_GUILD__			1
#define		__GCTYPE_CLEN__				2

enum ITEM_SUPPLY_TYPE {
	ITEM_SUPPLY_TYPE_PERCENT_HP_POTION = 1,
	ITEM_SUPPLY_TYPE_PERCENT_SP_POTION = 2,
	ITEM_SUPPLY_TYPE_FIXED_HP_POTION = 3,
	ITEM_SUPPLY_TYPE_FIXED_SP_POTION = 4
};


/// DO NOT CHANGE THE CONTENTS of (expPerLevel) between different calls
//  internal caches are created based on their values
DWORD DECLSPECIFIER cumulatedExperienceAtLevel(DWORD level, const DWORD (&expPerLevel)[MAX_LEVEL + 1]);
DWORD DECLSPECIFIER levelForCumulatedExperience(DWORD exp, const DWORD (&expPerLevel)[MAX_LEVEL + 1]);

struct DECLSPECIFIER AttackResult {
private:
	BYTE	bStatusKind;
	DWORD	dwCurUserHP;
public:
	AttackResult(BYTE statusKind, DWORD curUserHP): bStatusKind(statusKind), dwCurUserHP(curUserHP) { }
	AttackResult(const DSTC_ATTACK_MON_USER& attack): bStatusKind(attack.bStatusKind), dwCurUserHP(attack.dwCurUserHP) { }
	AttackResult(const DSTC_ATTACK_USER_USER& attack): bStatusKind(attack.bStatusKind), dwCurUserHP(attack.dwCurDefenseUserHP) { }
	void applyFor(std::function<void(DWORD)> hpApply, std::function<void(DWORD)> spApply);

	~AttackResult();
};

inline bool isContinousSkill(BYTE skillID) { 
	// these really should be migrated to an enum class or a better representation
	return (
		skillID == __SKILL_AURARECHARGE__ ||
		 // summoner
		skillID == __SKILL_SUMMONACCELERATOR__ ||
		skillID == __SKILL_SOULETER__ ||
		skillID == __SKILL_MINDEXPLOSION__ ||
		
		// sorceress
		skillID == __SKILL_EXPLOSION__ ||
		skillID == __SKILL_SPEEDCASTING__ ||

		// priest
		skillID == __SKILL_INCREASING ||
		skillID == __SKILL_SPELLBINDING__ ||

		// ranger
		skillID == __SKILL_SYLPHID__ ||
		skillID == __SKILL_SHAUT__ ||
		skillID == __SKILL_BERSERK__ ||

		// figher 
		skillID == __SKILL_LIFEFORCE__ ||
		skillID == __SKILL_BERSERKER__
		);
}