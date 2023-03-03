module;

#include <iostream>
#include <memory>
#include <bx/math.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <bgfx/bgfx.h>
#include <bimg/bimg.h>

#include <engine/core/logger.h>

export module renderer;

import serviceprovider;
import shaderpass;
import material;
import mesh;
import text;
import renderview;
import uniform;
import skybox;
import light;
import scenemanager;
import scene;
import transform;
import camera;
import scenenode;
import utils;
import texture;
import model;

export namespace annileen
{
    struct Shadow
    {
        bool useShadowSampler{ false };
        std::shared_ptr<Material> material{ nullptr };
        bgfx::FrameBufferHandle frameBuffer{ 0 };
        std::shared_ptr<Texture> texture{ nullptr };
        uint8_t textureRegisterId{ 0 };
    };

    class Renderer
    {
    private:
        const bgfx::Caps* m_Capabilities{ nullptr };
        const bgfx::ViewId m_ViewId{ 0 };

    public:
        Renderer() = default;
        ~Renderer();

    public:
        void init(int screenWidth, int screenHeight);
        void render(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Camera>& camera);
        void initFrame(const std::shared_ptr<Scene>& scene);
        const bgfx::Caps* getCapabilities() const noexcept;

    private:
        void initializeShadows();

        void renderSkybox(bgfx::ViewId viewId, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Skybox>& skybox);
        void renderSceneNode(bgfx::ViewId viewId, const std::shared_ptr<Model>& model, const std::shared_ptr<Material>& material);
   
    public:
        bool useShadows{ false };

    private:
        int m_ScreenWidth{ 0 };
        int m_ScreenHeight{ 0 };

        std::unique_ptr<Shadow> m_Shadow{ nullptr };

        RenderView* m_SceneRenderView{ nullptr };
        RenderView* m_ShadowRenderView{ nullptr };
        RenderView* m_SkyboxRenderView{ nullptr };
        RenderView* m_UIRenderView{ nullptr };
    };
}

module :private;

namespace annileen
{
    Renderer::~Renderer()
    {
        //TODO: remove
        std::cout << "Renderer destroyed." << std::endl;
    }

    void Renderer::initializeShadows()
    {
        m_Shadow = std::make_unique<Shadow>();

        bgfx::VertexLayout ms_layout;
        ms_layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Uint8, true, true)
            .end();
        // Shadow samplers are supported at least partially supported if texture
        // compare less equal feature is supported.
        m_Shadow->useShadowSampler = 0 != (m_Capabilities->supported & BGFX_CAPS_TEXTURE_COMPARE_LEQUAL);;

        auto shader{ ServiceProvider::getAssetManager()->getShader("sms_shadow") };
        auto shaderPass{ std::make_shared<ShaderPass>() };

        shaderPass->init(shader);
        shaderPass->setState(0
            | (m_Shadow->useShadowSampler ? 0 : BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A)
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | BGFX_STATE_CULL_CW
            | BGFX_STATE_MSAA);

        m_Shadow->material = std::make_shared<Material>();
        m_Shadow->material->setName("ShadowMaterial");
        m_Shadow->material->addShaderPass(shaderPass);

        bgfx::TextureHandle fbtextures[]
        {
            bgfx::createTexture2D(
                  static_cast<uint16_t>(ServiceProvider::getSettings()->getData()->shadows.shadowMapSize)
                , static_cast<uint16_t>(ServiceProvider::getSettings()->getData()->shadows.shadowMapSize)
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
            static_cast<uint16_t>(ServiceProvider::getSettings()->getData()->shadows.shadowMapSize),
            static_cast<uint16_t>(ServiceProvider::getSettings()->getData()->shadows.shadowMapSize),
            1,
            false,
            false,
            1,
            bgfx::TextureFormat::D16);

        m_Shadow->texture = std::make_shared<Texture>(fbtextures[0], textureInfo, bimg::Orientation::R0);
        m_Shadow->textureRegisterId = 1;
    }

    void Renderer::init(int screenWidth, int screenHeight)
    {
        m_ScreenWidth = screenWidth;
        m_ScreenHeight = screenHeight;

        m_Capabilities = bgfx::getCaps();

        initializeShadows();

        // Initialize Reserved Render Views
        RenderView::addReservedRenderView(RenderView::Shadow, "Shadows");
        RenderView::addReservedRenderView(RenderView::Scene, "Scene");
        RenderView::addReservedRenderView(RenderView::Skybox, "Skybox");
        RenderView::addReservedRenderView(RenderView::UI, "UI");
        //RenderView::addReservedRenderView(RenderView::PostProcessing, "Post-processing");
        
        m_ShadowRenderView = RenderView::getRenderView(RenderView::Shadow);
        m_SceneRenderView = RenderView::getRenderView(RenderView::Scene);
        m_SkyboxRenderView = RenderView::getRenderView(RenderView::Skybox);
        m_UIRenderView = RenderView::getRenderView(RenderView::UI);
    }

    void Renderer::render(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Camera>& camera)
    {
        std::shared_ptr<Light> mainLightForShadows{ nullptr };
        
        // Set light properties and get first light that generate shadows to be main light for shadows.
        for (const auto& light : scene->getLightList())
        {
            if (light->type == LightType::Directional)
            {
                Uniform::setVec3Uniform("u_lightDirection", light->getTransform().getForward());
                Uniform::setVec3Uniform("u_lightColor", light->color);
                Uniform::setFloatUniform("u_lightIntensity", light->intensity);
            }

            if (light->generateShadows && mainLightForShadows == nullptr)
            {
                mainLightForShadows = light;
            }
        }

        glm::mat4 mtxShadow;
        glm::mat4 lightMtx;

        if (ServiceProvider::getSettings()->getData()->shadows.enabled && mainLightForShadows != nullptr && mainLightForShadows->generateShadows)
        {
            glm::mat4 lightView;
            
            if (mainLightForShadows->type == LightType::Directional)
            {
				lightView = glm::lookAt(mainLightForShadows->getTransform().getForward(), glm::vec3(0, 0, 0), mainLightForShadows->getTransform().getUp());
                lightView = glm::translate(lightView, -camera->getTransform().position());
            }
            else
            {
                //TODO: This has to be tested as soon as we get other type of lights
                auto at{ mainLightForShadows->getTransform().position() + mainLightForShadows->getTransform().getForward() };
                lightView = glm::lookAt(mainLightForShadows->getTransform().position(), at, mainLightForShadows->getTransform().getUp());
            }
            
            const float area{ 50.0f };
            auto lightProj{ glm::ortho(-area, area, -area, area, -100.0f, 100.0f) };

            // Setup shadow
            bgfx::setViewRect(m_ShadowRenderView->getViewId(), 0, 0, 
                ServiceProvider::getSettings()->getData()->shadows.shadowMapSize,
                ServiceProvider::getSettings()->getData()->shadows.shadowMapSize);
            bgfx::setViewFrameBuffer(m_ShadowRenderView->getViewId(), m_Shadow->frameBuffer);
            bgfx::setViewTransform(m_ShadowRenderView->getViewId(), glm::value_ptr(lightView), glm::value_ptr(lightProj));

            // Clear backbuffer and shadowmap framebuffer at beginning.
            bgfx::setViewClear(m_ShadowRenderView->getViewId(), BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

            const float sy{ m_Capabilities->originBottomLeft ? 0.5f : -0.5f };
            const float sz{ m_Capabilities->homogeneousDepth ? 0.5f : 1.0f };
            const float tz{ m_Capabilities->homogeneousDepth ? 0.5f : 0.0f };
            const glm::mat4 mtxCrop
            {
                0.5f, 0.0f, 0.0f, 0.0f,
                0.0f,   sy, 0.0f, 0.0f,
                0.0f, 0.0f, sz,   0.0f,
                0.5f, 0.5f, tz,   1.0f,
            };

            mtxShadow = mtxCrop * lightProj * lightView;

            for (auto& sceneNode : scene->getNodeList())
            {
                auto model{ SceneManager::getInstance()->getModule<Model>(sceneNode) };

                if (model == nullptr || !sceneNode->getActive() || !model->enabled) continue;

                if (ServiceProvider::getSettings()->getData()->shadows.enabled && model->castShadows)
                {
                    renderSceneNode(m_ShadowRenderView->getViewId(), model, m_Shadow->material);
                }
            }
        }

        // Setup fog
        glm::vec3 settings{
            scene->fog.distance,
            scene->fog.power,
            scene->fog.enabled,
        };
        Uniform::setVec3Uniform("u_fogSettings", settings);
        Uniform::setVec3Uniform("u_fogColor", scene->fog.color);

        // Setup camera
        camera->updateMatrices(m_ScreenWidth, m_ScreenHeight);
        bgfx::setViewTransform(m_SceneRenderView->getViewId(), camera->getViewMatrixFloatArray(), camera->getProjectionMatrixFloatArray());
        bgfx::setViewRect(m_SceneRenderView->getViewId(), 0, 0, m_ScreenWidth, m_ScreenHeight);

        // Clear backbuffer and shadowmap framebuffer at beginning.
        bgfx::setViewClear(m_SceneRenderView->getViewId(), BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 
            convert_color_vec3_uint32(camera->clearColor), 1.0f, 0);
        bgfx::touch(m_SceneRenderView->getViewId());

        Uniform::setVec3Uniform("u_viewPos", camera->getTransform().position());

        for (auto& sceneNode : scene->getNodeList())
        {
            auto model{ SceneManager::getInstance()->getModule<Model>(sceneNode) };

            if (model == nullptr || model->getMeshGroup() == nullptr || !sceneNode->getActive() || !model->enabled) continue;

            if (ServiceProvider::getSettings()->getData()->shadows.enabled && model->receiveShadows)
            {
                lightMtx = mtxShadow * sceneNode->getTransform().getModelMatrix();
                Uniform::setMat4Uniform("u_lightMtx", lightMtx);
                Uniform::setTextureUniform("s_shadowMap", m_Shadow->texture, m_Shadow->textureRegisterId);
            }

            renderSceneNode(m_SceneRenderView->getViewId(), model, model->getMaterial());
        }

        const bx::Vec3 at { 0.0f, 0.0f,  0.0f };
        const bx::Vec3 eye { 0.0f, 0.0f, -1.0f };

        float view[16];
        bx::mtxLookAt(view, eye, at);

        const float centering{ 0.5f };
        
        float ortho[16];
        bx::mtxOrtho(
            ortho
            , centering
            , m_ScreenWidth + centering
            , m_ScreenHeight + centering
            , centering
            , 0.0f
            , 100.0f
            , 0.0f
            , m_Capabilities->homogeneousDepth
        );
        bgfx::setViewTransform(m_UIRenderView->getViewId(), view, ortho);
        bgfx::setViewRect(m_UIRenderView->getViewId(), 0, 0, m_ScreenWidth, m_ScreenHeight);
        
        for (auto& sceneNode : scene->getNodeList())
        {
            auto text{ SceneManager::getInstance()->getModule<Text>(sceneNode) };

            if (!sceneNode->getActive() || text == nullptr || !text->enabled ) continue;

            text->render(m_UIRenderView->getViewId());
        }

        if (camera->clearType == CameraClearType::CameraClearSkybox)
        {
            if (scene->getSkybox() != nullptr)
            {
                bgfx::setViewRect(m_SkyboxRenderView->getViewId(), 0, 0, m_ScreenWidth, m_ScreenHeight);
                bgfx::setViewTransform(m_SkyboxRenderView->getViewId(), camera->getViewRotationMatrixFloatArray(), camera->getProjectionMatrixFloatArray());
                renderSkybox(m_SkyboxRenderView->getViewId(), camera, scene->getSkybox());
            }
            else
            {
                ANNILEEN_LOG_WARNING(LoggingChannel::Renderer, "Camera clear type is set for skybox, but no skybox was set.");
            }
        }
    }

    void Renderer::initFrame(const std::shared_ptr<Scene>& scene)
    {
        scene->getCamera()->updateMatrices(m_ScreenWidth, m_ScreenHeight);
    }

    void Renderer::renderSkybox(bgfx::ViewId viewId, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Skybox>& skybox)
    {
        skybox->getModel()->getMaterial()->submitUniforms();

        auto meshGroup{ skybox->getModel()->getMeshGroup() };
        for (auto& mesh : meshGroup->m_Meshes)
        {
            bgfx::setVertexBuffer(0, mesh->getVertexBuffer());
            bgfx::setIndexBuffer(mesh->getIndexBuffer());
      
            auto material{ skybox->getModel()->getMaterial() };

            for (int shaderPassId{ 0 }; shaderPassId < material->getNumberOfShaderPasses(); ++shaderPassId)
            {
                auto shaderPass{ material->getShaderPassAt(shaderPassId) };
            
                bgfx::setState(shaderPass->getState());
                bgfx::submit(viewId, shaderPass->getShader()->getProgram());
            }
        }
    }

    void Renderer::renderSceneNode(bgfx::ViewId viewId, const std::shared_ptr<Model>& model, const std::shared_ptr<Material>& material)
    {
        material->submitUniforms();

        auto meshGroup{ model->getMeshGroup() };

        if (meshGroup == nullptr)
        {
            return;
        }

        for (auto& mesh : meshGroup->m_Meshes)
        {
            bgfx::setTransform(glm::value_ptr(model->getTransform().getModelMatrix()));
            bgfx::setVertexBuffer(0, mesh->getVertexBuffer());
            if (mesh->hasIndices())
                bgfx::setIndexBuffer(mesh->getIndexBuffer());
        
            for (int shaderPassId{ 0 }; shaderPassId < material->getNumberOfShaderPasses(); ++shaderPassId)
            {
                auto shaderPass{ material->getShaderPassAt(shaderPassId) };

                bgfx::setState(shaderPass->getState());
                bgfx::submit(viewId, shaderPass->getShader()->getProgram());
            }
        }
    }

    const bgfx::Caps* Renderer::getCapabilities() const noexcept
    {
        return m_Capabilities;
    } 
}
