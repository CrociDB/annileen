#include <engine/gui.h>

namespace annileen
{
	void Gui::beginFrame(glm::vec2 mousePos, uint8_t mouseButton, int32_t mouseScroll, uint32_t width, uint32_t height)
	{
		imguiBeginFrame(mousePos.x, mousePos.y, mouseButton, mouseScroll, width, height);
	}

	void Gui::endFrame()
	{
		imguiEndFrame();
	}

	Gui::Gui()
	{
		imguiCreate();
	}

	Gui::~Gui()
	{
		imguiDestroy();
	}
}