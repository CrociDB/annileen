#include <editor/applicationeditor.h>
#include <editor/editorgui.h>

namespace annileen
{
	ApplicationEditor::ApplicationEditor() : showEditorGui(true)
	{}

	ApplicationEditor::~ApplicationEditor()
	{}

	void ApplicationEditor::gui()
	{
		Application::gui();

		if (showEditorGui)
		{
			EditorGui::drawMainWindowToolbar();
			EditorGui::drawEditorGeneralInfoWindow();
			Scene* scene = getEngine()->getScene();
			if (scene != nullptr)
			{
				EditorGui::drawSelectedNodePropertiesWindow();
				EditorGui::drawEditorSceneTreeWindow(scene->getNodeList());
			}
			EditorGui::drawConsoleWindow();
		}
	}
}