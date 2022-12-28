module;

#pragma warning( push )
// Suppressing strcpy warning
#pragma warning( disable : 4996)
#include <imgui-utils/imgui.h>
#pragma warning( pop )

#include <glm.hpp>

export module gui;

export namespace annileen
{
	class Gui final
	{
	public:
		void beginFrame(glm::vec2 mousePos, uint8_t mouseButton, int32_t mouseScroll, uint16_t width, uint16_t height);
		void endFrame();

	private:
		Gui();
		~Gui();

		friend class Engine;
	};
}

namespace annileen
{
	void Gui::beginFrame(glm::vec2 mousePos, uint8_t mouseButton, int32_t mouseScroll, uint16_t width, uint16_t height)
	{
		imguiBeginFrame(static_cast<int32_t>(mousePos.x), static_cast<int32_t>(mousePos.y), mouseButton, mouseScroll, width, height);
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