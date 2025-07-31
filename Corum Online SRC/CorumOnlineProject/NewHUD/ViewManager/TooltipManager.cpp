#include "TooltipManager.h"

using namespace NewInterface;
using namespace CustomUI;

TooltipManager::TooltipManager(
	TooltipLayer* layer,
	TooltipCreateFn createFn
) {
	_layer = layer;
	_createFn = createFn;
}

TooltipLayer::Handle TooltipManager::currentInfoTooltipFor(TooltipObjectID objectID) {
	auto found = _currentDisplayedTooltips.find(objectID);
	if (found != _currentDisplayedTooltips.end()) {
		return (*found).second;
	}

	auto handle = _layer->addTooltip(_createFn(objectID));
	_currentDisplayedTooltips.insert(
		{ objectID, handle}
	);

	return handle;
}

void TooltipManager::handleHoveringEvent(TooltipObjectID objectID, CustomUI::Point mouseGlobalPos) {
	auto handle = currentInfoTooltipFor(objectID);
	_layer->updateTooltipPosition(handle, mouseGlobalPos);
}

void TooltipManager::handleHoveringEndEvent(TooltipObjectID objectID) {
	auto found = _currentDisplayedTooltips.find(objectID);
	if (found == _currentDisplayedTooltips.end()) { return; }

	auto handle = found->second;
	_layer->removeTooltip(handle);

	_currentDisplayedTooltips.erase(objectID);
}

void TooltipManager::clearAllTooltips() {
	for (const auto& item : _currentDisplayedTooltips) {
		_layer->removeTooltip(item.second);
	}

	_currentDisplayedTooltips.clear();
}