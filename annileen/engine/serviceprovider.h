#pragma once

//#include <engine/audio.h>
#include <engine/core/logger.h>
#include <engine/assetmanager.h>
#include <engine/settings.h>
#include <engine/text/fontmanager.h>
#include <engine/text/textbuffermanager.h>

namespace annileen
{
	class Logger;
	class AssetManager;
	class Settings;

	class ServiceProvider final
	{
	private:
		~ServiceProvider() {}
		ServiceProvider() {}

		//static Audio* m_audioService;
		static Logger* m_LoggerService;
		static AssetManager* m_AssetManagerService;
		static Settings* m_SettingsService;
		static FontManager* m_FontManager;
		static TextBufferManager* m_TextBufferManager;

	public:
		ServiceProvider(const ServiceProvider&) = delete;
		ServiceProvider& operator=(const ServiceProvider&) = delete;
		ServiceProvider(ServiceProvider&&) = delete;
		ServiceProvider& operator=(ServiceProvider&&) = delete;

		//static void initialize();

		//static void provideAudio(Audio* audio);
		//static Audio* getAudio();

		static void provideLogger(Logger* logger);
		static Logger* getLogger();

		static void provideAssetManager(AssetManager* assetManager);
		static AssetManager* getAssetManager();

		static void provideSettings(Settings* settings);
		static Settings* getSettings();

		static void provideFontManager(FontManager* fontManager);
		static FontManager* getFontManager();
		static TextBufferManager* getTextBufferManager();
	};
}