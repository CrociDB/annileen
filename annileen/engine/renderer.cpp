#include "renderer.h"

#include <iostream>

namespace annileen
{
    Renderer::Renderer() : useShadows(false), m_Engine(nullptr), m_Capabilities(nullptr), m_Shadow(nullptr)
    {
    }


    void Renderer::initializeShadows()
    {
        m_Shadow = new Shadow();

        bgfx::VertexLayout ms_layout;
        ms_layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Uint8, true, true)
            .end();

        m_Shadow->shadowMapSize = 1024;

        // Shadow samplers are supported at least partially supported if texture
        // compare less equal feature is supported.
        m_Shadow->useShadowSampler = 0 != (m_Capabilities->supported & BGFX_CAPS_TEXTURE_COMPARE_LEQUAL);;

        Shader* shader = Engine::getInstance()->getAssetManager()->loadShader("vs_sms_shadow.vs", "fs_sms_shadow.fs");
        std::shared_ptr<ShaderPass> shaderPass = std::make_shared<ShaderPass>();

        shaderPass->init(shader);
        shaderPass->setState(0
            | (m_Shadow->useShadowSampler ? 0 : BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A)
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | BGFX_STATE_CULL_CCW
            | BGFX_STATE_MSAA);

        m_Shadow->material = std::make_shared<Material>();
        m_Shadow->material->addShaderPass(shaderPass);

        bgfx::TextureHandle fbtextures[] =
        {
            bgfx::createTexture2D(
                  m_Shadow->shadowMapSize
                , m_Shadow->shadowMapSize
                , false
                , 1
                , bgfx::TextureFormat::D16
                , BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL
                ),
        };

        m_Shadow->frameBuffer = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);

        bgfx::TextureInfo textureInfo;
        bgfx::calcTextureSize(
            textureInfo,
            uint16_t(m_Shadow->shadowMapSize),
            uint16_t(m_Shadow->shadowMapSize),
            1,
            false,
            false,
            1,
            bgfx::TextureFormat::D16);

        m_Shadow->texture = new Texture(fbtextures[0], textureInfo, bimg::Orientation::R0);
        m_Shadow->textureRegisterId = 1;
    }

    void Renderer::init(Engine* engine)
    {
        m_Engine = engine;

        m_Capabilities = bgfx::getCaps();

        initializeShadows();

        // Initialize Reserved Render Views
        RenderView::addReservedRenderView(RenderView::Shadow, "Shadows");
        RenderView::addReservedRenderView(RenderView::Scene, "Scene");
        RenderView::addReservedRenderView(RenderView::Skybox, "Skybox");
        //RenderView::addReservedRenderView(RenderView::PostProcessing, "Post-processing");
        
        m_ShadowRenderView = RenderView::getRenderView(RenderView::Shadow);
        m_SceneRenderView = RenderView::getRenderView(RenderView::Scene);
        m_SkyboxRenderView = RenderView::getRenderView(RenderView::Skybox);
    }

    void Renderer::setActiveCamera(Camera* camera)
    {
        m_ActiveCamera = camera;
        // TODO: update setup
    }

    void Renderer::setSceneNodes(std::list<SceneNodePtr> sceneNodes)
    {
        m_SceneNodes = sceneNodes;
    }

    void Renderer::setSceneLights(std::list<Light*> sceneLights)
    {
        m_SceneLights = sceneLights;

        // Setup lights
        for (const auto& light : m_SceneLights)
        {
            if (light->type == Directional)
            {
                m_Uniform.setVec3Uniform("u_lightDirection", light->transform.getForward());
                m_Uniform.setVec3Uniform("u_lightColor", light->color);
                m_Uniform.setFloatUniform("u_lightIntensity", light->intensity);
            }
        }
    }

    void Renderer::render()
    {
        // Temp: for shadows
        // Setup lights.            
        // Define matrices.
        glm::vec3 lightPos = glm::vec3(0.0, 30.0, 0.0);
        const glm::vec3 eye = lightPos;
        const glm::vec3 at = glm::vec3(7.0f, 20.0f, 7.0f);
        glm::mat4 lightView = glm::lookAt(eye, at, glm::vec3(0, 1, 0));

        const float area = 30.0f;
        glm::mat4 lightProj = glm::ortho(-area, area, -area, area, -100.0f, 100.0f);

        // Setup shadow
        bgfx::setViewRect(m_ShadowRenderView->getViewId(), 0, 0, m_Shadow->shadowMapSize, m_Shadow->shadowMapSize);
        bgfx::setViewFrameBuffer(m_ShadowRenderView->getViewId(), m_Shadow->frameBuffer);
        bgfx::setViewTransform(m_ShadowRenderView->getViewId(), glm::value_ptr(lightView), glm::value_ptr(lightProj));

        // Clear backbuffer and shadowmap framebuffer at beginning.
        bgfx::setViewClear(m_ShadowRenderView->getViewId(), BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

        const float sy = m_Capabilities->originBottomLeft ? 0.5f : -0.5f;
        const float sz = m_Capabilities->homogeneousDepth ? 0.5f : 1.0f;
        const float tz = m_Capabilities->homogeneousDepth ? 0.5f : 0.0f;
        const glm::mat4 mtxCrop =
        {
            0.5f, 0.0f, 0.0f, 0.0f,
            0.0f,   sy, 0.0f, 0.0f,
            0.0f, 0.0f, sz,   0.0f,
            0.5f, 0.5f, tz,   1.0f,
        };

        glm::mat4 mtxShadow = mtxCrop * lightProj * lightView;
        glm::mat4 lightMtx;

        for (auto sceneNode : m_SceneNodes)
        {
            if (!sceneNode->hasModel() || !sceneNode->getAcive()) continue;

            renderSceneNode(m_ShadowRenderView->getViewId(), nullptr, sceneNode, m_Shadow->material);
        }

        // Setup camera
        m_ActiveCamera->updateMatrices();
        bgfx::setViewTransform(m_SceneRenderView->getViewId(), m_ActiveCamera->getViewMatrixFloatArray(), m_ActiveCamera->getProjectionMatrixFloatArray());
        bgfx::setViewRect(m_SceneRenderView->getViewId(), 0, 0, Engine::getInstance()->getWidth(), Engine::getInstance()->getHeight());

        // Clear backbuffer and shadowmap framebuffer at beginning.
        bgfx::setViewClear(m_SceneRenderView->getViewId(), BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

        m_Uniform.setVec3Uniform("u_viewPos", m_ActiveCamera->transform().position);

        for (auto sceneNode : m_SceneNodes)
        {
            if (!sceneNode->hasModel() || !sceneNode->getAcive()) continue;

            lightMtx = mtxShadow * sceneNode->getTransform().getModelMatrix();
            m_Uniform.setMat4Uniform("u_lightMtx", lightMtx);
            m_Uniform.setTextureUniform("s_shadowMap",  m_Shadow->texture, m_Shadow->textureRegisterId);

            m_Shadow->material->submitUniforms();
            renderSceneNode(m_SceneRenderView->getViewId(), nullptr, sceneNode, sceneNode->getModel()->getMaterial());
        }

        if (m_ActiveCamera->clearType == CameraClearType::CameraClearSkybox)
        {
            bgfx::setViewRect(m_SkyboxRenderView->getViewId(), 0, 0, Engine::getInstance()->getWidth(), Engine::getInstance()->getHeight());
            bgfx::setViewTransform(m_SkyboxRenderView->getViewId(), m_ActiveCamera->getViewRotationMatrixFloatArray(), m_ActiveCamera->getProjectionMatrixFloatArray());
            renderSkybox(m_SkyboxRenderView->getViewId(), m_ActiveCamera, m_Skybox);
        }
    }

    // TODO: Move this to camera
    void Renderer::clear(bgfx::ViewId viewId, const glm::vec3& color) const
    {
        bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, convert_color_vec3_uint32(color), 1.0f);
        bgfx::setViewRect(viewId, 0, 0, bgfx::BackbufferRatio::Equal);
    }

    void Renderer::clear(bgfx::ViewId viewId) const
    {
        clear(viewId, glm::vec3(0.2, 0.3, 1.0));
    }

    void Renderer::initFrame(Scene* scene)
    {
        scene->getCamera()->updateMatrices();
    }

    void Renderer::renderSkybox(bgfx::ViewId viewId, Camera* camera, Skybox* skybox)
    {
        skybox->getModel()->getMaterial()->submitUniforms();

        bgfx::setVertexBuffer(0, skybox->getModel()->getMesh()->getVertexBuffer());
        bgfx::setIndexBuffer(skybox->getModel()->getMesh()->getIndexBuffer());
      
        std::shared_ptr<Material> material = skybox->getModel()->getMaterial();

        for (int shaderPassId = 0; shaderPassId < material->getNumberOfShaderPasses(); ++shaderPassId)
        {
            std::shared_ptr<ShaderPass> shaderPass = material->getShaderPassAt(shaderPassId);
            
            bgfx::setState(shaderPass->getState());
            bgfx::submit(viewId, shaderPass->getShader()->getProgram());
        }
    }

    void Renderer::renderSceneNode(bgfx::ViewId viewId, Scene* scene, SceneNodePtr node, std::shared_ptr<Material> material)
    {
        material->submitUniforms();

        bgfx::setTransform(glm::value_ptr(node->getTransform().getModelMatrix()));
        bgfx::setVertexBuffer(0, node->getModel()->getMesh()->getVertexBuffer());
        if (node->getModel()->getMesh()->hasIndices()) 
            bgfx::setIndexBuffer(node->getModel()->getMesh()->getIndexBuffer());
        
        for (int shaderPassId = 0; shaderPassId < material->getNumberOfShaderPasses(); ++shaderPassId)
        {
            std::shared_ptr<ShaderPass> shaderPass = material->getShaderPassAt(shaderPassId);

            bgfx::setState(shaderPass->getState());
            bgfx::submit(viewId, shaderPass->getShader()->getProgram());
        }
    }

    const bgfx::Caps* Renderer::getCapabilities() const
    {
        return m_Capabilities;
    }
   
    Renderer::~Renderer()
    {
    }
}
