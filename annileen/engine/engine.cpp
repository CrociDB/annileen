#include <engine/engine.h>
#include <engine/renderer.h>
#include <engine/serviceprovider.h>
#include <engine/core/logger.h>
#include <engine/renderview.h>
#include <sstream>
#include <bx/math.h>
#include <glm.hpp>

namespace annileen
{
    // Initialize static variables
    bool Engine::m_Running = true;

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
            io.KeysDown[key] = true;
        if (action == GLFW_RELEASE)
            io.KeysDown[key] = false;

        // Modifiers are not reliable across systems
        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
#ifdef _WIN32
        io.KeySuper = false;
#else
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
#endif
    }

    void Engine::glfw_charCallback(GLFWwindow* window, unsigned int c)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.AddInputCharacter(c);
    }

    void Engine::glfw_mouseCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        getInstance()->getInput()->_setMousePosition(xpos, ypos);
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
        getInstance()->getInput()->_setMouseScroll(xoffset, yoffset);
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


    int Engine::init(int width, int height, std::string assetfile, std::string applicationName)
    {
        m_Width = width;
        m_Height = height;
        m_ApplicationName = applicationName;

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

        bgfx::renderFrame();

        bgfx::Init init;
        init.type = bgfx::RendererType::OpenGL;
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

        // Initialize services
        Logger* logger = new Logger();
        ServiceProvider::provideLogger(logger);

        AssetManager* assetManager = new AssetManager(assetfile);
        ServiceProvider::provideAssetManager(assetManager);

        Settings* settings = new Settings();
        ServiceProvider::provideSettings(settings);

        // TODO: Get parameter from settings.
        FontManager* fontManager = new FontManager(512);
        ServiceProvider::provideFontManager(fontManager);
        //

        m_Renderer = new Renderer();
        m_Renderer->init(this);


        m_TargetFPS = 60; 
        m_Time.timeScale = 1.0f;

        m_Running = true;
        m_Time.deltaTime = m_Time.unscaledDeltaTime = m_Time.time = 0;

        m_Gui = new Gui();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)

        // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;

        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;

        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

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

    Uniform* Engine::getUniform()
    {
        return &m_Uniform;
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
        return 1.0f / m_Time.deltaTime;
    }

    annileen::Time Engine::getTime()
    {
        return m_Time;
    }

    void Engine::setScene(Scene* scene)
    {
        m_CurrentScene = scene;
        m_Renderer->setScene(m_CurrentScene);
    }

    bool Engine::run()
    {
        double time = glfwGetTime();
        m_Time.unscaledDeltaTime = time - m_Time.time;
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

    void Engine::renderFrame()
    {
        if (m_CurrentScene != nullptr)
        {
            m_Renderer->setActiveCamera(m_CurrentScene->getCamera());            
            m_Renderer->render();            
        }

        bgfx::frame();
    }

    void Engine::releaseMem(void* _ptr, void* _userData)
    {
        delete[] _ptr;
    }

    Engine::Engine()
    {
        m_Input = std::make_shared<Input>();
    }

    Engine* Engine::getInstance()
    {
	    static Engine* instance = nullptr;

	    if (instance == nullptr)
	    {
		    instance = new Engine();
	    }

	    return instance;
    }

    Engine::~Engine()
    {
        Logger* logger = ServiceProvider::getLogger();
        ServiceProvider::provideLogger(nullptr);
        if (logger != nullptr)
        {
            delete logger;
        }

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

        FontManager* fontManager = ServiceProvider::getFontManager();
        ServiceProvider::provideFontManager(nullptr);
        if (fontManager != nullptr)
        {
            delete fontManager;
        }

        if (m_Gui != nullptr)
        {
            delete m_Gui;
        }

        m_Uniform.destroy();
        bgfx::shutdown();
        glfwTerminate();
    }
}
