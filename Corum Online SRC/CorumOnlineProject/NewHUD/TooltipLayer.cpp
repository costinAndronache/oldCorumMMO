#include "TooltipLayer.h"

using namespace CustomUI;
using namespace NewInterface;

TooltipLayer::TooltipLayer(CustomUI::Rect frameInParent) {
	_frameInParent = frameInParent;
	_internalCounter = 0;
}

TooltipLayer::Handle TooltipLayer::addTooltip(const std::vector<InfoLine>& infoLines) {
	auto handle = [=]() -> Handle {
		if (_freeHandles.empty()) {
			_internalCounter += 1;
			auto infoBox = registerChildRenderable<DynamicInfoBox>([=]() {
				return std::make_shared<DynamicInfoBox>(Point{ 0, 0 });
			});

			infoBox->updateWithLines(infoLines);
			_currentInfoBoxes.insert({ _internalCounter, infoBox });

			return _internalCounter;
		} else {
			auto existing = *(_freeHandles.begin());
			auto infoBox = _currentInfoBoxes[existing];
			infoBox->setHidden(false);
			infoBox->updateWithLines(infoLines);

			_freeHandles.erase(existing);
			return existing;
		}
	}();

	return handle;
}

void TooltipLayer::updateTooltipPosition(Handle handle, CustomUI::Point position) {
	if (_currentInfoBoxes.find(handle) != _currentInfoBoxes.end()) {
		auto infoBox = _currentInfoBoxes[handle];
		infoBox->updateOriginInParent(position);
	}
}

void TooltipLayer::removeTooltip(Handle handle) {
	if (_currentInfoBoxes.find(handle) != _currentInfoBoxes.end()) {
		auto infoBox = _currentInfoBoxes[handle];
		infoBox->setHidden(true);
		_freeHandles.insert(handle);
	}
}