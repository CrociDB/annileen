
#include <engine/core/glfw_imgui_utils.h>
#include <sstream>
#include <iostream>

#include <bx/bx.h>
#include <bx/allocator.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <GLFW/glfw3.h>
// GLFW will include xserver headers in Linux and it defines macro `None`. So including
// this math header before fixes the redefinition of this macro.
#include <bx/math.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>

#include <glm.hpp>
#include "engine/core/logger.h"

import engine;
import input;
import font;
import assetmanager;
import serviceprovider;
import renderer;
import uniform; 
import gui;

namespace annileen
{
    // Initialize static variables
    bool Engine::m_Running = true;
    Engine* Engine::s_Instance = nullptr;

    void Engine::glfw_errorCallback(int error, const char* description)
    {
        fprintf(stderr, "GLFW error %d: %s\n", error, description);
    }

    void Engine::glfw_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS)
        {
            getInstance()->getInput()->_setKeyDown(key, true);
        }
        else if (action == GLFW_RELEASE)
        {
            getInstance()->getInput()->_setKeyDown(key, false);
        }

        ImGuiIO& io = ImGui::GetIO();
        if (action == GLFW_PRESS)
            io.AddKeyEvent(ANNI_GLFW_KEY_TO_IMGUI(key), true);
        if (action == GLFW_RELEASE)
            io.AddKeyEvent(ANNI_GLFW_KEY_TO_IMGUI(key), false);

        // Modifiers are not reliable across systems
        io.KeyCtrl =    ImGui::IsKeyDown(ANNI_GLFW_KEY_TO_IMGUI(GLFW_KEY_LEFT_CONTROL)) || 
                        ImGui::IsKeyDown(ANNI_GLFW_KEY_TO_IMGUI(GLFW_KEY_RIGHT_CONTROL));
        io.KeyShift =   ImGui::IsKeyDown(ANNI_GLFW_KEY_TO_IMGUI(GLFW_KEY_LEFT_SHIFT)) ||
                        ImGui::IsKeyDown(ANNI_GLFW_KEY_TO_IMGUI(GLFW_KEY_RIGHT_SHIFT));
        io.KeyAlt =     ImGui::IsKeyDown(ANNI_GLFW_KEY_TO_IMGUI(GLFW_KEY_LEFT_ALT)) ||
                        ImGui::IsKeyDown(ANNI_GLFW_KEY_TO_IMGUI(GLFW_KEY_RIGHT_ALT));

#ifdef _WIN32
        io.KeySuper = false;
#else
        io.KeySuper =   ImGui::IsKeyDown(ANNI_GLFW_KEY_TO_IMGUI(GLFW_KEY_LEFT_SUPER)) ||
                        ImGui::IsKeyDown(ANNI_GLFW_KEY_TO_IMGUI(GLFW_KEY_RIGHT_SUPER));
#endif
    }

    void Engine::glfw_charCallback(GLFWwindow* window, unsigned int c)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.AddInputCharacter(c);
    }

    void Engine::glfw_mouseCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        getInstance()->getInput()->_setMousePosition(static_cast<float>(xpos), static_cast<float>(ypos));
    }

    void Engine::glfw_mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        if (action == GLFW_PRESS)
        {
            getInstance()->getInput()->_setMouseButton(button, true);
        } 
        else if (action == GLFW_RELEASE)
        {
            getInstance()->getInput()->_setMouseButton(button, false);
        }
    }

    void Engine::glfw_mouseCursorEnterCallback(GLFWwindow* window, int entered)
    {
        if (entered)
        {
            // The cursor entered the content area of the window
        }
        else
        {
            // The cursor left the content area of the window
        }
    }

    void Engine::glfw_mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        getInstance()->getInput()->_setMouseScroll(static_cast<float>(xoffset), static_cast<float>(yoffset));
    }

    void Engine::glfw_joystickCallback(int jid, int event)
    {
        if (event == GLFW_CONNECTED)
        {
            // The joystick was connected
        }
        else if (event == GLFW_DISCONNECTED)
        {
            // The joystick was disconnected
        }
    }


    int Engine::init(int width, int height, const std::string& assetfile, const std::string& settingsfile, std::string applicationName)
    {
        m_Width = width;
        m_Height = height;
        m_ApplicationName = applicationName;

        // Initialize services
        Logger::initialize();


        AssetManager* assetManager = new AssetManager(assetfile);
        ServiceProvider::provideAssetManager(assetManager);

        Settings* settings = new Settings();
        settings->loadSettings(settingsfile);
        ServiceProvider::provideSettings(settings);
        
        // Initialize GLFW
        glfwSetErrorCallback(&Engine::glfw_errorCallback);
        if (!glfwInit())
            return 1;
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_Window = glfwCreateWindow(m_Width, m_Height, "Annileen", nullptr, nullptr);
        if (!m_Window)
            return 1;

        glfwSetKeyCallback(m_Window, glfw_keyCallback);
        glfwSetMouseButtonCallback(m_Window, glfw_mouseButtonCallback);
        glfwSetCursorPosCallback(m_Window, glfw_mouseCursorPositionCallback);
        glfwSetScrollCallback(m_Window, glfw_mouseScrollCallback);
        glfwSetCharCallback(m_Window, glfw_charCallback);
        //glfwSetCursorEnterCallback(m_Window, glfw_mouseCursorEnterCallback);
        //glfwSetJoystickCallback(glwf_joystickCallback);

        // Initialize BGFX
        bgfx::renderFrame();

        bgfx::Init init;
        init.type = settings->getBGFXRendererType();
    #if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
        init.platformData.ndt = glfwGetX11Display();
        init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(m_Window);
    #elif BX_PLATFORM_OSX
        init.platformData.nwh = glfwGetCocoaWindow(m_Window);
    #elif BX_PLATFORM_WINDOWS
        init.platformData.nwh = glfwGetWin32Window(m_Window);
    #endif

        init.resolution.width = m_Width;
        init.resolution.height = m_Height;
        init.resolution.reset = BGFX_RESET_VSYNC;

        if (!bgfx::init(init))
            return 1;

        Font::initializeFontManager();

        m_Renderer = new Renderer();
        m_Renderer->init(this);

        m_TargetFPS = 60; 
        m_Time.timeScale = 1.0f;

        m_Running = true;
        m_Time.deltaTime = m_Time.unscaledDeltaTime = 0.0f;
        m_Time.time = 0;

        m_Gui = new Gui();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)

        return 0;
    }

    std::shared_ptr<Input> Engine::getInput()
    {
	    return m_Input;
    }

    Gui* Engine::getGui()
    {
        return m_Gui;
    }

    Renderer* Engine::getRenderer()
    {
        return m_Renderer;
    }

    GLFWwindow* Engine::getGLFWWindow()
    {
        return m_Window;
    }

    uint16_t Engine::getWidth() const
    {
        return m_Width;
    }

    uint16_t Engine::getHeight() const
    {
        return m_Height;
    }


    void Engine::setWindowTitle(std::string title)
    {
        glfwSetWindowTitle(m_Window, title.c_str());
    }

    void Engine::setFPSLock(uint8_t fps)
    {
        m_TargetFPS = fps;
    }

    int Engine::getFPS() const
    {
        return static_cast<int> (1.0f / m_Time.deltaTime);
    }

    annileen::Time Engine::getTime()
    {
        return m_Time;
    }

    void Engine::setScene(Scene* scene)
    {
        m_CurrentScene = scene;
    }

    bool Engine::run()
    {
        double time = glfwGetTime();
        m_Time.unscaledDeltaTime = static_cast<float>(time - m_Time.time);
        m_Time.deltaTime = m_Time.unscaledDeltaTime * m_Time.timeScale;
        m_Time.time = time;

        static float fpsCount = 0.0;
        fpsCount += m_Time.deltaTime;
        if (fpsCount >= 0.3f)
        {
            fpsCount = 0;
            std::stringstream ss;
            ss << "Annileen - " << m_ApplicationName << " - Framerate: " << getFPS() << " FPS";
            setWindowTitle(ss.str());
        }

        m_Running = m_Running && !glfwWindowShouldClose(m_Window);
        return m_Running;
    }

    void Engine::terminate()
    {
        m_Running = false;
    }

    void Engine::setMouseCapture(bool value)
    {
        if (value)
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void Engine::checkInputEvents()
    {
        m_Input->flushEvents();
        glfwPollEvents();
        int oldWidth = m_Width;
        int oldHeight = m_Height;
        glfwGetWindowSize(m_Window, &m_Width, &m_Height);

        if (m_Width != oldWidth || m_Height != oldHeight)
        {
            bgfx::reset(m_Width, m_Height, BGFX_RESET_VSYNC);            
        }
    }

    void Engine::render(Camera* replacementCamera)
    {
        if (m_CurrentScene != nullptr)
        {
            m_Renderer->render(m_CurrentScene, replacementCamera);            
        }

        bgfx::frame();
    }

    Engine::Engine()
    {
        m_Input = std::make_shared<Input>();
    }

    Engine* Engine::getInstance()
    {
        if (s_Instance == nullptr)
	    {
            s_Instance = new Engine();
	    }

	    return s_Instance;
    }

    void Engine::destroy()
    {
        delete s_Instance;
    }

    Engine::~Engine()
    {
        Logger::destroy();

        AssetManager* assetManager = ServiceProvider::getAssetManager();
        ServiceProvider::provideAssetManager(nullptr);
        if (assetManager != nullptr)
        {
            delete assetManager;
        }

        Settings* settings = ServiceProvider::getSettings();
        ServiceProvider::provideSettings(nullptr);
        if (settings != nullptr)
        {
            delete settings;
        }

        Font::destroyFontManager();

        if (m_Gui != nullptr)
        {
            delete m_Gui;
        }

        SceneManager* sceneManager = ServiceProvider::getSceneManager();
        if (sceneManager != nullptr)
        {
            delete sceneManager;
        }

        Uniform::destroy();
        bgfx::shutdown();
        glfwTerminate();
    }
}
