#ifndef _INPUT_H_
#define _INPUT_H_

#include <iostream>
#include <map>

#include <SDL.h>

class Input
{
private:
	std::map<SDL_Keycode, bool> m_Keymap;

	void initKeymap();

public:
	bool getKeyDown(SDL_Keycode keycode);

	void _setKeyDown(SDL_Keycode keycode, bool pressed);

	Input();
	~Input();
};

#endif
