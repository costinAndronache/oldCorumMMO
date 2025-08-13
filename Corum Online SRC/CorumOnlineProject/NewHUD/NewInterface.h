#pragma once
#include "LeftHUD.h"
#include "RightHUD.h"
#include "GroupedItemInventoryView.h"
#include "../NewUI/MouseTrackingSpriteRenderable.h"
#include "../NewUI/DragNDropSystem.h"
#include "ViewManager/DragNDropManager.h"
#include "UserInventoryView.h"
#include "ViewManager/EquipItemsManager.h"
#include "ViewManager/CharacterStatsManager.h"
#include "ViewManager/UserSkillsManager.h"
#include "DynamicInfoBox.h"
#include "ViewManager/TooltipHelper.h"


namespace NewInterface {
	class Interface: 
		public CustomUI::Renderable, 
		public CMainUserUpdateInterested,
		public CustomUI::DragNDropSystemRenderer {
	public:
		Interface(
			CustomUI::Size screenSize,
			CMainUser* mainUser,
			const LP_SKILL_LIST_MANAGER skillListManager,
			CItemResourceHash* resourceHash,
			SoundLibrary* soundLibrary,
			SharedNetwork* sharedNetwork,
			TooltipHelper* tooltipHelper
		);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int zIndex) override;
		bool swallowsMouse(CustomUI::Point mouse) override;
		void handleMouseDown(CustomUI::Point mouseGlobalOrigin, MouseButton button) override;
		void handleMouseUp(CustomUI::Point mouseGlobalOrigin, MouseButton button) override;
		void handleMouseMove(CustomUI::Point mouseGlobalOrigin) override;
		void processKeyDown(WPARAM wparam, LPARAM lparam) override;
	public:
		void updatedCurrentHP(CMainUser*, DWORD oldValue, DWORD newValue) override;
		void updatedCurrentSP(CMainUser*, DWORD oldValue, DWORD newValue) override;
		void updatedMAXHP(CMainUser*, DWORD oldValue, DWORD newValue) override;
		void updatedMAXSP(CMainUser*, DWORD oldValue, DWORD newValue) override;
		void updatedEXP(CMainUser*, DWORD oldValue, DWORD newValue) override;
		void updatedStatPoints(CMainUser*, DWORD oldValue, DWORD newValue) override;
		void updatedSkillPoints(CMainUser*, DWORD oldValue, DWORD newValue) override;
		void updatedCoolPoints(CMainUser*, float oldValue, float newValue) override;
		void updatedLevel(CMainUser*, DWORD oldValue, DWORD newValue) override;
		void updatedSkills(CMainUser*) override;
		void updatedLeftRightSkillSelection(CMainUser*) override;
		void updatedItemInventory(CMainUser*) override;

	public:
		virtual void renderOnMouseCursorAvatar(Renderable* avatar, 
			std::function<void(CustomUI::Rect avatarCurrentGlobalFrame)> onLeftMouseButtonUP
		);
		virtual void clearCurrentMouseCursorAvatar();
	private:
		std::shared_ptr<Interface> _thisAsShared;
		LP_SKILL_LIST_MANAGER _skillListManager;
		CMainUser* _mainUser;
		SoundLibrary* _soundLibrary;

		LeftHUD* _leftHUD;
		RightHUD* _rightHUD;
		NewItemsWindow* _newItemsWindow;
		CharacterStatsView* _statsView;
		UserSkillsView* _userSkillsView;
		NewSkillSelectionView* _skillSelectionView;
		DynamicInfoBox* _dynamicInfoBox;


		CustomUI::DragNDropSystem* _dragNDropSystem;
		CustomUI::MouseTrackingSpriteRenderable* _mouseTracking;
		DragNDropManager* _dragNDropManager;
		UserInventoryManager* _userInventoryManager;
		EquipItemsManager* _equipItemsManager;
		CharacterStatsManager* _statsManager;
		UserSkillsManager* _userSkillsManager;

		TooltipHelper* _tooltipHelper;
		TooltipLayer* _tooltipLayer;
		void updateLeftHUDWithSelectedLeftRightSkills();

		void toggleWindow(Renderable*);
		void hideWindow(Renderable*);
		void showWindow(Renderable*);

		void setupDisplacement(DisplacementHandleRenderable* handle, Renderable* forWindow);
	};
}

