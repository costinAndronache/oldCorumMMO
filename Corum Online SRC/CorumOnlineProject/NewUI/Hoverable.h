#pragma once

#include "Renderable.h"

namespace CustomUI {
	class Hoverable: public Renderable {
	public:
		using OnHovering = std::function<void(CustomUI::Point globalMousePoint)>;
		using OnHoveringEnd = std::function<void()>;
		Hoverable();
		void onHover(
			OnHovering onHovering,
			OnHoveringEnd onHoveringEnd
		);
	protected:
		void onMouseStateChange(
			CustomUI::Renderable::MouseState newState,
			CustomUI::Renderable::MouseState oldState
		) override;
		void onMouseMove(CustomUI::Point mouseGlobalOrigin) override;
	private:
		std::function<void(CustomUI::Point)> _onMouseMove;
		std::function<void(CustomUI::Renderable::MouseState)> _onMouseStateChange;
	};

}


