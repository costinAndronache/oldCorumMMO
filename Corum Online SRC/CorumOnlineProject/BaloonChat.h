//#############################################################################
// CPP			: BaloonChat.cpp
// Header		: BaloonChat.h
// Date			: 2004. 10. 05. ( Written by minjin )
// Description	: ��ǳ�� �׸��� ���� �Լ�
//#############################################################################

#ifndef __BALOON_CHAT_WND_H__
#define __BALOON_CHAT_WND_H__

#include "InitGame.h"
#include "InterfaceSpr.h"
#include "InterfaceSprDef.h"

#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4DyuchiGRX_common/IExecutive.h"
#else
#include "./../4DyuchiGRX_common/IExecutive.h"
#endif


// ä�ÿ� ��ǳ�� �׸��� �Լ� (minjin041002)
void RenderBaloonChatBox( VECTOR2* pV2Scailing, VECTOR2* pV2Position, int nIndex );


// ��������Ʈ ������ �������³� (minjin041002)
__inline IDISpriteObject* GetRegisteredSprite( DWORD dwKey );


// ä��ǳ�� ���ҽ� ������ ���� �迭 (minjin041002)
extern SHORT g_usBaloonChatWndResInfo[10][5];


// �迭 �ε��� ������ Define
#define RESOURCE_INDEX	0
#define RESOURCE_X_POS	1
#define RESOURCE_Y_POS	2
#define RESOURCE_X_ADJ	3
#define RESOURCE_Y_ADJ	4


#endif	// __BALOON_CHAT_WND_H__
