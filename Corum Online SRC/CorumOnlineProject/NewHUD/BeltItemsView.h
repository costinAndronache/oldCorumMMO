#pragma once
#include "LeftHUD.h"
#include <vector>

namespace NewInterface {
	class BeltItemView : public CustomUI::Renderable {
	public:
		BeltItemView(CustomUI::Rect frameInParent);

	public:
		CustomUI::Button* _button;
		CustomUI::SingleLineLabel* _instanceCountLabel;
	};

	class BeltItemsView: public CustomUI::Renderable {
	public:
		struct Appearance {
			CustomUI::Size itemSize;
			int spacing;
		};
		BeltItemsView(CustomUI::Rect frameInParent, 
					  const CItemResourceHash* resourceHash,
					  Appearance appearance);

		void updateWithItems(const CItem items[MAX_BELT_POOL]);

	private:
		const CItemResourceHash* _resourceHash;
		std::vector<BeltItemView*> _itemViews;
		Appearance _appearance;
	};
}


