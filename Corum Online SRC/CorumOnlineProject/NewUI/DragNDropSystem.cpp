#include "DragNDropSystem.h"

using namespace CustomUI;

DragNDropSystem::DragNDropSystem(DragNDropSystemRenderer* renderer, SoundLibrary* soundLibrary) {
	_renderer = renderer;
	_soundLibrary = soundLibrary;
}

void DragNDropSystem::registerRoute(
	DragNDropSender* sender,
	std::vector<DragNDropReceiver*> allowedReceivers,
	std::function<void(Rect globalFrameOnEnd, std::set<unsigned int> matchedReceiversIndexes)> onRouteMatch,
	std::function<void()> onNoRouteMatched
) {

	sender->onLeftMouseDragStart([=](Renderable* avatar, Rect globalFrameStart) {
		avatar->updateFrameInParent(globalFrameStart);
		_soundLibrary->playItemMousePickUp();

		_renderer->renderOnMouseCursorAvatar(
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

				_renderer->clearCurrentMouseCursorAvatar();
			}
		);
	});

}