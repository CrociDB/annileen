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

    void Renderer::renderSkybox(Camera* camera, Skybox* skybox)
    {
        skybox->getModel()->getMaterial()->submitUniforms();

        bgfx::setVertexBuffer(0, skybox->getModel()->getMesh()->getVertexBuffer());
        bgfx::setIndexBuffer(skybox->getModel()->getMesh()->getIndexBuffer());
        bgfx::setState(skybox->getModel()->getState());

        bgfx::submit(1, skybox->getModel()->getMaterial()->getShader()->getProgram());
    }

    void Renderer::renderSceneNode(Scene* scene, SceneNodePtr node)
    {
        node->getModel()->getMaterial()->submitUniforms();

        bgfx::setTransform(glm::value_ptr(node->getTransform().getModelMatrix()));
        bgfx::setVertexBuffer(0, node->getModel()->getMesh()->getVertexBuffer());
        if (node->getModel()->getMesh()->hasIndices()) 
            bgfx::setIndexBuffer(node->getModel()->getMesh()->getIndexBuffer());
        bgfx::setState(node->getModel()->getState());

        bgfx::submit(0, node->getModel()->getMaterial()->getShader()->getProgram());
    }
   
    Renderer::~Renderer()
    {
    }
}
