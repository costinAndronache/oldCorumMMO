#pragma once

#include "../InitGame.h"

class SoundLibrary {
public:
	static SoundLibrary* sharedInstance();
	SoundLibrary();

	void playItemMousePickUp();
	void playItemMouseDropInInterface();
	void playItemMouseDropInTile();
	void playStatPointUp();
	void playSkillPointUp();
	void playWindowOpen();
	void playWindowClose();
	void playButtonClick();
private:
	IMilesSoundLib* _soundLib;
};

