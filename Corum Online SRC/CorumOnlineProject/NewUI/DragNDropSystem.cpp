#include "DragNDropSystem.h"

using namespace CustomUI;

DragNDropSystem::DragNDropSystem(
	DragNDropSystemRenderer* renderer, 
	SoundLibrary* soundLibrary
) {
	_renderer = renderer;
	_soundLibrary = soundLibrary;
}

void DragNDropSystem::registerRoute(
	std::shared_ptr<DragNDropSender> sender,
	std::vector<std::shared_ptr<DragNDropReceiver>> allowedReceivers,
	std::function<void(Rect globalFrameOnEnd, std::set<unsigned int> matchedReceiversIndexes)> onRouteMatch,
	std::function<void()> onNoRouteMatched
) {
	auto weakThis = weak_from_this();

	sender->onLeftMouseDragStart(
		[weakThis, allowedReceivers, onRouteMatch, onNoRouteMatched]
		(std::shared_ptr<Renderable> avatar, Rect globalFrameStart) {
		auto tthis = weakThis.lock();
		if(!tthis) { return; }

		avatar->updateFrameInParent(globalFrameStart);
		tthis->_soundLibrary->playItemMousePickUp();
		 
		tthis->_renderer->renderOnMouseCursorAvatar(
			avatar,
			[=](Rect avatarCurrentGlobalFrame) {
				std::set<unsigned int> foundReceiversIndexes;
				for (int i = 0; i < allowedReceivers.size(); i++) {
					auto receiver = allowedReceivers[i];
					if (receiver->currentGlobalFrame()
						.containsPoint(avatarCurrentGlobalFrame.center())) {
						foundReceiversIndexes.insert(i);
					}
				}

				if (foundReceiversIndexes.empty()) {
					onNoRouteMatched();
				}
				else {
					onRouteMatch(avatarCurrentGlobalFrame, foundReceiversIndexes);
				}

				tthis->_renderer->clearCurrentMouseCursorAvatar();
			}
		);
	});

}