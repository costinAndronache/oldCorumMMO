#pragma once


#include "CommonServer.h"
#include "CommonHeader.h"
#include "CommonGlobalDefine.h"


#pragma pack(push,1)
struct DECLSPECIFIER ITEM_OPTION_SERVER
{
	DWORD	dwID;
	char	szItemName[50];
	BYTE	pbOption[MAX_ITEM_KIND1_CODE];
};


struct DECLSPECIFIER ITEM_EVENT
{
	DWORD	dwid;
	WORD	witem_id;
	DWORD	dwitem_quantity;
	DWORD	dwmax_count;
	DWORD	dwnow_count;
};


typedef struct DECLSPECIFIER SET_ITEM_INFO
{
	WORD				wSetItemID;
	char				szItemName_Kor[50];
	char				szItemName_Eng[50];
	BYTE				bFullSet;
	WORD				pwSetID[7];	
	SET_ITEM_OPTION_2	pBonusOption[5][2];		// �� 5������ ���� 2���� ������ �ִ�.
	SET_ITEM_OPTION_2	pFullOption[8];			// Set�� ���� ���������ÿ� ������ �ִ� Ǯ�� ���ʽ�.		
} *LPSET_ITEM_INFO;


typedef struct DECLSPECIFIER ITEM_MAKING_INFO
{
	WORD				wID;
	WORD				wItem_id1;
	WORD				wItem_id2;
	WORD				wComplete;
} *LPITEM_MAKING_INFO;


struct DECLSPECIFIER ITEM_UPGRADE
{
	DWORD	dwID;
	BYTE	bUpgradeLevel;
	BYTE	bCode;
	char	szUpgradeDesc[50];
	DWORD	dwMin;
	DWORD	dwMax;
	WORD	wLevel;
	WORD	wMinLevel;
	WORD	wMinGrade;
	WORD	wAddLevel;
	WORD	pwItemMax[ITEM_NUM_PER_OPTION];
};


typedef struct DECLSPECIFIER ITEM_COUNT
{
	WORD	wBaseItemID;		
	BYTE	bServerSet;
	DWORD	dwDungeonID;
} *LPITEM_COUNT;

typedef struct CHAT_LOG
{
	BYTE				bMessageType;		// enum CHAT_TYPE ���� 
	BYTE				bUserType;			// �Ϲ�, GM
	char				szSender[MAX_CHARACTER_NAME_LENGTH]; // ĳ���� ���� 
	char				szRecver[MAX_CHARACTER_NAME_LENGTH];
	WORD				wDungeonID;			// �ش� ����, Ȥ�� ���� ID
	char				szContents[0xff];	// ê�� ����
} *LPCHAT_LOG;

typedef struct DECLSPECIFIER ITEM_LOG
{
	ITEM_SERIAL			Serial;
	BYTE				bLogCode;
	BYTE				bServerSet;
	DWORD				dwDungeonID;
	CItem				Item;
	char				szOwnerName[20];		
	char				szSellerName[20];		

	ITEM_LOG()			{ memset(this,NULL,sizeof(ITEM_LOG)); }
} *LPITEM_LOG;


typedef struct DECLSPECIFIER KARZ_LOG
{
	ITEM_SERIAL			Serial;
	BYTE				bLogCode;
	BYTE				bServerSet;
	DWORD				dwDungeonID;
	CItem				Item;
	DWORD				dwMoney;
	char				szOwnerName[20];		
	char				szSellerName[20];		

	KARZ_LOG()			{ memset(this,NULL,sizeof(KARZ_LOG)); }
} *LPKARZ_LOG;


typedef struct DECLSPECIFIER SPEEDHACK_LOG
{
	char				szName[20];
	BYTE				bServerSet;
	DWORD				dwDungeonID;

	SPEEDHACK_LOG()		{ memset(this,NULL,sizeof(SPEEDHACK_LOG)); }
} *LPSPEEDHACK_LOG;


typedef struct DECLSPECIFIER INVALID_LOG_ITEM
{
	DWORD dwBaseItemID;
}* LPINVALID_LOG_ITEM;


struct DECLSPECIFIER USER_SET_ITEM_INFO
{
	DWORD	dwSetID;		// SetID.
	WORD	wCnt;			// �����ϰ� �ִ� ��Ʈ ������ ����.
	WORD	pwSetItemID[8];	// �����ϰ� �ִ� ��Ʈ ������ ���̵�.
	BOOL	isFullSet;
};


typedef struct DECLSPECIFIER ITEM_STORENATIVE
{
	DWORD	dwID;
	WORD	wItemId;
	BYTE	byType;	
	WORD	wMapId;
	
} *LP_ITEM_STORENATIVE;
#pragma pack(pop)


struct DECLSPECIFIER ITEM_CREATE
{
	void*				DelPos;
	VECTOR2				v2ItemPos;
	DWORD				dwCreateTick;		// �������� ������ ����� ƽ.
	DWORD				dwSectionNum;
	DWORD				dwOwnerIndex;
	DWORD				dwPartyIndex;		// ��Ƽ ��� ������ : �ִ��� 2005.1.26
	CItem				Item;
	BYTE				bTemp[3];
	BYTE				bCreateByType;
	DWORD				dwOwnerLimitSec;    // �����ۼ����� �����Ǵ� �ð�. : ��� 20050330
};

struct DECLSPECIFIER ITEM_SERVER
{
	VECTOR2				v2ItemPos;			// �������� ��ġ.
	DWORD				dwCurFramePerSec;	// �������� ������� �ð�.
	DWORD				dwOwnerFramePerSec;	// �����ۼ����� ������ �ð�.
	DWORD				dwOwnerLimitSec;    // �����ۼ����� �����Ǵ� �ð�. : ��� 20050330
	DWORD				dwOwnerIndex;		// ������ �ε�����ȣ.
	DWORD				dwPartyIndex;		// ��Ƽ ��� ������ : �ִ��� 2005.1.26
	DWORD				dwSectionNum;		// ���� �������� �ִ� ����.
	CItem				Item;				
};


DWORD		DECLSPECIFIER GetItemKind(WORD wItemID);
ITEM_TYPE	DECLSPECIFIER GetItemType(WORD wItemID);
void		DECLSPECIFIER SetCommonServerBaseItemHash(void* pBaseItemHash);









