#pragma once

#include "DungeonPacket.h"
#include "CommonServerExports.h"
#include "CommonClientDungeon.h"

COMMONSERVERDECLSPECIFIER const char* codeToString(USER_PROPERTY_CODE code);
COMMONSERVERDECLSPECIFIER void printUserStatusList(const USER_STATUS* statusList, int count);