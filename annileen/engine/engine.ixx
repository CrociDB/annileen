module;

#include <engine/core/glfw_imgui_utils.h>
#include <string>
#include <glm.hpp>

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

#include <memory>
#include "engine/forward_decl.h"

export module engine;

export namespace annileen
{
    struct Time
    {
        float deltaTime;
        float unscaledDeltaTime;
        double time;
        float timeScale;
    };

    class Engine final
    {
    private:
        static Engine* s_Instance;

        GLFWwindow* m_Window;
        int m_Width, m_Height;
        std::string m_ApplicationName;

        std::shared_ptr<Input> m_Input;

        Renderer* m_Renderer;
        static bool m_Running;

        Gui* m_Gui;

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
        static void glfw_charCallback(GLFWwindow* window, unsigned int c);

    public:
        int init(int width, int height, const std::string& assetfile, const std::string& settingsfile, std::string applicationName);

        std::shared_ptr<Input> getInput();
        Gui* getGui();
        Renderer* getRenderer();
        Uniform* getUniform();
        GLFWwindow* getGLFWWindow();

        uint16_t getWidth() const;
        uint16_t getHeight() const;

        void setWindowTitle(std::string title);
        void setFPSLock(uint8_t fps);

        int getFPS() const;
        Time getTime();

        void setScene(Scene* scene);
        Scene* getScene() { return m_CurrentScene; }

        bool run();
        void terminate();

        void setMouseCapture(bool value);
        void checkInputEvents();
        void render(Camera* replacementCamera);

        static Engine* getInstance();
        static void destroy();
        ~Engine();
    };

}
