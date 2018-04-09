#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "engine.h"

class Renderer
{
private:
    Engine* m_Engine;
    SDL_GLContext m_Context;

public:
    void init(Engine* engine);

    void renderFrame();

    Renderer();
    ~Renderer();
};

#endif
