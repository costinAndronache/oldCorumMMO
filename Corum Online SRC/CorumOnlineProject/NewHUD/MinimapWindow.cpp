#include "MinimapWindow.h"
#include "HUDSpriteCollection.h"
using namespace CustomUI;
using namespace NewInterface;

static Size dotSize = {5, 5};

MinimapWindow::MinimapWindow(CustomUI::Rect frameInParent) {
	_frameInParent = frameInParent;
	_handleCounter = 0;

	updateBackground(NewHUDResources::genericBackgroundSprite);

	auto displacementHandleFrame = Rect{{0, 0}, {25, 25}};
	_displacementHandle = registerChildRenderable<DisplacementHandleRenderable>([=](){
		return std::make_shared<DisplacementHandleRenderable>(displacementHandleFrame);
	});

	auto titleFrame = bounds()
		.withHeight(displacementHandleFrame.size.height)
		.withOriginOffsetBy({ (int)displacementHandleFrame.maxX(), 0});

	_titleLabel = registerChildRenderable<SingleLineLabel>([=](){
		return std::make_shared<SingleLineLabel>(titleFrame, SingleLineLabel::Appearance::defaultAppearance(), "");
	});

	auto mapSpriteFrame = bounds()
		.withHeightOffset(-displacementHandleFrame.size.height)
		.withY(displacementHandleFrame.maxY());

	_mapSprite = registerChildRenderable<SpriteRenderable>([=](){
		return std::make_shared<SpriteRenderable>(mapSpriteFrame, SpriteModel::zero);
	});
}

MinimapWindow::DotHandle MinimapWindow::addDot(CustomUI::SpriteModel sprite) {
	auto handle = _handleCounter++;

	auto renderable = registerChildRenderable<SpriteRenderable>([=](){
		return std::make_shared<SpriteRenderable>(Rect{{0, 0}, dotSize}, sprite);
	});

	_dots.insert({handle, renderable});

	return handle;
}

void MinimapWindow::updateDot(DotHandle handle, VECTOR2 normalizedPosition) {
	auto it = _dots.find(handle);
	if(it == _dots.end()) { return; }

	auto frame = _mapSprite->frameInParent();

	auto position = Point {
		(int)(normalizedPosition.x * frame.size.width),
		(int)(normalizedPosition.y * frame.size.height) + frame.origin.y 
	};

	position.x -= dotSize.width / 2;
	position.y -= dotSize.height / 2;

	it->second->updateOriginInParent(position);
}

void MinimapWindow::removeDot(DotHandle handle) {
	auto it = _dots.find(handle);
	if(it == _dots.end()) { return; }

	deconstructChildrenWhere([=](auto child){
		return child == it->second;
	}); 

	_dots.erase(handle);
}

void MinimapWindow::updateMapSprite(CustomUI::SpriteModel mapSprite, std::string title) {
	_mapSprite->updateSprite(mapSprite);
	_titleLabel->updateTextTo(title);
}