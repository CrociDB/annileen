#pragma once

#include <list>
#include <vector>
#include <engine/scenenode.h>

namespace annileen
{
	class SceneNode;

	class EditorGui final
	{
		std::vector<const char*> m_ConsoleLoggingChannelsList;
		std::vector<const char*> m_ConsoleLoggingLevelsList;

		EditorGui();
		~EditorGui();

		bool m_ShowToolsWindow;
		bool m_ShowSceneHierarchyWindow;
		bool m_ShowSceneNodePropertiesWindow;
		bool m_ShowConsoleWindow;
		bool m_ShowSettingsWindow;

		// These will probably become a list when we start allowing multiple selection.
		SceneNode* m_SelectedSceneNode;		
		SceneNode* m_SceneNodeToBeRemoved;

		void initialize();
		void render(Scene* scene);
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
			newSceneNode = new SceneNode(scene, nodeName);
			sceneNode->getParent()->addChildBefore(newSceneNode, sceneNode);
		}
		if (ImGui::Selectable("As child"))
		{
			newSceneNode = new SceneNode(scene, nodeName);
			sceneNode->addChild(newSceneNode);
		}
		if (ImGui::Selectable("Below"))
		{
			newSceneNode = new SceneNode(scene, nodeName);
			sceneNode->getParent()->addChildAfter(newSceneNode, sceneNode);
		}

		if (newSceneNode != nullptr)
		{
			SceneNodeModule* newModule = newSceneNode->addModule<T>();

			// TODO: change text to not need this.
			if constexpr (std::is_same<T, Text>::value)
			{
				Text* text = static_cast<Text*>(newModule);
				text->init(false);
			}
		}
	}
}