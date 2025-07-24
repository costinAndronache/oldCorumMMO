#pragma once
#include "../NewUI/RadioButtonGroup.h"
#include "../NewUI/PagedContainer.h"
#include "GenericItemsContainerView.h"

namespace NewInterface {
	class ItemInventoryView : public CustomUI::Renderable {
	public:
		using ItemLongPressHandlerLMB = GenericItemsContainerView::HandlerItemLongClickLEFT;
		using HandlerItemClickRIGHT = GenericItemsContainerView::HandlerItemClickRIGHT;

		struct Appearance {
			CustomUI::MatrixContainer::Appearance containerAppearance;
			int elementsCountPerPage;
		};

		using Sizes = CustomUI::MatrixContainer::Sizes;

		static Appearance smallInventoryAppearance;
		static Appearance largeInventoryAppearance;

		static CustomUI::Size appropriateSizeFor(Sizes sizes, int elementsCountPerPage);
		ItemInventoryView(CustomUI::Rect frameInParent, CItemResourceHash* resourceHash, Appearance appearance);

		void rebuild(
			const std::vector<CItem>& items, 
			ItemLongPressHandlerLMB onLongPressItemLMB = nullptr,
			HandlerItemClickRIGHT onRightClickItem = nullptr
		);

		int itemIndexForGlobalPoint(CustomUI::Point p); // -1 on not found

		void setHiddenStateForItemAtIndex(int index, bool isHidden);

	private:
		Appearance _appearance;
		CustomUI::PagedContainer* _pagedContainer;
		CItemResourceHash* _resourceHash;

		std::vector<GenericItemsContainerView*> _pages;
	};
}

