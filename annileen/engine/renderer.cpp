#include "renderer.h"

#include <iostream>

namespace annileen
{
    Renderer::Renderer()
    {
    }

    void Renderer::init(Engine* engine)
    {
        m_Engine = engine;

        clear();
    }

    void Renderer::clear(const glm::vec3& color) const
    {
        bgfx::setViewClear(m_ViewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, convert_color_vec3_uint32(color), 1.0f);
        bgfx::setViewRect(m_ViewId, 0, 0, bgfx::BackbufferRatio::Equal);
    }

    void Renderer::clear() const
    {
        clear(glm::vec3(0.2, 0.3, 1.0));
    }

    void Renderer::initFrame(Scene* scene)
    {
        scene->getCamera()->updateMatrices();
    }

    void Renderer::renderSceneNode(Scene* scene, SceneNode* node)
    {
        bgfx::setTransform(glm::value_ptr(node->getTransform().getModelMatrix()));
        bgfx::setVertexBuffer(0, node->getModel()->getMesh()->getVertexBuffer());
        if (node->getModel()->getMesh()->hasIndices()) 
            bgfx::setIndexBuffer(node->getModel()->getMesh()->getIndexBuffer());
        bgfx::setState(node->getModel()->getState());

        bgfx::submit(0, node->getModel()->getMaterial()->getShader()->getProgram());
    }

    //void Renderer::renderSkybox(Camera* camera, Skybox* skybox)
    //{
    //    glDepthMask(GL_FALSE);
    //    glDepthFunc(GL_LEQUAL);
    //    auto material = skybox->getMesh()->getMaterial();
    //    material->getShader()->use();
    //    material->useTextures();
    //
    //    glm::mat4 view = glm::mat4(glm::mat3(camera->getViewMatrix()));
    //
    //    material->getShader()->setMat4("view", view);
    //    material->getShader()->setMat4("projection", camera->getProjectionMatrix());
    //
    //    skybox->getMesh()->bindAndDraw();
    //    glDepthMask(GL_TRUE);
    //}
    //
    //void Renderer::renderMesh(Scene* scene, Mesh* mesh)
    //{
    //    glDepthFunc(GL_LESS);
    //    glEnable(GL_CULL_FACE);
    //    glCullFace(GL_BACK);
    //
    //    Camera* camera = scene->getCamera();
    //
    //    auto material = mesh->getMaterial();
    //    material->getShader()->use();
    //    material->useTextures();
    //
    //    glm::mat4 transform = mesh->transform.getModelMatrix();
    //
    //    // Lighting
    //    material->getShader()->setVec3("view_position", camera->transform.position);
    //
    //    material->getShader()->setVec3("fog_color", scene->fog.color);
    //    material->getShader()->setFloat("fog_dist", scene->fog.distance);
    //    material->getShader()->setFloat("fog_pow", scene->fog.power);
    //    material->getShader()->setFloat("fog_enabled", scene->fog.enabled);
    //
    //
    //    for (auto const& light : scene->getLightList())
    //    {
    //        if (light->type == LightDirectional)
    //        {
    //            material->getShader()->setVec3("light_direction", light->transform.getForward());
    //            material->getShader()->setVec3("light_color", light->color);
    //            material->getShader()->setFloat("light_intensity", light->intensity);
    //        }
    //    }
    //
    //    // Matrixes
    //    material->getShader()->setMat4("model", transform);
    //    material->getShader()->setMat4("view", camera->getViewMatrix());
    //    material->getShader()->setMat4("projection", camera->getProjectionMatrix());
    //
    //    mesh->bindAndDraw();
    //    glDisable(GL_CULL_FACE);
    //}

    Renderer::~Renderer()
    {
    }
}
