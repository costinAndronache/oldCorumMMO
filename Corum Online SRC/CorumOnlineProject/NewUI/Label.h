#pragma once
#include "CustomUIBase.h"
#include "Renderable.h"
#include <string>

namespace CustomUI {
	class SingleLineLabel: public Renderable {
	public:
		struct Appearance {
			Color color;
		};
		SingleLineLabel(Rect frameInParent, Appearance appearance, std::string& text);
		SingleLineLabel(Rect frameInParent, Appearance appearance, const char* text);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) override;

		static Size fittedSize(int charCount) {
			Size result = { charCount * 7, 9 };
			return result;
		}
	private:
		std::string _text;
		Appearance _appearance;
	};
}


