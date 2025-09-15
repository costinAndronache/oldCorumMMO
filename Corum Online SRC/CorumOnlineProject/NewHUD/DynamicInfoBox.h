#pragma once
#include "../NewUI/MatrixContainer.h"
#include "../NewUI/Label.h"
#include "../NewUI/BorderRenderable.h"

namespace NewInterface {
	class DynamicInfoBox: public CustomUI::Renderable {
	public:
		struct InfoLine {
			std::string text;
			CustomUI::Color color;
		};

		DynamicInfoBox(CustomUI::Point pointInOrigin);

		void updateWithLines(const std::vector<InfoLine>& lines);

	private:
		std::shared_ptr<CustomUI::MatrixContainer> _infoLinesContainer;
		std::shared_ptr<CustomUI::BorderRenderable> _border;
	};
}


