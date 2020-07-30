#pragma once

//#include <engine/audio.h>
#include <engine/core/logger.h>

namespace annileen
{
	class ServiceProvider final
	{
	private:
		~ServiceProvider() {}
		ServiceProvider() {}

		//static Audio* m_audioService;
		static Logger* m_loggerService;

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
	};
}