#pragma once

#include "../CharacterStatsView.h"
#include "../../InitGame.h"
#include "../../NewHelpers/StatusPointManager.h"
#include "../../NewHelpers/SoundLibrary.h"
namespace NewInterface {
	class CharacterStatsManager {
	public:
		using IncreasedAttribute = STATUS_POINT_KIND;
		using AttributeIncreaseHandler = std::function<void(IncreasedAttribute)>;
		CharacterStatsManager(
			CharacterStatsView* managedView,
			StatusPointManager* _statusPointManager,
			CMainUser* mainUser,
			SoundLibrary* soundLibrary
		);

		void refreshCharacterStats();
		static int maxEntryCount();
	private:
		using Model = CharacterAttributeView::Model;
		using ModelCreate = std::function<Model(CMainUser*, AttributeIncreaseHandler)>;
		class OneAttributeUpdater {
		public:
            OneAttributeUpdater(ModelCreate modelCreate);
			CharacterAttributeView::UpdateProxy* characterAttributeProxy();

			void refresh(CMainUser *, AttributeIncreaseHandler);
		private:
			CharacterAttributeView::UpdateProxy* _characterAttributeProxy;
			ModelCreate _modelCreate;
		};

		static std::vector<std::vector<ModelCreate>> _entriesGenerators;
        static const std::vector<std::vector<ModelCreate>>& entriesGenerators();

		StatusPointManager* _statusPointManager;
		CharacterStatsView* _managedView;
		CMainUser* _mainUser;
		SoundLibrary* _soundLibrary;

		std::vector<std::vector<OneAttributeUpdater>> _attributeUpdaters;
		void refreshCharacterStats(AttributeIncreaseHandler handler);
	};
}

