#ifndef _INPUT_H_
#define _INPUT_H_

#include <iostream>
#include <map>

#include <glm.hpp>

#include <SDL.h>

class Input
{
private:
	std::map<SDL_Keycode, bool> m_Keymap;
	glm::vec2 m_MousePosition;
	glm::vec2 m_MouseDelta;
	bool m_MouseButtons[3];

	void initKeymap();

public:
	bool getKeyDown(SDL_Keycode keycode);
	bool getMouseButtonDown(int button);
	glm::vec2 getMousePosition();
	glm::vec2 getMouseDelta();

	void _flushEvents();

	void _setKeyDown(SDL_Keycode keycode, bool pressed);

	void _setMousePosition(int x, int y);
	void _setMouseDelta(int x, int y);
	void _setMouseButton(int button, bool status);

	Input();
	~Input();
};

#endif
