#include "renderer.h"

#include <iostream>

Renderer::Renderer()
{
}

void Renderer::initMatrices()
{
    m_ViewMatrix = glm::mat4(1.0f);
    m_ViewMatrix = glm::translate(m_ViewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));

    m_ProjectionMatrix = glm::mat4(1.0f);
    m_ProjectionMatrix = glm::perspective(
        glm::radians(60.0f),
        (float)m_Engine->getWidth() / (float)m_Engine->getHeight(),
        0.1f,
        100.0f);
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initMatrices();
}

void Renderer::clear()
{
    glClearColor(0.28f, 0.28f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::swapBuffer()
{
    SDL_GL_SwapWindow(m_Engine->getSDLWindow());
}

void Renderer::renderMesh(Mesh* mesh)
{
    auto material = mesh->getMaterial();
    material->getShader()->use();
    material->useTextures();

    glm::mat4 transform = mesh->getUpdatedTransformMatrix();
    transform = glm::rotate(transform, 0.001f * SDL_GetTicks(), glm::vec3(1.0f, 1.0f, 0.0f));

    material->getShader()->setMat4("model", transform);
    material->getShader()->setMat4("view", m_ViewMatrix);
    material->getShader()->setMat4("projection", m_ProjectionMatrix);

    mesh->bindAndDraw();
}

Renderer::~Renderer()
{
    SDL_GL_DeleteContext(m_Context);
}
