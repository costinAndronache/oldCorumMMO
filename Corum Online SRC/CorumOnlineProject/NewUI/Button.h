#pragma once
#include "../InitGame.h"
#include "../../BaseLibrary/Timer.h"

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
			std::string text;
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


		void onClickStartLEFT(EventHandler event) { _onPressLEFT = event;  }
		void onClickEndLEFT(EventHandler event) { _onReleaseLEFT = event; }
		void onLongPressDetectedLEFT(EventHandler event) {
			_onLongPressDetectedLEFT = event;
		}

		void onClickEndRIGHT(EventHandler handler) { _onReleaseRIGHT = handler; }

	protected:
		void onMouseStateChange(MouseState newState, MouseState oldState, Point) override;

	private:
		SingleLineLabel* _label;
		EventHandler _onPressLEFT, _onReleaseLEFT;
		EventHandler _onPressRIGHT, _onReleaseRIGHT;
		EventHandler _onLongPressDetectedLEFT;
		Sprites _sprites;
		std::shared_ptr<Timer> _longPressTimerLEFT;
	};

}

