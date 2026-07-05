#pragma once
#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <unordered_map>
#include <string>
#include <memory>
#include "Shader.h"
#include "ShaderLoader.h"
#include "Core/FrostMath.h"
#include "Core/Logger.h"

namespace Frost
{
    struct Glyph
    {
        vec2  uvMin;
        vec2  uvMax;
        vec2  size;
        vec2  bearing;
        float advance;
    };

    class TextRenderer
    {
    public:
        TextRenderer() = default;
        ~TextRenderer();

        bool Init(const std::string& fontPath, const std::string& vertShaderPath, const std::string& fragShaderPath, unsigned int fontSize = 32);

        void RenderText(const std::string& text,
            float              x,
            float              y,
            float              scale,
            const vec4& color,
            const mat4& projection);

        bool IsInitialized() const { return m_Initialized; }

    private:
        bool LoadFont(const std::string& fontPath, unsigned int fontSize);

        unsigned int m_AtlasID = 0;
        int          m_AtlasWidth = 0;
        int          m_AtlasHeight = 0;
        unsigned int m_FontSize = 32;

        unsigned int m_VAO = 0;
        unsigned int m_VBO = 0;

        std::shared_ptr<Shader>          m_Shader;
        std::unordered_map<char, Glyph>  m_Glyphs;
        bool                             m_Initialized = false;
    };
}