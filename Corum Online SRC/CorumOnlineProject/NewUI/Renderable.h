#pragma once
#include "CustomUIBase.h"

namespace CustomUI {
	class Renderable {
	public:
		enum class MouseState { hovering, pressedInside, none };

		virtual void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) = 0;

		void handleMouseDown(Point mouse);
		void handleMouseUp(Point mouse);
		void handleMouseMove(Point mouse);
		void setHidden(bool isHidden) { _isHidden = isHidden; };
		bool getHidden() const { return _isHidden; }

		bool swallowsMouse(Point mouse);
	protected:
		Renderable() {
			_currentMouseState = MouseState::none;
			_isHidden = false;
		}

		Rect _frame;
		MouseState _currentMouseState;

		virtual void onMouseStateChange(MouseState newState, MouseState oldState) {}

		template<typename T>
		T* registerChildRenderable(std::function<T*()> creatingFn) {
			T* result = creatingFn();
			_childRenderables.push_back(result);
			return result;
		}

	private:
		bool _isHidden;
		std::vector<Renderable*> _childRenderables;
		void updateMouseState(MouseState newState);

	};

}

