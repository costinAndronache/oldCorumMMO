// Struct Effect Info....
#pragma once

#include "../CommonServer/CommonClientDungeon.h"

#define MAX_EFFECT_INFO			200
#define MAX_EFFECT_SOUND_NUM	200
#define MAX_PLAYER_SOUND_NUM	200	
//#define MAX_MONSTER_SOUND_NUM	6
#define MAX_GAMEPLAY_SOUND_NUM  200 

#define EFFECT_TYPE_DAMAGE			11
#define EFFECT_TYPE_STATUS			20		// 분기점..
#define EFFECT_TYPE_NORMAL			21		// 한번 나오고 끝나며, 목적오브켁트의 스폿을 따라다닌다.		

//--- 몬스터 사운 드 시작 번호 ---------//
#define MONSTER_SOUND_NEUTRAL		0
#define MONSTER_SOUND_TARGET		1
#define MONSTER_SOUND_GETHIT		2
#define MONSTER_SOUND_ATTACK		4
#define MONSTER_SOUND_DEATH			6
#define MONSTER_SOUND_MOVE			8


//--- Game Play 에서의 사운트 시작번호 -//
#define GAMEPLAY_HEABYSTONE_WALK	5
#define GAMEPLAY_HEABYSTONE_RUN		1
#define GAMEPLAY_AIRPLANE_LANDING	102
#define GAMEPLAY_AIRPLANE_TAKE_OFF	101
