#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "engine.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Renderer
{
private:
    Engine* m_Engine;
    SDL_GLContext m_Context;

public:
    void init(Engine* engine);

    void clear();
    void initFrame(Scene* scene);
    void swapBuffer();
    void renderMesh(Camera* camera, Mesh* mesh);

    Renderer();
    ~Renderer();
};

#endif
