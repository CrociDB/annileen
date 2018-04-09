#include "renderer.h"

#include <iostream>

Renderer::Renderer()
{
}

void Renderer::init(Engine* engine)
{
    m_Engine = engine;
    m_Context = SDL_GL_CreateContext(engine->getSDLWindow());

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failet initializing GLEW" << std::endl;
    }

    glViewport(0, 0, engine->getWidth(), engine->getHeight());
}

void Renderer::renderFrame()
{
    glClearColor(0.28f, 0.28f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    SDL_GL_SwapWindow(m_Engine->getSDLWindow());
}

Renderer::~Renderer()
{
    SDL_GL_DeleteContext(m_Context);
}
