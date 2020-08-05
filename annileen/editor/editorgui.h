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

		void initialize();
		void drawMainWindowToolbar();
		void drawEditorGeneralInfoWindow();
		void drawEditorSceneTreeWindow(const std::list<SceneNodePtr> sceneNodeList);
		void drawSelectedNodePropertiesWindow();
		void drawConsoleWindow();
		void _drawTree(SceneNodePtr const sceneNode);

		friend class ApplicationEditor;
	};
}