#include "application.h"

namespace annileen
{
	Application::Application() : m_Engine(nullptr) {}
	Application::~Application() {}

	void Application::initAnnileen()
	{
		m_Engine = annileen::Engine::getInstance();
		m_Engine->init(1920, 1080, "build_assets/assets.toml");
	}

	void Application::gui()
	{
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

			uint8_t mouseButton = (m_Engine->getInput()->getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) ? IMGUI_MBUT_LEFT : 0)
				| (m_Engine->getInput()->getMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT) ? IMGUI_MBUT_RIGHT : 0)
				| (m_Engine->getInput()->getMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE) ? IMGUI_MBUT_MIDDLE : 0);

			m_Engine->getGui()->beginFrame(m_Engine->getInput()->getMousePosition(), mouseButton, m_Engine->getInput()->getMouseScroll().y, m_Engine->getWidth(), m_Engine->getHeight());

			scene->update();

			update(dt);

			gui();

			m_Engine->getGui()->endFrame();

			m_Engine->renderFrame();
		}

		return 0;
	}

	void Application::destroy()
	{
		finish();
	}
}