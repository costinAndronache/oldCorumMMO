#include "InfoBarManager.h"

using namespace NewInterface;
using namespace CustomUI;

InfoBarManager::InfoBarManager(
	std::shared_ptr<HPInfoBarView> managedView, 
	I4DyuchiGXExecutive* executive
) {
	_managedView = managedView;
	_executive = executive;
}

void InfoBarManager::updateForCurrentHovering(GXOBJECT_HANDLE handleCurrentHoveredObject) {
	_managedView->setHidden(true);

	if(!handleCurrentHoveredObject) { return; }

	auto info = (LPObjectDesc)_executive->GetData(handleCurrentHoveredObject);
	if(!info) { return; }

	if(info->bType == OBJECT_TYPE_MONSTER || info->bType == OBJECT_TYPE_TRADER_MONSTER) {
		auto monster = (CMonster*)info->pInfo;
		char text[150];
		snprintf(text, sizeof(text), 
			"%s Lv%d (%d / %d)", 
			monster->m_szName, monster->m_dwLevel,
			monster->m_dwHP, monster->m_dwMaxHP
		);
		_managedView->setHidden(false);
		_managedView->updateNameWithScale(
			text,
			monster->m_dwHP / (float)monster->m_dwMaxHP
		);
	}
}