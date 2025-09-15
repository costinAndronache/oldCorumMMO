#pragma once
#include "Renderable.h"
#include "Button.h"
#include "Label.h"

namespace CustomUI {
	class PagerControl: public Renderable {
	public:
		typedef std::function<void(unsigned int index)> PageScrollEventHandler;
		struct Appearance {
			Button::Sprites previousPageBtnSprites;
			Button::Sprites nextPageBtnSprites;
			SingleLineLabel::Appearance labelAppearance;
			static Appearance defaultAppearance();
		};

		PagerControl(Rect frameInParent, Appearance appearance);
		
		void onPageScroll(PageScrollEventHandler handler) { _eventHandler = handler; }
		void updatePageCount(unsigned int pageCount, unsigned int pageIndexOneBased);
		void setActivePage(unsigned int pageIndexOneBased);

		unsigned int activePageIndex() { return _activePageIndex; }
	private:
		unsigned int _pageCount, _activePageIndex;
		Appearance _appearance;
		PageScrollEventHandler _eventHandler;
		std::shared_ptr<SingleLineLabel> _label;
		std::shared_ptr<Button> _previousPageBtn;
		std::shared_ptr<Button> _nextPageBtn;

		void tryScrollPrevious();
		void tryScrollNext();
	};
}
