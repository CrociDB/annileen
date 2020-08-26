#include <engine/settings.h>

namespace annileen
{
	const char* Settings::settingsFileName = "TODO.txt";

	Settings::Settings() : shadows(true)
	{
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
