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
			annileen::EditorGui::drawMainWindowToolbar();
			annileen::EditorGui::drawEditorGeneralInfoWindow();
			annileen::Scene* scene = getEngine()->getScene();
			if (scene != nullptr)
			{
				annileen::EditorGui::drawSelectedNodePropertiesWindow();
				annileen::EditorGui::drawEditorSceneTreeWindow(scene->getNodeList());
			}
		}
	}
}