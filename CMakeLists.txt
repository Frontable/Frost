

project(Engine)

add_library(Engine 
    Application.cpp
    "Platform/Window.cpp"
    Rendering/Essentials/Shader.cpp
    Rendering/Essentials/ShaderLoader.cpp
    Rendering/Essentials/Texture.cpp
    Rendering/Essentials/TextureLoader.cpp
    "Rendering/Core/SpriteBatchRenderer.cpp"
    Rendering/Core/Camera2D.cpp
    Logger/Logger.cpp
    
    
    
    
    "Platform/Input.cpp"



 "Rendering/Essentials/FontLoader.h" "Rendering/Essentials/FontLoader.cpp" "Rendering/Core/TextRenderer.h" "Rendering/Core/TextRenderer.cpp" "Rendering/Core/DebugRenderer.h" "Rendering/Core/DebugRenderer.cpp")

target_include_directories(Engine PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}
)


target_link_libraries(Engine PUBLIC sol2 lua glfw glad stb_image freetype )

