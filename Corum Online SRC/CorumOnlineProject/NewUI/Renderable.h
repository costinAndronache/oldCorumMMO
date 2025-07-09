#pragma once
#include "CustomUIBase.h"

#define __ownedByRenderable 
namespace CustomUI {
	class Renderable {
	public:
		enum class MouseButton { left, right};
		enum class MouseState { hovering, leftButtonPressedInside, rightButtonPressedInside, none };
		virtual void renderWithRenderer(I4DyuchiGXRenderer* renderer, int zIndex);

		virtual void handleMouseDown(Point mouse, MouseButton button);
		virtual void handleMouseUp(Point mouse, MouseButton button);
		void handleMouseMove(Point mouse);
		void handleKeyUp(WPARAM wparam, LPARAM lparam);
		void handleKeyDown(WPARAM wparam, LPARAM lparam);

		void setHidden(bool isHidden) { _isHidden = isHidden; };
		bool getHidden() const { return _isHidden; }

		bool swallowsMouse(Point mouse);
		bool swallowsKeyboard();

		Rect globalFrame() const;
		Rect boundingBoxInParent() const;
		Rect frameInParent() const { return _frameInParent; }
		Rect bounds() const { return Rect{ {0, 0}, _frameInParent.size }; }

		void updateOriginInParent(Point newOrigin) {
			_frameInParent.origin = newOrigin;
		}

		void updateFrameInParent(Rect newFrame) { _frameInParent = newFrame; }
	protected:
		Renderable() {
			_currentMouseState = MouseState::none;
			_isHidden = false;
			_parent = nullptr;
		}

		Rect _frameInParent;
		Renderable* _parent;

		MouseState _currentMouseState;

		virtual void onMouseStateChange(MouseState newState, MouseState oldState) {}
		virtual void processKeyUp(WPARAM wparam, LPARAM lparam) {}
		virtual void processKeyDown(WPARAM wparam, LPARAM lparam) {}
		virtual bool swallowsKeyEvents() { return false; }
		virtual bool swallowsMouseEvents() { return false; }

		template<typename T>
		T* __ownedByRenderable registerChildRenderable(std::function<T*()> creatingFn) {
			T* result = creatingFn();
			_childRenderables.push_back(result);
			result->_parent = this;
			return result;
		}

		void deconstructAllChildren();
	private:
		bool _isHidden;
		std::vector<Renderable*> _childRenderables;
		void updateMouseState(MouseState newState);

	};

}

