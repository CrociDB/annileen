#pragma once

#include <imgui-utils/imgui.h>
#include <glm.hpp>

namespace annileen
{
	class Gui
	{
	public:
		void init();
		void destroy();

		void beginFrame(glm::vec2 mousePos, uint8_t mouseButton, int32_t mouseScroll, uint32_t width, uint32_t height);
		void endFrame();

		Gui();
		~Gui();
	};
}

