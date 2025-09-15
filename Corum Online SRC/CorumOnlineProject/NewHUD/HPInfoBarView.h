#pragma once
#include "../NewUI/HorizontalScalingBar.h"
#include "../NewUI/Label.h"

namespace NewInterface {
	class HPInfoBarView: public CustomUI::Renderable {
	public:
		HPInfoBarView(CustomUI::Rect frameInParent);

		void updateNameWithScale(std::string name, float hpScale);
	private:
		std::shared_ptr<CustomUI::HorizontalScalingBar> _hpBar;
		std::shared_ptr<CustomUI::SingleLineLabel> _nameLabel;
	};
}


