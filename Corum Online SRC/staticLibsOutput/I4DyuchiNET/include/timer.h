#ifndef TIMER_H
#define TIMER_H

#include "stdafx.h"

void InitializeTimer();
DWORD AddTimer(DWORD dwPerioedTime,HANDLE hEvent,DWORD dwTimerIndex);
BOOL RemoveTimer(DWORD dwTimerIndex);
void RemoveAllTimer();
BOOL PauseTimer(DWORD dwTimerIndex);
BOOL ResumeTimer(DWORD dwTimerIndex);

#endif
