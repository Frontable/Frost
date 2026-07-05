#pragma once
#include <glad/glad.h>
#include <array>
#include "Shader.h"
#include "Texture.h"
#include "Core/FrostMath.h"

namespace Frost
{
    struct Vertex
    {
        vec3 position;
        vec2 uv;
        vec4 color;
    };

    struct SpriteInfo
    {
        vec2 uv;   // top-left pixel in atlas
        vec2 size; // pixel size
    };

    class BatchRenderer
    {
    public:
        static constexpr int MAX_SPRITES = 1000;
        static constexpr int VERTS_PER_QUAD = 4;
        static constexpr int INDICES_PER_QUAD = 6;
        static constexpr int MAX_VERTICES = MAX_SPRITES * VERTS_PER_QUAD;
        static constexpr int MAX_INDICES = MAX_SPRITES * INDICES_PER_QUAD;

        BatchRenderer();
        ~BatchRenderer();

        BatchRenderer(const BatchRenderer&) = delete;
        BatchRenderer& operator=(const BatchRenderer&) = delete;

        
        void Begin(Shader* shader, const mat4& viewProjection);

        void DrawSprite(const vec3& position,
            const vec2& size,
            float             rotation,
            const SpriteInfo& sprite,
            const Texture* texture,
            float             atlasW,
            float             atlasH,
            const vec4& color = { 1,1,1,1 });

        
        void End();

        int GetDrawCalls() const { return m_DrawCalls; }

    private:
        void Init();
        void Flush();
        void UploadVertices();

        Shader* m_Shader = nullptr;
        unsigned int m_CurrentTexID = 0;
        mat4         m_ViewProj;

        unsigned int m_VAO = 0;
        unsigned int m_VBO = 0;
        unsigned int m_EBO = 0;

        std::array<Vertex, MAX_VERTICES> m_Vertices{};
        int m_VertexCount = 0;
        int m_IndicesCount = 0;
        int m_DrawCalls = 0;
    };
}