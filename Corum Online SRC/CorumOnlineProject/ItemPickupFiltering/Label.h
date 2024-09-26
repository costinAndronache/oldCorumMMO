#pragma once
#include "CustomUIBase.h"
#include <string>

namespace CustomUI {
	class Label: public Renderable {
	public:
		struct Appearance {
			int size;
			Color color;
		};
		Label(Rect frame, Appearance appearance, std::string& text);
		Label(Rect frame, Appearance appearance, const char* text);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) override;

		static Size fittedSize(int charCount) {
			Size result = { charCount * 7, 9 };
			return result;
		}
	private:
		Rect _frame;
		std::string _text;
		Appearance _appearance;
	};
}


