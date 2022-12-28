module;

#include "glm.hpp"
#include <memory>

#define ANNILEEN_APPLICATION
#include <engine/definitions.h>

export module applicationworldbuilding;

import gamescene;

export ANNILEEN_APP_CLASS_DECLARATION(ApplicationWorldBuilding)
{
private:
	float m_Speed;
	float m_MovementSpeed;
	float m_Sensitivity;
	float m_Pitch, m_Yaw;
	bool m_CameraActive = false;
	uint8_t m_DebugActive = 0;

	annileen::Scene* init() override;
	void update(float deltaTime) override {}
	void finish() override {}
	
public:
	ApplicationWorldBuilding() = default;
	~ApplicationWorldBuilding() = default;
};

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
