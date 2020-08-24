#include <editor/editorgui.h>
#include <engine/engine.h>
#include <imgui-utils/imgui.h>
#include <glm.hpp>
#include <engine/core/logger.h>

namespace annileen
{
	EditorGui::EditorGui()
	{
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

	void EditorGui::drawMainWindowToolbar()
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
				if (ImGui::MenuItem("Exit", 0, false, true))
				{
					Engine::getInstance()->terminate();
				}
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

	void EditorGui::drawEditorGeneralInfoWindow()
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

	void EditorGui::drawEditorSceneTreeWindow(const std::list<SceneNodePtr> sceneNodeList)
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

	void annileen::EditorGui::drawSelectedNodePropertiesWindow()
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
	void EditorGui::_drawTree(SceneNodePtr const sceneNode)
	{
		static ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_OpenOnDoubleClick |
			ImGuiTreeNodeFlags_SpanAvailWidth;

		std::string name = sceneNode->getName();
		if (sceneNode->hasModel())
		{
			name += " (M)";
		}
		if (sceneNode->getAcive())
		{
			name += " (A)";
		}

		std::vector<SceneNodePtr> nodeChildren = sceneNode->getChildren();

		if (nodeChildren.empty())
		{
			ImGui::Text(name.c_str());
		}
		else
		{
			if (ImGui::TreeNodeEx(name.c_str(), node_flags))
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
		char temp[] = "Console";

		ImGui::SetNextWindowPos(
			ImVec2(10.0f, 820.0f)
			, ImGuiCond_FirstUseEver
		);
		ImGui::SetNextWindowSize(
			ImVec2(900.0f, 180.0f)
			, ImGuiCond_FirstUseEver
		);


		ImGui::Begin(temp);
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
			messages = ServiceProvider::getLogger()->getMessagesAtLevel(logger->getLoggingLevelsList()[levelOptionId-1]);
		}
		else if (levelOptionId == 0 && channelOptionId != 0)
		{
			messages = ServiceProvider::getLogger()->getMessagesAtChannel(logger->getLoggingChannelsList()[channelOptionId - 1]);
		}
		else if (levelOptionId != 0 && channelOptionId != 0)
		{
			messages = ServiceProvider::getLogger()->getMessages(logger->getLoggingLevelsList()[levelOptionId - 1], logger->getLoggingChannelsList()[channelOptionId - 1]);
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
}