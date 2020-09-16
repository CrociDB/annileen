#include <editor/editorgui.h>
#include <engine/engine.h>
#include <imgui-utils/imgui.h>
#include <glm.hpp>
#include <engine/core/logger.h>

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
			if (m_ShowSceneHierarchyWindow) drawSceneHierarchyWindow(scene->getNodeList());
			if (m_SelectedSceneNode != nullptr)
			{
				if(m_ShowSceneNodePropertiesWindow) drawSelectedNodePropertiesWindow();
			}
		}

		if (m_ShowConsoleWindow) drawConsoleWindow();
		if (m_ShowSettingsWindow) drawSettingsWindow();
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

	void EditorGui::drawSceneHierarchyWindow(const std::list<SceneNodePtr> sceneNodeList)
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
			ImVec2(10.0f, 690.0f)
			, ImGuiCond_FirstUseEver
		);
		ImGui::SetNextWindowSize(
			ImVec2(300.0f, 120.0f)
			, ImGuiCond_FirstUseEver
		);

		if (!ImGui::Begin("SceneNode Properties", &m_ShowSceneNodePropertiesWindow))
		{
			ImGui::End();
			return;
		}

		float position[3] = { 0,0,0 };
		ImGui::DragFloat3("Position", glm::value_ptr(m_SelectedSceneNode->getTransform().position), 0.01F);
		float rotation[3] = { 0,0,0 };
		ImGui::DragFloat3("Rotation", glm::value_ptr(m_SelectedSceneNode->getTransform().rotation), 0.01F);
		float scale[3] = { 0,0,0 };
		ImGui::DragFloat3("Scale", glm::value_ptr(m_SelectedSceneNode->getTransform().scale), 0.01F);
		ImGui::End();
	}

	// TODO: this will be refactored to use queue or stack
	void EditorGui::_drawTree(SceneNodePtr const sceneNode)
	{
		static ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_OpenOnDoubleClick |
			ImGuiTreeNodeFlags_SpanAvailWidth;

		std::vector<SceneNodePtr> nodeChildren = sceneNode->getChildren();

		if (nodeChildren.empty())
		{
			if (ImGui::Selectable(sceneNode->name.c_str(), sceneNode == m_SelectedSceneNode))
			{
				m_SelectedSceneNode = sceneNode;
			}
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

		ImGui::End();
	}
}
