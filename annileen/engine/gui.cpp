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

void Gui::beginFrame(glm::vec2 mousePos, uint8_t mouseButton, int32_t mouseScroll, uint32_t width, uint32_t height)
{
	imguiBeginFrame(mousePos.x, mousePos.y, mouseButton, mouseScroll, width, height);
}

void annileen::Gui::drawMainWindowToolbar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene", "CTRL+N", false, false)) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Open Scene", "CTRL+O", false, false)) {}  // Disabled item
			if (ImGui::MenuItem("Save Scene", "CTRL+S", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Exit", 0, false, false)) {}			
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z", false, false)) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item			
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("Generate Assets", 0, false, false)) {}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Gui::drawEditorGeneralInfoWindow()
{
	char temp[1024] = "Editor";

	ImGui::SetNextWindowPos(
		ImVec2(10.0f, 50.0f)
		, ImGuiCond_FirstUseEver
	);
	ImGui::SetNextWindowSize(
		ImVec2(300.0f, 210.0f)
		, ImGuiCond_FirstUseEver
	);

	ImGui::Begin(temp);

	//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(3.0f, 3.0f));
	//if (ImGui::Button(ICON_FA_REPEAT " Restart"))
	//{
	//}
	//
	//ImGui::SameLine();
	//if (ImGui::Button(ICON_KI_PREVIOUS " Prev"))
	//{
	//}

	//ImGui::SameLine();
	//if (ImGui::Button(ICON_KI_NEXT " Next"))
	//{
	//}
	//
	//ImGui::SameLine();
	//if (ImGui::Button(ICON_KI_EXIT " Exit"))
	//{
	//}

	//ImGui::SameLine();
	//if (ImGui::Button(ICON_FA_BAR_CHART))
	//{
	//}

	//ImGui::PopStyleVar();

	ImGui::End();
}

void Gui::drawEditorSceneTreeWindow(const std::list<SceneNode*> sceneNodeList) const
{
	char temp[] = "SceneTree";

	ImGui::SetNextWindowPos(
		ImVec2(10.0f, 270.0f)
		, ImGuiCond_FirstUseEver
	);
	ImGui::SetNextWindowSize(
		ImVec2(300.0f, 410.0f)
		, ImGuiCond_FirstUseEver
	);

	ImGui::Begin(temp);

	int nodeCount = 0;
	std::string name = "node";
	for (auto sceneNode : sceneNodeList)
	{
		_drawTree(sceneNode);
	}

	ImGui::End();
}

void annileen::Gui::drawSelectedNodePropertiesWindow()
{
	char temp[] = "SceneNode Properties";

	ImGui::SetNextWindowPos(
		ImVec2(10.0f, 690.0f)
		, ImGuiCond_FirstUseEver
	);
	ImGui::SetNextWindowSize(
		ImVec2(300.0f, 120.0f)
		, ImGuiCond_FirstUseEver
	);

	ImGui::Begin(temp);

	float position[3] = { 0,0,0 };
	ImGui::DragFloat3("Position", position);
	float rotation[3] = { 0,0,0 };
	ImGui::DragFloat3("Rotation", rotation);
	float scale[3] = { 0,0,0 };
	ImGui::DragFloat3("Scale", scale);
	ImGui::End();
}

// TODO: this will be refactored to use queue or stack
void Gui::_drawTree(SceneNode* const sceneNode) const
{
	std::string name = sceneNode->getName();
	if (sceneNode->hasModel())
	{
		name += " (M)";
	}
	if (sceneNode->getAcive())
	{
		name += " (A)";
	}

	std::vector<SceneNode*> nodeChildren = sceneNode->getChildren();
	
	if (nodeChildren.empty())
	{
		ImGui::Text(name.c_str());
	}
	else
	{
		if (ImGui::TreeNode(name.c_str()))
		{
			for (auto childNode : nodeChildren)
			{
				_drawTree(childNode);
			}
			ImGui::TreePop();
		}
	}
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
