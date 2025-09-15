#include "UserPreferencesManager.h"

using namespace CorumPreferences;

UserPreferencesManager::UserPreferencesManager(const wchar_t* appDirectory) {
	_prefsPath = appDirectory;

	_prefsPath.append(L"\\");
	_prefsPath.append(Preferences::defaultFileName());

	auto prefs = Preferences::buildFromFile(_prefsPath.c_str());
	_currentPreferences = std::shared_ptr<Preferences>(prefs);

}

std::shared_ptr<CorumPreferences::Preferences> UserPreferencesManager::currentPreferences() { 
	return _currentPreferences; 
}

void UserPreferencesManager::persistChanges(
	std::function<void(std::shared_ptr<CorumPreferences::Preferences>)> changeBlock) {
	changeBlock(_currentPreferences);
	_currentPreferences->serializeToFile(_prefsPath.c_str());
}