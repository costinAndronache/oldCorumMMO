#pragma once
#include "../HPInfoBarView.h"
#include "../../InitGame.h"

namespace NewInterface {
	class InfoBarManager {
	public:
		InfoBarManager(std::shared_ptr<HPInfoBarView> managedView, I4DyuchiGXExecutive* executive);

		void updateForCurrentHovering(GXOBJECT_HANDLE handleCurrentHoveredObject);

	private:
		std::shared_ptr<HPInfoBarView> _managedView;
		I4DyuchiGXExecutive* _executive;
	};
}


