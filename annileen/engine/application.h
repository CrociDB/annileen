#pragma once

#include "engine.h"

class Application
{

private:
	annileen::Engine* m_Engine;

	void initAnnileen();

	virtual annileen::Scene* init() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void finish() = 0;
	// TODO:
	// physicsUpdate
	// beforeRender
	// afterRender

protected:
	void destroy();

	inline annileen::Engine* getEngine()
	{
		return m_Engine;
	}

public:
	int run();
	Application();
	~Application();
};