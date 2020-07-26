#pragma once

#include <engine/application.h>

namespace annileen
{
	class ApplicationEditor : public Application
	{
	protected:
		void gui() override;
	public:

		bool showEditorGui;

		ApplicationEditor();
		~ApplicationEditor();
	};
}