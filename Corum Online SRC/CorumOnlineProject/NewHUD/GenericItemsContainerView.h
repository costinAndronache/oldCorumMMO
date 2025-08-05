#pragma once
#include "LeftHUD.h"
#include "../NewUI/MatrixContainer.h"
#include "../NewUI/Hoverable.h"
#include <vector>

namespace NewInterface {
	class GenericItemView : public CustomUI::Hoverable {
	public:
		GenericItemView(CustomUI::Rect frameInParent, CustomUI::SpriteModel underlay);

		CustomUI::Button* _button;
		CustomUI::SingleLineLabel* _instanceCountLabel;
	};

	class GenericItemsContainerView: public CustomUI::Renderable {
	public:
		using HandlerItemLongClickLEFT = std::function<void(CItem, CUISpriteModel, int index, CustomUI::Rect globalFrame)>;
		using HandlerItemClickRIGHT = std::function<void(CItem, int index)>;
		using HandlerItemHovering = std::function<void(CItem, int index, CustomUI::Point mousePointInGlobalCoords)>;
		using HandlerItemHoveringEnd = std::function<void(CItem, int index)>;

		struct Handlers {
			HandlerItemLongClickLEFT longClickLEFT;
			HandlerItemClickRIGHT clickRIGHT;
			HandlerItemHovering hovering;
			HandlerItemHoveringEnd hoveringEnd;
		};

		using Appearance = CustomUI::MatrixContainer::Appearance; 
	
		GenericItemsContainerView(CustomUI::Rect frameInParent, 
					  CItemResourceHash* resourceHash,
					  Appearance appearance);

		void updateWithItems(
			const std::vector<CItem>& items,
			CustomUI::SpriteModel itemUnderlaySprite,
			Handlers handlers
		);

		struct ItemWithUnderlay {
			CItem item;
			CustomUI::SpriteModel itemUnderlaySprite;
		};

		void updateWithItems(
			const std::vector<ItemWithUnderlay>& items,
			Handlers handlers
		);

		int itemIndexForGlobalPoint(CustomUI::Point p); // -1 on not found
		void setHiddenStateForItemAtIndex(int index, bool isHidden);


	private:
		CItemResourceHash* _resourceHash;
		std::vector<GenericItemView*> _itemViews;
		CustomUI::MatrixContainer* _matrixContainer;
		Appearance _appearance;
		Handlers _handlers;
	};
}


