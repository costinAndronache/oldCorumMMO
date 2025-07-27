#include "ItemInfoView.h"
#include "../CustomDXDrawing/Colors.h"
#include "../CorumResource.h"
#include "../Interface.h"

// 191 185, 32 32 menu1 tga
using namespace CustomUI;

SpriteModel ItemInfoViewResources::bgSpriteModel = { NULL, {38, 70}, };
SpriteModel ItemInfoViewResources::unknownSpriteModel = { NULL, {32, 32}, };

void ItemInfoViewResources::initialize() {
	if (bgSpriteModel.sprite == NULL) {
		bgSpriteModel.sprite = g_pRenderer->CreateSpriteObject(
			GetFile("menu_4.tif", DATA_TYPE_UI),
			0, 0,
			bgSpriteModel.naturalSize.width, bgSpriteModel.naturalSize.height,
			0);
	}

	if (unknownSpriteModel.sprite == NULL) {
		unknownSpriteModel.sprite = g_pRenderer->CreateSpriteObject(
			GetFile("menu_1.tga", DATA_TYPE_UI),
			191, 185,
			unknownSpriteModel.naturalSize.width, unknownSpriteModel.naturalSize.height,
			0);
	}
}

ItemInfoView::ItemInfoView(Model model, Rect frameInParent, SpriteModel backgroundSpriteModel): _model(model), _backgroundSpriteModel(backgroundSpriteModel) {
	_frameInParent = frameInParent;
}

void ItemInfoView::updateModel(Model newModel) {
	_model = newModel;
}

bool ItemInfoView::renderInfoIfMouseHover() {
	if (_model.item == NULL) {
		return false;
	}
	

	if (globalFrame().containsPoint(g_Mouse.MousePos)) {
		CInterface::GetInstance()->ItemInfoRender(_model.item, FALSE);
		return true;
	}

	return false;
}

void ItemInfoView::renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) {
	auto _rect = globalFrame();

	VECTOR2 vPos = { _rect.origin.x, _rect.origin.y };

	VECTOR2 scale = _rect.size.divideBy(_backgroundSpriteModel.naturalSize);
	g_pRenderer->RenderSprite(
		_backgroundSpriteModel.sprite,
		&scale, 0.0f, &vPos, 
		NULL, 0xffffffff, 
		order, RENDER_TYPE_DISABLE_TEX_FILTERING
	);


	if (_model.item == NULL) {
		return;
	}

	LP_ITEM_RESOURCE_EX lpItemResourceEx = g_pItemResourceHash->GetData(_model.item->m_wItemID);

	if (lpItemResourceEx)
	{
		VECTOR2 itemScale = { 1, 1 };
		if (_model.itemResourceSize.width > _rect.size.width || 
			_model.itemResourceSize.height > _rect.size.height) {
			itemScale.x = ((float)_rect.size.width) / _model.itemResourceSize.width;
			itemScale.y = ((float)_rect.size.height) / _model.itemResourceSize.height;
		}

		renderer->RenderSprite(lpItemResourceEx->pSpr
			, &itemScale, 0.0f, &vPos, NULL, 0xffffffff, order, RENDER_TYPE_DISABLE_TEX_FILTERING);
	}
	else {
		VECTOR2 scale = _rect.size.divideBy(ItemInfoViewResources::unknownSpriteModel.naturalSize);
		renderer->RenderSprite(ItemInfoViewResources::unknownSpriteModel.sprite
			, &scale, 0.0f, &vPos, NULL, 0xffffffff, order, RENDER_TYPE_DISABLE_TEX_FILTERING);
	}
}
