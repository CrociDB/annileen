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
		void drawNewSceneNodeContextMenu(SceneNodePtr const sceneNode);
		friend class ApplicationEditor;
	};
}