#include "applicationworldbuilding.h"
#include "gamescene.h"
#include "glm.hpp"

ApplicationWorldBuilding::ApplicationWorldBuilding() {}
ApplicationWorldBuilding::~ApplicationWorldBuilding() {}

annileen::Scene* ApplicationWorldBuilding::init()
{
	GameScene* scene = new GameScene();

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

void ApplicationWorldBuilding::update(float deltaTime)
{
    ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(350, 50), ImGuiCond_FirstUseEver);
    ImGui::Begin("Light", NULL, 0);

    static float lightDir[3] = { 
        getEngine()->getScene()->getLightList().front()->getTransform().euler().x, 
        getEngine()->getScene()->getLightList().front()->getTransform().euler().y,
        getEngine()->getScene()->getLightList().front()->getTransform().euler().z};

    ImGui::DragFloat3("Pos", lightDir, .5);

    ImGui::End();

    getEngine()->getScene()->getLightList().front()->getTransform().euler((glm::vec3(lightDir[0], lightDir[1], lightDir[2])));

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

void ApplicationWorldBuilding::finish()
{

}
