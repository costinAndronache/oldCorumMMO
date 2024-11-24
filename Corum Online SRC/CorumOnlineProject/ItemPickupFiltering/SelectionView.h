#pragma once

#include "CustomUIBase.h"
#include "Button.h"

namespace CustomUI {
	class SelectionView;

	class SelectionViewClient {
	public:
		virtual void selectionViewDidChangeSelectionState(SelectionView* view, bool isSelected) = 0;
	};

	class SelectionView: public Renderable, public ButtonClient {
	public:
		SelectionView(Rect frame, Renderable* subView, SelectionViewClient* client);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) override;
		void setSelectionState(bool isSelected);
		void setEnableSelection(bool selectionEnabled);
		bool isSelected();
		Renderable* renderable();

		void onButtonPress(Button* button) override;
		void onButtonPressRelease(Button* button) override;

		void* data;

	private:
		Rect _frame;
		Renderable* _renderable;
		SelectionViewClient* _client;

		Rect _checkmarkFrame;

		bool _isSelected;
		bool _selectionEnabled;
		Button* _button;
	};

	struct SelectionViewResources {
		static void initialize();
		static SpriteModel checkmarkSpriteModel;
	};
}


