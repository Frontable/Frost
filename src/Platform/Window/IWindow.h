#pragma once
#include <string>
#include <memory>

namespace Frost
{
    struct WindowProps
    {
        std::string title = "FrostEngine";
        int         width = 1280;
        int         height = 720;
        bool        vsync = true;
    };

    class IWindow
    {
    public:
        virtual ~IWindow() = default;

        virtual void Update() = 0;
        virtual void SwapBuffers() = 0;
        virtual bool ShouldClose()         const = 0;

        virtual int         Width()        const = 0;
        virtual int         Height()       const = 0;
        virtual std::string Title()        const = 0;
        virtual void* NativeHandle() const = 0;

        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync()             const = 0;

        static std::unique_ptr<IWindow> Create(const WindowProps& props = WindowProps{});
    };
}