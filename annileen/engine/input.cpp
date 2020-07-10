#include "input.h"

using namespace annileen;

void Input::initKeymap()
{
	m_Keymap = {
		{ GLFW_KEY_ENTER, false },
		{ GLFW_KEY_ESCAPE, false },
		{ GLFW_KEY_BACKSPACE, false },
		{ GLFW_KEY_TAB, false },
		{ GLFW_KEY_SPACE, false },

		{ GLFW_KEY_0, false },
		{ GLFW_KEY_1, false },
		{ GLFW_KEY_2, false },
		{ GLFW_KEY_3, false },
		{ GLFW_KEY_4, false },
		{ GLFW_KEY_5, false },
		{ GLFW_KEY_6, false },
		{ GLFW_KEY_7, false },
		{ GLFW_KEY_8, false },
		{ GLFW_KEY_9, false },

		{ GLFW_KEY_A, false },
		{ GLFW_KEY_B, false },
		{ GLFW_KEY_C, false },
		{ GLFW_KEY_D, false },
		{ GLFW_KEY_E, false },
		{ GLFW_KEY_F, false },
		{ GLFW_KEY_G, false },
		{ GLFW_KEY_H, false },
		{ GLFW_KEY_I, false },
		{ GLFW_KEY_J, false },
		{ GLFW_KEY_K, false },
		{ GLFW_KEY_L, false },
		{ GLFW_KEY_M, false },
		{ GLFW_KEY_N, false },
		{ GLFW_KEY_O, false },
		{ GLFW_KEY_P, false },
		{ GLFW_KEY_Q, false },
		{ GLFW_KEY_R, false },
		{ GLFW_KEY_S, false },
		{ GLFW_KEY_T, false },
		{ GLFW_KEY_U, false },
		{ GLFW_KEY_V, false },
		{ GLFW_KEY_W, false },
		{ GLFW_KEY_X, false },
		{ GLFW_KEY_Y, false },
		{ GLFW_KEY_Z, false },

		{ GLFW_KEY_RIGHT, false },
		{ GLFW_KEY_LEFT, false },
		{ GLFW_KEY_DOWN, false },
		{ GLFW_KEY_UP, false }
	};
}

bool Input::getKeyDown(int keycode)
{
	return m_Keymap[keycode];
}

bool Input::getMouseButtonDown(int button)
{
	button = glm::clamp(button, 0, 3);
	return m_MouseButtons[button];
}

glm::vec2 Input::getMousePosition()
{
	return m_MousePosition;
}

glm::vec2 Input::getMouseDelta()
{
	return m_MouseDelta;
}

void Input::_flushEvents()
{
	m_MouseDelta.x = m_MouseDelta.y = 0.0f;
}

void Input::_setKeyDown(int keycode, bool pressed)
{
	m_Keymap.insert_or_assign(keycode, pressed);
}

void Input::_setMousePosition(float x, float y)
{
	m_MousePosition.x = x;
	m_MousePosition.y = y;
}

void Input::_setMouseDelta(int x, int y)
{
	m_MouseDelta.x = x;
	m_MouseDelta.y = y;
}

void Input::_setMouseButton(int button, bool status)
{
	if (button >= 1 && button <= 3)
	{
		m_MouseButtons[button - 1] = status;
	}
}

Input::Input()
{
	initKeymap();
	for (int i = 0; i < 3; i++) m_MouseButtons[i] = false;

}

Input::~Input()
{
}