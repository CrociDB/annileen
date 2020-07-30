#include <engine/serviceprovider.h>

namespace annileen
{
	Logger* ServiceProvider::m_loggerService = nullptr;

	void ServiceProvider::provideLogger(Logger* logger)
	{
		m_loggerService = logger;
	}

	Logger* ServiceProvider::getLogger()
	{
		return m_loggerService;
	}
}