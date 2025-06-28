#pragma once

#include "CustomUIBase.h"
#include "Button.h"

namespace CustomUI {
	class SelectionView: public Renderable {
	public:
		typedef std::function<void(bool)> SelectionStateChangeHandler;
		SelectionView(Rect frame, Renderable* subView);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) override;
		void setSelectionState(bool isSelected);
		void setEnableSelection(bool selectionEnabled);
		bool isSelected();
		Renderable* renderable();

		void* data;

		void onSelectionStateChange(SelectionStateChangeHandler handler) {
			_handler = handler;
		}

	private:
		Renderable* _renderable;
		SelectionStateChangeHandler _handler;

		Rect _checkmarkFrame;

		bool _isSelected;
		bool _selectionEnabled;
		Button* _button;

		void onButtonPress(Button* button);
		void onButtonPressRelease(Button* button);
	};

	struct SelectionViewResources {
		static void initialize();
		static SpriteModel checkmarkSpriteModel;
	};
}


