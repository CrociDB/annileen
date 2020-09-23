#include <editor/editorgui.h>
#include <engine/engine.h>
#include <imgui-utils/imgui.h>
#include <glm.hpp>
#include <engine/core/logger.h>

#include <engine/model.h>
#include <engine/camera.h>
#include <engine/light.h>
#include <engine/text/text.h>
#include <engine/material.h>


namespace annileen
{
	EditorGui::EditorGui()
	{
		m_ShowToolsWindow = true;
		m_ShowSceneHierarchyWindow = true;
		m_ShowSceneNodePropertiesWindow = true;
		m_ShowConsoleWindow = true;
		m_ShowSettingsWindow = false;
		m_SelectedSceneNode = nullptr;
		m_SceneNodeToBeRemoved = nullptr;
	}

	EditorGui::~EditorGui()
	{

	}

	void EditorGui::initialize()
	{
		Logger *logger = ServiceProvider::getLogger();
		
		std::vector<LoggingLevel> loggingLevelsList = logger->getLoggingLevelsList();

		m_ConsoleLoggingLevelsList.push_back("All");
		for (LoggingLevel loggingLevel : loggingLevelsList)
		{
			m_ConsoleLoggingLevelsList.push_back(logger->getLoggingLevelString(loggingLevel));
		}

		std::vector<LoggingChannel> loggingChannelsList = logger->getLoggingChannelsList();

		m_ConsoleLoggingChannelsList.push_back("All");
		for (LoggingChannel loggingChannel : loggingChannelsList)
		{
			m_ConsoleLoggingChannelsList.push_back(logger->getLoggingChannelString(loggingChannel));
		}
	}

	void EditorGui::render(Scene* scene)
	{
		drawMainWindowToolbar();
		
		if (m_ShowToolsWindow) drawToolsWindow();
		
		if (scene != nullptr)
		{
			if (m_ShowSceneHierarchyWindow) drawSceneHierarchyWindow(scene->getRoot()->getChildren());
			if (m_SelectedSceneNode != nullptr)
			{
				if(m_ShowSceneNodePropertiesWindow) drawSelectedNodePropertiesWindow();
			}
		}

		if (m_ShowConsoleWindow) drawConsoleWindow();
		if (m_ShowSettingsWindow) drawSettingsWindow();

		if (m_SceneNodeToBeRemoved != nullptr)
		{
			Scene* scene = m_SceneNodeToBeRemoved->getParentScene();

			if (scene != nullptr)
			{
				if (m_SelectedSceneNode = m_SceneNodeToBeRemoved)
				{
					m_SelectedSceneNode = nullptr;
				}

				scene->destroyNode(m_SceneNodeToBeRemoved);
			}

			m_SceneNodeToBeRemoved = nullptr;
		}
	}

	void EditorGui::drawMainWindowToolbar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				//if (ImGui::MenuItem("New Scene", "CTRL+N", false, false)) {}
				//ImGui::Separator();
				//if (ImGui::MenuItem("Open Scene", "CTRL+O", false, false)) {}  // Disabled item
				//if (ImGui::MenuItem("Save Scene", "CTRL+S", false, false)) {}  // Disabled item
				//ImGui::Separator();
				if (ImGui::MenuItem("Exit", 0, false, true))
				{
					Engine::getInstance()->terminate();
				}
				ImGui::EndMenu();
			}
			//if (ImGui::BeginMenu("Edit"))
			//{
			//	if (ImGui::MenuItem("Undo", "CTRL+Z", false, false)) {}
			//	if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item			
			//	ImGui::EndMenu();
			//}
			/*if (ImGui::BeginMenu("Tools"))
			{
				if (ImGui::MenuItem("Generate Assets", 0, false, false)) {}
				ImGui::EndMenu();
			}*/

			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::MenuItem("Restore Default Layout", 0, false, true)) 
				{
					m_ShowToolsWindow = true;
					m_ShowSceneHierarchyWindow = true;
					m_ShowSceneNodePropertiesWindow = true;
					m_ShowConsoleWindow = true;
					m_ShowSettingsWindow = false;
				}
				ImGui::Separator();
				ImGui::MenuItem("Toolbar", nullptr, &m_ShowToolsWindow);
				ImGui::MenuItem("Scene Hierarchy", nullptr, &m_ShowSceneHierarchyWindow);
				ImGui::MenuItem("SceneNode Properties", nullptr, &m_ShowSceneNodePropertiesWindow);
				ImGui::MenuItem("Console", 0, &m_ShowConsoleWindow);
				ImGui::Separator();
				ImGui::MenuItem("Settings", 0, &m_ShowSettingsWindow);
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

	void EditorGui::drawToolsWindow()
	{
		ImGui::SetNextWindowPos(
			ImVec2(10.0f, 50.0f)
			, ImGuiCond_FirstUseEver
		);
		ImGui::SetNextWindowSize(
			ImVec2(300.0f, 210.0f)
			, ImGuiCond_FirstUseEver
		);

		if (!ImGui::Begin("Tools", &m_ShowToolsWindow))
		{
			ImGui::End();
			return;
		}

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

	void EditorGui::drawSceneHierarchyWindow(const std::vector<SceneNodePtr> sceneNodeList)
	{
		ImGui::SetNextWindowPos(
			ImVec2(10.0f, 270.0f)
			, ImGuiCond_FirstUseEver
		);
		ImGui::SetNextWindowSize(
			ImVec2(300.0f, 410.0f)
			, ImGuiCond_FirstUseEver
		);

		if (!ImGui::Begin("Scene Hierarchy", &m_ShowSceneHierarchyWindow))
		{
			ImGui::End();
			return;
		}

		for (auto sceneNode : sceneNodeList)
		{
			_drawTree(sceneNode);
		}

		ImGui::End();
	}

	void annileen::EditorGui::drawSelectedNodePropertiesWindow()
	{
		ImGui::SetNextWindowPos(
			ImVec2(320.0f, 270.0f)
			, ImGuiCond_FirstUseEver
		);
		ImGui::SetNextWindowSize(
			ImGui::GetContentRegionAvail()
			, ImGuiCond_FirstUseEver
		);

		if (!ImGui::Begin("SceneNode Properties", &m_ShowSceneNodePropertiesWindow))
		{
			ImGui::End();
			return;
		}


		Text* text = m_SelectedSceneNode->getModule<Text>();

		if (text == nullptr)
		{
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
			{
				auto position = m_SelectedSceneNode->getTransform().position();
				ImGui::DragFloat3("Position", glm::value_ptr(position), 0.01F);
				m_SelectedSceneNode->getTransform().position(position);

				auto rotationEuler = m_SelectedSceneNode->getTransform().euler();
				ImGui::DragFloat3("Rotation", glm::value_ptr(rotationEuler), 0.01F);
				m_SelectedSceneNode->getTransform().euler(rotationEuler);

				auto scale = m_SelectedSceneNode->getTransform().scale();
				ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.01F);
				m_SelectedSceneNode->getTransform().scale(scale);
			}
		}
		else
		{
			drawTextModuleProperties(text);
		}

		Model* model = m_SelectedSceneNode->getModule<Model>();
		if (model != nullptr)
		{
			drawModelModuleProperties(model);
		}

		Camera* camera = m_SelectedSceneNode->getModule<Camera>();
		if (camera != nullptr)
		{
			drawCameraModuleProperties(camera);
		}

		Light* light = m_SelectedSceneNode->getModule<Light>();
		if (light != nullptr)
		{
			drawLightModuleProperties(light);
		}
		ImGui::End();
	}

	// TODO: this will be refactored to use queue or stack
	void EditorGui::_drawTree(SceneNodePtr const sceneNode)
	{
		if (sceneNode == nullptr)
		{
			return;
		}

		static ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_OpenOnDoubleClick |
			ImGuiTreeNodeFlags_SpanAvailWidth;

		std::vector<SceneNodePtr> nodeChildren = sceneNode->getChildren();

		if (nodeChildren.empty())
		{
			ImGui::PushID(sceneNode->getId());
			if (ImGui::Selectable(sceneNode->name.c_str(), sceneNode == m_SelectedSceneNode))
			{
				m_SelectedSceneNode = sceneNode;
			}
			drawSceneNodeContextMenu(sceneNode);		
			ImGui::PopID();
		}
		else
		{
			if (ImGui::TreeNodeEx(sceneNode->name.c_str(), node_flags))
			{
				for (auto childNode : nodeChildren)
				{
					_drawTree(childNode);
				}
				ImGui::TreePop();
			}
		}
	}

	void EditorGui::drawSceneNodeContextMenu(SceneNodePtr const sceneNode)
	{
		if (!ImGui::BeginPopupContextItem())
		{
			return;
		}
		
		if (ImGui::BeginMenu("Add Scene Node"))
		{
			if (ImGui::BeginMenu("Model"))
			{
				drawNewSceneNodeContextMenu<Model>(sceneNode, "New Model");
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Camera"))
			{
				drawNewSceneNodeContextMenu<Camera>(sceneNode, "New Camera");
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Light"))
			{
				drawNewSceneNodeContextMenu<Light>(sceneNode, "New Light");
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Text"))
			{
				drawNewSceneNodeContextMenu<Text>(sceneNode, "New Text");
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Delete Scene Node"))
		{
			m_SceneNodeToBeRemoved = sceneNode;
		}
		ImGui::EndPopup();		
	}

	//void EditorGui::drawNewSceneNodeContextMenu(SceneNodePtr const sceneNode)
	//{
	//	Scene* scene = sceneNode->getParentScene();

	//	if (ImGui::Selectable("Above"))
	//	{
	//		SceneNodePtr newSceneNode = new SceneNode(scene, "SceneNode");
	//		sceneNode->getParent()->addChildBefore(newSceneNode, sceneNode);
	//	}
	//	if (ImGui::Selectable("As child"))
	//	{	
	//		SceneNodePtr newSceneNode = new SceneNode(scene, "SceneNode");			
	//		sceneNode->addChild(newSceneNode);
	//	}
	//	if (ImGui::Selectable("Below"))
	//	{
	//		SceneNodePtr newSceneNode = new SceneNode(scene, "SceneNode");
	//		sceneNode->getParent()->addChildAfter(newSceneNode, sceneNode);
	//	}
	//}

	void EditorGui::drawConsoleWindow()
	{
		ImGui::SetNextWindowPos(
			ImVec2(10.0f, 820.0f)
			, ImGuiCond_FirstUseEver
		);
		ImGui::SetNextWindowSize(
			ImVec2(900.0f, 180.0f)
			, ImGuiCond_FirstUseEver
		);

		if (!ImGui::Begin("Console", &m_ShowConsoleWindow))
		{
			ImGui::End();
			return;
		}

		if (ImGui::SmallButton("Clear")) 
		{ 
		}
		ImGui::SameLine();
		
		ImGui::PushItemWidth(150);
		static const char* levelOption = nullptr;
		static int levelOptionId = 0;
		if (ImGui::BeginCombo("Message Level", levelOption))
		{
			for (int n = 0; n < m_ConsoleLoggingLevelsList.size(); ++n)
			{
				const char* selectedOption = m_ConsoleLoggingLevelsList[n];
				bool isSelected = (levelOption == selectedOption);
				if (ImGui::Selectable(selectedOption, isSelected))
				{
					levelOption = selectedOption;
					levelOptionId = n;
				}
				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		
		ImGui::SameLine();
		static const char* channelOption = nullptr;
		static int channelOptionId = 0;
		if (ImGui::BeginCombo("Message Channel", channelOption))
		{
			for (int n = 0; n < m_ConsoleLoggingChannelsList.size(); ++n)
			{
				const char* selectedOption = m_ConsoleLoggingChannelsList[n];
				bool isSelected = (channelOption == selectedOption);
				if (ImGui::Selectable(selectedOption, isSelected))
				{
					channelOption = selectedOption;
					channelOptionId = n;
				}
				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		ImGuiTextFilter Filter;
		Filter.Draw("Filter");
		ImGui::PopItemWidth();
		ImGui::Separator();

		std::vector<Logger::Message> messages;

		Logger* logger = ServiceProvider::getLogger();
		
		if (levelOptionId != 0 && channelOptionId == 0)
		{
			messages = ServiceProvider::getLogger()->getMessagesAtLevel(logger->getLoggingLevelsList()[static_cast<int>(levelOptionId)-1]);
		}
		else if (levelOptionId == 0 && channelOptionId != 0)
		{
			messages = ServiceProvider::getLogger()->getMessagesAtChannel(logger->getLoggingChannelsList()[static_cast<int>(channelOptionId) - 1]);
		}
		else if (levelOptionId != 0 && channelOptionId != 0)
		{
			messages = ServiceProvider::getLogger()->getMessages(logger->getLoggingLevelsList()[static_cast<int>(levelOptionId) - 1], logger->getLoggingChannelsList()[static_cast<int>(channelOptionId) - 1]);
		}
		else
		{
			messages = ServiceProvider::getLogger()->getAllMessages();
		}

		ImVec4 infoColor = ImVec4(1, 1, 1, 1);
		ImVec4 errorColor = ImVec4(1, 0, 0, 1);
		ImVec4 warningColor = ImVec4(1, 1, 0, 1);

		for (int messageId = static_cast<int>(messages.size()) - 1; messageId >= 0; --messageId)
		{
			Logger::Message message = messages[messageId];
		
			switch (message.m_Level)
			{
			case LoggingLevel::Error: ImGui::PushStyleColor(ImGuiCol_Text, errorColor); break;
			case LoggingLevel::Warning: ImGui::PushStyleColor(ImGuiCol_Text, warningColor); break;
			case LoggingLevel::Info: ImGui::PushStyleColor(ImGuiCol_Text, infoColor); break;
			}
			ImGui::TextUnformatted(message.m_Message.c_str());
			ImGui::PopStyleColor();
		}

		ImGui::Separator();
	
		char InputBuf[256]; // hard coded for testing, will be changed
		memset(InputBuf, 0, sizeof(InputBuf));
		if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf)))
		{
		}

		ImGui::End();
	}

	void EditorGui::drawSettingsWindow()
	{
		ImGui::SetNextWindowPos(
			ImVec2(400.0f, 520.0f)
			, ImGuiCond_FirstUseEver
		);
		ImGui::SetNextWindowSize(
			ImVec2(400.0f, 180.0f)
			, ImGuiCond_FirstUseEver
		);

		if (!ImGui::Begin("Settings", &m_ShowSettingsWindow))
		{
			ImGui::End();
			return;
		}

		Settings* settings = ServiceProvider::getSettings();

		if (ImGui::CollapsingHeader("Shadows", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Checkbox("Enabled", &settings->shadows.enabled);
		}

		ImGui::End();
	}

	void EditorGui::drawModelModuleProperties(Model* model)
	{
		if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Checkbox("Enabled", &model->enabled);
			ImGui::Checkbox("Static", &model->isStatic);
			ImGui::Checkbox("Cast Shadows", &model->castShadows);
			ImGui::Checkbox("Receive Shadows", &model->receiveShadows);
		}
		if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
		{
			std::shared_ptr<Material> material = model->getMaterial();
			if (material != nullptr)
			{
				ImGui::Text("Name: %s", model->getMaterial()->getName());
			}
			else
			{
				ImGui::Text("Name: No material set");
			}
		}
	}

	void EditorGui::drawLightModuleProperties(Light* light)
	{
		if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Checkbox("Enabled", &light->enabled);
			ImGui::Checkbox("Static", &light->isStatic);
			ImGui::Checkbox("Generate Shadows", &light->generateShadows);
			ImGui::DragFloat("Intensity", &light->intensity, 0.1F, 0.0F,10.F);
			ImGui::ColorEdit3("Color", glm::value_ptr(light->color));
		}
	}

	void EditorGui::drawCameraModuleProperties(Camera* camera)
	{
		if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Checkbox("Enabled", &camera->enabled);
			ImGui::Checkbox("Static", &camera->isStatic);

			ImGui::ColorEdit4("Clear Color", glm::value_ptr(camera->clearColor));
			ImGui::DragFloat("Near", &camera->nearClip);
			ImGui::DragFloat("Far", &camera->farClip);
			ImGui::DragFloat("Field Of View", &camera->fieldOfView);
		}
	}

	void EditorGui::drawTextModuleProperties(Text* text)
	{
		bool staticText = text->isStatic();

		if(staticText)
		{
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			
			ImVec2 buttonSize = ImGui::GetWindowSize();
			buttonSize.x -= 25;
			buttonSize.y = 100;

			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 marker_min = ImVec2(pos.x + buttonSize.x, pos.y);
			ImVec2 marker_max = ImVec2(pos.x + buttonSize.x + 10, pos.y + ImGui::GetTextLineHeight());
			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + buttonSize.x);
			ImGui::Text("Properties cannot be changed because this text is static", buttonSize.x);
			// Draw actual text bounding box, following by marker of our expected limit (should not overlap!)
			draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 0, 255));
			draw_list->AddRectFilled(marker_min, marker_max, IM_COL32(255, 0, 255, 255));
			ImGui::PopTextWrapPos();
			//ImGui::Button("Properties cannot be changed \nbecause this text is static", buttonSize);
		}

		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			glm::vec2 screenPosition = text->getScreenPosition();
			ImGui::DragFloat2("Screen Position", glm::value_ptr(screenPosition));
			if (!staticText && screenPosition != text->getScreenPosition())
			{
				text->setScreenPosition(screenPosition.x, screenPosition.y);
			}
		}
		if (ImGui::CollapsingHeader("Text", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Checkbox("Enabled", &text->enabled);
			ImGui::Checkbox("Static", &staticText);
			text->setStatic(staticText);
			
			Text::TextStyle textStyle = text->getStyle();

			bool background = textStyle & Text::TextStyle::Background;
			ImGui::Checkbox("Background", &background);
			glm::vec3 backgroundColor = text->getBackgroundColor();
			ImGui::ColorEdit3("Background Color", glm::value_ptr(backgroundColor));
			if (!staticText && backgroundColor != text->getBackgroundColor())
			{
				text->setBackgroundColor(backgroundColor);
			}

			bool overline = textStyle & Text::TextStyle::Overline;
			ImGui::Checkbox("Overline", &overline);
			glm::vec3 overlineColor = text->getOverlineColor();
			ImGui::ColorEdit3("Overline Color", glm::value_ptr(overlineColor));
			if (!staticText && overlineColor != text->getOverlineColor())
			{
				text->setOverlineColor(overlineColor);
			}

			bool underline = textStyle & Text::TextStyle::Underline;
			ImGui::Checkbox("Underline", &underline);
			glm::vec3 underlineColor = text->getUnderlineColor();
			ImGui::ColorEdit3("Underline Color", glm::value_ptr(underlineColor));
			if (!staticText && underlineColor != text->getUnderlineColor())
			{
				text->setUnderlineColor(underlineColor);
			}

			bool strikethrough = textStyle & Text::TextStyle::StrikeThrough;
			ImGui::Checkbox("StrikeThrough", &strikethrough);
			glm::vec3 strikeThroughColor = text->getStrikeThroughColor();
			ImGui::ColorEdit3("StrikeThrough Color", glm::value_ptr(strikeThroughColor));
			if (!staticText && strikeThroughColor != text->getStrikeThroughColor())
			{
				text->setStrikeThroughColor(strikeThroughColor);
			}

			textStyle = static_cast<Text::TextStyle>((background ? Text::TextStyle::Background : 0) |
				(overline ? Text::TextStyle::Overline : 0) |
				(underline ? Text::TextStyle::Underline : 0) |
				(strikethrough ? Text::TextStyle::StrikeThrough : 0));

			if (!staticText && textStyle != text->getStyle())
			{
				text->setStyle(textStyle);
			}

			glm::vec3 textColor = text->getTextColor();
			ImGui::ColorEdit3("Text Color", glm::value_ptr(textColor));
			if (!staticText && textColor != text->getTextColor())
			{
				text->setTextColor(textColor);
			}
	
			std::string textContent = text->getText();
		
			ImGui::InputTextMultiline("Text", (char*) textContent.c_str(), textContent.capacity() + 1);
			if (!staticText)
			{
				text->setText(textContent);
			}
		}
	}
}
