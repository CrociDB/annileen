#ifndef _ENGINE_H_
#define _ENGINE_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <glm.hpp>

#include "scene.h"
#include "input.h"

class Renderer;

class Engine
{
private:
    SDL_Window* m_Window;
    uint16_t m_Width, m_Height;
    SDL_Event m_WindowEvent;

	Input* m_Input;
    Renderer* m_Renderer;
    bool m_Running;

    Scene* m_CurrentScene = NULL;

    Engine();
public:
    void init(uint16_t width, uint16_t height);

	Input* getInput();
	Renderer* getRenderer();
    SDL_Window* getSDLWindow();
    uint16_t getWidth() const;
    uint16_t getHeight() const;

    void setScene(Scene* scene);

    bool isRunning();

    void checkInputEvents();
    void renderFrame();

	static Engine* getInstance();
    ~Engine();
};

#endif
