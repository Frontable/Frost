#pragma once
#include "IWindow.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"

namespace Frost
{
    class GLFWWindow : public IWindow
    {
    public:
        GLFWWindow(const WindowProps& props);
        ~GLFWWindow() override;

        void Update()      override;
        void SwapBuffers() override;
        bool ShouldClose() const override;

        int         Width()        const override { return m_Data.width; }
        int         Height()       const override { return m_Data.height; }
        std::string Title()        const override { return m_Data.title; }
        void* NativeHandle() const override { return m_Window; }

        void SetVSync(bool enabled) override;
        bool IsVSync()        const override { return m_Data.vsync; }

    private:
        void Init(const WindowProps& props);
        void Shutdown();
        void SetCallbacks();

        GLFWwindow* m_Window = nullptr;

        struct WindowData
        {
            std::string title;
            int         width = 0;
            int         height = 0;
            bool        vsync = true;
        };

        WindowData m_Data;
    };
}