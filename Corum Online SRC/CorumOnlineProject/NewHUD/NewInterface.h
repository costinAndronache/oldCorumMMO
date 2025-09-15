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
#include "ViewManager/ExitOptionsWindowManager.h"
#include "ViewManager/MinimapWindowManager.h"
#include "ViewManager/CharDeathConfirmationManager.h"
#include "ViewManager/InfoBarManager.h"
#include "ViewManager/SkillSelectionManager.h"

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
			std::shared_ptr<SharedNetwork> network,
			std::shared_ptr<TooltipHelper> tooltipHelper,
			std::shared_ptr<GameExitManager> exitManager,
			I4DyuchiGXExecutive* executive,
			std::shared_ptr<UserPreferencesManager> userPreferencesManager
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
		void updatedPosition(CMainUser*) override;
		void updatedStatus(CMainUser*, UNIT_STATUS) override;

	public:
		virtual void renderOnMouseCursorAvatar(std::shared_ptr<Renderable> avatar, 
			std::function<void(CustomUI::Rect avatarCurrentGlobalFrame)> onLeftMouseButtonUP
		);
		virtual void clearCurrentMouseCursorAvatar();
		void updateForLayer(MinimapWindowManager::Layer layer);
		void processCurrentHovering(GXOBJECT_HANDLE hoveredObject);
	private:
		std::shared_ptr<Interface> _thisAsShared;
		LP_SKILL_LIST_MANAGER _skillListManager;
		CMainUser* _mainUser;
		SoundLibrary* _soundLibrary;

		std::shared_ptr<LeftHUD> _leftHUD;
		std::shared_ptr<RightHUD> _rightHUD;
		std::shared_ptr<NewItemsWindow> _newItemsWindow;
		std::shared_ptr<CharacterStatsView> _statsView;
		std::shared_ptr<UserSkillsView> _userSkillsView;
		std::shared_ptr<NewSkillSelectionView> _skillSelectionView;
		std::shared_ptr<ExitOptionsWindow> _exitOptionsWindow;
		std::shared_ptr<MinimapWindow> _minimapWindow;
		std::shared_ptr<ConfirmationModal> _charDeathConfirmationModal;
		std::shared_ptr<HPInfoBarView> _hpInfoBarView;

		std::shared_ptr<CustomUI::DragNDropSystem> _dragNDropSystem;
		std::shared_ptr<CustomUI::MouseTrackingSpriteRenderable> _mouseTracking;
		std::shared_ptr<DragNDropManager> _dragNDropManager;
		std::shared_ptr<UserInventoryManager> _userInventoryManager;
		std::shared_ptr<EquipItemsManager> _equipItemsManager;
		std::shared_ptr<CharacterStatsManager> _statsManager;
		std::shared_ptr<UserSkillsManager> _userSkillsManager;
		std::shared_ptr<ExitOptionsWindowManager> _exitOptionsManager;
		std::shared_ptr<MinimapWindowManager> _minimapWindowManager;
		std::shared_ptr<CharDeathConfirmationManager> _charDeathConfirmationManager;
		std::shared_ptr<InfoBarManager> _hpInfoBarManager;
		std::shared_ptr<SkillSelectionManager> _skillSelectionManager;

		std::shared_ptr<TooltipHelper> _tooltipHelper;
		std::shared_ptr<TooltipLayer> _tooltipLayer;
		void updateLeftHUDWithSelectedLeftRightSkills();

		void toggleWindow(Renderable*);
		void hideWindow(Renderable*);
		void showWindow(Renderable*);

		void setupDisplacement(
			std::shared_ptr<DisplacementHandleRenderable> handle, 
			std::weak_ptr<Renderable> forWindow
		);
	};
}

