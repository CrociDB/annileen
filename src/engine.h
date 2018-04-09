#ifndef _ENGINE_H_
#define _ENGINE_H_

#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <glm.hpp>

class Renderer;

class Engine
{
private:
    SDL_Window* m_Window;
    uint16_t m_Width, m_Height;
    SDL_Event m_WindowEvent;

    Renderer* m_Renderer;
    bool m_Running;

public:
    void init(uint16_t width, uint16_t height);

    Renderer* getRenderer();
    SDL_Window* getSDLWindow();
    uint16_t getWidth();
    uint16_t getHeight();

    bool isRunning();

    void checkInputEvents();
    void renderFrame();

    Engine();
    ~Engine();
};

#endif
