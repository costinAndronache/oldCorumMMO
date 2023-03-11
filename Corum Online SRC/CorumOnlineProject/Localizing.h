#ifndef __LOCALIZING_H__
#define __LOCALIZING_H__

#pragma once

// ���� deepdark�� ���� �ۼ�/������.----------------------------------------------- 

#define __ENGLISH_MODE			0	// ������
#define __THAILAND_MODE			11	// �±�����
#define __KOREAN_MODE			101	// �ѱ���, 2����Ʈ ���� 100�� �̻����� ����.
#define __JAPANESE_MODE			102	// �Ϻ���
#define __CHINESE_MODE			103	// �߱���
#define __TAIWAN_MODE			104	// �븸��

#define __INTERNATIONAL_MODE	200	// ���ͳ��ų�


#ifdef			JAPAN_LOCALIZING	
#	define __LOCALIZING_MODE		__JAPANESE_MODE
#elif defined	CHINA_LOCALIZING	
#	define __LOCALIZING_MODE		__CHINESE_MODE
#elif defined	TAIWAN_LOCALIZING	
#	define __LOCALIZING_MODE		__TAIWAN_MODE
#elif defined	ENGLISH_LOCALIZNG
#	define __LOCALIZING_MODE		__ENGLISH_MODE
#elif defined	THAILAND_LOCALIZING
#	define __LOCALIZING_MODE		__THAILAND_MODE
#else 	
#	define __LOCALIZING_MODE		__KOREAN_MODE	// �⺻������ �ѱ�����!
#endif
 
// IMEEdit�� ����� ������ üũ�ؼ� �����Ѵ�.
#if (__LOCALIZING_MODE > 100)	//	__KOREAN_MODE)
#	define _USE_IME
#endif

#define IS_IME_VERSION()		(__LOCALIZING_MODE > 100)

#define IS_ENGLISH_LOCALIZING()	(__LOCALIZING_MODE == __ENGLISH_MODE)
#define IS_THAILAND_LOCALIZING()(__LOCALIZING_MODE == __THAILAND_MODE)
#define IS_KOREA_LOCALIZING()	(__LOCALIZING_MODE == __KOREAN_MODE)
#define IS_JAPAN_LOCALIZING()	(__LOCALIZING_MODE == __JAPANESE_MODE)
#define IS_CHINA_LOCALIZING()	(__LOCALIZING_MODE == __CHINESE_MODE)
#define IS_TAIWAN_LOCALIZING()	(__LOCALIZING_MODE == __TAIWAN_MODE)

#define IS_INTERNATIONAL()		(__LOCALIZING_MODE == __INTERNATIONAL_MODE)

// ---------------------------------------------------------------------------------

#endif // __LOCALIZING_H__