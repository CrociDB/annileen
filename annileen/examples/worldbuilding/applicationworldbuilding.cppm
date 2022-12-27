#include "glm.hpp"

import applicationworldbuilding;

import gamescene;

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
}

void ApplicationWorldBuilding::finish()
{

}
