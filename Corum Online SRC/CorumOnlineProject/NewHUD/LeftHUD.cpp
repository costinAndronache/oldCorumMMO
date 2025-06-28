#include "LeftHUD.h"
#include "../InterfaceSpr.h"
#include "../InterfaceSprDef.h"
#include "HUDSpriteCollection.h"

using namespace CustomUI;

LeftHUD* LeftHUD::shared() {
	static LeftHUD* ptr = nullptr;
	if (!ptr) { ptr = new LeftHUD({ {1620, 900}, {300, 15} }); }
	return ptr;
}

LeftHUD::LeftHUD(Rect frame): 
	_frame(frame),
	_hpBar(HorizontalScalingBar(
		HUDSpriteCollection::spBarSprite, 
		frame, 
		HorizontalScalingBar::Direction::rightLeft)) {
}

void LeftHUD::renderWithRenderer(I4DyuchiGXRenderer* renderer) {
	_hpBar.renderWithRenderer(renderer);
}

bool LeftHUD::swallowsMouse(MouseState mouse) {
	return false;
}