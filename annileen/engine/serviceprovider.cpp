#include <engine/serviceprovider.h>

namespace annileen
{
	Logger* ServiceProvider::m_LoggerService = nullptr;
	AssetManager* ServiceProvider::m_AssetManagerService = nullptr;
	Settings* ServiceProvider::m_SettingsService = nullptr;
	FontManager* ServiceProvider::m_FontManager = nullptr;
	TextBufferManager* ServiceProvider::m_TextBufferManager = nullptr;

	void ServiceProvider::provideLogger(Logger* logger)
	{
		m_LoggerService = logger;
	}

	Logger* ServiceProvider::getLogger()
	{
		return m_LoggerService;
	}

	void ServiceProvider::provideAssetManager(AssetManager* assetManager)
	{
		m_AssetManagerService = assetManager;
	}

	AssetManager* ServiceProvider::getAssetManager()
	{
		return m_AssetManagerService;
	}

	void ServiceProvider::provideSettings(Settings* settings)
	{
		m_SettingsService = settings;
	}

	Settings* ServiceProvider::getSettings()
	{
		return m_SettingsService;
	}

	void ServiceProvider::provideFontManager(FontManager* fontManager)
	{
		m_FontManager = fontManager;

		if (m_TextBufferManager != nullptr)
		{
			delete m_TextBufferManager;
			m_TextBufferManager = nullptr;
		}

		if (m_FontManager != nullptr)
		{
			m_TextBufferManager = new TextBufferManager(m_FontManager);
		}
	}

	FontManager* ServiceProvider::getFontManager()
	{
		return m_FontManager;
	}

	TextBufferManager* ServiceProvider::getTextBufferManager()
	{
		return m_TextBufferManager;
	}
}