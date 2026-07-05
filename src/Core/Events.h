#pragma once
#include <string>
#include "FrostMath.h"

namespace Frost
{
    // 
    // Input
    // 
    struct KeyPressedEvent { int key; };
    struct KeyReleasedEvent { int key; };
    struct MouseMovedEvent { float x, y; };
    struct MouseClickedEvent { int button; float x, y; };

    // 
    // Window
    //
    struct WindowResizedEvent { int width, height; };
    struct WindowClosedEvent {};

    //
    // Scene
    // 
    struct SceneLoadedEvent { std::string name; };
    struct SceneUnloadedEvent { std::string name; };

    //
    // ECS
    // 
    struct EntityCreatedEvent { uint32_t entity; };
    struct EntityDestroyedEvent { uint32_t entity; };

} // namespace Frost