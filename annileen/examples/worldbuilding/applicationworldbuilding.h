#pragma once

#include "application.h"
#include "definitions.h"

class ApplicationWorldBuilding : public Application
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
