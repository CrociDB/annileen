#pragma once


#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <bgfx/bgfx.h>

#include <engine/engine.h>
#include <engine/renderview.h>

namespace annileen
{
    struct Shadow
    {
        bool useShadowSampler;
        std::shared_ptr<Material> material;
        bgfx::FrameBufferHandle frameBuffer;
        Texture* texture;
        uint8_t textureRegisterId;
    };

    class Scene;

    class Renderer
    {
    private:
        Engine* m_Engine;
        const bgfx::Caps* m_Capabilities;
        const bgfx::ViewId m_ViewId = 0;
        
        Uniform m_Uniform;

        Shadow* m_Shadow;
        Camera* m_ActiveCamera;
        Scene* m_Scene;

        RenderView* m_SceneRenderView;
        RenderView* m_ShadowRenderView;
        RenderView* m_SkyboxRenderView;
        RenderView* m_UIRenderView;

        void initializeShadows();

        void renderSkybox(bgfx::ViewId viewId, Camera* camera, Skybox* skybox);
        void renderSceneNode(bgfx::ViewId viewId, ModelPtr model, std::shared_ptr<Material> material);

    public:
        void init(Engine* engine);

        void setActiveCamera(Camera* camera);
        void setScene(Scene* scene);

        void render();

        void initFrame(Scene* scene);

        const bgfx::Caps* getCapabilities() const;

        bool useShadows;

        Renderer();
        ~Renderer();
    };
}
