#pragma once

#define NEW_COMMAND


// Windows Header Files:
#include <windows.h>
#include <windowsx.h>
#include <ole2.h>
#include <initguid.h>
#include <mmsystem.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <basetsd.h>
#include <ddraw.h>
#include <dmusici.h>
#include <TIME.H>
#include <ASSERT.H>

#include "StringFunc.h"
#include "oledb.h"
#include "../CommonServer/LoginPacket.h"
#include "../CommonServer/CharSelectPacket.h"
#include "../CommonServer/DungeonPacket.h"
#include "../CommonServer/GamePacket.h"
#include "../CommonServer/GMPacket.h"
#include "../CommonServer/RivalGuildWarPacket.h"
#include "../CommonServer/WorldPacket.h"
#include "../CommonServer/PartyMatchPacket.h"


#define FILE(id)		g_Res.GetResFileNameOnly(##id)	
#define SPR(id)			g_pSprManager->GetSprite(##id)
#define	RELEASE_SPR(id)	g_pSprManager->RemoveSprite(##id)
#define DIRECTINPUT_VERSION 0x0800


#pragma warning(disable : 4706)		// �񱳱��� ������ ���� �� �Ҵ� ���.
#pragma warning(disable : 4505)		// ������ ���� static �Լ� ���� ���
#pragma warning(disable : 4710)		// ������ ���� ��, inline ���� ���� ���
#pragma warning(disable : 4100)		// ������ ���� �Լ� ���� ���

// �Ʒ� ������ ������ ���ֶ�... �Ǽ��ϸ� ��� ���� ���ɼ��� �׻� �����Ѵ�....!!!
#pragma warning(disable : 4244)		// Ÿ�� ����ġ ���
#pragma warning(disable : 4018)		// �� ���� ��ȣ ����ġ ���
//#pragma warning(disable : 4245)		// ���Թ� ��ȣ ����ġ ���


#include <mmsystem.h>
#pragma comment (lib, "Winmm.lib")
