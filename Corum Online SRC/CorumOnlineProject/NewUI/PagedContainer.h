#pragma once

#include "PagerControl.h"

namespace CustomUI {
	class PagedContainer: public Renderable {
	public:
		static int appropriateHeightFor(int pageHeight);
		PagedContainer(Rect frameInParent);

		template<typename Model>
		void rebuildPages(const std::vector<Model>& models,
			std::function<Renderable* (Rect frameInParent, Model model, int index)> createFn) {
			deconstructChildrenWhere([=](Renderable* child) { return child != _pager; });
			_pages.erase(_pages.begin(), _pages.end());

			for (int i = 0; i < models.size(); i++) {
				auto model = models[i];
				auto renderable = registerChildRenderable<Renderable>([=]() {
					return createFn(_pageFrame, model, i);
				});
				_pages.push_back(renderable);
			}

			_pager->updatePageCount(_pages.size(), 1);
		}

		void setActivePage(unsigned int pageIndexOneBased);
		unsigned int activePageIndex() { return _pager->activePageIndex(); }
	private:
		PagerControl* _pager;
		Rect _pageFrame;
		std::vector<Renderable*> _pages;
	};
}

