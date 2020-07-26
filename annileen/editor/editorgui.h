#pragma once

#include <list>
#include <engine/scenenode.h>

namespace annileen
{
	class SceneNode;

	class EditorGui
	{
		EditorGui();
		~EditorGui();
	public:

		static void drawMainWindowToolbar();
		static void drawEditorGeneralInfoWindow();
		static void drawEditorSceneTreeWindow(const std::list<SceneNodePtr> sceneNodeList);
		static void drawSelectedNodePropertiesWindow();
		static void _drawTree(SceneNodePtr const sceneNode);
	};
}