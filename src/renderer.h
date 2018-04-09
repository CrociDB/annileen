#ifndef _RENDERER_H_
#define _RENDERER_H_

#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <glm.hpp>

class Renderer
{
private:
    SDL_Window* m_Window;
    SDL_GLContext m_Context;

public:
    void init(uint16_t width, uint16_t height);

    void renderFrame();

    SDL_Window* getSDLWindow();

    Renderer();
    ~Renderer();
};

#endif
