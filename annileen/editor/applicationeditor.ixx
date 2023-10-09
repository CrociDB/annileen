module;

#include <tuple>
#include <memory>
#include <glm.hpp>

export module anni.applicationeditor;

import anni.logger;
import anni.editorgui;
import anni.application;
import anni.scenemanager;
import anni.scenenode;
import anni.serviceprovider;

export namespace annileen
{
	class ApplicationEditor : public Application
	{
	private:
		std::unique_ptr<EditorGui> m_EditorGui;
		std::shared_ptr<Camera> m_EditorCamera;

		virtual void initializeEditorGui(const std::shared_ptr<Scene>& scene);
		virtual void editorUpdate(const std::shared_ptr<Scene>& scene, float deltaTime);

	protected:
		void render() override;

	public:

		bool showEditorGui;

		ApplicationEditor();
		virtual ~ApplicationEditor();
	};
}

module :private;

namespace annileen
{
	ApplicationEditor::ApplicationEditor() : showEditorGui(true), m_EditorCamera(nullptr)
	{
		m_EditorGui = std::make_unique<EditorGui>();
	}

	ApplicationEditor::~ApplicationEditor()
	{
	}

	void ApplicationEditor::initializeEditorGui(const std::shared_ptr<Scene>& scene)
	{
		m_EditorGui->initialize(m_Engine);
		
		auto cameraNode = scene->createNode("Editor Camera");
		cameraNode->m_Internal = true;
		m_EditorCamera = SceneManager::getInstance()->addModule<Camera>(scene, cameraNode);
		m_EditorCamera->fieldOfView = 60.0f;
		m_EditorCamera->nearClip = 0.1f;
		m_EditorCamera->farClip = 300.0f;
		m_EditorCamera->getTransform().translate(glm::vec3(-5.0f, 0.0f, -5.0f));
		m_EditorCamera->setForward(glm::vec3(0, 0, 1));
		cameraNode->flags = SceneNodeFlags_Hide;
		
		ANNILEEN_LOG_INFO(LoggingChannel::General, "Initialized Editor GUI");	 	
	}

	void ApplicationEditor::editorUpdate(const std::shared_ptr<Scene>& scene, float deltaTime)
	{
		if (m_EditorGui->m_Mode == EditorGui::Mode::Game)
		{
			update(deltaTime);
		}
		if (m_EditorGui->m_Mode == EditorGui::Mode::Editor)
		{
			m_EditorGui->processInput(m_EditorCamera, deltaTime);
		}
		if (showEditorGui)
		{
			m_EditorGui->render(scene, m_EditorCamera, deltaTime);
		}
		if (m_EditorGui->m_AssetHotreload)
		{
			ServiceProvider::getAssetManager()->updateAssetWatcher();
		}

		if (m_EditorGui->m_RenderSkybox && scene->getSkybox().get() != nullptr)
			m_EditorCamera->clearType = CameraClearType::CameraClearSkybox;
		else
			m_EditorCamera->clearType = CameraClearType::CameraClearColor;
	}

	void ApplicationEditor::render()
	{
		if (m_EditorGui->m_Mode == EditorGui::Mode::Game)
		{
			Application::render();
		}
		else
		{
			getEngine()->render(m_EditorCamera);
		}
	}
}