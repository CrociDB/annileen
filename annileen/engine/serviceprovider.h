#pragma once

//#include <engine/audio.h>
#include <engine/core/logger.h>
#include <engine/assetmanager.h>

namespace annileen
{
	class Logger;
	class AssetManager;

	class ServiceProvider final
	{
	private:
		~ServiceProvider() {}
		ServiceProvider() {}

		//static Audio* m_audioService;
		static Logger* m_LoggerService;
		static AssetManager* m_AssetManagerService;

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
	};
}