#include <engine/input.h>
#include <GLFW/glfw3.h>
#include <engine/serviceprovider.h>

namespace annileen
{
	void Input::initKeymap()
	{
		m_Keymap = {
			{ GLFW_KEY_ENTER, { false, false } },
			{ GLFW_KEY_ESCAPE, { false, false } },
			{ GLFW_KEY_BACKSPACE, { false, false } },
			{ GLFW_KEY_TAB, { false, false } },
			{ GLFW_KEY_SPACE, { false, false } },
			{ GLFW_KEY_LEFT_SHIFT , { false, false } },
			{ GLFW_KEY_RIGHT_SHIFT , { false, false } },

			{ GLFW_KEY_F1, { false, false } },
			{ GLFW_KEY_F2, { false, false } },
			{ GLFW_KEY_F3, { false, false } },
			{ GLFW_KEY_F4, { false, false } },
			{ GLFW_KEY_F5, { false, false } },
			{ GLFW_KEY_F6, { false, false } },
			{ GLFW_KEY_F7, { false, false } },
			{ GLFW_KEY_F8, { false, false } },
			{ GLFW_KEY_F9, { false, false } },
			{ GLFW_KEY_F10, { false, false } },
			{ GLFW_KEY_F11, { false, false } },
			{ GLFW_KEY_F12, { false, false } },

			{ GLFW_KEY_0, { false, false } },
			{ GLFW_KEY_1, { false, false } },
			{ GLFW_KEY_2, { false, false } },
			{ GLFW_KEY_3, { false, false } },
			{ GLFW_KEY_4, { false, false } },
			{ GLFW_KEY_5, { false, false } },
			{ GLFW_KEY_6, { false, false } },
			{ GLFW_KEY_7, { false, false } },
			{ GLFW_KEY_8, { false, false } },
			{ GLFW_KEY_9, { false, false } },

			{ GLFW_KEY_A, { false, false } },
			{ GLFW_KEY_B, { false, false } },
			{ GLFW_KEY_C, { false, false } },
			{ GLFW_KEY_D, { false, false } },
			{ GLFW_KEY_E, { false, false } },
			{ GLFW_KEY_F, { false, false } },
			{ GLFW_KEY_G, { false, false } },
			{ GLFW_KEY_H, { false, false } },
			{ GLFW_KEY_I, { false, false } },
			{ GLFW_KEY_J, { false, false } },
			{ GLFW_KEY_K, { false, false } },
			{ GLFW_KEY_L, { false, false } },
			{ GLFW_KEY_M, { false, false } },
			{ GLFW_KEY_N, { false, false } },
			{ GLFW_KEY_O, { false, false } },
			{ GLFW_KEY_P, { false, false } },
			{ GLFW_KEY_Q, { false, false } },
			{ GLFW_KEY_R, { false, false } },
			{ GLFW_KEY_S, { false, false } },
			{ GLFW_KEY_T, { false, false } },
			{ GLFW_KEY_U, { false, false } },
			{ GLFW_KEY_V, { false, false } },
			{ GLFW_KEY_W, { false, false } },
			{ GLFW_KEY_X, { false, false } },
			{ GLFW_KEY_Y, { false, false } },
			{ GLFW_KEY_Z, { false, false } },

			{ GLFW_KEY_RIGHT, { false, false } },
			{ GLFW_KEY_LEFT, { false, false } },
			{ GLFW_KEY_DOWN, { false, false } },
			{ GLFW_KEY_UP, { false, false } }
		};
	}

	bool Input::getKey(int keycode) const
	{
		return _getKey(keycode) && m_Enabled;
	}

	bool Input::_getKey(int keycode) const
	{
		return this->m_Keymap.at(keycode).m_KeyValue;
	}

	bool Input::getKeyDown(int keycode) const
	{
		return _getKeyDown(keycode) && m_Enabled;
	}

	bool Input::_getKeyDown(int keycode) const
	{
		return this->m_Keymap.at(keycode).m_KeyValue && !this->m_Keymap.at(keycode).m_LastKeyValue;
	}

	bool Input::getKeyUp(int keycode) const
	{
		return _getKeyUp(keycode) && m_Enabled;
	}

	bool Input::_getKeyUp(int keycode) const
	{
		return !this->m_Keymap.at(keycode).m_KeyValue && this->m_Keymap.at(keycode).m_LastKeyValue;
	}

	bool Input::getMouseButtonDown(int button) const
	{
		return _getMouseButtonDown(button) && m_Enabled;
	}

	bool Input::_getMouseButtonDown(int button) const
	{
		button = glm::clamp(button, 0, 3);
		return m_MouseButtons[button];
	}

	glm::vec2 Input::getMousePosition() const
	{
		if (m_Enabled)
		{
			return _getMousePosition();
		}

		return glm::vec2(0);
	}
	
	glm::vec2 Input::_getMousePosition() const
	{
		return m_MousePosition;
	}

	glm::vec2 Input::getMouseDelta() const
	{
		if (m_Enabled)
		{
			return _getMouseDelta();
		}

		return glm::vec2(0);
	}

	glm::vec2 Input::_getMouseDelta() const
	{
		return m_MouseDelta;
	}

	glm::vec2 Input::getMouseScroll() const
	{
		if (m_Enabled)
		{
			return _getMouseScroll();
		}

		return glm::vec2(0);
	}

	glm::vec2 Input::_getMouseScroll() const
	{
		return m_MouseScroll;
	}

	void Input::flushEvents()
	{ 
		m_MouseDelta.x = m_MouseDelta.y = 0.0f;
		for (auto& [k, v] : m_Keymap)
		{
			v.m_LastKeyValue = v.m_KeyValue;
		}
	}

	void Input::_setKeyDown(int keycode, bool pressed)
	{
		if (m_Keymap.count(keycode) > 0)
			m_Keymap[keycode].m_KeyValue = pressed;
	}

	void Input::_setMousePosition(float x, float y)
	{
		m_MouseDelta.x = x - m_MousePosition.x;
		m_MouseDelta.y = y - m_MousePosition.y;
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
		if (button >= 0 && button <= 3)
		{
			m_MouseButtons[button] = status;
		}
	}

	void Input::_setMouseScroll(float x, float y)
	{
		m_MouseScroll.x += x;
		m_MouseScroll.y += y;
	}

	Input::Input()
	{
		initKeymap();
		for (int i = 0; i < 3; i++) m_MouseButtons[i] = false;

		m_MousePosition.x = m_MousePosition.y = 0;
		m_MouseDelta.x = m_MouseDelta.y = 0;
		m_MouseScroll.x = m_MouseScroll.y = 0;
		m_Enabled = true;
	}

	Input::~Input()
	{
	}
}
