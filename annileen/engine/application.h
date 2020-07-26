#pragma once

#include "engine.h"

namespace annileen
{
	class Application
	{

	private:

		void initAnnileen();

		virtual Scene* init() = 0;
		virtual void update(float deltaTime) = 0;
		virtual void finish() = 0;
		// TODO:
		// physicsUpdate
		// beforeRender
		// afterRender

		Engine* m_Engine;

	protected:

		virtual void gui();
		void destroy();

		inline Engine* const getEngine() const
		{
			return m_Engine;
		}

	public:
		int run();
		Application();
		~Application();
	};
}