#include "engine.h"
//#include "renderer.h"

#include <sstream>

static void glfw_errorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

static void glfw_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    
}

uint32_t Engine::init(uint16_t width, uint16_t height)
{
    m_Width = width;
    m_Height = height;

    glfwSetErrorCallback(glfw_errorCallback);
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_Window = glfwCreateWindow(m_Width, m_Height, "Annileen", nullptr, nullptr);
    if (!m_Window)
        return 1;

    glfwSetKeyCallback(m_Window, glfw_keyCallback);

    bgfx::renderFrame();

    bgfx::Init init;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    init.platformData.ndt = glfwGetX11Display();
    init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(m_Window);
#elif BX_PLATFORM_OSX
    init.platformData.nwh = glfwGetCocoaWindow(m_Window);
#elif BX_PLATFORM_WINDOWS
    init.platformData.nwh = glfwGetWin32Window(m_Window);
#endif

    init.resolution.width = (uint32_t)m_Width;
    init.resolution.height = (uint32_t)m_Height;
    init.resolution.reset = BGFX_RESET_VSYNC;
    if (!bgfx::init(init))
        return 1;

	//m_Input = new Input();

    /*m_Renderer = new Renderer();
    m_Renderer->init(this);*/

    m_TargetFPS = 60; 
    m_Time.timeScale = 1.0f;

    m_Running = true;

    return 0;
}

//Input* Engine::getInput()
//{
//	return m_Input;
//}

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
    return 1.0f / m_Time.deltaTime;
}

Time Engine::getTime()
{
    return m_Time;
}


//void Engine::setScene(Scene* scene)
//{
//    m_CurrentScene = scene;
//}

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
        ss << "Annileen - Framerate: " << getFPS() << " FPS";
        setWindowTitle(ss.str());
    }

    m_Running = m_Running || !glfwWindowShouldClose(m_Window);
    return m_Running;
}

void Engine::terminate()
{
    m_Running = false;
}

void Engine::checkInputEvents()
{
    glfwPollEvents();
    uint16_t oldWidth = m_Width;
    uint16_t oldHeight = m_Height;
    glfwGetWindowSize(m_Window, (int*)&m_Width, (int*)&m_Height);

    if (m_Width != oldWidth || m_Height != oldHeight)
    {
        bgfx::reset((uint32_t)m_Width, (uint32_t)m_Height, BGFX_RESET_VSYNC);
        // Set view rect again
    }

    /*m_Input->_flushEvents();
    while (SDL_PollEvent(&m_WindowEvent))
    {
        if (m_WindowEvent.type == SDL_QUIT)
        {
            m_Running = false;
            break;
        }
		else if (m_WindowEvent.type == SDL_KEYDOWN)
		{
			m_Input->_setKeyDown(m_WindowEvent.key.keysym.sym, true);
		}
		else if (m_WindowEvent.type == SDL_KEYUP)
		{
			m_Input->_setKeyDown(m_WindowEvent.key.keysym.sym, false);
		}
        else if (m_WindowEvent.type == SDL_MOUSEMOTION)
        {
            m_Input->_setMousePosition(m_WindowEvent.motion.x, m_WindowEvent.motion.y);
            m_Input->_setMouseDelta(m_WindowEvent.motion.xrel, m_WindowEvent.motion.yrel);
        }
        else if (m_WindowEvent.type == SDL_MOUSEBUTTONDOWN)
        {
            m_Input->_setMouseButton(m_WindowEvent.button.button, true);
        }
        else if (m_WindowEvent.type == SDL_MOUSEBUTTONUP)
        {
            m_Input->_setMouseButton(m_WindowEvent.button.button, false);
        }
    }*/
}

void Engine::renderFrame()
{
    //auto camera = m_CurrentScene->getCamera();

    //m_Renderer->clear(camera->clearColor);

    //// Render objects seen in scene
    //if (m_CurrentScene != NULL)
    //{
    //    m_Renderer->initFrame(m_CurrentScene);
    //    for (auto& mesh : m_CurrentScene->getMeshList())
    //    {
    //        m_Renderer->renderMesh(m_CurrentScene, mesh);
    //    }
    //}

    //if (camera->clearType == CameraClearSkybox)
    //{
    //    m_Renderer->renderSkybox(camera, m_CurrentScene->getSkybox());
    //}

    //m_Renderer->swapBuffer();
}

Engine::Engine()
{
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
    bgfx::shutdown();
    glfwTerminate();
}
