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
	/*
	* IMPORTANT:: these positions are defined in terms of the width and height
	* of interface_left.tif / interface_right.tif in the UI.pak file
	*/
	const VECTOR2 hudSIZE = { 400, 121 };

	leftHUD = { {0, height - hudSIZE.y - 3 },  hudSIZE };
	// LEFT HUD
	hp = { leftHUD.origin.x, leftHUD.origin.y + 64 };
	btnItem = { leftHUD.origin.x + 83, leftHUD.origin.y + 40 };
	btnStats = { leftHUD.origin.x + 164, leftHUD.origin.y + 40 };
	btnSkill = { leftHUD.origin.x + 243, leftHUD.origin.y + 40 };
	btnTrade = { leftHUD.origin.x, leftHUD.origin.y + 15 };
	btnShop = { leftHUD.origin.x, leftHUD.origin.y + 36 };
	btnPK = { leftHUD.origin.x + 333, leftHUD.origin.y + 85 };
	expPos = { leftHUD.origin.x, leftHUD.origin.y + 80 };

	skillSelectionBottomRowStart = { leftHUD.origin.x + 116, leftHUD.origin.y };
	leftSkillPos = { leftHUD.origin.x + 133, leftHUD.origin.y + 86 };
	rightSkillPos = { leftHUD.origin.x + 248, leftHUD.origin.y + 86 };
	guardianSkillPos = { leftHUD.origin.x + 209, leftHUD.origin.y + 88 };

	rightHUD = { {width - hudSIZE.x, height - hudSIZE.y - 3 },  hudSIZE };
	// RIGHT HUD
	sp = { rightHUD.origin.x + 98, rightHUD.origin.y + 60 };
	cool = { rightHUD.origin.x + 98, rightHUD.origin.y + 74 };
	beltItemsStart = { rightHUD.origin.x + 102, rightHUD.origin.y + 84 };
	btnGroup = { rightHUD.origin.x + 113, rightHUD.origin.y + 36 };
	btnLair = { rightHUD.origin.x + 188, rightHUD.origin.y + 36 };
	btnChat = { rightHUD.origin.x + 272, rightHUD.origin.y + 36 };
	btnOptions = { rightHUD.origin.x + 34, rightHUD.origin.y + 83 };
	btnRest = { rightHUD.origin.x + 348, rightHUD.origin.y + 14 };
	btnMatch = { rightHUD.origin.x + 348, rightHUD.origin.y + 32 };;
	
	// MISC

	monsterHP1 = { 401, 0, };
	monsterHP2 = { 598, 0, };
	monsterGage = { 412, 3 };

	guardianInterface1 = { { 0, 50 }, { 60, 20 } };
	guardianPotionPos = { 93, 76 };
}