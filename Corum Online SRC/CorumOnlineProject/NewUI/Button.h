#pragma once
#include "../InitGame.h"
#include "CustomUIBase.h"
#include "Label.h"
#include "SpriteCollection.h"
#include "Renderable.h"
#include <functional>

namespace CustomUI {
	class Button: public Renderable
	{
	public:
		typedef std::function<void()> Event;

		struct LabelModel {
			const char* text;
			SingleLineLabel::Appearance appearance;
		};
		Button(SpriteModel spriteModel, SpriteModel pressedSpriteModel, Rect frameInParent);
		Button(SpriteModel spriteModel, SpriteModel pressedSpriteModel, LabelModel labelModel, Rect frameInParent);

		void updateSpriteModelTo(SpriteModel newModel);
		void updatePressedSpriteModelTo(SpriteModel newPressedStateSpriteModel);

		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order);


		void onPress(Event event) { _onPress = event;  }
		void onRelease(Event event) { _onRelease = event; }
	protected:
		void onMouseStateChange(MouseState newState, MouseState oldState) override;
	private:
		SpriteModel _spriteModel;
		SpriteModel _pressedSpriteModel;
		SingleLineLabel* _label;
		Event _onPress, _onRelease;
	};

}

