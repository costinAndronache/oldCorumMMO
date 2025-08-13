#pragma once

#include "../GenericItemsContainerView.h"
#include "../TooltipLayer.h"
#include "TooltipHelper.h"

#include <map>

namespace NewInterface {
	class TooltipManager {
	public:
		using TooltipObjectID = int;
		using InfoLine = DynamicInfoBox::InfoLine;
		using InfoLines = std::vector<InfoLine>;
		using TooltipCreateFn = std::function<InfoLines(TooltipObjectID)>;
		TooltipManager(
			TooltipLayer* layer,
			TooltipCreateFn createFn
		);

		void handleHoveringEvent(TooltipObjectID, CustomUI::Point mouseGlobalPos);
		void handleHoveringEndEvent(TooltipObjectID);
		void clearAllTooltips();
	private:
		TooltipCreateFn _createFn;
		TooltipLayer* _layer;
		std::map<TooltipObjectID, TooltipLayer::Handle> _currentDisplayedTooltips;

		TooltipLayer::Handle currentInfoTooltipFor(TooltipObjectID);
	};
}


