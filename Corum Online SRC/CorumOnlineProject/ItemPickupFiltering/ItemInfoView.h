#pragma once
#include "../InitGame.h"
#include <vector>
#include <string>

struct Size { int width; int height; };
struct Point { int x; int y; };
struct Rect {
	Point origin;
	Size size;
};

class ItemInfoView {
public:
	struct Model {
		CItem* item;
		Size itemResourceSize;
	};
	ItemInfoView(Model model, Rect frame);
	void updateModel(Model newModel);
	void renderImageWithRenderer(I4DyuchiGXRenderer* renderer);
	bool renderInfoIfMouseHover();
	static void setupBackgroundSprite(IDISpriteObject* bgSprite, Size bgSpriteSize);
private:
	const Rect _rect;
	Model _model;
	static IDISpriteObject* backgroundSprite;
	static Size bgSpriteSize;
};

