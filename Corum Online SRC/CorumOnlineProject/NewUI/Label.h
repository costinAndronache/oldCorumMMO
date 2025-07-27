#pragma once
#include "CustomUIBase.h"
#include "Renderable.h"
#include <string>

namespace CustomUI {
	class SingleLineLabel: public Renderable {
	public:
		enum class TextRenderingMode { withInsets, centered };
		struct Appearance {
			Appearance(Color c, IDIFontObject* f = nullptr): color(c), font(f)  {}
			Color color;
			IDIFontObject* font;

			static Appearance defaultAppearance() {
				return Appearance(Color::white, nullptr);
			}
		};
		SingleLineLabel(const Rect frameInParent, const Appearance appearance, const std::string& text);
		SingleLineLabel(const Rect frameInParent, const Appearance appearance, const char* text);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) override;

		static Size fittedSize(int charCount) {
			Size result = { charCount * 7, 9 };
			return result;
		}

		void updateTextTo(std::string newText);
		void updateRenderingModeToCentered();
		void updateRenderingModeToInsets(Insets insets);
	private:
		TextRenderingMode _mode;
		Insets _insetsForInsetMode;
		std::string _text;
		Appearance _appearance;
		Rect _renderingFrame;

		Rect renderingFrameForCurrentTextAndMode();
	};
}


