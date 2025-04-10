#pragma once

#include "DungeonPacket.h"
#include "CommonServerExports.h"
#include "CommonClientDungeon.h"

DECLSPECIFIER const char* codeToString(USER_PROPERTY_CODE code);
DECLSPECIFIER void printUserStatusList(const USER_STATUS* statusList, int count);