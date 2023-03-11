#pragma once
/*	
#######################################################
	#include �����ϴ°�
#######################################################
*/
#include <crtdbg.h>
#include <assert.h>
#include "MatchInfo.h"
#include "ObjectDefine.h"
#include "gamedefine.h"
#include "Chat.h"


/*	
#######################################################
	#define �����ϴ°�
#######################################################
*/
//=========================================//
// GM Command Deinfe.
#define	GM_COMMAND_PK_MODE		1
#define GM_COMMAND_CHAT_DISABLE	2
#define GM_COMMAND_CREATE		3
#define GM_COMMAND_POTAL		4
#define GM_COMMAND_RECALL		5
#define GM_COMMAND_USERPOTAL	6

#define ZODIAC_USE_TYPE_INVENTORY	0
#define ZODIAC_USE_TYPE_BELT		1


//////////////////////////////////////////////////////////////////////////
// enums
enum DUNGEON_LAYER_PROPERTY
{
	Dungeon_Layer_Property_Village,						// ����
	Dungeon_Layer_Property_Ancient,						// ��� 
	Dungeon_Layer_Property_Underground_Waterway,		// ���ϼ��� 
	Dungeon_Layer_Property_Underground,					// ���� 
	Dungeon_Layer_Property_Castle,						// ��
	Dungeon_Layer_Property_Ice,							// ���� 
	Dungeon_Layer_Property_Forest,						// ��
	Dungeon_Layer_Property_Lava,						// ���
	Dungeon_Layer_Property_Towner,						// ž 
	Dungeon_Layer_Property_Aqua,						// ���ߴ���
	Dungeon_Layer_Property_Ameritart,					// �巡���� ����
	Dungeon_Layer_Property_Light_House,					// ������
	Dungeon_Layer_Property_GuildWar_Field,				// ����� �ʵ�
};


/*	
#######################################################
	class �����ϴ°�
#######################################################
*/
class CMonster;
class CUser;
class CMap;
class COnlyList;
class CItem;
struct ITEM;
struct DSTC_JOIN;
struct DSTC_APPEAR;
struct DSTC_APPEAR_MON;
struct DSTC_CHANGE_LAYER;
struct CTDS_ITEM_PICKUP;
struct DSTC_ITEM_PICKUP;
extern CMap*		g_pMap;

typedef struct GlobalVariableDungeon
{
	CMap*					pMap;

	// �������̽� �����ҋ� �̾����� �� ������.
	//--------------------------------//

	//-- ä�� 
	BYTE					bCurChatLine;
	BYTE					bChatMode;
	char					szChatDisplayBuf[ MAX_DISPLAY_CHAT_LINE ][ MAX_CHAT_INPUT_LENGTH ];		//ä�� Display�� ���� 
	BOOL					bMainUserPotalJoin;	// �� ¥���� �� ������ ¥�� �̺����� �ʿ��ϳ�..��..¥������.
	//

	/*
	// �Ʒ� ������ �� �̻��Ͽ� �ּ�ó��.
	~GlobalVariableDungeon()
	{
		// Delete Map		
		if( g_pMap )
		{
			delete g_pMap;
			g_pMap = NULL;
		}
	}
	*/

} GLOBAL_VARIABLE_DUNGEON, *LPGlobalVariable_Dungeon;


/*	
#######################################################
	extern ���� �����ϴ°�
#######################################################
*/
extern int			g_iCameraX;
extern INT			g_iCurrentLayerUserNo;
extern INT			g_iNeedUserNo;
extern INT			g_iNeedKillMonsterNo;
extern BOOL			g_bIsLastLayer;
extern INT			g_iCurrentKillMonsterNo;
extern INT			g_iTotalUserCount;
extern DWORD		g_dwChangeLayerRemainTime;
extern BOOL			g_bMagicChk;
extern BOOL			g_bRButton;
extern BOOL			g_bBeltChk;
extern BOOL			g_bKeyChkUp;
extern BOOL			g_bGuildChk;
extern BOOL			g_bLshift;
extern BOOL			g_bLCtrl;
extern BOOL			g_bLAlt;
extern CMatchInfo	g_MatchInfo;
extern COnlyList*	g_pDyingMonsterList;		// ���� �ܻ��� ���� �ð��Ǹ� �������
extern LPGlobalVariable_Dungeon g_pGVDungeon;


/*	
#######################################################
	Function �����ϴ°�
#######################################################
*/
void SendPKMode(BOOL bPKMode);
BOOL InitGameDungeon();
void UpdateGameDungeon();
void ReleaseGameDungeon();
//======================================================================//
// Send Packet Func ����.												//
//======================================================================//
void SendMovePacket();
void SendStopPacket();
BOOL SendPacketRequestDungeonInfo(WORD wDungeonID);
//======================================================================//
// Effect ����.															//
//======================================================================//
void EffectOnceAndHideFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
//======================================================================//
// Render ����.															//
//======================================================================//
DWORD __stdcall AfterRenderGameDungeon();
DWORD __stdcall BeforeRenderGameDungeon();
void ChatRender();									// ä�� �޼��� ���.
void SkillRender();									// ��ų Ű ���.
void EventDungeonRender( void );					// �̺�Ʈ ���� ���� ������.
//======================================================================//
// NPC ����.															//
//======================================================================//
BOOL LoadNpcInfo(DWORD dwMapID);
void ConvertKeyDown(char* szKey);
void RequestDungeonInfo( BYTE bType, DWORD dwIndex );
void SetFocusInput(int nCurFocus, int nInputType, int nOrder, int nLeft, int nRight, int nTop, int nBottom, BOOL bPos = FALSE);
BOOL ShortKeyEmoticon(WORD wParam, int iLowKey, int iHighKey);	
//======================================================================//
// Game Func.															//
//======================================================================//
BOOL PathFindDungeon();
BOOL SetPathFindMove();	// �Լ��� �ѹ��� ȣ�� �ɋ����� ��ã�� ����� �ϳ��� �����Ѵ�.
void DungeonEnvironmentSetting(DUNGEON_DATA_EX* pDungeon);
//======================================================================//
// Init, Remove.														//
//======================================================================//
BOOL	InitChrInfo( char* szFileName, DWORD dwKind, DWORD dwFlag );
void	InitMainPlayer( DSTC_JOIN* pJoin );
void	InitMainPlayer( DSTC_CHANGE_LAYER* pLayer );
CUser*	InitPlayer( DSTC_APPEAR* pAppear );
void	InitMonster( DSTC_APPEAR_MON* pAppear );
#ifdef DEVELOP_MODE
void LoadDebugTile();
void UnloadDebugTile();
#endif
//======================================================================//
// Input Func.															//
//======================================================================//
void OnKeyDownDungeon(WPARAM wParam, LPARAM lParam);
void OnKeyUpDungeon(WPARAM wParam, LPARAM lParam);
void OnLButtonDownDungeon(WPARAM wParam, LPARAM lParam);
void OnLButtonUpDungeon(WPARAM wParam, LPARAM lParam);
void OnRButtonDownDungeon(WPARAM wParam, LPARAM lParam);
void OnRButtonUpDungeon(WPARAM wParam, LPARAM lParam);
void OnMouseMoveDungeon(WPARAM wParam, LPARAM lParam);
void OnMouseWheelDungeon(WPARAM wParam, LPARAM lParam);
void OnCharDungeon(BYTE bCh);
void OnTimerEventDungeon(DWORD dwTimerIndex);
void MouseEventDungeon();
BOOL OnLButtonDownInterfaceDungeon();	// OnLButtonDownDungeon�ȿ� ���� �Լ� ���� TRUE�̸� return.
//======================================================================//
// Update Func.															//
//======================================================================//
void PathFindMoveFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void PlayerMoveFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void DamageNumberFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void UpdateMainPlayer( WORD wSectionNum );
void UpdatePlayer();
void UpdateMonster();
void UpdateItem();
void GXClickProc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void PlayerShopProc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void ShopProc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void OpenQuantityWnd(BYTE bType);
void DisplayMatchInfoOnDungeon();
BOOL ItemCollision(int nIndex);
BOOL BeltCollision();
BOOL GuardianCollision();
void MagicArrayUsed(BYTE byIndex);
void ChatMsg();
void ErrMessage(char* szInfo, DWORD dwColor, BYTE byType = DEF_CHAT_TYPE_ALLCHAT);
void InputChatName(BOOL bChk = FALSE);
void SetKey(int nKey);
void ChatOff();

void CameraAccelatingMove(void);
void SelectSummonMonster(CMonster* pMonster, BOOL bSelect);
void PlayItemSound(WORD	wItemId);
DWORD __stdcall AfterInterporation(AFTER_INTERPOLATION_CALL_BACK_ARG* pArg);
BOOL FindEmptyPosNearDungeon(VECTOR3* vpDungeonPos, VECTOR3 *vpNearPos, BOOL bVillage = FALSE);

//------------------------------------------------------------------------
// minjin ( 2004. 10. 06. ) Source Code Refactoring
//------------------------------------------------------------------------
void SetEffectIndexAll( CUser* pUser, DWORD dwEffectIndex );
void RenderRectChatBox( CUser* pUser, VECTOR2* v2Scale, VECTOR2* v2Position, int nIndex );
void RenderChatMsgOnBox( CUser* pUser, VECTOR2* pV2Pos, int nIndex, DWORD dwColor );
void RenderChatMsgOnBox( CUser* pUser, int x, int y, int nIndex, DWORD dwColor );
