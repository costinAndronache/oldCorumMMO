#ifndef __PROFILE_H__
#define __PROFILE_H__

#pragma once


#define USE_FUNCTION_PROFILE					// �Լ� ���� �ð� üũ�� ���ؼ��� �����Ѵ�.
#define FUNCTION_PROCESS_TIME_LIMIT		10*1000	// 10 SEC.


#ifdef USE_FUNCTION_PROFILE

#define START_FUNCTION_PROFILE() \
	DWORD dwStartProfile___ = GetTickCount(); 


#define END_FUNCTION_PROFILE() \
	DWORD dwEndProfile___ = GetTickCount();\
	if(dwEndProfile___ - dwStartProfile___ > FUNCTION_PROCESS_TIME_LIMIT) \
	{ \
		char szLogMsg[0xff] ={0,}; \
		sprintf(szLogMsg, "Function Takes over limited time (%s %d lines)", __FILE__, __LINE__);\
	    Log(LOG_IMPORTANT, szLogMsg);\
	}
#else

#define START_FUNCTION_PROFILE()
#define END_FUNCTION_PROFILE()

#endif 


#endif 