#include "Label.h"

using namespace CustomUI;

Label::Label(Rect frame, Appearance appearance, std::string& text): 
	_frame(frame),  _appearance(appearance), _text(text) {
}

Label::Label(Rect frame, Appearance appearance, const char* text):
	_frame(frame), _appearance(appearance) {
	_text = std::string(text);
}

void Label::renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) { 
	RECT r = { _frame.origin.x, _frame.origin.y - 5, _frame.maxX(), _frame.maxY() };
	renderer->RenderFont(GetFont(), (char*)_text.c_str(), _text.size(), &r, _appearance.color.asDXColor(), CHAR_CODE_TYPE_ASCII, order, 0);
}