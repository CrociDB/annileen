#pragma once

#include <list>
#include <engine/scenenode.h>

namespace annileen
{
	class SceneNode;

	class EditorGui final
	{
		EditorGui() = delete;
		~EditorGui() = delete;
	public:

		static void drawMainWindowToolbar();
		static void drawEditorGeneralInfoWindow();
		static void drawEditorSceneTreeWindow(const std::list<SceneNodePtr> sceneNodeList);
		static void drawSelectedNodePropertiesWindow();
		static void drawConsoleWindow();
		static void _drawTree(SceneNodePtr const sceneNode);
	};
}