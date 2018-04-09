#include "renderer.h"

#include <iostream>

Renderer::Renderer()
{
}

void Renderer::init(uint16_t width, uint16_t height)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    m_Window = SDL_CreateWindow("Clonecraft", 300, 300, width, height, SDL_WINDOW_OPENGL);
    m_Context = SDL_GL_CreateContext(m_Window);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failet initializing GLEW" << std::endl;
    }

    glViewport(0, 0, width, height);
}

void Renderer::renderFrame()
{
    glClearColor(0.28f, 0.28f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    SDL_GL_SwapWindow(m_Window);
}

SDL_Window* Renderer::getSDLWindow()
{
    return m_Window;
}

Renderer::~Renderer()
{
    SDL_GL_DeleteContext(m_Context);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
}
