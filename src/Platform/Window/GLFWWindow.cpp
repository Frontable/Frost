#include "GLFWWindow.h"
#include "Core/Logger.h"
#include "Core/Assert.h"
#include "Core/Events.h"
#include "Core/EventBus.h"

namespace Frost
{
    std::unique_ptr<IWindow> IWindow::Create(const WindowProps& props)
    {
        return std::make_unique<GLFWWindow>(props);
    }

    GLFWWindow::GLFWWindow(const WindowProps& props)
    {
        Init(props);
    }

    GLFWWindow::~GLFWWindow()
    {
        Shutdown();
    }

    void GLFWWindow::Init(const WindowProps& props)
    {
        m_Data.title = props.title;
        m_Data.width = props.width;
        m_Data.height = props.height;
        m_Data.vsync = props.vsync;

        if (!glfwInit())
        {
            FROST_ERROR("Failed to initialize GLFW");
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(
            m_Data.width, m_Data.height,
            m_Data.title.c_str(),
            nullptr, nullptr
        );

        FROST_ASSERT_ALWAYS(m_Window, "Failed to create GLFW window");

        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(m_Data.vsync);
        SetCallbacks();

        FROST_LOG("Window created: %s (%dx%d)",
            m_Data.title.c_str(), m_Data.width, m_Data.height);
    }

    void GLFWWindow::SetCallbacks()
    {
        // window resize
        glfwSetWindowSizeCallback(m_Window,
            [](GLFWwindow* w, int width, int height)
            {
                auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(w));
                data.width = width;
                data.height = height;
                glViewport(0, 0, width, height);
                EventBus::Emit(WindowResizedEvent{ width, height });
            }
        );

        // window close
        glfwSetWindowCloseCallback(m_Window,
            [](GLFWwindow* w)
            {
                EventBus::Emit(WindowClosedEvent{});
            }
        );

        // key events
        glfwSetKeyCallback(m_Window,
            [](GLFWwindow* w, int key, int scancode, int action, int mods)
            {
                if (key < 0) return;
                if (action == GLFW_PRESS || action == GLFW_REPEAT)
                    EventBus::Emit(KeyPressedEvent{ key });
                else if (action == GLFW_RELEASE)
                    EventBus::Emit(KeyReleasedEvent{ key });
            }
        );

        // mouse move
        glfwSetCursorPosCallback(m_Window,
            [](GLFWwindow* w, double x, double y)
            {
                EventBus::Emit(MouseMovedEvent{ (float)x, (float)y });
            }
        );

        // mouse click
        glfwSetMouseButtonCallback(m_Window,
            [](GLFWwindow* w, int button, int action, int mods)
            {
                if (action == GLFW_PRESS)
                {
                    double x, y;
                    glfwGetCursorPos(w, &x, &y);
                    EventBus::Emit(MouseClickedEvent{ button, (float)x, (float)y });
                }
            }
        );
    }

    void GLFWWindow::Update()
    {
        glfwPollEvents();
    }

    void GLFWWindow::SwapBuffers()
    {
        glfwSwapBuffers(m_Window);
    }

    bool GLFWWindow::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Window);
    }

    void GLFWWindow::SetVSync(bool enabled)
    {
        glfwSwapInterval(enabled ? 1 : 0);
        m_Data.vsync = enabled;
    }

    void GLFWWindow::Shutdown()
    {
        if (m_Window)
        {
            FROST_LOG("Window destroyed: %s", m_Data.title.c_str());
            glfwDestroyWindow(m_Window);
            glfwTerminate();
            m_Window = nullptr;
        }
    }
}