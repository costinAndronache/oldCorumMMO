#include "ItemInventoryView.h"
#include "../Utils/Utils.h"
#include "../NewUI/PagedMatrixContainer.h"

using namespace CustomUI;
using namespace NewInterface;

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

void ItemInventoryView::rebuild(const std::vector<CItem>& items) {
	auto split = splitVector<CItem>(items, _appearance.elementsCountPerPage);
	_pagedContainer->rebuildPages< std::vector<CItem> >(split,
		[=](Rect frame, std::vector<CItem>& modelsForCurrentPage, int currentPageIndex) {
		auto mc = new GenericItemsContainerView(frame, _resourceHash, 
			{
				{ 34, 34},
				3,
				8,
				NewHUDResources::inventoryItemUnderlaySprite
			}
		);

		mc->updateWithItems(modelsForCurrentPage);

		return mc;
	});

	_pagedContainer->setActivePage(1);
}
