#include "BaloonChat.h"

//------------------------------------------------------------------------
// Date			: 2004. 10. 05. ( Written by minjin )
// Description	: ��ǳ�� ��ġ ������ ����
//------------------------------------------------------------------------
SHORT g_usBaloonChatWndResInfo[10][5] = 
{
	//														   Adjustments
	//	Sprite Index,							x		y		x		y
	{	SPR_INTERFACE_BALOON_CHAT_WND_LTOP,		0,		0,		-8,		-8	},
	{	SPR_INTERFACE_BALOON_CHAT_WND_CTOP,		8,		0,		0,		-8	},
	{	SPR_INTERFACE_BALOON_CHAT_WND_RTOP,		9,		0,		0,		-8	},
	{	SPR_INTERFACE_BALOON_CHAT_WND_LMID,		0,		8,		-8,		0	},
	{	SPR_INTERFACE_BALOON_CHAT_WND_CMID,		8,		8,		0,		0	},
	{	SPR_INTERFACE_BALOON_CHAT_WND_RMID,		9,		8,		0,		0	},
	{	SPR_INTERFACE_BALOON_CHAT_WND_LBOT,		0,		9,		-8,		0	},
	{	SPR_INTERFACE_BALOON_CHAT_WND_CBOT,		8,		9,		0,		0	},
	{	SPR_INTERFACE_BALOON_CHAT_WND_RBOT,		9,		9,		0,		0	},
	{	SPR_INTERFACE_BALOON_CHAT_WND_TAIL,		17,		0,		70,		8	}		
};

//------------------------------------------------------------------------
// Function		: __inline GetRegisteredSprite
// Date			: 2004. 10. 05. ( Written by minjin )
// Description	: �������̽� ��������Ʈ �ؽ�����
//				  Ű������ ��������Ʈ ������Ʈ ������ ������
//------------------------------------------------------------------------
__inline IDISpriteObject* GetRegisteredSprite( DWORD dwKey )
{
	return g_pInterfaceSprHash->GetData(dwKey)->pSpr;
}

//------------------------------------------------------------------------
// Function		: RenderBaloonChatBox
// Date			: 2004. 10. 05. ( Written by minjin )
// Description	: �� ����, ��ǳ�� ������� ȭ�鿡 ǥ��
//------------------------------------------------------------------------
void RenderBaloonChatBox( VECTOR2* pV2Scailing, VECTOR2* pV2Position, int nIndex)
{
	VECTOR2 vScale;
	VECTOR2 vPosition;

	// �迭�� ����ִ� ��������Ʈ ���� ������ ��´�
	for ( int i = 0; i < 10; i++ )
	{
		// ó���� ��ġ �������� �ϰ�.. (������ ��� �ڽ� ���� �������..)
		vPosition.x = pV2Position->x + g_usBaloonChatWndResInfo[i][RESOURCE_X_ADJ];
		vPosition.y = pV2Position->y + g_usBaloonChatWndResInfo[i][RESOURCE_Y_ADJ];

		// �����ϸ� �ʿ��� ��������Ʈ �����ϸ��ϱ�

		vScale.x = 1.0f;		// �⺻ �����ϸ� ���� �ϰ�,
		vScale.y = 1.0f;

		switch ( g_usBaloonChatWndResInfo[i][RESOURCE_INDEX] )
		{
		case SPR_INTERFACE_BALOON_CHAT_WND_CMID:	// ����� �ִ°� (���μ��� �� ���δ�)
			vScale.x = pV2Scailing->x;
			vScale.y = pV2Scailing->y;
			break;

		case SPR_INTERFACE_BALOON_CHAT_WND_CTOP:	// ���� ���.. (���η� ���δ�)
			vScale.x = pV2Scailing->x;
			break;

		case SPR_INTERFACE_BALOON_CHAT_WND_CBOT:	// �Ʒ��� ���.. (���η� ���δ�)
			vScale.x = pV2Scailing->x;
			break;

		case SPR_INTERFACE_BALOON_CHAT_WND_LMID:	// ���� �߰�.. (���η� ���δ�)
			vScale.y = pV2Scailing->y;
			break;

		case SPR_INTERFACE_BALOON_CHAT_WND_RMID:	// ������ �߰�.. (���η� ���δ�)
			vScale.y = pV2Scailing->y;
			break;

		default:
			break;
		}

		// �����ϸ� �����´��, ��ġ ����
		switch ( g_usBaloonChatWndResInfo[i][RESOURCE_INDEX] )
		{
		case SPR_INTERFACE_BALOON_CHAT_WND_RTOP:
			vPosition.x += pV2Scailing->x;
			break;

		case SPR_INTERFACE_BALOON_CHAT_WND_RMID:
			vPosition.x += pV2Scailing->x;
			break;

		case SPR_INTERFACE_BALOON_CHAT_WND_LBOT:
			vPosition.y += pV2Scailing->y;
			break;

		case SPR_INTERFACE_BALOON_CHAT_WND_CBOT:
			vPosition.y += pV2Scailing->y;
			break;

		case SPR_INTERFACE_BALOON_CHAT_WND_RBOT:
			vPosition.x += pV2Scailing->x;
			vPosition.y += pV2Scailing->y;
			break;

		case SPR_INTERFACE_BALOON_CHAT_WND_TAIL:
			vPosition.y += pV2Scailing->y;
			break;

		default:
			break;
		}

		g_pRenderer->RenderSprite( GetRegisteredSprite( g_usBaloonChatWndResInfo[i][RESOURCE_INDEX] )
			, &vScale, 0.0, &vPosition, NULL, 0xb0ffffff, nIndex, 0);
	}
}
