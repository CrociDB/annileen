#pragma once

#include <engine/application.h>
#include <editor/editorgui.h>

namespace annileen
{
	class ApplicationEditor : public Application
	{
	private:
		EditorGui *m_EditorGui;

		virtual void initializeEditorGui();
		virtual void editorUpdate(float deltaTime);
	public:

		bool showEditorGui;

		ApplicationEditor();
		~ApplicationEditor();
	};
}