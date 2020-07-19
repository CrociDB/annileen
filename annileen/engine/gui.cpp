#include "gui.h"

using namespace annileen;

void Gui::init()
{
	imguiCreate();
}

void Gui::destroy()
{
	imguiDestroy();
}

void Gui::beginFrame(uint32_t width, uint32_t height)
{
	imguiBeginFrame(0,0,0,0, width, height);
}

void Gui::drawSomeGUI()
{
	char temp[1024] = "Some GUI is being drawn!";

	ImGui::SetNextWindowPos(
		ImVec2(10.0f, 50.0f)
		, ImGuiCond_FirstUseEver
	);
	ImGui::SetNextWindowSize(
		ImVec2(300.0f, 210.0f)
		, ImGuiCond_FirstUseEver
	);

	ImGui::Begin(temp);

	ImGui::TextWrapped("Testing Text Wrapped");

	ImGui::Separator();

	ImGui::TextWrapped("Now some buttons");

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(3.0f, 3.0f));
	if (ImGui::Button(ICON_FA_REPEAT " Restart"))
	{
	}
	
	ImGui::SameLine();
	if (ImGui::Button(ICON_KI_PREVIOUS " Prev"))
	{
	}

	ImGui::SameLine();
	if (ImGui::Button(ICON_KI_NEXT " Next"))
	{
	}
	
	ImGui::SameLine();
	if (ImGui::Button(ICON_KI_EXIT " Exit"))
	{
	}

	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_BAR_CHART))
	{
	}

	ImGui::PopStyleVar();

	ImGui::End();
}

void Gui::endFrame()
{
	imguiEndFrame();
}

Gui::Gui()
{

}

Gui::~Gui()
{

}
