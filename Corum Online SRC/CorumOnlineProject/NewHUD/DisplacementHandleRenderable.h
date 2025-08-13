#pragma once

#include "../NewUI/SpriteRenderable.h"

namespace NewInterface {
	class DisplacementHandleRenderable: public CustomUI::Renderable {
	public:
		using DisplacementHandler = std::function<void(int deltaX, int deltaY)>;
		DisplacementHandleRenderable(CustomUI::Rect frameInParent);

		void updateSprite(CustomUI::SpriteModel sprite);
		void onDisplacement(DisplacementHandler handler);

	protected:
		void onMouseStateChange(
			CustomUI::Renderable::MouseState newState, 
			CustomUI::Renderable::MouseState oldState,
			CustomUI::Point atMouseGlobalPoint
		) override;
		void onMouseMove(CustomUI::Point mouseGlobalOrigin) override;
	private:
		DisplacementHandler _handler;
		CustomUI::SpriteRenderable* _spriteRenderable;
		CustomUI::Point _lastGlobalPosition;
		std::function<void(CustomUI::Point)> _onMouseMove;
	};
}

