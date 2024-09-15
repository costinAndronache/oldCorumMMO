#include "ItemInfoView.h"
#include "../CustomDXDrawing/Colors.h"
#include "../CorumResource.h"
#include "../Interface.h"

using namespace CustomUI;
IDISpriteObject* ItemInfoView::backgroundSprite = NULL;
Size ItemInfoView::bgSpriteSize;

void ItemInfoView::setupBackgroundSprite(IDISpriteObject* bgSprite, Size bgSpriteSize) {
	ItemInfoView::backgroundSprite = bgSprite;
	ItemInfoView::bgSpriteSize = bgSpriteSize;
}

ItemInfoView::ItemInfoView(Model model, Rect rect) : 
	 _rect(rect), _model(model) {

}

void ItemInfoView::updateModel(Model newModel) {
	_model = newModel;
}

bool ItemInfoView::renderInfoIfMouseHover() {
	if (_model.item == NULL) {
		return false;
	}

	if (_rect.isGlobalMouseInside()) {
		CInterface::GetInstance()->ItemInfoRender(_model.item, FALSE);
		return true;
	}

	return false;
}

void ItemInfoView::renderImageWithRenderer(I4DyuchiGXRenderer* renderer, int order) {
	VECTOR2 vPos = { _rect.origin.x, _rect.origin.y };

	VECTOR2 scale = { 
		((float)_rect.size.width) / ItemInfoView::bgSpriteSize.width, 
	    ((float)_rect.size.height) / ItemInfoView::bgSpriteSize.height
	};
	g_pRenderer->RenderSprite(backgroundSprite
		, &scale, 0.0f, &vPos, NULL, 0xffffffff, order, RENDER_TYPE_DISABLE_TEX_FILTERING);


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

		g_pRenderer->RenderSprite(lpItemResourceEx->pSpr
			, &itemScale, 0.0f, &vPos, NULL, 0xffffffff, order, RENDER_TYPE_DISABLE_TEX_FILTERING);
	}
}
