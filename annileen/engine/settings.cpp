#include <engine/settings.h>

namespace annileen
{
	const char* Settings::settingsFileName = "TODO.txt";

	Settings::Settings()
	{
		shadows.enabled = true;
		shadows.shadowMapSize = 1024;

		loadSettings();
	}

	Settings::~Settings()
	{
		saveSettings();
	}

	void Settings::loadSettings()
	{
		// TODO: load settings from settings file.
	}

	void Settings::saveSettings()
	{
		// TODO: save settings to settings file.		
	}
}
