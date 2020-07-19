#pragma once

#include <iostream>
#include <map>
#include <glm.hpp>
#include <GLFW/glfw3.h>

namespace annileen
{
	struct KeyData
	{
		bool m_KeyValue;
		bool m_LastKeyValue;
	};

	class Input
	{
	private:
		std::map<int, KeyData> m_Keymap;
		glm::vec2 m_MousePosition;
		glm::vec2 m_MouseDelta;
		bool m_MouseButtons[3];

		void initKeymap();

	public:
		bool getKey(int keycode);
		bool getKeyDown(int keycode);
		bool getKeyUp(int keycode);
		bool getMouseButtonDown(int button);
		glm::vec2 getMousePosition();
		glm::vec2 getMouseDelta();

		void flushEvents();

		void _setKeyDown(int keycode, bool pressed);

		void _setMousePosition(float x, float y);
		void _setMouseDelta(int x, int y);
		void _setMouseButton(int button, bool status);

		Input();
		~Input();
	};
}
