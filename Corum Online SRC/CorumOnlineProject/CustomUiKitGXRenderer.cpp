#include "CustomUiKitGXRenderer.h"
#include "CustomDXDrawing/Renderer.h"
#include <d3d8.h>
#include <d3dx8.h>
#include "CustomDXDrawing/Colors.h"

using namespace CustomUiKit;

typedef union {
	CustomUiKit::Color color;
	DWORD dword;
} ColorToDword;

static CustomUiKitGXRenderer* instance = NULL;

CustomUiKitGXRenderer::CustomUiKitGXRenderer(CustomDXDrawing::Renderer* inputDxRenderer): dxRenderer(inputDxRenderer) {

}

CustomUiKitGXRenderer* CustomUiKitGXRenderer::sharedInstance() {
	if (instance == NULL) {
		LPDIRECT3DDEVICE8 ptr = NULL;
		g_pRenderer->GetD3DDevice(IID_IDirect3DDevice8, (void**)&ptr);

		instance = new CustomUiKitGXRenderer(new CustomDXDrawing::Renderer(ptr));
	}
	return instance;
}

void CustomUiKitGXRenderer::fillFrame(CustomUiKit::Rect frame, CustomUiKit::Color color) const {
	DWORD dwordColor = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
	dxRenderer->BoxFilled(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height, BLACK(5));
}

void CustomUiKitGXRenderer::drawLine(CustomUiKit::Point from, CustomUiKit::Point to, CustomUiKit::Color color) const {

}

void CustomUiKitGXRenderer::drawTextParagraph(const char* text, CustomUiKit::Rect box) const {

}