#include "Label.h"

using namespace CustomUI;

SingleLineLabel::SingleLineLabel(const Rect frameInParent, const Appearance appearance, const std::string& text):
	_appearance(appearance), _text(text) {
	_frameInParent = frameInParent;
	_mode = TextRenderingMode::withInsets;
	_insetsForInsetMode = { 0, 0, 0, 0 };

	_renderingSize = fittedSize(text);
}

SingleLineLabel::SingleLineLabel(const Rect frameInParent, const Appearance appearance, const char* text):
	_appearance(appearance) {
	_frameInParent = frameInParent;
	_text = std::string(text);
	_mode = TextRenderingMode::withInsets;
	_insetsForInsetMode = { 0, 0, 0, 0 };
	_renderingSize = fittedSize(text);
}

Rect SingleLineLabel::renderingFrameForCurrentTextAndMode() {
	switch (_mode) {
	case TextRenderingMode::withInsets:
		return globalFrame().withInsets(_insetsForInsetMode);
		break;
	case TextRenderingMode::centered:
		auto actualFrame = Rect{ {0, 0}, _renderingSize };
		return actualFrame
			.centeredWith(globalFrame());
	}
}

Size SingleLineLabel::fittedSize(const std::string& text) {
	auto lines = CustomUI::strtok(text, "\n");
	auto biggestLine = std::max_element(
		std::begin(lines),
		std::end(lines),
		[=](const std::string& a, const std::string& b) { return a.size() < b.size(); }
	);

	if (biggestLine == std::end(lines)) { return { 0, 0 }; }

	Size result = { biggestLine->size() * 7, lines.size() * 9 };
	return result;
}

void SingleLineLabel::handleRenderableHierarchyUpdateEvent() { }

void SingleLineLabel::updateTextTo(std::string newText) {
	_text = std::move(newText);
	_renderingSize = fittedSize(_text);
}

void SingleLineLabel::updateRenderingModeToCentered() { 
	_mode = TextRenderingMode::centered; 
}
void SingleLineLabel::updateRenderingModeToInsets(Insets insets) {
	_mode = TextRenderingMode::withInsets;
	_insetsForInsetMode = insets;
}

void SingleLineLabel::renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) {
	Renderable::renderWithRenderer(renderer, order);
	auto renderingFrame = renderingFrameForCurrentTextAndMode();
	RECT r{
		renderingFrame.origin.x,
		renderingFrame.origin.y,
		renderingFrame.maxX(),
		renderingFrame.maxY()
	};

	IDIFontObject* font = _appearance.font ? _appearance.font : GetFont();
	renderer->RenderFont(font, (char*)_text.c_str(), _text.size(), &r, _appearance.color.asDXColor(), CHAR_CODE_TYPE_ASCII, order + 1, 0);
}