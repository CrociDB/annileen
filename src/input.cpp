#include "input.h"


void Input::initKeymap()
{
	m_Keymap = {
		{ SDLK_RETURN, false },
		{ SDLK_ESCAPE, false },
		{ SDLK_BACKSPACE , false },
		{ SDLK_TAB, false },
		{ SDLK_SPACE, false },

		{ SDLK_0, false },
		{ SDLK_1, false },
		{ SDLK_2, false },
		{ SDLK_3, false },
		{ SDLK_4, false },
		{ SDLK_5, false },
		{ SDLK_6, false },
		{ SDLK_7, false },
		{ SDLK_8, false },
		{ SDLK_9, false },

		{ SDLK_a, false },
		{ SDLK_b, false },
		{ SDLK_c, false },
		{ SDLK_d, false },
		{ SDLK_e, false },
		{ SDLK_f, false },
		{ SDLK_g, false },
		{ SDLK_h, false },
		{ SDLK_i, false },
		{ SDLK_j, false },
		{ SDLK_k, false },
		{ SDLK_l, false },
		{ SDLK_m, false },
		{ SDLK_n, false },
		{ SDLK_o, false },
		{ SDLK_p, false },
		{ SDLK_q, false },
		{ SDLK_r, false },
		{ SDLK_s, false },
		{ SDLK_t, false },
		{ SDLK_u, false },
		{ SDLK_v, false },
		{ SDLK_w, false },
		{ SDLK_x, false },
		{ SDLK_y, false },
		{ SDLK_z, false },

		{ SDLK_RIGHT, false },
		{ SDLK_LEFT, false },
		{ SDLK_DOWN, false },
		{ SDLK_UP, false }
	};
}

bool Input::getKeyDown(SDL_Keycode keycode)
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

void Input::_setKeyDown(SDL_Keycode keycode, bool pressed)
{
	m_Keymap.insert_or_assign(keycode, pressed);
}

void Input::_setMousePosition(int x, int y)
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
