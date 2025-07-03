#include "Label.h"

using namespace CustomUI;

SingleLineLabel::SingleLineLabel(Rect frameInParent, Appearance appearance, std::string& text):
	_appearance(appearance), _text(text) {
	_frameInParent = frameInParent;

}

SingleLineLabel::SingleLineLabel(Rect frameInParent, Appearance appearance, const char* text):
	_appearance(appearance) {
	_frameInParent = frameInParent;
	_text = std::string(text);
}

void SingleLineLabel::renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) {
	const auto frame = globalFrame();
	RECT r = { frame.origin.x, frame.origin.y, frame.maxX(), frame.maxY() };

	IDIFontObject* font = _appearance.font ? _appearance.font : GetFont();
	renderer->RenderFont(font, (char*)_text.c_str(), _text.size(), &r, _appearance.color.asDXColor(), CHAR_CODE_TYPE_ASCII, order, 0);
}