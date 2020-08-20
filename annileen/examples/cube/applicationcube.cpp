#include "applicationcube.h"
#include "glm.hpp"
#include "scene.h"
#include "shaderpass.h"

ApplicationCube::ApplicationCube() {}
ApplicationCube::~ApplicationCube() {}

using namespace annileen;

annileen::Scene* ApplicationCube::init()
{
    Scene* scene = new Scene();
    getEngine()->setScene(scene);

    annileen::Shader* shader = getEngine()->getAssetManager()->loadShader("unlit.vs", "unlit.fs");
    
    std::shared_ptr<ShaderPass> shaderPass = std::make_shared<ShaderPass>();
    shaderPass->init(shader);

    std::shared_ptr<Material> material = std::make_shared<Material>();
    material->addShaderPass(shaderPass);

    bgfx::VertexLayout vlayout;
    vlayout .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();

    auto vdata = bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices));
    auto idata = bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList));

    Mesh* mesh = new Mesh();
    mesh->init(vdata, vlayout, idata, sizeof(s_cubeTriList) / sizeof(uint16_t));

    std::shared_ptr<Model> model = std::make_shared<Model>();
    model->init(mesh, material);

    SceneNode* node = scene->createNode();
    node->setModel(model);
    node->getTransform().translate(glm::vec3(0.0, 0.0, 2.0));
    node->getTransform().rotate(glm::vec3(0.0, 45.0, 0.0));

    // Initialize Camera
    m_Speed = 3.0f;
    m_MovementSpeed = 2.8f;
    m_Sensitivity = 18.0f;
    m_Pitch = 0.0f;
    m_Yaw = 0.0f;

    m_CameraActive = false;
    m_DebugActive = 0;

    return scene;
}

void ApplicationCube::update(float deltaTime)
{
    Camera* camera = getEngine()->getScene()->getCamera();
    m_MovementSpeed = getEngine()->getInput()->getKey(GLFW_KEY_LEFT_SHIFT) ? 5.0 * m_Speed : m_Speed;

    if (getEngine()->getInput()->getKey(GLFW_KEY_S))
    {
        camera->transform().translate(deltaTime * -m_MovementSpeed * camera->getForward());
    }
    if (getEngine()->getInput()->getKey(GLFW_KEY_W))
    {
        camera->transform().translate(deltaTime * m_MovementSpeed * camera->getForward());
    }
    if (getEngine()->getInput()->getKey(GLFW_KEY_A))
    {
        camera->transform().translate(deltaTime * m_MovementSpeed * camera->getRight());
    }
    if (getEngine()->getInput()->getKey(GLFW_KEY_D))
    {
        camera->transform().translate(deltaTime * -m_MovementSpeed * camera->getRight());
    }
    if (getEngine()->getInput()->getKey(GLFW_KEY_Q))
    {
        camera->transform().translate(deltaTime * -m_MovementSpeed * glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (getEngine()->getInput()->getKey(GLFW_KEY_E))
    {
        camera->transform().translate(deltaTime * m_MovementSpeed * glm::vec3(0.0f, 1.0f, 0.0f));
    }

    if (m_CameraActive)
    {
        // Camera mouse control
        auto mouseDelta = getEngine()->getInput()->getMouseDelta();

        m_Yaw += mouseDelta.x * m_Sensitivity * deltaTime;
        m_Pitch += -mouseDelta.y * m_Sensitivity * deltaTime;
        m_Pitch = glm::clamp(m_Pitch, -89.0f, 89.0f);
        glm::vec3 cameraForward{
        glm::cos(glm::radians(m_Pitch)) * glm::cos(glm::radians(m_Yaw)),
        glm::sin(glm::radians(m_Pitch)),
        glm::cos(glm::radians(m_Pitch)) * glm::sin(glm::radians(m_Yaw))
        };
        camera->setForward(glm::normalize(cameraForward));
    }


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

void ApplicationCube::finish()
{

}
