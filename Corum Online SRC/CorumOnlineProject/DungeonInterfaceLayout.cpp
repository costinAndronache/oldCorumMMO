#include "DungeonInterfaceLayout.h"

VECTOR2 hudLeft = { 0, 640 };
VECTOR2 hudRight = { 512, 640 };
VECTOR2 hp = { 0, 709 };
VECTOR2 sp = { 723, 709 };
VECTOR2 expPos = { 0, 725 };
VECTOR2 cool = { 723, 725 };
VECTOR2 beltItemsStart{ 727, 733 };

VECTOR2 btnItem = { 83, 684 };
VECTOR2 btnStats{ 164, 684 };
VECTOR2 btnSkill{ 243, 684 };
VECTOR2 btnGroup{ 747, 684 };
VECTOR2 btnLair{ 827, 684 };
VECTOR2 btnChat{ 907, 684 };
VECTOR2 btnOptions{ 659, 731 };
VECTOR2 btnTrade{ 0, 661 };
VECTOR2 btnShop{ 0, 679 };
VECTOR2 btnRest{ 977, 661 };
VECTOR2 btnMatch{ 977, 679 };
VECTOR2 btnPK{ 333, 731 };

VECTOR2 monsterHP1{ 401, 0, };
VECTOR2 monsterHP2{ 598, 0, };
VECTOR2 monsterGage{ 412, 3 };

VECTOR2 skillSelectionBottomRowStart{ 116, 640 };
VECTOR2 leftSkillPos{ 133, 733 };
VECTOR2 rightSkillPos{ 248, 733 };
VECTOR2 guardianSkillPos{ 209, 733 };

Rect guardianInterface1{ { 0, 50 }, { 60, 20 } };
VECTOR2 guardianPotionPos{ 93, 76 };

Rect leftHUD{ {0, 660 }, {370, 108} };
Rect rightHUD{ {653, 660 }, {370, 108} };


const auto beltItemSize = 30;
const auto beltItemSpacing = 7;

VECTOR2 beltItemPos(int index) {
	return { 
		(float)beltItemsStart.x + (index * beltItemSize + index * beltItemSpacing), 
		beltItemsStart.y 
	};
}

int	beltItemIndexAt(VECTOR2 pos) {
	const auto maxIndex = 7;
	const auto totalWidth = maxIndex * beltItemSize + maxIndex * beltItemSpacing;
	if (!(beltItemsStart.x <= pos.x && pos.x <= beltItemsStart.x + totalWidth)) {
		return -1;
	}

	return (pos.x - beltItemsStart.x) / (beltItemSize + beltItemSpacing);

}

bool pointInsideRect(VECTOR2 point, VECTOR2 rectTopLeft, int w, int h) {
	const auto x = point.x;
	const auto y = point.y;
	return (x >= rectTopLeft.x && x <= rectTopLeft.x + w) && (y >= rectTopLeft.y && y <= rectTopLeft.y + h);
}

bool pointInsideRect(VECTOR2 point, Rect rect) {
	return pointInsideRect(point, rect.origin, (int)rect.size.x, (int)rect.size.y);
}

bool pointInsideSquare(VECTOR2 point, VECTOR2 topLeft, int size) {
	const auto x = g_Mouse.MousePos.x;
	const auto y = g_Mouse.MousePos.y;
	return (x >= topLeft.x && x <= topLeft.x + size) && (y >= topLeft.y && y <= topLeft.y + size);
}