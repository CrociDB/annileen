#pragma once

#include "application.h"
#include "definitions.h"

// Example cube
struct PosColorVertex
{
    float pos[3];
    uint8_t col;
};
static PosColorVertex s_cubeVertices[] =
{
    {-1.0f,  1.0f,  1.0f, (uint8_t)0xff000000 },
    { 1.0f,  1.0f,  1.0f, (uint8_t)0xff0000ff },
    {-1.0f, -1.0f,  1.0f, (uint8_t)0xff00ff00 },
    { 1.0f, -1.0f,  1.0f, (uint8_t)0xff00ffff },
    {-1.0f,  1.0f, -1.0f, (uint8_t)0xffff0000 },
    { 1.0f,  1.0f, -1.0f, (uint8_t)0xffff00ff },
    {-1.0f, -1.0f, -1.0f, (uint8_t)0xffffff00 },
    { 1.0f, -1.0f, -1.0f, (uint8_t)0xffffffff },
};

static const uint16_t s_cubeTriList[] =
{
    0, 1, 2, // 0
    1, 3, 2,
    4, 6, 5, // 2
    5, 6, 7,
    0, 2, 4, // 4
    4, 2, 6,
    1, 5, 3, // 6
    5, 7, 3,
    0, 4, 1, // 8
    4, 5, 1,
    2, 3, 6, // 10
    6, 3, 7,
};

class ApplicationCube : public Application
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
	ApplicationCube();
	~ApplicationCube();
};

ANNILEEN_APP_MAIN(ApplicationCube)