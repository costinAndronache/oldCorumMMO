#pragma once
#include "../../CorumPreferences/CorumPreferences.h"
#include <memory>
#include <string>

class UserPreferencesManager {
public:
	UserPreferencesManager(const wchar_t* appDirectory);
	std::shared_ptr<CorumPreferences::Preferences> currentPreferences();
	void persistChanges(std::function<void(std::shared_ptr<CorumPreferences::Preferences>)> changeBlock);

private:
	std::shared_ptr<CorumPreferences::Preferences> _currentPreferences;
	std::wstring _prefsPath;
};

