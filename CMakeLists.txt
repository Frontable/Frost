cmake_minimum_required(VERSION 3.20)
project(Frost LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(vendor)

file(GLOB_RECURSE FROST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

add_library(Frost STATIC ${FROST_SOURCES} "src/Rendering/DebugRenderer.h" "src/Rendering/DebugRenderer.cpp")

target_include_directories(Frost
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/freetype/include>
)

if(WIN32)
    target_link_libraries(Frost
        PUBLIC
            glad
            glfw
            freetype
            opengl32
    )
elseif(UNIX)
    target_link_libraries(Frost
        PUBLIC
            glad
            glfw
            freetype
            GL
            dl
            pthread
            X11
    )
endif()

target_compile_definitions(Frost
    PUBLIC
        $<$<CONFIG:Debug>:FROST_DEBUG>
        $<$<CONFIG:Release>:FROST_RELEASE>
)

if(MSVC)
    target_compile_options(Frost PRIVATE /W4)
else()
    target_compile_options(Frost PRIVATE
        -Wall
        -Wextra
        -Wno-unused-parameter
    )
endif()


#[[
cmake_minimum_required(VERSION 3.20)
project(Frost LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 

add_subdirectory(vendor)

file(GLOB_RECURSE FROST_SOURCES
    "FrostEngine/*.cpp"
    "FrostEngine/*.h"
)

add_library(Frost STATIC ${FROST_SOURCES} "src/ECS/Entity.h" "src/ECS/SparseSet.h" 
"src/ECS/ComponentPool.h" "src/ECS/View.h" "src/ECS/Registry.h" "src/ECS/ScriptBase.h" 
"src/ECS/ScriptComponent.h" "src/ECS/ScriptSystem.h"  "src/Rendering/Shader.h" 
"src/Rendering/ShaderLoader.h" "src/Rendering/ShaderLoader.cpp" "src/Rendering/Texture.h" 
"src/Rendering/TextureLoader.h" "src/Rendering/TextureLoader.cpp" "src/Rendering/BatchRenderer.h" 
"src/Rendering/BatchRenderer.cpp" "src/Rendering/Camera2D.h" "src/Rendering/Camera2D.cpp" "src/Assets/AssetHandle.h" 
"src/Assets/AssetManager.h" "src/Assets/AssetRegistry.h" "src/Scene/Scene.h" "src/Scene/SceneManager.h" "src/Core/Application.h" 
"src/Core/Application.cpp" "src/Core/EntryPoint.h" "Frost.h")

target_include_directories(Frost
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(Frost
        glad
        glfw
        freetype
        stb_image
        opengl32
)

target_compile_definitions(Frost
    PUBLIC
        $<$<CONFIG:Debug>:FROST_DEBUG>
        $<$<CONFIG:Release>:FROST_RELEASE>
)
#]]


#[[
target_include_directories(Frost
    PUBLIC
        # src/ is the root — so #include "Core/Logger.h" works
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/freetype/include>
)

target_link_libraries(Frost
        glad
        glfw
        freetype
        opengl32
)

target_compile_definitions(Frost
    PUBLIC
        $<$<CONFIG:Debug>:FROST_DEBUG>
        $<$<CONFIG:Release>:FROST_RELEASE>
)

if(MSVC)
    target_compile_options(Frost PRIVATE /W3)
endif()

target_compile_definitions(Frost
    PUBLIC
        $<$<CONFIG:Debug>:FROST_DEBUG>
        $<$<CONFIG:Release>:FROST_RELEASE>
)

if(MSVC)
    target_compile_options(Frost PRIVATE /W3)
endif()

]]#
