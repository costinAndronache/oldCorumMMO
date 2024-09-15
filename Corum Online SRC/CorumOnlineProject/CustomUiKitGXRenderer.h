#pragma once
#include "InitGame.h"
#include "CustomUiKit/Renderer.h"
#include "CustomDXDrawing/Renderer.h"

class CustomUiKitGXRenderer: public CustomUiKit::Renderer {
public:
	CustomUiKitGXRenderer(CustomDXDrawing::Renderer* inputDxRenderer);
	static CustomUiKitGXRenderer* sharedInstance();
	virtual void fillFrame(CustomUiKit::Rect frame, CustomUiKit::Color color) const override;
	virtual void drawLine(CustomUiKit::Point from, CustomUiKit::Point to, CustomUiKit::Color color) const override;
	virtual void drawTextParagraph(const char* text, CustomUiKit::Rect box) const override;
private:
	CustomDXDrawing::Renderer* dxRenderer;
};

