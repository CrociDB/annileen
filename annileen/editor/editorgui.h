#pragma once

#include <list>
#include <vector>
#include <engine/scenenode.h>

#include <dear-imgui/imgui.h>

namespace annileen
{
	class SceneNode;

	class EditorGui final
	{
		enum Mode
		{
			Editor,
			Game
		};

		struct InputConfig
		{
			float mouseSpeed;
			float movementSpeed;
			float sensitivity;
			float yaw;
			float pitch;
			int debugScreenActive;
		};

		InputConfig m_InputConfig;

		std::vector<const char*> m_ConsoleLoggingChannelsList;
		std::vector<const char*> m_ConsoleLoggingLevelsList;

		EditorGui();
		~EditorGui();

		bool m_ShowToolsWindow;
		bool m_ShowSceneHierarchyWindow;
		bool m_ShowSceneNodePropertiesWindow;
		bool m_ShowConsoleWindow;
		bool m_ShowSettingsWindow;

		bool m_AssetHotreload;

		bool m_HasWindowFocused;

		Mode m_Mode;

		// These will probably become a list when we start allowing multiple selection.
		SceneNode* m_SelectedSceneNode;		
		SceneNode* m_SceneNodeToBeRemoved;

		void initialize();
		void processInput(Camera* camera, float deltaTime);
		void render(Scene* scene, float deltaTime);
		void drawMainWindowToolbar();
		void drawToolsWindow();
		void drawSceneHierarchyWindow(const std::vector<SceneNodePtr> sceneNodeList);
		void drawSelectedNodePropertiesWindow();
		void drawConsoleWindow();
		void drawSettingsWindow();
		void _drawTree(SceneNodePtr const sceneNode);

		// Modules
		void drawModelModuleProperties(Model* model);
		void drawLightModuleProperties(Light* light);
		void drawCameraModuleProperties(Camera* camera);
		void drawTextModuleProperties(Text* text);
		void drawModelMaterialProperties(std::shared_ptr<Material> material);

		void drawSceneNodeContextMenu(SceneNodePtr const sceneNode);
		
		template <class T>
		void drawNewSceneNodeContextMenu(SceneNodePtr const sceneNode, std::string nodeName);
		friend class ApplicationEditor;
	};

	
	template <class T>
	void EditorGui::drawNewSceneNodeContextMenu(SceneNodePtr const sceneNode, std::string nodeName)
	{
		Scene* scene = sceneNode->getParentScene();
		SceneNodePtr newSceneNode = nullptr;

		if (ImGui::Selectable("Above"))
		{
			newSceneNode = scene->createNode(nodeName);
			newSceneNode->setParent(sceneNode->getParent());
			newSceneNode->setSiblingPosition(sceneNode->getSiblingIterator());
		}
		if (ImGui::Selectable("As child"))
		{
			newSceneNode = scene->createNode(nodeName);
			newSceneNode->setParent(sceneNode);
		}
		if (ImGui::Selectable("Below"))
		{
			newSceneNode = scene->createNode(nodeName);
			newSceneNode->setParent(sceneNode->getParent());
			newSceneNode->setSiblingPosition(sceneNode->getSiblingIterator() + 1);
		}

		if (newSceneNode != nullptr)
		{
			SceneNodeModule* newModule = newSceneNode->addModule<T>();
		}
	}
}
