#pragma once
#include "Texture.h"
#include <memory>
#include <string>

namespace Frost
{
    class TextureLoader
    {
    public:
        TextureLoader() = delete;

        static std::shared_ptr<Texture> Create(
            const std::string& path,
            Texture::Filter    filter = Texture::Filter::Linear,
            bool               flipUV = true);

        // Create a 1x1 white texture — used for colored quads/particles
        static std::shared_ptr<Texture> CreateWhite();
    };
}