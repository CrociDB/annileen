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

		SceneNode* m_SelectedSceneNode;

		void initialize();
		void render(Scene* scene);
		void drawMainWindowToolbar();
		void drawToolsWindow();
		void drawSceneHierarchyWindow(const std::list<SceneNodePtr> sceneNodeList);
		void drawSelectedNodePropertiesWindow();
		void drawConsoleWindow();
		void drawSettingsWindow();
		void _drawTree(SceneNodePtr const sceneNode);

		friend class ApplicationEditor;
	};
}