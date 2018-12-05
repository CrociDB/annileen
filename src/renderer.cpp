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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::clear(const glm::vec3& color) const
{
    glClearColor(color.x, color.y, color.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::initFrame(Scene* scene)
{
    scene->getCamera()->updateMatrices();
}

void Renderer::swapBuffer()
{
    SDL_GL_SwapWindow(m_Engine->getSDLWindow());
}

void Renderer::renderSkybox(Camera* camera, Skybox* skybox)
{
    glDepthMask(GL_FALSE);
    auto material = skybox->getMesh()->getMaterial();
    material->getShader()->use();
    material->useTextures();

    glm::mat4 view = glm::mat4(glm::mat3(camera->getViewMatrix()));

    material->getShader()->setMat4("view", view);
    material->getShader()->setMat4("projection", camera->getProjectionMatrix());

    skybox->getMesh()->bindAndDraw();
    glDepthMask(GL_TRUE);
}

void Renderer::renderMesh(Camera* camera, Mesh* mesh)
{
    auto material = mesh->getMaterial();
    material->getShader()->use();
    material->useTextures();

    glm::mat4 transform = mesh->transform.getModelMatrix();

    material->getShader()->setMat4("model", transform);
    material->getShader()->setMat4("view", camera->getViewMatrix());
    material->getShader()->setMat4("projection", camera->getProjectionMatrix());

    mesh->bindAndDraw();
}

Renderer::~Renderer()
{
    SDL_GL_DeleteContext(m_Context);
}
