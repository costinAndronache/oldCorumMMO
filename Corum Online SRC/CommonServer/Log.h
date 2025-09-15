#ifndef __LOG_H__
#define __LOG_H__


#pragma once


#include "CommonServer.h"


#define MAX_LOG_LENGTH		512
#define NUM_OF_LOG_TYPE		7


#define LOG_FATAL			0
#define LOG_IMPORTANT		1
#define LOG_NORMAL			2
#define LOG_IGNORE			3
#define LOG_DEBUG			4
#define LOG_ALL				5
#define	LOG_JUST_FILEWRITE	6
#define LOG_JUST_DISPLAY	7
#define LOG_JUST_DISPLAY1	8


void COMMONSERVERDECLSPECIFIER InitLog();
void COMMONSERVERDECLSPECIFIER FreeLog();
void COMMONSERVERDECLSPECIFIER GetConsoleKeyInput(PINPUT_RECORD pInput, LPDWORD pResult);
void COMMONSERVERDECLSPECIFIER Log(int type, char *logmsg, ...);
void COMMONSERVERDECLSPECIFIER WriteText(char* msg, bool type = true);


#endif 