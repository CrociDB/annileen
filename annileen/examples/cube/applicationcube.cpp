#define ANNILEEN_APPLICATION
#include "definitions.h"

#include <glm.hpp>

#include <engine/scene.h>
#include <engine/shaderpass.h>
#include <engine/serviceprovider.h>
#include <engine/material.h>
#include <engine/mesh.h>

using namespace annileen;

ANNILEEN_APP_CLASS_DECLARATION(ApplicationCube)
{
private:
    float m_Speed;
    float m_MovementSpeed;
    float m_Sensitivity;
    float m_Pitch, m_Yaw;
    bool m_CameraActive = false;
    uint8_t m_DebugActive = 0;

    SceneNodePtr m_ModelNode = nullptr;

    annileen::Scene* init()
    {
        Scene* scene = new Scene();
        //getEngine()->setScene(scene);

        annileen::Shader* shader = nullptr;
        if (ServiceProvider::getSettings()->shadows.enabled)
        {
            shader = ServiceProvider::getAssetManager()->loadShader("voxel_shadow.vs", "voxel_shadow.fs");
        }
        else
        {
            shader = ServiceProvider::getAssetManager()->loadShader("voxel_noshadow.vs", "voxel_noshadow.fs");
        }

        std::shared_ptr<ShaderPass> shaderPass = std::make_shared<ShaderPass>();
        shaderPass->init(shader);
        shaderPass->setState(BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | BGFX_STATE_CULL_CW
            | BGFX_STATE_MSAA
            | UINT64_C(0));

        std::shared_ptr<Material> material = std::make_shared<Material>();
        material->addShaderPass(shaderPass);
        material->setName("ModelMaterial");


        m_ModelNode = scene->createNode("Model");
        ModelPtr model = m_ModelNode->addModule<Model>();
        model->init(ServiceProvider::getAssetManager()->loadMesh("bunny.obj"), material);

        m_ModelNode->getTransform().translate(glm::vec3(-1.0, -1.0, -1.0));
         
        SceneNodePtr cameraNode = scene->createNode("Camera");
        Camera* camera = cameraNode->addModule<Camera>(); 
        camera->fieldOfView = 60.0f;
        camera->nearClip = 0.1f;
        camera->farClip = 300.0f;
        
        SceneNodePtr lightNode = scene->createNode("Light");
        Light* light = lightNode->addModule<Light>();

        light->color = glm::vec3(1.0f, 1.0f, .8f);
        light->type = LightType::Directional;
        light->intensity = 0.8f;
        light->getTransform().rotate(glm::vec3(-40.0f, 0.0f, -40.0f));

        // Initialize Camera
        m_Speed = 3.0f;
        m_MovementSpeed = 2.8f;
        m_Sensitivity = 18.0f;
        m_Pitch = -6.0f;
        m_Yaw = -128.0f;

        m_CameraActive = false;
        m_DebugActive = 0;

        return scene;
    }

    void update(float deltaTime)
    {
        m_ModelNode->getTransform().rotate(glm::vec3(0.0, 15.0 * deltaTime, 0.0));
    }

    void finish() {}

public:
    ApplicationCube() {}
    ~ApplicationCube() {}
};

ANNILEEN_APP_MAIN(ApplicationCube, "Cube")
