#pragma once
#include "InitGame.h"

struct Rect {
	VECTOR2 origin;
	VECTOR2 size;
};

extern VECTOR2 hudLeft;
extern VECTOR2 hudRight;
extern VECTOR2 hp;
extern VECTOR2 sp;
extern VECTOR2 expPos;
extern VECTOR2 cool;
extern VECTOR2 beltItemsStart;

extern VECTOR2 btnItem;
extern VECTOR2 btnStats;
extern VECTOR2 btnSkill;
extern VECTOR2 btnGroup;
extern VECTOR2 btnLair;
extern VECTOR2 btnChat;
extern VECTOR2 btnOptions;
extern VECTOR2 btnTrade;
extern VECTOR2 btnShop;
extern VECTOR2 btnRest;
extern VECTOR2 btnMatch;
extern VECTOR2 btnPK;

extern VECTOR2 monsterHP1;
extern VECTOR2 monsterHP2;
extern VECTOR2 monsterGage;

extern VECTOR2 skillSelectionBottomRowStart;
extern VECTOR2 leftSkillPos;
extern VECTOR2 rightSkillPos;
extern VECTOR2 guardianSkillPos;

extern Rect guardianInterface1;
extern VECTOR2 guardianPotionPos;

extern Rect leftHUD;
extern Rect rightHUD;

#define SKILL_ICON_SIZE					32
#define SKILL_SELECTION_ROW_COUNT		5

VECTOR2 beltItemPos(int index);
int	beltItemIndexAt(VECTOR2 pos);

bool pointInsideRect(VECTOR2 point, VECTOR2 rectTopLeft, int w = 50, int h = 14);
bool pointInsideRect(VECTOR2 point, Rect rect);
bool pointInsideSquare(VECTOR2 point, VECTOR2 topLeft, int size = SKILL_ICON_SIZE);
