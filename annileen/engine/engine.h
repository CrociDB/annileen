#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <iostream>

#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <GLFW/glfw3.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>

#include <glm.hpp>

#include "scene.h"
//#include "input.h"

namespace annileen
{
    class Renderer;

    struct Time
    {
        float deltaTime;
        float unscaledDeltaTime;
        double time;
        float timeScale;
    };

    class Engine
    {
    private:
        GLFWwindow* m_Window;
        uint16_t m_Width, m_Height;

        //Input* m_Input;
        Renderer* m_Renderer;
        static bool m_Running;
        
        Time m_Time;
        uint8_t m_TargetFPS;

        Scene* m_CurrentScene = nullptr;

        Engine();

        // GLFW Callbacks
        static void glfw_errorCallback(int error, const char* description);    
        static void glfw_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);    
        static void glfw_mouseCursorPositionCallback(GLFWwindow* window, double xpos, double ypos);    
        static void glfw_mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void glfw_mouseCursorEnterCallback(GLFWwindow* window, int entered);
        static void glfw_mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        static void glfw_joystickCallback(int jid, int event);

    public:
        uint32_t init(uint16_t width, uint16_t height);

        //Input* getInput();
        Renderer* getRenderer();
        GLFWwindow* getGLFWWindow();
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

    inline uint32_t convert_color_vec3_uint32(glm::vec3 col) 
    {
        return ((uint32_t)(col.x * 0xFF) << 24) + ((uint32_t)(col.y * 0xFF) << 16) + ((uint32_t)(col.z * 0xFF) << 8) + 0xFF;
    }
}

#endif
