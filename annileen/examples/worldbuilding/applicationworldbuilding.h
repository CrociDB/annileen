#pragma once

#define ANNILEEN_APPLICATION
#include <engine/definitions.h>

ANNILEEN_APP_CLASS_DECLARATION(ApplicationWorldBuilding)
{

private:
	float m_Speed;
	float m_MovementSpeed;
	float m_Sensitivity;
	float m_Pitch, m_Yaw;
	bool m_CameraActive = false;
	uint8_t m_DebugActive = 0;

	annileen::Scene* init() override;
	void update(float deltaTime) override;
	void finish() override;
	
public:
	ApplicationWorldBuilding();
	~ApplicationWorldBuilding();
};

ANNILEEN_APP_MAIN(ApplicationWorldBuilding)
