#pragma once 

#include "stdafx.h"
#include "typedef.h"

BOOL StartConnectThread(DWORD dwMaxConnectNumAtSameTime);
BOOL Connect(char* szIP,WORD wPort,CONNECTSUCCESSFUNC SuccessFunc,CONNECTFAILFUNC FailFunc,CNetwork* pNetwork,void* pExt);
void EndConnectThread();
