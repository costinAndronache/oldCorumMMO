#pragma once


#include "../SS3D_0719/4DYUCHIGX/4DyuchiGXGFunc/type.h"
#include "../CommonServer/CommonHeader.h"
#include "../CommonServer/AbyssHash64.h"
#include "../CommonServer/ComInterface.h"
#include "../CommonServer/CommonClientDungeon.h"
#include "../CommonServer/CommonAllServers.h"
#include "../CommonServer/CommonClientDungeonLoginWorld.h"

#include "ItemMoveManager.h"
#include "ItemUsedManager.h"
#include "GlobalDefine.h"
#include "user.h"


class	CDungeon;
class	CMonster;	
struct	BASEMONSTER;
class	CItemTradeHash;
class	CItemShopHash;
class	CItemStoreHash;
class	CShopTileHash;
class	CPlayerShopLogHash;
class	CSection;
class	COwnServer;
class	CDungeonLayer;
struct	DSTC_APPEAR;
struct	DSTC_APPEAR_MON;
class	CUser;
class	CItem;
struct	VECTOR2;
struct	ITEM_SERVER;
struct	ITEM_OPTION_SERVER;
class	CItemAttrLayer;
struct	ITEM_UPGRADE;
class	CGuildListHash;
class	cGDI;
class	CUnit;
class	CAcceptTable;
class	CDungeonTable;
struct	BASEITEM_BOTARY;
class   CMonsterTable;
class   CUserHash;
class   CClassMemoryPool;
enum	UNIT_CLASS_ID;


typedef CAbyssHash<SET_ITEM_INFO>		SET_ITEM_INFO_HASH,		* LPSET_ITEM_INFO_HASH;
typedef CAbyssHash64<CBaseItem>			BASEITEM_HASH,			* LPBASEITEM_HASH;
typedef CAbyssHash64<ITEM_SERVER>		ITEM_HASH,				* LPITEM_HASH;
typedef CAbyssHash<ITEM_MAKING_INFO>	ITEM_MAKING_INFO_HASH,	* LPITEM_MAKING_INFO_HASH;
typedef CAbyssHash<INVALID_LOG_ITEM>	INVALID_LOG_ITEM_HASH,	* LPINVALID_LOG_ITEM_HASH;
typedef CAbyssHash<USER_SET_ITEM_INFO>	USER_SETITEM_HASH,		* LPUSER_SETITEM_HASH;
typedef CAbyssHash<ITEM_STORENATIVE>	ITEM_STORENATIVE_HASH,	* LPITEM_STORENATIVE_HASH;
typedef CAbyssHash<ITEM_EVENT>			ITEM_EVENT_HASH,		* LPITEM_EVENT_HASH;


class CPlayerShopLogHash : public CAbyssHash64<PLAYERSHOP_LOG>
{
public:
	
	void DeleteData( PLAYERSHOP_LOG* pData );
};

class CItemTradeHash : public CAbyssHash64<ITEM_TRADE>
{
public:
	
	void DeleteData( ITEM_TRADE* pData );
};

class CItemStoreHash :  public CAbyssHash64<ITEM_STORE>
{
public:
	
	void DeleteData( ITEM_STORE* pData );
};

class CItemShopHash :  public CAbyssHash64<ITEM_SHOP>
{
public:
	
	void DeleteData( ITEM_SHOP* pData );
};

class CShopTileHash :  public CAbyssHash64<SHOP_TILE>
{
public:
	
	void DeleteData( SHOP_TILE* pData );
};

class CUserHash : public CAbyssHash<CUser>
{
public:
	
	void DeleteData(CUser* pData);
};

class CGuildListHash :  public CAbyssHash64<GUILD_NODE>
{
public:
	
	void DeleteData( GUILD_NODE* pData );
};

class CVirtualLinkItemNativeHash : public CAbyssHash64<CItem>
{
public:	

	void DeleteData( CItem* pData );
};



#define		GM_COMMAND_PK_MODE		1
#define		GM_COMMAND_CHAT_DISABLE	2
#define		GM_COMMAND_CREATE		3
#define		GM_COMMAND_POTAL		4
#define		GM_COMMAND_RECALL		5
#define		GM_COMMAND_USERPOTAL	6 


#define		CRIMINAL_USER_ATTACK			1	// PK할때
#define		CRIMINAL_ATTACK_SUMMON_BY_USER	2	// 소환물을 공격할때
#define		CRIMINAL_ATTACK_USER_BY_SUMMON	3	// 소환물을 이용해 공격할때
#define		CRIMINAL_USER_BLUE_LEVEL_PK		6	// 유저를 때릴때.
#define		CRIMINAL_USER_RED_LEVEL_PK		4	// 유저를 때릴때.
#define		CRIMINAL_MONSTER_KILL			7	// 몬스터를 죽일때.


#define		MAX_ITEM_OWNER_FRAME	10		// 아이템의 소유권 시간.


#define		ITEM_DUNGEONMANAGER_MATERIALS	0
#define		ITEM_DUNGEONMANAGER_GUARDIAN	1


#define		MAX_DUNGEON_PROPERTY			5		// 던전 속성


#define		ITEM_LOG_PICKUP					1		// 아이템을 줍다.
#define		ITEM_LOG_DROP					2		// 아이템을 떨구다.
#define		ITEM_LOG_USED					3		// 아이템 사용 
#define		ITEM_LOG_EXCHANGE				4		// 아이템을 교환으로 줍다.
#define		ITEM_LOG_SUM					5		// 아이템을 합쳤다.
#define		ITEM_LOG_SUB					6		// 아이템을 나누었다.
#define		ITEM_LOG_DELETE_BY_SUM			7		// 아이템 합치기에 의해 삭제 됐다.
#define		ITEM_LOG_CREATE_BY_SUB			8		// 아이템 나누기에 의해 생성됐다.
#define		ITEM_LOG_CREATE_BY_SHOP			9		// NPC에게 구입했다.
#define		ITEM_LOG_DELETE_BY_SHOP			10		// NPC에게 팔다.
#define		ITEM_LOG_UPGRADE				11		// 아이템 업그레이드
#define		ITEM_LOG_PLAYERSHOP				12		
#define		ITEM_LOG_TRADE					13
#define		ITEM_LOG_UPGRADE_FAIL			14		// 아이템 업그레이드 실패 
#define		ITEM_LOG_UPGRADE_FAIL_INSURANCE 15		// 아이템 업그레이트 실패 이지만 보험 처리
#define		ITEM_LOG_KARZ_DROP				16	 
#define		ITEM_LOG_KARZ_PICKUP			17
#define		ITEM_LOG_USER_INSURANCE			18
#define		ITEM_LOG_PLAYERSHOP_BUY			19
#define		ITEM_LOG_PLAYERSHOP_SELL		20
#define		ITEM_LOG_PLAYERSHOP_BUY_KARZ	21
#define		ITEM_LOG_PLAYERSHOP_SELL_KARZ	22
#define		ITEM_LOG_MAKING					23
#define		ITEM_LOG_MAKING_FAIL			24
#define		ITEM_LOG_TRADE_KARZ				25
#define		ITEM_LOG_KARZ_RECEIVE			26
#define		ITEM_LOG_KARZ_DRAWOUT			27


// 아이템 로그 추가 김영대 2005.02.23
#define		ITEM_LOG_ITEM_MALL				28		// 아이템몰에서 구입
#define		ITEM_LOG_DUNGEON_ACQUISITION	29		// 던전 산출물
#define		ITEM_LOG_GUARDIAN_INCUBATION	30		// 가디언 아이템 부화
#define		ITEM_LOG_SEAL_OF_HERO			31		// 영웅의 증표 사용으로 얻은 아이템
#define		ITEM_LOG_GM_CREATITEM			32		// GM 아이템 생성

#define		ITEM_LOG_PLAYERSHOT_OPEN		33		// 노점에 아이템 올릴때
#define		ITEM_LOG_PLAYERSHOT_CLOSE		34		// 노점 닫을때
#define		ITEM_LOG_BANK_IN				35		// 창고에 아이템 넣을때
#define		ITEM_LOG_BANK_OUT				36		// 창고에서 아이템 뺄때
#define		ITEM_LOG_BANK_KARZ_IN			37		// 창고에 카르츠 보관
#define		ITEM_LOG_BANK_KARZ_OUT			38		// 창고에서 카르츠 찾을때


typedef struct ATTACK_RESULT
{
	BYTE	byType;	
	DWORD	dwDamage;
} * LPATTACK_RESULT;

typedef struct SKILL_RESULT
{
	BYTE	byType;	
	int		nDamage;
} * LPSKILL_RESULT;

struct SKILL_TARGETINFO
{
	DWORD			dwTargetType;
	CUnit*			pUnit;	
};

struct PARAM_TARGETCOUNT
{
	CDungeonLayer*		pDungeonLayer;
	VECTOR2*			vecStart;
	VECTOR2*			vecDest;
	SKILL_TARGETINFO*	pTargetInfo;
	CUser*				pMaster;
	DWORD				dwTargetIndex;
	BYTE				bTargetType;
	BYTE				bSkillKind;
	BYTE				bSkillLevel;	
};

struct Character_Propensity
{
	DWORD	dwID;
	BYTE	bSelf_Defense;
	BYTE	bWeight;
	short	wCriminalPointMin;
	short	wCriminalPointMax;
	BYTE	byExp_Loss;
	BYTE	byItem_Loss_Min;
	BYTE	byItem_Loss_Max;
	BYTE	byItem_Loss_Count;
	BYTE	byItem_GetBack_Rate;	
};

typedef struct DUNGEON_ENTER_USER_COUNT
{
	unsigned long int	iID;
	char				szName[MAX_DUNGEON_NAME_LENGTH];
	unsigned long int	iUserCount;	
} *LPDUNGEON_ENTER_USER_COUNT;

struct NPC_TABLE
{
	DWORD dwID;
	DWORD dwType;	
};


extern STMPOOL_HANDLE				g_pItemPool;		
extern STMPOOL_HANDLE				g_pCreateItemPool;	
extern STMPOOL_HANDLE				g_pItemTradePool;
extern STMPOOL_HANDLE				g_pItemStorePool;
extern STMPOOL_HANDLE				g_pItemShopPool;
extern STMPOOL_HANDLE				g_pShopTilePool;
extern STMPOOL_HANDLE				g_pPlayerShopLogPool;
extern STMPOOL_HANDLE				g_pGuildListPool;
extern STMPOOL_HANDLE				g_pNodePool;
extern STMPOOL_HANDLE				g_pPortalBufferPool;
extern STMPOOL_HANDLE				g_pVirtualLinkItemNativePool;

extern BYTE							g_bEnableSpeedHackLog;
extern BYTE							g_bEnableItemLog;
extern BYTE							g_byStstus[MAX_LEVEL];
extern BYTE							g_byGuardianStatus[MAX_LEVEL];
extern BYTE							GAME_DB;
extern BYTE							LOG_DB;
extern DWORD						g_dwTickCount;
extern DWORD						g_dwSpeedHackCheckPeriod;
extern DWORD						g_dwSpeedHackCheckError;
extern DWORD						g_dwScreenBufferLastRow;
extern DWORD						g_pGuilRankId;
extern DWORD						g_dwPlayerShopKey;
extern DWORD						g_pdwExp[MAX_LEVEL +1];
extern DWORD						g_pdwGuardianExp[MAX_LEVEL +1];
extern DWORD						g_dwOwnServerID;
extern DWORD						g_dwOwnServerSetNum;
extern int							g_ConsoleLogLevel;
extern int							g_FileLogLevel;
extern BOOL							g_bMonitor;
extern BOOL							g_bIsServerRunning;
extern HANDLE						g_hIn;
extern HANDLE						g_hOut;
extern HWND							g_hDlg;
extern FILE*						g_fpLog;
extern cGDI*						g_pGDI;
extern CRITICAL_SECTION				g_criLog;

extern CItemTradeHash*				g_pItemTradeHash;
extern CItemShopHash*				g_pItemShopHash;
extern CItemStoreHash*				g_pItemStoreHash;
extern CShopTileHash*				g_pShopTileHash;
extern CPlayerShopLogHash*			g_pPlayerShopLogHash;
extern CGuildListHash*				g_pGuildListHash;
extern CUserHash*					g_pUserChkHash;
extern LPSET_ITEM_INFO_HASH			g_pSetItemInfoHash;
extern LPITEM_MAKING_INFO_HASH		g_pItemMakingInfoHash;
extern LPBASEITEM_HASH				g_pBaseItemHash;
extern LPINVALID_LOG_ITEM_HASH		g_pInvalidLogItemHash;
extern LPUSER_SETITEM_HASH			g_pUserSetItemHash;
extern LPITEM_STORENATIVE_HASH		g_pItemStoreNativeHash;
extern LPITEM_EVENT_HASH			g_pItemEventHash;

extern BASE_CLASS_INFO				g_pBaseClassInfo[MAX_CLASS+1];
extern BASE_CLASS_INFO				g_pBaseRaceInfo[MAX_RACE+1];
extern BASE_CLASS_INFO				g_pMonsterBaseClassInfo[MAX_CLASS+1];
extern Character_Propensity			g_pCharacter_Propensity[MAX_CHARACTER_PROPENSITY];
extern DUNGEONPRODUCTIONITEMMINMAX	g_DungeonProductionItemMinMax[MAX_DUNGEON_PROPERTY];
extern NPC_TABLE					*g_NPCTable;
extern CPTable						g_pCPTable[MAX_CP_TABLE];

extern ITEM_OPTION_SERVER*			g_pItemOptionTable;
extern CItemAttrLayer*				g_pItemAttrLayer;
extern ITEM_UPGRADE*				g_pItemUpgradeTable[ MAX_ITEM_UPGRADE_LEVEL + 1 ];

extern CAcceptTable*				g_pAcceptTable;
extern CDungeonTable*				g_pDungeonTable;
extern CClassMemoryPool*			g_pMonsterClassPool;
extern COwnServer*					g_pThis;
extern BASEITEM_BOTARY*				g_pBaseBotary;
extern CMonsterTable*				g_pMonsterTable;

extern CItemMoveManager				g_ItemMoveManager;
extern CItemUsedManager				g_ItemUsedManager;

extern IOleDBThread*				g_pDb;
extern IOleDBThread*				g_pLogDb;	

#ifdef DEV_MODE
extern BYTE							g_bEnableChatLog;
extern BYTE							g_bEnableSpeedHackLog;
extern BYTE							g_bEnableItemLog;
#else
extern BYTE							g_bEnableChatLog;
extern BYTE							g_bEnableSpeedHackLog;
extern BYTE							g_bEnableItemLog;
#endif


extern void	(*PacketProc[ MAX_UPDATE_GAME ][ MAX_PACKET_NUM ])( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
extern void ListenForUserside();
extern WORD UnpackPresentBox(CItem* pItem, WORD* pCount);

BOOL SetChatLogInDungeon(CHAT_TYPE eChatType, 
						 CUser * pUser, 
						 CUser * pRecvUser, 
						 char * pszChat, 
						 int ChatLlen);

BOOL	SpeedHackLog( CUser* pUser, DWORD dwHackApplication = 0 );
BOOL	SpeedHackCheck_Attack( CUser* pUser, BOOL bUserMon = TRUE );
int		AttackResult_Unit_Unit( CUnit* pAttackUnit, CUnit* pDefenseUnit, LPATTACK_RESULT pResult );
BOOL	IsGuildWar(CUser *pOwnUser, CUser* pTargetUser);
BOOL	IsGuildWar(const CDungeon* pDungeon, DWORD dwOffenseGuildWarID, DWORD dwDefenseGuildWarID, DWORD dwDOffencePartyId, DWORD dwDefencePartyId, enum ENUM_GODMODE_STATUS eGodMode);
int		GetUserEmptyInventoryPos(CUser* pUser, BOOL bSmallInv);
int		GetSmallInvEmptyPos( CUser* pUser );
int		GetLargeInvEmptyPos( CUser* pUser );
void	CmdSkill( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
CUnit*	CreateUnitInstance(UNIT_CLASS_ID eClsID);
void	SendDungeonJoinFailMessage(DWORD dwConnectionIndex, DUNGEON_JOIN_FAIL eDungeonFail);
void	MakeAppearMonser( DSTC_APPEAR_MON* pAppearMon, CMonster* pMonster );
void	SendToWorldServerForGuildPoint(DWORD dwOffenseUserID, DWORD dwDefenseUserID, BYTE byGuildPoint, DWORD dwOffenseGuildID, DWORD dwDefenseGuildID);
void	MonsterKillByUser( CUser* pUser, CMonster* pMonster );
void	MonsterKillByMonster( CMonster* pAttackMon, CMonster* pDefenseMon );
void	MatchRecordCount(CUser * pUser);
void	MatchWinCount(CUser * pUser);
void	MatchLoseCount(CUser * pUser);
void	SetBadAction(CUnit* pOffense, CUnit* pDefense);
DWORD	GetExpTableOfLevel(GAME_OBJECT_TYPE eObjectType, DWORD dwLevel);
DWORD	GetCumulatedExpByLevel(GAME_OBJECT_TYPE eObjectType, DWORD dwLevel);
DWORD	GetLevelForCumulatedExp(GAME_OBJECT_TYPE eObjectType, DWORD cumulatedExp);
BYTE	GetStatPointByLevel(GAME_OBJECT_TYPE eObjectType, DWORD dwLevel);
BOOL	IsValidUnit(const CUnit* pUnit);
void	SendPortalFailMessage(CUser* pUser, WORD wToDungeonID, BYTE bErrorCode, WORD wPortalItemID);
WORD	UnpackPresentBox(CItem* pItem, WORD* pCount);


