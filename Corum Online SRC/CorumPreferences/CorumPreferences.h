#pragma once

#include <functional>
#include <vector>

namespace CorumPreferences {
	using Data = std::vector<uint8_t>;
	struct Resolution { int width; int height; };

	class Preferences {
	public:
		static Preferences* buildFromData(const Data& data);
		static Preferences* buildFromFile(const char* filePath);
		static Preferences defaults();

		static const char* defaultFileName() { return "CorumPrefs.prefs"; } 

	public:
		Preferences();
        bool windowMode() const { return _windowMode; };
		Resolution resolution() const { return _resolution; }

		void storeWindowMode(bool wm) { _windowMode = wm; }
		void storeResolution(Resolution r) { _resolution = r; }

		Data serialize() const;

		void serializeToFile(const char* filePath) const;

	private:
		bool _windowMode;
		Resolution _resolution;
	};
}