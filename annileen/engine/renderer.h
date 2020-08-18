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
        uint16_t shadowMapSize;
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
        std::list<Light*> m_SceneLights;
        std::list<SceneNodePtr> m_SceneNodes;
        Skybox* m_Skybox;

        RenderView* m_SceneRenderView;
        RenderView* m_ShadowRenderView;
        RenderView* m_SkyboxRenderView;

        void initializeShadows();

        void renderSkybox(bgfx::ViewId viewId, Camera* camera, Skybox* skybox);
        void renderSceneNode(bgfx::ViewId viewId, Scene* scene, SceneNodePtr node, std::shared_ptr<Material> material);

    public:
        void init(Engine* engine);

        void setActiveCamera(Camera* camera);
        void setSceneNodes(std::list<SceneNodePtr> sceneNodes);
        void setSceneLights(std::list<Light*> sceneLights);
        void setSkybox(Skybox* skybox) { m_Skybox = skybox; };
        void render();

        void clear(bgfx::ViewId viewId, const glm::vec3& color) const;
        void clear(bgfx::ViewId viewId) const;
        void initFrame(Scene* scene);

        const bgfx::Caps* getCapabilities() const;

        bool useShadows;

        Renderer();
        ~Renderer();
    };
}
