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
		typedef std::function<void()> EventHandler;

		struct LabelModel {
			LabelModel(): text(""), appearance(Color::white) {}
			const char* text;
			SingleLineLabel::Appearance appearance;
		};

		struct Sprites {
			SpriteModel normal;
			SpriteModel hovered;
			SpriteModel pressed;
			
			static Sprites allZero;
		};

		Button(Sprites sprites, Rect frameInParent);
		Button(Sprites sprites, LabelModel labelModel, Rect frameInParent);

		void updateSpriteModelTo(Sprites newSprites);

		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order);


		void onClickStart(EventHandler event) { _onPress = event;  }
		void onClickEnd(EventHandler event) { _onRelease = event; }

	protected:
		void onMouseStateChange(MouseState newState, MouseState oldState) override;

	private:
		SingleLineLabel* _label;
		EventHandler _onPress, _onRelease;
		Sprites _sprites;

	};

}

