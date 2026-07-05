#pragma once
#include <glad/glad.h>
#include <vector>
#include <memory>
#include "Shader.h"
#include "ShaderLoader.h"
#include "Core/FrostMath.h"
#include "Core/Logger.h"

namespace Frost
{
    class DebugRenderer
    {
    public:
        DebugRenderer() = default;
        ~DebugRenderer();

        bool Init(const std::string& vertPath, const std::string& fragPath);

        void DrawCircle(vec2 center, float radius,
            vec4 color = { 0.0f, 1.0f, 0.0f, 1.0f });

        void Flush(const mat4& viewProjection);

        bool IsInitialized() const { return m_Initialized; }

    private:
        struct CircleData
        {
            vec2  center;
            float radius;
            vec4  color;
        };

        std::vector<CircleData>  m_Circles;
        std::shared_ptr<Shader>  m_Shader;

        unsigned int m_VAO = 0;
        unsigned int m_VBO = 0;
        bool         m_Initialized = false;

        static constexpr int SEGMENTS = 32;
    };
}