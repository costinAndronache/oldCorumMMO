#pragma once
#include "CustomUIBase.h"
#include "Renderable.h"
#include <string>

namespace CustomUI {
	class SingleLineLabel: public Renderable {
	public:
		struct Appearance {
			Appearance(Color c, IDIFontObject* f = nullptr, Point offsets = { 0, 0}): color(c), font(f), offsets(offsets)  {}
			Color color;
			IDIFontObject* font;
			Point offsets;
		};
		SingleLineLabel(const Rect frameInParent, const Appearance appearance, const std::string& text);
		SingleLineLabel(const Rect frameInParent, const Appearance appearance, const char* text);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) override;

		static Size fittedSize(int charCount) {
			Size result = { charCount * 7, 9 };
			return result;
		}

		void updateTextTo(std::string newText) { _text = newText; }
	private:
		std::string _text;
		Appearance _appearance;
	};
}


