#include "PagerControl.h"
#include "../NewHUD/HUDSpriteCollection.h"

using namespace CustomUI;

PagerControl::Appearance PagerControl::Appearance::defaultAppearance() {
	auto labelAppearance = SingleLineLabel::Appearance(Color::white);
	return Appearance {
		NewHUDResources::previous,
		NewHUDResources::next,
		labelAppearance
	};
}

PagerControl::PagerControl(Rect frameInParent, Appearance appearance): 
 _appearance(appearance) {
	_frameInParent = frameInParent;
	_pageCount = 1; _activePageIndex = 1;

	const auto width = bounds().size.width;
	const auto prevPageBtnFrame = bounds()
		.withWidth(0.25 * width);

	const auto labelDedidcatedArea = bounds()
		.withWidth(0.5 * width)
		.withX(prevPageBtnFrame.maxX());

	const auto labelFrame = Rect{ {0, 0},
		SingleLineLabel::fittedSize("100/100")
	}.centeredWith(labelDedidcatedArea);

	const auto nextPageBtnFrame = bounds()
		.withWidth(0.25 * width)
		.withX(labelDedidcatedArea.maxX());

	_previousPageBtn = registerChildRenderable<Button>([=]() {
		return new Button(appearance.previousPageBtnSprites, prevPageBtnFrame);
	});

	_label = registerChildRenderable<SingleLineLabel>([=]() {
		return new SingleLineLabel(labelFrame, appearance.labelAppearance, "");
	});

	_nextPageBtn = registerChildRenderable<Button>([=]() {
		return new Button(appearance.nextPageBtnSprites, nextPageBtnFrame);
	});

	_previousPageBtn->onClickEndLEFT([=]() {
		tryScrollPrevious();
	});

	_nextPageBtn->onClickEndLEFT([=]() {
		tryScrollNext();
	});
}

void PagerControl::updatePageCount(unsigned int pageCount, unsigned int pageIndexOneBased) {
	if (!(pageIndexOneBased <= pageCount)) { return;  }
	_pageCount = pageCount;
	setActivePage(pageIndexOneBased);
}


void PagerControl::setActivePage(unsigned int activePageIndex) {
	if (!(1 <= activePageIndex && activePageIndex <= _pageCount)) { return; }
	_activePageIndex = activePageIndex;

	char text[10];
	wsprintf(text, "%d / %d", activePageIndex, _pageCount);
	_label->updateTextTo(std::string(text));

}

void PagerControl::tryScrollPrevious() {
	if (_activePageIndex == 1) { return;  }

	setActivePage(_activePageIndex - 1);
	if (_eventHandler) {
		_eventHandler(_activePageIndex);
	}
}

void PagerControl::tryScrollNext() {
	if (_activePageIndex == _pageCount) { return;  }
	
	setActivePage(_activePageIndex + 1);
	if (_eventHandler) {
		_eventHandler(_activePageIndex);
	}
}