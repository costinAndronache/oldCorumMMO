#pragma once

#include "Renderable.h"

namespace CustomUI {
	class MouseTrackingSpriteRenderable: public Renderable {
	public:
		MouseTrackingSpriteRenderable(Rect frameInParent) {
			_frameInParent = frameInParent;
			_mouseAvatar = nullptr;
		};

		void trackWithNewRenderable(
			Renderable* newAvatar, 
			std::function<void(Rect)> onMouseLeftButtonUp
		) {
			deconstructAllChildren();
			_mouseAvatar = newAvatar;
			_onMouseLeftButtonUp = onMouseLeftButtonUp;

			if (newAvatar == nullptr) { return;  }

			registerChildRenderable<Renderable>([=]() { return  newAvatar;});
		}

		bool isCurrentlyTracking() { return _mouseAvatar != nullptr; }
		virtual bool swallowsMouse(Point) override { return false; }

	protected:
		void onMouseMove(Point mouseGlobalOrigin) override {
			if (_mouseAvatar) {
				_mouseAvatar->updateFrameInParent(
					_mouseAvatar->globalFrame().centeredOnPoint(mouseGlobalOrigin)
				);
			}
		}

		void handleMouseUp(Point mouseGlobalOrigin, MouseButton button) override {
			if (button == MouseButton::left && _onMouseLeftButtonUp && _mouseAvatar) {
				_onMouseLeftButtonUp(_mouseAvatar->globalFrame());
			}
		}

	private:
		std::function<void(Rect avatarCurrentFrame)> _onMouseLeftButtonUp;
		Renderable* _mouseAvatar;
	};
}

