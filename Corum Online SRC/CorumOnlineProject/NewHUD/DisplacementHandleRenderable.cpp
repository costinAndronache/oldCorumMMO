#include "DisplacementHandleRenderable.h"
#include "HUDSpriteCollection.h"

using namespace CustomUI;
using namespace NewInterface;

DisplacementHandleRenderable::DisplacementHandleRenderable(CustomUI::Rect frameInParent) {
	_frameInParent = frameInParent;
	_spriteRenderable = registerChildRenderable<SpriteRenderable>([=](){
		return std::make_shared<SpriteRenderable>(bounds(), NewHUDResources::displacementIcon);
	});

	_handler = nullptr;
	_onMouseMove = nullptr;
}

void DisplacementHandleRenderable::updateSprite(CustomUI::SpriteModel sprite) {
	_spriteRenderable->updateSprite(sprite);
}


void DisplacementHandleRenderable::onDisplacement(DisplacementHandler handler) {
	_handler = handler;
}

void DisplacementHandleRenderable::onMouseStateChange(MouseState newState, MouseState oldState, Point atMouseGlobalPoint) {
	if(newState == Renderable::MouseState::leftButtonPressedInside && _handler) {
		_lastGlobalPosition = atMouseGlobalPoint;
		_onMouseMove = [=](Point mouseGlobalPos) {
			const auto dx = mouseGlobalPos.x - _lastGlobalPosition.x;
			const auto dy = mouseGlobalPos.y - _lastGlobalPosition.y;
            _lastGlobalPosition = mouseGlobalPos;

			_handler(dx, dy);
		};
	} else {
		_onMouseMove = nullptr;
	}
}

void DisplacementHandleRenderable::onMouseMove(Point mouseGlobalOrigin) {
	if(_onMouseMove) { _onMouseMove(mouseGlobalOrigin); }
}