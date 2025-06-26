#include "DungeonInterfaceLayout.h"

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

void buildInterfaceLayoutPositions(int width, int height) {
	const VECTOR2 hudSIZE = { 370, 127 };

	leftHUD = { {0, height - hudSIZE.y },  hudSIZE };
	// LEFT HUD
	hp = { leftHUD.origin.x, leftHUD.origin.y + 69 };
	btnItem = { leftHUD.origin.x + 83, leftHUD.origin.y + 44 }; // 684
	btnStats = { leftHUD.origin.x + 164, leftHUD.origin.y + 44 };
	btnSkill = { leftHUD.origin.x + 243, leftHUD.origin.y + 44 };
	btnTrade = { leftHUD.origin.x, leftHUD.origin.y + 20 }; // 661
	btnShop = { leftHUD.origin.x, leftHUD.origin.y + 40 }; // 679
	btnPK = { leftHUD.origin.x + 333, leftHUD.origin.y + 90 }; // 731
	expPos = { leftHUD.origin.x, leftHUD.origin.y + 85 }; // 725

	skillSelectionBottomRowStart = { leftHUD.origin.x + 116, leftHUD.origin.y };
	leftSkillPos = { leftHUD.origin.x + 133, leftHUD.origin.y + 93 }; // 733
	rightSkillPos = { leftHUD.origin.x + 248, leftHUD.origin.y + 93 };
	guardianSkillPos = { leftHUD.origin.x + 209, leftHUD.origin.y + 93 };


	const auto hudY = 640;

	rightHUD = { {0, hudY }, hudSIZE };
	// RIGHT HUD
	sp = { rightHUD.origin.x + 70, rightHUD.origin.y + 69 };
	cool = { 723, rightHUD.origin.y + 85 };
	beltItemsStart = { 727, rightHUD.origin.y + 93 };
	btnGroup = { 747, rightHUD.origin.y + 44 };
	btnLair = { 827, rightHUD.origin.y + 44 };
	btnChat = { 907, rightHUD.origin.y + 44 };
	btnOptions = { 659, rightHUD.origin.y + 90 };
	btnRest = { 977, rightHUD.origin.y + 20 };
	btnMatch = { 977, rightHUD.origin.y + 40 };
	
	// MISC

	monsterHP1 = { 401, 0, };
	monsterHP2 = { 598, 0, };
	monsterGage = { 412, 3 };

	guardianInterface1 = { { 0, 50 }, { 60, 20 } };
	guardianPotionPos = { 93, 76 };
}