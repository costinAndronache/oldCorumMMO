#pragma once
#include "../NewUI/RadioButtonGroup.h"
#include "../NewUI/PagedContainer.h"
#include "GenericItemsContainerView.h"

namespace NewInterface {
	class ItemInventoryView : public CustomUI::Renderable {
	public:
		struct Appearance {
			CustomUI::MatrixContainer::Appearance containerAppearance;
			int elementsCountPerPage;
		};

		using Sizes = CustomUI::MatrixContainer::Sizes;

		static CustomUI::Size appropriateSizeFor(Sizes sizes, int elementsCountPerPage);
		ItemInventoryView(CustomUI::Rect frameInParent, CItemResourceHash* resourceHash, Appearance appearance);

		void rebuild(const std::vector<CItem>& items);
	private:
		Appearance _appearance;
		CustomUI::PagedContainer* _pagedContainer;
		CItemResourceHash* _resourceHash;
	};
}

