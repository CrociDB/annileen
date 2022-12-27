module;

#include <imgui-utils/imgui.h>
#include <glm.hpp>

export module gui;

export namespace annileen
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