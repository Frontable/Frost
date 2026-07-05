#pragma once
#include <array>
#include <GLFW/glfw3.h>
#include "Core/EventBus.h"
#include "Core/Events.h"
#include "Core/FrostMath.h"

namespace Frost
{
    class Input
    {
    public:
        static void Init()
        {
            
            EventBus::On<KeyPressedEvent>([](const KeyPressedEvent& e)
                {
                    if (e.key < 0 || e.key > GLFW_KEY_LAST) return;
                    s_Keys[e.key] = true;
                    s_KeysJustPressed[e.key] = true;
                });

            EventBus::On<KeyReleasedEvent>([](const KeyReleasedEvent& e)
                {
                    if (e.key < 0 || e.key > GLFW_KEY_LAST) return;
                    s_Keys[e.key] = false;
                    s_KeysJustReleased[e.key] = true;
                });

            EventBus::On<MouseMovedEvent>([](const MouseMovedEvent& e)
                {
                    s_MouseX = e.x;
                    s_MouseY = e.y;
                });
        }

        // call once per frame BEFORE polling events
        static void Update()
        {
            s_KeysJustPressed.fill(false);
            s_KeysJustReleased.fill(false);
        }

        static bool IsKeyPressed(int key)
        {
            if (key < 0 || key > GLFW_KEY_LAST) return false;
            return s_Keys[key];
        }

        static bool IsKeyJustPressed(int key)
        {
            if (key < 0 || key > GLFW_KEY_LAST) return false;
            return s_KeysJustPressed[key];
        }

        static bool IsKeyJustReleased(int key)
        {
            if (key < 0 || key > GLFW_KEY_LAST) return false;
            return s_KeysJustReleased[key];
        }

        static float GetMouseX() { return s_MouseX; }
        static float GetMouseY() { return s_MouseY; }
        static vec2  GetMousePos() { return { s_MouseX, s_MouseY }; }

    private:
        static inline std::array<bool, GLFW_KEY_LAST + 1> s_Keys{};
        static inline std::array<bool, GLFW_KEY_LAST + 1> s_KeysJustPressed{};
        static inline std::array<bool, GLFW_KEY_LAST + 1> s_KeysJustReleased{};
        static inline float s_MouseX = 0.0f;
        static inline float s_MouseY = 0.0f;
    };
}