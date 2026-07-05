#include "TextRenderer.h"
#include <iostream>
#include <vector>

namespace Frost
{
    TextRenderer::~TextRenderer()
    {
        if (m_AtlasID) glDeleteTextures(1, &m_AtlasID);
        if (m_VAO)     glDeleteVertexArrays(1, &m_VAO);
        if (m_VBO)     glDeleteBuffers(1, &m_VBO);
    }

    bool TextRenderer::Init(const std::string& fontPath,
        const std::string& vertPath,
        const std::string& fragPath,
        unsigned int fontSize)
    {
        m_FontSize = fontSize;
        if (!LoadFont(fontPath, fontSize)) return false;

        m_Shader = ShaderLoader::Create(vertPath, fragPath);
        if (!m_Shader)
        {
            FROST_ERROR("TextRenderer failed to load shaders");
            return false;
        }

        
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER,
            sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
            4 * sizeof(float), (void*)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
            4 * sizeof(float), (void*)(2 * sizeof(float)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        m_Initialized = true;
        FROST_LOG("TextRenderer initialized: %s (%upx)",
            fontPath.c_str(), fontSize);
        return true;
    }

    bool TextRenderer::LoadFont(const std::string& fontPath,
        unsigned int       fontSize)
    {
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            FROST_ERROR("Failed to init FreeType");
            return false;
        }

        FT_Face face;
        if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
        {
            FROST_ERROR("Failed to load font: %s", fontPath.c_str());
            FT_Done_FreeType(ft);
            return false;
        }

        FT_Set_Pixel_Sizes(face, 0, fontSize);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        
        m_AtlasWidth = 0;
        m_AtlasHeight = 0;

        for (unsigned char c = 32; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;
            m_AtlasWidth += face->glyph->bitmap.width + 2;
            m_AtlasHeight = std::max(m_AtlasHeight,
                (int)face->glyph->bitmap.rows);
        }

        glGenTextures(1, &m_AtlasID);
        glBindTexture(GL_TEXTURE_2D, m_AtlasID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
            m_AtlasWidth, m_AtlasHeight,
            0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        
        int xOffset = 0;
        for (unsigned char c = 32; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;

            auto& bmp = face->glyph->bitmap;

            glTexSubImage2D(GL_TEXTURE_2D, 0,
                xOffset, 0,
                bmp.width, bmp.rows,
                GL_RED, GL_UNSIGNED_BYTE,
                bmp.buffer);

            Glyph glyph;
            glyph.size = { (float)bmp.width,              (float)bmp.rows };
            glyph.bearing = { (float)face->glyph->bitmap_left, (float)face->glyph->bitmap_top };
            glyph.advance = (float)(face->glyph->advance.x >> 6);
            glyph.uvMin = { (float)xOffset / m_AtlasWidth,  0.0f };
            glyph.uvMax = { (float)(xOffset + bmp.width) / m_AtlasWidth,
                              (float)bmp.rows / m_AtlasHeight };

            m_Glyphs[c] = glyph;
            xOffset += bmp.width + 2;
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        return true;
    }

    void TextRenderer::RenderText(const std::string& text,
        float              x,
        float              y,
        float              scale,
        const vec4& color,
        const mat4& projection)
    {
        if (!m_Initialized) return;

        m_Shader->Bind();
        m_Shader->SetMat4("uProjection", projection);

        glUniform4f(glGetUniformLocation(m_Shader->ID(), "textColor"),
            color.r, color.g, color.b, color.a);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_AtlasID);
        m_Shader->SetInt("text", 0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindVertexArray(m_VAO);

        float cursorX = x;

        for (char c : text)
        {
            if (c < 32 || c >= 128) continue;

            const Glyph& g = m_Glyphs.at(c);

            float xpos = cursorX + g.bearing.x * scale;
            float ypos = y + (m_FontSize - g.bearing.y) * scale;
            float w = g.size.x * scale;
            float h = g.size.y * scale;

            float verts[6][4] = {
                { xpos,     ypos + h, g.uvMin.x, g.uvMax.y },
                { xpos,     ypos,     g.uvMin.x, g.uvMin.y },
                { xpos + w, ypos,     g.uvMax.x, g.uvMin.y },
                { xpos,     ypos + h, g.uvMin.x, g.uvMax.y },
                { xpos + w, ypos,     g.uvMax.x, g.uvMin.y },
                { xpos + w, ypos + h, g.uvMax.x, g.uvMax.y },
            };

            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            cursorX += g.advance * scale;
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}