#include <editor/applicationeditor.h>
#include <editor/editorgui.h>
#include <engine/core/logger.h>

namespace annileen
{
	ApplicationEditor::ApplicationEditor() : showEditorGui(true)
	{
		m_EditorGui = new EditorGui();
	}

	ApplicationEditor::~ApplicationEditor()
	{
		if (m_EditorGui != nullptr)
		{
			delete m_EditorGui;
		}
	}

	void ApplicationEditor::initializeEditorGui()
	{
		m_EditorGui->initialize();
		
		ANNILEEN_LOG_INFO(LoggingChannel::General, "Initialized Editor GUI");	 	
	}

	void ApplicationEditor::updateEditorGui()
	{
		if (showEditorGui)
		{
			m_EditorGui->drawMainWindowToolbar();
			m_EditorGui->drawEditorGeneralInfoWindow();
			Scene* scene = getEngine()->getScene();
			if (scene != nullptr)
			{
				m_EditorGui->drawSelectedNodePropertiesWindow();
				m_EditorGui->drawEditorSceneTreeWindow(scene->getNodeList());
			}
			m_EditorGui->drawConsoleWindow();
		}
	}
}