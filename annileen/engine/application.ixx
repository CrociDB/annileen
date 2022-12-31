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
		Application() : m_Engine(nullptr), m_NoCamera(nullptr), m_NoCameraText(nullptr) {}
		~Application() {}

		Engine* m_Engine;
		std::string m_ApplicationName;

		Camera* m_NoCamera;
		// This is temporary, the scenenode should be actiaved/deactivated instead.
		Text* m_NoCameraText;

	// ApplicationEditor has to be able to inject the editor gui stuff
	#ifdef _ANNILEEN_COMPILER_EDITOR
		friend class ApplicationEditor;
		virtual void initializeEditorGui() = 0;
		virtual void editorUpdate(float deltaTime) = 0;
	#endif

	private:
		void initAnnileen()
		{
			m_Engine = annileen::Engine::getInstance();
			m_Engine->init(1920, 1080, "build_assets/assets.toml", "build_assets/settings.toml", m_ApplicationName);
		}

	public:
		int run(std::string applicationName)
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
			getEngine()->getSceneManager()->setScene(scene);
	
	#ifdef _ANNILEEN_COMPILER_EDITOR
			initializeEditorGui();
	#endif
	
			SceneNodePtr cameraNode = getEngine()->getSceneManager()->getScene()->createNode("No camera");
			cameraNode->m_Internal = true;
			m_NoCamera = cameraNode->addModule<Camera>();
			m_NoCamera->fieldOfView = 60.0f;
			m_NoCamera->nearClip = 0.01f;
			m_NoCamera->farClip = 0.02f;
			m_NoCamera->getTransform().translate(glm::vec3(-5.0f, 0.0f, -5.0f));
			m_NoCamera->setForward(glm::vec3(0, 0, 1));
			m_NoCamera->clearColor = glm::vec3(0, 0, 0);
			cameraNode->flags = SceneNodeFlags_Hide;
			
			SceneNodePtr TextNode = getEngine()->getSceneManager()->getScene()->createNode("No camera text");
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
	
	#ifdef _ANNILEEN_COMPILER_EDITOR
				editorUpdate(dt);
	#else
				update(dt);
	#endif
	
				m_Engine->getGui()->endFrame();
	
				render();
			}

			return 0;
		}

	protected:
		virtual Scene* init() = 0;
		virtual void finish() = 0;
		virtual void update(float deltaTime) = 0;

		inline Engine* const getEngine() const
		{
			return m_Engine;
		}

		virtual void render()
		{
			Camera* camera = getEngine()->getSceneManager()->getScene()->getCamera();
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

		void destroy()
		{
			finish();
			Engine::destroy();
		}
	};
}
