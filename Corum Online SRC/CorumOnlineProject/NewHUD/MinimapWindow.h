#pragma once
#include "DisplacementHandleRenderable.h"
#include "../NewUI/Label.h"
#include <map>


namespace NewInterface {
	class MinimapWindow: public CustomUI::Renderable {
	public:
		typedef int DotHandle;
	public:
		MinimapWindow(CustomUI::Rect frameInParent);

		DotHandle addDot(CustomUI::SpriteModel sprite);
		void updateDot(DotHandle handle, VECTOR2 normalizedPosition);
		void removeDot(DotHandle handle);

		void updateMapSprite(CustomUI::SpriteModel mapSprite, std::string title);

		std::shared_ptr<DisplacementHandleRenderable> displacementHandle() { return _displacementHandle; }
	private:
		DotHandle _handleCounter;
		std::map<DotHandle, std::shared_ptr<CustomUI::SpriteRenderable>> _dots;
		std::shared_ptr<CustomUI::SpriteRenderable> _mapSprite;
		std::shared_ptr<DisplacementHandleRenderable> _displacementHandle;
		std::shared_ptr<CustomUI::SingleLineLabel> _titleLabel;
	};
}

