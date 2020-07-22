#pragma once

#include"imgui-utils/imgui.h"
#include<glm.hpp>
#include <list>
#include <engine/scenenode.h>
#include <engine/engine.h>

namespace annileen
{
	class Gui
	{
	public:
		void init();
		void destroy();

		void beginFrame(glm::vec2 mousePos, uint8_t mouseButton, int32_t mouseScroll, uint32_t width, uint32_t height);
		void endFrame();

		void drawMainWindowToolbar();
		void drawEditorGeneralInfoWindow();
		void drawEditorSceneTreeWindow(const std::list<SceneNodePtr> sceneNodeList) const;
		void drawSelectedNodePropertiesWindow();

		void _drawTree(SceneNodePtr const sceneNode) const;
		
		Gui();
		~Gui();
	};
}

