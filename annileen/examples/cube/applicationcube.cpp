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
        getEngine()->setScene(scene);

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


        m_ModelNode = scene->createNode("Model");
        ModelPtr model = m_ModelNode->addModule<Model>();
        model->init(ServiceProvider::getAssetManager()->loadMesh("statue.obj"), material);

        m_ModelNode->getTransform().translate(glm::vec3(-10.0, -10.0, -10.0));
        m_ModelNode->getTransform().scale = glm::vec3(0.05, 0.05, 0.05);
        //modelNode->getTransform().rotate(glm::vec3(0.0, 45.0, 0.0));

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

        Camera* camera = getEngine()->getScene()->getCamera();
        m_MovementSpeed = getEngine()->getInput()->getKey(GLFW_KEY_LEFT_SHIFT) ? 5.0 * m_Speed : m_Speed;

        if (getEngine()->getInput()->getKey(GLFW_KEY_S))
        {
            camera->getTransform().translate(deltaTime * -m_MovementSpeed * camera->getForward());
        }
        if (getEngine()->getInput()->getKey(GLFW_KEY_W))
        {
            camera->getTransform().translate(deltaTime * m_MovementSpeed * camera->getForward());
        }
        if (getEngine()->getInput()->getKey(GLFW_KEY_A))
        {
            camera->getTransform().translate(deltaTime * m_MovementSpeed * camera->getRight());
        }
        if (getEngine()->getInput()->getKey(GLFW_KEY_D))
        {
            camera->getTransform().translate(deltaTime * -m_MovementSpeed * camera->getRight());
        }
        if (getEngine()->getInput()->getKey(GLFW_KEY_Q))
        {
            camera->getTransform().translate(deltaTime * -m_MovementSpeed * glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (getEngine()->getInput()->getKey(GLFW_KEY_E))
        {
            camera->getTransform().translate(deltaTime * m_MovementSpeed * glm::vec3(0.0f, 1.0f, 0.0f));
        }

        if (m_CameraActive)
        {
            // Camera mouse control
            auto mouseDelta = getEngine()->getInput()->getMouseDelta();

            m_Yaw += mouseDelta.x * m_Sensitivity * deltaTime;
            m_Pitch += -mouseDelta.y * m_Sensitivity * deltaTime;
            m_Pitch = glm::clamp(m_Pitch, -89.0f, 89.0f);
        }

        glm::vec3 cameraForward{
            glm::cos(glm::radians(m_Pitch)) * glm::cos(glm::radians(m_Yaw)),
            glm::sin(glm::radians(m_Pitch)),
            glm::cos(glm::radians(m_Pitch)) * glm::sin(glm::radians(m_Yaw))
        };
        camera->setForward(glm::normalize(cameraForward));


        if (getEngine()->getInput()->getKeyDown(GLFW_KEY_ESCAPE))
        {
            getEngine()->terminate();
        }

        if (getEngine()->getInput()->getKeyDown(GLFW_KEY_F1))
        {
            m_DebugActive = (m_DebugActive + 1) % 6;
            switch (m_DebugActive)
            {
            case 1:
                bgfx::setDebug(BGFX_DEBUG_STATS);
                break;
            case 2:
                bgfx::setDebug(BGFX_DEBUG_PROFILER);
                break;
            case 3:
                bgfx::setDebug(BGFX_DEBUG_IFH);
                break;
            case 4:
                bgfx::setDebug(BGFX_DEBUG_TEXT);
                break;
            case 5:
                bgfx::setDebug(BGFX_DEBUG_WIREFRAME);
                break;
            case 0:
            default:
                bgfx::setDebug(BGFX_DEBUG_NONE);
                break;
            }
        }

        if (getEngine()->getInput()->getKeyDown(GLFW_KEY_F2))
        {
            m_CameraActive = !m_CameraActive;
            getEngine()->setMouseCapture(m_CameraActive);
        }
    }

    void finish() {}

public:
    ApplicationCube() {}
    ~ApplicationCube() {}
};

ANNILEEN_APP_MAIN(ApplicationCube, "Cube")
