#include "application.h"

Application::Application() : m_Engine(nullptr) {}
Application::~Application() {}

void Application::initAnnileen()
{
	m_Engine = annileen::Engine::getInstance();
	m_Engine->init(1920, 1080, "build_assets/assets.toml");
}

int Application::run()
{
	initAnnileen();

	annileen::Scene* scene = init();

	// Put assert for scene nullptr once we have our assert class
	if (scene == nullptr)
	{
		std::cerr << "Ops. Scene is null!" << std::endl;
		return 1;
	}

	m_Engine->setScene(scene);

	scene->start();

	while (m_Engine->run())
	{
		auto dt = m_Engine->getTime().deltaTime;
		m_Engine->checkInputEvents();
		scene->update();

		update(dt);

		m_Engine->renderFrame();
	}

	return 0;
}

void Application::destroy()
{
	finish();
}