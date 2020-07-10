#pragma once

#include <iostream>
#include <map>
#include <glm.hpp>
#include <GLFW/glfw3.h>

namespace annileen
{
	class Input
	{
	private:
		std::map<int, bool> m_Keymap;
		glm::vec2 m_MousePosition;
		glm::vec2 m_MouseDelta;
		bool m_MouseButtons[3];

		void initKeymap();

	public:
		bool getKeyDown(int keycode);
		bool getMouseButtonDown(int button);
		glm::vec2 getMousePosition();
		glm::vec2 getMouseDelta();

		void _flushEvents();

		void _setKeyDown(int keycode, bool pressed);

		void _setMousePosition(float x, float y);
		void _setMouseDelta(int x, int y);
		void _setMouseButton(int button, bool status);

		Input();
		~Input();
	};
}
