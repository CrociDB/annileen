module;

#include "imgui-utils/imgui.h"
#include <engine/core/glfw_imgui_utils.h>
#include <string>
#include <iostream>
#include <glm.hpp>

#include <GLFW/glfw3.h>
// GLFW will include xserver headers in Linux and it defines macro `None`. So including
// this math header before fixes the redefinition of this macro.
#include <bx/math.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>

#include "engine/forward_decl.h"

export module application;

import text;
import engine;
import camera;
import scenemanager;
import scene;
import scenenode;
import input;
import gui;

export namespace annileen
{
	class Application
	{
	public:
		// ApplicationEditor has to be able to inject the editor gui stuff
	#ifdef _ANNILEEN_COMPILER_EDITOR
		friend class ApplicationEditor;
	#endif

	public:
		Application() = default;
		virtual ~Application();

	// ApplicationEditor has to be able to inject the editor gui stuff
	#ifdef _ANNILEEN_COMPILER_EDITOR
		virtual void initializeEditorGui(std::shared_ptr<Scene> scene) = 0;
		virtual void editorUpdate(std::shared_ptr<Scene> scene, float deltaTime) = 0;
	#endif

	private:
		void initAnnileen();

	protected:
		virtual std::shared_ptr<Scene> init() = 0;
		virtual void finish() = 0;
		virtual void update(float deltaTime) = 0;
		virtual void render();

		Engine* const getEngine() const;

	public:
		int run(const std::string& applicationName);

	private:
		Engine* m_Engine{ nullptr };
		std::string m_ApplicationName{ "" };
		Camera* m_NoCamera{ nullptr };
		// This is temporary, the scenenode should be activated/deactivated instead.
		Text* m_NoCameraText{ nullptr };
	};
}

namespace annileen
{
	Engine* const Application::getEngine() const
	{
		return m_Engine;
	}

	void Application::initAnnileen()
	{
		m_Engine = annileen::Engine::getInstance();
		m_Engine->init("build_assets/assets.toml", "build_assets/settings.toml", m_ApplicationName);
	}

	int Application::run(const std::string& applicationName)
	{
		m_ApplicationName = std::move(applicationName);

		initAnnileen();

		std::shared_ptr<Scene> scene = init();

		// Put assert for scene nullptr once we have our assert class
		if (scene == nullptr)
		{
			std::cerr << "Ops. Scene is null!" << std::endl;
			return 1;
		}

		scene->start();

#ifdef _ANNILEEN_COMPILER_EDITOR
		initializeEditorGui(scene);
#endif

		auto cameraNode = scene->createNode("No camera");
		cameraNode->m_Internal = true;
		m_NoCamera = SceneManager::getInstance()->addModule<Camera>(scene, cameraNode);
		m_NoCamera->fieldOfView = 60.0f;
		m_NoCamera->nearClip = 0.01f;
		m_NoCamera->farClip = 0.02f;
		m_NoCamera->getTransform().translate(glm::vec3(-5.0f, 0.0f, -5.0f));
		m_NoCamera->setForward(glm::vec3(0, 0, 1));
		m_NoCamera->clearColor = glm::vec3(0, 0, 0);
		cameraNode->flags = SceneNodeFlags_Hide;

		auto textNode = scene->createNode("No camera text");
		textNode->m_Internal = true;
		m_NoCameraText = SceneManager::getInstance()->addModule<Text>(scene, textNode);
		m_NoCameraText->setStatic(true);
		m_NoCameraText->setScreenPosition(getEngine()->getWidth() / 2.0f - 100.0f, getEngine()->getHeight() / 2.0f);
		m_NoCameraText->setTextColor(glm::vec3(1, 1, 1));
		m_NoCameraText->setStyle(Text::TextStyle::Normal);
		m_NoCameraText->setText("No Camera");
		textNode->setParent(cameraNode);

		cameraNode->setActive(false);

		while (getEngine()->run())
		{
			auto dt{ getEngine()->getTime().deltaTime };
			getEngine()->checkInputEvents();

			uint8_t mouseButton = (getEngine()->getInput()->_getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) ? IMGUI_MBUT_LEFT : 0)
				| (getEngine()->getInput()->_getMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT) ? IMGUI_MBUT_RIGHT : 0)
				| (getEngine()->getInput()->_getMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE) ? IMGUI_MBUT_MIDDLE : 0);

			getEngine()->getGui()->beginFrame(
				getEngine()->getInput()->_getMousePosition(),
				mouseButton, 
				static_cast<int32_t>(getEngine()->getInput()->_getMouseScroll().y),
				getEngine()->getWidth(), getEngine()->getHeight());

			scene->update();

#ifdef _ANNILEEN_COMPILER_EDITOR
			editorUpdate(scene, dt);
#else
			update(dt);
#endif

			getEngine()->getGui()->endFrame();

			render();
		}

		finish();

		return 0;
	}

	void Application::render()
	{
		Camera* camera{ SceneManager::getInstance()->getScene()->getCamera() };
		if (camera == nullptr)
		{
			m_NoCamera->getSceneNode()->setActive(true);
			getEngine()->render(m_NoCamera);
			m_NoCamera->getSceneNode()->setActive(false);
		}
		else
		{
			getEngine()->render(camera);
		}
	}

	Application::~Application()
	{
		Engine::destroy();
	}
}