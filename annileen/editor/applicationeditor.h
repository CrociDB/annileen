#pragma once

#include <engine/application.h>
#include <editor/editorgui.h>

namespace annileen
{
	class ApplicationEditor : public Application
	{
	private:
		EditorGui *m_EditorGui;
		Camera* m_EditorCamera;

		virtual void initializeEditorGui();
		virtual void editorUpdate(float deltaTime);
		virtual void render();

	public:

		bool showEditorGui;

		ApplicationEditor();
		~ApplicationEditor();
	};
}