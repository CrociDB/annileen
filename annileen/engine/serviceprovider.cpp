#include <engine/serviceprovider.h>

namespace annileen
{
	Logger* ServiceProvider::m_LoggerService = nullptr;
	AssetManager* ServiceProvider::m_AssetManagerService = nullptr;

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
}