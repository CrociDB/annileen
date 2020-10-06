#pragma once

//#include <engine/audio.h>
#include <engine/core/logger.h>
#include <engine/assetmanager.h>
#include <engine/settings.h>
#include <engine/text/fontmanager.h>
#include <engine/text/textbuffermanager.h>
#include <engine/scene/scenemanager.h>

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
		static Logger* s_LoggerService;
		static AssetManager* s_AssetManagerService;
		static Settings* s_SettingsService;
		static FontManager* s_FontManager;
		static TextBufferManager* s_TextBufferManager;
		static SceneManager* s_SceneManager;

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

		static void provideSceneManager(SceneManager* sceneManager);
		static SceneManager* getSceneManager();
	};
}
