#pragma once
#include "Renderable.h"
#include "Button.h"
#include "Label.h"

namespace CustomUI {
	class PagerControl: public Renderable {
	public:
		typedef std::function<void(int index)> PageScrollEventHandler;
		struct Appearance {
			Button::Sprites previousPageBtnSprites;
			Button::Sprites nextPageBtnSprites;
			SingleLineLabel::Appearance labelAppearance;
			static Appearance defaultAssets();
		};

		PagerControl(Rect frameInParent, unsigned int pageCount, Appearance appearance);
		
		void onPageScroll(PageScrollEventHandler handler) { _eventHandler = handler; }

	private:
		unsigned int _pageCount, _currentIndex;
		Appearance _appearance;
		PageScrollEventHandler _eventHandler;
		SingleLineLabel* _label;
		Button* _previousPageBtn;
		Button* _nextPageBtn;
	};
}
