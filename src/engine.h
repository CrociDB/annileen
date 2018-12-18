#ifndef _ENGINE_H_
#define _ENGINE_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <glm.hpp>

#include "scene.h"
#include "input.h"

class Renderer;

struct Time
{
    float deltaTime;
    float unscaledDeltaTime;
    float time;
    float timeScale;
};

class Engine
{
private:
    SDL_Window* m_Window;
    uint16_t m_Width, m_Height;
    SDL_Event m_WindowEvent;

	Input* m_Input;
    Renderer* m_Renderer;
    bool m_Running;
    
    Time m_Time;
    uint8_t m_TargetFPS;

    Scene* m_CurrentScene = NULL;

    Engine();
public:
    void init(uint16_t width, uint16_t height);

	Input* getInput();
	Renderer* getRenderer();
    SDL_Window* getSDLWindow();
    uint16_t getWidth() const;
    uint16_t getHeight() const;

    void setWindowTitle(std::string title);
    void setFPSLock(uint8_t fps);

    int getFPS() const;
    Time getTime();

    void setScene(Scene* scene);

    bool run();
    void terminate();

    void checkInputEvents();
    void renderFrame();

	static Engine* getInstance();
    ~Engine();
};

#endif
