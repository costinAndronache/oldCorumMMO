#pragma once
#include "CustomUIBase.h"

#define __ownedByRenderable 
namespace CustomUI {
	class Renderable: public std::enable_shared_from_this<Renderable> {
	public:
		enum class MouseButton { left, right};
		enum class MouseState { hovering, leftButtonPressedInside, rightButtonPressedInside, none };
		virtual void renderWithRenderer(I4DyuchiGXRenderer* renderer, int zIndex);

		virtual void handleMouseDown(Point mouseGlobalOrigin, MouseButton button);
		virtual void handleMouseUp(Point mouseGlobalOrigin, MouseButton button);
		virtual void handleMouseMove(Point mouseGlobalOrigin);
		void handleKeyUp(WPARAM wparam, LPARAM lparam);
		void handleKeyDown(WPARAM wparam, LPARAM lparam);

		void setHidden(bool isHidden) { _isHidden = isHidden; };
		bool getHidden() const { return _isHidden; }

		void toggleHiddenState() { _isHidden = !_isHidden; }

		virtual bool swallowsMouse(Point mouse);
		bool swallowsKeyboard();

		Rect globalFrame() const;
		Rect boundingBoxInParent() const;
		Rect frameInParent() const { return _frameInParent; }
		Rect bounds() const { return Rect{ {0, 0}, _frameInParent.size }; }

		void updateOriginInParent(Point newOrigin) {
			_frameInParent.origin = newOrigin;
		}

		void updateZIndexOffsetForce(int offset) {
			_zIndexOffsetForce = offset;
		}

		void updateFrameInParent(Rect newFrame) { _frameInParent = newFrame; }
		void updateBackground(SpriteModel backgroundSprite) { _backgroundSprite = backgroundSprite; }

		MouseState currentMouseState() const { return _currentMouseState; }
		virtual ~Renderable() { deconstructAllChildren(); }
	protected:
		Renderable() {
			_parent = nullptr;
			_frameInParent = Rect::zero();
			_currentMouseState = MouseState::none;
			_isHidden = false;
			_backgroundSprite = SpriteModel::zero;
			_zIndexOffsetForce = 0;
		}

		Rect _frameInParent;
		/*std::weak_ptr<*/Renderable* _parent;

		MouseState _currentMouseState;

		virtual void onMouseStateChange(MouseState newState, MouseState oldState, Point atMouseGlobalCoords) {}
		virtual void onMouseMove(Point mouseGlobalOrigin) { }
		virtual void processKeyUp(WPARAM wparam, LPARAM lparam) {}
		virtual void processKeyDown(WPARAM wparam, LPARAM lparam) {}
		virtual bool swallowsKeyEvents() { return false; }
		virtual bool swallowsMouseEvents() { return false; }

		template<typename T>
		std::shared_ptr<T> registerChildRenderable(std::function<std::shared_ptr<T>()> creatingFn) {
			auto result = creatingFn();
			_childRenderables.push_back(result);
			result->updateParentTo(this);
			return result;
		}

		void deconstructAllChildren();
		void deconstructChildrenWhere(std::function<bool(std::shared_ptr<Renderable>)> eligibleToDeconstruct);

		int _zIndexOffsetForce;
		std::vector<std::shared_ptr<Renderable>> _childRenderables;

		virtual void handleRenderableHierarchyUpdateEvent();
	private:
		bool _isHidden;
		void updateMouseState(MouseState newState, Point mouseGlobalCoords);
		SpriteModel _backgroundSprite;

		void updateParentTo(/*std::weak_ptr*/Renderable* parent); 
	};

}

