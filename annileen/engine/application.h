#pragma once

#include <engine/engine.h>

namespace annileen
{
	class Application
	{

	private:

		void initAnnileen();
		// TODO:
		// physicsUpdate
		// beforeRender
		// afterRender

		Engine* m_Engine;
		std::string m_ApplicationName;

// ApplicationEditor has to be able to inject the editor gui stuff
#ifdef _DEBUG
		friend class ApplicationEditor;
		virtual void initializeEditorGui() = 0;
		virtual void editorUpdate(float deltaTime) = 0;
#endif

	protected:

		virtual Scene* init() = 0;
		virtual void finish() = 0;
		virtual void update(float deltaTime) = 0;

		void destroy();

		inline Engine* const getEngine() const
		{
			return m_Engine;
		}

	public:
		int run(std::string applicationName);
		Application();
		~Application();
	};
}