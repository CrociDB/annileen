#pragma once

#include"imgui-utils/imgui.h"

namespace annileen
{
	class Gui
	{
	public:
		void init();
		void destroy();

		void beginFrame(uint32_t width, uint32_t height);
		void endFrame();

		void drawSomeGUI();

		Gui();
		~Gui();
	};
}

