#pragma once

#include <imgui-utils/imgui.h>
#include <glm.hpp>

namespace annileen
{
	class Gui final
	{
	public:
		void beginFrame(glm::vec2 mousePos, uint8_t mouseButton, int32_t mouseScroll, uint32_t width, uint32_t height);
		void endFrame();

	private:
		Gui();
		~Gui();

		friend class Engine;
	};
}

