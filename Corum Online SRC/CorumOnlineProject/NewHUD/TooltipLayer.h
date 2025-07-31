#pragma once

#include "DynamicInfoBox.h"
#include <vector>
#include <map>
#include <set>

namespace NewInterface {

	class TooltipLayer: public CustomUI::Renderable {
	public:
		using Handle = int;
		using InfoLine = DynamicInfoBox::InfoLine;
		TooltipLayer(CustomUI::Rect frameInParent);

		Handle addTooltip(const std::vector<InfoLine>& infoLines);
		void updateTooltipPosition(Handle handle, CustomUI::Point position);
		void removeTooltip(Handle handle);

		virtual bool swallowsMouse(CustomUI::Point) override { return false; }
	private:
		Handle _internalCounter;
		std::map<Handle, DynamicInfoBox*> _currentInfoBoxes;
		std::set<Handle> _freeHandles;
	};
}


