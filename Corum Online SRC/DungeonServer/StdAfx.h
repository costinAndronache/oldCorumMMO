#if !defined(AFX_STDAFX_H__71751A48_D7EF_4A0E_814A_24507D90D4D1__INCLUDED_)
#define AFX_STDAFX_H__71751A48_D7EF_4A0E_814A_24507D90D4D1__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 
#define WIN32_LEAN_AND_MEAN//windwos ��� �־ ���� ������

#define _WIN32_WINNT 0x0500


#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <ASSERT.H>
#include <CRTDBG.H>
#include "GlobalDefine.h"
#include "TrafficLog.h"


#pragma comment(lib, "SS3DGFunc.lib")
#pragma comment(lib, "CommonServer.lib")


#pragma warning(disable : 4100) // ������ ���� �Լ� ���� ���
#pragma warning(disable : 4706)	// �� ���� ������ �� ���� ���
#pragma warning(disable : 4710)	// ������ ���� ��, inline ���� ���� ���
#pragma warning(disable : 4702) // �����Ҽ� ���� �ڵ� ����
// �Ʒ� ������ ������ ���ֶ�... �Ǽ��ϸ� ��� ���� ���ɼ��� �׻� �����Ѵ�....!!!
#pragma warning(disable : 4244)	// Ÿ�� ����ġ ���... ���� disable ���� ���� ������ Ȯ���ϰ� ĳ��Ʈ�ض�!!
#pragma warning(disable : 4127)

#include "../4DyuchiGXGFunc/global.h"
#include "../CommonServer/CommonAccessory.h"
#include "../CommonServer/ComInterface.h"
#include "../CommonServer/GamePacket.h"
#include "../CommonServer/DungeonPacket.h"
#include "../CommonServer/ServerPacket.h"
#include "../CommonServer/RivalGuildWarPacket.h"
#include "../CommonServer/GMPacket.h"
#include "../CommonServer/NTNetwork.h"

#include <mmsystem.h>
#pragma comment (lib, "Winmm.lib")

#ifdef _DEBUG
#	define CHECK_MEMORY()			if( !_CrtCheckMemory( ) ) __asm int 3
#else
#	define CHECK_MEMORY()
#endif


#endif 





