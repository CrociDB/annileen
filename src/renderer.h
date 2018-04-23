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

    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ProjectionMatrix;

    void initMatrices();

public:
    void init(Engine* engine);

    void clear();
    void swapBuffer();
    void renderMesh(Mesh* mesh);

    Renderer();
    ~Renderer();
};

#endif
