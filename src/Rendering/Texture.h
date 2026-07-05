#pragma once
#include <glad/glad.h>
#include <string>

namespace Frost
{
    class Texture
    {
    public:
        enum class Filter { Nearest, Linear };

        Texture(unsigned int id, int width, int height,
            int channels, const std::string& path = "")
            : m_ID(id)
            , m_Width(width)
            , m_Height(height)
            , m_Channels(channels)
            , m_Path(path) {
        }

        ~Texture()
        {
            if (m_ID) glDeleteTextures(1, &m_ID);
        }

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        void Bind(unsigned int slot = 0) const
        {
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, m_ID);
        }

        void Unbind() const
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        unsigned int ID()       const { return m_ID; }
        int          Width()    const { return m_Width; }
        int          Height()   const { return m_Height; }
        int          Channels() const { return m_Channels; }
        std::string  Path()     const { return m_Path; }

    private:
        unsigned int m_ID = 0;
        int          m_Width = 0;
        int          m_Height = 0;
        int          m_Channels = 0;
        std::string  m_Path;
    };
}