#include "application.h"
#include "text/text.h"

namespace annileen
{
	Application::Application() : m_Engine(nullptr), m_NoCamera(nullptr), m_NoCameraText(nullptr) {}
	Application::~Application() {}

	void Application::initAnnileen()
	{
		m_Engine = annileen::Engine::getInstance();
		m_Engine->init(1920, 1080, "build_assets/assets.toml", "build_assets/settings.toml", m_ApplicationName);
	}

	int Application::run(std::string applicationName)
	{
		m_ApplicationName = applicationName;

		initAnnileen();


		annileen::Scene* scene = init();

		// Put assert for scene nullptr once we have our assert class
		if (scene == nullptr)
		{
			std::cerr << "Ops. Scene is null!" << std::endl;
			return 1;
		}

		scene->start();
		m_Engine->setScene(scene);

#ifdef _DEBUG
		initializeEditorGui();
#endif

		SceneNodePtr cameraNode = m_Engine->getScene()->createNode("No camera");
		cameraNode->m_Internal = true;
		m_NoCamera = cameraNode->addModule<Camera>();
		m_NoCamera->fieldOfView = 60.0f;
		m_NoCamera->nearClip = 0.01f;
		m_NoCamera->farClip = 0.02f;
		m_NoCamera->getTransform().translate(glm::vec3(-5.0f, 0.0f, -5.0f));
		m_NoCamera->setForward(glm::vec3(0, 0, 1));
		m_NoCamera->clearColor = glm::vec3(0, 0, 0);
		cameraNode->flags = SceneNodeFlags_Hide;
		
		SceneNodePtr TextNode = m_Engine->getScene()->createNode("No camera text");
		TextNode->m_Internal = true;
		m_NoCameraText = TextNode->addModule<Text>();
		m_NoCameraText->setStatic(true);
		m_NoCameraText->setScreenPosition(Engine::getInstance()->getWidth()/2.0f - 100.0f, Engine::getInstance()->getHeight()/2.0f);
		m_NoCameraText->setTextColor(glm::vec3(1, 1, 1));
		m_NoCameraText->setStyle(Text::TextStyle::Normal);
		m_NoCameraText->setText("No Camera");
		TextNode->setParent(cameraNode);

		cameraNode->setActive(false);

		while (m_Engine->run())
		{
			auto dt = m_Engine->getTime().deltaTime;
			m_Engine->checkInputEvents();

			uint8_t mouseButton = (m_Engine->getInput()->_getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) ? IMGUI_MBUT_LEFT : 0)
				| (m_Engine->getInput()->_getMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT) ? IMGUI_MBUT_RIGHT : 0)
				| (m_Engine->getInput()->_getMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE) ? IMGUI_MBUT_MIDDLE : 0);

			m_Engine->getGui()->beginFrame(m_Engine->getInput()->_getMousePosition(), mouseButton, static_cast<int32_t>(m_Engine->getInput()->_getMouseScroll().y), 
				m_Engine->getWidth(), m_Engine->getHeight());

			scene->update();

#ifdef _DEBUG
			editorUpdate(dt);
#else
			update(dt);
#endif

			m_Engine->getGui()->endFrame();

			render();
		}

		return 0;
	}

	void Application::render()
	{
		Camera* camera = getEngine()->getScene()->getCamera();
		if (camera == nullptr)
		{
			m_NoCamera->getSceneNode()->setActive(true);
			m_Engine->render(m_NoCamera);	
			m_NoCamera->getSceneNode()->setActive(false);
		}
		else
		{
			m_Engine->render(camera);
		}
	}

	void Application::destroy()
	{
		finish();
		Engine::destroy();
	}
}
