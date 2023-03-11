#pragma once

#include "define.h"

#define MAX_INPUT_BUFFER_NUM		21

enum	INPUT_BUFFER_ENUM
{
	INPUT_BUFFER_0 = 0,					// ����ϸ� �ȵ�.
	INPUT_BUFFER_CHARACTER_NAME1 = 1,
	INPUT_BUFFER_1 = 1,	
	INPUT_BUFFER_WORLD_CHAT	= 1,
	INPUT_BUFFER_LOGIN_ID = 1,
	INPUT_BUFFER_CHARACTER_NAME2 = 2,
	INPUT_BUFFER_2 = 2,
	INPUT_BUFFER_LOGIN_PASSWORD = 2,
	INPUT_BUFFER_CHARACTER_RACE1 = 3,
	INPUT_BUFFER_3 = 3,
	INPUT_BUFFER_LOGIN_SERVER_LIST = 3,
	INPUT_BUFFER_CHARACTER_CLASS1 = 4,
	INPUT_BUFFER_4 = 4,
	INPUT_BUFFER_CHARACTER_EXP = 5,
	INPUT_BUFFER_5 = 5,
	INPUT_BUFFER_CHARACTER_LEVEL = 6,
	INPUT_BUFFER_6 = 6,
	INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME = 6,
	INPUT_BUFFER_CHARACTER_STR = 7,
	INPUT_BUFFER_7 = 7,
	INPUT_BUFFER_WORLD_DUNGEON_NAME	= 7,
	INPUT_BUFFER_CHARACTER_VIT = 8,
	INPUT_BUFFER_8 = 8,
	INPUT_BUFFER_WORLD_DUNGEON_SIEGE_DEFENSECOUNT = 8,
	INPUT_BUFFER_CHARACTER_DEX = 9,
	INPUT_BUFFER_9 = 9,
	INPUT_BUFFER_WORLD_DUNGEON_SIEGE_TIME	=	9,
	INPUT_BUFFER_CHARACTER_INT = 10,
	INPUT_BUFFER_10 = 10,
	INPUT_BUFFER_WORLD_DUNGEON_LEVELUP_TIME	= 10,
	INPUT_BUFFER_CHARACTER_EGO = 11,
	INPUT_BUFFER_11 = 11,
	INPUT_BUFFER_WORLD_DUNGEON_MANAGER	=	11,
	INPUT_BUFFER_WORLD_DUNGEON_JOIN_FAIL	=	11,
	INPUT_BUFFER_12 = 12,
	INPUT_BUFFER_WORLD_DUNGEON_MEMO	= 12,
	INPUT_BUFFER_13 = 13,
	INPUT_BUFFER_WORLD_DUNGEON_TYPE = 13,
	INPUT_BUFFER_CHARACTER_RACE2 = 14,
	INPUT_BUFFER_14 = 14,
	INPUT_BUFFER_WORLD_DUNGEON_LEVELTYPE = 14,
	INPUT_BUFFER_CHARACTER_CLASS2 = 15,
	INPUT_BUFFER_15 = 15,
	INPUT_BUFFER_WORLD_DUNGEON_LEVEL = 15,
	INPUT_BUFFER_16 = 16,
	INPUT_BUFFER_WORLD_DUNGEON_LAYER_COUNT = 16,
	INPUT_BUFFER_CHARACTER_JUMIN = 16,
	INPUT_BUFFER_17 = 17,
	INPUT_BUFFER_WORLD_DUNGEON_GUILD = 17,
	INPUT_BUFFER_18 = 18,
	INPUT_BUFFER_WORLD_DUNGEON_MANAGER2 = 18,
	INPUT_BUFFER_19 = 19,
	INPUT_BUFFER_WORLD_DUNGEON_ITEM	= 19,
	INPUT_BUFFER_20 = 20,
	INPUT_BUFFER_PARTY_ENTRY = 20,
};

struct INPUT_BUFFER
{
	DWORD	dwColor;	

	BYTE	bFontIndex;			//Font �迭 ��ȣ 
	RECT	Pos;				//ȭ��� Display�� ��ǥ 
	DWORD	dwInputIndex;		//���� InputBuffer Index
	BYTE	bLine;				//���� ����
	BYTE	bPassword;			//�н������ ���ϰ��� *�� ���� 
	char	szBuffer[ MAX_INPUT_BUFFER_SIZE ];	//Buffer

	int		nOrder;

	char*	GetCurPos()	{	return &szBuffer[ dwInputIndex ];	}

	BOOL PtInRect(LONG x, LONG y)
	{
		if(x >= Pos.left && x <= Pos.right && y >= Pos.top && y <= Pos.bottom)
			return TRUE;
		else
			return FALSE;
	};
};

class CInputManager
{
	BOOL			m_bDrawCursor;					// ���� �������� Ŀ�� �������� ����!! by deepdark.
	DWORD			m_dwCurFocusInputID;			// ���� ĳ���� ���̵�
	INPUT_BUFFER	m_InputBuffer[MAX_INPUT_BUFFER_NUM];
	
	

public:
	CInputManager();
	~CInputManager();

	void			Initialize();
	void			InitializeInput(DWORD dwInputID, BYTE bFontID, RECT& rt, DWORD dwColor, int nOrder, BYTE bPassword = FALSE, BOOL bChk = TRUE);
	DWORD			GetCurFocusInputID();					// ���� ��ǲ���� ���̵� ����
	DWORD			GetInputBufferLength(DWORD dwInputID);	//	�ش� ��ǲ���ۿ� ä���� ���ڼ�
	void			InsertCharacter(DWORD dwInputID, char* szBuf, DWORD dwLen);			// ���� ��ǲ���� �������� ���� ��ŭ ī�� �Ѵ�.
	
	void			SetCharacter(DWORD dwInputID, DWORD dwLen);

	void			RenderInputBuffer(DWORD dwInputID);
	void			RenderInputBufferAll();				// ��ǲ���ۿ� �ִ� ������ ����϶�.
	void			BlinkInputCaret();					// ���� ĳ���� �����Ÿ���.
	void			SetFocusInput(DWORD dwInputID);								// ���� ��ǲ���۸� ���Ѵ�.
	void			ClearInput(DWORD dwInputID);								// ��ǲ ���۸� �����.
	void			SetInputColor(DWORD dwInputID, DWORD dwColor);				// ��ǲ ������ ���� ���Ѵ�.
	void			RenderConvertToSpecialCharacter(DWORD dwInputID, char c);	// Ư���� ���ڷ� ���۱��� ��ŭ ä���
	char*			GetInputBuffer(DWORD dwInputID, BOOL bChk = FALSE);			// ĳ���� ���� ���� ���ڿ��� ���´�.
	BOOL			PtInRect(DWORD dwInputID, long x, long y);
	void			BackSpaceEvent();
	int				GetInputOrder(DWORD dwInputID);
	void			SetInputOrder(DWORD dwInputID, int nOrder);
	RECT&			GetInputRect(DWORD dwInputID);
	BOOL			IsDrawCursor(); // added by deepdark

	void			SetInput(DWORD dwInputID, char* pszBuffer);
	
};
