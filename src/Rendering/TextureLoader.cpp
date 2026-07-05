#include "TextureLoader.h"
#include <glad/glad.h>
#include <iostream>
#include "Core/Logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


namespace Frost
{
    std::shared_ptr<Texture> TextureLoader::Create(
        const std::string& path,
        Texture::Filter    filter,
        bool               flipUV)
    {
        stbi_set_flip_vertically_on_load(flipUV ? 1 : 0);

        int width, height, channels;
        unsigned char* data = stbi_load(
            path.c_str(), &width, &height, &channels, 0);

        if (!data)
        {
            FROST_ERROR("Failed to load texture: %s", path.c_str());
            return nullptr;
        }

        GLenum format = GL_RGBA;
        switch (channels)
        {
        case 1: format = GL_RED;  break;
        case 3: format = GL_RGB;  break;
        case 4: format = GL_RGBA; break;
        }

        unsigned int id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        int glFilter = (filter == Texture::Filter::Linear)
            ? GL_LINEAR : GL_NEAREST;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter);

        glTexImage2D(GL_TEXTURE_2D, 0, format,
            width, height, 0,
            format, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);

        FROST_LOG("Loaded texture: %s (%dx%d %dch)",
            path.c_str(), width, height, channels);

        return std::make_shared<Texture>(id, width, height, channels, path);
    }

    std::shared_ptr<Texture> TextureLoader::CreateWhite()
    {
        unsigned int id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        unsigned char white[] = { 255, 255, 255, 255 };
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
            1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        FROST_LOG("Created white texture");
        return std::make_shared<Texture>(id, 1, 1, 4, "white");
    }
}