#include "PagedContainer.h"

using namespace CustomUI;

const auto pagerHeight = 30;

int PagedContainer::appropriateHeightFor(int pageHeight) {
	return pageHeight + pagerHeight;
}

PagedContainer::PagedContainer(Rect frameInParent) {
	_frameInParent = frameInParent;
	_pageFrame = bounds()
		.withHeightOffset(-pagerHeight);
	
	const auto pagerControlFrame = bounds()
		.withHeight(pagerHeight)
		.withY(_pageFrame.maxY());

	_pager = registerChildRenderable<PagerControl>([=]() {
		return std::make_shared<PagerControl>(pagerControlFrame,
			PagerControl::Appearance::defaultAppearance()
		);
	});

	_pager->onPageScroll([=](unsigned int index) {
		setActivePage(index);
	});
}

void PagedContainer::setActivePage(unsigned int pageIndexOneBased) {
	for (auto page : _pages) { page->setHidden(true); }
	if (!(1 <= pageIndexOneBased && pageIndexOneBased <= _pages.size())) { 
		if (_pages.size() > 0) {
			setActivePage(1);
		}
		return;  
	}

	_pages[pageIndexOneBased - 1]->setHidden(false);
	_pager->setActivePage(pageIndexOneBased);
}