#include "engine.h"
#include "renderer.h"


void Engine::init(uint16_t width, uint16_t height)
{
    m_Width = width;
    m_Height = height;

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    m_Window = SDL_CreateWindow("Clonecraft", 300, 300, width, height, SDL_WINDOW_OPENGL);
    
    m_Renderer = new Renderer();
    m_Renderer->init(this);

    m_Running = true;
}

Renderer* Engine::getRenderer()
{
    return m_Renderer;
}

SDL_Window* Engine::getSDLWindow()
{
    return m_Window;
}

uint16_t Engine::getWidth()
{
    return m_Width;
}

uint16_t Engine::getHeight()
{
    return m_Height;
}

void Engine::setScene(Scene* scene)
{
    m_CurrentScene = scene;
}

bool Engine::isRunning()
{
    return m_Running;
}

void Engine::checkInputEvents()
{
    while (SDL_PollEvent(&m_WindowEvent))
    {
        if (m_WindowEvent.type == SDL_QUIT)
        {
            m_Running = false;
            break;
        }
    }
}

void Engine::renderFrame()
{
    m_Renderer->renderFrame();
}

Engine::Engine()
{
}

Engine::~Engine()
{
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
}
