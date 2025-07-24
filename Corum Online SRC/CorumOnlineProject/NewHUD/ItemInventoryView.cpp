#include "ItemInventoryView.h"
#include "../Utils/Utils.h"
#include "../NewUI/PagedMatrixContainer.h"

using namespace CustomUI;
using namespace NewInterface;

ItemInventoryView::Appearance ItemInventoryView::smallInventoryAppearance{
	{
		MatrixContainer::VerticalGrowthDirection::downwards,
		{
			{ 40, 40},
			7,
			5,
			5
		}
	},
	48
};

ItemInventoryView::Appearance ItemInventoryView::largeInventoryAppearance {
	{
		MatrixContainer::VerticalGrowthDirection::downwards,
		{
			{ 40, 80},
			7,
			5,
			5
		}
	},
	24
};

Size ItemInventoryView::appropriateSizeFor(Sizes sizes, int elementsCountPerPage) {
	return PagedMatrixContainer::appropriateSizeFor(sizes, elementsCountPerPage);
}

ItemInventoryView::ItemInventoryView(CustomUI::Rect frameInParent, CItemResourceHash* resourceHash, Appearance appearance) {
	_frameInParent = frameInParent;
	_appearance = appearance;
	_resourceHash = resourceHash;
	_pagedContainer = registerChildRenderable<PagedContainer>([=]() {
		return new PagedContainer(bounds());
	});
}

void ItemInventoryView::rebuild(
	const std::vector<CItem>& items, 
	ItemLongPressHandlerLMB onLongPressItemLMB,
	HandlerItemClickRIGHT onRightClickItem) 
{
	_pages.clear();

	auto split = splitVector<CItem>(items, _appearance.elementsCountPerPage);
	_pagedContainer->rebuildPages< std::vector<CItem> >(split,
		[=](Rect frame, std::vector<CItem>& modelsForCurrentPage, int currentPageIndex) {
		auto mc = new GenericItemsContainerView(
			frame, _resourceHash, _appearance.containerAppearance
		);

		mc->updateWithItems(
			modelsForCurrentPage, 
			NewHUDResources::inventoryItemUnderlaySprite,
		[=](CItem item, CUISpriteModel sprite, int index, Rect globalFrame) {
			const auto indexInOriginal = _appearance.elementsCountPerPage * currentPageIndex + index;
			if (onLongPressItemLMB) {
				onLongPressItemLMB(item, sprite, index, globalFrame);
			}
		},
		[=](CItem item, int index) {
			const auto indexInOriginal = _appearance.elementsCountPerPage * currentPageIndex + index;
			if (onRightClickItem) {
				onRightClickItem(item, indexInOriginal);
			}
		});

		_pages.push_back(mc);
		return mc;
	});

	_pagedContainer->setActivePage(1);
}

int ItemInventoryView::itemIndexForGlobalPoint(CustomUI::Point p) { // -1 on not found 
	const auto activePageIndex = _pagedContainer->activePageIndex() - 1;
	const auto view = _pages[activePageIndex];

	const auto localIndex = view->itemIndexForGlobalPoint(p);
	if (localIndex < 0) { return -1; }

	return activePageIndex * _appearance.elementsCountPerPage + localIndex;
}

void ItemInventoryView::setHiddenStateForItemAtIndex(int index, bool isHidden) {
	const auto pageIndex = index / _appearance.elementsCountPerPage;
	const auto localIndex = index % _appearance.elementsCountPerPage;

	if (!(0 <= pageIndex && pageIndex < _pages.size())) { return;  }
	_pages[pageIndex]->setHiddenStateForItemAtIndex(localIndex, isHidden);
}