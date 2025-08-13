#pragma once
#include "Renderable.h"
#include <set>
#include "../NewHelpers/SoundLibrary.h"
namespace CustomUI {
	
	class DragNDropSender {
	public:
		virtual void onLeftMouseDragStart(std::function<void(Renderable*, Rect globalFrameStart)> ) = 0;
	};

	class DragNDropReceiver {
	public:
		virtual Rect currentGlobalFrame() = 0;
	};

	class DragNDropSystemRenderer {
	public:
		virtual void renderOnMouseCursorAvatar(Renderable* avatar, std::function<void(Rect avatarCurrentGlobalFrame)> onLeftMouseButtonUP) = 0;
		virtual void clearCurrentMouseCursorAvatar() = 0;
	};

	class DragNDropSystem {
	public:
		DragNDropSystem(DragNDropSystemRenderer* renderer, SoundLibrary* soundLibrary);

		void registerRoute(
			DragNDropSender* sender,
			std::vector<DragNDropReceiver*> allowedReceivers,
			std::function<void(Rect globalFrameOnEnd, std::set<unsigned int> matchedReceiversIndexes)> onRouteMatch,
			std::function<void()> onNoRouteMatched
		);

	private:
		DragNDropSystemRenderer* _renderer;
		SoundLibrary* _soundLibrary;
	};
}

