#include <engine/serviceprovider.h>

namespace annileen
{
	Logger* ServiceProvider::s_LoggerService = nullptr;
	AssetManager* ServiceProvider::s_AssetManagerService = nullptr;
	Settings* ServiceProvider::s_SettingsService = nullptr;
	FontManager* ServiceProvider::s_FontManager = nullptr;
	TextBufferManager* ServiceProvider::s_TextBufferManager = nullptr;
	SceneManager* ServiceProvider::s_SceneManager = nullptr;

	void ServiceProvider::provideLogger(Logger* logger)
	{
		s_LoggerService = logger;
	}

	Logger* ServiceProvider::getLogger()
	{
		return s_LoggerService;
	}

	void ServiceProvider::provideAssetManager(AssetManager* assetManager)
	{
		s_AssetManagerService = assetManager;
	}

	AssetManager* ServiceProvider::getAssetManager()
	{
		return s_AssetManagerService;
	}

	void ServiceProvider::provideSettings(Settings* settings)
	{
		s_SettingsService = settings;
	}

	Settings* ServiceProvider::getSettings()
	{
		return s_SettingsService;
	}

	void ServiceProvider::provideFontManager(FontManager* fontManager)
	{
		s_FontManager = fontManager;

		if (s_TextBufferManager != nullptr)
		{
			delete s_TextBufferManager;
			s_TextBufferManager = nullptr;
		}

		if (s_FontManager != nullptr)
		{
			s_TextBufferManager = new TextBufferManager(s_FontManager);
		}
	}

	FontManager* ServiceProvider::getFontManager()
	{
		return s_FontManager;
	}

	TextBufferManager* ServiceProvider::getTextBufferManager()
	{
		return s_TextBufferManager;
	}

	void ServiceProvider::provideSceneManager(SceneManager* sceneManager)
	{
		if (s_SceneManager != nullptr)
		{
			delete s_SceneManager;
		}

		s_SceneManager = sceneManager;
	}
	
	SceneManager* ServiceProvider::getSceneManager()
	{
		return s_SceneManager;
	}
}
